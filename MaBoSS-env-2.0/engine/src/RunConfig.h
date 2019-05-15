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
     RunConfig.h

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     January-March 2011
*/

#ifndef _RUNCONFIG_H_
#define _RUNCONFIG_H_

#include "RandomGenerator.h"
#include "BooleanNetwork.h"
class MaBEstEngine;

class RunConfig {

  static RunConfig* instance;
  mutable RandomGeneratorFactory* randgen_factory;
  double time_tick;
  double max_time;
  unsigned int sample_count;
  bool discrete_time;
  bool use_physrandgen;
  int seed_pseudorand;
  bool display_traj;
  unsigned int thread_count;
  unsigned int statdist_traj_count;
  double statdist_cluster_threshold;
  unsigned int statdist_similarity_cache_max_size;
  void dump_perform(Network* network, std::ostream& os, bool is_template) const;

  RunConfig();

 public:
  static RunConfig* getInstance() {
    if (NULL == instance) {
      instance = new RunConfig();
    }
    return instance;
  }

  int parse(Network* network, const char* file = NULL);
  int parseExpression(Network* network, const char* expr);
  void setParameter(const std::string& param, double value);

  RandomGeneratorFactory* getRandomGeneratorFactory() const;
  double getTimeTick() const {return time_tick;}
  double getMaxTime() const {return max_time;}
  unsigned int getSampleCount() const {return sample_count;}
  bool isDiscreteTime() const {return discrete_time;}
  int getSeedPseudoRandom() const {return seed_pseudorand;}
  void setSeedPseudoRandom(int seed) { seed_pseudorand = seed;}
  void display(Network* network, time_t start_time, time_t end_time, MaBEstEngine& mabest, std::ostream& os) const;
  bool displayTrajectories() const {return display_traj;}
  unsigned int getThreadCount() const {return thread_count;}
  unsigned int getStatDistTrajCount() const {return statdist_traj_count <= sample_count ? statdist_traj_count : sample_count;}
  double getStatdistClusterThreshold() const {return statdist_cluster_threshold;}
  unsigned int getStatDistSimilarityCacheMaxSize() const {return statdist_similarity_cache_max_size;}

  void generateTemplate(Network* network, std::ostream& os) const;
  void dump(Network* network, std::ostream& os) const;
};

extern FILE* RCin;
extern int RCparse();
extern void runconfig_setNetwork(Network* network);
extern void RC_set_file(const char* file);
extern void RC_set_expr(const char* expr);

#endif
