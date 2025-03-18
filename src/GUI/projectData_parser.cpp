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
#define yyparse         projectDataparse
#define yylex           projectDatalex
#define yyerror         projectDataerror
#define yydebug         projectDatadebug
#define yynerrs         projectDatanerrs
#define yylval          projectDatalval
#define yychar          projectDatachar

/* First part of user prologue.  */
#line 22 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"

//debug:
#define YYDEBUG 1
#define YYPRINT(file, type, value) yyprint(file, type, value)
//debug

#include "projectData.h"
#include <Message.h>
#include <vector>
#include <complex>

extern ProjectData prjData;

#define YYINITDEPTH  (1024 * 64)
#define	YYMAXDEPTH	(YYINITDEPTH * 64)


extern char *projectDatatext;
void yyerror(char *s);
int yyparse (void);
int yylex ();


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


void projectData_msg(int type, char *fmt, ...);

namespace ProjData {
 std::vector<double>  *Fbuff=NULL;
 std::vector<std::complex<double> >  *Cbuff=NULL;
 std::vector<double>  Fvec; 
}


#line 125 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"

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

#include "projectData_parser.hpp"
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
  YYSYMBOL_DEF = 7,                        /* DEF  */
  YYSYMBOL_MAINASSNAME = 8,                /* MAINASSNAME  */
  YYSYMBOL_VARFILEPATH = 9,                /* VARFILEPATH  */
  YYSYMBOL_S2PFILEPATH = 10,               /* S2PFILEPATH  */
  YYSYMBOL_NETWORK = 11,                   /* NETWORK  */
  YYSYMBOL_LENGTH = 12,                    /* LENGTH  */
  YYSYMBOL_IMAG = 13,                      /* IMAG  */
  YYSYMBOL_FREQ = 14,                      /* FREQ  */
  YYSYMBOL_UNIT = 15,                      /* UNIT  */
  YYSYMBOL_EXP = 16,                       /* EXP  */
  YYSYMBOL_MESH = 17,                      /* MESH  */
  YYSYMBOL_CONFORMAL = 18,                 /* CONFORMAL  */
  YYSYMBOL_TET = 19,                       /* TET  */
  YYSYMBOL_SHARED = 20,                    /* SHARED  */
  YYSYMBOL_REFINE = 21,                    /* REFINE  */
  YYSYMBOL_RESONANCE = 22,                 /* RESONANCE  */
  YYSYMBOL_ENERGY = 23,                    /* ENERGY  */
  YYSYMBOL_WAVELENGTH = 24,                /* WAVELENGTH  */
  YYSYMBOL_LOCAL = 25,                     /* LOCAL  */
  YYSYMBOL_MESHING3D = 26,                 /* MESHING3D  */
  YYSYMBOL_ANA = 27,                       /* ANA  */
  YYSYMBOL_BAND = 28,                      /* BAND  */
  YYSYMBOL_NUM = 29,                       /* NUM  */
  YYSYMBOL_MOR = 30,                       /* MOR  */
  YYSYMBOL_RESPONSE = 31,                  /* RESPONSE  */
  YYSYMBOL_SYMMETRIC = 32,                 /* SYMMETRIC  */
  YYSYMBOL_PARAMETER = 33,                 /* PARAMETER  */
  YYSYMBOL_TYPE = 34,                      /* TYPE  */
  YYSYMBOL_TOPOLOGY = 35,                  /* TOPOLOGY  */
  YYSYMBOL_PART = 36,                      /* PART  */
  YYSYMBOL_XSCALE = 37,                    /* XSCALE  */
  YYSYMBOL_YSCALE = 38,                    /* YSCALE  */
  YYSYMBOL_AUTO = 39,                      /* AUTO  */
  YYSYMBOL_ZERO = 40,                      /* ZERO  */
  YYSYMBOL_POLE = 41,                      /* POLE  */
  YYSYMBOL_WINDOW = 42,                    /* WINDOW  */
  YYSYMBOL_CURVE = 43,                     /* CURVE  */
  YYSYMBOL_CIRCLE = 44,                    /* CIRCLE  */
  YYSYMBOL_XYPLANE = 45,                   /* XYPLANE  */
  YYSYMBOL_YZPLANE = 46,                   /* YZPLANE  */
  YYSYMBOL_ZXPLANE = 47,                   /* ZXPLANE  */
  YYSYMBOL_SYMMETRY = 48,                  /* SYMMETRY  */
  YYSYMBOL_IDEAL = 49,                     /* IDEAL  */
  YYSYMBOL_CANONICAL = 50,                 /* CANONICAL  */
  YYSYMBOL_FILTER = 51,                    /* FILTER  */
  YYSYMBOL_CUSTOM = 52,                    /* CUSTOM  */
  YYSYMBOL_PASS = 53,                      /* PASS  */
  YYSYMBOL_STOP = 54,                      /* STOP  */
  YYSYMBOL_KRYLOV = 55,                    /* KRYLOV  */
  YYSYMBOL_ORDER = 56,                     /* ORDER  */
  YYSYMBOL_RETURNLOSS = 57,                /* RETURNLOSS  */
  YYSYMBOL_OUTBAND = 58,                   /* OUTBAND  */
  YYSYMBOL_MAPPING = 59,                   /* MAPPING  */
  YYSYMBOL_TUNING = 60,                    /* TUNING  */
  YYSYMBOL_METHOD = 61,                    /* METHOD  */
  YYSYMBOL_SOURCE = 62,                    /* SOURCE  */
  YYSYMBOL_ITERMAX = 63,                   /* ITERMAX  */
  YYSYMBOL_AUTOMATIC = 64,                 /* AUTOMATIC  */
  YYSYMBOL_DESIGN = 65,                    /* DESIGN  */
  YYSYMBOL_WITH = 66,                      /* WITH  */
  YYSYMBOL_MAPPED = 67,                    /* MAPPED  */
  YYSYMBOL_TX = 68,                        /* TX  */
  YYSYMBOL_ZEROS = 69,                     /* ZEROS  */
  YYSYMBOL_SPICE = 70,                     /* SPICE  */
  YYSYMBOL_SET = 71,                       /* SET  */
  YYSYMBOL_CENTRAL = 72,                   /* CENTRAL  */
  YYSYMBOL_CONFIG = 73,                    /* CONFIG  */
  YYSYMBOL_ADD = 74,                       /* ADD  */
  YYSYMBOL_CONJUGATE = 75,                 /* CONJUGATE  */
  YYSYMBOL_PORT = 76,                      /* PORT  */
  YYSYMBOL_IMPEDANCE = 77,                 /* IMPEDANCE  */
  YYSYMBOL_TARGET = 78,                    /* TARGET  */
  YYSYMBOL_QFACTOR = 79,                   /* QFACTOR  */
  YYSYMBOL_UNIFORM = 80,                   /* UNIFORM  */
  YYSYMBOL_INDUCTIVE = 81,                 /* INDUCTIVE  */
  YYSYMBOL_SKIN = 82,                      /* SKIN  */
  YYSYMBOL_LOSSY = 83,                     /* LOSSY  */
  YYSYMBOL_PREDISTORTED = 84,              /* PREDISTORTED  */
  YYSYMBOL_OPTIMIZE = 85,                  /* OPTIMIZE  */
  YYSYMBOL_ILOSSPP = 86,                   /* ILOSSPP  */
  YYSYMBOL_TRUSTR = 87,                    /* TRUSTR  */
  YYSYMBOL_CUTOFF = 88,                    /* CUTOFF  */
  YYSYMBOL_RATIO = 89,                     /* RATIO  */
  YYSYMBOL_RECOMPUTE = 90,                 /* RECOMPUTE  */
  YYSYMBOL_JACOBIAN = 91,                  /* JACOBIAN  */
  YYSYMBOL_ERRORT = 92,                    /* ERRORT  */
  YYSYMBOL_ONLY = 93,                      /* ONLY  */
  YYSYMBOL_TRANSVERSEJ = 94,               /* TRANSVERSEJ  */
  YYSYMBOL_XTOL = 95,                      /* XTOL  */
  YYSYMBOL_GRADDX = 96,                    /* GRADDX  */
  YYSYMBOL_REMESH = 97,                    /* REMESH  */
  YYSYMBOL_FIRST = 98,                     /* FIRST  */
  YYSYMBOL_DECOMPOSITION = 99,             /* DECOMPOSITION  */
  YYSYMBOL_MATERIAL = 100,                 /* MATERIAL  */
  YYSYMBOL_MODELIZATION = 101,             /* MODELIZATION  */
  YYSYMBOL_COMPONENT = 102,                /* COMPONENT  */
  YYSYMBOL_SAVE = 103,                     /* SAVE  */
  YYSYMBOL_REDUCTION = 104,                /* REDUCTION  */
  YYSYMBOL_RELOAD = 105,                   /* RELOAD  */
  YYSYMBOL_NEEDED = 106,                   /* NEEDED  */
  YYSYMBOL_DONE = 107,                     /* DONE  */
  YYSYMBOL_CHANGED = 108,                  /* CHANGED  */
  YYSYMBOL_tMIN = 109,                     /* tMIN  */
  YYSYMBOL_tMAX = 110,                     /* tMAX  */
  YYSYMBOL_PLUSI = 111,                    /* PLUSI  */
  YYSYMBOL_MINUSI = 112,                   /* MINUSI  */
  YYSYMBOL_113_ = 113,                     /* ','  */
  YYSYMBOL_114_ = 114,                     /* '['  */
  YYSYMBOL_115_ = 115,                     /* ']'  */
  YYSYMBOL_116_ = 116,                     /* '{'  */
  YYSYMBOL_117_ = 117,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 118,                 /* $accept  */
  YYSYMBOL_All = 119,                      /* All  */
  YYSYMBOL_PrjDataItems = 120,             /* PrjDataItems  */
  YYSYMBOL_PrjDataItem = 121,              /* PrjDataItem  */
  YYSYMBOL_SFFloat = 122,                  /* SFFloat  */
  YYSYMBOL_SFComplex = 123,                /* SFComplex  */
  YYSYMBOL_SFFloatList = 124,              /* SFFloatList  */
  YYSYMBOL_MFFloat = 125,                  /* MFFloat  */
  YYSYMBOL_SFComplexList = 126,            /* SFComplexList  */
  YYSYMBOL_MFComplex = 127,                /* MFComplex  */
  YYSYMBOL_MFFloatOrMFComplex = 128,       /* MFFloatOrMFComplex  */
  YYSYMBOL_ImportData = 129,               /* ImportData  */
  YYSYMBOL_CAD = 130,                      /* CAD  */
  YYSYMBOL_Units = 131,                    /* Units  */
  YYSYMBOL_Mesh = 132,                     /* Mesh  */
  YYSYMBOL_Mwm = 133,                      /* Mwm  */
  YYSYMBOL_Mor = 134,                      /* Mor  */
  YYSYMBOL_FreqAna = 135,                  /* FreqAna  */
  YYSYMBOL_FilterDesign = 136,             /* FilterDesign  */
  YYSYMBOL_137_1 = 137,                    /* $@1  */
  YYSYMBOL_WorkStatus = 138                /* WorkStatus  */
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
typedef yytype_int16 yy_state_t;

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
#define YYLAST   384

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  118
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  145
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  391

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   367


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
       2,     2,     2,     2,   113,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   114,     2,   115,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   116,     2,   117,     2,     2,     2,     2,
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
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   100,   100,   104,   105,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   140,   145,   154,   158,   189,
     190,   191,   192,   193,   197,   198,   203,   204,   205,   206,
     207,   211,   212,   217,   218,   272,   276,   283,   287,   294,
     298,   305,   309,   313,   317,   321,   325,   329,   333,   337,
     340,   343,   347,   351,   355,   359,   363,   370,   377,   382,
     386,   390,   394,   401,   406,   411,   415,   420,   424,   429,
     433,   437,   441,   445,   449,   453,   457,   461,   465,   469,
     473,   477,   481,   484,   488,   492,   496,   499,   503,   507,
     511,   515,   519,   523,   527,   531,   534,   538,   542,   546,
     552,   556,   562,   566,   573,   577,   581,   585,   589,   593,
     597,   604,   609,   614,   618,   622,   627,   631,   635,   639,
     643,   647,   651,   651,   659,   663,   667,   671,   675,   679,
     683,   687,   691,   694,   697,   700,   706,   710,   714,   718,
     722,   726,   730,   735,   739,   743
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
  "STRING", "NAME", "DEF", "MAINASSNAME", "VARFILEPATH", "S2PFILEPATH",
  "NETWORK", "LENGTH", "IMAG", "FREQ", "UNIT", "EXP", "MESH", "CONFORMAL",
  "TET", "SHARED", "REFINE", "RESONANCE", "ENERGY", "WAVELENGTH", "LOCAL",
  "MESHING3D", "ANA", "BAND", "NUM", "MOR", "RESPONSE", "SYMMETRIC",
  "PARAMETER", "TYPE", "TOPOLOGY", "PART", "XSCALE", "YSCALE", "AUTO",
  "ZERO", "POLE", "WINDOW", "CURVE", "CIRCLE", "XYPLANE", "YZPLANE",
  "ZXPLANE", "SYMMETRY", "IDEAL", "CANONICAL", "FILTER", "CUSTOM", "PASS",
  "STOP", "KRYLOV", "ORDER", "RETURNLOSS", "OUTBAND", "MAPPING", "TUNING",
  "METHOD", "SOURCE", "ITERMAX", "AUTOMATIC", "DESIGN", "WITH", "MAPPED",
  "TX", "ZEROS", "SPICE", "SET", "CENTRAL", "CONFIG", "ADD", "CONJUGATE",
  "PORT", "IMPEDANCE", "TARGET", "QFACTOR", "UNIFORM", "INDUCTIVE", "SKIN",
  "LOSSY", "PREDISTORTED", "OPTIMIZE", "ILOSSPP", "TRUSTR", "CUTOFF",
  "RATIO", "RECOMPUTE", "JACOBIAN", "ERRORT", "ONLY", "TRANSVERSEJ",
  "XTOL", "GRADDX", "REMESH", "FIRST", "DECOMPOSITION", "MATERIAL",
  "MODELIZATION", "COMPONENT", "SAVE", "REDUCTION", "RELOAD", "NEEDED",
  "DONE", "CHANGED", "tMIN", "tMAX", "PLUSI", "MINUSI", "','", "'['",
  "']'", "'{'", "'}'", "$accept", "All", "PrjDataItems", "PrjDataItem",
  "SFFloat", "SFComplex", "SFFloatList", "MFFloat", "SFComplexList",
  "MFComplex", "MFFloatOrMFComplex", "ImportData", "CAD", "Units", "Mesh",
  "Mwm", "Mor", "FreqAna", "FilterDesign", "$@1", "WorkStatus", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-222)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-3)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -222,    14,   247,  -222,  -222,     4,    18,    23,    -1,    43,
      31,   -18,    16,    25,    46,    72,    42,    75,    -3,   -21,
      34,    32,    44,    61,    37,    49,    79,    68,    81,    -4,
      60,    89,   113,    45,    17,    70,    71,    82,    85,    -5,
      86,    87,  -222,  -222,  -222,  -222,  -222,  -222,  -222,  -222,
    -222,  -222,  -222,  -222,  -222,  -222,    96,   180,    90,   192,
     182,    47,    91,    52,   110,   111,   179,   200,    33,   176,
      -6,   136,   202,   139,   193,   143,   150,   178,    27,   205,
     207,   208,     0,   181,   152,   185,   186,   212,   177,   160,
     -24,    83,   149,   144,   141,   177,   140,   172,   197,   195,
     187,   162,   168,   163,    13,   159,   -14,    21,   177,   229,
     230,   231,   232,   233,   115,   223,   133,   135,   239,   240,
     134,   169,   216,   243,  -222,   246,   248,   249,    28,   224,
     225,   234,   259,   263,   189,  -222,   177,   191,   177,   177,
     196,   256,   242,  -222,   177,   273,   252,   177,   283,   285,
     286,   262,    40,   206,   288,  -222,  -222,  -222,   297,   298,
     235,   228,   301,   302,   177,   177,  -222,  -222,  -222,  -222,
     177,   304,   305,   306,   307,   177,   309,   310,   177,    93,
     220,   177,   177,  -222,   177,   177,  -222,   312,   313,   177,
     315,   316,   279,   318,   281,   292,   284,   251,   323,   324,
     177,   177,  -222,   177,   325,   326,   177,   177,  -222,  -222,
    -222,  -222,  -222,  -222,   222,   173,   308,   330,   331,  -222,
    -222,   250,   322,   311,   335,  -222,  -222,  -222,  -222,   338,
     177,   339,   340,   347,  -222,  -222,   177,  -222,   350,   177,
     177,   177,   327,   351,   177,  -222,   352,   177,  -222,  -222,
    -222,   177,   354,   177,   177,   355,  -222,  -222,   290,   291,
    -222,  -222,   177,   177,  -222,  -222,  -222,  -222,  -222,  -222,
    -222,  -222,  -222,   358,   359,   360,  -222,  -222,   253,  -222,
    -222,  -222,  -222,   177,  -222,  -222,   356,  -222,   361,   362,
     363,   365,  -222,  -222,  -222,  -222,  -222,  -222,  -222,  -222,
    -222,   280,   357,   343,   370,  -222,  -222,   177,   345,   372,
    -222,  -222,   177,  -222,  -222,  -222,  -222,  -222,  -222,  -222,
    -222,   373,  -222,  -222,  -222,  -222,   177,  -222,   177,  -222,
    -222,   374,   375,  -222,  -222,  -222,  -222,  -222,   177,  -222,
    -222,  -222,  -222,   348,  -222,  -222,  -222,  -222,   177,   353,
     377,  -222,  -222,   378,  -222,  -222,  -222,  -222,  -222,  -222,
    -222,     9,     1,    12,    15,   380,  -222,   381,  -222,  -222,
     177,   177,   177,  -222,  -222,   177,  -222,    77,  -222,  -222,
    -222,  -222,  -222,  -222,  -222,  -222,  -222,  -222,  -222,  -222,
    -222
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     8,     5,     6,     7,    11,    12,     9,
      10,    13,    37,    35,    36,    38,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,     0,     0,     0,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,     0,     0,
       0,     0,     0,     0,     0,     0,   113,    16,    15,   116,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   122,     0,     0,   119,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   125,     0,     0,     0,     0,     0,    57,   139,
     137,   136,   138,   140,     0,     0,     0,     0,     0,   142,
     145,     0,     0,     0,     0,   144,    40,    97,    98,     0,
       0,     0,     0,     0,    41,    44,     0,    43,     0,     0,
       0,     0,     0,     0,     0,    67,     0,     0,    59,    62,
     124,     0,     0,     0,     0,     0,    77,    80,     0,     0,
      79,    92,     0,     0,   117,    83,    82,    84,    86,    85,
      88,    87,    94,     0,     0,     0,    93,    95,     0,   118,
     120,   121,    81,     0,   104,   105,     0,   110,     0,     0,
       0,     0,   135,   132,   133,   134,   127,   130,   126,   128,
     129,     0,     0,     0,     0,   141,   143,     0,     0,     0,
      74,   100,     0,    47,    45,    46,    52,    42,    49,    50,
      51,     0,    60,    63,    68,    58,     0,   102,     0,    65,
     103,     0,     0,   111,   112,    89,    90,    91,    23,    33,
      34,   123,    66,     0,   106,   107,   109,    96,     0,     0,
       0,    71,    70,     0,    75,    99,    61,    64,   101,   115,
     114,     0,     0,     0,     0,     0,    69,     0,    72,    76,
       0,     0,     0,    24,    19,     0,    31,     0,    26,    22,
      25,    21,    29,    32,    28,   108,    73,    17,    18,    20,
      27
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -222,  -222,  -222,  -222,   -88,  -221,  -222,  -222,  -222,  -222,
    -222,  -222,  -222,  -222,  -222,  -222,  -222,  -222,  -222,  -222,
    -222
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    42,   377,   362,   363,   339,   364,   340,
     341,    43,    44,    45,    46,    47,    48,    49,    50,   278,
      51
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     169,    62,    55,    63,   167,   168,    64,   186,   171,    52,
      99,    75,   167,   168,     3,   167,   168,   114,   167,   168,
     208,    56,   139,    53,   202,   115,    65,   195,    54,    57,
      77,   167,   168,    67,   158,   159,   100,   172,   173,   174,
     230,   151,    68,   167,   168,   101,    60,   102,   237,   198,
     239,   240,    76,   196,   136,   175,   244,   137,    59,   247,
      69,   103,    61,   104,   253,   152,   160,   229,    72,   153,
     154,   199,   200,   201,   161,    78,   262,   263,   203,   252,
      79,   126,   264,   127,   204,   128,    70,   269,    82,    73,
     272,    66,    80,   276,   277,    98,   279,   280,   116,   117,
      83,   283,    71,    58,   140,    74,   205,   206,   207,    81,
     120,    84,   294,   295,   375,   296,   376,    97,   299,   300,
     370,   371,   372,   109,   373,   379,   121,   380,   382,   214,
     383,   105,    85,    86,   108,    87,    88,    89,    90,    91,
     106,   378,   312,   384,   176,   215,   177,    92,   316,   141,
     142,   318,   319,   320,   390,    93,   323,    94,    95,   325,
      96,   130,   131,   326,   107,   328,   329,   144,   145,   110,
     178,   147,   148,   179,   333,   334,   180,   111,   181,   182,
     167,   168,   222,   223,   273,   274,   302,   303,   370,   371,
     112,   113,   118,   119,   122,   342,   123,   124,   125,   132,
     133,   129,   134,   135,   138,   143,   149,   146,   155,   150,
     156,   157,   163,   164,   165,   166,   162,   170,   183,   352,
     185,   184,   187,   188,   355,   189,   190,   193,   191,   192,
     194,   197,   209,   210,   211,   212,   213,   216,   357,   217,
     358,   218,   219,   220,   221,   224,   225,    -2,     4,   226,
     361,   227,   228,   231,   232,     5,     6,     7,     8,     9,
     366,    10,   234,   233,    11,    12,   235,    13,    14,    15,
     242,   243,    16,   374,    17,   381,   245,    18,   236,    19,
     238,   246,   387,   388,   389,   241,   248,    20,   249,   250,
     251,   255,    21,    22,    23,   254,    24,    25,    26,    27,
     256,   257,   258,   259,   260,   261,    28,   265,   266,   267,
     268,    29,   270,   271,   275,   281,   282,    30,   284,   285,
     286,   287,   288,   289,   291,   290,   292,   293,   297,   298,
      31,    32,   301,   305,   306,    33,   308,   304,   310,   307,
     309,   311,   313,   314,    34,    35,    36,    37,    38,    39,
     315,    40,    41,   317,   322,   324,   321,   327,   330,   331,
     332,   335,   336,   337,   344,   345,   346,   338,   347,   348,
     343,   349,   350,   351,   353,   354,   356,   359,   360,   365,
     368,   369,   367,   385,   386
};

