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
     MaBEstEngine.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     January-March 2011
*/

#include "MaBEstEngine.h"
#include "Probe.h"
#include "Utils.h"
#include <stdlib.h>
#include <math.h>
#include <iomanip>
#include <dlfcn.h>
#include <iostream>

const std::string MaBEstEngine::VERSION = "2.0";
size_t RandomGenerator::generated_number_count = 0;
static const char* MABOSS_USER_FUNC_INIT = "maboss_user_func_init";

void MaBEstEngine::init()
{
  extern void builtin_functions_init();
  builtin_functions_init();
}

void MaBEstEngine::loadUserFuncs(const char* module)
{
  init();

  void* dl = dlopen(module, RTLD_LAZY);
  if (NULL == dl) {
    std::cerr << dlerror() << "\n";
    exit(1);
  }

  void* sym = dlsym(dl, MABOSS_USER_FUNC_INIT);
  if (sym == NULL) {
    std::cerr << "symbol " << MABOSS_USER_FUNC_INIT << "() not found in user func module: " << module << "\n";
    exit(1);
  }
  typedef void (*init_t)(std::map<std::string, Function*>*);
  init_t init_fun = (init_t)sym;
  init_fun(Function::getFuncMap());
}

MaBEstEngine::MaBEstEngine(Network* network, RunConfig* runconfig) :
  network(network), time_tick(runconfig->getTimeTick()), max_time(runconfig->getMaxTime()), sample_count(runconfig->getSampleCount()), discrete_time(runconfig->isDiscreteTime()), thread_count(runconfig->getThreadCount()) {

  tid = NULL;

  if (thread_count > sample_count) {
    thread_count = sample_count;
  }

  if (thread_count > 1 && !runconfig->getRandomGeneratorFactory()->isThreadSafe()) {
    std::cerr << "Warning: non reentrant random, may not work properly in multi-threaded mode\n";
  }
  network->updateRandomGenerator(runconfig);

  const std::vector<Node*>& nodes = network->getNodes();
  std::vector<Node*>::const_iterator begin = nodes.begin();
  std::vector<Node*>::const_iterator end = nodes.end();

  NetworkState internal_state;
  bool has_internal = false;
  refnode_count = 0;
  while (begin != end) {
    Node* node = *begin;
    if (node->isInternal()) {
      has_internal = true;
      internal_state.setNodeState(node, true);
    }
    if (node->isReference()) {
      reference_state.setNodeState(node, node->getReferenceState());
      refnode_count++;
    }
    ++begin;
  }

  merged_cumulator = NULL;
  cumulator_v.resize(thread_count);
  unsigned int count = sample_count / thread_count;
  unsigned int firstcount = count + sample_count - count * thread_count;
  for (unsigned int nn = 0; nn < thread_count; ++nn) {
    Cumulator* cumulator = new Cumulator(runconfig->getTimeTick(), runconfig->getMaxTime(), (nn == 0 ? firstcount : count));
    if (has_internal) {
#ifdef USE_BITSET
      NetworkState_Impl state2 = ~internal_state.getState();
      cumulator->setOutputMask(state2);
#else
      cumulator->setOutputMask(~internal_state.getState());
#endif
    }
    cumulator_v[nn] = cumulator;
  }
}

NodeIndex MaBEstEngine::getTargetNode(RandomGenerator* random_generator, const MAP<NodeIndex, double>& nodeTransitionRates, double total_rate) const
{
  double U_rand2 = random_generator->generate();
  double random_rate = U_rand2 * total_rate;
  MAP<NodeIndex, double>::const_iterator begin = nodeTransitionRates.begin();
  MAP<NodeIndex, double>::const_iterator end = nodeTransitionRates.end();
  NodeIndex node_idx = INVALID_NODE_INDEX;
  while (begin != end && random_rate > 0.) {
    node_idx = (*begin).first;
    double rate = (*begin).second;
    random_rate -= rate;
    ++begin;
  }

  assert(node_idx != INVALID_NODE_INDEX);
  assert(network->getNode(node_idx)->getIndex() == node_idx);
  return node_idx;
}

