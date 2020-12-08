/* A Bison parser, made by GNU Bison 1.875c.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TK_IDENTIFIERS = 258,
     TK_EVENTS = 259,
     TK_NETWORK = 260,
     TK_AUTOMATON = 261,
     TK_STATE = 262,
     TK_TRANS = 263,
     TK_FROM = 264,
     TK_SYN = 265,
     TK_LOC = 266,
     TK_RESULTS = 267,
     TK_CONTINUOUS = 268,
     TK_DISCRETE = 269,
     TK_REACH = 270,
     TK_PARTIAL = 271,
     TK_MIN = 272,
     TK_MAX = 273,
     TK_STATE_NUM = 274,
     TK_NBR = 275,
     TK_REWARD = 276,
     TK_SUM_RW = 277,
     TK_PROD_RW = 278,
     TK_ID = 279,
     TK_INTEGER = 280,
     TK_REAL = 281,
     TK_EXPONENT = 282,
     TK_LEFT_BRACKET = 283,
     TK_RIGHT_BRACKET = 284,
     TK_LEFT_PARENTHESIS = 285,
     TK_RIGHT_PARENTHESIS = 286,
     TK_LEFT_SQUARE_BRACKET = 287,
     TK_RIGHT_SQUARE_BRACKET = 288,
     TK_SEMICOLON = 289,
     TK_COMMA = 290,
     TK_ATTRIB = 291,
     TK_COLON = 292,
     TK_POINT = 293,
     TK_2DOTS = 294,
     TK_DOTS = 295,
     TK_SIMPLE = 296,
     TK_DOUBLE = 297,
     TK_MINUS_MINUS = 298,
     TK_MINUS = 299,
     TK_PLUS_PLUS = 300,
     TK_PLUS = 301,
     TK_NOT = 302,
     TK_XOR = 303,
     TK_OR = 304,
     TK_AND = 305,
     TK_DIV = 306,
     TK_MULT = 307,
     TK_MINOR_EQUAL = 308,
     TK_MINOR = 309,
     TK_MAJOR_EQUAL = 310,
     TK_MAJOR = 311,
     TK_UNEQUAL = 312,
     TK_EQUAL = 313
   };
#endif
#define TK_IDENTIFIERS 258
#define TK_EVENTS 259
#define TK_NETWORK 260
#define TK_AUTOMATON 261
#define TK_STATE 262
#define TK_TRANS 263
#define TK_FROM 264
#define TK_SYN 265
#define TK_LOC 266
#define TK_RESULTS 267
#define TK_CONTINUOUS 268
#define TK_DISCRETE 269
#define TK_REACH 270
#define TK_PARTIAL 271
#define TK_MIN 272
#define TK_MAX 273
#define TK_STATE_NUM 274
#define TK_NBR 275
#define TK_REWARD 276
#define TK_SUM_RW 277
#define TK_PROD_RW 278
#define TK_ID 279
#define TK_INTEGER 280
#define TK_REAL 281
#define TK_EXPONENT 282
#define TK_LEFT_BRACKET 283
#define TK_RIGHT_BRACKET 284
#define TK_LEFT_PARENTHESIS 285
#define TK_RIGHT_PARENTHESIS 286
#define TK_LEFT_SQUARE_BRACKET 287
#define TK_RIGHT_SQUARE_BRACKET 288
#define TK_SEMICOLON 289
#define TK_COMMA 290
#define TK_ATTRIB 291
#define TK_COLON 292
#define TK_POINT 293
#define TK_2DOTS 294
#define TK_DOTS 295
#define TK_SIMPLE 296
#define TK_DOUBLE 297
#define TK_MINUS_MINUS 298
#define TK_MINUS 299
#define TK_PLUS_PLUS 300
#define TK_PLUS 301
#define TK_NOT 302
#define TK_XOR 303
#define TK_OR 304
#define TK_AND 305
#define TK_DIV 306
#define TK_MULT 307
#define TK_MINOR_EQUAL 308
#define TK_MINOR 309
#define TK_MAJOR_EQUAL 310
#define TK_MAJOR 311
#define TK_UNEQUAL 312
#define TK_EQUAL 313




/* Copy the first part of user declarations.  */
#line 1 "./src/lb/glc.y"

//====================================================================================//
//                                                                                    //
//                           Parser and Semantic Analysis                             //
//                                                                                    //
//====================================================================================//
//  This File:   glc.y                       Tool: YACC                               //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    01/Mar/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 07/Nov/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  YACC is a general-purpose parser generator that converts a context-free grammar   //
// a C program.In this module are make the actions to generate the internal SAN's     //
// tables and the others functionalities of this textual interface for PEPS.          //
//====================================================================================//
#include "sanfile.h"

//====================================================================================//
//                                   Global variables                                 //
//====================================================================================//

NET *net; // Network structure
DOM *dom; // Dominion structure
EDIC *edic; // Events dictionary

inf2pos itop;  // Used to convert of infix operator expression form
               //   to postfix operator expression form 

FUNC *t_ident; // Auxiliar table to identifiers

aut_st    replic_map;      // replication map to perform lump

int not_error = 1;

int tour; // Set the times that the parser was called, the first to count the number
          // of automatas, the number of states, the number of transitions and others
int n_aut; // Number of automatons in network, this counter is increased in the first 
           // pass to alloc the automaton structure in second pass
int n_state; // Number of states in each automaton
int n_trans; // Number of transitions in each state
int n_mst; // Number of master events
int n_sync; // Number of synchronizing events
int n_local; // Number of local events
int n_events; // Number of events (local and synchronizing)
int aut_group; // Automaton group number 
//====================================================================================//
//                                      Variables                                     //
//====================================================================================//

char temp[MAX_AUTOMATON_NAME];
char tmp[FUNCTION], tmp1[FUNCTION];  // Variables used to grouping characters
char fnc[FUNCTION]; // Variable to store the functions
char fname[MAX_FUNC_NAME];
char event_name[MAX_EVENT_NAME];
char i1n[MAX_FUNC_NAME];
char i2n[MAX_FUNC_NAME];

int with_id, sum_or_prod;
int r_t;
double v_tmp;
int i_tmp;
int i1, i2, iv, ia, di, dl;
int from_dst, f_t;
int event_type;
int event_rate;
int event_mst;
int queue_p, queue_m;

char aut_replic[MAX_AUTOMATON_NAME];
char state_queue[MAX_STATE_NAME];
char aux_dst[MAX_STATE_NAME];
int aut; // current automaton index
int replic; // number of replics
int state; // current state index
int queue; // queue
int trans; // transition
int mst; // master event
int synch; // synchronizing event
int local; // local event
int **mat_trans; // This matrix store the number of transitions of each automaton-state


//====================================================================================//
//                                   Parser variables                                 //
//====================================================================================//

extern int yynerrs;  // Variable of the YACC generator to count the number of errors.  
int yylineno;        // Variable of the YACC generator to count the current line

extern int yylex(void); // Get the next token
extern void yyrestart(FILE *input_file);
extern char *yytext; 
extern FILE *yyin; // Input file


//====================================================================================//
//                                   Function errors                                  //
//====================================================================================//
//  These functions analysis which error occurred and put the error into a message    //
// more specific.                                                                     //
//====================================================================================//


//====================================================================================
// Function that YACC use to put sintatic message errors                      
//====================================================================================
extern void yyerror(char *s)
{
  yynerrs++; 
  not_error = 0;
  Sintatic_Error(yynerrs, yylineno, s, yytext);
}

//====================================================================================
// Function to put semantic error messages 
//====================================================================================
void type_error(const char* error_number)
{
  yynerrs++;
  not_error = 0;
  Semantic_Error_YACC(yynerrs, yylineno, yytext, error_number);
}


void alloc_mat_aut(int aaut)
{
  mat_trans = new int*[aaut];  
}