static const yytype_int16 yycheck[] =
{
      88,    19,     3,    21,     3,     4,    24,    95,    32,     5,
      14,    14,     3,     4,     0,     3,     4,    22,     3,     4,
     108,    22,    28,     5,     3,    30,    44,    14,     5,    30,
      51,     3,     4,    17,    34,    35,    40,    61,    62,    63,
     128,    14,    17,     3,     4,    49,    15,    51,   136,    63,
     138,   139,    55,    40,    21,    79,   144,    24,    15,   147,
      14,    65,    31,    67,   152,    38,    66,    39,    26,    42,
      43,    85,    86,    87,    74,    41,   164,   165,    57,    39,
      48,    34,   170,    36,    63,    38,    14,   175,    51,    14,
     178,   109,    48,   181,   182,    14,   184,   185,   103,   104,
      51,   189,    30,   104,   110,    30,    85,    86,    87,    48,
      14,    32,   200,   201,   113,   203,   115,    49,   206,   207,
     111,   112,   113,   106,   115,   113,    30,   115,   113,    14,
     115,    71,    53,    54,    89,    56,    57,    58,    59,    60,
      51,   362,   230,   364,    61,    30,    63,    68,   236,    13,
      14,   239,   240,   241,   375,    76,   244,    78,    79,   247,
      81,   109,   110,   251,    51,   253,   254,    28,    29,    99,
      87,    28,    29,    90,   262,   263,    93,   106,    95,    96,
       3,     4,    13,    14,    91,    92,    13,    14,   111,   112,
     108,   106,   106,   106,    14,   283,   106,     5,    16,    89,
      89,   110,    23,     3,    28,     3,    56,    14,     3,    31,
       3,     3,    60,    28,    28,     3,    35,    57,    69,   307,
      79,    77,    82,    51,   312,    28,    31,    59,    41,    67,
      67,    72,     3,     3,     3,     3,     3,    14,   326,   106,
     328,   106,     3,     3,   110,    29,     3,     0,     1,     3,
     338,     3,     3,    29,    29,     8,     9,    10,    11,    12,
     348,    14,     3,    29,    17,    18,     3,    20,    21,    22,
      14,    29,    25,   361,    27,   363,     3,    30,    89,    32,
      89,    29,   370,   371,   372,    89,     3,    40,     3,     3,
      28,     3,    45,    46,    47,    89,    49,    50,    51,    52,
       3,     3,    67,    75,     3,     3,    59,     3,     3,     3,
       3,    64,     3,     3,    94,     3,     3,    70,     3,     3,
      41,     3,    41,    31,    73,    41,     3,     3,     3,     3,
      83,    84,   110,     3,     3,    88,    14,    29,     3,    89,
      29,     3,     3,     3,    97,    98,    99,   100,   101,   102,
       3,   104,   105,     3,     3,     3,    29,     3,     3,    69,
      69,     3,     3,     3,     3,     3,     3,   114,     3,    89,
      14,    14,    29,     3,    29,     3,     3,     3,     3,    31,
       3,     3,    29,     3,     3
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   119,   120,     0,     1,     8,     9,    10,    11,    12,
      14,    17,    18,    20,    21,    22,    25,    27,    30,    32,
      40,    45,    46,    47,    49,    50,    51,    52,    59,    64,
      70,    83,    84,    88,    97,    98,    99,   100,   101,   102,
     104,   105,   121,   129,   130,   131,   132,   133,   134,   135,
     136,   138,     5,     5,     5,     3,    22,    30,   104,    15,
      15,    31,    19,    21,    24,    44,   109,    17,    17,    14,
      14,    30,    26,    14,    30,    14,    55,    51,    41,    48,
      48,    48,    51,    51,    32,    53,    54,    56,    57,    58,
      59,    60,    68,    76,    78,    79,    81,    49,    14,    14,
      40,    49,    51,    65,    67,    71,    51,    51,    89,   106,
      99,   106,   108,   106,    22,    30,   103,   104,   106,   106,
      14,    30,    14,   106,     5,    16,    34,    36,    38,   110,
     109,   110,    89,    89,    23,     3,    21,    24,    28,    28,
     110,    13,    14,     3,    28,    29,    14,    28,    29,    56,
      31,    14,    38,    42,    43,     3,     3,     3,    34,    35,
      66,    74,    35,    60,    28,    28,     3,     3,     4,   122,
      57,    32,    61,    62,    63,    79,    61,    63,    87,    90,
      93,    95,    96,    69,    77,    79,   122,    82,    51,    28,
      31,    41,    67,    59,    67,    14,    40,    72,    63,    85,
      86,    87,     3,    57,    63,    85,    86,    87,   122,     3,
       3,     3,     3,     3,    14,    30,    14,   106,   106,     3,
       3,   110,    13,    14,    29,     3,     3,     3,     3,    39,
     122,    29,    29,    29,     3,     3,    89,   122,    89,   122,
     122,    89,    14,    29,   122,     3,    29,   122,     3,     3,
       3,    28,    39,   122,    89,     3,     3,     3,    67,    75,
       3,     3,   122,   122,   122,     3,     3,     3,     3,   122,
       3,     3,   122,    91,    92,    94,   122,   122,   137,   122,
     122,     3,     3,   122,     3,     3,    41,     3,    41,    31,
      41,    73,     3,     3,   122,   122,   122,     3,     3,   122,
     122,   110,    13,    14,    29,     3,     3,    89,    14,    29,
       3,     3,   122,     3,     3,     3,   122,     3,   122,   122,
     122,    29,     3,   122,     3,   122,   122,     3,   122,   122,
       3,    69,    69,   122,   122,     3,     3,     3,   114,   125,
     127,   128,   122,    14,     3,     3,     3,     3,    89,    14,
      29,     3,   122,    29,     3,   122,     3,   122,   122,     3,
       3,   122,   123,   124,   126,    31,   122,    29,     3,     3,
     111,   112,   113,   115,   122,   113,   115,   122,   123,   113,
     115,   122,   113,   115,   123,     3,     3,   122,   122,   122,
     123
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   118,   119,   120,   120,   121,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   122,   122,   123,   123,   124,
     124,   124,   124,   124,   125,   125,   126,   126,   126,   126,
     126,   127,   127,   128,   128,   129,   129,   130,   130,   131,
     131,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   133,   134,   134,
     134,   134,   134,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   137,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     2,
       3,     2,     2,     0,     3,     3,     2,     3,     2,     2,
       0,     3,     3,     1,     1,     2,     2,     2,     2,     3,
       4,     4,     5,     4,     4,     5,     5,     5,     3,     5,
       5,     5,     5,     3,     3,     3,     3,     3,     5,     4,
       5,     6,     4,     5,     6,     5,     5,     4,     5,     6,
       6,     5,     6,     7,     5,     6,     7,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     5,
       5,     5,     4,     4,     4,     4,     5,     4,     4,     6,
       5,     6,     5,     5,     4,     4,     5,     5,     7,     5,
       4,     5,     5,     3,     6,     6,     3,     4,     4,     3,
       4,     4,     0,     5,     4,     3,     4,     4,     4,     4,
       4,     3,     4,     4,     4,     4,     3,     3,     3,     3,
       3,     4,     3,     4,     4,     3
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
  case 14: /* PrjDataItem: error  */
#line 119 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                        {YYABORT;}
#line 1505 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 15: /* SFFloat: FLOATING  */
#line 141 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                {
		      (yyval.fval) = (yyvsp[0].fval);
		      if(ProjData::Fbuff) ProjData::Fbuff->push_back((yyvsp[0].fval));
		}
