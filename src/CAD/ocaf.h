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
#include <TopoDS_Shell.hxx>
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
#include <TCollection_ExtendedString.hxx>
#include <TColStd_HSequenceOfExtendedString.hxx>
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
 int wgPorts;
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


int getFileLine(FILE *fin, std::string &str);

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
  double cutoffref;
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
   cutoffref=1;
   BrCond.clear();
  }
  int Shared(){return cmp1!=std::string("-") && cmp2!=std::string("-");}
  void read(FILE *fin)
  {
/*
	rewind(fp);
        __gnu_cxx::stdio_filebuf<char> fb (fp, std::ios_base::in);
        istream in (&fb);    // create a stream from file buffer
*/
	std::string str;
	fscanf(fin,"%d ",&level); 
	getFileLine(fin, name);
	getFileLine(fin, sfname);
	getFileLine(fin, cmp1);
	getFileLine(fin, cmp2);
	fscanf(fin,"%lf ",&epsr);
	fscanf(fin,"%lf ",&mur);
	fscanf(fin,"%lf ",&meshref);
	fscanf(fin,"%lf ",&cutoffref);
	std::string line;
	while (getFileLine(fin, line))  if(BrCond.find(line)==BrCond.end()) if(!line.empty()) BrCond.insert(line);
  }
  void write(FILE *fout)
  {
/*
	rewind(fp);
        __gnu_cxx::stdio_filebuf<char> fb (fp, std::ios_base::out);
        ostream out (&fb);    // create a stream from file buffer
*/
	fprintf(fout,"%d\n",level);
	fprintf(fout,"%s\n",name.c_str());
	fprintf(fout,"%s\n",sfname.c_str());
	fprintf(fout,"%s\n",cmp1.c_str());
	fprintf(fout,"%s\n",cmp2.c_str());
	fprintf(fout,"%f\n",epsr);
	fprintf(fout,"%f\n",mur);
	fprintf(fout,"%f\n",meshref);
	fprintf(fout,"%f\n",cutoffref);
	typedef std::set<std::string, std::less<std::string> >::const_iterator BdrIt;
	for (BdrIt it=BrCond.begin(); it!= BrCond.end(); it++) fprintf(fout,"%s\n",it->c_str());
  }
};

struct EdgeData
{
  int level;
  std::string name;
  std::string scname;
  std::string LPname;
  double P1[3];
  double epsr;
  double mur;
  double meshref;
  double cutoffref;
  int hasConstU;
  int singular;
  std::set<std::string, std::less<std::string> > BrCond;
  public:
  EdgeData(){
   level=0;
   name="-";
   scname="-";
   LPname="-";
   epsr=1;
   mur=1;
   meshref=1;
   hasConstU=0;
   singular=0;
   BrCond.clear();
  }
  void read(FILE *fin)
  {
	std::string str;
	fscanf(fin,"%d ",&level); 
	getFileLine(fin, name);
	getFileLine(fin, scname);
	getFileLine(fin, LPname);
	fscanf(fin,"%lf %lf %lf ",&P1[0],&P1[1],&P1[2]); 
	fscanf(fin,"%lf ",&epsr); 
	fscanf(fin,"%lf ",&mur);  
	fscanf(fin,"%lf ",&meshref); 
	fscanf(fin,"%lf ",&cutoffref);
        fscanf(fin,"%d ",&hasConstU); 
        fscanf(fin,"%d ",&singular); 
	std::string line;
	while (getFileLine(fin, line))  if(BrCond.find(line)==BrCond.end()) if(!line.empty()) BrCond.insert(line);
  }
  void write(FILE *fout)
  {
	fprintf(fout,"%d\n",level);
	fprintf(fout,"%s\n",name.c_str());
	fprintf(fout,"%s\n",scname.c_str());
	fprintf(fout,"%s\n",LPname.c_str());
	fprintf(fout,"%f %f %f\n",P1[0],P1[1],P1[2]);
	fprintf(fout,"%f\n",epsr);
	fprintf(fout,"%f\n",mur);
	fprintf(fout,"%f\n",meshref);
	fprintf(fout,"%f\n",cutoffref);
	fprintf(fout,"%d\n",hasConstU);
	fprintf(fout,"%d\n",singular);
	typedef std::set<std::string, std::less<std::string> >::const_iterator BdrIt;
	for (BdrIt it=BrCond.begin(); it!= BrCond.end(); it++) fprintf(fout,"%s\n",it->c_str());
  }
};

