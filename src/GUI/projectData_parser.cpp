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
#line 22 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"

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


#line 125 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"

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
  YYSYMBOL_TET = 17,                       /* TET  */
  YYSYMBOL_SHARED = 18,                    /* SHARED  */
  YYSYMBOL_REFINE = 19,                    /* REFINE  */
  YYSYMBOL_RESONANCE = 20,                 /* RESONANCE  */
  YYSYMBOL_ENERGY = 21,                    /* ENERGY  */
  YYSYMBOL_WAVELENGTH = 22,                /* WAVELENGTH  */
  YYSYMBOL_LOCAL = 23,                     /* LOCAL  */
  YYSYMBOL_MESHING3D = 24,                 /* MESHING3D  */
  YYSYMBOL_ANA = 25,                       /* ANA  */
  YYSYMBOL_BAND = 26,                      /* BAND  */
  YYSYMBOL_NUM = 27,                       /* NUM  */
  YYSYMBOL_MOR = 28,                       /* MOR  */
  YYSYMBOL_RESPONSE = 29,                  /* RESPONSE  */
  YYSYMBOL_SYMMETRIC = 30,                 /* SYMMETRIC  */
  YYSYMBOL_PARAMETER = 31,                 /* PARAMETER  */
  YYSYMBOL_TYPE = 32,                      /* TYPE  */
  YYSYMBOL_TOPOLOGY = 33,                  /* TOPOLOGY  */
  YYSYMBOL_PART = 34,                      /* PART  */
  YYSYMBOL_XSCALE = 35,                    /* XSCALE  */
  YYSYMBOL_YSCALE = 36,                    /* YSCALE  */
  YYSYMBOL_AUTO = 37,                      /* AUTO  */
  YYSYMBOL_ZERO = 38,                      /* ZERO  */
  YYSYMBOL_POLE = 39,                      /* POLE  */
  YYSYMBOL_WINDOW = 40,                    /* WINDOW  */
  YYSYMBOL_CURVE = 41,                     /* CURVE  */
  YYSYMBOL_CIRCLE = 42,                    /* CIRCLE  */
  YYSYMBOL_XYPLANE = 43,                   /* XYPLANE  */
  YYSYMBOL_YZPLANE = 44,                   /* YZPLANE  */
  YYSYMBOL_ZXPLANE = 45,                   /* ZXPLANE  */
  YYSYMBOL_SYMMETRY = 46,                  /* SYMMETRY  */
  YYSYMBOL_IDEAL = 47,                     /* IDEAL  */
  YYSYMBOL_CANONICAL = 48,                 /* CANONICAL  */
  YYSYMBOL_FILTER = 49,                    /* FILTER  */
  YYSYMBOL_CUSTOM = 50,                    /* CUSTOM  */
  YYSYMBOL_PASS = 51,                      /* PASS  */
  YYSYMBOL_KRYLOV = 52,                    /* KRYLOV  */
  YYSYMBOL_ORDER = 53,                     /* ORDER  */
  YYSYMBOL_RETURNLOSS = 54,                /* RETURNLOSS  */
  YYSYMBOL_OUTBAND = 55,                   /* OUTBAND  */
  YYSYMBOL_MAPPING = 56,                   /* MAPPING  */
  YYSYMBOL_TUNING = 57,                    /* TUNING  */
  YYSYMBOL_METHOD = 58,                    /* METHOD  */
  YYSYMBOL_SOURCE = 59,                    /* SOURCE  */
  YYSYMBOL_ITERMAX = 60,                   /* ITERMAX  */
  YYSYMBOL_AUTOMATIC = 61,                 /* AUTOMATIC  */
  YYSYMBOL_DESIGN = 62,                    /* DESIGN  */
  YYSYMBOL_WITH = 63,                      /* WITH  */
  YYSYMBOL_MAPPED = 64,                    /* MAPPED  */
  YYSYMBOL_TX = 65,                        /* TX  */
  YYSYMBOL_ZEROS = 66,                     /* ZEROS  */
  YYSYMBOL_SPICE = 67,                     /* SPICE  */
  YYSYMBOL_SET = 68,                       /* SET  */
  YYSYMBOL_CENTRAL = 69,                   /* CENTRAL  */
  YYSYMBOL_CONFIG = 70,                    /* CONFIG  */
  YYSYMBOL_ADD = 71,                       /* ADD  */
  YYSYMBOL_CONJUGATE = 72,                 /* CONJUGATE  */
  YYSYMBOL_PORT = 73,                      /* PORT  */
  YYSYMBOL_IMPEDANCE = 74,                 /* IMPEDANCE  */
  YYSYMBOL_TARGET = 75,                    /* TARGET  */
  YYSYMBOL_QFACTOR = 76,                   /* QFACTOR  */
  YYSYMBOL_UNIFORM = 77,                   /* UNIFORM  */
  YYSYMBOL_INDUCTIVE = 78,                 /* INDUCTIVE  */
  YYSYMBOL_SKIN = 79,                      /* SKIN  */
  YYSYMBOL_LOSSY = 80,                     /* LOSSY  */
  YYSYMBOL_PREDISTORTED = 81,              /* PREDISTORTED  */
  YYSYMBOL_OPTIMIZE = 82,                  /* OPTIMIZE  */
  YYSYMBOL_ILOSSPP = 83,                   /* ILOSSPP  */
  YYSYMBOL_TRUSTR = 84,                    /* TRUSTR  */
  YYSYMBOL_CUTOFF = 85,                    /* CUTOFF  */
  YYSYMBOL_RATIO = 86,                     /* RATIO  */
  YYSYMBOL_RECOMPUTE = 87,                 /* RECOMPUTE  */
  YYSYMBOL_JACOBIAN = 88,                  /* JACOBIAN  */
  YYSYMBOL_ERRORT = 89,                    /* ERRORT  */
  YYSYMBOL_ONLY = 90,                      /* ONLY  */
  YYSYMBOL_TRANSVERSEJ = 91,               /* TRANSVERSEJ  */
  YYSYMBOL_XTOL = 92,                      /* XTOL  */
  YYSYMBOL_GRADDX = 93,                    /* GRADDX  */
  YYSYMBOL_REMESH = 94,                    /* REMESH  */
  YYSYMBOL_FIRST = 95,                     /* FIRST  */
  YYSYMBOL_DECOMPOSITION = 96,             /* DECOMPOSITION  */
  YYSYMBOL_MATERIAL = 97,                  /* MATERIAL  */
  YYSYMBOL_MODELIZATION = 98,              /* MODELIZATION  */
  YYSYMBOL_COMPONENT = 99,                 /* COMPONENT  */
  YYSYMBOL_SAVE = 100,                     /* SAVE  */
  YYSYMBOL_REDUCTION = 101,                /* REDUCTION  */
  YYSYMBOL_RELOAD = 102,                   /* RELOAD  */
  YYSYMBOL_NEEDED = 103,                   /* NEEDED  */
  YYSYMBOL_DONE = 104,                     /* DONE  */
  YYSYMBOL_CHANGED = 105,                  /* CHANGED  */
  YYSYMBOL_tMIN = 106,                     /* tMIN  */
  YYSYMBOL_tMAX = 107,                     /* tMAX  */
  YYSYMBOL_PLUSI = 108,                    /* PLUSI  */
  YYSYMBOL_MINUSI = 109,                   /* MINUSI  */
  YYSYMBOL_110_ = 110,                     /* ','  */
  YYSYMBOL_111_ = 111,                     /* '['  */
  YYSYMBOL_112_ = 112,                     /* ']'  */
  YYSYMBOL_113_ = 113,                     /* '{'  */
  YYSYMBOL_114_ = 114,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 115,                 /* $accept  */
  YYSYMBOL_All = 116,                      /* All  */
  YYSYMBOL_PrjDataItems = 117,             /* PrjDataItems  */
  YYSYMBOL_PrjDataItem = 118,              /* PrjDataItem  */
  YYSYMBOL_SFFloat = 119,                  /* SFFloat  */
  YYSYMBOL_SFComplex = 120,                /* SFComplex  */
  YYSYMBOL_SFFloatList = 121,              /* SFFloatList  */
  YYSYMBOL_MFFloat = 122,                  /* MFFloat  */
  YYSYMBOL_SFComplexList = 123,            /* SFComplexList  */
  YYSYMBOL_MFComplex = 124,                /* MFComplex  */
  YYSYMBOL_MFFloatOrMFComplex = 125,       /* MFFloatOrMFComplex  */
  YYSYMBOL_ImportData = 126,               /* ImportData  */
  YYSYMBOL_CAD = 127,                      /* CAD  */
  YYSYMBOL_Units = 128,                    /* Units  */
  YYSYMBOL_Mesh = 129,                     /* Mesh  */
  YYSYMBOL_Mwm = 130,                      /* Mwm  */
  YYSYMBOL_Mor = 131,                      /* Mor  */
  YYSYMBOL_FreqAna = 132,                  /* FreqAna  */
  YYSYMBOL_FilterDesign = 133,             /* FilterDesign  */
  YYSYMBOL_134_1 = 134,                    /* $@1  */
  YYSYMBOL_WorkStatus = 135                /* WorkStatus  */
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
#define YYLAST   365

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  115
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  140
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  372

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   364


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
       2,     2,     2,     2,   110,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   111,     2,   112,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   113,     2,   114,     2,     2,     2,     2,
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
     105,   106,   107,   108,   109
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    99,    99,   103,   104,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   139,   144,   153,   157,   188,
     189,   190,   191,   192,   196,   197,   202,   203,   204,   205,
     206,   210,   211,   216,   217,   271,   275,   282,   286,   293,
     297,   304,   308,   312,   316,   320,   324,   328,   332,   335,
     338,   342,   346,   350,   354,   358,   365,   372,   377,   381,
     385,   392,   397,   402,   406,   411,   415,   420,   424,   428,
     432,   436,   440,   444,   448,   452,   456,   460,   464,   467,
     471,   475,   479,   482,   486,   490,   494,   498,   502,   506,
     510,   514,   517,   521,   525,   529,   535,   539,   545,   549,
     556,   560,   564,   568,   572,   576,   580,   587,   592,   596,
     600,   605,   609,   613,   617,   621,   625,   629,   629,   637,
     641,   645,   649,   653,   657,   661,   665,   669,   672,   675,
     678,   684,   688,   692,   696,   700,   704,   708,   713,   717,
     721
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
  "NETWORK", "LENGTH", "FREQ", "UNIT", "EXP", "MESH", "TET", "SHARED",
  "REFINE", "RESONANCE", "ENERGY", "WAVELENGTH", "LOCAL", "MESHING3D",
  "ANA", "BAND", "NUM", "MOR", "RESPONSE", "SYMMETRIC", "PARAMETER",
  "TYPE", "TOPOLOGY", "PART", "XSCALE", "YSCALE", "AUTO", "ZERO", "POLE",
  "WINDOW", "CURVE", "CIRCLE", "XYPLANE", "YZPLANE", "ZXPLANE", "SYMMETRY",
  "IDEAL", "CANONICAL", "FILTER", "CUSTOM", "PASS", "KRYLOV", "ORDER",
  "RETURNLOSS", "OUTBAND", "MAPPING", "TUNING", "METHOD", "SOURCE",
  "ITERMAX", "AUTOMATIC", "DESIGN", "WITH", "MAPPED", "TX", "ZEROS",
  "SPICE", "SET", "CENTRAL", "CONFIG", "ADD", "CONJUGATE", "PORT",
  "IMPEDANCE", "TARGET", "QFACTOR", "UNIFORM", "INDUCTIVE", "SKIN",
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