#line 1514 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 16: /* SFFloat: INTEGER  */
#line 146 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                {
		       (yyval.fval) = (double)(yyvsp[0].ival);
		       if(ProjData::Fbuff) ProjData::Fbuff->push_back((double)(yyvsp[0].ival));
		}
#line 1523 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 17: /* SFComplex: SFFloat PLUSI SFFloat  */
#line 155 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                {
		       if(ProjData::Cbuff) ProjData::Cbuff->push_back(std::complex<double>((yyvsp[-2].fval),(yyvsp[0].fval)));
		}
#line 1531 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 18: /* SFComplex: SFFloat MINUSI SFFloat  */
#line 159 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                {
		       if(ProjData::Cbuff) ProjData::Cbuff->push_back(std::complex<double>((yyvsp[-2].fval),-(yyvsp[0].fval)));
		}
#line 1539 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 24: /* MFFloat: '[' SFFloat ']'  */
#line 197 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                          {}
#line 1545 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 25: /* MFFloat: '[' SFFloatList ']'  */
#line 198 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                              {}
#line 1551 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 31: /* MFComplex: '[' SFComplex ']'  */
#line 211 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                            {}
#line 1557 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 32: /* MFComplex: '[' SFComplexList ']'  */
#line 212 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                                {}
#line 1563 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 33: /* MFFloatOrMFComplex: MFFloat  */
#line 217 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                  {}
#line 1569 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 34: /* MFFloatOrMFComplex: MFComplex  */
#line 218 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                    {}
#line 1575 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 35: /* ImportData: VARFILEPATH STRING  */
#line 273 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.varFilePath=(yyvsp[0].sval);
            }
