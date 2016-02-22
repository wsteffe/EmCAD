/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

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

/* Copy the first part of user declarations.  */
#line 22 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:339  */

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


#line 149 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "model_parser.hpp".  */
#ifndef YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED
# define YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED
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
    RESISTANCE = 288,
    SURFACERESISTANCE = 289,
    MESHREFINEMENT = 290,
    COMPSOLID = 291,
    VOLTYPE = 292,
    EPSILONR = 293,
    MUR = 294,
    EPSLORENTZ = 295,
    MULORENTZ = 296,
    ECONDUCTIVITY = 297,
    HCONDUCTIVITY = 298,
    ETANDELTA = 299,
    HTANDELTA = 300,
    MATERIAL = 301,
    COLOR = 302,
    VOLUMES = 303,
    DISABLED = 304,
    tDIELECTRIC = 305,
    tHOLE = 306,
    tBOUNDARYCONDITION = 307,
    tWAVEGUIDE = 308,
    tLINEPORT = 309,
    tSPLITTER = 310,
    tGRID = 311,
    tCOMPONENT = 312,
    tINTERFACE = 313,
    tNET = 314,
    tASSEMBLY = 315,
    tUNDEFINED = 316,
    SOLID = 317
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 97 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:355  */

int	ival;
double	fval;
char	*sval;

#line 258 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE modellval;

int modelparse (void);

#endif /* !YY_MODEL_HOME_WALTER_MWCAD_EMCAD_EMCAD_SRC_DATABASE_MODEL_PARSER_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 273 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
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
#define YYLAST   156

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  68
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  64
/* YYNRULES -- Number of rules.  */
#define YYNRULES  125
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  181

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   317

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    63,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    64,     2,    65,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    66,     2,    67,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   127,   127,   130,   132,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   156,   168,   172,   186,   191,
     192,   193,   194,   198,   199,   229,   230,   231,   232,   236,
     237,   253,   257,   258,   263,   268,   275,   282,   293,   304,
     305,   309,   309,   310,   310,   311,   311,   315,   319,   327,
     328,   332,   332,   333,   333,   338,   338,   343,   343,   351,
     361,   369,   370,   374,   374,   375,   375,   376,   376,   377,
     377,   378,   378,   379,   379,   380,   380,   381,   381,   382,
     382,   383,   383,   384,   384,   392,   403,   414,   418,   422,
     426,   430,   434,   438,   442,   446,   450,   454,   458,   462,
     467,   468,   472,   478,   478,   482,   482,   486,   486,   490,
     490,   494,   494,   498,   498,   502,   502,   503,   503,   505,
     505,   507,   507,   510,   519,   528
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
  "MWM_LINEPORT", "LENGTH", "FREQUENCY", "BAND", "RESISTANCE",
  "SURFACERESISTANCE", "MESHREFINEMENT", "COMPSOLID", "VOLTYPE",
  "EPSILONR", "MUR", "EPSLORENTZ", "MULORENTZ", "ECONDUCTIVITY",
  "HCONDUCTIVITY", "ETANDELTA", "HTANDELTA", "MATERIAL", "COLOR",
  "VOLUMES", "DISABLED", "tDIELECTRIC", "tHOLE", "tBOUNDARYCONDITION",
  "tWAVEGUIDE", "tLINEPORT", "tSPLITTER", "tGRID", "tCOMPONENT",
  "tINTERFACE", "tNET", "tASSEMBLY", "tUNDEFINED", "SOLID", "','", "'['",
  "']'", "'{'", "'}'", "$accept", "All", "MwmItems", "MwmItem", "SFInt32",
  "SFFloat", "SFVec3f", "SFInt32List", "MFInt32", "SFVec3fList", "MFVec3f",
  "NodeBegin", "NodeEnd", "Name", "SetCircuitType", "Level",
  "DefaultBoundCond", "Import", "UnitsElements", "UnitsElement", "$@1",
  "$@2", "$@3", "UnitsBegin", "Units", "InvariantElements",
  "InvariantElement", "$@4", "$@5", "$@6", "$@7", "InvariantBegin",
  "Invariant", "MaterialElements", "MaterialElement", "$@8", "$@9", "$@10",
  "$@11", "$@12", "$@13", "$@14", "$@15", "$@16", "$@17", "$@18",
  "MaterialBegin", "Material", "VolumeType", "VolumeElements",
  "VolumeElement", "$@19", "$@20", "$@21", "$@22", "$@23", "$@24", "$@25",
  "$@26", "$@27", "$@28", "VolumeBegin", "Volume", "CircuitName", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,    44,    91,    93,   123,   125
};
# endif

