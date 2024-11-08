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
#line 22 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"

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


#line 155 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"

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
  YYSYMBOL_TEMTABULARORDER = 18,           /* TEMTABULARORDER  */
  YYSYMBOL_GRIDNUM = 19,                   /* GRIDNUM  */
  YYSYMBOL_PML = 20,                       /* PML  */
  YYSYMBOL_INVARIANT = 21,                 /* INVARIANT  */
  YYSYMBOL_TRANSLATION = 22,               /* TRANSLATION  */
  YYSYMBOL_ROTATION = 23,                  /* ROTATION  */
  YYSYMBOL_ANGLE = 24,                     /* ANGLE  */
  YYSYMBOL_ORIGIN = 25,                    /* ORIGIN  */
  YYSYMBOL_AXIS = 26,                      /* AXIS  */
  YYSYMBOL_MWM_VOLUME = 27,                /* MWM_VOLUME  */
  YYSYMBOL_MWM_INVARIANT = 28,             /* MWM_INVARIANT  */
  YYSYMBOL_MWM_UNITS = 29,                 /* MWM_UNITS  */
  YYSYMBOL_MWM_LINEPORT = 30,              /* MWM_LINEPORT  */
  YYSYMBOL_LENGTH = 31,                    /* LENGTH  */
  YYSYMBOL_FREQUENCY = 32,                 /* FREQUENCY  */
  YYSYMBOL_BAND = 33,                      /* BAND  */
  YYSYMBOL_SURFACE = 34,                   /* SURFACE  */
  YYSYMBOL_RESISTANCE = 35,                /* RESISTANCE  */
  YYSYMBOL_INDUCTANCE = 36,                /* INDUCTANCE  */
  YYSYMBOL_IMPEDANCE = 37,                 /* IMPEDANCE  */
  YYSYMBOL_LOSSFACTOR = 38,                /* LOSSFACTOR  */
  YYSYMBOL_QFACTOR = 39,                   /* QFACTOR  */
  YYSYMBOL_SURFRATIO = 40,                 /* SURFRATIO  */
  YYSYMBOL_BALLRADIUS = 41,                /* BALLRADIUS  */
  YYSYMBOL_ROUGH = 42,                     /* ROUGH  */
  YYSYMBOL_MODELTYPE = 43,                 /* MODELTYPE  */
  YYSYMBOL_RZ = 44,                        /* RZ  */
  YYSYMBOL_RQ = 45,                        /* RQ  */
  YYSYMBOL_MESHREFINEMENT = 46,            /* MESHREFINEMENT  */
  YYSYMBOL_CUTOFFREFINEMENT = 47,          /* CUTOFFREFINEMENT  */
  YYSYMBOL_COMPSOLID = 48,                 /* COMPSOLID  */
  YYSYMBOL_VOLTYPE = 49,                   /* VOLTYPE  */
  YYSYMBOL_EPSILONR = 50,                  /* EPSILONR  */
  YYSYMBOL_MUR = 51,                       /* MUR  */
  YYSYMBOL_EPSLORENTZ = 52,                /* EPSLORENTZ  */
  YYSYMBOL_MULORENTZ = 53,                 /* MULORENTZ  */
  YYSYMBOL_POLESRESIDUES = 54,             /* POLESRESIDUES  */
  YYSYMBOL_POLESNUM = 55,                  /* POLESNUM  */
  YYSYMBOL_ECONDUCTIVITY = 56,             /* ECONDUCTIVITY  */
  YYSYMBOL_HCONDUCTIVITY = 57,             /* HCONDUCTIVITY  */
  YYSYMBOL_ETANDELTA = 58,                 /* ETANDELTA  */
  YYSYMBOL_HTANDELTA = 59,                 /* HTANDELTA  */
  YYSYMBOL_MATERIAL = 60,                  /* MATERIAL  */
  YYSYMBOL_COLOR = 61,                     /* COLOR  */
  YYSYMBOL_VOLUMES = 62,                   /* VOLUMES  */
  YYSYMBOL_DISABLED = 63,                  /* DISABLED  */
  YYSYMBOL_tDIELECTRIC = 64,               /* tDIELECTRIC  */
  YYSYMBOL_tHOLE = 65,                     /* tHOLE  */
  YYSYMBOL_tBOUNDARYCONDITION = 66,        /* tBOUNDARYCONDITION  */
  YYSYMBOL_tWAVEGUIDE = 67,                /* tWAVEGUIDE  */
  YYSYMBOL_tLINEPORT = 68,                 /* tLINEPORT  */
  YYSYMBOL_tSPLITTER = 69,                 /* tSPLITTER  */
  YYSYMBOL_tGRID = 70,                     /* tGRID  */
  YYSYMBOL_tCOMPONENT = 71,                /* tCOMPONENT  */
  YYSYMBOL_tINTERFACE = 72,                /* tINTERFACE  */
  YYSYMBOL_tNET = 73,                      /* tNET  */
  YYSYMBOL_tASSEMBLY = 74,                 /* tASSEMBLY  */
  YYSYMBOL_tUNDEFINED = 75,                /* tUNDEFINED  */
  YYSYMBOL_SOLID = 76,                     /* SOLID  */
  YYSYMBOL_77_ = 77,                       /* ','  */
  YYSYMBOL_78_ = 78,                       /* '['  */
  YYSYMBOL_79_ = 79,                       /* ']'  */
  YYSYMBOL_80_ = 80,                       /* '{'  */
  YYSYMBOL_81_ = 81,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 82,                  /* $accept  */
  YYSYMBOL_All = 83,                       /* All  */
  YYSYMBOL_MwmItems = 84,                  /* MwmItems  */
  YYSYMBOL_MwmItem = 85,                   /* MwmItem  */
  YYSYMBOL_SFInt32 = 86,                   /* SFInt32  */
  YYSYMBOL_SFFloat = 87,                   /* SFFloat  */
  YYSYMBOL_SFVec3f = 88,                   /* SFVec3f  */
  YYSYMBOL_SFInt32List = 89,               /* SFInt32List  */
  YYSYMBOL_MFInt32 = 90,                   /* MFInt32  */
  YYSYMBOL_SFVec3fList = 91,               /* SFVec3fList  */
  YYSYMBOL_MFVec3f = 92,                   /* MFVec3f  */
  YYSYMBOL_SFVec4f = 93,                   /* SFVec4f  */
  YYSYMBOL_SFVec4fList = 94,               /* SFVec4fList  */
  YYSYMBOL_MFVec4f = 95,                   /* MFVec4f  */
  YYSYMBOL_NodeBegin = 96,                 /* NodeBegin  */
  YYSYMBOL_NodeEnd = 97,                   /* NodeEnd  */
  YYSYMBOL_Name = 98,                      /* Name  */
  YYSYMBOL_SetCircuitType = 99,            /* SetCircuitType  */
  YYSYMBOL_Level = 100,                    /* Level  */
  YYSYMBOL_DefaultBoundCond = 101,         /* DefaultBoundCond  */
  YYSYMBOL_Import = 102,                   /* Import  */
  YYSYMBOL_UnitsElements = 103,            /* UnitsElements  */
  YYSYMBOL_UnitsElement = 104,             /* UnitsElement  */
  YYSYMBOL_105_1 = 105,                    /* $@1  */
  YYSYMBOL_106_2 = 106,                    /* $@2  */
  YYSYMBOL_107_3 = 107,                    /* $@3  */
  YYSYMBOL_UnitsBegin = 108,               /* UnitsBegin  */
  YYSYMBOL_Units = 109,                    /* Units  */
  YYSYMBOL_InvariantElements = 110,        /* InvariantElements  */
  YYSYMBOL_InvariantElement = 111,         /* InvariantElement  */
  YYSYMBOL_112_4 = 112,                    /* $@4  */
  YYSYMBOL_113_5 = 113,                    /* $@5  */
  YYSYMBOL_114_6 = 114,                    /* $@6  */
  YYSYMBOL_115_7 = 115,                    /* $@7  */
  YYSYMBOL_InvariantBegin = 116,           /* InvariantBegin  */
  YYSYMBOL_Invariant = 117,                /* Invariant  */
  YYSYMBOL_MaterialElements = 118,         /* MaterialElements  */
  YYSYMBOL_MaterialElement = 119,          /* MaterialElement  */
  YYSYMBOL_120_8 = 120,                    /* $@8  */
  YYSYMBOL_121_9 = 121,                    /* $@9  */
  YYSYMBOL_122_10 = 122,                   /* $@10  */
  YYSYMBOL_123_11 = 123,                   /* $@11  */
  YYSYMBOL_124_12 = 124,                   /* $@12  */
  YYSYMBOL_125_13 = 125,                   /* $@13  */
  YYSYMBOL_126_14 = 126,                   /* $@14  */
  YYSYMBOL_127_15 = 127,                   /* $@15  */
  YYSYMBOL_128_16 = 128,                   /* $@16  */
  YYSYMBOL_129_17 = 129,                   /* $@17  */
  YYSYMBOL_130_18 = 130,                   /* $@18  */
  YYSYMBOL_131_19 = 131,                   /* $@19  */
  YYSYMBOL_132_20 = 132,                   /* $@20  */
  YYSYMBOL_133_21 = 133,                   /* $@21  */
  YYSYMBOL_134_22 = 134,                   /* $@22  */
  YYSYMBOL_135_23 = 135,                   /* $@23  */
  YYSYMBOL_136_24 = 136,                   /* $@24  */
  YYSYMBOL_137_25 = 137,                   /* $@25  */
  YYSYMBOL_138_26 = 138,                   /* $@26  */
  YYSYMBOL_139_27 = 139,                   /* $@27  */
  YYSYMBOL_140_28 = 140,                   /* $@28  */
  YYSYMBOL_MaterialBegin = 141,            /* MaterialBegin  */
  YYSYMBOL_Material = 142,                 /* Material  */
  YYSYMBOL_VolumeType = 143,               /* VolumeType  */
  YYSYMBOL_VolumeElements = 144,           /* VolumeElements  */
  YYSYMBOL_VolumeElement = 145,            /* VolumeElement  */
  YYSYMBOL_146_29 = 146,                   /* $@29  */
  YYSYMBOL_147_30 = 147,                   /* $@30  */
  YYSYMBOL_148_31 = 148,                   /* $@31  */
  YYSYMBOL_149_32 = 149,                   /* $@32  */
  YYSYMBOL_150_33 = 150,                   /* $@33  */
  YYSYMBOL_151_34 = 151,                   /* $@34  */
  YYSYMBOL_152_35 = 152,                   /* $@35  */
  YYSYMBOL_153_36 = 153,                   /* $@36  */
  YYSYMBOL_154_37 = 154,                   /* $@37  */
  YYSYMBOL_155_38 = 155,                   /* $@38  */
  YYSYMBOL_156_39 = 156,                   /* $@39  */
  YYSYMBOL_VolumeBegin = 157,              /* VolumeBegin  */
  YYSYMBOL_Volume = 158,                   /* Volume  */
  YYSYMBOL_CircuitName = 159               /* CircuitName  */
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
#define YYLAST   198

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  82
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  78
/* YYNRULES -- Number of rules.  */
#define YYNRULES  154
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  228

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   331


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
       2,     2,     2,     2,    77,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    78,     2,    79,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    80,     2,    81,     2,     2,     2,     2,
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
      75,    76
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   130,   130,   134,   135,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   159,   171,   175,   189,   194,
     195,   196,   197,   201,   202,   232,   233,   234,   235,   239,
     240,   245,   249,   250,   251,   252,   256,   257,   273,   277,
     278,   283,   288,   295,   303,   314,   326,   327,   331,   331,
     332,   332,   333,   333,   337,   341,   349,   350,   354,   354,
     355,   355,   360,   360,   365,   365,   373,   383,   391,   392,
     396,   396,   397,   397,   398,   398,   399,   399,   400,   400,
     401,   401,   402,   402,   403,   403,   404,   404,   405,   405,
     406,   406,   407,   407,   408,   408,   409,   409,   410,   410,
     411,   411,   412,   412,   413,   413,   414,   414,   415,   415,
     416,   416,   424,   435,   446,   450,   454,   458,   462,   466,
     470,   474,   478,   482,   486,   490,   494,   499,   500,   504,
     510,   510,   514,   514,   518,   518,   522,   522,   526,   526,
     530,   530,   534,   534,   538,   538,   539,   539,   541,   541,
     543,   543,   546,   555,   564
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
  "TMPORTSNUM", "TEMTABULARORDER", "GRIDNUM", "PML", "INVARIANT",
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
  "$@36", "$@37", "$@38", "$@39", "VolumeBegin", "Volume", "CircuitName", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-167)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-3)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -167,    20,   173,  -167,  -167,    34,    41,    41,    54,    53,
      57,  -167,  -167,  -167,  -167,  -167,   -17,  -167,   -17,  -167,
     -17,  -167,   -17,  -167,  -167,  -167,  -167,  -167,  -167,  -167,
      21,    61,  -167,  -167,     5,    24,    75,    30,  -167,  -167,
    -167,  -167,  -167,  -167,  -167,  -167,   -11,     5,  -167,    51,
     -11,    24,    -2,    38,  -167,  -167,  -167,  -167,  -167,  -167,
    -167,  -167,  -167,   -11,    75,   123,    11,    50,    50,    50,
      -4,  -167,  -167,    10,  -167,  -167,  -167,  -167,  -167,  -167,
    -167,  -167,   124,    50,    50,    10,    10,    50,    50,    50,
      50,    16,  -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,
    -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,
    -167,  -167,  -167,  -167,  -167,  -167,    77,  -167,   -11,    11,
    -167,  -167,  -167,  -167,  -167,  -167,    50,    50,  -167,  -167,
      50,    10,    10,    50,    50,    12,  -167,    47,  -167,  -167,
    -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,
    -167,  -167,  -167,    41,  -167,  -167,    41,    41,    41,    41,
      41,    41,    41,    50,    50,    41,  -167,  -167,  -167,  -167,
    -167,     1,    50,  -167,  -167,  -167,  -167,  -167,    50,    50,
    -167,  -167,    50,  -167,    50,    50,    50,    50,    50,    50,
      50,  -167,     8,  -167,  -167,  -167,  -167,  -167,  -167,  -167,
    -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,     4,    50,
    -167,    50,  -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,
    -167,  -167,  -167,  -167,  -167,    50,  -167,  -167
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,    14,     0,     0,     0,     0,     0,
       0,     4,     6,     8,     9,    10,     0,     5,     0,     7,
       0,    11,     0,    12,    13,    45,    15,    42,    43,    41,
       0,     0,    44,    38,    47,    57,    69,   126,   112,   152,
      66,    54,   154,    48,    50,    52,     0,    47,    64,     0,
       0,    57,     0,     0,    70,    72,    82,    84,    78,    80,
      74,    76,   110,     0,    69,     0,   128,     0,     0,     0,
      39,    55,    46,     0,    58,    62,    60,    67,    56,    88,
      90,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,    68,   114,   115,   116,   117,   118,   119,
     120,   122,   123,   121,   124,   125,   130,   132,   134,   136,
     138,   140,   142,   144,   146,   148,     0,   150,     0,   128,
      17,    16,    49,    51,    53,    40,    28,     0,    29,    65,
       0,     0,     0,     0,     0,     0,    94,     0,    98,   104,
     102,    96,   106,   108,    92,    71,    73,    83,    85,    79,
      81,    75,    77,    22,    23,   111,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   129,   151,   153,   127,
      25,     0,     0,    59,    63,    61,    89,    91,    35,     0,
      36,    87,     0,   100,     0,     0,     0,     0,     0,     0,
       0,    19,     0,   131,   133,   135,   137,   139,   141,   143,
     145,   147,   149,    27,    30,    26,    18,    32,     0,     0,
      95,     0,    99,   105,   103,    97,   107,   109,    93,    21,
      24,    20,    34,    37,    33,     0,   101,    31
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -167,  -167,  -167,  -167,    -7,   -66,  -120,  -167,  -167,  -167,
     -76,  -166,  -167,  -167,    23,   -25,  -167,  -167,  -167,  -167,
    -167,    42,  -167,  -167,  -167,  -167,  -167,  -167,    40,  -167,
    -167,  -167,  -167,  -167,  -167,  -167,    28,  -167,  -167,  -167,
    -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,
    -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,
    -167,  -167,   -24,  -167,  -167,  -167,  -167,  -167,  -167,  -167,
    -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    11,    27,   127,   128,   192,   155,   171,
     129,   180,   208,   181,    34,    71,    30,    12,    13,    14,
      15,    46,    47,    67,    68,    69,    16,    17,    50,    51,
     130,   132,   131,    73,    18,    19,    63,    64,    83,    84,
      89,    90,    87,    88,    85,    86,   135,   133,   134,   190,
     182,   187,   184,   211,   186,   185,   188,   189,    91,    20,
      21,    66,   118,   119,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   167,    22,    23,    24
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      28,   122,   123,   124,   120,   121,   170,   120,   121,   147,
     148,    26,   207,   120,   121,   120,   121,   145,   146,    26,
       3,   149,   150,   151,   152,    77,   106,   107,   108,   109,
     110,   111,   112,    79,    80,    38,    43,    44,    92,    25,
      45,    35,   224,    36,    26,    37,    48,    49,    39,    40,
      41,   205,    81,   120,   121,   174,   175,   113,   114,   115,
      29,   172,    32,    33,   173,    31,    42,   176,   177,   179,
      70,   116,    82,   125,   117,    74,    75,    76,   203,    65,
     204,   222,   166,   223,   154,   219,   183,   220,   126,    72,
     178,    78,    93,   168,   153,   169,     0,   200,   201,     0,
       0,     0,     0,     0,     0,     0,   206,     0,     0,    52,
       0,     0,   179,   209,     0,     0,   210,    53,   212,   213,
     214,   215,   216,   217,   218,    54,    55,    56,    57,     0,
       0,    58,    59,    60,    61,     0,    62,     0,     0,     0,
       0,     0,   179,   225,     0,   226,   191,     0,     0,   193,
     194,   195,   196,   197,   198,   199,   136,     0,   202,   227,
       0,   137,   138,     0,   139,   140,     0,   141,   142,   143,
       0,     0,     0,    -2,     4,     0,     0,     0,     0,   144,
       5,     6,     7,     8,     9,   221,    10,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105
};