double MaBEstEngine::computeTH(const MAP<NodeIndex, double>& nodeTransitionRates, double total_rate) const
{
  if (nodeTransitionRates.size() == 1) {
    return 0.;
  }

  MAP<NodeIndex, double>::const_iterator begin = nodeTransitionRates.begin();
  MAP<NodeIndex, double>::const_iterator end = nodeTransitionRates.end();
  double TH = 0.;
  double rate_internal = 0.;

  while (begin != end) {
    NodeIndex index = (*begin).first;
    double rate = (*begin).second;
    if (network->getNode(index)->isInternal()) {
      rate_internal += rate;
    }
    ++begin;
  }

  double total_rate_non_internal = total_rate - rate_internal;

  begin = nodeTransitionRates.begin();

  while (begin != end) {
    NodeIndex index = (*begin).first;
    double rate = (*begin).second;
    if (!network->getNode(index)->isInternal()) {
      double proba = rate / total_rate_non_internal;
      TH -= log2(proba) * proba;
    }
    ++begin;
  }

  return TH;
}

struct ArgWrapper {
  MaBEstEngine* mabest;
  unsigned int start_count_thread;
  unsigned int sample_count_thread;
  Cumulator* cumulator;
  RandomGeneratorFactory* randgen_factory;
  int seed;
  STATE_MAP<NetworkState_Impl, unsigned int>* fixpoint_map;
  std::ostream* output_traj;

  ArgWrapper(MaBEstEngine* mabest, unsigned int start_count_thread, unsigned int sample_count_thread, Cumulator* cumulator, RandomGeneratorFactory* randgen_factory, int seed, STATE_MAP<NetworkState_Impl, unsigned int>* fixpoint_map, std::ostream* output_traj) :
    mabest(mabest), start_count_thread(start_count_thread), sample_count_thread(sample_count_thread), cumulator(cumulator), randgen_factory(randgen_factory), seed(seed), fixpoint_map(fixpoint_map), output_traj(output_traj) { }
};

void* MaBEstEngine::threadWrapper(void *arg)
{
  ArgWrapper* warg = (ArgWrapper*)arg;
  try {
    warg->mabest->runThread(warg->cumulator, warg->start_count_thread, warg->sample_count_thread, warg->randgen_factory, warg->seed, warg->fixpoint_map, warg->output_traj);
  } catch(const BNException& e) {
    std::cerr << e;
  }
  return NULL;
}

void MaBEstEngine::runThread(Cumulator* cumulator, unsigned int start_count_thread, unsigned int sample_count_thread, RandomGeneratorFactory* randgen_factory, int seed, STATE_MAP<NetworkState_Impl, unsigned int>* fixpoint_map, std::ostream* output_traj)
{
  const std::vector<Node*>& nodes = network->getNodes();
  std::vector<Node*>::const_iterator begin = nodes.begin();
  std::vector<Node*>::const_iterator end = nodes.end();
  unsigned int stable_cnt = 0;
  NetworkState network_state; 

  RandomGenerator* random_generator = randgen_factory->generateRandomGenerator(seed);
  for (unsigned int nn = 0; nn < sample_count_thread; ++nn) {
    random_generator->setSeed(seed+start_count_thread+nn);
    cumulator->rewind();
    network->initStates(network_state);
    double tm = 0.;
    unsigned int step = 0;
    if (NULL != output_traj) {
      (*output_traj) << "\nTrajectory #" << (nn+1) << '\n';
      (*output_traj) << " istate\t";
      network_state.displayOneLine(*output_traj, network);
      (*output_traj) << '\n';
    }
    while (tm < max_time) {
      double total_rate = 0.;
      MAP<NodeIndex, double> nodeTransitionRates;
      begin = nodes.begin();

      while (begin != end) {
	Node* node = *begin;
	NodeIndex node_idx = node->getIndex();
	if (node->getNodeState(network_state)) {
	  double r_down = node->getRateDown(network_state);
	  if (r_down != 0.0) {
	    total_rate += r_down;
	    nodeTransitionRates[node_idx] = r_down;
	  }
	} else {
	  double r_up = node->getRateUp(network_state);
	  if (r_up != 0.0) {
	    total_rate += r_up;
	    nodeTransitionRates[node_idx] = r_up;
	  }
	}
	++begin;
      }

      // EV: 2018-12-19 suppressed this block and integrated fixed point management below
      /*
      if (total_rate == 0.0) {
	std::cerr << "FP\n";
	// may have several fixpoint maps
	if (fixpoint_map->find(network_state.getState()) == fixpoint_map->end()) {
	  (*fixpoint_map)[network_state.getState()] = 1;
	} else {
	  (*fixpoint_map)[network_state.getState()]++;
	}
	cumulator->cumul(network_state, max_time, 0.);
	tm = max_time;
	stable_cnt++;
	break;
      }
      */

      double TH;
      if (total_rate == 0) {
	tm = max_time;
	TH = 0.;
	if (fixpoint_map->find(network_state.getState()) == fixpoint_map->end()) {
	  (*fixpoint_map)[network_state.getState()] = 1;
	} else {
	  (*fixpoint_map)[network_state.getState()]++;
	}
	stable_cnt++;
      } else {
	double transition_time ;
	if (discrete_time) {
	  transition_time = time_tick;
	} else {
	  double U_rand1 = random_generator->generate();
	  transition_time = -log(U_rand1) / total_rate;
	}
	
	tm += transition_time;
	TH = computeTH(nodeTransitionRates, total_rate);
      }

      if (NULL != output_traj) {
	(*output_traj) << std::setprecision(10) << tm << '\t';
	network_state.displayOneLine(*output_traj, network);
	(*output_traj) << '\t' << TH << '\n';
      }

      cumulator->cumul(network_state, tm, TH);

      if (tm >= max_time) {
	break;
      }

      NodeIndex node_idx = getTargetNode(random_generator, nodeTransitionRates, total_rate);
      network_state.flipState(network->getNode(node_idx));
      step++;
    }
    cumulator->trajectoryEpilogue();
  }
  delete random_generator;
}