#define YYPACT_NINF -115

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-115)))

#define YYTABLE_NINF -3

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
    -115,     5,    78,  -115,  -115,    23,    27,    27,     4,    12,
      38,  -115,  -115,  -115,  -115,  -115,   -40,  -115,   -40,  -115,
     -40,  -115,   -40,  -115,  -115,  -115,  -115,  -115,  -115,  -115,
      19,    39,  -115,  -115,    18,    -3,    59,    13,  -115,  -115,
    -115,  -115,  -115,  -115,  -115,  -115,   -12,    18,  -115,    29,
     -12,    -3,    28,  -115,  -115,  -115,  -115,  -115,  -115,  -115,
    -115,  -115,  -115,   -12,    59,    57,    21,    31,    31,    31,
      -4,  -115,  -115,     8,  -115,  -115,  -115,  -115,  -115,  -115,
      31,    31,    31,     8,     8,    31,    31,    31,    31,    10,
    -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,
    -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,
    -115,  -115,    58,  -115,   -12,    21,  -115,  -115,  -115,  -115,
    -115,  -115,    31,    31,  -115,  -115,    31,     8,     8,    31,
    -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,    27,
    -115,  -115,    27,    27,    27,    27,    27,    27,    31,    31,
      27,  -115,  -115,  -115,  -115,  -115,     3,    31,  -115,  -115,
    -115,    31,  -115,     6,  -115,  -115,  -115,  -115,  -115,  -115,
    -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,
    -115
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,    14,     0,     0,     0,     0,     0,
       0,     4,     6,     8,     9,    10,     0,     5,     0,     7,
       0,    11,     0,    12,    13,    38,    15,    35,    36,    34,
       0,     0,    37,    31,    40,    50,    62,    99,    85,   123,
      59,    47,   125,    41,    43,    45,     0,    40,    57,     0,
       0,    50,     0,    79,    63,    65,    75,    77,    71,    73,
      67,    69,    83,     0,    62,     0,   101,     0,     0,     0,
      32,    48,    39,     0,    51,    55,    53,    60,    49,    81,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    61,    87,    88,    89,    90,    91,    92,    93,    95,
      96,    94,    97,    98,   103,   105,   107,   109,   111,   113,
     115,   119,     0,   121,     0,   101,    17,    16,    42,    44,
      46,    33,    28,     0,    29,    58,     0,     0,     0,     0,
      80,    64,    66,    76,    78,    72,    74,    68,    70,    22,
      23,    84,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   102,   122,   124,   100,    25,     0,     0,    52,    56,
      54,     0,    19,     0,   104,   106,   108,   110,   112,   114,
     116,   118,   120,    27,    30,    26,    18,    82,    21,    24,
      20
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -115,  -115,  -115,  -115,    -7,   -65,  -114,  -115,  -115,  -115,
     -52,     7,   -49,  -115,  -115,  -115,  -115,  -115,    15,  -115,
    -115,  -115,  -115,  -115,  -115,    22,  -115,  -115,  -115,  -115,
    -115,  -115,  -115,    16,  -115,  -115,  -115,  -115,  -115,  -115,
    -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,   -38,
    -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,
    -115,  -115,  -115,  -115
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    11,    27,   123,   124,   163,   141,   156,
     125,    34,    71,    30,    12,    13,    14,    15,    46,    47,
      67,    68,    69,    16,    17,    50,    51,   126,   128,   127,
      73,    18,    19,    63,    64,    81,    82,    87,    88,    85,
      86,    83,    84,    80,   129,    89,    20,    21,    66,   114,
     115,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     152,    22,    23,    24
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      28,    77,   118,   119,   120,     3,   116,   117,   155,    26,
      29,   116,   117,    26,    90,   130,   131,   132,    48,    49,
     135,   136,   137,   138,    31,    35,    33,    36,    25,    37,
      26,   133,   134,    38,   116,   117,   104,   105,   106,   107,
     108,   109,   175,    32,    42,    39,    40,    41,    43,    44,
      65,    45,    74,    75,    76,    70,   110,   111,   157,   121,
      79,   158,    72,   151,   161,   153,   173,   112,   174,   178,
     113,   179,   122,    78,   139,   159,   160,   154,    -2,     4,
      91,     0,   140,   170,   171,     5,     6,     7,     8,     9,
      52,    10,   176,    53,     0,     0,   177,    54,    55,    56,
      57,    58,    59,    60,    61,     0,    62,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   162,     0,     0,   164,   165,   166,   167,   168,
     169,     0,     0,   172,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   180
};

