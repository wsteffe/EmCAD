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


#include <QtCore/QString>
#include <math.h>
#include <map>
#include <set>
#include <vector>
#include <array>


enum FreqRespParType {SPAR=0, ZPAR=1, YPAR=2};
#ifndef _SELECTEDCIRCUIT_H_
#define _SELECTEDCIRCUIT_H_
enum SelectedCircuit {ELECROMAGNETICDEVICE=0, MAPPEDCIRCUIT=1, IDEALCIRCUIT=2};
#endif
enum FilterMapMethod {SYMMETRICFILTER=0, TUNECIRCUIT=1};


struct StringList
{
  std::set<std::string, std::less<std::string> > list;
  void read(const char*filename);
  void save(const char*filename);
};


template<typename T> struct StringMap
{
  std::map<std::string, T, std::less<std::string> > map;
  void read(const char*filename);
  void save(const char*filename);
  void clear(){map.clear();}
};

class WorkStatus
{
// true means that it is defined.
 public:
 int reloadNeeded;
 int decompositionNeeded;
 int firstDecomposition;
 int materialChanged;
 int remeshNeeded;
 int componentsaveNeeded;
 int modelizationNeeded;
 int reductionNeeded;
 WorkStatus();
 void reset();
 void checkReduction();
};


typedef std::set< std::array<int, 2> >::iterator ZeroPoleCurvesIterator;

struct ProjectData
{
    ProjectData();
// Project Global variables:
    QString  projectName;
    QString  mainAssName;
    int    network;
    char   lengthUnitName[5];
    int    freqUnitE;    //10 power of Hz;
    int    meshPerWavelen;
    double freqBand[2];
    double anaFreqBand[2];
    double zpFreqBand[2];
    double zpImFreqBand[2];
    std::set< std::array<int,2>> zeropoleCurves;
    int    MORFreqNum;
    int    anaFreqNum;
    int    anaMORFreqNum;
    int    autoFreqResponse;
    int    autoMappedFreqResponse;
    int    autoZeropole;
    int    autoMappedZeropole;
    int    autoFilterMapping;

    double filterPassBand[2];
    double filterRetLoss;
    double filterQfactor;
    int    filterOrder;
    int    filtermapMethod;
    int    filtermapItermax;
    std::vector<double> filterZeros;

    StringList components;
    StringList wgcomponents;

    WorkStatus workStatus;

// freq responce
    FreqRespParType freqRespParType;
    SelectedCircuit analysedCircuit;
    SelectedCircuit plottedCircuit;
    int freqRespParPart;
    int freqRespYscaleAuto;
    double freqRespYscale[3]; //start, stop, step
    int zeropoleYscaleAuto;
    double zeropoleYscale[3]; //start, stop, step


    StringMap<int> ports;
    StringMap<double> portloads;

    void saveSettings();
    void readSettings();
    void saveWorkStatus();
    void readWorkStatus();
    void savePorts();
    void readPorts();
    void savePortLoads();
    void readPortLoads();
    double lengthUnit(){
	  if(!strcmp(lengthUnitName,"M"))  return pow(10.0,0);
     else if(!strcmp(lengthUnitName,"MM")) return pow(10.0,-3);
     else if(!strcmp(lengthUnitName,"UM")) return pow(10.0,-6);
     else return -1;
    }
    QString freqUnitName()
    {
      switch(freqUnitE){
         case 0: 
	       return QString("Hz");
         case 3: 
	       return QString("KHz");
         case 6: 
	       return QString("MHz");
         case 9: 
	       return QString("GHz");
       }
      return QString("Unknown_Unit");
    }
    double freqUnit(){return pow(10.0,freqUnitE);}
};

