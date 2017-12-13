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
     Cumulator.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     January-March 2011
*/

#include "BooleanNetwork.h"
#include "Cumulator.h"
#include "RunConfig.h"
#include <sstream>
#include <iomanip>
#include <math.h>
#include <float.h>

double distance(const STATE_MAP<NetworkState_Impl, double>& proba_dist1, const STATE_MAP<NetworkState_Impl, double>& proba_dist2)
{
  return 0.;
}

void Cumulator::check() const
{
  // check that for each tick (except the last one), the sigma of each map == 1.
  for (int nn = 0; nn < max_tick_index; ++nn) {
    const CumulMap& mp = get_map(nn);
    CumulMap::Iterator iter = mp.iterator();
    double sum = 0.;
    while (iter.hasNext()) {
      TickValue tick_value;
      iter.next(tick_value);
      sum += tick_value.tm_slice;
    }
    sum /= time_tick*sample_count;
    assert(sum >= 1. - 0.0001 && sum <= 1. + 0.0001);
  }
}

void Cumulator::trajectoryEpilogue()
{
  assert(sample_num < sample_count);

  ProbaDist::Iterator curtraj_proba_dist_iter = curtraj_proba_dist.iterator();

  double proba_max_time = 0.;

  while (curtraj_proba_dist_iter.hasNext()) {
    double tm_slice;
    curtraj_proba_dist_iter.next(tm_slice);
    proba_max_time += tm_slice;
  }

  //std::cout << "Trajepilogue #" << (sample_num+1) << " " << proba_max_time << '\n';
  double proba = 0;
  curtraj_proba_dist_iter.rewind();

  ProbaDist& proba_dist = proba_dist_v[sample_num++];
  while (curtraj_proba_dist_iter.hasNext()) {
    NetworkState_Impl state;
    double tm_slice;
    curtraj_proba_dist_iter.next(state, tm_slice);
    //assert(proba_dist.find(state) == proba_dist.end());
    double new_tm_slice = tm_slice / proba_max_time;
    proba_dist.set(state, new_tm_slice);
    proba += new_tm_slice;
  }

  assert(proba >= 0.9999 && proba <= 1.0001);
}

void Cumulator::computeMaxTickIndex()
{
  /*
  unsigned int tmp_tick_index = tick_index + !tick_completed;
  if (max_tick_index > tmp_tick_index) {
    max_tick_index = tmp_tick_index;
  }
  */
  if (max_tick_index > tick_index) {
    max_tick_index = tick_index;
  }
}

void Cumulator::epilogue(Network* network, const NetworkState& reference_state)
{
  computeMaxTickIndex();

  //check();

  // compute H (Entropy), TH (Transition entropy) and HD (Hamming distance)
  H_v.resize(max_tick_index);
  TH_v.resize(max_tick_index);
#ifndef HD_BUG
  HD_v.resize(max_tick_index);
#endif

  maxcols = 0;
  double ratio = time_tick * sample_count;
  for (int nn = 0; nn < max_tick_index; ++nn) { // time tick
    const CumulMap& mp = get_map(nn);
    CumulMap::Iterator iter = mp.iterator();
    H_v[nn] = 0.;
    TH_v[nn] = 0.;
#ifndef HD_BUG
    MAP<unsigned int, double>& hd_m = HD_v[nn];
#endif
    while (iter.hasNext()) {
      NetworkState_Impl state;
      TickValue tick_value;
      iter.next(state, tick_value);
      double tm_slice = tick_value.tm_slice;
      double proba = tm_slice / ratio;      
      double TH = tick_value.TH / sample_count;
      H_v[nn] += -log2(proba) * proba;
#ifndef HD_BUG
      int hd = reference_state.hamming(network, state);
      if (hd_m.find(hd) == hd_m.end()) {
	hd_m[hd] = proba;
      } else {
	hd_m[hd] += proba;
      }
#endif
      TH_v[nn] += TH;
    }
    TH_v[nn] /= time_tick;
    if (mp.size() > maxcols) {
      maxcols = mp.size();
    }
  }

#ifdef HD_BUG
  HD_v.resize(max_tick_index);

  for (int nn = 0; nn < max_tick_index; ++nn) { // time tick
    const HDCumulMap& hd_mp = get_hd_map(nn);
    HDCumulMap::Iterator iter = hd_mp.iterator();
    MAP<unsigned int, double>& hd_m = HD_v[nn];
    while (iter.hasNext()) {
      NetworkState_Impl state;
      double tm_slice;
      iter.next(state, tm_slice);
      double proba = tm_slice / ratio;      
      int hd = reference_state.hamming(network, state);
      if (hd_m.find(hd) == hd_m.end()) {
	hd_m[hd] = proba;
      } else {
	hd_m[hd] += proba;
      }
    }
  }
#endif
}

