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
  YYSYMBOL_ORIENTATION = 22,               /* ORIENTATION  */
  YYSYMBOL_GRIDNUM = 23,                   /* GRIDNUM  */
  YYSYMBOL_PML = 24,                       /* PML  */
  YYSYMBOL_INVARIANT = 25,                 /* INVARIANT  */
  YYSYMBOL_TRANSLATION = 26,               /* TRANSLATION  */
  YYSYMBOL_ROTATION = 27,                  /* ROTATION  */
  YYSYMBOL_ANGLE = 28,                     /* ANGLE  */
  YYSYMBOL_ORIGIN = 29,                    /* ORIGIN  */
  YYSYMBOL_AXIS = 30,                      /* AXIS  */
  YYSYMBOL_MWM_VOLUME = 31,                /* MWM_VOLUME  */
  YYSYMBOL_MWM_INVARIANT = 32,             /* MWM_INVARIANT  */
  YYSYMBOL_MWM_UNITS = 33,                 /* MWM_UNITS  */
  YYSYMBOL_MWM_LINEPORT = 34,              /* MWM_LINEPORT  */
  YYSYMBOL_LENGTH = 35,                    /* LENGTH  */
  YYSYMBOL_FREQUENCY = 36,                 /* FREQUENCY  */
  YYSYMBOL_BAND = 37,                      /* BAND  */
  YYSYMBOL_SURFACE = 38,                   /* SURFACE  */
  YYSYMBOL_RESISTANCE = 39,                /* RESISTANCE  */
  YYSYMBOL_INDUCTANCE = 40,                /* INDUCTANCE  */
  YYSYMBOL_IMPEDANCE = 41,                 /* IMPEDANCE  */
  YYSYMBOL_LOSSFACTOR = 42,                /* LOSSFACTOR  */
  YYSYMBOL_QFACTOR = 43,                   /* QFACTOR  */
  YYSYMBOL_SURFRATIO = 44,                 /* SURFRATIO  */
  YYSYMBOL_BALLRADIUS = 45,                /* BALLRADIUS  */
  YYSYMBOL_ROUGH = 46,                     /* ROUGH  */
  YYSYMBOL_MODELTYPE = 47,                 /* MODELTYPE  */
  YYSYMBOL_RZ = 48,                        /* RZ  */
  YYSYMBOL_RQ = 49,                        /* RQ  */
  YYSYMBOL_MESHREFINEMENT = 50,            /* MESHREFINEMENT  */
  YYSYMBOL_CUTOFFREFINEMENT = 51,          /* CUTOFFREFINEMENT  */
  YYSYMBOL_COMPSOLID = 52,                 /* COMPSOLID  */
  YYSYMBOL_VOLTYPE = 53,                   /* VOLTYPE  */
  YYSYMBOL_EPSILONR = 54,                  /* EPSILONR  */
  YYSYMBOL_MUR = 55,                       /* MUR  */
  YYSYMBOL_EPSLORENTZ = 56,                /* EPSLORENTZ  */
  YYSYMBOL_MULORENTZ = 57,                 /* MULORENTZ  */
  YYSYMBOL_POLESRESIDUES = 58,             /* POLESRESIDUES  */
  YYSYMBOL_POLESNUM = 59,                  /* POLESNUM  */
  YYSYMBOL_ECONDUCTIVITY = 60,             /* ECONDUCTIVITY  */
  YYSYMBOL_HCONDUCTIVITY = 61,             /* HCONDUCTIVITY  */
  YYSYMBOL_ETANDELTA = 62,                 /* ETANDELTA  */
  YYSYMBOL_HTANDELTA = 63,                 /* HTANDELTA  */
  YYSYMBOL_MATERIAL = 64,                  /* MATERIAL  */
  YYSYMBOL_COLOR = 65,                     /* COLOR  */
  YYSYMBOL_VOLUMES = 66,                   /* VOLUMES  */
  YYSYMBOL_DISABLED = 67,                  /* DISABLED  */
  YYSYMBOL_tDIELECTRIC = 68,               /* tDIELECTRIC  */
  YYSYMBOL_tHOLE = 69,                     /* tHOLE  */
  YYSYMBOL_tBOUNDARYCONDITION = 70,        /* tBOUNDARYCONDITION  */
  YYSYMBOL_tWAVEGUIDE = 71,                /* tWAVEGUIDE  */
  YYSYMBOL_tLINEPORT = 72,                 /* tLINEPORT  */
  YYSYMBOL_tSPLITTER = 73,                 /* tSPLITTER  */
  YYSYMBOL_tGRID = 74,                     /* tGRID  */
  YYSYMBOL_tCOMPONENT = 75,                /* tCOMPONENT  */
  YYSYMBOL_tINTERFACE = 76,                /* tINTERFACE  */
  YYSYMBOL_tNET = 77,                      /* tNET  */
  YYSYMBOL_tASSEMBLY = 78,                 /* tASSEMBLY  */
  YYSYMBOL_tUNDEFINED = 79,                /* tUNDEFINED  */
  YYSYMBOL_SOLID = 80,                     /* SOLID  */
  YYSYMBOL_81_ = 81,                       /* ','  */
  YYSYMBOL_82_ = 82,                       /* '['  */
  YYSYMBOL_83_ = 83,                       /* ']'  */
  YYSYMBOL_84_ = 84,                       /* '{'  */
  YYSYMBOL_85_ = 85,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 86,                  /* $accept  */
  YYSYMBOL_All = 87,                       /* All  */
  YYSYMBOL_MwmItems = 88,                  /* MwmItems  */
  YYSYMBOL_MwmItem = 89,                   /* MwmItem  */
  YYSYMBOL_SFInt32 = 90,                   /* SFInt32  */
  YYSYMBOL_SFFloat = 91,                   /* SFFloat  */
  YYSYMBOL_SFVec3f = 92,                   /* SFVec3f  */
  YYSYMBOL_SFInt32List = 93,               /* SFInt32List  */
  YYSYMBOL_MFInt32 = 94,                   /* MFInt32  */
  YYSYMBOL_SFVec3fList = 95,               /* SFVec3fList  */
  YYSYMBOL_MFVec3f = 96,                   /* MFVec3f  */
  YYSYMBOL_SFVec4f = 97,                   /* SFVec4f  */
  YYSYMBOL_SFVec4fList = 98,               /* SFVec4fList  */
  YYSYMBOL_MFVec4f = 99,                   /* MFVec4f  */
  YYSYMBOL_NodeBegin = 100,                /* NodeBegin  */
  YYSYMBOL_NodeEnd = 101,                  /* NodeEnd  */
  YYSYMBOL_Name = 102,                     /* Name  */
  YYSYMBOL_SetCircuitType = 103,           /* SetCircuitType  */
  YYSYMBOL_Level = 104,                    /* Level  */
  YYSYMBOL_DefaultBoundCond = 105,         /* DefaultBoundCond  */
  YYSYMBOL_Import = 106,                   /* Import  */
  YYSYMBOL_UnitsElements = 107,            /* UnitsElements  */
  YYSYMBOL_UnitsElement = 108,             /* UnitsElement  */
  YYSYMBOL_109_1 = 109,                    /* $@1  */
  YYSYMBOL_110_2 = 110,                    /* $@2  */
  YYSYMBOL_111_3 = 111,                    /* $@3  */
  YYSYMBOL_UnitsBegin = 112,               /* UnitsBegin  */
  YYSYMBOL_Units = 113,                    /* Units  */
  YYSYMBOL_InvariantElements = 114,        /* InvariantElements  */
  YYSYMBOL_InvariantElement = 115,         /* InvariantElement  */
  YYSYMBOL_116_4 = 116,                    /* $@4  */
  YYSYMBOL_117_5 = 117,                    /* $@5  */
  YYSYMBOL_118_6 = 118,                    /* $@6  */
  YYSYMBOL_119_7 = 119,                    /* $@7  */
  YYSYMBOL_InvariantBegin = 120,           /* InvariantBegin  */
  YYSYMBOL_Invariant = 121,                /* Invariant  */
  YYSYMBOL_MaterialElements = 122,         /* MaterialElements  */
  YYSYMBOL_MaterialElement = 123,          /* MaterialElement  */
  YYSYMBOL_124_8 = 124,                    /* $@8  */
  YYSYMBOL_125_9 = 125,                    /* $@9  */
  YYSYMBOL_126_10 = 126,                   /* $@10  */
  YYSYMBOL_127_11 = 127,                   /* $@11  */
  YYSYMBOL_128_12 = 128,                   /* $@12  */
  YYSYMBOL_129_13 = 129,                   /* $@13  */
  YYSYMBOL_130_14 = 130,                   /* $@14  */
  YYSYMBOL_131_15 = 131,                   /* $@15  */
  YYSYMBOL_132_16 = 132,                   /* $@16  */
  YYSYMBOL_133_17 = 133,                   /* $@17  */
  YYSYMBOL_134_18 = 134,                   /* $@18  */
  YYSYMBOL_135_19 = 135,                   /* $@19  */
  YYSYMBOL_136_20 = 136,                   /* $@20  */
  YYSYMBOL_137_21 = 137,                   /* $@21  */
  YYSYMBOL_138_22 = 138,                   /* $@22  */
  YYSYMBOL_139_23 = 139,                   /* $@23  */
  YYSYMBOL_140_24 = 140,                   /* $@24  */
  YYSYMBOL_141_25 = 141,                   /* $@25  */
  YYSYMBOL_142_26 = 142,                   /* $@26  */
  YYSYMBOL_143_27 = 143,                   /* $@27  */
  YYSYMBOL_144_28 = 144,                   /* $@28  */
  YYSYMBOL_MaterialBegin = 145,            /* MaterialBegin  */
  YYSYMBOL_Material = 146,                 /* Material  */
  YYSYMBOL_VolumeType = 147,               /* VolumeType  */
  YYSYMBOL_VolumeElements = 148,           /* VolumeElements  */
  YYSYMBOL_VolumeElement = 149,            /* VolumeElement  */
  YYSYMBOL_150_29 = 150,                   /* $@29  */
  YYSYMBOL_151_30 = 151,                   /* $@30  */
  YYSYMBOL_152_31 = 152,                   /* $@31  */
  YYSYMBOL_153_32 = 153,                   /* $@32  */
  YYSYMBOL_154_33 = 154,                   /* $@33  */
  YYSYMBOL_155_34 = 155,                   /* $@34  */
  YYSYMBOL_156_35 = 156,                   /* $@35  */
  YYSYMBOL_157_36 = 157,                   /* $@36  */
  YYSYMBOL_158_37 = 158,                   /* $@37  */
  YYSYMBOL_159_38 = 159,                   /* $@38  */
  YYSYMBOL_160_39 = 160,                   /* $@39  */
  YYSYMBOL_161_40 = 161,                   /* $@40  */
  YYSYMBOL_162_41 = 162,                   /* $@41  */
  YYSYMBOL_163_42 = 163,                   /* $@42  */
  YYSYMBOL_164_43 = 164,                   /* $@43  */
  YYSYMBOL_VolumeBegin = 165,              /* VolumeBegin  */
  YYSYMBOL_Volume = 166,                   /* Volume  */
  YYSYMBOL_CircuitName = 167               /* CircuitName  */
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
#define YYLAST   233

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  86
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  82
/* YYNRULES -- Number of rules.  */
#define YYNRULES  162
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  240

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   335


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
       2,     2,     2,     2,    81,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    82,     2,    83,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    84,     2,    85,     2,     2,     2,     2,
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
      75,    76,    77,    78,    79,    80
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   131,   131,   135,   136,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   160,   172,   176,   190,   195,
     196,   197,   198,   202,   203,   233,   234,   235,   236,   240,
     241,   246,   250,   251,   252,   253,   257,   258,   274,   278,
     279,   284,   289,   296,   304,   315,   327,   328,   332,   332,
     333,   333,   334,   334,   338,   342,   350,   351,   355,   355,
     356,   356,   361,   361,   366,   366,   374,   384,   392,   393,
     397,   397,   398,   398,   399,   399,   400,   400,   401,   401,
     402,   402,   403,   403,   404,   404,   405,   405,   406,   406,
     407,   407,   408,   408,   409,   409,   410,   410,   411,   411,
     412,   412,   413,   413,   414,   414,   415,   415,   416,   416,
     417,   417,   425,   436,   447,   451,   455,   459,   463,   467,
     471,   475,   479,   483,   487,   491,   495,   500,   501,   505,
     511,   511,   515,   515,   519,   519,   523,   523,   527,   527,
     531,   531,   535,   535,   539,   539,   543,   543,   547,   547,
     551,   551,   555,   555,   556,   556,   558,   558,   560,   560,
     563,   572,   581
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
  "DISCONNECTEDTEM", "ORIENTATION", "GRIDNUM", "PML", "INVARIANT",
  "TRANSLATION", "ROTATION", "ANGLE", "ORIGIN", "AXIS", "MWM_VOLUME",
  "MWM_INVARIANT", "MWM_UNITS", "MWM_LINEPORT", "LENGTH", "FREQUENCY",
  "BAND", "SURFACE", "RESISTANCE", "INDUCTANCE", "IMPEDANCE", "LOSSFACTOR",
  "QFACTOR", "SURFRATIO", "BALLRADIUS", "ROUGH", "MODELTYPE", "RZ", "RQ",
  "MESHREFINEMENT", "CUTOFFREFINEMENT", "COMPSOLID", "VOLTYPE", "EPSILONR",
  "MUR", "EPSLORENTZ", "MULORENTZ", "POLESRESIDUES", "POLESNUM",
  "ECONDUCTIVITY", "HCONDUCTIVITY", "ETANDELTA", "HTANDELTA", "MATERIAL",
  "COLOR", "VOLUMES", "DISABLED", "tDIELECTRIC", "tHOLE",
  "tBOUNDARYCONDITION", "tWAVEGUIDE", "tLINEPORT", "tSPLITTER", "tGRID",
  "tCOMPONENT", "tINTERFACE", "tNET", "tASSEMBLY", "tUNDEFINED", "SOLID",
  "','", "'['", "']'", "'{'", "'}'", "$accept", "All", "MwmItems",
  "MwmItem", "SFInt32", "SFFloat", "SFVec3f", "SFInt32List", "MFInt32",
  "SFVec3fList", "MFVec3f", "SFVec4f", "SFVec4fList", "MFVec4f",
  "NodeBegin", "NodeEnd", "Name", "SetCircuitType", "Level",
  "DefaultBoundCond", "Import", "UnitsElements", "UnitsElement", "$@1",
  "$@2", "$@3", "UnitsBegin", "Units", "InvariantElements",
  "InvariantElement", "$@4", "$@5", "$@6", "$@7", "InvariantBegin",
  "Invariant", "MaterialElements", "MaterialElement", "$@8", "$@9", "$@10",
  "$@11", "$@12", "$@13", "$@14", "$@15", "$@16", "$@17", "$@18", "$@19",
  "$@20", "$@21", "$@22", "$@23", "$@24", "$@25", "$@26", "$@27", "$@28",
  "MaterialBegin", "Material", "VolumeType", "VolumeElements",
  "VolumeElement", "$@29", "$@30", "$@31", "$@32", "$@33", "$@34", "$@35",
  "$@36", "$@37", "$@38", "$@39", "$@40", "$@41", "$@42", "$@43",
  "VolumeBegin", "Volume", "CircuitName", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-182)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-3)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -182,     9,   179,  -182,  -182,    18,    38,    38,    36,    34,
      45,  -182,  -182,  -182,  -182,  -182,   -32,  -182,   -32,  -182,
     -32,  -182,   -32,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
      12,    55,  -182,  -182,   -14,     3,     2,     8,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,   -19,   -14,  -182,    40,
     -19,     3,   -20,    33,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,  -182,  -182,   -19,     2,   154,   153,    32,    32,    32,
      -9,  -182,  -182,    -2,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,  -182,    62,    32,    32,    -2,    -2,    32,    32,    32,
      32,    15,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
      68,  -182,   -19,   153,  -182,  -182,  -182,  -182,  -182,  -182,
      32,    32,  -182,  -182,    32,    -2,    -2,    32,    32,    13,
    -182,    31,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,  -182,    38,  -182,  -182,
      38,    38,    38,    38,    38,    38,    38,    38,    38,    38,
      38,    32,    32,    38,  -182,  -182,  -182,  -182,  -182,     4,
      32,  -182,  -182,  -182,  -182,  -182,    32,    32,  -182,  -182,
      32,  -182,    32,    32,    32,    32,    32,    32,    32,  -182,
       7,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
      11,    32,  -182,    32,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,  -182,    32,  -182,  -182
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,    14,     0,     0,     0,     0,     0,
       0,     4,     6,     8,     9,    10,     0,     5,     0,     7,
       0,    11,     0,    12,    13,    45,    15,    42,    43,    41,
       0,     0,    44,    38,    47,    57,    69,   126,   112,   160,
      66,    54,   162,    48,    50,    52,     0,    47,    64,     0,
       0,    57,     0,     0,    70,    72,    82,    84,    78,    80,
      74,    76,   110,     0,    69,     0,   128,     0,     0,     0,
      39,    55,    46,     0,    58,    62,    60,    67,    56,    88,
      90,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,    68,   114,   115,   116,   117,   118,   119,
     120,   122,   123,   121,   124,   125,   130,   132,   134,   138,
     140,   142,   144,   136,   146,   148,   150,   152,   154,   156,
       0,   158,     0,   128,    17,    16,    49,    51,    53,    40,
      28,     0,    29,    65,     0,     0,     0,     0,     0,     0,
      94,     0,    98,   104,   102,    96,   106,   108,    92,    71,
      73,    83,    85,    79,    81,    75,    77,    22,    23,   111,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   129,   159,   161,   127,    25,     0,
       0,    59,    63,    61,    89,    91,    35,     0,    36,    87,
       0,   100,     0,     0,     0,     0,     0,     0,     0,    19,
       0,   131,   133,   135,   139,   141,   143,   145,   137,   147,
     149,   151,   153,   155,   157,    27,    30,    26,    18,    32,
       0,     0,    95,     0,    99,   105,   103,    97,   107,   109,
      93,    21,    24,    20,    34,    37,    33,     0,   101,    31
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -182,  -182,  -182,  -182,    -7,   -56,  -124,  -182,  -182,  -182,
     -82,  -181,  -182,  -182,    29,   -26,  -182,  -182,  -182,  -182,
    -182,    30,  -182,  -182,  -182,  -182,  -182,  -182,    25,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,    22,  -182,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,  -182,   -44,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,  -182
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    11,    27,   131,   132,   200,   159,   179,
     133,   188,   220,   189,    34,    71,    30,    12,    13,    14,
      15,    46,    47,    67,    68,    69,    16,    17,    50,    51,
     134,   136,   135,    73,    18,    19,    63,    64,    83,    84,
      89,    90,    87,    88,    85,    86,   139,   137,   138,   198,
     190,   195,   192,   223,   194,   193,   196,   197,    91,    20,
      21,    66,   122,   123,   160,   161,   162,   167,   163,   164,
     165,   166,   168,   169,   170,   171,   172,   173,   175,    22,
      23,    24
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      28,   124,   125,   151,   152,   219,   178,   124,   125,     3,
      26,   126,   127,   128,   124,   125,   124,   125,    26,    79,
      80,    43,    44,    25,    77,    45,    38,   149,   150,    48,
      49,   153,   154,   155,   156,   124,   125,    92,    81,   236,
      52,    26,    29,    39,    40,    41,    31,    35,    53,    36,
      32,    37,    33,   182,   183,   217,    54,    55,    56,    57,
      42,    65,    58,    59,    60,    61,    70,    62,    74,    75,
      76,    82,   129,   174,   191,   180,    78,    72,   181,   177,
     130,   184,   185,   187,   158,   215,    93,   216,   231,     0,
     232,     0,   234,     0,   235,   186,   176,   157,   140,     0,
       0,     0,     0,   141,   142,     0,   143,   144,     0,   145,
     146,   147,     0,     0,     0,   212,   213,     0,     0,     0,
       0,   148,     0,     0,   218,     0,     0,     0,     0,     0,
     187,   221,     0,     0,   222,     0,   224,   225,   226,   227,
     228,   229,   230,     0,     0,     0,     0,     0,     0,     0,
     199,     0,     0,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   187,   237,   214,   238,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    -2,
       4,   239,     0,     0,     0,     0,     5,     6,     7,     8,
       9,     0,    10,   233,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   117,   118,   119,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   120,     0,     0,
     121,     0,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105
};

