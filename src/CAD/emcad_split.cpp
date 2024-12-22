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
#include <OStools.h>

#include <sys/stat.h> 
#include <string>

#define USEXMLXCAF 1


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
      if (argc < 1){
          fprintf(stderr, "\n");
	  fprintf(stderr, \
          "  Usage:   emcad_split subprojDir\n"\
          " \n");
	  return 0;
      }

      std::string subprojDir=argv[1];
      
#if defined(_WIN32) || defined(__WIN32__)
      subprojDir=cleanWindowsPath(subprojDir);
#endif    


//*****

      MwOCAF* ocaf=new MwOCAF();
      ocaf->workopen(subprojDir.c_str());
//      ocaf->checkPartNF(ocaf->mainDoc);
      ocaf->regenerateIndexedSubShapes();
      ocaf->split();
      ocaf->regenerateIndexedSubShapes();
      ocaf->initFEPdataStruct();
      ocaf->readFEproperties();
      if(ocaf->subCompNum>1)
      ocaf->setSuperFaces();
      int subCompNum=ocaf->subCompNum;
      delete ocaf;
      for (int subcmpI=1; subcmpI<=subCompNum; subcmpI++) {
       ocaf=new MwOCAF();
       ocaf->workopen(subprojDir.c_str());
       ocaf->regenerateIndexedSubShapes();
       ocaf->extractSubcomp(subcmpI);
       ocaf->saveSubdomainMap();
       std::string subSplitModel=subprojDir;
       subSplitModel+="/model";
       char tag[10]; sprintf(tag,"_%d",subcmpI);
       subSplitModel+=tag;
       if(USEXMLXCAF)  subSplitModel+=".xml";
       else            subSplitModel+=".dxc";
       ocaf->saveAs(subSplitModel.c_str());
       delete ocaf;
      }
      return 0;
      
}
