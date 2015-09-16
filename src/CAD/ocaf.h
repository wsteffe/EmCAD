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


#ifndef _MWOCAF_H_
#define _MWOCAF_H_

#if defined(HAVE_OCE_CONFIG_H)
 #include <oce-config.h>
#elif defined(HAVE_OCC_CONFIG_H)
 #include <config.h>
#endif

#include <AIS_InteractiveContext.hxx>
#include <TDocStd_Document.hxx>

#include <TDF_Label.hxx>
#include <TDF_LabelSequence.hxx>
#include <TCollection_AsciiString.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Solid.hxx>
#include <Bnd_Box.hxx>

#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_LayerTool.hxx>
#include <XCAFDoc_MaterialTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_DimTolTool.hxx>
#include <ShapeBuild_ReShape.hxx>

#include <TopTools_IndexedMapOfShape.hxx>
#include <TopTools_DataMapOfShapeShape.hxx>
#include <TopTools_DataMapOfIntegerShape.hxx>
#include <TopTools_HArray1OfShape.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TColStd_HSequenceOfExtendedString.hxx>
#include <StlMesh_Mesh.hxx>
#include <MeshVS_Mesh.hxx>
#include <TColStd_HArray1OfTransient.hxx>
#include <TopTools_HSequenceOfShape.hxx>

#include <TDocStd_SequenceOfDocument.hxx>
#include <TopTools_LocationSet.hxx>
#include <TColStd_HArray1OfInteger.hxx>
#include <Quantity_NameOfColor.hxx>
#include <TPrsStd_AISPresentation.hxx>

#include <map>
#include <set>
#include <vector>

#include "DataBase.h"
#include "OStools.h"

/*
#include <stdio.h>
#include <iostream>
#include <ext/stdio_filebuf.h>
*/

class ImprintFailure{};
class MeshingFailure{};
class InconsistentModel{};


struct stepFileNotFound
{
 stepFileNotFound(char *name){filename =name;}
 std::string filename;
};


class ProjectStatus
{
// true means that it is defined.
 public:
 int geometry;
 int partTypes;
 int partMaterials;
 int linePorts;
 ProjectStatus();
 void save(const char*filename);
 int read(const char*filename);
 int defined();
};


typedef char lstr_t[256];
struct cmp_str{
 bool operator()(const lstr_t a, const lstr_t b)
 {
    return strcmp(a,b) <0;
 }
};


struct FaceData
{
  int level;
  std::string name;
  std::string sfname;
  std::string cmp1;
  std::string cmp2;
  double epsr;
  double mur;
  double meshref;
  int shared;
  std::set<std::string, std::less<std::string> > BrCond;
  public:
  FaceData(){
   level=0;
   name="-";
   sfname="-";
   cmp1="-";
   cmp2="-";
   epsr=1;
   mur=1;
   meshref=1;
   shared=0;
   BrCond.clear();
  }
  int Shared(){return cmp1!=std::string("-") && cmp2!=std::string("-");}
  void read(std::ifstream &in)
  {
/*
	rewind(fp);
        __gnu_cxx::stdio_filebuf<char> fb (fp, std::ios_base::in);
        istream in (&fb);    // create a stream from file buffer
*/
	std::string str;
	in>>level; getLine(in, str);
	getLine(in, name);
	getLine(in, sfname);
	getLine(in, cmp1);
	getLine(in, cmp2);
	in>>epsr; getLine(in, str);
	in>>mur;  getLine(in, str);
	in>>meshref;  getLine(in, str);
        in>>shared; getLine(in, str);
	std::string line;
	while (!getLine(in, line).eof())  if(BrCond.find(line)==BrCond.end()) if(!line.empty()) BrCond.insert(line);
  }
  void write(std::ofstream &out)
  {
/*
	rewind(fp);
        __gnu_cxx::stdio_filebuf<char> fb (fp, std::ios_base::out);
        ostream out (&fb);    // create a stream from file buffer
*/
	out << level; out << endl;
	out << name; out << endl;
	out << sfname; out << endl;
	out << cmp1; out << endl;
	out << cmp2; out << endl;
	out << epsr; out << endl;
	out << mur; out << endl;
	out << meshref; out << endl;
	out << shared; out << endl;
	typedef std::set<std::string, std::less<std::string> >::const_iterator BdrIt;
	for (BdrIt it=BrCond.begin(); it!= BrCond.end(); it++) {
		std::string tmp=*it;
		out << tmp.c_str(); out <<endl;}
  }
};

