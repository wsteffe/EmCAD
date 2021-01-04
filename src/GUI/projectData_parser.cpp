/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

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
#line 22 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"

//debug:
#define YYDEBUG 1
#define YYPRINT(file, type, value) yyprint(file, type, value)
//debug

#include "projectData.h"
#include <Message.h>
#include <vector>

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
}


#line 121 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"

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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED
# define YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int projectDatadebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INTEGER = 258,
    FLOATING = 259,
    STRING = 260,
    NAME = 261,
    DEF = 262,
    MAINASSNAME = 263,
    VARFILEPATH = 264,
    S2PFILEPATH = 265,
    NETWORK = 266,
    LENGTH = 267,
    FREQ = 268,
    UNIT = 269,
    EXP = 270,
    MESH = 271,
    REFINE = 272,
    ENERGY = 273,
    WAVELENGTH = 274,
    LOCAL = 275,
    MESHING3D = 276,
    ANA = 277,
    BAND = 278,
    NUM = 279,
    MOR = 280,
    RESPONSE = 281,
    SYMMETRIC = 282,
    PARAMETER = 283,
    TYPE = 284,
    TOPOLOGY = 285,
    PART = 286,
    XSCALE = 287,
    YSCALE = 288,
    AUTO = 289,
    ZERO = 290,
    POLE = 291,
    WINDOW = 292,
    CURVE = 293,
    CIRCLE = 294,
    IDEAL = 295,
    CANONICAL = 296,
    FILTER = 297,
    CUSTOM = 298,
    PASS = 299,
    KRYLOV = 300,
    ORDER = 301,
    RETURNLOSS = 302,
    OUTBAND = 303,
    MAPPING = 304,
    TUNING = 305,
    METHOD = 306,
    SOURCE = 307,
    ITERMAX = 308,
    AUTOMATIC = 309,
    DESIGN = 310,
    MAPPED = 311,
    TX = 312,
    ZEROS = 313,
    PORT = 314,
    IMPEDANCE = 315,
    TARGET = 316,
    QFACTOR = 317,
    UNIFORM = 318,
    INDUCTIVE = 319,
    SKIN = 320,
    LOSSY = 321,
    PREDISTORTED = 322,
    OPTIMIZE = 323,
    ILOSSPP = 324,
    TRUSTR = 325,
    CUTOFF = 326,
    RATIO = 327,
    RECOMPUTE = 328,
    JACOBIAN = 329,
    ERRORT = 330,
    ONLY = 331,
    TRANSVERSEJ = 332,
    XTOL = 333,
    GRADDX = 334,
    REMESH = 335,
    FIRST = 336,
    DECOMPOSITION = 337,
    MATERIAL = 338,
    MODELIZATION = 339,
    COMPONENT = 340,
    SAVE = 341,
    REDUCTION = 342,
    RELOAD = 343,
    NEEDED = 344,
    DONE = 345,
    CHANGED = 346,
    tMIN = 347,
    tMAX = 348
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 66 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"

int	ival;
double	fval;
char	*sval;

#line 273 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE projectDatalval;

int projectDataparse (void);

#endif /* !YY_PROJECTDATA_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_GUI_PROJECTDATA_PARSER_HPP_INCLUDED  */



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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


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
#define YYLAST   269

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  99
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  17
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  273

