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
     RPC.h

   Authors:
     Eric Viara <viara@sysra.com>
 
   Date:
     May 2018
*/

#ifndef _RPC_H_
#define _RPC_H_

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
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include <string>

#define rpc_Success 0
#define rpc_Error 1

#define rpc_isSocketValid(S) ((S) >= 0)

struct rpc_PortHandle {
  int domain, type;
  char *portname;
  union {
    struct {
      int sockin_fd;
      struct sockaddr_in sock_in_name;
    } in;
    struct {
      int sockun_fd;
      struct sockaddr_un sock_un_name;
    } un;
  } u;
};


class rpc_Server {

protected:
  rpc_PortHandle* port_h;
  std::string host;
  std::string port;

  rpc_Server(const std::string& host, const std::string& port) : host(host), port(port) {}
  int bind(const char** p_rpc_portname = NULL);
  virtual int listen();
  virtual void manageRequest(int fd, const char* request) = 0;

  virtual ~rpc_Server() {
  }
};

class rpc_Client {

protected:
  int sock_fd;
  std::string host;
  std::string port;

  rpc_Client(const std::string& host, const std::string& port) : host(host), port(port) {}
  int open();
  int close();
};

extern char* rpc_readStringData(int fd);
extern ssize_t rpc_writeStringData(int fd, const char* data, size_t len);

#endif
