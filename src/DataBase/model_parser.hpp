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

#ifndef YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED
# define YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int modeldebug;
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
    IMPORT = 262,
    ASSEMBLYTYPE = 263,
    LEVEL = 264,
    DEF = 265,
    SET = 266,
    CIRCUITNAME = 267,
    DEFAULTBC = 268,
    MWM_MATERIAL = 269,
    TEMPORTSNUM = 270,
    TEPORTSNUM = 271,
    TMPORTSNUM = 272,
    GRIDNUM = 273,
    PML = 274,
    INVARIANT = 275,
    TRANSLATION = 276,
    ROTATION = 277,
    ANGLE = 278,
    ORIGIN = 279,
    AXIS = 280,
    MWM_VOLUME = 281,
    MWM_INVARIANT = 282,
    MWM_UNITS = 283,
    MWM_LINEPORT = 284,
    LENGTH = 285,
    FREQUENCY = 286,
    BAND = 287,
    SURFACE = 288,
    RESISTANCE = 289,
    INDUCTANCE = 290,
    IMPEDANCE = 291,
    LOSSFACTOR = 292,
    QFACTOR = 293,
    ROUGH = 294,
    MESHREFINEMENT = 295,
    COMPSOLID = 296,
    VOLTYPE = 297,
    EPSILONR = 298,
    MUR = 299,
    EPSLORENTZ = 300,
    MULORENTZ = 301,
    POLESRESIDUES = 302,
    POLESNUM = 303,
    ECONDUCTIVITY = 304,
    HCONDUCTIVITY = 305,
    ETANDELTA = 306,
    HTANDELTA = 307,
    MATERIAL = 308,
    COLOR = 309,
    VOLUMES = 310,
    DISABLED = 311,
    tDIELECTRIC = 312,
    tHOLE = 313,
    tBOUNDARYCONDITION = 314,
    tWAVEGUIDE = 315,
    tLINEPORT = 316,
    tSPLITTER = 317,
    tGRID = 318,
    tCOMPONENT = 319,
    tINTERFACE = 320,
    tNET = 321,
    tASSEMBLY = 322,
    tUNDEFINED = 323,
    SOLID = 324
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 97 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"

int	ival;
double	fval;
char	*sval;

#line 133 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE modellval;

int modelparse (void);

#endif /* !YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED  */
