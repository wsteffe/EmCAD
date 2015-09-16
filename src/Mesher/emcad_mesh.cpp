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
#include "mesher.h"

#include <sys/stat.h> 
#include <string>


extern int modeldebug;
extern int model_flex_debug;


int main(int argc, char **argv)
{
    modeldebug=0;
    model_flex_debug=0;

// ** input data:
    if (argc < 5){
          fprintf(stderr, "\n");
	  fprintf(stderr, \
          "  Usage:   emcad_mesh  asstype meshsize connModeFraction subprojDir mwmDir\n"\
          " \n");
	  return 0;
    }

    int assType= (int) atoi(argv[1]);
    double meshsize=(double) atof(argv[2]);
    std::string subprojDir=argv[3];
    std::string mwmDir=argv[4];


//*****

    if(assType==COMPONENT){
      MwOCAF* ocaf=new MwOCAF();
      ocaf->workopen(subprojDir.c_str());
      MESHER::addIF(ocaf, subprojDir.c_str(),mwmDir.c_str());
      delete ocaf;
      ocaf=new MwOCAF();
      std::string partitionDir=subprojDir; partitionDir+="/Partition";
      ocaf->workopen(partitionDir.c_str());
      MESHER::meshModel(ocaf, meshsize, subprojDir.c_str(),mwmDir.c_str());
      delete ocaf;
    }else{
       MwOCAF* ocaf=new MwOCAF();
       ocaf->workopen(subprojDir.c_str());
       MESHER::meshModel(ocaf, 1.1*meshsize, subprojDir.c_str());
       delete ocaf;
    }


    return 0;
}
