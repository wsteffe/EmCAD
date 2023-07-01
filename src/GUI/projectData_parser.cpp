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
  YYSYMBOL_IDEAL = 43,                     /* IDEAL  */
  YYSYMBOL_CANONICAL = 44,                 /* CANONICAL  */
  YYSYMBOL_FILTER = 45,                    /* FILTER  */
  YYSYMBOL_CUSTOM = 46,                    /* CUSTOM  */
  YYSYMBOL_PASS = 47,                      /* PASS  */
  YYSYMBOL_KRYLOV = 48,                    /* KRYLOV  */
  YYSYMBOL_ORDER = 49,                     /* ORDER  */
  YYSYMBOL_RETURNLOSS = 50,                /* RETURNLOSS  */
  YYSYMBOL_OUTBAND = 51,                   /* OUTBAND  */
  YYSYMBOL_MAPPING = 52,                   /* MAPPING  */
  YYSYMBOL_TUNING = 53,                    /* TUNING  */
  YYSYMBOL_METHOD = 54,                    /* METHOD  */
  YYSYMBOL_SOURCE = 55,                    /* SOURCE  */
  YYSYMBOL_ITERMAX = 56,                   /* ITERMAX  */
  YYSYMBOL_AUTOMATIC = 57,                 /* AUTOMATIC  */
  YYSYMBOL_DESIGN = 58,                    /* DESIGN  */
  YYSYMBOL_WITH = 59,                      /* WITH  */
  YYSYMBOL_MAPPED = 60,                    /* MAPPED  */
  YYSYMBOL_TX = 61,                        /* TX  */
  YYSYMBOL_ZEROS = 62,                     /* ZEROS  */
  YYSYMBOL_ADD = 63,                       /* ADD  */
  YYSYMBOL_CONJUGATE = 64,                 /* CONJUGATE  */
  YYSYMBOL_PORT = 65,                      /* PORT  */
  YYSYMBOL_IMPEDANCE = 66,                 /* IMPEDANCE  */
  YYSYMBOL_TARGET = 67,                    /* TARGET  */
  YYSYMBOL_QFACTOR = 68,                   /* QFACTOR  */
  YYSYMBOL_UNIFORM = 69,                   /* UNIFORM  */
  YYSYMBOL_INDUCTIVE = 70,                 /* INDUCTIVE  */
  YYSYMBOL_SKIN = 71,                      /* SKIN  */
  YYSYMBOL_LOSSY = 72,                     /* LOSSY  */
  YYSYMBOL_PREDISTORTED = 73,              /* PREDISTORTED  */
  YYSYMBOL_OPTIMIZE = 74,                  /* OPTIMIZE  */
  YYSYMBOL_ILOSSPP = 75,                   /* ILOSSPP  */
  YYSYMBOL_TRUSTR = 76,                    /* TRUSTR  */
  YYSYMBOL_CUTOFF = 77,                    /* CUTOFF  */
  YYSYMBOL_RATIO = 78,                     /* RATIO  */
  YYSYMBOL_RECOMPUTE = 79,                 /* RECOMPUTE  */
  YYSYMBOL_JACOBIAN = 80,                  /* JACOBIAN  */
  YYSYMBOL_ERRORT = 81,                    /* ERRORT  */
  YYSYMBOL_ONLY = 82,                      /* ONLY  */
  YYSYMBOL_TRANSVERSEJ = 83,               /* TRANSVERSEJ  */
  YYSYMBOL_XTOL = 84,                      /* XTOL  */
  YYSYMBOL_GRADDX = 85,                    /* GRADDX  */
  YYSYMBOL_REMESH = 86,                    /* REMESH  */
  YYSYMBOL_FIRST = 87,                     /* FIRST  */
  YYSYMBOL_DECOMPOSITION = 88,             /* DECOMPOSITION  */
  YYSYMBOL_MATERIAL = 89,                  /* MATERIAL  */
  YYSYMBOL_MODELIZATION = 90,              /* MODELIZATION  */
  YYSYMBOL_COMPONENT = 91,                 /* COMPONENT  */
  YYSYMBOL_SAVE = 92,                      /* SAVE  */
  YYSYMBOL_REDUCTION = 93,                 /* REDUCTION  */
  YYSYMBOL_RELOAD = 94,                    /* RELOAD  */
  YYSYMBOL_NEEDED = 95,                    /* NEEDED  */
  YYSYMBOL_DONE = 96,                      /* DONE  */
  YYSYMBOL_CHANGED = 97,                   /* CHANGED  */
  YYSYMBOL_tMIN = 98,                      /* tMIN  */
  YYSYMBOL_tMAX = 99,                      /* tMAX  */
  YYSYMBOL_PLUSI = 100,                    /* PLUSI  */
  YYSYMBOL_MINUSI = 101,                   /* MINUSI  */
  YYSYMBOL_102_ = 102,                     /* ','  */
  YYSYMBOL_103_ = 103,                     /* '['  */
  YYSYMBOL_104_ = 104,                     /* ']'  */
  YYSYMBOL_105_ = 105,                     /* '{'  */
  YYSYMBOL_106_ = 106,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 107,                 /* $accept  */
  YYSYMBOL_All = 108,                      /* All  */
  YYSYMBOL_PrjDataItems = 109,             /* PrjDataItems  */
  YYSYMBOL_PrjDataItem = 110,              /* PrjDataItem  */
  YYSYMBOL_SFFloat = 111,                  /* SFFloat  */
  YYSYMBOL_SFComplex = 112,                /* SFComplex  */
  YYSYMBOL_SFFloatList = 113,              /* SFFloatList  */
  YYSYMBOL_MFFloat = 114,                  /* MFFloat  */
  YYSYMBOL_SFComplexList = 115,            /* SFComplexList  */
  YYSYMBOL_MFComplex = 116,                /* MFComplex  */
  YYSYMBOL_MFFloatOrMFComplex = 117,       /* MFFloatOrMFComplex  */
  YYSYMBOL_ImportData = 118,               /* ImportData  */
  YYSYMBOL_CAD = 119,                      /* CAD  */
  YYSYMBOL_Units = 120,                    /* Units  */
  YYSYMBOL_Mesh = 121,                     /* Mesh  */
  YYSYMBOL_Mwm = 122,                      /* Mwm  */
  YYSYMBOL_Mor = 123,                      /* Mor  */
  YYSYMBOL_FreqAna = 124,                  /* FreqAna  */
  YYSYMBOL_FilterDesign = 125,             /* FilterDesign  */
  YYSYMBOL_126_1 = 126,                    /* $@1  */
  YYSYMBOL_WorkStatus = 127                /* WorkStatus  */
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
#define YYLAST   351

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  107
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  136
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  358

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   356


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
       2,     2,     2,     2,   102,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   103,     2,   104,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   105,     2,   106,     2,     2,     2,     2,
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
      95,    96,    97,    98,    99,   100,   101
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    97,    97,   101,   102,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   137,   142,   151,   155,   186,
     187,   188,   189,   190,   194,   195,   200,   201,   202,   203,
     204,   208,   209,   214,   215,   269,   273,   280,   284,   291,
     295,   302,   306,   310,   314,   318,   322,   326,   330,   333,
     336,   340,   344,   351,   358,   363,   367,   371,   378,   383,
     388,   392,   397,   401,   406,   410,   414,   418,   422,   426,
     430,   434,   438,   442,   446,   450,   453,   457,   461,   465,
     468,   472,   476,   480,   484,   488,   492,   496,   500,   503,
     507,   511,   517,   521,   527,   531,   538,   542,   546,   550,
     554,   558,   562,   569,   574,   578,   582,   587,   591,   595,
     599,   603,   607,   611,   611,   619,   623,   627,   631,   635,
     639,   643,   647,   651,   654,   657,   660,   666,   670,   674,
     678,   682,   686,   690,   695,   699,   703
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
  "WINDOW", "CURVE", "CIRCLE", "IDEAL", "CANONICAL", "FILTER", "CUSTOM",
  "PASS", "KRYLOV", "ORDER", "RETURNLOSS", "OUTBAND", "MAPPING", "TUNING",
  "METHOD", "SOURCE", "ITERMAX", "AUTOMATIC", "DESIGN", "WITH", "MAPPED",
  "TX", "ZEROS", "ADD", "CONJUGATE", "PORT", "IMPEDANCE", "TARGET",
  "QFACTOR", "UNIFORM", "INDUCTIVE", "SKIN", "LOSSY", "PREDISTORTED",
  "OPTIMIZE", "ILOSSPP", "TRUSTR", "CUTOFF", "RATIO", "RECOMPUTE",
  "JACOBIAN", "ERRORT", "ONLY", "TRANSVERSEJ", "XTOL", "GRADDX", "REMESH",
  "FIRST", "DECOMPOSITION", "MATERIAL", "MODELIZATION", "COMPONENT",
  "SAVE", "REDUCTION", "RELOAD", "NEEDED", "DONE", "CHANGED", "tMIN",
  "tMAX", "PLUSI", "MINUSI", "','", "'['", "']'", "'{'", "'}'", "$accept",
  "All", "PrjDataItems", "PrjDataItem", "SFFloat", "SFComplex",
  "SFFloatList", "MFFloat", "SFComplexList", "MFComplex",
  "MFFloatOrMFComplex", "ImportData", "CAD", "Units", "Mesh", "Mwm", "Mor",
  "FreqAna", "FilterDesign", "$@1", "WorkStatus", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-270)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-3)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -270,    11,   229,  -270,  -270,    18,    26,    39,     2,    40,
      31,   -13,    55,    71,    45,    69,    49,    -5,    56,    70,
      68,    82,   110,    92,   125,    -3,   100,   103,    63,    59,
      85,    77,    61,    79,     4,    81,    86,  -270,  -270,  -270,
    -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,
    -270,    53,   157,    87,   174,   168,    84,    89,   -71,   107,
     108,   166,    -4,   163,   -23,   177,   188,    65,   179,    96,
     144,   165,    28,    19,   162,   143,   171,   195,   121,   149,
     -18,    52,   138,   135,   134,   121,   132,   159,   181,   176,
     169,   151,   160,   153,    34,    91,    76,   121,   206,   211,
     212,   213,   214,    75,   205,   124,   126,   217,   219,   127,
     215,   197,   224,  -270,   228,   230,   232,    22,   209,   223,
     231,   241,   243,   154,   121,   173,   121,   121,   178,   236,
    -270,   121,   261,   238,   121,   263,   265,   266,   244,    30,
     193,   273,   274,   275,   220,   218,   276,   280,   121,  -270,
    -270,  -270,  -270,   121,   281,   282,   284,   285,   121,   286,
     287,   121,    88,   208,   121,   121,  -270,   121,   121,  -270,
     289,   290,   121,   291,   292,   257,   294,   259,   270,   264,
     297,   301,   121,   121,  -270,   121,   302,   304,   121,   121,
    -270,  -270,  -270,  -270,  -270,  -270,   210,   295,   283,   308,
     309,  -270,  -270,   235,   298,   311,  -270,  -270,  -270,  -270,
     318,   121,   321,   323,   324,  -270,  -270,   121,  -270,   325,
     121,   121,   121,   326,   121,  -270,   327,   121,  -270,  -270,
    -270,   121,   328,   121,   121,   329,  -270,  -270,   271,   272,
    -270,  -270,   121,  -270,  -270,  -270,  -270,  -270,  -270,  -270,
    -270,  -270,   332,   333,   334,  -270,  -270,   237,  -270,  -270,
    -270,  -270,   121,  -270,  -270,   330,  -270,   335,   336,   338,
    -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,   267,
     315,   341,  -270,  -270,   121,   343,  -270,  -270,   121,  -270,
    -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,
    -270,   121,  -270,   121,  -270,  -270,   344,   345,  -270,  -270,
    -270,  -270,   121,  -270,  -270,  -270,  -270,   320,  -270,  -270,
    -270,   121,   347,  -270,  -270,  -270,  -270,  -270,  -270,  -270,
    -270,    -2,    10,    13,    17,   348,  -270,  -270,   121,   121,
     121,  -270,  -270,   121,  -270,    29,  -270,  -270,  -270,  -270,
    -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     8,     5,
       6,     7,    11,    12,     9,    10,    13,    37,    35,    36,
      38,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
      16,    15,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,     0,     0,   110,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   116,     0,     0,     0,     0,     0,
      53,   130,   128,   127,   129,   131,     0,     0,     0,     0,
       0,   133,   136,     0,     0,     0,   135,    40,    89,    90,
       0,     0,     0,     0,     0,    41,    44,     0,    43,     0,
       0,     0,     0,     0,     0,    62,     0,     0,    55,    57,
     115,     0,     0,     0,     0,     0,    70,    73,     0,     0,
      72,    85,     0,   108,    76,    75,    77,    79,    78,    81,
      80,    87,     0,     0,     0,    86,    88,     0,   109,   111,
     112,    74,     0,    96,    97,     0,   102,     0,     0,     0,
     126,   123,   124,   125,   118,   121,   117,   119,   120,     0,
       0,     0,   132,   134,     0,     0,    68,    92,     0,    47,
      45,    46,    51,    42,    48,    49,    50,    56,    58,    63,
      54,     0,    94,     0,    60,    95,     0,     0,   103,    82,
      83,    84,    23,    33,    34,   114,    61,     0,    98,    99,
     101,     0,     0,    66,    65,    69,    91,    59,    93,   106,
     105,     0,     0,     0,     0,     0,    64,    67,     0,     0,
       0,    24,    19,     0,    31,     0,    26,    22,    25,    21,
      29,    32,    28,   100,    17,    18,    20,    27
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -270,  -270,  -270,  -270,   -78,  -269,  -270,  -270,  -270,  -270,
    -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,
    -270
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    37,   345,   332,   333,   313,   334,   314,
     315,    38,    39,    40,    41,    42,    43,    44,    45,   257,
      46
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     152,   150,   151,   127,    57,    50,    58,   169,    69,    59,
      89,     3,   154,   150,   151,   124,   150,   151,   125,   190,
     150,   151,    51,    47,   103,   150,   151,   119,   120,    60,
      52,    48,   104,   150,   151,    90,   155,   156,   157,   211,
      91,   138,    92,    70,    49,    55,   218,   178,   220,   221,
     158,   142,   143,   224,    54,    93,   227,    94,    64,   210,
      56,   233,    67,   346,   139,   352,   109,   232,   140,   141,
     242,    62,   179,    65,   357,   243,   128,    68,   144,   184,
     248,   110,   145,   251,    63,    61,   255,   256,   196,   258,
     259,   131,   132,    66,   262,    53,   105,   106,   338,   339,
     340,    71,   341,   197,   272,   273,   159,   274,   160,    72,
     277,   278,   343,    73,   344,   347,   115,   348,   116,   350,
     117,   351,   134,   135,   150,   151,   185,    74,   161,   338,
     339,   162,   186,   288,   163,    87,   164,   165,    88,   292,
      75,    97,   294,   295,   296,    95,   298,   180,    96,   300,
     187,   188,   189,   301,    98,   303,   304,    76,   101,    77,
      78,    79,    80,    81,   308,   181,   182,   183,   252,   253,
     111,    82,   100,    99,   102,    83,   107,    84,    85,   113,
      86,   108,   112,   114,   316,   121,   122,   123,   118,   126,
     129,   130,   133,   136,   137,   146,   147,   148,   149,   153,
     166,   167,   168,   170,   171,   173,   324,   172,   174,   191,
     326,   175,   176,   177,   192,   193,   194,   195,   198,   199,
     201,   200,   202,   327,   205,   328,   203,   206,   204,    -2,
       4,   207,   217,   208,   331,   209,   212,     5,     6,     7,
       8,     9,    10,   336,   215,    11,   216,    12,    13,    14,
     213,   219,    15,   342,    16,   349,   222,    17,   214,    18,
     354,   355,   356,   223,   225,   226,   228,    19,   229,   230,
     231,   234,    20,    21,    22,    23,   235,   236,   237,   240,
     238,    24,   239,   241,   244,   245,    25,   246,   247,   249,
     250,   254,   260,   261,   263,   264,   265,   266,   267,   268,
     270,    26,    27,   269,   271,   275,    28,   276,   280,   279,
     281,   282,   283,   284,   286,    29,    30,    31,    32,    33,
      34,   287,    35,    36,   289,   285,   290,   291,   293,   297,
     299,   302,   305,   306,   307,   309,   310,   311,   318,   319,
     312,   320,   322,   317,   323,   321,   325,   329,   330,   335,
     337,   353
};

