/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_TOUCHSTONE_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_POSTPROC_TOUCHSTONE_PARSER_HPP_INCLUDED
# define YY_TOUCHSTONE_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_POSTPROC_TOUCHSTONE_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int touchstonedebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INTEGER = 258,
    FLOATING = 259,
    STRING = 260,
    PORTNAME = 261,
    NUMBEROFPORTS = 262,
    NUMBEROFFREQ = 263,
    TWOPORTDATAORDER = 264,
    ORDER_21_12 = 265,
    ORDER_12_21 = 266,
    NETWORKDATA = 267,
    MATRIXFORMAT = 268,
    FULL = 269,
    OPTIONS = 270,
    HZ = 271,
    KHZ = 272,
    MHZ = 273,
    GHZ = 274,
    tDB = 275,
    tMA = 276,
    tRI = 277,
    tS = 278,
    tY = 279,
    tZ = 280,
    tH = 281,
    tG = 282,
    tR = 283
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 73 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/PostProc/touchstone.y"

int	ival;
double	fval;
char	*sval;

#line 92 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/PostProc/touchstone_parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE touchstonelval;

int touchstoneparse (void);

#endif /* !YY_TOUCHSTONE_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_POSTPROC_TOUCHSTONE_PARSER_HPP_INCLUDED  */
