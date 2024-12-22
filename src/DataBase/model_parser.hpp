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
    TEMTABULARORDER1 = 273,        /* TEMTABULARORDER1  */
    TEMTABULARORDER2 = 274,        /* TEMTABULARORDER2  */
    TEMREFCONDUCTOR = 275,         /* TEMREFCONDUCTOR  */
    DISCONNECTEDTEM = 276,         /* DISCONNECTEDTEM  */
    ORIENTATION = 277,             /* ORIENTATION  */
    GRIDNUM = 278,                 /* GRIDNUM  */
    PML = 279,                     /* PML  */
    INVARIANT = 280,               /* INVARIANT  */
    TRANSLATION = 281,             /* TRANSLATION  */
    ROTATION = 282,                /* ROTATION  */
    ANGLE = 283,                   /* ANGLE  */
    ORIGIN = 284,                  /* ORIGIN  */
    AXIS = 285,                    /* AXIS  */
    MWM_VOLUME = 286,              /* MWM_VOLUME  */
    MWM_INVARIANT = 287,           /* MWM_INVARIANT  */
    MWM_UNITS = 288,               /* MWM_UNITS  */
    MWM_LINEPORT = 289,            /* MWM_LINEPORT  */
    LENGTH = 290,                  /* LENGTH  */
    FREQUENCY = 291,               /* FREQUENCY  */
    BAND = 292,                    /* BAND  */
    SURFACE = 293,                 /* SURFACE  */
    RESISTANCE = 294,              /* RESISTANCE  */
    INDUCTANCE = 295,              /* INDUCTANCE  */
    IMPEDANCE = 296,               /* IMPEDANCE  */
    LOSSFACTOR = 297,              /* LOSSFACTOR  */
    QFACTOR = 298,                 /* QFACTOR  */
    SURFRATIO = 299,               /* SURFRATIO  */
    BALLRADIUS = 300,              /* BALLRADIUS  */
    ROUGH = 301,                   /* ROUGH  */
    MODELTYPE = 302,               /* MODELTYPE  */
    RZ = 303,                      /* RZ  */
    RQ = 304,                      /* RQ  */
    MESHREFINEMENT = 305,          /* MESHREFINEMENT  */
    CUTOFFREFINEMENT = 306,        /* CUTOFFREFINEMENT  */
    COMPSOLID = 307,               /* COMPSOLID  */
    VOLTYPE = 308,                 /* VOLTYPE  */
    EPSILONR = 309,                /* EPSILONR  */
    MUR = 310,                     /* MUR  */
    EPSLORENTZ = 311,              /* EPSLORENTZ  */
    MULORENTZ = 312,               /* MULORENTZ  */
    POLESRESIDUES = 313,           /* POLESRESIDUES  */
    POLESNUM = 314,                /* POLESNUM  */
    ECONDUCTIVITY = 315,           /* ECONDUCTIVITY  */
    HCONDUCTIVITY = 316,           /* HCONDUCTIVITY  */
    ETANDELTA = 317,               /* ETANDELTA  */
    HTANDELTA = 318,               /* HTANDELTA  */
    MATERIAL = 319,                /* MATERIAL  */
    COLOR = 320,                   /* COLOR  */
    VOLUMES = 321,                 /* VOLUMES  */
    DISABLED = 322,                /* DISABLED  */
    tDIELECTRIC = 323,             /* tDIELECTRIC  */
    tHOLE = 324,                   /* tHOLE  */
    tBOUNDARYCONDITION = 325,      /* tBOUNDARYCONDITION  */
    tWAVEGUIDE = 326,              /* tWAVEGUIDE  */
    tLINEPORT = 327,               /* tLINEPORT  */
    tSPLITTER = 328,               /* tSPLITTER  */
    tGRID = 329,                   /* tGRID  */
    tCOMPONENT = 330,              /* tCOMPONENT  */
    tINTERFACE = 331,              /* tINTERFACE  */
    tNET = 332,                    /* tNET  */
    tASSEMBLY = 333,               /* tASSEMBLY  */
    tUNDEFINED = 334,              /* tUNDEFINED  */
    SOLID = 335                    /* SOLID  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 99 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"

int	ival;
double	fval;
char	*sval;

#line 150 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE modellval;


int modelparse (void);


#endif /* !YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_DEV_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED  */
