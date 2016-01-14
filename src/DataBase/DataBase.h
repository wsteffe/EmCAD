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


#ifndef _DATABASE_H_
#define _DATABASE_H_

#include<MallocUtils.h>
#include<string>
#include<vector>
#include<map>
#include<ListUtils.h>
#include<TreeUtils.h>
#include<Message.h>
#include<string.h>

#include<buffer.h>

#ifndef PIG
#define PIG 3.14159265
#endif


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


class MatBuffer{
public:
  Buffer<double, 100, 3>  epsLorentz;
  Buffer<double, 100, 3>  muLorentz;
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
  int      edispersive;
  int      hdispersive;
  Vec<double>   epsLorentz;
  Vec<double>   muLorentz;
  double     Sresistance;
  double freqBand[2];
  int     color[4];
  static MatBuffer buff;
  Material();
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
  int        compSolid;
  int        defined;
  int        disabled;
  int        gridNum;
  int        PML;
  int        invariant;
  double     meshRefinement;
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
  bool  hasGeo;
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
