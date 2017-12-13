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
     Probe.h

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     January-March 2011
*/

#ifndef _PROBE_H_
#define _PROBE_H_

#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>

class Probe {

  struct tms tms0, tms1;
  struct timeval tv0, tv1;
  double ticks_per_second;

 public:
  Probe(bool _start = true) {
    ticks_per_second = (double)sysconf(_SC_CLK_TCK);
    if (_start) {
      start();
    }
  }

  void start() {
    gettimeofday(&tv0, NULL);
    times(&tms0);
  }

  void stop() {
    gettimeofday(&tv1, NULL);
    times(&tms1);
  }

  long long elapsed_usecs() const {
    return Probe::usecs(tv0, tv1);
  }

  long long elapsed_msecs() const {
    return Probe::msecs(tv0, tv1);
  }

  long long user_msecs() const {
    return ((tms1.tms_utime - tms0.tms_utime)/ticks_per_second)*1000;
  }

  long long sys_msecs() const {
    return ((tms1.tms_stime - tms0.tms_stime)/ticks_per_second)*1000;
  }

  static long long usecs(const timeval& tv0, const timeval& tv1) {
    return (tv1.tv_sec - tv0.tv_sec) * 1000000 + (tv1.tv_usec - tv0.tv_usec);
  }

  static long long msecs(const timeval& tv0, const timeval& tv1) {
    return usecs(tv0, tv1)/1000;
  }
};

#endif

