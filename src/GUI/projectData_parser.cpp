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
  YYSYMBOL_FREQ = 13,                      /* FREQ  */
  YYSYMBOL_UNIT = 14,                      /* UNIT  */
  YYSYMBOL_EXP = 15,                       /* EXP  */
  YYSYMBOL_MESH = 16,                      /* MESH  */
  YYSYMBOL_CONFORMAL = 17,                 /* CONFORMAL  */
  YYSYMBOL_TET = 18,                       /* TET  */
  YYSYMBOL_SHARED = 19,                    /* SHARED  */
  YYSYMBOL_REFINE = 20,                    /* REFINE  */
  YYSYMBOL_RESONANCE = 21,                 /* RESONANCE  */
  YYSYMBOL_ENERGY = 22,                    /* ENERGY  */
  YYSYMBOL_WAVELENGTH = 23,                /* WAVELENGTH  */
  YYSYMBOL_LOCAL = 24,                     /* LOCAL  */
  YYSYMBOL_MESHING3D = 25,                 /* MESHING3D  */
  YYSYMBOL_ANA = 26,                       /* ANA  */
  YYSYMBOL_BAND = 27,                      /* BAND  */
  YYSYMBOL_NUM = 28,                       /* NUM  */
  YYSYMBOL_MOR = 29,                       /* MOR  */
  YYSYMBOL_RESPONSE = 30,                  /* RESPONSE  */
  YYSYMBOL_SYMMETRIC = 31,                 /* SYMMETRIC  */
  YYSYMBOL_PARAMETER = 32,                 /* PARAMETER  */
  YYSYMBOL_TYPE = 33,                      /* TYPE  */
  YYSYMBOL_TOPOLOGY = 34,                  /* TOPOLOGY  */
  YYSYMBOL_PART = 35,                      /* PART  */
  YYSYMBOL_XSCALE = 36,                    /* XSCALE  */
  YYSYMBOL_YSCALE = 37,                    /* YSCALE  */
  YYSYMBOL_AUTO = 38,                      /* AUTO  */
  YYSYMBOL_ZERO = 39,                      /* ZERO  */
  YYSYMBOL_POLE = 40,                      /* POLE  */
  YYSYMBOL_WINDOW = 41,                    /* WINDOW  */
  YYSYMBOL_CURVE = 42,                     /* CURVE  */
  YYSYMBOL_CIRCLE = 43,                    /* CIRCLE  */
  YYSYMBOL_XYPLANE = 44,                   /* XYPLANE  */
  YYSYMBOL_YZPLANE = 45,                   /* YZPLANE  */
  YYSYMBOL_ZXPLANE = 46,                   /* ZXPLANE  */
  YYSYMBOL_SYMMETRY = 47,                  /* SYMMETRY  */
  YYSYMBOL_IDEAL = 48,                     /* IDEAL  */
  YYSYMBOL_CANONICAL = 49,                 /* CANONICAL  */
  YYSYMBOL_FILTER = 50,                    /* FILTER  */
  YYSYMBOL_CUSTOM = 51,                    /* CUSTOM  */
  YYSYMBOL_PASS = 52,                      /* PASS  */
  YYSYMBOL_STOP = 53,                      /* STOP  */
  YYSYMBOL_KRYLOV = 54,                    /* KRYLOV  */
  YYSYMBOL_ORDER = 55,                     /* ORDER  */
  YYSYMBOL_RETURNLOSS = 56,                /* RETURNLOSS  */
  YYSYMBOL_OUTBAND = 57,                   /* OUTBAND  */
  YYSYMBOL_MAPPING = 58,                   /* MAPPING  */
  YYSYMBOL_TUNING = 59,                    /* TUNING  */
  YYSYMBOL_METHOD = 60,                    /* METHOD  */
  YYSYMBOL_SOURCE = 61,                    /* SOURCE  */
  YYSYMBOL_ITERMAX = 62,                   /* ITERMAX  */
  YYSYMBOL_AUTOMATIC = 63,                 /* AUTOMATIC  */
  YYSYMBOL_DESIGN = 64,                    /* DESIGN  */
  YYSYMBOL_WITH = 65,                      /* WITH  */
  YYSYMBOL_MAPPED = 66,                    /* MAPPED  */
  YYSYMBOL_TX = 67,                        /* TX  */
  YYSYMBOL_ZEROS = 68,                     /* ZEROS  */
  YYSYMBOL_SPICE = 69,                     /* SPICE  */
  YYSYMBOL_SET = 70,                       /* SET  */
  YYSYMBOL_CENTRAL = 71,                   /* CENTRAL  */
  YYSYMBOL_CONFIG = 72,                    /* CONFIG  */
  YYSYMBOL_ADD = 73,                       /* ADD  */
  YYSYMBOL_CONJUGATE = 74,                 /* CONJUGATE  */
  YYSYMBOL_PORT = 75,                      /* PORT  */
  YYSYMBOL_IMPEDANCE = 76,                 /* IMPEDANCE  */
  YYSYMBOL_TARGET = 77,                    /* TARGET  */
  YYSYMBOL_QFACTOR = 78,                   /* QFACTOR  */
  YYSYMBOL_UNIFORM = 79,                   /* UNIFORM  */
  YYSYMBOL_INDUCTIVE = 80,                 /* INDUCTIVE  */
  YYSYMBOL_SKIN = 81,                      /* SKIN  */
  YYSYMBOL_LOSSY = 82,                     /* LOSSY  */
  YYSYMBOL_PREDISTORTED = 83,              /* PREDISTORTED  */
  YYSYMBOL_OPTIMIZE = 84,                  /* OPTIMIZE  */
  YYSYMBOL_ILOSSPP = 85,                   /* ILOSSPP  */
  YYSYMBOL_TRUSTR = 86,                    /* TRUSTR  */
  YYSYMBOL_CUTOFF = 87,                    /* CUTOFF  */
  YYSYMBOL_RATIO = 88,                     /* RATIO  */
  YYSYMBOL_RECOMPUTE = 89,                 /* RECOMPUTE  */
  YYSYMBOL_JACOBIAN = 90,                  /* JACOBIAN  */
  YYSYMBOL_ERRORT = 91,                    /* ERRORT  */
  YYSYMBOL_ONLY = 92,                      /* ONLY  */
  YYSYMBOL_TRANSVERSEJ = 93,               /* TRANSVERSEJ  */
  YYSYMBOL_XTOL = 94,                      /* XTOL  */
  YYSYMBOL_GRADDX = 95,                    /* GRADDX  */
  YYSYMBOL_REMESH = 96,                    /* REMESH  */
  YYSYMBOL_FIRST = 97,                     /* FIRST  */
  YYSYMBOL_DECOMPOSITION = 98,             /* DECOMPOSITION  */
  YYSYMBOL_MATERIAL = 99,                  /* MATERIAL  */
  YYSYMBOL_MODELIZATION = 100,             /* MODELIZATION  */
  YYSYMBOL_COMPONENT = 101,                /* COMPONENT  */
  YYSYMBOL_SAVE = 102,                     /* SAVE  */
  YYSYMBOL_REDUCTION = 103,                /* REDUCTION  */
  YYSYMBOL_RELOAD = 104,                   /* RELOAD  */
  YYSYMBOL_NEEDED = 105,                   /* NEEDED  */
  YYSYMBOL_DONE = 106,                     /* DONE  */
  YYSYMBOL_CHANGED = 107,                  /* CHANGED  */
  YYSYMBOL_tMIN = 108,                     /* tMIN  */
  YYSYMBOL_tMAX = 109,                     /* tMAX  */
  YYSYMBOL_PLUSI = 110,                    /* PLUSI  */
  YYSYMBOL_MINUSI = 111,                   /* MINUSI  */
  YYSYMBOL_112_ = 112,                     /* ','  */
  YYSYMBOL_113_ = 113,                     /* '['  */
  YYSYMBOL_114_ = 114,                     /* ']'  */
  YYSYMBOL_115_ = 115,                     /* '{'  */
  YYSYMBOL_116_ = 116,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 117,                 /* $accept  */
  YYSYMBOL_All = 118,                      /* All  */
  YYSYMBOL_PrjDataItems = 119,             /* PrjDataItems  */
  YYSYMBOL_PrjDataItem = 120,              /* PrjDataItem  */
  YYSYMBOL_SFFloat = 121,                  /* SFFloat  */
  YYSYMBOL_SFComplex = 122,                /* SFComplex  */
  YYSYMBOL_SFFloatList = 123,              /* SFFloatList  */
  YYSYMBOL_MFFloat = 124,                  /* MFFloat  */
  YYSYMBOL_SFComplexList = 125,            /* SFComplexList  */
  YYSYMBOL_MFComplex = 126,                /* MFComplex  */
  YYSYMBOL_MFFloatOrMFComplex = 127,       /* MFFloatOrMFComplex  */
  YYSYMBOL_ImportData = 128,               /* ImportData  */
  YYSYMBOL_CAD = 129,                      /* CAD  */
  YYSYMBOL_Units = 130,                    /* Units  */
  YYSYMBOL_Mesh = 131,                     /* Mesh  */
  YYSYMBOL_Mwm = 132,                      /* Mwm  */
  YYSYMBOL_Mor = 133,                      /* Mor  */
  YYSYMBOL_FreqAna = 134,                  /* FreqAna  */
  YYSYMBOL_FilterDesign = 135,             /* FilterDesign  */
  YYSYMBOL_136_1 = 136,                    /* $@1  */
  YYSYMBOL_WorkStatus = 137                /* WorkStatus  */
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
#define YYLAST   373

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  117
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  142
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  379

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   366


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
       2,     2,     2,     2,   112,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   113,     2,   114,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   115,     2,   116,     2,     2,     2,     2,
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
     105,   106,   107,   108,   109,   110,   111
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
     386,   390,   397,   402,   407,   411,   416,   420,   425,   429,
     433,   437,   441,   445,   449,   453,   457,   461,   465,   469,
     472,   476,   480,   484,   487,   491,   495,   499,   503,   507,
     511,   515,   519,   522,   526,   530,   534,   540,   544,   550,
     554,   561,   565,   569,   573,   577,   581,   585,   592,   597,
     602,   606,   610,   615,   619,   623,   627,   631,   635,   639,
     639,   647,   651,   655,   659,   663,   667,   671,   675,   679,
     682,   685,   688,   694,   698,   702,   706,   710,   714,   718,
     723,   727,   731
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
  "NETWORK", "LENGTH", "FREQ", "UNIT", "EXP", "MESH", "CONFORMAL", "TET",
  "SHARED", "REFINE", "RESONANCE", "ENERGY", "WAVELENGTH", "LOCAL",
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