static const yytype_int16 yycheck[] =
{
       7,     3,     4,    85,    86,   186,   130,     3,     4,     0,
       3,    67,    68,    69,     3,     4,     3,     4,     3,    39,
      40,    35,    36,     5,    50,    39,    14,    83,    84,    26,
      27,    87,    88,    89,    90,     3,     4,    63,    58,   220,
      38,     3,     6,    31,    32,    33,    12,    18,    46,    20,
       5,    22,    84,   135,   136,   179,    54,    55,    56,    57,
       5,    53,    60,    61,    62,    63,    85,    65,    28,    29,
      30,    38,    81,     5,    43,   131,    51,    47,   134,   123,
      82,   137,   138,   139,    91,    81,    64,    83,    81,    -1,
      83,    -1,    81,    -1,    83,    82,   122,    82,    36,    -1,
      -1,    -1,    -1,    41,    42,    -1,    44,    45,    -1,    47,
      48,    49,    -1,    -1,    -1,   171,   172,    -1,    -1,    -1,
      -1,    59,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,
     186,   187,    -1,    -1,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   220,   221,   173,   223,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,     0,
       1,   237,    -1,    -1,    -1,    -1,     7,     8,     9,    10,
      11,    -1,    13,   200,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    50,    51,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    87,    88,     0,     1,     7,     8,     9,    10,    11,
      13,    89,   103,   104,   105,   106,   112,   113,   120,   121,
     145,   146,   165,   166,   167,     5,     3,    90,    90,     6,
     102,    12,     5,    84,   100,   100,   100,   100,    14,    31,
      32,    33,     5,    35,    36,    39,   107,   108,    26,    27,
     114,   115,    38,    46,    54,    55,    56,    57,    60,    61,
      62,    63,    65,   122,   123,    53,   147,   109,   110,   111,
      85,   101,   107,   119,    28,    29,    30,   101,   114,    39,
      40,    58,    38,   124,   125,   130,   131,   128,   129,   126,
     127,   144,   101,   122,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    50,    51,    52,
      64,    67,   148,   149,     3,     4,    91,    91,    91,    81,
      82,    91,    92,    96,   116,   118,   117,   133,   134,   132,
      36,    41,    42,    44,    45,    47,    48,    49,    59,    91,
      91,    96,    96,    91,    91,    91,    91,    82,    90,    94,
     150,   151,   152,   154,   155,   156,   157,   153,   158,   159,
     160,   161,   162,   163,     5,   164,   101,   148,    92,    95,
      91,    91,    96,    96,    91,    91,    82,    91,    97,    99,
     136,    43,   138,   141,   140,   137,   142,   143,   135,    90,
      93,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    91,    91,    90,    81,    83,    92,    91,    97,
      98,    91,    91,   139,    91,    91,    91,    91,    91,    91,
      91,    81,    83,    90,    81,    83,    97,    91,    91,    91
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    86,    87,    88,    88,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    90,    91,    91,    92,    93,
      93,    93,    93,    94,    94,    95,    95,    95,    95,    96,
      96,    97,    98,    98,    98,    98,    99,    99,   100,   101,
     101,   102,   103,   104,   105,   106,   107,   107,   109,   108,
     110,   108,   111,   108,   112,   113,   114,   114,   116,   115,
     117,   115,   118,   115,   119,   115,   120,   121,   122,   122,
     124,   123,   125,   123,   126,   123,   127,   123,   128,   123,
     129,   123,   130,   123,   131,   123,   132,   123,   133,   123,
     134,   123,   135,   123,   136,   123,   137,   123,   138,   123,
     139,   123,   140,   123,   141,   123,   142,   123,   143,   123,
     144,   123,   145,   146,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   148,   148,   149,
     150,   149,   151,   149,   152,   149,   153,   149,   154,   149,
     155,   149,   156,   149,   157,   149,   158,   149,   159,   149,
     160,   149,   161,   149,   162,   149,   163,   149,   164,   149,
     165,   166,   167
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
       0,     3,     0,     3,     0,     3,     0,     3,     0,     2,
       3,     5,     3
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
#line 150 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                        {YYABORT;}
#line 1509 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 15: /* SFInt32: INTEGER  */
#line 161 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
			if(Ibuff) Ibuff->input((yyvsp[0].ival));
		}
