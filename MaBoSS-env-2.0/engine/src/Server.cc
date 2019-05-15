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
     Server.cc

   Authors:
     Eric Viara <viara@sysra.com>
 
   Date:
     May 2018
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <signal.h>

#include "Client.h"
#include "Server.h"
#include "DataStreamer.h"
#include "RPC.h"
#include "Utils.h"
#include "MaBEstEngine.h"

Server* Server::server;
static const char* RPC_portname;

static void unlink_tempfiles_handler(int sig)
{
  const std::string& pidfile = Server::getInstance()->getPidFile();
  if (pidfile.length() > 0) {
    unlink(pidfile.c_str());
  }
  if (NULL != RPC_portname) {
    unlink(RPC_portname);
  }
  exit(1);
}

int Server::manageRequests()
{
  MaBEstEngine::init();
  if (bind(&RPC_portname) == rpc_Success) {
    if (0 != pidfile.length()) {
      std::ofstream fpidfile(pidfile.c_str());
      if (fpidfile.bad() || fpidfile.fail()) {
	std::cerr << "cannot open pidfile " << pidfile << " for writing\n";
	return 1;
      }
      fpidfile << getpid() << '\n';
      fpidfile.close();
    }
    signal(SIGINT, unlink_tempfiles_handler);
    signal(SIGQUIT, unlink_tempfiles_handler);
    signal(SIGTERM, unlink_tempfiles_handler);
    signal(SIGABRT, unlink_tempfiles_handler);

    signal(SIGCHLD, SIG_IGN);
    
    time_t t = time(NULL);
    char* now = ctime(&t);
    if (!quiet) {
      std::cerr << "\n" << prog << " [listen=" << host << ":" << port << "] Ready at " << now;
    }
    listen();
    return 0;
  }
  return 1;
}

static std::ofstream* create_temp_file(const std::string& file, std::vector<std::string>& files_to_delete_v)
{
  std::ofstream* os = new std::ofstream(file.c_str());
  files_to_delete_v.push_back(file);
  return os;
}

static void delete_temp_files(const std::vector<std::string>& files_to_delete_v)
{
  for (std::vector<std::string>::const_iterator iter = files_to_delete_v.begin(); iter != files_to_delete_v.end(); ++iter) {
    unlink(iter->c_str());
  }
}

