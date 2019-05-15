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
     Function.h

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
   July 2018
*/

#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <vector>
#include <map>
#include <math.h>

class ArgumentList;
class Expression;
class Node;
class NetworkState;
class Function;

class Function {
  std::string funname;
  unsigned int min_args;
  unsigned int max_args;
  static std::map<std::string, Function*>* func_map;

protected:
  Function(const std::string& funname, unsigned int min_args, unsigned int max_args = ~0U);

public:
  const std::string& getFunName() const {return funname;}
  unsigned int getMinArgs() const {return min_args;}
  unsigned int getMaxArgs() const {return max_args;}

  static Function* getFunction(const std::string& funname);

  virtual bool isDeterministic() const {return true;}

  virtual std::string getDescription() const = 0;

  void check(ArgumentList* arg_list);

  virtual double eval(const Node* this_node, const NetworkState& network_state, ArgumentList* arg_list) = 0;

  static void displayFunctionDescriptions(std::ostream& os);
  static std::map<std::string, Function*>* getFuncMap() {return func_map;}
  static void setFuncMap(std::map<std::string, Function*>* _func_map) {func_map = _func_map;}
};

#endif
