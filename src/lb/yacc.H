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




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



