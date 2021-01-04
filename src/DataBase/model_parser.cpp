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
#define yyparse         modelparse
#define yylex           modellex
#define yyerror         modelerror
#define yydebug         modeldebug
#define yynerrs         modelnerrs
#define yylval          modellval
#define yychar          modelchar

/* First part of user prologue.  */
#line 22 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"

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


void model_msg(int type, char *fmt, ...);

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


#line 152 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"

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
#ifndef YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED
# define YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int modeldebug;
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
    IMPORT = 262,
    ASSEMBLYTYPE = 263,
    LEVEL = 264,
    DEF = 265,
    SET = 266,
    CIRCUITNAME = 267,
    DEFAULTBC = 268,
    MWM_MATERIAL = 269,
    TEMPORTSNUM = 270,
    TEPORTSNUM = 271,
    TMPORTSNUM = 272,
    GRIDNUM = 273,
    PML = 274,
    INVARIANT = 275,
    TRANSLATION = 276,
    ROTATION = 277,
    ANGLE = 278,
    ORIGIN = 279,
    AXIS = 280,
    MWM_VOLUME = 281,
    MWM_INVARIANT = 282,
    MWM_UNITS = 283,
    MWM_LINEPORT = 284,
    LENGTH = 285,
    FREQUENCY = 286,
    BAND = 287,
    SURFACE = 288,
    RESISTANCE = 289,
    INDUCTANCE = 290,
    IMPEDANCE = 291,
    LOSSFACTOR = 292,
    QFACTOR = 293,
    ROUGH = 294,
    MESHREFINEMENT = 295,
    COMPSOLID = 296,
    VOLTYPE = 297,
    EPSILONR = 298,
    MUR = 299,
    EPSLORENTZ = 300,
    MULORENTZ = 301,
    POLESRESIDUES = 302,
    POLESNUM = 303,
    ECONDUCTIVITY = 304,
    HCONDUCTIVITY = 305,
    ETANDELTA = 306,
    HTANDELTA = 307,
    MATERIAL = 308,
    COLOR = 309,
    VOLUMES = 310,
    DISABLED = 311,
    tDIELECTRIC = 312,
    tHOLE = 313,
    tBOUNDARYCONDITION = 314,
    tWAVEGUIDE = 315,
    tLINEPORT = 316,
    tSPLITTER = 317,
    tGRID = 318,
    tCOMPONENT = 319,
    tINTERFACE = 320,
    tNET = 321,
    tASSEMBLY = 322,
    tUNDEFINED = 323,
    SOLID = 324
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 97 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"

int	ival;
double	fval;
char	*sval;

#line 280 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE modellval;

int modelparse (void);

#endif /* !YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_TET_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED  */



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
#define YYLAST   172

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  75
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  72
/* YYNRULES -- Number of rules.  */
#define YYNRULES  142
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  209