#define YYPACT_NINF (-211)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-3)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -211,    10,   178,  -211,  -211,    23,    27,    36,     0,     8,
      -6,    -9,    21,    33,    39,     4,    34,    48,   -11,    16,
      51,    25,    59,    63,    44,    66,   231,    50,   109,    14,
      55,    78,    94,    64,    53,    70,    65,    54,    68,     2,
      69,    71,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,
    -211,  -211,  -211,  -211,  -211,  -211,    58,   156,    72,   170,
     165,   122,    73,   -62,    93,    95,   162,   182,    19,   166,
     -21,   183,   197,   107,   188,   110,   148,   175,    60,   203,
     205,   207,    -8,   177,   153,   186,   187,   213,   144,   164,
       7,    47,   150,   145,   147,   144,   149,   181,   206,   208,
     195,   171,   184,   173,    17,   169,    46,    -2,   144,   240,
     242,   243,   245,   246,    61,   237,   146,   151,   249,   250,
     154,   244,   227,   255,  -211,   256,   264,   265,    32,   241,
     252,   257,   267,   288,   204,  -211,   144,   209,   144,   144,
     211,   266,  -211,   144,   290,   268,   144,   292,   297,   298,
     275,    41,   215,   301,  -211,  -211,  -211,   302,   304,   247,
     236,   309,   311,   144,   144,  -211,  -211,  -211,  -211,   144,
     312,   313,   314,   315,   144,   316,   317,   144,    74,   228,
     144,   144,  -211,   144,   144,  -211,   319,   320,   144,   321,
     322,   286,   324,   289,   300,   291,   260,   325,   330,   144,
     144,  -211,   144,   331,   332,   144,   144,  -211,  -211,  -211,
    -211,  -211,  -211,   229,   323,   318,   334,   336,  -211,  -211,
     253,   326,   337,  -211,  -211,  -211,  -211,   339,   144,   340,
     341,   342,  -211,  -211,   144,  -211,   344,   144,   144,   144,
     345,   144,  -211,   346,   144,  -211,  -211,  -211,   144,   347,
     144,   144,   348,  -211,  -211,   284,   285,  -211,  -211,   144,
     144,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,
     352,   353,   354,  -211,  -211,   248,  -211,  -211,  -211,  -211,
     144,  -211,  -211,   349,  -211,   355,   356,   357,   360,  -211,
    -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,   276,   338,
     362,  -211,  -211,   144,   364,  -211,  -211,   144,  -211,  -211,
    -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,
     144,  -211,   144,  -211,  -211,   365,   366,  -211,  -211,  -211,
    -211,  -211,   144,  -211,  -211,  -211,  -211,   343,  -211,  -211,
    -211,  -211,   144,   367,  -211,  -211,  -211,  -211,  -211,  -211,
    -211,  -211,     9,     1,    12,    15,   368,  -211,  -211,   144,
     144,   144,  -211,  -211,   144,  -211,    56,  -211,  -211,  -211,
    -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211
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
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,    56,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    16,    15,   113,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   119,     0,     0,   116,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   122,     0,     0,     0,     0,     0,    57,   136,   134,
     133,   135,   137,     0,     0,     0,     0,     0,   139,   142,
       0,     0,     0,   141,    40,    94,    95,     0,     0,     0,
       0,     0,    41,    44,     0,    43,     0,     0,     0,     0,
       0,     0,    66,     0,     0,    59,    61,   121,     0,     0,
       0,     0,     0,    74,    77,     0,     0,    76,    89,     0,
       0,   114,    80,    79,    81,    83,    82,    85,    84,    91,
       0,     0,     0,    90,    92,     0,   115,   117,   118,    78,
       0,   101,   102,     0,   107,     0,     0,     0,     0,   132,
     129,   130,   131,   124,   127,   123,   125,   126,     0,     0,
       0,   138,   140,     0,     0,    72,    97,     0,    47,    45,
      46,    52,    42,    49,    50,    51,    60,    62,    67,    58,
       0,    99,     0,    64,   100,     0,     0,   108,   109,    86,
      87,    88,    23,    33,    34,   120,    65,     0,   103,   104,
     106,    93,     0,     0,    70,    69,    73,    96,    63,    98,
     112,   111,     0,     0,     0,     0,     0,    68,    71,     0,
       0,     0,    24,    19,     0,    31,     0,    26,    22,    25,
      21,    29,    32,    28,   105,    17,    18,    20,    27
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -211,  -211,  -211,  -211,   -88,  -210,  -211,  -211,  -211,  -211,
    -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,
    -211
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    42,   366,   353,   354,   333,   355,   334,
     335,    43,    44,    45,    46,    47,    48,    49,    50,   275,
      51
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     168,   201,    75,    55,   166,   167,   139,   185,    60,    62,
       3,    63,   166,   167,    64,   166,   167,    70,   166,   167,
     207,    56,    59,   114,    61,   157,   158,    99,    52,    57,
     194,   115,    53,    71,    65,   166,   167,    67,   170,   136,
     228,    54,   137,    76,   166,   167,   130,   131,   235,    68,
     237,   238,    69,   100,   202,   241,   195,   159,   244,    72,
     203,    73,   101,   250,   102,   160,    77,   171,   172,   173,
     227,   120,    79,   150,   213,   259,   260,    74,   103,   249,
     104,   261,   204,   205,   206,   174,   266,   121,   140,   269,
     214,    78,   273,   274,    82,   276,   277,   151,    97,    66,
     280,   152,   153,    58,   116,   117,    80,   175,   197,   176,
      81,   291,   292,   364,   293,   365,    83,   296,   297,   359,
     360,   361,    98,   362,   368,   105,   369,   371,   106,   372,
     198,   199,   200,   177,   143,   144,   178,   146,   147,   179,
     307,   180,   181,   367,   107,   373,   311,   166,   167,   313,
     314,   315,   108,   317,   378,   126,   319,   127,   109,   128,
     320,   112,   322,   323,   270,   271,   359,   360,   110,   122,
     111,   327,   328,   113,   118,   124,   119,   123,    -2,     4,
     125,   132,   129,   133,   134,   135,     5,     6,     7,     8,
       9,    10,   336,   138,    11,    12,   141,    13,    14,    15,
     142,   145,    16,   148,    17,   149,   154,    18,   155,    19,
     156,   161,   162,   163,   164,   345,   165,    20,   182,   347,
     169,   183,    21,    22,    23,   184,    24,    25,    26,    27,
     186,   187,   348,   188,   349,   190,    28,   191,   189,   193,
     196,    29,   192,   208,   352,   209,   210,    30,   211,   212,
     215,   216,   218,   219,   357,   222,   217,   221,   223,   224,
      31,    32,    84,   220,   363,    33,   370,   225,   226,   229,
     232,   375,   376,   377,    34,    35,    36,    37,    38,    39,
     230,    40,    41,    85,    86,   231,    87,    88,    89,    90,
      91,   233,   234,   242,   240,   245,   243,   236,    92,   239,
     246,   247,   248,   251,   252,   253,    93,   254,    94,    95,
     256,    96,   257,   255,   258,   262,   263,   264,   265,   267,
     268,   272,   278,   279,   281,   282,   283,   284,   289,   285,
     286,   287,   288,   290,   294,   295,   299,   301,   298,   302,
     305,   303,   306,   308,   309,   310,   300,   312,   316,   318,
     321,   324,   325,   326,   304,   329,   330,   331,   338,   339,
     340,   332,   337,   341,   342,   344,   343,   346,   350,   351,
     358,   374,     0,   356
};