#define YYUNDEFTOK  2
#define YYMAXUTOK   348


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    94,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    95,     2,    96,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    97,     2,    98,     2,     2,     2,     2,
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
      85,    86,    87,    88,    89,    90,    91,    92,    93
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    93,    93,    97,    98,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   131,   135,   167,   168,   169,
     170,   174,   175,   228,   232,   239,   243,   250,   254,   261,
     265,   269,   273,   278,   282,   289,   296,   301,   305,   312,
     317,   322,   326,   331,   335,   339,   343,   347,   351,   355,
     358,   362,   366,   370,   373,   377,   381,   385,   389,   393,
     397,   401,   405,   409,   413,   417,   423,   427,   433,   437,
     444,   448,   452,   456,   460,   464,   468,   475,   480,   484,
     488,   492,   496,   500,   504,   508,   508,   512,   516,   520,
     524,   528,   532,   536,   540,   544,   547,   550,   553,   559,
     563,   567,   571,   575,   579,   583,   587
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTEGER", "FLOATING", "STRING", "NAME",
  "DEF", "MAINASSNAME", "VARFILEPATH", "S2PFILEPATH", "NETWORK", "LENGTH",
  "FREQ", "UNIT", "EXP", "MESH", "REFINE", "ENERGY", "WAVELENGTH", "LOCAL",
  "MESHING3D", "ANA", "BAND", "NUM", "MOR", "RESPONSE", "SYMMETRIC",
  "PARAMETER", "TYPE", "TOPOLOGY", "PART", "XSCALE", "YSCALE", "AUTO",
  "ZERO", "POLE", "WINDOW", "CURVE", "CIRCLE", "IDEAL", "CANONICAL",
  "FILTER", "CUSTOM", "PASS", "KRYLOV", "ORDER", "RETURNLOSS", "OUTBAND",
  "MAPPING", "TUNING", "METHOD", "SOURCE", "ITERMAX", "AUTOMATIC",
  "DESIGN", "MAPPED", "TX", "ZEROS", "PORT", "IMPEDANCE", "TARGET",
  "QFACTOR", "UNIFORM", "INDUCTIVE", "SKIN", "LOSSY", "PREDISTORTED",
  "OPTIMIZE", "ILOSSPP", "TRUSTR", "CUTOFF", "RATIO", "RECOMPUTE",
  "JACOBIAN", "ERRORT", "ONLY", "TRANSVERSEJ", "XTOL", "GRADDX", "REMESH",
  "FIRST", "DECOMPOSITION", "MATERIAL", "MODELIZATION", "COMPONENT",
  "SAVE", "REDUCTION", "RELOAD", "NEEDED", "DONE", "CHANGED", "tMIN",
  "tMAX", "','", "'['", "']'", "'{'", "'}'", "$accept", "All",
  "PrjDataItems", "PrjDataItem", "SFFloat", "SFFloatList", "MFFloat",
  "ImportData", "CAD", "Units", "Mesh", "Mwm", "Mor", "FreqAna",
  "FilterDesign", "$@1", "WorkStatus", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,    44,    91,    93,   123,   125
};
# endif

