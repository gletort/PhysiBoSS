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
    ProbaDist.h

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     January-March 2011
*/

#ifndef _PROBADIST_H_
#define _PROBADIST_H_

#include <assert.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "BooleanNetwork.h"

#define CLUSTER_OPTIM

class ProbaDist {
  STATE_MAP<NetworkState_Impl, double> mp;

 public:
  size_t size() const {
    return mp.size();
  }

  void incr(NetworkState_Impl state, double tm_slice) {
    STATE_MAP<NetworkState_Impl, double>::iterator proba_iter = mp.find(state);
    if (proba_iter == mp.end()) {
      mp[state] = tm_slice;
    } else {
      (*proba_iter).second += tm_slice;
    }
  }

  void clear() {
    mp.clear();
  }

  void set(NetworkState_Impl state, double tm_slice) {
    mp[state] = tm_slice;
  }

  bool hasState(NetworkState_Impl state, double& tm_slice) const {
    STATE_MAP<NetworkState_Impl, double>::const_iterator iter = mp.find(state);
    if (iter != mp.end()) {
      tm_slice = (*iter).second;
      return true;
    }
    return false;
  }

  class Iterator {
    
    const ProbaDist& proba_dist_map;
    STATE_MAP<NetworkState_Impl, double>::const_iterator iter, end;

  public:
  Iterator(const ProbaDist& proba_dist_map) : proba_dist_map(proba_dist_map) {
      iter = proba_dist_map.mp.begin();
      end = proba_dist_map.mp.end();
    }
	
    void rewind() {
      iter = proba_dist_map.mp.begin();
    }

    bool hasNext() const {
      return iter != end;
    }

    void next(NetworkState_Impl& state, double& tm_slice) {
      state = (*iter).first;
      tm_slice = (*iter).second;
      ++iter;
    }

    void next(NetworkState_Impl& state) {
      state = (*iter).first;
      ++iter;
    }

    void next(double& tm_slice) {
      tm_slice = (*iter).second;
      ++iter;
    }
  };	

  void display(std::ostream& os, Network* network) const;

  Iterator iterator() {return Iterator(*this);}
  Iterator iterator() const {return Iterator(*this);}
};

class ProbaDistClusterFactory;

class ProbaDistCluster {

  MAP<unsigned int, ProbaDist> proba_dist_map;
  ProbaDistClusterFactory* factory;
  struct Proba {
    double proba;
    double probaSquare;
    Proba() : proba(0.), probaSquare(0.) { }
    Proba(double proba, double probaSquare) : proba(proba), probaSquare(probaSquare) { }
  };
  // state -> Proba
  STATE_MAP<NetworkState_Impl, Proba> stat_dist_map;

 public:
  ProbaDistCluster(ProbaDistClusterFactory* factory) : factory(factory) { }

  void add(unsigned int index, const ProbaDist& proba_dist);
  static double similarity(unsigned int nn1, const ProbaDist& proba_dist1, unsigned int nn2, const ProbaDist& proba_dist2, double** similarity_cache);

  size_t size() const {return proba_dist_map.size();}

  void complete(double threshold, unsigned int statdist_traj_count);
  void computeStationaryDistribution();

  void display(Network* network, std::ostream& os) const;
  void displayStationaryDistribution(Network* network, std::ostream& os) const;
};

class ProbaDistClusterFactory {

  std::vector<ProbaDistCluster*> proba_dist_cluster_v;
  MAP<unsigned int, bool> proba_dist_clusterized;
#ifdef CLUSTER_OPTIM
  MAP<unsigned int, bool> proba_dist_not_clusterized;
#endif
  const std::vector<ProbaDist> proba_dist_v;
  unsigned int statdist_traj_count;
  double** similarity_cache;

 public:
  ProbaDistClusterFactory(const std::vector<ProbaDist>& proba_dist_v, unsigned int statdist_traj_count) : proba_dist_v(proba_dist_v), statdist_traj_count(statdist_traj_count), similarity_cache(NULL) {
#ifdef CLUSTER_OPTIM
    for (unsigned int nn = 0; nn < statdist_traj_count; ++nn) {
      proba_dist_not_clusterized[nn] = true;
    }
    //std::cout << "SIZE: " << proba_dist_not_clusterized.size() << '\n';
#endif
  }

  ProbaDistCluster* newCluster() {
    ProbaDistCluster* proba_dist_cluster = new ProbaDistCluster(this);
    proba_dist_cluster_v.push_back(proba_dist_cluster);
    return proba_dist_cluster;
  }

  void setClusterized(unsigned int index) {
    proba_dist_clusterized[index] = true;
#ifdef CLUSTER_OPTIM
    MAP<unsigned int, bool>::iterator iter = proba_dist_not_clusterized.find(index);
    if (iter != proba_dist_not_clusterized.end()) {
      proba_dist_not_clusterized.erase(iter);
    }
#endif
  }

  double** getSimilarityCache() {
    return similarity_cache;
  }

#ifdef CLUSTER_OPTIM
  const MAP<unsigned int, bool>& getNotClusterizedMap() const {
    return proba_dist_not_clusterized;
  }
#endif

  bool isClusterized(unsigned int index) const {
    return proba_dist_clusterized.find(index) != proba_dist_clusterized.end();
  }

  const ProbaDist& getProbaDist(unsigned int index) const {
    //assert(index < proba_dist_v.size());
    return proba_dist_v[index];
  }

  void cacheSimilarities();

  void makeClusters(double threshold);
  void computeStationaryDistribution();
  void displayStationaryDistribution(Network* network, std::ostream& os) const;

  void display(Network* network, std::ostream& os) const;
};

#endif