void MaBEstEngine::run(std::ostream* output_traj)
{
  tid = new pthread_t[thread_count];
  RandomGeneratorFactory* randgen_factory = RunConfig::getInstance()->getRandomGeneratorFactory();
  int seed = RunConfig::getInstance()->getSeedPseudoRandom();
  unsigned int start_sample_count = 0;
  Probe probe;
  for (unsigned int nn = 0; nn < thread_count; ++nn) {
    STATE_MAP<NetworkState_Impl, unsigned int>* fixpoint_map = new STATE_MAP<NetworkState_Impl, unsigned int>();
    fixpoint_map_v.push_back(fixpoint_map);
    ArgWrapper* warg = new ArgWrapper(this, start_sample_count, cumulator_v[nn]->getSampleCount(), cumulator_v[nn], randgen_factory, seed, fixpoint_map, output_traj);
    pthread_create(&tid[nn], NULL, MaBEstEngine::threadWrapper, warg);
    arg_wrapper_v.push_back(warg);

    start_sample_count += cumulator_v[nn]->getSampleCount();
  }
  for (unsigned int nn = 0; nn < thread_count; ++nn) {
    pthread_join(tid[nn], NULL);
  }
  probe.stop();
  elapsed_core_runtime = probe.elapsed_msecs();
  user_core_runtime = probe.user_msecs();
  probe.start();
  epilogue();
  probe.stop();
  elapsed_epilogue_runtime = probe.elapsed_msecs();
  user_epilogue_runtime = probe.user_msecs();
}  

STATE_MAP<NetworkState_Impl, unsigned int>* MaBEstEngine::mergeFixpointMaps()
{
  if (1 == fixpoint_map_v.size()) {
    return new STATE_MAP<NetworkState_Impl, unsigned int>(*fixpoint_map_v[0]);
  }

  STATE_MAP<NetworkState_Impl, unsigned int>* fixpoint_map = new STATE_MAP<NetworkState_Impl, unsigned int>();
  std::vector<STATE_MAP<NetworkState_Impl, unsigned int>*>::iterator begin = fixpoint_map_v.begin();
  std::vector<STATE_MAP<NetworkState_Impl, unsigned int>*>::iterator end = fixpoint_map_v.end();
  while (begin != end) {
    STATE_MAP<NetworkState_Impl, unsigned int>* fp_map = *begin;
    STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator b = fp_map->begin();
    STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator e = fp_map->end();
    while (b != e) {
      NetworkState_Impl state = (*b).first;
      if (fixpoint_map->find(state) == fixpoint_map->end()) {
	(*fixpoint_map)[state] = (*b).second;
      } else {
	(*fixpoint_map)[state] += (*b).second;
      }
      ++b;
    }
    ++begin;
  }
  return fixpoint_map;
}

void MaBEstEngine::epilogue()
{
  merged_cumulator = Cumulator::mergeCumulators(cumulator_v);
  merged_cumulator->epilogue(network, reference_state);

  STATE_MAP<NetworkState_Impl, unsigned int>* merged_fixpoint_map = mergeFixpointMaps();

  STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator b = merged_fixpoint_map->begin();
  STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator e = merged_fixpoint_map->end();

  while (b != e) {
    fixpoints[NetworkState((*b).first).getState()] = (*b).second;
    ++b;
  }
  delete merged_fixpoint_map;
}