static const yytype_int16 yycheck[] =
{
       7,    50,    67,    68,    69,     0,     3,     4,   122,     3,
       6,     3,     4,     3,    63,    80,    81,    82,    21,    22,
      85,    86,    87,    88,    12,    18,    66,    20,     5,    22,
       3,    83,    84,    14,     3,     4,    15,    16,    17,    18,
      19,    20,   156,     5,     5,    26,    27,    28,    30,    31,
      37,    33,    23,    24,    25,    67,    35,    36,   123,    63,
      32,   126,    47,     5,   129,   114,    63,    46,    65,    63,
      49,    65,    64,    51,    64,   127,   128,   115,     0,     1,
      64,    -1,    89,   148,   149,     7,     8,     9,    10,    11,
      31,    13,   157,    34,    -1,    -1,   161,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,   142,   143,   144,   145,   146,
     147,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    69,    70,     0,     1,     7,     8,     9,    10,    11,
      13,    71,    82,    83,    84,    85,    91,    92,    99,   100,
     114,   115,   129,   130,   131,     5,     3,    72,    72,     6,
      81,    12,     5,    66,    79,    79,    79,    79,    14,    26,
      27,    28,     5,    30,    31,    33,    86,    87,    21,    22,
      93,    94,    31,    34,    38,    39,    40,    41,    42,    43,
      44,    45,    47,   101,   102,    37,   116,    88,    89,    90,
      67,    80,    86,    98,    23,    24,    25,    80,    93,    32,
     111,   103,   104,   109,   110,   107,   108,   105,   106,   113,
      80,   101,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    15,    16,    17,    18,    19,    20,
      35,    36,    46,    49,   117,   118,     3,     4,    73,    73,
      73,    63,    64,    73,    74,    78,    95,    97,    96,   112,
      73,    73,    73,    78,    78,    73,    73,    73,    73,    64,
      72,    76,   119,   120,   121,   122,   123,   124,   125,   126,
     127,     5,   128,    80,   117,    74,    77,    73,    73,    78,
      78,    73,    72,    75,    72,    72,    72,    72,    72,    72,
      73,    73,    72,    63,    65,    74,    73,    73,    63,    65,
      72
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    68,    69,    70,    70,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    72,    73,    73,    74,    75,
      75,    75,    75,    76,    76,    77,    77,    77,    77,    78,
      78,    79,    80,    80,    81,    82,    83,    84,    85,    86,
      86,    88,    87,    89,    87,    90,    87,    91,    92,    93,
      93,    95,    94,    96,    94,    97,    94,    98,    94,    99,
     100,   101,   101,   103,   102,   104,   102,   105,   102,   106,
     102,   107,   102,   108,   102,   109,   102,   110,   102,   111,
     102,   112,   102,   113,   102,   114,   115,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     117,   117,   118,   119,   118,   120,   118,   121,   118,   122,
     118,   123,   118,   124,   118,   125,   118,   126,   118,   127,
     118,   128,   118,   129,   130,   131
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       2,     2,     0,     1,     3,     1,     2,     2,     0,     1,
       3,     1,     1,     2,     1,     2,     2,     2,     2,     2,
       0,     0,     3,     0,     3,     0,     3,     3,     4,     2,
       0,     0,     4,     0,     4,     0,     4,     0,     3,     3,
       4,     2,     0,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     5,     0,     3,     3,     4,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     0,
       2,     0,     2,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     2,     3,     5,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

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
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
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
#line 146 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {YYABORT;}
#line 1503 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 157 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
			if(Ibuff) Ibuff->input((yyvsp[0].ival));
		}
