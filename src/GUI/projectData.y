/*
 * This file is part of the EmCAD program which constitutes the client
 * side of an electromagnetic modeler delivered as a cloud based service.
 * 
 * Copyright (C) 2015  Walter Steffe
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


%{
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

%} 

%union {
int	ival;
double	fval;
char	*sval;
}

%token <ival> INTEGER
%token <fval> FLOATING
%token <sval> STRING NAME

%token DEF MAINASSNAME NETWORK LENGTH FREQ UNIT EXP MESH WAVELENGTH RATIO ANA BAND NUM MOR
%token RESPONSE PARAMETER TYPE PART XSCALE YSCALE AUTO ZERO POLE CURVE
%token FILTER PASS ORDER RETURNLOSS QFACTOR MAPPING METHOD ITERMAX AUTOMATIC MAPPED TX ZEROS
%token REMESH FIRST DECOMPOSITION MATERIAL MODELIZATION COMPONENT SAVE REDUCTION RELOAD NEEDED DONE CHANGED


%type <fval> SFFloat
%type <ival> SFBool SFInt32

%start All

%%

All
        : PrjDataItems
        ;

PrjDataItems 
         :  // nothing
         | PrjDataItems PrjDataItem
         ;


PrjDataItem
	: CAD
        | Units
        | Mesh
        | FreqAna
        | FilterDesign
        | Mor
        | WorkStatus
	| error		{YYABORT;}
	;




//***	BaseType


SFInt32
	: INTEGER
	;

SFBool
	: INTEGER
	;


SFFloat
	: FLOATING
		{
		      if(ProjData::Fbuff) ProjData::Fbuff->push_back($1);
		}
	| INTEGER
		{
		       $$ = (double)$1;
		       if(ProjData::Fbuff) ProjData::Fbuff->push_back((double)$1);
		}
	;


SFVec2f
	: SFFloat SFFloat {}
	;


SFVec3f
	: SFFloat SFFloat SFFloat {}
	;


SFInt32List
	: SFInt32
	| SFInt32List SFInt32
	| SFInt32List ','
	|
	;

MFInt32
	: SFInt32 {}
	| '[' SFInt32List ']' {}
	; 


SFFloatList
	: SFFloat
	| SFFloatList SFFloat
	| SFFloatList ',' 
	|
	;

MFFloat
	: SFFloat {}
	| '[' SFFloatList ']' {}
	; 


SFVec2fList
	: SFVec2f
	| SFVec2fList SFVec2f
	| SFVec2fList ','
	|
	;

MFVec2f
	: SFVec2f
	| '[' SFVec2fList ']'
	; 

SFVec3fList
	: SFVec3f
	| SFVec3fList SFVec3f
	| SFVec3fList ','
	|
	;

MFVec3f
	: SFVec3f
	| '[' SFVec3fList ']'
	; 



StringList
	 :  STRING {strList->add($1);}
	 |  StringList STRING {strList->add($2);}
	 |  StringList ','
	 ;

MFString
	: '[' StringList ']'
	; 

NodeBegin
	: '{'
	;

NodeEnd
	: '}'
	| '}' ','
	;

//*** 
CAD:
	   MAINASSNAME STRING
            {
              prjData.mainAssName=$2;
            }
        |  NETWORK  INTEGER
            {
              prjData.network=$2;
            }
	;

Units:
           LENGTH UNIT STRING
            {
              strcpy(prjData.lengthUnitName,$3);
            }
        |  FREQ UNIT EXP INTEGER
            {
              prjData.freqUnitE =$4;
            }
	;

Mesh:
           MESH WAVELENGTH RATIO INTEGER
            {
              prjData.meshPerWavelen=$4;
            }

Mor:
           MOR FREQ BAND SFFloat SFFloat
            {
              prjData.freqBand[0]=$4;
              prjData.freqBand[1]=$5;
            }
        |  MOR FREQ NUM INTEGER
            {
              prjData.MORFreqNum=$4;
            }
	;

FreqAna:
           ANA FREQ BAND SFFloat SFFloat
            {
              prjData.anaFreqBand[0]=$4;
              prjData.anaFreqBand[1]=$5;
            }
        |  ZERO POLE FREQ BAND SFFloat SFFloat
            {
              prjData.zpFreqBand[0]=$5;
              prjData.zpFreqBand[1]=$6;
            }
        |  ANA FREQ NUM INTEGER
            {
              prjData.anaFreqNum=$4;
            }
        |  ANA MOR FREQ NUM INTEGER
            {
              prjData.anaMORFreqNum=$5;
            }
        |  FILTER MAPPING METHOD INTEGER
            {
              prjData.filtermapMethod=$4;
            }
        |  FILTER MAPPING ITERMAX INTEGER
            {
              prjData.filtermapItermax=$4;
            }
        |  FREQ RESPONSE TYPE INTEGER
            {
              prjData.freqRespParType = (FreqRespParType) $4;
            }
        |  FREQ RESPONSE PART INTEGER
            {
              prjData.freqRespParPart = $4;
            }
        |  FREQ RESPONSE YSCALE SFFloat SFFloat SFFloat
            {
              prjData.freqRespYscale[0]=$4;
              prjData.freqRespYscale[1]=$5;
              prjData.freqRespYscale[2]=$6;
            }
        |  FREQ RESPONSE YSCALE AUTO INTEGER
            {
              prjData.freqRespYscaleAuto=$5;
            }
        |  ZERO POLE YSCALE SFFloat SFFloat SFFloat
            {
              prjData.zeropoleYscale[0]=$4;
              prjData.zeropoleYscale[1]=$5;
              prjData.zeropoleYscale[2]=$6;
            }
        |  ZERO POLE YSCALE AUTO INTEGER
            {
              prjData.zeropoleYscaleAuto=$5;
            }
        |  ZERO POLE CURVE INTEGER INTEGER
            {
              std::array<int, 2> Sij_ports;
              Sij_ports[0]=$4;
              Sij_ports[1]=$5;
              prjData.zeropoleCurves.insert(Sij_ports);
            }
        |  AUTOMATIC FREQ RESPONSE  INTEGER
            {
              prjData.autoFreqResponse=$4;
            }
        |  AUTOMATIC ZERO POLE INTEGER
            {
              prjData.autoZeropole=$4;
            }
        |  AUTOMATIC MAPPED FREQ RESPONSE INTEGER
            {
              prjData.autoMappedFreqResponse=$5;
            }
        |  AUTOMATIC MAPPED ZERO POLE  INTEGER
            {
              prjData.autoMappedZeropole=$5;
            }
        |  AUTOMATIC FILTER MAPPING  INTEGER
            {
              prjData.autoFilterMapping=$4;
            }
	;

FilterDesign:
           FILTER PASS BAND SFFloat SFFloat
            {
              prjData.filterPassBand[0]=$4;
              prjData.filterPassBand[1]=$5;
            }
        |  FILTER ORDER INTEGER
            {
              prjData.filterOrder = $3;
            }
        |  FILTER RETURNLOSS SFFloat
            {
              prjData.filterRetLoss = $3;
            }
        |  FILTER QFACTOR SFFloat
            {
              prjData.filterQfactor = $3;
            }
        |  FILTER TX ZEROS {ProjData::Fbuff =&prjData.filterZeros; ProjData::Fbuff->clear();} MFFloat
            {
              ProjData::Fbuff=NULL;
            }

WorkStatus:
	   DECOMPOSITION  NEEDED INTEGER
            {
              prjData.workStatus.decompositionNeeded=$3;
            }
	|  FIRST DECOMPOSITION INTEGER
            {
              prjData.workStatus.firstDecomposition=$3;
            }
        |  MATERIAL CHANGED  INTEGER
            {
              prjData.workStatus.materialChanged=$3;
            }
        |  REMESH  NEEDED  INTEGER
            {
              prjData.workStatus.remeshNeeded=$3;
            }
        |  MODELIZATION  NEEDED  INTEGER
            {
              prjData.workStatus.modelizationNeeded=$3;
            }
        |  COMPONENT SAVE  NEEDED  INTEGER
            {
              prjData.workStatus.componentsaveNeeded=$4;
            }
        |  REDUCTION  NEEDED  INTEGER
            {
              prjData.workStatus.reductionNeeded=$3;
            }
        |  RELOAD  NEEDED  INTEGER
            {
              prjData.workStatus.reloadNeeded=$3;
            }

%%


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