static const yytype_int16 yycheck[] =
{
      78,     3,     4,    26,    17,     3,    19,    85,    13,    22,
      13,     0,    30,     3,     4,    19,     3,     4,    22,    97,
       3,     4,    20,     5,    20,     3,     4,    98,    99,    42,
      28,     5,    28,     3,     4,    38,    54,    55,    56,   117,
      43,    13,    45,    48,     5,    14,   124,    13,   126,   127,
      68,    32,    33,   131,    14,    58,   134,    60,    13,    37,
      29,   139,    13,   332,    36,   334,    13,    37,    40,    41,
     148,    16,    38,    28,   343,   153,    99,    28,    59,     3,
     158,    28,    63,   161,    13,    98,   164,   165,    13,   167,
     168,    26,    27,    24,   172,    93,    92,    93,   100,   101,
     102,    45,   104,    28,   182,   183,    54,   185,    56,    39,
     188,   189,   102,    45,   104,   102,    32,   104,    34,   102,
      36,   104,    26,    27,     3,     4,    50,    45,    76,   100,
     101,    79,    56,   211,    82,    43,    84,    85,    13,   217,
      30,    78,   220,   221,   222,    45,   224,    56,    45,   227,
      74,    75,    76,   231,    95,   233,   234,    47,    97,    49,
      50,    51,    52,    53,   242,    74,    75,    76,    80,    81,
      13,    61,    95,    88,    95,    65,    95,    67,    68,     5,
      70,    95,    95,    15,   262,    78,    78,    21,    99,    26,
      13,     3,    13,    49,    29,    33,    53,    26,     3,    50,
      62,    66,    68,    71,    45,    29,   284,    26,    39,     3,
     288,    60,    52,    60,     3,     3,     3,     3,    13,    95,
       3,    95,     3,   301,    27,   303,    99,     3,    13,     0,
       1,     3,    78,     3,   312,     3,    27,     8,     9,    10,
      11,    12,    13,   321,     3,    16,     3,    18,    19,    20,
      27,    78,    23,   331,    25,   333,    78,    28,    27,    30,
     338,   339,   340,    27,     3,    27,     3,    38,     3,     3,
      26,    78,    43,    44,    45,    46,     3,     3,     3,     3,
      60,    52,    64,     3,     3,     3,    57,     3,     3,     3,
       3,    83,     3,     3,     3,     3,    39,     3,    39,    29,
       3,    72,    73,    39,     3,     3,    77,     3,    13,    99,
      27,     3,     3,    78,     3,    86,    87,    88,    89,    90,
      91,     3,    93,    94,     3,    27,     3,     3,     3,     3,
       3,     3,     3,    62,    62,     3,     3,     3,     3,     3,
     103,     3,    27,    13,     3,    78,     3,     3,     3,    29,
       3,     3
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,   108,   109,     0,     1,     8,     9,    10,    11,    12,
      13,    16,    18,    19,    20,    23,    25,    28,    30,    38,
      43,    44,    45,    46,    52,    57,    72,    73,    77,    86,
      87,    88,    89,    90,    91,    93,    94,   110,   118,   119,
     120,   121,   122,   123,   124,   125,   127,     5,     5,     5,
       3,    20,    28,    93,    14,    14,    29,    17,    19,    22,
      42,    98,    16,    13,    13,    28,    24,    13,    28,    13,
      48,    45,    39,    45,    45,    30,    47,    49,    50,    51,
      52,    53,    61,    65,    67,    68,    70,    43,    13,    13,
      38,    43,    45,    58,    60,    45,    45,    78,    95,    88,
      95,    97,    95,    20,    28,    92,    93,    95,    95,    13,
      28,    13,    95,     5,    15,    32,    34,    36,    99,    98,
      99,    78,    78,    21,    19,    22,    26,    26,    99,    13,
       3,    26,    27,    13,    26,    27,    49,    29,    13,    36,
      40,    41,    32,    33,    59,    63,    33,    53,    26,     3,
       3,     4,   111,    50,    30,    54,    55,    56,    68,    54,
      56,    76,    79,    82,    84,    85,    62,    66,    68,   111,
      71,    45,    26,    29,    39,    60,    52,    60,    13,    38,
      56,    74,    75,    76,     3,    50,    56,    74,    75,    76,
     111,     3,     3,     3,     3,     3,    13,    28,    13,    95,
      95,     3,     3,    99,    13,    27,     3,     3,     3,     3,
      37,   111,    27,    27,    27,     3,     3,    78,   111,    78,
     111,   111,    78,    27,   111,     3,    27,   111,     3,     3,
       3,    26,    37,   111,    78,     3,     3,     3,    60,    64,
       3,     3,   111,   111,     3,     3,     3,     3,   111,     3,
       3,   111,    80,    81,    83,   111,   111,   126,   111,   111,
       3,     3,   111,     3,     3,    39,     3,    39,    29,    39,
       3,     3,   111,   111,   111,     3,     3,   111,   111,    99,
      13,    27,     3,     3,    78,    27,     3,     3,   111,     3,
       3,     3,   111,     3,   111,   111,   111,     3,   111,     3,
     111,   111,     3,   111,   111,     3,    62,    62,   111,     3,
       3,     3,   103,   114,   116,   117,   111,    13,     3,     3,
       3,    78,    27,     3,   111,     3,   111,   111,   111,     3,
       3,   111,   112,   113,   115,    29,   111,     3,   100,   101,
     102,   104,   111,   102,   104,   111,   112,   102,   104,   111,
     102,   104,   112,     3,   111,   111,   111,   112
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,   107,   108,   109,   109,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   111,   111,   112,   112,   113,
     113,   113,   113,   113,   114,   114,   115,   115,   115,   115,
     115,   116,   116,   117,   117,   118,   118,   119,   119,   120,
     120,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   122,   123,   123,   123,   123,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   126,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     2,
       3,     2,     2,     0,     3,     3,     2,     3,     2,     2,
       0,     3,     3,     1,     1,     2,     2,     2,     2,     3,
       4,     4,     5,     4,     4,     5,     5,     5,     5,     5,
       5,     5,     3,     3,     5,     4,     5,     4,     5,     6,
       5,     5,     4,     5,     6,     6,     5,     6,     5,     6,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     5,     5,     5,     4,     4,     4,     4,     4,
       4,     6,     5,     6,     5,     5,     4,     4,     5,     5,
       7,     5,     4,     5,     3,     6,     6,     3,     4,     4,
       3,     4,     4,     0,     5,     4,     3,     4,     4,     4,
       4,     4,     3,     4,     4,     4,     4,     3,     3,     3,
       3,     3,     4,     3,     4,     4,     3
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
#line 116 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                        {YYABORT;}
#line 1471 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 15: /* SFFloat: FLOATING  */
#line 138 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                {
		      (yyval.fval) = (yyvsp[0].fval);
		      if(ProjData::Fbuff) ProjData::Fbuff->push_back((yyvsp[0].fval));
		}