struct EdgeData
{
  int level;
  std::string name;
  std::string LPname;
  double P1[3];
  double epsr;
  double mur;
  double meshref;
  int singular;
  std::set<std::string, std::less<std::string> > BrCond;
  public:
  EdgeData(){
   level=0;
   name="-";
   LPname="-";
   epsr=1;
   mur=1;
   meshref=1;
   singular=0;
   BrCond.clear();
  }
  void read(std::ifstream &in)
  {
        std::string str;
	in>>level; getLine(in, str);
	getLine(in, name);
	getLine(in, LPname);
	in>>P1[0]; in>>P1[1]; in>>P1[2]; getLine(in, str);
	in>>epsr; getLine(in, str);
	in>>mur;  getLine(in, str);
	in>>meshref;  getLine(in, str);
        in>>singular; getLine(in, str);
	std::string line;
	while (!getLine(in, line).eof())  if(BrCond.find(line)==BrCond.end())  if(!line.empty()) BrCond.insert(line);
  }
  void write(std::ofstream &out)
  {
	out << level; out << endl;
	out << name; out << endl;
	out << LPname; out << endl;
	out << P1[0]; out << "  ";
       	out << P1[1]; out << "  ";
       	out << P1[2]; out << endl;
	out << epsr; out << endl;
	out << mur; out << endl;
	out << meshref; out << endl;
	out << singular;out << endl;
	typedef std::set<std::string, std::less<std::string> >::const_iterator BdrIt;
	for (BdrIt it=BrCond.begin(); it!= BrCond.end(); it++) {out << *it; out <<endl;}
  }
};

struct VertexData
{
  int level;
  std::string name;
  double epsr;
  double mur;
  double meshref;
  int singular;
  std::set<std::string, std::less<std::string> > BrCond;
  public:
  VertexData(){
   level=0;
   name="-";
   epsr=1;
   mur=1;
   meshref=1;
   singular=0;
   BrCond.clear();
  }
  void read(std::ifstream &in)
  {
        std::string str;
	in>>level; getLine(in, str);
	getLine(in, name);
	in>>epsr; getLine(in, str);
	in>>mur;  getLine(in, str);
	in>>meshref;  getLine(in, str);
        in>>singular; getLine(in, str);
	std::string line;
	while (!getLine(in, line).eof())  if(BrCond.find(line)==BrCond.end()) if(!line.empty()) BrCond.insert(line);
  }
  void write(std::ofstream &out)
  {
	out << level; out << endl;
	out << name; out << endl;
	out << epsr; out << endl;
	out << mur; out << endl;
	out << meshref; out << endl;
	out << singular;out << endl;
	typedef std::set<std::string, std::less<std::string> >::const_iterator BdrIt;
	for (BdrIt it=BrCond.begin(); it!= BrCond.end(); it++) {out << *it; out <<endl;}
  }
};