#line 1583 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 36: /* ImportData: S2PFILEPATH STRING  */
#line 277 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.s2pFilePath=(yyvsp[0].sval);
            }
#line 1591 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 37: /* CAD: MAINASSNAME STRING  */
#line 284 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.mainAssName=(yyvsp[0].sval);
            }
#line 1599 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 38: /* CAD: NETWORK INTEGER  */
#line 288 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.network=(yyvsp[0].ival);
            }
#line 1607 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 39: /* Units: LENGTH UNIT STRING  */
#line 295 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              strcpy(prjData.lengthUnitName,(yyvsp[0].sval));
            }
#line 1615 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 40: /* Units: FREQ UNIT EXP INTEGER  */
#line 299 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqUnitE =(yyvsp[0].ival);
            }
#line 1623 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 41: /* Mesh: MESH WAVELENGTH RATIO INTEGER  */
#line 306 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshPerWavelen=(yyvsp[0].ival);
            }
#line 1631 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 42: /* Mesh: SHARED MESH WAVELENGTH RATIO INTEGER  */
#line 310 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.sharedMeshPerWavelen=(yyvsp[0].ival);
            }
#line 1639 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 43: /* Mesh: SHARED MESH REFINE SFFloat  */
#line 314 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.sharedMeshRefine=(yyvsp[0].fval);
            }
