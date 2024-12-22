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


#if defined(HAVE_OCE_CONFIG_H)
 #include <oce-config.h>
#elif defined(HAVE_OCC_CONFIG_H)
 #include <config.h>
#endif

#include <iostream>
#include "ocaf.h"

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

extern int XYplaneSymmetry;
extern int YZplaneSymmetry;
extern int ZXplaneSymmetry;

int main(int argc, char **argv)
{
      modeldebug=0;
      model_flex_debug=0;
      for(int i=1; i<argc-2; i++) for(int l=0; l<strlen(argv[i]); l++) argv[i][l]=tolower(argv[i][l]); 
      int ii =1;
      while (ii < argc-3) {
	     if (!strcmp(argv[ii], "-xyplanesymmetry") ) { ii++; XYplaneSymmetry =atoi(argv[ii++]);}
	else if (!strcmp(argv[ii], "-yzplanesymmetry") ) { ii++; YZplaneSymmetry =atoi(argv[ii++]);}
	else if (!strcmp(argv[ii], "-zxplanesymmetry") ) { ii++; ZXplaneSymmetry =atoi(argv[ii++]);}
	else {
         fprintf(stderr, "\n  Sintax Error with the Agrument: \"%s\" \n\n", argv[ii]);
         return 1;
       }
      }
// ** input data:
      if (argc-ii < 2){
          fprintf(stderr, "\n");
	  fprintf(stderr, \
          "  Usage:   emcad_imprint [-XYplaneSymmetry 1/2 -YZplaneSymmetry 1/2 -ZXplaneSymmetry 1/2 ] downImprint subprojDir\n"\
          " \n");
	  exit(1);
//	  return 1;
      }
      int downImprint= (int) atoi(argv[ii++]);
      std::string subprojDir=argv[ii++];

#if defined(_WIN32) || defined(__WIN32__)
      subprojDir=cleanWindowsPath(subprojDir);
#endif    


//*****

      if(downImprint){
        MwOCAF* ocaf=new MwOCAF();
        ocaf->workopen(subprojDir.c_str());
        if (ocaf->EmP->assemblyType==COMPONENT||ocaf->hasDownIF) ocaf->imprint(downImprint);
        if(ocaf->EmP->assemblyType==NET) ocaf->savePartsIF();
        ocaf->initFEPdataStruct();
	if(ocaf->EmP->assemblyType==COMPONENT){
	     ocaf->makeFaceAdjCells();
	     ocaf->setFaceComp();
             ocaf->setDisabledVolumes();
	}
        ocaf->worksave();
        ocaf->closeDoc();
        delete ocaf;
      } else {
        MwOCAF* ocaf=new MwOCAF();
        ocaf->workopen(subprojDir.c_str());
        if (ocaf->EmP->assemblyType==COMPONENT||ocaf->hasDownIF) ocaf->imprint(downImprint);
        ocaf->saveIF();
        if(ocaf->EmP->level==0) {
          ocaf->initFEPdataStruct();
          if(ocaf->EmP->assemblyType==NET) ocaf->savePartsIF();
	  if(ocaf->EmP->assemblyType==COMPONENT){
//                 ocaf->makeFaceAdjCells();
		 ocaf->setFaceComp();
	  }
        } 
        if(ocaf->EmP->assemblyType==COMPONENT) ocaf->updatePartColors();
        ocaf->worksave();
        ocaf->closeDoc();
        delete ocaf;
      }
      return 0;
      
}
