/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         modelparse
#define yylex           modellex
#define yyerror         modelerror
#define yydebug         modeldebug
#define yynerrs         modelnerrs
#define yylval          modellval
#define yychar          modelchar

/* First part of user prologue.  */
#line 22 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"

//debug:
#define YYDEBUG 1
#define YYPRINT(file, type, value) yyprint(file, type, value)
//debug

#include <InputOutput.h>
#include <ocaf.h>
#include <DataBase.h>
#include <Message.h>

#define YYINITDEPTH  (1024 * 64)
#define	YYMAXDEPTH	(YYINITDEPTH * 64)


extern char *modeltext;
void yyerror(char *s);
int yyparse (void);
int yylex ();

int changedBinFile=1;
int openBinFile=0;

namespace DB {
 extern char yyFileName[256];
 extern int yyLineNum;
}

//debug:
typedef union YYSTYPE
{
int	ival;
double	fval;
char	*sval;
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
static void yyprint (FILE *file, int type, YYSTYPE value);
//debug


char mainCircName[256];

extern DB::Units unit;
DB::EmProblem *loadingEmP;
MwOCAF *loadingOcaf;

bool step_file_reloaded;
bool reload_step_file;

DB::BufferIn<double> 	*Fbuff;
DB::BufferIn<int> 	*Ibuff;

DB::StringList_t *strList;

DB::MatBuffer        DB::Material::buff  =DB::MatBuffer();
DB::Material *mat;
DB::Volume   *vol;
DB::Transform *invt;
DB::str_t    name,str1,str2;


DB::Buffer<char,10,80>	 StringBuffer;
DB::Buffer<char,10,80>	 *Sbuff=&StringBuffer;
DB::Vec<char>            Cnames;

DB::Buffer<int, 10> 	IntBuffer;
DB::Buffer<double, 10> 	DoubleBuffer;
DB::Vec<int>	matColor;
DB::Vec<double>	dVec;

int Spec,Spmc,Swg,Sres;
DB::Volume  *Specv;


#line 155 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "model_parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INTEGER = 3,                    /* INTEGER  */
  YYSYMBOL_FLOATING = 4,                   /* FLOATING  */
  YYSYMBOL_STRING = 5,                     /* STRING  */
  YYSYMBOL_NAME = 6,                       /* NAME  */
  YYSYMBOL_IMPORT = 7,                     /* IMPORT  */
  YYSYMBOL_ASSEMBLYTYPE = 8,               /* ASSEMBLYTYPE  */
  YYSYMBOL_LEVEL = 9,                      /* LEVEL  */
  YYSYMBOL_DEF = 10,                       /* DEF  */
  YYSYMBOL_SET = 11,                       /* SET  */
  YYSYMBOL_CIRCUITNAME = 12,               /* CIRCUITNAME  */
  YYSYMBOL_DEFAULTBC = 13,                 /* DEFAULTBC  */
  YYSYMBOL_MWM_MATERIAL = 14,              /* MWM_MATERIAL  */
  YYSYMBOL_TEMPORTSNUM = 15,               /* TEMPORTSNUM  */
  YYSYMBOL_TEPORTSNUM = 16,                /* TEPORTSNUM  */
  YYSYMBOL_TMPORTSNUM = 17,                /* TMPORTSNUM  */
  YYSYMBOL_TEMTABULARORDER1 = 18,          /* TEMTABULARORDER1  */
  YYSYMBOL_TEMTABULARORDER2 = 19,          /* TEMTABULARORDER2  */
  YYSYMBOL_TEMREFCONDUCTOR = 20,           /* TEMREFCONDUCTOR  */
  YYSYMBOL_DISCONNECTEDTEM = 21,           /* DISCONNECTEDTEM  */
  YYSYMBOL_SUBCIRCUITINDEX = 22,           /* SUBCIRCUITINDEX  */
  YYSYMBOL_SUBCIRCUITIFINDEX = 23,         /* SUBCIRCUITIFINDEX  */
  YYSYMBOL_ORIENTATION = 24,               /* ORIENTATION  */
  YYSYMBOL_GRIDNUM = 25,                   /* GRIDNUM  */
  YYSYMBOL_PML = 26,                       /* PML  */
  YYSYMBOL_INVARIANT = 27,                 /* INVARIANT  */
  YYSYMBOL_TRANSLATION = 28,               /* TRANSLATION  */
  YYSYMBOL_ROTATION = 29,                  /* ROTATION  */
  YYSYMBOL_ANGLE = 30,                     /* ANGLE  */
  YYSYMBOL_ORIGIN = 31,                    /* ORIGIN  */
  YYSYMBOL_AXIS = 32,                      /* AXIS  */
  YYSYMBOL_MWM_VOLUME = 33,                /* MWM_VOLUME  */
  YYSYMBOL_MWM_INVARIANT = 34,             /* MWM_INVARIANT  */
  YYSYMBOL_MWM_UNITS = 35,                 /* MWM_UNITS  */
  YYSYMBOL_MWM_LINEPORT = 36,              /* MWM_LINEPORT  */
  YYSYMBOL_LENGTH = 37,                    /* LENGTH  */
  YYSYMBOL_FREQUENCY = 38,                 /* FREQUENCY  */
  YYSYMBOL_BAND = 39,                      /* BAND  */
  YYSYMBOL_SURFACE = 40,                   /* SURFACE  */
  YYSYMBOL_RESISTANCE = 41,                /* RESISTANCE  */
  YYSYMBOL_INDUCTANCE = 42,                /* INDUCTANCE  */
  YYSYMBOL_IMPEDANCE = 43,                 /* IMPEDANCE  */
  YYSYMBOL_LOSSFACTOR = 44,                /* LOSSFACTOR  */
  YYSYMBOL_QFACTOR = 45,                   /* QFACTOR  */
  YYSYMBOL_SURFRATIO = 46,                 /* SURFRATIO  */
  YYSYMBOL_BALLRADIUS = 47,                /* BALLRADIUS  */
  YYSYMBOL_ROUGH = 48,                     /* ROUGH  */
  YYSYMBOL_MODELTYPE = 49,                 /* MODELTYPE  */
  YYSYMBOL_RZ = 50,                        /* RZ  */
  YYSYMBOL_RQ = 51,                        /* RQ  */
  YYSYMBOL_MESHREFINEMENT = 52,            /* MESHREFINEMENT  */
  YYSYMBOL_CUTOFFREFINEMENT = 53,          /* CUTOFFREFINEMENT  */
  YYSYMBOL_COMPSOLID = 54,                 /* COMPSOLID  */
  YYSYMBOL_VOLTYPE = 55,                   /* VOLTYPE  */
  YYSYMBOL_EPSILONR = 56,                  /* EPSILONR  */
  YYSYMBOL_MUR = 57,                       /* MUR  */
  YYSYMBOL_EPSLORENTZ = 58,                /* EPSLORENTZ  */
  YYSYMBOL_MULORENTZ = 59,                 /* MULORENTZ  */
  YYSYMBOL_POLESRESIDUES = 60,             /* POLESRESIDUES  */
  YYSYMBOL_POLESNUM = 61,                  /* POLESNUM  */
  YYSYMBOL_ECONDUCTIVITY = 62,             /* ECONDUCTIVITY  */
  YYSYMBOL_HCONDUCTIVITY = 63,             /* HCONDUCTIVITY  */
  YYSYMBOL_ETANDELTA = 64,                 /* ETANDELTA  */
  YYSYMBOL_HTANDELTA = 65,                 /* HTANDELTA  */
  YYSYMBOL_MATERIAL = 66,                  /* MATERIAL  */
  YYSYMBOL_COLOR = 67,                     /* COLOR  */
  YYSYMBOL_VOLUMES = 68,                   /* VOLUMES  */
  YYSYMBOL_DISABLED = 69,                  /* DISABLED  */
  YYSYMBOL_tDIELECTRIC = 70,               /* tDIELECTRIC  */
  YYSYMBOL_tHOLE = 71,                     /* tHOLE  */
  YYSYMBOL_tBOUNDARYCONDITION = 72,        /* tBOUNDARYCONDITION  */
  YYSYMBOL_tWAVEGUIDE = 73,                /* tWAVEGUIDE  */
  YYSYMBOL_tLINEPORT = 74,                 /* tLINEPORT  */
  YYSYMBOL_tSPLITTER = 75,                 /* tSPLITTER  */
  YYSYMBOL_tGRID = 76,                     /* tGRID  */
  YYSYMBOL_tCOMPONENT = 77,                /* tCOMPONENT  */
  YYSYMBOL_tINTERFACE = 78,                /* tINTERFACE  */
  YYSYMBOL_tNET = 79,                      /* tNET  */
  YYSYMBOL_tASSEMBLY = 80,                 /* tASSEMBLY  */
  YYSYMBOL_tUNDEFINED = 81,                /* tUNDEFINED  */
  YYSYMBOL_SOLID = 82,                     /* SOLID  */
  YYSYMBOL_83_ = 83,                       /* ','  */
  YYSYMBOL_84_ = 84,                       /* '['  */
  YYSYMBOL_85_ = 85,                       /* ']'  */
  YYSYMBOL_86_ = 86,                       /* '{'  */
  YYSYMBOL_87_ = 87,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 88,                  /* $accept  */
  YYSYMBOL_All = 89,                       /* All  */
  YYSYMBOL_MwmItems = 90,                  /* MwmItems  */
  YYSYMBOL_MwmItem = 91,                   /* MwmItem  */
  YYSYMBOL_SFInt32 = 92,                   /* SFInt32  */
  YYSYMBOL_SFFloat = 93,                   /* SFFloat  */
  YYSYMBOL_SFVec3f = 94,                   /* SFVec3f  */
  YYSYMBOL_SFInt32List = 95,               /* SFInt32List  */
  YYSYMBOL_MFInt32 = 96,                   /* MFInt32  */
  YYSYMBOL_SFVec3fList = 97,               /* SFVec3fList  */
  YYSYMBOL_MFVec3f = 98,                   /* MFVec3f  */
  YYSYMBOL_SFVec4f = 99,                   /* SFVec4f  */
  YYSYMBOL_SFVec4fList = 100,              /* SFVec4fList  */
  YYSYMBOL_MFVec4f = 101,                  /* MFVec4f  */
  YYSYMBOL_NodeBegin = 102,                /* NodeBegin  */
  YYSYMBOL_NodeEnd = 103,                  /* NodeEnd  */
  YYSYMBOL_Name = 104,                     /* Name  */
  YYSYMBOL_SetCircuitType = 105,           /* SetCircuitType  */
  YYSYMBOL_Level = 106,                    /* Level  */
  YYSYMBOL_DefaultBoundCond = 107,         /* DefaultBoundCond  */
  YYSYMBOL_Import = 108,                   /* Import  */
  YYSYMBOL_UnitsElements = 109,            /* UnitsElements  */
  YYSYMBOL_UnitsElement = 110,             /* UnitsElement  */
  YYSYMBOL_111_1 = 111,                    /* $@1  */
  YYSYMBOL_112_2 = 112,                    /* $@2  */
  YYSYMBOL_113_3 = 113,                    /* $@3  */
  YYSYMBOL_UnitsBegin = 114,               /* UnitsBegin  */
  YYSYMBOL_Units = 115,                    /* Units  */
  YYSYMBOL_InvariantElements = 116,        /* InvariantElements  */
  YYSYMBOL_InvariantElement = 117,         /* InvariantElement  */
  YYSYMBOL_118_4 = 118,                    /* $@4  */
  YYSYMBOL_119_5 = 119,                    /* $@5  */
  YYSYMBOL_120_6 = 120,                    /* $@6  */
  YYSYMBOL_121_7 = 121,                    /* $@7  */
  YYSYMBOL_InvariantBegin = 122,           /* InvariantBegin  */
  YYSYMBOL_Invariant = 123,                /* Invariant  */
  YYSYMBOL_MaterialElements = 124,         /* MaterialElements  */
  YYSYMBOL_MaterialElement = 125,          /* MaterialElement  */
  YYSYMBOL_126_8 = 126,                    /* $@8  */
  YYSYMBOL_127_9 = 127,                    /* $@9  */
  YYSYMBOL_128_10 = 128,                   /* $@10  */
  YYSYMBOL_129_11 = 129,                   /* $@11  */
  YYSYMBOL_130_12 = 130,                   /* $@12  */
  YYSYMBOL_131_13 = 131,                   /* $@13  */
  YYSYMBOL_132_14 = 132,                   /* $@14  */
  YYSYMBOL_133_15 = 133,                   /* $@15  */
  YYSYMBOL_134_16 = 134,                   /* $@16  */
  YYSYMBOL_135_17 = 135,                   /* $@17  */
  YYSYMBOL_136_18 = 136,                   /* $@18  */
  YYSYMBOL_137_19 = 137,                   /* $@19  */
  YYSYMBOL_138_20 = 138,                   /* $@20  */
  YYSYMBOL_139_21 = 139,                   /* $@21  */
  YYSYMBOL_140_22 = 140,                   /* $@22  */
  YYSYMBOL_141_23 = 141,                   /* $@23  */
  YYSYMBOL_142_24 = 142,                   /* $@24  */
  YYSYMBOL_143_25 = 143,                   /* $@25  */
  YYSYMBOL_144_26 = 144,                   /* $@26  */
  YYSYMBOL_145_27 = 145,                   /* $@27  */
  YYSYMBOL_146_28 = 146,                   /* $@28  */
  YYSYMBOL_MaterialBegin = 147,            /* MaterialBegin  */
  YYSYMBOL_Material = 148,                 /* Material  */
  YYSYMBOL_VolumeType = 149,               /* VolumeType  */
  YYSYMBOL_VolumeElements = 150,           /* VolumeElements  */
  YYSYMBOL_VolumeElement = 151,            /* VolumeElement  */
  YYSYMBOL_152_29 = 152,                   /* $@29  */
  YYSYMBOL_153_30 = 153,                   /* $@30  */
  YYSYMBOL_154_31 = 154,                   /* $@31  */
  YYSYMBOL_155_32 = 155,                   /* $@32  */
  YYSYMBOL_156_33 = 156,                   /* $@33  */
  YYSYMBOL_157_34 = 157,                   /* $@34  */
  YYSYMBOL_158_35 = 158,                   /* $@35  */
  YYSYMBOL_159_36 = 159,                   /* $@36  */
  YYSYMBOL_160_37 = 160,                   /* $@37  */
  YYSYMBOL_161_38 = 161,                   /* $@38  */
  YYSYMBOL_162_39 = 162,                   /* $@39  */
  YYSYMBOL_163_40 = 163,                   /* $@40  */
  YYSYMBOL_164_41 = 164,                   /* $@41  */
  YYSYMBOL_165_42 = 165,                   /* $@42  */
  YYSYMBOL_166_43 = 166,                   /* $@43  */
  YYSYMBOL_167_44 = 167,                   /* $@44  */
  YYSYMBOL_168_45 = 168,                   /* $@45  */
  YYSYMBOL_VolumeBegin = 169,              /* VolumeBegin  */
  YYSYMBOL_Volume = 170,                   /* Volume  */
  YYSYMBOL_CircuitName = 171               /* CircuitName  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   239

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  88
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  84
/* YYNRULES -- Number of rules.  */
#define YYNRULES  166
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  246

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   337


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    83,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    84,     2,    85,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    86,     2,    87,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   132,   132,   136,   137,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   161,   173,   177,   191,   196,
     197,   198,   199,   203,   204,   234,   235,   236,   237,   241,
     242,   247,   251,   252,   253,   254,   258,   259,   275,   279,
     280,   285,   290,   297,   305,   316,   328,   329,   333,   333,
     334,   334,   335,   335,   339,   343,   351,   352,   356,   356,
     357,   357,   362,   362,   367,   367,   375,   385,   393,   394,
     398,   398,   399,   399,   400,   400,   401,   401,   402,   402,
     403,   403,   404,   404,   405,   405,   406,   406,   407,   407,
     408,   408,   409,   409,   410,   410,   411,   411,   412,   412,
     413,   413,   414,   414,   415,   415,   416,   416,   417,   417,
     418,   418,   426,   437,   448,   452,   456,   460,   464,   468,
     472,   476,   480,   484,   488,   492,   496,   501,   502,   506,
     512,   512,   516,   516,   520,   520,   524,   524,   528,   528,
     532,   532,   536,   536,   540,   540,   544,   544,   548,   548,
     552,   552,   556,   556,   560,   560,   564,   564,   565,   565,
     567,   567,   569,   569,   572,   581,   590
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INTEGER", "FLOATING",
  "STRING", "NAME", "IMPORT", "ASSEMBLYTYPE", "LEVEL", "DEF", "SET",
  "CIRCUITNAME", "DEFAULTBC", "MWM_MATERIAL", "TEMPORTSNUM", "TEPORTSNUM",
  "TMPORTSNUM", "TEMTABULARORDER1", "TEMTABULARORDER2", "TEMREFCONDUCTOR",
  "DISCONNECTEDTEM", "SUBCIRCUITINDEX", "SUBCIRCUITIFINDEX", "ORIENTATION",
  "GRIDNUM", "PML", "INVARIANT", "TRANSLATION", "ROTATION", "ANGLE",
  "ORIGIN", "AXIS", "MWM_VOLUME", "MWM_INVARIANT", "MWM_UNITS",
  "MWM_LINEPORT", "LENGTH", "FREQUENCY", "BAND", "SURFACE", "RESISTANCE",
  "INDUCTANCE", "IMPEDANCE", "LOSSFACTOR", "QFACTOR", "SURFRATIO",
  "BALLRADIUS", "ROUGH", "MODELTYPE", "RZ", "RQ", "MESHREFINEMENT",
  "CUTOFFREFINEMENT", "COMPSOLID", "VOLTYPE", "EPSILONR", "MUR",
  "EPSLORENTZ", "MULORENTZ", "POLESRESIDUES", "POLESNUM", "ECONDUCTIVITY",
  "HCONDUCTIVITY", "ETANDELTA", "HTANDELTA", "MATERIAL", "COLOR",
  "VOLUMES", "DISABLED", "tDIELECTRIC", "tHOLE", "tBOUNDARYCONDITION",
  "tWAVEGUIDE", "tLINEPORT", "tSPLITTER", "tGRID", "tCOMPONENT",
  "tINTERFACE", "tNET", "tASSEMBLY", "tUNDEFINED", "SOLID", "','", "'['",
  "']'", "'{'", "'}'", "$accept", "All", "MwmItems", "MwmItem", "SFInt32",
  "SFFloat", "SFVec3f", "SFInt32List", "MFInt32", "SFVec3fList", "MFVec3f",
  "SFVec4f", "SFVec4fList", "MFVec4f", "NodeBegin", "NodeEnd", "Name",
  "SetCircuitType", "Level", "DefaultBoundCond", "Import", "UnitsElements",
  "UnitsElement", "$@1", "$@2", "$@3", "UnitsBegin", "Units",
  "InvariantElements", "InvariantElement", "$@4", "$@5", "$@6", "$@7",
  "InvariantBegin", "Invariant", "MaterialElements", "MaterialElement",
  "$@8", "$@9", "$@10", "$@11", "$@12", "$@13", "$@14", "$@15", "$@16",
  "$@17", "$@18", "$@19", "$@20", "$@21", "$@22", "$@23", "$@24", "$@25",
  "$@26", "$@27", "$@28", "MaterialBegin", "Material", "VolumeType",
  "VolumeElements", "VolumeElement", "$@29", "$@30", "$@31", "$@32",
  "$@33", "$@34", "$@35", "$@36", "$@37", "$@38", "$@39", "$@40", "$@41",
  "$@42", "$@43", "$@44", "$@45", "VolumeBegin", "Volume", "CircuitName", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-188)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-3)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -188,     7,   191,  -188,  -188,    16,    20,    20,    30,    34,
      36,  -188,  -188,  -188,  -188,  -188,   -44,  -188,   -44,  -188,
     -44,  -188,   -44,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
      10,    45,  -188,  -188,    -4,    -1,     0,    -3,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,   -21,    -4,  -188,    38,
     -21,    -1,   -22,    31,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,   -21,     0,   158,   157,    57,    57,    57,
     -11,  -188,  -188,    11,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,    60,    57,    57,    11,    11,    57,    57,    57,
      57,    15,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,    68,  -188,   -21,   157,  -188,  -188,  -188,  -188,
    -188,  -188,    57,    57,  -188,  -188,    57,    11,    11,    57,
      57,    13,  -188,    32,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,    20,
    -188,  -188,    20,    20,    20,    20,    20,    20,    20,    20,
      20,    20,    20,    20,    20,    57,    57,    20,  -188,  -188,
    -188,  -188,  -188,     2,    57,  -188,  -188,  -188,  -188,  -188,
      57,    57,  -188,  -188,    57,  -188,    57,    57,    57,    57,
      57,    57,    57,  -188,     5,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,     9,    57,  -188,    57,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,    57,  -188,  -188
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,    14,     0,     0,     0,     0,     0,
       0,     4,     6,     8,     9,    10,     0,     5,     0,     7,
       0,    11,     0,    12,    13,    45,    15,    42,    43,    41,
       0,     0,    44,    38,    47,    57,    69,   126,   112,   164,
      66,    54,   166,    48,    50,    52,     0,    47,    64,     0,
       0,    57,     0,     0,    70,    72,    82,    84,    78,    80,
      74,    76,   110,     0,    69,     0,   128,     0,     0,     0,
      39,    55,    46,     0,    58,    62,    60,    67,    56,    88,
      90,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,    68,   114,   115,   116,   117,   118,   119,
     120,   122,   123,   121,   124,   125,   130,   132,   134,   138,
     140,   142,   144,   146,   148,   136,   150,   152,   154,   156,
     158,   160,     0,   162,     0,   128,    17,    16,    49,    51,
      53,    40,    28,     0,    29,    65,     0,     0,     0,     0,
       0,     0,    94,     0,    98,   104,   102,    96,   106,   108,
      92,    71,    73,    83,    85,    79,    81,    75,    77,    22,
      23,   111,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   129,   163,
     165,   127,    25,     0,     0,    59,    63,    61,    89,    91,
      35,     0,    36,    87,     0,   100,     0,     0,     0,     0,
       0,     0,     0,    19,     0,   131,   133,   135,   139,   141,
     143,   145,   147,   149,   137,   151,   153,   155,   157,   159,
     161,    27,    30,    26,    18,    32,     0,     0,    95,     0,
      99,   105,   103,    97,   107,   109,    93,    21,    24,    20,
      34,    37,    33,     0,   101,    31
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -188,  -188,  -188,  -188,    -7,   -58,  -128,  -188,  -188,  -188,
     -84,  -187,  -188,  -188,    29,   -28,  -188,  -188,  -188,  -188,
    -188,    27,  -188,  -188,  -188,  -188,  -188,  -188,    25,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,    22,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,   -46,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    11,    27,   133,   134,   204,   161,   183,
     135,   192,   226,   193,    34,    71,    30,    12,    13,    14,
      15,    46,    47,    67,    68,    69,    16,    17,    50,    51,
     136,   138,   137,    73,    18,    19,    63,    64,    83,    84,
      89,    90,    87,    88,    85,    86,   141,   139,   140,   202,
     194,   199,   196,   229,   198,   197,   200,   201,    91,    20,
      21,    66,   124,   125,   162,   163,   164,   171,   165,   166,
     167,   168,   169,   170,   172,   173,   174,   175,   176,   177,
     179,    22,    23,    24
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      28,   153,   154,   225,   182,   126,   127,     3,    26,   128,
     129,   130,   126,   127,   126,   127,   126,   127,    26,    79,
      80,    25,    77,    26,    38,   151,   152,    48,    49,   155,
     156,   157,   158,    43,    44,    92,    29,    45,    81,   242,
      52,    32,    33,    39,    40,    41,    31,    35,    53,    36,
      42,    37,    65,   186,   187,   223,    54,    55,    56,    57,
     126,   127,    58,    59,    60,    61,    70,    62,    74,    75,
      76,    82,   131,   178,    72,   184,    78,   195,   185,   181,
       0,   188,   189,   191,   160,   221,    93,   222,   237,     0,
     238,     0,   240,     0,   241,   132,   180,   190,   142,   159,
       0,     0,     0,   143,   144,     0,   145,   146,     0,   147,
     148,   149,     0,     0,     0,     0,     0,   218,   219,     0,
       0,   150,     0,     0,     0,     0,   224,     0,     0,     0,
       0,     0,   191,   227,     0,     0,   228,     0,   230,   231,
     232,   233,   234,   235,   236,     0,     0,     0,     0,     0,
       0,     0,   203,     0,     0,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   191,   243,
     220,   244,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   245,     0,     0,     0,     0,
       0,    -2,     4,     0,     0,     0,     0,   239,     5,     6,
       7,     8,     9,     0,    10,     0,     0,     0,     0,   119,
     120,   121,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   122,     0,     0,   123,     0,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105
};

