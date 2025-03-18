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


#include <QtCore/QString>
#include <math.h>
#include <map>
#include <set>
#include <vector>
#include <complex>


enum FreqRespParType {SPAR=0, ZPAR=1, YPAR=2};
#ifndef _SELECTEDCIRCUIT_H_
#define _SELECTEDCIRCUIT_H_
enum SelectedCircuit {ELECROMAGNETICDEVICE=0, MAPPEDCIRCUIT=1, IDEALCIRCUIT=2, IMPORTEDCIRCUIT=3, IDEALCIRCUITMAPPEDTZ=4, IMPORTEDRESPONSE=5};
#endif
enum FilterMapSource {ZEROPOLES=0, IMPORTED_RESPONSE=1, IMPORTED_CIRCUIT=2};
enum FilterTopology  {SYMMETRIC_TRANSVERSE_LC=0, SYMMETRIC_TRANSVERSE_JLC=1, SYMMETRIC_ONLY_LC=2, SYMMETRIC_WITH_MAGICT=3 };
enum FilterType      {CHEBYSHEV=0, MAXIMALLY_FLAT=1, STOP_BAND=2, STOP_PASS_BAND=3};


struct StringList
{
  std::set<std::string, std::less<std::string> > list;
  void read(const char*filename);
  void save(const char*filename);
  void clear(){list.clear();}
};

struct StringStringVecMap
{
  std::map<std::string, std::vector<std::string>, std::less<std::string> > map;
  void read(const char*filename);
  void save(const char*filename);
  void clear(){map.clear();}
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
 int cmpReductionNeeded;
 int netReductionNeeded;
 WorkStatus();
 void reset();
 void checkNetReduction();
 void checkCmpReduction();
};


typedef std::set< std::pair<int, int> >::iterator ZeroPoleCurvesIterator;
typedef std::map< std::pair<int, int>, double >::iterator SparseMatIterator;

struct ProjectData
{
    ProjectData();
// Project Global variables:
    QString  projectName;
    QString  mainAssName;
    QString  varFilePath;
    QString  s2pFilePath;
    int    network;
    char   lengthUnitName[5];
    int    freqUnitE;    //10 power of Hz;
    int    meshPerWavelen;
    int    sharedMeshPerWavelen;
    double sharedMeshRefine;
    int    meshPerCircle;
    int    meshRefineMinNum;
    int    meshRefineMaxNum;
    int    meshTetMaxNum;
    int    conformalMeshIF;
    double meshMinEnergyRatio;
    int    localMeshing3d;
    int    XYplaneSymmetry;
    int    YZplaneSymmetry;
    int    ZXplaneSymmetry;
    double freqBand[2];
    double resonFreqMaxRatio;
    double cmpResonFreqMaxRatio;
    double netResonFreqMaxRatio;
    double anaFreqBand[2];
    double zpFreqBand[2];
    double zpWinRatio;
    double mapFreqBand[2];
    double cutoffRatio;
    std::set< std::pair<int,int> > zeropoleCurves;
    std::map< std::pair<int,int>, double > idealFilterJ;
    std::map< std::pair<int,int>, double > idealFilterCK;
    std::map< std::pair<int,int>, double > idealFilterLK;
    std::map< std::pair<int,int>, double > idealFilterG;
    std::map< std::pair<int,int>, int > idealFilterJvar;
    std::map< std::pair<int,int>, int > idealFilterCKvar;
    std::map< std::pair<int,int>, int > idealFilterLKvar;
    std::map< std::pair<int,int>, int > idealFilterGvar;
    std::vector<double> idealFilterResonFreq;
    std::vector<double> idealFilterImpedance;
    int    MORFreqNum;
    int    MORFreqNumR;
    int    MORFreqNumI;
    int    KrylovOrder;
    int    anaFreqNum;
    int    cmpMORFreqNum;
    int    cmpMORFreqNumR;
    int    cmpMORFreqNumI;
    int    netMORFreqNum;
    int    netMORFreqNumR;
    int    netMORFreqNumI;
    int    autoFreqResponse;
    int    autoMappedFreqResponse;
    int    autoDesignWithMappedTz;
    int    autoIdealMappedTzFreqResponse;
    int    autoZeropole;
    int    autoMappedZeropole;
    int    autoFilterMapping;

    double filterPassBand[2];
    double filterStopBand[2];
    double filterRetLoss;
    double filterOutbandRetLoss;
    double filterQfactor;
    double filterTargetQfactor;
    int    filterInductiveSkin;
    double filterPortImpedance;
    int    filterOrder;
    int    idealFilterWithMappedTZ;
    int    idealFilterAddConjugateTZ;
    int    symmFilterResponse;
    int    customIdealFilter;
    int    predistortedFilter;
    int    predistFilterOptim;
    double predistFilterOptimRL;
    double predistFilterOptimILpp;
    int    predistFilterOptimIterMax;
    double predistFilterOptimTrustR;
    int    idealFilterType;
    int    canonicalFilterTopology;
    int    filtermapSymmetric;
    int    filtermapSource;
    double filtermapQfactor;
    int    filterTuneMethod;
    int    filterTuneItermax;
    int    filterTuneRecomputeError;
    int    filterTuneRecomputeJaco;
    int    filterSymmetricTuning;
    int    filterTuneOnlyJt;
    int    spiceSetCentralConfig;
    double filterTuneXtol;
    double filterTuneTrustR;
    std::vector<std::complex<double> > filterZeros;

    StringList subcomponents;
    StringStringVecMap wgcomponents;
    std::map<std::string, int> componentSubNum;

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
    void updateComponentSubNum();
    int readFilterTxZeros(char* fname);
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