static const yytype_int16 yycheck[] =
{
      88,     3,    13,     3,     3,     4,    27,    95,    14,    18,
       0,    20,     3,     4,    23,     3,     4,    13,     3,     4,
     108,    21,    14,    21,    30,    33,    34,    13,     5,    29,
      13,    29,     5,    29,    43,     3,     4,    16,    31,    20,
     128,     5,    23,    54,     3,     4,   108,   109,   136,    16,
     138,   139,    13,    39,    56,   143,    39,    65,   146,    25,
      62,    13,    48,   151,    50,    73,    50,    60,    61,    62,
      38,    13,    47,    13,    13,   163,   164,    29,    64,    38,
      66,   169,    84,    85,    86,    78,   174,    29,   109,   177,
      29,    40,   180,   181,    50,   183,   184,    37,    48,   108,
     188,    41,    42,   103,   102,   103,    47,    60,    62,    62,
      47,   199,   200,   112,   202,   114,    50,   205,   206,   110,
     111,   112,    13,   114,   112,    70,   114,   112,    50,   114,
      84,    85,    86,    86,    27,    28,    89,    27,    28,    92,
     228,    94,    95,   353,    50,   355,   234,     3,     4,   237,
     238,   239,    88,   241,   364,    33,   244,    35,   105,    37,
     248,   107,   250,   251,    90,    91,   110,   111,    98,    13,
     105,   259,   260,   105,   105,     5,   105,   105,     0,     1,
      15,    88,   109,    88,    22,     3,     8,     9,    10,    11,
      12,    13,   280,    27,    16,    17,    13,    19,    20,    21,
       3,    13,    24,    55,    26,    30,     3,    29,     3,    31,
       3,    34,    59,    27,    27,   303,     3,    39,    68,   307,
      56,    76,    44,    45,    46,    78,    48,    49,    50,    51,
      81,    50,   320,    27,   322,    40,    58,    66,    30,    66,
      71,    63,    58,     3,   332,     3,     3,    69,     3,     3,
      13,   105,     3,     3,   342,    28,   105,    13,     3,     3,
      82,    83,    31,   109,   352,    87,   354,     3,     3,    28,
       3,   359,   360,   361,    96,    97,    98,    99,   100,   101,
      28,   103,   104,    52,    53,    28,    55,    56,    57,    58,
      59,     3,    88,     3,    28,     3,    28,    88,    67,    88,
       3,     3,    27,    88,     3,     3,    75,     3,    77,    78,
      74,    80,     3,    66,     3,     3,     3,     3,     3,     3,
       3,    93,     3,     3,     3,     3,    40,     3,     3,    40,
      30,    40,    72,     3,     3,     3,    13,     3,   109,     3,
       3,    88,     3,     3,     3,     3,    28,     3,     3,     3,
       3,     3,    68,    68,    28,     3,     3,     3,     3,     3,
       3,   113,    13,     3,    88,     3,    28,     3,     3,     3,
       3,     3,    -1,    30
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   118,   119,     0,     1,     8,     9,    10,    11,    12,
      13,    16,    17,    19,    20,    21,    24,    26,    29,    31,
      39,    44,    45,    46,    48,    49,    50,    51,    58,    63,
      69,    82,    83,    87,    96,    97,    98,    99,   100,   101,
     103,   104,   120,   128,   129,   130,   131,   132,   133,   134,
     135,   137,     5,     5,     5,     3,    21,    29,   103,    14,
      14,    30,    18,    20,    23,    43,   108,    16,    16,    13,
      13,    29,    25,    13,    29,    13,    54,    50,    40,    47,
      47,    47,    50,    50,    31,    52,    53,    55,    56,    57,
      58,    59,    67,    75,    77,    78,    80,    48,    13,    13,
      39,    48,    50,    64,    66,    70,    50,    50,    88,   105,
      98,   105,   107,   105,    21,    29,   102,   103,   105,   105,
      13,    29,    13,   105,     5,    15,    33,    35,    37,   109,
     108,   109,    88,    88,    22,     3,    20,    23,    27,    27,
     109,    13,     3,    27,    28,    13,    27,    28,    55,    30,
      13,    37,    41,    42,     3,     3,     3,    33,    34,    65,
      73,    34,    59,    27,    27,     3,     3,     4,   121,    56,
      31,    60,    61,    62,    78,    60,    62,    86,    89,    92,
      94,    95,    68,    76,    78,   121,    81,    50,    27,    30,
      40,    66,    58,    66,    13,    39,    71,    62,    84,    85,
      86,     3,    56,    62,    84,    85,    86,   121,     3,     3,
       3,     3,     3,    13,    29,    13,   105,   105,     3,     3,
     109,    13,    28,     3,     3,     3,     3,    38,   121,    28,
      28,    28,     3,     3,    88,   121,    88,   121,   121,    88,
      28,   121,     3,    28,   121,     3,     3,     3,    27,    38,
     121,    88,     3,     3,     3,    66,    74,     3,     3,   121,
     121,   121,     3,     3,     3,     3,   121,     3,     3,   121,
      90,    91,    93,   121,   121,   136,   121,   121,     3,     3,
     121,     3,     3,    40,     3,    40,    30,    40,    72,     3,
       3,   121,   121,   121,     3,     3,   121,   121,   109,    13,
      28,     3,     3,    88,    28,     3,     3,   121,     3,     3,
       3,   121,     3,   121,   121,   121,     3,   121,     3,   121,
     121,     3,   121,   121,     3,    68,    68,   121,   121,     3,
       3,     3,   113,   124,   126,   127,   121,    13,     3,     3,
       3,     3,    88,    28,     3,   121,     3,   121,   121,   121,
       3,     3,   121,   122,   123,   125,    30,   121,     3,   110,
     111,   112,   114,   121,   112,   114,   121,   122,   112,   114,
     121,   112,   114,   122,     3,   121,   121,   121,   122
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   117,   118,   119,   119,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   121,   121,   122,   122,   123,
     123,   123,   123,   123,   124,   124,   125,   125,   125,   125,
     125,   126,   126,   127,   127,   128,   128,   129,   129,   130,
     130,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   132,   133,   133,
     133,   133,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   136,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137
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
       5,     4,     5,     6,     5,     5,     4,     5,     6,     6,
       5,     6,     5,     6,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     5,     5,     5,     4,
       4,     4,     4,     5,     4,     4,     6,     5,     6,     5,
       5,     4,     4,     5,     5,     7,     5,     4,     5,     5,
       3,     6,     6,     3,     4,     4,     3,     4,     4,     0,
       5,     4,     3,     4,     4,     4,     4,     4,     3,     4,
       4,     4,     4,     3,     3,     3,     3,     3,     4,     3,
       4,     4,     3
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
#line 1496 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 15: /* SFFloat: FLOATING  */
#line 141 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                {
		      (yyval.fval) = (yyvsp[0].fval);
		      if(ProjData::Fbuff) ProjData::Fbuff->push_back((yyvsp[0].fval));
		}
