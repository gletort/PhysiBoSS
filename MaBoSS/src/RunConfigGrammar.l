
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
*/

#include <math.h>

//static char *yytokstr();
static unsigned int input_lineno = 1;
static const char* file;
static const char* expr;
static void skip_comment(void);
%}

fracconst	([0-9]+\.[0-9]+)|([0-9]+\.)|(\.[0-9]+)
exppart		[eE](\-|\+)?[0-9]+

%%

[\t\f\v\r ]+ { }
"\n" { input_lineno++; }
"/*" { skip_comment(); }
"//".* { }
[Tt][Rr][Uu][Ee]        { yylval.l = 1; return INTEGER; }
[Ff][Aa][Ll][Ss][Ee]    { yylval.l = 0; return INTEGER; }
"$"[a-zA-Z_][a-zA-Z_0-9]* {
  yylval.str = strdup(yytext);
  return VARIABLE;
}
[a-zA-Z_][a-zA-Z_0-9]* {
  yylval.str = strdup(yytext);
  return SYMBOL;
}
[0-9]+			{ sscanf(yytext, "%lld", &yylval.l); return INTEGER; }
{fracconst}{exppart}?	{ yylval.d = atof(yytext); return DOUBLE; }
[0-9]+{exppart}		{ yylval.d = atof(yytext); return DOUBLE; }
.			{ return yytext[0]; }

%%

static void skip_comment(void)
{
  int c1, c2;

  c1 = yyinput();
  c2 = yyinput();

  while (c2 != EOF && !(c1 == '*' && c2 == '/')) {
    if (c1 == '\n') {
      ++input_lineno;
    }
    c1 = c2;
    c2 = yyinput();
  }
}

int yywrap(void)
{
  return 1;
}

void yy_set_file(const char* _file)
{
  input_lineno = 1;
  file = _file;
  expr = NULL;
}

void yy_set_expr(const char* _expr)
{
  input_lineno = 1;
  file = NULL;
  expr = _expr;
}

static void yyerror(const char *)
{
  char tok[32];
  sprintf(tok, "%u", input_lineno);
  if (file) {
    throw BNException("configuration syntax error at line #" + std::string(tok) + " in file \"" + file + "\"");
  } else {
    throw BNException("configuration syntax error in expression '" + std::string(expr) + "'");
  }
}

std::string yy_error_head()
 {
   std::ostringstream ostr;
   ostr << "configuration parsing at line #" << input_lineno << ": ";
   return ostr.str();
 }

// to avoid compiler warning when using -Wall option
void runconfig_to_make_compiler_happy()
{
  unput('x');
}