#define YYPACT_NINF (-261)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-3)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -261,     8,   233,  -261,  -261,    16,    33,    60,     3,    94,
       0,    17,   104,   123,    28,   117,    64,    -4,   110,   126,
      97,   120,   121,   119,   122,    77,   125,   157,     4,   105,
     127,   128,    88,    75,    79,    76,    78,    81,     2,    82,
      83,  -261,  -261,  -261,  -261,  -261,  -261,  -261,  -261,  -261,
    -261,  -261,  -261,  -261,  -261,    72,   167,    84,   176,   173,
     103,    85,   -74,    96,   108,   169,   107,   165,   -24,   180,
     192,    19,   183,    49,   144,   170,    65,   195,   197,   198,
      -8,   171,   145,   177,   202,    92,   152,    13,   -23,   141,
     134,   133,    92,   131,   162,   187,   185,   178,   151,   163,
     154,   -10,   153,    80,    -2,    92,   217,   218,   220,   221,
     222,    86,   213,   124,   129,   225,   227,   130,   223,   208,
     235,  -261,   236,   244,   245,     7,   228,   230,   232,   251,
     261,   179,    92,   184,    92,    92,   186,   239,  -261,    92,
     270,   247,    92,   272,   276,   281,   259,    23,   200,   284,
    -261,  -261,  -261,   285,   287,   229,   219,   289,   292,    92,
    -261,  -261,  -261,  -261,    92,   293,   294,   295,   296,    92,
     298,   299,    92,    57,   212,    92,    92,  -261,    92,    92,
    -261,   301,   302,    92,   303,   304,   269,   306,   271,   282,
     273,   246,   312,   314,    92,    92,  -261,    92,   316,   317,
      92,    92,  -261,  -261,  -261,  -261,  -261,  -261,   214,   309,
     297,   320,   322,  -261,  -261,   240,   310,   330,  -261,  -261,
    -261,  -261,   333,    92,   335,   336,   337,  -261,  -261,    92,
    -261,   338,    92,    92,    92,   339,    92,  -261,   340,    92,
    -261,  -261,  -261,    92,   341,    92,    92,   342,  -261,  -261,
     280,   283,  -261,  -261,    92,  -261,  -261,  -261,  -261,  -261,
    -261,  -261,  -261,  -261,   344,   345,   347,  -261,  -261,   241,
    -261,  -261,  -261,  -261,    92,  -261,  -261,   343,  -261,   348,
     350,   351,   352,  -261,  -261,  -261,  -261,  -261,  -261,  -261,
    -261,  -261,   274,   331,   354,  -261,  -261,    92,   356,  -261,
    -261,    92,  -261,  -261,  -261,  -261,  -261,  -261,  -261,  -261,
    -261,  -261,  -261,  -261,    92,  -261,    92,  -261,  -261,   358,
     359,  -261,  -261,  -261,  -261,    92,  -261,  -261,  -261,  -261,
     334,  -261,  -261,  -261,  -261,    92,   361,  -261,  -261,  -261,
    -261,  -261,  -261,  -261,  -261,     9,     1,    12,    15,   362,
    -261,  -261,    92,    92,    92,  -261,  -261,    92,  -261,    48,
    -261,  -261,  -261,  -261,  -261,  -261,  -261,  -261,  -261,  -261,
    -261,  -261
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
       0,     4,     8,     5,     6,     7,    11,    12,     9,    10,
      13,    37,    35,    36,    38,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,     0,     0,     0,     0,     0,     0,     0,
     108,    16,    15,   111,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   117,     0,     0,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   120,     0,     0,     0,
       0,     0,    56,   134,   132,   131,   133,   135,     0,     0,
       0,     0,     0,   137,   140,     0,     0,     0,   139,    40,
      93,    94,     0,     0,     0,     0,     0,    41,    44,     0,
      43,     0,     0,     0,     0,     0,     0,    65,     0,     0,
      58,    60,   119,     0,     0,     0,     0,     0,    73,    76,
       0,     0,    75,    88,     0,   112,    79,    78,    80,    82,
      81,    84,    83,    90,     0,     0,     0,    89,    91,     0,
     113,   115,   116,    77,     0,   100,   101,     0,   106,     0,
       0,     0,     0,   130,   127,   128,   129,   122,   125,   121,
     123,   124,     0,     0,     0,   136,   138,     0,     0,    71,
      96,     0,    47,    45,    46,    51,    42,    48,    49,    50,
      59,    61,    66,    57,     0,    98,     0,    63,    99,     0,
       0,   107,    85,    86,    87,    23,    33,    34,   118,    64,
       0,   102,   103,   105,    92,     0,     0,    69,    68,    72,
      95,    62,    97,   110,   109,     0,     0,     0,     0,     0,
      67,    70,     0,     0,     0,    24,    19,     0,    31,     0,
      26,    22,    25,    21,    29,    32,    28,   104,    17,    18,
      20,    27
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -261,  -261,  -261,  -261,   -85,  -260,  -261,  -261,  -261,  -261,
    -261,  -261,  -261,  -261,  -261,  -261,  -261,  -261,  -261,  -261,
    -261
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    41,   359,   346,   347,   326,   348,   327,
     328,    42,    43,    44,    45,    46,    47,    48,    49,   269,
      50
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     163,   196,   135,   189,   161,   162,    54,   180,     3,    73,
     161,   162,   161,   162,    59,   161,   162,    96,   161,   162,
     202,    51,   111,    55,   153,   154,   161,   162,   190,    60,
     112,    56,   127,   128,    61,   170,    62,   171,    52,    63,
     223,    68,    97,   165,   222,   139,   140,   230,    74,   232,
     233,    98,   197,    99,   236,   155,    69,   239,   198,    64,
     244,   172,   245,   156,   173,    53,   100,   174,   101,   175,
     176,   166,   167,   168,   254,   142,   143,    71,   146,   255,
     199,   200,   201,   136,   260,   117,   360,   263,   366,   169,
     267,   268,    72,   270,   271,   161,   162,   371,   274,   208,
     118,   147,   113,   114,    57,   148,   149,    82,    58,   285,
     286,   357,   287,   358,   209,   290,   291,   352,   353,   354,
      66,   355,   361,    65,   362,   364,   132,   365,    83,   133,
      84,    85,    86,    87,    88,   123,    67,   124,   301,   125,
     192,    70,    89,    77,   305,   264,   265,   307,   308,   309,
      90,   311,    91,    92,   313,    93,   352,   353,   314,    75,
     316,   317,   193,   194,   195,    76,    78,    79,    80,   321,
      95,    81,    94,   102,   105,   107,   103,   104,   106,   108,
     119,   121,   129,   109,   110,   115,   116,   120,   122,   329,
     131,   134,   126,   137,   130,   138,   141,   144,   150,   145,
     151,   152,   158,   159,   157,   160,   164,   177,   178,   179,
     181,   182,   338,   183,   184,   186,   340,   185,   188,   187,
     203,   204,   191,   205,   206,   207,   210,   211,   213,   341,
     214,   342,   212,    -2,     4,   217,   216,   215,   218,   219,
     345,     5,     6,     7,     8,     9,    10,   220,   221,    11,
     350,    12,    13,    14,   227,   224,    15,   225,    16,   226,
     356,    17,   363,    18,   228,   229,   235,   368,   369,   370,
     231,    19,   234,   237,   238,   240,    20,    21,    22,   241,
      23,    24,    25,    26,   242,   243,   246,   247,   248,    27,
     249,   251,   252,   250,    28,   253,   256,   257,   258,   259,
      29,   261,   262,   266,   272,   273,   275,   276,   277,   278,
     279,   280,   281,    30,    31,   283,   282,   284,    32,   288,
     289,   292,   293,   295,   294,   296,   297,    33,    34,    35,
      36,    37,    38,   299,    39,    40,   300,   298,   302,   303,
     304,   306,   310,   312,   315,   318,   319,   322,   323,   320,
     324,   331,   325,   332,   333,   334,   330,   337,   336,   339,
     335,   343,   344,   349,   351,   367
};

