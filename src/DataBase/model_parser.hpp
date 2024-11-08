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

#ifndef YY_MODEL_HOME_WALTER_MWCAD_EMCAD_CONFORMALMESH_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED
# define YY_MODEL_HOME_WALTER_MWCAD_EMCAD_CONFORMALMESH_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED
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
    TEMTABULARORDER = 273,         /* TEMTABULARORDER  */
    GRIDNUM = 274,                 /* GRIDNUM  */
    PML = 275,                     /* PML  */
    INVARIANT = 276,               /* INVARIANT  */
    TRANSLATION = 277,             /* TRANSLATION  */
    ROTATION = 278,                /* ROTATION  */
    ANGLE = 279,                   /* ANGLE  */
    ORIGIN = 280,                  /* ORIGIN  */
    AXIS = 281,                    /* AXIS  */
    MWM_VOLUME = 282,              /* MWM_VOLUME  */
    MWM_INVARIANT = 283,           /* MWM_INVARIANT  */
    MWM_UNITS = 284,               /* MWM_UNITS  */
    MWM_LINEPORT = 285,            /* MWM_LINEPORT  */
    LENGTH = 286,                  /* LENGTH  */
    FREQUENCY = 287,               /* FREQUENCY  */
    BAND = 288,                    /* BAND  */
    SURFACE = 289,                 /* SURFACE  */
    RESISTANCE = 290,              /* RESISTANCE  */
    INDUCTANCE = 291,              /* INDUCTANCE  */
    IMPEDANCE = 292,               /* IMPEDANCE  */
    LOSSFACTOR = 293,              /* LOSSFACTOR  */
    QFACTOR = 294,                 /* QFACTOR  */
    SURFRATIO = 295,               /* SURFRATIO  */
    BALLRADIUS = 296,              /* BALLRADIUS  */
    ROUGH = 297,                   /* ROUGH  */
    MODELTYPE = 298,               /* MODELTYPE  */
    RZ = 299,                      /* RZ  */
    RQ = 300,                      /* RQ  */
    MESHREFINEMENT = 301,          /* MESHREFINEMENT  */
    CUTOFFREFINEMENT = 302,        /* CUTOFFREFINEMENT  */
    COMPSOLID = 303,               /* COMPSOLID  */
    VOLTYPE = 304,                 /* VOLTYPE  */
    EPSILONR = 305,                /* EPSILONR  */
    MUR = 306,                     /* MUR  */
    EPSLORENTZ = 307,              /* EPSLORENTZ  */
    MULORENTZ = 308,               /* MULORENTZ  */
    POLESRESIDUES = 309,           /* POLESRESIDUES  */
    POLESNUM = 310,                /* POLESNUM  */
    ECONDUCTIVITY = 311,           /* ECONDUCTIVITY  */
    HCONDUCTIVITY = 312,           /* HCONDUCTIVITY  */
    ETANDELTA = 313,               /* ETANDELTA  */
    HTANDELTA = 314,               /* HTANDELTA  */
    MATERIAL = 315,                /* MATERIAL  */
    COLOR = 316,                   /* COLOR  */
    VOLUMES = 317,                 /* VOLUMES  */
    DISABLED = 318,                /* DISABLED  */
    tDIELECTRIC = 319,             /* tDIELECTRIC  */
    tHOLE = 320,                   /* tHOLE  */
    tBOUNDARYCONDITION = 321,      /* tBOUNDARYCONDITION  */
    tWAVEGUIDE = 322,              /* tWAVEGUIDE  */
    tLINEPORT = 323,               /* tLINEPORT  */
    tSPLITTER = 324,               /* tSPLITTER  */
    tGRID = 325,                   /* tGRID  */
    tCOMPONENT = 326,              /* tCOMPONENT  */
    tINTERFACE = 327,              /* tINTERFACE  */
    tNET = 328,                    /* tNET  */
    tASSEMBLY = 329,               /* tASSEMBLY  */
    tUNDEFINED = 330,              /* tUNDEFINED  */
    SOLID = 331                    /* SOLID  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 99 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"

int	ival;
double	fval;
char	*sval;

#line 146 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE modellval;


int modelparse (void);


#endif /* !YY_MODEL_HOME_WALTER_MWCAD_EMCAD_CONFORMALMESH_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED  */
