/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
    FILTER = 296,
    PASS = 297,
    KRYLOV = 298,
    ORDER = 299,
    RETURNLOSS = 300,
    OUTBAND = 301,
    MAPPING = 302,
    TUNING = 303,
    METHOD = 304,
    SOURCE = 305,
    ITERMAX = 306,
    AUTOMATIC = 307,
    DESIGN = 308,
    MAPPED = 309,
    TX = 310,
    ZEROS = 311,
    PORT = 312,
    IMPEDANCE = 313,
    QFACTOR = 314,
    UNIFORM = 315,
    INDUCTIVE = 316,
    SKIN = 317,
    LOSSY = 318,
    PREDISTORTED = 319,
    OPTIMIZE = 320,
    ILOSSPP = 321,
    TRUSTR = 322,
    CUTOFF = 323,
    RATIO = 324,
    RECOMPUTE = 325,
    JACOBIAN = 326,
    ERRORT = 327,
    ONLY = 328,
    TRANSVERSEJ = 329,
    XTOL = 330,
    GRADDX = 331,
    REMESH = 332,
    FIRST = 333,
    DECOMPOSITION = 334,
    MATERIAL = 335,
    MODELIZATION = 336,
    COMPONENT = 337,
    SAVE = 338,
    REDUCTION = 339,
    RELOAD = 340,
    NEEDED = 341,
    DONE = 342,
    CHANGED = 343,
    tMIN = 344,
    tMAX = 345
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 66 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y" /* yacc.c:1909  */

int	ival;
double	fval;
char	*sval;

#line 151 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE projectDatalval;

int projectDataparse (void);

#endif /* !YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED  */