static const yytype_int16 yycheck[] =
{
      85,     3,    26,    13,     3,     4,     3,    92,     0,    13,
       3,     4,     3,     4,    14,     3,     4,    13,     3,     4,
     105,     5,    20,    20,    32,    33,     3,     4,    38,    29,
      28,    28,   106,   107,    17,    58,    19,    60,     5,    22,
     125,    13,    38,    30,    37,    26,    27,   132,    52,   134,
     135,    47,    54,    49,   139,    63,    28,   142,    60,    42,
      37,    84,   147,    71,    87,     5,    62,    90,    64,    92,
      93,    58,    59,    60,   159,    26,    27,    13,    13,   164,
      82,    83,    84,   107,   169,    13,   346,   172,   348,    76,
     175,   176,    28,   178,   179,     3,     4,   357,   183,    13,
      28,    36,   100,   101,   101,    40,    41,    30,    14,   194,
     195,   110,   197,   112,    28,   200,   201,   108,   109,   110,
      16,   112,   110,   106,   112,   110,    19,   112,    51,    22,
      53,    54,    55,    56,    57,    32,    13,    34,   223,    36,
      60,    24,    65,    46,   229,    88,    89,   232,   233,   234,
      73,   236,    75,    76,   239,    78,   108,   109,   243,    49,
     245,   246,    82,    83,    84,    39,    46,    46,    49,   254,
      13,    49,    47,    68,    86,    96,    49,    49,   103,   103,
      13,     5,    86,   105,   103,   103,   103,   103,    15,   274,
      21,    26,   107,    13,    86,     3,    13,    53,     3,    29,
       3,     3,    57,    26,    33,     3,    54,    66,    74,    76,
      79,    49,   297,    26,    29,    64,   301,    39,    64,    56,
       3,     3,    69,     3,     3,     3,    13,   103,     3,   314,
       3,   316,   103,     0,     1,    27,    13,   107,     3,     3,
     325,     8,     9,    10,    11,    12,    13,     3,     3,    16,
     335,    18,    19,    20,     3,    27,    23,    27,    25,    27,
     345,    28,   347,    30,     3,    86,    27,   352,   353,   354,
      86,    38,    86,     3,    27,     3,    43,    44,    45,     3,
      47,    48,    49,    50,     3,    26,    86,     3,     3,    56,
       3,    72,     3,    64,    61,     3,     3,     3,     3,     3,
      67,     3,     3,    91,     3,     3,     3,     3,    39,     3,
      39,    29,    39,    80,    81,     3,    70,     3,    85,     3,
       3,   107,    13,     3,    27,     3,    86,    94,    95,    96,
      97,    98,    99,     3,   101,   102,     3,    27,     3,     3,
       3,     3,     3,     3,     3,     3,    66,     3,     3,    66,
       3,     3,   111,     3,     3,     3,    13,     3,    27,     3,
      86,     3,     3,    29,     3,     3
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   116,   117,     0,     1,     8,     9,    10,    11,    12,
      13,    16,    18,    19,    20,    23,    25,    28,    30,    38,
      43,    44,    45,    47,    48,    49,    50,    56,    61,    67,
      80,    81,    85,    94,    95,    96,    97,    98,    99,   101,
     102,   118,   126,   127,   128,   129,   130,   131,   132,   133,
     135,     5,     5,     5,     3,    20,    28,   101,    14,    14,
      29,    17,    19,    22,    42,   106,    16,    13,    13,    28,
      24,    13,    28,    13,    52,    49,    39,    46,    46,    46,
      49,    49,    30,    51,    53,    54,    55,    56,    57,    65,
      73,    75,    76,    78,    47,    13,    13,    38,    47,    49,
      62,    64,    68,    49,    49,    86,   103,    96,   103,   105,
     103,    20,    28,   100,   101,   103,   103,    13,    28,    13,
     103,     5,    15,    32,    34,    36,   107,   106,   107,    86,
      86,    21,    19,    22,    26,    26,   107,    13,     3,    26,
      27,    13,    26,    27,    53,    29,    13,    36,    40,    41,
       3,     3,     3,    32,    33,    63,    71,    33,    57,    26,
       3,     3,     4,   119,    54,    30,    58,    59,    60,    76,
      58,    60,    84,    87,    90,    92,    93,    66,    74,    76,
     119,    79,    49,    26,    29,    39,    64,    56,    64,    13,
      38,    69,    60,    82,    83,    84,     3,    54,    60,    82,
      83,    84,   119,     3,     3,     3,     3,     3,    13,    28,
      13,   103,   103,     3,     3,   107,    13,    27,     3,     3,
       3,     3,    37,   119,    27,    27,    27,     3,     3,    86,
     119,    86,   119,   119,    86,    27,   119,     3,    27,   119,
       3,     3,     3,    26,    37,   119,    86,     3,     3,     3,
      64,    72,     3,     3,   119,   119,     3,     3,     3,     3,
     119,     3,     3,   119,    88,    89,    91,   119,   119,   134,
     119,   119,     3,     3,   119,     3,     3,    39,     3,    39,
      29,    39,    70,     3,     3,   119,   119,   119,     3,     3,
     119,   119,   107,    13,    27,     3,     3,    86,    27,     3,
       3,   119,     3,     3,     3,   119,     3,   119,   119,   119,
       3,   119,     3,   119,   119,     3,   119,   119,     3,    66,
      66,   119,     3,     3,     3,   111,   122,   124,   125,   119,
      13,     3,     3,     3,     3,    86,    27,     3,   119,     3,
     119,   119,   119,     3,     3,   119,   120,   121,   123,    29,
     119,     3,   108,   109,   110,   112,   119,   110,   112,   119,
     120,   110,   112,   119,   110,   112,   120,     3,   119,   119,
     119,   120
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   115,   116,   117,   117,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   119,   119,   120,   120,   121,
     121,   121,   121,   121,   122,   122,   123,   123,   123,   123,
     123,   124,   124,   125,   125,   126,   126,   127,   127,   128,
     128,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   130,   131,   131,   131,
     131,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   134,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     2,
       3,     2,     2,     0,     3,     3,     2,     3,     2,     2,
       0,     3,     3,     1,     1,     2,     2,     2,     2,     3,
       4,     4,     5,     4,     4,     5,     5,     5,     5,     5,
       5,     5,     3,     3,     3,     3,     3,     5,     4,     5,
       4,     5,     6,     5,     5,     4,     5,     6,     6,     5,
       6,     5,     6,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     5,     5,     5,     4,     4,
       4,     4,     5,     4,     4,     6,     5,     6,     5,     5,
       4,     4,     5,     5,     7,     5,     4,     5,     3,     6,
       6,     3,     4,     4,     3,     4,     4,     0,     5,     4,
       3,     4,     4,     4,     4,     4,     3,     4,     4,     4,
       4,     3,     3,     3,     3,     3,     4,     3,     4,     4,
       3
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
#line 118 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
                        {YYABORT;}
#line 1492 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 15: /* SFFloat: FLOATING  */
#line 140 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
                {
		      (yyval.fval) = (yyvsp[0].fval);
		      if(ProjData::Fbuff) ProjData::Fbuff->push_back((yyvsp[0].fval));
		}
#line 1501 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 16: /* SFFloat: INTEGER  */
#line 145 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
                {
		       (yyval.fval) = (double)(yyvsp[0].ival);
		       if(ProjData::Fbuff) ProjData::Fbuff->push_back((double)(yyvsp[0].ival));
		}
#line 1510 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 17: /* SFComplex: SFFloat PLUSI SFFloat  */
#line 154 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
                {
		       if(ProjData::Cbuff) ProjData::Cbuff->push_back(std::complex<double>((yyvsp[-2].fval),(yyvsp[0].fval)));
		}
#line 1518 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 18: /* SFComplex: SFFloat MINUSI SFFloat  */
#line 158 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
                {
		       if(ProjData::Cbuff) ProjData::Cbuff->push_back(std::complex<double>((yyvsp[-2].fval),-(yyvsp[0].fval)));
		}
#line 1526 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 24: /* MFFloat: '[' SFFloat ']'  */
#line 196 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
                          {}
#line 1532 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 25: /* MFFloat: '[' SFFloatList ']'  */
#line 197 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
                              {}
#line 1538 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 31: /* MFComplex: '[' SFComplex ']'  */
#line 210 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
                            {}
#line 1544 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 32: /* MFComplex: '[' SFComplexList ']'  */
#line 211 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
                                {}
#line 1550 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 33: /* MFFloatOrMFComplex: MFFloat  */
#line 216 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
                  {}
#line 1556 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 34: /* MFFloatOrMFComplex: MFComplex  */
#line 217 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
                    {}
#line 1562 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 35: /* ImportData: VARFILEPATH STRING  */
#line 272 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.varFilePath=(yyvsp[0].sval);
            }