#line 1480 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 16: /* SFFloat: INTEGER  */
#line 143 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                {
		       (yyval.fval) = (double)(yyvsp[0].ival);
		       if(ProjData::Fbuff) ProjData::Fbuff->push_back((double)(yyvsp[0].ival));
		}
#line 1489 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 17: /* SFComplex: SFFloat PLUSI SFFloat  */
#line 152 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                {
		       if(ProjData::Cbuff) ProjData::Cbuff->push_back(std::complex<double>((yyvsp[-2].fval),(yyvsp[0].fval)));
		}
#line 1497 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 18: /* SFComplex: SFFloat MINUSI SFFloat  */
#line 156 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                {
		       if(ProjData::Cbuff) ProjData::Cbuff->push_back(std::complex<double>((yyvsp[-2].fval),-(yyvsp[0].fval)));
		}
#line 1505 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 24: /* MFFloat: '[' SFFloat ']'  */
#line 194 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                          {}
#line 1511 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 25: /* MFFloat: '[' SFFloatList ']'  */
#line 195 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                              {}
#line 1517 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 31: /* MFComplex: '[' SFComplex ']'  */
#line 208 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                            {}
#line 1523 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 32: /* MFComplex: '[' SFComplexList ']'  */
#line 209 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                                {}
#line 1529 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 33: /* MFFloatOrMFComplex: MFFloat  */
#line 214 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                  {}
#line 1535 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 34: /* MFFloatOrMFComplex: MFComplex  */
#line 215 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                    {}
#line 1541 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 35: /* ImportData: VARFILEPATH STRING  */
#line 270 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.varFilePath=(yyvsp[0].sval);
            }
