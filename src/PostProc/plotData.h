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


#include <QStringList>
#include <vector>
#include <map>


typedef std::map< std::pair<int, int>, std::pair< std::vector<double>, std::vector<double> >  >::iterator PlotDataCurveMapIterator;

void convert2db(char format[5], double val[2], double oval[2]);

struct PlotData{
 char param;
 char frequnit[5];
 char format[5];
 double R;
 int numberOfPorts;
 int numberOfFreq;
 int numberOfCurves;
 std::vector<double> frequencies;
 std::vector<double> curveArray;
 std::map< std::pair<int, int>, std::pair< std::vector<double>, std::vector<double> >  > curveMap;
 QStringList portnames;
 void clear(){
  frequencies.clear();
  curveArray.clear();
  portnames.clear();
  curveMap.clear();
 }
 void getCurveData(int ip, int jp, int k, double x[]){
   int Np=numberOfPorts;
   bool convert_to_dB= (param=='S') && strcmp(format,"DB");
   for (int l=0; l< numberOfFreq; l++){
      int ijl=(2*ip+2*Np*jp)+2*Np*Np*l;
      if(!convert_to_dB) 
        x[l]=curveArray[k+ijl];
      else{
        double val[2]; 
	convert2db(format, &curveArray[ijl], val);
        x[l]=val[k];
      }	    
   }
 }
 int setFrequnit(char funit[5])
 {
   if (!strcmp(funit, "Hz") || !strcmp(funit, "KHz") || !strcmp(funit, "MHz") || !strcmp(funit, "GHz") )
     {strcpy(frequnit,funit); return 0;}
   else 
     return 1;
 }
};

