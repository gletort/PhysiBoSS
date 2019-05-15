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

/* All symbols defined below should begin with RC or YY, to avoid
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
#line 2 "RunConfigGrammar.y" /* yacc.c:339  */

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

extern int RClex();

static void RCerror(const char *s);
static Network* network;

extern std::string RC_error_head();

#line 110 "RunConfigGrammar.tab.c" /* yacc.c:339  */

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
extern int RCdebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum RCtokentype
  {
    VARIABLE = 258,
    SYMBOL = 259,
    DOUBLE = 260,
    INTEGER = 261,
    LOGAND = 262,
    LOGOR = 263,
    LOGXOR = 264,
    LOGNOT = 265,
    EQUAL = 266,
    NOT_EQUAL = 267,
    NODE = 268,
    GTEQ = 269,
    LTEQ = 270
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 46 "RunConfigGrammar.y" /* yacc.c:355  */

  Expression* expr;
  char* str;
  double d;
  long long l;
  std::vector<const Node*>* node_list;
  std::vector<Expression*>* expr_list;
  IStateGroup::ProbaIState* istate_expr;
  std::vector<IStateGroup::ProbaIState*>* istate_expr_list;

#line 174 "RunConfigGrammar.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE RClval;

int RCparse (void);



/* Copy the second part of user declarations.  */

#line 191 "RunConfigGrammar.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 RCtype_uint8;
#else
typedef unsigned char RCtype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 RCtype_int8;
#else
typedef signed char RCtype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 RCtype_uint16;
#else
typedef unsigned short int RCtype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 RCtype_int16;
#else
typedef short int RCtype_int16;
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
/* Suppress an incorrect diagnostic about RClval being uninitialized.  */
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


#if ! defined RCoverflow || YYERROR_VERBOSE

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
#endif /* ! defined RCoverflow || YYERROR_VERBOSE */


#if (! defined RCoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union RCalloc
{
  RCtype_int16 RCss_alloc;
  YYSTYPE RCvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union RCalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (RCtype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T RCnewbytes;                                            \
        YYCOPY (&RCptr->Stack_alloc, Stack, RCsize);                    \
        Stack = &RCptr->Stack_alloc;                                    \
        RCnewbytes = RCstacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        RCptr += RCnewbytes / sizeof (*RCptr);                          \
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
          YYSIZE_T RCi;                         \
          for (RCi = 0; RCi < (Count); RCi++)   \
            (Dst)[RCi] = (Src)[RCi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  16
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   86

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  34
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  53
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  101

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by RClex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   270

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? RCtranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by RClex, without out-of-bounds checking.  */
static const RCtype_uint8 RCtranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    26,     2,     2,     2,     2,     2,     2,
      22,    23,    27,    24,    21,    25,    18,    28,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    33,    16,
      29,    17,    30,    32,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    19,     2,    20,    31,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const RCtype_uint16 RCrline[] =
{
       0,    84,    84,    87,    92,    93,    94,    95,    98,   106,
     133,   146,   152,   157,   164,   169,   176,   182,   187,   194,
     203,   207,   211,   215,   221,   227,   231,   235,   239,   243,
     249,   253,   257,   263,   267,   271,   277,   281,   285,   289,
     293,   299,   303,   307,   313,   317,   323,   327,   333,   337,
     341,   347,   351,   357
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const RCtname[] =
{
  "$end", "error", "$undefined", "VARIABLE", "SYMBOL", "DOUBLE",
  "INTEGER", "LOGAND", "LOGOR", "LOGXOR", "LOGNOT", "EQUAL", "NOT_EQUAL",
  "NODE", "GTEQ", "LTEQ", "';'", "'='", "'.'", "'['", "']'", "','", "'('",
  "')'", "'+'", "'-'", "'!'", "'*'", "'/'", "'<'", "'>'", "'^'", "'?'",
  "':'", "$accept", "translation_unit", "decl", "runconfig_decl",
  "node_attr_decl", "symbol_istate_list", "symbol_list",
  "istate_expression_list", "istate_expression", "expression_list",
  "var_decl", "primary_expression", "postfix_expression",
  "unary_expression", "multiplicative_expression", "additive_expression",
  "relational_expression", "equality_expression", "logical_and_expression",
  "logical_or_expression", "logical_xor_expression",
  "conditional_expression", "expression", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const RCtype_uint16 RCtoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,    59,    61,    46,    91,
      93,    44,    40,    41,    43,    45,    33,    42,    47,    60,
      62,    94,    63,    58
};
# endif

#define YYPACT_NINF -68

#define RCpact_value_is_default(Yystate) \
  (!!((Yystate) == (-68)))

#define YYTABLE_NINF -1

#define RCtable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const RCtype_int8 RCpact[] =
{
      41,   -14,    -8,   -68,    13,    24,   -68,   -68,   -68,    -5,
     -68,     9,     9,    38,   -68,    32,   -68,   -68,    42,   -68,
     -68,   -68,     9,     9,     9,     9,     9,   -68,   -68,   -68,
      28,    34,    -7,    50,    64,    65,     7,   -68,    56,    58,
      59,   -68,    71,    60,   -68,    55,   -68,   -68,   -68,     9,
       9,     9,     9,     9,     9,     9,     9,     9,     9,     9,
       9,     9,     9,     9,   -68,   -68,     9,   -68,    15,   -68,
     -68,   -68,    28,    28,    34,    34,    34,    34,    -7,    -7,
      50,    64,    65,    65,    46,    66,    20,   -68,    61,     9,
     -68,   -68,    15,    15,   -68,   -68,    43,   -68,   -68,    15,
     -68
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const RCtype_uint8 RCdefact[] =
{
       0,     0,     0,     7,     0,     0,     2,     5,     6,     0,
       4,     0,     0,     0,    12,     0,     1,     3,     0,    22,
      21,    20,     0,     0,     0,     0,     0,    24,    25,    30,
      33,    36,    41,    44,    46,    48,    51,    53,     0,     0,
       0,    11,     0,     0,    29,     0,    26,    27,    28,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    19,     8,     0,    13,     0,    23,
      31,    32,    34,    35,    40,    39,    37,    38,    42,    43,
      45,    47,    49,    50,     0,     0,     0,    14,     0,     0,
       9,    10,     0,     0,    52,    15,     0,    17,    16,     0,
      18
};

  /* YYPGOTO[NTERM-NUM].  */
static const RCtype_int8 RCpgoto[] =
{
     -68,   -68,    76,   -68,   -68,   -68,   -68,   -68,    -9,   -68,
     -68,   -67,   -68,   -20,    14,    -6,    10,    25,    26,     8,
     -68,    -4,   -12
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const RCtype_int8 RCdefgoto[] =
{
      -1,     5,     6,     7,     8,     9,    15,    86,    87,    96,
      10,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const RCtype_uint8 RCtable[] =
{
      39,    88,    44,    11,    46,    47,    48,    53,    54,    12,
      13,    45,    19,    18,    20,    21,    61,    14,    19,    22,
      20,    21,    55,    56,    16,    88,    97,     1,     2,    70,
      71,    23,   100,    24,    25,    26,    91,    23,    62,    63,
       3,    92,    40,     4,     1,     2,    43,    74,    75,    76,
      77,    84,    41,    42,    85,    49,    50,     3,    51,    52,
       4,    57,    58,    98,    99,    72,    73,    78,    79,    82,
      83,    59,    64,    60,    65,    67,    66,    68,    69,    89,
      93,    17,    90,    95,    80,    94,    81
};

static const RCtype_uint8 RCcheck[] =
{
      12,    68,    22,    17,    24,    25,    26,    14,    15,    17,
      18,    23,     3,    18,     5,     6,     9,     4,     3,    10,
       5,     6,    29,    30,     0,    92,    93,     3,     4,    49,
      50,    22,    99,    24,    25,    26,    16,    22,    31,    32,
      16,    21,     4,    19,     3,     4,     4,    53,    54,    55,
      56,    63,    20,    21,    66,    27,    28,    16,    24,    25,
      19,    11,    12,    20,    21,    51,    52,    57,    58,    61,
      62,     7,    16,     8,    16,     4,    17,    17,    23,    33,
      19,     5,    16,    92,    59,    89,    60
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const RCtype_uint8 RCstos[] =
{
       0,     3,     4,    16,    19,    35,    36,    37,    38,    39,
      44,    17,    17,    18,     4,    40,     0,    36,    18,     3,
       5,     6,    10,    22,    24,    25,    26,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    56,
       4,    20,    21,     4,    47,    56,    47,    47,    47,    27,
      28,    24,    25,    14,    15,    29,    30,    11,    12,     7,
       8,     9,    31,    32,    16,    16,    17,     4,    17,    23,
      47,    47,    48,    48,    49,    49,    49,    49,    50,    50,
      51,    52,    53,    53,    56,    56,    41,    42,    45,    33,
      16,    16,    21,    19,    55,    42,    43,    45,    20,    21,
      45
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const RCtype_uint8 RCr1[] =
{
       0,    34,    35,    35,    36,    36,    36,    36,    37,    38,
      38,    39,    40,    40,    41,    41,    42,    43,    43,    44,
      45,    45,    45,    45,    46,    47,    47,    47,    47,    47,
      48,    48,    48,    49,    49,    49,    50,    50,    50,    50,
      50,    51,    51,    51,    52,    52,    53,    53,    54,    54,
      54,    55,    55,    56
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const RCtype_uint8 RCr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     4,     6,
       6,     3,     1,     3,     1,     3,     4,     1,     3,     4,
       1,     1,     1,     3,     1,     1,     2,     2,     2,     2,
       1,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     3,     1,     3,     1,     3,     1,     3,
       3,     1,     5,     1
};


#define RCerrok         (RCerrstatus = 0)
#define RCclearin       (RCchar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto RCacceptlab
#define YYABORT         goto RCabortlab
#define YYERROR         goto RCerrorlab


#define YYRECOVERING()  (!!RCerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (RCchar == YYEMPTY)                                        \
    {                                                           \
      RCchar = (Token);                                         \
      RClval = (Value);                                         \
      YYPOPSTACK (RClen);                                       \
      RCstate = *RCssp;                                         \
      goto RCbackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      RCerror (YY_("syntax error: cannot back up")); \
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
  if (RCdebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (RCdebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      RC_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
RC_symbol_value_print (FILE *RCoutput, int RCtype, YYSTYPE const * const RCvaluep)
{
  FILE *RCo = RCoutput;
  YYUSE (RCo);
  if (!RCvaluep)
    return;
# ifdef YYPRINT
  if (RCtype < YYNTOKENS)
    YYPRINT (RCoutput, RCtoknum[RCtype], *RCvaluep);
# endif
  YYUSE (RCtype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
RC_symbol_print (FILE *RCoutput, int RCtype, YYSTYPE const * const RCvaluep)
{
  YYFPRINTF (RCoutput, "%s %s (",
             RCtype < YYNTOKENS ? "token" : "nterm", RCtname[RCtype]);

  RC_symbol_value_print (RCoutput, RCtype, RCvaluep);
  YYFPRINTF (RCoutput, ")");
}

/*------------------------------------------------------------------.
| RC_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
RC_stack_print (RCtype_int16 *RCbottom, RCtype_int16 *RCtop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; RCbottom <= RCtop; RCbottom++)
    {
      int RCbot = *RCbottom;
      YYFPRINTF (stderr, " %d", RCbot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (RCdebug)                                                  \
    RC_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
RC_reduce_print (RCtype_int16 *RCssp, YYSTYPE *RCvsp, int RCrule)
{
  unsigned long int RClno = RCrline[RCrule];
  int RCnrhs = RCr2[RCrule];
  int RCi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             RCrule - 1, RClno);
  /* The symbols being reduced.  */
  for (RCi = 0; RCi < RCnrhs; RCi++)
    {
      YYFPRINTF (stderr, "   $%d = ", RCi + 1);
      RC_symbol_print (stderr,
                       RCstos[RCssp[RCi + 1 - RCnrhs]],
                       &(RCvsp[(RCi + 1) - (RCnrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (RCdebug)                          \
    RC_reduce_print (RCssp, RCvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int RCdebug;
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

# ifndef RCstrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define RCstrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
RCstrlen (const char *RCstr)
{
  YYSIZE_T RClen;
  for (RClen = 0; RCstr[RClen]; RClen++)
    continue;
  return RClen;
}
#  endif
# endif

# ifndef RCstpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define RCstpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
RCstpcpy (char *RCdest, const char *RCsrc)
{
  char *RCd = RCdest;
  const char *RCs = RCsrc;

  while ((*RCd++ = *RCs++) != '\0')
    continue;

  return RCd - 1;
}
#  endif
# endif

# ifndef RCtnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for RCerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from RCtname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
RCtnamerr (char *RCres, const char *RCstr)
{
  if (*RCstr == '"')
    {
      YYSIZE_T RCn = 0;
      char const *RCp = RCstr;

      for (;;)
        switch (*++RCp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++RCp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (RCres)
              RCres[RCn] = *RCp;
            RCn++;
            break;

          case '"':
            if (RCres)
              RCres[RCn] = '\0';
            return RCn;
          }
    do_not_strip_quotes: ;
    }

  if (! RCres)
    return RCstrlen (RCstr);

  return RCstpcpy (RCres, RCstr) - RCres;
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
RCsyntax_error (YYSIZE_T *RCmsg_alloc, char **RCmsg,
                RCtype_int16 *RCssp, int RCtoken)
{
  YYSIZE_T RCsize0 = RCtnamerr (YY_NULLPTR, RCtname[RCtoken]);
  YYSIZE_T RCsize = RCsize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *RCformat = YY_NULLPTR;
  /* Arguments of RCformat. */
  char const *RCarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int RCcount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in RCchar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated RCchar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (RCtoken != YYEMPTY)
    {
      int RCn = RCpact[*RCssp];
      RCarg[RCcount++] = RCtname[RCtoken];
      if (!RCpact_value_is_default (RCn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int RCxbegin = RCn < 0 ? -RCn : 0;
          /* Stay within bounds of both RCcheck and RCtname.  */
          int RCchecklim = YYLAST - RCn + 1;
          int RCxend = RCchecklim < YYNTOKENS ? RCchecklim : YYNTOKENS;
          int RCx;

          for (RCx = RCxbegin; RCx < RCxend; ++RCx)
            if (RCcheck[RCx + RCn] == RCx && RCx != YYTERROR
                && !RCtable_value_is_error (RCtable[RCx + RCn]))
              {
                if (RCcount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    RCcount = 1;
                    RCsize = RCsize0;
                    break;
                  }
                RCarg[RCcount++] = RCtname[RCx];
                {
                  YYSIZE_T RCsize1 = RCsize + RCtnamerr (YY_NULLPTR, RCtname[RCx]);
                  if (! (RCsize <= RCsize1
                         && RCsize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  RCsize = RCsize1;
                }
              }
        }
    }

  switch (RCcount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        RCformat = S;                       \
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
    YYSIZE_T RCsize1 = RCsize + RCstrlen (RCformat);
    if (! (RCsize <= RCsize1 && RCsize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    RCsize = RCsize1;
  }

  if (*RCmsg_alloc < RCsize)
    {
      *RCmsg_alloc = 2 * RCsize;
      if (! (RCsize <= *RCmsg_alloc
             && *RCmsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *RCmsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *RCp = *RCmsg;
    int RCi = 0;
    while ((*RCp = *RCformat) != '\0')
      if (*RCp == '%' && RCformat[1] == 's' && RCi < RCcount)
        {
          RCp += RCtnamerr (RCp, RCarg[RCi++]);
          RCformat += 2;
        }
      else
        {
          RCp++;
          RCformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
RCdestruct (const char *RCmsg, int RCtype, YYSTYPE *RCvaluep)
{
  YYUSE (RCvaluep);
  if (!RCmsg)
    RCmsg = "Deleting";
  YY_SYMBOL_PRINT (RCmsg, RCtype, RCvaluep, RClocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (RCtype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int RCchar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE RClval;
/* Number of syntax errors so far.  */
int RCnerrs;


/*----------.
| RCparse.  |
`----------*/

int
RCparse (void)
{
    int RCstate;
    /* Number of tokens to shift before error messages enabled.  */
    int RCerrstatus;

    /* The stacks and their tools:
       'RCss': related to states.
       'RCvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow RCoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    RCtype_int16 RCssa[YYINITDEPTH];
    RCtype_int16 *RCss;
    RCtype_int16 *RCssp;

    /* The semantic value stack.  */
    YYSTYPE RCvsa[YYINITDEPTH];
    YYSTYPE *RCvs;
    YYSTYPE *RCvsp;

    YYSIZE_T RCstacksize;

  int RCn;
  int RCresult;
  /* Lookahead token as an internal (translated) token number.  */
  int RCtoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE RCval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char RCmsgbuf[128];
  char *RCmsg = RCmsgbuf;
  YYSIZE_T RCmsg_alloc = sizeof RCmsgbuf;
#endif

#define YYPOPSTACK(N)   (RCvsp -= (N), RCssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int RClen = 0;

  RCssp = RCss = RCssa;
  RCvsp = RCvs = RCvsa;
  RCstacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  RCstate = 0;
  RCerrstatus = 0;
  RCnerrs = 0;
  RCchar = YYEMPTY; /* Cause a token to be read.  */
  goto RCsetstate;

/*------------------------------------------------------------.
| RCnewstate -- Push a new state, which is found in RCstate.  |
`------------------------------------------------------------*/
 RCnewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  RCssp++;

 RCsetstate:
  *RCssp = RCstate;

  if (RCss + RCstacksize - 1 <= RCssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T RCsize = RCssp - RCss + 1;

#ifdef RCoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *RCvs1 = RCvs;
        RCtype_int16 *RCss1 = RCss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if RCoverflow is a macro.  */
        RCoverflow (YY_("memory exhausted"),
                    &RCss1, RCsize * sizeof (*RCssp),
                    &RCvs1, RCsize * sizeof (*RCvsp),
                    &RCstacksize);

        RCss = RCss1;
        RCvs = RCvs1;
      }
#else /* no RCoverflow */
# ifndef YYSTACK_RELOCATE
      goto RCexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= RCstacksize)
        goto RCexhaustedlab;
      RCstacksize *= 2;
      if (YYMAXDEPTH < RCstacksize)
        RCstacksize = YYMAXDEPTH;

      {
        RCtype_int16 *RCss1 = RCss;
        union RCalloc *RCptr =
          (union RCalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (RCstacksize));
        if (! RCptr)
          goto RCexhaustedlab;
        YYSTACK_RELOCATE (RCss_alloc, RCss);
        YYSTACK_RELOCATE (RCvs_alloc, RCvs);
#  undef YYSTACK_RELOCATE
        if (RCss1 != RCssa)
          YYSTACK_FREE (RCss1);
      }
# endif
#endif /* no RCoverflow */

      RCssp = RCss + RCsize - 1;
      RCvsp = RCvs + RCsize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) RCstacksize));

      if (RCss + RCstacksize - 1 <= RCssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", RCstate));

  if (RCstate == YYFINAL)
    YYACCEPT;

  goto RCbackup;

/*-----------.
| RCbackup.  |
`-----------*/
RCbackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  RCn = RCpact[RCstate];
  if (RCpact_value_is_default (RCn))
    goto RCdefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (RCchar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      RCchar = RClex ();
    }

  if (RCchar <= YYEOF)
    {
      RCchar = RCtoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      RCtoken = YYTRANSLATE (RCchar);
      YY_SYMBOL_PRINT ("Next token is", RCtoken, &RClval, &RClloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  RCn += RCtoken;
  if (RCn < 0 || YYLAST < RCn || RCcheck[RCn] != RCtoken)
    goto RCdefault;
  RCn = RCtable[RCn];
  if (RCn <= 0)
    {
      if (RCtable_value_is_error (RCn))
        goto RCerrlab;
      RCn = -RCn;
      goto RCreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (RCerrstatus)
    RCerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", RCtoken, &RClval, &RClloc);

  /* Discard the shifted token.  */
  RCchar = YYEMPTY;

  RCstate = RCn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++RCvsp = RClval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto RCnewstate;


/*-----------------------------------------------------------.
| RCdefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
RCdefault:
  RCn = RCdefact[RCstate];
  if (RCn == 0)
    goto RCerrlab;
  goto RCreduce;


/*-----------------------------.
| RCreduce -- Do a reduction.  |
`-----------------------------*/
RCreduce:
  /* RCn is the number of a rule to reduce with.  */
  RClen = RCr2[RCn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  RCval = RCvsp[1-RClen];


  YY_REDUCE_PRINT (RCn);
  switch (RCn)
    {
        case 2:
#line 85 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
}
#line 1337 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 88 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
}
#line 1344 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 99 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  NetworkState network_state;
  double value = (RCvsp[-1].expr)->eval(NULL, network_state);
  RunConfig::getInstance()->setParameter((RCvsp[-3].str), value);
}
#line 1354 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 107 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  Node* node = network->getNode((RCvsp[-5].str));
  NetworkState network_state;
  double value = (RCvsp[-1].expr)->eval(NULL, network_state);
  if (!strcasecmp((RCvsp[-3].str), "istate")) {
    if (backward_istate) {
      node->setIState((bool)value);
    } else {
      if (value < 0) {
	new IStateGroup(node);
      } else {
	new IStateGroup(node, (RCvsp[-1].expr));
      }
    }
  } else if (!strcasecmp((RCvsp[-3].str), "is_internal")) {
    node->isInternal((bool)value);
  } else if (!strcasecmp((RCvsp[-3].str), "refstate")) {
    if (value < 0) {
      node->setReference(false);
    } else {
      node->setReferenceState((NodeState)value);
    }
  } else {
    throw BNException(std::string(RC_error_head() + "invalid node attribute: ") + (RCvsp[-3].str) + ", valid attributes are: istate or is_internal");
  }
}
#line 1385 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 134 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  if (strcasecmp((RCvsp[-3].str), "istate")) {
    throw BNException(std::string(RC_error_head() + "invalid node group attribute: ") + (RCvsp[-3].str) + ", valid attribute is istate");
  }
  std::string error_msg;
  new IStateGroup((RCvsp[-5].node_list), (RCvsp[-1].istate_expr_list), error_msg);
  if (error_msg.length() > 0) {
    throw BNException(std::string(RC_error_head() + error_msg));
  }
}
#line 1400 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 147 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.node_list) = (RCvsp[-1].node_list);
}
#line 1408 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 153 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.node_list) = new std::vector<const Node*>();
  (RCval.node_list)->push_back(network->getNode((RCvsp[0].str)));
}
#line 1417 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 158 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.node_list) = (RCvsp[-2].node_list);
  (RCval.node_list)->push_back(network->getNode((RCvsp[0].str)));
}
#line 1426 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 165 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.istate_expr_list) = new std::vector<IStateGroup::ProbaIState*>();
  (RCval.istate_expr_list)->push_back((RCvsp[0].istate_expr));
}
#line 1435 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 170 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.istate_expr_list) = (RCvsp[-2].istate_expr_list);
  (RCval.istate_expr_list)->push_back((RCvsp[0].istate_expr));
}
#line 1444 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 177 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.istate_expr) = new IStateGroup::ProbaIState((RCvsp[-3].expr), (RCvsp[-1].expr_list));
}
#line 1452 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 183 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr_list) = new std::vector<Expression*>();
  (RCval.expr_list)->push_back((RCvsp[0].expr));
}
#line 1461 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 188 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr_list) = (RCvsp[-2].expr_list);
  (RCval.expr_list)->push_back((RCvsp[0].expr));
}
#line 1470 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 195 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  SymbolTable* symtab = SymbolTable::getInstance();
  const Symbol* symbol = symtab->getOrMakeSymbol((RCvsp[-3].str));
  NetworkState dummy_state;
  symtab->setSymbolValue(symbol, (RCvsp[-1].expr)->eval(NULL, dummy_state));
}
#line 1481 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 204 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new ConstantExpression((RCvsp[0].l));
}
#line 1489 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 208 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new ConstantExpression((RCvsp[0].d));
}
#line 1497 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 212 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new SymbolExpression(SymbolTable::getInstance()->getOrMakeSymbol((RCvsp[0].str)));
}
#line 1505 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 216 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new ParenthesisExpression((RCvsp[-1].expr));
}
#line 1513 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 222 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = (RCvsp[0].expr);
}
#line 1521 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 228 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = (RCvsp[0].expr);
}
#line 1529 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 232 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = (RCvsp[0].expr);
}
#line 1537 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 236 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new SubExpression(new ConstantExpression(0.0), (RCvsp[0].expr));
}
#line 1545 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 240 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new NotLogicalExpression((RCvsp[0].expr));
}
#line 1553 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 244 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new NotLogicalExpression((RCvsp[0].expr));
}
#line 1561 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 250 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = (RCvsp[0].expr);
}
#line 1569 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 254 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new MulExpression((RCvsp[-2].expr), (RCvsp[0].expr));
}
#line 1577 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 258 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new DivExpression((RCvsp[-2].expr), (RCvsp[0].expr));
}
#line 1585 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 264 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = (RCvsp[0].expr);
}
#line 1593 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 268 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new AddExpression((RCvsp[-2].expr), (RCvsp[0].expr));
}
#line 1601 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 272 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new SubExpression((RCvsp[-2].expr), (RCvsp[0].expr));
}
#line 1609 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 278 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = (RCvsp[0].expr);
}
#line 1617 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 282 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new LetterExpression((RCvsp[-2].expr), (RCvsp[0].expr));
}
#line 1625 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 286 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new GreaterExpression((RCvsp[-2].expr), (RCvsp[0].expr));
}
#line 1633 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 290 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new LetterOrEqualExpression((RCvsp[-2].expr), (RCvsp[0].expr));
}
#line 1641 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 294 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new GreaterOrEqualExpression((RCvsp[-2].expr), (RCvsp[0].expr));
}
#line 1649 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 300 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = (RCvsp[0].expr);
}
#line 1657 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 304 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new EqualExpression((RCvsp[-2].expr), (RCvsp[0].expr));
}
#line 1665 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 308 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new NotEqualExpression((RCvsp[-2].expr), (RCvsp[0].expr));
}
#line 1673 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 314 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = (RCvsp[0].expr);
}
#line 1681 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 318 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new AndLogicalExpression((RCvsp[-2].expr), (RCvsp[0].expr));
}
#line 1689 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 324 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = (RCvsp[0].expr);
}
#line 1697 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 328 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new OrLogicalExpression((RCvsp[-2].expr), (RCvsp[0].expr));
}
#line 1705 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 334 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = (RCvsp[0].expr);
}
#line 1713 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 338 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new XorLogicalExpression((RCvsp[-2].expr), (RCvsp[0].expr));
}
#line 1721 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 342 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new XorLogicalExpression((RCvsp[-2].expr), (RCvsp[0].expr));
}
#line 1729 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 348 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = (RCvsp[0].expr);
}
#line 1737 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 352 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = new CondExpression((RCvsp[-4].expr), (RCvsp[-2].expr), (RCvsp[0].expr));
}
#line 1745 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 358 "RunConfigGrammar.y" /* yacc.c:1646  */
    {
  (RCval.expr) = (RCvsp[0].expr);
}
#line 1753 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
    break;


