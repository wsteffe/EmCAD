/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_EMCAD_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED
# define YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_EMCAD_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int projectDatadebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INTEGER = 258,
    FLOATING = 259,
    STRING = 260,
    NAME = 261,
    DEF = 262,
    MAINASSNAME = 263,
    NETWORK = 264,
    LENGTH = 265,
    FREQ = 266,
    UNIT = 267,
    EXP = 268,
    MESH = 269,
    WAVELENGTH = 270,
    RATIO = 271,
    ANA = 272,
    BAND = 273,
    NUM = 274,
    MOR = 275,
    RESPONSE = 276,
    PARAMETER = 277,
    TYPE = 278,
    PART = 279,
    XSCALE = 280,
    YSCALE = 281,
    AUTO = 282,
    ZERO = 283,
    POLE = 284,
    CURVE = 285,
    FILTER = 286,
    PASS = 287,
    ORDER = 288,
    RETURNLOSS = 289,
    QFACTOR = 290,
    MAPPING = 291,
    METHOD = 292,
    ITERMAX = 293,
    AUTOMATIC = 294,
    MAPPED = 295,
    TX = 296,
    ZEROS = 297,
    REMESH = 298,
    FIRST = 299,
    DECOMPOSITION = 300,
    MATERIAL = 301,
    MODELIZATION = 302,
    COMPONENT = 303,
    SAVE = 304,
    REDUCTION = 305,
    RELOAD = 306,
    NEEDED = 307,
    DONE = 308,
    CHANGED = 309
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 66 "/home/walter/MwCAD/EmCAD/EmCAD/src/GUI/projectData.y" /* yacc.c:1909  */

int	ival;
double	fval;
char	*sval;

#line 115 "/home/walter/MwCAD/EmCAD/EmCAD/src/GUI/projectData_parser.hpp" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE projectDatalval;

int projectDataparse (void);

#endif /* !YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_EMCAD_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED  */