static const yytype_int16 yycheck[] =
{
       7,    85,    86,   190,   132,     3,     4,     0,     3,    67,
      68,    69,     3,     4,     3,     4,     3,     4,     3,    41,
      42,     5,    50,     3,    14,    83,    84,    28,    29,    87,
      88,    89,    90,    37,    38,    63,     6,    41,    60,   226,
      40,     5,    86,    33,    34,    35,    12,    18,    48,    20,
       5,    22,    55,   137,   138,   183,    56,    57,    58,    59,
       3,     4,    62,    63,    64,    65,    87,    67,    30,    31,
      32,    40,    83,     5,    47,   133,    51,    45,   136,   125,
      -1,   139,   140,   141,    91,    83,    64,    85,    83,    -1,
      85,    -1,    83,    -1,    85,    84,   124,    84,    38,    84,
      -1,    -1,    -1,    43,    44,    -1,    46,    47,    -1,    49,
      50,    51,    -1,    -1,    -1,    -1,    -1,   175,   176,    -1,
      -1,    61,    -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,
      -1,    -1,   190,   191,    -1,    -1,   194,    -1,   196,   197,
     198,   199,   200,   201,   202,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   226,   227,
     177,   229,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,   243,    -1,    -1,    -1,    -1,
      -1,     0,     1,    -1,    -1,    -1,    -1,   204,     7,     8,
       9,    10,    11,    -1,    13,    -1,    -1,    -1,    -1,    52,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    89,    90,     0,     1,     7,     8,     9,    10,    11,
      13,    91,   105,   106,   107,   108,   114,   115,   122,   123,
     147,   148,   169,   170,   171,     5,     3,    92,    92,     6,
     104,    12,     5,    86,   102,   102,   102,   102,    14,    33,
      34,    35,     5,    37,    38,    41,   109,   110,    28,    29,
     116,   117,    40,    48,    56,    57,    58,    59,    62,    63,
      64,    65,    67,   124,   125,    55,   149,   111,   112,   113,
      87,   103,   109,   121,    30,    31,    32,   103,   116,    41,
      42,    60,    40,   126,   127,   132,   133,   130,   131,   128,
     129,   146,   103,   124,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    52,
      53,    54,    66,    69,   150,   151,     3,     4,    93,    93,
      93,    83,    84,    93,    94,    98,   118,   120,   119,   135,
     136,   134,    38,    43,    44,    46,    47,    49,    50,    51,
      61,    93,    93,    98,    98,    93,    93,    93,    93,    84,
      92,    96,   152,   153,   154,   156,   157,   158,   159,   160,
     161,   155,   162,   163,   164,   165,   166,   167,     5,   168,
     103,   150,    94,    97,    93,    93,    98,    98,    93,    93,
      84,    93,    99,   101,   138,    45,   140,   143,   142,   139,
     144,   145,   137,    92,    95,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    93,    93,
      92,    83,    85,    94,    93,    99,   100,    93,    93,   141,
      93,    93,    93,    93,    93,    93,    93,    83,    85,    92,
      83,    85,    99,    93,    93,    93
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    88,    89,    90,    90,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    92,    93,    93,    94,    95,
      95,    95,    95,    96,    96,    97,    97,    97,    97,    98,
      98,    99,   100,   100,   100,   100,   101,   101,   102,   103,
     103,   104,   105,   106,   107,   108,   109,   109,   111,   110,
     112,   110,   113,   110,   114,   115,   116,   116,   118,   117,
     119,   117,   120,   117,   121,   117,   122,   123,   124,   124,
     126,   125,   127,   125,   128,   125,   129,   125,   130,   125,
     131,   125,   132,   125,   133,   125,   134,   125,   135,   125,
     136,   125,   137,   125,   138,   125,   139,   125,   140,   125,
     141,   125,   142,   125,   143,   125,   144,   125,   145,   125,
     146,   125,   147,   148,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   150,   150,   151,
     152,   151,   153,   151,   154,   151,   155,   151,   156,   151,
     157,   151,   158,   151,   159,   151,   160,   151,   161,   151,
     162,   151,   163,   151,   164,   151,   165,   151,   166,   151,
     167,   151,   168,   151,   169,   170,   171
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       2,     2,     0,     1,     3,     1,     2,     2,     0,     1,
       3,     4,     1,     2,     2,     0,     1,     3,     1,     1,
       2,     1,     2,     2,     2,     2,     2,     0,     0,     3,
       0,     3,     0,     3,     3,     4,     2,     0,     0,     4,
       0,     4,     0,     4,     0,     3,     3,     4,     2,     0,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     4,     0,     4,
       0,     4,     0,     5,     0,     5,     0,     5,     0,     5,
       0,     6,     0,     5,     0,     5,     0,     5,     0,     5,
       0,     3,     3,     4,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     0,     2,     0,     2,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     2,     3,     5,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 14: /* MwmItem: error  */
#line 151 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                        {YYABORT;}
#line 1516 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 15: /* SFInt32: INTEGER  */
#line 162 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
			if(Ibuff) Ibuff->input((yyvsp[0].ival));
		}