#line 1517 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 16: /* SFFloat: FLOATING  */
#line 173 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
			if(Fbuff) Fbuff->input((yyvsp[0].fval));
		}
#line 1525 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 17: /* SFFloat: INTEGER  */
#line 177 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
			(yyval.fval) = (double)(yyvsp[0].ival);
			if(Fbuff) Fbuff->input((double)(yyvsp[0].ival));
		}
#line 1534 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 18: /* SFVec3f: SFFloat SFFloat SFFloat  */
#line 190 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                  {}
#line 1540 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 23: /* MFInt32: SFInt32  */
#line 202 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                  {}
#line 1546 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 24: /* MFInt32: '[' SFInt32List ']'  */
#line 203 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                              {}
#line 1552 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 31: /* SFVec4f: SFFloat SFFloat SFFloat SFFloat  */
#line 246 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                          {}
#line 1558 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 41: /* Name: NAME  */
#line 284 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
             {if (strlen((yyvsp[0].sval)) > sizeof(DB::str_t)-1) YYERROR; else strcpy(name,(yyvsp[0].sval));}
#line 1564 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 42: /* SetCircuitType: ASSEMBLYTYPE SFInt32  */
#line 290 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
			loadingEmP->assemblyType=(yyvsp[0].ival);
		}
#line 1572 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 43: /* Level: LEVEL SFInt32  */
#line 297 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
			loadingEmP->level=(yyvsp[0].ival);
		}