static const yytype_int16 yycheck[] =
{
       7,    67,    68,    69,     3,     4,   126,     3,     4,    85,
      86,     3,   178,     3,     4,     3,     4,    83,    84,     3,
       0,    87,    88,    89,    90,    50,    15,    16,    17,    18,
      19,    20,    21,    35,    36,    14,    31,    32,    63,     5,
      35,    18,   208,    20,     3,    22,    22,    23,    27,    28,
      29,   171,    54,     3,     4,   131,   132,    46,    47,    48,
       6,   127,     5,    80,   130,    12,     5,   133,   134,   135,
      81,    60,    34,    77,    63,    24,    25,    26,    77,    49,
      79,    77,     5,    79,    91,    77,    39,    79,    78,    47,
      78,    51,    64,   118,    78,   119,    -1,   163,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    34,
      -1,    -1,   178,   179,    -1,    -1,   182,    42,   184,   185,
     186,   187,   188,   189,   190,    50,    51,    52,    53,    -1,
      -1,    56,    57,    58,    59,    -1,    61,    -1,    -1,    -1,
      -1,    -1,   208,   209,    -1,   211,   153,    -1,    -1,   156,
     157,   158,   159,   160,   161,   162,    32,    -1,   165,   225,
      -1,    37,    38,    -1,    40,    41,    -1,    43,    44,    45,
      -1,    -1,    -1,     0,     1,    -1,    -1,    -1,    -1,    55,
       7,     8,     9,    10,    11,   192,    13,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    83,    84,     0,     1,     7,     8,     9,    10,    11,
      13,    85,    99,   100,   101,   102,   108,   109,   116,   117,
     141,   142,   157,   158,   159,     5,     3,    86,    86,     6,
      98,    12,     5,    80,    96,    96,    96,    96,    14,    27,
      28,    29,     5,    31,    32,    35,   103,   104,    22,    23,
     110,   111,    34,    42,    50,    51,    52,    53,    56,    57,
      58,    59,    61,   118,   119,    49,   143,   105,   106,   107,
      81,    97,   103,   115,    24,    25,    26,    97,   110,    35,
      36,    54,    34,   120,   121,   126,   127,   124,   125,   122,
     123,   140,    97,   118,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    15,    16,    17,    18,
      19,    20,    21,    46,    47,    48,    60,    63,   144,   145,
       3,     4,    87,    87,    87,    77,    78,    87,    88,    92,
     112,   114,   113,   129,   130,   128,    32,    37,    38,    40,
      41,    43,    44,    45,    55,    87,    87,    92,    92,    87,
      87,    87,    87,    78,    86,    90,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,     5,   156,    97,   144,
      88,    91,    87,    87,    92,    92,    87,    87,    78,    87,
      93,    95,   132,    39,   134,   137,   136,   133,   138,   139,
     131,    86,    89,    86,    86,    86,    86,    86,    86,    86,
      87,    87,    86,    77,    79,    88,    87,    93,    94,    87,
      87,   135,    87,    87,    87,    87,    87,    87,    87,    77,
      79,    86,    77,    79,    93,    87,    87,    87
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    82,    83,    84,    84,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    86,    87,    87,    88,    89,
      89,    89,    89,    90,    90,    91,    91,    91,    91,    92,
      92,    93,    94,    94,    94,    94,    95,    95,    96,    97,
      97,    98,    99,   100,   101,   102,   103,   103,   105,   104,
     106,   104,   107,   104,   108,   109,   110,   110,   112,   111,
     113,   111,   114,   111,   115,   111,   116,   117,   118,   118,
     120,   119,   121,   119,   122,   119,   123,   119,   124,   119,
     125,   119,   126,   119,   127,   119,   128,   119,   129,   119,
     130,   119,   131,   119,   132,   119,   133,   119,   134,   119,
     135,   119,   136,   119,   137,   119,   138,   119,   139,   119,
     140,   119,   141,   142,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   144,   144,   145,
     146,   145,   147,   145,   148,   145,   149,   145,   150,   145,
     151,   145,   152,   145,   153,   145,   154,   145,   155,   145,
     156,   145,   157,   158,   159
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
       0,     2,     3,     5,     3
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
#line 149 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                        {YYABORT;}
#line 1483 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 15: /* SFInt32: INTEGER  */
#line 160 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
			if(Ibuff) Ibuff->input((yyvsp[0].ival));
		}