void Cumulator::displayCSV(Network* network, unsigned int refnode_count, std::ostream& os_probtraj, std::ostream& os_statdist) const
{
  std::vector<Node*>::const_iterator begin_network;

  os_probtraj << "Time\tTH" << (COMPUTE_ERRORS ? "\tErrorTH" : "") << "\tH";

  for (unsigned int jj = 0; jj <= refnode_count; ++jj) {
    os_probtraj << "\tHD=" << jj;
  }

  for (unsigned int nn = 0; nn < maxcols; ++nn) {
    os_probtraj << "\tState\tProba" << (COMPUTE_ERRORS ? "\tErrorProba" : "");
  }

  os_probtraj << '\n';

  os_probtraj << std::fixed;
  os_probtraj << std::setprecision(6);
  double time_tick2 = time_tick * time_tick;
  double ratio = time_tick*sample_count;
  for (int nn = 0; nn < max_tick_index; ++nn) {
    os_probtraj << std::setprecision(4) << std::fixed << (nn*time_tick);
    // TH
    const CumulMap& mp = get_map(nn);
    CumulMap::Iterator iter = mp.iterator();
    os_probtraj << '\t' << TH_v[nn];

    // ErrorTH
    //assert((size_t)nn < TH_square_v.size());
    if (COMPUTE_ERRORS) {
      double TH_square = TH_square_v[nn];
      double TH = TH_v[nn]; // == TH
      double variance_TH = (TH_square / ((sample_count-1) * time_tick2)) - (TH*TH*sample_count/(sample_count-1));
      double err_TH;
      double variance_TH_sample_count = variance_TH/sample_count;
      //assert(variance_TH > 0.0);
      if (variance_TH_sample_count >= 0.0) {
	err_TH = sqrt(variance_TH_sample_count);
      } else {
	err_TH = 0.;
      }
      os_probtraj << '\t' << err_TH;
    }

    // H
    os_probtraj << '\t' << H_v[nn];

    // HD
    const MAP<unsigned int, double>& hd_m = HD_v[nn];
    for (unsigned int hd = 0; hd <= refnode_count; ++hd) { 
      os_probtraj << '\t';
      MAP<unsigned int, double>::const_iterator hd_m_iter = hd_m.find(hd);
      if (hd_m_iter != hd_m.end()) {
	os_probtraj << (*hd_m_iter).second;
      } else {
	os_probtraj << "0.0";
      }
    }

    // Proba, ErrorProba
    while (iter.hasNext()) {
      NetworkState_Impl state;
      TickValue tick_value;
      iter.next(state, tick_value);
      double proba = tick_value.tm_slice / ratio;      
      //double TH = tick_value.TH / sample_count;
      os_probtraj << '\t';
      NetworkState network_state(state);
      network_state.displayOneLine(os_probtraj, network);
      os_probtraj << '\t' << std::setprecision(6) << proba;
      if (COMPUTE_ERRORS) {
	double tm_slice_square = tick_value.tm_slice_square;
	double variance_proba = (tm_slice_square / ((sample_count-1) * time_tick2)) - (proba*proba*sample_count/(sample_count-1));
	double err_proba;
	double variance_proba_sample_count = variance_proba/sample_count;
	if (variance_proba_sample_count >= DBL_MIN) {
	  err_proba = sqrt(variance_proba_sample_count);
	} else {
	  err_proba = 0.;
	}
	os_probtraj << '\t' << err_proba;
      }
    }
    os_probtraj << '\n';
  }

  // should not be in cumulator, but somehwere in ProbaDist*

  // Probability distribution
  unsigned int statdist_traj_count = RunConfig::getInstance()->getStatDistTrajCount();
  if (statdist_traj_count == 0) {
    return;
  }

  unsigned int max_size = 0;
  unsigned int cnt = 0;
  unsigned int proba_dist_size = proba_dist_v.size();
  for (unsigned int nn = 0; nn < proba_dist_size; ++nn) {
    const ProbaDist& proba_dist = proba_dist_v[nn];
    if (proba_dist.size() > max_size) {
      max_size = proba_dist.size();
    }
    cnt++;
    if (cnt > statdist_traj_count) {
      break;
    }
  }

  //unsigned int proba_dist_traj_count = cnt;

  os_statdist << "Trajectory";
  for (unsigned int nn = 0; nn < max_size; ++nn) {
    os_statdist << "\tState\tProba";
  }

  os_statdist << '\n';
  cnt = 0;
  for (unsigned int nn = 0; nn < proba_dist_size; ++nn) {
    const ProbaDist& proba_dist = proba_dist_v[nn];
    os_statdist << "#" << (cnt+1);
    cnt++;

    proba_dist.display(os_statdist, network);
    if (cnt >= statdist_traj_count) {
      break;
    }
  }

#if 0
  // compute similarity between all proba_dist (< statdist_traj_count), just for testing
  for (unsigned int nn1 = 0; nn1 < statdist_traj_count; ++nn1) {
    const ProbaDist& proba_dist1 = proba_dist_v[nn1];
    for (unsigned int nn2 = nn1+1; nn2 < statdist_traj_count; ++nn2) {
      const ProbaDist& proba_dist2 = proba_dist_v[nn2];
      similarity(network, proba_dist1, proba_dist2);
    }
  }
#endif

  // should not be called from here, but from MaBestEngine
  ProbaDistClusterFactory* clusterFactory = new ProbaDistClusterFactory(proba_dist_v, statdist_traj_count);
  clusterFactory->makeClusters(RunConfig::getInstance()->getStatdistClusterThreshold());
  clusterFactory->display(network, os_statdist);
  clusterFactory->computeStationaryDistribution();
  clusterFactory->displayStationaryDistribution(network, os_statdist);
}

