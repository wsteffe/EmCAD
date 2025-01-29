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


#ifndef _DATABASE_H_
#define _DATABASE_H_

#include<MallocUtils.h>
#include<math.h>
#include<string>
#include<vector>
#include<map>
#include<List.h>
#include<Tree.h>
#include<Message.h>
#include<string.h>

#include<buffer.h>


#define UNDEFINED          0
#define DIELECTRIC         1
#define WAVEGUIDE          2
#define BOUNDARYCOND       3
#define SPLITTER           4
#define HOLE               5
#define LINEPORT           6
#define INTERFACE          7
#define ASSEMBLY          10
#define COMPONENT         11
#define NET               12
#define PARTITION         13
#define GRID              14

#define RIGHT          1
#define LEFT           2



namespace DB {

class Units {
public:
  double  xm;
  double  xHz;
  double  xOhm;
  Units();
};

class PhysicConstants {
public:
  double  c0;
  double  eps0;
  double  mu0;
  double  Z0;
//----
void set(Units u);
};


typedef char str_t[80];

struct  StringList_t {
 List_T  *list;
 StringList_t();
 ~StringList_t();
 void add(char *string);
 void get(int i, str_t str);
 int num();
 void reset();
};

enum  RoughSurfModelType { GradientModel=0, HurayTwoParam=1, HurayCannonBall=2, LossAndQfactors=3, HurayHexBase=4};

class MatBuffer{
public:
  Buffer<double, 100, 3>  epsLorentz;
  Buffer<double, 100, 3>  muLorentz;
  Buffer<double, 100, 4>  surfPolesRes;
};
class Material {
public:
  str_t    name;
  int      I;
  double   epsr;
  double   mur;
  double   etandelta;
  double   htandelta;
  double   econductivity;
  double   hconductivity;
  Vec<double>   epsLorentz;
  Vec<double>   muLorentz;
  Vec<double>   surfPolesRes;
  double    Sresistance;
  double    Sinductance;
  double    roughSurfFreq;
  int       roughSurfModelType;
  double    roughSurfLossFactor;
  double    roughSurfImpedanceQ;
  double    roughSurfBallRadius;
  double    roughSurfSurfRatio;
  double    roughSurfRz;
  double    roughSurfRq;
  int       roughSurfFitPolesNum;
  int     color[4];
  static MatBuffer buff;
  Material();
  int  edispersive(){return (fabs(econductivity) >1.e-30 || epsLorentz.n>0);}
  int  hdispersive(){return (fabs(hconductivity) >1.e-30 || muLorentz.n>0);}
  int  sdispersive(){return (fabs(Sresistance) >1.e-30 || surfPolesRes.n>0);}
  void flush();
  Material& operator =(const Material &rhs);
};



//-------------------------------------
class Volume {
public:
  str_t      name;
  int        type;
  str_t      material;
  int        master;
  int        TEMportsNum;
  int        TEportsNum;
  int        TMportsNum;
  int        orientation;
  int        TEM_TabularOrder1;
  int        TEM_TabularOrder2;
  int        TEM_refCond;
  int        disconnectedTEM;
  int        subcircuitIndex;
  int        subcircuitIFindex;
  int        defined;
  int        disabled;
  int        gridNum;
  int        PML;
  int        invariant;
  double     meshRefinement;
  double     cutoffRefinement;
  int        cellNum;
  std::map<int,int> Fmap, Cmap;
  Volume();
  Volume & operator =(Volume const &rhs);
};

class Transform {
public:
  str_t      name;
  double     trasl[3];
  double     rotAxis[3];
  double     rotOrigin[3];
  double     rotAngle;
  Transform();
  Transform& operator =(const Transform &rhs);
};

int fcmp_Material(const void *a, const void *b);


class EmProblem{
public:
  Tree_T  *materials, *volumes, *invariants;
//------------------------
  EmProblem();
  ~EmProblem();
  void addMaterial  (Material*  m);
  void replaceMaterial  (Material*  m);
  void insertMaterial(Material* m);
  void delMaterial  (Material*  m);
  void insertVolume (Volume*    v);
  void delVolume    (Volume*    v);
  void addInvariant (Transform* t);
  void insertInvariant (Transform* t);
  void delInvariant    (Transform* t);
  void save(FILE *fid, bool onlyMesh=false, bool onlyWG=false, std::string WGprefix=std::string(""));
  void saveMaterials(FILE *fid);
  std::string stepFilePath;
  std::string occFilePath;
  bool hasGeo;
  int  level;
  str_t defaultBC;
  int assemblyType;
  Material  *FindMaterial(const str_t name);
  Volume    *FindVolume(const str_t name);
  Transform *FindInvariant(const str_t name);
  int volNum(){ return Tree_Nbr(volumes);}
};


}; //namespace
 
#endif
