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

#ifndef YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_CONFORMALMESH_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED
# define YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_CONFORMALMESH_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED
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
    XYPLANE = 298,                 /* XYPLANE  */
    YZPLANE = 299,                 /* YZPLANE  */
    ZXPLANE = 300,                 /* ZXPLANE  */
    SYMMETRY = 301,                /* SYMMETRY  */
    IDEAL = 302,                   /* IDEAL  */
    CANONICAL = 303,               /* CANONICAL  */
    FILTER = 304,                  /* FILTER  */
    CUSTOM = 305,                  /* CUSTOM  */
    PASS = 306,                    /* PASS  */
    KRYLOV = 307,                  /* KRYLOV  */
    ORDER = 308,                   /* ORDER  */
    RETURNLOSS = 309,              /* RETURNLOSS  */
    OUTBAND = 310,                 /* OUTBAND  */
    MAPPING = 311,                 /* MAPPING  */
    TUNING = 312,                  /* TUNING  */
    METHOD = 313,                  /* METHOD  */
    SOURCE = 314,                  /* SOURCE  */
    ITERMAX = 315,                 /* ITERMAX  */
    AUTOMATIC = 316,               /* AUTOMATIC  */
    DESIGN = 317,                  /* DESIGN  */
    WITH = 318,                    /* WITH  */
    MAPPED = 319,                  /* MAPPED  */
    TX = 320,                      /* TX  */
    ZEROS = 321,                   /* ZEROS  */
    SPICE = 322,                   /* SPICE  */
    SET = 323,                     /* SET  */
    CENTRAL = 324,                 /* CENTRAL  */
    CONFIG = 325,                  /* CONFIG  */
    ADD = 326,                     /* ADD  */
    CONJUGATE = 327,               /* CONJUGATE  */
    PORT = 328,                    /* PORT  */
    IMPEDANCE = 329,               /* IMPEDANCE  */
    TARGET = 330,                  /* TARGET  */
    QFACTOR = 331,                 /* QFACTOR  */
    UNIFORM = 332,                 /* UNIFORM  */
    INDUCTIVE = 333,               /* INDUCTIVE  */
    SKIN = 334,                    /* SKIN  */
    LOSSY = 335,                   /* LOSSY  */
    PREDISTORTED = 336,            /* PREDISTORTED  */
    OPTIMIZE = 337,                /* OPTIMIZE  */
    ILOSSPP = 338,                 /* ILOSSPP  */
    TRUSTR = 339,                  /* TRUSTR  */
    CUTOFF = 340,                  /* CUTOFF  */
    RATIO = 341,                   /* RATIO  */
    RECOMPUTE = 342,               /* RECOMPUTE  */
    JACOBIAN = 343,                /* JACOBIAN  */
    ERRORT = 344,                  /* ERRORT  */
    ONLY = 345,                    /* ONLY  */
    TRANSVERSEJ = 346,             /* TRANSVERSEJ  */
    XTOL = 347,                    /* XTOL  */
    GRADDX = 348,                  /* GRADDX  */
    REMESH = 349,                  /* REMESH  */
    FIRST = 350,                   /* FIRST  */
    DECOMPOSITION = 351,           /* DECOMPOSITION  */
    MATERIAL = 352,                /* MATERIAL  */
    MODELIZATION = 353,            /* MODELIZATION  */
    COMPONENT = 354,               /* COMPONENT  */
    SAVE = 355,                    /* SAVE  */
    REDUCTION = 356,               /* REDUCTION  */
    RELOAD = 357,                  /* RELOAD  */
    NEEDED = 358,                  /* NEEDED  */
    DONE = 359,                    /* DONE  */
    CHANGED = 360,                 /* CHANGED  */
    tMIN = 361,                    /* tMIN  */
    tMAX = 362,                    /* tMAX  */
    PLUSI = 363,                   /* PLUSI  */
    MINUSI = 364                   /* MINUSI  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 69 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"

int	ival;
double	fval;
char	*sval;

#line 179 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE projectDatalval;


int projectDataparse (void);


#endif /* !YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_CONFORMALMESH_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED  */