#define YYUNDEFTOK  2
#define YYMAXUTOK   324


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
       2,     2,     2,     2,    70,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    71,     2,    72,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    73,     2,    74,     2,     2,     2,     2,
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
      65,    66,    67,    68,    69
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   127,   127,   131,   132,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   156,   168,   172,   186,   191,
     192,   193,   194,   198,   199,   229,   230,   231,   232,   236,
     237,   242,   246,   247,   248,   249,   253,   254,   270,   274,
     275,   280,   285,   292,   300,   311,   323,   324,   328,   328,
     329,   329,   330,   330,   334,   338,   346,   347,   351,   351,
     352,   352,   357,   357,   362,   362,   370,   380,   388,   389,
     393,   393,   394,   394,   395,   395,   396,   396,   397,   397,
     398,   398,   399,   399,   400,   400,   401,   401,   402,   402,
     403,   403,   404,   404,   405,   405,   406,   406,   407,   407,
     408,   408,   416,   427,   438,   442,   446,   450,   454,   458,
     462,   466,   470,   474,   478,   482,   486,   491,   492,   496,
     502,   502,   506,   506,   510,   510,   514,   514,   518,   518,
     522,   522,   526,   526,   527,   527,   529,   529,   531,   531,
     534,   543,   552
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTEGER", "FLOATING", "STRING", "NAME",
  "IMPORT", "ASSEMBLYTYPE", "LEVEL", "DEF", "SET", "CIRCUITNAME",
  "DEFAULTBC", "MWM_MATERIAL", "TEMPORTSNUM", "TEPORTSNUM", "TMPORTSNUM",
  "GRIDNUM", "PML", "INVARIANT", "TRANSLATION", "ROTATION", "ANGLE",
  "ORIGIN", "AXIS", "MWM_VOLUME", "MWM_INVARIANT", "MWM_UNITS",
  "MWM_LINEPORT", "LENGTH", "FREQUENCY", "BAND", "SURFACE", "RESISTANCE",
  "INDUCTANCE", "IMPEDANCE", "LOSSFACTOR", "QFACTOR", "ROUGH",
  "MESHREFINEMENT", "COMPSOLID", "VOLTYPE", "EPSILONR", "MUR",
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
  "$@17", "$@18", "$@19", "$@20", "$@21", "$@22", "$@23", "MaterialBegin",
  "Material", "VolumeType", "VolumeElements", "VolumeElement", "$@24",
  "$@25", "$@26", "$@27", "$@28", "$@29", "$@30", "$@31", "$@32", "$@33",
  "VolumeBegin", "Volume", "CircuitName", YY_NULLPTR
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
      44,    91,    93,   123,   125
};
# endif

#define YYPACT_NINF (-160)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-3)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -160,    27,   112,  -160,  -160,    26,     6,     6,    10,    22,
      34,  -160,  -160,  -160,  -160,  -160,    -5,  -160,    -5,  -160,
      -5,  -160,    -5,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
      30,    70,  -160,  -160,    -2,    42,    -3,    36,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,    14,    -2,  -160,    29,
      14,    42,     3,    58,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,    14,    -3,    94,    77,    96,    96,    96,
      31,  -160,  -160,     9,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,    24,    96,    96,     9,     9,    96,    96,    96,
      96,    16,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,    97,  -160,    14,    77,  -160,  -160,
    -160,  -160,  -160,  -160,    96,    96,  -160,  -160,    96,     9,
       9,    96,    96,    11,  -160,    65,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,     6,  -160,  -160,     6,
       6,     6,     6,     6,     6,    96,    96,     6,  -160,  -160,
    -160,  -160,  -160,     1,    96,  -160,  -160,  -160,  -160,  -160,
      96,    96,  -160,  -160,    96,  -160,    96,    96,  -160,     7,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,     4,    96,  -160,    96,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,    96,  -160,  -160
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,    14,     0,     0,     0,     0,     0,
       0,     4,     6,     8,     9,    10,     0,     5,     0,     7,
       0,    11,     0,    12,    13,    45,    15,    42,    43,    41,
       0,     0,    44,    38,    47,    57,    69,   116,   102,   140,
      66,    54,   142,    48,    50,    52,     0,    47,    64,     0,
       0,    57,     0,     0,    70,    72,    82,    84,    78,    80,
      74,    76,   100,     0,    69,     0,   118,     0,     0,     0,
      39,    55,    46,     0,    58,    62,    60,    67,    56,    88,
      90,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   103,    68,   104,   105,   106,   107,   108,   109,
     110,   112,   113,   111,   114,   115,   120,   122,   124,   126,
     128,   130,   132,   136,     0,   138,     0,   118,    17,    16,
      49,    51,    53,    40,    28,     0,    29,    65,     0,     0,
       0,     0,     0,     0,    94,     0,    96,    92,    71,    73,
      83,    85,    79,    81,    75,    77,    22,    23,   101,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   119,   139,
     141,   117,    25,     0,     0,    59,    63,    61,    89,    91,
      35,     0,    36,    87,     0,    98,     0,     0,    19,     0,
     121,   123,   125,   127,   129,   131,   133,   135,   137,    27,
      30,    26,    18,    32,     0,     0,    95,     0,    97,    93,
      21,    24,    20,    34,    37,    33,     0,    99,    31
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -160,  -160,  -160,  -160,    -7,   -66,  -118,  -160,  -160,  -160,
     -60,  -159,  -160,  -160,    63,   -30,  -160,  -160,  -160,  -160,
    -160,    59,  -160,  -160,  -160,  -160,  -160,  -160,    56,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,    45,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,    -1,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    11,    27,   125,   126,   179,   148,   163,
     127,   172,   194,   173,    34,    71,    30,    12,    13,    14,
      15,    46,    47,    67,    68,    69,    16,    17,    50,    51,
     128,   130,   129,    73,    18,    19,    63,    64,    83,    84,
      89,    90,    87,    88,    85,    86,   133,   131,   132,   177,
     174,   176,   197,    91,    20,    21,    66,   116,   117,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   159,    22,
      23,    24
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      28,   120,   121,   122,   118,   119,   162,   118,   119,    26,
      26,   193,   118,   119,   118,   119,    29,   138,   139,    26,
      77,   142,   143,   144,   145,   140,   141,     3,    43,    44,
      52,    25,    45,    92,    31,   205,    53,    79,    80,    32,
      54,    55,    56,    57,    38,   191,    58,    59,    60,    61,
      81,    62,    74,    75,    76,   134,    39,    40,    41,   164,
     135,   136,   165,    48,    49,   168,   169,   171,    33,   166,
     167,   189,   137,   190,   203,    42,   204,   200,    65,   201,
     124,    35,   170,    36,   147,    37,   160,   146,    70,   186,
     187,    82,   106,   107,   108,   109,   110,   111,   192,   118,
     119,   123,   158,   175,   171,   195,    72,    78,   196,    93,
     198,   199,    -2,     4,     0,     0,   161,   112,   113,     5,
       6,     7,     8,     9,     0,    10,     0,     0,   171,   206,
     114,   207,     0,   115,     0,     0,     0,     0,     0,   178,
     208,     0,   180,   181,   182,   183,   184,   185,     0,     0,
     188,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   202
};