#define YYPACT_NINF (-77)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-3)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -77,    16,   140,   -77,   -77,    19,    26,    31,     8,     7,
      -6,   -12,    28,    23,     4,    -3,    41,    37,    49,    51,
     151,    21,    42,    30,    56,    57,    32,    20,    18,    24,
      14,    27,    33,    36,    38,   -77,   -77,   -77,   -77,   -77,
     -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   105,
      97,    25,    29,    58,    59,    96,    98,   129,    -1,   120,
      10,    88,   109,    13,    39,   106,    87,   115,   139,    86,
      99,   -14,    50,    85,    94,    83,    86,    82,   113,   135,
     133,   125,   108,   117,   112,    -7,     9,     6,    86,   166,
     168,   170,   171,   173,    95,   174,   182,   -77,   183,   184,
     185,    11,   169,   187,   189,   124,    86,   -77,    86,   199,
     179,    86,   201,   202,   206,   191,    15,   144,   214,   215,
     216,   223,   226,    86,   -77,   -77,   -77,   -77,    86,   227,
     228,   229,   230,    86,   231,   232,    86,    22,   159,    86,
      86,   -77,    86,    86,   -77,   234,   235,    86,   236,   237,
     205,   239,   207,   218,   209,   243,   244,    86,    86,   -77,
      86,   245,   246,    86,    86,   -77,   -77,   -77,   -77,   -77,
     -77,   247,   -77,   -77,   -77,   -77,   -77,   248,    86,   249,
     -77,   -77,    86,    86,    86,   -77,   250,    86,   -77,   -77,
     -77,    86,   251,    86,    86,   252,   -77,   -77,   -77,   -77,
      86,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,
     253,   254,   255,   -77,   -77,     0,   -77,   -77,   -77,   -77,
      86,   -77,   -77,   256,   -77,   257,   258,   259,   -77,   -77,
     -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,    86,
     -77,   -77,   -77,   -77,   -77,   -77,    86,   -77,    86,   -77,
     -77,   -77,   -77,   -77,   -77,    86,   -77,   -77,   -77,   233,
     -77,   -77,   -77,   -77,   -77,   -77,   -77,    -2,   260,   -77,
     -77,   -77,   -77
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     0,     1,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     8,     5,     6,     7,
      11,    12,     9,    10,    13,    25,    23,    24,    26,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    27,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,    16,    15,    79,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,    35,   102,   100,    99,   101,
     103,     0,   105,   106,    28,    63,    64,     0,     0,     0,
      29,    30,     0,     0,     0,    43,     0,     0,    37,    38,
      87,     0,     0,     0,     0,     0,    45,    47,    46,    59,
       0,    80,    50,    49,    51,    53,    52,    55,    54,    61,
       0,     0,     0,    60,    62,     0,    81,    83,    84,    48,
       0,    70,    71,     0,    76,     0,     0,     0,    98,    95,
      96,    97,    90,    93,    89,    91,    92,   104,    66,     0,
      31,    33,    32,    39,    44,    36,     0,    68,     0,    41,
      69,    77,    56,    57,    58,    20,    21,    86,    42,     0,
      72,    73,    75,    65,    40,    67,    17,     0,     0,    19,
      22,    18,    74
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -77,   -77,   -77,   -77,   -76,   -77,   -77,   -77,   -77,   -77,
     -77,   -77,   -77,   -77,   -77,   -77,   -77
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    35,   127,   267,   257,    36,    37,    38,
      39,    40,    41,    42,    43,   215,    44
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     144,   125,   126,   125,   126,    52,   153,    53,    50,   159,
      60,    48,   165,   129,   125,   126,     3,    58,   125,   126,
      51,    49,   108,   109,    45,   178,   115,    54,   154,    59,
     183,    46,   184,   111,   112,   187,    47,   130,   131,   132,
     193,    56,    61,    80,    57,   177,   116,   200,   133,   192,
     117,   118,   201,   160,    99,    79,   100,   206,   101,   161,
     209,    78,   155,   213,   214,    81,   216,   217,   119,   120,
      82,   220,    83,    63,   162,   163,   164,   156,   157,   158,
      55,   230,   231,    62,   232,    84,    85,   235,   236,   125,
     126,    64,   269,    65,   270,   255,   210,   211,    86,    87,
      90,   134,   239,   135,    88,    92,   241,   242,   243,    89,
      97,   245,    98,    91,   105,   246,    93,   248,   249,    94,
     136,   106,   102,   137,   251,    95,   138,    96,   139,   140,
     103,   104,   107,   110,   113,   114,   121,   122,   123,   256,
      -2,     4,   124,   141,   258,   143,   128,   145,     5,     6,
       7,     8,     9,    10,   142,   146,    11,    12,   147,   148,
      13,   149,    14,   263,   150,    15,   151,    16,   152,   166,
     264,   167,   265,   168,   169,    17,   170,   172,    66,   266,
      18,    19,    20,    21,   171,   173,   174,   175,   176,    22,
     180,   271,   181,   179,    23,    67,   182,    68,    69,    70,
      71,    72,   185,   186,   188,   189,    24,    25,    73,   190,
      74,    26,    75,    76,   191,    77,   194,   195,   196,   197,
      27,    28,    29,    30,    31,    32,   198,    33,    34,   199,
     202,   203,   204,   205,   207,   208,   212,   218,   219,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   233,   234,
     237,   238,   240,   244,   247,   250,   252,   253,   254,   268,
     260,   261,   262,   272,     0,     0,     0,     0,     0,   259
};