#line 1549 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 36: /* ImportData: S2PFILEPATH STRING  */
#line 274 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.s2pFilePath=(yyvsp[0].sval);
            }
#line 1557 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 37: /* CAD: MAINASSNAME STRING  */
#line 281 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.mainAssName=(yyvsp[0].sval);
            }
#line 1565 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 38: /* CAD: NETWORK INTEGER  */
#line 285 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.network=(yyvsp[0].ival);
            }
#line 1573 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 39: /* Units: LENGTH UNIT STRING  */
#line 292 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              strcpy(prjData.lengthUnitName,(yyvsp[0].sval));
            }
#line 1581 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 40: /* Units: FREQ UNIT EXP INTEGER  */
#line 296 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqUnitE =(yyvsp[0].ival);
            }
#line 1589 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 41: /* Mesh: MESH WAVELENGTH RATIO INTEGER  */
#line 303 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshPerWavelen=(yyvsp[0].ival);
            }
#line 1597 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 42: /* Mesh: SHARED MESH WAVELENGTH RATIO INTEGER  */
#line 307 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.sharedMeshPerWavelen=(yyvsp[0].ival);
            }
#line 1605 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 43: /* Mesh: SHARED MESH REFINE SFFloat  */
#line 311 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.sharedMeshRefine=(yyvsp[0].fval);
            }