#line 1491 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 16: /* SFFloat: FLOATING  */
#line 172 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
			if(Fbuff) Fbuff->input((yyvsp[0].fval));
		}
#line 1499 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 17: /* SFFloat: INTEGER  */
#line 176 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
			(yyval.fval) = (double)(yyvsp[0].ival);
			if(Fbuff) Fbuff->input((double)(yyvsp[0].ival));
		}
#line 1508 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 18: /* SFVec3f: SFFloat SFFloat SFFloat  */
#line 189 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                  {}
#line 1514 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 23: /* MFInt32: SFInt32  */
#line 201 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                  {}
#line 1520 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 24: /* MFInt32: '[' SFInt32List ']'  */
#line 202 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                              {}
#line 1526 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 31: /* SFVec4f: SFFloat SFFloat SFFloat SFFloat  */
#line 245 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                          {}
#line 1532 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 41: /* Name: NAME  */
#line 283 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
             {if (strlen((yyvsp[0].sval)) > sizeof(DB::str_t)-1) YYERROR; else strcpy(name,(yyvsp[0].sval));}
#line 1538 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 42: /* SetCircuitType: ASSEMBLYTYPE SFInt32  */
#line 289 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
			loadingEmP->assemblyType=(yyvsp[0].ival);
		}
#line 1546 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 43: /* Level: LEVEL SFInt32  */
#line 296 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
			loadingEmP->level=(yyvsp[0].ival);
		}