#line 1570 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 36: /* ImportData: S2PFILEPATH STRING  */
#line 276 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.s2pFilePath=(yyvsp[0].sval);
            }
#line 1578 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 37: /* CAD: MAINASSNAME STRING  */
#line 283 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.mainAssName=(yyvsp[0].sval);
            }
#line 1586 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 38: /* CAD: NETWORK INTEGER  */
#line 287 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.network=(yyvsp[0].ival);
            }
#line 1594 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 39: /* Units: LENGTH UNIT STRING  */
#line 294 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              strcpy(prjData.lengthUnitName,(yyvsp[0].sval));
            }
#line 1602 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 40: /* Units: FREQ UNIT EXP INTEGER  */
#line 298 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.freqUnitE =(yyvsp[0].ival);
            }
#line 1610 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 41: /* Mesh: MESH WAVELENGTH RATIO INTEGER  */
#line 305 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.meshPerWavelen=(yyvsp[0].ival);
            }
#line 1618 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 42: /* Mesh: SHARED MESH WAVELENGTH RATIO INTEGER  */
#line 309 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.sharedMeshPerWavelen=(yyvsp[0].ival);
            }
#line 1626 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 43: /* Mesh: SHARED MESH REFINE SFFloat  */
#line 313 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.sharedMeshRefine=(yyvsp[0].fval);
            }
