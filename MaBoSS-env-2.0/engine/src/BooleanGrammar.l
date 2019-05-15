
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
     BooleanGrammar.l

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     January-March 2011
*/

#include <math.h>

static char *yytokstr();
static unsigned int input_lineno = 1;
static void skip_comment(void);
%}

fracconst	([0-9]+\.[0-9]+)|([0-9]+\.)|(\.[0-9]+)
exppart		[eE](\-|\+)?[0-9]+

%%

[\t\f\v\r ]+ { }
"\n" { input_lineno++; }
"/*" { skip_comment(); }
"//".* { }
[a-zA-Z_][a-zA-Z_0-9]* {
  if (!strcasecmp(yytext, "NOT")) {
     return LOGNOT;
  }
  if (!strcasecmp(yytext, "AND")) {
     return LOGAND;
  }
  if (!strcasecmp(yytext, "OR")) {
     return LOGOR;
  }
  if (!strcasecmp(yytext, "XOR")) {
     return LOGXOR;
  }
  if (!strcasecmp(yytext, "NODE")) {
     return NODE;
  }
  yylval.str = strdup(yytext);
  return IDENTIFIER;
}
"$"[a-zA-Z_][a-zA-Z_0-9]* {
  yylval.str = strdup(yytext);
  return VARIABLE;
}
\" {
  yylval.str = yytokstr();
  return STRING;
}
"=="			{ return EQUAL; }
"!="			{ return NOT_EQUAL; }
"<="			{ return LTEQ; }
">="			{ return GTEQ; }
"&&"|"&"		{ return LOGAND; }
"||"|"|"		{ return LOGOR; }
"!"			{ return LOGNOT; }
[0-9]+			{ sscanf(yytext, "%lld", &yylval.l); return INTEGER; }
{fracconst}{exppart}?	{ yylval.d = atof(yytext); return DOUBLE; }
[0-9]+{exppart}		{ yylval.d = atof(yytext); return DOUBLE; }
.			{ return yytext[0]; }

%%

#define _ESC_(X, Y) case X: *p++ = Y; break

static void yypurgestring(unsigned char *s)
{
  unsigned char c;
  unsigned char *p = s;
  while ((c = *s) != 0) {
    if (c == '\\') {
       c = *++s;
       switch(c) {
         _ESC_('a', '\a');
	 _ESC_('b', '\b');
	 _ESC_('f', '\f');
	 _ESC_('n', '\n');
	 _ESC_('r', '\r');
	 _ESC_('t', '\t');
	 _ESC_('v', '\v');
	 _ESC_('\'', '\'');
	 _ESC_('\"', '"');
	 _ESC_('\\', '\\');

	 default:
	   *p++ = '\\';
	   *p++ = c;
        }
       s++;
      }
      else
        *p++ = *s++;
    }

  *p = 0;
}

static char *yytokstr()
{
   int s_size;
   unsigned char *s, c;
   s_size = 32;
   s = (unsigned char *)malloc(s_size);

   int n, backslash;
   for (n = 0, backslash = 0; (c = yyinput()) != 0 && c != 255; n++) {
       if (c == '\n') {
         input_lineno++;
       }
       else if (c == '\\') {
         backslash = !backslash;
       }
       else if (c == '"' && !backslash) {
	 break;
       }
       else {
         backslash = 0;
       }

       if (n >= s_size) {
           s_size *= 2;
           s = (unsigned char *)realloc(s, s_size);
       }

       s[n] = c;
     }

   if (n >= s_size) {
       s_size += 4;
       s = (unsigned char *)realloc(s, s_size);
   }
   
   s[n] = 0;
   yypurgestring(s);
   return (char *)s;
}

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

static void yyerror(const char *)
{
  char tok[32];
  sprintf(tok, "%u", input_lineno);
  throw BNException("BND syntax error at line #" + std::string(tok));
}

// to avoid compiler warning when using -Wall option
void boolean_grammar_to_make_compiler_happy()
{
  unput('x');
}
