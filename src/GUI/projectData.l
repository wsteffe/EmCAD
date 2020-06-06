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
#include <projectData_parser.hpp>
#include<ListUtils.h>
#include <Message.h>

extern char projectDataFileName[256];
int projectDataLineNum=1;

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

FileStack projectData_fStack;



%}

%x expectID
%option noyywrap
%option never-interactive


%x incl
%{
#define MAX_INCLUDE_DEPTH 10
YY_BUFFER_STATE projectData_include_stack[MAX_INCLUDE_DEPTH];
int projectData_include_stack_ptr = 0;
%}


string	\"[^\"]*\"
ID    [[:alnum:]_\.\|\^]* 

%%

-?[0-9]+					{ projectDatalval.ival = atoi(yytext); return INTEGER;}
-?[0-9]*\.?[0-9]*([eE][+-]?[0-9]+)?		{ projectDatalval.fval = (double)atof(yytext); return FLOATING;}
0x[0-9A-Fa-f]+					{ sscanf(yytext, "0x%x", &projectDatalval.ival); return INTEGER; }

","					{ return ','; }
"}"					{ return '}'; }
"{"					{ return '{'; }
"["					{ return '['; }
"]"					{ return ']'; }



<*>[ \t]+			{ }
#.*				{ }
#.*\n				{ projectDataLineNum++;}
\n				{ projectDataLineNum++;}

{string}			{ yytext[yyleng-1] = '\0'; projectDatalval.sval = &yytext[1]; return STRING; }
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
<expectID>{string}		{ yytext[yyleng-1] = '\0'; projectDatalval.sval = &yytext[1];
                                  BEGIN(INITIAL); return NAME;}

<incl>[^ \t\n]+   { /* got the include file name */
        if ( projectData_include_stack_ptr >= MAX_INCLUDE_DEPTH )
            {
            fprintf( stderr, "Includes nested too deeply" );
            exit( 1 );
            }

        projectData_include_stack[projectData_include_stack_ptr++] =
            YY_CURRENT_BUFFER;

        projectData_fStack.push(projectDataFileName,projectDataLineNum);
        strcpy(projectDataFileName, yytext);
        projectDataLineNum =1;

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
        if ( --projectData_include_stack_ptr < 0 )
            {
             yyterminate();
            }

        else
            {
             projectData_fStack.pop(projectDataFileName,projectDataLineNum);

             yy_delete_buffer( YY_CURRENT_BUFFER );
             yy_switch_to_buffer(
                 projectData_include_stack[projectData_include_stack_ptr] );
            }
        }



<INITIAL>"."include		{BEGIN(incl);}


TRUE				{ projectDatalval.ival = 1; return INTEGER; }
FALSE				{ projectDatalval.ival = 0; return INTEGER; }

DEF				{ BEGIN(expectID); return DEF;}
mainAssName			{ return MAINASSNAME; }
varFilePath			{ return VARFILEPATH; }
s2pFilePath			{ return S2PFILEPATH; }
network		   	        { return NETWORK; }
unit		 	        { return UNIT; }
length			        { return LENGTH; }
mor	                        { return MOR; }
ana	                        { return ANA; }
symmetric	                { return SYMMETRIC; }
response                        { return RESPONSE; }
port		                { return PORT; }
impedance	                { return IMPEDANCE; }
zero                            { return ZERO; }
pole                            { return POLE; }
window                          { return WINDOW; }
circle                          { return CIRCLE; }
curve                           { return CURVE; }
xscale                          { return XSCALE; }
yscale                          { return YSCALE; }
auto                            { return AUTO; }
ideal                           { return IDEAL; }
type                            { return TYPE; }
topology                        { return TOPOLOGY; }
part                            { return PART; }
parameter                       { return PARAMETER; }
freq	                        { return FREQ; }
num	                        { return NUM; }
filter	                	{ return FILTER; }
krylov	                	{ return KRYLOV; }
order	                	{ return ORDER; }
cutoff	                	{ return CUTOFF; }
min	                	{ return tMIN; }
max	                	{ return tMAX; }
ratio	                	{ return RATIO; }
returnloss	                { return RETURNLOSS; }
outband		                { return OUTBAND; }
Qfactor	                	{ return QFACTOR; }
uniform	                	{ return UNIFORM; }
inductive	                { return INDUCTIVE; }
skin		                { return SKIN; }
lossy	                	{ return LOSSY; }
predistorted                	{ return PREDISTORTED; }
optimize	               	{ return OPTIMIZE; }
ilosspp	                	{ return ILOSSPP; }
trustR	                	{ return TRUSTR; }
tx	                	{ return TX; }
zeros	                	{ return ZEROS; }
pass	                	{ return PASS; }
automatic	  	 	{ return AUTOMATIC; }
design	  	 		{ return DESIGN; }
mapping	  	 	        { return MAPPING; }
tuning	  	 	        { return TUNING; }
only	  	 	        { return ONLY; }
transversej	  	 	{ return TRANSVERSEJ; }
method	  	 	        { return METHOD; }
source	  	 	        { return SOURCE; }
recompute	  	 	{ return RECOMPUTE;}
error		  	 	{ return ERRORT;}
xtol		  	 	{ return XTOL;}
trustd		  	 	{ return TRUSTR;}
graddx		  	 	{ return GRADDX;}
jacobian	  	 	{ return JACOBIAN;}
mapped	  	 	        { return MAPPED; }
itermax	                        { return ITERMAX; }
band	                        { return BAND; }
exp	                        { return EXP; }
mesh	                        { return MESH; }
refine	                        { return REFINE; }
energy	                        { return ENERGY; }
meshing3d	                { return MESHING3D; }
local	                        { return LOCAL; }
wavelength	                { return WAVELENGTH; }
ratio                           { return RATIO; }
decomposition                   { return DECOMPOSITION; }
first                           { return FIRST; }
material                        { return MATERIAL; }
remesh                          { return REMESH; }
modelization                    { return MODELIZATION; }
component                       { return COMPONENT; }
save                            { return SAVE; }
reduction                       { return REDUCTION; }
reload                          { return RELOAD; }
needed                          { return NEEDED; }
changed                         { return CHANGED; }



%%


void projectDataSetInputFile(FILE *fp)
{
  projectDataLineNum = 1;
  yyrestart(fp);
}