#line 1505 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 16: /* SFFloat: INTEGER  */
#line 146 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                {
		       (yyval.fval) = (double)(yyvsp[0].ival);
		       if(ProjData::Fbuff) ProjData::Fbuff->push_back((double)(yyvsp[0].ival));
		}
#line 1514 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 17: /* SFComplex: SFFloat PLUSI SFFloat  */
#line 155 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                {
		       if(ProjData::Cbuff) ProjData::Cbuff->push_back(std::complex<double>((yyvsp[-2].fval),(yyvsp[0].fval)));
		}
#line 1522 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 18: /* SFComplex: SFFloat MINUSI SFFloat  */
#line 159 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                {
		       if(ProjData::Cbuff) ProjData::Cbuff->push_back(std::complex<double>((yyvsp[-2].fval),-(yyvsp[0].fval)));
		}
#line 1530 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 24: /* MFFloat: '[' SFFloat ']'  */
#line 197 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                          {}
#line 1536 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 25: /* MFFloat: '[' SFFloatList ']'  */
#line 198 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                              {}
#line 1542 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 31: /* MFComplex: '[' SFComplex ']'  */
#line 211 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                            {}
#line 1548 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 32: /* MFComplex: '[' SFComplexList ']'  */
#line 212 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                                {}
#line 1554 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 33: /* MFFloatOrMFComplex: MFFloat  */
#line 217 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                  {}
#line 1560 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 34: /* MFFloatOrMFComplex: MFComplex  */
#line 218 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                    {}
#line 1566 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 35: /* ImportData: VARFILEPATH STRING  */
#line 273 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.varFilePath=(yyvsp[0].sval);
            }