static const yytype_int16 yycheck[] =
{
      76,     3,     4,     3,     4,    17,    13,    19,    14,     3,
      13,     3,    88,    27,     3,     4,     0,    13,     3,     4,
      26,    14,    23,    24,     5,   101,    13,    39,    35,    25,
     106,     5,   108,    23,    24,   111,     5,    51,    52,    53,
     116,    13,    45,    13,    21,    34,    33,   123,    62,    34,
      37,    38,   128,    47,    29,    13,    31,   133,    33,    53,
     136,    40,    53,   139,   140,    35,   142,   143,    29,    30,
      40,   147,    42,    36,    68,    69,    70,    68,    69,    70,
      92,   157,   158,    42,   160,    55,    56,   163,   164,     3,
       4,    42,    94,    42,    96,    95,    74,    75,    42,    42,
      82,    51,   178,    53,    72,    91,   182,   183,   184,    89,
       5,   187,    15,    89,    18,   191,    89,   193,   194,    86,
      70,    23,    93,    73,   200,    89,    76,    89,    78,    79,
      72,    72,     3,    13,    46,    26,    30,    50,    23,   215,
       0,     1,     3,    58,   220,    62,    47,    65,     8,     9,
      10,    11,    12,    13,    60,    42,    16,    17,    23,    26,
      20,    36,    22,   239,    56,    25,    49,    27,    56,     3,
     246,     3,   248,     3,     3,    35,     3,     3,    27,   255,
      40,    41,    42,    43,    89,     3,     3,     3,     3,    49,
       3,   267,     3,    24,    54,    44,    72,    46,    47,    48,
      49,    50,     3,    24,     3,     3,    66,    67,    57,     3,
      59,    71,    61,    62,    23,    64,    72,     3,     3,     3,
      80,    81,    82,    83,    84,    85,     3,    87,    88,     3,
       3,     3,     3,     3,     3,     3,    77,     3,     3,     3,
       3,    36,     3,    36,    26,    36,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,    26,
       3,     3,     3,     3,    -1,    -1,    -1,    -1,    -1,    13
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,   100,   101,     0,     1,     8,     9,    10,    11,    12,
      13,    16,    17,    20,    22,    25,    27,    35,    40,    41,
      42,    43,    49,    54,    66,    67,    71,    80,    81,    82,
      83,    84,    85,    87,    88,   102,   106,   107,   108,   109,
     110,   111,   112,   113,   115,     5,     5,     5,     3,    14,
      14,    26,    17,    19,    39,    92,    13,    21,    13,    25,
      13,    45,    42,    36,    42,    42,    27,    44,    46,    47,
      48,    49,    50,    57,    59,    61,    62,    64,    40,    13,
      13,    35,    40,    42,    55,    56,    42,    42,    72,    89,
      82,    89,    91,    89,    86,    89,    89,     5,    15,    29,
      31,    33,    93,    72,    72,    18,    23,     3,    23,    24,
      13,    23,    24,    46,    26,    13,    33,    37,    38,    29,
      30,    30,    50,    23,     3,     3,     4,   103,    47,    27,
      51,    52,    53,    62,    51,    53,    70,    73,    76,    78,
      79,    58,    60,    62,   103,    65,    42,    23,    26,    36,
      56,    49,    56,    13,    35,    53,    68,    69,    70,     3,
      47,    53,    68,    69,    70,   103,     3,     3,     3,     3,
       3,    89,     3,     3,     3,     3,     3,    34,   103,    24,
       3,     3,    72,   103,   103,     3,    24,   103,     3,     3,
       3,    23,    34,   103,    72,     3,     3,     3,     3,     3,
     103,   103,     3,     3,     3,     3,   103,     3,     3,   103,
      74,    75,    77,   103,   103,   114,   103,   103,     3,     3,
     103,     3,     3,    36,     3,    36,    26,    36,     3,     3,
     103,   103,   103,     3,     3,   103,   103,     3,     3,   103,
       3,   103,   103,   103,     3,   103,   103,     3,   103,   103,
       3,   103,     3,     3,     3,    95,   103,   105,   103,    13,
       3,     3,     3,   103,   103,   103,   103,   104,    26,    94,
      96,   103,     3
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    99,   100,   101,   101,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   103,   103,   104,   104,   104,
     104,   105,   105,   106,   106,   107,   107,   108,   108,   109,
     109,   109,   109,   109,   109,   110,   111,   111,   111,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   113,   113,   113,
     113,   113,   113,   113,   113,   114,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   115,
     115,   115,   115,   115,   115,   115,   115
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       0,     1,     3,     2,     2,     2,     2,     3,     4,     4,
       4,     5,     5,     5,     3,     3,     5,     4,     4,     5,
       6,     5,     5,     4,     5,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     5,     5,     5,     4,
       4,     4,     4,     4,     4,     6,     5,     6,     5,     5,
       4,     4,     5,     5,     7,     5,     4,     5,     3,     3,
       4,     4,     3,     4,     4,     0,     5,     4,     3,     4,
       4,     4,     4,     4,     3,     4,     4,     4,     4,     3,
       3,     3,     3,     3,     4,     3,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
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
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
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

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
  case 14:
#line 112 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
                        {YYABORT;}
#line 1634 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 15:
#line 132 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
                {
		      if(ProjData::Fbuff) ProjData::Fbuff->push_back((yyvsp[0].fval));
		}
#line 1642 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 16:
#line 136 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
                {
		       (yyval.fval) = (double)(yyvsp[0].ival);
		       if(ProjData::Fbuff) ProjData::Fbuff->push_back((double)(yyvsp[0].ival));
		}
#line 1651 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 21:
#line 174 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
                  {}
#line 1657 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 22:
#line 175 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
                              {}
#line 1663 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 23:
#line 229 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.varFilePath=(yyvsp[0].sval);
            }