void MaBEstEngine::display(std::ostream& output_probtraj, std::ostream& output_statdist, std::ostream& output_fp, bool hexfloat) const
{
  Probe probe;
  merged_cumulator->displayCSV(network, refnode_count, output_probtraj, output_statdist, hexfloat);
  probe.stop();
  elapsed_statdist_runtime = probe.elapsed_msecs();
  user_statdist_runtime = probe.user_msecs();

  unsigned int statdist_traj_count = RunConfig::getInstance()->getStatDistTrajCount();
  if (statdist_traj_count == 0) {
    output_statdist << "Trajectory\tState\tProba\n";
  }

  output_fp << "Fixed Points (" << fixpoints.size() << ")\n";
  if (0 == fixpoints.size()) {
    return;
  }

#ifdef HAS_STD_HEXFLOAT
  if (hexfloat) {
    output_fp << std::hexfloat;
  }
#endif

  STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator begin = fixpoints.begin();
  STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator end = fixpoints.end();
  
  output_fp << "FP\tProba\tState\t";
  network->displayHeader(output_fp);
  for (unsigned int nn = 0; begin != end; ++nn) {
    const NetworkState& network_state = (*begin).first;
    output_fp << "#" << (nn+1) << "\t";
    if (hexfloat) {
      output_fp << fmthexdouble((double)(*begin).second / sample_count) <<  "\t";
    } else {
      output_fp << ((double)(*begin).second / sample_count) <<  "\t";
    }
    network_state.displayOneLine(output_fp, network);
    output_fp << '\t';
    network_state.display(output_fp, network);
    ++begin;
  }
}

const std::map<double, STATE_MAP<NetworkState_Impl, double> > MaBEstEngine::getStateDists() const {
  return merged_cumulator->getStateDists();
}

const STATE_MAP<NetworkState_Impl, double> MaBEstEngine::getNthStateDist(int nn) const {
  return merged_cumulator->getNthStateDist(nn);
}

const STATE_MAP<NetworkState_Impl, double> MaBEstEngine::getAsymptoticStateDist() const {
  return merged_cumulator->getAsymptoticStateDist();
}

const std::map<double, std::map<Node *, double> > MaBEstEngine::getNodesDists() const {

  std::map<double, std::map<Node *, double> > result;

  const std::map<double, STATE_MAP<NetworkState_Impl, double> > state_dist = merged_cumulator->getStateDists();

  std::map<double, STATE_MAP<NetworkState_Impl, double> >::const_iterator begin = state_dist.begin();
  std::map<double, STATE_MAP<NetworkState_Impl, double> >::const_iterator end = state_dist.end();
  
  const std::vector<Node*>& nodes = network->getNodes();

  while(begin != end) {

    std::map<Node *, double> node_dist;
    STATE_MAP<NetworkState_Impl, double> present_state_dist = begin->second;

    std::vector<Node *>::const_iterator nodes_begin = nodes.begin();
    std::vector<Node *>::const_iterator nodes_end = nodes.end();

    while(nodes_begin != nodes_end) {

      double dist = 0;

      STATE_MAP<NetworkState_Impl, double>::const_iterator states_begin = present_state_dist.begin();
      STATE_MAP<NetworkState_Impl, double>::const_iterator states_end = present_state_dist.end();
    
      while(states_begin != states_end) {

        NetworkState state = states_begin->first;
        dist += states_begin->second * ((double) state.getNodeState(*nodes_begin));

        states_begin++;
      }

      node_dist[*nodes_begin] = dist;

      nodes_begin++;
    }

    result[begin->first] = node_dist;

    begin++;
  }

  return result;
}

const std::map<Node*, double> MaBEstEngine::getNthNodesDist(int nn) const {
  std::map<Node *, double> result;

  const STATE_MAP<NetworkState_Impl, double> state_dist = merged_cumulator->getNthStateDist(nn);
  
  const std::vector<Node*>& nodes = network->getNodes();
  std::vector<Node *>::const_iterator nodes_begin = nodes.begin();
  std::vector<Node *>::const_iterator nodes_end = nodes.end();

  while(nodes_begin != nodes_end) {

    double dist = 0;

    STATE_MAP<NetworkState_Impl, double>::const_iterator states_begin = state_dist.begin();
    STATE_MAP<NetworkState_Impl, double>::const_iterator states_end = state_dist.end();
  
    while(states_begin != states_end) {

      NetworkState state = states_begin->first;
      dist += states_begin->second * ((double) state.getNodeState(*nodes_begin));

      states_begin++;
    }

    result[*nodes_begin] = dist;

    nodes_begin++;
  }

  return result;  
}