#line 1634 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 44: /* Mesh: MESH CIRCLE RATIO INTEGER  */
#line 317 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.meshPerCircle=(yyvsp[0].ival);
            }
#line 1642 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 45: /* Mesh: MESH REFINE tMIN NUM INTEGER  */
#line 321 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.meshRefineMinNum=(yyvsp[0].ival);
            }
#line 1650 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 46: /* Mesh: MESH REFINE tMAX NUM INTEGER  */
#line 325 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.meshRefineMaxNum=(yyvsp[0].ival);
            }
#line 1658 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 47: /* Mesh: MESH TET tMAX NUM INTEGER  */
#line 329 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.meshTetMaxNum=(yyvsp[0].ival);
            }
#line 1666 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 48: /* Mesh: REFINE FREQ BAND SFFloat SFFloat  */
#line 333 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
            }
#line 1673 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 49: /* Mesh: RESONANCE FREQ BAND SFFloat SFFloat  */
#line 336 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
            }
#line 1680 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 50: /* Mesh: RESONANCE FREQ tMAX RATIO SFFloat  */
#line 339 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.resonFreqMaxRatio=(yyvsp[0].fval);
            }
#line 1688 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 51: /* Mesh: MESH tMIN ENERGY RATIO SFFloat  */
#line 343 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.meshMinEnergyRatio=(yyvsp[0].fval);
            }