#line 1574 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 36: /* ImportData: S2PFILEPATH STRING  */
#line 277 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.s2pFilePath=(yyvsp[0].sval);
            }
#line 1582 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 37: /* CAD: MAINASSNAME STRING  */
#line 284 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.mainAssName=(yyvsp[0].sval);
            }
#line 1590 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 38: /* CAD: NETWORK INTEGER  */
#line 288 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.network=(yyvsp[0].ival);
            }
#line 1598 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 39: /* Units: LENGTH UNIT STRING  */
#line 295 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              strcpy(prjData.lengthUnitName,(yyvsp[0].sval));
            }
#line 1606 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 40: /* Units: FREQ UNIT EXP INTEGER  */
#line 299 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqUnitE =(yyvsp[0].ival);
            }
#line 1614 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 41: /* Mesh: MESH WAVELENGTH RATIO INTEGER  */
#line 306 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshPerWavelen=(yyvsp[0].ival);
            }
#line 1622 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 42: /* Mesh: SHARED MESH WAVELENGTH RATIO INTEGER  */
#line 310 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.sharedMeshPerWavelen=(yyvsp[0].ival);
            }
#line 1630 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 43: /* Mesh: SHARED MESH REFINE SFFloat  */
#line 314 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.sharedMeshRefine=(yyvsp[0].fval);
            }
#line 1638 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 44: /* Mesh: MESH CIRCLE RATIO INTEGER  */
#line 318 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshPerCircle=(yyvsp[0].ival);
            }
#line 1646 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 45: /* Mesh: MESH REFINE tMIN NUM INTEGER  */
#line 322 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshRefineMinNum=(yyvsp[0].ival);
            }
#line 1654 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 46: /* Mesh: MESH REFINE tMAX NUM INTEGER  */
#line 326 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshRefineMaxNum=(yyvsp[0].ival);
            }
#line 1662 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 47: /* Mesh: MESH TET tMAX NUM INTEGER  */
#line 330 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshTetMaxNum=(yyvsp[0].ival);
            }
#line 1670 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 48: /* Mesh: CONFORMAL MESH INTEGER  */
#line 334 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.conformalMeshIF=(yyvsp[0].ival);
            }
#line 1678 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 49: /* Mesh: REFINE FREQ BAND SFFloat SFFloat  */
#line 338 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 1685 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 50: /* Mesh: RESONANCE FREQ BAND SFFloat SFFloat  */
#line 341 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 1692 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 51: /* Mesh: RESONANCE FREQ tMAX RATIO SFFloat  */
#line 344 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.resonFreqMaxRatio=(yyvsp[0].fval);
            }
#line 1700 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 52: /* Mesh: MESH tMIN ENERGY RATIO SFFloat  */
#line 348 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshMinEnergyRatio=(yyvsp[0].fval);
            }