const std::map<Node*, double> MaBEstEngine::getAsymptoticNodesDist() const {
  std::map<Node *, double> result;

  const STATE_MAP<NetworkState_Impl, double> state_dist = merged_cumulator->getAsymptoticStateDist();
  
  const std::vector<Node*>& nodes = network->getNodes();
  std::vector<Node *>::const_iterator nodes_begin = nodes.begin();
  std::vector<Node *>::const_iterator nodes_end = nodes.end();

  while(nodes_begin != nodes_end) {

    double dist = 0;

    STATE_MAP<NetworkState_Impl, double>::const_iterator states_begin = state_dist.begin();
    STATE_MAP<NetworkState_Impl, double>::const_iterator states_end = state_dist.end();
  
    while(states_begin != states_end) {

      NetworkState state = states_begin->first;
      dist += states_begin->second * ((double) state.getNodeState(*nodes_begin));

      states_begin++;
    }

    result[*nodes_begin] = dist;

    nodes_begin++;
  }

  return result;  
}

const std::map<double, double> MaBEstEngine::getNodeDists(Node * node) const {
 
  std::map<double, double> result;

  const std::map<double, STATE_MAP<NetworkState_Impl, double> > state_dist = merged_cumulator->getStateDists();

  std::map<double, STATE_MAP<NetworkState_Impl, double> >::const_iterator begin = state_dist.begin();
  std::map<double, STATE_MAP<NetworkState_Impl, double> >::const_iterator end = state_dist.end();

  while(begin != end) {

    STATE_MAP<NetworkState_Impl, double> present_state_dist = begin->second;
    double dist = 0;

    STATE_MAP<NetworkState_Impl, double>::const_iterator states_begin = present_state_dist.begin();
    STATE_MAP<NetworkState_Impl, double>::const_iterator states_end = present_state_dist.end();
  
    while(states_begin != states_end) {

      NetworkState state = states_begin->first;
      dist += states_begin->second * ((double) state.getNodeState(node));

      states_begin++;
    }
    result[begin->first] = dist;

    begin++;
  }

  return result; 
}

double MaBEstEngine::getNthNodeDist(Node * node, int nn) const {

  double result = 0;

  const STATE_MAP<NetworkState_Impl, double> state_dist = merged_cumulator->getNthStateDist(nn);
  
  STATE_MAP<NetworkState_Impl, double>::const_iterator states_begin = state_dist.begin();
  STATE_MAP<NetworkState_Impl, double>::const_iterator states_end = state_dist.end();

  while(states_begin != states_end) {

    NetworkState state = states_begin->first;
    result += states_begin->second * ((double) state.getNodeState(node));

    states_begin++;
  }

  return result;  
}

double MaBEstEngine::getAsymptoticNodeDist(Node * node) const {

  double result = 0;

  const STATE_MAP<NetworkState_Impl, double> state_dist = merged_cumulator->getAsymptoticStateDist();
  
  STATE_MAP<NetworkState_Impl, double>::const_iterator states_begin = state_dist.begin();
  STATE_MAP<NetworkState_Impl, double>::const_iterator states_end = state_dist.end();

  while(states_begin != states_end) {

    NetworkState state = states_begin->first;
    result += states_begin->second * ((double) state.getNodeState(node));

    states_begin++;
  }

  return result;  
}

MaBEstEngine::~MaBEstEngine()
{
  for (std::vector<Cumulator*>::iterator iter = cumulator_v.begin(); iter < cumulator_v.end(); ++iter) {
    delete *iter;
  }

  for (std::vector<STATE_MAP<NetworkState_Impl, unsigned int>*>::iterator iter = fixpoint_map_v.begin(); iter < fixpoint_map_v.end(); ++iter) {
    delete *iter;
  }

  for (std::vector<ArgWrapper*>::iterator iter = arg_wrapper_v.begin(); iter < arg_wrapper_v.end(); ++iter) {
    delete *iter;
  }

  delete merged_cumulator;
  delete [] tid;
}

