%{
/******************************************************************
*
*	MWM for C++
*
*	Copyright (C) Walter Steffe` 2004-2004
*
*	File:	mwm.l
*
******************************************************************/
 
#include <stdio.h>
#include <string.h>
#include <mwm_parser.hpp>
#include<List.h>
#include <Message.h>

namespace DB{
 extern char yyFileName[256];
 extern int yyLineNum;
}

namespace MWM{

typedef struct {
  char fileName[256];
  int lineNum;
} fileState;

struct FileStack{
 DB::List_T *stack;
 FileStack(){
  stack=DB::List_Create(2, 2, sizeof(fileState *));
 }
 ~FileStack(){
   fileState *st;
   for(int i=0; i<List_Nbr(stack); i++){
    List_Read(stack,i,&st);
    delete(st);
   }

   List_Delete(stack);
 }
 void push(char *fileName, int lineNum){
  fileState *st;
  st=new fileState();
  strcpy(st->fileName,fileName);
  st->lineNum=lineNum;
  List_Add(stack,&st);
 }
 void pop(char *fileName, int &lineNum){
  fileState *st;
  List_Pop(stack,&st);
  strcpy(fileName,st->fileName);
  lineNum=st->lineNum;
  delete(st);
 }
};

FileStack fStack;
extern int changedBinFile;

}

#define input	yyinput
#define output	yyoutput

%}

%x expectID
%option noyywrap

%x incl
%{
#define MAX_INCLUDE_DEPTH 10
namespace MWM{
 YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];
 int include_stack_ptr = 0;
}
%}


string	\"[^\"]*\"
ID    [[:alnum:]_\.\|\^]* 

%%

-?[0-9]+					{ mwmlval.ival = atoi(yytext); return INTEGER;}
-?[0-9]*\.?[0-9]*([eE][+-]?[0-9]+)?		{ mwmlval.fval = (double)atof(yytext); return FLOAT;}
0x[0-9A-Fa-f]+					{ sscanf(yytext, "0x%x", &mwmlval.ival); return INTEGER; }

","					{ return ','; }
"}"					{ return '}'; }
"{"					{ return '{'; }
"["					{ return '['; }
"]"					{ return ']'; }



<*>[ \t]+			{ }
#.*				{ }
#.*\n				{ DB::yyLineNum++;}
\n				{ DB::yyLineNum++;}

{string}			{ yytext[yyleng-1] = '\0'; mwmlval.sval = yytext+1; return STRING; }
<expectID>{ID}			{ /*puts back ID onto the input stream as a string enclosed in '"' */
          			  /* yytext is copied because unput() trashes yytext */
					int i; char *yycopy = strdup( yytext );
          				unput( '"' );
          				for ( i = yyleng - 1; i >= 0; --i ) unput( yycopy[i] );
          				unput( '"' );
          				free( yycopy );
				}
<expectID>{string}		{ yytext[yyleng-1] = '\0'; mwmlval.sval = yytext+1;
                                  BEGIN(INITIAL); return NAME;}

<incl>[^ \r\t\n]+   { /* got the include file name */
        if ( MWM::include_stack_ptr >= MAX_INCLUDE_DEPTH )
            {
            fprintf( stderr, "Includes nested too deeply" );
            exit( 1 );
            }

        MWM::include_stack[MWM::include_stack_ptr++] =
            YY_CURRENT_BUFFER;

        MWM::fStack.push(DB::yyFileName,DB::yyLineNum);
        strcpy(DB::yyFileName, yytext);
        MWM::changedBinFile=1;
        DB::yyLineNum =1;

        yyin = fopen( yytext, "r" );

        if(!yyin){
         DB::Msg(ERROR, "Cannot open file %s\n", yytext);
	 return 0;
        }

        yy_switch_to_buffer(
            yy_create_buffer( yyin, YY_BUF_SIZE ) );

        BEGIN(INITIAL);
        }

<<EOF>> {
        if ( --MWM::include_stack_ptr < 0 )
            {
             yyterminate();
            }

        else
            {
             MWM::fStack.pop(DB::yyFileName,DB::yyLineNum);

             yy_delete_buffer( YY_CURRENT_BUFFER );
             yy_switch_to_buffer(
                 MWM::include_stack[MWM::include_stack_ptr] );
            }
        }


<INITIAL>"."include		{BEGIN(incl);}


TRUE				{ mwmlval.ival = 1; return INTEGER; }
FALSE				{ mwmlval.ival = 0; return INTEGER; }

DEF				{ BEGIN(expectID); return DEF;}
.set				{ return SET;}
MWM_Tag				{ return MWM_TAG;}
position			{ return POSITION;}
string				{ return TAGSTRING;}
MWM_Surface			{ return MWM_SURFACE; }
MWM_Curve			{ return MWM_CURVE; }
MWM_Units			{ return MWM_UNITS; }
name				{ return NAME; }
length				{ return LENGTH; }
frequency			{ return FREQUENCY; }
volumes				{ return VOLUMES; }
cutoffRefinement		{ return CUTOFFREFINEMENT; }
curves				{ return CURVES; }
curveSign			{ return CURVESIGN; }
curveLength			{ return CURVELENGTH; }
curvePnum			{ return CURVEPNUM; }
PECcurves			{ return PECCURVES; }
ECcurves			{ return ECCURVES; }
PMCcurves			{ return PMCCURVES; }
LPcurves			{ return LPCURVES; }
PECvertices			{ return PECVERTICES; }
ECvertices			{ return ECVERTICES; }
PMCvertices			{ return PMCVERTICES; }
SHAREDvertices			{ return SHAREDVERTICES; }
vertices			{ return VERTICES; }
curveVertices			{ return CURVEVERTICES; }
curvePoints			{ return CURVEPOINTS; }
points				{ return POINTS; }
pointsU				{ return POINTSU; }
pointsUV			{ return POINTSUV; }
pointsI				{ return POINTSI; }
normals				{ return NORMALS; }
curveTangents			{ return CTANGENTS; }
triangles			{ return TRIANGLES; }
triaCenters			{ return TRIACENTERS; }
PROTO {
	   int c=0, bigBracket;
	   while ((c=input()) != '[' && c != EOF) if (c== '\n') DB::yyLineNum++;

	   bigBracket =1;
	   while (bigBracket && (c=input())!=EOF) {
	      switch(c) {
		 case ']' :  bigBracket--;   break;
		 case '[' :  bigBracket++;   break;
		 case '\n':  DB::yyLineNum++;    break;
		 case '#'  : /* skip comment */ {
		    do {c = input();} while (c != '\n');
		    DB::yyLineNum++;
             	 }
	      }
	   } 

	   while ((c=input()) != '{' && c != EOF) if (c== '\n') DB::yyLineNum++;

	   bigBracket =1;
	   while (bigBracket && (c=input())!=EOF) {
	      switch(c) {
		 case '}' :  bigBracket--;   break;
		 case '{' :  bigBracket++;   break;
		 case '\n':  DB::yyLineNum++;    break;
		 case '#'  : /* skip comment */ {
		    do {c = input();} while (c != '\n');
		    DB::yyLineNum++;
             	 }
	      }
	   } 
}



%%


void mwmSetInputFile(FILE *fp)
{
  DB::yyLineNum = 1;
  yyrestart(fp);
}