class MwOCAF
{
 public:
  MwOCAF(Handle_AIS_InteractiveContext aContext=NULL);
  ~MwOCAF();
  void setDocTools();
  void setProjectDir(const char *name);
  void removeLabel(const TDF_Label& lab);
//! Returns sub-label of DocLabel() with tag 6. <br>
  bool  hasParts();
  TDF_Label makePartsLabel();
  TDF_Label partsLabel();
  TDF_Label makeDataLabel();
  TDF_Label dataLabel();
  void setData();
  void getData();
  TDF_Label BConditionsLabel() ;
  int isParts(TDF_Label label);
  TopLoc_Location location();

//Names:
  int  setLabelName(TDF_Label label, TCollection_AsciiString name);
  bool getLabelName(TDF_Label label, TCollection_AsciiString &name);
  bool getPartName (int I, TCollection_AsciiString &name);
  void setMissingNames(TDF_Label root);
  void setMissingNames();
  void setGlobalNames(TDF_Label root);
  void setGlobalNames();
//Materials:
  int isMaterial(TDF_Label label);
  int isMaterials(TDF_Label label);
  void getMaterials(TDF_Label &label);
  TDF_Label addMaterial(TCollection_AsciiString name);
  void removeAllMaterials();
  void setLabelMaterial(TDF_Label& label, TDF_Label& MatL);
//  void setLabelMaterial(TDF_Label label, TCollection_ExtendedString &material);

//Boundary Conditions:
  int isBCondition(TDF_Label label);
  int isBConditions(TDF_Label label);
  void getBConditions(TDF_Label &label);
  TDF_Label addBCondition(TCollection_AsciiString name);
  void removeAllBConditions();
  void setLabelBCondition(TDF_Label& label, TDF_Label& MatL);

//Layers:
  int  isLayer(TDF_Label label);
  int  isLayers(TDF_Label label);
  void getLayers(TDF_Label &label);
  bool getLabelLayer(TDF_Label label, TCollection_ExtendedString &layer);
  bool getLabelLayers(TDF_Label label, Handle_TColStd_HSequenceOfExtendedString &layers);
  void setLabelLayer(TDF_Label label, TCollection_ExtendedString &layer);
//DGTs:
  int  isDGTs(TDF_Label label);
//Structure:
  bool hasIF;
  bool hasDownIF;
  int isAssembly(TDF_Label label);
  int isShape(TDF_Label label);
  int isSimpleShape(TDF_Label label);
  int isCompound(TDF_Label label);
  int isReference(TDF_Label label);
  int isTopLevel(TDF_Label label);
  int isFree(TDF_Label label);
  void getFreeShapes(TDF_LabelSequence frshapes);
  int isPart(TDF_Label label);
  int isComponent(TDF_Label label);
  int isDisabled(TDF_Label label);
  int isSolid(TDF_Label label);
  int isSheet(TDF_Label label);
  int isFace(TDF_Label label);
  int isEdge(TDF_Label label);
  void setLabelShape(TDF_Label item, TopoDS_Shape shape);
//  bool getLabelMaterial(TDF_Label label, TCollection_ExtendedString &layer);
//Appearance
  DB::Volume * getLabelVol(TDF_Label label);
  DB::Volume * getLabelUpperVol(TDF_Label label, DB::EmProblem *upperEmP);
  void assignMatColor(TDF_Label label);
  void changeColor(TDF_Label label, Quantity_NameOfColor colName);
  void updateAssembly(TDF_Label item);
  void updatePartColors();
  void removeWithSubComponents(TDF_Label label);
  void setLabelIndex(TDF_Label item, int I);
  int  getLabelIndex(TDF_Label item);
  Handle(TPrsStd_AISPresentation) setLabelPresentation(TDF_Label label, Quantity_NameOfColor colName=Quantity_NOC_SALMON4, double transp=0);
  void displayLabelShape(TDF_Label label, bool B);
  void setVisibility(const TDF_Label& label,const bool B = true);
  void displayLabelMesh(TDF_Label label, bool B);
  void highlightLabel(TDF_Label label, bool B);
  void displayAllParts();
  void openLabel(TDF_Label label);
  void closeLabel(TDF_Label label);
  void setSelectedLabels();
  void checkWires();
  void heal(Handle(ShapeBuild_ReShape) &reshape);
  void imprint();
  void evalBBox();
  void evalSize(double size[3]);
  void makeSplitFaces();
  bool updateSplitPlanes();
  void updatePartitionVolumes(DB::EmProblem *upperEmP);
  void makeSplitFaces(TDF_Label label);
  bool makeGridFaces(TDF_Label label);
  void split();
  void generateSubshapes(const TDF_Label &label);
  void generateSubshapes(const TDF_Label &label, Handle(XCAFDoc_ShapeTool) shtool);
  void generateSubshapes();
//  void storeSolidInDB(const TDF_Label &label);
//  void storeSolidsInDB();
  void regenerate();
  TDF_LabelSequence*  getEquivalents (const TDF_Label& label);
  void regenerateIndexedSubShapes();