#line 1580 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 44: /* DefaultBoundCond: DEFAULTBC STRING  */
#line 305 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
	             strcpy(loadingEmP->defaultBC,(yyvsp[0].sval));
		}
#line 1588 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 45: /* Import: IMPORT STRING  */
#line 316 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  loadingEmP->stepFilePath=(yyvsp[0].sval);
                  if(loadingOcaf) 
                     step_file_reloaded=loadingOcaf->importSTEP_OR_DXC((yyvsp[0].sval), reload_step_file);
		}
#line 1598 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 48: /* $@1: %empty  */
#line 332 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1604 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 49: /* UnitsElement: LENGTH $@1 SFFloat  */
#line 332 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                           {unit.xm   =(yyvsp[0].fval);  }
#line 1610 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 50: /* $@2: %empty  */
#line 333 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1616 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 51: /* UnitsElement: FREQUENCY $@2 SFFloat  */
#line 333 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                           {unit.xHz  =(yyvsp[0].fval);  }
#line 1622 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 52: /* $@3: %empty  */
#line 334 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1628 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 53: /* UnitsElement: RESISTANCE $@3 SFFloat  */
#line 334 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                           {unit.xOhm =(yyvsp[0].fval);  }
#line 1634 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 58: /* $@4: %empty  */
#line 355 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1640 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 59: /* InvariantElement: ROTATION ANGLE $@4 SFFloat  */
#line 355 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                    {invt->rotAngle=(yyvsp[0].fval); }
#line 1646 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 60: /* $@5: %empty  */
#line 356 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                          {Fbuff = &DoubleBuffer; Fbuff->reset();}
#line 1652 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 61: /* InvariantElement: ROTATION AXIS $@5 MFVec3f  */
#line 357 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
            {
              DoubleBuffer.flush(dVec); 
              for(int i=0; i<3; i++) invt->rotAxis[i]=dVec[i];
            }
