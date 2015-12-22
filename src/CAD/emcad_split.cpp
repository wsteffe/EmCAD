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


#include <iostream>
#include "ocaf.h"
#include <OStools.h>

#include <sys/stat.h> 
#include <string>



extern int modeldebug;
extern int model_flex_debug;

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

//*****

      MwOCAF* ocaf=new MwOCAF();
      ocaf->workopen(subprojDir.c_str());
//      ocaf->checkPartNF(ocaf->mainDoc);
      ocaf->regenerateIndexedSubShapes();
      ocaf->split();
      ocaf->saveSplitFaceMap();
      std::string splitModel=subprojDir;  splitModel+="/Partition/model.dxc";
      ocaf->save(splitModel.c_str());
      std::string  emFileName=subprojDir; emFileName+="/Partition/model.em";
      FILE *fid= fopen(nativePath(emFileName).c_str(), "w");
      fprintf(fid, "Level  %d\n", ocaf->EmP.level+1);
      fprintf(fid, "AssemblyType  %d\n", PARTITION);
      fprintf(fid, "DefaultBoundCond  \"%s\"\n", ocaf->EmP.defaultBC);
      ocaf->EmP.save(fid);
      fclose(fid);
      ocaf->closeDoc();
      delete ocaf;
      return 0;
      
}