#line 1708 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 53: /* Mesh: LOCAL MESHING3D INTEGER  */
#line 352 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.localMeshing3d=(yyvsp[0].ival);
            }
#line 1716 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 54: /* Mesh: XYPLANE SYMMETRY INTEGER  */
#line 356 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.XYplaneSymmetry=(yyvsp[0].ival);
            }
#line 1724 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 55: /* Mesh: YZPLANE SYMMETRY INTEGER  */
#line 360 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.YZplaneSymmetry=(yyvsp[0].ival);
            }
#line 1732 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 56: /* Mesh: ZXPLANE SYMMETRY INTEGER  */
#line 364 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.ZXplaneSymmetry=(yyvsp[0].ival);
            }
#line 1740 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 57: /* Mwm: CUTOFF RATIO SFFloat  */
#line 371 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
           {
              prjData.cutoffRatio=(yyvsp[0].fval);
           }
#line 1748 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 58: /* Mor: MOR FREQ BAND SFFloat SFFloat  */
#line 378 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqBand[0]=(yyvsp[-1].fval);
              prjData.freqBand[1]=(yyvsp[0].fval);
            }
#line 1757 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 59: /* Mor: MOR FREQ NUM INTEGER  */
#line 383 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.MORFreqNum=(yyvsp[0].ival);
            }
#line 1765 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 60: /* Mor: RESONANCE MOR FREQ NUM INTEGER  */
#line 387 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.MORFreqNum1=(yyvsp[0].ival);
            }
#line 1773 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 61: /* Mor: MOR KRYLOV ORDER INTEGER  */
#line 391 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.KrylovOrder=(yyvsp[0].ival);
            }
#line 1781 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 62: /* FreqAna: ANA FREQ BAND SFFloat SFFloat  */
#line 398 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.anaFreqBand[0]=(yyvsp[-1].fval);
              prjData.anaFreqBand[1]=(yyvsp[0].fval);
            }
#line 1790 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 63: /* FreqAna: ZERO POLE FREQ BAND SFFloat SFFloat  */
#line 403 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.zpFreqBand[0]=(yyvsp[-1].fval);
              prjData.zpFreqBand[1]=(yyvsp[0].fval);
            }
#line 1799 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 64: /* FreqAna: ZERO POLE WINDOW RATIO SFFloat  */
#line 408 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.zpWinRatio=(yyvsp[0].fval);
            }
#line 1807 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 65: /* FreqAna: MAPPING FREQ BAND SFFloat SFFloat  */
#line 412 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.mapFreqBand[0]=(yyvsp[-1].fval);
              prjData.mapFreqBand[1]=(yyvsp[0].fval);
            }
#line 1816 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 66: /* FreqAna: ANA FREQ NUM INTEGER  */
#line 417 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.anaFreqNum=(yyvsp[0].ival);
            }
#line 1824 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 67: /* FreqAna: ANA MOR FREQ NUM INTEGER  */
#line 421 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              if(prjData.network) prjData.netMORFreqNum=(yyvsp[0].ival);
              else                prjData.cmpMORFreqNum=(yyvsp[0].ival);
            }
#line 1833 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 68: /* FreqAna: COMPONENT RESONANCE FREQ tMAX RATIO SFFloat  */
#line 426 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.cmpResonFreqMaxRatio=(yyvsp[0].fval);
            }
#line 1841 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 69: /* FreqAna: NETWORK RESONANCE FREQ tMAX RATIO SFFloat  */
#line 430 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.netResonFreqMaxRatio=(yyvsp[0].fval);
            }
#line 1849 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 70: /* FreqAna: COMPONENT MOR FREQ NUM INTEGER  */
#line 434 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.cmpMORFreqNum=(yyvsp[0].ival);
            }
#line 1857 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 71: /* FreqAna: COMPONENT RESONANCE MOR FREQ NUM INTEGER  */
#line 438 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.cmpMORFreqNum1=(yyvsp[0].ival);
            }
#line 1865 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 72: /* FreqAna: NETWORK MOR FREQ NUM INTEGER  */
#line 442 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.netMORFreqNum=(yyvsp[0].ival);
            }
#line 1873 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 73: /* FreqAna: NETWORK RESONANCE MOR FREQ NUM INTEGER  */
#line 446 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.netMORFreqNum1=(yyvsp[0].ival);
            }
#line 1881 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 74: /* FreqAna: IDEAL FILTER TYPE INTEGER  */
#line 450 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.idealFilterType=(yyvsp[0].ival);
            }
#line 1889 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 75: /* FreqAna: IDEAL FILTER TYPE INTEGER  */
#line 454 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.idealFilterType=(yyvsp[0].ival);
            }
#line 1897 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 76: /* FreqAna: CANONICAL FILTER TOPOLOGY INTEGER  */
#line 458 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.canonicalFilterTopology=(yyvsp[0].ival);
            }
#line 1905 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 77: /* FreqAna: IDEAL FILTER TOPOLOGY INTEGER  */
#line 462 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.canonicalFilterTopology=(yyvsp[0].ival);
            }
#line 1913 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 78: /* FreqAna: CUSTOM IDEAL FILTER INTEGER  */
#line 466 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.customIdealFilter=(yyvsp[0].ival);
            }
#line 1921 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 79: /* FreqAna: FILTER MAPPING METHOD INTEGER  */
#line 470 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 1928 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 80: /* FreqAna: FILTER MAPPING SYMMETRIC INTEGER  */
#line 473 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filtermapSymmetric=(yyvsp[0].ival);
            }
#line 1936 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 81: /* FreqAna: FILTER MAPPING SOURCE INTEGER  */
#line 477 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filtermapSource=(yyvsp[0].ival);
            }
#line 1944 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 82: /* FreqAna: FILTER MAPPING QFACTOR SFFloat  */
#line 481 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filtermapQfactor = (yyvsp[0].fval);
            }
