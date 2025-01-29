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
    SUBCIRCUITINDEX = 277,         /* SUBCIRCUITINDEX  */
    SUBCIRCUITIFINDEX = 278,       /* SUBCIRCUITIFINDEX  */
    ORIENTATION = 279,             /* ORIENTATION  */
    GRIDNUM = 280,                 /* GRIDNUM  */
    PML = 281,                     /* PML  */
    INVARIANT = 282,               /* INVARIANT  */
    TRANSLATION = 283,             /* TRANSLATION  */
    ROTATION = 284,                /* ROTATION  */
    ANGLE = 285,                   /* ANGLE  */
    ORIGIN = 286,                  /* ORIGIN  */
    AXIS = 287,                    /* AXIS  */
    MWM_VOLUME = 288,              /* MWM_VOLUME  */
    MWM_INVARIANT = 289,           /* MWM_INVARIANT  */
    MWM_UNITS = 290,               /* MWM_UNITS  */
    MWM_LINEPORT = 291,            /* MWM_LINEPORT  */
    LENGTH = 292,                  /* LENGTH  */
    FREQUENCY = 293,               /* FREQUENCY  */
    BAND = 294,                    /* BAND  */
    SURFACE = 295,                 /* SURFACE  */
    RESISTANCE = 296,              /* RESISTANCE  */
    INDUCTANCE = 297,              /* INDUCTANCE  */
    IMPEDANCE = 298,               /* IMPEDANCE  */
    LOSSFACTOR = 299,              /* LOSSFACTOR  */
    QFACTOR = 300,                 /* QFACTOR  */
    SURFRATIO = 301,               /* SURFRATIO  */
    BALLRADIUS = 302,              /* BALLRADIUS  */
    ROUGH = 303,                   /* ROUGH  */
    MODELTYPE = 304,               /* MODELTYPE  */
    RZ = 305,                      /* RZ  */
    RQ = 306,                      /* RQ  */
    MESHREFINEMENT = 307,          /* MESHREFINEMENT  */
    CUTOFFREFINEMENT = 308,        /* CUTOFFREFINEMENT  */
    COMPSOLID = 309,               /* COMPSOLID  */
    VOLTYPE = 310,                 /* VOLTYPE  */
    EPSILONR = 311,                /* EPSILONR  */
    MUR = 312,                     /* MUR  */
    EPSLORENTZ = 313,              /* EPSLORENTZ  */
    MULORENTZ = 314,               /* MULORENTZ  */
    POLESRESIDUES = 315,           /* POLESRESIDUES  */
    POLESNUM = 316,                /* POLESNUM  */
    ECONDUCTIVITY = 317,           /* ECONDUCTIVITY  */
    HCONDUCTIVITY = 318,           /* HCONDUCTIVITY  */
    ETANDELTA = 319,               /* ETANDELTA  */
    HTANDELTA = 320,               /* HTANDELTA  */
    MATERIAL = 321,                /* MATERIAL  */
    COLOR = 322,                   /* COLOR  */
    VOLUMES = 323,                 /* VOLUMES  */
    DISABLED = 324,                /* DISABLED  */
    tDIELECTRIC = 325,             /* tDIELECTRIC  */
    tHOLE = 326,                   /* tHOLE  */
    tBOUNDARYCONDITION = 327,      /* tBOUNDARYCONDITION  */
    tWAVEGUIDE = 328,              /* tWAVEGUIDE  */
    tLINEPORT = 329,               /* tLINEPORT  */
    tSPLITTER = 330,               /* tSPLITTER  */
    tGRID = 331,                   /* tGRID  */
    tCOMPONENT = 332,              /* tCOMPONENT  */
    tINTERFACE = 333,              /* tINTERFACE  */
    tNET = 334,                    /* tNET  */
    tASSEMBLY = 335,               /* tASSEMBLY  */
    tUNDEFINED = 336,              /* tUNDEFINED  */
    SOLID = 337                    /* SOLID  */
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

#line 152 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE modellval;


int modelparse (void);


#endif /* !YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_DEV_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED  */