#line 1613 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 44: /* Mesh: MESH CIRCLE RATIO INTEGER  */
#line 315 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshPerCircle=(yyvsp[0].ival);
            }
#line 1621 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 45: /* Mesh: MESH REFINE tMIN NUM INTEGER  */
#line 319 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshRefineMinNum=(yyvsp[0].ival);
            }
#line 1629 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 46: /* Mesh: MESH REFINE tMAX NUM INTEGER  */
#line 323 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshRefineMaxNum=(yyvsp[0].ival);
            }
#line 1637 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 47: /* Mesh: MESH TET tMAX NUM INTEGER  */
#line 327 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshTetMaxNum=(yyvsp[0].ival);
            }
#line 1645 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 48: /* Mesh: REFINE FREQ BAND SFFloat SFFloat  */
#line 331 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 1652 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 49: /* Mesh: RESONANCE FREQ BAND SFFloat SFFloat  */
#line 334 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 1659 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 50: /* Mesh: RESONANCE FREQ tMAX RATIO SFFloat  */
#line 337 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.resonFreqMaxRatio=(yyvsp[0].fval);
            }
#line 1667 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 51: /* Mesh: MESH tMIN ENERGY RATIO SFFloat  */
#line 341 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.meshMinEnergyRatio=(yyvsp[0].fval);
            }
#line 1675 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 52: /* Mesh: LOCAL MESHING3D INTEGER  */
#line 345 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.localMeshing3d=(yyvsp[0].ival);
            }