struct VertexData
{
  int level;
  std::string name;
  double epsr;
  double mur;
  double meshref;
  int shared;
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
   shared=0;
   BrCond.clear();
  }
  void read(FILE *fin)
  {
	std::string str;
	fscanf(fin,"%d ",&level); 
	getFileLine(fin, name);
	fscanf(fin,"%lf ",&epsr); 
	fscanf(fin,"%lf ",&mur);  
	fscanf(fin,"%lf ",&meshref); 
        fscanf(fin,"%d ",&shared); 
        fscanf(fin,"%d ",&singular); 
	std::string line;
	while (getFileLine(fin, line))  if(BrCond.find(line)==BrCond.end()) if(!line.empty()) BrCond.insert(line);
  }
  void write(FILE *fout)
  {
	fprintf(fout,"%d\n",level);
	fprintf(fout,"%s\n",name.c_str());
	fprintf(fout,"%f\n",epsr);
	fprintf(fout,"%f\n",mur);
	fprintf(fout,"%f\n",meshref);
	fprintf(fout,"%d\n",shared);
	fprintf(fout,"%d\n",singular);
	typedef std::set<std::string, std::less<std::string> >::const_iterator BdrIt;
	for (BdrIt it=BrCond.begin(); it!= BrCond.end(); it++) fprintf(fout,"%s\n",it->c_str());
  }
  bool onWG(){return BrCond.find(std::string("WAVEGUIDE"))!=BrCond.end();}
};

TCollection_AsciiString remove_SUB(TCollection_AsciiString assName);

class MwOCAF
{
 public:
  MwOCAF(Handle(AIS_InteractiveContext) aContext=NULL);
  ~MwOCAF();
  void setDocTools();
  void setProjectDir(const char *name);
  void removeLabel(const TDF_Label& lab);
//! Returns sub-label of DocLabel() with tag 6. <br>
  bool  hasParts();
  TDF_Label makeDataLabel();
  TDF_Label dataLabel();
  void setData();
  void getData();
  TDF_Label BConditionsLabel() ;
  int isParts(TDF_Label label);

  bool hasSplitter();
  bool hasSolidBC();
  bool hasPML();

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
  bool getLabelLayers(TDF_Label label, Handle(TColStd_HSequenceOfExtendedString) &layers);
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
  int volType(TDF_Label label);
  int isComponent(TDF_Label label);
  int isDisabled(TDF_Label label);
  int isSolid(TDF_Label label);
  int isShell(TDF_Label label);
  int isSheet(TDF_Label label);
  int isFace(TDF_Label label);
  int isEdge(TDF_Label label);
  int isVertex(TDF_Label label);
  void setLabelShape(TDF_Label item, TopoDS_Shape shape);
//  bool getLabelMaterial(TDF_Label label, TCollection_ExtendedString &layer);
//Appearance
  DB::Volume * getLabelVol(TDF_Label label);
  DB::Volume * getLabelUpperVol(TDF_Label label, DB::EmProblem *upperEmP);
  void assignMatColor(TDF_Label label);
  void changeColor(TDF_Label label, Quantity_NameOfColor colName);
  void updatePartColors();
  void removeWithSubComponents(TDF_Label label);
  void setLabelIndex(TDF_Label item, int I);
  int  getLabelIndex(TDF_Label item);
  Handle(TPrsStd_AISPresentation) setLabelPresentation(TDF_Label label, double transp=0);
  Handle(TPrsStd_AISPresentation) setLabelPresentationProp(TDF_Label label);
  void displayLabelShape(TDF_Label label, bool B);
  void setVisibility(const TDF_Label& label,const bool B = true);
  void displayLabelMesh(TDF_Label label, bool B);
  void highlightLabel(TDF_Label label, bool B);
  void displayAllParts();
  void openLabel(TDF_Label label);
  void closeLabel(TDF_Label label);
  void setSelectedLabels();
  void checkWires();
  void heal();
  void imprint(int downImprint);
  void evalBBox();
  void evalSize(double size[3]);
  void makeSymmPlane(int id, int itype);
  TopoDS_Shape makeSymmBox(int XYplaneSymmetry, int YZplaneSymmetry, int ZXplaneSymmetry);
  void makeSplitFaces();
//  bool updateSplitPlanes();
  void makeSplitFaces(TDF_Label label);
  bool makeGridFaces(TDF_Label label);
  gp_Vec getEdgeDir(TDF_Label label);
  void split();
  void extractSubcomp(int subcompI);
  void generateSubshapes(const TDF_Label &label, bool skipSPLITTER );
  void generateSubshapes(const TDF_Label &label, Handle(XCAFDoc_ShapeTool) shtool, bool skipSPLITTER);
  void generateSubshapes(bool skipSPLITTER=true);
//  void storeSolidInDB(const TDF_Label &label);
//  void storeSolidsInDB();
  TDF_LabelSequence*  getEquivalents (const TDF_Label& label);
  void regenerateIndexedSubShapes();

