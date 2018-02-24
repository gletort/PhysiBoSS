/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with CTBNDL or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 2 "BooleanGrammar.y" /* yacc.c:339  */

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
     BooleanGrammar.y

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     January-March 2011
*/

#include "BooleanGrammar.h"

extern int CTBNDLlex();
static void CTBNDLerror(const char *s);
MAP<std::string, bool> NodeDecl::node_def_map;
static Network* current_network;

#line 106 "BooleanGrammar.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int CTBNDLdebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum CTBNDLtokentype
  {
    IDENTIFIER = 258,
    VARIABLE = 259,
    STRING = 260,
    DOUBLE = 261,
    INTEGER = 262,
    LOGAND = 263,
    LOGOR = 264,
    LOGXOR = 265,
    LOGNOT = 266,
    EQUAL = 267,
    NOT_EQUAL = 268,
    NODE = 269,
    GTEQ = 270,
    LTEQ = 271
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 42 "BooleanGrammar.y" /* yacc.c:355  */

  std::vector<NodeDecl*>* node_decl_list;
  NodeDecl* node_decl;
  std::vector<NodeDeclItem*>* node_decl_item_list;
  NodeDeclItem* node_decl_item;
  Expression* expr;
  char* str;
  double d;
  long long l;

#line 171 "BooleanGrammar.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE CTBNDLlval;

int CTBNDLparse (void);



/* Copy the second part of user declarations.  */

#line 188 "BooleanGrammar.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 CTBNDLtype_uint8;
#else
typedef unsigned char CTBNDLtype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 CTBNDLtype_int8;
#else
typedef signed char CTBNDLtype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 CTBNDLtype_uint16;
#else
typedef unsigned short int CTBNDLtype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 CTBNDLtype_int16;
#else
typedef short int CTBNDLtype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about CTBNDLlval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined CTBNDLoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined CTBNDLoverflow || YYERROR_VERBOSE */


#if (! defined CTBNDLoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union CTBNDLalloc
{
  CTBNDLtype_int16 CTBNDLss_alloc;
  YYSTYPE CTBNDLvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union CTBNDLalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (CTBNDLtype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T CTBNDLnewbytes;                                            \
        YYCOPY (&CTBNDLptr->Stack_alloc, Stack, CTBNDLsize);                    \
        Stack = &CTBNDLptr->Stack_alloc;                                    \
        CTBNDLnewbytes = CTBNDLstacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        CTBNDLptr += CTBNDLnewbytes / sizeof (*CTBNDLptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T CTBNDLi;                         \
          for (CTBNDLi = 0; CTBNDLi < (Count); CTBNDLi++)   \
            (Dst)[CTBNDLi] = (Src)[CTBNDLi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   79

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  34
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  48
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  84

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by CTBNDLlex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   271

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? CTBNDLtranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by CTBNDLlex, without out-of-bounds checking.  */
static const CTBNDLtype_uint8 CTBNDLtranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    27,     2,     2,     2,     2,     2,     2,
      23,    24,    28,    25,     2,    26,     2,    29,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    19,    21,
      30,    20,    31,    33,    22,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    32,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    17,     2,    18,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const CTBNDLtype_uint16 CTBNDLrline[] =
{
       0,    79,    79,    84,    91,    96,   101,   111,   116,   123,
     128,   136,   142,   147,   152,   156,   160,   166,   196,   200,
     204,   208,   212,   218,   222,   226,   232,   236,   240,   246,
     250,   254,   258,   262,   268,   272,   276,   282,   286,   292,
     296,   302,   306,   310,   316,   320,   326,   332,   333
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const CTBNDLtname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "VARIABLE", "STRING",
  "DOUBLE", "INTEGER", "LOGAND", "LOGOR", "LOGXOR", "LOGNOT", "EQUAL",
  "NOT_EQUAL", "NODE", "GTEQ", "LTEQ", "'{'", "'}'", "':'", "'='", "';'",
  "'@'", "'('", "')'", "'+'", "'-'", "'!'", "'*'", "'/'", "'<'", "'>'",
  "'^'", "'?'", "$accept", "translation_unit", "node_decl",
  "node_decl_item_list", "node_decl_item", "primary_expression",
  "postfix_expression", "unary_expression", "multiplicative_expression",
  "additive_expression", "relational_expression", "equality_expression",
  "logical_and_expression", "logical_or_expression",
  "logical_xor_expression", "conditional_expression", "expression",
  "term_opt", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const CTBNDLtype_uint16 CTBNDLtoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   123,   125,    58,
      61,    59,    64,    40,    41,    43,    45,    33,    42,    47,
      60,    62,    94,    63
};
# endif

#define YYPACT_NINF -16

#define CTBNDLpact_value_is_default(Yystate) \
  (!!((Yystate) == (-16)))

#define YYTABLE_NINF -1

#define CTBNDLtable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const CTBNDLtype_int8 CTBNDLpact[] =
{
      20,   -11,    39,    16,   -16,    32,    36,   -16,   -16,   -16,
     -16,   -16,   -16,    32,    45,    32,    32,    32,    32,   -16,
     -16,   -16,   -14,    -5,    31,    28,    48,    58,    -8,   -16,
      47,     0,   -16,   -16,    46,   -16,   -16,   -16,    32,    32,
      32,    32,    32,    32,    32,    32,    32,    32,    32,    32,
      32,    32,    32,   -16,   -16,    49,   -16,     4,   -16,   -16,
     -16,   -16,   -14,   -14,    -5,    -5,    -5,    -5,    31,    31,
      28,    48,    58,    58,    52,     6,   -16,   -16,    32,    51,
      53,   -16,   -16,   -16
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const CTBNDLtype_uint8 CTBNDLdefact[] =
{
       0,     0,     0,     0,     2,     0,     0,     1,     3,    11,
      13,    15,    14,     0,     0,     0,     0,     0,     0,    17,
      18,    23,    26,    29,    34,    37,    39,    41,    44,    46,
      47,     0,    22,    12,     0,    19,    20,    21,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,     6,     0,     5,     0,     7,    16,
      24,    25,    27,    28,    33,    32,    30,    31,    35,    36,
      38,    40,    42,    43,     0,     0,     4,     8,     0,     0,
       0,    45,    10,     9
};

  /* YYPGOTO[NTERM-NUM].  */
static const CTBNDLtype_int8 CTBNDLpgoto[] =
{
     -16,   -16,    70,   -16,    18,   -16,   -16,   -12,    23,     7,
      -2,    29,    27,    15,   -16,     1,   -15,   -16
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const CTBNDLtype_int8 CTBNDLdefgoto[] =
{
      -1,     3,     4,    57,    58,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    54
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const CTBNDLtype_uint8 CTBNDLtable[] =
{
      34,    32,    50,    55,    35,    36,    37,    55,     5,     9,
      10,    79,    11,    12,    38,    39,     7,    13,    56,     1,
      40,    41,    76,     1,    51,    52,    60,    61,    14,    15,
       2,    16,    17,    18,     2,     9,    10,    74,    11,    12,
      46,    47,     6,    13,    68,    69,    42,    43,    33,    64,
      65,    66,    67,    31,    14,    15,    48,    16,    17,    18,
      80,    44,    45,    62,    63,    72,    73,    49,    53,    75,
      59,    78,    82,     8,    83,    77,    71,    70,     0,    81
};

static const CTBNDLtype_int8 CTBNDLcheck[] =
{
      15,    13,    10,     3,    16,    17,    18,     3,    19,     3,
       4,     5,     6,     7,    28,    29,     0,    11,    18,     3,
      25,    26,    18,     3,    32,    33,    38,    39,    22,    23,
      14,    25,    26,    27,    14,     3,     4,    52,     6,     7,
      12,    13,     3,    11,    46,    47,    15,    16,     3,    42,
      43,    44,    45,    17,    22,    23,     8,    25,    26,    27,
      75,    30,    31,    40,    41,    50,    51,     9,    21,    20,
      24,    19,    21,     3,    21,    57,    49,    48,    -1,    78
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const CTBNDLtype_uint8 CTBNDLstos[] =
{
       0,     3,    14,    35,    36,    19,     3,     0,    36,     3,
       4,     6,     7,    11,    22,    23,    25,    26,    27,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    17,    41,     3,    50,    41,    41,    41,    28,    29,
      25,    26,    15,    16,    30,    31,    12,    13,     8,     9,
      10,    32,    33,    21,    51,     3,    18,    37,    38,    24,
      41,    41,    42,    42,    43,    43,    43,    43,    44,    44,
      45,    46,    47,    47,    50,    20,    18,    38,    19,     5,
      50,    49,    21,    21
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const CTBNDLtype_uint8 CTBNDLr1[] =
{
       0,    34,    35,    35,    36,    36,    36,    37,    37,    38,
      38,    39,    39,    39,    39,    39,    39,    40,    41,    41,
      41,    41,    41,    42,    42,    42,    43,    43,    43,    44,
      44,    44,    44,    44,    45,    45,    45,    46,    46,    47,
      47,    48,    48,    48,    49,    49,    50,    51,    51
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const CTBNDLtype_uint8 CTBNDLr2[] =
{
       0,     2,     1,     2,     5,     4,     4,     1,     2,     4,
       4,     1,     2,     1,     1,     1,     3,     1,     1,     2,
       2,     2,     2,     1,     3,     3,     1,     3,     3,     1,
       3,     3,     3,     3,     1,     3,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     5,     1,     0,     1
};


#define CTBNDLerrok         (CTBNDLerrstatus = 0)
#define CTBNDLclearin       (CTBNDLchar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto CTBNDLacceptlab
#define YYABORT         goto CTBNDLabortlab
#define YYERROR         goto CTBNDLerrorlab


#define YYRECOVERING()  (!!CTBNDLerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (CTBNDLchar == YYEMPTY)                                        \
    {                                                           \
      CTBNDLchar = (Token);                                         \
      CTBNDLlval = (Value);                                         \
      YYPOPSTACK (CTBNDLlen);                                       \
      CTBNDLstate = *CTBNDLssp;                                         \
      goto CTBNDLbackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      CTBNDLerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (CTBNDLdebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (CTBNDLdebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      CTBNDL_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
CTBNDL_symbol_value_print (FILE *CTBNDLoutput, int CTBNDLtype, YYSTYPE const * const CTBNDLvaluep)
{
  FILE *CTBNDLo = CTBNDLoutput;
  YYUSE (CTBNDLo);
  if (!CTBNDLvaluep)
    return;
# ifdef YYPRINT
  if (CTBNDLtype < YYNTOKENS)
    YYPRINT (CTBNDLoutput, CTBNDLtoknum[CTBNDLtype], *CTBNDLvaluep);
# endif
  YYUSE (CTBNDLtype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
CTBNDL_symbol_print (FILE *CTBNDLoutput, int CTBNDLtype, YYSTYPE const * const CTBNDLvaluep)
{
  YYFPRINTF (CTBNDLoutput, "%s %s (",
             CTBNDLtype < YYNTOKENS ? "token" : "nterm", CTBNDLtname[CTBNDLtype]);

  CTBNDL_symbol_value_print (CTBNDLoutput, CTBNDLtype, CTBNDLvaluep);
  YYFPRINTF (CTBNDLoutput, ")");
}

/*------------------------------------------------------------------.
| CTBNDL_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
CTBNDL_stack_print (CTBNDLtype_int16 *CTBNDLbottom, CTBNDLtype_int16 *CTBNDLtop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; CTBNDLbottom <= CTBNDLtop; CTBNDLbottom++)
    {
      int CTBNDLbot = *CTBNDLbottom;
      YYFPRINTF (stderr, " %d", CTBNDLbot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (CTBNDLdebug)                                                  \
    CTBNDL_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
CTBNDL_reduce_print (CTBNDLtype_int16 *CTBNDLssp, YYSTYPE *CTBNDLvsp, int CTBNDLrule)
{
  unsigned long int CTBNDLlno = CTBNDLrline[CTBNDLrule];
  int CTBNDLnrhs = CTBNDLr2[CTBNDLrule];
  int CTBNDLi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             CTBNDLrule - 1, CTBNDLlno);
  /* The symbols being reduced.  */
  for (CTBNDLi = 0; CTBNDLi < CTBNDLnrhs; CTBNDLi++)
    {
      YYFPRINTF (stderr, "   $%d = ", CTBNDLi + 1);
      CTBNDL_symbol_print (stderr,
                       CTBNDLstos[CTBNDLssp[CTBNDLi + 1 - CTBNDLnrhs]],
                       &(CTBNDLvsp[(CTBNDLi + 1) - (CTBNDLnrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (CTBNDLdebug)                          \
    CTBNDL_reduce_print (CTBNDLssp, CTBNDLvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int CTBNDLdebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef CTBNDLstrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define CTBNDLstrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
CTBNDLstrlen (const char *CTBNDLstr)
{
  YYSIZE_T CTBNDLlen;
  for (CTBNDLlen = 0; CTBNDLstr[CTBNDLlen]; CTBNDLlen++)
    continue;
  return CTBNDLlen;
}
#  endif
# endif

# ifndef CTBNDLstpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define CTBNDLstpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
CTBNDLstpcpy (char *CTBNDLdest, const char *CTBNDLsrc)
{
  char *CTBNDLd = CTBNDLdest;
  const char *CTBNDLs = CTBNDLsrc;

  while ((*CTBNDLd++ = *CTBNDLs++) != '\0')
    continue;

  return CTBNDLd - 1;
}
#  endif
# endif

# ifndef CTBNDLtnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for CTBNDLerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from CTBNDLtname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
CTBNDLtnamerr (char *CTBNDLres, const char *CTBNDLstr)
{
  if (*CTBNDLstr == '"')
    {
      YYSIZE_T CTBNDLn = 0;
      char const *CTBNDLp = CTBNDLstr;

      for (;;)
        switch (*++CTBNDLp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++CTBNDLp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (CTBNDLres)
              CTBNDLres[CTBNDLn] = *CTBNDLp;
            CTBNDLn++;
            break;

          case '"':
            if (CTBNDLres)
              CTBNDLres[CTBNDLn] = '\0';
            return CTBNDLn;
          }
    do_not_strip_quotes: ;
    }

  if (! CTBNDLres)
    return CTBNDLstrlen (CTBNDLstr);

  return CTBNDLstpcpy (CTBNDLres, CTBNDLstr) - CTBNDLres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
CTBNDLsyntax_error (YYSIZE_T *CTBNDLmsg_alloc, char **CTBNDLmsg,
                CTBNDLtype_int16 *CTBNDLssp, int CTBNDLtoken)
{
  YYSIZE_T CTBNDLsize0 = CTBNDLtnamerr (YY_NULLPTR, CTBNDLtname[CTBNDLtoken]);
  YYSIZE_T CTBNDLsize = CTBNDLsize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *CTBNDLformat = YY_NULLPTR;
  /* Arguments of CTBNDLformat. */
  char const *CTBNDLarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int CTBNDLcount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in CTBNDLchar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated CTBNDLchar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (CTBNDLtoken != YYEMPTY)
    {
      int CTBNDLn = CTBNDLpact[*CTBNDLssp];
      CTBNDLarg[CTBNDLcount++] = CTBNDLtname[CTBNDLtoken];
      if (!CTBNDLpact_value_is_default (CTBNDLn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int CTBNDLxbegin = CTBNDLn < 0 ? -CTBNDLn : 0;
          /* Stay within bounds of both CTBNDLcheck and CTBNDLtname.  */
          int CTBNDLchecklim = YYLAST - CTBNDLn + 1;
          int CTBNDLxend = CTBNDLchecklim < YYNTOKENS ? CTBNDLchecklim : YYNTOKENS;
          int CTBNDLx;

          for (CTBNDLx = CTBNDLxbegin; CTBNDLx < CTBNDLxend; ++CTBNDLx)
            if (CTBNDLcheck[CTBNDLx + CTBNDLn] == CTBNDLx && CTBNDLx != YYTERROR
                && !CTBNDLtable_value_is_error (CTBNDLtable[CTBNDLx + CTBNDLn]))
              {
                if (CTBNDLcount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    CTBNDLcount = 1;
                    CTBNDLsize = CTBNDLsize0;
                    break;
                  }
                CTBNDLarg[CTBNDLcount++] = CTBNDLtname[CTBNDLx];
                {
                  YYSIZE_T CTBNDLsize1 = CTBNDLsize + CTBNDLtnamerr (YY_NULLPTR, CTBNDLtname[CTBNDLx]);
                  if (! (CTBNDLsize <= CTBNDLsize1
                         && CTBNDLsize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  CTBNDLsize = CTBNDLsize1;
                }
              }
        }
    }

  switch (CTBNDLcount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        CTBNDLformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T CTBNDLsize1 = CTBNDLsize + CTBNDLstrlen (CTBNDLformat);
    if (! (CTBNDLsize <= CTBNDLsize1 && CTBNDLsize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    CTBNDLsize = CTBNDLsize1;
  }

  if (*CTBNDLmsg_alloc < CTBNDLsize)
    {
      *CTBNDLmsg_alloc = 2 * CTBNDLsize;
      if (! (CTBNDLsize <= *CTBNDLmsg_alloc
             && *CTBNDLmsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *CTBNDLmsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *CTBNDLp = *CTBNDLmsg;
    int CTBNDLi = 0;
    while ((*CTBNDLp = *CTBNDLformat) != '\0')
      if (*CTBNDLp == '%' && CTBNDLformat[1] == 's' && CTBNDLi < CTBNDLcount)
        {
          CTBNDLp += CTBNDLtnamerr (CTBNDLp, CTBNDLarg[CTBNDLi++]);
          CTBNDLformat += 2;
        }
      else
        {
          CTBNDLp++;
          CTBNDLformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
CTBNDLdestruct (const char *CTBNDLmsg, int CTBNDLtype, YYSTYPE *CTBNDLvaluep)
{
  YYUSE (CTBNDLvaluep);
  if (!CTBNDLmsg)
    CTBNDLmsg = "Deleting";
  YY_SYMBOL_PRINT (CTBNDLmsg, CTBNDLtype, CTBNDLvaluep, CTBNDLlocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (CTBNDLtype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int CTBNDLchar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE CTBNDLlval;
/* Number of syntax errors so far.  */
int CTBNDLnerrs;


/*----------.
| CTBNDLparse.  |
`----------*/

int
CTBNDLparse (void)
{
    int CTBNDLstate;
    /* Number of tokens to shift before error messages enabled.  */
    int CTBNDLerrstatus;

    /* The stacks and their tools:
       'CTBNDLss': related to states.
       'CTBNDLvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow CTBNDLoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    CTBNDLtype_int16 CTBNDLssa[YYINITDEPTH];
    CTBNDLtype_int16 *CTBNDLss;
    CTBNDLtype_int16 *CTBNDLssp;

    /* The semantic value stack.  */
    YYSTYPE CTBNDLvsa[YYINITDEPTH];
    YYSTYPE *CTBNDLvs;
    YYSTYPE *CTBNDLvsp;

    YYSIZE_T CTBNDLstacksize;

  int CTBNDLn;
  int CTBNDLresult;
  /* Lookahead token as an internal (translated) token number.  */
  int CTBNDLtoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE CTBNDLval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char CTBNDLmsgbuf[128];
  char *CTBNDLmsg = CTBNDLmsgbuf;
  YYSIZE_T CTBNDLmsg_alloc = sizeof CTBNDLmsgbuf;
#endif

#define YYPOPSTACK(N)   (CTBNDLvsp -= (N), CTBNDLssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int CTBNDLlen = 0;

  CTBNDLssp = CTBNDLss = CTBNDLssa;
  CTBNDLvsp = CTBNDLvs = CTBNDLvsa;
  CTBNDLstacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  CTBNDLstate = 0;
  CTBNDLerrstatus = 0;
  CTBNDLnerrs = 0;
  CTBNDLchar = YYEMPTY; /* Cause a token to be read.  */
  goto CTBNDLsetstate;

/*------------------------------------------------------------.
| CTBNDLnewstate -- Push a new state, which is found in CTBNDLstate.  |
`------------------------------------------------------------*/
 CTBNDLnewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  CTBNDLssp++;

 CTBNDLsetstate:
  *CTBNDLssp = CTBNDLstate;

  if (CTBNDLss + CTBNDLstacksize - 1 <= CTBNDLssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T CTBNDLsize = CTBNDLssp - CTBNDLss + 1;

#ifdef CTBNDLoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *CTBNDLvs1 = CTBNDLvs;
        CTBNDLtype_int16 *CTBNDLss1 = CTBNDLss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if CTBNDLoverflow is a macro.  */
        CTBNDLoverflow (YY_("memory exhausted"),
                    &CTBNDLss1, CTBNDLsize * sizeof (*CTBNDLssp),
                    &CTBNDLvs1, CTBNDLsize * sizeof (*CTBNDLvsp),
                    &CTBNDLstacksize);

        CTBNDLss = CTBNDLss1;
        CTBNDLvs = CTBNDLvs1;
      }
#else /* no CTBNDLoverflow */
# ifndef YYSTACK_RELOCATE
      goto CTBNDLexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= CTBNDLstacksize)
        goto CTBNDLexhaustedlab;
      CTBNDLstacksize *= 2;
      if (YYMAXDEPTH < CTBNDLstacksize)
        CTBNDLstacksize = YYMAXDEPTH;

      {
        CTBNDLtype_int16 *CTBNDLss1 = CTBNDLss;
        union CTBNDLalloc *CTBNDLptr =
          (union CTBNDLalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (CTBNDLstacksize));
        if (! CTBNDLptr)
          goto CTBNDLexhaustedlab;
        YYSTACK_RELOCATE (CTBNDLss_alloc, CTBNDLss);
        YYSTACK_RELOCATE (CTBNDLvs_alloc, CTBNDLvs);
#  undef YYSTACK_RELOCATE
        if (CTBNDLss1 != CTBNDLssa)
          YYSTACK_FREE (CTBNDLss1);
      }
# endif
#endif /* no CTBNDLoverflow */

      CTBNDLssp = CTBNDLss + CTBNDLsize - 1;
      CTBNDLvsp = CTBNDLvs + CTBNDLsize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) CTBNDLstacksize));

      if (CTBNDLss + CTBNDLstacksize - 1 <= CTBNDLssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", CTBNDLstate));

  if (CTBNDLstate == YYFINAL)
    YYACCEPT;

  goto CTBNDLbackup;

/*-----------.
| CTBNDLbackup.  |
`-----------*/
CTBNDLbackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  CTBNDLn = CTBNDLpact[CTBNDLstate];
  if (CTBNDLpact_value_is_default (CTBNDLn))
    goto CTBNDLdefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (CTBNDLchar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      CTBNDLchar = CTBNDLlex ();
    }

  if (CTBNDLchar <= YYEOF)
    {
      CTBNDLchar = CTBNDLtoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      CTBNDLtoken = YYTRANSLATE (CTBNDLchar);
      YY_SYMBOL_PRINT ("Next token is", CTBNDLtoken, &CTBNDLlval, &CTBNDLlloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  CTBNDLn += CTBNDLtoken;
  if (CTBNDLn < 0 || YYLAST < CTBNDLn || CTBNDLcheck[CTBNDLn] != CTBNDLtoken)
    goto CTBNDLdefault;
  CTBNDLn = CTBNDLtable[CTBNDLn];
  if (CTBNDLn <= 0)
    {
      if (CTBNDLtable_value_is_error (CTBNDLn))
        goto CTBNDLerrlab;
      CTBNDLn = -CTBNDLn;
      goto CTBNDLreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (CTBNDLerrstatus)
    CTBNDLerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", CTBNDLtoken, &CTBNDLlval, &CTBNDLlloc);

  /* Discard the shifted token.  */
  CTBNDLchar = YYEMPTY;

  CTBNDLstate = CTBNDLn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++CTBNDLvsp = CTBNDLlval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto CTBNDLnewstate;


/*-----------------------------------------------------------.
| CTBNDLdefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
CTBNDLdefault:
  CTBNDLn = CTBNDLdefact[CTBNDLstate];
  if (CTBNDLn == 0)
    goto CTBNDLerrlab;
  goto CTBNDLreduce;


/*-----------------------------.
| CTBNDLreduce -- Do a reduction.  |
`-----------------------------*/
CTBNDLreduce:
  /* CTBNDLn is the number of a rule to reduce with.  */
  CTBNDLlen = CTBNDLr2[CTBNDLn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  CTBNDLval = CTBNDLvsp[1-CTBNDLlen];


  YY_REDUCE_PRINT (CTBNDLn);
  switch (CTBNDLn)
    {
        case 2:
#line 80 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.node_decl_list) = new std::vector<NodeDecl*>();
  (CTBNDLval.node_decl_list)->push_back((CTBNDLvsp[0].node_decl));
}
#line 1322 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 85 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLvsp[-1].node_decl_list)->push_back((CTBNDLvsp[0].node_decl));
  (CTBNDLval.node_decl_list) = (CTBNDLvsp[-1].node_decl_list);
}
#line 1331 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 92 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.node_decl) = new NodeDecl((CTBNDLvsp[-3].str), (CTBNDLvsp[-1].node_decl_item_list));
  free((CTBNDLvsp[-3].str));
}
#line 1340 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 97 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.node_decl) = new NodeDecl((CTBNDLvsp[-2].str), NULL);
  free((CTBNDLvsp[-2].str));
}
#line 1349 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 102 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  NodeDeclItem* decl_item = new NodeDeclItem("logic", (CTBNDLvsp[-1].expr));
  std::vector<NodeDeclItem*>* decl_item_v = new std::vector<NodeDeclItem*>();
  decl_item_v->push_back(decl_item);
  (CTBNDLval.node_decl) = new NodeDecl((CTBNDLvsp[-3].str), decl_item_v);
  free((CTBNDLvsp[-3].str));
}
#line 1361 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 112 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.node_decl_item_list) = new std::vector<NodeDeclItem*>();
  (CTBNDLval.node_decl_item_list)->push_back((CTBNDLvsp[0].node_decl_item));
}
#line 1370 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 117 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLvsp[-1].node_decl_item_list)->push_back((CTBNDLvsp[0].node_decl_item));
  (CTBNDLval.node_decl_item_list) = (CTBNDLvsp[-1].node_decl_item_list);
}
#line 1379 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 124 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.node_decl_item) = new NodeDeclItem((CTBNDLvsp[-3].str), (CTBNDLvsp[-1].expr));
  free((CTBNDLvsp[-3].str));
}
#line 1388 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 129 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.node_decl_item) = new NodeDeclItem((CTBNDLvsp[-3].str), (CTBNDLvsp[-1].str));
  free((CTBNDLvsp[-3].str));
  free((CTBNDLvsp[-1].str));
}
#line 1398 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 137 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  Node* node = current_network->getOrMakeNode((CTBNDLvsp[0].str));
  (CTBNDLval.expr) = new NodeExpression(node);
  free((CTBNDLvsp[0].str));
}
#line 1408 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 143 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new AliasExpression((CTBNDLvsp[0].str));
  free((CTBNDLvsp[0].str));
}
#line 1417 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 148 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new SymbolExpression(SymbolTable::getInstance()->getOrMakeSymbol((CTBNDLvsp[0].str)));
  free((CTBNDLvsp[0].str));
}
#line 1426 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 153 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new ConstantExpression((CTBNDLvsp[0].l));
}
#line 1434 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 157 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new ConstantExpression((CTBNDLvsp[0].d));
}
#line 1442 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 161 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new ParenthesisExpression((CTBNDLvsp[-1].expr));
}
#line 1450 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 167 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = (CTBNDLvsp[0].expr);
}
#line 1458 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 197 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = (CTBNDLvsp[0].expr);
}
#line 1466 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 201 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = (CTBNDLvsp[0].expr);
}
#line 1474 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 205 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new SubExpression(new ConstantExpression(0.0), (CTBNDLvsp[0].expr));
}
#line 1482 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 209 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new NotLogicalExpression((CTBNDLvsp[0].expr));
}
#line 1490 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 213 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new NotLogicalExpression((CTBNDLvsp[0].expr));
}
#line 1498 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 219 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = (CTBNDLvsp[0].expr);
}
#line 1506 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 223 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new MulExpression((CTBNDLvsp[-2].expr), (CTBNDLvsp[0].expr));
}
#line 1514 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 227 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new DivExpression((CTBNDLvsp[-2].expr), (CTBNDLvsp[0].expr));
}
#line 1522 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 233 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = (CTBNDLvsp[0].expr);
}
#line 1530 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 237 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new AddExpression((CTBNDLvsp[-2].expr), (CTBNDLvsp[0].expr));
}
#line 1538 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 241 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new SubExpression((CTBNDLvsp[-2].expr), (CTBNDLvsp[0].expr));
}
#line 1546 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 247 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = (CTBNDLvsp[0].expr);
}
#line 1554 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 251 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new LetterExpression((CTBNDLvsp[-2].expr), (CTBNDLvsp[0].expr));
}
#line 1562 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 255 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new GreaterExpression((CTBNDLvsp[-2].expr), (CTBNDLvsp[0].expr));
}
#line 1570 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 259 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new LetterOrEqualExpression((CTBNDLvsp[-2].expr), (CTBNDLvsp[0].expr));
}
#line 1578 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 263 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new GreaterOrEqualExpression((CTBNDLvsp[-2].expr), (CTBNDLvsp[0].expr));
}
#line 1586 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 269 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = (CTBNDLvsp[0].expr);
}
#line 1594 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 273 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new EqualExpression((CTBNDLvsp[-2].expr), (CTBNDLvsp[0].expr));
}
#line 1602 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 277 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new NotEqualExpression((CTBNDLvsp[-2].expr), (CTBNDLvsp[0].expr));
}
#line 1610 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 283 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = (CTBNDLvsp[0].expr);
}
#line 1618 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 287 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new AndLogicalExpression((CTBNDLvsp[-2].expr), (CTBNDLvsp[0].expr));
}
#line 1626 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 293 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = (CTBNDLvsp[0].expr);
}
#line 1634 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 297 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new OrLogicalExpression((CTBNDLvsp[-2].expr), (CTBNDLvsp[0].expr));
}
#line 1642 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 303 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = (CTBNDLvsp[0].expr);
}
#line 1650 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 307 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new XorLogicalExpression((CTBNDLvsp[-2].expr), (CTBNDLvsp[0].expr));
}
#line 1658 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 311 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new XorLogicalExpression((CTBNDLvsp[-2].expr), (CTBNDLvsp[0].expr));
}
#line 1666 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 317 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = (CTBNDLvsp[0].expr);
}
#line 1674 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 321 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = new CondExpression((CTBNDLvsp[-4].expr), (CTBNDLvsp[-2].expr), (CTBNDLvsp[0].expr));
}
#line 1682 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 327 "BooleanGrammar.y" /* yacc.c:1646  */
    {
  (CTBNDLval.expr) = (CTBNDLvsp[0].expr);
}
#line 1690 "BooleanGrammar.tab.c" /* yacc.c:1646  */
    break;