#line 1524 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 16: /* SFFloat: FLOATING  */
#line 174 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
			if(Fbuff) Fbuff->input((yyvsp[0].fval));
		}
#line 1532 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 17: /* SFFloat: INTEGER  */
#line 178 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
			(yyval.fval) = (double)(yyvsp[0].ival);
			if(Fbuff) Fbuff->input((double)(yyvsp[0].ival));
		}
#line 1541 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 18: /* SFVec3f: SFFloat SFFloat SFFloat  */
#line 191 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                  {}
#line 1547 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 23: /* MFInt32: SFInt32  */
#line 203 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                  {}
#line 1553 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 24: /* MFInt32: '[' SFInt32List ']'  */
#line 204 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                              {}
#line 1559 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 31: /* SFVec4f: SFFloat SFFloat SFFloat SFFloat  */
#line 247 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                          {}
#line 1565 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 41: /* Name: NAME  */
#line 285 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
             {if (strlen((yyvsp[0].sval)) > sizeof(DB::str_t)-1) YYERROR; else strcpy(name,(yyvsp[0].sval));}
#line 1571 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 42: /* SetCircuitType: ASSEMBLYTYPE SFInt32  */
#line 291 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
			loadingEmP->assemblyType=(yyvsp[0].ival);
		}
#line 1579 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 43: /* Level: LEVEL SFInt32  */
#line 298 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
			loadingEmP->level=(yyvsp[0].ival);
		}