#line 1952 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 83: /* FreqAna: FILTER MAPPING ITERMAX INTEGER  */
#line 485 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 1959 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 84: /* FreqAna: FILTER TUNING ITERMAX INTEGER  */
#line 488 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneItermax=(yyvsp[0].ival);
            }
#line 1967 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 85: /* FreqAna: FILTER TUNING METHOD INTEGER  */
#line 492 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneMethod=(yyvsp[0].ival);
            }
#line 1975 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 86: /* FreqAna: FILTER TUNING RECOMPUTE JACOBIAN INTEGER  */
#line 496 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneRecomputeJaco=(yyvsp[0].ival);
            }
#line 1983 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 87: /* FreqAna: FILTER TUNING RECOMPUTE ERRORT INTEGER  */
#line 500 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneRecomputeError=(yyvsp[0].ival);
            }
#line 1991 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 88: /* FreqAna: FILTER TUNING ONLY TRANSVERSEJ INTEGER  */
#line 504 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneOnlyJt=(yyvsp[0].ival);
            }
#line 1999 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 89: /* FreqAna: FILTER SYMMETRIC TUNING INTEGER  */
#line 508 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterSymmetricTuning=(yyvsp[0].ival);
            }
#line 2007 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 90: /* FreqAna: FILTER TUNING XTOL SFFloat  */
#line 512 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneXtol=(yyvsp[0].fval);
            }
#line 2015 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 91: /* FreqAna: FILTER TUNING TRUSTR SFFloat  */
#line 516 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneTrustR=(yyvsp[0].fval);
            }
#line 2023 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 92: /* FreqAna: FILTER TUNING GRADDX SFFloat  */
#line 520 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 2030 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 93: /* FreqAna: SPICE SET CENTRAL CONFIG INTEGER  */
#line 523 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.spiceSetCentralConfig=(yyvsp[0].ival);
            }
#line 2038 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 94: /* FreqAna: FREQ RESPONSE TYPE INTEGER  */
#line 527 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqRespParType = (FreqRespParType) (yyvsp[0].ival);
            }
#line 2046 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 95: /* FreqAna: FREQ RESPONSE PART INTEGER  */
#line 531 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqRespParPart = (yyvsp[0].ival);
            }
#line 2054 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 96: /* FreqAna: FREQ RESPONSE YSCALE SFFloat SFFloat SFFloat  */
#line 535 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqRespYscale[0]=(yyvsp[-2].fval);
              prjData.freqRespYscale[1]=(yyvsp[-1].fval);
              prjData.freqRespYscale[2]=(yyvsp[0].fval);
            }
#line 2064 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 97: /* FreqAna: FREQ RESPONSE YSCALE AUTO INTEGER  */
#line 541 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqRespYscaleAuto=(yyvsp[0].ival);
            }
#line 2072 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 98: /* FreqAna: ZERO POLE YSCALE SFFloat SFFloat SFFloat  */
#line 545 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.zeropoleYscale[0]=(yyvsp[-2].fval);
              prjData.zeropoleYscale[1]=(yyvsp[-1].fval);
              prjData.zeropoleYscale[2]=(yyvsp[0].fval);
            }
#line 2082 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 99: /* FreqAna: ZERO POLE YSCALE AUTO INTEGER  */
#line 551 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.zeropoleYscaleAuto=(yyvsp[0].ival);
            }
#line 2090 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 100: /* FreqAna: ZERO POLE CURVE INTEGER INTEGER  */
#line 555 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              std::pair<int, int> Sij_ports;
              Sij_ports.first=(yyvsp[-1].ival);
              Sij_ports.second=(yyvsp[0].ival);
              prjData.zeropoleCurves.insert(Sij_ports);
            }
#line 2101 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 101: /* FreqAna: AUTOMATIC FREQ RESPONSE INTEGER  */
#line 562 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoFreqResponse=(yyvsp[0].ival);
            }
#line 2109 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 102: /* FreqAna: AUTOMATIC ZERO POLE INTEGER  */
#line 566 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoZeropole=(yyvsp[0].ival);
            }
#line 2117 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 103: /* FreqAna: AUTOMATIC DESIGN MAPPED POLE INTEGER  */
#line 570 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoDesignWithMappedTz=(yyvsp[0].ival);
            }
#line 2125 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 104: /* FreqAna: AUTOMATIC MAPPED FREQ RESPONSE INTEGER  */
#line 574 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoMappedFreqResponse=(yyvsp[0].ival);
            }
#line 2133 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 105: /* FreqAna: AUTOMATIC IDEAL MAPPED POLE FREQ RESPONSE INTEGER  */
#line 578 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoIdealMappedTzFreqResponse=(yyvsp[0].ival);
            }
#line 2141 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 106: /* FreqAna: AUTOMATIC MAPPED ZERO POLE INTEGER  */
#line 582 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoMappedZeropole=(yyvsp[0].ival);
            }
#line 2149 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 107: /* FreqAna: AUTOMATIC FILTER MAPPING INTEGER  */
#line 586 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoFilterMapping=(yyvsp[0].ival);
            }
#line 2157 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 108: /* FilterDesign: FILTER PASS BAND SFFloat SFFloat  */
#line 593 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterPassBand[0]=(yyvsp[-1].fval);
              prjData.filterPassBand[1]=(yyvsp[0].fval);
            }
#line 2166 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 109: /* FilterDesign: FILTER STOP BAND SFFloat SFFloat  */
#line 598 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterStopBand[0]=(yyvsp[-1].fval);
              prjData.filterStopBand[1]=(yyvsp[0].fval);
            }
#line 2175 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 110: /* FilterDesign: FILTER ORDER INTEGER  */
#line 603 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterOrder = (yyvsp[0].ival);
            }
#line 2183 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 111: /* FilterDesign: IDEAL FILTER ADD CONJUGATE ZEROS INTEGER  */
#line 607 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.idealFilterAddConjugateTZ = (yyvsp[0].ival);
            }