  void makeShapeIndex2Labels();
  void makeFaceAdjCells();
  void makeFaceAdjCells(TDF_Label solidL);
  void readFEproperties();
  void setFEproperties();
  void setDefaultFEPBC();

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
  void saveImportedStruct(const char *dir, int level, TCollection_AsciiString assName,
	       	          TopLoc_Location assLoc, TDF_Label root, bool addImport=false);
  void saveImportedStruct(const char *dir);
  bool savePartsIF();
  void addToComponentLists(std::set<std::string, std::less<std::string> > *componentlist=NULL,
		           std::map<std::string, std::vector<std::string>, std::less<std::string> > *wgcomponentlist=NULL
		          );
 public:
  void initFEPdataStruct();
  void loadGlobalMat(const char* wkprojpath, bool onlyIfModified=false);
  void workopen(const char* wkprojpath, int subcomp=0);
  int read_prjstatus();
  void save_prjstatus();
  void worksave();
  void setDisabledVolumes();
  void addPorts(std::map<std::string, int, std::less<std::string> > &ports, std::map<std::string, double, std::less<std::string> > &portloads);
  void getAssName(TCollection_AsciiString &assName);
  void getAssName(std::string &assName);
  void setAssName(TCollection_AsciiString assName);
  void setFaceComp();
  void setFaceSubdom();
  void setSuperFaces();
  void setSuperCurves();
  void setSuperCurveFaceData(
          std::map<std::string, std::set<std::string> > &SCSFlinks,
	  std::map<std::string, bool > &SFhasPMC,
  	  std::map<std::string, bool > &SChasPMC
       );
  static void setSuperCurvesConstU(const char* projectDir,
	  std::map<std::string, std::set<std::string> > &SCSFlinks,
	  std::map<std::string, bool > &SFhasPMC,
  	  std::map<std::string, bool > &SChasPMC,
          std::map<std::string, bool>  &superCurveHasConstU
       );
  void writeSuperCurvesConstU(std::map<std::string, bool>  &superCurveHasConstU);
  void checkSuperFacesTEMnum();
  int evalTEMnum(TopoDS_Shape S, bool intPort=true);
  void setTEMnum();
  void setConductorMap();
  void savePartitionMap();
  void readPartitionMap();
  void saveSubdomainMap();
  void readSubdomainMap();
  void putVolumesInDB();
  void checkPartNF(Handle(TDocStd_Document) doc);
  void save();
  void saveAs(const char* fileName);
  void saveIF();
//  void savePartRenum (int I,  const char* fileName);
  Handle( MeshVS_Mesh) getSubMeshVS(int I){
    return Handle( MeshVS_Mesh)::DownCast (shapeIndex2SubMesh->Value(I));
  }
  bool  importSTEP_OR_DXC( char* fileName, bool update );

  DB::EmProblem *EmP;
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
 
  int extFaceNum;
  int extEdgeNum;
  int extVertexNum;
  int subCompNum;
  int subComp;

  std::vector<int>  solidSubdomMap;
  std::vector<int>  faceSubdomMap;
  std::vector<int>  splitFaceSubdomMap;
  std::vector<int>  subSplitFacesMap;
  std::vector<int>  subSplitEdgesMap;
  std::vector<int>  subSplitVerticesMap;
  std::vector<int>  edgeConductorMap;
  std::vector<int>  faceConductorMap;

  FaceData    *faceData;
  EdgeData    *edgeData;
  VertexData  *vertexData;
  FILE *faceDataLock;