#line 1757 "RunConfigGrammar.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter RCchar, and that requires
     that RCtoken be updated with the new translation.  We take the
     approach of translating immediately before every use of RCtoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering RCchar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", RCr1[RCn], &RCval, &RCloc);

  YYPOPSTACK (RClen);
  RClen = 0;
  YY_STACK_PRINT (RCss, RCssp);

  *++RCvsp = RCval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  RCn = RCr1[RCn];

  RCstate = RCpgoto[RCn - YYNTOKENS] + *RCssp;
  if (0 <= RCstate && RCstate <= YYLAST && RCcheck[RCstate] == *RCssp)
    RCstate = RCtable[RCstate];
  else
    RCstate = RCdefgoto[RCn - YYNTOKENS];

  goto RCnewstate;


/*--------------------------------------.
| RCerrlab -- here on detecting error.  |
`--------------------------------------*/
RCerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  RCtoken = RCchar == YYEMPTY ? YYEMPTY : YYTRANSLATE (RCchar);

  /* If not already recovering from an error, report this error.  */
  if (!RCerrstatus)
    {
      ++RCnerrs;
#if ! YYERROR_VERBOSE
      RCerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR RCsyntax_error (&RCmsg_alloc, &RCmsg, \
                                        RCssp, RCtoken)
      {
        char const *RCmsgp = YY_("syntax error");
        int RCsyntax_error_status;
        RCsyntax_error_status = YYSYNTAX_ERROR;
        if (RCsyntax_error_status == 0)
          RCmsgp = RCmsg;
        else if (RCsyntax_error_status == 1)
          {
            if (RCmsg != RCmsgbuf)
              YYSTACK_FREE (RCmsg);
            RCmsg = (char *) YYSTACK_ALLOC (RCmsg_alloc);
            if (!RCmsg)
              {
                RCmsg = RCmsgbuf;
                RCmsg_alloc = sizeof RCmsgbuf;
                RCsyntax_error_status = 2;
              }
            else
              {
                RCsyntax_error_status = YYSYNTAX_ERROR;
                RCmsgp = RCmsg;
              }
          }
        RCerror (RCmsgp);
        if (RCsyntax_error_status == 2)
          goto RCexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (RCerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (RCchar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (RCchar == YYEOF)
            YYABORT;
        }
      else
        {
          RCdestruct ("Error: discarding",
                      RCtoken, &RClval);
          RCchar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto RCerrlab1;


/*---------------------------------------------------.
| RCerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
RCerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label RCerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto RCerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (RClen);
  RClen = 0;
  YY_STACK_PRINT (RCss, RCssp);
  RCstate = *RCssp;
  goto RCerrlab1;


/*-------------------------------------------------------------.
| RCerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
RCerrlab1:
  RCerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      RCn = RCpact[RCstate];
      if (!RCpact_value_is_default (RCn))
        {
          RCn += YYTERROR;
          if (0 <= RCn && RCn <= YYLAST && RCcheck[RCn] == YYTERROR)
            {
              RCn = RCtable[RCn];
              if (0 < RCn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (RCssp == RCss)
        YYABORT;


      RCdestruct ("Error: popping",
                  RCstos[RCstate], RCvsp);
      YYPOPSTACK (1);
      RCstate = *RCssp;
      YY_STACK_PRINT (RCss, RCssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++RCvsp = RClval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", RCstos[RCn], RCvsp, RClsp);

  RCstate = RCn;
  goto RCnewstate;


/*-------------------------------------.
| RCacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
RCacceptlab:
  RCresult = 0;
  goto RCreturn;

/*-----------------------------------.
| RCabortlab -- YYABORT comes here.  |
`-----------------------------------*/
RCabortlab:
  RCresult = 1;
  goto RCreturn;

#if !defined RCoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| RCexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
RCexhaustedlab:
  RCerror (YY_("memory exhausted"));
  RCresult = 2;
  /* Fall through.  */
#endif

RCreturn:
  if (RCchar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      RCtoken = YYTRANSLATE (RCchar);
      RCdestruct ("Cleanup: discarding lookahead",
                  RCtoken, &RClval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (RClen);
  YY_STACK_PRINT (RCss, RCssp);
  while (RCssp != RCss)
    {
      RCdestruct ("Cleanup: popping",
                  RCstos[*RCssp], RCvsp);
      YYPOPSTACK (1);
    }
#ifndef RCoverflow
  if (RCss != RCssa)
    YYSTACK_FREE (RCss);
#endif
#if YYERROR_VERBOSE
  if (RCmsg != RCmsgbuf)
    YYSTACK_FREE (RCmsg);
#endif
  return RCresult;
}
#line 363 "RunConfigGrammar.y" /* yacc.c:1906  */


void runconfig_setNetwork(Network* _network)
{
  network = _network;
}

#include "lex.RC.cc"
