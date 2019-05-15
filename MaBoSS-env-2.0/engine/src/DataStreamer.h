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
     DataStreamer.h
     
     Authors:
     Eric Viara <viara@sysra.com>
     
     Date:
     May 2018
*/

#ifndef _DATASTREAMER_H_
#define _DATASTREAMER_H_

#include <string>
#include <vector>
#include "Client.h"
#include "Server.h"

class DataStreamer {
public:
  static const std::string MABOSS_MAGIC;
  static const std::string PROTOCOL_VERSION;
  static const std::string PROTOCOL_VERSION_NUMBER;
  static const std::string FLAGS;
  static const unsigned long long HEXFLOAT_FLAG;
  static const unsigned long long OVERRIDE_FLAG;
  static const unsigned long long AUGMENT_FLAG;
  static const std::string COMMAND;
  static const std::string RUN_COMMAND;
  static const std::string CHECK_COMMAND;
  static const std::string PARSE_COMMAND;
  static const std::string RETURN;
  static const std::string STATUS;
  static const std::string ERROR_MESSAGE;
  static const std::string NETWORK;
  static const std::string CONFIGURATION;
  static const std::string CONFIGURATION_EXPRESSIONS;
  static const std::string CONFIGURATION_VARIABLES;
  static const std::string STATIONARY_DISTRIBUTION;
  static const std::string TRAJECTORY_PROBABILITY;
  static const std::string TRAJECTORIES;
  static const std::string FIXED_POINTS;
  static const std::string RUN_LOG;
  static std::string error(int status, const std::string& errmsg);

private:
  class HeaderItem {
    std::string directive;
    size_t from;
    size_t to;
    std::string value;

  public:
    HeaderItem(const std::string& directive, size_t from, size_t to) : directive(directive), from(from), to(to) { }
    HeaderItem(const std::string& directive, const std::string& value) : directive(directive), from(0), to(0), value(value) { }
    const std::string& getDirective() const {return directive;}
    size_t getFrom() const {return from;}
    size_t getTo() const {return to;}
    const std::string& getValue() const {return value;}
  };

  static int parse_header_items(const std::string &header, std::vector<HeaderItem>& header_item_v, std::string& err_data);

public:
  static void buildStreamData(std::string& data, const ClientData& client_data);
  static void buildStreamData(std::string &data, const ServerData& server_data);
  static int parseStreamData(ClientData& client_data, const std::string& data, std::string& err_data);
  static int parseStreamData(ServerData& server_data, const std::string& data);
};

#endif
