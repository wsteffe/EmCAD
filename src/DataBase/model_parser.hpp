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

#ifndef YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_DEV_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED
# define YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_DEV_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int modeldebug;
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
    NAME = 261,                    /* NAME  */
    IMPORT = 262,                  /* IMPORT  */
    ASSEMBLYTYPE = 263,            /* ASSEMBLYTYPE  */
    LEVEL = 264,                   /* LEVEL  */
    DEF = 265,                     /* DEF  */
    SET = 266,                     /* SET  */
    CIRCUITNAME = 267,             /* CIRCUITNAME  */
    DEFAULTBC = 268,               /* DEFAULTBC  */
    MWM_MATERIAL = 269,            /* MWM_MATERIAL  */
    TEMPORTSNUM = 270,             /* TEMPORTSNUM  */
    TEPORTSNUM = 271,              /* TEPORTSNUM  */
    TMPORTSNUM = 272,              /* TMPORTSNUM  */
    GRIDNUM = 273,                 /* GRIDNUM  */
    PML = 274,                     /* PML  */
    INVARIANT = 275,               /* INVARIANT  */
    TRANSLATION = 276,             /* TRANSLATION  */
    ROTATION = 277,                /* ROTATION  */
    ANGLE = 278,                   /* ANGLE  */
    ORIGIN = 279,                  /* ORIGIN  */
    AXIS = 280,                    /* AXIS  */
    MWM_VOLUME = 281,              /* MWM_VOLUME  */
    MWM_INVARIANT = 282,           /* MWM_INVARIANT  */
    MWM_UNITS = 283,               /* MWM_UNITS  */
    MWM_LINEPORT = 284,            /* MWM_LINEPORT  */
    LENGTH = 285,                  /* LENGTH  */
    FREQUENCY = 286,               /* FREQUENCY  */
    BAND = 287,                    /* BAND  */
    SURFACE = 288,                 /* SURFACE  */
    RESISTANCE = 289,              /* RESISTANCE  */
    INDUCTANCE = 290,              /* INDUCTANCE  */
    IMPEDANCE = 291,               /* IMPEDANCE  */
    LOSSFACTOR = 292,              /* LOSSFACTOR  */
    QFACTOR = 293,                 /* QFACTOR  */
    SURFRATIO = 294,               /* SURFRATIO  */
    BALLRADIUS = 295,              /* BALLRADIUS  */
    ROUGH = 296,                   /* ROUGH  */
    MODELTYPE = 297,               /* MODELTYPE  */
    RZ = 298,                      /* RZ  */
    RQ = 299,                      /* RQ  */
    MESHREFINEMENT = 300,          /* MESHREFINEMENT  */
    CUTOFFREFINEMENT = 301,        /* CUTOFFREFINEMENT  */
    COMPSOLID = 302,               /* COMPSOLID  */
    VOLTYPE = 303,                 /* VOLTYPE  */
    EPSILONR = 304,                /* EPSILONR  */
    MUR = 305,                     /* MUR  */
    EPSLORENTZ = 306,              /* EPSLORENTZ  */
    MULORENTZ = 307,               /* MULORENTZ  */
    POLESRESIDUES = 308,           /* POLESRESIDUES  */
    POLESNUM = 309,                /* POLESNUM  */
    ECONDUCTIVITY = 310,           /* ECONDUCTIVITY  */
    HCONDUCTIVITY = 311,           /* HCONDUCTIVITY  */
    ETANDELTA = 312,               /* ETANDELTA  */
    HTANDELTA = 313,               /* HTANDELTA  */
    MATERIAL = 314,                /* MATERIAL  */
    COLOR = 315,                   /* COLOR  */
    VOLUMES = 316,                 /* VOLUMES  */
    DISABLED = 317,                /* DISABLED  */
    tDIELECTRIC = 318,             /* tDIELECTRIC  */
    tHOLE = 319,                   /* tHOLE  */
    tBOUNDARYCONDITION = 320,      /* tBOUNDARYCONDITION  */
    tWAVEGUIDE = 321,              /* tWAVEGUIDE  */
    tLINEPORT = 322,               /* tLINEPORT  */
    tSPLITTER = 323,               /* tSPLITTER  */
    tGRID = 324,                   /* tGRID  */
    tCOMPONENT = 325,              /* tCOMPONENT  */
    tINTERFACE = 326,              /* tINTERFACE  */
    tNET = 327,                    /* tNET  */
    tASSEMBLY = 328,               /* tASSEMBLY  */
    tUNDEFINED = 329,              /* tUNDEFINED  */
    SOLID = 330                    /* SOLID  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 97 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"

int	ival;
double	fval;
char	*sval;

#line 145 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE modellval;


int modelparse (void);


#endif /* !YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_DEV_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED  */
