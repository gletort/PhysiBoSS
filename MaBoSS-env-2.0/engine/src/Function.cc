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
     Function.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     July 2018
*/

#include "BooleanNetwork.h"
#include <iostream>

std::map<std::string, Function*>* Function::func_map;

Function::Function(const std::string& funname, unsigned int min_args, unsigned int max_args) : funname(funname), min_args(min_args), max_args(max_args == ~0U ? min_args : max_args)
{
  if (NULL == func_map) {
    func_map = new std::map<std::string, Function*>();
  }
  (*func_map)[funname] = this;
}

void Function::check(ArgumentList* arg_list)
{
  size_t count = arg_list->getExpressionListCount();
  if (count < min_args) {
    throw BNException("not enough parameters in " + funname + " call");
  }
  if (count > max_args) {
    throw BNException("too many parameters in " + funname + " call");
  }
}

Function* Function::getFunction(const std::string& funname)
{
  if (func_map == NULL) {return NULL;}
  std::map<std::string, Function*>::iterator iter = func_map->find(funname);
  if (iter == func_map->end()) {
    return NULL;
  }
  return iter->second;
}

void Function::displayFunctionDescriptions(std::ostream& os)
{
  if (func_map == NULL) {return;}
  for (std::map<std::string, Function*>::iterator iter = func_map->begin(); iter != func_map->end(); ++iter) {
    os << "  " << iter->second->getDescription() << "\n\n";
  }
}
