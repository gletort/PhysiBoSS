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
     MaBoSS.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     January-March 2011
*/

#include "MaBEstEngine.h"
#include "EnsembleEngine.h"
#include <fstream>
#include <stdlib.h>
#include "Utils.h"

const char* prog = "MaBoSS";

static int usage(std::ostream& os = std::cerr)
{
  os << "\nUsage:\n\n";
  os << "  " << prog << " [-h|--help]\n\n";
  os << "  " << prog << " [-V|--version]\n\n";
  os << "  " << prog << " [-c|--config CONF_FILE] [-v|--config-vars VAR1=NUMERIC[,VAR2=...]] [-e|--config-expr CONFIG_EXPR] -o|--output OUTPUT BOOLEAN_NETWORK_FILE\n\n";
  os << "  " << prog << " [-c|--config CONF_FILE] [-v|--config-vars VAR1=NUMERIC[,VAR2=...]] [-e|--config-expr CONFIG_EXPR] -d|--dump-config BOOLEAN_NETWORK_FILE\n\n";
  os << "  " << prog << " [-c|--config CONF_FILE] [-v|--config-vars VAR1=NUMERIC[,VAR2=...]] [-e|--config-expr CONFIG_EXPR] -l|--generate-logical-expressions BOOLEAN_NETWORK_FILE\n\n";
  os << "  " << prog << " -t|--generate-config-template BOOLEAN_NETWORK_FILE\n";
  os << "  " << prog << " [--check]\n";
  os << "  " << prog << " [--override]\n";
  os << "  " << prog << " [--augment]\n";
  os << "  " << prog << " [--hexfloat]\n";
  return 1;
}

static int help()
{
  //  std::cout << "\n=================================================== " << prog << " help " << "===================================================\n";
  (void)usage(std::cout);
  std::cout << "\nOptions:\n\n";
  std::cout << "  -V --version                            : displays MaBoSS version\n";
  std::cout << "  -c --config CONF_FILE                   : uses CONF_FILE as a configuration file\n";
  std::cout << "  -v --config-vars VAR=NUMERIC[,VAR2=...] : sets the value of the given variables to the given numeric values\n";
  //  std::cout << "                                        the VAR value in the configuration file (if present) will be overriden\n";
  std::cout << "  -e --config-expr CONFIG_EXPR            : evaluates the configuration expression; may have multiple expressions\n";
  std::cout << "                                            separated by semi-colons\n";
  std::cout << "  --override                              : if set, a new node definition will replace a previous one\n";
  std::cout << "  --augment                               : if set, a new node definition will complete (add non existing attributes) / override (replace existing attributes) a previous one\n";
  std::cout << "  -o --output OUTPUT                      : prefix to be used for output files; when present run MaBoSS simulation process\n";
  std::cout << "  -d --dump-config                        : dumps configuration and exits\n";
  std::cout << "  -t --generate-config-template           : generates template configuration and exits\n";
  std::cout << "  -l --generate-logical-expressions       : generates the logical expressions and exits\n";
  std::cout << "  --check                                 : checks network and configuration files and exits\n";
  std::cout << "  --hexfloat                              : displays double in hexadecimal format\n";
  std::cout << "  -h --help                               : displays this message\n";
  std::cout << "\nNotices:\n";
  std::cout << "\n1. --config and --config-expr options can be used multiple times;\n";
  std::cout << "   multiple --config and/or --config-expr options are managed in the order given at the command line;\n";
  std::cout << "   --config-vars VAR=VALUE always overrides any VAR assignment in a configuration file or expression\n";
  std::cout << "\n2. --dump-config, --generate-config-template, --generate-logical-expressions and --output are exclusive options\n";
  std::cout << '\n';

  std::cout << "Builtin functions:\n\n";
  Function::displayFunctionDescriptions(std::cout);

  return 0;
}