#line 1647 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 44: /* Mesh: MESH CIRCLE RATIO INTEGER  */
#line 318 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshPerCircle=(yyvsp[0].ival);
            }
#line 1655 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 45: /* Mesh: MESH REFINE tMIN NUM INTEGER  */
#line 322 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshRefineMinNum=(yyvsp[0].ival);
            }
#line 1663 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 46: /* Mesh: MESH REFINE tMAX NUM INTEGER  */
#line 326 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshRefineMaxNum=(yyvsp[0].ival);
            }
#line 1671 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 47: /* Mesh: MESH TET tMAX NUM INTEGER  */
#line 330 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshTetMaxNum=(yyvsp[0].ival);
            }
#line 1679 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 48: /* Mesh: CONFORMAL MESH INTEGER  */
#line 334 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.conformalMeshIF=(yyvsp[0].ival);
            }
#line 1687 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 49: /* Mesh: REFINE FREQ BAND SFFloat SFFloat  */
#line 338 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 1694 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 50: /* Mesh: RESONANCE FREQ BAND SFFloat SFFloat  */
#line 341 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 1701 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 51: /* Mesh: RESONANCE FREQ tMAX RATIO SFFloat  */
#line 344 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.resonFreqMaxRatio=(yyvsp[0].fval);
            }
