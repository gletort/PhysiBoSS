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
     MaBoSS.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     January-March 2011
*/

#include "MaBEstEngine.h"
#include <fstream>
#include <stdlib.h>

static const char* prog;

static int usage(std::ostream& os = std::cerr)
{
  os << "\nUsage:\n\n";
  os << "  " << prog << " [-h|--help]\n\n";
  os << "  " << prog << " [-V|--version]\n\n";
  os << "  " << prog << " [-c|--config CONF_FILE] [-v|--config-vars VAR1=NUMERIC[,VAR2=...]] [-e|--config-expr CONFIG_EXPR] -o|--output OUTPUT BOOLEAN_NETWORK_FILE\n\n";
  os << "  " << prog << " [-c|--config CONF_FILE] [-v|--config-vars VAR1=NUMERIC[,VAR2=...]] [-e|--config-expr CONFIG_EXPR] -d|--dump-config BOOLEAN_NETWORK_FILE\n\n";
  os << "  " << prog << " [-c|--config CONF_FILE] [-v|--config-vars VAR1=NUMERIC[,VAR2=...]] [-e|--config-expr CONFIG_EXPR] -l|--generate-logical-expressions BOOLEAN_NETWORK_FILE\n\n";
  os << "  " << prog << " -t|--generate-config-template BOOLEAN_NETWORK_FILE\n";
  return 1;
}

static int setConfigVariables(std::vector<std::string>& runvar_v)
{
  SymbolTable* symtab = SymbolTable::getInstance();
  std::vector<std::string>::const_iterator begin = runvar_v.begin();
  std::vector<std::string>::const_iterator end = runvar_v.end();

  while (begin != end) {
    const std::string& var_values = *begin;
    size_t o_var_value_pos = 0;
    for (;;) {
      if (o_var_value_pos == std::string::npos) {
	break;
      }
      size_t var_value_pos = var_values.find(',', o_var_value_pos);
      std::string var_value = var_value_pos == std::string::npos ? var_values.substr(o_var_value_pos) : var_values.substr(o_var_value_pos, var_value_pos-o_var_value_pos);
      o_var_value_pos = var_value_pos + (var_value_pos == std::string::npos ? 0 : 1);
      size_t pos = var_value.find('=');
      if (pos == std::string::npos) {
	std::cerr << '\n' << prog << ": invalid var format [" << var_value << "] VAR=BOOL_OR_DOUBLE expected\n";
	return 1;
      }
      std::string ovar = var_value.substr(0, pos);
      std::string var = ovar[0] != '$' ? "$" + ovar : ovar;
      const Symbol* symbol = symtab->getOrMakeSymbol(var);
      std::string value = var_value.substr(pos+1);
      if (!strcasecmp(value.c_str(), "true")) {
	symtab->overrideSymbolValue(symbol, 1);
      } else if (!strcasecmp(value.c_str(), "false")) {
	symtab->overrideSymbolValue(symbol, 0);
      } else {
	double dval;
	int r = sscanf(value.c_str(), "%lf", &dval);
	if (r != 1) {
	  std::cerr << '\n' << prog << ": invalid value format [" << var_value << "] " << ovar << "=BOOL_OR_DOUBLE expected\n";
	  return 1;
	}
	symtab->overrideSymbolValue(symbol, dval);
      }
    }
    ++begin;
  }
  return 0;
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
  std::cout << "  -o --output OUTPUT                      : prefix to be used for output files; when present run MaBoSS simulation process\n";
  std::cout << "  -d --dump-config                        : dumps configuration and exits\n";
  std::cout << "  -t --generate-config-template           : generates template configuration and exits\n";
  std::cout << "  -l --generate-logical-expressions       : generates the logical expressions and exits\n";
  std::cout << "  -h --help                               : displays this message\n";
  std::cout << "\nNotices:\n";
  std::cout << "\n1. --config and --config-expr options can be used multiple times;\n";
  std::cout << "   multiple --config and/or --config-expr options are managed in the order given at the command line;\n";
  std::cout << "   --config-vars VAR=VALUE always overrides any VAR assignment in a configuration file or expression\n";
  std::cout << "\n2. --dump-config, --generate-config-template, --generate-logical-expressions and --output are exclusive options\n";
  std::cout << '\n';
  return 0;
}

