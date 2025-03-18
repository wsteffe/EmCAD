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
    IMAG = 268,                    /* IMAG  */
    FREQ = 269,                    /* FREQ  */
    UNIT = 270,                    /* UNIT  */
    EXP = 271,                     /* EXP  */
    MESH = 272,                    /* MESH  */
    CONFORMAL = 273,               /* CONFORMAL  */
    TET = 274,                     /* TET  */
    SHARED = 275,                  /* SHARED  */
    REFINE = 276,                  /* REFINE  */
    RESONANCE = 277,               /* RESONANCE  */
    ENERGY = 278,                  /* ENERGY  */
    WAVELENGTH = 279,              /* WAVELENGTH  */
    LOCAL = 280,                   /* LOCAL  */
    MESHING3D = 281,               /* MESHING3D  */
    ANA = 282,                     /* ANA  */
    BAND = 283,                    /* BAND  */
    NUM = 284,                     /* NUM  */
    MOR = 285,                     /* MOR  */
    RESPONSE = 286,                /* RESPONSE  */
    SYMMETRIC = 287,               /* SYMMETRIC  */
    PARAMETER = 288,               /* PARAMETER  */
    TYPE = 289,                    /* TYPE  */
    TOPOLOGY = 290,                /* TOPOLOGY  */
    PART = 291,                    /* PART  */
    XSCALE = 292,                  /* XSCALE  */
    YSCALE = 293,                  /* YSCALE  */
    AUTO = 294,                    /* AUTO  */
    ZERO = 295,                    /* ZERO  */
    POLE = 296,                    /* POLE  */
    WINDOW = 297,                  /* WINDOW  */
    CURVE = 298,                   /* CURVE  */
    CIRCLE = 299,                  /* CIRCLE  */
    XYPLANE = 300,                 /* XYPLANE  */
    YZPLANE = 301,                 /* YZPLANE  */
    ZXPLANE = 302,                 /* ZXPLANE  */
    SYMMETRY = 303,                /* SYMMETRY  */
    IDEAL = 304,                   /* IDEAL  */
    CANONICAL = 305,               /* CANONICAL  */
    FILTER = 306,                  /* FILTER  */
    CUSTOM = 307,                  /* CUSTOM  */
    PASS = 308,                    /* PASS  */
    STOP = 309,                    /* STOP  */
    KRYLOV = 310,                  /* KRYLOV  */
    ORDER = 311,                   /* ORDER  */
    RETURNLOSS = 312,              /* RETURNLOSS  */
    OUTBAND = 313,                 /* OUTBAND  */
    MAPPING = 314,                 /* MAPPING  */
    TUNING = 315,                  /* TUNING  */
    METHOD = 316,                  /* METHOD  */
    SOURCE = 317,                  /* SOURCE  */
    ITERMAX = 318,                 /* ITERMAX  */
    AUTOMATIC = 319,               /* AUTOMATIC  */
    DESIGN = 320,                  /* DESIGN  */
    WITH = 321,                    /* WITH  */
    MAPPED = 322,                  /* MAPPED  */
    TX = 323,                      /* TX  */
    ZEROS = 324,                   /* ZEROS  */
    SPICE = 325,                   /* SPICE  */
    SET = 326,                     /* SET  */
    CENTRAL = 327,                 /* CENTRAL  */
    CONFIG = 328,                  /* CONFIG  */
    ADD = 329,                     /* ADD  */
    CONJUGATE = 330,               /* CONJUGATE  */
    PORT = 331,                    /* PORT  */
    IMPEDANCE = 332,               /* IMPEDANCE  */
    TARGET = 333,                  /* TARGET  */
    QFACTOR = 334,                 /* QFACTOR  */
    UNIFORM = 335,                 /* UNIFORM  */
    INDUCTIVE = 336,               /* INDUCTIVE  */
    SKIN = 337,                    /* SKIN  */
    LOSSY = 338,                   /* LOSSY  */
    PREDISTORTED = 339,            /* PREDISTORTED  */
    OPTIMIZE = 340,                /* OPTIMIZE  */
    ILOSSPP = 341,                 /* ILOSSPP  */
    TRUSTR = 342,                  /* TRUSTR  */
    CUTOFF = 343,                  /* CUTOFF  */
    RATIO = 344,                   /* RATIO  */
    RECOMPUTE = 345,               /* RECOMPUTE  */
    JACOBIAN = 346,                /* JACOBIAN  */
    ERRORT = 347,                  /* ERRORT  */
    ONLY = 348,                    /* ONLY  */
    TRANSVERSEJ = 349,             /* TRANSVERSEJ  */
    XTOL = 350,                    /* XTOL  */
    GRADDX = 351,                  /* GRADDX  */
    REMESH = 352,                  /* REMESH  */
    FIRST = 353,                   /* FIRST  */
    DECOMPOSITION = 354,           /* DECOMPOSITION  */
    MATERIAL = 355,                /* MATERIAL  */
    MODELIZATION = 356,            /* MODELIZATION  */
    COMPONENT = 357,               /* COMPONENT  */
    SAVE = 358,                    /* SAVE  */
    REDUCTION = 359,               /* REDUCTION  */
    RELOAD = 360,                  /* RELOAD  */
    NEEDED = 361,                  /* NEEDED  */
    DONE = 362,                    /* DONE  */
    CHANGED = 363,                 /* CHANGED  */
    tMIN = 364,                    /* tMIN  */
    tMAX = 365,                    /* tMAX  */
    PLUSI = 366,                   /* PLUSI  */
    MINUSI = 367                   /* MINUSI  */
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

#line 182 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE projectDatalval;


int projectDataparse (void);


#endif /* !YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_EMCAD_DEV_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED  */