void Server::run(const ClientData& client_data, ServerData& server_data)
{
  static const std::string hst = "==================";
  std::ostream* output_run = NULL;
  std::ostream* output_traj = NULL;
  std::ostream* output_probtraj = NULL;
  std::ostream* output_statdist = NULL;
  std::ostream* output_fp = NULL;

  std::ostringstream ostr;
  struct timeval tv;
  gettimeofday(&tv, 0);
  ostr << "/tmp/MaBoSS-server_" << tv.tv_sec << "_" << tv.tv_usec << "_" << getpid();
  std::string output = ostr.str();
  std::string traj_file = std::string(output) + "_traj.txt";
  std::string runlog_file = std::string(output) + "_run.txt";
  std::string probtraj_file = std::string(output) + "_probtraj.csv";
  std::string statdist_file = std::string(output) + "_statdist.csv";
  std::string fp_file = std::string(output) + "_fp.csv";

  std::vector<std::string> files_to_delete_v;

  try {
    time_t start_time, end_time;
    time(&start_time);
    char* timebuf = ctime(&start_time);
    timebuf[strlen(timebuf)-1] = 0;
    if (!quiet) {
      std::cerr << "\n" << hst << " " << prog << " running simulation at " << timebuf << " " << hst << "\n";
    }

    Node::setOverride((client_data.getFlags() & DataStreamer::OVERRIDE_FLAG) != 0);
    Node::setAugment((client_data.getFlags() & DataStreamer::AUGMENT_FLAG) != 0);
    if (Node::isOverride() && Node::isAugment()) {
      server_data.setStatus(2);
      server_data.setErrorMessage("override and augment are exclusive flags");
      return;
    }

    Network* network = new Network();
    std::string network_file = output + "_network.bnd";
    filePutContents(network_file, client_data.getNetwork());
    files_to_delete_v.push_back(network_file);

    network->parse(network_file.c_str());

    RunConfig* runconfig = RunConfig::getInstance();
    const std::string& config_vars = client_data.getConfigVars();
    if (config_vars.length() > 0) {
      std::vector<std::string> runconfig_var_v;
      runconfig_var_v.push_back(config_vars);
      if (setConfigVariables(prog, runconfig_var_v)) {
	delete_temp_files(files_to_delete_v);
	//return 1;
	// TBD: error
	return;
      }      
    }

    const std::vector<std::string>& configs = client_data.getConfigs();
    for (std::vector<std::string>::const_iterator iter = configs.begin(); iter != configs.end(); ++iter) {
      runconfig->parseExpression(network, iter->c_str());
    }

    const std::vector<std::string>& config_exprs = client_data.getConfigExprs();
    for (std::vector<std::string>::const_iterator iter = config_exprs.begin(); iter != config_exprs.end(); ++iter) {
      runconfig->parseExpression(network, iter->c_str());
    }

    IStateGroup::checkAndComplete(network);

    SymbolTable::getInstance()->checkSymbols();

    if (client_data.getCommand() == DataStreamer::CHECK_COMMAND) {
      delete_temp_files(files_to_delete_v);
      server_data.setStatus(0);
      return;
    }

    if (runconfig->displayTrajectories()) {
      if (runconfig->getThreadCount() > 1) {
	if (!quiet) {
	  std::cerr << '\n' << prog << ": warning: cannot display trajectories in multi-threaded mode\n";
	}
      } else {
	output_traj = create_temp_file(traj_file, files_to_delete_v);
      }
    }

    output_run = create_temp_file(runlog_file, files_to_delete_v);
    output_probtraj = create_temp_file(probtraj_file, files_to_delete_v);
    output_statdist = create_temp_file(statdist_file, files_to_delete_v);
    output_fp = create_temp_file(fp_file, files_to_delete_v);

    bool hexfloat = (client_data.getFlags() & DataStreamer::HEXFLOAT_FLAG) != 0;
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
    server_data.setStatus(0);
    std::string contents;
    fileGetContents(statdist_file, contents);
    server_data.setStatDist(contents);
    fileGetContents(probtraj_file, contents);
    server_data.setProbTraj(contents);
    fileGetContents(runlog_file, contents);
    server_data.setRunLog(contents);
    fileGetContents(fp_file, contents);
    server_data.setFP(contents);
    if (NULL != output_traj) {
      fileGetContents(traj_file, contents);
      server_data.setTraj(contents);
    }

    if (!quiet) {
      std::cerr << "\n" << server_data.getRunLog();
    }
    timebuf = ctime(&end_time);
    timebuf[strlen(timebuf)-1] = 0;
    if (!quiet) {
      std::cerr << hst << " " << prog << " simulation finished at " << timebuf << " " << hst << "\n";;
    }
  } catch(const BNException& e) {
    if (!quiet) {
      std::cerr << "\n" << hst << " " << prog << " simulation error [[\n" << e << "]] " << hst << "\n";
    }
    delete_temp_files(files_to_delete_v);
    server_data.setStatus(1);
    server_data.setErrorMessage(e.getMessage());
    return;
  }
  delete_temp_files(files_to_delete_v);
}

void Server::manageRequest(int fd, const char* request)
{
  ClientData client_data;
  ServerData server_data;
  std::string err_data;
  int status;

  if (verbose) {
    std::cout << "request [" << request << "]\n";
  }

  if ((status = DataStreamer::parseStreamData(client_data, request, err_data)) != 0) {
    server_data.setStatus(status);
    server_data.setErrorMessage(err_data);
    //rpc_writeStringData(fd, err_data.c_str(), err_data.length());
    //return;
  } else {
    if (client_data.getCommand() == DataStreamer::RUN_COMMAND || client_data.getCommand() == DataStreamer::CHECK_COMMAND) {
      run(client_data, server_data);
    } else {
      std::cerr << "unknown command \"" << client_data.getCommand() << "\"\n";
      return;
    }
  }

  std::string data;
  DataStreamer::buildStreamData(data, server_data);
  if (verbose) {
    std::cout << "response [" << data << "]\n";
  }
  rpc_writeStringData(fd, data.c_str(), data.length());
}