#line 1554 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 44: /* DefaultBoundCond: DEFAULTBC STRING  */
#line 304 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
	             strcpy(loadingEmP->defaultBC,(yyvsp[0].sval));
		}
#line 1562 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 45: /* Import: IMPORT STRING  */
#line 315 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
                  loadingEmP->stepFilePath=(yyvsp[0].sval);
                  if(loadingOcaf) 
                     step_file_reloaded=loadingOcaf->importSTEP_OR_DXC((yyvsp[0].sval), reload_step_file);
		}
#line 1572 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 48: /* $@1: %empty  */
#line 331 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1578 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 49: /* UnitsElement: LENGTH $@1 SFFloat  */
#line 331 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                           {unit.xm   =(yyvsp[0].fval);  }
#line 1584 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 50: /* $@2: %empty  */
#line 332 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1590 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 51: /* UnitsElement: FREQUENCY $@2 SFFloat  */
#line 332 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                           {unit.xHz  =(yyvsp[0].fval);  }
#line 1596 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 52: /* $@3: %empty  */
#line 333 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1602 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 53: /* UnitsElement: RESISTANCE $@3 SFFloat  */
#line 333 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                           {unit.xOhm =(yyvsp[0].fval);  }
#line 1608 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 58: /* $@4: %empty  */
#line 354 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1614 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 59: /* InvariantElement: ROTATION ANGLE $@4 SFFloat  */
#line 354 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                    {invt->rotAngle=(yyvsp[0].fval); }
#line 1620 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 60: /* $@5: %empty  */
#line 355 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                          {Fbuff = &DoubleBuffer; Fbuff->reset();}
#line 1626 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 61: /* InvariantElement: ROTATION AXIS $@5 MFVec3f  */
#line 356 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
            {
              DoubleBuffer.flush(dVec); 
              for(int i=0; i<3; i++) invt->rotAxis[i]=dVec[i];
            }