class ConfigOpt {
  std::string file_or_expr;
  bool is_expr;
  static unsigned int runconfig_file_cnt;
  static unsigned int runconfig_expr_cnt;
public:
  ConfigOpt(const std::string& file_or_expr, bool is_expr) : file_or_expr(file_or_expr), is_expr(is_expr) {
    if (is_expr) {runconfig_expr_cnt++;} else {runconfig_file_cnt++;}
  }
  bool isExpr() const {return is_expr;}
  const std::string& getExpr() const {assert(is_expr); return file_or_expr;}
  const std::string& getFile() const {assert(!is_expr); return file_or_expr;}

  static unsigned int getFileCount() {return runconfig_file_cnt;}
  static unsigned int getExprCount() {return runconfig_expr_cnt;}
};

unsigned int ConfigOpt::runconfig_file_cnt = 0;
unsigned int ConfigOpt::runconfig_expr_cnt = 0;

int main(int argc, char* argv[])
{
  const char* output = NULL;
  std::vector<ConfigOpt> runconfig_file_or_expr_v;
  std::vector<std::string> runconfig_var_v;
  const char* ctbndl_file = NULL;
  bool dump_config = false;
  bool generate_config_template = false;
  bool generate_logical_expressions = false;
  dont_shrink_logical_expressions = false; // global flag
  prog = argv[0];

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
      } else if (!strcmp(s, "-o") || !strcmp(s, "--output")) {
	if (nn == argc-1) {std::cerr << '\n' << prog << ": missing value after option " << s << '\n'; return usage();}
	output = argv[++nn];
      } else if (!strcmp(s, "-c") || !strcmp(s, "--config")) {
	if (nn == argc-1) {std::cerr << '\n' << prog << ": missing value after option " << s << '\n'; return usage();}
	runconfig_file_or_expr_v.push_back(ConfigOpt(argv[++nn], false));
      } else if (!strcmp(s, "--help") || !strcmp(s, "-h")) {
	return help();
      } else {
	std::cerr << '\n' << prog << ": unknown option " << s << std::endl;
	return usage();
      }
    } else if (ctbndl_file == NULL) {
      ctbndl_file = argv[nn];
    } else {
      std::cerr << '\n' << prog << ": boolean network file is already set to " << ctbndl_file << " [" << s << "]" << std::endl;
    }
  }

  if (NULL == ctbndl_file) {
    std::cerr << '\n' << prog << ": boolean network file is missing\n";
    return usage();
  }
    
  if (!dump_config && !generate_config_template && !generate_logical_expressions && output == NULL) {
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

  std::ostream* output_run = NULL;
  std::ostream* output_traj = NULL;
  std::ostream* output_probtraj = NULL;
  std::ostream* output_statdist = NULL;
  std::ostream* output_fp = NULL;

  try {
    time_t start_time, end_time;
    Network* network = new Network();

    network->parse(ctbndl_file);

    RunConfig* runconfig = RunConfig::getInstance();

    if (generate_config_template) {
      IStateGroup::checkAndComplete(network);
      runconfig->generateTemplate(network, std::cout);
      return 0;
    }

    if (setConfigVariables(runconfig_var_v)) {
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
    mabest.display(*output_probtraj, *output_statdist, *output_fp);
    time(&end_time);

    runconfig->display(network, start_time, end_time, mabest, *output_run);

    ((std::ofstream*)output_run)->close();
    if (NULL != output_traj) {
      ((std::ofstream*)output_traj)->close();
    }
    ((std::ofstream*)output_probtraj)->close();
    ((std::ofstream*)output_statdist)->close();
    ((std::ofstream*)output_fp)->close();
  } catch(const BNException& e) {
    std::cerr << '\n' << prog << ": " << e;
    return 1;
  }

  return 0;
}
