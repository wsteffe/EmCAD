/*
 * This file is part of the EmCAD program which constitutes the client
 * side of an electromagnetic modeler delivered as a cloud based service.
 * 
 * Copyright (C) 2015-2020  Walter Steffe
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
 
#include <stdio.h>
#include <string.h>
#include <model_parser.hpp>
#include<ListUtils.h>
#include <Message.h>

extern char modelFileName[256];
int modelLineNum=1;

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

   DB::List_Delete(stack);
 }
 void push(char *fileName, int lineNum){
  fileState *st;
  st=new fileState();
  strcpy(st->fileName,fileName);
  st->lineNum=lineNum;
  DB::List_Add(stack,&st);
 }
 void pop(char *fileName, int &lineNum){
  fileState *st;
  DB::List_Pop(stack,&st);
  strcpy(fileName,st->fileName);
  lineNum=st->lineNum;
  delete(st);
 }
};

FileStack fStack;
extern int changedBinFile;



%}

%x expectID
%option noyywrap
%option never-interactive


%x incl
%{
#define MAX_INCLUDE_DEPTH 10
YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];
int include_stack_ptr = 0;
%}


string	\"[^\"]*\"
ID    [[:alnum:]_\.\|\^]* 

%%

-?[0-9]+					{ modellval.ival = atoi(yytext); return INTEGER;}
-?[0-9]*\.?[0-9]*([eE][+-]?[0-9]+)?		{ modellval.fval = (double)atof(yytext); return FLOATING;}
0x[0-9A-Fa-f]+					{ sscanf(yytext, "0x%x", &modellval.ival); return INTEGER; }

","					{ return ','; }
"}"					{ return '}'; }
"{"					{ return '{'; }
"["					{ return '['; }
"]"					{ return ']'; }



<*>[ \t]+			{ }
#.*				{ }
#.*\n				{ modelLineNum++;}
\n				{ modelLineNum++;}

{string}			{ yytext[yyleng-1] = '\0'; modellval.sval = &yytext[1]; return STRING; }
<expectID>{ID}			{ /*puts back ID onto the input stream as a string enclosed in '"' */
          			  /* yytext is copied because unput() trashes yytext */
					int i; 
                                        char *yycopy = new char [ strlen(yytext) + 1 ];
                                        strcpy(yycopy, yytext);
          				unput( '"' );
          				for ( i = yyleng - 1; i >= 0; --i ) unput( yycopy[i] );
          				unput( '"' );
          				delete [] yycopy ;
				}
<expectID>{string}		{ yytext[yyleng-1] = '\0'; modellval.sval = &yytext[1];
                                  BEGIN(INITIAL); return NAME;}

<incl>[^ \t\n]+   { /* got the include file name */
        if ( include_stack_ptr >= MAX_INCLUDE_DEPTH )
            {
            fprintf( stderr, "Includes nested too deeply" );
            exit( 1 );
            }

        include_stack[include_stack_ptr++] =
            YY_CURRENT_BUFFER;

        fStack.push(modelFileName,modelLineNum);
        strcpy(modelFileName, yytext);
        changedBinFile=1;
        modelLineNum =1;

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
        if ( --include_stack_ptr < 0 )
            {
             yyterminate();
            }

        else
            {
             fStack.pop(modelFileName,modelLineNum);

             yy_delete_buffer( YY_CURRENT_BUFFER );
             yy_switch_to_buffer(
                 include_stack[include_stack_ptr] );
            }
        }


Import				{ return IMPORT;}

<INITIAL>"."include		{BEGIN(incl);}


TRUE				{ modellval.ival = 1; return INTEGER; }
FALSE				{ modellval.ival = 0; return INTEGER; }

