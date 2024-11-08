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


#include <iostream>
#include "ocaf.h"
#include "mesher.h"
#include "mwm.h"

#if defined(TETMESHER)
#include "tetmesher.h"
#endif

#include <sys/stat.h> 
#include <string>


extern int modeldebug;
extern int model_flex_debug;

namespace DB {
 char yyFileName[256];
 int yyLineNum;
 void yyMsg(int type, const char *fmt, ...){
  va_list args;
  char tmp[1024];
  va_start (args, fmt);
  vsprintf (tmp, fmt, args);
  va_end (args);
  Msg(type, "'%s', line %d : %s", yyFileName, yyLineNum, tmp);
 }
}

int main(int argc, char **argv)
{
    modeldebug=0;
    model_flex_debug=0;

// ** input data:
    if (argc < 2){
		fprintf(stderr, 
                "\n"
                "   Usage:      emcad_mesh  {Options}\n\n");
		fprintf(stderr,
                "   Options= \n"\
                "       -asstype         <integer>    \n"\
                "       -meshsize        <float>      \n"\
                "       -meshpercircle   <integer>    \n"\
                "       -subcomp         <integer>    \n"\
                "       -dir             <string>     \n"\
                "       -outdir          <string>     \n"\
                "       -onserver        <0/1>        \n"\
                "       -mesh3DonClient  <0/1>        \n\n");
		exit(0);
    }

    if (argc < 5){
          fprintf(stderr, "\n");
	  fprintf(stderr, \
          "  Usage:   emcad_mesh  asstype meshsize meshpercircle subprojDir outDir\n"\
          " \n");
	  return 0;
    }

    int assType= COMPONENT;
    double meshsize=0;
    double sharedMeshsize=1.0;
    int meshpercircle=0;
    int subCompI=0;
    std::string subprojDir;
    std::string outDir;
    int onServer=0;
    int mesh3DonClient=0;

    int i=1;
    while (i < argc-1) {
      for(int l=0; l<strlen(argv[i]); l++) argv[i][l]=tolower(argv[i][l]); 
       if (   !strcmp(argv[i], "-asstype")) { 
         i++; 
         assType = (int) atoi(argv[i++]);
       }
       else if ( !strcmp(argv[i], "-meshsize")  ) { 
         i++; 
         meshsize=(double) atof(argv[i++]);
       }
       else if ( !strcmp(argv[i], "-meshpercircle")  ) { 
         i++; 
         meshpercircle=(int) atoi(argv[i++]);
       }
       else if ( !strcmp(argv[i], "-sharedmeshsize")  ) { 
         i++; 
         sharedMeshsize=(double) atof(argv[i++]);
       }
       else if ( !strcmp(argv[i], "-subcomp")  ) { 
         i++; 
         subCompI=(int) atoi(argv[i++]);
       }
       else if ( !strcmp(argv[i], "-dir") ) { 
         i++; 
         subprojDir=argv[i++];
       }
       else if ( !strcmp(argv[i], "-outdir") ) { 
         i++; 
         outDir=argv[i++];
       }
       else if ( !strcmp(argv[i], "-onserver") ) { 
         i++; 
         onServer=(int) atoi(argv[i++]);
       }
       else if ( !strcmp(argv[i], "-mesh3donclient") ) { 
         i++; 
         mesh3DonClient=(int) atoi(argv[i++]);
       }
       else {
        fprintf(stderr, "\n  Sintax Error with the Agrument: \"%s\" \n\n", argv[i]);
        exit(1);
       }
    }

    if(outDir.length()==0 && subprojDir.length()>0 ) outDir=subprojDir;

#if defined(_WIN32) || defined(__WIN32__)
    subprojDir=cleanWindowsPath(subprojDir);
    outDir=cleanWindowsPath(outDir);
#endif    

//*****

 if(assType==COMPONENT){
      MwOCAF* ocaf=new MwOCAF();
      std::string modelPath=subprojDir+"/model.xml";
      int subCompNum=0;
      if(FileExists(modelPath.c_str())){
        ocaf->workopen((nativePath(subprojDir)).c_str());
        bool meshWG=!ocaf->subCompNum;
        if(onServer){
         bool mesh3D=!ocaf->subCompNum;
         bool meshIF=false;
//         if(mesh3D) MESHER::addIF(ocaf, subprojDir.c_str(),outDir.c_str());
         if(mesh3D) MESHER::meshModel(ocaf, meshIF, mesh3D, meshWG, meshsize, sharedMeshsize, meshpercircle, subprojDir.c_str(),outDir.c_str());
        } else {
         bool mesh3D=!ocaf->subCompNum && mesh3DonClient;
         bool meshIF=ocaf->subCompNum;
         if( mesh3D || meshWG || meshIF)   MESHER::meshModel(ocaf, meshIF, mesh3D, meshWG, meshsize, sharedMeshsize, meshpercircle, subprojDir.c_str(),outDir.c_str());
        }
        subCompNum=ocaf->subCompNum;
        ocaf->closeDoc();
        delete ocaf;
      }
      int subcmpMin,subcmpMax;
      if(onServer and subCompI>0){subcmpMin=subcmpMax=subCompI;}
      else if(!onServer) {subcmpMin=1; subcmpMax=subCompNum;}
      else {subcmpMin=1; subcmpMax=0;}
      bool mesh3D=onServer|| (!onServer && mesh3DonClient);
      bool meshIF=false;
      bool meshWG=true;
      if(mesh3D || meshIF) for (int subcmpI=subcmpMin; subcmpI<=subcmpMax; subcmpI++) {
         ocaf=new MwOCAF();
         ocaf->workopen(subprojDir.c_str(),subcmpI);
	 MESHER::meshModel(ocaf, meshIF, mesh3D, meshWG, meshsize, sharedMeshsize, meshpercircle, subprojDir.c_str(),outDir.c_str());
         ocaf->closeDoc();
         delete ocaf;
      }
 } else {
      MwOCAF* ocaf=new MwOCAF();
      ocaf->workopen(subprojDir.c_str());
      bool mesh3D=false;
      bool meshWG=false;
      bool meshIF=true;
      MESHER::meshModel(ocaf, meshIF, mesh3D, meshWG, meshsize, sharedMeshsize, meshpercircle, subprojDir.c_str(),outDir.c_str());
      ocaf->closeDoc();
      delete ocaf;
 }


    return 0;
}