#line 1587 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 44: /* DefaultBoundCond: DEFAULTBC STRING  */
#line 306 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
	             strcpy(loadingEmP->defaultBC,(yyvsp[0].sval));
		}
#line 1595 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 45: /* Import: IMPORT STRING  */
#line 317 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  loadingEmP->stepFilePath=(yyvsp[0].sval);
                  if(loadingOcaf) 
                     step_file_reloaded=loadingOcaf->importSTEP_OR_DXC((yyvsp[0].sval), reload_step_file);
		}
#line 1605 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 48: /* $@1: %empty  */
#line 333 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1611 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 49: /* UnitsElement: LENGTH $@1 SFFloat  */
#line 333 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                           {unit.xm   =(yyvsp[0].fval);  }
#line 1617 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 50: /* $@2: %empty  */
#line 334 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1623 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 51: /* UnitsElement: FREQUENCY $@2 SFFloat  */
#line 334 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                           {unit.xHz  =(yyvsp[0].fval);  }
#line 1629 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 52: /* $@3: %empty  */
#line 335 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1635 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 53: /* UnitsElement: RESISTANCE $@3 SFFloat  */
#line 335 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                           {unit.xOhm =(yyvsp[0].fval);  }
#line 1641 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 58: /* $@4: %empty  */
#line 356 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1647 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 59: /* InvariantElement: ROTATION ANGLE $@4 SFFloat  */
#line 356 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                    {invt->rotAngle=(yyvsp[0].fval); }
#line 1653 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 60: /* $@5: %empty  */
#line 357 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                          {Fbuff = &DoubleBuffer; Fbuff->reset();}
#line 1659 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 61: /* InvariantElement: ROTATION AXIS $@5 MFVec3f  */
#line 358 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
            {
              DoubleBuffer.flush(dVec); 
              for(int i=0; i<3; i++) invt->rotAxis[i]=dVec[i];
            }