#line 1661 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 62: /* $@6: %empty  */
#line 361 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                            {Fbuff = &DoubleBuffer; Fbuff->reset();}
#line 1667 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 63: /* InvariantElement: ROTATION ORIGIN $@6 MFVec3f  */
#line 362 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
            {
              DoubleBuffer.flush(dVec); 
              for(int i=0; i<3; i++) invt->rotOrigin[i]=dVec[i];
            }
#line 1676 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 64: /* $@7: %empty  */
#line 366 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                          {Fbuff = &DoubleBuffer; Fbuff->reset();}
#line 1682 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 65: /* InvariantElement: TRANSLATION $@7 MFVec3f  */
#line 367 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
            {
              DoubleBuffer.flush(dVec); 
              for(int i=0; i<3; i++) invt->trasl[i]=dVec[i];
            }
#line 1691 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 66: /* InvariantBegin: DEF Name MWM_INVARIANT  */
#line 375 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
         {     
              invt =loadingEmP->FindInvariant(name);
              if(invt) loadingEmP->delInvariant(invt);
	      invt = new DB::Transform();
	      strcpy(invt->name,name);
	      loadingEmP->addInvariant(invt);
	 }
#line 1703 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 70: /* $@8: %empty  */
#line 397 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1709 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 71: /* MaterialElement: EPSILONR $@8 SFFloat  */
#line 397 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->epsr=(yyvsp[0].fval);     }
#line 1715 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 72: /* $@9: %empty  */
#line 398 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1721 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 73: /* MaterialElement: MUR $@9 SFFloat  */
#line 398 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->mur=(yyvsp[0].fval);          }
#line 1727 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 74: /* $@10: %empty  */
#line 399 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1733 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 75: /* MaterialElement: ETANDELTA $@10 SFFloat  */
#line 399 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->etandelta=(yyvsp[0].fval);}
#line 1739 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 76: /* $@11: %empty  */
#line 400 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1745 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 77: /* MaterialElement: HTANDELTA $@11 SFFloat  */
#line 400 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->htandelta=(yyvsp[0].fval);}
#line 1751 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 78: /* $@12: %empty  */
#line 401 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1757 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 79: /* MaterialElement: ECONDUCTIVITY $@12 SFFloat  */
#line 401 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->econductivity=(yyvsp[0].fval);}
#line 1763 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 80: /* $@13: %empty  */
#line 402 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1769 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 81: /* MaterialElement: HCONDUCTIVITY $@13 SFFloat  */
#line 402 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->hconductivity=(yyvsp[0].fval);}
#line 1775 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 82: /* $@14: %empty  */
#line 403 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff  =&mat->buff.epsLorentz; Fbuff->reset();}
#line 1781 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 84: /* $@15: %empty  */
#line 404 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff  =&mat->buff.muLorentz; Fbuff->reset();}
#line 1787 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 86: /* $@16: %empty  */
#line 405 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff  =&mat->buff.surfPolesRes; Fbuff->reset();}
#line 1793 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 88: /* $@17: %empty  */
#line 406 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1799 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 89: /* MaterialElement: SURFACE RESISTANCE $@17 SFFloat  */
#line 406 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->Sresistance=(yyvsp[0].fval); }
#line 1805 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 90: /* $@18: %empty  */
#line 407 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1811 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 91: /* MaterialElement: SURFACE INDUCTANCE $@18 SFFloat  */
#line 407 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->Sinductance=(yyvsp[0].fval); }
#line 1817 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 92: /* $@19: %empty  */
#line 408 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1823 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 93: /* MaterialElement: ROUGH SURFACE POLESNUM $@19 SFFloat  */
#line 408 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfFitPolesNum=(yyvsp[0].fval); }
#line 1829 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 94: /* $@20: %empty  */
#line 409 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1835 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 95: /* MaterialElement: ROUGH SURFACE FREQUENCY $@20 SFFloat  */
#line 409 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfFreq=(yyvsp[0].fval); }
#line 1841 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 96: /* $@21: %empty  */
#line 410 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1847 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 97: /* MaterialElement: ROUGH SURFACE MODELTYPE $@21 SFFloat  */
#line 410 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfModelType=(yyvsp[0].fval); }
#line 1853 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 98: /* $@22: %empty  */
#line 411 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1859 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 99: /* MaterialElement: ROUGH SURFACE LOSSFACTOR $@22 SFFloat  */
#line 411 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfLossFactor=(yyvsp[0].fval);}
#line 1865 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 100: /* $@23: %empty  */
#line 412 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                           {Fbuff = NULL;}
#line 1871 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 101: /* MaterialElement: ROUGH SURFACE IMPEDANCE QFACTOR $@23 SFFloat  */
#line 412 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfImpedanceQ=(yyvsp[0].fval);}
#line 1877 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 102: /* $@24: %empty  */
#line 413 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1883 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 103: /* MaterialElement: ROUGH SURFACE BALLRADIUS $@24 SFFloat  */
#line 413 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfBallRadius=(yyvsp[0].fval);}
#line 1889 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 104: /* $@25: %empty  */
#line 414 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1895 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 105: /* MaterialElement: ROUGH SURFACE SURFRATIO $@25 SFFloat  */
#line 414 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfSurfRatio=(yyvsp[0].fval);}
#line 1901 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 106: /* $@26: %empty  */
#line 415 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1907 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 107: /* MaterialElement: ROUGH SURFACE RZ $@26 SFFloat  */
#line 415 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfRz=(yyvsp[0].fval);}
#line 1913 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 108: /* $@27: %empty  */
#line 416 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1919 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 109: /* MaterialElement: ROUGH SURFACE RQ $@27 SFFloat  */
#line 416 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                                           {mat->roughSurfRq=(yyvsp[0].fval);}
#line 1925 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 110: /* $@28: %empty  */
#line 417 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                    { Ibuff = &IntBuffer; Ibuff->reset();}
#line 1931 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 111: /* MaterialElement: COLOR $@28 MFInt32  */
#line 418 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
            {
              IntBuffer.flush(matColor); 
              for(int i=0; i<4; i++) mat->color[i]=matColor[i];
            }
