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
    MWM_VOLUME = 273,
    MWM_UNITS = 274,
    MWM_LINEPORT = 275,
    LENGTH = 276,
    FREQUENCY = 277,
    BAND = 278,
    RESISTANCE = 279,
    SURFACERESISTANCE = 280,
    MESHREFINEMENT = 281,
    COMPSOLID = 282,
    VOLTYPE = 283,
    EPSILONR = 284,
    MUR = 285,
    EPSLORENTZ = 286,
    MULORENTZ = 287,
    ECONDUCTIVITY = 288,
    HCONDUCTIVITY = 289,
    ETANDELTA = 290,
    HTANDELTA = 291,
    MATERIAL = 292,
    COLOR = 293,
    VOLUMES = 294,
    DISABLED = 295,
    tDIELECTRIC = 296,
    tHOLE = 297,
    tBOUNDARYCONDITION = 298,
    tWAVEGUIDE = 299,
    tLINEPORT = 300,
    tSPLITTER = 301,
    tGRID = 302,
    tASSEMBLY = 303,
    tUNDEFINED = 304,
    SOLID = 305
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 94 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1909  */

int	ival;
double	fval;
char	*sval;

#line 111 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.hpp" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE modellval;

int modelparse (void);

#endif /* !YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED  */