#line 1635 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 62: /* $@6: %empty  */
#line 360 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                            {Fbuff = &DoubleBuffer; Fbuff->reset();}
#line 1641 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 63: /* InvariantElement: ROTATION ORIGIN $@6 MFVec3f  */
#line 361 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
            {
              DoubleBuffer.flush(dVec); 
              for(int i=0; i<3; i++) invt->rotOrigin[i]=dVec[i];
            }
#line 1650 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 64: /* $@7: %empty  */
#line 365 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                          {Fbuff = &DoubleBuffer; Fbuff->reset();}
#line 1656 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 65: /* InvariantElement: TRANSLATION $@7 MFVec3f  */
#line 366 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
            {
              DoubleBuffer.flush(dVec); 
              for(int i=0; i<3; i++) invt->trasl[i]=dVec[i];
            }
#line 1665 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 66: /* InvariantBegin: DEF Name MWM_INVARIANT  */
#line 374 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
         {     
              invt =loadingEmP->FindInvariant(name);
              if(invt) loadingEmP->delInvariant(invt);
	      invt = new DB::Transform();
	      strcpy(invt->name,name);
	      loadingEmP->addInvariant(invt);
	 }
#line 1677 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 70: /* $@8: %empty  */
#line 396 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1683 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 71: /* MaterialElement: EPSILONR $@8 SFFloat  */
#line 396 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->epsr=(yyvsp[0].fval);     }
#line 1689 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 72: /* $@9: %empty  */
#line 397 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1695 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 73: /* MaterialElement: MUR $@9 SFFloat  */
#line 397 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->mur=(yyvsp[0].fval);          }
#line 1701 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 74: /* $@10: %empty  */
#line 398 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1707 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 75: /* MaterialElement: ETANDELTA $@10 SFFloat  */
#line 398 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->etandelta=(yyvsp[0].fval);}
#line 1713 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 76: /* $@11: %empty  */
#line 399 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1719 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 77: /* MaterialElement: HTANDELTA $@11 SFFloat  */
#line 399 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->htandelta=(yyvsp[0].fval);}
#line 1725 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 78: /* $@12: %empty  */
#line 400 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1731 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 79: /* MaterialElement: ECONDUCTIVITY $@12 SFFloat  */
#line 400 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->econductivity=(yyvsp[0].fval);}
#line 1737 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 80: /* $@13: %empty  */
#line 401 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1743 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 81: /* MaterialElement: HCONDUCTIVITY $@13 SFFloat  */
#line 401 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->hconductivity=(yyvsp[0].fval);}
#line 1749 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 82: /* $@14: %empty  */
#line 402 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff  =&mat->buff.epsLorentz; Fbuff->reset();}
#line 1755 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 84: /* $@15: %empty  */
#line 403 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff  =&mat->buff.muLorentz; Fbuff->reset();}
#line 1761 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 86: /* $@16: %empty  */
#line 404 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff  =&mat->buff.surfPolesRes; Fbuff->reset();}
#line 1767 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 88: /* $@17: %empty  */
#line 405 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1773 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 89: /* MaterialElement: SURFACE RESISTANCE $@17 SFFloat  */
#line 405 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->Sresistance=(yyvsp[0].fval); }
#line 1779 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 90: /* $@18: %empty  */
#line 406 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1785 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 91: /* MaterialElement: SURFACE INDUCTANCE $@18 SFFloat  */
#line 406 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->Sinductance=(yyvsp[0].fval); }
#line 1791 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 92: /* $@19: %empty  */
#line 407 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1797 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 93: /* MaterialElement: ROUGH SURFACE POLESNUM $@19 SFFloat  */
#line 407 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->roughSurfFitPolesNum=(yyvsp[0].fval); }
#line 1803 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 94: /* $@20: %empty  */
#line 408 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1809 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 95: /* MaterialElement: ROUGH SURFACE FREQUENCY $@20 SFFloat  */
#line 408 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->roughSurfFreq=(yyvsp[0].fval); }
#line 1815 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 96: /* $@21: %empty  */
#line 409 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1821 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 97: /* MaterialElement: ROUGH SURFACE MODELTYPE $@21 SFFloat  */
#line 409 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->roughSurfModelType=(yyvsp[0].fval); }
#line 1827 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 98: /* $@22: %empty  */
#line 410 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1833 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 99: /* MaterialElement: ROUGH SURFACE LOSSFACTOR $@22 SFFloat  */
#line 410 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->roughSurfLossFactor=(yyvsp[0].fval);}
#line 1839 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 100: /* $@23: %empty  */
#line 411 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                           {Fbuff = NULL;}
#line 1845 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 101: /* MaterialElement: ROUGH SURFACE IMPEDANCE QFACTOR $@23 SFFloat  */
#line 411 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->roughSurfImpedanceQ=(yyvsp[0].fval);}
#line 1851 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 102: /* $@24: %empty  */
#line 412 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1857 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 103: /* MaterialElement: ROUGH SURFACE BALLRADIUS $@24 SFFloat  */
#line 412 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->roughSurfBallRadius=(yyvsp[0].fval);}
#line 1863 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 104: /* $@25: %empty  */
#line 413 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1869 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 105: /* MaterialElement: ROUGH SURFACE SURFRATIO $@25 SFFloat  */
#line 413 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->roughSurfSurfRatio=(yyvsp[0].fval);}
#line 1875 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 106: /* $@26: %empty  */
#line 414 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1881 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 107: /* MaterialElement: ROUGH SURFACE RZ $@26 SFFloat  */
#line 414 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->roughSurfRz=(yyvsp[0].fval);}
#line 1887 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 108: /* $@27: %empty  */
#line 415 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1893 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 109: /* MaterialElement: ROUGH SURFACE RQ $@27 SFFloat  */
#line 415 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                                           {mat->roughSurfRq=(yyvsp[0].fval);}
#line 1899 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 110: /* $@28: %empty  */
#line 416 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                    { Ibuff = &IntBuffer; Ibuff->reset();}
#line 1905 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 111: /* MaterialElement: COLOR $@28 MFInt32  */
#line 417 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
            {
              IntBuffer.flush(matColor); 
              for(int i=0; i<4; i++) mat->color[i]=matColor[i];
            }