#line 1511 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 169 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
			if(Fbuff) Fbuff->input((yyvsp[0].fval));
		}
#line 1519 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 173 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
			(yyval.fval) = (double)(yyvsp[0].ival);
			if(Fbuff) Fbuff->input((double)(yyvsp[0].ival));
		}
#line 1528 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 186 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {}
#line 1534 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 198 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {}
#line 1540 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 199 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {}
#line 1546 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 263 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {if (strlen((yyvsp[0].sval)) > sizeof(DB::str_t)-1) YYERROR; else strcpy(name,(yyvsp[0].sval));}
#line 1552 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 269 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
			loadingEmP->assemblyType=(yyvsp[0].ival);
		}
#line 1560 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 276 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
			loadingEmP->level=(yyvsp[0].ival);
		}
#line 1568 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 283 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
	             strcpy(loadingEmP->defaultBC,(yyvsp[0].sval));
		}
#line 1576 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 294 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
                  if(loadingOcaf) 
                     step_file_reloaded=loadingOcaf->importSTEP_OR_DXC((yyvsp[0].sval), reload_step_file);
		}
#line 1585 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 309 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = NULL;}
#line 1591 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 309 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {unit.xm   =(yyvsp[0].fval);  }
#line 1597 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 310 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = NULL;}
#line 1603 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 310 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {unit.xHz  =(yyvsp[0].fval);  }
#line 1609 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 311 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = NULL;}
#line 1615 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 311 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {unit.xOhm =(yyvsp[0].fval);  }
#line 1621 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 332 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = NULL;}
#line 1627 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 332 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {invt->rotAngle=(yyvsp[0].fval); }
#line 1633 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 333 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = &DoubleBuffer; Fbuff->reset();}
#line 1639 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 334 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
              DoubleBuffer.flush(&dVec); 
              for(int i=0; i<3; i++) invt->rotAxis[i]=dVec[i];
            }
#line 1648 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 338 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = &DoubleBuffer; Fbuff->reset();}
#line 1654 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 339 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
              DoubleBuffer.flush(&dVec); 
              for(int i=0; i<3; i++) invt->rotOrigin[i]=dVec[i];
            }
#line 1663 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 343 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = &DoubleBuffer; Fbuff->reset();}
#line 1669 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 344 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
              DoubleBuffer.flush(&dVec); 
              for(int i=0; i<3; i++) invt->trasl[i]=dVec[i];
            }
#line 1678 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 352 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {     
              invt =loadingEmP->FindInvariant(name);
              if(invt) loadingEmP->delInvariant(invt);
	      invt = new DB::Transform();
	      strcpy(invt->name,name);
	      loadingEmP->addInvariant(invt);
	 }
#line 1690 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 374 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = NULL;}
#line 1696 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 374 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {mat->epsr=(yyvsp[0].fval);     }
#line 1702 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 375 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = NULL;}
#line 1708 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 375 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {mat->mur=(yyvsp[0].fval);          }
#line 1714 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 376 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = NULL;}
#line 1720 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 376 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {mat->etandelta=(yyvsp[0].fval); mat->edispersive=1;}
#line 1726 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 377 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = NULL;}
#line 1732 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 377 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {mat->htandelta=(yyvsp[0].fval); mat->edispersive=1;}
#line 1738 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 378 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = NULL;}
#line 1744 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 378 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {mat->econductivity=(yyvsp[0].fval); mat->edispersive=1;}
#line 1750 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 379 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = NULL;}
#line 1756 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 379 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {mat->hconductivity=(yyvsp[0].fval); mat->hdispersive=1; }
#line 1762 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 380 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff  = &mat->buff.epsLorentz; Fbuff->reset();}
#line 1768 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 380 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {mat->edispersive=1;}
#line 1774 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 381 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff  = &mat->buff.muLorentz; Fbuff->reset();}
#line 1780 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 381 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {mat->hdispersive=1;}
#line 1786 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 382 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = NULL;}
#line 1792 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 382 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {mat->Sresistance=(yyvsp[0].fval); }
#line 1798 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 383 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = NULL;}
#line 1804 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 383 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {mat->freqBand[0]=(yyvsp[-1].fval); mat->freqBand[1]=(yyvsp[0].fval); }
#line 1810 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 384 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    { Ibuff = &IntBuffer; Ibuff->reset();}
#line 1816 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 385 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
              IntBuffer.flush(&matColor); 
              for(int i=0; i<4; i++) mat->color[i]=matColor[i];
            }