#line 1683 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 53: /* Mwm: CUTOFF RATIO SFFloat  */
#line 352 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
           {
              prjData.cutoffRatio=(yyvsp[0].fval);
           }
#line 1691 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 54: /* Mor: MOR FREQ BAND SFFloat SFFloat  */
#line 359 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqBand[0]=(yyvsp[-1].fval);
              prjData.freqBand[1]=(yyvsp[0].fval);
            }
#line 1700 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 55: /* Mor: MOR FREQ NUM INTEGER  */
#line 364 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.MORFreqNum=(yyvsp[0].ival);
            }
#line 1708 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 56: /* Mor: RESONANCE MOR FREQ NUM INTEGER  */
#line 368 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.MORFreqNum1=(yyvsp[0].ival);
            }
#line 1716 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 57: /* Mor: MOR KRYLOV ORDER INTEGER  */
#line 372 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.KrylovOrder=(yyvsp[0].ival);
            }
#line 1724 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 58: /* FreqAna: ANA FREQ BAND SFFloat SFFloat  */
#line 379 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.anaFreqBand[0]=(yyvsp[-1].fval);
              prjData.anaFreqBand[1]=(yyvsp[0].fval);
            }
#line 1733 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 59: /* FreqAna: ZERO POLE FREQ BAND SFFloat SFFloat  */
#line 384 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.zpFreqBand[0]=(yyvsp[-1].fval);
              prjData.zpFreqBand[1]=(yyvsp[0].fval);
            }
#line 1742 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 60: /* FreqAna: ZERO POLE WINDOW RATIO SFFloat  */
#line 389 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.zpWinRatio=(yyvsp[0].fval);
            }
#line 1750 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 61: /* FreqAna: MAPPING FREQ BAND SFFloat SFFloat  */
#line 393 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.mapFreqBand[0]=(yyvsp[-1].fval);
              prjData.mapFreqBand[1]=(yyvsp[0].fval);
            }
#line 1759 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 62: /* FreqAna: ANA FREQ NUM INTEGER  */
#line 398 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.anaFreqNum=(yyvsp[0].ival);
            }
#line 1767 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 63: /* FreqAna: ANA MOR FREQ NUM INTEGER  */
#line 402 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              if(prjData.network) prjData.netMORFreqNum=(yyvsp[0].ival);
              else                prjData.cmpMORFreqNum=(yyvsp[0].ival);
            }
#line 1776 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 64: /* FreqAna: COMPONENT RESONANCE FREQ tMAX RATIO SFFloat  */
#line 407 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.cmpResonFreqMaxRatio=(yyvsp[0].fval);
            }
#line 1784 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 65: /* FreqAna: NETWORK RESONANCE FREQ tMAX RATIO SFFloat  */
#line 411 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.netResonFreqMaxRatio=(yyvsp[0].fval);
            }
#line 1792 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 66: /* FreqAna: COMPONENT MOR FREQ NUM INTEGER  */
#line 415 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.cmpMORFreqNum=(yyvsp[0].ival);
            }
#line 1800 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 67: /* FreqAna: COMPONENT RESONANCE MOR FREQ NUM INTEGER  */
#line 419 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.cmpMORFreqNum1=(yyvsp[0].ival);
            }
#line 1808 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 68: /* FreqAna: NETWORK MOR FREQ NUM INTEGER  */
#line 423 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.netMORFreqNum=(yyvsp[0].ival);
            }
#line 1816 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 69: /* FreqAna: NETWORK RESONANCE MOR FREQ NUM INTEGER  */
#line 427 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.netMORFreqNum1=(yyvsp[0].ival);
            }
#line 1824 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 70: /* FreqAna: IDEAL FILTER TYPE INTEGER  */
#line 431 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.idealFilterType=(yyvsp[0].ival);
            }
#line 1832 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 71: /* FreqAna: IDEAL FILTER TYPE INTEGER  */
#line 435 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.idealFilterType=(yyvsp[0].ival);
            }
#line 1840 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 72: /* FreqAna: CANONICAL FILTER TOPOLOGY INTEGER  */
#line 439 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.canonicalFilterTopology=(yyvsp[0].ival);
            }
#line 1848 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 73: /* FreqAna: IDEAL FILTER TOPOLOGY INTEGER  */
#line 443 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.canonicalFilterTopology=(yyvsp[0].ival);
            }
#line 1856 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 74: /* FreqAna: CUSTOM IDEAL FILTER INTEGER  */
#line 447 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.customIdealFilter=(yyvsp[0].ival);
            }
#line 1864 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 75: /* FreqAna: FILTER MAPPING METHOD INTEGER  */
#line 451 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 1871 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 76: /* FreqAna: FILTER MAPPING SYMMETRIC INTEGER  */
#line 454 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filtermapSymmetric=(yyvsp[0].ival);
            }
#line 1879 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 77: /* FreqAna: FILTER MAPPING SOURCE INTEGER  */
#line 458 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filtermapSource=(yyvsp[0].ival);
            }
#line 1887 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 78: /* FreqAna: FILTER MAPPING QFACTOR SFFloat  */
#line 462 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filtermapQfactor = (yyvsp[0].fval);
            }
#line 1895 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 79: /* FreqAna: FILTER MAPPING ITERMAX INTEGER  */
#line 466 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 1902 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 80: /* FreqAna: FILTER TUNING ITERMAX INTEGER  */
#line 469 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneItermax=(yyvsp[0].ival);
            }
#line 1910 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 81: /* FreqAna: FILTER TUNING METHOD INTEGER  */
#line 473 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneMethod=(yyvsp[0].ival);
            }