#line 1696 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 52: /* Mesh: LOCAL MESHING3D INTEGER  */
#line 347 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.localMeshing3d=(yyvsp[0].ival);
            }
#line 1704 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 53: /* Mesh: XYPLANE SYMMETRY INTEGER  */
#line 351 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.XYplaneSymmetry=(yyvsp[0].ival);
            }
#line 1712 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 54: /* Mesh: YZPLANE SYMMETRY INTEGER  */
#line 355 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.YZplaneSymmetry=(yyvsp[0].ival);
            }
#line 1720 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 55: /* Mesh: ZXPLANE SYMMETRY INTEGER  */
#line 359 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.ZXplaneSymmetry=(yyvsp[0].ival);
            }
#line 1728 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 56: /* Mwm: CUTOFF RATIO SFFloat  */
#line 366 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
           {
              prjData.cutoffRatio=(yyvsp[0].fval);
           }
#line 1736 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 57: /* Mor: MOR FREQ BAND SFFloat SFFloat  */
#line 373 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.freqBand[0]=(yyvsp[-1].fval);
              prjData.freqBand[1]=(yyvsp[0].fval);
            }
#line 1745 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 58: /* Mor: MOR FREQ NUM INTEGER  */
#line 378 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.MORFreqNum=(yyvsp[0].ival);
            }
#line 1753 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 59: /* Mor: RESONANCE MOR FREQ NUM INTEGER  */
#line 382 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.MORFreqNum1=(yyvsp[0].ival);
            }
#line 1761 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 60: /* Mor: MOR KRYLOV ORDER INTEGER  */
#line 386 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.KrylovOrder=(yyvsp[0].ival);
            }
#line 1769 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 61: /* FreqAna: ANA FREQ BAND SFFloat SFFloat  */
#line 393 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.anaFreqBand[0]=(yyvsp[-1].fval);
              prjData.anaFreqBand[1]=(yyvsp[0].fval);
            }
#line 1778 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 62: /* FreqAna: ZERO POLE FREQ BAND SFFloat SFFloat  */
#line 398 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.zpFreqBand[0]=(yyvsp[-1].fval);
              prjData.zpFreqBand[1]=(yyvsp[0].fval);
            }
#line 1787 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 63: /* FreqAna: ZERO POLE WINDOW RATIO SFFloat  */
#line 403 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.zpWinRatio=(yyvsp[0].fval);
            }
#line 1795 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 64: /* FreqAna: MAPPING FREQ BAND SFFloat SFFloat  */
#line 407 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.mapFreqBand[0]=(yyvsp[-1].fval);
              prjData.mapFreqBand[1]=(yyvsp[0].fval);
            }
#line 1804 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 65: /* FreqAna: ANA FREQ NUM INTEGER  */
#line 412 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.anaFreqNum=(yyvsp[0].ival);
            }
#line 1812 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 66: /* FreqAna: ANA MOR FREQ NUM INTEGER  */
#line 416 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              if(prjData.network) prjData.netMORFreqNum=(yyvsp[0].ival);
              else                prjData.cmpMORFreqNum=(yyvsp[0].ival);
            }
#line 1821 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 67: /* FreqAna: COMPONENT RESONANCE FREQ tMAX RATIO SFFloat  */
#line 421 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.cmpResonFreqMaxRatio=(yyvsp[0].fval);
            }
#line 1829 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 68: /* FreqAna: NETWORK RESONANCE FREQ tMAX RATIO SFFloat  */
#line 425 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.netResonFreqMaxRatio=(yyvsp[0].fval);
            }
#line 1837 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 69: /* FreqAna: COMPONENT MOR FREQ NUM INTEGER  */
#line 429 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.cmpMORFreqNum=(yyvsp[0].ival);
            }
#line 1845 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 70: /* FreqAna: COMPONENT RESONANCE MOR FREQ NUM INTEGER  */
#line 433 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.cmpMORFreqNum1=(yyvsp[0].ival);
            }
#line 1853 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 71: /* FreqAna: NETWORK MOR FREQ NUM INTEGER  */
#line 437 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.netMORFreqNum=(yyvsp[0].ival);
            }
#line 1861 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 72: /* FreqAna: NETWORK RESONANCE MOR FREQ NUM INTEGER  */
#line 441 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.netMORFreqNum1=(yyvsp[0].ival);
            }
#line 1869 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 73: /* FreqAna: IDEAL FILTER TYPE INTEGER  */
#line 445 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.idealFilterType=(yyvsp[0].ival);
            }
#line 1877 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 74: /* FreqAna: IDEAL FILTER TYPE INTEGER  */
#line 449 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.idealFilterType=(yyvsp[0].ival);
            }
#line 1885 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 75: /* FreqAna: CANONICAL FILTER TOPOLOGY INTEGER  */
#line 453 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.canonicalFilterTopology=(yyvsp[0].ival);
            }
#line 1893 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 76: /* FreqAna: IDEAL FILTER TOPOLOGY INTEGER  */
#line 457 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.canonicalFilterTopology=(yyvsp[0].ival);
            }
#line 1901 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 77: /* FreqAna: CUSTOM IDEAL FILTER INTEGER  */
#line 461 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.customIdealFilter=(yyvsp[0].ival);
            }
#line 1909 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 78: /* FreqAna: FILTER MAPPING METHOD INTEGER  */
#line 465 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
            }
#line 1916 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 79: /* FreqAna: FILTER MAPPING SYMMETRIC INTEGER  */
#line 468 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filtermapSymmetric=(yyvsp[0].ival);
            }
#line 1924 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 80: /* FreqAna: FILTER MAPPING SOURCE INTEGER  */
#line 472 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filtermapSource=(yyvsp[0].ival);
            }