#line 1694 "BooleanGrammar.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter CTBNDLchar, and that requires
     that CTBNDLtoken be updated with the new translation.  We take the
     approach of translating immediately before every use of CTBNDLtoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering CTBNDLchar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", CTBNDLr1[CTBNDLn], &CTBNDLval, &CTBNDLloc);

  YYPOPSTACK (CTBNDLlen);
  CTBNDLlen = 0;
  YY_STACK_PRINT (CTBNDLss, CTBNDLssp);

  *++CTBNDLvsp = CTBNDLval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  CTBNDLn = CTBNDLr1[CTBNDLn];

  CTBNDLstate = CTBNDLpgoto[CTBNDLn - YYNTOKENS] + *CTBNDLssp;
  if (0 <= CTBNDLstate && CTBNDLstate <= YYLAST && CTBNDLcheck[CTBNDLstate] == *CTBNDLssp)
    CTBNDLstate = CTBNDLtable[CTBNDLstate];
  else
    CTBNDLstate = CTBNDLdefgoto[CTBNDLn - YYNTOKENS];

  goto CTBNDLnewstate;


/*--------------------------------------.
| CTBNDLerrlab -- here on detecting error.  |
`--------------------------------------*/
CTBNDLerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  CTBNDLtoken = CTBNDLchar == YYEMPTY ? YYEMPTY : YYTRANSLATE (CTBNDLchar);

  /* If not already recovering from an error, report this error.  */
  if (!CTBNDLerrstatus)
    {
      ++CTBNDLnerrs;
#if ! YYERROR_VERBOSE
      CTBNDLerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR CTBNDLsyntax_error (&CTBNDLmsg_alloc, &CTBNDLmsg, \
                                        CTBNDLssp, CTBNDLtoken)
      {
        char const *CTBNDLmsgp = YY_("syntax error");
        int CTBNDLsyntax_error_status;
        CTBNDLsyntax_error_status = YYSYNTAX_ERROR;
        if (CTBNDLsyntax_error_status == 0)
          CTBNDLmsgp = CTBNDLmsg;
        else if (CTBNDLsyntax_error_status == 1)
          {
            if (CTBNDLmsg != CTBNDLmsgbuf)
              YYSTACK_FREE (CTBNDLmsg);
            CTBNDLmsg = (char *) YYSTACK_ALLOC (CTBNDLmsg_alloc);
            if (!CTBNDLmsg)
              {
                CTBNDLmsg = CTBNDLmsgbuf;
                CTBNDLmsg_alloc = sizeof CTBNDLmsgbuf;
                CTBNDLsyntax_error_status = 2;
              }
            else
              {
                CTBNDLsyntax_error_status = YYSYNTAX_ERROR;
                CTBNDLmsgp = CTBNDLmsg;
              }
          }
        CTBNDLerror (CTBNDLmsgp);
        if (CTBNDLsyntax_error_status == 2)
          goto CTBNDLexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (CTBNDLerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (CTBNDLchar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (CTBNDLchar == YYEOF)
            YYABORT;
        }
      else
        {
          CTBNDLdestruct ("Error: discarding",
                      CTBNDLtoken, &CTBNDLlval);
          CTBNDLchar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto CTBNDLerrlab1;


/*---------------------------------------------------.
| CTBNDLerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
CTBNDLerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label CTBNDLerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto CTBNDLerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (CTBNDLlen);
  CTBNDLlen = 0;
  YY_STACK_PRINT (CTBNDLss, CTBNDLssp);
  CTBNDLstate = *CTBNDLssp;
  goto CTBNDLerrlab1;


/*-------------------------------------------------------------.
| CTBNDLerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
CTBNDLerrlab1:
  CTBNDLerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      CTBNDLn = CTBNDLpact[CTBNDLstate];
      if (!CTBNDLpact_value_is_default (CTBNDLn))
        {
          CTBNDLn += YYTERROR;
          if (0 <= CTBNDLn && CTBNDLn <= YYLAST && CTBNDLcheck[CTBNDLn] == YYTERROR)
            {
              CTBNDLn = CTBNDLtable[CTBNDLn];
              if (0 < CTBNDLn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (CTBNDLssp == CTBNDLss)
        YYABORT;


      CTBNDLdestruct ("Error: popping",
                  CTBNDLstos[CTBNDLstate], CTBNDLvsp);
      YYPOPSTACK (1);
      CTBNDLstate = *CTBNDLssp;
      YY_STACK_PRINT (CTBNDLss, CTBNDLssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++CTBNDLvsp = CTBNDLlval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", CTBNDLstos[CTBNDLn], CTBNDLvsp, CTBNDLlsp);

  CTBNDLstate = CTBNDLn;
  goto CTBNDLnewstate;


/*-------------------------------------.
| CTBNDLacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
CTBNDLacceptlab:
  CTBNDLresult = 0;
  goto CTBNDLreturn;

/*-----------------------------------.
| CTBNDLabortlab -- YYABORT comes here.  |
`-----------------------------------*/
CTBNDLabortlab:
  CTBNDLresult = 1;
  goto CTBNDLreturn;

#if !defined CTBNDLoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| CTBNDLexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
CTBNDLexhaustedlab:
  CTBNDLerror (YY_("memory exhausted"));
  CTBNDLresult = 2;
  /* Fall through.  */
#endif

CTBNDLreturn:
  if (CTBNDLchar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      CTBNDLtoken = YYTRANSLATE (CTBNDLchar);
      CTBNDLdestruct ("Cleanup: discarding lookahead",
                  CTBNDLtoken, &CTBNDLlval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (CTBNDLlen);
  YY_STACK_PRINT (CTBNDLss, CTBNDLssp);
  while (CTBNDLssp != CTBNDLss)
    {
      CTBNDLdestruct ("Cleanup: popping",
                  CTBNDLstos[*CTBNDLssp], CTBNDLvsp);
      YYPOPSTACK (1);
    }
#ifndef CTBNDLoverflow
  if (CTBNDLss != CTBNDLssa)
    YYSTACK_FREE (CTBNDLss);
#endif
#if YYERROR_VERBOSE
  if (CTBNDLmsg != CTBNDLmsgbuf)
    YYSTACK_FREE (CTBNDLmsg);
#endif
  return CTBNDLresult;
}
#line 336 "BooleanGrammar.y" /* yacc.c:1906  */


#include "lex.CTBNDL.cc"

void set_current_network(Network* network)
{
  current_network = network;
}

Network* get_current_network()
{
  return current_network;
}

