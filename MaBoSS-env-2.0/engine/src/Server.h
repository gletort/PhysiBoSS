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
     Server.h
     
     Authors:
     Eric Viara <viara@sysra.com>
     
     Date:
     May 2018
*/

#ifndef _SERVER_H_
#define _SERVER_H_

#include <string>
#include "RPC.h"
#include "Utils.h"

class ClientData;

class ServerData {
  int status;
  std::string error_msg;
  std::string statdist;
  std::string probtraj;
  std::string traj;
  std::string fp;
  std::string run_log;

public:
  void setStatus(int status) {
    this->status = status;
  }

  void setErrorMessage(const std::string& error_msg) {
    this->error_msg = stringReplaceAll(error_msg, "\n", NL_PATTERN);
  }

  void setStatDist(const std::string& statdist) {
    this->statdist = statdist;
  }

  void setProbTraj(const std::string& probtraj) {
    this->probtraj = probtraj;
  }

  void setTraj(const std::string& traj) {
    this->traj = traj;
  }

  void setFP(const std::string& fp) {
    this->fp = fp;
  }

  void setRunLog(const std::string& run_log) {
    this->run_log = run_log;
  }

  int getStatus() const {
    return status;
  }

  const std::string getErrorMessage() const {
    return stringReplaceAll(error_msg, NL_PATTERN, "\n");
  }

  const std::string& getErrorMessageRaw() const {
    return error_msg;
  }

  const std::string& getStatDist() const {
    return statdist;
  }

  const std::string& getProbTraj() const {
    return probtraj;
  }

  const std::string& getTraj() const {
    return traj;
  }

  const std::string& getFP() const {
    return fp;
  }

  const std::string& getRunLog() const {
    return run_log;
  }
};

class Server : public rpc_Server {
  std::string prog;
  std::string pidfile;
  bool quiet;
  bool verbose;
  static Server* server;

  Server(const std::string& host, const std::string& port, const std::string& prog, const std::string& pidfile, bool quiet = false, bool verbose = false) : rpc_Server(host, port), prog(prog), pidfile(pidfile), quiet(verbose), verbose(verbose) { }

  void run(const ClientData& client_data, ServerData& server_data);

public:
  static Server* getServer(const std::string& host, const std::string& port, const std::string& prog, const std::string& pidfile = "", bool quiet = false, bool verbose = false) {
    if (NULL == server) {
      server = new Server(host, port, prog, pidfile, quiet, verbose);
    }
    return server;
  }

  static Server* getInstance() {
    return server;
  }

  void setQuiet(bool quiet) {this->quiet = quiet;}
  void setVerbose(bool verbose) {this->verbose = verbose;}

  const std::string& getPidFile() const {return pidfile;}

  int manageRequests();
  void manageRequest(int fd, const char* request);
};

#endif