#line 1668 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 62: /* $@6: %empty  */
#line 362 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                            {Fbuff = &DoubleBuffer; Fbuff->reset();}
#line 1674 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 63: /* InvariantElement: ROTATION ORIGIN $@6 MFVec3f  */
#line 363 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
            {
              DoubleBuffer.flush(dVec); 
              for(int i=0; i<3; i++) invt->rotOrigin[i]=dVec[i];
            }
#line 1683 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 64: /* $@7: %empty  */
#line 367 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                          {Fbuff = &DoubleBuffer; Fbuff->reset();}
#line 1689 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 65: /* InvariantElement: TRANSLATION $@7 MFVec3f  */
#line 368 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
            {
              DoubleBuffer.flush(dVec); 
              for(int i=0; i<3; i++) invt->trasl[i]=dVec[i];
            }
#line 1698 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 66: /* InvariantBegin: DEF Name MWM_INVARIANT  */
#line 376 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
         {     
              invt =loadingEmP->FindInvariant(name);
              if(invt) loadingEmP->delInvariant(invt);
	      invt = new DB::Transform();
	      strcpy(invt->name,name);
	      loadingEmP->addInvariant(invt);
	 }
#line 1710 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 70: /* $@8: %empty  */
#line 398 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1716 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 71: /* MaterialElement: EPSILONR $@8 SFFloat  */
#line 398 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->epsr=(yyvsp[0].fval);     }
#line 1722 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 72: /* $@9: %empty  */
#line 399 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1728 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 73: /* MaterialElement: MUR $@9 SFFloat  */
#line 399 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->mur=(yyvsp[0].fval);          }
#line 1734 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 74: /* $@10: %empty  */
#line 400 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1740 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 75: /* MaterialElement: ETANDELTA $@10 SFFloat  */
#line 400 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->etandelta=(yyvsp[0].fval);}
#line 1746 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 76: /* $@11: %empty  */
#line 401 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1752 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 77: /* MaterialElement: HTANDELTA $@11 SFFloat  */
#line 401 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->htandelta=(yyvsp[0].fval);}
#line 1758 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 78: /* $@12: %empty  */
#line 402 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1764 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 79: /* MaterialElement: ECONDUCTIVITY $@12 SFFloat  */
#line 402 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->econductivity=(yyvsp[0].fval);}
#line 1770 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 80: /* $@13: %empty  */
#line 403 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1776 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 81: /* MaterialElement: HCONDUCTIVITY $@13 SFFloat  */
#line 403 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->hconductivity=(yyvsp[0].fval);}
#line 1782 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 82: /* $@14: %empty  */
#line 404 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff  =&mat->buff.epsLorentz; Fbuff->reset();}
#line 1788 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 84: /* $@15: %empty  */
#line 405 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff  =&mat->buff.muLorentz; Fbuff->reset();}
#line 1794 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 86: /* $@16: %empty  */
#line 406 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff  =&mat->buff.surfPolesRes; Fbuff->reset();}
#line 1800 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 88: /* $@17: %empty  */
#line 407 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1806 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 89: /* MaterialElement: SURFACE RESISTANCE $@17 SFFloat  */
#line 407 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->Sresistance=(yyvsp[0].fval); }
#line 1812 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 90: /* $@18: %empty  */
#line 408 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1818 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 91: /* MaterialElement: SURFACE INDUCTANCE $@18 SFFloat  */
#line 408 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->Sinductance=(yyvsp[0].fval); }
#line 1824 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 92: /* $@19: %empty  */
#line 409 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1830 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 93: /* MaterialElement: ROUGH SURFACE POLESNUM $@19 SFFloat  */
#line 409 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfFitPolesNum=(yyvsp[0].fval); }
#line 1836 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 94: /* $@20: %empty  */
#line 410 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1842 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 95: /* MaterialElement: ROUGH SURFACE FREQUENCY $@20 SFFloat  */
#line 410 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfFreq=(yyvsp[0].fval); }
#line 1848 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 96: /* $@21: %empty  */
#line 411 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1854 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 97: /* MaterialElement: ROUGH SURFACE MODELTYPE $@21 SFFloat  */
#line 411 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfModelType=(yyvsp[0].fval); }
#line 1860 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 98: /* $@22: %empty  */
#line 412 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1866 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 99: /* MaterialElement: ROUGH SURFACE LOSSFACTOR $@22 SFFloat  */
#line 412 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfLossFactor=(yyvsp[0].fval);}
#line 1872 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 100: /* $@23: %empty  */
#line 413 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                           {Fbuff = NULL;}
#line 1878 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 101: /* MaterialElement: ROUGH SURFACE IMPEDANCE QFACTOR $@23 SFFloat  */
#line 413 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfImpedanceQ=(yyvsp[0].fval);}
#line 1884 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 102: /* $@24: %empty  */
#line 414 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1890 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 103: /* MaterialElement: ROUGH SURFACE BALLRADIUS $@24 SFFloat  */
#line 414 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfBallRadius=(yyvsp[0].fval);}
#line 1896 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 104: /* $@25: %empty  */
#line 415 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1902 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 105: /* MaterialElement: ROUGH SURFACE SURFRATIO $@25 SFFloat  */
#line 415 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfSurfRatio=(yyvsp[0].fval);}
#line 1908 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 106: /* $@26: %empty  */
#line 416 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1914 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 107: /* MaterialElement: ROUGH SURFACE RZ $@26 SFFloat  */
#line 416 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfRz=(yyvsp[0].fval);}
#line 1920 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 108: /* $@27: %empty  */
#line 417 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1926 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 109: /* MaterialElement: ROUGH SURFACE RQ $@27 SFFloat  */
#line 417 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfRq=(yyvsp[0].fval);}
#line 1932 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 110: /* $@28: %empty  */
#line 418 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    { Ibuff = &IntBuffer; Ibuff->reset();}
#line 1938 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 111: /* MaterialElement: COLOR $@28 MFInt32  */
#line 419 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
            {
              IntBuffer.flush(matColor); 
              for(int i=0; i<4; i++) mat->color[i]=matColor[i];
            }
