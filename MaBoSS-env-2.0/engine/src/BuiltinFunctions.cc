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
     BuiltinFunctions.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     July 2018
*/

#include "BooleanNetwork.h"

//
// User function definition
//

class LogFunction : public Function {

public:
  LogFunction() : Function("log", 1, 2) { }

  double eval(const Node* this_node, const NetworkState& network_state, ArgumentList* arg_list) {
    const std::vector<Expression*>& expr_v = arg_list->getExpressionList();
    std::vector<Expression*>::const_iterator iter = expr_v.begin();
    double val = (*iter)->eval(this_node, network_state);
    if (expr_v.size() == 1) {
      return log(val);
    }

    iter++;
    double base = (*iter)->eval(this_node, network_state);
    return log(val) / log(base);
  }

  std::string getDescription() const {
    return "double log(double VALUE[, double BASE=e])\n  computes the value of the natural logarithm of VALUE; uses BASE if set";
  }
};

class ExpFunction : public Function {

public:
  ExpFunction() : Function("exp", 1, 2) { }

  double eval(const Node* this_node, const NetworkState& network_state, ArgumentList* arg_list) {
    const std::vector<Expression*>& expr_v = arg_list->getExpressionList();
    std::vector<Expression*>::const_iterator iter = expr_v.begin();
    double val = (*iter)->eval(this_node, network_state);
    if (expr_v.size() == 1) {
      return exp(val);
    }

    iter++;
    double base = (*iter)->eval(this_node, network_state);
    return exp(val * log(base));
  }

  std::string getDescription() const {
    return "double exp(double VALUE[, double BASE=e])\n  computes the base-e exponential of VALUE; uses BASE if set";
  }
};

void builtin_functions_init()
{
  static int init = false;
  if (!init) {
    new LogFunction();
    new ExpFunction();
    init = true;
  }
}