#line 1914 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 112: /* MaterialBegin: DEF Name MWM_MATERIAL  */
#line 425 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {     
                        mat =loadingEmP->FindMaterial(name);
                        if(mat) loadingEmP->delMaterial(mat);
			mat = new DB::Material();
			strcpy(mat->name,name);
			loadingEmP->addMaterial(mat);
		}
#line 1926 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 113: /* Material: MaterialBegin NodeBegin MaterialElements NodeEnd  */
#line 436 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
          { 
             mat->flush(); 
          }
#line 1934 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 114: /* VolumeType: VOLTYPE tDIELECTRIC  */
#line 447 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
		   vol->type=DIELECTRIC;
                }
#line 1942 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 115: /* VolumeType: VOLTYPE tHOLE  */
#line 451 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
		   vol->type=HOLE;
                }
#line 1950 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 116: /* VolumeType: VOLTYPE tBOUNDARYCONDITION  */
#line 455 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
		   vol->type=BOUNDARYCOND;
                }
#line 1958 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 117: /* VolumeType: VOLTYPE tWAVEGUIDE  */
#line 459 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
		   vol->type=WAVEGUIDE;
                }
#line 1966 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 118: /* VolumeType: VOLTYPE tLINEPORT  */
#line 463 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
		   vol->type=LINEPORT;
                }