#line 1947 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 112: /* MaterialBegin: DEF Name MWM_MATERIAL  */
#line 427 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {     
                        mat =loadingEmP->FindMaterial(name);
                        if(mat) loadingEmP->delMaterial(mat);
			mat = new DB::Material();
			strcpy(mat->name,name);
			loadingEmP->addMaterial(mat);
		}
#line 1959 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 113: /* Material: MaterialBegin NodeBegin MaterialElements NodeEnd  */
#line 438 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
          { 
             mat->flush(); 
          }
#line 1967 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 114: /* VolumeType: VOLTYPE tDIELECTRIC  */
#line 449 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=DIELECTRIC;
                }
#line 1975 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 115: /* VolumeType: VOLTYPE tHOLE  */
#line 453 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=HOLE;
                }
#line 1983 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 116: /* VolumeType: VOLTYPE tBOUNDARYCONDITION  */
#line 457 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=BOUNDARYCOND;
                }
#line 1991 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 117: /* VolumeType: VOLTYPE tWAVEGUIDE  */
#line 461 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=WAVEGUIDE;
                }
#line 1999 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 118: /* VolumeType: VOLTYPE tLINEPORT  */
#line 465 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=LINEPORT;
                }
#line 2007 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 119: /* VolumeType: VOLTYPE tSPLITTER  */
#line 469 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=SPLITTER;
                }