#line 1932 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 81: /* FreqAna: FILTER MAPPING QFACTOR SFFloat  */
#line 476 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filtermapQfactor = (yyvsp[0].fval);
            }
#line 1940 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 82: /* FreqAna: FILTER MAPPING ITERMAX INTEGER  */
#line 480 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
            }
#line 1947 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 83: /* FreqAna: FILTER TUNING ITERMAX INTEGER  */
#line 483 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterTuneItermax=(yyvsp[0].ival);
            }
#line 1955 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 84: /* FreqAna: FILTER TUNING METHOD INTEGER  */
#line 487 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterTuneMethod=(yyvsp[0].ival);
            }
#line 1963 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 85: /* FreqAna: FILTER TUNING RECOMPUTE JACOBIAN INTEGER  */
#line 491 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterTuneRecomputeJaco=(yyvsp[0].ival);
            }
#line 1971 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 86: /* FreqAna: FILTER TUNING RECOMPUTE ERRORT INTEGER  */
#line 495 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterTuneRecomputeError=(yyvsp[0].ival);
            }
#line 1979 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 87: /* FreqAna: FILTER TUNING ONLY TRANSVERSEJ INTEGER  */
#line 499 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterTuneOnlyJt=(yyvsp[0].ival);
            }
#line 1987 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 88: /* FreqAna: FILTER SYMMETRIC TUNING INTEGER  */
#line 503 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterSymmetricTuning=(yyvsp[0].ival);
            }
#line 1995 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 89: /* FreqAna: FILTER TUNING XTOL SFFloat  */
#line 507 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterTuneXtol=(yyvsp[0].fval);
            }
#line 2003 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 90: /* FreqAna: FILTER TUNING TRUSTR SFFloat  */
#line 511 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterTuneTrustR=(yyvsp[0].fval);
            }
#line 2011 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 91: /* FreqAna: FILTER TUNING GRADDX SFFloat  */
#line 515 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
            }
#line 2018 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 92: /* FreqAna: SPICE SET CENTRAL CONFIG INTEGER  */
#line 518 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.spiceSetCentralConfig=(yyvsp[0].ival);
            }
#line 2026 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 93: /* FreqAna: FREQ RESPONSE TYPE INTEGER  */
#line 522 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.freqRespParType = (FreqRespParType) (yyvsp[0].ival);
            }
#line 2034 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 94: /* FreqAna: FREQ RESPONSE PART INTEGER  */
#line 526 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.freqRespParPart = (yyvsp[0].ival);
            }
#line 2042 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 95: /* FreqAna: FREQ RESPONSE YSCALE SFFloat SFFloat SFFloat  */
#line 530 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.freqRespYscale[0]=(yyvsp[-2].fval);
              prjData.freqRespYscale[1]=(yyvsp[-1].fval);
              prjData.freqRespYscale[2]=(yyvsp[0].fval);
            }
#line 2052 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 96: /* FreqAna: FREQ RESPONSE YSCALE AUTO INTEGER  */
#line 536 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.freqRespYscaleAuto=(yyvsp[0].ival);
            }
#line 2060 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 97: /* FreqAna: ZERO POLE YSCALE SFFloat SFFloat SFFloat  */
#line 540 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.zeropoleYscale[0]=(yyvsp[-2].fval);
              prjData.zeropoleYscale[1]=(yyvsp[-1].fval);
              prjData.zeropoleYscale[2]=(yyvsp[0].fval);
            }
#line 2070 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 98: /* FreqAna: ZERO POLE YSCALE AUTO INTEGER  */
#line 546 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.zeropoleYscaleAuto=(yyvsp[0].ival);
            }
#line 2078 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 99: /* FreqAna: ZERO POLE CURVE INTEGER INTEGER  */
#line 550 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              std::pair<int, int> Sij_ports;
              Sij_ports.first=(yyvsp[-1].ival);
              Sij_ports.second=(yyvsp[0].ival);
              prjData.zeropoleCurves.insert(Sij_ports);
            }
#line 2089 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 100: /* FreqAna: AUTOMATIC FREQ RESPONSE INTEGER  */
#line 557 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.autoFreqResponse=(yyvsp[0].ival);
            }
#line 2097 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 101: /* FreqAna: AUTOMATIC ZERO POLE INTEGER  */
#line 561 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.autoZeropole=(yyvsp[0].ival);
            }
#line 2105 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 102: /* FreqAna: AUTOMATIC DESIGN MAPPED POLE INTEGER  */
#line 565 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.autoDesignWithMappedTz=(yyvsp[0].ival);
            }
#line 2113 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 103: /* FreqAna: AUTOMATIC MAPPED FREQ RESPONSE INTEGER  */
#line 569 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.autoMappedFreqResponse=(yyvsp[0].ival);
            }
#line 2121 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 104: /* FreqAna: AUTOMATIC IDEAL MAPPED POLE FREQ RESPONSE INTEGER  */
#line 573 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.autoIdealMappedTzFreqResponse=(yyvsp[0].ival);
            }
#line 2129 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 105: /* FreqAna: AUTOMATIC MAPPED ZERO POLE INTEGER  */
#line 577 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.autoMappedZeropole=(yyvsp[0].ival);
            }
#line 2137 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 106: /* FreqAna: AUTOMATIC FILTER MAPPING INTEGER  */
#line 581 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.autoFilterMapping=(yyvsp[0].ival);
            }
#line 2145 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 107: /* FilterDesign: FILTER PASS BAND SFFloat SFFloat  */
#line 588 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterPassBand[0]=(yyvsp[-1].fval);
              prjData.filterPassBand[1]=(yyvsp[0].fval);
            }
#line 2154 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 108: /* FilterDesign: FILTER ORDER INTEGER  */
#line 593 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterOrder = (yyvsp[0].ival);
            }
#line 2162 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 109: /* FilterDesign: IDEAL FILTER ADD CONJUGATE ZEROS INTEGER  */
#line 597 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.idealFilterAddConjugateTZ = (yyvsp[0].ival);
            }