void Cumulator::add(unsigned int where, const CumulMap& add_cumul_map)
{
  CumulMap& to_cumul_map = get_map(where);

  CumulMap::Iterator iter = add_cumul_map.iterator();
  while (iter.hasNext()) {
    NetworkState_Impl state;
    TickValue tick_value;
    iter.next(state, tick_value);
    to_cumul_map.add(state, tick_value);
  }
}

#ifdef HD_BUG
void Cumulator::add(unsigned int where, const HDCumulMap& add_hd_cumul_map)
{
  HDCumulMap& to_hd_cumul_map = get_hd_map(where);

  HDCumulMap::Iterator iter = add_hd_cumul_map.iterator();
  while (iter.hasNext()) {
    NetworkState_Impl state;
    double tm_slice;
    iter.next(state, tm_slice);
    to_hd_cumul_map.add(state, tm_slice);
  }
}
#endif

Cumulator* Cumulator::mergeCumulators(std::vector<Cumulator*>& cumulator_v)
{
  size_t size = cumulator_v.size();
  if (1 == size) {
    Cumulator* cumulator = cumulator_v[0];
    return new Cumulator(*cumulator);
  }

  RunConfig* runconfig = RunConfig::getInstance();
  Cumulator* ret_cumul = new Cumulator(runconfig->getTimeTick(), runconfig->getMaxTime(), runconfig->getSampleCount());
  size_t min_cumul_size = ~0ULL;
  size_t min_tick_index_size = ~0ULL;
  std::vector<Cumulator*>::iterator begin = cumulator_v.begin();
  std::vector<Cumulator*>::iterator end = cumulator_v.end();
  while (begin != end) {
    Cumulator* cumulator = *begin;
    cumulator->computeMaxTickIndex();
    if (cumulator->cumul_map_v.size() < min_cumul_size) {
      min_cumul_size = cumulator->cumul_map_v.size();
    }
    if ((size_t)cumulator->max_tick_index < min_tick_index_size) {
      min_tick_index_size = cumulator->max_tick_index;
    }
    ++begin;
  }

  ret_cumul->cumul_map_v.resize(min_cumul_size);
#ifdef HD_BUG
  ret_cumul->hd_cumul_map_v.resize(min_cumul_size);
#endif
  ret_cumul->max_tick_index = ret_cumul->tick_index = min_tick_index_size;

  begin = cumulator_v.begin();
  unsigned int rr = 0;
  for (unsigned int jj = 0; begin != end; ++jj) {
    Cumulator* cumulator = *begin;
    for (unsigned int nn = 0; nn < min_cumul_size; ++nn) {
      ret_cumul->add(nn, cumulator->cumul_map_v[nn]);
#ifdef HD_BUG
      ret_cumul->add(nn, cumulator->hd_cumul_map_v[nn]);
#endif
      ret_cumul->TH_square_v[nn] += cumulator->TH_square_v[nn];
    }
    unsigned int proba_dist_size = cumulator->proba_dist_v.size();
    for (unsigned int ii = 0; ii < proba_dist_size; ++ii) {
      assert(ret_cumul->proba_dist_v.size() > rr);
      ret_cumul->proba_dist_v[rr++] = cumulator->proba_dist_v[ii];
    }
    ++begin;
  }
  return ret_cumul;
}
