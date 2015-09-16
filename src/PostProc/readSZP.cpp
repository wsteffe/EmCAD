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
#include <vector>
#include <Message.h>
#include <string.h>
#include "plotData.h"


#ifndef PIG
#define PIG 3.14159265
#endif


extern PlotData plotData;

int readSZP(const char* fName){

  std::vector<double> zeroloads;
  std::vector<int> loadports;
  int Nloads=0;
  int loadtype;

  FILE *fid;
  if(!(fid = fopen(fName, "r"))){
    DB::Msg(ERROR, "Cannot open file %s\n", fName);
    return 1;
  }
  double funit;
  int c0,c,nz;
  c=0;
  int ipld;
  do{
    do {c0=c; c=getc(fid);} while(!(c0=='\n'&&(c=='!'||c=='+')||c==EOF));
    if(c=='+'){
      loadtype=getc(fid);
      fscanf(fid,"%d", &Nloads);
      for(int j=0; j<Nloads; j++) {
         fscanf(fid,"%d", &ipld);
	 loadports.push_back(ipld);
      }
    }
    if(c=='!'){
      std::array<int, 2> ports;
      char funit[5];
      fscanf(fid,"%d %d %d %s",&ports[0],&ports[1],&nz,funit);
      if(plotData.setFrequnit(funit)){
             fprintf(stderr, "invalid frequency unit in %s \n", fName);
             return 1;
      }
      for(int i=0; i<nz; i++) {
       double freq[2];
       fscanf(fid,"%lf %lf",&freq[0],&freq[1]);
       plotData.curveMap[ports][0].push_back(freq[0]);
       plotData.curveMap[ports][1].push_back(freq[1]);
       if(ports[0]>0) for(int j=0; j<Nloads; j++) {
         double Yld[2];
         fscanf(fid,"%lf %lf",&Yld[0], &Yld[1]);
         zeroloads.push_back(Yld[0]);  
	 zeroloads.push_back(Yld[1]);
       }
      }
    }
  } while(c!=EOF);
  fclose(fid);
  plotData.numberOfCurves=plotData.curveMap.size();
  return 0;
}