#line 2191 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 112: /* FilterDesign: IDEAL FILTER WITH MAPPED ZEROS INTEGER  */
#line 611 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.idealFilterWithMappedTZ = (yyvsp[0].ival);
              if(prjData.idealFilterWithMappedTZ) prjData.idealFilterAddConjugateTZ=0;
            }
#line 2200 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 113: /* FilterDesign: FILTER RETURNLOSS SFFloat  */
#line 616 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterRetLoss = (yyvsp[0].fval);
            }
#line 2208 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 114: /* FilterDesign: FILTER OUTBAND RETURNLOSS SFFloat  */
#line 620 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterOutbandRetLoss = (yyvsp[0].fval);
            }
#line 2216 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 115: /* FilterDesign: FILTER PORT IMPEDANCE SFFloat  */
#line 624 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterPortImpedance = (yyvsp[0].fval);
            }
#line 2224 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 116: /* FilterDesign: FILTER QFACTOR SFFloat  */
#line 628 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterQfactor = (yyvsp[0].fval);
            }
#line 2232 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 117: /* FilterDesign: FILTER TARGET QFACTOR SFFloat  */
#line 632 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTargetQfactor = (yyvsp[0].fval);
            }
#line 2240 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 118: /* FilterDesign: FILTER INDUCTIVE SKIN INTEGER  */
#line 636 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterInductiveSkin = (yyvsp[0].ival);
            }
#line 2248 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 119: /* $@1: %empty  */
#line 639 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                           {ProjData::Cbuff =&prjData.filterZeros; ProjData::Cbuff->clear();
                            ProjData::Fbuff =&ProjData::Fvec; ProjData::Fvec.clear();}
#line 2255 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 120: /* FilterDesign: FILTER TX ZEROS $@1 MFFloatOrMFComplex  */
#line 641 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              if(prjData.filterZeros.size()==0){
               for (int i=0; i<ProjData::Fvec.size() ; i++) prjData.filterZeros.push_back(std::complex<double>(ProjData::Fvec[i],0.0));
               ProjData::Fbuff=NULL;
              } else ProjData::Cbuff=NULL;
            }
#line 2266 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 121: /* FilterDesign: SYMMETRIC FILTER RESPONSE INTEGER  */
#line 648 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.symmFilterResponse = (yyvsp[0].ival);
            }
#line 2274 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 122: /* FilterDesign: PREDISTORTED FILTER INTEGER  */
#line 652 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistortedFilter = (yyvsp[0].ival);
            }
#line 2282 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 123: /* FilterDesign: PREDISTORTED FILTER OPTIMIZE INTEGER  */
#line 656 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistFilterOptim = (yyvsp[0].ival);
            }
#line 2290 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 124: /* FilterDesign: PREDISTORTED FILTER RETURNLOSS SFFloat  */
#line 660 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimRL = (yyvsp[0].fval);
            }
#line 2298 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 125: /* FilterDesign: PREDISTORTED FILTER ILOSSPP SFFloat  */
#line 664 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimILpp = (yyvsp[0].fval);
            }
#line 2306 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 126: /* FilterDesign: PREDISTORTED FILTER TRUSTR SFFloat  */
#line 668 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimTrustR = (yyvsp[0].fval);
            }
#line 2314 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 127: /* FilterDesign: PREDISTORTED FILTER ITERMAX INTEGER  */
#line 672 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimIterMax = (yyvsp[0].ival);
            }
#line 2322 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 128: /* FilterDesign: PREDISTORTED FILTER INTEGER  */
#line 676 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistortedFilter = (yyvsp[0].ival);
            }
#line 2330 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 129: /* FilterDesign: LOSSY FILTER OPTIMIZE INTEGER  */
#line 680 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 2337 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 130: /* FilterDesign: LOSSY FILTER ILOSSPP SFFloat  */
#line 683 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 2344 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 131: /* FilterDesign: LOSSY FILTER TRUSTR SFFloat  */
#line 686 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 2351 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 132: /* FilterDesign: LOSSY FILTER ITERMAX INTEGER  */
#line 689 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 2358 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 133: /* WorkStatus: DECOMPOSITION NEEDED INTEGER  */
#line 695 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.decompositionNeeded=(yyvsp[0].ival);
            }
#line 2366 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 134: /* WorkStatus: FIRST DECOMPOSITION INTEGER  */
#line 699 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.firstDecomposition=(yyvsp[0].ival);
            }
#line 2374 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 135: /* WorkStatus: MATERIAL CHANGED INTEGER  */
#line 703 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.materialChanged=(yyvsp[0].ival);
            }
#line 2382 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 136: /* WorkStatus: REMESH NEEDED INTEGER  */
#line 707 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.remeshNeeded=(yyvsp[0].ival);
            }
#line 2390 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 137: /* WorkStatus: MODELIZATION NEEDED INTEGER  */
#line 711 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.modelizationNeeded=(yyvsp[0].ival);
            }
#line 2398 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 138: /* WorkStatus: COMPONENT SAVE NEEDED INTEGER  */
#line 715 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.componentsaveNeeded=(yyvsp[0].ival);
            }
#line 2406 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 139: /* WorkStatus: REDUCTION NEEDED INTEGER  */
#line 719 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.cmpReductionNeeded=(yyvsp[0].ival);
              prjData.workStatus.netReductionNeeded=(yyvsp[0].ival);
            }
#line 2415 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 140: /* WorkStatus: COMPONENT REDUCTION NEEDED INTEGER  */
#line 724 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.cmpReductionNeeded=(yyvsp[0].ival);
            }
#line 2423 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 141: /* WorkStatus: NETWORK REDUCTION NEEDED INTEGER  */
#line 728 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.netReductionNeeded=(yyvsp[0].ival);
            }
#line 2431 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 142: /* WorkStatus: RELOAD NEEDED INTEGER  */
#line 732 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.reloadNeeded=(yyvsp[0].ival);
            }
#line 2439 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;


#line 2443 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"

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

#line 736 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"



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