static const yytype_int16 yycheck[] =
{
       7,    67,    68,    69,     3,     4,   124,     3,     4,     3,
       3,   170,     3,     4,     3,     4,     6,    83,    84,     3,
      50,    87,    88,    89,    90,    85,    86,     0,    30,    31,
      33,     5,    34,    63,    12,   194,    39,    34,    35,     5,
      43,    44,    45,    46,    14,   163,    49,    50,    51,    52,
      47,    54,    23,    24,    25,    31,    26,    27,    28,   125,
      36,    37,   128,    21,    22,   131,   132,   133,    73,   129,
     130,    70,    48,    72,    70,     5,    72,    70,    42,    72,
      71,    18,    71,    20,    91,    22,   116,    71,    74,   155,
     156,    33,    15,    16,    17,    18,    19,    20,   164,     3,
       4,    70,     5,    38,   170,   171,    47,    51,   174,    64,
     176,   177,     0,     1,    -1,    -1,   117,    40,    41,     7,
       8,     9,    10,    11,    -1,    13,    -1,    -1,   194,   195,
      53,   197,    -1,    56,    -1,    -1,    -1,    -1,    -1,   146,
     206,    -1,   149,   150,   151,   152,   153,   154,    -1,    -1,
     157,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   179
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    76,    77,     0,     1,     7,     8,     9,    10,    11,
      13,    78,    92,    93,    94,    95,   101,   102,   109,   110,
     129,   130,   144,   145,   146,     5,     3,    79,    79,     6,
      91,    12,     5,    73,    89,    89,    89,    89,    14,    26,
      27,    28,     5,    30,    31,    34,    96,    97,    21,    22,
     103,   104,    33,    39,    43,    44,    45,    46,    49,    50,
      51,    52,    54,   111,   112,    42,   131,    98,    99,   100,
      74,    90,    96,   108,    23,    24,    25,    90,   103,    34,
      35,    47,    33,   113,   114,   119,   120,   117,   118,   115,
     116,   128,    90,   111,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    15,    16,    17,    18,
      19,    20,    40,    41,    53,    56,   132,   133,     3,     4,
      80,    80,    80,    70,    71,    80,    81,    85,   105,   107,
     106,   122,   123,   121,    31,    36,    37,    48,    80,    80,
      85,    85,    80,    80,    80,    80,    71,    79,    83,   134,
     135,   136,   137,   138,   139,   140,   141,   142,     5,   143,
      90,   132,    81,    84,    80,    80,    85,    85,    80,    80,
      71,    80,    86,    88,   125,    38,   126,   124,    79,    82,
      79,    79,    79,    79,    79,    79,    80,    80,    79,    70,
      72,    81,    80,    86,    87,    80,    80,   127,    80,    80,
      70,    72,    79,    70,    72,    86,    80,    80,    80
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    75,    76,    77,    77,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    79,    80,    80,    81,    82,
      82,    82,    82,    83,    83,    84,    84,    84,    84,    85,
      85,    86,    87,    87,    87,    87,    88,    88,    89,    90,
      90,    91,    92,    93,    94,    95,    96,    96,    98,    97,
      99,    97,   100,    97,   101,   102,   103,   103,   105,   104,
     106,   104,   107,   104,   108,   104,   109,   110,   111,   111,
     113,   112,   114,   112,   115,   112,   116,   112,   117,   112,
     118,   112,   119,   112,   120,   112,   121,   112,   122,   112,
     123,   112,   124,   112,   125,   112,   126,   112,   127,   112,
     128,   112,   129,   130,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   132,   132,   133,
     134,   133,   135,   133,   136,   133,   137,   133,   138,   133,
     139,   133,   140,   133,   141,   133,   142,   133,   143,   133,
     144,   145,   146
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
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
       0,     4,     0,     5,     0,     5,     0,     5,     0,     6,
       0,     3,     3,     4,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     0,     2,     0,     2,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     2,
       3,     5,     3
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
#line 146 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                        {YYABORT;}
#line 1629 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 15:
#line 157 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
			if(Ibuff) Ibuff->input((yyvsp[0].ival));
		}