#line 1709 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 52: /* Mesh: MESH tMIN ENERGY RATIO SFFloat  */
#line 348 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshMinEnergyRatio=(yyvsp[0].fval);
            }
#line 1717 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 53: /* Mesh: LOCAL MESHING3D INTEGER  */
#line 352 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.localMeshing3d=(yyvsp[0].ival);
            }
#line 1725 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 54: /* Mesh: XYPLANE SYMMETRY INTEGER  */
#line 356 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.XYplaneSymmetry=(yyvsp[0].ival);
            }
#line 1733 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 55: /* Mesh: YZPLANE SYMMETRY INTEGER  */
#line 360 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.YZplaneSymmetry=(yyvsp[0].ival);
            }
#line 1741 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 56: /* Mesh: ZXPLANE SYMMETRY INTEGER  */
#line 364 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.ZXplaneSymmetry=(yyvsp[0].ival);
            }
#line 1749 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 57: /* Mwm: CUTOFF RATIO SFFloat  */
#line 371 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
           {
              prjData.cutoffRatio=(yyvsp[0].fval);
           }
#line 1757 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 58: /* Mor: MOR FREQ BAND SFFloat SFFloat  */
#line 378 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqBand[0]=(yyvsp[-1].fval);
              prjData.freqBand[1]=(yyvsp[0].fval);
            }
#line 1766 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 59: /* Mor: MOR FREQ NUM INTEGER  */
#line 383 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.MORFreqNum=(yyvsp[0].ival);
            }
#line 1774 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 60: /* Mor: RESONANCE MOR FREQ NUM INTEGER  */
#line 387 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.MORFreqNumR=(yyvsp[0].ival);
            }
#line 1782 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 61: /* Mor: RESONANCE MOR IMAG FREQ NUM INTEGER  */
#line 391 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.MORFreqNumI=(yyvsp[0].ival);
            }
#line 1790 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 62: /* Mor: MOR KRYLOV ORDER INTEGER  */
#line 395 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.KrylovOrder=(yyvsp[0].ival);
            }
#line 1798 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 63: /* FreqAna: ANA FREQ BAND SFFloat SFFloat  */
#line 402 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.anaFreqBand[0]=(yyvsp[-1].fval);
              prjData.anaFreqBand[1]=(yyvsp[0].fval);
            }
#line 1807 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 64: /* FreqAna: ZERO POLE FREQ BAND SFFloat SFFloat  */
#line 407 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.zpFreqBand[0]=(yyvsp[-1].fval);
              prjData.zpFreqBand[1]=(yyvsp[0].fval);
            }
#line 1816 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 65: /* FreqAna: ZERO POLE WINDOW RATIO SFFloat  */
#line 412 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.zpWinRatio=(yyvsp[0].fval);
            }
#line 1824 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 66: /* FreqAna: MAPPING FREQ BAND SFFloat SFFloat  */
#line 416 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.mapFreqBand[0]=(yyvsp[-1].fval);
              prjData.mapFreqBand[1]=(yyvsp[0].fval);
            }
#line 1833 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 67: /* FreqAna: ANA FREQ NUM INTEGER  */
#line 421 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.anaFreqNum=(yyvsp[0].ival);
            }
#line 1841 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 68: /* FreqAna: ANA MOR FREQ NUM INTEGER  */
#line 425 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              if(prjData.network) prjData.netMORFreqNum=(yyvsp[0].ival);
              else                prjData.cmpMORFreqNum=(yyvsp[0].ival);
            }
#line 1850 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 69: /* FreqAna: COMPONENT RESONANCE FREQ tMAX RATIO SFFloat  */
#line 430 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.cmpResonFreqMaxRatio=(yyvsp[0].fval);
            }
#line 1858 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 70: /* FreqAna: NETWORK RESONANCE FREQ tMAX RATIO SFFloat  */
#line 434 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.netResonFreqMaxRatio=(yyvsp[0].fval);
            }
#line 1866 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 71: /* FreqAna: COMPONENT MOR FREQ NUM INTEGER  */
#line 438 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.cmpMORFreqNum=(yyvsp[0].ival);
            }
#line 1874 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 72: /* FreqAna: COMPONENT RESONANCE MOR FREQ NUM INTEGER  */
#line 442 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.cmpMORFreqNumR=(yyvsp[0].ival);
            }
#line 1882 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 73: /* FreqAna: COMPONENT RESONANCE MOR IMAG FREQ NUM INTEGER  */
#line 446 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.cmpMORFreqNumI=(yyvsp[0].ival);
            }
#line 1890 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 74: /* FreqAna: NETWORK MOR FREQ NUM INTEGER  */
#line 450 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.netMORFreqNum=(yyvsp[0].ival);
            }
#line 1898 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 75: /* FreqAna: NETWORK RESONANCE MOR FREQ NUM INTEGER  */
#line 454 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.netMORFreqNumR=(yyvsp[0].ival);
            }
#line 1906 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 76: /* FreqAna: NETWORK RESONANCE MOR IMAG FREQ NUM INTEGER  */
#line 458 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.netMORFreqNumI=(yyvsp[0].ival);
            }
#line 1914 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 77: /* FreqAna: IDEAL FILTER TYPE INTEGER  */
#line 462 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.idealFilterType=(yyvsp[0].ival);
            }
#line 1922 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 78: /* FreqAna: IDEAL FILTER TYPE INTEGER  */
#line 466 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.idealFilterType=(yyvsp[0].ival);
            }
#line 1930 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 79: /* FreqAna: CANONICAL FILTER TOPOLOGY INTEGER  */
#line 470 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.canonicalFilterTopology=(yyvsp[0].ival);
            }
#line 1938 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 80: /* FreqAna: IDEAL FILTER TOPOLOGY INTEGER  */
#line 474 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.canonicalFilterTopology=(yyvsp[0].ival);
            }
