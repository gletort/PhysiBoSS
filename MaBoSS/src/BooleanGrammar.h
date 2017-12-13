
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
     BooleanGrammar.h

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     January-March 2011
*/

#ifndef _BOOLEANGRAMMAR_H_
#define _BOOLEANGRAMMAR_H_

#include <vector>
#include "BooleanNetwork.h"

extern void set_current_network(Network* network);
extern Network* get_current_network();

class NodeDeclItem {

  std::string identifier;
  Expression* expr;
  std::string str;

public:
  NodeDeclItem(const std::string& identifier, Expression* expr) : identifier(identifier), expr(expr) { }
  NodeDeclItem(const std::string& identifier, const std::string& str) : identifier(identifier), expr(NULL), str(str) { }

  const std::string& getIdentifier() const {return identifier;}
  Expression* getExpression() {return expr;}
  const std::string& getString() const {return str;}
};

class NodeDecl {

  // node definition should not be here (in the grammar) because it is also used by the API (even when grammar is not used at all)
  static MAP<std::string, bool> node_def_map;

public:
  NodeDecl(const std::string& identifier, std::vector<NodeDeclItem*>* node_decl_item_list) {
    if (isNodeDefined(identifier)) {
      throw BNException("node " + identifier + " already defined");
    }

    defineNode(identifier);

    Node* node = get_current_network()->getOrMakeNode(identifier);
    if (NULL == node_decl_item_list) {
      return;
    }
    size_t size = node_decl_item_list->size();
    for (size_t nn = 0; nn < size; nn++) {
      Expression* expr = (*node_decl_item_list)[nn]->getExpression();
      if (NULL != expr) {
	node->setAttributeExpression((*node_decl_item_list)[nn]->getIdentifier(), expr);
      } else {
	node->setAttributeString((*node_decl_item_list)[nn]->getIdentifier(), (*node_decl_item_list)[nn]->getString());
      }
    }
  }

  static bool isNodeDefined(const std::string& identifier) {
    return node_def_map.find(identifier) != node_def_map.end();
  }

  static void defineNode(const std::string& identifier) {
    node_def_map[identifier] = true;
  }

  static void reset() {
    node_def_map.clear();
  }
};

#endif