#line 1637 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 16:
#line 169 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
			if(Fbuff) Fbuff->input((yyvsp[0].fval));
		}
#line 1645 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 17:
#line 173 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
			(yyval.fval) = (double)(yyvsp[0].ival);
			if(Fbuff) Fbuff->input((double)(yyvsp[0].ival));
		}
#line 1654 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 18:
#line 186 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                  {}
#line 1660 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 23:
#line 198 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                  {}
#line 1666 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 24:
#line 199 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                              {}
#line 1672 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 31:
#line 242 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                          {}
#line 1678 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 41:
#line 280 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
             {if (strlen((yyvsp[0].sval)) > sizeof(DB::str_t)-1) YYERROR; else strcpy(name,(yyvsp[0].sval));}
#line 1684 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 42:
#line 286 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
			loadingEmP->assemblyType=(yyvsp[0].ival);
		}
#line 1692 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 43:
#line 293 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
			loadingEmP->level=(yyvsp[0].ival);
		}
#line 1700 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 44:
#line 301 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
	             strcpy(loadingEmP->defaultBC,(yyvsp[0].sval));
		}
#line 1708 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 45:
#line 312 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
                  loadingEmP->stepFilePath=(yyvsp[0].sval);
                  if(loadingOcaf) 
                     step_file_reloaded=loadingOcaf->importSTEP_OR_DXC((yyvsp[0].sval), reload_step_file);
		}
#line 1718 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 48:
#line 328 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1724 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 49:
#line 328 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                           {unit.xm   =(yyvsp[0].fval);  }
#line 1730 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 50:
#line 329 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1736 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 51:
#line 329 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                           {unit.xHz  =(yyvsp[0].fval);  }
#line 1742 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 52:
#line 330 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1748 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 53:
#line 330 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                           {unit.xOhm =(yyvsp[0].fval);  }
#line 1754 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 58:
#line 351 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 1760 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 59:
#line 351 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                                    {invt->rotAngle=(yyvsp[0].fval); }
#line 1766 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 60:
#line 352 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                          {Fbuff = &DoubleBuffer; Fbuff->reset();}
#line 1772 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 61:
#line 353 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
            {
              DoubleBuffer.flush(&dVec); 
              for(int i=0; i<3; i++) invt->rotAxis[i]=dVec[i];
            }
