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

#ifndef YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_EMCAD_DEV_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED
# define YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_EMCAD_DEV_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int projectDatadebug;
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
    DEF = 262,                     /* DEF  */
    MAINASSNAME = 263,             /* MAINASSNAME  */
    VARFILEPATH = 264,             /* VARFILEPATH  */
    S2PFILEPATH = 265,             /* S2PFILEPATH  */
    NETWORK = 266,                 /* NETWORK  */
    LENGTH = 267,                  /* LENGTH  */
    FREQ = 268,                    /* FREQ  */
    UNIT = 269,                    /* UNIT  */
    EXP = 270,                     /* EXP  */
    MESH = 271,                    /* MESH  */
    CONFORMAL = 272,               /* CONFORMAL  */
    TET = 273,                     /* TET  */
    SHARED = 274,                  /* SHARED  */
    REFINE = 275,                  /* REFINE  */
    RESONANCE = 276,               /* RESONANCE  */
    ENERGY = 277,                  /* ENERGY  */
    WAVELENGTH = 278,              /* WAVELENGTH  */
    LOCAL = 279,                   /* LOCAL  */
    MESHING3D = 280,               /* MESHING3D  */
    ANA = 281,                     /* ANA  */
    BAND = 282,                    /* BAND  */
    NUM = 283,                     /* NUM  */
    MOR = 284,                     /* MOR  */
    RESPONSE = 285,                /* RESPONSE  */
    SYMMETRIC = 286,               /* SYMMETRIC  */
    PARAMETER = 287,               /* PARAMETER  */
    TYPE = 288,                    /* TYPE  */
    TOPOLOGY = 289,                /* TOPOLOGY  */
    PART = 290,                    /* PART  */
    XSCALE = 291,                  /* XSCALE  */
    YSCALE = 292,                  /* YSCALE  */
    AUTO = 293,                    /* AUTO  */
    ZERO = 294,                    /* ZERO  */
    POLE = 295,                    /* POLE  */
    WINDOW = 296,                  /* WINDOW  */
    CURVE = 297,                   /* CURVE  */
    CIRCLE = 298,                  /* CIRCLE  */
    XYPLANE = 299,                 /* XYPLANE  */
    YZPLANE = 300,                 /* YZPLANE  */
    ZXPLANE = 301,                 /* ZXPLANE  */
    SYMMETRY = 302,                /* SYMMETRY  */
    IDEAL = 303,                   /* IDEAL  */
    CANONICAL = 304,               /* CANONICAL  */
    FILTER = 305,                  /* FILTER  */
    CUSTOM = 306,                  /* CUSTOM  */
    PASS = 307,                    /* PASS  */
    STOP = 308,                    /* STOP  */
    KRYLOV = 309,                  /* KRYLOV  */
    ORDER = 310,                   /* ORDER  */
    RETURNLOSS = 311,              /* RETURNLOSS  */
    OUTBAND = 312,                 /* OUTBAND  */
    MAPPING = 313,                 /* MAPPING  */
    TUNING = 314,                  /* TUNING  */
    METHOD = 315,                  /* METHOD  */
    SOURCE = 316,                  /* SOURCE  */
    ITERMAX = 317,                 /* ITERMAX  */
    AUTOMATIC = 318,               /* AUTOMATIC  */
    DESIGN = 319,                  /* DESIGN  */
    WITH = 320,                    /* WITH  */
    MAPPED = 321,                  /* MAPPED  */
    TX = 322,                      /* TX  */
    ZEROS = 323,                   /* ZEROS  */
    SPICE = 324,                   /* SPICE  */
    SET = 325,                     /* SET  */
    CENTRAL = 326,                 /* CENTRAL  */
    CONFIG = 327,                  /* CONFIG  */
    ADD = 328,                     /* ADD  */
    CONJUGATE = 329,               /* CONJUGATE  */
    PORT = 330,                    /* PORT  */
    IMPEDANCE = 331,               /* IMPEDANCE  */
    TARGET = 332,                  /* TARGET  */
    QFACTOR = 333,                 /* QFACTOR  */
    UNIFORM = 334,                 /* UNIFORM  */
    INDUCTIVE = 335,               /* INDUCTIVE  */
    SKIN = 336,                    /* SKIN  */
    LOSSY = 337,                   /* LOSSY  */
    PREDISTORTED = 338,            /* PREDISTORTED  */
    OPTIMIZE = 339,                /* OPTIMIZE  */
    ILOSSPP = 340,                 /* ILOSSPP  */
    TRUSTR = 341,                  /* TRUSTR  */
    CUTOFF = 342,                  /* CUTOFF  */
    RATIO = 343,                   /* RATIO  */
    RECOMPUTE = 344,               /* RECOMPUTE  */
    JACOBIAN = 345,                /* JACOBIAN  */
    ERRORT = 346,                  /* ERRORT  */
    ONLY = 347,                    /* ONLY  */
    TRANSVERSEJ = 348,             /* TRANSVERSEJ  */
    XTOL = 349,                    /* XTOL  */
    GRADDX = 350,                  /* GRADDX  */
    REMESH = 351,                  /* REMESH  */
    FIRST = 352,                   /* FIRST  */
    DECOMPOSITION = 353,           /* DECOMPOSITION  */
    MATERIAL = 354,                /* MATERIAL  */
    MODELIZATION = 355,            /* MODELIZATION  */
    COMPONENT = 356,               /* COMPONENT  */
    SAVE = 357,                    /* SAVE  */
    REDUCTION = 358,               /* REDUCTION  */
    RELOAD = 359,                  /* RELOAD  */
    NEEDED = 360,                  /* NEEDED  */
    DONE = 361,                    /* DONE  */
    CHANGED = 362,                 /* CHANGED  */
    tMIN = 363,                    /* tMIN  */
    tMAX = 364,                    /* tMAX  */
    PLUSI = 365,                   /* PLUSI  */
    MINUSI = 366                   /* MINUSI  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 69 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"

int	ival;
double	fval;
char	*sval;

#line 181 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE projectDatalval;


int projectDataparse (void);


#endif /* !YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_EMCAD_DEV_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED  */
