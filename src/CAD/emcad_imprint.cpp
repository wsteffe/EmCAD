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

int main(int argc, char **argv)
{
      modeldebug=0;
      model_flex_debug=0;

// ** input data:
      if (argc < 2){
          fprintf(stderr, "\n");
	  fprintf(stderr, \
          "  Usage:   emcad_imprint downImprint subprojDir\n"\
          " \n");
	  exit(1);
//	  return 1;
      }

      int downImprint= (int) atoi(argv[1]);
      std::string subprojDir=argv[2];

#if defined(_WIN32) || defined(__WIN32__)
      subprojDir=cleanWindowsPath(subprojDir);
#endif    


//*****

      if(downImprint){
        MwOCAF* ocaf=new MwOCAF();
        ocaf->workopen(subprojDir.c_str());
        if (ocaf->EmP.assemblyType==COMPONENT||ocaf->hasDownIF) ocaf->imprint();
        if(ocaf->EmP.assemblyType==NET) ocaf->savePartsIF();
        ocaf->initFEPdataStruct();
	if(ocaf->EmP.assemblyType==COMPONENT){
//	     ocaf->makeFaceAdjCells();
	     ocaf->setFaceComp();
             ocaf->setDisabledVolumes();
	}
        ocaf->worksave();
        ocaf->closeDoc();
        delete ocaf;
      } else {
        MwOCAF* ocaf=new MwOCAF();
        ocaf->workopen(subprojDir.c_str());
        if (ocaf->EmP.assemblyType==COMPONENT||ocaf->hasDownIF) ocaf->imprint();
        ocaf->saveIF();
        if(!ocaf->EmP.level) {
          if(ocaf->EmP.assemblyType==NET) ocaf->savePartsIF();
          ocaf->initFEPdataStruct();
	  if(ocaf->EmP.assemblyType==COMPONENT){
//                 ocaf->makeFaceAdjCells();
		 ocaf->setFaceComp();
	  }
        } 
        if(ocaf->EmP.assemblyType==COMPONENT) ocaf->updatePartColors();
        ocaf->worksave();
        ocaf->closeDoc();
        delete ocaf;
      }
      return 0;
      
}