#line 1918 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 82: /* FreqAna: FILTER TUNING RECOMPUTE JACOBIAN INTEGER  */
#line 477 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneRecomputeJaco=(yyvsp[0].ival);
            }
#line 1926 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 83: /* FreqAna: FILTER TUNING RECOMPUTE ERRORT INTEGER  */
#line 481 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneRecomputeError=(yyvsp[0].ival);
            }
#line 1934 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 84: /* FreqAna: FILTER TUNING ONLY TRANSVERSEJ INTEGER  */
#line 485 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneOnlyJt=(yyvsp[0].ival);
            }
#line 1942 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 85: /* FreqAna: FILTER SYMMETRIC TUNING INTEGER  */
#line 489 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterSymmetricTuning=(yyvsp[0].ival);
            }
#line 1950 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 86: /* FreqAna: FILTER TUNING XTOL SFFloat  */
#line 493 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneXtol=(yyvsp[0].fval);
            }
#line 1958 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 87: /* FreqAna: FILTER TUNING TRUSTR SFFloat  */
#line 497 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTuneTrustR=(yyvsp[0].fval);
            }
#line 1966 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 88: /* FreqAna: FILTER TUNING GRADDX SFFloat  */
#line 501 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 1973 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 89: /* FreqAna: FREQ RESPONSE TYPE INTEGER  */
#line 504 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqRespParType = (FreqRespParType) (yyvsp[0].ival);
            }
#line 1981 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 90: /* FreqAna: FREQ RESPONSE PART INTEGER  */
#line 508 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqRespParPart = (yyvsp[0].ival);
            }
#line 1989 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 91: /* FreqAna: FREQ RESPONSE YSCALE SFFloat SFFloat SFFloat  */
#line 512 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqRespYscale[0]=(yyvsp[-2].fval);
              prjData.freqRespYscale[1]=(yyvsp[-1].fval);
              prjData.freqRespYscale[2]=(yyvsp[0].fval);
            }
#line 1999 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 92: /* FreqAna: FREQ RESPONSE YSCALE AUTO INTEGER  */
#line 518 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.freqRespYscaleAuto=(yyvsp[0].ival);
            }
#line 2007 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 93: /* FreqAna: ZERO POLE YSCALE SFFloat SFFloat SFFloat  */
#line 522 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.zeropoleYscale[0]=(yyvsp[-2].fval);
              prjData.zeropoleYscale[1]=(yyvsp[-1].fval);
              prjData.zeropoleYscale[2]=(yyvsp[0].fval);
            }
#line 2017 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 94: /* FreqAna: ZERO POLE YSCALE AUTO INTEGER  */
#line 528 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.zeropoleYscaleAuto=(yyvsp[0].ival);
            }
#line 2025 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 95: /* FreqAna: ZERO POLE CURVE INTEGER INTEGER  */
#line 532 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              std::pair<int, int> Sij_ports;
              Sij_ports.first=(yyvsp[-1].ival);
              Sij_ports.second=(yyvsp[0].ival);
              prjData.zeropoleCurves.insert(Sij_ports);
            }
#line 2036 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 96: /* FreqAna: AUTOMATIC FREQ RESPONSE INTEGER  */
#line 539 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoFreqResponse=(yyvsp[0].ival);
            }
#line 2044 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 97: /* FreqAna: AUTOMATIC ZERO POLE INTEGER  */
#line 543 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoZeropole=(yyvsp[0].ival);
            }
#line 2052 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 98: /* FreqAna: AUTOMATIC DESIGN MAPPED POLE INTEGER  */
#line 547 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoDesignWithMappedTz=(yyvsp[0].ival);
            }
#line 2060 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 99: /* FreqAna: AUTOMATIC MAPPED FREQ RESPONSE INTEGER  */
#line 551 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoMappedFreqResponse=(yyvsp[0].ival);
            }
#line 2068 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 100: /* FreqAna: AUTOMATIC IDEAL MAPPED POLE FREQ RESPONSE INTEGER  */
#line 555 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoIdealMappedTzFreqResponse=(yyvsp[0].ival);
            }
#line 2076 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 101: /* FreqAna: AUTOMATIC MAPPED ZERO POLE INTEGER  */
#line 559 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoMappedZeropole=(yyvsp[0].ival);
            }
#line 2084 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 102: /* FreqAna: AUTOMATIC FILTER MAPPING INTEGER  */
#line 563 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.autoFilterMapping=(yyvsp[0].ival);
            }
#line 2092 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 103: /* FilterDesign: FILTER PASS BAND SFFloat SFFloat  */
#line 570 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterPassBand[0]=(yyvsp[-1].fval);
              prjData.filterPassBand[1]=(yyvsp[0].fval);
            }
#line 2101 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 104: /* FilterDesign: FILTER ORDER INTEGER  */
#line 575 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterOrder = (yyvsp[0].ival);
            }
#line 2109 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 105: /* FilterDesign: IDEAL FILTER ADD CONJUGATE ZEROS INTEGER  */
#line 579 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.idealFilterAddConjugateTZ = (yyvsp[0].ival);
            }
#line 2117 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 106: /* FilterDesign: IDEAL FILTER WITH MAPPED ZEROS INTEGER  */
#line 583 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.idealFilterWithMappedTZ = (yyvsp[0].ival);
              if(prjData.idealFilterWithMappedTZ) prjData.idealFilterAddConjugateTZ=0;
            }
#line 2126 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 107: /* FilterDesign: FILTER RETURNLOSS SFFloat  */
#line 588 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterRetLoss = (yyvsp[0].fval);
            }