#line 1940 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 112: /* MaterialBegin: DEF Name MWM_MATERIAL  */
#line 426 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {     
                        mat =loadingEmP->FindMaterial(name);
                        if(mat) loadingEmP->delMaterial(mat);
			mat = new DB::Material();
			strcpy(mat->name,name);
			loadingEmP->addMaterial(mat);
		}
#line 1952 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 113: /* Material: MaterialBegin NodeBegin MaterialElements NodeEnd  */
#line 437 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
          { 
             mat->flush(); 
          }
#line 1960 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 114: /* VolumeType: VOLTYPE tDIELECTRIC  */
#line 448 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=DIELECTRIC;
                }
#line 1968 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 115: /* VolumeType: VOLTYPE tHOLE  */
#line 452 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=HOLE;
                }
#line 1976 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 116: /* VolumeType: VOLTYPE tBOUNDARYCONDITION  */
#line 456 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=BOUNDARYCOND;
                }
#line 1984 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 117: /* VolumeType: VOLTYPE tWAVEGUIDE  */
#line 460 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=WAVEGUIDE;
                }
#line 1992 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 118: /* VolumeType: VOLTYPE tLINEPORT  */
#line 464 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=LINEPORT;
                }
#line 2000 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 119: /* VolumeType: VOLTYPE tSPLITTER  */
#line 468 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=SPLITTER;
                }
