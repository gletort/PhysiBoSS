/* 
   MaBoSS (Markov Boolean Stochastic Simulator)
   Copyright (C) 2011-2018 Institut Curie, 26 rue d'Ulm, Paris, France
   
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
     MaBEstEngine.h

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     January-March 2011
*/

#ifndef _MABESTENGINE_H_
#define _MABESTENGINE_H_

#include <string>
#include <map>
#include <vector>
#include <assert.h>

#include "BooleanNetwork.h"
#include "Cumulator.h"
#include "RandomGenerator.h"
#include "RunConfig.h"

struct ArgWrapper;

class MaBEstEngine {

  Network* network;
  double time_tick;
  double max_time;
  unsigned int sample_count;
  bool discrete_time;
  unsigned int thread_count;
  //std::map<NetworkState, unsigned int> fixpoints;
  STATE_MAP<NetworkState_Impl, unsigned int> fixpoints;
  mutable long long elapsed_core_runtime, user_core_runtime, elapsed_statdist_runtime, user_statdist_runtime, elapsed_epilogue_runtime, user_epilogue_runtime;

  NetworkState reference_state;
  unsigned int refnode_count;
  Cumulator* merged_cumulator;
  std::vector<STATE_MAP<NetworkState_Impl, unsigned int>*> fixpoint_map_v;
  std::vector<Cumulator*> cumulator_v;
  std::vector<ArgWrapper*> arg_wrapper_v;
  pthread_t* tid;
  NodeIndex getTargetNode(RandomGenerator* random_generator, const MAP<NodeIndex, double>& nodeTransitionRates, double total_rate) const;
  double computeTH(const MAP<NodeIndex, double>& nodeTransitionRates, double total_rate) const;
  void epilogue();
  static void* threadWrapper(void *arg);
  void runThread(Cumulator* cumulator, unsigned int start_count_thread, unsigned int sample_count_thread, RandomGeneratorFactory* randgen_factory, int seed, STATE_MAP<NetworkState_Impl, unsigned int>* fixpoint_map, std::ostream* output_traj);
  STATE_MAP<NetworkState_Impl, unsigned int>* mergeFixpointMaps();

public:
  static const std::string VERSION;

  static void init();
  static void loadUserFuncs(const char* module);

  MaBEstEngine(Network* network, RunConfig* runconfig);

  void run(std::ostream* output_traj);

  //const std::map<NetworkState, unsigned int>& getFixpoints() const {return fixpoints;}
  const STATE_MAP<NetworkState_Impl, unsigned int>& getFixpoints() const {return fixpoints;}

  bool converges() const {return fixpoints.size() > 0;}

  void display(std::ostream& output_probtraj, std::ostream& output_statdist, std::ostream& output_fp, bool hexfloat = false) const;

  const std::map<double, STATE_MAP<NetworkState_Impl, double> > getStateDists() const;
  const STATE_MAP<NetworkState_Impl, double> getNthStateDist(int nn) const;
  const STATE_MAP<NetworkState_Impl, double> getAsymptoticStateDist() const;

  const std::map<double, std::map<Node *, double> > getNodesDists() const;
  const std::map<Node*, double> getNthNodesDist(int nn) const;
  const std::map<Node*, double> getAsymptoticNodesDist() const;

  const std::map<double, double> getNodeDists(Node * node) const;
  double getNthNodeDist(Node * node, int nn) const;
  double getAsymptoticNodeDist(Node * node) const;
  
  int getMaxTickIndex() const {return merged_cumulator->getMaxTickIndex();} 

  long long getElapsedCoreRunTime() const {return elapsed_core_runtime;}
  long long getUserCoreRunTime() const {return user_core_runtime;}

  long long getElapsedEpilogueRunTime() const {return elapsed_epilogue_runtime;}
  long long getUserEpilogueRunTime() const {return user_epilogue_runtime;}

  long long getElapsedStatDistRunTime() const {return elapsed_statdist_runtime;}
  long long getUserStatDistRunTime() const {return user_statdist_runtime;}

  ~MaBEstEngine();
};

#endif
