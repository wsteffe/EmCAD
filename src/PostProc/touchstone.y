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

#include <Message.h>
#include <stdio.h>
#include <QString>
#include <QStringList>

#include "plotData.h"

#define YYINITDEPTH  (1024 * 64)
#define	YYMAXDEPTH	(YYINITDEPTH * 64)


extern char *touchstonetext;
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


void touchstone_msg(int type, char *fmt, ...);


int freqi;
int curvei;
int compi;
int fNum;
int cNum;

extern PlotData plotData;



%} 

%union {
int	ival;
double	fval;
char	*sval;
}

%token <ival> INTEGER
%token <fval> FLOATING
%token <sval> STRING PORTNAME

%token NUMBEROFPORTS NUMBEROFFREQ TWOPORTDATAORDER ORDER_21_12 ORDER_12_21 NETWORKDATA
%token MATRIXFORMAT FULL
%token OPTIONS HZ KHZ MHZ GHZ tDB tMA tRI
%token tS tY tZ tH tG tR

%type <fval> Number

%start All


%%

All
        : TouchItems
        ;

TouchItems 
         :  // nothing
         | TouchItems TouchItem
         ;


TouchItem
	: SetOptions
	| SetNumberOfPorts
	| SetNumberOfFreq
	| SetTwoPortOrder
	| SetPortName
	| SetMatrixFormat
	| StartNetworkData
        | NetParValues
	| error		{YYABORT;}
	;




Number
	: FLOATING
	| INTEGER {$$ = (double)$1;}
	;

//*** 

FreqUnit
	: HZ  {strcpy(plotData.frequnit,"Hz");}
        | KHZ {strcpy(plotData.frequnit,"KHz");}
        | MHZ {strcpy(plotData.frequnit,"MHz");}
        | GHZ {strcpy(plotData.frequnit,"GHz");}
        ;

Format
	: tDB {strcpy(plotData.format,"DB");}
        | tMA {strcpy(plotData.format,"MA");}
        | tRI {strcpy(plotData.format,"RI");}
        ;

Param
	: tS {plotData.param='S';}
        | tY {plotData.param='Y';}
        | tZ {plotData.param='Z';}
        | tH {plotData.param='H';}
        | tG {plotData.param='G';}
        ;



SetOptions
	: OPTIONS  FreqUnit  Param  Format tR  Number
		{
                  plotData.R=$6;
		}
	;

SetNumberOfPorts
	: NUMBEROFPORTS  Number
		{
			plotData.numberOfPorts=$2;
		}
	;

SetNumberOfFreq
	: NUMBEROFFREQ  Number
		{
			fNum=plotData.numberOfFreq=$2;
		}
	;

SetTwoPortOrder
	: TWOPORTDATAORDER  ORDER_21_12
		{
		}
	| TWOPORTDATAORDER  ORDER_12_21
		{
		}
	;

SetMatrixFormat
	: MATRIXFORMAT  FULL
        ;

StartNetworkData
	: NETWORKDATA 
          {
                 curvei=-1; freqi=0; compi=0;
                 cNum=plotData.numberOfCurves=plotData.numberOfPorts*plotData.numberOfPorts;
                 plotData.frequencies.resize(plotData.numberOfFreq);
                 plotData.curveArray.resize(fNum*2*cNum);
          } 
	;

NetParVal
	:  FLOATING
           {
                   if(curvei==-1){
                       plotData.frequencies[freqi]=$1;
                       curvei++;
                   } else {
                       plotData.curveArray[freqi+compi*fNum+curvei*2*fNum]=$1; 
                       if(compi==1){
                          curvei++;
                          if(curvei==plotData.numberOfCurves) {curvei=-1; freqi++;}
                          compi=0;
                       } else compi=1;
                   }
           }
	;

NetParValues
	: NetParVal
	| NetParValues NetParVal
	;

SetPortName
        :PORTNAME {plotData.portnames<<$1;}
        ;

%%


extern int touchstoneLineNum;
extern void SetTouchstoneFile(FILE *fp);

char touchstoneFileName[256];


void yyerror(char *s){
  printf("\n  %s (%s) at file: %s, line: %d\n\n", s, touchstonetext, touchstoneFileName, touchstoneLineNum);
}

static void yyprint (FILE *file, int type, YYSTYPE value)
{
  if (type==STRING)
     fprintf(file, " %s", value.sval);
  else if (type==FLOATING)
     fprintf(file, " %f", value.fval);
  else if (type==INTEGER)
     fprintf(file, " %d", value.ival);
}


void touchstone_msg(int type, char *fmt, ...){
  va_list args;
  char tmp[1024];

  va_start (args, fmt);
  vsprintf (tmp, fmt, args);
  va_end (args);

  DB::Msg(type, "'%s', line %d : %s", touchstoneFileName, touchstoneLineNum, tmp);

}

int readTouchstone(const char *fName) 
{
        FILE *fid;
        if(!(fid = fopen(fName, "r"))){
		DB::Msg(ERROR, "Cannot open file %s\n", fName);
		return 1;
	}
        strncpy(touchstoneFileName, fName, 255);
        SetTouchstoneFile(fid);
        int result=yyparse();
        if(strcmp(fName,"-")) fclose(fid);
	return result;
}