#line 2008 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 120: /* VolumeType: VOLTYPE tGRID  */
#line 472 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=GRID;
                }
#line 2016 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 121: /* VolumeType: VOLTYPE tNET  */
#line 476 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=NET;
                }
#line 2024 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 122: /* VolumeType: VOLTYPE tCOMPONENT  */
#line 480 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=COMPONENT;
                }
#line 2032 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 123: /* VolumeType: VOLTYPE tINTERFACE  */
#line 484 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=INTERFACE;
                }
#line 2040 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 124: /* VolumeType: VOLTYPE tASSEMBLY  */
#line 488 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=ASSEMBLY;
                }
#line 2048 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 125: /* VolumeType: VOLTYPE tUNDEFINED  */
#line 492 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
		   vol->type=UNDEFINED;
                }
#line 2056 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 129: /* VolumeElement: MATERIAL STRING  */
#line 506 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                   if (strlen((yyvsp[0].sval)) > sizeof(DB::str_t)-1) YYERROR;
                   if(!loadingEmP->FindMaterial((yyvsp[0].sval)))  DB::yyMsg(FATAL, "Undefined material %s\n", (yyvsp[0].sval));
		   strcpy(vol->material,(yyvsp[0].sval));
                }
#line 2066 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 130: /* $@29: %empty  */
#line 511 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2072 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 131: /* VolumeElement: TEMPORTSNUM $@29 SFInt32  */
#line 512 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->TEMportsNum =(yyvsp[0].ival);
                }
