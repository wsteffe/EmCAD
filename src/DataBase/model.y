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

%} 

%union {
int	ival;
double	fval;
char	*sval;
}

%token <ival> INTEGER
%token <fval> FLOATING
%token <sval> STRING NAME

%token IMPORT
%token ASSEMBLYTYPE  LEVEL
%token DEF SET CIRCUITNAME DEFAULTBC
%token MWM_MATERIAL TEMPORTSNUM TEPORTSNUM TMPORTSNUM GRIDNUM PML INVARIANT TRANSLATION ROTATION ANGLE ORIGIN AXIS MWM_VOLUME MWM_INVARIANT MWM_UNITS MWM_LINEPORT
%token LENGTH FREQUENCY BAND RESISTANCE SURFACERESISTANCE MESHREFINEMENT COMPSOLID
%token VOLTYPE EPSILONR MUR EPSLORENTZ MULORENTZ ECONDUCTIVITY HCONDUCTIVITY ETANDELTA HTANDELTA  MATERIAL COLOR
%token VOLUMES DISABLED
%token tDIELECTRIC tHOLE tBOUNDARYCONDITION tWAVEGUIDE tLINEPORT tSPLITTER tGRID tCOMPONENT tINTERFACE tNET tASSEMBLY tUNDEFINED
%token SOLID


%type <fval> SFFloat
%type <ival> SFBool SFInt32

%start All


%%

All
        : MwmItems
        ;

MwmItems 
         :  // nothing
         | MwmItems MwmItem
         ;


MwmItem
	: Units
	| SetCircuitType
	| Invariant
        | Level
        | DefaultBoundCond
	| Import
	| Material
	| Volume
	| CircuitName
	| error		{YYABORT;}
	;




//***	BaseType


SFInt32
	: INTEGER
		{
			if(Ibuff) Ibuff->input($1);
		}
	;

SFBool
	: INTEGER
	;