  int  PECedge(int EI) {
      if (edgeData[EI-1].BrCond.find(std::string("PEC"))!=edgeData[EI-1].BrCond.end()) return 1;
      if (WGedge(EI)) if(ECedge(EI)) return 1;
      return 0;
  }
  int  PMCedge(int EI) {return edgeData[EI-1].BrCond.find(std::string("PMC"))==edgeData[EI-1].BrCond.end() ? 0:1;}
  int  LPedge(int EI)  {return edgeData[EI-1].LPname==std::string("-") ? 0:1;}
  int  WGedge(int EI)  {return edgeData[EI-1].BrCond.find(std::string("WAVEGUIDE"))==edgeData[EI-1].BrCond.end() ? 0:1;}
  int  ECedge(int EI) {
      if (edgeData[EI-1].BrCond.find(std::string("PEC"))!=edgeData[EI-1].BrCond.end()) return 1;
      typedef std::set<std::string, std::less<std::string> > ::iterator BdrIt;
      for (std::set<std::string, std::less<std::string> > ::iterator bit=edgeData[EI-1].BrCond.begin(); bit!= edgeData[EI-1].BrCond.end(); bit++){
	 DB::Material* mat= EmP->FindMaterial(bit->c_str());
	 if(mat) if((mat->Sresistance>0 && mat->Sresistance<1) || mat->Sinductance>0) return 1;
      }
      return 0;
  }
  
  int  PMCvertex(int PI) {return vertexData[PI-1].BrCond.find(std::string("PMC"))==vertexData[PI-1].BrCond.end() ? 0:1;}
  int  PECvertex(int PI) {
      if (vertexData[PI-1].BrCond.find(std::string("PEC"))!=vertexData[PI-1].BrCond.end()) return 1;
      return 0;
  }
  int  ECvertex(int PI) {
      if (vertexData[PI-1].BrCond.find(std::string("PEC"))!=vertexData[PI-1].BrCond.end()) return 1;
      typedef std::set<std::string, std::less<std::string> > ::iterator BdrIt;
      for (std::set<std::string, std::less<std::string> > ::iterator bit=vertexData[PI-1].BrCond.begin(); bit!= vertexData[PI-1].BrCond.end(); bit++){
	 DB::Material* mat= EmP->FindMaterial(bit->c_str());
	 if(mat) if((mat->Sresistance>0 && mat->Sresistance<1) || mat->Sinductance>0) return 1;
      }
      return 0;
  }

  int  PECface(int FI) {
      if (faceData[FI-1].BrCond.find(std::string("PEC"))!=faceData[FI-1].BrCond.end()) return 1;
      return 0;
  }
  int  ECface(int FI) {
      if (faceData[FI-1].BrCond.find(std::string("PEC"))!=faceData[FI-1].BrCond.end()) return 1;
      typedef std::set<std::string, std::less<std::string> > ::iterator BdrIt;
      for (std::set<std::string, std::less<std::string> > ::iterator bit=faceData[FI-1].BrCond.begin(); bit!= faceData[FI-1].BrCond.end(); bit++){
	 DB::Material* mat= EmP->FindMaterial(bit->c_str());
	 if(mat) if((mat->Sresistance>0 && mat->Sresistance<1) || mat->Sinductance>0) return 1;
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

  int  numberOfParts(){if(!theParts.IsNull()) return theParts.NbChildren(); else return 0;}
  TDF_LabelSequence *theSelectedLabels;

  int partNameCompMap(const char* cstr);
  
 private:
  void makeSolid(const TopoDS_CompSolid& S, TopoDS_Solid &Solid);
  void makeUPIF(TopoDS_Shape &IF);
  void makeIF(TopoDS_Shape &IF);
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
  bool hasMultibodyPart(Handle(XCAFDoc_ShapeTool) shapeTool, TDF_Label &ass);
  bool hasSubAssembly(Handle(XCAFDoc_ShapeTool) shapeTool, TDF_Label &ass);
  bool hasEmSubComponent(Handle(XCAFDoc_ShapeTool) shapeTool, TDF_Label &ass);
  TDF_Label getRefLabel(const TDF_Label &label);
  TDF_Label replaceLabelShape(TDF_Label label, TopoDS_Shape newS);
  TopoDS_Solid   theMergedSolid;
  TopoDS_Shape   theIF;
  
  TopTools_IndexedMapOfShape UPIFsubshapes;


  Handle(AIS_InteractiveContext)  myContext;
  void resetShapeTool();
  void makeShapeIndex2Labels(const TDF_Label &label);
  void setIntData(int *Iarray, int len);
  int  intData(int i);
  int Imax;
  TDF_LabelSequence **shapeIndex2Labels;
  Handle(TColStd_HArray1OfTransient)   shapeIndex2SubMesh;
};

TCollection_AsciiString remove_SUB(TCollection_AsciiString assName);

#endif