#line 2134 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 108: /* FilterDesign: FILTER OUTBAND RETURNLOSS SFFloat  */
#line 592 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterOutbandRetLoss = (yyvsp[0].fval);
            }
#line 2142 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 109: /* FilterDesign: FILTER PORT IMPEDANCE SFFloat  */
#line 596 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterPortImpedance = (yyvsp[0].fval);
            }
#line 2150 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 110: /* FilterDesign: FILTER QFACTOR SFFloat  */
#line 600 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterQfactor = (yyvsp[0].fval);
            }
#line 2158 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 111: /* FilterDesign: FILTER TARGET QFACTOR SFFloat  */
#line 604 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterTargetQfactor = (yyvsp[0].fval);
            }
#line 2166 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 112: /* FilterDesign: FILTER INDUCTIVE SKIN INTEGER  */
#line 608 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.filterInductiveSkin = (yyvsp[0].ival);
            }
#line 2174 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 113: /* $@1: %empty  */
#line 611 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
                           {ProjData::Cbuff =&prjData.filterZeros; ProjData::Cbuff->clear();
                            ProjData::Fbuff =&ProjData::Fvec; ProjData::Fvec.clear();}
#line 2181 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 114: /* FilterDesign: FILTER TX ZEROS $@1 MFFloatOrMFComplex  */
#line 613 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              if(prjData.filterZeros.size()==0){
               for (int i=0; i<ProjData::Fvec.size() ; i++) prjData.filterZeros.push_back(std::complex<double>(ProjData::Fvec[i],0.0));
               ProjData::Fbuff=NULL;
              } else ProjData::Cbuff=NULL;
            }
#line 2192 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 115: /* FilterDesign: SYMMETRIC FILTER RESPONSE INTEGER  */
#line 620 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.symmFilterResponse = (yyvsp[0].ival);
            }
#line 2200 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 116: /* FilterDesign: PREDISTORTED FILTER INTEGER  */
#line 624 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistortedFilter = (yyvsp[0].ival);
            }
#line 2208 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 117: /* FilterDesign: PREDISTORTED FILTER OPTIMIZE INTEGER  */
#line 628 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistFilterOptim = (yyvsp[0].ival);
            }
#line 2216 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 118: /* FilterDesign: PREDISTORTED FILTER RETURNLOSS SFFloat  */
#line 632 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimRL = (yyvsp[0].fval);
            }
#line 2224 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 119: /* FilterDesign: PREDISTORTED FILTER ILOSSPP SFFloat  */
#line 636 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimILpp = (yyvsp[0].fval);
            }
#line 2232 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 120: /* FilterDesign: PREDISTORTED FILTER TRUSTR SFFloat  */
#line 640 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimTrustR = (yyvsp[0].fval);
            }
#line 2240 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 121: /* FilterDesign: PREDISTORTED FILTER ITERMAX INTEGER  */
#line 644 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimIterMax = (yyvsp[0].ival);
            }
#line 2248 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 122: /* FilterDesign: PREDISTORTED FILTER INTEGER  */
#line 648 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.predistortedFilter = (yyvsp[0].ival);
            }
#line 2256 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 123: /* FilterDesign: LOSSY FILTER OPTIMIZE INTEGER  */
#line 652 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 2263 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 124: /* FilterDesign: LOSSY FILTER ILOSSPP SFFloat  */
#line 655 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 2270 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 125: /* FilterDesign: LOSSY FILTER TRUSTR SFFloat  */
#line 658 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 2277 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 126: /* FilterDesign: LOSSY FILTER ITERMAX INTEGER  */
#line 661 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
            }
#line 2284 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 127: /* WorkStatus: DECOMPOSITION NEEDED INTEGER  */
#line 667 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.decompositionNeeded=(yyvsp[0].ival);
            }
#line 2292 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 128: /* WorkStatus: FIRST DECOMPOSITION INTEGER  */
#line 671 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.firstDecomposition=(yyvsp[0].ival);
            }
#line 2300 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 129: /* WorkStatus: MATERIAL CHANGED INTEGER  */
#line 675 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.materialChanged=(yyvsp[0].ival);
            }
#line 2308 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 130: /* WorkStatus: REMESH NEEDED INTEGER  */
#line 679 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.remeshNeeded=(yyvsp[0].ival);
            }
#line 2316 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 131: /* WorkStatus: MODELIZATION NEEDED INTEGER  */
#line 683 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.modelizationNeeded=(yyvsp[0].ival);
            }
#line 2324 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 132: /* WorkStatus: COMPONENT SAVE NEEDED INTEGER  */
#line 687 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.componentsaveNeeded=(yyvsp[0].ival);
            }
#line 2332 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 133: /* WorkStatus: REDUCTION NEEDED INTEGER  */
#line 691 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.cmpReductionNeeded=(yyvsp[0].ival);
              prjData.workStatus.netReductionNeeded=(yyvsp[0].ival);
            }
#line 2341 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 134: /* WorkStatus: COMPONENT REDUCTION NEEDED INTEGER  */
#line 696 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.cmpReductionNeeded=(yyvsp[0].ival);
            }
#line 2349 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 135: /* WorkStatus: NETWORK REDUCTION NEEDED INTEGER  */
#line 700 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.netReductionNeeded=(yyvsp[0].ival);
            }
#line 2357 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;

  case 136: /* WorkStatus: RELOAD NEEDED INTEGER  */
#line 704 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"
            {
              prjData.workStatus.reloadNeeded=(yyvsp[0].ival);
            }
#line 2365 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"
    break;


#line 2369 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData_parser.cpp"

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

#line 708 "/home/walter/MwCAD/EmCAD/EmCAD-dev/src/GUI/projectData.y"



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