void alloc_mat_state(int aaut, int sstate)
{
  mat_trans[aaut] = new int[sstate];  
  for(int i=0; i<sstate; i++)
    mat_trans[aaut][i] = 0;
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 343 "./src/lb/yacc.C"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   209

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  109
/* YYNRULES -- Number of rules. */
#define YYNRULES  183
/* YYNRULES -- Number of states. */
#define YYNSTATES  284

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   313

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     9,    12,    13,    14,    15,    23,    25,
      26,    29,    30,    31,    41,    42,    43,    53,    55,    56,
      60,    61,    62,    67,    69,    73,    75,    77,    79,    81,
      82,    84,    86,    87,    88,    96,    99,   102,   105,   106,
     107,   108,   114,   115,   119,   122,   127,   132,   135,   138,
     139,   140,   146,   147,   151,   152,   156,   158,   160,   162,
     165,   167,   168,   169,   170,   178,   180,   181,   182,   186,
     187,   188,   194,   195,   197,   199,   201,   203,   205,   207,
     209,   211,   213,   215,   217,   219,   221,   223,   225,   227,
     229,   231,   233,   235,   237,   238,   239,   247,   249,   250,
     251,   252,   261,   263,   265,   266,   267,   268,   269,   280,
     282,   283,   284,   285,   286,   296,   298,   299,   300,   301,
     309,   310,   311,   316,   317,   318,   319,   328,   330,   331,
     332,   336,   338,   340,   342,   345,   348,   349,   354,   355,
     356,   357,   364,   366,   367,   368,   373,   374,   378,   379,
     381,   383,   387,   388,   389,   394,   396,   400,   401,   402,
     407,   409,   413,   414,   417,   419,   420,   424,   425,   426,
     434,   435,   437,   439,   440,   442,   444,   448,   449,   454,
     455,   459,   460,   462
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
      60,     0,    -1,    61,    66,   109,   113,   157,    -1,     3,
      62,    -1,    -1,    -1,    -1,    24,    63,    36,    64,    79,
      34,    65,    -1,    62,    -1,    -1,     4,    67,    -1,    -1,
      -1,    11,    68,    24,    69,    73,    76,   164,    78,    72,
      -1,    -1,    -1,    10,    70,    24,    71,    73,    76,   164,
      78,    72,    -1,    67,    -1,    -1,    32,    74,    33,    -1,
      -1,    -1,    25,    75,   163,    25,    -1,    24,    -1,    30,
      77,    31,    -1,    77,    -1,    24,    -1,   106,    -1,    34,
      -1,    -1,    80,    -1,    83,    -1,    -1,    -1,    32,    25,
      81,   163,    25,    82,    33,    -1,    89,    86,    -1,    83,
      85,    -1,    35,    84,    -1,    -1,    -1,    -1,   108,    87,
      89,    88,    86,    -1,    -1,    30,    83,    31,    -1,   107,
      89,    -1,    17,    30,    84,    31,    -1,    18,    30,    84,
      31,    -1,    21,   101,    -1,    19,   101,    -1,    -1,    -1,
      20,    90,    96,    91,   101,    -1,    -1,    92,    94,    95,
      -1,    -1,    24,    93,   103,    -1,   106,    -1,    22,    -1,
      23,    -1,    97,   100,    -1,    97,    -1,    -1,    -1,    -1,
      32,   101,    98,   163,   101,    99,    33,    -1,   101,    -1,
      -1,    -1,    24,   102,   103,    -1,    -1,    -1,    32,   104,
      25,   105,    33,    -1,    -1,    26,    -1,    25,    -1,    27,
      -1,    46,    -1,    44,    -1,    47,    -1,    52,    -1,    51,
      -1,    46,    -1,    44,    -1,    58,    -1,    57,    -1,    56,
      -1,    55,    -1,    54,    -1,    53,    -1,    50,    -1,    49,
      -1,    48,    -1,    41,    -1,    42,    -1,    -1,    -1,   112,
      15,   110,    36,    83,   111,    34,    -1,    16,    -1,    -1,
      -1,    -1,     5,    24,   114,    30,   116,    31,   115,   117,
      -1,    14,    -1,    13,    -1,    -1,    -1,    -1,    -1,     6,
     118,    24,   119,   149,   123,   120,   128,   121,   122,    -1,
     117,    -1,    -1,    -1,    -1,    -1,     7,   124,    24,   125,
     152,   155,   133,   126,   127,    -1,   123,    -1,    -1,    -1,
      -1,     9,    24,   129,   131,   130,   133,   128,    -1,    -1,
      -1,    32,    25,   132,    33,    -1,    -1,    -1,    -1,     8,
     134,    30,   137,    31,   141,   135,   136,    -1,   133,    -1,
      -1,    -1,    24,   138,   139,    -1,    58,    -1,    45,    -1,
      43,    -1,    44,    25,    -1,    46,    25,    -1,    -1,    32,
      25,   140,    33,    -1,    -1,    -1,    -1,    24,   142,   145,
     143,   147,   144,    -1,   141,    -1,    -1,    -1,    32,    25,
     146,    33,    -1,    -1,    30,   148,    31,    -1,    -1,   106,
      -1,    24,    -1,    32,   150,    33,    -1,    -1,    -1,    25,
     151,   163,    25,    -1,    24,    -1,    32,   153,    33,    -1,
      -1,    -1,    25,   154,   163,    25,    -1,    24,    -1,    30,
     156,    31,    -1,    -1,   162,   106,    -1,    24,    -1,    -1,
      12,   158,   159,    -1,    -1,    -1,    24,   160,    36,    83,
     161,    34,   159,    -1,    -1,    46,    -1,    44,    -1,    -1,
      39,    -1,    40,    -1,    24,   166,   165,    -1,    -1,    35,
      24,   166,   165,    -1,    -1,    28,   167,    29,    -1,    -1,
      24,    -1,    25,   163,    25,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   166,   166,   174,   175,   180,   190,   179,   194,   195,
     203,   208,   213,   207,   217,   222,   216,   228,   229,   233,
     234,   238,   238,   246,   259,   260,   264,   282,   298,   299,
     306,   307,   324,   325,   324,   336,   340,   344,   345,   349,
     351,   348,   353,   357,   358,   359,   360,   361,   366,   372,
     376,   371,   399,   399,   428,   428,   450,   454,   455,   459,
     474,   476,   485,   487,   484,   492,   493,   497,   497,   508,
     509,   508,   511,   515,   516,   517,   521,   525,   529,   533,
     534,   535,   536,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   547,   555,   557,   555,   567,   568,   576,
     577,   576,   582,   583,   593,   597,   612,   623,   592,   631,
     632,   641,   646,   670,   640,   691,   692,   698,   702,   697,
     713,   718,   717,   721,   730,   742,   729,   774,   775,   780,
     779,   806,   823,   845,   867,   890,   917,   916,   921,   925,
     926,   925,  1056,  1057,  1062,  1061,  1066,  1070,  1071,  1075,
    1149,  1252,  1253,  1257,  1257,  1278,  1304,  1305,  1309,  1309,
    1335,  1367,  1368,  1372,  1382,  1408,  1408,  1414,  1421,  1413,
    1428,  1438,  1439,  1440,  1444,  1445,  1449,  1450,  1454,  1455,
    1459,  1460,  1464,  1465
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TK_IDENTIFIERS", "TK_EVENTS",
  "TK_NETWORK", "TK_AUTOMATON", "TK_STATE", "TK_TRANS", "TK_FROM",
  "TK_SYN", "TK_LOC", "TK_RESULTS", "TK_CONTINUOUS", "TK_DISCRETE",
  "TK_REACH", "TK_PARTIAL", "TK_MIN", "TK_MAX", "TK_STATE_NUM", "TK_NBR",
  "TK_REWARD", "TK_SUM_RW", "TK_PROD_RW", "TK_ID", "TK_INTEGER", "TK_REAL",
  "TK_EXPONENT", "TK_LEFT_BRACKET", "TK_RIGHT_BRACKET",
  "TK_LEFT_PARENTHESIS", "TK_RIGHT_PARENTHESIS", "TK_LEFT_SQUARE_BRACKET",
  "TK_RIGHT_SQUARE_BRACKET", "TK_SEMICOLON", "TK_COMMA", "TK_ATTRIB",
  "TK_COLON", "TK_POINT", "TK_2DOTS", "TK_DOTS", "TK_SIMPLE", "TK_DOUBLE",
  "TK_MINUS_MINUS", "TK_MINUS", "TK_PLUS_PLUS", "TK_PLUS", "TK_NOT",
  "TK_XOR", "TK_OR", "TK_AND", "TK_DIV", "TK_MULT", "TK_MINOR_EQUAL",
  "TK_MINOR", "TK_MAJOR_EQUAL", "TK_MAJOR", "TK_UNEQUAL", "TK_EQUAL",
  "$accept", "start", "blk_id", "blk_int_id", "@1", "@2", "blk_int_id2",
  "blk_events", "blk_int_ev", "@3", "@4", "@5", "@6", "blk_int_ev2",
  "event_replic", "int_event_replic", "@7", "rate", "int_rate",
  "semi_colon", "blk_exp_dom", "blk_def_dom", "@8", "@9", "blk_exp",
  "blk_exp2", "list_exp", "exp", "@10", "@11", "elem", "@12", "@13", "@14",
  "@15", "op_rw", "dominion", "interval", "interval_1", "@16", "@17",
  "id_stat", "id_replic", "@18", "replic", "@19", "@20", "number",
  "op_unary", "op_bin", "reach", "@21", "@22", "t_reach", "blk_net", "@23",
  "@24", "model_type", "blk_autom", "@25", "@26", "@27", "@28",
  "blk_autom2", "blk_state", "@29", "@30", "@31", "blk_state2", "blk_from",
  "@32", "@33", "blk_from_replic", "@34", "blk_trans", "@35", "@36",
  "blk_trans2", "dst_state", "@37", "dst_replic", "@38", "blk_type_trans",
  "@39", "@40", "blk_type_trans2", "e_replic", "@41", "blk_prob", "prob",
  "blk_replic", "replic_number", "@42", "blk_queue", "queue_number", "@43",
  "blk_reward", "reward_number", "blk_res", "@44", "blk_def_res", "@45",
  "@46", "signal", "dots", "aut_list", "list_member", "aut_dom",
  "aut_dom_int", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    59,    60,    61,    61,    63,    64,    62,    65,    65,
      66,    68,    69,    67,    70,    71,    67,    72,    72,    73,
      73,    75,    74,    74,    76,    76,    77,    77,    78,    78,
      79,    79,    81,    82,    80,    83,    84,    85,    85,    87,
      88,    86,    86,    89,    89,    89,    89,    89,    89,    90,
      91,    89,    92,    89,    93,    89,    89,    94,    94,    95,
      96,    96,    98,    99,    97,   100,   100,   102,   101,   104,
     105,   103,   103,   106,   106,   106,   107,   107,   107,   108,
     108,   108,   108,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   110,   111,   109,   112,   112,   114,
     115,   113,   116,   116,   118,   119,   120,   121,   117,   122,
     122,   124,   125,   126,   123,   127,   127,   129,   130,   128,
     128,   132,   131,   131,   134,   135,   133,   136,   136,   138,
     137,   137,   137,   137,   137,   137,   140,   139,   139,   142,
     143,   141,   144,   144,   146,   145,   145,   147,   147,   148,
     148,   149,   149,   151,   150,   150,   152,   152,   154,   153,
     153,   155,   155,   156,   156,   158,   157,   160,   161,   159,
     159,   162,   162,   162,   163,   163,   164,   164,   165,   165,
     166,   166,   167,   167
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     5,     2,     0,     0,     0,     7,     1,     0,
       2,     0,     0,     9,     0,     0,     9,     1,     0,     3,
       0,     0,     4,     1,     3,     1,     1,     1,     1,     0,
       1,     1,     0,     0,     7,     2,     2,     2,     0,     0,
       0,     5,     0,     3,     2,     4,     4,     2,     2,     0,
       0,     5,     0,     3,     0,     3,     1,     1,     1,     2,
       1,     0,     0,     0,     7,     1,     0,     0,     3,     0,
       0,     5,     0,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     0,     7,     1,     0,     0,
       0,     8,     1,     1,     0,     0,     0,     0,    10,     1,
       0,     0,     0,     0,     9,     1,     0,     0,     0,     7,
       0,     0,     4,     0,     0,     0,     8,     1,     0,     0,
       3,     1,     1,     1,     2,     2,     0,     4,     0,     0,
       0,     6,     1,     0,     0,     4,     0,     3,     0,     1,
       1,     3,     0,     0,     4,     1,     3,     0,     0,     4,
       1,     3,     0,     2,     1,     0,     3,     0,     0,     7,
       0,     1,     1,     0,     1,     1,     3,     0,     4,     0,
       3,     0,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       4,     0,     0,     0,     5,     3,     1,     0,    98,     0,
      14,    11,    10,    97,     0,     0,     6,     0,     0,     0,
       0,    94,    52,    15,    12,    99,   165,     2,     0,     0,
       0,     0,    49,     0,    54,    74,    73,    75,    52,     0,
      77,    76,    78,     0,    30,    31,    42,     0,    56,    52,
      20,    20,     0,   170,    52,    52,    52,    67,    48,    61,
      47,    72,     0,    32,     9,    92,    93,    82,    81,    91,
      90,    89,    80,    79,    88,    87,    86,    85,    84,    83,
      35,    39,    57,    58,     0,    44,     0,     0,     0,     0,
     167,   166,    95,    38,     0,     0,    72,     0,    50,    60,
      69,    55,    43,     0,     8,     7,    52,    53,    66,    23,
      21,     0,    26,     0,   177,    25,    27,   177,   103,   102,
       0,     0,     0,    52,    36,    45,    46,    68,    62,     0,
       0,   174,   175,     0,    40,    59,    65,     0,    19,     0,
     181,    29,    29,   100,    52,    96,    37,     0,    51,    70,
      33,    42,     0,    24,     0,   179,    28,    18,    18,     0,
     168,     0,     0,     0,    41,    22,   182,     0,     0,     0,
     176,    17,    16,    13,   104,   101,     0,    63,    71,    34,
       0,   180,   181,     0,   170,     0,   183,   179,   105,   169,
      64,   178,   152,     0,     0,   155,   153,     0,   111,   106,
       0,   151,     0,   120,     0,   112,     0,   107,   154,   157,
     117,   110,     0,   162,   123,   109,   108,   160,   158,     0,
     173,     0,     0,   118,     0,   156,   164,   172,   171,     0,
       0,   124,   113,   121,     0,     0,   161,   163,     0,   116,
       0,   120,   159,     0,   115,   114,   122,   119,   129,   133,
       0,   132,     0,   131,     0,   138,   134,   135,     0,     0,
     130,   139,   125,   136,   146,   128,     0,     0,   140,   127,
     126,   137,   144,   148,     0,     0,   143,   145,   150,   149,
       0,   142,   141,   147
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     2,     3,     5,     9,    22,   105,     8,   171,    18,
      51,    17,    50,   172,    87,   111,   137,   114,   115,   157,
      43,    44,   103,   163,    93,    94,   124,    80,   106,   151,
      46,    59,   129,    47,    61,    84,   107,    98,    99,   147,
     185,   135,    58,    96,   101,   130,   162,    48,    49,    81,
      14,    28,   122,    15,    20,    52,   159,   120,   175,   183,
     192,   203,   211,   216,   199,   202,   209,   239,   245,   207,
     214,   234,   223,   240,   232,   238,   265,   270,   254,   255,
     260,   266,   262,   264,   273,   282,   268,   274,   276,   280,
     194,   197,   200,   213,   219,   224,   221,   229,    27,    53,
      91,   121,   176,   230,   133,   141,   170,   155,   168
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -228
static const short yypact[] =
{
      17,     8,    43,    54,  -228,  -228,  -228,    -1,    59,    24,
    -228,  -228,  -228,  -228,    65,    61,  -228,    60,    66,    73,
      87,  -228,    27,  -228,  -228,  -228,  -228,  -228,    42,    55,
      70,    77,  -228,    77,  -228,  -228,  -228,  -228,    62,    78,
    -228,  -228,  -228,    68,  -228,  -228,    96,   -10,  -228,    62,
      72,    72,    75,    83,    62,    62,    62,  -228,  -228,    79,
    -228,    80,    82,  -228,     8,  -228,  -228,  -228,  -228,  -228,
    -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,
    -228,  -228,  -228,  -228,    79,  -228,    12,    -9,    -9,    36,
    -228,  -228,  -228,    81,    84,    86,    80,    77,  -228,  -228,
    -228,  -228,  -228,    16,  -228,  -228,    62,  -228,    77,  -228,
    -228,    85,  -228,    -2,    90,  -228,  -228,    90,  -228,  -228,
      88,    74,    89,    62,  -228,  -228,  -228,  -228,  -228,    77,
      95,  -228,  -228,    97,  -228,  -228,  -228,    16,  -228,    93,
      98,    94,    94,  -228,    62,  -228,  -228,    16,  -228,  -228,
    -228,    96,   102,  -228,    37,    99,  -228,    -1,    -1,   115,
    -228,    77,   100,   103,  -228,  -228,  -228,    16,   106,   105,
    -228,  -228,  -228,  -228,  -228,  -228,   107,  -228,  -228,  -228,
     114,  -228,    98,   108,    83,   122,  -228,    99,  -228,  -228,
    -228,  -228,   124,    40,   150,  -228,  -228,   125,  -228,  -228,
      16,  -228,   135,   151,   136,  -228,   138,  -228,  -228,   131,
    -228,   115,    44,   101,   132,  -228,  -228,  -228,  -228,   133,
     -13,   157,   142,  -228,    16,  -228,  -228,  -228,  -228,   137,
      14,  -228,  -228,  -228,   157,   144,  -228,  -228,   140,   150,
     139,   151,  -228,   -16,  -228,  -228,  -228,  -228,  -228,  -228,
     146,  -228,   148,  -228,   143,   145,  -228,  -228,   152,   153,
    -228,  -228,  -228,  -228,   147,   157,   149,   155,  -228,  -228,
    -228,  -228,  -228,   154,   156,    69,   152,  -228,  -228,  -228,
     159,  -228,  -228,  -228
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -228,  -228,  -228,   111,  -228,  -228,  -228,  -228,   174,  -228,
    -228,  -228,  -228,    25,   134,  -228,  -228,   104,    91,    45,
    -228,  -228,  -228,  -228,   -19,   -51,  -228,    35,  -228,  -228,
     -43,  -228,  -228,  -228,  -228,  -228,  -228,  -228,   109,  -228,
    -228,  -228,   -31,  -228,   110,  -228,  -228,   -87,  -228,  -228,
    -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,   -20,  -228,
    -228,  -228,  -228,  -228,   -45,  -228,  -228,  -228,  -228,   -46,
    -228,  -228,  -228,  -228,  -227,  -228,  -228,  -228,  -228,  -228,
    -228,  -228,   -80,  -228,  -228,  -228,  -228,  -228,  -228,  -228,
    -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,
      13,  -228,  -228,  -228,  -133,    92,    11,    18,  -228
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned short yytable[] =
{
     116,   116,    60,    45,   152,    95,    85,   241,   248,    10,
      11,   226,    82,    83,   161,   112,    35,    36,    37,    62,
       1,   113,   112,    35,    36,    37,   116,   249,   250,   251,
     252,   227,     4,   228,   180,    92,   109,   110,   269,    35,
      36,    37,   253,     6,    29,    30,    31,    32,    33,   118,
     119,    34,    35,    36,    37,   131,   132,    38,     7,    39,
      16,   166,   167,   134,   195,   196,   128,   204,   217,   218,
      19,    40,   146,    41,    42,    13,    21,   136,    54,    29,
      30,    31,    32,    33,    23,    55,    34,    35,    36,    37,
      24,   235,    38,   278,    35,    36,    37,    25,   148,    26,
      56,    57,    64,    63,    86,    89,    40,    90,    41,    42,
     144,    97,   100,   102,   140,   125,   123,   126,   138,   143,
     149,   174,   150,   145,   153,   160,   154,   165,   156,   182,
     177,   220,   188,   178,   169,   181,   179,    65,    66,   186,
      67,   184,    68,   237,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,   190,   193,   198,   201,   205,
     206,   208,   210,   212,   222,   231,   225,   233,   236,   242,
     243,   256,   246,   257,   258,   104,   261,   259,   263,   267,
     272,    12,   271,   173,   275,    88,   164,   158,   279,   277,
     283,   215,   117,   108,   244,   247,   281,   189,   191,     0,
     187,     0,     0,     0,   139,     0,   127,     0,     0,   142
};

static const short yycheck[] =
{
      87,    88,    33,    22,   137,    56,    49,   234,    24,    10,
      11,    24,    22,    23,   147,    24,    25,    26,    27,    38,
       3,    30,    24,    25,    26,    27,   113,    43,    44,    45,
      46,    44,    24,    46,   167,    54,    24,    25,   265,    25,
      26,    27,    58,     0,    17,    18,    19,    20,    21,    13,
      14,    24,    25,    26,    27,    39,    40,    30,     4,    32,
      36,    24,    25,   106,    24,    25,    97,   200,    24,    25,
       5,    44,   123,    46,    47,    16,    15,   108,    36,    17,
      18,    19,    20,    21,    24,    30,    24,    25,    26,    27,
      24,   224,    30,    24,    25,    26,    27,    24,   129,    12,
      30,    24,    34,    25,    32,    30,    44,    24,    46,    47,
      36,    32,    32,    31,    24,    31,    35,    31,    33,    31,
      25,     6,    25,    34,    31,   144,    28,    25,    34,    24,
     161,    30,    24,    33,    35,    29,    33,    41,    42,    25,
      44,    34,    46,   230,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    33,    32,     7,    33,    24,
       9,    25,    24,    32,    32,     8,    33,    25,    31,    25,
      30,    25,    33,    25,    31,    64,    24,    32,    25,    32,
      25,     7,    33,   158,    30,    51,   151,   142,   275,    33,
      31,   211,    88,    84,   239,   241,   276,   184,   187,    -1,
     182,    -1,    -1,    -1,   113,    -1,    96,    -1,    -1,   117
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,    60,    61,    24,    62,     0,     4,    66,    63,
      10,    11,    67,    16,   109,   112,    36,    70,    68,     5,
     113,    15,    64,    24,    24,    24,    12,   157,   110,    17,
      18,    19,    20,    21,    24,    25,    26,    27,    30,    32,
      44,    46,    47,    79,    80,    83,    89,    92,   106,   107,
      71,    69,   114,   158,    36,    30,    30,    24,   101,    90,
     101,    93,    83,    25,    34,    41,    42,    44,    46,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      86,   108,    22,    23,    94,    89,    32,    73,    73,    30,
      24,   159,    83,    83,    84,    84,   102,    32,    96,    97,
      32,   103,    31,    81,    62,    65,    87,    95,    97,    24,
      25,    74,    24,    30,    76,    77,   106,    76,    13,    14,
     116,   160,   111,    35,    85,    31,    31,   103,   101,    91,
     104,    39,    40,   163,    89,   100,   101,    75,    33,    77,
      24,   164,   164,    31,    36,    34,    84,    98,   101,    25,
      25,    88,   163,    31,    28,   166,    34,    78,    78,   115,
      83,   163,   105,    82,    86,    25,    24,    25,   167,    35,
     165,    67,    72,    72,     6,   117,   161,   101,    33,    33,
     163,    29,    24,   118,    34,    99,    25,   166,    24,   159,
      33,   165,   119,    32,   149,    24,    25,   150,     7,   123,
     151,    33,   124,   120,   163,    24,     9,   128,    25,   125,
      24,   121,    32,   152,   129,   117,   122,    24,    25,   153,
      30,   155,    32,   131,   154,    33,    24,    44,    46,   156,
     162,     8,   133,    25,   130,   163,    31,   106,   134,   126,
     132,   133,    25,    30,   123,   127,    33,   128,    24,    43,
      44,    45,    46,    58,   137,   138,    25,    25,    31,    32,
     139,    24,   141,    25,   142,   135,   140,    32,   145,   133,
     136,    33,    25,   143,   146,    30,   147,    33,    24,   106,
     148,   141,   144,    31
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 5:
#line 180 "./src/lb/glc.y"
    { strcpy(fname,(char *) yytext);
      if(tour==1){
	    itop.reset(); 
	    ia = -1; 	
	  }
	  if(tour==4){
        itop.reset();  	
	    ia = -1; 	
      }
	;}
    break;

  case 6:
#line 190 "./src/lb/glc.y"
    {itop.reset();;}
    break;

  case 11:
#line 208 "./src/lb/glc.y"
    { if(tour==1)
	    n_events++;
	  event_type = 0;
	;}
    break;

  case 12:
#line 213 "./src/lb/glc.y"
    { strcpy(fname,(char*)yytext);
	  di = dl = 0;
	;}
    break;

  case 14:
#line 217 "./src/lb/glc.y"
    { if(tour==1)
	    n_events++;
	  event_type = 1;
	;}
    break;

  case 15:
#line 222 "./src/lb/glc.y"
    { strcpy(fname,(char*)yytext);
    di = dl = 0;
  ;}
    break;

  case 21:
#line 238 "./src/lb/glc.y"
    {di = atol((char*) yytext); ;}
    break;

  case 22:
#line 239 "./src/lb/glc.y"
    { dl = atol((char*) yytext); 
      n_events+= dl-di;
      if((dl-di)<0){
        Semantic_Error(fname, Parser_114);
        not_error = 0;
      }
    ;}
    break;

  case 23:
#line 247 "./src/lb/glc.y"
    { if(dom->get_name((char *) yytext)>=0){
        di = dom->get_first((char *) yytext);
        dl = dom->get_last((char *) yytext);
        n_events+= dl-di;        
      }
      else {
        type_error(Parser_01);
      }
    ;}
    break;

  case 26:
#line 265 "./src/lb/glc.y"
    { if(tour==4){
        if(net->get_ident_name((char *) yytext)<0){
          type_error(Parser_01);
        }
        else{
          if(dl-di<=0){
            edic->put_event(fname,event_type, "0", net->get_ident_name((char*)yytext));
          }
          else{
            for(int i=di; i<=dl; i++){
              sprintf(event_name, "%s[%d]", fname,i);
              edic->put_event(event_name,event_type, "0", net->get_ident_name((char*)yytext));
            }
          }
        }
      }
    ;}
    break;

  case 27:
#line 283 "./src/lb/glc.y"
    { if(tour==4){
        if(dl-di<=0){
          edic->put_event(fname,event_type, (char*)yytext, -1);
        }
        else{
          for(int i=di; i<=dl; i++){
            sprintf(event_name, "%s[%d]", fname,i);
            edic->put_event(event_name,event_type, (char*)yytext, -1);
          }
        }
      }
    ;}
    break;

  case 31:
#line 308 "./src/lb/glc.y"
    { if(tour==1){
    	itop.get_func(fnc);
        itop.reset();
	    t_ident->put_name(fname);
        t_ident->put_exp(fnc);
      }
      if(tour==4){
		itop.get_func(fnc);
        itop.reset();
	    net->put_ident_name(fname);
        net->put_ident_exp(fnc);
      }
    ;}
    break;

  case 32:
#line 324 "./src/lb/glc.y"
    {di = atol((char*)yytext);;}
    break;

  case 33:
#line 325 "./src/lb/glc.y"
    { dl = atol((char*)yytext);
      dom->put_name(fname);
      if(dom->put_dom(di, dl)<0){
        Semantic_Error(fname, Parser_114);
        not_error = 0;
      }
    ;}
    break;

  case 39:
#line 349 "./src/lb/glc.y"
    { itop.put_op((char*) yytext);;}
    break;

  case 40:
#line 351 "./src/lb/glc.y"
    { itop.put_elem();;}
    break;

  case 44:
#line 358 "./src/lb/glc.y"
    { itop.put_elem(); ;}
    break;

  case 45:
#line 359 "./src/lb/glc.y"
    { itop.put_elem("min");;}
    break;

  case 46:
#line 360 "./src/lb/glc.y"
    { itop.put_elem("max");;}
    break;

  case 47:
#line 362 "./src/lb/glc.y"
    { strcpy(tmp,"% "); 
      sprintf(tmp,"%s%s ",tmp,tmp1);
      itop.put_elem(tmp);
    ;}
    break;

  case 48:
#line 367 "./src/lb/glc.y"
    { strcpy(tmp, "@ ");
      sprintf(tmp,"%s%s ",tmp,tmp1);
      itop.put_elem(tmp);
    ;}
    break;

  case 49:
#line 372 "./src/lb/glc.y"
    { strcpy(tmp, "@$ ");
      i1 = i2 = 0;
    ;}
    break;

  case 50:
#line 376 "./src/lb/glc.y"
    { if(i1>i2 && tour==4){
        Semantic_Error(fname, Parser_105);
        not_error = 0;
      }
    ;}
    break;

  case 51:
#line 382 "./src/lb/glc.y"
    { strcpy(temp,tmp1); 
      if(tour==1){
        sprintf(tmp,"%s%s %s %s ", tmp, temp, i1n, i2n);
        itop.put_elem(tmp);
      }
      if(tour==4){
        sprintf(tmp,"%s%s %d %d ", tmp, temp, i1, i2);
        itop.put_elem(tmp);
        if(net->get_name(temp, i1, i2)==-1){
          Semantic_Warning(fname, temp, Parser_103);
        }
        if(net->get_name(temp, i1, i2)==0){
          Semantic_Warning(fname, temp, Parser_104);
        }
      } 

    ;}
    break;

  case 52:
#line 399 "./src/lb/glc.y"
    { tmp[0]='\0';
       i1 = i2 = 0;
    ;}
    break;

  case 53:
#line 403 "./src/lb/glc.y"
    { if(i1>i2 && tour==4){
        Semantic_Error(fname, Parser_105);
        not_error = 0;
      } 
      if(with_id){
        if(sum_or_prod)
    	  strcpy(tmp1,"%S ");
        else strcpy(tmp1,"%P ");
      }
      else{
        if(sum_or_prod)
          strcpy(tmp1,"%$ ");
        else
          strcpy(tmp1,"%# ");
      }
      if(tour==1){
        sprintf(tmp1,"%s%s %s %s ",tmp1, temp, i1n, i2n);
        itop.put_elem(tmp1);
      }
      if(tour==4){
        sprintf(tmp1,"%s%s %d %d ",tmp1, temp, i1, i2);
        itop.put_elem(tmp1);
      }
      
    ;}
    break;

  case 54:
#line 428 "./src/lb/glc.y"
    {strcpy(tmp1, (char*) yytext);;}
    break;

  case 55:
#line 430 "./src/lb/glc.y"
    { if(tour==4 || tour==1){
        if(net->get_name(tmp1, ia)>=0){
          sprintf(tmp1,"%d ", net->get_name(tmp1, ia)); 
        }
        else{
          if(net->get_name(tmp1)>=0){
            sprintf(tmp1,"%d ", net->get_name(tmp1));
          }
          else{
            if(t_ident->get_name(tmp1)<0){
              strcat(tmp1," ");          
            }
            else{
              t_ident->get_exp(tmp1,tmp1); 
            }
          }
        }
        itop.put_elem(tmp1); 
      }
    ;}
    break;

  case 56:
#line 450 "./src/lb/glc.y"
    { itop.put_elem((char*) yytext);;}
    break;

  case 57:
#line 454 "./src/lb/glc.y"
    {sum_or_prod = 1; ;}
    break;

  case 58:
#line 455 "./src/lb/glc.y"
    {sum_or_prod = 0;;}
    break;

  case 59:
#line 460 "./src/lb/glc.y"
    { strcpy(temp,tmp1); 
      if(tour==4 && with_id==1){
        if(net->get_name(temp, i1, i2)==-1){
          Semantic_Warning(fname, temp, Parser_103);
        }
        if(net->get_name(temp, i1, i2)==0){
          Semantic_Warning(fname, temp, Parser_104);
        }
      } 
    ;}
    break;

  case 61:
#line 476 "./src/lb/glc.y"
    { if(tour==4){
        i1 = 0;
        i2 = net->get_net_n_aut()-1;
      }
    ;}
    break;

  case 62:
#line 485 "./src/lb/glc.y"
    { i1 = net->get_name(tmp1); strcpy(i1n,tmp1); tmp1[0] = '\0'; ;}
    break;

  case 63:
#line 487 "./src/lb/glc.y"
    { i2 = net->get_name(tmp1); strcpy(i2n,tmp1); tmp1[0] = '\0'; ;}
    break;

  case 65:
#line 492 "./src/lb/glc.y"
    {with_id = 1;;}
    break;

  case 66:
#line 493 "./src/lb/glc.y"
    {with_id = 0;;}
    break;

  case 67:
#line 497 "./src/lb/glc.y"
    {strcpy(tmp1,(char*) yytext);;}
    break;

  case 68:
#line 499 "./src/lb/glc.y"
    { if(tour==4){
    	if(net->get_name(tmp1)>=0){
          ia = net->get_name(tmp1);
        }
      }
    ;}
    break;

  case 69:
#line 508 "./src/lb/glc.y"
    {strcat(tmp1,(char*) yytext);;}
    break;

  case 70:
#line 509 "./src/lb/glc.y"
    {strcat(tmp1,(char*) yytext);;}
    break;

  case 71:
#line 510 "./src/lb/glc.y"
    {strcat(tmp1,(char*) yytext);;}
    break;

  case 76:
#line 522 "./src/lb/glc.y"
    { itop.put_elem("1");
      itop.put_op("*");
    ;}
    break;

  case 77:
#line 526 "./src/lb/glc.y"
    { itop.put_elem("-1");
      itop.put_op("*");
    ;}
    break;

  case 78:
#line 529 "./src/lb/glc.y"
    { itop.put_op((char*) yytext);;}
    break;

  case 94:
#line 555 "./src/lb/glc.y"
    {itop.reset(); ia = -1; strcpy(fname,(char *) yytext);;}
    break;

  case 95:
#line 557 "./src/lb/glc.y"
    { if(tour==4){
    	itop.get_func(fnc);
        itop.reset();
        net->put_reach_exp(fnc);
      }
    ;}
    break;

  case 97:
#line 567 "./src/lb/glc.y"
    {r_t = 1; PRF::prf.partial_rss();;}
    break;

  case 98:
#line 568 "./src/lb/glc.y"
    {r_t = 0; PRF::prf.full_rss();;}
    break;

  case 99:
#line 576 "./src/lb/glc.y"
    { net->put_net_name((char*) yytext);;}
    break;

  case 100:
#line 577 "./src/lb/glc.y"
    { aut = 0; ;}
    break;

  case 102:
#line 582 "./src/lb/glc.y"
    { net->put_net_model(DISCRETE);;}
    break;

  case 103:
#line 583 "./src/lb/glc.y"
    {net->put_net_model(CONTINUOUS);;}
    break;

  case 104:
#line 593 "./src/lb/glc.y"
    { if(tour==1)
        n_aut++;
    ;}
    break;

  case 105:
#line 597 "./src/lb/glc.y"
    { replic = 1;
      strcpy(fname,(char*)yytext);
      if(tour==2){
        n_state = 0;
        if(!net->put_aut_name(aut, (char*) yytext)){
          type_error(Parser_03);
        }
        replic_map.put(aut, aut_group);
        aut++;
      }
      if(tour>=3){
        state = 0;
      }
    ;}
    break;

  case 106:
#line 612 "./src/lb/glc.y"
    { if(tour==2){
        for(int i=1;i<=replic;i++){
          if(!net->put_aut_n_state(aut-i, n_state)){
            not_error = 0;
          }
          alloc_mat_state(aut-i, n_state);
        }
        aut_group++;
      }
    ;}
    break;

  case 107:
#line 623 "./src/lb/glc.y"
    { if(tour>=3){
        aut+= replic;
      }
    ;}
    break;

  case 111:
#line 641 "./src/lb/glc.y"
    { if(tour==2)
        n_state++;
      f_t = 0;
    ;}
    break;

  case 112:
#line 646 "./src/lb/glc.y"
    { strcpy(fname,(char*)yytext);
      if(tour==3){
        n_trans = 0;
        if(t_ident->get_name((char*) yytext)>0){
          type_error(Parser_06);
        }
        for(int i=0; i<replic; i++){
          if(!net->put_state_name(aut+i, state, (char*) yytext)){
            type_error(Parser_05);
          }
          net->put_state_reward(aut+i, state, state);
        }
        state++;
        queue = 1;
      }	
      if(tour>=4){
        state++;
        queue = 1;	
        trans = 0;
      }
      queue_p = 0;
      queue_m = 0;
    ;}
    break;

  case 113:
#line 670 "./src/lb/glc.y"
    { if(tour==3){
        for(int i=0;i<replic;i++){    
          for(int j=1;j<=queue;j++){
            mat_trans[aut+i][state-j] = n_trans;
            if(queue_p>0 && queue_p>=j && queue>1)
              mat_trans[aut+i][state-j]--;
            if(queue_m<0 && ((queue-j)<abs(queue_m))&& queue>1){
              mat_trans[aut+i][state-j]--;
            }
          }
        }
        
      }
      if(tour>=4){
        state+=queue-1;
      }
    ;}
    break;

  case 117:
#line 698 "./src/lb/glc.y"
    { strcpy(temp,(char*)yytext);
      f_t = 1;
    ;}
    break;

  case 118:
#line 702 "./src/lb/glc.y"
    { if(tour>=3){
        from_dst = net->get_name(temp,aut);
        if(from_dst<0){
          yynerrs++;
          not_error = 0;
          Semantic_Error_YACC(yynerrs, yylineno, temp, Parser_100);
          from_dst = 0;
        }
      }
    ;}
    break;

  case 121:
#line 718 "./src/lb/glc.y"
    { sprintf(temp,"%s[%s]",temp,(char*)yytext);
     ;}
    break;

  case 124:
#line 730 "./src/lb/glc.y"
    { if (tour==3){
        n_trans++;
        if(f_t)
          for(int i=0;i<replic;i++){    
            mat_trans[aut+i][from_dst]++;
          }
      }
      n_mst = 0;
      n_sync = 0;
      n_local = 0;  
    ;}
    break;

  case 125:
#line 742 "./src/lb/glc.y"
    { if(tour==4){
        if(f_t==0){
     	for(int i=0; i<replic; i++){
      	  for(int j=abs(queue_m); j<(queue-queue_p); j++){	
              if(!net->put_event_n_mst(aut+i, state+j-1, n_mst))
                not_error = 0;
              if(!net->put_event_n_sync(aut+i, state+j-1, n_sync))
                not_error = 0;
              if(!net->put_event_n_local(aut+i, state+j-1, n_local))
                not_error = 0;
//              cout << "aloc(" << aut+i << "," << state+j-1 << "): m(" << n_mst << ") - s(" << n_sync <<  ") - l(" << n_local << ")\n";
            }
          }
        }
        else{
      	for(int i=0; i<replic; i++){
            if(!net->put_event_n_mst(aut+i, from_dst, n_mst))
              not_error = 0;
            if(!net->put_event_n_sync(aut+i, from_dst, n_sync))
              not_error = 0;
            if(!net->put_event_n_local(aut+i, from_dst, n_local))
              not_error = 0;
//              cout << "+aloc(" << aut+i << "," << from_dst << "): m(" << n_mst << ") - s(" << n_sync <<  ") - l(" << n_local << ")\n";

          }
        }
      }
    ;}
    break;

  case 129:
#line 780 "./src/lb/glc.y"
    {if(tour>=4)
  	 sprintf(aux_dst,"%s",(char*) yytext);
    ;}
    break;

  case 130:
#line 784 "./src/lb/glc.y"
    { if(tour>=4){
        if(f_t==0){
          for(int i=0; i<replic; i++){
            for(int j=0; j<queue; j++){	
              from_dst = net->put_trans_dst(aut+i,state+j-1,1,aux_dst);
              if(from_dst<0){
                type_error(Parser_100); 
              }  
            }
          }
        }
        else{
          for(int i=0; i<replic; i++){
            if(net->put_from_dst(aut+i,from_dst,aux_dst)<0){
              type_error(Parser_100); 
            }  
          }
        }
        queue_p = 0;
        queue_m = 0;
      }
    ;}
    break;

  case 131:
#line 807 "./src/lb/glc.y"
    { if(tour>=4){
        if(f_t==0){
          for(int i=0; i<replic; i++){
            for(int j=0; j<queue; j++){	
        	  net->put_trans_dst(aut+i,state+j-1,4,(char*) yytext);
        	  from_dst = state-1;
            }
          }
        }
        else{
          type_error(Parser_115); 
        }
        queue_p = 0;
        queue_m = 0;
      }
    ;}
    break;

  case 132:
#line 824 "./src/lb/glc.y"
    { if(tour>=4){
        if(f_t==0){   
          for(int i=0; i<replic; i++){
            for(int j=0; j<queue-1; j++){	
              from_dst = net->put_trans_dst(aut+i,state+j-1,2,(char*) yytext);
              if(from_dst<0){
                type_error(Parser_101); 
              }  
            }
          }
          if(queue==1){
            type_error(Parser_101); 
          }
        }
        else{
          type_error(Parser_115); 
        }
        queue_m = 0;        
      }
      queue_p = 1;
    ;}
    break;

  case 133:
#line 846 "./src/lb/glc.y"
    { if(tour>=4){
        if(f_t==0){
          for(int i=0; i<replic; i++){
            for(int j=1; j<queue; j++){	
              from_dst = net->put_trans_dst(aut+i,state+j-1,3,(char*) yytext);
              if(from_dst<0){
                type_error(Parser_101); 
              }  
            }
          }
          if(queue==1){
            type_error(Parser_101); 
          }
        }
        else{
          type_error(Parser_115); 
        }
        queue_p = 0;                
      }
      queue_m = -1;
    ;}
    break;

  case 134:
#line 868 "./src/lb/glc.y"
    { if(tour>=4){
        if(f_t==0){
          for(int i=0; i<replic; i++){
            for(int j=atol((char*)yytext); j<queue; j++){	
          	sprintf(aux_dst,"-%s",(char*) yytext);
              from_dst = net->put_trans_dst(aut+i,state+j-1,5,aux_dst);
              if(from_dst<0){
                type_error(Parser_101); 
              }  
            }
          }
          if(queue==1){
            type_error(Parser_101); 
          }          
        }
        else{
          type_error(Parser_115); 
        }
        queue_p = 0;
      }
      queue_m = - atol((char*) yytext);
    ;}
    break;

  case 135:
#line 891 "./src/lb/glc.y"
    { if(tour>=4){
        if(f_t==0){
          for(int i=0; i<replic; i++){
            for(int j=0; j<queue-atol((char*)yytext); j++){	
          	sprintf(aux_dst,"-%s",(char*) yytext);
              from_dst = net->put_trans_dst(aut+i,state+j-1,5,aux_dst);
              if(from_dst<0){
                type_error(Parser_101); 
              }  
            }
          }
          if(queue==1){
            type_error(Parser_101); 
          }          
        }
        else{
          type_error(Parser_115); 
        }
        queue_m = 0;
      }
      queue_p = atol((char*) yytext);
    ;}
    break;

  case 136:
#line 917 "./src/lb/glc.y"
    { if(tour>=4)
        sprintf(aux_dst,"%s[%s]",aux_dst,(char*) yytext);
    ;}
    break;

  case 139:
#line 925 "./src/lb/glc.y"
    {strcpy(event_name, (char*)yytext);;}
    break;

  case 140:
#line 926 "./src/lb/glc.y"
    { if(tour>=4){
        if(edic->get_type(event_name)<0){
          type_error(Parser_04);
      }
      else{
        event_type = edic->get_type(event_name);
        event_rate = edic->get_f_rate(event_name);
        if(event_rate<0){
          edic->get_rate(event_name, fname);
        }
        switch(event_type){
          case 0: if(tour==4) // local event
                    n_local++;
                  if(tour==5){
                    if(f_t==0){
                      for(int i=0; i<replic; i++){
      	              for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                          if(net->put_local_name(aut+i, state+j-1, event_name)==0){
                            type_error(Parser_112);
                          }
                  	    if((from_dst+j)==(state+j-1)){
                            net->put_local_rate(aut+i, state+j-1,  0, "0", -1);
                            cout << "Semantic Warning - " << event_name << " - (** " \
                                 << Parser_111 <<  " **)." \
                                 << endl;
                          }
                          else{
                            if(event_rate>=0){
                              net->put_local_rate(aut+i, state+j-1, 1, "0", event_rate);
                            }
                            else{
                              net->put_local_rate(aut+i, state+j-1, 0, fname, -1);
                            }
                          }
                        }
                      }
                    } 
                    else{
                      for(int i=0; i<replic; i++){
                        if(net->put_local_name(aut+i, from_dst, event_name)==0){
                          type_error(Parser_112);
                        }
                	    if(from_dst==net->get_name(aux_dst,aut+i)){
                          net->put_local_rate(aut+i, from_dst, 0, "0", -1);
                          cout << "Semantic Warning - " << event_name << " - (** " \
                               << Parser_111 <<  " **)." \
                               << endl;
                        }
                        else{
                          if(event_rate>=0){
                            net->put_local_rate(aut+i, from_dst, 1, "0", event_rate);
                          }
                          else{
                            net->put_local_rate(aut+i, from_dst, 0, fname, -1);
                          }
                        }
                      }
                    }
                  }
            break; 
          case 1: if(edic->get_mst(event_name)==aut || edic->get_mst(event_name)<0){ // synchronized event
                    if(tour==4)
                      n_mst++;
                      edic->put_mst(event_name, aut);                      
                    if(tour==5){
                      if(f_t==0){
                      	for(int i=0; i<replic; i++){
          	              for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                            if(net->put_mst_name(aut+i, state+j-1, event_name)==0){
                              type_error(Parser_112);
                            }
                            if(event_rate>=0){
                              net->put_mst_rate(aut+i, state+j-1, 1, "-1", event_rate);
                            }
                            else{
                              net->put_mst_rate(aut+i, state+j-1, 0, fname, -1);                          
                            }
                            event_mst = 1;
                          }
                        }
                      }
                      else{
                    	  for(int i=0; i<replic; i++){
                          if(net->put_mst_name(aut+i, from_dst, event_name)==0){
                            type_error(Parser_112);
                          }
                          if(event_rate>=0){
                            net->put_mst_rate(aut+i, from_dst, 1, "-1",event_rate);
                          }
                          else{
                            net->put_mst_rate(aut+i, from_dst, 0, fname, -1);                          
                          }
                          event_mst = 1;                    	  
                        }
                      }
                    }  
                  }
                  else{
                    if(tour==4)
                      n_sync++;
                    if(tour==5){
                      if(f_t==0){
                    	for(int i=0; i<replic; i++){ 
        	              for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                            if(net->put_sync_name(aut+i, state+j-1, event_name)==0){
                              type_error(Parser_112);
                            }            	
                            event_mst = 0;
                          }
                        }
                      }  
                      else{
                      	for(int i=0; i<replic; i++){ 
                          if(net->put_sync_name(aut+i, from_dst, event_name)==0){
                            type_error(Parser_112);
                          }            	
                          event_mst = 0;
                        }
                      }
                    }                  
                  }
            break;
          }
        }
      }
    ;}
    break;

  case 144:
#line 1062 "./src/lb/glc.y"
    { if(tour>=4)
        sprintf(event_name,"%s[%s]",event_name,(char*) yytext);
    ;}
    break;

  case 149:
#line 1076 "./src/lb/glc.y"
    { switch(event_type){
        case 0: if(tour==5){
              	if(event_rate>=0){
              	  net->get_ident_exp(event_rate, fname);
              	}
              	sprintf(tmp1,"%s %s * ", yytext, fname);
              	if(net->get_ident_exp(tmp1)<0){
              	  net->put_ident_name("new_rate");
              	  net->put_ident_exp(tmp1);
              	}
              	if(f_t==0){
                	for(int i=0; i<replic; i++){
    	              for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                	    if((from_dst+j)==(state+j-1))
                          net->put_local_rate(aut+i, state+j-1, 0, "0", -1);
                        else
                          net->put_local_rate(aut+i, state+j-1, 1, (char *)yytext, net->get_ident_exp(tmp1));
                      }
                    }
                  }
                  else{
                    for(int i=0; i<replic; i++){
               	   if(from_dst==net->get_name(aux_dst,aut+i)){
                        net->put_local_rate(aut+i, from_dst, 0, "0", -1);
                      }
                      else{
                        net->put_local_rate(aut+i, from_dst, 1, (char *)yytext, net->get_ident_exp(tmp1));
                      }
                    }
                  }
                }
          break; 
        case 1: if(tour==5){
        	      if(edic->get_mst(event_name)==aut || edic->get_mst(event_name)<0){
                	if(event_rate>=0){
                	  net->get_ident_exp(event_rate, fname);                  
                	}
                	sprintf(tmp1,"%s %s *", yytext, fname);
                	if(net->get_ident_exp(tmp1)<0){
                	  net->put_ident_name("new_rate");
                	  net->put_ident_exp(tmp1);
                	}
                	if(f_t==0){
                  	for(int i=0; i<replic; i++){
      	              for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                  	    net->put_mst_rate(aut+i, state+j-1, 1, (char*) yytext, net->get_ident_exp(tmp1));
                        }
                      }
                    }
                    else{
                  	for(int i=0; i<replic; i++){
                	      net->put_mst_rate(aut+i, from_dst, 1, (char*) yytext, net->get_ident_exp(tmp1));
                      }
                    }
                  }
                  else{
                    if(f_t==0){
                  	for(int i=0; i<replic; i++){
    	                for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                          net->put_sync_prob(aut+i, state+j-1, 0, (char*) yytext, -1);
                        }
                      }
                    }
                    else{
                  	for(int i=0; i<replic; i++){
                        net->put_sync_prob(aut+i, from_dst, 0, (char*) yytext, -1);
                      }
                    }
                  }
                }
          break;
      }
    ;}
    break;

  case 150:
#line 1150 "./src/lb/glc.y"
    { switch(event_type){
        case 0: if(tour==5){
          	    if(net->get_ident_name((char *) yytext)<0){
          	      type_error(Parser_01);
          	    }
          	    else{
                	if(event_rate>=0){
              	    net->get_ident_exp(event_rate, fname);
              	  }
                	net->get_ident_exp((char*) yytext, tmp1);
              	  sprintf(tmp1,"%s %s * ", tmp1, fname);
                	if(net->get_ident_exp(tmp1)<0){
                	  net->put_ident_name("new_rate");
                	  net->put_ident_exp(tmp1);
              	  }
                    if(f_t==0){
                  	for(int i=0; i<replic; i++){
      	              for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                  	    if((from_dst+j)==(state+j-1))
                            net->put_local_rate(aut+i, state+j-1, 0, "0", -1);
                          else
                            net->put_local_rate(aut+i, state+j-1, 1, "0", net->get_ident_exp(tmp1));
                        }
                      }
                    }
                    else{
                  	for(int i=0; i<replic; i++){
                 	   if(from_dst==net->get_name(aux_dst,aut+i)){
                          net->put_local_rate(aut+i, from_dst, 0, "0", -1);
                        }
                        else{
                          net->put_local_rate(aut+i, from_dst, 1, "0", net->get_ident_exp(tmp1));
                        }
                      }
                    }
                  }
                }
          break; 
        case 1: if(tour==5){
                  if(edic->get_mst(event_name)==aut || edic->get_mst(event_name)<0){
          	      if(net->get_ident_name((char *) yytext)<0){
            	      type_error(Parser_01);
                    }
            	    else{
                  	if(event_rate>=0){
                	    net->get_ident_exp(event_rate, fname);
                	  }
                	  net->get_ident_exp((char*) yytext, tmp1);
                	  sprintf(tmp1,"%s %s *", tmp1, fname);
                  	if(net->get_ident_exp(tmp1)<0){
                  	  net->put_ident_name("new_rate");
                  	  net->put_ident_exp(tmp1);
                	  }
                	  if(f_t==0){
                    	for(int i=0; i<replic; i++){
      	                for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                    	    net->put_mst_rate(aut+i, state+j-1, 1, (char*) yytext, net->get_ident_exp(tmp1));
                          }
                        }
                      }
                      else{
                    	for(int i=0; i<replic; i++){
                  	    net->put_mst_rate(aut+i, from_dst, 1, (char*) yytext, net->get_ident_exp(tmp1));
                        }
                      }
                    }
                  }
                  else{
                    if(f_t==0){
                  	for(int i=0; i<replic; i++){
      	              for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                  	    if(net->get_ident_name((char *) yytext)<0){
    	                    type_error(Parser_01);
                  	    }
                  	    else{
                            net->put_sync_prob(aut+i, state+j-1,  1, (char *) yytext, net->get_ident_name((char *) yytext));
                          }
                        } 
                      }
                    }
                    else{
                  	for(int i=0; i<replic; i++){
                	    if(net->get_ident_name((char *) yytext)<0){
  	                    type_error(Parser_01);
                	    }
                	    else{
                          net->put_sync_prob(aut+i, from_dst, 1, (char *) yytext, net->get_ident_name((char *) yytext));
                        }
                      }
                    } 
                  }
                }
          break;
      }
    ;}
    break;

  case 153:
#line 1257 "./src/lb/glc.y"
    {di = atol((char*) yytext);;}
    break;

  case 154:
#line 1258 "./src/lb/glc.y"
    { dl = atol((char*) yytext);
      replic = dl - di+1;
      if(replic<1){
        Semantic_Error(fname, Parser_114);
        not_error = 0;
      }
      if(tour==1){
        n_aut+= replic - 1;
      }
      if(tour==2){
        aut--;
        net->get_aut_name(aut, temp);
        for(int i=di;i<=dl;i++){
          sprintf(aut_replic,"%s[%d]",temp,i);
          net->put_aut_name(aut, aut_replic);
          replic_map.put(aut, aut_group);
          aut++;
        }
      }
    ;}
    break;

  case 155:
#line 1279 "./src/lb/glc.y"
    { if(dom->get_name((char *) yytext)>=0){
        di = dom->get_first((char *) yytext);
        dl = dom->get_last((char *) yytext);
        replic = dl-di+1;
        if(tour==1){
          n_aut+= replic-1;
        }
        if(tour==2){
          aut--;
          net->get_aut_name(aut, temp);
          for(int i=di;i<=dl;i++){
            sprintf(aut_replic,"%s[%d]",temp,i);
            net->put_aut_name(aut, aut_replic);
            replic_map.put(aut, aut_group);
            aut++;
          }
        }
      }
      else {
        type_error(Parser_01);
      }
    ;}
    break;

  case 158:
#line 1309 "./src/lb/glc.y"
    {di = atol((char*) yytext);;}
    break;

  case 159:
#line 1310 "./src/lb/glc.y"
    { dl = atol((char*) yytext);
      queue = dl - di+1;
      if(queue<1){
        Semantic_Error(fname, Parser_114);
        not_error = 0;
      }
      if(tour==2){
        n_state+= queue-1;
      }
      if(tour==3){
        state--;
        net->get_state_name(aut, state, temp);
        for(int i=di;i<=dl;i++){
          sprintf(state_queue,"%s[%d]",temp,i);
          if(t_ident->get_name(state_queue)>0){
            type_error(Parser_06);
          }
          for(int j=0; j<replic; j++){
            net->put_state_name(aut+j, state, state_queue);
            net->put_state_reward(aut+j, state, state);
          }
          state++;
        }
      }
    ;}
    break;

  case 160:
#line 1336 "./src/lb/glc.y"
    { if(dom->get_name((char *) yytext)>=0){
        di = dom->get_first((char *) yytext);
        dl = dom->get_last((char *) yytext);
        queue = dl - di+1;
        if(tour==2){
          n_state+= queue-1;
        }
        if(tour==3){
          state--;
          net->get_state_name(aut, state, temp);
          for(int i=di;i<=dl;i++){
            sprintf(state_queue,"%s[%d]",temp,i);
            if(t_ident->get_name(state_queue)>0){
              type_error(Parser_06);
            }
            for(int j=0; j<replic; j++){
              net->put_state_name(aut+j, state, state_queue);
              net->put_state_reward(aut+j, state, state);
            }
            state++;
          }
        }
      }
      else {
        type_error(Parser_01);
      }
    ;}
    break;

  case 163:
#line 1373 "./src/lb/glc.y"
    { strcat(temp, (char*) yytext);
      if(tour==3){
        for(int i=0; i<replic;i++){
          for(int j=0; j<queue; j++){
    	    net->put_state_reward(aut+i,state-1-j,atof(temp));
    	  }
        }
      }   	
    ;}
    break;

  case 164:
#line 1383 "./src/lb/glc.y"
    { if(t_ident->get_name((char *) yytext)>=0){
    	if(t_ident->exp_value(t_ident->get_name((char *) yytext), &v_tmp)){
          if(tour==3){
            for(int i=0; i<replic;i++){
              for(int j=0; j<queue; j++){
    	        net->put_state_reward(aut+i,state-1-j,v_tmp);
    	      }
            }
          }
        }
        else{
          type_error(Parser_02);
        }
      }
      else{
        type_error(Parser_01);
      }   	
    ;}
    break;

  case 165:
#line 1408 "./src/lb/glc.y"
    {itop.reset(); ;}
    break;

  case 167:
#line 1414 "./src/lb/glc.y"
    { if(tour==4){
    	net->put_result_name((char *) yytext);
	    strcpy(fname,(char *) yytext);    	
	    ia = -1;
      }
    ;}
    break;

  case 168:
#line 1421 "./src/lb/glc.y"
    { if(tour==4){
    	itop.get_func(fnc);
        itop.reset();
        net->put_result_exp(fnc);
      }
    ;}
    break;

  case 171:
#line 1438 "./src/lb/glc.y"
    {strcpy(temp, "");;}
    break;

  case 172:
#line 1439 "./src/lb/glc.y"
    {strcpy(temp, "-");;}
    break;

  case 173:
#line 1440 "./src/lb/glc.y"
    {strcpy(temp, "");;}
    break;


    }

/* Line 1000 of yacc.c.  */
#line 2806 "./src/lb/yacc.C"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
		 yydestruct (yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
	  yydestruct (yytoken, &yylval);
	  yychar = YYEMPTY;

	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 1467 "./src/lb/glc.y"



void Init_Variables()
{
  yylineno = 0;
}


//int main()
//void Compile_Network(peps_name name, const char* path_, const char* results_path_)
void Compile_Network(const peps_name san_level_name, const peps_name des_level_name)
{ 
  peps_name n_tmp;
  peps_name n_tmp1;

  net = new NET;
  t_ident = new FUNC;
  dom = new DOM;
  edic = new EDIC;

  sprintf(n_tmp, "%s.san", san_level_name);

  n_aut = 0;
  aut = 0;
  state = 0;
  tour = 0;
  n_events = 0;

  t_ident->alloc(2); // auxiliar table to identifiers
  dom->alloc(1); // auxiliar table to dominion
  net->reset();
   
  Init_Variables();
  tour++; // first pass
  yyin = fopen(n_tmp, "r");
  yyparse();
  fclose(yyin);


  if(not_error){
    aut_group = 0;
    not_error = net->put_net_n_aut(n_aut); // alloc n_aut automata
    replic_map.create(n_aut); // create replic map to n_aut automata    
    alloc_mat_aut(n_aut); // alloc matrix to store automata-state
    edic->put_n_event(n_events);
    Init_Variables();
    net->reset();    
    tour++; // second pass
    yyin = fopen(n_tmp, "r");
    yyrestart(yyin);
    yyparse();
    fclose(yyin);
  }

  if(not_error){
    Init_Variables();
    net->reset();
    tour++; // third pass
    yyin = fopen(n_tmp, "r");
    yyrestart(yyin);
    yyparse();
    fclose(yyin);
    if(!net->put_net_n_ident(2))// alloc n_ident identifiers structure
      not_error = 0; 
    if(!net->put_net_n_result(2)) // alloc n_result results structure
      not_error = 0; 

    for(int i=0; i<net->get_net_n_aut(); i++){
      for(int j=0; j<net->get_aut_n_state(i); j++){
        if(!net->put_state_n_trans(i, j, mat_trans[i][j]))
          not_error = 0;
      }
    }
  }

  if(not_error){
    Init_Variables();
    net->reset();
    tour++; // fourth pass
    yyin = fopen(n_tmp, "r");
    yyrestart(yyin);
    yyparse();
    fclose(yyin);
    if(!net->rm_ident_cnst()){
      not_error = 0;
    }
    if(!net->rm_reach_cnst()){
      not_error = 0;     
    }        
    if(!net->rm_result_cnst()){
      not_error = 0;     
    }
  }

  if(not_error){
    Init_Variables();
    net->reset();
    tour++; // fifth pass
    yyin = fopen(n_tmp, "r");
    yyrestart(yyin);
    yyparse();
    fclose(yyin);
  }

  if(not_error){
    strcpy(n_tmp, des_level_name);  	
    net->put2file_des(n_tmp);
    net->put2file_dic(n_tmp);
    net->put2file_tft(n_tmp);
    if(!r_t){
      net->put2file_fct(n_tmp);
      cout << endl;
    }
    else{
      if(!Exists_File(n_tmp, fct_file)){
        net->put2file_fct(n_tmp);
        cout << " (partial reachable state space)\n";
      }
      else{
        cout << " :-) existing file '" << n_tmp << ".fct ' used as reachable state space\n";  
      }
    } 
    net->put2file_res(n_tmp);
  } 

  itop.reset();

  if(net)
    delete net;
  if(t_ident)
    delete t_ident;
  if(dom)
    delete dom;
  if(edic)
    delete edic;

  if(!not_error)
    exit(0);
}