#line 1671 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 24:
#line 233 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.s2pFilePath=(yyvsp[0].sval);
            }
#line 1679 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 25:
#line 240 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.mainAssName=(yyvsp[0].sval);
            }
#line 1687 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 26:
#line 244 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.network=(yyvsp[0].ival);
            }
#line 1695 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 27:
#line 251 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              strcpy(prjData.lengthUnitName,(yyvsp[0].sval));
            }
#line 1703 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 28:
#line 255 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.freqUnitE =(yyvsp[0].ival);
            }
#line 1711 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 29:
#line 262 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.meshPerWavelen=(yyvsp[0].ival);
            }
#line 1719 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 30:
#line 266 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.meshPerCircle=(yyvsp[0].ival);
            }
#line 1727 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 31:
#line 270 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.meshRefineMaxNum=(yyvsp[0].ival);
            }
#line 1735 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 32:
#line 274 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
//              prjData.refFreqBand[0]=$4;
//              prjData.refFreqBand[1]=$5;
            }
#line 1744 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 33:
#line 279 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.meshMinEnergyRatio=(yyvsp[0].fval);
            }
#line 1752 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 34:
#line 283 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.localMeshing3d=(yyvsp[0].ival);
            }
#line 1760 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 35:
#line 290 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
           {
              prjData.cutoffRatio=(yyvsp[0].fval);
           }
#line 1768 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 36:
#line 297 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.freqBand[0]=(yyvsp[-1].fval);
              prjData.freqBand[1]=(yyvsp[0].fval);
            }
#line 1777 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 37:
#line 302 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.MORFreqNum=(yyvsp[0].ival);
            }
#line 1785 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 38:
#line 306 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.KrylovOrder=(yyvsp[0].ival);
            }
#line 1793 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 39:
#line 313 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.anaFreqBand[0]=(yyvsp[-1].fval);
              prjData.anaFreqBand[1]=(yyvsp[0].fval);
            }