#line 1946 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 81: /* FreqAna: CUSTOM IDEAL FILTER INTEGER  */
#line 478 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.customIdealFilter=(yyvsp[0].ival);
            }
#line 1954 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 82: /* FreqAna: FILTER MAPPING METHOD INTEGER  */
#line 482 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 1961 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 83: /* FreqAna: FILTER MAPPING SYMMETRIC INTEGER  */
#line 485 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filtermapSymmetric=(yyvsp[0].ival);
            }
#line 1969 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 84: /* FreqAna: FILTER MAPPING SOURCE INTEGER  */
#line 489 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filtermapSource=(yyvsp[0].ival);
            }
#line 1977 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 85: /* FreqAna: FILTER MAPPING QFACTOR SFFloat  */
#line 493 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filtermapQfactor = (yyvsp[0].fval);
            }
#line 1985 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 86: /* FreqAna: FILTER MAPPING ITERMAX INTEGER  */
#line 497 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 1992 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 87: /* FreqAna: FILTER TUNING ITERMAX INTEGER  */
#line 500 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneItermax=(yyvsp[0].ival);
            }
#line 2000 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 88: /* FreqAna: FILTER TUNING METHOD INTEGER  */
#line 504 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneMethod=(yyvsp[0].ival);
            }
#line 2008 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 89: /* FreqAna: FILTER TUNING RECOMPUTE JACOBIAN INTEGER  */
#line 508 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneRecomputeJaco=(yyvsp[0].ival);
            }
#line 2016 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 90: /* FreqAna: FILTER TUNING RECOMPUTE ERRORT INTEGER  */
#line 512 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneRecomputeError=(yyvsp[0].ival);
            }
#line 2024 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 91: /* FreqAna: FILTER TUNING ONLY TRANSVERSEJ INTEGER  */
#line 516 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneOnlyJt=(yyvsp[0].ival);
            }
#line 2032 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 92: /* FreqAna: FILTER SYMMETRIC TUNING INTEGER  */
#line 520 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterSymmetricTuning=(yyvsp[0].ival);
            }
#line 2040 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 93: /* FreqAna: FILTER TUNING XTOL SFFloat  */
#line 524 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneXtol=(yyvsp[0].fval);
            }
#line 2048 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 94: /* FreqAna: FILTER TUNING TRUSTR SFFloat  */
#line 528 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneTrustR=(yyvsp[0].fval);
            }
#line 2056 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 95: /* FreqAna: FILTER TUNING GRADDX SFFloat  */
#line 532 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 2063 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 96: /* FreqAna: SPICE SET CENTRAL CONFIG INTEGER  */
#line 535 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.spiceSetCentralConfig=(yyvsp[0].ival);
            }
#line 2071 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 97: /* FreqAna: FREQ RESPONSE TYPE INTEGER  */
#line 539 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqRespParType = (FreqRespParType) (yyvsp[0].ival);
            }
#line 2079 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 98: /* FreqAna: FREQ RESPONSE PART INTEGER  */
#line 543 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqRespParPart = (yyvsp[0].ival);
            }
#line 2087 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 99: /* FreqAna: FREQ RESPONSE YSCALE SFFloat SFFloat SFFloat  */
#line 547 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqRespYscale[0]=(yyvsp[-2].fval);
              prjData.freqRespYscale[1]=(yyvsp[-1].fval);
              prjData.freqRespYscale[2]=(yyvsp[0].fval);
            }
#line 2097 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 100: /* FreqAna: FREQ RESPONSE YSCALE AUTO INTEGER  */
#line 553 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqRespYscaleAuto=(yyvsp[0].ival);
            }
#line 2105 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 101: /* FreqAna: ZERO POLE YSCALE SFFloat SFFloat SFFloat  */
#line 557 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.zeropoleYscale[0]=(yyvsp[-2].fval);
              prjData.zeropoleYscale[1]=(yyvsp[-1].fval);
              prjData.zeropoleYscale[2]=(yyvsp[0].fval);
            }
#line 2115 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 102: /* FreqAna: ZERO POLE YSCALE AUTO INTEGER  */
#line 563 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.zeropoleYscaleAuto=(yyvsp[0].ival);
            }
#line 2123 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 103: /* FreqAna: ZERO POLE CURVE INTEGER INTEGER  */
#line 567 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              std::pair<int, int> Sij_ports;
              Sij_ports.first=(yyvsp[-1].ival);
              Sij_ports.second=(yyvsp[0].ival);
              prjData.zeropoleCurves.insert(Sij_ports);
            }
#line 2134 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 104: /* FreqAna: AUTOMATIC FREQ RESPONSE INTEGER  */
#line 574 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoFreqResponse=(yyvsp[0].ival);
            }
#line 2142 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 105: /* FreqAna: AUTOMATIC ZERO POLE INTEGER  */
#line 578 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoZeropole=(yyvsp[0].ival);
            }
#line 2150 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 106: /* FreqAna: AUTOMATIC DESIGN MAPPED POLE INTEGER  */
#line 582 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoDesignWithMappedTz=(yyvsp[0].ival);
            }
#line 2158 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 107: /* FreqAna: AUTOMATIC MAPPED FREQ RESPONSE INTEGER  */
#line 586 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoMappedFreqResponse=(yyvsp[0].ival);
            }
#line 2166 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 108: /* FreqAna: AUTOMATIC IDEAL MAPPED POLE FREQ RESPONSE INTEGER  */
#line 590 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoIdealMappedTzFreqResponse=(yyvsp[0].ival);
            }
#line 2174 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 109: /* FreqAna: AUTOMATIC MAPPED ZERO POLE INTEGER  */
#line 594 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoMappedZeropole=(yyvsp[0].ival);
            }
#line 2182 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 110: /* FreqAna: AUTOMATIC FILTER MAPPING INTEGER  */
#line 598 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoFilterMapping=(yyvsp[0].ival);
            }
#line 2190 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 111: /* FilterDesign: FILTER PASS BAND SFFloat SFFloat  */
#line 605 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterPassBand[0]=(yyvsp[-1].fval);
              prjData.filterPassBand[1]=(yyvsp[0].fval);
            }
#line 2199 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 112: /* FilterDesign: FILTER STOP BAND SFFloat SFFloat  */
#line 610 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterStopBand[0]=(yyvsp[-1].fval);
              prjData.filterStopBand[1]=(yyvsp[0].fval);
            }
#line 2208 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 113: /* FilterDesign: FILTER ORDER INTEGER  */
#line 615 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterOrder = (yyvsp[0].ival);
            }
#line 2216 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 114: /* FilterDesign: IDEAL FILTER ADD CONJUGATE ZEROS INTEGER  */
#line 619 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.idealFilterAddConjugateTZ = (yyvsp[0].ival);
            }
#line 2224 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 115: /* FilterDesign: IDEAL FILTER WITH MAPPED ZEROS INTEGER  */
#line 623 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.idealFilterWithMappedTZ = (yyvsp[0].ival);
              if(prjData.idealFilterWithMappedTZ) prjData.idealFilterAddConjugateTZ=0;
            }