#line 1781 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 62:
#line 357 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                            {Fbuff = &DoubleBuffer; Fbuff->reset();}
#line 1787 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 63:
#line 358 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
            {
              DoubleBuffer.flush(&dVec); 
              for(int i=0; i<3; i++) invt->rotOrigin[i]=dVec[i];
            }
#line 1796 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 64:
#line 362 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                          {Fbuff = &DoubleBuffer; Fbuff->reset();}
#line 1802 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 65:
#line 363 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
            {
              DoubleBuffer.flush(&dVec); 
              for(int i=0; i<3; i++) invt->trasl[i]=dVec[i];
            }
#line 1811 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 66:
#line 371 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
         {     
              invt =loadingEmP->FindInvariant(name);
              if(invt) loadingEmP->delInvariant(invt);
	      invt = new DB::Transform();
	      strcpy(invt->name,name);
	      loadingEmP->addInvariant(invt);
	 }
#line 1823 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 70:
#line 393 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1829 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 71:
#line 393 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                                           {mat->epsr=(yyvsp[0].fval);     }
#line 1835 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 72:
#line 394 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1841 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 73:
#line 394 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                                           {mat->mur=(yyvsp[0].fval);          }
#line 1847 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 74:
#line 395 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1853 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 75:
#line 395 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                                           {mat->etandelta=(yyvsp[0].fval);}
#line 1859 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 76:
#line 396 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1865 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 77:
#line 396 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                                           {mat->htandelta=(yyvsp[0].fval);}
#line 1871 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 78:
#line 397 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1877 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 79:
#line 397 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                                           {mat->econductivity=(yyvsp[0].fval);}
#line 1883 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 80:
#line 398 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1889 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 81:
#line 398 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                                           {mat->hconductivity=(yyvsp[0].fval);}
#line 1895 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 82:
#line 399 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                    {Fbuff  =&mat->buff.epsLorentz; Fbuff->reset();}
#line 1901 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 84:
#line 400 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                    {Fbuff  =&mat->buff.muLorentz; Fbuff->reset();}
#line 1907 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 86:
#line 401 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                    {Fbuff  =&mat->buff.surfPolesRes; Fbuff->reset();}
#line 1913 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 88:
#line 402 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1919 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 89:
#line 402 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                                           {mat->Sresistance=(yyvsp[0].fval); }
#line 1925 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 90:
#line 403 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1931 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 91:
#line 403 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                                           {mat->Sinductance=(yyvsp[0].fval); }
#line 1937 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 92:
#line 404 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1943 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 93:
#line 404 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                                           {mat->roughSurfFitPolesNum=(yyvsp[0].fval); }
#line 1949 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 94:
#line 405 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1955 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 95:
#line 405 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                                           {mat->roughSurfFreq=(yyvsp[0].fval); }
#line 1961 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 96:
#line 406 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                    {Fbuff = NULL;}
#line 1967 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 97:
#line 406 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                                           {mat->roughSurfLossFactor=(yyvsp[0].fval);}
#line 1973 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 98:
#line 407 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                           {Fbuff = NULL;}
#line 1979 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 99:
#line 407 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                                           {mat->roughSurfImpedanceQ=(yyvsp[0].fval);}
#line 1985 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 100:
#line 408 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                    { Ibuff = &IntBuffer; Ibuff->reset();}
#line 1991 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 101:
#line 409 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
            {
              IntBuffer.flush(&matColor); 
              for(int i=0; i<4; i++) mat->color[i]=matColor[i];
            }
#line 2000 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 102:
#line 417 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {     
                        mat =loadingEmP->FindMaterial(name);
                        if(mat) loadingEmP->delMaterial(mat);
			mat = new DB::Material();
			strcpy(mat->name,name);
			loadingEmP->addMaterial(mat);
		}
#line 2012 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 103:
#line 428 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
          { 
             mat->flush(); 
          }
#line 2020 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 104:
#line 439 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
		   vol->type=DIELECTRIC;
                }
#line 2028 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 105:
#line 443 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
		   vol->type=HOLE;
                }
#line 2036 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 106:
#line 447 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
		   vol->type=BOUNDARYCOND;
                }