#line 2170 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 110: /* FilterDesign: IDEAL FILTER WITH MAPPED ZEROS INTEGER  */
#line 601 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.idealFilterWithMappedTZ = (yyvsp[0].ival);
              if(prjData.idealFilterWithMappedTZ) prjData.idealFilterAddConjugateTZ=0;
            }
#line 2179 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 111: /* FilterDesign: FILTER RETURNLOSS SFFloat  */
#line 606 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterRetLoss = (yyvsp[0].fval);
            }
#line 2187 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 112: /* FilterDesign: FILTER OUTBAND RETURNLOSS SFFloat  */
#line 610 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterOutbandRetLoss = (yyvsp[0].fval);
            }
#line 2195 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 113: /* FilterDesign: FILTER PORT IMPEDANCE SFFloat  */
#line 614 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterPortImpedance = (yyvsp[0].fval);
            }
#line 2203 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 114: /* FilterDesign: FILTER QFACTOR SFFloat  */
#line 618 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterQfactor = (yyvsp[0].fval);
            }
#line 2211 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 115: /* FilterDesign: FILTER TARGET QFACTOR SFFloat  */
#line 622 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterTargetQfactor = (yyvsp[0].fval);
            }
#line 2219 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 116: /* FilterDesign: FILTER INDUCTIVE SKIN INTEGER  */
#line 626 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.filterInductiveSkin = (yyvsp[0].ival);
            }
#line 2227 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 117: /* $@1: %empty  */
#line 629 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
                           {ProjData::Cbuff =&prjData.filterZeros; ProjData::Cbuff->clear();
                            ProjData::Fbuff =&ProjData::Fvec; ProjData::Fvec.clear();}
#line 2234 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 118: /* FilterDesign: FILTER TX ZEROS $@1 MFFloatOrMFComplex  */
#line 631 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              if(prjData.filterZeros.size()==0){
               for (int i=0; i<ProjData::Fvec.size() ; i++) prjData.filterZeros.push_back(std::complex<double>(ProjData::Fvec[i],0.0));
               ProjData::Fbuff=NULL;
              } else ProjData::Cbuff=NULL;
            }
#line 2245 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 119: /* FilterDesign: SYMMETRIC FILTER RESPONSE INTEGER  */
#line 638 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.symmFilterResponse = (yyvsp[0].ival);
            }
#line 2253 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 120: /* FilterDesign: PREDISTORTED FILTER INTEGER  */
#line 642 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.predistortedFilter = (yyvsp[0].ival);
            }
#line 2261 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 121: /* FilterDesign: PREDISTORTED FILTER OPTIMIZE INTEGER  */
#line 646 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.predistFilterOptim = (yyvsp[0].ival);
            }
#line 2269 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 122: /* FilterDesign: PREDISTORTED FILTER RETURNLOSS SFFloat  */
#line 650 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimRL = (yyvsp[0].fval);
            }
#line 2277 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 123: /* FilterDesign: PREDISTORTED FILTER ILOSSPP SFFloat  */
#line 654 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimILpp = (yyvsp[0].fval);
            }
#line 2285 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 124: /* FilterDesign: PREDISTORTED FILTER TRUSTR SFFloat  */
#line 658 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimTrustR = (yyvsp[0].fval);
            }
#line 2293 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 125: /* FilterDesign: PREDISTORTED FILTER ITERMAX INTEGER  */
#line 662 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimIterMax = (yyvsp[0].ival);
            }
#line 2301 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 126: /* FilterDesign: PREDISTORTED FILTER INTEGER  */
#line 666 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.predistortedFilter = (yyvsp[0].ival);
            }
#line 2309 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 127: /* FilterDesign: LOSSY FILTER OPTIMIZE INTEGER  */
#line 670 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
            }
#line 2316 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 128: /* FilterDesign: LOSSY FILTER ILOSSPP SFFloat  */
#line 673 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
            }
#line 2323 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 129: /* FilterDesign: LOSSY FILTER TRUSTR SFFloat  */
#line 676 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
            }
#line 2330 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 130: /* FilterDesign: LOSSY FILTER ITERMAX INTEGER  */
#line 679 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
            }
#line 2337 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 131: /* WorkStatus: DECOMPOSITION NEEDED INTEGER  */
#line 685 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.workStatus.decompositionNeeded=(yyvsp[0].ival);
            }
#line 2345 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 132: /* WorkStatus: FIRST DECOMPOSITION INTEGER  */
#line 689 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.workStatus.firstDecomposition=(yyvsp[0].ival);
            }
#line 2353 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 133: /* WorkStatus: MATERIAL CHANGED INTEGER  */
#line 693 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.workStatus.materialChanged=(yyvsp[0].ival);
            }
#line 2361 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 134: /* WorkStatus: REMESH NEEDED INTEGER  */
#line 697 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.workStatus.remeshNeeded=(yyvsp[0].ival);
            }
#line 2369 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 135: /* WorkStatus: MODELIZATION NEEDED INTEGER  */
#line 701 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.workStatus.modelizationNeeded=(yyvsp[0].ival);
            }
#line 2377 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 136: /* WorkStatus: COMPONENT SAVE NEEDED INTEGER  */
#line 705 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.workStatus.componentsaveNeeded=(yyvsp[0].ival);
            }
#line 2385 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 137: /* WorkStatus: REDUCTION NEEDED INTEGER  */
#line 709 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.workStatus.cmpReductionNeeded=(yyvsp[0].ival);
              prjData.workStatus.netReductionNeeded=(yyvsp[0].ival);
            }
#line 2394 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 138: /* WorkStatus: COMPONENT REDUCTION NEEDED INTEGER  */
#line 714 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.workStatus.cmpReductionNeeded=(yyvsp[0].ival);
            }
#line 2402 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 139: /* WorkStatus: NETWORK REDUCTION NEEDED INTEGER  */
#line 718 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.workStatus.netReductionNeeded=(yyvsp[0].ival);
            }
#line 2410 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;

  case 140: /* WorkStatus: RELOAD NEEDED INTEGER  */
#line 722 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"
            {
              prjData.workStatus.reloadNeeded=(yyvsp[0].ival);
            }
#line 2418 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"
    break;


#line 2422 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData_parser.cpp"

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

#line 726 "/home/walter/MwCAD/EmCAD/conformalMesh/src/GUI/projectData.y"



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