#line 1974 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 119: /* VolumeType: VOLTYPE tSPLITTER  */
#line 467 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
		   vol->type=SPLITTER;
                }
#line 1982 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 120: /* VolumeType: VOLTYPE tGRID  */
#line 471 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
		   vol->type=GRID;
                }
#line 1990 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 121: /* VolumeType: VOLTYPE tNET  */
#line 475 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
		   vol->type=NET;
                }
#line 1998 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 122: /* VolumeType: VOLTYPE tCOMPONENT  */
#line 479 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
		   vol->type=COMPONENT;
                }
#line 2006 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 123: /* VolumeType: VOLTYPE tINTERFACE  */
#line 483 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
		   vol->type=INTERFACE;
                }
#line 2014 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 124: /* VolumeType: VOLTYPE tASSEMBLY  */
#line 487 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
		   vol->type=ASSEMBLY;
                }
#line 2022 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 125: /* VolumeType: VOLTYPE tUNDEFINED  */
#line 491 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
		   vol->type=UNDEFINED;
                }
#line 2030 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 129: /* VolumeElement: MATERIAL STRING  */
#line 505 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
                   if (strlen((yyvsp[0].sval)) > sizeof(DB::str_t)-1) YYERROR;
                   if(!loadingEmP->FindMaterial((yyvsp[0].sval)))  DB::yyMsg(FATAL, "Undefined material %s\n", (yyvsp[0].sval));
		   strcpy(vol->material,(yyvsp[0].sval));
                }
