/* 
   MaBoSS (Markov Boolean Stochastic Simulator)
   Copyright (C) 2011 Institut Curie, 26 rue d'Ulm, Paris, France
   
   MaBoSS is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   MaBoSS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA 
*/

/*
   Module:
     ProbaDist.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     January-March 2011
*/

class Network;

#include "ProbaDist.h"
#include "BooleanNetwork.h"
#include "RunConfig.h"
#include <iomanip>
#include <math.h>
#include <float.h>

/*
static double abs(double d)
{
  return d > 0. ? d : -d;
}
*/

void ProbaDistCluster::add(unsigned int index, const ProbaDist& proba_dist)
{
  proba_dist_map[index] = proba_dist;
  factory->setClusterized(index);
}

void ProbaDistCluster::complete(double threshold, unsigned int statdist_traj_count)
{
  for (;;) {
    unsigned int added_proba_dist_cnt = 0;
    std::vector<unsigned int> toadd_map;
    MAP<unsigned int, ProbaDist>::iterator begin = proba_dist_map.begin();
    MAP<unsigned int, ProbaDist>::iterator end = proba_dist_map.end();
    while (begin != end) {
      unsigned int nn1 = (*begin).first;
      const ProbaDist& proba_dist1 = (*begin).second;
#ifdef CLUSTER_OPTIM
      MAP<unsigned int, bool>::const_iterator not_clusterized_iter = factory->getNotClusterizedMap().begin();
      MAP<unsigned int, bool>::const_iterator not_clusterized_end = factory->getNotClusterizedMap().end();
      while (not_clusterized_iter != not_clusterized_end) {
	unsigned int nn2 = (*not_clusterized_iter).first;
	const ProbaDist& proba_dist2 = factory->getProbaDist(nn2);
	double simil = similarity(nn1, proba_dist1, nn2, proba_dist2, factory->getSimilarityCache());
	if (simil >= threshold) {
	  toadd_map.push_back(nn2);
	  added_proba_dist_cnt++;
	}
	++not_clusterized_iter;
      }
#else
      for (unsigned int nn2 = 0; nn2 < statdist_traj_count; ++nn2) { // optimizatin: should avoid to scan all proba_dist, using a complement map
	if (!factory->isClusterized(nn2)) {
	  const ProbaDist& proba_dist2 = factory->getProbaDist(nn2);
	  double simil = similarity(nn1, proba_dist1, nn2, proba_dist2, factory->getSimilarityCache());
	  if (simil >= threshold) {
	    add(nn2, proba_dist2);
	    added_proba_dist_cnt++;
	  }
	}
      }
#endif
      ++begin;
    }
#ifdef CLUSTER_OPTIM
    std::vector<unsigned int>::iterator b = toadd_map.begin();
    std::vector<unsigned int>::iterator e = toadd_map.end();
    while (b != e) {
      unsigned int nn2 = *b;
      add(nn2, factory->getProbaDist(nn2));
      ++b;
    }
#endif
    if (!added_proba_dist_cnt) {
      break;
    }
  }
}

void ProbaDistClusterFactory::makeClusters(double threshold)
{
  if (statdist_traj_count <= RunConfig::getInstance()->getStatDistSimilarityCacheMaxSize()) {
    cacheSimilarities();
  }
  for (unsigned int nn1 = 0; nn1 < statdist_traj_count; ++nn1) { // optimization: should avoid to scan all proba_dist, using a complement map
    if (!isClusterized(nn1)) {
      ProbaDistCluster* cluster = newCluster();
      cluster->add(nn1, getProbaDist(nn1));
      cluster->complete(threshold, statdist_traj_count);
    }
  }
}

void ProbaDistClusterFactory::cacheSimilarities()
{
  similarity_cache = new double*[statdist_traj_count];
  for (unsigned int nn1 = 0; nn1 < statdist_traj_count; ++nn1) {
    similarity_cache[nn1] = new double[statdist_traj_count];
  }

  for (unsigned int nn1 = 0; nn1 < statdist_traj_count; ++nn1) {
    for (unsigned int nn2 = nn1; nn2 < statdist_traj_count; ++nn2) {
      similarity_cache[nn1][nn2] = ProbaDistCluster::similarity(nn1, getProbaDist(nn1), nn2, getProbaDist(nn2), NULL);
    }
  }
}

void ProbaDistCluster::display(Network* network, std::ostream& os) const
{
  MAP<unsigned int, ProbaDist>::const_iterator begin = proba_dist_map.begin();
  MAP<unsigned int, ProbaDist>::const_iterator end = proba_dist_map.end();

  while (begin != end) {
    unsigned int nn = (*begin).first;
    const ProbaDist& proba_dist = (*begin).second;
    os << "#" << (nn+1);
    proba_dist.display(os, network);
    ++begin;
  }
}

void ProbaDistClusterFactory::display(Network* network, std::ostream& os) const
{
  unsigned int size = proba_dist_cluster_v.size();
  for (unsigned int nn = 0; nn < size; ++nn) {
    ProbaDistCluster* cluster = proba_dist_cluster_v[nn];
    os << "\nTrajectory[cluster=#" << (nn+1) << ",size=" << cluster->size() << "]\tState\tProba\tState\tProba\tState\tProba\tState\tProba ...\n";
    cluster->display(network, os);
  }
}

