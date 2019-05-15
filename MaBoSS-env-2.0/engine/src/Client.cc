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
     Client.cc

   Authors:
     Eric Viara <viara@sysra.com>
 
   Date:
     May 2018
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <resolv.h>
#include <netdb.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Client.h"
#include "Server.h"
#include "DataStreamer.h"
#include "RPC.h"

void Client::send(const ClientData& client_data, ServerData& server_data)
{
  if (open() == rpc_Success) {
    std::string data;
    // 2018-11-08: warning: command and comm_mode not yet implemented
    DataStreamer::buildStreamData(data, client_data);
    if (verbose) {
      std::cout << "client sending request [" << data << "]\n";
    }

    rpc_writeStringData(sock_fd, data.c_str(), data.length());
    
    char* response = rpc_readStringData(sock_fd);
    if (response != NULL) {
      if (verbose) {
	std::cout << "client received [" << response << "]\n";
      }
      DataStreamer::parseStreamData(server_data, response);
      free(response);
    }

    close();
  }
  // else throw error
}

