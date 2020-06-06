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
#include <touchstone_parser.hpp>
#include <limits>
#include <math.h>
#include <Message.h>


extern char touchstoneFileName[256];
int touchstoneLineNum=1;


typedef struct {
  char fileName[256];
  int lineNum;
} fileState;




%}

%x skipline_state
%x options_state
%x portnames_state

%option noyywrap
%option never-interactive


string	\"[^\"]*\"

%%

"![Port Names]"		{ BEGIN(portnames_state); }
<portnames_state>\n           BEGIN(INITIAL);

"!"  BEGIN(skipline_state);
<skipline_state>[^\n]*        /* eat anything that's not a '\n' */
<skipline_state>\n             BEGIN(INITIAL);

"#"    { BEGIN(options_state); return OPTIONS;}
<options_state>\n             BEGIN(INITIAL);


<options_state,INITIAL>-?[0-9]+					{double d=(double)atof(yytext); 
                                                                  if(fabs(d)< std::numeric_limits<int>::max()){
                                                                    touchstonelval.ival =atoi(yytext); return INTEGER;
                                                                  } else {touchstonelval.fval =d; return FLOATING;}
                                                                }
<options_state,INITIAL>-?[0-9]*\.?[0-9]*([eE][+-]?[0-9]+)?	{ touchstonelval.fval = (double)atof(yytext); return FLOATING;}



<*>[ \t]+			{ }
\r                              { }
\n				{ touchstoneLineNum++;}

{string}			{ yytext[yyleng-1] = '\0'; touchstonelval.sval = &yytext[1]; return STRING; }
<portnames_state>{string}		{ yytext[yyleng-1] = '\0'; touchstonelval.sval = &yytext[1]; return PORTNAME; }


<<EOF>> {
             yyterminate();
        }


"[Version]"                   BEGIN(skipline_state);
"[Number of Ports]"		{ return NUMBEROFPORTS; }
"[Number of Frequencies]"	{ return NUMBEROFFREQ; }
"[Two-Port Data Order]"         { return TWOPORTDATAORDER; }
"[Network Data]"                { return NETWORKDATA; }
"[Matrix Format]"               { return MATRIXFORMAT; }
"Full"                          { return FULL; }
21_12                           { return ORDER_21_12; }
12_21                           { return ORDER_12_21; }
<options_state>Hz                     { return HZ; }
<options_state>HZ                     { return HZ; }
<options_state>KHz                    { return KHZ; }
<options_state>KHZ                    { return KHZ; }
<options_state>MHz                    { return MHZ; }
<options_state>MHZ                    { return MHZ; }
<options_state>GHz                    { return GHZ; }
<options_state>GHZ                    { return GHZ; }
<options_state>dB                     { return tDB; }
<options_state>DB                     { return tDB; }
<options_state>MA                     { return tMA; }
<options_state>RI                     { return tRI; }
<options_state>S                      { return tS; }
<options_state>Y                      { return tY; }
<options_state>Z                      { return tZ; }
<options_state>G                      { return tG; }
<options_state>R                      { return tR; }

%%


void SetTouchstoneFile(FILE *fp)
{
  touchstoneLineNum = 1;
  yyrestart(fp);
}


