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

#ifndef YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED
# define YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED
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
    VARFILEPATH = 264,
    S2PFILEPATH = 265,
    NETWORK = 266,
    LENGTH = 267,
    FREQ = 268,
    UNIT = 269,
    EXP = 270,
    MESH = 271,
    REFINE = 272,
    ENERGY = 273,
    WAVELENGTH = 274,
    LOCAL = 275,
    MESHING3D = 276,
    ANA = 277,
    BAND = 278,
    NUM = 279,
    MOR = 280,
    RESPONSE = 281,
    SYMMETRIC = 282,
    PARAMETER = 283,
    TYPE = 284,
    TOPOLOGY = 285,
    PART = 286,
    XSCALE = 287,
    YSCALE = 288,
    AUTO = 289,
    ZERO = 290,
    POLE = 291,
    WINDOW = 292,
    CURVE = 293,
    CIRCLE = 294,
    IDEAL = 295,
    CANONICAL = 296,
    FILTER = 297,
    CUSTOM = 298,
    PASS = 299,
    KRYLOV = 300,
    ORDER = 301,
    RETURNLOSS = 302,
    OUTBAND = 303,
    MAPPING = 304,
    TUNING = 305,
    METHOD = 306,
    SOURCE = 307,
    ITERMAX = 308,
    AUTOMATIC = 309,
    DESIGN = 310,
    MAPPED = 311,
    TX = 312,
    ZEROS = 313,
    PORT = 314,
    IMPEDANCE = 315,
    TARGET = 316,
    QFACTOR = 317,
    UNIFORM = 318,
    INDUCTIVE = 319,
    SKIN = 320,
    LOSSY = 321,
    PREDISTORTED = 322,
    OPTIMIZE = 323,
    ILOSSPP = 324,
    TRUSTR = 325,
    CUTOFF = 326,
    RATIO = 327,
    RECOMPUTE = 328,
    JACOBIAN = 329,
    ERRORT = 330,
    ONLY = 331,
    TRANSVERSEJ = 332,
    XTOL = 333,
    GRADDX = 334,
    REMESH = 335,
    FIRST = 336,
    DECOMPOSITION = 337,
    MATERIAL = 338,
    MODELIZATION = 339,
    COMPONENT = 340,
    SAVE = 341,
    REDUCTION = 342,
    RELOAD = 343,
    NEEDED = 344,
    DONE = 345,
    CHANGED = 346,
    tMIN = 347,
    tMAX = 348
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 66 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"

int	ival;
double	fval;
char	*sval;

#line 157 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE projectDatalval;

int projectDataparse (void);

#endif /* !YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED  */