#line 1825 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 393 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {     
                        mat =loadingEmP->FindMaterial(name);
                        if(mat) loadingEmP->delMaterial(mat);
			mat = new DB::Material();
			strcpy(mat->name,name);
			loadingEmP->addMaterial(mat);
		}
#line 1837 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 404 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    { 
             mat->flush(); 
          }
#line 1845 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 415 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
		   vol->type=DIELECTRIC;
                }
#line 1853 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 419 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
		   vol->type=HOLE;
                }
#line 1861 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 423 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
		   vol->type=BOUNDARYCOND;
                }
#line 1869 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 427 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
		   vol->type=WAVEGUIDE;
                }
#line 1877 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 431 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
		   vol->type=LINEPORT;
                }
#line 1885 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 435 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
		   vol->type=SPLITTER;
                }
#line 1893 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 439 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
		   vol->type=GRID;
                }
#line 1901 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 443 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
		   vol->type=NET;
                }
#line 1909 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 447 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
		   vol->type=COMPONENT;
                }
#line 1917 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 451 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
		   vol->type=INTERFACE;
                }
#line 1925 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 455 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
		   vol->type=ASSEMBLY;
                }
#line 1933 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 459 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
		   vol->type=UNDEFINED;
                }
#line 1941 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 473 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
                   if (strlen((yyvsp[0].sval)) > sizeof(DB::str_t)-1) YYERROR;
                   if(!loadingEmP->FindMaterial((yyvsp[0].sval)))  model_msg(FATAL, "Undefined material %s\n", (yyvsp[0].sval));
		   strcpy(vol->material,(yyvsp[0].sval));
                }
#line 1951 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 478 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Ibuff  =NULL;}
#line 1957 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 479 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
                  vol->TEMportsNum =(yyvsp[0].ival);
                }
#line 1965 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 482 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Ibuff  =NULL;}
#line 1971 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 483 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
                  vol->TEportsNum =(yyvsp[0].ival);
                }
#line 1979 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 486 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Ibuff  =NULL;}
#line 1985 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 487 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
                  vol->TMportsNum =(yyvsp[0].ival);
                }
#line 1993 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 490 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Ibuff  =NULL;}
#line 1999 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 491 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
                  vol->gridNum =(yyvsp[0].ival);
                }
#line 2007 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 494 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Ibuff  =NULL;}
#line 2013 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 495 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
                  vol->PML =(yyvsp[0].ival);
                }
#line 2021 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 498 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Ibuff  =NULL;}
#line 2027 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 499 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
                  vol->invariant =(yyvsp[0].ival);
                }
#line 2035 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 502 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = NULL;}
#line 2041 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 502 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {vol->meshRefinement=(yyvsp[0].fval); }
#line 2047 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 503 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Fbuff = NULL;}
#line 2053 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 503 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {vol->meshRefinement=(yyvsp[0].fval); }
#line 2059 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 505 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {Ibuff = NULL;}
#line 2065 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 505 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {vol->compSolid=(yyvsp[0].ival); }
#line 2071 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 507 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    { }
#line 2077 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 507 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {vol->disabled=1; }
#line 2083 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 511 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
			vol = new DB::Volume();
			strcpy(vol->name,name);
			loadingEmP->insertVolume(vol);
		}
#line 2093 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 529 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1646  */
    {
             if (strlen((yyvsp[0].sval))>sizeof(DB::str_t)-1) YYERROR; 
             else strcpy(mainCircName,(yyvsp[0].sval));
            }
#line 2102 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
    break;


#line 2106 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model_parser.cpp" /* yacc.c:1646  */
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

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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
                  yystos[*yyssp], yyvsp);
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
#line 535 "/home/walter/MwCAD/EmCAD/EmCAD/src/DataBase/model.y" /* yacc.c:1906  */



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
        loadingEmP=&loadingOcaf->EmP;
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


