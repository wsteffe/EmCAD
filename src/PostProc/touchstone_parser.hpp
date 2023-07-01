/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_TOUCHSTONE_HOME_WALTER_MWCAD_EMCAD_EMCAD_DEV_SRC_POSTPROC_TOUCHSTONE_PARSER_HPP_INCLUDED
# define YY_TOUCHSTONE_HOME_WALTER_MWCAD_EMCAD_EMCAD_DEV_SRC_POSTPROC_TOUCHSTONE_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int touchstonedebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INTEGER = 258,                 /* INTEGER  */
    FLOATING = 259,                /* FLOATING  */
    STRING = 260,                  /* STRING  */
    PORTNAME = 261,                /* PORTNAME  */
    NUMBEROFPORTS = 262,           /* NUMBEROFPORTS  */
    NUMBEROFFREQ = 263,            /* NUMBEROFFREQ  */
    TWOPORTDATAORDER = 264,        /* TWOPORTDATAORDER  */
    ORDER_21_12 = 265,             /* ORDER_21_12  */
    ORDER_12_21 = 266,             /* ORDER_12_21  */
    NETWORKDATA = 267,             /* NETWORKDATA  */
    MATRIXFORMAT = 268,            /* MATRIXFORMAT  */
    FULL = 269,                    /* FULL  */
    OPTIONS = 270,                 /* OPTIONS  */
    HZ = 271,                      /* HZ  */
    KHZ = 272,                     /* KHZ  */
    MHZ = 273,                     /* MHZ  */
    GHZ = 274,                     /* GHZ  */
    tDB = 275,                     /* tDB  */
    tMA = 276,                     /* tMA  */
    tRI = 277,                     /* tRI  */
    tS = 278,                      /* tS  */
    tY = 279,                      /* tY  */
    tZ = 280,                      /* tZ  */
    tH = 281,                      /* tH  */
    tG = 282,                      /* tG  */
    tR = 283                       /* tR  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 73 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/PostProc/touchstone.y"

int	ival;
double	fval;
char	*sval;

#line 98 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/PostProc/touchstone_parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE touchstonelval;


int touchstoneparse (void);


#endif /* !YY_TOUCHSTONE_HOME_WALTER_MWCAD_EMCAD_EMCAD_DEV_SRC_POSTPROC_TOUCHSTONE_PARSER_HPP_INCLUDED  */