#line 1802 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 40:
#line 318 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.zpFreqBand[0]=(yyvsp[-1].fval);
              prjData.zpFreqBand[1]=(yyvsp[0].fval);
            }
#line 1811 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 41:
#line 323 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.zpWinRatio=(yyvsp[0].fval);
            }
#line 1819 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 42:
#line 327 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.mapFreqBand[0]=(yyvsp[-1].fval);
              prjData.mapFreqBand[1]=(yyvsp[0].fval);
            }
#line 1828 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 43:
#line 332 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.anaFreqNum=(yyvsp[0].ival);
            }
#line 1836 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 44:
#line 336 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.anaMORFreqNum=(yyvsp[0].ival);
            }
#line 1844 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 45:
#line 340 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.idealFilterType=(yyvsp[0].ival);
            }
#line 1852 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 46:
#line 344 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.canonicalFilterTopology=(yyvsp[0].ival);
            }
#line 1860 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 47:
#line 348 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.canonicalFilterTopology=(yyvsp[0].ival);
            }
#line 1868 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 48:
#line 352 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.customIdealFilter=(yyvsp[0].ival);
            }
#line 1876 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 49:
#line 356 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
            }
#line 1883 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 50:
#line 359 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filtermapSymmetric=(yyvsp[0].ival);
            }
#line 1891 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 51:
#line 363 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filtermapSource=(yyvsp[0].ival);
            }
#line 1899 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 52:
#line 367 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filtermapQfactor = (yyvsp[0].fval);
            }
#line 1907 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 53:
#line 371 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
            }
#line 1914 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 54:
#line 374 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterTuneItermax=(yyvsp[0].ival);
            }
#line 1922 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 55:
#line 378 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterTuneMethod=(yyvsp[0].ival);
            }
#line 1930 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 56:
#line 382 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterTuneRecomputeJaco=(yyvsp[0].ival);
            }
#line 1938 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 57:
#line 386 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterTuneRecomputeError=(yyvsp[0].ival);
            }
#line 1946 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 58:
#line 390 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterTuneOnlyJt=(yyvsp[0].ival);
            }
#line 1954 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 59:
#line 394 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterSymmetricTuning=(yyvsp[0].ival);
            }
#line 1962 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 60:
#line 398 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterTuneXtol=(yyvsp[0].fval);
            }
#line 1970 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 61:
#line 402 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterTuneTrustR=(yyvsp[0].fval);
            }
#line 1978 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 62:
#line 406 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterTuneGradDx=(yyvsp[0].fval);
            }
#line 1986 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 63:
#line 410 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.freqRespParType = (FreqRespParType) (yyvsp[0].ival);
            }
#line 1994 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 64:
#line 414 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.freqRespParPart = (yyvsp[0].ival);
            }
#line 2002 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 65:
#line 418 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.freqRespYscale[0]=(yyvsp[-2].fval);
              prjData.freqRespYscale[1]=(yyvsp[-1].fval);
              prjData.freqRespYscale[2]=(yyvsp[0].fval);
            }
#line 2012 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 66:
#line 424 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.freqRespYscaleAuto=(yyvsp[0].ival);
            }
#line 2020 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 67:
#line 428 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.zeropoleYscale[0]=(yyvsp[-2].fval);
              prjData.zeropoleYscale[1]=(yyvsp[-1].fval);
              prjData.zeropoleYscale[2]=(yyvsp[0].fval);
            }
#line 2030 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 68:
#line 434 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.zeropoleYscaleAuto=(yyvsp[0].ival);
            }
#line 2038 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 69:
#line 438 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              std::pair<int, int> Sij_ports;
              Sij_ports.first=(yyvsp[-1].ival);
              Sij_ports.second=(yyvsp[0].ival);
              prjData.zeropoleCurves.insert(Sij_ports);
            }
#line 2049 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 70:
#line 445 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.autoFreqResponse=(yyvsp[0].ival);
            }
#line 2057 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 71:
#line 449 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.autoZeropole=(yyvsp[0].ival);
            }