  void makeShapeIndex2Labels();
  void makeFaceAdjCells();
  void makeFaceAdjCells(TDF_Label solidL);
  void makeFaceAdjBdrCond(TDF_Label solidL);
  void makeFaceAdjBdrCond();
  void setFEproperties();
  void saveFEproperties();

  void evalSuperFaceMeshSize(double mshsize0);
  bool makeTheCompSolid();
//  bool makeTheCompound();
  bool newDoc();
  bool openDoc(const char* fileName);
  bool closeDoc();
  bool openUPIF  (const char* fileName);
  bool openParts();
  void readPartsStatus();
  void setPartsStatus();
  void setDefaultPartTypes(bool onlyUndefined=false);
  void saveImportedStruct(const char *dir, int level, TCollection_AsciiString assName,
	       	          TopLoc_Location assLoc, TDF_Label root,
		          std::set<std::string, std::less<std::string> > *componentlist,
		          std::set<std::string, std::less<std::string> > *wgcomponentlist,
			  bool addImport=false);
  void saveImportedStruct(const char *dir,
		          bool compIsAss,
		          std::set<std::string, std::less<std::string> > *componentlist=NULL,
		          std::set<std::string, std::less<std::string> > *wgcomponentlist=NULL
		          );
  bool savePartsIF();
 public:
  void setFaceDataLock();
  void freeFaceDataLock();
  void saveFEPlinks();
  void loadGlobalMat(const char* wkprojpath, bool onlyIfModified=false);
  void workopen(const char* wkprojpath);
  void worksave(const char* wkprojpath);
  void addComponent(std::set<std::string, std::less<std::string> > &list);
  void setDisabledVolumes();
  void addPorts(std::map<std::string, int, std::less<std::string> > &ports, std::map<std::string, double, std::less<std::string> > &portloads);
  void getAssName(TCollection_AsciiString &assName);
  void getAssName(std::string &assName);
  void setFaceComp();
  void setDefaultFEPBC();
  void setSuperFaces();
  void checkSuperFacesTEMnum();
  int evalTEMnum(TopoDS_Shape S, bool intPort=true);
  void setTEMnum();
  void saveSplitFaceMap();
  void putVolumesInDB();
  void checkPartNF(Handle(TDocStd_Document) doc);
  void save ( const char* fileName);
  void saveIF();
//  void savePartRenum (int I,  const char* fileName);
  Handle( MeshVS_Mesh) getSubMeshVS(int I){
    return Handle( MeshVS_Mesh)::DownCast (shapeIndex2SubMesh->Value(I));
  }
  bool  importSTEP_OR_DXC( char* fileName, bool update );

  DB::EmProblem EmP;
  std::string projectDir;
  Handle(TDocStd_Document) mainDoc;
  TDF_Label            theShapes;
  TDF_Label            theLayers;
  TDF_Label            theMaterials;
  TDF_Label            theDGTs;
  TDF_Label            theParts;
  TDF_Label            theData;
  TDF_Label            theBConditions;
  TopoDS_CompSolid     theCompSolid;
  TopTools_DataMapOfIntegerShape thePartShapes;
//  TopoDS_Compound      theCompound;
  TopTools_IndexedMapOfShape  *indexedShapes;
  TopTools_IndexedMapOfShape  *indexedSolids;
  TopTools_IndexedMapOfShape  *indexedFaces;
  TopTools_IndexedMapOfShape  *indexedEdges;
  TopTools_IndexedMapOfShape  *indexedVertices;
  TCollection_AsciiString     *faceAdjParts; //first part is where face normal enters
  TCollection_AsciiString     *faceAdjBdrCond; //first part is where face normal enters
 