#line 2233 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 116: /* FilterDesign: FILTER RETURNLOSS SFFloat  */
#line 628 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterRetLoss = (yyvsp[0].fval);
            }
#line 2241 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 117: /* FilterDesign: FILTER OUTBAND RETURNLOSS SFFloat  */
#line 632 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterOutbandRetLoss = (yyvsp[0].fval);
            }
#line 2249 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 118: /* FilterDesign: FILTER PORT IMPEDANCE SFFloat  */
#line 636 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterPortImpedance = (yyvsp[0].fval);
            }
#line 2257 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 119: /* FilterDesign: FILTER QFACTOR SFFloat  */
#line 640 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterQfactor = (yyvsp[0].fval);
            }
#line 2265 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 120: /* FilterDesign: FILTER TARGET QFACTOR SFFloat  */
#line 644 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTargetQfactor = (yyvsp[0].fval);
            }
#line 2273 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 121: /* FilterDesign: FILTER INDUCTIVE SKIN INTEGER  */
#line 648 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterInductiveSkin = (yyvsp[0].ival);
            }
#line 2281 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 122: /* $@1: %empty  */
#line 651 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                           {ProjData::Cbuff =&prjData.filterZeros; ProjData::Cbuff->clear();
                            ProjData::Fbuff =&ProjData::Fvec; ProjData::Fvec.clear();}
#line 2288 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 123: /* FilterDesign: FILTER TX ZEROS $@1 MFFloatOrMFComplex  */
#line 653 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              if(prjData.filterZeros.size()==0){
               for (int i=0; i<ProjData::Fvec.size() ; i++) prjData.filterZeros.push_back(std::complex<double>(ProjData::Fvec[i],0.0));
               ProjData::Fbuff=NULL;
              } else ProjData::Cbuff=NULL;
            }
#line 2299 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 124: /* FilterDesign: SYMMETRIC FILTER RESPONSE INTEGER  */
#line 660 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.symmFilterResponse = (yyvsp[0].ival);
            }
#line 2307 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 125: /* FilterDesign: PREDISTORTED FILTER INTEGER  */
#line 664 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistortedFilter = (yyvsp[0].ival);
            }
#line 2315 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 126: /* FilterDesign: PREDISTORTED FILTER OPTIMIZE INTEGER  */
#line 668 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistFilterOptim = (yyvsp[0].ival);
            }
#line 2323 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 127: /* FilterDesign: PREDISTORTED FILTER RETURNLOSS SFFloat  */
#line 672 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimRL = (yyvsp[0].fval);
            }
#line 2331 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 128: /* FilterDesign: PREDISTORTED FILTER ILOSSPP SFFloat  */
#line 676 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimILpp = (yyvsp[0].fval);
            }
#line 2339 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 129: /* FilterDesign: PREDISTORTED FILTER TRUSTR SFFloat  */
#line 680 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimTrustR = (yyvsp[0].fval);
            }
#line 2347 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 130: /* FilterDesign: PREDISTORTED FILTER ITERMAX INTEGER  */
#line 684 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimIterMax = (yyvsp[0].ival);
            }
#line 2355 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 131: /* FilterDesign: PREDISTORTED FILTER INTEGER  */
#line 688 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistortedFilter = (yyvsp[0].ival);
            }
#line 2363 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 132: /* FilterDesign: LOSSY FILTER OPTIMIZE INTEGER  */
#line 692 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 2370 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 133: /* FilterDesign: LOSSY FILTER ILOSSPP SFFloat  */
#line 695 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 2377 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 134: /* FilterDesign: LOSSY FILTER TRUSTR SFFloat  */
#line 698 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 2384 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 135: /* FilterDesign: LOSSY FILTER ITERMAX INTEGER  */
#line 701 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 2391 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 136: /* WorkStatus: DECOMPOSITION NEEDED INTEGER  */
#line 707 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.decompositionNeeded=(yyvsp[0].ival);
            }
#line 2399 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 137: /* WorkStatus: FIRST DECOMPOSITION INTEGER  */
#line 711 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.firstDecomposition=(yyvsp[0].ival);
            }
#line 2407 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 138: /* WorkStatus: MATERIAL CHANGED INTEGER  */
#line 715 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.materialChanged=(yyvsp[0].ival);
            }
#line 2415 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 139: /* WorkStatus: REMESH NEEDED INTEGER  */
#line 719 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.remeshNeeded=(yyvsp[0].ival);
            }
#line 2423 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 140: /* WorkStatus: MODELIZATION NEEDED INTEGER  */
#line 723 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.modelizationNeeded=(yyvsp[0].ival);
            }
#line 2431 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 141: /* WorkStatus: COMPONENT SAVE NEEDED INTEGER  */
#line 727 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.componentsaveNeeded=(yyvsp[0].ival);
            }
#line 2439 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 142: /* WorkStatus: REDUCTION NEEDED INTEGER  */
#line 731 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.cmpReductionNeeded=(yyvsp[0].ival);
              prjData.workStatus.netReductionNeeded=(yyvsp[0].ival);
            }
#line 2448 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 143: /* WorkStatus: COMPONENT REDUCTION NEEDED INTEGER  */
#line 736 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.cmpReductionNeeded=(yyvsp[0].ival);
            }
#line 2456 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 144: /* WorkStatus: NETWORK REDUCTION NEEDED INTEGER  */
#line 740 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.netReductionNeeded=(yyvsp[0].ival);
            }
#line 2464 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 145: /* WorkStatus: RELOAD NEEDED INTEGER  */
#line 744 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.reloadNeeded=(yyvsp[0].ival);
            }
#line 2472 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;


#line 2476 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"

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

#line 748 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"



extern int projectDataLineNum;

char projectDataFileName[256];
extern void projectDataSetInputFile(FILE *fp);

void yyerror(char *s){
  printf("\n  %s (%s) at file: %s, line: %d\n\n", s, projectDatatext, projectDataFileName, projectDataLineNum);
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


void projectData_msg(int type, char *fmt, ...){
  va_list args;
  char tmp[1024];

  va_start (args, fmt);
  vsprintf (tmp, fmt, args);
  va_end (args);

  DB::Msg(type, "'%s', line %d : %s", projectDataFileName, projectDataLineNum, tmp);

}


int loadPrjData(const char *fName) 
{
        FILE *fid;
        if(!(fid = fopen(fName, "r"))){
		DB::Msg(ERROR, "Cannot open file %s\n", fName);
		return 1;
	}
        strncpy(projectDataFileName, fName, 255);
        projectDataSetInputFile(fid);
        int result=yyparse();
        fclose(fid);
//        if(prjData.refFreqBand[0]<1.e-10) prjData.refFreqBand[0]=prjData.freqBand[0];
	return result;
}




