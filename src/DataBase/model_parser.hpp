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

#ifndef YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED
# define YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED
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
    RESISTANCE = 288,
    SURFACERESISTANCE = 289,
    MESHREFINEMENT = 290,
    COMPSOLID = 291,
    VOLTYPE = 292,
    EPSILONR = 293,
    MUR = 294,
    EPSLORENTZ = 295,
    MULORENTZ = 296,
    ECONDUCTIVITY = 297,
    HCONDUCTIVITY = 298,
    ETANDELTA = 299,
    HTANDELTA = 300,
    MATERIAL = 301,
    COLOR = 302,
    VOLUMES = 303,
    DISABLED = 304,
    tDIELECTRIC = 305,
    tHOLE = 306,
    tBOUNDARYCONDITION = 307,
    tWAVEGUIDE = 308,
    tLINEPORT = 309,
    tSPLITTER = 310,
    tGRID = 311,
    tASSEMBLY = 312,
    tUNDEFINED = 313,
    SOLID = 314
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 97 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1909  */

int	ival;
double	fval;
char	*sval;

#line 120 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.hpp" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE modellval;

int modelparse (void);

#endif /* !YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED  */