#line 2015 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 120: /* VolumeType: VOLTYPE tGRID  */
#line 473 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=GRID;
                }
#line 2023 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 121: /* VolumeType: VOLTYPE tNET  */
#line 477 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=NET;
                }
#line 2031 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 122: /* VolumeType: VOLTYPE tCOMPONENT  */
#line 481 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=COMPONENT;
                }
#line 2039 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 123: /* VolumeType: VOLTYPE tINTERFACE  */
#line 485 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=INTERFACE;
                }
#line 2047 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 124: /* VolumeType: VOLTYPE tASSEMBLY  */
#line 489 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=ASSEMBLY;
                }
#line 2055 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 125: /* VolumeType: VOLTYPE tUNDEFINED  */
#line 493 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=UNDEFINED;
                }
#line 2063 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 129: /* VolumeElement: MATERIAL STRING  */
#line 507 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                   if (strlen((yyvsp[0].sval)) > sizeof(DB::str_t)-1) YYERROR;
                   if(!loadingEmP->FindMaterial((yyvsp[0].sval)))  DB::yyMsg(FATAL, "Undefined material %s\n", (yyvsp[0].sval));
		   strcpy(vol->material,(yyvsp[0].sval));
                }
#line 2073 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 130: /* $@29: %empty  */
#line 512 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2079 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 131: /* VolumeElement: TEMPORTSNUM $@29 SFInt32  */
#line 513 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->TEMportsNum =(yyvsp[0].ival);
                }
#line 2087 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 132: /* $@30: %empty  */
#line 516 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2093 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 133: /* VolumeElement: TEPORTSNUM $@30 SFInt32  */
#line 517 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->TEportsNum =(yyvsp[0].ival);
                }
#line 2101 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 134: /* $@31: %empty  */
#line 520 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2107 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 135: /* VolumeElement: TMPORTSNUM $@31 SFInt32  */
#line 521 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->TMportsNum =(yyvsp[0].ival);
                }
