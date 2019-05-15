
%{
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
     RunConfigGrammar.l

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     January-March 2011
     updated to add extended initial conditions: October 2014
*/

#include "BooleanNetwork.h"
#include "RunConfig.h"

extern int yylex();

static void yyerror(const char *s);
static Network* network;

extern std::string yy_error_head();
%}

%union {
  Expression* expr;
  char* str;
  double d;
  long long l;
  std::vector<const Node*>* node_list;
  std::vector<Expression*>* expr_list;
  IStateGroup::ProbaIState* istate_expr;
  std::vector<IStateGroup::ProbaIState*>* istate_expr_list;
}

%type<expr> primary_expression 
%type<expr> postfix_expression 
%type<expr> unary_expression 
%type<expr> multiplicative_expression 
%type<expr> additive_expression 
%type<expr> relational_expression 
%type<expr> equality_expression
%type<expr> logical_and_expression
%type<expr> logical_or_expression
%type<expr> logical_xor_expression
%type<expr> conditional_expression
%type<expr> expression
%type<node_list> symbol_list
%type<node_list> symbol_istate_list
%type<expr_list> expression_list
%type<istate_expr> istate_expression
%type<istate_expr_list> istate_expression_list

%token<str> VARIABLE
%token<str> SYMBOL
%token<d> DOUBLE
%token<l> INTEGER

%token LOGAND LOGOR LOGXOR LOGNOT EQUAL NOT_EQUAL NODE GTEQ LTEQ

%%

translation_unit: decl
{
}
| translation_unit decl
{
}
;

decl: var_decl
| runconfig_decl
| node_attr_decl
| ';'
;

runconfig_decl: SYMBOL '=' expression ';'
{
  NetworkState network_state;
  double value = $3->eval(NULL, network_state);
  RunConfig::getInstance()->setParameter($1, value);
}
;

node_attr_decl: SYMBOL '.' SYMBOL '=' expression ';'
{
  Node* node = network->getNode($1);
  NetworkState network_state;
  double value = $5->eval(NULL, network_state);
  if (!strcasecmp($3, "istate")) {
    if (backward_istate) {
      node->setIState((bool)value);
    } else {
      if (value < 0) {
	new IStateGroup(node);
      } else {
	new IStateGroup(node, $5);
      }
    }
  } else if (!strcasecmp($3, "is_internal")) {
    node->isInternal((bool)value);
  } else if (!strcasecmp($3, "refstate")) {
    if (value < 0) {
      node->setReference(false);
    } else {
      node->setReferenceState((NodeState)value);
    }
  } else {
    throw BNException(std::string(yy_error_head() + "invalid node attribute: ") + $3 + ", valid attributes are: istate or is_internal");
  }
}
| symbol_istate_list '.' SYMBOL '=' istate_expression_list ';'
{
  if (strcasecmp($3, "istate")) {
    throw BNException(std::string(yy_error_head() + "invalid node group attribute: ") + $3 + ", valid attribute is istate");
  }
  std::string error_msg;
  new IStateGroup($1, $5, error_msg);
  if (error_msg.length() > 0) {
    throw BNException(std::string(yy_error_head() + error_msg));
  }
}
;

symbol_istate_list: '[' symbol_list ']'
{
  $$ = $2;
}
;

symbol_list: SYMBOL
{
  $$ = new std::vector<const Node*>();
  $$->push_back(network->getNode($1));
}
| symbol_list ',' SYMBOL
{
  $$ = $1;
  $$->push_back(network->getNode($3));
}
;

istate_expression_list: istate_expression
{
  $$ = new std::vector<IStateGroup::ProbaIState*>();
  $$->push_back($1);
}
| istate_expression_list ',' istate_expression
{
  $$ = $1;
  $$->push_back($3);
}
;

istate_expression: primary_expression '[' expression_list ']'
{
  $$ = new IStateGroup::ProbaIState($1, $3);
}
;

expression_list: primary_expression
{
  $$ = new std::vector<Expression*>();
  $$->push_back($1);
}
| expression_list ',' primary_expression
{
  $$ = $1;
  $$->push_back($3);
}
;

var_decl: VARIABLE '=' expression ';'
{
  SymbolTable* symtab = SymbolTable::getInstance();
  const Symbol* symbol = symtab->getOrMakeSymbol($1);
  NetworkState dummy_state;
  symtab->setSymbolValue(symbol, $3->eval(NULL, dummy_state));
}
;

primary_expression: INTEGER
{
  $$ = new ConstantExpression($1);
}
| DOUBLE
{
  $$ = new ConstantExpression($1);
}
| VARIABLE
{
  $$ = new SymbolExpression(SymbolTable::getInstance()->getOrMakeSymbol($1));
}
| '(' expression ')'
{
  $$ = new ParenthesisExpression($2);
}
;

postfix_expression: primary_expression
{
  $$ = $1;
}
;

unary_expression: postfix_expression
{
  $$ = $1;
}
| '+' unary_expression
{
  $$ = $2;
}
| '-' unary_expression
{
  $$ = new SubExpression(new ConstantExpression(0.0), $2);
}
| '!' unary_expression
{
  $$ = new NotLogicalExpression($2);
}
| LOGNOT unary_expression
{
  $$ = new NotLogicalExpression($2);
}
;

multiplicative_expression: unary_expression
{
  $$ = $1;
}
| multiplicative_expression '*' unary_expression
{
  $$ = new MulExpression($1, $3);
}
| multiplicative_expression '/' unary_expression
{
  $$ = new DivExpression($1, $3);
}
;

additive_expression: multiplicative_expression
{
  $$ = $1;
}
| additive_expression '+' multiplicative_expression
{
  $$ = new AddExpression($1, $3);
}
| additive_expression '-' multiplicative_expression
{
  $$ = new SubExpression($1, $3);
}
;

relational_expression: additive_expression
{
  $$ = $1;
}
| relational_expression '<' additive_expression
{
  $$ = new LetterExpression($1, $3);
}
| relational_expression '>' additive_expression
{
  $$ = new GreaterExpression($1, $3);
}
| relational_expression LTEQ additive_expression
{
  $$ = new LetterOrEqualExpression($1, $3);
}
| relational_expression GTEQ additive_expression
{
  $$ = new GreaterOrEqualExpression($1, $3);
}
;

equality_expression: relational_expression
{
  $$ = $1;
}
| equality_expression EQUAL relational_expression
{
  $$ = new EqualExpression($1, $3);
}
| equality_expression NOT_EQUAL relational_expression
{
  $$ = new NotEqualExpression($1, $3);
}
;

logical_and_expression: equality_expression
{
  $$ = $1;
}
| logical_and_expression LOGAND equality_expression
{
  $$ = new AndLogicalExpression($1, $3);
}
;

logical_or_expression: logical_and_expression
{
  $$ = $1;
}
| logical_or_expression LOGOR logical_and_expression
{
  $$ = new OrLogicalExpression($1, $3);
}
;

logical_xor_expression: logical_or_expression
{
  $$ = $1;
}
| logical_xor_expression LOGXOR logical_or_expression
{
  $$ = new XorLogicalExpression($1, $3);
}
| logical_xor_expression '^' logical_or_expression
{
  $$ = new XorLogicalExpression($1, $3);
}
;

conditional_expression: logical_xor_expression
{
  $$ = $1;
}
| logical_xor_expression '?' expression ':' conditional_expression
{
  $$ = new CondExpression($1, $3, $5);
}
;

expression: conditional_expression
{
  $$ = $1;
}
;

%%

void runconfig_setNetwork(Network* _network)
{
  network = _network;
}

#include "lex.RC.cc"