SFFloat
	: FLOATING
		{
			if(Fbuff) Fbuff->input($1);
		}
	| INTEGER
		{
			$$ = (double)$1;
			if(Fbuff) Fbuff->input((double)$1);
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

Name  : NAME {if (strlen($1) > sizeof(DB::str_t)-1) YYERROR; else strcpy(name,$1);}

// ***   SetCircuitType

SetCircuitType
	: ASSEMBLYTYPE  SFInt32
		{
			loadingEmP->assemblyType=$2;
		}
	;

Level
	: LEVEL  SFInt32
		{
			loadingEmP->level=$2;
		}
	;

DefaultBoundCond
	: DEFAULTBC  STRING
		{
	             strcpy(loadingEmP->defaultBC,$2);
		}
	;



// ***   Import

Import
	: IMPORT  STRING 
		{
                  if(loadingOcaf) 
                     step_file_reloaded=loadingOcaf->importSTEP_OR_DXC($2, reload_step_file);
		}
	;


// ***   Units

UnitsElements
	: UnitsElement UnitsElements
	|
	;

UnitsElement
	:  LENGTH	     {Fbuff = NULL;}    SFFloat    {unit.xm   =$3;  }
	|  FREQUENCY	     {Fbuff = NULL;}    SFFloat    {unit.xHz  =$3;  }
	|  RESISTANCE	     {Fbuff = NULL;}    SFFloat    {unit.xOhm =$3;  }
	;

UnitsBegin
	: DEF Name MWM_UNITS 
	;

Units
	: UnitsBegin NodeBegin UnitsElements NodeEnd
	;



// ***   Invariant

InvariantElements
	: InvariantElement InvariantElements
	|
	;

InvariantElement
	:  ROTATION  ANGLE   {Fbuff = NULL;}    SFFloat             {invt->rotAngle=$4; }
	|  ROTATION  AXIS {Fbuff = &DoubleBuffer; Fbuff->reset();}      MFVec3f
            {
              DoubleBuffer.flush(&dVec); 
              for(int i=0; i<3; i++) invt->rotAxis[i]=dVec[i];
            }
	|  ROTATION  ORIGIN {Fbuff = &DoubleBuffer; Fbuff->reset();}      MFVec3f
            {
              DoubleBuffer.flush(&dVec); 
              for(int i=0; i<3; i++) invt->rotOrigin[i]=dVec[i];
            }
        |  TRANSLATION    {Fbuff = &DoubleBuffer; Fbuff->reset();}      MFVec3f
            {
              DoubleBuffer.flush(&dVec); 
              for(int i=0; i<3; i++) invt->trasl[i]=dVec[i];
            }
	;

InvariantBegin
	: DEF Name  MWM_INVARIANT 
	 {     
              invt =loadingEmP->FindInvariant(name);
              if(invt) loadingEmP->delInvariant(invt);
	      invt = new DB::Transform();
	      strcpy(invt->name,name);
	      loadingEmP->addInvariant(invt);
	 }

Invariant
	: InvariantBegin NodeBegin InvariantElements NodeEnd
	;



// ***   Material

MaterialElements
	: MaterialElement MaterialElements
	|
	;

MaterialElement
	:  EPSILONR	        {Fbuff = NULL;}    SFFloat             {mat->epsr=$3;     }
	|  MUR		        {Fbuff = NULL;}    SFFloat             {mat->mur=$3;          }
	|  ETANDELTA            {Fbuff = NULL;}    SFFloat             {mat->etandelta=$3; mat->edispersive=1;}
	|  HTANDELTA            {Fbuff = NULL;}    SFFloat             {mat->htandelta=$3; mat->edispersive=1;}
	|  ECONDUCTIVITY        {Fbuff = NULL;}    SFFloat             {mat->econductivity=$3; mat->edispersive=1;}
	|  HCONDUCTIVITY        {Fbuff = NULL;}    SFFloat             {mat->hconductivity=$3; mat->hdispersive=1; }
	|  EPSLORENTZ	        {Fbuff  = &mat->buff.epsLorentz; Fbuff->reset();}   MFVec3f  {mat->edispersive=1;}
	|  MULORENTZ            {Fbuff  = &mat->buff.muLorentz; Fbuff->reset();}    MFVec3f  {mat->hdispersive=1;}
	|  SURFACERESISTANCE    {Fbuff = NULL;}    SFFloat             {mat->Sresistance=$3; }
        |  FREQUENCY BAND       {Fbuff = NULL;}    SFFloat SFFloat     {mat->freqBand[0]=$4; mat->freqBand[1]=$5; }
        |  COLOR                { Ibuff = &IntBuffer; Ibuff->reset();}	   MFInt32
            {
              IntBuffer.flush(&matColor); 
              for(int i=0; i<4; i++) mat->color[i]=matColor[i];
            }
	;

MaterialBegin
	: DEF Name  MWM_MATERIAL 
		{     
                        mat =loadingEmP->FindMaterial(name);
                        if(mat) loadingEmP->delMaterial(mat);
			mat = new DB::Material();
			strcpy(mat->name,name);
			loadingEmP->addMaterial(mat);
		}
	;

Material
	: MaterialBegin NodeBegin MaterialElements NodeEnd
          { 
             mat->flush(); 
          }
	;


//***   Volume


VolumeType
	:  VOLTYPE	  tDIELECTRIC
                {
		   vol->type=DIELECTRIC;
                }
	|  VOLTYPE	  tHOLE
                {
		   vol->type=HOLE;
                }
	|  VOLTYPE	  tBOUNDARYCONDITION
                {
		   vol->type=BOUNDARYCOND;
                }
	|  VOLTYPE   tWAVEGUIDE
                {
		   vol->type=WAVEGUIDE;
                }
	|  VOLTYPE   tLINEPORT
                {
		   vol->type=LINEPORT;
                }
	|  VOLTYPE   tSPLITTER
                {
		   vol->type=SPLITTER;
                }
	|  VOLTYPE   tGRID
                {
		   vol->type=GRID;
                }
	|  VOLTYPE   tNET
                {
		   vol->type=NET;
                }
	|  VOLTYPE   tCOMPONENT
                {
		   vol->type=COMPONENT;
                }
	|  VOLTYPE   tINTERFACE
                {
		   vol->type=INTERFACE;
                }
	|  VOLTYPE   tASSEMBLY
                {
		   vol->type=ASSEMBLY;
                }
	|  VOLTYPE   tUNDEFINED
                {
		   vol->type=UNDEFINED;
                }
	|
	;


VolumeElements
	: VolumeElement VolumeElements
	|
	;

VolumeElement
	:  MATERIAL    STRING
                {
                   if (strlen($2) > sizeof(DB::str_t)-1) YYERROR;
                   if(!loadingEmP->FindMaterial($2))  model_msg(FATAL, "Undefined material %s\n", $2);
		   strcpy(vol->material,$2);
                }
	|  TEMPORTSNUM	{Ibuff  =NULL;}       SFInt32
                {
                  vol->TEMportsNum =$3;
                }
	|  TEPORTSNUM	{Ibuff  =NULL;}       SFInt32
                {
                  vol->TEportsNum =$3;
                }
	|  TMPORTSNUM	{Ibuff  =NULL;}       SFInt32
                {
                  vol->TMportsNum =$3;
                }
	|  GRIDNUM	{Ibuff  =NULL;}       SFInt32
                {
                  vol->gridNum =$3;
                }
	|  PML	{Ibuff  =NULL;}       SFInt32
                {
                  vol->PML =$3;
                }
	|  INVARIANT  {Ibuff  =NULL;}       SFInt32
                {
                  vol->invariant =$3;
                }
	|  MESHREFINEMENT    {Fbuff = NULL;}    SFFloat      {vol->meshRefinement=$3; }
	|  MESHREFINEMENT    {Fbuff = NULL;}    SFFloat      {vol->meshRefinement=$3; }

	|  COMPSOLID         {Ibuff = NULL;}    SFInt32      {vol->compSolid=$3; }

	|  DISABLED          { }                             {vol->disabled=1; }
	;
VolumeBegin
	: DEF Name MWM_VOLUME 
		{
			vol = new DB::Volume();
			strcpy(vol->name,name);
			loadingEmP->insertVolume(vol);
		}
	;

Volume
	: VolumeBegin NodeBegin VolumeType VolumeElements NodeEnd
	;




                                                             
//***   CircuitName
CircuitName
	:  SET CIRCUITNAME STRING
            {
             if (strlen($3)>sizeof(DB::str_t)-1) YYERROR; 
             else strcpy(mainCircName,$3);
            }
	;

%%


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


