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
     Client.h
     
     Authors:
     Eric Viara <viara@sysra.com>
     
     Date:
     May 2018
*/

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>
#include <vector>

#include "RPC.h"

class ServerData;

class ClientData {
  std::string protocol_version;
  unsigned long long flags;
  std::string command;

  std::string network;
  std::vector<std::string> config_v;
  std::vector<std::string> config_expr_v;
  std::string config_vars;

public:
  void setProtocolVersion(const std::string& protocol_version) {
    this->protocol_version = protocol_version;
  }

  void setFlags(unsigned long long flags) {
    this->flags = flags;
  }

  void setCommand(const std::string& command) {
    this->command = command;
  }

  void setNetwork(const std::string& network) {
    this->network = network;
  }

  void addConfig(const std::string& config) {
    config_v.push_back(config);
  }

  void addConfigExpr(const std::string& config_expr) {
    config_expr_v.push_back(config_expr);
  }

  void setConfigVars(const std::string& config_vars) {
    this->config_vars = config_vars;
  }

  const std::string& getProtocolVersion() const {
    return protocol_version;
  }

  const unsigned long long getFlags() const {
    return flags;
  }

  const std::string& getCommand() const {
    return command;
  }

  const std::string& getNetwork() const {
    return network;
  }

  const std::vector<std::string>& getConfigs() const {
    return config_v;
  }

  const std::string& getConfigVars() const {
    return config_vars;
  }

  const std::vector<std::string>& getConfigExprs() const {
    return config_expr_v;
  }
};

class Client : public rpc_Client {
  std::string host;
  std::string port;
  bool verbose;

public:
  Client(const std::string& host, const std::string& port, bool verbose = false) : rpc_Client(host, port), verbose(verbose) { }

  void send(const ClientData& client_data, ServerData& server_data);
};

#endif