int main(int argc, char* argv[])
{
  const char* output = NULL;
  std::vector<ConfigOpt> runconfig_file_or_expr_v;
  std::vector<std::string> runconfig_var_v;
  const char* ctbndl_file = NULL;
  bool ensemble = false;
  std::vector<char *> ctbndl_files;
  bool dump_config = false;
  bool generate_config_template = false;
  bool generate_logical_expressions = false;
  bool hexfloat = false;
  bool check = false;
  dont_shrink_logical_expressions = false; // global flag
  
  MaBEstEngine::init();

  for (int nn = 1; nn < argc; ++nn) {
    const char* s = argv[nn];
    if (s[0] == '-') {
      if (!strcmp(s, "-version") || !strcmp(s, "--version") || !strcmp(s, "-V")) { // keep -version for backward compatibility
	std::cout << "MaBoSS version " + MaBEstEngine::VERSION << " [networks up to " << MAXNODES << " nodes]\n";
	return 0;
      } else if (!strcmp(s, "--config-vars") || !strcmp(s, "-v")) {
	if (nn == argc-1) {std::cerr << '\n' << prog << ": missing value after option " << s << '\n'; return usage();}
	runconfig_var_v.push_back(argv[++nn]);
      } else if (!strcmp(s, "--config-expr") || !strcmp(s, "-e")) {
	if (nn == argc-1) {std::cerr << '\n' << prog << ": missing value after option " << s << '\n'; return usage();}
	runconfig_file_or_expr_v.push_back(ConfigOpt(argv[++nn], true));
      } else if (!strcmp(s, "--dump-config") || !strcmp(s, "-d")) {
	dump_config = true;
      } else if (!strcmp(s, "--generate-config-template") || !strcmp(s, "-t")) {
	generate_config_template = true;
      } else if (!strcmp(s, "--generate-logical-expressions") || !strcmp(s, "-l")) {
	generate_logical_expressions = true;
      } else if (!strcmp(s, "--dont-shrink-logical-expressions")) {
	dont_shrink_logical_expressions = true;
      } else if (!strcmp(s, "--ensemble")) {
  ensemble = true;
      } else if (!strcmp(s, "--load-user-functions")) {
	if (nn == argc-1) {std::cerr << '\n' << prog << ": missing value after option " << s << '\n'; return usage();}
	MaBEstEngine::loadUserFuncs(argv[++nn]);
      } else if (!strcmp(s, "-o") || !strcmp(s, "--output")) {
	if (nn == argc-1) {std::cerr << '\n' << prog << ": missing value after option " << s << '\n'; return usage();}
	output = argv[++nn];
      } else if (!strcmp(s, "-c") || !strcmp(s, "--config")) {
	if (nn == argc-1) {std::cerr << '\n' << prog << ": missing value after option " << s << '\n'; return usage();}
	runconfig_file_or_expr_v.push_back(ConfigOpt(argv[++nn], false));
      } else if (!strcmp(s, "--override")) {
	if (Node::isAugment()) {
	  std::cerr << '\n' << prog << ": --override and --augment are exclusive options\n"; return usage();
	}
	Node::setOverride(true);
      } else if (!strcmp(s, "--augment")) {
	if (Node::isOverride()) {
	  std::cerr << '\n' << prog << ": --override and --augment are exclusive options\n"; return usage();
	}
	Node::setAugment(true);
      } else if (!strcmp(s, "--check")) {
	check = true;
      } else if (!strcmp(s, "--hexfloat")) {
	hexfloat = true;
      } else if (!strcmp(s, "--help") || !strcmp(s, "-h")) {
	return help();
      } else {
	std::cerr << '\n' << prog << ": unknown option " << s << std::endl;
	return usage();
      }
    } else if (!ensemble && ctbndl_file == NULL) {
      ctbndl_file = argv[nn];
    } else if (ensemble) {
      ctbndl_files.push_back(argv[nn]);
    } else {
      std::cerr << '\n' << prog << ": boolean network file is already set to " << ctbndl_file << " [" << s << "]" << std::endl;
    }
  }

  if (!ensemble && NULL == ctbndl_file)
  {
    std::cerr << '\n'
              << prog << ": boolean network file is missing\n";
    return usage();
  }

  if (ensemble && ctbndl_files.size() == 0) {
    std::cerr << '\n'
              << prog << ": ensemble networks are missing\n";
    return usage();
  }
    
  if (!dump_config && !generate_config_template && !generate_logical_expressions && !check && output == NULL) {
    std::cerr << '\n' << prog << ": ouput option is not set\n";
    return usage();
  }

  if (dump_config && generate_config_template) {
    std::cerr << '\n' << prog << ": --dump-config and --generate-config-template are exclusive options\n";
    return usage();
  }

  if (dump_config && output) {
    std::cerr << '\n' << prog << ": --dump-config and -o|--output are exclusive options\n";
    return usage();
  }

  if (generate_config_template && output) {
    std::cerr << '\n' << prog << ": --generate-config-template and -o|--output are exclusive options\n";
    return usage();
  }

  if (generate_config_template && ConfigOpt::getFileCount() > 0) {
    std::cerr<< '\n'  << prog << ": --generate-config-template and -c|--config are exclusive options\n";
    return usage();
  }

  if (generate_config_template && ConfigOpt::getExprCount() > 0) {
    std::cerr << '\n' << prog << ": --generate-config-template and --config-expr are exclusive options\n";
    return usage();
  }

  if (generate_config_template && runconfig_var_v.size() > 0) {
    std::cerr << '\n' << prog << ": --generate-config-template and --config-vars are exclusive options\n";
    return usage();
  }

  if (check && output) {
    std::cerr << '\n' << prog << ": --check and -o|--output are exclusive options\n";
    return usage();
  }

  std::ostream* output_run = NULL;
  std::ostream* output_traj = NULL;
  std::ostream* output_probtraj = NULL;
  std::ostream* output_statdist = NULL;
  std::ostream* output_fp = NULL;

  try {
    time_t start_time, end_time;

    if (ensemble) {

      std::vector<Network *> networks;
      RunConfig* runconfig = RunConfig::getInstance();      

      Network* first_network = new Network();
      first_network->parse(ctbndl_files[0]);
      networks.push_back(first_network);

      std::vector<ConfigOpt>::const_iterator begin = runconfig_file_or_expr_v.begin();
      std::vector<ConfigOpt>::const_iterator end = runconfig_file_or_expr_v.end();
      while (begin != end) {
        const ConfigOpt& cfg = *begin;
        if (cfg.isExpr()) {
          runconfig->parseExpression(networks[0], (cfg.getExpr() + ";").c_str());
        } else {
          runconfig->parse(networks[0], cfg.getFile().c_str());
        }
        ++begin;
      }

      IStateGroup::checkAndComplete(networks[0]);

      std::map<std::string, NodeIndex> nodes_indexes;
      std::vector<Node*> first_network_nodes = first_network->getNodes();
      for (unsigned int i=0; i < first_network_nodes.size(); i++) {
        Node* t_node = first_network_nodes[i];
        nodes_indexes[t_node->getLabel()] = t_node->getIndex();
      }

      for (unsigned int i=1; i < ctbndl_files.size(); i++) {
        
        Network* network = new Network();
        network->parse(ctbndl_files[i], &nodes_indexes);
        networks.push_back(network);

        const std::vector<Node*> nodes = networks[i]->getNodes();
        for (unsigned int j=0; j < nodes.size(); j++) {
            if (!first_network_nodes[j]->istateSetRandomly()) {
                nodes[j]->setIState(first_network_nodes[j]->getIState(first_network));
            }

            nodes[j]->isInternal(first_network_nodes[j]->isInternal());

            // if (!first_network_nodes[j]->isReference()) {
            //   nodes[j]->setReferenceState(first_network_nodes[j]->getReferenceState());
            // }
        }

        IStateGroup::checkAndComplete(networks[i]);
      }
      SymbolTable::getInstance()->checkSymbols();

      // output_run = new std::ofstream((std::string(output) + "_run.txt").c_str());
      output_probtraj = new std::ofstream((std::string(output) + "_probtraj.csv").c_str());
      output_statdist = new std::ofstream((std::string(output) + "_statdist.csv").c_str());
      output_fp = new std::ofstream((std::string(output) + "_fp.csv").c_str());

      time(&start_time);
      EnsembleEngine engine(networks, runconfig);
      engine.run(NULL);
      engine.display(*output_probtraj, *output_statdist, *output_fp, hexfloat);
      time(&end_time);

      // ((std::ofstream*)output_run)->close();
      ((std::ofstream*)output_probtraj)->close();
      ((std::ofstream*)output_statdist)->close();
      ((std::ofstream*)output_fp)->close();

    } else {
        
      Network* network = new Network();

      network->parse(ctbndl_file);

      RunConfig* runconfig = RunConfig::getInstance();

      if (generate_config_template) {
        IStateGroup::checkAndComplete(network);
        runconfig->generateTemplate(network, std::cout);
        return 0;
      }

      if (setConfigVariables(prog, runconfig_var_v)) {
        return 1;
      }      

      std::vector<ConfigOpt>::const_iterator begin = runconfig_file_or_expr_v.begin();
      std::vector<ConfigOpt>::const_iterator end = runconfig_file_or_expr_v.end();
      while (begin != end) {
        const ConfigOpt& cfg = *begin;
        if (cfg.isExpr()) {
    runconfig->parseExpression(network, (cfg.getExpr() + ";").c_str());
        } else {
    runconfig->parse(network, cfg.getFile().c_str());
        }
        ++begin;
      }

      IStateGroup::checkAndComplete(network);

      SymbolTable::getInstance()->checkSymbols();

      if (check) {
        return 0;
      }

      if (generate_logical_expressions) {
        network->generateLogicalExpressions(std::cout);
        return 0;
      }

      if (dump_config) {
        runconfig->dump(network, std::cout);
        return 0;
      }

      if (runconfig->displayTrajectories()) {
        if (runconfig->getThreadCount() > 1) {
    std::cerr << '\n' << prog << ": warning: cannot display trajectories in multi-threaded mode\n";
        } else {
    output_traj = new std::ofstream((std::string(output) + "_traj.txt").c_str());
        }
      }

      output_run = new std::ofstream((std::string(output) + "_run.txt").c_str());
      output_probtraj = new std::ofstream((std::string(output) + "_probtraj.csv").c_str());
      output_statdist = new std::ofstream((std::string(output) + "_statdist.csv").c_str());
      output_fp = new std::ofstream((std::string(output) + "_fp.csv").c_str());

      time(&start_time);
      MaBEstEngine mabest(network, runconfig);
      mabest.run(output_traj);
      mabest.display(*output_probtraj, *output_statdist, *output_fp, hexfloat);
      time(&end_time);

      runconfig->display(network, start_time, end_time, mabest, *output_run);

      ((std::ofstream*)output_run)->close();
      if (NULL != output_traj) {
        ((std::ofstream*)output_traj)->close();
      }
      ((std::ofstream*)output_probtraj)->close();
      ((std::ofstream*)output_statdist)->close();
      ((std::ofstream*)output_fp)->close();
    }
  } catch(const BNException& e) {
    std::cerr << '\n' << prog << ": " << e;
    return 1;
  }

  return 0;
}