#line 2115 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 136: /* $@32: %empty  */
#line 524 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2121 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 137: /* VolumeElement: ORIENTATION $@32 SFInt32  */
#line 525 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->orientation=(yyvsp[0].ival);
                }
#line 2129 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 138: /* $@33: %empty  */
#line 528 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                {Ibuff  =NULL;}
#line 2135 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 139: /* VolumeElement: TEMTABULARORDER1 $@33 SFInt32  */
#line 529 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->TEM_TabularOrder1=(yyvsp[0].ival);
                }
#line 2143 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 140: /* $@34: %empty  */
#line 532 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                {Ibuff  =NULL;}
#line 2149 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 141: /* VolumeElement: TEMTABULARORDER2 $@34 SFInt32  */
#line 533 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->TEM_TabularOrder2=(yyvsp[0].ival);
                }
#line 2157 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 142: /* $@35: %empty  */
#line 536 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                {Ibuff  =NULL;}
#line 2163 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 143: /* VolumeElement: TEMREFCONDUCTOR $@35 SFInt32  */
#line 537 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->TEM_refCond=(yyvsp[0].ival);
                }
#line 2171 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 144: /* $@36: %empty  */
#line 540 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                {Ibuff  =NULL;}
#line 2177 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 145: /* VolumeElement: DISCONNECTEDTEM $@36 SFInt32  */
#line 541 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->disconnectedTEM =(yyvsp[0].ival);
                }
#line 2185 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 146: /* $@37: %empty  */
#line 544 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                {Ibuff  =NULL;}
#line 2191 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 147: /* VolumeElement: SUBCIRCUITINDEX $@37 SFInt32  */
#line 545 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->subcircuitIndex =(yyvsp[0].ival);
                }
#line 2199 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 148: /* $@38: %empty  */
#line 548 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                {Ibuff  =NULL;}
#line 2205 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 149: /* VolumeElement: SUBCIRCUITIFINDEX $@38 SFInt32  */
#line 549 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->subcircuitIFindex =(yyvsp[0].ival);
                }
#line 2213 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 150: /* $@39: %empty  */
#line 552 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2219 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 151: /* VolumeElement: GRIDNUM $@39 SFInt32  */
#line 553 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->gridNum =(yyvsp[0].ival);
                }
#line 2227 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 152: /* $@40: %empty  */
#line 556 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {Ibuff  =NULL;}
#line 2233 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 153: /* VolumeElement: PML $@40 SFInt32  */
#line 557 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->PML =(yyvsp[0].ival);
                }
#line 2241 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 154: /* $@41: %empty  */
#line 560 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                      {Ibuff  =NULL;}
#line 2247 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 155: /* VolumeElement: INVARIANT $@41 SFInt32  */
#line 561 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->invariant =(yyvsp[0].ival);
                }
#line 2255 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 156: /* $@42: %empty  */
#line 564 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 2261 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 157: /* VolumeElement: MESHREFINEMENT $@42 SFFloat  */
#line 564 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                             {vol->meshRefinement=(yyvsp[0].fval); }
#line 2267 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 158: /* $@43: %empty  */
#line 565 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 2273 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 159: /* VolumeElement: CUTOFFREFINEMENT $@43 SFFloat  */
#line 565 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                             {vol->cutoffRefinement=(yyvsp[0].fval); }
#line 2279 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 160: /* $@44: %empty  */
#line 567 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             {Ibuff = NULL;}
#line 2285 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 161: /* VolumeElement: COMPSOLID $@44 SFInt32  */
#line 567 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                             { }
#line 2291 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 162: /* $@45: %empty  */
#line 569 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             { }
#line 2297 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 163: /* VolumeElement: DISABLED $@45  */
#line 569 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                             {vol->disabled=1; }
#line 2303 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 164: /* VolumeBegin: DEF Name MWM_VOLUME  */
#line 573 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
			vol = new DB::Volume();
			strcpy(vol->name,name);
			loadingEmP->insertVolume(vol);
		}
#line 2313 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 166: /* CircuitName: SET CIRCUITNAME STRING  */
#line 591 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
            {
             if (strlen((yyvsp[0].sval))>sizeof(DB::str_t)-1) YYERROR; 
             else strcpy(mainCircName,(yyvsp[0].sval));
            }
#line 2322 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;


#line 2326 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 597 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"



extern void modelSetInputFile(FILE *fp);


void yyerror(char *s){
  printf("\n  %s (%s) at file: %s, line: %d\n\n", s, modeltext, DB::yyFileName, DB::yyLineNum);
}

static void yyprint (FILE *file, int type, YYSTYPE value)
{
  if (type==STRING || type==NAME)
     fprintf(file, " %s", value.sval);
  else if (type==FLOATING)
     fprintf(file, " %f", value.fval);
  else if (type==INTEGER)
     fprintf(file, " %d", value.ival);
}


void model_msg(int type, char *fmt, ...){
  va_list args;
  char tmp[1024];

  va_start (args, fmt);
  vsprintf (tmp, fmt, args);
  va_end (args);

  DB::Msg(type, "'%s', line %d : %s", DB::yyFileName, DB::yyLineNum, tmp);

}


int loadModel(MwOCAF* ocaf, const char *fName, bool update) 
{
        reload_step_file=update;
        loadingOcaf=ocaf;
        loadingEmP=loadingOcaf->EmP;
        FILE *fid;
        if(!(fid = fopen(fName, "r"))){
		DB::Msg(ERROR, "Cannot open file %s\n", fName);
		return 1;
	}
        strncpy(DB::yyFileName, fName, 255);
        modelSetInputFile(fid);
        step_file_reloaded=false;
        int result=yyparse();
        if(strcmp(fName,"-")) fclose(fid);
	return step_file_reloaded;
}


int loadProblem(DB::EmProblem* EmP, const char *fName) 
{
        loadingOcaf=NULL;
        loadingEmP=EmP;
        FILE *fid;
        if(!(fid = fopen(fName, "r"))){
		DB::Msg(ERROR, "Cannot open file %s\n", fName);
		return 1;
	}
        strncpy(DB::yyFileName, fName, 255);
        modelSetInputFile(fid);
        step_file_reloaded=false;
        int result=yyparse();
        if(strcmp(fName,"-")) fclose(fid);
	return step_file_reloaded;
}