#line 2044 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 107:
#line 451 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
		   vol->type=WAVEGUIDE;
                }
#line 2052 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 108:
#line 455 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
		   vol->type=LINEPORT;
                }
#line 2060 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 109:
#line 459 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
		   vol->type=SPLITTER;
                }
#line 2068 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 110:
#line 463 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
		   vol->type=GRID;
                }
#line 2076 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 111:
#line 467 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
		   vol->type=NET;
                }
#line 2084 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 112:
#line 471 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
		   vol->type=COMPONENT;
                }
#line 2092 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 113:
#line 475 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
		   vol->type=INTERFACE;
                }
#line 2100 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 114:
#line 479 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
		   vol->type=ASSEMBLY;
                }
#line 2108 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 115:
#line 483 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
		   vol->type=UNDEFINED;
                }
#line 2116 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 119:
#line 497 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
                   if (strlen((yyvsp[0].sval)) > sizeof(DB::str_t)-1) YYERROR;
                   if(!loadingEmP->FindMaterial((yyvsp[0].sval)))  model_msg(FATAL, "Undefined material %s\n", (yyvsp[0].sval));
		   strcpy(vol->material,(yyvsp[0].sval));
                }
#line 2126 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 120:
#line 502 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2132 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 121:
#line 503 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
                  vol->TEMportsNum =(yyvsp[0].ival);
                }
#line 2140 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 122:
#line 506 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2146 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 123:
#line 507 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
                  vol->TEportsNum =(yyvsp[0].ival);
                }
#line 2154 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 124:
#line 510 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2160 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 125:
#line 511 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
                  vol->TMportsNum =(yyvsp[0].ival);
                }
#line 2168 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 126:
#line 514 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                        {Ibuff  =NULL;}
#line 2174 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 127:
#line 515 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
                  vol->gridNum =(yyvsp[0].ival);
                }
#line 2182 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 128:
#line 518 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {Ibuff  =NULL;}
#line 2188 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 129:
#line 519 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
                  vol->PML =(yyvsp[0].ival);
                }
#line 2196 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 130:
#line 522 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                      {Ibuff  =NULL;}
#line 2202 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 131:
#line 523 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
                  vol->invariant =(yyvsp[0].ival);
                }
#line 2210 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 132:
#line 526 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 2216 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 133:
#line 526 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                             {vol->meshRefinement=(yyvsp[0].fval); }
#line 2222 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 134:
#line 527 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                             {Fbuff = NULL;}
#line 2228 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 135:
#line 527 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                             {vol->meshRefinement=(yyvsp[0].fval); }
#line 2234 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 136:
#line 529 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                             {Ibuff = NULL;}
#line 2240 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 137:
#line 529 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                             { }
#line 2246 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 138:
#line 531 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                             { }
#line 2252 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 139:
#line 531 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                                                             {vol->disabled=1; }
#line 2258 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 140:
#line 535 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
                {
			vol = new DB::Volume();
			strcpy(vol->name,name);
			loadingEmP->insertVolume(vol);
		}
#line 2268 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;

  case 142:
#line 553 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"
            {
             if (strlen((yyvsp[0].sval))>sizeof(DB::str_t)-1) YYERROR; 
             else strcpy(mainCircName,(yyvsp[0].sval));
            }
#line 2277 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"
    break;


#line 2281 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model_parser.cpp"

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
#line 559 "/home/walter/MwCAD/EmCAD/EmCAD-tet/src/DataBase/model.y"



extern int modelLineNum;
extern void SetInputFile(FILE *fp);

char modelFileName[256];


void yyerror(char *s){
  printf("\n  %s (%s) at file: %s, line: %d\n\n", s, modeltext, modelFileName, modelLineNum);
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

  DB::Msg(type, "'%s', line %d : %s", modelFileName, modelLineNum, tmp);

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
        strncpy(modelFileName, fName, 255);
        SetInputFile(fid);
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
        strncpy(modelFileName, fName, 255);
        SetInputFile(fid);
        step_file_reloaded=false;
        int result=yyparse();
        if(strcmp(fName,"-")) fclose(fid);
	return step_file_reloaded;
}


