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
    TET = 272,                     /* TET  */
    SHARED = 273,                  /* SHARED  */
    REFINE = 274,                  /* REFINE  */
    RESONANCE = 275,               /* RESONANCE  */
    ENERGY = 276,                  /* ENERGY  */
    WAVELENGTH = 277,              /* WAVELENGTH  */
    LOCAL = 278,                   /* LOCAL  */
    MESHING3D = 279,               /* MESHING3D  */
    ANA = 280,                     /* ANA  */
    BAND = 281,                    /* BAND  */
    NUM = 282,                     /* NUM  */
    MOR = 283,                     /* MOR  */
    RESPONSE = 284,                /* RESPONSE  */
    SYMMETRIC = 285,               /* SYMMETRIC  */
    PARAMETER = 286,               /* PARAMETER  */
    TYPE = 287,                    /* TYPE  */
    TOPOLOGY = 288,                /* TOPOLOGY  */
    PART = 289,                    /* PART  */
    XSCALE = 290,                  /* XSCALE  */
    YSCALE = 291,                  /* YSCALE  */
    AUTO = 292,                    /* AUTO  */
    ZERO = 293,                    /* ZERO  */
    POLE = 294,                    /* POLE  */
    WINDOW = 295,                  /* WINDOW  */
    CURVE = 296,                   /* CURVE  */
    CIRCLE = 297,                  /* CIRCLE  */
    IDEAL = 298,                   /* IDEAL  */
    CANONICAL = 299,               /* CANONICAL  */
    FILTER = 300,                  /* FILTER  */
    CUSTOM = 301,                  /* CUSTOM  */
    PASS = 302,                    /* PASS  */
    KRYLOV = 303,                  /* KRYLOV  */
    ORDER = 304,                   /* ORDER  */
    RETURNLOSS = 305,              /* RETURNLOSS  */
    OUTBAND = 306,                 /* OUTBAND  */
    MAPPING = 307,                 /* MAPPING  */
    TUNING = 308,                  /* TUNING  */
    METHOD = 309,                  /* METHOD  */
    SOURCE = 310,                  /* SOURCE  */
    ITERMAX = 311,                 /* ITERMAX  */
    AUTOMATIC = 312,               /* AUTOMATIC  */
    DESIGN = 313,                  /* DESIGN  */
    WITH = 314,                    /* WITH  */
    MAPPED = 315,                  /* MAPPED  */
    TX = 316,                      /* TX  */
    ZEROS = 317,                   /* ZEROS  */
    ADD = 318,                     /* ADD  */
    CONJUGATE = 319,               /* CONJUGATE  */
    PORT = 320,                    /* PORT  */
    IMPEDANCE = 321,               /* IMPEDANCE  */
    TARGET = 322,                  /* TARGET  */
    QFACTOR = 323,                 /* QFACTOR  */
    UNIFORM = 324,                 /* UNIFORM  */
    INDUCTIVE = 325,               /* INDUCTIVE  */
    SKIN = 326,                    /* SKIN  */
    LOSSY = 327,                   /* LOSSY  */
    PREDISTORTED = 328,            /* PREDISTORTED  */
    OPTIMIZE = 329,                /* OPTIMIZE  */
    ILOSSPP = 330,                 /* ILOSSPP  */
    TRUSTR = 331,                  /* TRUSTR  */
    CUTOFF = 332,                  /* CUTOFF  */
    RATIO = 333,                   /* RATIO  */
    RECOMPUTE = 334,               /* RECOMPUTE  */
    JACOBIAN = 335,                /* JACOBIAN  */
    ERRORT = 336,                  /* ERRORT  */
    ONLY = 337,                    /* ONLY  */
    TRANSVERSEJ = 338,             /* TRANSVERSEJ  */
    XTOL = 339,                    /* XTOL  */
    GRADDX = 340,                  /* GRADDX  */
    REMESH = 341,                  /* REMESH  */
    FIRST = 342,                   /* FIRST  */
    DECOMPOSITION = 343,           /* DECOMPOSITION  */
    MATERIAL = 344,                /* MATERIAL  */
    MODELIZATION = 345,            /* MODELIZATION  */
    COMPONENT = 346,               /* COMPONENT  */
    SAVE = 347,                    /* SAVE  */
    REDUCTION = 348,               /* REDUCTION  */
    RELOAD = 349,                  /* RELOAD  */
    NEEDED = 350,                  /* NEEDED  */
    DONE = 351,                    /* DONE  */
    CHANGED = 352,                 /* CHANGED  */
    tMIN = 353,                    /* tMIN  */
    tMAX = 354,                    /* tMAX  */
    PLUSI = 355,                   /* PLUSI  */
    MINUSI = 356                   /* MINUSI  */
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

#line 171 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE projectDatalval;


int projectDataparse (void);


#endif /* !YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_EMCAD_DEV_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED  */