void ProbaDistClusterFactory::displayStationaryDistribution(Network* network, std::ostream& os) const
{
  unsigned int size = proba_dist_cluster_v.size();
  os << "\nCluster\tState\tProba\tErrorProba\tState\tProba\tErrorProba\tState\tProba\tErrorProba\tState\tProba\tErrorProba...\n";
  for (unsigned int nn = 0; nn < size; ++nn) {
    ProbaDistCluster* cluster = proba_dist_cluster_v[nn];
    os << "#" << (nn+1);
    cluster->displayStationaryDistribution(network, os);
    os << '\n';
  }
}

void ProbaDistCluster::computeStationaryDistribution()
{
  MAP<unsigned int, ProbaDist>::iterator begin = proba_dist_map.begin();
  MAP<unsigned int, ProbaDist>::iterator end = proba_dist_map.end();
  while (begin != end) {
    const ProbaDist& proba_dist = (*begin).second;
    ProbaDist::Iterator iter(proba_dist);
    while (iter.hasNext()) {
      NetworkState_Impl state;
      double proba;
      iter.next(state, proba);
      if (stat_dist_map.find(state) == stat_dist_map.end()) {
	stat_dist_map[state] = Proba(proba, proba*proba);
      } else {
	stat_dist_map[state].proba += proba;
	stat_dist_map[state].probaSquare += proba*proba;
      }
    }
    ++begin;
  }
}

void ProbaDistCluster::displayStationaryDistribution(Network* network, std::ostream& os) const
{
  STATE_MAP<NetworkState_Impl, Proba>::const_iterator stat_dist_iter = stat_dist_map.begin();
  STATE_MAP<NetworkState_Impl, Proba>::const_iterator stat_dist_end = stat_dist_map.end();

  unsigned int sz = size();
  const double minsquaredouble = DBL_MIN*DBL_MIN;
  while (stat_dist_iter != stat_dist_end) {
    NetworkState_Impl state = (*stat_dist_iter).first;
    const Proba& pb = (*stat_dist_iter).second;
    NetworkState network_state(state);
    os << '\t';
    network_state.displayOneLine(os, network);
    double proba = pb.proba/sz;
    double probaSquare = pb.probaSquare/sz;
    double vr = (probaSquare-proba*proba)/(sz-1); // EV 2014-10-07: in case of sz == 1, vr is nan
    //    os << '\t' << proba << " (" <<  (probaSquare-proba*proba) << ")" << '\t';
    os << '\t' << proba << '\t';
    double variance;
    if (vr < minsquaredouble || sz <= 1) { // EV 2014-10-07: sz <= 1 to avoid nan values
      variance = 0.0;
    } else {
      variance = sqrt(vr);
    }
    os << variance;
    ++stat_dist_iter;
  }
}

void ProbaDistClusterFactory::computeStationaryDistribution()
{
  unsigned int size = proba_dist_cluster_v.size();
  for (unsigned int nn = 0; nn < size; ++nn) {
    ProbaDistCluster* cluster = proba_dist_cluster_v[nn];
    cluster->computeStationaryDistribution();
  }
}

double ProbaDistCluster::similarity(unsigned int nn1, const ProbaDist& proba_dist1, unsigned int nn2, const ProbaDist& proba_dist2, double** similarity_cache)
{
  if (NULL != similarity_cache) {
    return nn2 > nn1 ? similarity_cache[nn1][nn2] : similarity_cache[nn2][nn1];
  }

  ProbaDist::Iterator proba_dist1_iter = proba_dist1.iterator();
  double simil1 = 0.0;
  double simil2 = 0.0;
  unsigned int out_of_support = 0;
  while (proba_dist1_iter.hasNext()) {
    NetworkState_Impl state;
    double proba1, proba2;
    proba_dist1_iter.next(state, proba1);
    if (proba_dist2.hasState(state, proba2)) {
      simil1 += proba1;
      simil2 += proba2;
    } else {
      out_of_support++;
    }
  }

#if 0
  std::cout << "Similarity between [" << out_of_support << "]:\n";
  std::cout << "   ";
  displayProbaDist(std::cout, network, proba_dist1);
  std::cout << "AND\n";
  std::cout << "   ";
  displayProbaDist(std::cout, network, proba_dist2);
  std::cout << simil1 << " " << simil2 << " " << (simil1*simil2) << "\n\n";
#endif
  return simil1 * simil2;
}

void ProbaDist::display(std::ostream& os, Network* network) const
{
  ProbaDist::Iterator proba_dist_iter = iterator();
  os << std::setprecision(10);
  while (proba_dist_iter.hasNext()) {
    NetworkState_Impl state;
    double proba;
    proba_dist_iter.next(state, proba);
    NetworkState network_state(state);
    os << '\t';
    network_state.displayOneLine(os, network);
    os << '\t' << proba;
  }
  os << '\n';
}