#line 2080 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 132: /* $@30: %empty  */
#line 515 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2086 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 133: /* VolumeElement: TEPORTSNUM $@30 SFInt32  */
#line 516 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->TEportsNum =(yyvsp[0].ival);
                }
#line 2094 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 134: /* $@31: %empty  */
#line 519 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2100 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 135: /* VolumeElement: TMPORTSNUM $@31 SFInt32  */
#line 520 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->TMportsNum =(yyvsp[0].ival);
                }
#line 2108 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 136: /* $@32: %empty  */
#line 523 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2114 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 137: /* VolumeElement: ORIENTATION $@32 SFInt32  */
#line 524 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->orientation=(yyvsp[0].ival);
                }
#line 2122 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 138: /* $@33: %empty  */
#line 527 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                {Ibuff  =NULL;}
#line 2128 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 139: /* VolumeElement: TEMTABULARORDER1 $@33 SFInt32  */
#line 528 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->TEM_TabularOrder1=(yyvsp[0].ival);
                }
#line 2136 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 140: /* $@34: %empty  */
#line 531 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                {Ibuff  =NULL;}
#line 2142 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 141: /* VolumeElement: TEMTABULARORDER2 $@34 SFInt32  */
#line 532 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->TEM_TabularOrder2=(yyvsp[0].ival);
                }
#line 2150 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 142: /* $@35: %empty  */
#line 535 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                {Ibuff  =NULL;}
#line 2156 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 143: /* VolumeElement: TEMREFCONDUCTOR $@35 SFInt32  */
#line 536 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->TEM_refCond=(yyvsp[0].ival);
                }
#line 2164 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 144: /* $@36: %empty  */
#line 539 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                {Ibuff  =NULL;}
#line 2170 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 145: /* VolumeElement: DISCONNECTEDTEM $@36 SFInt32  */
#line 540 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->disconnectedTEM =(yyvsp[0].ival);
                }
#line 2178 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 146: /* $@37: %empty  */
#line 543 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2184 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 147: /* VolumeElement: GRIDNUM $@37 SFInt32  */
#line 544 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->gridNum =(yyvsp[0].ival);
                }
#line 2192 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 148: /* $@38: %empty  */
#line 547 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {Ibuff  =NULL;}
#line 2198 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 149: /* VolumeElement: PML $@38 SFInt32  */
#line 548 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->PML =(yyvsp[0].ival);
                }
#line 2206 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 150: /* $@39: %empty  */
#line 551 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                      {Ibuff  =NULL;}
#line 2212 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 151: /* VolumeElement: INVARIANT $@39 SFInt32  */
#line 552 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
                  vol->invariant =(yyvsp[0].ival);
                }
#line 2220 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 152: /* $@40: %empty  */
#line 555 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 2226 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 153: /* VolumeElement: MESHREFINEMENT $@40 SFFloat  */
#line 555 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                             {vol->meshRefinement=(yyvsp[0].fval); }
#line 2232 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 154: /* $@41: %empty  */
#line 556 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 2238 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 155: /* VolumeElement: CUTOFFREFINEMENT $@41 SFFloat  */
#line 556 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                             {vol->cutoffRefinement=(yyvsp[0].fval); }
#line 2244 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 156: /* $@42: %empty  */
#line 558 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             {Ibuff = NULL;}
#line 2250 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 157: /* VolumeElement: COMPSOLID $@42 SFInt32  */
#line 558 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                             { }
#line 2256 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 158: /* $@43: %empty  */
#line 560 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                             { }
#line 2262 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 159: /* VolumeElement: DISABLED $@43  */
#line 560 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                                                             {vol->disabled=1; }
#line 2268 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 160: /* VolumeBegin: DEF Name MWM_VOLUME  */
#line 564 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
                {
			vol = new DB::Volume();
			strcpy(vol->name,name);
			loadingEmP->insertVolume(vol);
		}
#line 2278 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;

  case 162: /* CircuitName: SET CIRCUITNAME STRING  */
#line 582 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"
            {
             if (strlen((yyvsp[0].sval))>sizeof(DB::str_t)-1) YYERROR; 
             else strcpy(mainCircName,(yyvsp[0].sval));
            }
#line 2287 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"
    break;


#line 2291 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model_parser.cpp"

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

#line 588 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/DataBase/model.y"



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


