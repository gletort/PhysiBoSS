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
     Utils.h

   Authors:
     Eric Viara <viara@sysra.com>
 
   Date:
     May 2018
*/

#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>
#include <assert.h>

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

extern int checkArgMissing(const char* prog, const char* opt, int nn, int argc);
extern int fileGetContents(const std::string& file, std::string& contents);
extern int filePutContents(const std::string& file, const std::string& data);
extern std::string stringReplaceAll(const std::string& subject, const std::string& from, const std::string& to);

extern const std::string NL_PATTERN;
extern const char* fmthexdouble(double d);

#endif