DEF				{ BEGIN(expectID); return DEF;}
.set				{ return SET;}
MWM_Material			{ return MWM_MATERIAL; }
MWM_Volume			{ return MWM_VOLUME; }
MWM_Units			{ return MWM_UNITS; }
MWM_Invariant			{ return MWM_INVARIANT; }
AssemblyType			{ return ASSEMBLYTYPE; }
Level		 	        { return LEVEL; }
Dielectric			{ return tDIELECTRIC; }
Hole				{ return tHOLE; }
DefaultBoundCond                { return DEFAULTBC; }
CircuitName			{ return CIRCUITNAME; }
BoundaryCondition		{ return tBOUNDARYCONDITION; }
Net	  	 		{ return tNET; }
Component 		        { return tCOMPONENT; }
Interface 		        { return tINTERFACE; }
Assembly 		        { return tASSEMBLY; }
WaveGuide			{ return tWAVEGUIDE; }
LinePort			{ return tLINEPORT; }
Splitter		 	{ return tSPLITTER; }
Grid			 	{ return tGRID; }
Undefined			{ return tUNDEFINED; }
name				{ return NAME; }
type				{ return VOLTYPE; }
length				{ return LENGTH; }
frequency			{ return FREQUENCY; }
band	                        { return BAND; }
resistance			{ return RESISTANCE; }
inductance			{ return INDUCTANCE; }
impedance			{ return IMPEDANCE; }
lossfactor			{ return LOSSFACTOR; }
Qfactor			        { return QFACTOR; }
surfRatio			{ return SURFRATIO; }
ballRadius			{ return BALLRADIUS; }
Rz			        { return RZ; }
Rq			        { return RQ; }
modelType			{ return MODELTYPE; }
epsLorentz			{ return EPSLORENTZ; }
muLorentz			{ return MULORENTZ; }
polesResidues			{ return POLESRESIDUES; }
polesNum			{ return POLESNUM; }
surface		                { return SURFACE; }
rough		                { return ROUGH; }
epsilonr			{ return EPSILONR; }
mur				{ return MUR; }
Conductivity                    { return ECONDUCTIVITY; }
eConductivity                   { return ECONDUCTIVITY; }
hConductivity                   { return HCONDUCTIVITY; }
mConductivity                   { return HCONDUCTIVITY; }
eTanDelta                       { return ETANDELTA; }
hTanDelta                       { return HTANDELTA; }
color                           { return COLOR; }
material			{ return MATERIAL; }
TEMportsNum			{ return TEMPORTSNUM; }
TEportsNum			{ return TEPORTSNUM; }
TMportsNum			{ return TMPORTSNUM; }
gridNum			        { return GRIDNUM; }
PML			        { return PML; }
invariant			{ return INVARIANT; }
translation			{ return TRANSLATION; }
rotation			{ return ROTATION; }
axis			        { return AXIS; }
origin			        { return ORIGIN; }
angle			        { return ANGLE; }
meshRefinement			{ return MESHREFINEMENT; }
cutoffRefinement		{ return CUTOFFREFINEMENT; }
compSolid			{ return COMPSOLID; }
disabled                        { return DISABLED; }
volumes				{ return VOLUMES; }
solid				{ return SOLID; }
PROTO {
	   int c=0, bigBracket;
	   while ((c=yyinput()) != '[' && c != EOF) if (c== '\n') modelLineNum++;

	   bigBracket =1;
	   while (bigBracket && (c=yyinput())!=EOF) {
	      switch(c) {
		 case ']' :  bigBracket--;   break;
		 case '[' :  bigBracket++;   break;
		 case '\n':  modelLineNum++;    break;
		 case '#'  : /* skip comment */ {
		    do {c = yyinput();} while (c != '\n');
		    modelLineNum++;
             	 }
	      }
	   } 

	   while ((c=yyinput()) != '{' && c != EOF) if (c== '\n') modelLineNum++;

	   bigBracket =1;
	   while (bigBracket && (c=yyinput())!=EOF) {
	      switch(c) {
		 case '}' :  bigBracket--;   break;
		 case '{' :  bigBracket++;   break;
		 case '\n':  modelLineNum++;    break;
		 case '#'  : /* skip comment */ {
		    do {c = yyinput();} while (c != '\n');
		    modelLineNum++;
             	 }
	      }
	   } 
}



%%


void SetInputFile(FILE *fp)
{
  modelLineNum = 1;
  yyrestart(fp);
}