#line 2040 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 130: /* $@29: %empty  */
#line 510 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2046 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 131: /* VolumeElement: TEMPORTSNUM $@29 SFInt32  */
#line 511 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
                  vol->TEMportsNum =(yyvsp[0].ival);
                }
#line 2054 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 132: /* $@30: %empty  */
#line 514 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2060 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 133: /* VolumeElement: TEPORTSNUM $@30 SFInt32  */
#line 515 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
                  vol->TEportsNum =(yyvsp[0].ival);
                }
#line 2068 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 134: /* $@31: %empty  */
#line 518 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2074 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 135: /* VolumeElement: TMPORTSNUM $@31 SFInt32  */
#line 519 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
                  vol->TMportsNum =(yyvsp[0].ival);
                }
#line 2082 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 136: /* $@32: %empty  */
#line 522 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                {Ibuff  =NULL;}
#line 2088 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 137: /* VolumeElement: TEMTABULARORDER $@32 SFInt32  */
#line 523 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
                  vol->TEM_TabularOrder=(yyvsp[0].ival);
                }
#line 2096 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 138: /* $@33: %empty  */
#line 526 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2102 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 139: /* VolumeElement: GRIDNUM $@33 SFInt32  */
#line 527 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
                  vol->gridNum =(yyvsp[0].ival);
                }
#line 2110 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 140: /* $@34: %empty  */
#line 530 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {Ibuff  =NULL;}
#line 2116 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 141: /* VolumeElement: PML $@34 SFInt32  */
#line 531 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
                  vol->PML =(yyvsp[0].ival);
                }
#line 2124 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 142: /* $@35: %empty  */
#line 534 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                      {Ibuff  =NULL;}
#line 2130 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 143: /* VolumeElement: INVARIANT $@35 SFInt32  */
#line 535 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
                  vol->invariant =(yyvsp[0].ival);
                }
#line 2138 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 144: /* $@36: %empty  */
#line 538 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 2144 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 145: /* VolumeElement: MESHREFINEMENT $@36 SFFloat  */
#line 538 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                             {vol->meshRefinement=(yyvsp[0].fval); }
#line 2150 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 146: /* $@37: %empty  */
#line 539 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 2156 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 147: /* VolumeElement: CUTOFFREFINEMENT $@37 SFFloat  */
#line 539 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                             {vol->cutoffRefinement=(yyvsp[0].fval); }
#line 2162 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 148: /* $@38: %empty  */
#line 541 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                             {Ibuff = NULL;}
#line 2168 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 149: /* VolumeElement: COMPSOLID $@38 SFInt32  */
#line 541 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                             { }
#line 2174 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 150: /* $@39: %empty  */
#line 543 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                             { }
#line 2180 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 151: /* VolumeElement: DISABLED $@39  */
#line 543 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                                                             {vol->disabled=1; }
#line 2186 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 152: /* VolumeBegin: DEF Name MWM_VOLUME  */
#line 547 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
                {
			vol = new DB::Volume();
			strcpy(vol->name,name);
			loadingEmP->insertVolume(vol);
		}
#line 2196 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;

  case 154: /* CircuitName: SET CIRCUITNAME STRING  */
#line 565 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"
            {
             if (strlen((yyvsp[0].sval))>sizeof(DB::str_t)-1) YYERROR; 
             else strcpy(mainCircName,(yyvsp[0].sval));
            }
#line 2205 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"
    break;


#line 2209 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model_parser.cpp"

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

#line 571 "/home/walter/MwCAD/EmCAD/conformalMesh/src/DataBase/model.y"



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