  FaceData    *faceData;
  EdgeData    *edgeData;
  VertexData  *vertexData;
  FILE *faceDataLock;

  int  PECedge(int EI) {
      if (edgeData[EI-1].BrCond.find(std::string("PEC"))!=edgeData[EI-1].BrCond.end()) return 1;
      if (WGedge(EI)) if(!PMCedge(EI)) if (edgeData[EI-1].BrCond.size()>1) return 1;
      return 0;
  }
  int  PMCedge(int EI) {return edgeData[EI-1].BrCond.find(std::string("PMC"))==edgeData[EI-1].BrCond.end() ? 0:1;}
  int  LPedge(int EI)  {return edgeData[EI-1].LPname==std::string("-") ? 0:1;}
  int  WGedge(int EI)  {return edgeData[EI-1].BrCond.find(std::string("WAVEGUIDE"))==edgeData[EI-1].BrCond.end() ? 0:1;}
  int  ECedge(int EI) {
      if(PECedge(EI)) return 1;
      typedef std::set<std::string, std::less<std::string> > ::iterator BdrIt;
      for (std::set<std::string, std::less<std::string> > ::iterator bit=edgeData[EI-1].BrCond.begin(); bit!= edgeData[EI-1].BrCond.end(); bit++){
	 DB::Material* mat= EmP.FindMaterial(bit->c_str());
	 if(mat) if(mat->Sresistance>0 && mat->Sresistance<1) return 1;
      }
      return 0;
  }

  Bnd_Box BBox;

  ProjectStatus prjStatus;

  Handle(XCAFDoc_ShapeTool)    shapeTool;
  Handle(XCAFDoc_ColorTool)    colorTool;
  Handle(XCAFDoc_LayerTool)    layerTool;
  Handle(XCAFDoc_MaterialTool) materialTool;
  Handle(XCAFDoc_DimTolTool)   dimtolTool;

  bool hasUPIF;
  bool worksaveNeeded;
  bool partitionVolSaveNeeded;

  int  numberOfParts(){if(!theParts.IsNull()) return theParts.NbChildren(); else return 0;}
  TDF_LabelSequence *theSelectedLabels;

 private:
  void makeSolid(const TopoDS_CompSolid& S, TopoDS_Solid &Solid);
  void makeIF(TopoDS_Compound &IF);
  void newIndexedSubShapes();
  void makeIF_IndexedSubShapes();
  void makePartsIndexedSubShapes();
  void makeExternalIndexedSubShapes(TopTools_IndexedMapOfShape  &externalSubshapes);
  void makeIndexedShapes(const TDF_Label &label);
  void makeIndexedShapes();
  void renumIndexedSubShapes(TDF_Label label,
		           std::map<int,int> &f_permu,
		           std::map<int,int> &e_permu,
		           std::map<int,int> &v_permu
			   );
  bool hasMultibodyPart(Handle(XCAFDoc_ShapeTool) shapeTool, TDF_Label ass);
  bool hasSubAssembly(Handle(XCAFDoc_ShapeTool) shapeTool, TDF_Label ass);
  TDF_Label replaceLabelShape(TDF_Label label, TopoDS_Shape newS);
  TopoDS_Solid      theMergedSolid;
  TopoDS_Compound   theIF;
  Handle(TColStd_HArray1OfInteger)  splitFacesMap;
  Handle(TColStd_HArray1OfInteger)  splitEdgesMap;
  Handle(TColStd_HArray1OfInteger)  splitVerticesMap;

  TopTools_IndexedMapOfShape UPIFsubshapes;

  int extFaceNum;
  int extEdgeNum;
  int extVertexNum;

  Handle(StlMesh_Mesh) theStlMesh;

  Handle_AIS_InteractiveContext  myContext;
  void resetShapeTool();
  void makeShapeIndex2Labels(const TDF_Label &label);
  void setIntData(int *Iarray, int len);
  int  intData(int i);
  int Imax;
  TDF_LabelSequence **shapeIndex2Labels;
  Handle(TColStd_HArray1OfTransient)   shapeIndex2SubMesh;
};



#endif