#line 2065 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 72:
#line 453 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.autoDesignWithMappedTz=(yyvsp[0].ival);
            }
#line 2073 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 73:
#line 457 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.autoMappedFreqResponse=(yyvsp[0].ival);
            }
#line 2081 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 74:
#line 461 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.autoIdealMappedTzFreqResponse=(yyvsp[0].ival);
            }
#line 2089 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 75:
#line 465 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.autoMappedZeropole=(yyvsp[0].ival);
            }
#line 2097 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 76:
#line 469 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.autoFilterMapping=(yyvsp[0].ival);
            }
#line 2105 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 77:
#line 476 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterPassBand[0]=(yyvsp[-1].fval);
              prjData.filterPassBand[1]=(yyvsp[0].fval);
            }
#line 2114 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 78:
#line 481 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterOrder = (yyvsp[0].ival);
            }
#line 2122 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 79:
#line 485 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterRetLoss = (yyvsp[0].fval);
            }
#line 2130 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 80:
#line 489 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterOutbandRetLoss = (yyvsp[0].fval);
            }
#line 2138 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 81:
#line 493 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterPortImpedance = (yyvsp[0].fval);
            }
#line 2146 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 82:
#line 497 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterQfactor = (yyvsp[0].fval);
            }
#line 2154 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 83:
#line 501 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterTargetQfactor = (yyvsp[0].fval);
            }
#line 2162 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 84:
#line 505 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.filterInductiveSkin = (yyvsp[0].ival);
            }
#line 2170 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 85:
#line 508 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
                           {ProjData::Fbuff =&prjData.filterZeros; ProjData::Fbuff->clear();}
#line 2176 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 86:
#line 509 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              ProjData::Fbuff=NULL;
            }
#line 2184 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 87:
#line 513 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.symmFilterResponse = (yyvsp[0].ival);
            }
#line 2192 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 88:
#line 517 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.predistortedFilter = (yyvsp[0].ival);
            }
#line 2200 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 89:
#line 521 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.predistFilterOptim = (yyvsp[0].ival);
            }
#line 2208 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 90:
#line 525 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimRL = (yyvsp[0].fval);
            }
#line 2216 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 91:
#line 529 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimILpp = (yyvsp[0].fval);
            }
#line 2224 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 92:
#line 533 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimTrustR = (yyvsp[0].fval);
            }
#line 2232 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 93:
#line 537 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.predistFilterOptimIterMax = (yyvsp[0].ival);
            }
#line 2240 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 94:
#line 541 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.predistortedFilter = (yyvsp[0].ival);
            }
#line 2248 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 95:
#line 545 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
            }
#line 2255 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 96:
#line 548 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
            }
#line 2262 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 97:
#line 551 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
            }
#line 2269 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 98:
#line 554 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
            }
#line 2276 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 99:
#line 560 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.workStatus.decompositionNeeded=(yyvsp[0].ival);
            }
#line 2284 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 100:
#line 564 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.workStatus.firstDecomposition=(yyvsp[0].ival);
            }
#line 2292 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 101:
#line 568 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.workStatus.materialChanged=(yyvsp[0].ival);
            }
#line 2300 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 102:
#line 572 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.workStatus.remeshNeeded=(yyvsp[0].ival);
            }
#line 2308 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 103:
#line 576 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.workStatus.modelizationNeeded=(yyvsp[0].ival);
            }
#line 2316 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 104:
#line 580 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.workStatus.componentsaveNeeded=(yyvsp[0].ival);
            }
#line 2324 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 105:
#line 584 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.workStatus.reductionNeeded=(yyvsp[0].ival);
            }
#line 2332 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;

  case 106:
#line 588 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"
            {
              prjData.workStatus.reloadNeeded=(yyvsp[0].ival);
            }
#line 2340 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"
    break;


#line 2344 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData_parser.cpp"

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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
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
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 592 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/GUI/projectData.y"



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
	return result;
}




