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


#if defined(HAVE_OCE_CONFIG_H)
 #include <oce-config.h>
#elif defined(HAVE_OCC_CONFIG_H)
 #include <config.h>
#endif

#include <sstream>

#include <iostream>
#include <fstream>
#include <algorithm>

#include "ocaf.h"
#include "InputOutput.h"
#include "OStools.h"
#include "assert.h"

#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

#include <XCAFDoc.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_Location.hxx>
#include <XmlXCAFDrivers_DocumentRetrievalDriver.hxx>
#include <XmlXCAFDrivers_DocumentStorageDriver.hxx>

#include <TDF_ChildIterator.hxx>
#include <TDF_Reference.hxx>
#include <Quantity_Color.hxx>
#include <XCAFDoc_ColorType.hxx>
#include <Quantity_TypeOfColor.hxx>
#include <TDataStd_Integer.hxx>
#include <TDataStd_IntegerArray.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_TreeNode.hxx>
#include <TNaming_NamedShape.hxx>
#include <TNaming_Builder.hxx>
#include <TNaming_Iterator.hxx>
#include <TNaming_Tool.hxx>
#include <TopoDS_Shape.hxx>

#include <TopTools_Array1OfShape.hxx>

#include <TopTools_DataMapOfShapeInteger.hxx>
#include <TopTools_MapIteratorOfMapOfShape.hxx>
#include <TopTools_DataMapOfShapeListOfShape.hxx>
#include <BOPAlgo_Builder.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <BOPAlgo_GlueEnum.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Iterator.hxx>

#include <ShapeFix_Shape.hxx>
#include <ShapeFix_Wireframe.hxx>
#include <ShapeUpgrade_ShapeDivideClosed.hxx>
#include <ShapeUpgrade_ShapeDivideClosedEdges.hxx>
#include <ShapeAnalysis_Wire.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepTools.hxx>
#include <BRepLib.hxx>
//#include <BRepLib_MakeSolid.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopExp.hxx>

#include <BRepAdaptor_Surface.hxx>
#include <BRepTopAdaptor_TopolTool.hxx>
#include <LocalAnalysis_SurfaceContinuity.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <Geom2d_Curve.hxx>
#include <TopOpeBRepTool_TOOL.hxx>
#include <GeomAPI_IntCS.hxx>

#include <BRepCheck_Analyzer.hxx>
#include <ShapeAnalysis_ShapeContents.hxx>

#include <ShapeCustom.hxx>
#include <ShapeCustom_TrsfModification.hxx>
#include <BRepTools_Modifier.hxx>

#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <GeomLProp_SLProps.hxx>

#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>

#include <MeshVS_Mesh.hxx>
#include <MeshVS_MeshPrsBuilder.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>

#include <TCollection_HAsciiString.hxx>

#include <TDocStd_XLinkTool.hxx> 

#include <TPrsStd_AISViewer.hxx>
#include <TPrsStd_DriverTable.hxx>
#include <XCAFPrs_Driver.hxx>
#include <TColStd_SequenceOfInteger.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>

#include <BRepOffsetAPI_MakePipe.hxx>
#include <gp_Circ.hxx>
#include <BRepBndLib.hxx>
#include <BRep_Tool.hxx>
#include <gp_Vec.hxx>
#include <GeomPlate_BuildAveragePlane.hxx>
#include <gp_Pln.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Line.hxx>
#include <TColgp_HArray1OfPnt.hxx>

#include <BRepAdaptor_Curve.hxx>
#include <GCPnts_AbscissaPoint.hxx>

#include <omp.h>

#define USEXMLXCAF 1


#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif
	      
#define CHECK_CREATED_SHAPES 0

int XYplaneSymmetry=0;
int YZplaneSymmetry=0;
int ZXplaneSymmetry=0;


template <typename T> std::string to_string ( T Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }

ProjectStatus::ProjectStatus(){
   geometry=1;
   partTypes=0;
   partMaterials=0;
   linePorts=1;
   wgPorts=1;
}
int ProjectStatus::defined(){return geometry*partTypes*partMaterials*linePorts;}
void ProjectStatus::save(const char*filename){
   FILE *fid= fopen(nativePath(filename).c_str(), "w");
   fprintf(fid, "%d\n", geometry);
   fprintf(fid, "%d\n", partTypes);
   fprintf(fid, "%d\n", partMaterials);
   fprintf(fid, "%d\n", linePorts);
   fprintf(fid, "%d\n", wgPorts);
   fclose(fid);
}
int ProjectStatus::read(const char*filename){
   if(!FileExists(filename)) return 0;
   FILE *fid= fopen(nativePath(filename).c_str(), "r");
   fscanf(fid, "%d\n", &geometry);
   fscanf(fid, "%d\n", &partTypes);
   fscanf(fid, "%d\n", &partMaterials);
   fscanf(fid, "%d\n", &linePorts);
   fscanf(fid, "%d\n", &wgPorts);
   fclose(fid);
   return 1;
}

int getFileLine(FILE *fin, std::string &str)
{
 char cstring [100];
 if ( fgets (cstring , 100 , fin) != NULL ){
    str=std::string(cstring);
    if (!str.empty() && str[str.length()-1] == '\n') str.erase(str.length()-1);
    if (!str.empty() && str[str.length()-1] == '\r') str.erase(str.length()-1);
    return 1;
 } else {
    str=std::string("");
    return 0;
 }
}

TDF_Label MwOCAF::getRefLabel(const TDF_Label &label){
    Handle(TDF_Reference)  refAtt;
    TDF_Label refLabel;
    if(label.FindAttribute(TDF_Reference::GetID(),refAtt)) refLabel=refAtt->Get();
    else refLabel.Nullify();
    return refLabel;
}



MwOCAF::MwOCAF(Handle(AIS_InteractiveContext) aContext){
   worksaveNeeded=false;
   Imax=0;
   indexedShapes=NULL;
   indexedSolids=NULL;
   indexedFaces=NULL;
   indexedEdges=NULL;
   indexedVertices=NULL;
   
   extFaceNum=0;
   extEdgeNum=0;
   extVertexNum=0;
   subCompNum=0;
   subComp=0;

   shapeIndex2Labels =NULL;
   faceAdjParts=NULL;

   theSelectedLabels=NULL;
   edgeData=NULL;
   faceData=NULL;
   vertexData=NULL;
   
   hasDownIF=false;
   hasIF=false;

//   partVertexRenum=NULL;
   hasUPIF=false;
   myContext=aContext;

   EmP=new DB::EmProblem();
}

void  curveTangent(Handle(Geom_Curve) GC, gp_Pnt GP, gp_Vec &T)
{
           GeomAPI_ProjectPointOnCurve Projector (GP, GC);
           Standard_Real u=Projector.LowerDistanceParameter();
	   gp_Pnt P;
	   GC->D1(u, P,T);
}

void  curveTangent(Handle(Geom_Curve) GC, gp_Pnt GP,
		     double &Tx, double &Ty, double &Tz
		){
           GeomAPI_ProjectPointOnCurve Projector (GP, GC);
           Standard_Real u=Projector.LowerDistanceParameter();
	   gp_Pnt P; gp_Vec T;
	   GC->D1(u, P,T);
	   Tx=T.X();
	   Ty=T.Y();
	   Tz=T.Z();
}

void  curvePointAndTangent(Handle(Geom_Curve) GC, gp_Pnt GP,
		     gp_Pnt &P, gp_Vec &T)
{
           GeomAPI_ProjectPointOnCurve Projector (GP, GC);
           Standard_Real u=Projector.LowerDistanceParameter();
	   GC->D1(u, P,T);
}

void  curvePointAndTangent(Handle(Geom_Curve) GC, gp_Pnt GP,
		     double &Px, double &Py, double &Pz,
		     double &Tx, double &Ty, double &Tz
		){
           GeomAPI_ProjectPointOnCurve Projector (GP, GC);
           Standard_Real u=Projector.LowerDistanceParameter();
	   gp_Pnt P; gp_Vec T;
	   GC->D1(u, P,T);
	   Px=P.X();
	   Py=P.Y();
	   Pz=P.Z();
	   Tx=T.X();
	   Ty=T.Y();
	   Tz=T.Z();
}


gp_Vec  supNormal(Handle(Geom_Surface) GS, gp_Pnt GP){
           GeomAPI_ProjectPointOnSurf Projector (GP, GS);
           Standard_Real u,v;
           Projector.LowerDistanceParameters(u, v);
           GeomLProp_SLProps props(GS,u,v,1,0.001);
           return props.Normal();
}

void  supNormal(Handle(Geom_Surface) GS, 
		        double Px,  double Py,  double Pz,
		        double &Nx, double &Ny, double &Nz
			){
          gp_Pnt GP=gp_Pnt(Px,Py,Pz);
          gp_Vec N=supNormal(GS, GP);
	  Nx=N.X();
	  Ny=N.Y();
	  Nz=N.Z();
}


gp_Vec faceNormal(TopoDS_Shape F){
	    if(F.ShapeType()!=TopAbs_FACE) return gp_Vec(0,0,0);
	    Handle(Geom_Surface) GS = BRep_Tool::Surface(TopoDS::Face(F));
            TopExp_Explorer exp(F,TopAbs_EDGE);
	    TopoDS_Edge E=TopoDS::Edge(exp.Current());
            TopoDS_Vertex V1,V2;
            TopExp::Vertices(E,V1,V2, false);
            gp_Pnt P1 = BRep_Tool::Pnt(V1);
	    gp_Vec n=supNormal(GS, P1);
	    if(F.Orientation()!=TopAbs_FORWARD) n*=-1;;
            return(n);
	}

void  supPointAndNormal(Handle(Geom_Surface) GS, 
		        double Px,  double Py,  double Pz,
		        double &SPx, double &SPy, double &SPz,
		        double &Nx, double &Ny, double &Nz
			){
          gp_Pnt GP=gp_Pnt(Px,Py,Pz);
          GeomAPI_ProjectPointOnSurf Projector (GP, GS);
          gp_Pnt SP = Projector.NearestPoint();
	  SPx=SP.X();
	  SPy=SP.Y();
	  SPz=SP.Z();
          Standard_Real u,v;
          Projector.LowerDistanceParameters(u, v);
          GeomLProp_SLProps props(GS,u,v,1,0.001);
          gp_Dir N=props.Normal();
	  Nx=N.X();
	  Ny=N.Y();
	  Nz=N.Z();
}

void  putPointOnSurf(Handle(Geom_Surface) GS, 
		        double Px,  double Py,  double Pz,
		        double &SPx, double &SPy, double &SPz
			){
          gp_Pnt GP=gp_Pnt(Px,Py,Pz);
          GeomAPI_ProjectPointOnSurf Projector (GP, GS);
          gp_Pnt SP = Projector.NearestPoint();
	  SPx=SP.X();
	  SPy=SP.Y();
	  SPz=SP.Z();
}



MwOCAF::~MwOCAF()
{
  if(shapeIndex2Labels){
    for (int i=0; i< Imax; i++) if(shapeIndex2Labels[i]) delete shapeIndex2Labels[i];
    free(shapeIndex2Labels); shapeIndex2Labels=NULL;
  }
  theMergedSolid.Nullify();
  theIF.Nullify();
  if(indexedShapes)        { delete indexedShapes;  indexedShapes=NULL;}
  if(indexedSolids)        { delete indexedSolids;  indexedSolids=NULL;}
  if(indexedFaces)         { delete indexedFaces;   indexedFaces=NULL;}
  if(indexedEdges)         { delete indexedEdges;   indexedEdges=NULL;}
  if(indexedVertices)      { delete indexedVertices;   indexedVertices=NULL;}
  if(faceAdjParts)         { delete [] faceAdjParts;   faceAdjParts=NULL;}
  if(faceData)             { delete [] faceData;   faceData=NULL;}
  if(edgeData)             { delete [] edgeData;   edgeData=NULL;}
  if(vertexData)           { delete [] vertexData; vertexData=NULL;}
  if(EmP)     delete EmP;
}


int strEndsWith(std::string &str, std::string &suffix)
{
  int strLen = str.length();
  int suffixLen = suffix.length();
  if(strLen<suffixLen) return 0;
  return str.substr(strLen-suffixLen,suffixLen)==suffix;
}


bool isNetName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<4) return false;
  TCollection_AsciiString start=name.SubString(1,4);
  start.LowerCase();
  return (start=="net_");
}

bool isCompName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<4) return false;
  TCollection_AsciiString start=name.SubString(1,4);
  start.LowerCase();
  return (start=="cmp_");
}

bool isIFName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<3) return false;
  TCollection_AsciiString start=name.SubString(1,3);
  start.LowerCase();
  return (start=="if_");
}


bool isBCondName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<3) return false;
  TCollection_AsciiString start=name.SubString(1,3);
  start.LowerCase();
  return (start=="bc_");
}

bool isDielName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<5) return false;
  TCollection_AsciiString start=name.SubString(1,5);
  start.LowerCase();
  return (start=="diel_");
}

bool isHoleName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<5) return false;
  TCollection_AsciiString start=name.SubString(1,5);
  start.LowerCase();
  return (start=="hole_");
}


bool isSplitName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<6) return false;
  TCollection_AsciiString start=name.SubString(1,6);
  start.LowerCase();
  return (start=="split_");
}

bool isGridName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<5) return false;
  TCollection_AsciiString start=name.SubString(1,5);
  start.LowerCase();
  return (start=="grid_");
}

bool isWgPortName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<4) return false;
  TCollection_AsciiString start=name.SubString(1,4);
  start.LowerCase();
  return (start=="wgp_");
}

bool isLinePortName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<3) return false;
  TCollection_AsciiString start=name.SubString(1,3);
  start.LowerCase();
  return (start=="lp_");
}


bool isValidMultibodyPartName(TCollection_AsciiString &name)
{
 if(isDielName(name))       return true; 
 if(isHoleName(name))       return true; 
 if(isBCondName(name))      return true; 
 if(isGridName(name))       return true; 
 if(isSplitName(name))      return true; 
 if(isWgPortName(name))     return true; 
 if(isLinePortName(name))   return true; 
 return false;
}


bool isValidPartName(TCollection_AsciiString &name)
{
 if(isValidMultibodyPartName(name))  return true; 
 if(isNetName(name))  return true;
 if(isCompName(name)) return true;
 if(isIFName(name))   return true;
 return false;
}

struct LabelAttributes
{
  bool hasName;
  bool hasLayers;
  bool hasColor;
  bool hasColorL;
  TCollection_AsciiString name;
  TColStd_HSequenceOfExtendedString layers;
  Quantity_Color color;
  TDF_Label colorL;
  LabelAttributes(){
    hasName=hasLayers=hasColor=hasColorL=false;
  }
};

void copyLabelAttributes( LabelAttributes &attr, const TDF_Label &label,
			 Handle(XCAFDoc_ColorTool) colTool, 
			 Handle(XCAFDoc_LayerTool) layTool)
{
 Handle(TDataStd_Name)  nameAtt;
 attr.hasName =label.FindAttribute(TDataStd_Name::GetID(),nameAtt);
 if(attr.hasName) attr.name=nameAtt->Get();
 Handle(TColStd_HSequenceOfExtendedString) layers;
 attr.hasLayers =layTool->GetLayers(label,layers);
 if(attr.hasLayers) attr.layers=*layers;
 attr.hasColor  =colTool->GetColor(label,XCAFDoc_ColorGen,attr.color);
 attr.hasColorL =colTool->GetColor(label,XCAFDoc_ColorGen,attr.colorL);
}

void pasteLabelAttributes( LabelAttributes &attr, TDF_Label &label,
			 Handle(XCAFDoc_ColorTool) colTool, 
			 Handle(XCAFDoc_LayerTool) layTool)
{
       if(attr.hasName){
           Handle(TDataStd_Name)  nameAtt=TDataStd_Name::Set(label,attr.name);
       }
       if(attr.hasLayers) 
	       for (int i=1; i<=attr.layers.Length(); i++) layTool->SetLayer(label,attr.layers.Value(i));
       if(attr.hasColor)  
	       colTool->SetColor(label,attr.color,XCAFDoc_ColorGen);
       if(attr.hasColorL) 
	       colTool->SetColor(label,attr.colorL,XCAFDoc_ColorGen);
}



void MwOCAF::setProjectDir(const char *name){
 projectDir=std::string(name);
}	

bool MwOCAF::hasParts() 
{
  if (mainDoc.IsNull()) return false;
  TDF_Label parts=XCAFDoc_DocumentTool::DocLabel(mainDoc->Main()).FindChild(6,Standard_False);
  if (parts.IsNull()) return false;
  return (parts.NbChildren() >0);
}


TDF_Label MwOCAF::BConditionsLabel() 
{
  TDF_Label L = XCAFDoc_DocumentTool::DocLabel(mainDoc->Main()).FindChild(7,Standard_True);
  TDataStd_Name::Set(L, "Boundary Conditions");
  return L;
}


TDF_Label MwOCAF::dataLabel() 
{
  TDF_Label L = XCAFDoc_DocumentTool::DocLabel(mainDoc->Main()).FindChild(8,Standard_False);
  return L;
}

TDF_Label MwOCAF::makeDataLabel() 
{
  TDF_Label L = XCAFDoc_DocumentTool::DocLabel(mainDoc->Main()).FindChild(8,Standard_True);
  return L;
}


void MwOCAF::resetShapeTool(){
  TDF_Label L=XCAFDoc_DocumentTool::ShapesLabel(mainDoc->Main());
  if (L.FindAttribute (XCAFDoc_ShapeTool::GetID(), shapeTool)) L.ForgetAttribute(XCAFDoc_ShapeTool::GetID());
  shapeTool = XCAFDoc_ShapeTool::Set(L);
  shapeTool->ComputeShapes(L);
}

void MwOCAF::setDocTools(){
  shapeTool    =XCAFDoc_DocumentTool::ShapeTool(mainDoc->Main());
  colorTool    =XCAFDoc_DocumentTool::ColorTool(mainDoc->Main());
  layerTool    =XCAFDoc_DocumentTool::LayerTool(mainDoc->Main());
  materialTool =XCAFDoc_DocumentTool::MaterialTool(mainDoc->Main());
  dimtolTool   =XCAFDoc_DocumentTool::DimTolTool(mainDoc->Main());
  setMissingNames(mainDoc->Main());
  theShapes=XCAFDoc_DocumentTool::ShapesLabel(mainDoc->Main());
  theLayers=XCAFDoc_DocumentTool::LayersLabel(mainDoc->Main());
  theMaterials=XCAFDoc_DocumentTool::MaterialsLabel(mainDoc->Main());
  theDGTs=XCAFDoc_DocumentTool::DGTsLabel(mainDoc->Main());
  theParts = XCAFDoc_DocumentTool::DocLabel(mainDoc->Main()).FindChild(6,Standard_True);
//  if(hasParts()) theParts=partsLabel();
//  else theParts.Nullify();
  theData=dataLabel();
//  theBConditions=BConditionsLabel();
}

bool hasSolid(TopoDS_Shape S)
{
 TopTools_IndexedMapOfShape tmp;
 TopExp::MapShapes(S,TopAbs_SOLID,tmp);
 return tmp.Extent();
}
bool hasFace(TopoDS_Shape S)
{
 TopTools_IndexedMapOfShape tmp;
 TopExp::MapShapes(S,TopAbs_FACE,tmp);
 return tmp.Extent();
}

int MwOCAF::isAssembly(TDF_Label label){
  return shapeTool->IsAssembly(label);
}
int MwOCAF::isTopLevel(TDF_Label label){
  return shapeTool->IsTopLevel(label);
}
int MwOCAF::isFree(TDF_Label label){
  return shapeTool->IsFree(label);
}
void MwOCAF::getFreeShapes(TDF_LabelSequence frshapes){
 shapeTool->GetFreeShapes(frshapes);
}

void MwOCAF::removeLabel(const TDF_Label& label)
{
 label.ForgetAllAttributes (Standard_True);
}

bool MwOCAF::getLabelName(TDF_Label item, TCollection_AsciiString &name){
  Handle(TDataStd_Name)  nameAtt;
  if(item.FindAttribute(TDataStd_Name::GetID(),nameAtt)){
    name=nameAtt->Get();
    return 1;
  }
  return 0;
}

int MwOCAF::isPart(TDF_Label label){
  TCollection_AsciiString name;
  if(!getLabelName(label, name)) return false;
//  bool itis=isComponent(label) || isSolid(label);
  bool itis=isValidPartName(name);
  if(itis) itis=itis && EmP->FindVolume(name.ToCString());
  return itis;
}

int MwOCAF::volType(TDF_Label label){
  DB::Volume *vol=getLabelVol(label);
  if(!vol) return 0;
  else return vol->type;
}



int MwOCAF::isSimpleShape(TDF_Label label){
  return shapeTool->IsSimpleShape(label);
}
int MwOCAF::isShape(TDF_Label label){
  return shapeTool->IsShape(label);
}
int MwOCAF::isReference(TDF_Label label){
  return shapeTool->IsReference(label);
}
int MwOCAF::isCompound(TDF_Label label){
  return shapeTool->IsCompound(label);
}

int MwOCAF::isComponent(TDF_Label label){
  return shapeTool->IsComponent(label);
}

int MwOCAF::isDisabled(TDF_Label label){
  DB::Volume *vol=getLabelVol(label);
  if(vol) return vol->disabled;
  else    return 0;
}

int MwOCAF::isSolid(TDF_Label label){
  TopoDS_Shape shape = shapeTool->GetShape(label);
  if(shape.IsNull()) return 0;
  else return (shape.ShapeType()==TopAbs_SOLID);
}
int MwOCAF::isShell(TDF_Label label){
  TopoDS_Shape shape = shapeTool->GetShape(label);
  if(shape.IsNull()) return 0;
  else return (shape.ShapeType()==TopAbs_SHELL);
}
int MwOCAF::isSheet(TDF_Label label){
  TopoDS_Shape S = shapeTool->GetShape(label);
  if(S.IsNull()) return 0;
  else return !hasSolid(S) && hasFace(S);
}
int MwOCAF::isFace(TDF_Label label){
  TopoDS_Shape shape = shapeTool->GetShape(label);
  if(shape.IsNull()) return 0;
  else return (shape.ShapeType()==TopAbs_FACE);
}
int MwOCAF::isEdge(TDF_Label label){
  TopoDS_Shape shape = shapeTool->GetShape(label);
  if(shape.IsNull()) return 0;
  else return (shape.ShapeType()==TopAbs_EDGE);
}
int MwOCAF::isVertex(TDF_Label label){
  TopoDS_Shape shape = shapeTool->GetShape(label);
  if(shape.IsNull()) return 0;
  else return (shape.ShapeType()==TopAbs_VERTEX);
}

void MwOCAF::setLabelShape(TDF_Label label, TopoDS_Shape shape){
 shapeTool->SetShape(label,shape);
}


//-----------------------------------------
//Names:

int MwOCAF::setLabelName(TDF_Label item, TCollection_AsciiString name){
  Handle(TDataStd_Name)  nameAtt=TDataStd_Name::Set(item,name);
  return 0;
}


//-------------------
//PartsData:
bool MwOCAF::getPartName(int I, TCollection_AsciiString &name){
  TDF_Label label1 = theParts.FindChild(I,Standard_False);
  return getLabelName(label1, name);
}

int MwOCAF::isParts(TDF_Label item){
  return(item==theParts);
}


//-------------
//Materials:

int MwOCAF::isMaterial(TDF_Label label){
  return materialTool->IsMaterial(label);
}
int MwOCAF::isMaterials(TDF_Label item){
  return(item==theMaterials);
}
void MwOCAF::getMaterials(TDF_Label &item){
  item=theMaterials;
}

TDF_Label MwOCAF::addMaterial(TCollection_AsciiString name){
 TCollection_HAsciiString name_=TCollection_HAsciiString(name);
 TCollection_HAsciiString description=TCollection_HAsciiString();
 Standard_Real density=1;
 TCollection_HAsciiString densityName=TCollection_HAsciiString("Epsilonr");
 TCollection_HAsciiString densityValType=TCollection_HAsciiString("Real");
 return materialTool->AddMaterial( &name_, &description, density, &densityName, &densityValType);
}
 
void MwOCAF::setLabelMaterial(TDF_Label& label, TDF_Label& matL){
 materialTool->SetMaterial(label,matL);
}

void MwOCAF::removeAllMaterials()
{
 TDF_LabelSequence labels;
 materialTool->GetMaterialLabels(labels);
 for (int i = 1; i <= labels.Length(); i++){
      TDF_Label label=labels.Value(i);
      label.ForgetAllAttributes (Standard_True);
 }
}


//-------------
//Boundary Conditions:
int MwOCAF::isBCondition(TDF_Label label){
  return label.Father()==theBConditions;
}
int MwOCAF::isBConditions(TDF_Label item){
  return(item==theBConditions);
}
void MwOCAF::getBConditions(TDF_Label &item){
  item=theBConditions;
}

TDF_Label MwOCAF::addBCondition(TCollection_AsciiString name)
{
  TDF_TagSource aTag;
  TDF_Label L = aTag.NewChild ( theBConditions );
  TDataStd_Name::Set(L,name);
  return L;
}

void MwOCAF::removeAllBConditions()
{
 for (TDF_ChildIterator it(theBConditions,Standard_False); it.More(); it.Next()){
    TDF_Label label = it.Value();
    label.ForgetAllAttributes (Standard_True);
 }
}

void MwOCAF::setLabelBCondition(TDF_Label& L, TDF_Label& condL)
{
  // set reference
  Handle(TDataStd_TreeNode) refNode, mainNode;
  mainNode = TDataStd_TreeNode::Set ( condL, XCAFDoc::MaterialRefGUID() );
  refNode  = TDataStd_TreeNode::Set ( L,     XCAFDoc::MaterialRefGUID() );
  refNode->Remove(); // abv: fix against bug in TreeNode::Append()
  mainNode->Append(refNode);
}


//-------------
//Layers:
//

int MwOCAF::isLayer(TDF_Label label){
  return layerTool->IsLayer(label);
}

int MwOCAF::isLayers(TDF_Label item){
  return(item==theLayers);
}

void MwOCAF::getLayers(TDF_Label &item){
  item=theLayers;
}

bool MwOCAF::getLabelLayer(TDF_Label label, TCollection_ExtendedString &layer){
  return layerTool->GetLayer(label,layer);
}

void MwOCAF::setLabelLayer(TDF_Label label, TCollection_ExtendedString &layer){
  layerTool->SetLayer(label,layer);
}

bool MwOCAF::getLabelLayers(TDF_Label label, Handle(TColStd_HSequenceOfExtendedString) &layers){
  return layerTool->GetLayers(label,layers);
}


//DGTs:
int MwOCAF::isDGTs(TDF_Label item){
  return(item==theDGTs);
}
//--------------------
//
DB::Volume * MwOCAF::getLabelVol(TDF_Label label){
      Handle(TDataStd_Name)  nameAtt;
      if (!label.FindAttribute(TDataStd_Name::GetID(),nameAtt)) return NULL;
      TCollection_AsciiString name=nameAtt->Get();
      return EmP->FindVolume(name.ToCString());
}

DB::Volume * MwOCAF::getLabelUpperVol(TDF_Label label, DB::EmProblem *upperEmP){
      Handle(TDataStd_Name)  nameAtt;
      if (!label.FindAttribute(TDataStd_Name::GetID(),nameAtt)) return NULL;
      TCollection_AsciiString name=nameAtt->Get();
      return upperEmP->FindVolume(name.ToCString());
}


//--------------------



void MwOCAF::setLabelIndex(TDF_Label item, int I){
  if (item.IsAttribute(TDataStd_Integer::GetID()))  item.ForgetAttribute(TDataStd_Integer::GetID());
  Handle(TDataStd_Integer) intAtt = TDataStd_Integer::Set(item,I);
}



int MwOCAF::getLabelIndex(TDF_Label item){
  int I;
  Handle(TDataStd_Integer) intAtt = new TDataStd_Integer();
  if(item.FindAttribute(TDataStd_Integer::GetID(),intAtt))
    I=intAtt->Get();
  else
    I=0;
  return I;
}


void MwOCAF::changeColor(TDF_Label label, Quantity_NameOfColor colName){
  colorTool->SetColor(label,Quantity_Color(colName),XCAFDoc_ColorGen);
  Handle(TPrsStd_AISPresentation)  prs;
  if(label.FindAttribute(TPrsStd_AISPresentation::GetID(),prs)){
    if(prs->HasOwnColor()) {
	if(prs->Color()!=colName)  prs->SetColor(colName);
    } else  
	prs->SetColor(colName);
    prs->Update();
  }
  //recursion:
  for (TDF_ChildIterator it(label,Standard_False); it.More(); it.Next()) 
  {
     TDF_Label item = it.Value();
     changeColor(item, colName);  //recursion;
  }
}


void MwOCAF::assignMatColor(TDF_Label label){
  Quantity_NameOfColor colName=Quantity_NOC_SALMON4;
  DB::Volume *vol=getLabelVol(label);
  if(vol){
     if(vol->type==SPLITTER || vol->type==GRID) colName=Quantity_NOC_SEAGREEN;
     if(vol->type==WAVEGUIDE) colName=Quantity_NOC_RED;
     else{
       DB::Material* mat = EmP->FindMaterial(vol->material);
       if(mat){
	 Standard_Real r=mat->color[0]/255.0;
	 Standard_Real g=mat->color[1]/255.0;
	 Standard_Real b=mat->color[2]/255.0;
	 colName=Quantity_Color::Name(r,g,b);
       }
     }
  }
  changeColor(label,colName);
}




/*
void MwOCAF::assignMatColor(TDF_Label label){
  Quantity_Color col=Quantity_Color(Quantity_NOC_SALMON4);
  DB::Volume *vol=getLabelVol(label);
  if(vol){
      DB::Material* mat = EmP->FindMaterial(vol->material);
      if(mat){
	 Standard_Real r=mat->color[0]/255.0;
	 Standard_Real g=mat->color[1]/255.0;
	 Standard_Real b=mat->color[2]/255.0;
	 col.SetValues(r,g,b,Quantity_TOC_RGB);
      }
  }
  TDF_Label referred; 
  bool isref=shapeTool->GetReferredShape(label,referred);
  if(isref){
       	   colorTool->UnSetColor(referred,XCAFDoc_ColorGen);
           colorTool->SetColor(referred, col, XCAFDoc_ColorGen);
  } else {		
           colorTool->UnSetColor(label,XCAFDoc_ColorGen);
           colorTool->SetColor(label, col, XCAFDoc_ColorGen);
  }
}
*/

Handle(TPrsStd_AISPresentation) MwOCAF::setLabelPresentation(TDF_Label label, double transp)
{ 

  TopoDS_Shape S= shapeTool->GetShape(label);
  Handle(TNaming_NamedShape) NS;
  if(!label.FindAttribute(TNaming_NamedShape::GetID(),NS))  {
    TNaming_Builder builder(label);
    builder.Generated(S);
    NS=builder.NamedShape();
  }
  Handle(TPrsStd_AISPresentation)  prs;
  label.FindAttribute(TPrsStd_AISPresentation::GetID(),prs);
  prs=TPrsStd_AISPresentation::Set(NS);


  if(!prs->HasOwnColor()) prs->SetColor(Quantity_NOC_SALMON4);


//  prs->SetColor(colName);
  prs->SetMode(AIS_Shaded);
  prs->SetSelectionMode(0);
  if(transp>0 && transp<1) prs->SetTransparency(transp);
  prs->Update();
  TPrsStd_AISViewer::Update(mainDoc->GetData()->Root());
  return prs;
}


Handle(TPrsStd_AISPresentation) MwOCAF::setLabelPresentationProp(TDF_Label label)
{ 
  Handle(TPrsStd_AISPresentation)  prs;
  if(!label.FindAttribute(TPrsStd_AISPresentation::GetID(),prs)) return;
  Quantity_Color color;
  XCAFDoc_ColorType ctype=XCAFDoc_ColorGen;
  bool labelHasColor =colorTool->GetColor(label,ctype,color);
  if(labelHasColor){
        Standard_Real r,g,b;
        color.Values(r,g,b,Quantity_TOC_RGB);
        Quantity_NameOfColor colName=Quantity_Color::Name(r,g,b);
	prs->SetColor(colName);
  }
  return prs;
}



void MwOCAF::displayLabelShape(TDF_Label label, bool B)
{ 
  if(B) if(isDisabled(label)) return;
  if(!shapeTool->IsShape(label)) return;
  Handle(TPrsStd_AISPresentation)  prs=setLabelPresentation(label);
  setLabelPresentationProp(label);
  if(B)  prs->Display(Standard_True);
  else   prs->Erase(Standard_True);
}


void MwOCAF::setVisibility(const TDF_Label& label,const bool B){
// colorTool->SetVisibility(label, B);
 Handle(TPrsStd_AISPresentation)  prs;
 if(label.FindAttribute(TPrsStd_AISPresentation::GetID(),prs)) prs->SetDisplayed(B);
}

void MwOCAF::displayLabelMesh(TDF_Label label, bool B)
{ 
  if(!myContext) return;
  if(!isShape(label)) return;
  if(!indexedShapes) return;
  TopoDS_Shape S= shapeTool->GetShape(label);
  if(S.ShapeType()==TopAbs_FACE){
     int I=indexedShapes->FindIndex(S);
     if(I){
         Handle( MeshVS_Mesh) faceVSMesh=getSubMeshVS(I);
         if(!faceVSMesh.IsNull()){
          if(B)  {
                     faceVSMesh->SetColor(Quantity_NOC_AZURE);
	             faceVSMesh->SetDisplayMode( MeshVS_DMF_WireFrame );
	             faceVSMesh->SetHilightMode( MeshVS_DMF_WireFrame );
		     myContext->Display(faceVSMesh, true);
		 }
          else   myContext->Erase(faceVSMesh, true);
	 }
     }
  }
}




void MwOCAF::highlightLabel(TDF_Label label, bool B)
{
 if(!myContext) return;
 Handle(TPrsStd_AISPresentation)  prs;
 if(label.FindAttribute(TPrsStd_AISPresentation::GetID(),prs)){
     Handle(AIS_InteractiveObject) iobj=(prs->GetAIS());
     if(B) myContext->Hilight(iobj, Standard_True);
     else  myContext->Unhilight(iobj, Standard_True);
 }
}

void MwOCAF::setMissingNames(TDF_Label root)
{
  TCollection_AsciiString name;
  Handle(TDataStd_Name) nameAtt;
  bool hasNameAtt=root.FindAttribute(TDataStd_Name::GetID(),nameAtt);
  if(hasNameAtt) name=nameAtt->Get();
  bool missingName=name.IsEmpty() || (name==TCollection_AsciiString("NONE"));
  if(missingName) {
    TopoDS_Shape shape;
    if(shapeTool->GetShape(root,shape)) if(!shape.IsNull()){
       char cstr[100];
       int itag= root.Tag();
       if (shape.ShapeType() == TopAbs_COMPOUND){
	  sprintf(cstr,"Comp%d",itag);                   
	  TCollection_AsciiString name(cstr);
          if(isAssembly(root)) nameAtt=TDataStd_Name::Set(root,name);
       }else if (shape.ShapeType() == TopAbs_SOLID){
	  sprintf(cstr,"Solid%d",itag);
          TCollection_AsciiString name(cstr);
          nameAtt=TDataStd_Name::Set(root,name);
       }else if (shape.ShapeType() == TopAbs_SHELL){
	  sprintf(cstr,"Shell%d",itag);
          TCollection_AsciiString name(cstr);
          nameAtt=TDataStd_Name::Set(root,name);
       } else if (shape.ShapeType() == TopAbs_WIRE){
	  sprintf(cstr,"Wire%d",itag);
          TCollection_AsciiString name(cstr);
          nameAtt=TDataStd_Name::Set(root,name);
       } else if (shape.ShapeType() == TopAbs_FACE){
	  sprintf(cstr,"F%d",itag);
          TCollection_AsciiString name(cstr);
          nameAtt=TDataStd_Name::Set(root,name);
       } else if (shape.ShapeType() == TopAbs_EDGE){
	  sprintf(cstr,"E%d",itag);
          TCollection_AsciiString name(cstr);
          nameAtt=TDataStd_Name::Set(root,name);
       }
    }
  }
  for (TDF_ChildIterator it(root,Standard_False); it.More(); it.Next()) 
  {
     TDF_Label item = it.Value();
     setMissingNames(item);  //recursion;
  }
}


int edgeNum(TopoDS_Shape &S){
   TopTools_IndexedMapOfShape Emap;
   for (TopExp_Explorer exp(S,TopAbs_EDGE); exp.More(); exp.Next())  Emap.Add(exp.Current());
   return Emap.Extent();
}


void MwOCAF::setMissingNames(){
  setMissingNames(theShapes);
}

void MwOCAF::setGlobalNames(TDF_Label root)
{
  Handle(TDataStd_Name)  nameAtt;
  bool hasName =root.FindAttribute(TDataStd_Name::GetID(),nameAtt);
  TCollection_AsciiString name;
  if(hasName) name=nameAtt->Get();
  TopoDS_Shape shape;
  if(shapeTool->GetShape(root,shape)) if(!shape.IsNull()){
       char cstr[100];
       if (shape.ShapeType() == TopAbs_SOLID){
	  int I=indexedSolids->FindIndex(shape); 
	  sprintf(cstr,"Vol%d",I);
          TCollection_AsciiString name(cstr);
          if(!hasName) nameAtt=TDataStd_Name::Set(root,name);
       } else if (shape.ShapeType() == TopAbs_FACE){
	  shape.Orientation(TopAbs_FORWARD);
	  int I=indexedFaces->FindIndex(shape); 
	  if(I>0) sprintf(cstr,"F%d",I);
	  else    sprintf(cstr,"F%d",0);
          TCollection_AsciiString name(cstr);
          nameAtt=TDataStd_Name::Set(root,name);
       } else if (shape.ShapeType() == TopAbs_EDGE){
	  shape.Orientation(TopAbs_FORWARD);
	  int I=indexedEdges->FindIndex(shape); 
	  sprintf(cstr,"E%d",I);
          TCollection_AsciiString name(cstr);
          nameAtt=TDataStd_Name::Set(root,name);
       } else if (shape.ShapeType() == TopAbs_VERTEX){
	  shape.Orientation(TopAbs_FORWARD);
	  int I=indexedVertices->FindIndex(shape); 
	  sprintf(cstr,"P%d",I);
          TCollection_AsciiString name(cstr);
          nameAtt=TDataStd_Name::Set(root,name);
       }

  }
  for (TDF_ChildIterator it(root,Standard_False); it.More(); it.Next()) 
  {
     TDF_Label item = it.Value();
     setGlobalNames(item);  //recursion;
  }
}

void MwOCAF::setGlobalNames(){
  setGlobalNames(theShapes);
}


void MwOCAF::setSelectedLabels()
{
//  Handle(AIS_InteractiveContext) myContext=context->getContext();
  if(!myContext || !indexedShapes) return;
  for(myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected()){
   TopoDS_Shape ashape = myContext->SelectedShape();
   if(ashape.IsNull()) theSelectedLabels=NULL;
   else{
    int I=indexedShapes->FindIndex(ashape);
    if(I) theSelectedLabels=shapeIndex2Labels[I-1]; 
    else  theSelectedLabels=NULL;
   }
  }
}


void MwOCAF::openLabel(TDF_Label label)
{
 for (TDF_ChildIterator it(label, Standard_False); it.More(); it.Next()) 
 {
     TDF_Label item = it.Value();
     if(volType(label)!=GRID) displayLabelShape(item, Standard_True);  //recursion;
 }
}

void MwOCAF::closeLabel(TDF_Label label)
{
 for (TDF_ChildIterator it(label, Standard_False); it.More(); it.Next()) 
 {
     TDF_Label item = it.Value();
     displayLabelShape(item, Standard_False);  //recursion;
 }
}




void MwOCAF::setIntData(int *Iarray, int len){
 if(theData.IsNull()) theData=makeDataLabel();
 Handle(TDataStd_IntegerArray)  IarrAtt;
 if(!theData.FindAttribute(TDataStd_IntegerArray::GetID(),IarrAtt)){
   IarrAtt = TDataStd_IntegerArray::Set (theData, 0, len-1);
 }
 for(int i =0; i<len; i++) IarrAtt->SetValue(i, Iarray[i]); 
}

int MwOCAF::intData(int i){
 if(theData.IsNull()) return 0;
 Handle(TDataStd_IntegerArray)  IarrAtt;
 if(!theData.FindAttribute(TDataStd_IntegerArray::GetID(),IarrAtt)) return 0;
 if (i< IarrAtt->Lower() || i > IarrAtt->Upper()) return 0;
 return IarrAtt->Value(i); 
}



void MwOCAF::displayAllParts(){
 if(theParts.IsNull()) return;
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
     TDF_Label label = it.Value();
     TDF_Label label1 = getRefLabel(label);
     if(label1.IsNull()) label1=label;
     if(volType(label1)!=GRID) displayLabelShape(label1, true);
 }
}


bool MwOCAF::makeTheCompSolid(){
 BRep_Builder builder;
 builder.MakeCompSolid(theCompSolid);
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
     TDF_Label label1 = it.Value();
     Handle(TDF_Reference)  refAtt;
     assert(label1.FindAttribute(TDF_Reference::GetID(),refAtt));
     TDF_Label label = refAtt->Get();
     bool skip=true;
     DB::Volume *vol=getLabelVol(label1);
     if(vol) skip=(vol->type==WAVEGUIDE || vol->type==BOUNDARYCOND || vol->type==SPLITTER  || vol->type==GRID || vol->type==LINEPORT);
     if(skip) continue;
     TopoDS_Shape S = shapeTool->GetShape(label);
     if(!S.IsNull()){ 
	     if(S.ShapeType()==TopAbs_SOLID) builder.Add(theCompSolid,S);
	     else if(S.ShapeType()==TopAbs_COMPSOLID || S.ShapeType()==TopAbs_COMPOUND) 
                 for(TopExp_Explorer exp(S,TopAbs_SOLID); exp.More(); exp.Next()) 
			 builder.Add(theCompSolid,exp.Current());
	     else continue;
     }
 }
 return true;
}

/*
bool MwOCAF::makeTheCompound(){
 BRep_Builder builder;
 builder.MakeCompound(theCompound);
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
     TDF_Label label1 = it.Value();
     Handle(TDF_Reference)  refAtt;
     assert(label1.FindAttribute(TDF_Reference::GetID(),refAtt));
     TDF_Label label = refAtt->Get();
     bool skip=false;
     DB::Volume *vol=getLabelVol(label);
     if(vol) skip=(vol->type==WAVEGUIDE || vol->type==BOUNDARYCOND || vol->type==SPLITTER || vol->type==GRID);
     if(skip) continue;
     TopoDS_Shape S = shapeTool->GetShape(label);
     if(!S.IsNull()){ 
	     if(S.ShapeType()==TopAbs_SOLID || S.ShapeType()==TopAbs_COMPSOLID) builder.Add(theCompound,S);
	     else return false;
     }
 }
 return true;
}
*/

void MwOCAF::removeWithSubComponents(TDF_Label label){
   for (TDF_ChildIterator it(label,Standard_False); it.More(); it.Next()) {
      TDF_Label label1 = it.Value();
      removeWithSubComponents(label1);
   }
   TDF_Label referred; 
   bool isref=shapeTool->GetReferredShape(label,referred);
   if(isComponent(label))    shapeTool->RemoveComponent(label);
   else		             shapeTool->RemoveShape(label);
   if(isref) removeWithSubComponents(referred);
}



void shapeCompare(TopoDS_Shape S1, TopoDS_Shape S2){
 ShapeAnalysis_ShapeContents safc1;
 ShapeAnalysis_ShapeContents safc2;
//set a corresponding flag for storing faces based on the surfaces
 safc1.Perform(S1);
 safc2.Perform(S2);
//getting the number of offset surfaces in the shape
 if(safc2.NbShells()-safc1.NbShells()) std::cout << "NbShells Diff=" << safc2.NbShells()-safc1.NbShells() <<"\n";
 if(safc2.NbFaces()-safc1.NbFaces())   std::cout << "NbFaces Diff=" << safc2.NbFaces()-safc1.NbFaces() <<"\n";
 if(safc2.NbWires()-safc1.NbWires())   std::cout << "NbWires Diff=" << safc2.NbWires()-safc1.NbWires() <<"\n";
 if(safc2.NbEdges()-safc1.NbEdges())   std::cout << "NbEdges Diff=" << safc2.NbEdges()-safc1.NbEdges() <<"\n";
 if(safc2.NbVertices()-safc1.NbVertices())   std::cout << "NbVertices Diff=" << safc2.NbVertices()-safc1.NbVertices() <<"\n";
 if(safc2.NbBigSplines()-safc1.NbBigSplines())   std::cout << "NbBigSplines Diff=" << safc2.NbBigSplines()-safc1.NbBigSplines() <<"\n";
 if(safc2.NbC0Surfaces()-safc1.NbC0Surfaces())   std::cout << "NbC0Surfaces Diff=" << safc2.NbC0Surfaces()-safc1.NbC0Surfaces() <<"\n";
 if(safc2.NbC0Curves()-safc1.NbC0Curves())   std::cout << "NbC0Curves Diff=" << safc2.NbC0Curves()-safc1.NbC0Curves() <<"\n";
 if(safc2.NbOffsetSurf()-safc1.NbOffsetSurf())   std::cout << "NbOffsetSurf Diff=" << safc2.NbOffsetSurf()-safc1.NbOffsetSurf() <<"\n";
 if(safc2.NbIndirectSurf()-safc1.NbIndirectSurf())   std::cout << "NbIndirectSurf Diff=" << safc2.NbIndirectSurf()-safc1.NbIndirectSurf() <<"\n";
 if(safc2.NbOffsetCurves()-safc1.NbOffsetCurves())   std::cout << "NbOffsetCurves Diff=" << safc2.NbOffsetCurves()-safc1.NbOffsetCurves() <<"\n";
 if(safc2.NbTrimmedCurve2d()-safc1.NbTrimmedCurve2d())   std::cout << "NbTrimmedCurve2d Diff=" << safc2.NbTrimmedCurve2d()-safc1.NbTrimmedCurve2d() <<"\n";
 if(safc2.NbTrimmedCurve3d()-safc1.NbTrimmedCurve3d())   std::cout << "NbTrimmedCurve3d Diff=" << safc2.NbTrimmedCurve3d()-safc1.NbTrimmedCurve3d() <<"\n";
 if(safc2.NbBSplibeSurf()-safc1.NbBSplibeSurf())   std::cout << "NbBSplibeSurf Diff=" << safc2.NbBSplibeSurf()-safc1.NbBSplibeSurf() <<"\n";
 if(safc2.NbBezierSurf()-safc1.NbBezierSurf())   std::cout << "NbBezierSurf Diff=" << safc2.NbBezierSurf()-safc1.NbBezierSurf() <<"\n";
 if(safc2.NbTrimSurf()-safc1.NbTrimSurf())   std::cout << "NbTrimSurf Diff=" << safc2.NbTrimSurf()-safc1.NbTrimSurf() <<"\n";
 if(safc2.NbWireWitnSeam()-safc1.NbWireWitnSeam())   std::cout << "NbWireWitnSeam Diff=" << safc2.NbWireWitnSeam()-safc1.NbWireWitnSeam() <<"\n";
 if(safc2.NbWireWithSevSeams()-safc1.NbWireWithSevSeams())   std::cout << "NbWireWithSevSeams Diff=" << safc2.NbWireWithSevSeams()-safc1.NbWireWithSevSeams() <<"\n";
 if(safc2.NbFaceWithSevWires()-safc1.NbFaceWithSevWires())   std::cout << "NbFaceWithSevWires Diff=" << safc2.NbFaceWithSevWires()-safc1.NbFaceWithSevWires() <<"\n";
 if(safc2.NbNoPCurve()-safc1.NbNoPCurve())   std::cout << "NbNoPCurve Diff=" << safc2.NbNoPCurve()-safc1.NbNoPCurve() <<"\n";
 if(safc2.NbFreeFaces()-safc1.NbFreeFaces())   std::cout << "NbFreeFaces Diff=" << safc2.NbFreeFaces()-safc1.NbFreeFaces() <<"\n";
 if(safc2.NbFreeWires()-safc1.NbFreeWires())   std::cout << "NbFreeWires Diff=" << safc2.NbFreeWires()-safc1.NbFreeWires() <<"\n";
 if(safc2.NbFreeEdges()-safc1.NbFreeEdges())   std::cout << "NbFreeEdges Diff=" << safc2.NbFreeEdges()-safc1.NbFreeEdges() <<"\n";
 if(safc2.NbSharedShells()-safc1.NbSharedShells())   std::cout << "NbSharedShells Diff=" << safc2.NbSharedShells()-safc1.NbSharedShells() <<"\n";
 if(safc2.NbSharedFaces()-safc1.NbSharedFaces())   std::cout << "NbSharedFaces Diff=" << safc2.NbSharedFaces()-safc1.NbSharedFaces() <<"\n";
 if(safc2.NbSharedWires()-safc1.NbSharedWires())   std::cout << "NbSharedWires Diff=" << safc2.NbSharedWires()-safc1.NbSharedWires() <<"\n";
 if(safc2.NbSharedEdges()-safc1.NbSharedEdges())   std::cout << "NbSharedEdges Diff=" << safc2.NbSharedEdges()-safc1.NbSharedEdges() <<"\n";
 if(safc2.NbSharedVertices()-safc1.NbSharedVertices())   std::cout << "NbSharedVertices Diff=" << safc2.NbSharedVertices()-safc1.NbSharedVertices() <<"\n";
}


void MwOCAF::checkWires(){
  TopoDS_Compound   healedShape;
  BRep_Builder builder;
  builder.MakeCompound(healedShape);
  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
  {
      TDF_Label label1 = it.Value();
      DB::Volume *vol=getLabelVol(label1);
      if(!vol) continue;
      if(vol->type==SPLITTER || vol->type==GRID) continue;
      Handle(TDF_Reference)  refAtt;
      if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
      TDF_Label label = refAtt->Get();
      TopoDS_Shape S = shapeTool->GetShape(label);
      if(!S.IsNull()) builder.Add(healedShape, S);
  }

  for(TopExp_Explorer exp1(healedShape,TopAbs_FACE); exp1.More(); exp1.Next()){
   TopoDS_Face face=TopoDS::Face(exp1.Current());
   for(TopExp_Explorer exp2(face,TopAbs_WIRE); exp2.More(); exp2.Next()){
    TopoDS_Wire wire=TopoDS::Wire(exp2.Current());
    double precision=1.e-4;
    ShapeAnalysis_Wire saw (wire, face, precision);
//Creates a tool and loads objects into it
    if (saw.CheckOrder()) {
     std::cout<< "Some edges in the wire need to be reordered"<<std::endl;
     std::cout<<"Please ensure that all the edges are correctly ordered before further analysis"<<std::endl;
     return;
    }
    if (saw.CheckSmall (precision)) 
      std::cout<<"Wire contains edge(s) shorter than "<<precision<<std::endl;
    if (saw.CheckConnected())
      std::cout<<"Wire is disconnected"<<std::endl;
    if (saw.CheckSelfIntersection())
      std::cout<<"Wire has self-intersecting or intersecting adjacent edges" << std::endl;
   }
  }

}

void MwOCAF::heal(){

//----
//  initial healing:
  TopoDS_Compound   compoundShape;
  BRep_Builder builder;
  builder.MakeCompound(compoundShape);
  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
  {
      TDF_Label label1 = it.Value();
      DB::Volume *vol=getLabelVol(label1);
      if(!vol) continue;
      if(vol->type==SPLITTER || vol->type==GRID) continue;
      Handle(TDF_Reference)  refAtt;
      if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
      TDF_Label label = refAtt->Get();
      TopoDS_Shape S = shapeTool->GetShape(label);
      if(!S.IsNull()) builder.Add(compoundShape, S);
  }


  ShapeFix_Shape SFS(compoundShape);
//  SFS.FixShellTool()->FixOrientationMode()=Standard_True;

  SFS.FixFaceTool()->FixMissingSeamMode() =Standard_True;
  SFS.FixFaceTool()->FixOrientationMode()=Standard_False;
  SFS.FixFaceTool()->FixSmallAreaWireMode() = Standard_False;
  SFS.FixWireTool()->ModifyTopologyMode() =Standard_True;
  SFS.FixWireTool()->FixConnectedMode() = Standard_False;
  SFS.FixWireTool()->FixEdgeCurvesMode() = Standard_False;
  SFS.FixWireTool()->FixDegeneratedMode() = Standard_False;
  SFS.FixWireTool()->FixSelfIntersectionMode() = Standard_False; 
  SFS.FixWireTool()->FixLackingMode() = Standard_False; 
  SFS.FixWireTool()->FixSmallMode() =Standard_True;
  SFS.FixWireTool()->FixShiftedMode() =Standard_True;
  SFS.Perform();
  Handle(ShapeBuild_ReShape) SFS_context=SFS.Context();

  TopoDS_Shape result=compoundShape;
  if(SFS.Status(ShapeExtend_DONE)) result=SFS.Shape();

  ShapeUpgrade_ShapeDivideClosedEdges SDCE(result);
  SDCE.Perform();
  Handle(ShapeBuild_ReShape) SDCE_context=SDCE.GetContext();

/*
  ShapeUpgrade_ShapeDivideClosed SDC(result);
  SDC.Perform();
  Handle(ShapeBuild_ReShape) SDC_context=SDC.GetContext();
*/

  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
  {
     TDF_Label label1 = it.Value();
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue;
     if(vol->type==SPLITTER || vol->type==GRID) continue;
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
     TDF_Label label = refAtt->Get();
     TopoDS_Shape S = shapeTool->GetShape(label);
     if(!S.IsNull()) {
           TopoDS_Shape S1=SFS_context->Apply(S); if(S1.IsNull()) S1=S;
           TopoDS_Shape newS=SDCE_context->Apply(S1); if(newS.IsNull()) newS=S1;
           if(!newS.IsNull() && !newS.IsEqual(S)){ 
	     TDF_Label newlabel=replaceLabelShape(label,newS);
	     displayLabelShape(newlabel, true);
	     refAtt->Set(newlabel);
           } 
     } 
  }

  if(hasUPIF){
    for (int I = 1; I <=UPIFsubshapes.Extent() ; I++)
    {
       TopoDS_Shape S =UPIFsubshapes.FindKey(I);
       TopoDS_Shape S1=SFS_context->Apply(S); if(S1.IsNull()) S1=S;
       TopoDS_Shape newS=SDCE_context->Apply(S1); if(newS.IsNull()) newS=S1;
       if(!newS.IsNull() && !newS.IsEqual(S)) UPIFsubshapes.Substitute(I,newS);
    }
  }

}




TDF_Label MwOCAF::replaceLabelShape(TDF_Label label, TopoDS_Shape newS){
             Handle(TDF_Reference)  refAtt;
             TDF_Label label1;
	     bool usingRefLabel=label.FindAttribute(TDF_Reference::GetID(),refAtt);
             if(usingRefLabel) label1=refAtt->Get();
             else label1=label;
             Handle(TDataStd_Name)  nameAtt;
	     TCollection_AsciiString name;
	     bool hasName =label1.FindAttribute(TDataStd_Name::GetID(),nameAtt);
             if(hasName) name=nameAtt->Get();
             Handle(TColStd_HSequenceOfExtendedString) layers;
	     bool hasLayers=layerTool->GetLayers(label1,layers);
             Quantity_Color color;
	     TDF_Label colorL;
	     XCAFDoc_ColorType ctype=XCAFDoc_ColorGen;
	     bool hasColor =colorTool->GetColor(label1,ctype,color);
	     bool hasColorL=colorTool->GetColor(label1,ctype,colorL);
	     TDF_Label newlabel;
	     TDF_Label father=label1.Father();
             Handle(TDataStd_Name)  fatherNameAtt;
             TCollection_AsciiString fatherName;
             if(father.FindAttribute(TDataStd_Name::GetID(),fatherNameAtt)) fatherName=fatherNameAtt->Get();;
	     displayLabelShape(label1, false);
             if(CHECK_CREATED_SHAPES) if(newS.ShapeType()==TopAbs_SOLID|| newS.ShapeType()==TopAbs_COMPSOLID){
	        BRepCheck_Analyzer ana (newS, true);
	        assert(ana.IsValid());
	     }
             if(isComponent(label1)){
		  removeWithSubComponents(label1);
		  newlabel=shapeTool->AddComponent(father,newS); 
	     } else{
		  shapeTool->RemoveShape(label1);
	          newlabel=shapeTool->AddShape(newS);
	     }
	     if(!newlabel.IsNull()){
                  if(hasName){
                     Handle(TDataStd_Name)  newNameAtt;
                     assert(newlabel.FindAttribute(TDataStd_Name::GetID(),newNameAtt));
                     newNameAtt=TDataStd_Name::Set(newlabel,name);
		  }
	          if(hasLayers) for (int i = 1; i <= layers->Length(); i++)
	                              layerTool->SetLayer(newlabel,layers->Value(i));
	          if(hasColor)  colorTool->SetColor(newlabel,color,ctype);
	          if(hasColorL) colorTool->SetColor(newlabel,colorL,ctype);
	     }
	     if(usingRefLabel) refAtt->Set(newlabel);
             shapeTool->UpdateAssemblies();
	     return newlabel;
}



void MwOCAF::imprint(int downImprint){


 bool hasSymmetry=false;
 TopoDS_Shape symmBox;
 if(EmP->assemblyType==COMPONENT && !downImprint){
   BBox.SetVoid();
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
     TDF_Label label1 = it.Value();
     DB::Volume *vol=getLabelVol(label1);
     TDF_Label label = getRefLabel(label1);
     if(!vol) continue;
     if(vol->type==GRID) makeGridFaces(label1);
     if(vol->type==DIELECTRIC){
        TopoDS_Shape S= shapeTool->GetShape(label);
        BRepBndLib::Add(S, BBox);
     }
    }

   if(XYplaneSymmetry) makeSymmPlane(3, XYplaneSymmetry);
   if(YZplaneSymmetry) makeSymmPlane(1, YZplaneSymmetry);
   if(ZXplaneSymmetry) makeSymmPlane(2, ZXplaneSymmetry);
   hasSymmetry=XYplaneSymmetry||YZplaneSymmetry||ZXplaneSymmetry;
   if(hasSymmetry) symmBox=makeSymmBox(XYplaneSymmetry,YZplaneSymmetry,ZXplaneSymmetry);

 }

 if(hasUPIF || numberOfParts()>=1){ // else skip all imprinting:

// Define the imprinting parts
 TopTools_IndexedMapOfShape  imprintingParts;
 assert(hasParts());
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
     TDF_Label label1 = it.Value();
     DB::Volume *vol=getLabelVol(label1);
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;
     if(!vol) continue;
//     if(vol->type==SPLITTER || vol->type==GRID ) continue;
     TopoDS_Shape S;
     if(!(thePartShapes.IsBound(label1.Tag()))){
        S= shapeTool->GetShape(label);
        assert(!S.IsNull());
        if(S.ShapeType()==TopAbs_COMPOUND){
	   if(vol->type==WAVEGUIDE){
                TopoDS_Shell  newShell;
                BRep_Builder builder;
                builder.MakeShell(newShell);
		int fnum=0;
		for (TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
		    TopoDS_Face F=TopoDS::Face(exp.Current());
		    builder.Add(newShell,F);
		    fnum++;
		}
		if(fnum>0) if(!newShell.IsNull()) S=newShell;
           } 
           else if(vol->type==LINEPORT){
               TopoDS_Compound  newComp;
               BRep_Builder builder;
               builder.MakeCompound(newComp);
	       int en=0;
               for (TopExp_Explorer exp(S,TopAbs_EDGE); exp.More(); exp.Next()){
	           TopoDS_Edge E=TopoDS::Edge(exp.Current());
	           builder.Add(newComp,E);
	           en++;
	       }
               if(en>0) if(!newComp.IsNull()) S=newComp;
           }
        }
        if(vol->type==DIELECTRIC && hasSymmetry){
            S = BRepAlgoAPI_Common(S, symmBox);
        }
        thePartShapes.Bind(label1.Tag(),S);
     }
//     S.Location (loc.Multiplied(S.Location()));
     S=thePartShapes(label1.Tag());
     if(EmP->assemblyType==COMPONENT && S.ShapeType()==TopAbs_COMPOUND && !downImprint){
         int SoNum=0;
         int FNum=0;
         for (TopExp_Explorer exp(S,TopAbs_SOLID); exp.More(); exp.Next()){ 
	      TopoDS_Shape Si=exp.Current();
	      SoNum++;
	      imprintingParts.Add(Si);
         }
	 if(SoNum==0) for (TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){ 
	      TopoDS_Shape Si=exp.Current();
	      FNum++;
	      imprintingParts.Add(Si);
         }
     } else imprintingParts.Add(S);
  }


//  Imprinting:
  BOPAlgo_Splitter PS;
  if(EmP->assemblyType!=COMPONENT) PS.SetGlue(BOPAlgo_GlueShift);
  PS.SetFuzzyValue(1.e-3);
  PS.SetNonDestructive(true);

  {
    TopoDS_Compound  imprintingShapes;
    BRep_Builder builder;
    builder.MakeCompound(imprintingShapes);
    TopTools_MapOfShape ShapesMap, ToolsMap;
    for (int I=1; I<=imprintingParts.Extent(); I++){
       TopoDS_Shape S =imprintingParts.FindKey(I);
       if (ShapesMap.Add(S)){
	  if(downImprint) builder.Add(imprintingShapes,S); 
	  else            PS.AddArgument(S);
       }
    }
    if(downImprint) PS.AddArgument(imprintingShapes);
    if(hasUPIF) for (int I = 1; I <=UPIFsubshapes.Extent() ; I++){
       TopoDS_Shape S =UPIFsubshapes.FindKey(I);
       if (!ShapesMap.Contains(S) && ToolsMap.Add(S)) PS.AddTool(S);
    }
  }

  try {
    PS.Perform();
  } catch ( Standard_Failure ) {
	  throw ImprintFailure();
  }
  const TopTools_DataMapOfShapeListOfShape& PSmap=PS.Images();

//  Replace imprinting result in UPIFsubshapes:
  if(hasUPIF){
    for (int I = 1; I <=UPIFsubshapes.Extent() ; I++)
    {
       TopoDS_Shape S =UPIFsubshapes.FindKey(I);
//       const TopTools_ListOfShape& RSlist = PS.Modified(S);
       if(PSmap.IsBound(S)){
         const TopTools_ListOfShape& RSlist = PSmap.Find(S);
//         assert(RSlist.Extent()==1);
         TopTools_ListIteratorOfListOfShape RSit(RSlist);
	 TopoDS_Shape newS=RSit.Value();
	 if (!UPIFsubshapes.Contains(newS)){
	    if(S.ShapeType()==TopAbs_FACE){
              gp_Vec n1=faceNormal(S);
              gp_Vec n2=faceNormal(newS);
              if(n1.X()*n2.X()+n1.Y()*n2.Y()+n1.Z()*n2.Z()<0) newS=newS.Reversed();
	    }
            UPIFsubshapes.Substitute(I,newS);
	 }
       }
    }
  }
  if(EmP->assemblyType==NET){
     std::vector<int> colorPalette;
     colorPalette.push_back(Quantity_NOC_SEAGREEN);
     colorPalette.push_back(Quantity_NOC_AZURE);
     colorPalette.push_back(Quantity_NOC_RED);
     colorPalette.push_back(Quantity_NOC_BLUE);
     colorPalette.push_back(Quantity_NOC_CYAN);
     colorPalette.push_back(Quantity_NOC_ORANGE);
     colorPalette.push_back(Quantity_NOC_YELLOW);
     int ncol=colorPalette.size();
     int icol=0;
     for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
       TDF_Label label1 = it.Value();
       if((thePartShapes.IsBound(label1.Tag()))){
 	  TopoDS_Shape S=thePartShapes(label1.Tag());
          if(PSmap.IsBound(S)){
             const TopTools_ListOfShape& RSlist = PSmap.Find(S);
             if(!RSlist.IsEmpty()) S=*(RSlist.begin());
	  }
          if(!S.IsNull()){ //Replace Shape with new shape:
	      TDF_Label newlabel=replaceLabelShape(label1,S);
              Handle(TPrsStd_AISPresentation) prs=setLabelPresentation(newlabel,0.5);
              changeColor(newlabel,Quantity_NameOfColor(colorPalette[icol%ncol]));
	      displayLabelShape(newlabel, true);
              icol++;
          }
       }
     }
  } else if(EmP->assemblyType==COMPONENT){
//  TDF_LabelSequence dielLabels;
    std::vector<std::pair<double,int>> partPriorityTag;
    for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
    {
     TDF_Label label1 = it.Value();
//---
     double epsrInv=0;
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue;
     if(vol->type==DIELECTRIC) if(vol->material){
	DB::Material* mat= EmP->FindMaterial(vol->material);
	if(mat) epsrInv=1.0/mat->epsr;  
     }
     if(vol->type==WAVEGUIDE || vol->type==SPLITTER) epsrInv=10.0;
     std::pair<double,int> p=std::make_pair(epsrInv,it.Value().Tag());
     partPriorityTag.push_back(p);
    }
    std::sort(partPriorityTag.begin(),partPriorityTag.end(), std::less<std::pair<double,int> >());

    TopTools_IndexedMapOfShape  insertedSolids;
    TopTools_IndexedMapOfShape  dielFaces;

//-----------------------------------------------------------------------------
//  Replace original parts with the imprinted parts:
    for (int I=0; I < partPriorityTag.size(); I++){
       int tag=partPriorityTag[I].second;
       TDF_Label label1 = theParts.FindChild(tag,false);
       DB::Volume *vol=getLabelVol(label1);
//---
       Handle(TDataStd_Name)  nameAtt;
       TCollection_AsciiString name;
       if(label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) name=nameAtt->Get();
//---
       if(!vol) {std::cout <<"undefined item  \"" << name.ToCString() << "\"" << std::endl; assert(false);}; 
//       if(vol->type==SPLITTER || vol->type==GRID ) continue;
/*
     Handle(XCAFDoc_Location) locAtt;
     assert(label1.FindAttribute(XCAFDoc_Location::GetID(),locAtt));
     TopLoc_Location loc=locAtt->Get();
     newS.Location (loc.Multiplied(S.Location()));
*/
       TDF_Label label = getRefLabel(label1);
       if(label.IsNull()) label=label1;
       TopoDS_Shape S=thePartShapes(label1.Tag());
       TopoDS_Shape newS;
       TopoDS_Compound  newComp;
       BRep_Builder builder;
       TopoDS_CompSolid  newCompS;
       builder.MakeCompSolid(newCompS);
       TopoDS_Shell  newShell;
       builder.MakeShell(newShell);
       TopTools_ListOfShape RSlist;
       int SoNum=0;
       int FNum=0;
       if(S.ShapeType()==TopAbs_COMPOUND && !downImprint){
           for (TopExp_Explorer exp(S,TopAbs_SOLID); exp.More(); exp.Next()){ 
	      TopoDS_Shape Si=exp.Current();
              if(PSmap.IsBound(Si)){
		 RSlist= PSmap.Find(Si);
	         for (TopTools_ListIteratorOfListOfShape RSit(RSlist); RSit.More(); RSit.Next())
		     if(RSit.Value().ShapeType()==TopAbs_SOLID) if(insertedSolids.FindIndex(RSit.Value())<1){
			  insertedSolids.Add(RSit.Value());
			  builder.Add(newCompS,RSit.Value());
			  SoNum++;
		     }
	       } else if(Si.ShapeType()==TopAbs_SOLID){
	                  insertedSolids.Add(Si);
			  builder.Add(newCompS,Si);
			  SoNum++;
	       }
	   }
	   if(SoNum==0) for (TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){ 
	      TopoDS_Shape Si=exp.Current();
              if(PSmap.IsBound(Si)){
		 RSlist= PSmap.Find(Si);
	         for (TopTools_ListIteratorOfListOfShape RSit(RSlist); RSit.More(); RSit.Next())
		      if(RSit.Value().ShapeType()==TopAbs_FACE){
                             TopoDS_Shape F = RSit.Value(); 
                             F.Orientation(TopAbs_FORWARD);
			     if(   vol->type==WAVEGUIDE  && dielFaces.FindIndex(F)>0
			       ||  vol->type==BOUNDARYCOND
			       ||  vol->type==SPLITTER
			     ){
		               builder.Add(newShell,RSit.Value());
			        FNum++;
			     }
		       }
	       } else if(Si.ShapeType()==TopAbs_SOLID){
                             TopoDS_Shape F = Si; 
                             F.Orientation(TopAbs_FORWARD);
			     if(   vol->type==WAVEGUIDE  && dielFaces.FindIndex(F)>0
			       ||  vol->type==BOUNDARYCOND
			       ||  vol->type==SPLITTER
			     ){
		               builder.Add(newShell,Si);
			       FNum++;
			     }
	       } 
           } 
           if(SoNum>0) newS=newCompS;
           else if(FNum>0) newS=newShell;
       } else {
         if(PSmap.IsBound(S)) RSlist= PSmap.Find(S);
         if(!RSlist.IsEmpty()) newS=*(RSlist.begin());
         else RSlist.Append(S);
//Replace Shape with new shape:
         if(vol->type==DIELECTRIC || vol->type==BOUNDARYCOND ) {
           if(!RSlist.IsEmpty()) {
              TopoDS_CompSolid  newCompS;
              builder.MakeCompSolid(newCompS);
	      for (TopTools_ListIteratorOfListOfShape RSit(RSlist); RSit.More(); RSit.Next()){
                  TopoDS_Shape Si=RSit.Value();
	  	  if(Si.ShapeType()==TopAbs_COMPOUND || Si.ShapeType()==TopAbs_COMPSOLID){
                      for (TopExp_Explorer exp(Si,TopAbs_SOLID); exp.More(); exp.Next()){ 
			 TopoDS_Shape Sii=exp.Current();
		          if(CHECK_CREATED_SHAPES){
			    BRepCheck_Analyzer ana (Sii, true);
			    assert(ana.IsValid());
			  }
			  if(insertedSolids.FindIndex(Sii)<1)
			  {
			     insertedSolids.Add(Sii);
			     builder.Add(newCompS,Sii);
			     SoNum++;
			  }
		      }
		  } else if(Si.ShapeType()==TopAbs_SOLID){
		      if(CHECK_CREATED_SHAPES){
			 BRepCheck_Analyzer ana (Si, true);
			 assert(ana.IsValid());
		      }
		      if(insertedSolids.FindIndex(Si)<1)
		      {
			  insertedSolids.Add(Si);
			  builder.Add(newCompS,Si); 
			  SoNum++;
		      }
		  }
               }
	       if(SoNum>0) newS=newCompS;
             }
           }
           if(vol->type==WAVEGUIDE || vol->type==SPLITTER  || vol->type==BOUNDARYCOND && SoNum==0) {
             if(!RSlist.IsEmpty()) {
                 BRep_Builder builder;
                 builder.MakeShell(newShell);
	         for (TopTools_ListIteratorOfListOfShape RSit(RSlist); RSit.More(); RSit.Next()){
                    TopoDS_Shape Si=RSit.Value();
	  	    if(Si.ShapeType()==TopAbs_COMPOUND || Si.ShapeType()==TopAbs_SHELL)
                      for (TopExp_Explorer exp(Si,TopAbs_FACE); exp.More(); exp.Next()){
                          TopoDS_Shape F = exp.Current(); 
                          F.Orientation(TopAbs_FORWARD);
			  if(   vol->type==WAVEGUIDE  && dielFaces.FindIndex(F)>0
			    ||  vol->type==BOUNDARYCOND
			    || vol->type==SPLITTER
			  ){
			    builder.Add(newShell,exp.Current());
			    FNum++;
			   }
		     }
		     else if(Si.ShapeType()==TopAbs_FACE){
                          TopoDS_Shape F = Si; 
                          F.Orientation(TopAbs_FORWARD);
			  if(  vol->type==WAVEGUIDE  && dielFaces.FindIndex(F)>0
			    || vol->type==BOUNDARYCOND
			    || vol->type==SPLITTER
			  ){
			    builder.Add(newShell,Si);
			    FNum++;
			   }
		      }
                 }
	       if(FNum>0) newS=newShell;
             }
           }
       }
       if(!newS.IsNull()){ //Replace Shape with new shape:
	  TDF_Label newlabel=replaceLabelShape(label1,newS);
	  displayLabelShape(newlabel, true);
          if(vol->type==DIELECTRIC) {
               for (TopExp_Explorer exp(newS,TopAbs_FACE); exp.More(); exp.Next()){
                      TopoDS_Shape F = exp.Current(); 
                      F.Orientation(TopAbs_FORWARD);
		      dielFaces.Add(F);
               }
           }
       } 
    }
  }

} //skip all imprinting:

//  heal();
  
  if(!hasUPIF){
     makeTheCompSolid();
     makeSolid(theCompSolid,theMergedSolid);
     theIF=theMergedSolid;
  }

   resetShapeTool();
   generateSubshapes(false);
   newIndexedSubShapes();
   makeIF_IndexedSubShapes();
   makePartsIndexedSubShapes();
   setGlobalNames();
  
}


void MwOCAF::updatePartColors(){
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
   TDF_Label label1 = it.Value();
   DB::Volume *vol=getLabelVol(label1);
   if(vol) if(vol->type==DIELECTRIC || vol->type==HOLE || vol->type==BOUNDARYCOND || vol->PML || vol->type==WAVEGUIDE || vol->type==SPLITTER ){
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
     TDF_Label label = refAtt->Get();
     Handle(TPrsStd_AISPresentation)  prs; 
     if(!label.FindAttribute(TPrsStd_AISPresentation::GetID(),prs)) prs=setLabelPresentation(label);
     bool B=prs->IsDisplayed();
     if(vol->type==WAVEGUIDE) changeColor(label,Quantity_NOC_RED);
     else if(vol->type==SPLITTER){
	     changeColor(label,Quantity_NOC_SEAGREEN);
             prs->SetTransparency(0.5);
     } else   assignMatColor(label);
     displayLabelShape(label, B);
   }
 }
}

void addToParts(Handle(XCAFDoc_ShapeTool) shapeTool, const TDF_Label &label){

   TDF_Label theParts = XCAFDoc_DocumentTool::DocLabel(label).FindChild(6,Standard_True);
   TDF_TagSource aTag;
   TDF_Label label1 = aTag.NewChild(theParts);

   Handle(TDF_Reference)  refAtt=TDF_Reference::Set(label1,label);
   TopLoc_Location loc=shapeTool->GetLocation(label);
   Handle(XCAFDoc_Location) locAtt=XCAFDoc_Location::Set(label1,loc);
   Handle(TDataStd_Name)  nameAtt;
   if(label.FindAttribute(TDataStd_Name::GetID(),nameAtt)){
      Handle(TDataStd_Name) nameAtt1=TDataStd_Name::Set(label1,nameAtt->Get());
   }
}

TopoDS_Shape MwOCAF::makeSymmBox(int XYplaneSymmetry, int YZplaneSymmetry, int ZXplaneSymmetry){
  double BB[2][3];
  BBox.Get(BB[0][0], BB[0][1], BB[0][2], BB[1][0], BB[1][1], BB[1][2]);
  if(XYplaneSymmetry) BB[0][2]=(BB[0][2]+BB[1][2])/2;
  if(YZplaneSymmetry) BB[0][0]=(BB[0][0]+BB[1][0])/2;
  if(ZXplaneSymmetry) BB[0][1]=(BB[0][1]+BB[1][1])/2;
  gp_Pnt P[2];
  for (int i=0; i<2; i++) P[i]= gp_Pnt(BB[i][0],BB[i][1],BB[i][2]);
  TopoDS_Shape symmBox =BRepPrimAPI_MakeBox(P[0], P[1]);
  return symmBox;
}

void MwOCAF::makeSymmPlane(int id, int itype){
  double BB1[2][3];
  BBox.Get(BB1[0][0], BB1[0][1], BB1[0][2], BB1[1][0], BB1[1][1], BB1[1][2]);
  double BB[3][2];
  for (int i=0; i<2; i++) for (int j=0; j<3; j++) BB[i][j]=BB1[i][(j+id)%3];
  double PP[4][3];
  PP[0][0]=BB[0][0]; PP[0][1]=BB[0][1]; PP[0][2]=0;
  PP[1][0]=BB[1][0]; PP[1][1]=BB[0][1]; PP[1][2]=0;
  PP[2][0]=BB[1][0]; PP[2][1]=BB[1][1]; PP[2][2]=0;
  PP[3][0]=BB[0][0]; PP[3][1]=BB[1][1]; PP[3][2]=0;
  gp_Pnt P[4];
  for (int i=0; i<4; i++) P[i]= gp_Pnt(PP[i][(3-id)%3],PP[i][(3-id+1)%3],PP[i][(3-id+2)%3]);
  TopoDS_Edge e1=BRepBuilderAPI_MakeEdge (P[0],P[1]);
  TopoDS_Edge e2=BRepBuilderAPI_MakeEdge (P[1],P[2]);
  TopoDS_Edge e3=BRepBuilderAPI_MakeEdge (P[2],P[3]);
  TopoDS_Edge e4=BRepBuilderAPI_MakeEdge (P[3],P[0]);
  TopoDS_Wire w = BRepBuilderAPI_MakeWire(e1,e2,e3,e4);
  TopoDS_Face F=BRepBuilderAPI_MakeFace(w);

  double vec[3]={0,0,0}; vec[id-1]=(BB1[0][id-1]+BB1[1][id-1])/2;
  gp_Vec trasl(vec[0],vec[1],vec[2]);  
  gp_Trsf trsf;
  trsf.SetTranslation(trasl);
  BRepBuilderAPI_Transform aTransform(trsf);
  Standard_Boolean toCopy = Standard_True;  // share entities if possible
  aTransform.Perform(F, toCopy);
  F=TopoDS::Face(aTransform.Shape());

  TDF_LabelSequence FreeLabels;
  shapeTool->GetFreeShapes(FreeLabels);
  assert(FreeLabels.Length()>0);
  TDF_Label ass=FreeLabels.Value(1);
  TDF_Label newlabel=shapeTool->AddComponent(ass,F);
	          
  Handle(TDataStd_Name) nameAtt;
  bool hasNameAtt=newlabel.FindAttribute(TDataStd_Name::GetID(),nameAtt);
  TCollection_AsciiString name;
  switch(itype){
    case 3: name="BC_symmetryPlaneXY";
	    break;
    case 1: name="BC_symmetryPlaneYZ";
	    break;
    case 2: name="BC_symmetryPlaneZX";
	    break;
  }
  nameAtt=TDataStd_Name::Set(newlabel,name);
  Handle(TPrsStd_AISPresentation) prs=setLabelPresentation(newlabel,0.8);
  addToParts(shapeTool, newlabel);
  DB::Volume *vol = new DB::Volume();
  vol->type=BOUNDARYCOND;
  strcpy(vol->name,name.ToCString());
  switch(itype){
    case 1: 
	  strcpy(vol->material, "PEC");
          changeColor(newlabel,Quantity_NOC_BLUE);
	  break;
    case 2: 
	  strcpy(vol->material, "PMC");
          changeColor(newlabel,Quantity_NOC_BROWN);
	  break;
  }
  EmP->insertVolume(vol);
  displayLabelShape(newlabel, true);
  shapeTool->UpdateAssemblies();
}


void MwOCAF::evalSize(double size[3]){
//Compute the Bounding Box
  TopoDS_Shape S;
  if(!shapeTool->GetShape(theShapes,S)) return;
  Bnd_Box B;
  BRepBndLib::Add(S, B);
  double BBX[2],BBY[2],BBZ[2];
  B.Get(BBX[0], BBY[0], BBZ[0], BBX[1], BBY[1], BBZ[1]);
  size[0]=BBX[1]-BBX[0];
  size[1]=BBY[1]-BBY[0];
  size[2]=BBZ[1]-BBX[0];
}

void MwOCAF::evalBBox(){
//Compute the Bounding Box
  BBox.SetVoid();
  if(theMergedSolid.IsNull()){
      makeTheCompSolid();
      makeSolid(theCompSolid,theMergedSolid);
  }
  BRepBndLib::Add(theMergedSolid, BBox);
}

void MwOCAF::makeSplitFaces(){
 evalBBox();
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
   TDF_Label label1 = it.Value();
   DB::Volume *vol=getLabelVol(label1);
   if(vol){
//	 if(vol->type==SPLITTER)  makeSplitFaces(label1);
         if(vol->type==GRID) makeGridFaces(label1);
   }
 }
}

/*
bool MwOCAF::updateSplitPlanes(){
 evalBBox();
 bool changed=false;
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
   TDF_Label label1 = it.Value();
   TDF_Label label = getRefLabel(label1);
   if(label.IsNull()) label=label1;
   DB::Volume *vol=getLabelVol(label1);
   if(vol) if(vol->type==GRID) {
     TopoDS_Shape S1= shapeTool->GetShape(label);
     TopTools_IndexedMapOfShape edges;
     TopExp::MapShapes(S1,TopAbs_EDGE,edges);
     if (edges.Extent()>1) continue;
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
     changed= changed||makeGridFaces(label1);
   }
 }
 return changed;
}
*/

void MwOCAF::makeSplitFaces(TDF_Label label1){
     double BBX[2],BBY[2],BBZ[2];
     BBox.Get(BBX[0], BBY[0], BBZ[0], BBX[1], BBY[1], BBZ[1]);
     gp_Pnt CP = gp_Pnt((BBX[0]+BBX[1])/2,
		     (BBY[0]+BBY[1])/2,
		     (BBZ[0]+BBZ[1])/2
		    );
     DB::Volume *vol=getLabelVol(label1);
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;
     TopoDS_Shape S= shapeTool->GetShape(label);
     if(S.IsNull()) return;
     if(S.ShapeType()!=TopAbs_COMPOUND) return;
     TopTools_IndexedMapOfShape faces;
     TopExp::MapShapes(S,TopAbs_FACE,faces);
     if (faces.Extent()>0) return;
     BRep_Builder builder;
     TopoDS_Compound  newS;
     builder.MakeCompound(newS);
     TopoDS_Compound  profile;
     builder.MakeCompound(profile);
     TopTools_IndexedMapOfShape wires;
     TopExp::MapShapes(S,TopAbs_WIRE,wires);
     for (int WI=1; WI<=wires.Extent(); WI++){
       TopoDS_Wire W = TopoDS::Wire(wires.FindKey(WI));
       builder.Add(profile,W);
       builder.Add(newS,W);
     }
     TopTools_IndexedMapOfShape vertices;
     TopExp::MapShapes(S,TopAbs_VERTEX,vertices);
     int PNum=vertices.Extent();
     Handle(TColgp_HArray1OfPnt) points = new TColgp_HArray1OfPnt(1, PNum);
     for (int PI=1; PI<=PNum; PI++) {
               TopoDS_Vertex V = TopoDS::Vertex(vertices.FindKey(PI));
               points->SetValue(PI,BRep_Tool::Pnt(V));
     }
     Standard_Real Tol=1.e-10;
     GeomPlate_BuildAveragePlane Pbuilder=GeomPlate_BuildAveragePlane(points, PNum, Tol,1, 1);
     gp_Pln plane=Pbuilder.Plane()->Pln();
     gp_Ax1 axis=plane.Axis();
     gp_Vec Ndir=axis.Direction();
     double Zmax=-10^10;
     double Zmin= 10^10;
     for (int i =0; i <2; i++)
     for (int j =0; j <2; j++)
     for (int k =0; k <2; k++)
     {
       gp_Pnt P = gp_Pnt(BBX[i],BBY[j],BBZ[k]);
       gp_Vec vec=gp_Vec(CP, P);
       Zmin=min(Zmin,Ndir.Dot(vec));
       Zmax=max(Zmax,Ndir.Dot(vec));
     }
     Zmax*=1.2;
     Zmin*=1.2;
     gp_Vec Tvec1=Zmin*Ndir;
     gp_Vec Tvec2=Zmax*Ndir;
     gp_Pnt P1 =CP;
     P1.Translate(Tvec1);
     gp_Pnt P2 =CP;
     P2.Translate(Tvec2);
     TopoDS_Wire spine = BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge (P1,P2));
     TopoDS_Shape extrude = BRepOffsetAPI_MakePipe (spine, profile);
     builder.Add(newS,extrude);
     TDF_Label newlabel=replaceLabelShape(label1,newS);
     Handle(TPrsStd_AISPresentation) prs=setLabelPresentation(newlabel,0.5);
     changeColor(newlabel,Quantity_NOC_SEAGREEN);
     displayLabelShape(newlabel, true);
     S= shapeTool->GetShape(newlabel); assert(S.IsEqual(newS));
}

double BB_orthoR(double BBX[2],double BBY[2],double BBZ[2], gp_Pnt O, gp_Vec Z )
{
     double R=0;
     for (int i =0; i <2; i++)
     for (int j =0; j <2; j++)
     for (int k =0; k <2; k++)
     {
        gp_Pnt P = gp_Pnt(BBX[i],BBY[j],BBZ[k]);
        gp_Vec OP=gp_Vec(O, P);
        R=max(R,Z.CrossMagnitude(OP));
     }
     return R;
}
double BB_paraR(double BBX[2],double BBY[2],double BBZ[2], gp_Pnt O, gp_Vec Z )
{
     double R=0;
     for (int i =0; i <2; i++)
     for (int j =0; j <2; j++)
     for (int k =0; k <2; k++)
     {
        gp_Pnt P = gp_Pnt(BBX[i],BBY[j],BBZ[k]);
        gp_Vec OP=gp_Vec(O, P);
        R=max(R,fabs(Z.Dot(OP)));
     }
     return R;
}



bool MwOCAF::makeGridFaces(TDF_Label label1){
     double BBX[2],BBY[2],BBZ[2];
     BBox.Get(BBX[0], BBY[0], BBZ[0], BBX[1], BBY[1], BBZ[1]);
     gp_Pnt BOX_CP = gp_Pnt((BBX[0]+BBX[1])/2,
		     (BBY[0]+BBY[1])/2,
		     (BBZ[0]+BBZ[1])/2
		    );
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;
     DB::Volume *vol=getLabelVol(label);
     TopoDS_Shape S= shapeTool->GetShape(label);
     if(S.IsNull()) return false;
     TopTools_IndexedMapOfShape faces;
     TopExp::MapShapes(S,TopAbs_FACE,faces);
     if (faces.Extent()==vol->gridNum+1)  return false;
     bool hasSection=faces.Extent()==1;
     TopoDS_Face sectFace;
     double SectCurve_u=0;
     gp_Pnt SectCurve_P;
     bool hasSectCurve_P;
     TopTools_IndexedMapOfShape Fedges;
     if(hasSection){
	 sectFace=TopoDS::Face(faces.FindKey(1));
         TopExp::MapShapes(sectFace,TopAbs_EDGE,Fedges);
     }
     TopTools_IndexedMapOfShape edges;
     for (TopExp_Explorer exp(S,TopAbs_EDGE); exp.More(); exp.Next()){
       TopoDS_Shape E = exp.Current(); 
       if(Fedges.FindIndex(E) < 1) if(edges.FindIndex(E) < 1) edges.Add(E);
     }
     if(edges.Extent()<1) return false;
     bool hasLine=false;
     bool hasCircle=false;
     Handle(Geom_Circle) gCirc; 
     Handle(Geom_Line) gLine; 
     TopoDS_Edge Eline,Ecircle;
     double line_u1,line_u2, circle_u1,circle_u2;
     bool fullCircle;
     circle_u1=10^3;
     circle_u2=-10^3;
     for (int I=1; I<=edges.Extent(); I++){
      TopoDS_Edge E= TopoDS::Edge(edges.FindKey(I));
      double u1,u2;
      Handle(Geom_Curve) gc=BRep_Tool::Curve (E, u1, u2);
      if(gc->IsInstance (STANDARD_TYPE (Geom_Line))){
	   hasLine=true;
	   Eline=E;
           gLine =Handle(Geom_Line)::DownCast (gc);
	   line_u1=u1;
	   line_u2=u2;
      }
      if(gc->IsInstance (STANDARD_TYPE (Geom_Circle))){
	   hasCircle=true;
	   Ecircle=E;
           gCirc =Handle(Geom_Circle)::DownCast (gc);
	   //fullCircle is composed of two arcs
	   circle_u1=min(circle_u1,u1);
	   circle_u2=max(circle_u2,u2);
	   fullCircle=fabs(u2-u1-2*M_PI)<1.e-5;
      }
     }
     gp_Pnt Circ_O;
     gp_Dir Circ_Dir;
     if(hasCircle){
        Circ_Dir=gCirc->Circ().Axis().Direction();
        Circ_O=gCirc->Circ().Axis().Location();
     }
     if(hasSection){
	  Handle(Geom_Surface) gSection = BRep_Tool::Surface(sectFace);
          GeomAPI_IntCS aInterCS;
          if(hasCircle) aInterCS.Perform(gCirc,gSection);
	  else if(hasLine) aInterCS.Perform(gLine,gSection);
	  if(aInterCS.IsDone()){
             double faceU,faceV;
             aInterCS.Parameters(1,faceU,faceV,SectCurve_u);
	     hasSectCurve_P=aInterCS.NbPoints()>0;
	     if(hasSectCurve_P) SectCurve_P=aInterCS.Point(1);
	  }
     }

     bool cylGrid=hasLine && hasCircle;
     vol->invariant=!cylGrid;
     int planeNum=vol->gridNum+1;
     TopoDS_Edge Esweep;
     if(cylGrid) Esweep=Eline;
     else{ Esweep=hasCircle ? Ecircle : Eline; }
     if(!cylGrid){
       DB::Transform *invt = new DB::Transform();
       strcpy(invt->name,vol->name);
       EmP->insertInvariant(invt);
       invt=EmP->FindInvariant(vol->name);
       if(hasCircle){
	 invt->rotAngle=(circle_u2-circle_u1)/(planeNum-1);
	 invt->rotAxis[0]=Circ_Dir.X();
	 invt->rotAxis[1]=Circ_Dir.Y();
	 invt->rotAxis[2]=Circ_Dir.Z();
	 invt->rotOrigin[0]=Circ_O.X();
	 invt->rotOrigin[1]=Circ_O.Y();
	 invt->rotOrigin[2]=Circ_O.Z();
       } else if(hasLine) {
         TopoDS_Vertex V1,V2;
         TopExp::Vertices(Eline,V1,V2);
         gp_Pnt P1 = BRep_Tool::Pnt(V1);
         gp_Pnt P2 = BRep_Tool::Pnt(V2);
         gp_Vec trasl=gp_Vec(P1, P2)/(planeNum-1);
	 invt->trasl[0]=trasl.X();
	 invt->trasl[1]=trasl.Y();
	 invt->trasl[2]=trasl.Z();
       }
     }
     BRepAdaptor_Curve C(TopoDS::Edge(Esweep));
     double Elen = GCPnts_AbscissaPoint::Length (C);
     BRep_Builder builder;
     TopoDS_Compound  newS;
     builder.MakeCompound(newS);
     builder.Add(newS,Esweep);
     double u1,u2;
     Handle(Geom_Curve) gec=BRep_Tool::Curve(Esweep, u1, u2);
     for (int VI=0; VI<planeNum; VI++){
        gp_Pnt P;
        gp_Vec Etan;
	gec->D1(u1+(u2-u1)*VI/(planeNum-1),P,Etan);
        Etan=Etan/Etan.Magnitude();
	if(cylGrid){
          gp_Pnt O=gCirc->Circ().Axis().Location();
	  gp_Dir Zdir=gCirc->Circ().Axis().Direction();
	  gp_Vec Zvec=Zdir;
          gp_Vec trasl=gp_Vec(O,BOX_CP);
          trasl=(Zvec.Dot(trasl))*Zvec;
          O.Translate(trasl);
	  gp_Vec OP=gp_Vec(O, P);
          double R=Zvec.CrossMagnitude(OP);
          double h=1.2*BB_paraR(BBX,BBY,BBZ, O, Zvec );
	  if(R<h/1.e6) continue; 
	  gp_Vec OP2=h*Zvec;
          gp_Pnt P2=O; P2.Translate(OP2);
	  gp_Vec OP1=-h*Zvec;
          gp_Pnt P1=O; P1.Translate(OP1);
          gp_Ax2 ax=gp_Ax2(O,Zdir);
          gp_Circ sec_c = gp_Circ(ax,R);
          TopoDS_Wire spine = BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge (P1,P2));
          TopoDS_Edge sec_cE=BRepBuilderAPI_MakeEdge(sec_c);
          TopoDS_Wire sec_cW=BRepBuilderAPI_MakeWire(sec_cE);
          TopoDS_Shape cyl = BRepOffsetAPI_MakePipe (spine, sec_cW);
          builder.Add(newS,cyl);
	}else if(hasCircle && hasSection){
	  if(fullCircle && VI==planeNum-1) continue;
	  gp_Trsf trsf;
          gp_Ax1 ax1=gp_Ax1(Circ_O,Circ_Dir);
	  double alpha=fullCircle? 2*M_PI*VI/(planeNum-1): circle_u1+(circle_u2-circle_u1)*VI/(planeNum-1)-SectCurve_u;
	  trsf.SetRotation(ax1, alpha);
          BRepBuilderAPI_Transform aTransform(trsf);
          Standard_Boolean toCopy = Standard_True;  // share entities if possible
          aTransform.Perform(sectFace, toCopy);
          builder.Add(newS,aTransform.Shape());
	}else if(hasSection && hasSectCurve_P){
          gp_Dir Edir=gp_Dir(Etan);
          gp_Vec trasl=gp_Vec(SectCurve_P, P);
          trasl=(Etan.Dot(trasl))*Etan;
	  gp_Trsf trsf;
	  trsf.SetTranslation(trasl);
          BRepBuilderAPI_Transform aTransform(trsf);
          Standard_Boolean toCopy = Standard_True;  // share entities if possible
          aTransform.Perform(sectFace, toCopy);
          builder.Add(newS,aTransform.Shape());
	}else if(!hasSection){
          gp_Dir Edir=gp_Dir(Etan);
          gp_Vec trasl=gp_Vec(BOX_CP, P);
          trasl=(Etan.Dot(trasl))*Etan;
          gp_Pnt O=BOX_CP; O.Translate(trasl);
          gp_Ax2 ax=gp_Ax2(O,Edir);
          double R=1.1*BB_orthoR(BBX,BBY,BBZ, O, Etan );
          gp_Circ sec_c = gp_Circ(ax,R);
          TopoDS_Edge sec_cE=BRepBuilderAPI_MakeEdge(sec_c);
          TopoDS_Wire sec_cW=BRepBuilderAPI_MakeWire(sec_cE);
          TopoDS_Face F=BRepBuilderAPI_MakeFace(sec_cW);
          builder.Add(newS,F);
	}
     }
     TDF_Label newlabel=replaceLabelShape(label1,newS);
     Quantity_Color col(Quantity_NOC_SEAGREEN);
     Handle(TPrsStd_AISPresentation) prs=setLabelPresentation(newlabel,0.5);
     changeColor(newlabel,Quantity_NOC_SEAGREEN);
     displayLabelShape(newlabel, false);
     S= shapeTool->GetShape(newlabel); assert(S.IsEqual(newS));
     return true;
}

/*
bool MwOCAF::makeGridFaces(TDF_Label label1){
     double BBX[2],BBY[2],BBZ[2];
     BBox.Get(BBX[0], BBY[0], BBZ[0], BBX[1], BBY[1], BBZ[1]);
     gp_Pnt CP = gp_Pnt((BBX[0]+BBX[1])/2,
		     (BBY[0]+BBY[1])/2,
		     (BBZ[0]+BBZ[1])/2
		    );
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) return false;
     TDF_Label label = refAtt->Get();
     DB::Volume *vol=getLabelVol(label1);
     TopoDS_Shape S= shapeTool->GetShape(label1);
     if(S.IsNull()) return false;
     double R=0;
     TopTools_IndexedMapOfShape edges;
     TopExp::MapShapes(S,TopAbs_EDGE,edges);
     assert(edges.Extent()==1);
     TopoDS_Edge E= TopoDS::Edge(edges.FindKey(1));
     TopTools_IndexedMapOfShape Evertices;
     TopExp::MapShapes(E,TopAbs_VERTEX,Evertices);
     int EVNum=Evertices.Extent();
     TopTools_IndexedMapOfShape splitVertices;
     {
       TopTools_IndexedMapOfShape tmp;
       tmp=Evertices;
       TopExp::MapShapes(S,TopAbs_VERTEX,tmp);
       for (int VI=EVNum+1; VI<=tmp.Extent(); VI++) splitVertices.Add(tmp.FindKey(VI));
     }
     TopoDS_Shape S0= shapeTool->GetShape(label);
     TopTools_IndexedMapOfShape faces0;
     TopExp::MapShapes(S0,TopAbs_FACE,faces0);
     if (faces0.Extent()==splitVertices.Extent())  return false;
     TopoDS_Vertex V1,V2;
     TopExp::Vertices(E,V1,V2);
     gp_Pnt P1 = BRep_Tool::Pnt(V1);
     gp_Pnt P2 = BRep_Tool::Pnt(V2);
     gp_Vec Evec=gp_Vec(P1, P2);
     Evec=Evec/Evec.Magnitude();
     for (int i =0; i <2; i++)
     for (int j =0; j <2; j++)
     for (int k =0; k <2; k++)
     {
        gp_Pnt P = gp_Pnt(BBX[i],BBY[j],BBZ[k]);
        gp_Vec vec=gp_Vec(CP, P);
        R=max(R,Evec.CrossMagnitude(vec));
     }
     R*=1.3;
     BRep_Builder builder;
     TopoDS_Compound  newS;
     builder.MakeCompound(newS);
     builder.Add(newS,E);
     for (int VI=1; VI<=splitVertices.Extent(); VI++){
       TopoDS_Vertex V = TopoDS::Vertex(splitVertices.FindKey(VI));
       gp_Pnt P = BRep_Tool::Pnt(V);
       gp_Vec vec=gp_Vec(CP, P);
       gp_Vec Tvec=(Evec.Dot(vec))*Evec;
       P =CP;
       P.Translate(Tvec);
       gp_Dir Edir=gp_Dir(Evec);
       gp_Ax2 ax=gp_Ax2(P,Edir);
       gp_Circ c = gp_Circ(ax,R);
       TopoDS_Edge cE=BRepBuilderAPI_MakeEdge(c);
       TopoDS_Wire cW=BRepBuilderAPI_MakeWire(cE);
       TopoDS_Face F=BRepBuilderAPI_MakeFace(cW);
       builder.Add(newS,V);
       builder.Add(newS,F);
     }
     TDF_Label newlabel=replaceLabelShape(label,newS);
     refAtt->Set(newlabel);
     Quantity_Color col(Quantity_NOC_SEAGREEN);
     setLabelPresentation(newlabel,Quantity_NOC_SEAGREEN, 0.5);
     displayLabelShape(newlabel, true);
     S= shapeTool->GetShape(newlabel); assert(S.IsEqual(newS));
     return true;
}
*/


gp_Vec MwOCAF::getEdgeDir(TDF_Label label1){
     DB::Volume *vol=getLabelVol(label1);
     TopoDS_Shape S= shapeTool->GetShape(label1);
     if(S.IsNull()) return gp_Vec(0,0,0);
     double R=0;
     TopTools_IndexedMapOfShape edges;
     TopExp::MapShapes(S,TopAbs_EDGE,edges);
     assert(edges.Extent()==1);
     TopoDS_Edge E= TopoDS::Edge(edges.FindKey(1));
     TopoDS_Vertex V1,V2;
     TopExp::Vertices(E,V1,V2);
     gp_Pnt P1 = BRep_Tool::Pnt(V1);
     gp_Pnt P2 = BRep_Tool::Pnt(V2);
     gp_Vec vec=gp_Vec(P1, P2);
     vec=vec/vec.Magnitude();
     return vec;
}

bool MwOCAF::hasSplitter(){
 if(!hasParts()) return false;
 bool has=false;
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
     TDF_Label label1 = it.Value();
     DB::Volume *vol=getLabelVol(label1);
     has=has || vol->type==SPLITTER || vol->type==GRID;
 }
 return has;
}

bool MwOCAF::hasSolidBC(){
 if(!hasParts()) return false;
 bool has=false;
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
     TDF_Label label1 = it.Value();
     DB::Volume *vol=getLabelVol(label1);
     if(vol->type==BOUNDARYCOND) {
       TDF_Label label = getRefLabel(label1);
       if(label.IsNull()) label=label1;
       TopoDS_Shape S=shapeTool->GetShape(label);
       has=has || S.ShapeType()==TopAbs_SOLID;
     }
 }
 return has;
}


bool MwOCAF::hasPML(){
 if(!hasParts()) return false;
 bool has=false;
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
     TDF_Label label1 = it.Value();
     DB::Volume *vol=getLabelVol(label1);
     has=has || vol->PML;
 }
 return has;
}


void MwOCAF::split(){
  assert(hasParts());

  std::string interfacesPath=projectDir+std::string("/interfaces");
  interfacesPath=nativePath(interfacesPath);
  if(!FileExists(interfacesPath.c_str()))  createdir(interfacesPath.c_str());
  std::string emPath=projectDir+std::string("/model.em");
  if(EmP->level==0) {
      FILE *fid= fopen(emPath.c_str(), "w");
      fprintf(fid, "Level  %d\n", EmP->level);
      fprintf(fid, "AssemblyType  %d\n", EmP->assemblyType);
      fprintf(fid, "DefaultBoundCond  \"%s\"\n", EmP->defaultBC);
      if(!EmP->stepFilePath.empty()) fprintf(fid, "\nImport  \"%s\"\n\n", EmP->stepFilePath.c_str());
      EmP->save(fid);
      fclose(fid);	
  }
  std::string statusPath=projectDir+std::string("/model.status");
	 
//----------------------------------------------------------------------------------
 if(hasSplitter()){

/*

// BRepLib::Precision(1.e-4);
 BOPAlgo_Builder PS;
 PS.SetFuzzyValue(1.e-5);
// PS.SetNonDestructive(true);

 TopTools_MapOfShape ShapesMap, ToolsMap;

  {
    for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
      TDF_Label label1 = it.Value();
      TDF_Label label = getRefLabel(label1);
      if(label.IsNull()) label=label1;
      DB::Volume *vol=getLabelVol(label1);
      if(!vol) continue;
      if(vol->type==GRID ) continue;
      Handle(TDataStd_Name)  nameAtt;
      if(label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)){
       TCollection_AsciiString name=nameAtt->Get();
      }
      TopoDS_Shape S=shapeTool->GetShape(label);
      assert(!S.IsNull());
      if(vol->type==DIELECTRIC || vol->type==HOLE) assert(S.ShapeType()==TopAbs_SOLID || S.ShapeType()==TopAbs_COMPOUND || S.ShapeType()==TopAbs_COMPSOLID);
      if(vol->type==WAVEGUIDE) assert(S.ShapeType()==TopAbs_SHELL || S.ShapeType()==TopAbs_FACE  || S.ShapeType()==TopAbs_COMPOUND);
      if(!S.IsNull()) if (ShapesMap.Add(S)) {
	   if(vol->type==SPLITTER){
		BRepBuilderAPI_Sewing Sew;
                Sew.Add(S); 
                Sew.Perform();
                TopoDS_Shape newS=Sew.SewedShape();
                if(!newS.IsNull()){ //Replace Shape with new shape:
	          TDF_Label newlabel=replaceLabelShape(label1,newS);
	          displayLabelShape(newlabel, true);
		  S=newS;
		}
	   }
           PS.AddArgument(S);
      }
    }
  }
  TopTools_IndexedMapOfShape  upIndexedFaces(*indexedFaces);
  TopTools_IndexedMapOfShape  upIndexedEdges(*indexedEdges);
  TopTools_IndexedMapOfShape  upIndexedVertices(*indexedVertices);


  try {
     PS.Perform();
  } catch ( Standard_Failure ) {throw ImprintFailure();}
  if (PS.HasErrors()){
    return;
  }

  const TopTools_DataMapOfShapeListOfShape& PSmap=PS.Images();

  TopTools_MapOfShape dielectricFaces2;


//-----------------------------------------------------------------------------
//  Replace original parts with the splitted parts:
//First replaces DIELECRICs :
  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
  {
     TDF_Label label1 = it.Value();
//---
     Handle(TDataStd_Name)  nameAtt;
     TCollection_AsciiString name;
     if(label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) name=nameAtt->Get();
//---
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue;
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;
     TopoDS_Shape S=shapeTool->GetShape(label);
     TopoDS_Shape newS;
     if(vol->type==DIELECTRIC ) {
	 int SoNum=0;
         TopTools_ListOfShape RSlist;
	 if(PSmap.IsBound(S)) 
	     RSlist= PSmap.Find(S);
	 else 
	     RSlist.Append(S);
         if(!RSlist.IsEmpty()) {
              TopoDS_CompSolid  newCompS;
              BRep_Builder builder;
              builder.MakeCompSolid(newCompS);
//            builder.MakeCompound(newCompS);
	      for (TopTools_ListIteratorOfListOfShape RSit(RSlist); RSit.More(); RSit.Next()){
                  TopoDS_Shape Si=RSit.Value();
	  	  if(Si.ShapeType()==TopAbs_COMPOUND || Si.ShapeType()==TopAbs_COMPSOLID){
                      for (TopExp_Explorer exp(Si,TopAbs_SOLID); exp.More(); exp.Next()){ 
			 TopoDS_Shape Sii=exp.Current();
		          if(CHECK_CREATED_SHAPES){
			    BRepCheck_Analyzer ana (Sii, true);
			    assert(ana.IsValid());
			  }
		          builder.Add(newCompS,Sii); SoNum++;
		      }
		  } else if(Si.ShapeType()==TopAbs_SOLID){
		      if(CHECK_CREATED_SHAPES){
			 BRepCheck_Analyzer ana (Si, true);
			 assert(ana.IsValid());
		      }
		      builder.Add(newCompS,Si); SoNum++;
		  }
               }
	       if(SoNum>0) newS=newCompS;
         }
         if(!newS.IsNull()){ //Replace Shape with new shape:
	      TDF_Label newlabel=replaceLabelShape(label1,newS);
	      displayLabelShape(newlabel, true);
         } 
     }
  }

*/

// store dielFaces;
  TopTools_IndexedMapOfShape  dielFaces;
  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
  {
     TDF_Label label1 = it.Value();
//---
     Handle(TDataStd_Name)  nameAtt;
     if(label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)){
      TCollection_AsciiString name=nameAtt->Get();
     }
//---
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue;
     if(vol->type!=DIELECTRIC ) continue;
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;
     TopoDS_Shape S=shapeTool->GetShape(label);
     if(S.ShapeType()==TopAbs_SOLID || S.ShapeType()==TopAbs_COMPSOLID) for (TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
           TopoDS_Shape F = exp.Current(); 
           F.Orientation(TopAbs_FORWARD);
           if(dielFaces.FindIndex(F) < 1) dielFaces.Add(F);
     }
   }

/*

//Than it replaces for WAVEGUIDEs BOUNDARYCONDs, SPLITTERs, and LINEPORTS:
  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
  {
     TDF_Label label1 = it.Value();
//---
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue;
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;
     TopoDS_Shape S=shapeTool->GetShape(label);
     if(vol->type==WAVEGUIDE || vol->type==BOUNDARYCOND || vol->type==SPLITTER || vol->type==LINEPORT) if(PSmap.IsBound(S)){
       const TopTools_ListOfShape &RSlist = PSmap.Find(S);
       if(!RSlist.IsEmpty()) {
	     TopoDS_Shape newS;
             if(vol->type==WAVEGUIDE || vol->type==BOUNDARYCOND || vol->type==SPLITTER){
	         int useShell=S.ShapeType()==TopAbs_FACE || S.ShapeType()==TopAbs_SHELL;
                 TopoDS_Shell  newShell;
                 TopoDS_Compound  newComp;
                 BRep_Builder builder;
	         if(useShell) builder.MakeShell(newShell);
	         else         builder.MakeCompound(newComp);
	         for (TopTools_ListIteratorOfListOfShape RSit(RSlist); RSit.More(); RSit.Next()){
	              if(RSit.Value().ShapeType()==TopAbs_FACE){
                           TopoDS_Shape F=RSit.Value();
                           F.Orientation(TopAbs_FORWARD);
                           if(dielFaces.FindIndex(F) > 0){
				   if(useShell) builder.Add(newShell,TopoDS::Face(RSit.Value()));
				   else         builder.Add(newComp, TopoDS::Face(RSit.Value()));
			   }
		      } else{
                           TopoDS_Shape Si=RSit.Value();
                           for (TopExp_Explorer exp(Si,TopAbs_FACE); exp.More(); exp.Next()){
                               TopoDS_Shape F = exp.Current(); 
                               F.Orientation(TopAbs_FORWARD);
                               if(dielFaces.FindIndex(F) > 0){
			          if(useShell) builder.Add(newShell,TopoDS::Face(exp.Current()));
				  else         builder.Add(newComp, TopoDS::Face(exp.Current()));
			       }
			   }
		      }
                 }
		 if(useShell) newS=newShell;
		 else         newS=newComp;                    
             } else if(vol->type==LINEPORT){
                 TopoDS_Compound  newComp;
                 BRep_Builder builder;
                 builder.MakeCompound(newComp);
	         for (TopTools_ListIteratorOfListOfShape RSit(RSlist); RSit.More(); RSit.Next()){
	              if(RSit.Value().ShapeType()==TopAbs_EDGE) builder.Add(newComp,RSit.Value());
		      else if(RSit.Value().ShapeType()==TopAbs_COMPOUND)
                           for (TopExp_Explorer exp(RSit.Value(),TopAbs_EDGE); exp.More(); exp.Next()) builder.Add(newComp,exp.Current());
                 }
	         newS =newComp;
             }
//Replace Shape with new shape:
             if(!newS.IsNull()){ 
	        TDF_Label newlabel=replaceLabelShape(label1,newS);
	        displayLabelShape(newlabel, true);
             } 
        }
     }
  }
  
*/

// store splitFaces;
  TopTools_IndexedMapOfShape  splitFaces;
  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
  {
     TDF_Label label1 = it.Value();
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;

//---
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue;
     if(vol->type==SPLITTER ){
       TopoDS_Shape S = shapeTool->GetShape(label);
       if(S.ShapeType()==TopAbs_FACE){
           TopoDS_Shape F=S;
           F.Orientation(TopAbs_FORWARD);
           if(splitFaces.FindIndex(F) < 1) splitFaces.Add(F);
       }
       if(S.ShapeType()==TopAbs_SHELL || S.ShapeType()==TopAbs_COMPOUND) for (TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
           TopoDS_Shape F = exp.Current(); 
           F.Orientation(TopAbs_FORWARD);
           if(splitFaces.FindIndex(F) < 1) splitFaces.Add(F);
       }
     }
   }

// store BCFaces;
  TopTools_IndexedMapOfShape  BCFaces;
  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
  {
     TDF_Label label1 = it.Value();
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;

     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue;
     if(vol->type==BOUNDARYCOND ){
       TopoDS_Shape S = shapeTool->GetShape(label);
       if(S.ShapeType()==TopAbs_FACE){
           TopoDS_Shape F=S;
           F.Orientation(TopAbs_FORWARD);
           if(BCFaces.FindIndex(F) < 1) BCFaces.Add(F);
       }
       if(S.ShapeType()==TopAbs_SHELL || S.ShapeType()==TopAbs_COMPOUND) for (TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
           TopoDS_Shape F = exp.Current(); 
           F.Orientation(TopAbs_FORWARD);
           if(BCFaces.FindIndex(F) < 1) BCFaces.Add(F);
       }
     }
   }
//----


/*

//  resetShapeTool();
//  Handle(ShapeBuild_ReShape) reshape;
//  heal(reshape);
//  checkWires();
  hasUPIF=false;
  makeIF(theIF);
//  makeTheCompSolid();
//  makeSolid(theCompSolid,theMergedSolid);
  resetShapeTool();
  generateSubshapes(false);
  newIndexedSubShapes();
  makeIF_IndexedSubShapes();
  makePartsIndexedSubShapes();
  setGlobalNames();

  int FNum=indexedFaces->Extent();
  splitFacesMap.resize(FNum,0);
  int upFNum=upIndexedFaces.Extent();
  int upENum=upIndexedEdges.Extent();
  int upPNum=upIndexedVertices.Extent();
  for (int upFI=1; upFI<=upFNum; upFI++){
      TopoDS_Shape F = upIndexedFaces.FindKey(upFI);
      if(PS.Images().IsBound(F)){
          const TopTools_ListOfShape& RSlist = PS.Images().Find(F);
          if(RSlist.Extent()>0) for (TopTools_ListIteratorOfListOfShape RSit(RSlist); RSit.More(); RSit.Next()){
//	       if (reshape->Status(RSit.Value(), Fi, Standard_True)>=0){
                 TopoDS_Shape Fi=RSit.Value();
	         Fi.Orientation(TopAbs_FORWARD);
	         if(indexedFaces->Contains(Fi) && !splitFaces.Contains(Fi)){
		   int FI = indexedFaces->FindIndex(Fi);
                   splitFacesMap[FI-1]=upFI;
	         }
//	       }
          }
      } else if(indexedFaces->Contains(F)){
		 int FI = indexedFaces->FindIndex(F);
                 splitFacesMap[FI-1]=upFI;
      }

  }
  int ENum=indexedEdges->Extent();
  splitEdgesMap.resize(ENum,0);
  for (int upEI=1; upEI<=upENum; upEI++){
      TopoDS_Shape E = upIndexedEdges.FindKey(upEI);
      if(PS.Images().IsBound(E)){
          const TopTools_ListOfShape& RSlist = PS.Images().Find(E);
	  if(RSlist.Extent()>0) for (TopTools_ListIteratorOfListOfShape RSit(RSlist); RSit.More(); RSit.Next()) 
//		if (reshape->Status(RSit.Value(), Ei, Standard_True)>=0)
	  {
	      TopoDS_Shape Ei=RSit.Value();
	      Ei.Orientation(TopAbs_FORWARD);
	      if(indexedEdges->Contains(Ei)){
		   int EI = indexedEdges->FindIndex(Ei);
                   splitEdgesMap[EI-1]=upEI;
	      }
          } 
      } else  if(indexedEdges->Contains(E)){
	      int EI = indexedEdges->FindIndex(E);
              splitEdgesMap[EI-1]=upEI;
      }
  }
  int PNum=indexedVertices->Extent();
  splitVerticesMap.resize(PNum,0);
  for (int upPI=1; upPI<=upPNum; upPI++){
      TopoDS_Shape P = upIndexedVertices.FindKey(upPI);
      if(PS.Images().IsBound(P)){
          const TopTools_ListOfShape& RSlist = PS.Images().Find(P);
	  if(RSlist.Extent()>0) for (TopTools_ListIteratorOfListOfShape RSit(RSlist); RSit.More(); RSit.Next())
//		  if (reshape->Status(RSit.Value(), Pi, Standard_True)>=0)
	  {
	      TopoDS_Shape Pi=RSit.Value();
	      Pi.Orientation(TopAbs_FORWARD);
	      if(indexedVertices->Contains(Pi)){
		   int PI = indexedVertices->FindIndex(Pi);
                   splitVerticesMap[PI-1]=upPI;
	      }
          }
      } else if(indexedVertices->Contains(P)){
              int PI = indexedVertices->FindIndex(P);
              splitVerticesMap[PI-1]=upPI;
      }

  }

*/

  int FNum=indexedFaces->Extent();
  int ENum=indexedEdges->Extent();
  int PNum=indexedVertices->Extent();

  int VNum=indexedSolids->Extent();
  TopTools_IndexedMapOfShape  noSplitFaces;
  for (int FI=1; FI<=FNum; FI++){
   TopoDS_Shape F =indexedFaces->FindKey(FI);
   if(splitFaces.FindIndex(F) < 1) if(BCFaces.FindIndex(F) < 1) noSplitFaces.Add(F);
  }


  int NSFNum=noSplitFaces.Extent();
  int noSplitfaceSolids[2*NSFNum];  for (int i=0; i<2*NSFNum; i++) noSplitfaceSolids[i]=0;
  for (int VI=1; VI<=VNum; VI++){
   TopoDS_Shape S =indexedSolids->FindKey(VI);
   for (TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
      TopoDS_Shape F = exp.Current(); 
      F.Orientation(TopAbs_FORWARD);
      int FI=noSplitFaces.FindIndex(F);
      if( FI>0){
	  if(!noSplitfaceSolids[2*(FI-1)]) noSplitfaceSolids[2*(FI-1)+0]=VI;
	  else                             noSplitfaceSolids[2*(FI-1)+1]=VI;
      }
    }
   }
   solidSubdomMap.resize(VNum);
   for (int VI=1; VI<=VNum; VI++) solidSubdomMap[VI-1]=VI;
   int changed=1;  
   while(changed){
      changed=0;  
      for (int FI=1; FI<=NSFNum; FI++) {
	  int VI1=noSplitfaceSolids[2*(FI-1)+0];
	  int VI2=noSplitfaceSolids[2*(FI-1)+1];
	  if(VI1>0 && VI2>0 && solidSubdomMap[VI1-1]!=solidSubdomMap[VI2-1]) {
		  changed=1; 
		  int Icomp=min(solidSubdomMap[VI1-1],solidSubdomMap[VI2-1]);
		  solidSubdomMap[VI1-1]=solidSubdomMap[VI2-1]=Icomp;
	  }
      }
   }
   std::map<int,int> renum;
   int compI=0;
   for (int VI=1; VI<=VNum; VI++) if(renum.find(solidSubdomMap[VI-1])==renum.end()) renum[solidSubdomMap[VI-1]]=++compI;
   subCompNum=compI;
   for (int VI=1; VI<=VNum; VI++) solidSubdomMap[VI-1]=renum[solidSubdomMap[VI-1]];

   splitFaceSubdomMap.resize(2*FNum,0);
   faceSubdomMap.resize(2*FNum,0);
   for (int VI=1; VI<=VNum; VI++){
    TopoDS_Shape S =indexedSolids->FindKey(VI);
    for (TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
      TopoDS_Shape F = exp.Current(); 
      int si=(F.Orientation()==TopAbs_FORWARD) ? 1 : 0; //first part is where face normal enters
      F.Orientation(TopAbs_FORWARD);
      int FI1=splitFaces.FindIndex(F);
      int FI=indexedFaces->FindIndex(F);
      faceSubdomMap[2*(FI-1)+si]=solidSubdomMap[VI-1];
      if( FI1>0) splitFaceSubdomMap[2*(FI-1)+si]=solidSubdomMap[VI-1];
     }
   }

    savePartitionMap();
    std::string asstypeFileName=projectDir; asstypeFileName+="/subcompNum";
    FILE *subcmpfid= fopen(nativePath(asstypeFileName).c_str(), "w");
    fprintf(subcmpfid, "%d\n", subCompNum);
    fclose(subcmpfid);
  } else{  //no splitter
    int FNum=indexedFaces->Extent();
    int ENum=indexedEdges->Extent();
    int PNum=indexedVertices->Extent();
    int VNum=indexedSolids->Extent();
    subCompNum=0;
    std::string asstypeFileName=projectDir; asstypeFileName+="/subcompNum";
    FILE *subcmpfid= fopen(nativePath(asstypeFileName).c_str(), "w");
    fprintf(subcmpfid, "%d\n", subCompNum);
    fclose(subcmpfid);
    splitFaceSubdomMap.resize(2*FNum,min(1,subCompNum));
    faceSubdomMap.resize(2*FNum,min(1,subCompNum));
    solidSubdomMap.resize(VNum,min(1,subCompNum));
    savePartitionMap();
    std::string occFilePath=projectDir+std::string("/model");
    if(USEXMLXCAF) occFilePath+=".xml";
    else           occFilePath+=".dxc";
  }
//-------------
//----------------------------------------------------------------------------------

  setFaceSubdom();

}


void MwOCAF::extractSubcomp(int subcompI){


  TopTools_IndexedMapOfShape  dielFaces,solidEdges;
  for (int VI=1; VI<=indexedSolids->Extent(); VI++) if(solidSubdomMap[VI-1]==subcompI) {
      TopoDS_Shape S = indexedSolids->FindKey(VI);
      for (TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
        TopoDS_Shape F = exp.Current(); 
        F.Orientation(TopAbs_FORWARD);
        if(dielFaces.FindIndex(F) < 1) dielFaces.Add(F);
      }
      for (TopExp_Explorer exp(S,TopAbs_EDGE); exp.More(); exp.Next()){
        TopoDS_Shape E = exp.Current(); 
        E.Orientation(TopAbs_FORWARD);
        if(solidEdges.FindIndex(E) < 1) solidEdges.Add(E);
      }
  }
  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
  {
     TDF_Label label1 = it.Value();
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;

//---
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue;
     TopoDS_Shape S = shapeTool->GetShape(label);
     TopoDS_Shape newS;
     if(vol->type==DIELECTRIC ){
       if(S.ShapeType()==TopAbs_SOLID){
	  int VI=indexedSolids->FindIndex(S);
	  if(solidSubdomMap[VI-1]==subcompI) newS=S;
       }else if(S.ShapeType()==TopAbs_COMPSOLID){
          TopoDS_CompSolid  newCompS;
          BRep_Builder builder;
          builder.MakeCompSolid(newCompS);
	  int soNum=0;
          for (TDF_ChildIterator it(label,Standard_False); it.More(); it.Next()) {
             TDF_Label label2 = it.Value();
             TopoDS_Shape Si = shapeTool->GetShape(label2);
	     int VI=indexedSolids->FindIndex(Si);
	     if(solidSubdomMap[VI-1]==subcompI) {builder.Add(newCompS,Si); soNum++;}
	  }
	  if(soNum>0) newS=newCompS;
        }
     }
     if(vol->type==BOUNDARYCOND || vol->type==WAVEGUIDE || vol->type==SPLITTER )
     if(S.ShapeType()==TopAbs_FACE || S.ShapeType()==TopAbs_SHELL || S.ShapeType()==TopAbs_COMPOUND  || S.ShapeType()==TopAbs_COMPSOLID){
                TopoDS_Compound  newComp;
                TopoDS_Shell  newShell;
                BRep_Builder builder;
		if(S.ShapeType()==TopAbs_COMPOUND  || S.ShapeType()==TopAbs_COMPSOLID) builder.MakeCompound(newComp);
	        else  builder.MakeShell(newShell);
		int voidComp=1;
		int voidShell=1;
		if(S.ShapeType()==TopAbs_FACE){
                     TopoDS_Shape F=S;
                     F.Orientation(TopAbs_FORWARD);
	             if(dielFaces.FindIndex(F)) {builder.Add(newShell,S); voidShell=0;}
	         }
		 else for(TopExp_Explorer exp(S, TopAbs_FACE); exp.More(); exp.Next()){
                     TopoDS_Shape F=exp.Current();
                     F.Orientation(TopAbs_FORWARD);
		     if(dielFaces.FindIndex(F)) 
			   if(S.ShapeType()==TopAbs_COMPOUND   || S.ShapeType()==TopAbs_COMPSOLID) {builder.Add(newComp,exp.Current()); voidComp=0;}
		           else                                                                    {builder.Add(newShell,exp.Current()); voidShell=0;}
                }
		if(!voidShell) newS=newShell;
		else if(!voidComp) newS=newComp;
     }
     if(vol->type==LINEPORT){
                 TopoDS_Compound  newComp;
		 int voidComp=1;
                 BRep_Builder builder;
                 builder.MakeCompound(newComp);
                 for (TopExp_Explorer exp(S,TopAbs_EDGE); exp.More(); exp.Next()){
                     TopoDS_Shape E=exp.Current();
                     E.Orientation(TopAbs_FORWARD);
		     if(solidEdges.FindIndex(E)) {builder.Add(newComp,exp.Current()); voidComp=0;}
	         }
	         if(!voidComp) newS=newComp;
     }
     if(!newS.IsNull()){ //Replace Shape with new shape:
	 TDF_Label newlabel=replaceLabelShape(label1,newS);
	 displayLabelShape(newlabel, true);
     } else {
         removeWithSubComponents(label);
         shapeTool->UpdateAssemblies();
     } 

  }

  TopTools_IndexedMapOfShape  oldIndexedSolids=*indexedSolids;
  TopTools_IndexedMapOfShape  oldIndexedFaces=*indexedFaces;
  TopTools_IndexedMapOfShape  oldIndexedEdges=*indexedEdges;
  TopTools_IndexedMapOfShape  oldIndexedVertices=*indexedVertices;

  hasUPIF=false;
  makeIF(theIF);
//  makeTheCompSolid();
//  makeSolid(theCompSolid,theMergedSolid);
  resetShapeTool();
  generateSubshapes(false);
  newIndexedSubShapes();
  makeIF_IndexedSubShapes();
  makePartsIndexedSubShapes();
  setGlobalNames();

  int FNum=indexedFaces->Extent();
  subSplitFacesMap.resize(FNum,0);
  int ENum=indexedEdges->Extent();
  subSplitEdgesMap.resize(ENum,0);
  int PNum=indexedVertices->Extent();
  subSplitVerticesMap.resize(PNum,0);

  for(int FI=1; FI <= FNum; FI++)     subSplitFacesMap[FI-1] =oldIndexedFaces.FindIndex(indexedFaces->FindKey(FI));
  for(int EI=1; EI <= ENum; EI++)     subSplitEdgesMap[EI-1] =oldIndexedEdges.FindIndex(indexedEdges->FindKey(EI));
  for(int PI=1; PI <= PNum; PI++)  subSplitVerticesMap[PI-1] =oldIndexedVertices.FindIndex(indexedVertices->FindKey(PI));

  TCollection_AsciiString assName; getAssName(assName);
  char tag[10]; sprintf(tag,"_SUB%d",subcompI); assName+=tag;
  setAssName(assName);
  
  subComp=subcompI;

}

void makeISubShapes(   TopTools_IndexedMapOfShape  *indexedFaces,
		       TopTools_IndexedMapOfShape  *indexedEdges,
		       TopTools_IndexedMapOfShape  *indexedVertices,
		       TopoDS_Shape S)
{
 for (TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
     TopoDS_Shape F = exp.Current(); 
     F.Orientation(TopAbs_FORWARD);
     if(indexedFaces->FindIndex(F) < 1) indexedFaces->Add(F);
 }
 for (TopExp_Explorer exp(S,TopAbs_EDGE); exp.More(); exp.Next()){
     TopoDS_Shape E = exp.Current();
     E.Orientation(TopAbs_FORWARD);
     if(indexedEdges->FindIndex(E) < 1) indexedEdges->Add(E);
 }
 for (TopExp_Explorer exp(S,TopAbs_VERTEX); exp.More(); exp.Next()){
     TopoDS_Shape V = exp.Current();
     V.Orientation(TopAbs_FORWARD);
     if(indexedVertices->FindIndex(V) < 1) indexedVertices->Add(V);
 }
}

void makeISubShapes(   TopTools_IndexedMapOfShape  *indexedFaces,
		       TopTools_IndexedMapOfShape  *indexedEdges,
		       TopTools_IndexedMapOfShape  *indexedVertices,
		       TopTools_IndexedMapOfShape &UPIFsubshapes)
{
 for (int I = 1; I <=UPIFsubshapes.Extent() ; I++){
    TopoDS_Shape S=UPIFsubshapes.FindKey(I);
    S.Orientation(TopAbs_FORWARD);
    if(S.ShapeType()==TopAbs_FACE){
      if(indexedFaces->FindIndex(S) < 1) indexedFaces->Add(S);
    }
    else if(S.ShapeType()==TopAbs_EDGE){
      if(indexedEdges->FindIndex(S) < 1) indexedEdges->Add(S);
    }
    else if(S.ShapeType()==TopAbs_VERTEX){
      if(indexedVertices->FindIndex(S) < 1) indexedVertices->Add(S);
    }
 }
}




TDF_LabelSequence*  MwOCAF::getEquivalents (const TDF_Label& label){
   if(!indexedShapes) return NULL;
   TopoDS_Shape S = shapeTool->GetShape(label);
   if(!S.IsNull()){ 
	 int I=indexedShapes->FindIndex(S); 
	 if(I) { 
	   return(shapeIndex2Labels[I-1]); 
	 }
	 return NULL;
   }
   return NULL;
}

void MwOCAF::makeIndexedShapes(const TDF_Label &label){
   TopoDS_Shape S = shapeTool->GetShape(label);
   if(!S.IsNull()) indexedShapes->Add(S);
   for (TDF_ChildIterator it1(label, Standard_True); it1.More(); it1.Next()){
       makeIndexedShapes(it1.Value());
   }
}
void MwOCAF::makeIndexedShapes(){
 if(indexedShapes) delete indexedShapes;
 indexedShapes=new TopTools_IndexedMapOfShape;
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
 {
     TDF_Label label = it.Value();
     TDF_Label label1 = getRefLabel(label);
     if(label1.IsNull()) label1=label;
     makeIndexedShapes(label1);
 }
}

void MwOCAF::newIndexedSubShapes()
{
 if(indexedSolids)   indexedSolids->Clear();
 else                indexedSolids=new TopTools_IndexedMapOfShape;
 if(indexedFaces)    indexedFaces->Clear();
 else                indexedFaces=new TopTools_IndexedMapOfShape;
 if(indexedEdges)    indexedEdges->Clear();
 else                indexedEdges=new TopTools_IndexedMapOfShape;
 if(indexedVertices) indexedVertices->Clear();
 else                indexedVertices=new TopTools_IndexedMapOfShape;
}

void MwOCAF::makeIF_IndexedSubShapes(){
   if(hasUPIF) makeISubShapes(indexedFaces,indexedEdges,indexedVertices,UPIFsubshapes);
   else        makeISubShapes(indexedFaces,indexedEdges,indexedVertices,theIF);
   extFaceNum=indexedFaces->Extent();
   extEdgeNum=indexedEdges->Extent();
   extVertexNum=indexedVertices->Extent();
   setData();
}

void MwOCAF::makePartsIndexedSubShapes()
{
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
 {
     TDF_Label label1 = it.Value();
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;

     bool skip=false;
     if(EmP->assemblyType==COMPONENT){
      DB::Volume *vol=getLabelVol(label1);
      if(vol) skip=(vol->type==WAVEGUIDE || vol->type==LINEPORT || vol->type==BOUNDARYCOND || vol->type==SPLITTER  || vol->type==GRID);
     }
     if(skip) continue;
     TopoDS_Shape Sh = shapeTool->GetShape(label);
     for (TopExp_Explorer exp(Sh,TopAbs_SOLID); exp.More(); exp.Next()){
       TopoDS_Solid S = TopoDS::Solid(exp.Current()); 
       if(indexedSolids->FindIndex(S) < 1) indexedSolids->Add(S);
     }
     for (TopExp_Explorer exp(Sh,TopAbs_FACE); exp.More(); exp.Next()){
        TopoDS_Face F = TopoDS::Face(exp.Current()); F.Orientation(TopAbs_FORWARD);
        if(indexedFaces->FindIndex(F) < 1) indexedFaces->Add(F);
     }
     for (TopExp_Explorer exp(Sh,TopAbs_EDGE); exp.More(); exp.Next()){
       TopoDS_Edge E = TopoDS::Edge(exp.Current()); E.Orientation(TopAbs_FORWARD);
       if(indexedEdges->FindIndex(E) < 1) indexedEdges->Add(E);
     }
     for (TopExp_Explorer exp(Sh,TopAbs_VERTEX); exp.More(); exp.Next()){
       TopoDS_Vertex V = TopoDS::Vertex(exp.Current()); V.Orientation(TopAbs_FORWARD);
       if(indexedVertices->FindIndex(V) < 1) indexedVertices->Add(V);
     }
 }
}

struct couple_cmp{
 bool operator()(const int *a, const int *b) const
 {
    for(int i = 0; i < 2; i++){
     if(a[i]<b[i]) return true;
     else if(b[i]<a[i]) return false;
    }
    return false;
 }
};


int labelNameIndex(const char* cstr){
 int cstrlen=strlen(cstr);
 int i; for(i=cstrlen-1; i>0&&isdigit(cstr[i]); ) i--;
 int n=0; if (i<cstrlen-1) sscanf (&cstr[i+1],"%d",&n);
 return n;
}

int labelIndex(TDF_Label label){
 Handle(TDataStd_Name)  nameAtt;
 if(!label.FindAttribute(TDataStd_Name::GetID(),nameAtt)) return 0;
 TCollection_AsciiString name=nameAtt->Get();
 const char* cstr=name.ToCString();
 return labelNameIndex(cstr);
};


void MwOCAF::renumIndexedSubShapes(TDF_Label label,
		                 std::map<int,int> &f_permu,
				 std::map<int,int> &e_permu,
				 std::map<int,int> &v_permu
				 )
{
  for (TDF_ChildIterator Fit(label,Standard_False); Fit.More(); Fit.Next()){ 
        TDF_Label Flabel = Fit.Value();
        int FI=labelIndex(Flabel);
	if(FI<1 ) continue;
        TopoDS_Shape F = shapeTool->GetShape(Flabel); F.Orientation(TopAbs_FORWARD);
	int FI0=indexedFaces->FindIndex(F); if(FI0>0) f_permu[FI]=FI0;
	for (TDF_ChildIterator Eit(Flabel,Standard_False); Eit.More(); Eit.Next()){ 
           TDF_Label Elabel = Eit.Value();
           int EI=labelIndex(Elabel);
	   if(EI<1 ) continue;
           TopoDS_Shape E =shapeTool->GetShape(Elabel); E.Orientation(TopAbs_FORWARD);
           int EI0=indexedEdges->FindIndex(E); if(EI0>0) e_permu[EI]=EI0;
	   for (TDF_ChildIterator Pit(Elabel,Standard_False); Pit.More(); Pit.Next()){ 
             TDF_Label Plabel = Pit.Value();
             int PI=labelIndex(Plabel);
	     if(PI<1 ) continue;
             TopoDS_Shape P = shapeTool->GetShape(Plabel); P.Orientation(TopAbs_FORWARD);
             int PI0=indexedVertices->FindIndex(P); if(PI0>0) v_permu[PI]=PI0;
           }
        }
  }
}

void completePermu(int N, std::map<int,int> &permu){
  std::set<int> Iset;
  for(int I = 1; I <= N; I++) Iset.insert(I);
  for(int I = 1; I <= N; I++) if(permu.find(I)!=permu.end()) Iset.erase(permu[I]);
  for(int I = 1; I <= N; I++) if(permu.find(I)==permu.end()) {permu[I]=*Iset.begin(); Iset.erase(permu[I]); }
}

void MwOCAF::regenerateIndexedSubShapes(){
 if(!indexedFaces){
      newIndexedSubShapes();
      makePartsIndexedSubShapes();
 }
 std::map<int,int> f_permu,e_permu,v_permu;
 assert(hasParts());
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
 {
    TDF_Label label1 = it.Value();
    bool skip=false;
    DB::Volume *vol=getLabelVol(label1);
    if(vol) skip=vol->type==SPLITTER || vol->type==GRID || vol->type==LINEPORT;
    if(skip) continue;
    TDF_Label label = getRefLabel(label1);
    if(label.IsNull()) label=label1;
    Handle(TDataStd_Name)  nameAtt;
    bool hasName =label.FindAttribute(TDataStd_Name::GetID(),nameAtt);
    if(!hasName) continue;
    TCollection_AsciiString name=nameAtt->Get();
    TopoDS_Shape S = shapeTool->GetShape(label);
    if(S.IsNull()) continue;
    if(S.ShapeType()==TopAbs_COMPSOLID){
          for (TDF_ChildIterator it2(label,Standard_False); it2.More(); it2.Next()){
              TDF_Label label2 = it2.Value();
              TopoDS_Shape S2 = shapeTool->GetShape(label2);
	      renumIndexedSubShapes(label2, f_permu,e_permu,v_permu);
	  }
    } else renumIndexedSubShapes(label, f_permu,e_permu,v_permu);
  }
  completePermu(indexedFaces->Extent(),    f_permu);
  completePermu(indexedEdges->Extent(),    e_permu);
  completePermu(indexedVertices->Extent(), v_permu);
  
  TopTools_IndexedMapOfShape  indexedFaces1, indexedEdges1, indexedVertices1;
  for(int FI = 1; FI <= indexedFaces->Extent(); FI++){
      assert(f_permu.find(FI)!=f_permu.end() );
      indexedFaces1.Add(indexedFaces->FindKey(f_permu[FI]));
  }
  for(int EI = 1; EI <= indexedEdges->Extent(); EI++){
      assert(e_permu.find(EI)!=e_permu.end() );
      indexedEdges1.Add(indexedEdges->FindKey(e_permu[EI]));
  }
  for(int PI = 1; PI <= indexedVertices->Extent(); PI++){
      assert(v_permu.find(PI)!=v_permu.end() );
      indexedVertices1.Add(indexedVertices->FindKey(v_permu[PI]));
  }
  indexedFaces->Clear();
  for(int FI = 1; FI <= indexedFaces1.Extent(); FI++) 
	  indexedFaces->Add(indexedFaces1.FindKey(FI));
  indexedEdges->Clear();
  for(int EI = 1; EI <= indexedEdges1.Extent(); EI++) 
	  indexedEdges->Add(indexedEdges1.FindKey(EI));
  indexedVertices->Clear();
  for(int PI = 1; PI <= indexedVertices1.Extent(); PI++) 
	  indexedVertices->Add(indexedVertices1.FindKey(PI));

}

int MwOCAF::partNameCompMap(const char* cstr){
 if(strncmp(cstr,"Vol",3)) return 0;
 int VI=labelNameIndex(cstr);
 if(solidSubdomMap.size()<VI) return 0;
 return solidSubdomMap[VI-1];
}

void MwOCAF::makeShapeIndex2Labels(const TDF_Label &label){
   if(!indexedShapes) return;
   TopoDS_Shape S = shapeTool->GetShape(label);
   if(!S.IsNull()) {
     int I=indexedShapes->FindIndex(S); 
     if(I) {
	 int i=I-1;
	 if (!shapeIndex2Labels[i]) shapeIndex2Labels[i]=new TDF_LabelSequence();
	 shapeIndex2Labels[i]->Append(label);
     }
   }
   for (TDF_ChildIterator it1(label, Standard_False); it1.More(); it1.Next()){
	makeShapeIndex2Labels(it1.Value());
   }
}
void MwOCAF::makeShapeIndex2Labels(){
   makeIndexedShapes();
   if(shapeIndex2Labels){
    for (int i=0; i< Imax; i++) if(shapeIndex2Labels[i]) delete shapeIndex2Labels[i];
    free(shapeIndex2Labels);
   } 
   Imax=indexedShapes->Extent();
   shapeIndex2Labels =(TDF_LabelSequence **) malloc(Imax*sizeof(TDF_LabelSequence *));
   for (int i=0; i< Imax; i++) shapeIndex2Labels[i]=NULL;
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
      TDF_Label label = it.Value();
      TDF_Label label1 = getRefLabel(label);
      if(label1.IsNull()) label1=label;
      makeShapeIndex2Labels(label1);
   }
}


TCollection_AsciiString remove_SUB(TCollection_AsciiString assName){
 TCollection_AsciiString result=assName;
 char name[assName.Length()+1]; strcpy(name,assName.ToCString());
 int l; for(l=strlen(name)-1; l>0 && isdigit(name[l]); ) l--;
 l-=3; if( l>=0 && !strncmp(&name[l],"_SUB",4)) {name[l]='\0'; result=TCollection_AsciiString(name);}
 return result;
}


void MwOCAF::makeFaceAdjCells(TDF_Label label){
    TopoDS_Shape S = shapeTool->GetShape(label);
    Handle(TDataStd_Name)  nameAtt;
    if(!label.FindAttribute(TDataStd_Name::GetID(),nameAtt)) return;
    TCollection_AsciiString volName=nameAtt->Get();
    DB::Volume *vol=getLabelVol(label);
    TCollection_AsciiString assName; getAssName(assName);
    TCollection_AsciiString partName;
    if(vol->type==WAVEGUIDE){
      partName=assName;
      if(!strncmp(volName.ToCString(),"WGP",3)) partName=remove_SUB(partName);
      partName+=TCollection_AsciiString("__")+volName ;
    } else  partName=volName;
    if(EmP->assemblyType==NET || vol->type==DIELECTRIC || vol->type==WAVEGUIDE){
       if(S.ShapeType()==TopAbs_COMPSOLID)
        for (TopExp_Explorer it1(S,TopAbs_SOLID); it1.More(); it1.Next()){
       TopoDS_Shape So = it1.Current();
       for (TopExp_Explorer it(So,TopAbs_FACE); it.More(); it.Next()){
            TopoDS_Shape F = it.Current();
            int si=(F.Orientation()==TopAbs_FORWARD) ? 1 : 0; //first part is where face normal enters
            F.Orientation(TopAbs_FORWARD);
            int FI=indexedFaces->FindIndex(F);
            if(FI){
		 assert(faceAdjParts[2*(FI-1)+si]==TCollection_AsciiString("-"));
		 faceAdjParts[2*(FI-1)+si]=partName;
            }
       }
       } else for (TopExp_Explorer it(S,TopAbs_FACE); it.More(); it.Next()){
            TopoDS_Shape F = it.Current();
            int si=(F.Orientation()==TopAbs_FORWARD) ? 1 : 0; //first part is where face normal enters
            F.Orientation(TopAbs_FORWARD);
            int FI=indexedFaces->FindIndex(F);
            if(FI){
		 if(S.ShapeType()==TopAbs_SOLID) assert(faceAdjParts[2*(FI-1)+si]==TCollection_AsciiString("-"));
		 else if(vol->type==WAVEGUIDE && faceAdjParts[2*(FI-1)+si]!=TCollection_AsciiString("-")) si=1-si;
		 assert(faceAdjParts[2*(FI-1)+si]==TCollection_AsciiString("-"));
		 faceAdjParts[2*(FI-1)+si]=partName;
            }
         }
      }
}

void MwOCAF::makeFaceAdjCells(){
   int FImax=indexedFaces->Extent();
   if(faceAdjParts)  delete [] faceAdjParts; 
   faceAdjParts=new TCollection_AsciiString[2*FImax]; 
   for (int i = 0; i < 2*FImax; i++) faceAdjParts[i]=TCollection_AsciiString("-");
// solids are processed first in order to define boundary orientation;
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
      TDF_Label label1 = it.Value();
      TDF_Label label = getRefLabel(label1);
      if(label.IsNull()) label=label1;
      Handle(TDataStd_Name)  nameAtt;
      if(!label.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
      DB::Volume *vol=getLabelVol(label);
      if(!vol) continue; 
      TopoDS_Shape S = shapeTool->GetShape(label);
      if(S.IsNull())  continue;
      if(S.ShapeType()==TopAbs_SOLID || S.ShapeType()==TopAbs_COMPSOLID || (vol->type==DIELECTRIC && S.ShapeType()==TopAbs_COMPOUND) ) makeFaceAdjCells(label);
   }
   if(EmP->assemblyType==NET) return;
// then waveguides are processed
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
      TDF_Label label1 = it.Value();
      TDF_Label label = getRefLabel(label1);
      if(label.IsNull()) label=label1;
      Handle(TDataStd_Name)  nameAtt;
      if(!label.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
      DB::Volume *vol=getLabelVol(label);
      if(!vol) continue; 
      TopoDS_Shape S = shapeTool->GetShape(label);
      if(S.IsNull())  continue;
      if(vol->type==WAVEGUIDE)  makeFaceAdjCells(label);
   }

}



void MwOCAF::readFEproperties(){
   using namespace boost::interprocess;

   int FNum=indexedFaces->Extent();
   int ENum=indexedEdges->Extent();
   int PNum=indexedVertices->Extent();
   int VNum=indexedSolids->Extent();
   if(!faceData) faceData=new FaceData[FNum];
   if(!edgeData) edgeData=new EdgeData[ENum];
   if(!vertexData) vertexData=new VertexData[PNum];

   for (int FI = 1; FI <= FNum; FI++)  {
	   faceData[FI-1].level=EmP->level;
   }
   for (int EI = 1; EI <= ENum; EI++)  {
	   edgeData[EI-1].level=EmP->level;
   }
   for (int PI = 1; PI <= PNum; PI++)  {
	   vertexData[PI-1].level=EmP->level;
   }


   for (int FI =1; FI <= FNum; FI++){ 
    int UFI=subComp? subSplitFacesMap[FI-1] : FI;
    if(UFI) {
        char tag[10]; sprintf(tag,"%d",UFI);
        std::string FPath;
	FPath=projectDir+std::string ("/interfaces/F")+tag;
        FPath=nativePath(FPath);
        FILE *fin=fopen(FPath.c_str(),"r");
        assert(fin);
    	assert(setLock(fin,"r")==0);
        faceData[FI-1].read(fin);
	assert(releaseLock(fin)==0);
        fclose(fin);
      }
   }
   for (int EI =1; EI <= ENum; EI++){
    int UEI=subComp? subSplitEdgesMap[EI-1] : EI;
    int UUEI=UEI;
    if(UEI) {
        char tag[10]; sprintf(tag,"%d",UEI);
        std::string EPath;
	EPath=projectDir+std::string ("/interfaces/E")+tag;
        EPath=nativePath(EPath);
        FILE *fin=fopen(EPath.c_str(),"r");
        assert(fin);
    	assert(setLock(fin,"r")==0);
	edgeData[EI-1].read(fin);
	assert(releaseLock(fin)==0);
        fclose(fin);
    }
   }
   for (int PI =1; PI <= PNum; PI++) {
    int UPI=subComp? subSplitVerticesMap[PI-1] : PI;
    if(UPI){ 
        char tag[10]; sprintf(tag,"%d",UPI);
        std::string PPath;
	PPath=projectDir+std::string ("/interfaces/P")+tag;
        PPath=nativePath(PPath);
        FILE *fin=fopen(PPath.c_str(),"r");
        assert(fin);
    	assert(setLock(fin,"r")==0);
	vertexData[PI-1].read(fin);
	assert(releaseLock(fin)==0);
        fclose(fin);
      }
    }


}



/*

void MwOCAF::storeSolidInDB(const TDF_Label &label){
   DB::Volume *vol = new DB::Volume();
   TCollection_AsciiString name;
   Handle(TColStd_HSequenceOfExtendedString) layers;
   if(getLabelName(label, name)) strcpy(vol->name,name.ToCString());
   if(getLabelLayers(label, layers)){
       TCollection_ExtendedString layer=layers->Value(1);
       int cstrlen= layer.LengthOfCString();
       char* cstr=(char *) malloc((cstrlen+1)*sizeof(char));
       layer.ToUTF8CString(cstr);
       strcpy(vol->material, cstr);
       free(cstr);
   }
   EmP->insertVolume(vol);
}
void MwOCAF::storeSolidsInDB(){
 if(thePartsLabels->Length()>0)
    for (int i = 1; i <= thePartsLabels->Length(); i++) storeSolidInDB(thePartsLabels->Value(i));
}
*/


extern XCAFDoc_DataMapOfShapeLabel multiBodyShapesMap;

void  getMultiBodies( 
		   Handle(XCAFDoc_ShapeTool) STool2, Handle(XCAFDoc_ColorTool) ColTool2, Handle(XCAFDoc_LayerTool) layTool2, 
		   TopoDS_Shape S2,
		   std::map<std::string, std::set<int> > &nameShapeI,
		   std::map<int, LabelAttributes> &iAttrs,
                   TopTools_IndexedMapOfShape  &iShapes
		 )
{

 
//---
   if(S2.IsNull()) return;
   TopoDS_Shape S0 = S2;
   TopLoc_Location loc;
   S0.Location (loc);
   if(multiBodyShapesMap.IsBound(S0)) {
      TDF_Label label2 =multiBodyShapesMap.Find(S0);
      LabelAttributes attr;
      copyLabelAttributes(attr, label2, ColTool2, layTool2);
      if(isValidMultibodyPartName(attr.name)){
            int I=iShapes.Add(S2);
            std::string str=attr.name.ToCString();
            nameShapeI[str].insert(I);
	    iAttrs[I]=attr;
	    return;
      }
   }
   for (TopoDS_Iterator it(S2); it.More(); it.Next())
	     getMultiBodies(STool2, ColTool2,layTool2, it.Value(), nameShapeI, iAttrs, iShapes);
   
}



void  getAssMultiBodies( 
		   Handle(XCAFDoc_ShapeTool) STool2, Handle(XCAFDoc_ColorTool) ColTool2, Handle(XCAFDoc_LayerTool) layTool2, 
		   TDF_Label label2,
		   std::map<std::string, std::set<int> > &nameShapeI,
		   std::map<int, LabelAttributes> &iAttrs,
                   TopTools_IndexedMapOfShape  &iShapes
		 )
{
     for (TDF_ChildIterator it(label2, Standard_False); it.More(); it.Next())  {
        TDF_Label sublabel2 = it.Value();
        TopoDS_Shape subS2 = STool2->GetShape(sublabel2);
        Handle(TDataStd_Name) sub2NameAtt;
        if(!sublabel2.FindAttribute(TDataStd_Name::GetID(),sub2NameAtt)) continue;
        TCollection_AsciiString sub2Name=sub2NameAtt->Get();
        LabelAttributes attr;
        copyLabelAttributes(attr, sublabel2, ColTool2, layTool2);
        if(isValidMultibodyPartName(sub2Name)){
           std::string str=sub2Name.ToCString();
	   if(nameShapeI.find(str)==nameShapeI.end()){
             int I=iShapes.Add(subS2);
             nameShapeI[str].insert(I);
	     iAttrs[I]=attr;
           }
	}
   }
}



void  copySubAss(
	           Handle(XCAFDoc_ShapeTool) STool1, Handle(XCAFDoc_ColorTool) ColTool1, Handle(XCAFDoc_LayerTool) layTool1, const TDF_Label &label1, 
		   Handle(XCAFDoc_ShapeTool) STool2, Handle(XCAFDoc_ColorTool) ColTool2, Handle(XCAFDoc_LayerTool) layTool2, const TDF_Label &label2,
	       	   TopLoc_Location assLoc, int assemblytype
		 )
{

   LabelAttributes attr;
   copyLabelAttributes(attr, label2, ColTool2, layTool2);

   TDF_Label label2ref=label2;
   if(STool2->IsReference(label2)) STool2->GetReferredShape(label2,label2ref);
   LabelAttributes rattr;
   copyLabelAttributes(rattr, label2ref, ColTool2, layTool2);
 

//---
//---
   TopoDS_Shape S2 = STool2->GetShape(label2); if(S2.IsNull()) return;
   TDF_Label sublabel1=label1;
/*
  If S2 is top level shape in the current document and current document not main document label2 holds a referred not located shape.
  In this case the actual location of S2 is stored in the upper document (in a sublabel of the theParts label).
*/
   if(assemblytype==NET || isCompName(attr.name)){
          TDF_TagSource aTag;
          sublabel1 = aTag.NewChild(label1);
	  TopoDS_Shape S1=S2;
          S1.Location (assLoc.Multiplied(S1.Location()));
	  STool1->SetShape(sublabel1,S1);
          if(!sublabel1.IsNull()){
	    pasteLabelAttributes(attr, sublabel1, ColTool1, layTool1);
	    STool1->UpdateAssemblies();
 	  }
   }
   if(assemblytype==NET) {
     TopLoc_Location loc2= STool2->GetLocation(label2);
     loc2=assLoc.Multiplied(loc2);
     for (TDF_ChildIterator it(label2ref, Standard_False); it.More(); it.Next()) {
        TDF_Label sublabel2 = it.Value();
        TopoDS_Shape subS2 = STool2->GetShape(sublabel2);
        Handle(TDataStd_Name) sub2NameAtt;
        if(!sublabel2.FindAttribute(TDataStd_Name::GetID(),sub2NameAtt)) continue;
        TCollection_AsciiString sub2Name=sub2NameAtt->Get();
        if(sub2Name.IsEmpty()){ 
          TDF_Label rsublabel2;
          Handle(TDataStd_Name) rsub2NameAtt;
	  if(STool2->GetReferredShape(sublabel2,rsublabel2)) 
	    if(rsublabel2.FindAttribute(TDataStd_Name::GetID(),rsub2NameAtt)){
             TCollection_AsciiString rsub2Name=rsub2NameAtt->Get();
             if(!(rsub2Name.IsEmpty())) sub2NameAtt->Set(rsub2Name);
	    }   
        }
        LabelAttributes subattr;
        copyLabelAttributes(subattr, sublabel2, ColTool2, layTool2);
	if((isCompName(subattr.name) || isNetName(subattr.name)) && !subS2.IsNull()){
	      TopoDS_Shape subS1=subS2;
              subS1.Location (loc2.Multiplied(subS2.Location()));
	      TDF_Label subsublabel1=STool1->AddComponent(sublabel1,subS1);
              pasteLabelAttributes(subattr, subsublabel1, ColTool1, layTool1);
              STool1->UpdateAssemblies();
	      addToParts(STool1, subsublabel1);
	}
     }
   } 
   if(assemblytype==COMPONENT){
            std::map<std::string, std::set<int> > nameShapeI;
            std::map<int, LabelAttributes> iAttrs;
            TopTools_IndexedMapOfShape  iShapes;
	    getMultiBodies(STool2, ColTool2,layTool2, S2, nameShapeI, iAttrs, iShapes);    
	    getAssMultiBodies(STool2, ColTool2,layTool2, label2ref, nameShapeI, iAttrs, iShapes);    
            typedef std::map<std::string, std::set<int> > ::iterator NameIt;
	    for (NameIt nit=nameShapeI.begin(); nit!= nameShapeI.end(); nit++){
	      TopoDS_Shape subS2;
              LabelAttributes subattr;
	      if((*nit).second.size()>1){ //shapes stored on several labels with identical names are put in a compound
                TopoDS_Compound  Cmp;  
                BRep_Builder builder;
	        builder.MakeCompound(Cmp);
	        for (std::set<int> ::iterator tit=(*nit).second.begin(); tit!= (*nit).second.end(); tit++){
	         int I=*tit;
                 TopoDS_Shape S = iShapes.FindKey(I);
	         subattr=iAttrs[I];
                 builder.Add(Cmp,S);
	        }
                subS2=Cmp;
	       } else {
	         std::set<int> ::iterator tit=(*nit).second.begin();
	         int I=*tit;
                 subS2 = iShapes.FindKey(I);
	         subattr=iAttrs[I];
	      }
	      TopoDS_Shape subS1=subS2;
              subS1.Location (assLoc.Multiplied(subS2.Location()));
	      TDF_Label subsublabel1=STool1->AddComponent(sublabel1,subS1);
              pasteLabelAttributes(subattr, subsublabel1, ColTool1, layTool1);
              STool1->UpdateAssemblies();
	      addToParts(STool1, subsublabel1);
            }	  
     }
}


void MwOCAF::generateSubshapes(const TDF_Label &label, Handle(XCAFDoc_ShapeTool) shtool, bool skipSPLITTER){
   TopoDS_Shape Sh = shtool->GetShape(label);
   Quantity_Color color;
   XCAFDoc_ColorType ctype=XCAFDoc_ColorGen;
   bool hasColor =colorTool->GetColor(label,ctype,color);
   if(!Sh.IsNull()&&label.NbChildren()==0){
         TopExp_Explorer exp1;
	 if(Sh.ShapeType()==TopAbs_COMPSOLID){
            for(exp1.Init(Sh, TopAbs_SOLID); exp1.More(); exp1.Next()){
              TopoDS_Shape S=exp1.Current();
	      TDF_TagSource aTag;
              TDF_Label label1 = aTag.NewChild(label);
	      shtool->SetShape(label1,S);
//              Quantity_Color col(Quantity_NOC_SALMON4);
//              colorTool->SetColor(label1, col, XCAFDoc_ColorGen);
	      if(hasColor)  colorTool->SetColor(label1,color,ctype);
	      generateSubshapes(label1,skipSPLITTER);
	    }
	 } else if(Sh.ShapeType()==TopAbs_SOLID || Sh.ShapeType()==TopAbs_SHELL|| Sh.ShapeType()==TopAbs_WIRE || Sh.ShapeType()==TopAbs_COMPOUND){
	    bool skip=false;
	    bool lp=false;
            DB::Volume *vol=getLabelVol(label);
            if(vol){ skip=vol->type==GRID || vol->type==SPLITTER && skipSPLITTER; lp=vol->type==LINEPORT;}
            if(!skip){
             if(lp) for(exp1.Init(Sh, TopAbs_EDGE); exp1.More(); exp1.Next()){
               TopoDS_Shape E=exp1.Current();
	       TDF_TagSource aTag;
               TDF_Label label1 = aTag.NewChild(label);
	       shtool->SetShape(label1,E);
	       generateSubshapes(label1,skipSPLITTER);
             }
	     else for(exp1.Init(Sh, TopAbs_FACE); exp1.More(); exp1.Next()){
               TopoDS_Shape F=exp1.Current();
	       TDF_TagSource aTag;
               TDF_Label label1 = aTag.NewChild(label);
	       shtool->SetShape(label1,F);
//              Quantity_Color col(Quantity_NOC_SALMON4);
//              colorTool->SetColor(label1, col, XCAFDoc_ColorGen);
	       if(hasColor)  colorTool->SetColor(label1,color,ctype);
	       generateSubshapes(label1,skipSPLITTER);
	     }
	    }
	 }else if(Sh.ShapeType()==TopAbs_FACE){
            for(exp1.Init(Sh, TopAbs_EDGE); exp1.More(); exp1.Next()){
               TopoDS_Shape E=exp1.Current();
	       TDF_TagSource aTag;
               TDF_Label label1 = aTag.NewChild(label);
	       shtool->SetShape(label1,E);
	       if(hasColor)  colorTool->SetColor(label1,color,ctype);
	       generateSubshapes(label1,skipSPLITTER);
            }
	 }else if(Sh.ShapeType()==TopAbs_EDGE){
            for(exp1.Init(Sh, TopAbs_VERTEX); exp1.More(); exp1.Next()){
               TopoDS_Shape V=exp1.Current();
	       TDF_TagSource aTag;
               TDF_Label label1 = aTag.NewChild(label);
	       shtool->SetShape(label1,V);
	       if(hasColor)  colorTool->SetColor(label1,color,ctype);
            }
         }
	 shtool->UpdateAssemblies();
   } else  for (TDF_ChildIterator it1(label, Standard_False); it1.More(); it1.Next()){
       TDF_Label label1 = it1.Value();
       generateSubshapes(label1, shtool,skipSPLITTER);
   }
}


void MwOCAF::generateSubshapes(const TDF_Label &label, bool skipSPLITTER){
 generateSubshapes(label, shapeTool,skipSPLITTER);
}


void MwOCAF::generateSubshapes(bool skipSPLITTER){
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
 {
     TDF_Label label = it.Value();
     TDF_Label label1 = getRefLabel(label);
     if(label1.IsNull()) label1=label;
     TCollection_AsciiString label1Name;
     Handle(TDataStd_Name)  label1NameAtt;
     if(label1.FindAttribute(TDataStd_Name::GetID(),label1NameAtt)) label1Name=label1NameAtt->Get();;
     generateSubshapes(label1,skipSPLITTER);
 }
}

void MwOCAF::checkPartNF(Handle(TDocStd_Document) doc){
 TDF_Label parts = XCAFDoc_DocumentTool::DocLabel(doc->Main()).FindChild(6,Standard_True);
 for (TDF_ChildIterator it(parts,Standard_False); it.More(); it.Next()) 
 {
     TDF_Label label1 = it.Value();
     Handle(TDataStd_Name)  nameAtt;
     if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
     TCollection_AsciiString partName=nameAtt->Get();
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;
     TopoDS_Shape S= shapeTool->GetShape(label);
     int NF=0; for(TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()) NF++;
     std::cout<< "part :"<< partName.ToCString() <<std::endl;
     std::cout<< "NF :" << NF <<std::endl;
 }
}





bool MwOCAF::openUPIF(const char* fileName)
{  
    UPIFsubshapes.Clear();
    hasUPIF=openShapes( fileName, UPIFsubshapes);
    return hasUPIF;
}


void MwOCAF::setData()
{
 int Iarray[]={extFaceNum,extEdgeNum,extVertexNum};
 setIntData(Iarray,3);
}

void MwOCAF::getData()
{
 if(theData.IsNull()) return;
 extFaceNum=intData(0);
 extEdgeNum=intData(1);
 extVertexNum=intData(2);
}

bool MwOCAF::newDoc()
{
  Handle(XCAFApp_Application) XCAFapp=XCAFApp_Application::GetApplication();
  if(USEXMLXCAF){
      XCAFapp->DefineFormat ("XmlXCAF", "Xml XCAF Document", "xml",
                        new XmlXCAFDrivers_DocumentRetrievalDriver, 
                        new XmlXCAFDrivers_DocumentStorageDriver ("Copyright: Open Cascade, 2001-2002"));
      XCAFapp->NewDocument("XmlXCAF",mainDoc);
  }else{
      XCAFapp->NewDocument("MDTV-XCAF",mainDoc);
  }
  return true;
}
bool MwOCAF::openDoc(const char* fileName)
{
    Handle(XCAFApp_Application) XCAFapp=XCAFApp_Application::GetApplication();
    if(USEXMLXCAF)  XCAFapp->DefineFormat ("XmlXCAF", "Xml XCAF Document", "xml",
                                           new XmlXCAFDrivers_DocumentRetrievalDriver, 
                                           new XmlXCAFDrivers_DocumentStorageDriver ("Copyright: Open Cascade, 2001-2002"));
    PCDM_ReaderStatus status;
    TCollection_ExtendedString Oname=TCollection_ExtendedString(nativePath(fileName).c_str());
    bool success = true;
    int I=XCAFapp->IsInSession(Oname);
    if(I>0) XCAFapp->GetDocument(I,mainDoc);
    else   { status = XCAFapp->Open(Oname, mainDoc); success = (status == PCDM_RS_OK); }
    assert(success);
    setDocTools();
    getData();
    return (success);
}

bool MwOCAF::closeDoc()
{
  if(mainDoc.IsNull()) return false;
  if(mainDoc->CanClose()!=CDM_CCS_OK) return false;
  Handle(XCAFApp_Application) XCAFapp=XCAFApp_Application::GetApplication();
  XCAFapp->Close(mainDoc);
  if(EmP) {delete EmP; EmP=NULL;}
  return true;
}


int loadModel(MwOCAF* ocaf, const char *fName, bool update=false);
int loadProblem(DB::EmProblem* EmP, const char *fName);

void saveProblem(DB::EmProblem *emp, std::string filePath)
{
   FILE *fid= fopen(nativePath(filePath).c_str(), "w");
   fprintf(fid, "Level  %d\n", emp->level);
   fprintf(fid, "AssemblyType  %d\n", emp->assemblyType);
   fprintf(fid, "DefaultBoundCond  \"%s\"\n", emp->defaultBC);
   if(!emp->stepFilePath.empty()) fprintf(fid, "\nImport  \"%s\"\n\n", emp->stepFilePath.c_str());
   emp->save(fid);
   fclose(fid);
}


void MwOCAF::loadGlobalMat(const char* wkprojpath, bool onlyIfModified){
 std::string matFileName=wkprojpath;
 for (int i = 0; i < EmP->level; ++i) matFileName+="/..";
 matFileName+="/Data/materials.mat";
 matFileName=nativePath(matFileName);
 bool matFileExists=FileExists(matFileName.c_str());
 if(!matFileExists) return;
 bool doit;
 if(onlyIfModified){
   std::string emFileName=wkprojpath; emFileName+="/model.em";
   emFileName=nativePath(emFileName);
   bool emFileExists=FileExists(emFileName.c_str());
   doit =emFileExists && file1NewerThanFile2(matFileName.c_str(),emFileName.c_str());
 } else doit=true;
 if(doit) loadModel(this, matFileName.c_str() );
}

void MwOCAF::workopen(const char* wkprojpath, int subcomp)
{
//        mutex.lock();
        if(EmP) delete EmP;
        EmP=new DB::EmProblem();
        char tag[10]; if(subcomp) sprintf(tag,"_%d",subcomp); else sprintf(tag,"");
        EmP->occFilePath=wkprojpath; EmP->occFilePath+="/model";  EmP->occFilePath+=tag;
        if(USEXMLXCAF) EmP->occFilePath+=".xml";
	else           EmP->occFilePath+=".dxc";
        projectDir=wkprojpath;
	if(!subcomp){
         std::string prjstatusFileName=projectDir+"/model.status";
         prjstatusFileName=nativePath(prjstatusFileName);
         prjStatus.read(prjstatusFileName.c_str());
	}
        std::string emFileName=projectDir; emFileName+="/model"; emFileName+=".em";
        emFileName=nativePath(emFileName);
	EmP->occFilePath=nativePath(EmP->occFilePath);
        if(FileExists(emFileName.c_str())) loadModel(this, emFileName.c_str(), false);
        if(FileExists(EmP->occFilePath.c_str())){
	   openDoc(EmP->occFilePath.c_str());
	   subComp=subcomp;
	   EmP->hasGeo=true;
	}
        std::string UPIFfileName=projectDir+".brep";
        UPIFfileName=nativePath(UPIFfileName);
	if(FileExists(UPIFfileName.c_str())) openUPIF(UPIFfileName.c_str());
	hasDownIF=false;
	if(EmP->assemblyType==NET) hasDownIF=openParts();
	if(EmP->assemblyType==NET) readPartsStatus();
	else if(EmP->assemblyType==COMPONENT &!subComp) setPartsStatus();
	std::string IFfileName=projectDir+"/model.brep";
        IFfileName=nativePath(IFfileName);
        hasIF=FileExists(IFfileName.c_str());
        loadGlobalMat(nativePath(wkprojpath).c_str(), true);
        if(EmP->assemblyType==COMPONENT) readPartitionMap();
        if(subcomp) readSubdomainMap();
}

int MwOCAF::read_prjstatus(){
   std::string prjstatusFileName=projectDir; prjstatusFileName+="/model.status";
   prjstatusFileName=nativePath(prjstatusFileName);
   if(FileExists(prjstatusFileName.c_str())){ prjStatus.read(prjstatusFileName.c_str()); return 1;}
   else return 0;
}

void MwOCAF::save_prjstatus(){
   std::string prjstatusFileName=projectDir; prjstatusFileName+="/model.status";
   prjstatusFileName=nativePath(prjstatusFileName);
   prjStatus.save(prjstatusFileName.c_str());
}

void MwOCAF::worksave()
{
   save();
   if(!subComp) save_prjstatus();
   std::string emFilePath=projectDir; emFilePath+="/model.em";
   saveProblem(EmP,emFilePath);
   std::string asstypeFileName=projectDir; asstypeFileName+="/assemblyType";
   FILE *assfid= fopen(nativePath(asstypeFileName).c_str(), "w");
   fprintf(assfid, "%d\n", EmP->assemblyType);
   fprintf(assfid, "%d\n", subCompNum);
   fclose(assfid);
   worksaveNeeded=false;
}

void FixShape(TopoDS_Shape &S)
{
	  TopoDS_Shape S0=S;
          try
          {
          Handle(ShapeFix_Shape) SFS = new ShapeFix_Shape;
          SFS->Init (S);
          SFS->SetMaxTolerance(1.e-6);
          SFS->FixWireTool()->FixRemovePCurveMode() =1;
          SFS->FixWireTool()->FixConnectedMode()=1,
          SFS->FixWireTool()->FixSmallMode() =1;
          SFS->FixFaceTool()->FixSmallAreaWireMode() =1;
          SFS->Perform();
          S=SFS->Shape();
/*
          Handle(ShapeFix_Wireframe) SFWF = new ShapeFix_Wireframe;
          SFWF->Load(S);
          SFWF->SetPrecision (Precision::Confusion());
          SFWF->SetMaxTolerance ( aSolidTol );
          
          SFWF->ModeDropSmallEdges() = Standard_True;
          SFWF->FixSmallEdges();
          SFWF->FixWireGaps();
          S=SFWF->Shape();
*/
          }
     catch(...)
          {
           S=S0;
          }
}


void MwOCAF::save()
{
    if(mainDoc.IsNull()) return;
    Handle(XCAFApp_Application) XCAFapp=XCAFApp_Application::GetApplication();
    XCAFapp->Save(mainDoc);
}


void MwOCAF::saveAs(const char* fileName)
{
    if(mainDoc.IsNull()) return;
    TCollection_ExtendedString Oname=TCollection_ExtendedString(nativePath(fileName).c_str());
    Handle(XCAFApp_Application) XCAFapp=XCAFApp_Application::GetApplication();
    XCAFapp->SaveAs(mainDoc,Oname);
}

/*
void MwOCAF::saveIF(char* fileName)
{
    if(IFDoc.IsNull())   return;
    TCollection_ExtendedString Oname=TCollection_ExtendedString(fileName);
    theApp->SaveAs(IFDoc,Oname);
}
*/



void MwOCAF::saveIF()
{
    std::string fileName=projectDir;
    fileName+=std::string("/model.brep");
    fileName=nativePath(fileName);
    std::string  tmpName=projectDir;
    tmpName+=std::string ("/tmp.brep");
    tmpName=nativePath(tmpName);
    saveShapeIfDiff( fileName.c_str(), theIF, tmpName.c_str(),true);
}



void MwOCAF::savePartitionMap(){
// save superfaces_face_lists:
     std::string splitFaceFileName=projectDir+std::string ("/partition.map");
     FILE *out=fopen(nativePath(splitFaceFileName).c_str(), "w");
     if(out){
      fprintf(out, "%d  faceNum \n", indexedFaces->Extent() );
      fprintf(out, "%d  solidNum \n", solidSubdomMap.size() );
      for (int VI =1; VI <= solidSubdomMap.size(); VI++) fprintf(out, "%d  %d\n", VI, solidSubdomMap[VI-1]);
      int SplitFNum=0;
      for (int FI =1; FI <= splitFaceSubdomMap.size()/2; FI++) if(splitFaceSubdomMap[2*(FI-1)+0] || splitFaceSubdomMap[2*(FI-1)+1]) SplitFNum++;
      fprintf(out, "%d  splitFaceNum \n", SplitFNum );
      for (int FI =1; FI <= splitFaceSubdomMap.size()/2; FI++) if(splitFaceSubdomMap[2*(FI-1)+0] || splitFaceSubdomMap[2*(FI-1)+1] )
	                                                         fprintf(out, "%d  %d  %d\n", FI, splitFaceSubdomMap[2*(FI-1)+0], splitFaceSubdomMap[2*(FI-1)+1] );
      fclose(out);
     }
}

void MwOCAF::readPartitionMap(){

   std::string splitFaceFileName=projectDir+std::string("/partition.map");
   FILE *mapf=fopen(nativePath(splitFaceFileName).c_str(), "r");
   if(!mapf) return;
   char cstr[100];
   int FNum, VNum, I, CompI;
   fscanf(mapf, "%d", &FNum);  fgets(cstr,100,mapf);
   if (fscanf(mapf, "%d", &VNum)!=EOF){
      fgets(cstr,100,mapf);
      if(VNum) solidSubdomMap.resize(VNum,1);
      else     solidSubdomMap.clear();
      subCompNum=0;
      for (int VI =1; VI <= VNum; VI++){ fscanf(mapf, "%d %d", &I, &CompI); solidSubdomMap[VI-1]=CompI; subCompNum=max(subCompNum,CompI); }
   }
   int SplitFNum;
   if (fscanf(mapf, "%d", &SplitFNum)!=EOF){
      fgets(cstr,100,mapf);
      if(SplitFNum) splitFaceSubdomMap.resize(2*FNum,0);
      int FI,CompI1,CompI2;
      for (int I =1; I <= SplitFNum; I++){ fscanf(mapf, "%d %d %d", &FI, &CompI1, &CompI2); splitFaceSubdomMap[2*(FI-1)+0]=CompI1; splitFaceSubdomMap[2*(FI-1)+1]=CompI2; }
   }
   fclose(mapf);
}

void MwOCAF::saveSubdomainMap(){
// save superfaces_face_lists:
     if(!subComp) return;
     char tag[10]; sprintf(tag,"_%d",subComp);
     std::string subSplitFaceFileName=projectDir+std::string ("/partition")+std::string(tag)+std::string(".map");
     FILE *out=fopen(nativePath(subSplitFaceFileName).c_str(), "w");
     if(out){
      fprintf(out, "%d  faceNum \n", subSplitFacesMap.size() );
      for (int FI =1; FI <= subSplitFacesMap.size(); FI++) fprintf(out, "%d  %d\n", FI, subSplitFacesMap[FI-1]);
      fprintf(out, "%d  edgeNum \n", subSplitEdgesMap.size() );
      for (int EI =1; EI <= subSplitEdgesMap.size(); EI++) fprintf(out, "%d  %d\n", EI, subSplitEdgesMap[EI-1]);
      fprintf(out, "%d  pointNum \n", subSplitVerticesMap.size() );
      for (int PI =1; PI <= subSplitVerticesMap.size(); PI++) fprintf(out, "%d  %d\n", PI, subSplitVerticesMap[PI-1]);

      fclose(out);
     }
}

void MwOCAF::readSubdomainMap(){

   if(EmP->assemblyType!=COMPONENT || !subComp) return;

   char tag[10]; sprintf(tag,"_%d",subComp);
   std::string splitFaceFileName=projectDir+std::string ("/partition")+std::string(tag)+std::string(".map");
   FILE *mapf=fopen(nativePath(splitFaceFileName).c_str(), "r");
   if(!mapf) return;
   char cstr[100];
   int FNum, ENum, PNum, VNum, I, UFI, UEI, UPI, CompI;
   fscanf(mapf, "%d", &FNum);  fgets(cstr,100,mapf); if(FNum) subSplitFacesMap.resize(FNum,0);
   for (int FI =1; FI <= FNum; FI++){ fscanf(mapf, "%d %d", &I, &UFI); subSplitFacesMap[FI-1]=UFI; }
   fscanf(mapf, "%d", &ENum);  fgets(cstr,100,mapf); if(ENum) subSplitEdgesMap.resize(ENum,0);
   for (int EI =1; EI <= ENum; EI++){ fscanf(mapf, "%d %d", &I, &UEI); subSplitEdgesMap[EI-1]=UEI; }
   fscanf(mapf, "%d", &PNum);  fgets(cstr,100,mapf); if(PNum) subSplitVerticesMap.resize(PNum,0);
   for (int PI =1; PI <= PNum; PI++){ fscanf(mapf, "%d %d", &I, &UPI); subSplitVerticesMap[PI-1]=UPI; }
   fclose(mapf);
   
}

void MwOCAF::saveSuperfaceSplitterMap(){
// save superfaces_face_lists:
     if(subComp) return;
     std::string fileName=projectDir+std::string ("/sfaceSplitter")+std::string(".map");
     FILE *out=fopen(nativePath(fileName).c_str(), "w");
     if(out){
      fprintf(out, "%d  sfaceNum \n", superfaceSplitterMap.size() );
      for (std::map<std::string,std::string>::const_iterator it=superfaceSplitterMap.begin(); it!= superfaceSplitterMap.end(); it++)
          fprintf(out, "%s  %s\n", it->first.c_str(), it->second.c_str());
      fclose(out);
     }
}

void MwOCAF::readSuperfaceSplitterMap(){
   if(EmP->assemblyType!=COMPONENT) return;
   std::string fileName=projectDir+std::string ("/sfaceSplitter")+std::string(".map");
   FILE *mapf=fopen(nativePath(fileName).c_str(), "r");
   if(!mapf) return;
   char cstr1[200],cstr2[200];
   int sFNum; fscanf(mapf, "%d", &sFNum);  fgets(cstr1,200,mapf);
   for (int i =0; i < sFNum; i++){
	fscanf(mapf, "%s %s", cstr1, cstr2 ); 
	superfaceSplitterMap[std::string(cstr1)]=std::string(cstr2);
   }
   fclose(mapf);
}


bool MwOCAF::openParts()
{
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
     TDF_Label label1 = it.Value();
     Handle(TDataStd_Name)  nameAtt;
     assert(label1.FindAttribute(TDataStd_Name::GetID(),nameAtt));
     TCollection_AsciiString partName=nameAtt->Get();
     std::string fileName=projectDir;
     fileName+=std::string("/");
     fileName+=std::string(partName.ToCString());
     fileName+=std::string("/model.brep");
     fileName=nativePath(fileName);
     TopoDS_Shape S1;
     if(!openShape(fileName.c_str(), S1)) 
	     return false;
/*
     Handle(XCAFDoc_Location) locAtt;
     assert(label1.FindAttribute(XCAFDoc_Location::GetID(),locAtt));
     TopLoc_Location loc=locAtt->Get();
     S1.Location (loc.Multiplied(S1.Location()));
*/
     thePartShapes.Bind(label1.Tag(),S1);
   }
   return true;
}




void MwOCAF::readPartsStatus()
{
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
     TDF_Label label1 = it.Value();
     Handle(TDataStd_Name)  nameAtt;
     assert(label1.FindAttribute(TDataStd_Name::GetID(),nameAtt));
     TCollection_AsciiString partName=nameAtt->Get();
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue;
     if(vol->type==SPLITTER) continue;
     std::string fileName=projectDir;
     fileName+=std::string("/");
     fileName+=std::string(partName.ToCString());
     fileName+=std::string("/model.status");
     fileName=nativePath(fileName);
     ProjectStatus subStatus;
     if(!subStatus.read(fileName.c_str())) continue;
     vol->defined=subStatus.defined();
   }
}


void MwOCAF::setPartsStatus()
{
   if (theParts.IsNull()) { prjStatus.geometry=0; return;}
   prjStatus.partTypes=true;
   int has_prjstatus=read_prjstatus();
   int partTypes=1;
   int partMaterials=1;
   int wgPorts=1;
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
     TDF_Label label1 = it.Value();
     Handle(TDataStd_Name)  nameAtt;
     if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
     TCollection_AsciiString partName=nameAtt->Get();
     DB::Volume* vol = EmP->FindVolume(partName.ToCString());
     if(!vol) continue;
     vol->defined=1;
     if(vol->type==UNDEFINED) {partTypes=0; vol->defined=0;}
     if(vol->type==DIELECTRIC || vol->type==BOUNDARYCOND) if(!strcmp(vol->material,"?")) {partMaterials=0; vol->defined=0; }
     if(vol->type==BOUNDARYCOND) if(strcmp(vol->material,"?") && strcmp(vol->material,"PEC") && strcmp(vol->material,"PMC")){
	   DB::Material* mat= EmP->FindMaterial(vol->material);
	   if(!mat) {partMaterials=0; vol->defined=0; }
           else  if(mat->Sresistance==0 && mat->Sinductance==0) {partMaterials=0; vol->defined=0; }
     }
     if(vol->type==DIELECTRIC) if(strcmp(vol->material,"?")){
	   if(!strcmp(vol->material,"PEC")||!strcmp(vol->material,"PMC")) {partMaterials=0; vol->defined=0; }
	   else{ 
	      DB::Material* mat= EmP->FindMaterial(vol->material);
	      if(!mat) {partMaterials=0; vol->defined=0; }
              else if(mat->Sresistance!=0 || mat->Sinductance!=0) {partMaterials=0; vol->defined=0; }
	  }
     }
     if(vol->type==GRID) vol->defined=vol->gridNum>0 ? 1: 0;
     if(vol->type==WAVEGUIDE) {vol->defined=vol->TEMportsNum+vol->TEportsNum+vol->TMportsNum>0; if(!vol->defined) wgPorts=0;}
   }
   if(!has_prjstatus 
      || prjStatus.partTypes!=partTypes 
      || EmP->assemblyType==COMPONENT && prjStatus.partMaterials!=partMaterials 
      || EmP->assemblyType==COMPONENT && prjStatus.wgPorts!=wgPorts)
   {
	 prjStatus.partTypes=partTypes;
	 if(EmP->assemblyType==COMPONENT) prjStatus.partMaterials=partMaterials;
	 if(EmP->assemblyType==COMPONENT) prjStatus.wgPorts=wgPorts;
	 save_prjstatus();
   }
}


TopAbs_ShapeEnum shapeType(TopoDS_Shape S)
{
 if(S.ShapeType()!=TopAbs_COMPOUND) return S.ShapeType();
 TopAbs_ShapeEnum type=TopAbs_SHAPE;
 for (TopoDS_Iterator it(S); it.More(); it.Next()) type=min(type,it.Value().ShapeType());
 return type;
}

int defaultPartType(TCollection_AsciiString partName)
{
// Defaults from Names
     if(isCompName(partName))           return COMPONENT;
     else if(isIFName(partName))        return INTERFACE;
     else if(isSplitName(partName))     return SPLITTER;
     else if(isGridName(partName))      return GRID;
     else if(isWgPortName(partName))    return WAVEGUIDE;
     else if(isLinePortName(partName))  return LINEPORT;
     else if(isDielName(partName))      return DIELECTRIC;
     else if(isHoleName(partName))      return HOLE;
     else if(isBCondName(partName))     return BOUNDARYCOND;
// Defaults from Geometries
     return 0;
}



void TransformShape(TopoDS_Shape &S, gp_Trsf T)
{
         Handle(ShapeCustom_TrsfModification) TM = new ShapeCustom_TrsfModification(T);
         TopTools_DataMapOfShapeShape context;
         BRepTools_Modifier MD;
         TopoDS_Shape res = ShapeCustom::ApplyModifier (S, TM, context, MD );
	 S=res;
}



int putVolumesInDB_(DB::EmProblem *emP, Handle(XCAFDoc_ShapeTool) shapeTool, TDF_Label access){
 int modified=0;
 TDF_Label Parts = XCAFDoc_DocumentTool::DocLabel(access).FindChild(6,Standard_True);
 for (TDF_ChildIterator it(Parts,Standard_False); it.More(); it.Next()) 
 {
     TDF_Label label1 = it.Value();
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
     TDF_Label label = refAtt->Get();
     Handle(TDataStd_Name)  nameAtt;
     if(!label.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
     TCollection_AsciiString partName=nameAtt->Get();
     DB::Volume *vol=emP->FindVolume(partName.ToCString());
     if(!vol) {
	   modified=1;
	   vol = new DB::Volume();
	   strcpy(vol->name,partName.ToCString());
	   emP->insertVolume(vol);
           TopoDS_Shape S;
	   if(!shapeTool->GetShape(label,S))     vol->type=0;
	   else vol->type=defaultPartType(partName);
     }
 }
 return modified;
}

void MwOCAF::putVolumesInDB(){
  int modified=putVolumesInDB_(EmP, shapeTool, mainDoc->Main());
  if(modified){
   std::string emFilePath=projectDir; emFilePath+="/model.em";
   saveProblem(EmP,emFilePath);
  }
}

inline void setModified(const char *dir){
   std::string modified=std::string(dir)+std::string("/modified");
   FILE *fid= fopen(nativePath(modified).c_str(), "w");
   fclose(fid);
}


bool MwOCAF::hasSubAssembly(Handle(XCAFDoc_ShapeTool) shapeTool, TDF_Label &ass)
{
   bool hassubass=false;
   for (TDF_ChildIterator it(ass, Standard_False); it.More(); it.Next()){
        TDF_Label li = it.Value();
        TDF_Label rli=li;
        if(shapeTool->IsReference(li)) shapeTool->GetReferredShape(li,rli);
        if(shapeTool->IsAssembly(rli)){
          Handle(TDataStd_Name)  nameAtt;
          TCollection_AsciiString name;
          if(rli.FindAttribute(TDataStd_Name::GetID(),nameAtt)) name=nameAtt->Get();
	  if(name!=TCollection_AsciiString("REMOVE")) {hassubass=true; break;}
	}
   }
   return hassubass;
}

bool MwOCAF::hasEmSubComponent(Handle(XCAFDoc_ShapeTool) shapeTool, TDF_Label &ass)
{
   bool hassubcomp=false;
   TDF_Label rass=ass;
   if(shapeTool->IsReference(ass)) shapeTool->GetReferredShape(ass,rass);
   for (TDF_ChildIterator it(rass, Standard_False); it.More(); it.Next()){
        TDF_Label li = it.Value();
        TDF_Label rli=li;
        if(shapeTool->IsReference(li)) shapeTool->GetReferredShape(li,rli);
        Handle(TDataStd_Name)  nameAtt;
        TCollection_AsciiString name;
        if(rli.FindAttribute(TDataStd_Name::GetID(),nameAtt)) name=nameAtt->Get();
        if(isCompName(name) || isNetName(name)) if(name!=TCollection_AsciiString("REMOVE")) {hassubcomp=true; break;}
   }
   return hassubcomp;
}


void MwOCAF::saveImportedStruct(const char *dir,
		int level,
		TCollection_AsciiString assName,
	       	TopLoc_Location assLoc,
		TDF_Label ass,
		bool addImport)
{

// Creates a new subdocument for the subassembly:
   Handle(TDocStd_Document) subDoc;
   Handle(XCAFApp_Application) XCAFapp=XCAFApp_Application::GetApplication();
   if(USEXMLXCAF)  XCAFapp->NewDocument("XmlXCAF",subDoc);
   else            XCAFapp->NewDocument("MDTV-XCAF",subDoc);
   Handle(XCAFDoc_ShapeTool) subShapeTool =XCAFDoc_DocumentTool::ShapeTool(subDoc->Main());
   Handle(XCAFDoc_ColorTool) subColTool  =XCAFDoc_DocumentTool::ColorTool(subDoc->Main());
   Handle(XCAFDoc_LayerTool) subLayTool  =XCAFDoc_DocumentTool::LayerTool(subDoc->Main());
   TDF_Label subShapes=XCAFDoc_DocumentTool::ShapesLabel(subDoc->Main());

   int assemblyType=0;
   DB::EmProblem *subEmP=new DB::EmProblem();
   subEmP->level=level;
   strcpy(subEmP->defaultBC,EmP->defaultBC);
   if(level==0) subEmP->stepFilePath=EmP->stepFilePath;

   if(isCompName(assName) || isValidMultibodyPartName(assName))
      assemblyType=subEmP->assemblyType=COMPONENT;
   else if(isIFName(assName))
      assemblyType=subEmP->assemblyType=INTERFACE;
   else if(shapeTool->IsAssembly(ass))
      assemblyType=subEmP->assemblyType=NET;

   if(assemblyType==0) return;

// Copy the subassembly structure into the subdocument:
   copySubAss(subShapeTool,subColTool,subLayTool,subShapes, shapeTool, colorTool,layerTool, ass, assLoc, assemblyType);
   std::string emFilePath=dir; emFilePath+="/model.em";
   if(FileExists(emFilePath.c_str())) loadProblem(subEmP, emFilePath.c_str());
   int modified=putVolumesInDB_(subEmP, subShapeTool, subDoc->Main());
   if(modified){
        saveProblem(subEmP,emFilePath);
        if(assemblyType==NET) {
          std::string asstypeFileName=dir; asstypeFileName+="/assemblyType";
          FILE *assfid= fopen(nativePath(asstypeFileName).c_str(), "w");
          fprintf(assfid, "%d\n", assemblyType);
          fprintf(assfid, "%d\n", 0);
          fclose(assfid);
	}
   }
// set subass name attribute into the related parts label:
   TDF_Label subPL = XCAFDoc_DocumentTool::DocLabel(subDoc->Main()).FindChild(6,Standard_True);
   TDataStd_Name::Set(subPL, assName);


// *** make main directory and the interfaces directory for subassembly
   if(!FileExists(dir)) createdir(dir);
   std::string interfaces=std::string(dir)+std::string("/interfaces");
   interfaces=nativePath(interfaces);
   if(!FileExists(interfaces.c_str()))
       createdir(interfaces.c_str());
   else
       removeAllFilesInDir(interfaces.c_str());

// If considered subassembly is a  COMPONENT creates the Partition dir


   if(assemblyType==COMPONENT){
     std::string partition=std::string(dir)+std::string("/Partition");
     partition=nativePath(partition);
     std::string partitionInterfaces=std::string(dir)+std::string("/Partition/interfaces");
     partitionInterfaces=nativePath(partitionInterfaces);
     std::string cmp=assName.ToCString();
     if(FileExists(partitionInterfaces.c_str())) {removeAllFilesInDir(partitionInterfaces.c_str()); removeFile(partitionInterfaces.c_str());  }
     if(FileExists(partition.c_str())) {removeAllFilesInDir(partition.c_str()); removeFile(partition.c_str()); }
   }





   if(assemblyType==COMPONENT){
      std::set<std::string> exclude;
      exclude.insert(std::string("model.em"));
      exclude.insert(std::string("model.status"));
      exclude.insert(std::string("gmsh.opt"));
      removeAllFilesInDirExcludingEndingWith(dir,exclude);
      updatePartColors();
   }
   std::string prjstatusFileName=std::string(dir)+std::string("/model.status");
   prjstatusFileName=nativePath(prjstatusFileName);
   if(!FileExists(prjstatusFileName.c_str())) prjStatus.save(prjstatusFileName.c_str());

// Set subdirs list.  Remove downIF (part.brep)
   std::string subdfname=std::string(dir)+std::string("/subdirs");
   if(assemblyType==NET) {
       bool addSubdirs=!FileExists(subdfname.c_str());
       FILE *subdfid;
       if(addSubdirs) subdfid= fopen(nativePath(subdfname).c_str(), "w");
       TDF_Label subParts = XCAFDoc_DocumentTool::DocLabel(subDoc->Main()).FindChild(6,Standard_True);
       for (TDF_ChildIterator it(subParts,Standard_False); it.More(); it.Next()) 
       {
           TDF_Label label1 = it.Value();
           Handle(TDataStd_Name)  nameAtt;
           if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
           TCollection_AsciiString partName=nameAtt->Get();
           DB::Volume* vol = subEmP->FindVolume(partName.ToCString());
           if(addSubdirs) fprintf(subdfid, "%s\n", partName.ToCString());
	   std::string partDir=std::string(dir)+std::string("/")+partName.ToCString();
	   partDir=nativePath(partDir);
	   std::string downIF=partDir+".brep";
           if(FileExists(downIF.c_str())) remove(downIF.c_str());
       }
       if(addSubdirs) fclose(subdfid);
   }
// Remove upIF (model.brep)
   std::string upIF=std::string(dir)+std::string("/model.brep");
   upIF=nativePath(upIF);
   if(FileExists(upIF.c_str())) remove(upIF.c_str());

// Set modified.
   setModified(dir);

   std::string occFileName=std::string(dir);
   if(USEXMLXCAF) occFileName+=std::string("/model.xml");
   else           occFileName+=std::string("/model.dxc");
   occFileName=nativePath(occFileName);
   if(FileExists(occFileName.c_str())) remove(occFileName.c_str());
   XCAFapp->SaveAs(subDoc,TCollection_AsciiString(occFileName.c_str()));
//   checkPartNF(subDoc);
   XCAFapp->Close(subDoc);

// ** Writes initial model.em and assemblyType.
   emFilePath=std::string (dir)+std::string("/model.em");
   if(!FileExists(emFilePath.c_str())) { 
       FILE *fid= fopen(nativePath(emFilePath).c_str(), "w");
       fprintf(fid, "Level  %d\n", level);
       fprintf(fid, "AssemblyType  %d\n", assemblyType);
       fprintf(fid, "DefaultBoundCond  \"%s\"\n", subEmP->defaultBC);
       if(addImport) if(!EmP->stepFilePath.empty()) fprintf(fid, "\nImport  \"%s\"\n\n", EmP->stepFilePath.c_str());
       subEmP->save(fid);
       fclose(fid);
       std::string asstypeFileName=std::string(dir)+std::string("/assemblyType");
       FILE *assfid= fopen(nativePath(asstypeFileName).c_str(), "w");
       fprintf(assfid, "%d\n", assemblyType);
       fclose(assfid);
   }

   delete(subEmP);

// Recursion on sublevels
   TDF_Label rass=ass;
   TopLoc_Location subAssLoc=assLoc;
   shapeTool->GetReferredShape(ass,rass);
   if(assemblyType==NET) for (TDF_ChildIterator it(rass, Standard_False); it.More(); it.Next()){
     TDF_Label label = it.Value();
     Handle(TDataStd_Name)  nameAtt;
     if(!label.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
     TopLoc_Location loc= shapeTool->GetLocation(label);
     subAssLoc=assLoc.Multiplied(loc);
     TCollection_AsciiString subAssName=nameAtt->Get();
     std::string partDir=std::string (dir)+std::string ("/")+subAssName.ToCString();
     partDir=nativePath(partDir);
     if(!FileExists(partDir.c_str())) createdir(partDir.c_str());;
     saveImportedStruct(partDir.c_str(), level+1, subAssName.ToCString(), subAssLoc, label);
   }
}

void MwOCAF::saveImportedStruct(
		const char *dir
		)
{
   TDF_LabelSequence FreeLabels;
   shapeTool->GetFreeShapes(FreeLabels);
   assert(FreeLabels.Length()>0);
   TDF_Label ass;
   TCollection_AsciiString assName;
/*
   for (int i=1; i<=FreeLabels.Length(); i++) {
    ass =FreeLabels.Value(i);
    TCollection_AsciiString name;
    Handle(TDataStd_Name)  nameAtt;
    if(ass.FindAttribute(TDataStd_Name::GetID(),nameAtt)) assName=nameAtt->Get();
    if(!isCompName(assName) && assName!=TCollection_AsciiString("REMOVE")) break;
   }
*/

   ass=FreeLabels.Value(1);
   TopoDS_Shape S= shapeTool->GetShape(ass);
   TDF_Label main=shapeTool->FindMainShape (S);   if(!main.IsNull()) ass=main;
   TCollection_AsciiString name;
   Handle(TDataStd_Name)  nameAtt;
   if(ass.FindAttribute(TDataStd_Name::GetID(),nameAtt)) assName=nameAtt->Get();

   TopLoc_Location assLoc; assLoc.Identity();
   saveImportedStruct(dir, 0, assName.ToCString(), assLoc, ass, true);
}



void MwOCAF::addToComponentLists( std::set<std::string, std::less<std::string> > *componentlist,
		                  std::map<std::string, std::vector<std::string>, std::less<std::string> > *wgcomponentlist
		                 )
{
   if(subCompNum>0 && subComp==0) return;
   TCollection_AsciiString assName; getAssName(assName);
   std::string cmp=assName.ToCString();
   if(componentlist) if(componentlist->find(cmp)==componentlist->end()) componentlist->insert(cmp);
   if(wgcomponentlist) for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
          TDF_Label label1 = it.Value();
          TDF_Label label = getRefLabel(label1);
          if(label.IsNull()) label=label1;
          TopoDS_Shape S=shapeTool->GetShape(label);
	  if(S.IsNull()) continue;
          Handle(TDataStd_Name)  nameAtt;
          if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
          TCollection_AsciiString partName=nameAtt->Get();
          DB::Volume *vol=getLabelVol(label1);
          if(vol) if(vol->type==WAVEGUIDE) {
              (*wgcomponentlist)[cmp].push_back(std::string(vol->name));
	  }
    }
}

bool MwOCAF::savePartsIF()
{
   std::string  tmpbrep=projectDir+std::string ("/tmp.brep");
   tmpbrep=nativePath(tmpbrep);
   int I=0;
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
     I++;
     TDF_Label label1 = it.Value();
     Handle(TDataStd_Name)  nameAtt;
     assert(label1.FindAttribute(TDataStd_Name::GetID(),nameAtt));
     TCollection_AsciiString partName=nameAtt->Get();
     std::string  partDir=projectDir+std::string ("/")+partName.ToCString();
     partDir=nativePath(partDir);
     DB::Volume *vol=getLabelVol(label1);
     if(vol->type==SPLITTER || vol->type==GRID) continue;
//-------------------
//  save part geometry:
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;
     std::string  fileName=partDir+std::string (".brep");
     fileName=nativePath(fileName);
     TopoDS_Shape S= shapeTool->GetShape(label);
/*   
     TopLoc_Location zeroloc;
     S.Location (zeroloc);
*/
     assert(!S.IsNull());
     TopTools_IndexedMapOfShape Fmap,Emap,Pmap,subShapes;
     makeISubShapes(&Fmap,&Emap,&Pmap,S);
     int FNum=Fmap.Extent();
     for (int FI=1; FI<=FNum; FI++){
       TopoDS_Shape F = Fmap.FindKey(FI);
       int Fsign=(F.Orientation()==TopAbs_FORWARD) ? 1 : -1;
       if(subShapes.FindIndex(F) < 1) subShapes.Add(F);
     }
     int ENum=Emap.Extent();
     for (int EI=1; EI<=ENum; EI++){
       TopoDS_Shape E = Emap.FindKey(EI);
       if(subShapes.FindIndex(E) < 1) subShapes.Add(E);
     }
     int PNum=Pmap.Extent();
     for (int PI=1; PI<=PNum; PI++){
       TopoDS_Shape P = Pmap.FindKey(PI);
       if(subShapes.FindIndex(P) < 1) subShapes.Add(P);
     }
     saveShapesIfDiff( fileName.c_str(), subShapes, tmpbrep.c_str(), true);
   }
   return true;
}


void MwOCAF::addPorts(std::map<std::string, int, std::less<std::string> > &ports, std::map<std::string, double, std::less<std::string> > &portloads)
{
//   int I1=EmP->isTopLevel? I1 : extFaceNum+1;
   int I=0;
   std::set<int> initializedF;
   TCollection_AsciiString assName; getAssName(assName);
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
      TDF_Label label1 = it.Value();
      Handle(TDataStd_Name)  nameAtt;
      if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) return;
      TCollection_AsciiString volName=nameAtt->Get();
      DB::Volume *vol=EmP->FindVolume(volName.ToCString());
      if(vol->disabled) continue;
      if(vol->type!=WAVEGUIDE && vol->type!=LINEPORT) continue;
      TCollection_AsciiString partName=assName;
      if(vol->type==WAVEGUIDE && !strncmp(volName.ToCString(),"WGP",3)) partName=remove_SUB(assName);
      partName+=TCollection_AsciiString("__")+volName ;
      std::string name=partName.ToCString();
      if(vol->type==WAVEGUIDE){
        std::string TEMport,TEport,TMport;
        TEMport=name+"_TEM";
        TEport=name+"_TE";
        TMport=name+"_TM";
        ports[TEMport]=vol->TEMportsNum;
        ports[TEport]=vol->TEportsNum;
        ports[TMport]=vol->TMportsNum;
      }
      if(vol->type==LINEPORT){
        std::string Lport=name+"_v";
        if(ports.find(Lport)==ports.end()) ports[Lport]=1;
        if(portloads.find(Lport)==portloads.end()) portloads[Lport]=50.0;
      }
   }
}

void FileSetTop(std::string fname)
{
std::string ftop=fname+std::string(".top");
FILE *fp=fopen(ftop.c_str(),"w");
fclose(fp);
}

void SetBoostLockFile(std::string fname)
{
std::string ftop=fname+std::string(".lock");
FILE *fp=fopen(ftop.c_str(),"w");
fclose(fp);
}


bool FileIsTop(std::string fname)
{
std::string ftop=fname+std::string(".top");
return FileExists(ftop.c_str());
}


void MwOCAF::initFEPdataStruct(){
  using namespace boost::interprocess;

// *******
// Mapping of part entities to assembly entities:
  TopTools_IndexedMapOfShape  bdrFaces;
    if(EmP->assemblyType==COMPONENT){
    makeTheCompSolid();
    makeSolid(theCompSolid,theMergedSolid);
    for(TopExp_Explorer exp(theMergedSolid,TopAbs_FACE); exp.More(); exp.Next()){
            TopoDS_Shape F=exp.Current();
            F.Orientation(TopAbs_FORWARD);
	    bdrFaces.Add(F);
    }
  }
// *******
  Handle(TColStd_HArray1OfInteger)  *partFacesMap     =new Handle(TColStd_HArray1OfInteger)[numberOfParts()];
  Handle(TColStd_HArray1OfInteger)  *partEdgesMap     =new Handle(TColStd_HArray1OfInteger)[numberOfParts()];
  Handle(TColStd_HArray1OfInteger)  *partVerticesMap  =new Handle(TColStd_HArray1OfInteger)[numberOfParts()];
  int partI=0;
  for (TDF_ChildIterator Vit(theParts,Standard_False); Vit.More(); Vit.Next()) 
  {
     partI++;
     partFacesMap[partI-1]   =NULL;
     partEdgesMap[partI-1]   =NULL;
     partVerticesMap[partI-1]=NULL;
     TDF_Label label1 = Vit.Value();
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue;
     if(vol->type==SPLITTER || vol->type==GRID) continue;
     TopoDS_Shape S= shapeTool->GetShape(label);
     assert(!S.IsNull());
     TopTools_IndexedMapOfShape Fmap,Emap,Pmap;
     makeISubShapes(&Fmap,&Emap,&Pmap,S);
     if(Fmap.Extent()>0) partFacesMap[partI-1] = new TColStd_HArray1OfInteger(1,Fmap.Extent());
     for (int I=1; I<=Fmap.Extent(); I++){
       TopoDS_Shape F = Fmap.FindKey(I);
       int FI=indexedFaces->FindIndex(F);
       partFacesMap[partI-1]->SetValue(I,FI);
     }
     if(Emap.Extent()>0) partEdgesMap[partI-1] = new TColStd_HArray1OfInteger(1,Emap.Extent());
     for (int I=1; I<=Emap.Extent(); I++){
       TopoDS_Shape E = Emap.FindKey(I);
       int EI=indexedEdges->FindIndex(E);
       partEdgesMap[partI-1]->SetValue(I,EI);
     }
     if(Pmap.Extent()>0) partVerticesMap[partI-1] = new TColStd_HArray1OfInteger(1,Pmap.Extent());
     for (int I=1; I<=Pmap.Extent(); I++){
       TopoDS_Shape P = Pmap.FindKey(I);
       int PI=indexedVertices->FindIndex(P);
       partVerticesMap[partI-1]->SetValue(I,PI);
     }
  }

// ************  save Face Links
   TCollection_AsciiString assName; getAssName(assName);
   partI=0;
   std::set<int> initializedF;
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
   // link assembly faces to part faces 
      partI++;
      TDF_Label label1 = it.Value();
      Handle(TDataStd_Name)  nameAtt;
      if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) return;
      TCollection_AsciiString partName=nameAtt->Get();
      DB::Volume *vol=EmP->FindVolume(partName.ToCString());
      if(!vol) {std::cout <<"undefined item  \"" << partName.ToCString() << "\"" << std::endl; assert(false);}; 
      if(vol->type==SPLITTER || vol->type==GRID) continue;
      std::string partDir=projectDir+std::string("/")+partName.ToCString();
      partDir=nativePath(partDir);
      if(!partFacesMap[partI-1].IsNull()) for (int I =1; I <= partFacesMap[partI-1]->Length(); I++){
	 int FI=partFacesMap[partI-1]->Value(I);
	 if(!FI) continue;
	 char tag[10]; sprintf(tag,"%d",FI);
	 std::string FPath=projectDir+std::string("/interfaces/F")+tag;
	 FPath=nativePath(FPath);
	 if(initializedF.find(FI)==initializedF.end()) if(!FileExists(FPath.c_str())){
	     FaceData FD;
	     FD.level=EmP->level; 
	     FD.epsr=1.0; 
	     FD.mur=1.0; 
	     FD.meshref=1.0;
	     FD.cutoffref=1.0;
	     FD.cmp1="-";  FD.cmp2="-";
	     char cname[20]; sprintf(cname,"L%dF%d",FD.level,FI);
	     FD.name=cname;
             FD.sfname=std::string("-");
	     FILE *fout=fopen(FPath.c_str(),"w");
             assert(setLock(fout,"w")==0);
	     FD.write(fout);
	     fflush(fout);
             assert(releaseLock(fout)==0);
             fclose(fout);
	     initializedF.insert(FI);
	     FileSetTop(FPath);
	 }
         if(EmP->assemblyType==NET){
	    sprintf(tag,"%d",I);
            std::string  partFPath=partDir+std::string ("/interfaces/F")+tag;
	    partFPath=nativePath(partFPath);
	    createLink(FPath.c_str(), partFPath.c_str());
	    FILE* fin=fopen(FPath.c_str(),"r");
	    if(!fin){
              std::cout<< "missing file :"<< FPath.c_str() <<std::endl;
	      assert(false);
 	    }
	    FaceData fd;
            assert(setLock(fin,"r")==0);
	    fd.read(fin);
            assert(releaseLock(fin)==0);
	    fclose(fin);
            if(FI>extFaceNum){
	     std::string partFPath_mwm  =partFPath+std::string(".mwm");
	     std::string FPath_mwm=FPath+std::string(".mwm");
	     createLink(FPath_mwm.c_str(), partFPath_mwm.c_str());
	    }
	 }
         // set faces boundary conditions:
         DB::Volume *vol=EmP->FindVolume(partName.ToCString());
         if(vol->type==BOUNDARYCOND || vol->type==WAVEGUIDE) {
	   std::string bdrcond;
	   if(vol->type==BOUNDARYCOND ) bdrcond=std::string(vol->material);
	   if(vol->type==WAVEGUIDE ) bdrcond="WAVEGUIDE";
           if(FileExists(FPath.c_str())){
	     FILE *fp=fopen(FPath.c_str(),"r+");
//	     if(!in) continue;
	     if(!fp){
               std::cout<< "missing file :"<< FPath.c_str() <<std::endl;
	       assert(false);
 	     }
	     FaceData FD;
             assert(setLock(fp,"a")==0);
	     FD.read(fp);
	     std::string str=std::string(vol->material);
	     if(FD.BrCond.find(bdrcond)==FD.BrCond.end()){
	      FD.BrCond.insert(bdrcond);
              rewind(fp);
	      FD.write(fp); 
	      fflush(fp);
	     }	   
             assert(releaseLock(fp)==0);
             fclose(fp);
	   }
         }
      }
   }

// ************  save Vertex Links:
   partI=0;
   std::set<int> initializedP;
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
      partI++;
      TDF_Label label1 = it.Value();
      Handle(TDataStd_Name)  nameAtt;
      if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
      TCollection_AsciiString partName=nameAtt->Get();
      DB::Volume *vol=EmP->FindVolume(partName.ToCString());
      if(!vol) continue;
      if(vol->type==SPLITTER || vol->type==GRID) continue;
      std::string partDir=projectDir+std::string("/")+partName.ToCString();
      partDir=nativePath(partDir);
      if(!partVerticesMap[partI-1].IsNull()) for (int I =1; I <= partVerticesMap[partI-1]->Length(); I++){
	 int PI=partVerticesMap[partI-1]->Value(I);
	 if(!PI) continue;
	 char tag[10]; sprintf(tag,"%d",PI);
         std::string PPath=projectDir+std::string("/interfaces/P")+tag;
         PPath=nativePath(PPath);
	 if(initializedP.find(PI)==initializedP.end()) if(!FileExists(PPath.c_str())){
	     VertexData pd;
	     pd.epsr=1.0; 
	     pd.mur=1.0; 
	     pd.meshref=1.0; 
	     pd.level=EmP->level; 
	     char cname[20]; sprintf(cname,"L%dP%d",pd.level,PI);
	     pd.name=cname;
	     FILE *fout=fopen(PPath.c_str(),"w");
             assert(setLock(fout,"w")==0);
	     pd.write(fout); 
	     fflush(fout);
             assert(releaseLock(fout)==0);
             fclose(fout);
	     initializedP.insert(PI);
	     FileSetTop(PPath);
	 }
	 if(EmP->assemblyType!=COMPONENT){
            std::string  partPPath=partDir+std::string ("/interfaces/P"); 
	    sprintf(tag,"%d",I); partPPath+=tag;
            partPPath=nativePath(partPPath);
	    createLink(PPath.c_str(), partPPath.c_str());
	 }
         // set points boundary conditions:
	 DB::Volume *vol=EmP->FindVolume(partName.ToCString());
	 if(vol->type==BOUNDARYCOND) if(FileExists(PPath.c_str())){
	   FILE *fp=fopen(PPath.c_str(),"r+");
//	   if(!in) continue;
	   if(!fp){
             std::cout<< "missing file :"<< PPath.c_str() <<std::endl;
	     assert(false);
 	   }
	   VertexData pd;
           assert(setLock(fp,"a")==0);
	   pd.read(fp);
	   std::string str=std::string(vol->material);
	   if(pd.BrCond.find(str)==pd.BrCond.end()){
	      pd.BrCond.insert(str);
              rewind(fp);
	      pd.write(fp); 
	      fflush(fp);
	   }	   
           assert(releaseLock(fp)==0);
           fclose(fp);
	 }
      }
   }


// ************  save Edge Links:
   partI=0;
   std::set<int> initializedE;
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
      partI++;
      TDF_Label label1 = it.Value();
      Handle(TDataStd_Name)  nameAtt;
      if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
      TCollection_AsciiString partName=nameAtt->Get();
      TDF_Label label = getRefLabel(label1);
      if(label.IsNull()) label=label1;
      DB::Volume *vol=EmP->FindVolume(partName.ToCString());
      if(!vol) continue;
      if(vol->type==SPLITTER || vol->type==GRID) continue;
      std::string partDir=projectDir+std::string("/")+partName.ToCString();
      partDir=nativePath(partDir);
//      assert(!partEdgesMap[partI-1].IsNull());
      std::map<int,int> Emult;
      std::map<int,std::pair<gp_Vec, gp_Vec > > Enormals;
      if(vol->type==BOUNDARYCOND || vol->type==WAVEGUIDE){
         TopoDS_Shape S = shapeTool->GetShape(label);
         for(TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
          TopoDS_Shape F=exp.Current();
          F.Orientation(TopAbs_FORWARD);
	  Handle(Geom_Surface) GS = BRep_Tool::Surface(TopoDS::Face(F));
          for(TopExp_Explorer exp1(exp.Current(),TopAbs_EDGE); exp1.More(); exp1.Next()){
	    TopoDS_Edge E=TopoDS::Edge(exp1.Current());
            TopoDS_Vertex V1,V2;
            TopExp::Vertices(E,V1,V2, false);
            gp_Pnt P1 = BRep_Tool::Pnt(V1);
	    E.Orientation(TopAbs_FORWARD);
	    int EI=indexedEdges->FindIndex(E);
	    if((bdrFaces.Contains(F))) Emult[EI]+=2;
	    else if(vol->type==BOUNDARYCOND) Emult[EI]++;
	    if(Enormals.find(EI)==Enormals.end()){
		  gp_Vec n1=supNormal(GS, P1);
		  gp_Vec n2=gp_Vec(0.0,0.0,0.0);
                  Enormals[EI]=std::pair<gp_Vec, gp_Vec >(n1,n2);
	    } else {
                  Enormals[EI].second=supNormal(GS, P1);
            }

	  }
	 }
      }
      if(!partEdgesMap[partI-1].IsNull()) for (int I =1; I <= partEdgesMap[partI-1]->Length(); I++){
	 int EI=partEdgesMap[partI-1]->Value(I);
	 if(!EI) continue;
         std::string EPath;
	 char tag[10];
	 sprintf(tag,"%d",EI);
         EPath=projectDir+std::string("/interfaces/E")+tag;
	 if(initializedE.find(EI)==initializedE.end()) if(!FileExists(EPath.c_str())){
             TopoDS_Edge E = TopoDS::Edge(indexedEdges->FindKey(EI));
             bool  CumOri=false;
             TopoDS_Vertex V1,V2;
             TopExp::Vertices(E,V1,V2, CumOri);
	     EdgeData ed;
	     ed.level=EmP->level; 
             gp_Pnt P1 = BRep_Tool::Pnt(V1);
	     ed.P1[0] =P1.X();
	     ed.P1[1] =P1.Y();
	     ed.P1[2] =P1.Z();
	     ed.epsr=1.0; 
	     ed.mur=1.0;
	     ed.meshref=1.0; 
	     ed.cutoffref=1.0;
	     char cname[20]; sprintf(cname,"L%dE%d",ed.level,EI);
	     ed.name=cname;
	     FILE *fout=fopen(EPath.c_str(),"w");
	     assert(setLock(fout,"w")==0);
	     ed.write(fout);
	     fflush(fout);
	     assert(releaseLock(fout)==0);
             fclose(fout);
	     initializedE.insert(EI);
	     FileSetTop(EPath);
	 }
	 if(EmP->assemblyType==NET){
            std::string partEPath=partDir+std::string("/interfaces/E"); 
	    char tag[10]; sprintf(tag,"%d",I); partEPath+=tag;
            partEPath=nativePath(partEPath);
	    createLink(EPath.c_str(), partEPath.c_str());
	    std::string partEPath_mwm  =partEPath+std::string(".mwm");
	    std::string EPath_mwm=EPath+std::string(".mwm");
	    createLink(EPath_mwm.c_str(), partEPath_mwm.c_str());
	 }
         // set edges boundary conditions:
	 DB::Volume *vol=EmP->FindVolume(partName.ToCString());
	 if(vol->type==BOUNDARYCOND || vol->type==LINEPORT) if(FileExists(EPath.c_str())){
	    FILE *fin=fopen(EPath.c_str(),"r");
//	    if(!in) continue;
	    if(!fin){
              std::cout<< "missing file :"<< EPath.c_str() <<std::endl;
	      assert(false);
 	    }
	    EdgeData ed;
	    assert(setLock(fin,"r")==0);
	    ed.read(fin);
      	    assert(releaseLock(fin)==0);
            fclose(fin);
	    int edChanged=0;
	    if(vol->type==BOUNDARYCOND) if(ed.BrCond.find(std::string(vol->material))==ed.BrCond.end()){
	        ed.BrCond.insert(std::string(vol->material));
		edChanged=1;
            }
	    if(vol->type==LINEPORT) if(ed.LPname!=std::string(vol->name)){
	        ed.LPname=std::string(vol->name);
		edChanged=1;
            }
/*
//       Defines singular edges and singular points
            bool singularE=(Emult[EI]==1);
	    if(Emult[EI]>1 ) if(Enormals.find(EI)!=Enormals.end()) 
		    singularE=singularE || (Enormals[EI].first.CrossMagnitude(Enormals[EI].second)>0.7);
	    if(singularE){
	       ed.singular=1; edChanged=1;
               TopoDS_Edge E = TopoDS::Edge(indexedEdges->FindKey(EI));
               TopoDS_Vertex VV[2];
               bool  CumOri=false;
               TopExp::Vertices(E,VV[0],VV[1], CumOri);
               for (int i =0; i <2; i++){
                  int PI=indexedVertices->FindIndex(VV[i]);
//		  if(PI<=extVertexNum){
		    std::string PPath;
                    if(isPartition()&& splitVerticesMap[PI-1]){
		     char tag[10]; sprintf(tag,"%d",splitVerticesMap[PI-1]);
                     PPath=projectDir+std::string("/../interfaces/P")+tag;
		    }else{
		     char tag[10]; sprintf(tag,"%d",PI);
                     PPath=projectDir+std::string("/interfaces/P")+tag;
	            }
                    FILE *fp=fopen(PPath.c_str(),"r+");
		    if(!fp) continue;
		    VertexData pd;
        	    assert(setLock(fp,"a")==0);
	            pd.read(fp);
		    pd.singular=1;
		    rewind(fp);
	            pd.write(fp);
	            fflush(fp);
	            assert(releaseLock(fp)==0);
                    fclose(fp);
//	          }
	       }
	     }
*/
	     if(edChanged){
	        FILE *fout=fopen(EPath.c_str(),"w");
	        assert(setLock(fout,"w")==0);
	        ed.write(fout);
	        fflush(fout);
	        assert(releaseLock(fout)==0);
                fclose(fout);
	     }	   
	  }
      }
   }

   delete [] partFacesMap;
   delete [] partEdgesMap;
   delete [] partVerticesMap;

}

void MwOCAF::setFEproperties()
{
   using namespace boost::interprocess;

   if(EmP->assemblyType!=COMPONENT) return;
   if(subComp) return;
   
//   int I1=EmP->isTopLevel? I1 : extFaceNum+1;
   std::set<int> initializedF;
   std::set<int> initializedE;
   std::set<int> initializedP;
   int I=0;
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
   // link assembly faces to part faces 
      I++;
      TDF_Label label1 = it.Value();
      Handle(TDataStd_Name)  nameAtt;
      if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
      TCollection_AsciiString partName=nameAtt->Get();
      DB::Volume *vol=EmP->FindVolume(partName.ToCString());
      if(EmP->assemblyType==COMPONENT && vol->type!=DIELECTRIC && vol->type!=WAVEGUIDE  && vol->type!=BOUNDARYCOND  && vol->type!=SPLITTER) continue;
      DB::Material* mat= EmP->FindMaterial(vol->material);
      TDF_Label label = getRefLabel(label1);
      if(label.IsNull()) label=label1;
      TopoDS_Shape S = shapeTool->GetShape(label);
      for(TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
	 TopoDS_Shape F=exp.Current();
	 F.Orientation(TopAbs_FORWARD);
	 int FI=indexedFaces->FindIndex(F);
	 if(FI==0) continue;
	 char tag[10]; sprintf(tag,"%d",FI);
	 std::string FPath=projectDir+std::string("/interfaces/F")+tag;
         // set faces refraction index (maximum among adjiacent diel):
	#ifdef USE_SYMBOLIC_LINKS
	 bool reset=initializedF.find(FI)==initializedF.end() && !FileIsSymlink(FPath.c_str());
	#else
	 bool reset=initializedF.find(FI)==initializedF.end() && FileIsTop(FPath.c_str());
	#endif
	 FaceData FD;
         FILE *fp=fopen(FPath.c_str(),"r+");
	 if(!fp){
           std::cout<< "missing file :"<< FPath.c_str() <<std::endl;
	   assert(false);
	 }
         assert(setLock(fp,"a")==0);
	 FD.read(fp);
	 if(reset){
	    FD.epsr=FD.mur=FD.meshref=FD.cutoffref=1.0;
            initializedF.insert(FI);
	 }
	 FD.meshref=max(FD.meshref,vol->meshRefinement);
	 FD.cutoffref=max(FD.cutoffref,vol->cutoffRefinement);
	 if(mat){
	  FD.epsr   =max(FD.epsr,   mat->epsr);
	  FD.mur    =max(FD.mur,    mat->mur);
	 }
	 rewind(fp);
	 FD.write(fp);
	 fflush(fp);
	 assert(releaseLock(fp)==0);
         fclose(fp);
      }
      std::set<int> doneE;
      for(TopExp_Explorer exp(S,TopAbs_EDGE); exp.More(); exp.Next()){
	 TopoDS_Shape E=exp.Current();
	 E.Orientation(TopAbs_FORWARD);
	 int EI=indexedEdges->FindIndex(E);
	 if(EI==0) continue;
	 if(doneE.find(EI)!=doneE.end()) continue;
	 doneE.insert(EI);
	 char tag[10]; sprintf(tag,"%d",EI);
         std::string  EPath=projectDir+std::string ("/interfaces/E")+tag;
         EPath=nativePath(EPath);
         // set edges refraction index (maximum among adjiacent diel):
	 bool reset=initializedE.find(EI)==initializedE.end() && FileIsTop(EPath.c_str());
	 EdgeData ED;
         FILE *fp=fopen(EPath.c_str(),"r+");
	 if(!fp){
           std::cout<< "missing file :"<< EPath.c_str() <<std::endl;
	   assert(false);
	 }
	 assert(setLock(fp,"a")==0);
	 ED.read(fp);
	 if(reset){
	     ED.epsr=ED.mur=ED.meshref=ED.cutoffref=1.0;  
	     initializedE.insert(EI);
	 }
	 ED.meshref=max(ED.meshref,vol->meshRefinement);
	 ED.cutoffref=max(ED.cutoffref,vol->cutoffRefinement);
	 if(mat){
	  ED.epsr   =max(ED.epsr,   mat->epsr);
	  ED.mur    =max(ED.mur,    mat->mur);
	 }
	 rewind(fp);
	 ED.write(fp);
	 fflush(fp);
	 assert(releaseLock(fp)==0);
         fclose(fp);
      }
      std::set<int> doneP;
      for(TopExp_Explorer exp(S,TopAbs_VERTEX); exp.More(); exp.Next()){
	 TopoDS_Shape V=exp.Current();
	 V.Orientation(TopAbs_FORWARD);
	 int PI=indexedVertices->FindIndex(V);
	 if(PI==0) continue;
	 if(doneP.find(PI)!=doneP.end()) continue;
	 doneP.insert(PI);
	 char tag[10]; sprintf(tag,"%d",PI);
         std::string  PPath=projectDir+std::string("/interfaces/P")+tag;
         PPath=nativePath(PPath);
         // set points refraction index (maximum among adjiacent diel):
	 bool reset=initializedP.find(PI)==initializedP.end() && FileIsTop(PPath.c_str());
	 VertexData PD;
         FILE *fp=fopen(PPath.c_str(),"r+");
	 if(!fp){
           std::cout<< "missing file :"<< PPath.c_str() <<std::endl;
	   assert(false);
	 }
	 assert(setLock(fp,"a")==0);
	 PD.read(fp);
	 if(reset){
	    PD.epsr=PD.mur=PD.meshref=1.0; 
	    initializedP.insert(PI);
	 }
	 PD.meshref=max(PD.meshref,vol->meshRefinement);
	 if(mat){
	  PD.epsr   =max(PD.epsr,   mat->epsr);
	  PD.mur    =max(PD.mur,    mat->mur);
	 }
	 rewind(fp);
	 PD.write(fp);
	 fflush(fp);
	 assert(releaseLock(fp)==0);
         fclose(fp);
      }
   }

   readFEproperties();
   setDefaultFEPBC();
}

void MwOCAF::setDefaultFEPBC() //Default Baoundary Condition:
{
    using namespace boost::interprocess;

    if(subComp) return;

    for (int FI =1; FI <= extFaceNum; FI++){ 
        char tag[10]; sprintf(tag,"%d",FI);
        std::string FPath;
        if(faceData[FI-1].BrCond.size()==0 && !faceData[FI-1].Shared()){
	   FPath=projectDir+std::string ("/interfaces/F")+tag;
           FPath=nativePath(FPath);
           FILE *fout=fopen(FPath.c_str(), "a");
           faceData[FI-1].BrCond.insert(EmP->defaultBC);
	   assert(setLock(fout,"a")==0);
           fprintf(fout,"%s\n",EmP->defaultBC);
	   fflush(fout);
	   assert(releaseLock(fout)==0);
           fclose(fout);
	}
    }


    typedef std::set<std::string, std::less<std::string> >::const_iterator BdrIt;

    for (int FI =1; FI <= indexedFaces->Extent(); FI++){
        TopoDS_Shape F = indexedFaces->FindKey(FI);
        for (TopExp_Explorer exp(F,TopAbs_EDGE); exp.More(); exp.Next()){
            TopoDS_Shape E = exp.Current();
	    E.Orientation(TopAbs_FORWARD);
            int EI=indexedEdges->FindIndex(E);
             std::string  EPath=projectDir+std::string ("/interfaces/E");
	     char tag[10];
	     sprintf(tag,"%d",EI); EPath+=tag;
             EPath=nativePath(EPath);
	     bool changed=false;
             for (BdrIt it=faceData[FI-1].BrCond.begin(); it!= faceData[FI-1].BrCond.end(); it++) if(*it!="-"){
	         std::string str=*it;
	         if(edgeData[EI-1].BrCond.find(str)==edgeData[EI-1].BrCond.end()){
		     edgeData[EI-1].BrCond.insert(str);
		     changed=true;
	         }
              }
              if(changed){
	         FILE *fout=fopen(EPath.c_str(),"w");
	         assert(setLock(fout,"w")==0);
	         edgeData[EI-1].write(fout);
	         fflush(fout);
	         assert(releaseLock(fout)==0);
                 fclose(fout);
	    }
        }
        for (TopExp_Explorer exp(F,TopAbs_VERTEX); exp.More(); exp.Next()){
             TopoDS_Shape V = exp.Current();
             V.Orientation(TopAbs_FORWARD);
             int PI=indexedVertices->FindIndex(V);
             std::string  PPath=projectDir+std::string ("/interfaces/P");
	     char tag[10];
	     sprintf(tag,"%d",PI); PPath+=tag;
             PPath=nativePath(PPath);
	     bool changed=false;
             for (BdrIt it=faceData[FI-1].BrCond.begin(); it!= faceData[FI-1].BrCond.end(); it++) if(*it!="-"){
	         std::string str=*it;
	         if(vertexData[PI-1].BrCond.find(str)==vertexData[PI-1].BrCond.end()){
		     vertexData[PI-1].BrCond.insert(str);
		     changed=true;
	         }
              }
              if(changed){
	         FILE *fout=fopen(PPath.c_str(),"w");
	         assert(setLock(fout,"w")==0);
	         vertexData[PI-1].write(fout);
	         fflush(fout);
	         assert(releaseLock(fout)==0);
                 fclose(fout);
	     }
       }
    }



}


void MwOCAF::setDisabledVolumes()
{
   if(EmP->assemblyType==COMPONENT)
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
   // link assembly faces to part faces 
      TDF_Label label1 = it.Value();
      Handle(TDataStd_Name)  nameAtt;
      if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
      TCollection_AsciiString partName=nameAtt->Get();
      DB::Volume *vol=EmP->FindVolume(partName.ToCString());
      if(vol->type!=WAVEGUIDE) continue;
      TDF_Label label = getRefLabel(label1);
      if(label.IsNull()) label=label1;
      TopoDS_Shape S = shapeTool->GetShape(label);
      for(TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
	 TopoDS_Shape F=exp.Current();
	 F.Orientation(TopAbs_FORWARD);
	 int UFI=indexedFaces->FindIndex(F);
	 if(UFI==0) continue;
	 char tag[10]; sprintf(tag,"%d",UFI);
	 std::string UFPath=projectDir+std::string("/interfaces/F")+tag;
         UFPath=nativePath(UFPath);
         FILE *fin=fopen(UFPath.c_str(),"r");
	 if(!fin){
           std::cout<< "missing file :"<< UFPath.c_str() <<std::endl;
	   assert(false);
	 }
 	 assert(setLock(fin,"r")==0);
	 FaceData FD;
	 FD.read(fin);
	 assert(releaseLock(fin)==0);
         fclose(fin);
	 if(FD.cmp1!=FD.cmp2 && FD.cmp1!=std::string("-") && FD.cmp2!=std::string("-")) {vol->disabled=1; break;}
      }
   }
}

int MwOCAF::evalTEMnum(TopoDS_Shape S, bool intPort)
{
// ** find ContCurves
      std::set<int> contCurves;
      typedef std::set<int>::const_iterator IntSetIt;
      for(TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
	 TopoDS_Shape F=exp.Current();
	 F.Orientation(TopAbs_FORWARD);
         for(TopExp_Explorer exp(F,TopAbs_EDGE); exp.More(); exp.Next()){
	   TopoDS_Shape E=exp.Current();
	   E.Orientation(TopAbs_FORWARD);
	   int EI=indexedEdges->FindIndex(E);
	   if(EI) if(!LPedge(EI)){
	     IntSetIt it=contCurves.find(EI);
	     if(it==contCurves.end()) contCurves.insert(EI);
	     else if(!ECedge(EI)) contCurves.erase(it);
	   }
	 }
      }
      int contCurvNum=contCurves.size();
      std::set<int> contCurvesWithLP=contCurves;
      for(TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
	 TopoDS_Shape F=exp.Current();
	 F.Orientation(TopAbs_FORWARD);
         for(TopExp_Explorer exp(F,TopAbs_EDGE); exp.More(); exp.Next()){
	   TopoDS_Shape E=exp.Current();
	   E.Orientation(TopAbs_FORWARD);
	   int EI=indexedEdges->FindIndex(E);
	   if(EI) if(LPedge(EI)) 
		   if(contCurvesWithLP.find(EI)==contCurvesWithLP.end())
			   contCurvesWithLP.insert(EI);
	 }
      }
      int contCurvWithLPNum=contCurvesWithLP.size();

// ** Partitioning of Contour into the connected Contour Parts

    std::map<int,int> contPmap;
    int partNum=0;
    for (IntSetIt it=contCurves.begin(); it!= contCurves.end(); it++) {
        int EI=*it;
        TopoDS_Edge E = TopoDS::Edge(indexedEdges->FindKey(EI));
        if( intPort || !PMCedge(EI) ) {
            bool  CumOri=false;
            TopoDS_Vertex V1,V2;
            TopExp::Vertices(E,V1,V2, CumOri);
            int PI1=indexedVertices->FindIndex(V1);
            int PI2=indexedVertices->FindIndex(V2);
            if(contPmap.find(PI1)==contPmap.end()) contPmap[PI1]=++partNum;
            if(contPmap.find(PI2)==contPmap.end()) contPmap[PI2]=++partNum;
        }
    }

    typedef std::map<int,int>::iterator contPIt;
    int changed=1;  
    while(changed){
      changed=0;  
      for (IntSetIt it=contCurvesWithLP.begin(); it!= contCurvesWithLP.end(); it++) {
        int EI=*it;
        TopoDS_Edge E = TopoDS::Edge(indexedEdges->FindKey(EI));
        if( intPort || !PMCedge(EI) ) {
          bool  CumOri=false;
          TopoDS_Vertex V1,V2;
          TopExp::Vertices(E,V1,V2, CumOri);
          int PI1=indexedVertices->FindIndex(V1);
          int PI2=indexedVertices->FindIndex(V2);
          if(contPmap.find(PI1)!=contPmap.end())
          if(contPmap.find(PI2)!=contPmap.end()){
            int prt1=min(contPmap[PI1],contPmap[PI2]);
            int prt2=max(contPmap[PI1],contPmap[PI2]);
	    if(prt1!=prt2){
	       changed=1;
	       for (contPIt it=contPmap.begin(); it!= contPmap.end(); it++) 
		    if((*it).second==prt2) (*it).second=prt1;
	    }
          }
        }
      }
    }

    partNum=0;
    std::map<int,int> renum;
    for (contPIt it=contPmap.begin(); it!= contPmap.end(); it++)
       if(renum.find((*it).second)==renum.end()) {renum[(*it).second]=++partNum;}

    return partNum-1;
}

void MwOCAF::setTEMnum()
{
//   int I1=EmP->isTopLevel? I1 : extFaceNum+1;
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
   // link assembly faces to part faces 
      TDF_Label label1 = it.Value();
      Handle(TDataStd_Name)  nameAtt;
      if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) return;
      TCollection_AsciiString partName=nameAtt->Get();
      DB::Volume *vol=EmP->FindVolume(partName.ToCString());
      if(vol->type!=WAVEGUIDE) continue;
      if(vol->disabled) continue;
      TDF_Label label = getRefLabel(label1);
      if(label.IsNull()) label=label1;
      TopoDS_Shape S = shapeTool->GetShape(label);
      int TEMnum=evalTEMnum(S, false);
      if(vol->TEMportsNum!=TEMnum) {vol->TEMportsNum=TEMnum; worksaveNeeded=true;}
      if(vol->TEMportsNum==0 && vol->TEportsNum==0 && vol->TMportsNum==0) {vol->TEportsNum=1; worksaveNeeded=true;}
//--------
   }
}

void MwOCAF::setConductorMap()
{
   using namespace boost;
   typedef adjacency_list <vecS, vecS, undirectedS> Graph;
   Graph G;
   int ENum=indexedEdges->Extent();
   int FNum=indexedFaces->Extent();
   int N=0;
   std::map<int,int> CEI;
   for(int EI=1; EI <= ENum; EI++) if(ECedge(EI)) if(CEI.find(EI)==CEI.end()) CEI[EI]=N++;
   std::map<int,int> CFI;
   for(int FI=1; FI <= FNum; FI++) if(ECface(FI)) if(CFI.find(FI)==CFI.end()) CFI[FI]=N++;
   for (int FI=1; FI<=FNum; FI++)  if(ECface(FI)) {
     TopoDS_Shape F =indexedFaces->FindKey(FI);
     for (TopExp_Explorer expe(F,TopAbs_EDGE); expe.More(); expe.Next()) {
         TopoDS_Shape E= expe.Current();
	 E.Orientation(TopAbs_FORWARD);
	 int EI=indexedEdges->FindIndex(E);
	 if(EI>0){
	  assert(ECedge(EI));
          int ice=CEI[EI];
          int icf=CFI[FI];
          add_edge(ice,icf, G);
	 }
     }
   }
   std::map<int,int> CPI;
   for(int EI=1; EI <= ENum; EI++) if(ECedge(EI)){ 
         TopoDS_Edge E= TopoDS::Edge(indexedEdges->FindKey(EI));
         TopoDS_Vertex V1,V2;
         TopExp::Vertices(E,V1,V2);
         int ice=CEI[EI];
	 int IP1=indexedVertices->FindIndex(V1);
	 if(IP1>0){
	  if(CPI.find(IP1)==CPI.end()) CPI[IP1]=N++;
          int icp1=CPI[IP1];
          add_edge(ice,icp1, G);
	 }
	 int IP2=indexedVertices->FindIndex(V2);
	 if(IP2>0){
	  if(CPI.find(IP2)==CPI.end()) CPI[IP2]=N++;
          int icp2=CPI[IP2];
          add_edge(ice,icp2, G);
	 }
   }

   std::vector<int> component(num_vertices(G));
   int NumConductors =connected_components(G, &component[0]);
   edgeConductorMap.clear();
   edgeConductorMap.resize(ENum,0);
   for(int EI=1; EI <= ENum; EI++) if(ECedge(EI)) edgeConductorMap[EI-1]=1+component[CEI[EI]];
   faceConductorMap.clear();
   faceConductorMap.resize(FNum,0);
   for(int FI=1; FI <= FNum; FI++) if(ECface(FI)) faceConductorMap[FI-1]=1+component[CFI[FI]];
   std::vector<double> condArea(NumConductors,0.0);
   for(int FI=1; FI <= FNum; FI++) if(faceConductorMap[FI-1]>0){
     TopoDS_Shape F =indexedFaces->FindKey(FI);
     GProp_GProps gprops;
     BRepGProp::SurfaceProperties(F, gprops);
     double farea =gprops.Mass();
     condArea[faceConductorMap[FI-1]-1]+=farea;
   }
   //define portConductor
   std::vector<int> portConductor(ENum,0);
   for(int EI=1; EI <= ENum; EI++) if(WGedge(EI)) if(edgeConductorMap[EI-1]>0) portConductor[edgeConductorMap[EI-1]-1]=1;
   //Port Conductor with largest area is moved into the first positions
   double Amax=0;  int icondAMax=0;
   for(int i=0;i<NumConductors;i++) if(portConductor[i]) if(condArea[i]>Amax){icondAMax=i+1; Amax=condArea[i];}
   if(icondAMax==0) for(int i=0;i<NumConductors;i++) if(condArea[i]>Amax){icondAMax=i+1; Amax=condArea[i];}
   std::vector<int> renum(NumConductors,0);
   if(icondAMax>0) renum[icondAMax-1]=1;
   int j=1;  for(int i=0;i<NumConductors;i++) if(renum[i]==0) renum[i]=++j;
   std::vector<int> faceConductorMap1=faceConductorMap;
   for(int FI=1; FI <= FNum; FI++) if(faceConductorMap[FI-1]>0) faceConductorMap[FI-1]=renum[faceConductorMap1[FI-1]-1];
   std::vector<int> edgeConductorMap1=edgeConductorMap;
   for(int EI=1; EI <= ENum; EI++) if(edgeConductorMap[EI-1]>0) edgeConductorMap[EI-1]=renum[edgeConductorMap1[EI-1]-1];
   std::vector<int> portConductor1=portConductor;
   for(int i=0;i<NumConductors;i++) portConductor[i]=portConductor1[renum[i]-1];
   //Internal Conductors are moved after Port Conductors:
   for(int i=0;i<NumConductors;i++) renum[i]=0;
   j=0;  
   for(int i=0;i<NumConductors;i++) if(portConductor[i]) if(renum[i]==0) renum[i]=++j;
   for(int i=0;i<NumConductors;i++) if(!portConductor[i]) if(renum[i]==0) renum[i]=++j;
   faceConductorMap1=faceConductorMap;
   for(int FI=1; FI <= FNum; FI++) if(faceConductorMap[FI-1]>0) faceConductorMap[FI-1]=renum[faceConductorMap1[FI-1]-1];
   edgeConductorMap1=edgeConductorMap;
   for(int EI=1; EI <= ENum; EI++) if(edgeConductorMap[EI-1]>0) edgeConductorMap[EI-1]=renum[edgeConductorMap1[EI-1]-1];
}


void MwOCAF::getAssName(TCollection_AsciiString &assName){
    Handle(TDataStd_Name)  nameAtt;
    if(theParts.FindAttribute(TDataStd_Name::GetID(),nameAtt)) assName=nameAtt->Get();
}
void MwOCAF::getAssName(std::string &assName){
  TCollection_AsciiString name; getAssName(name);
  assName=std::string(name.ToCString());
}

void MwOCAF::setAssName(TCollection_AsciiString assName){
    Handle(TDataStd_Name)  nameAtt=TDataStd_Name::Set(theParts,assName);
}


void MwOCAF::setFaceComp(){
    using namespace boost::interprocess;

    TCollection_AsciiString assName;
    getAssName(assName);
    if(hasUPIF) for (int I = 1; I <=UPIFsubshapes.Extent() ; I++){
        TopoDS_Shape F =UPIFsubshapes.FindKey(I);
        if(F.ShapeType()!=TopAbs_FACE) continue;
	int iv1=(F.Orientation()==TopAbs_FORWARD) ? 0 : 1;
        F.Orientation(TopAbs_FORWARD);
        int FI=indexedFaces->FindIndex(F);
	if(FI<0) continue;
	TCollection_AsciiString vol1name=faceAdjParts[2*(FI-1)+iv1];
	DB::Volume *vol1=NULL;
        if(vol1name!=TCollection_AsciiString("-")) vol1=EmP->FindVolume(vol1name.ToCString());
	TCollection_AsciiString vol2name=faceAdjParts[2*(FI-1)+1-iv1];
        DB::Volume *vol2=NULL;
        if(vol2name!=TCollection_AsciiString("-")) vol2=EmP->FindVolume(vol2name.ToCString());
	int fsign=0;
	if(vol1) if(vol1->type==DIELECTRIC) fsign=1;
	if(vol2) if(vol2->type==DIELECTRIC) fsign=-1;
	assert(fsign!=0);
	std::string UFPath=projectDir+std::string("/interfaces/F");
        char tag[10]; sprintf(tag,"%d",FI); UFPath+=tag;
        UFPath=nativePath(UFPath);
        FILE *fp=fopen(UFPath.c_str(),"r+");
	if(!fp){
           std::cout<< "missing file :"<< UFPath.c_str() <<std::endl;
	   assert(false);
	}
	FaceData FD;
	assert(setLock(fp,"w")==0);
	FD.read(fp);
	if(fsign>0){
	    assert(FD.cmp1==std::string("-"));
	    FD.cmp1=assName.ToCString();
	} else if(fsign<0){
	    assert(FD.cmp2==std::string("-"));
	    FD.cmp2=assName.ToCString();
	}
	rewind(fp);
	FD.write(fp);
	fflush(fp);
	assert(releaseLock(fp)==0);
        fclose(fp);
    }
}


bool hasSUB(const char *name){
 int l;
 for(l=strlen(name)-1; l>0&&((!isalpha(name[l])||name[l]=='i')); ) l--;
 return ( l>1 && !strncmp(&name[l-2],"SUB",3));
}

void MwOCAF::setFaceSubdom(){
    using namespace boost::interprocess;
    TCollection_AsciiString assName;
    getAssName(assName);
    std::string strAssName=assName.ToCString();
    for (int FI =1; FI <= faceSubdomMap.size()/2; FI++) {
	int isub1=faceSubdomMap[2*(FI-1)];
	int isub2=faceSubdomMap[2*(FI-1)+1];
        if(isub1!=0 && isub2!=0 && isub1 == isub2) continue;
        if(isub1==0 && isub2==0) continue;
	std::string FPath=projectDir+std::string("/interfaces/F");
        char tag[10]; sprintf(tag,"%d",FI); FPath+=tag;
        FPath=nativePath(FPath);
        FILE *fp=fopen(FPath.c_str(),"r+");
	if(!fp){
           std::cout<< "missing file :"<< FPath.c_str() <<std::endl;
	   assert(false);
	}
	FaceData FD;
	assert(setLock(fp,"w")==0);
	FD.read(fp);
	char t1[10]; sprintf(t1,"_SUB%d",isub1);
	char t2[10]; sprintf(t2,"_SUB%d",isub2);
	if(isub1!=0 && isub2!=0 && isub1 != isub2){
           FD.cmp1=strAssName+std::string(t1);
	   FD.cmp2=strAssName+std::string(t2);
	}
	else if(isub1!=0 && FD.cmp1==strAssName) FD.cmp1=strAssName+std::string(t1);
	else if(isub2!=0 && FD.cmp1==strAssName) FD.cmp1=strAssName+std::string(t2);
	else if(isub1!=0 && FD.cmp2==strAssName) FD.cmp2=strAssName+std::string(t1);
	else if(isub2!=0 && FD.cmp2==strAssName) FD.cmp2=strAssName+std::string(t2);
	rewind(fp);
	FD.write(fp);
	fflush(fp);
	assert(releaseLock(fp)==0);
        fclose(fp);
    }
}




struct FaceData_: public FaceData 
{
 int FI;
};



struct Icouple
{
int I1;
int I2;
};


//  Modified by Sergey KHROMOV - Tue Dec 18 18:02:55 2001 Begin
Standard_Boolean isTangentFaces(const TopoDS_Edge &theEdge,
				const TopoDS_Face &theFace1,
				const TopoDS_Face &theFace2)
{
  if (BRep_Tool::Continuity( theEdge, theFace1, theFace2 ) != GeomAbs_C0)
    return Standard_True;

  Standard_Real aFirst;
  Standard_Real aLast;
    
// Obtaining of pcurves of edge on two faces.
  const Handle(Geom2d_Curve) aC2d1 = BRep_Tool::CurveOnSurface
                                                (theEdge, theFace1, aFirst, aLast);
  const Handle(Geom2d_Curve) aC2d2 = BRep_Tool::CurveOnSurface
                                                (theEdge, theFace2, aFirst, aLast);
  if (aC2d1.IsNull() || aC2d2.IsNull())
    return Standard_False;

// Obtaining of two surfaces from adjacent faces.
  Handle(Geom_Surface) aSurf1 = BRep_Tool::Surface(theFace1);
  Handle(Geom_Surface) aSurf2 = BRep_Tool::Surface(theFace2);

  if (aSurf1.IsNull() || aSurf2.IsNull())
    return Standard_False;

// Computation of the number of samples on the edge.
  BRepAdaptor_Surface              aBAS1(theFace1);
  BRepAdaptor_Surface              aBAS2(theFace2);
  Handle(BRepAdaptor_Surface)     aBAHS1      = new BRepAdaptor_Surface(aBAS1);
  Handle(BRepAdaptor_Surface)     aBAHS2      = new BRepAdaptor_Surface(aBAS2);
  Handle(BRepTopAdaptor_TopolTool) aTool1      = new BRepTopAdaptor_TopolTool(aBAHS1);
  Handle(BRepTopAdaptor_TopolTool) aTool2      = new BRepTopAdaptor_TopolTool(aBAHS2);
  Standard_Integer                 aNbSamples1 =     aTool1->NbSamples();
  Standard_Integer                 aNbSamples2 =     aTool2->NbSamples();
  Standard_Integer                 aNbSamples  =     Max(aNbSamples1, aNbSamples2);


// Computation of the continuity.
  Standard_Real    aPar;
  Standard_Real    aDelta = (aLast - aFirst)/(aNbSamples - 1);
  Standard_Integer i, nbNotDone = 0;

  for (i = 1, aPar = aFirst; i <= aNbSamples; i++, aPar += aDelta) {
    if (i == aNbSamples) aPar = aLast;

    LocalAnalysis_SurfaceContinuity aCont(aC2d1,  aC2d2,  aPar,
					  aSurf1, aSurf2, GeomAbs_G1,
					  0.001, 0.001, 0.1, 0.1, 0.1);
    if (!aCont.IsDone())
      {
	nbNotDone++;
	continue;
      }
    if (!aCont.IsG1())
      return Standard_False;
  }
  
  if (nbNotDone == aNbSamples)
    return Standard_False;

// Do not care about oriantation:

  //Compare normals of tangent faces in the middle point
/*
  Standard_Real MidPar = (aFirst + aLast)/2.;
  gp_Pnt2d uv1 = aC2d1->Value(MidPar);
  gp_Pnt2d uv2 = aC2d2->Value(MidPar);
  gp_Dir normal1, normal2;
  TopOpeBRepTool_TOOL::Nt( uv1, theFace1, normal1 );
  TopOpeBRepTool_TOOL::Nt( uv2, theFace2, normal2 );
  Standard_Real dot = normal1.Dot(normal2);
  if (dot < 0.)
    return Standard_False;
*/

  return Standard_True;
}
//  Modified by Sergey KHROMOV - Tue Dec 18 18:02:56 2001 End


int fdata_same_comp(FaceData_ *a, FaceData_ *b){
    if ((a->cmp1==b->cmp1) && (a->cmp2==b->cmp2)) return 1;
    if ((a->cmp1==b->cmp2) && (a->cmp2==b->cmp1)) return 1;
    return  0;
}


void MwOCAF::setSuperFaces()
{
     using namespace boost::interprocess;
     
//   int I1=EmP->isTopLevel? I1 : extFaceNum+1;
      std::vector<FaceData_ *> fdata;
      int ENum=indexedEdges->Extent();
      int      *tE=new int[ENum];
      for(int i =0; i < ENum; i++) tE[i]=-1;
      int FNum=indexedFaces->Extent();
      int FImax=(EmP->assemblyType==COMPONENT)? FNum : extFaceNum;
      for(int FI = 1; FI <= FImax; FI++) {
        std::string FPath=projectDir+std::string("/interfaces/F");
        char tag[10]; sprintf(tag,"%d",FI); FPath+=tag;
        FPath=nativePath(FPath);
        FILE *fin=fopen(FPath.c_str(),"r");
	if(!fin){
           std::cout<< "missing file :"<< FPath.c_str() <<std::endl;
	   assert(false);
	}
    	assert(setLock(fin,"r")==0);
	FaceData_ *fd=new(FaceData_);
        fd->read(fin);
	if(fd->cmp1!=fd->cmp2 && fd->cmp1!=std::string("-") && fd->cmp2!=std::string("-")){
	  fd->FI=FI;
          fd->sfname=fd->name;
	  fdata.push_back(fd);
	}else{
	  delete fd;
	}
	assert(releaseLock(fin)==0);
        fclose(fin);
      }
      int NF=fdata.size();
      std::map<int,Icouple> EFlinks;
      typedef std::map<int,Icouple>::iterator EFlIt;
      for(int FDI = 1; FDI <= NF; FDI++) {
          TopoDS_Shape F= indexedFaces->FindKey(fdata[FDI-1]->FI);
          for (TopExp_Explorer expe(F,TopAbs_EDGE); expe.More(); expe.Next()) {
	    TopoDS_Shape E=expe.Current();
	    E.Orientation(TopAbs_FORWARD);
	    int EI=indexedEdges->FindIndex(E);
	    EFlIt it =EFlinks.find(EI);
            if(it==EFlinks.end() ) { Icouple FF; FF.I1=FDI; FF.I2=0; EFlinks[EI]=FF;}
	    else if(FDI!=(*it).second.I1 && (*it).second.I2==0)  (*it).second.I2=FDI;
          }
      }
      bool changed=true;
      while(changed){
         changed=false;
	 for(EFlIt it =EFlinks.begin(); it !=EFlinks.end(); it++){
	   if(!(*it).second.I2) continue;
	   int FDI1=(*it).second.I1;
	   int FDI2=(*it).second.I2;
	   if(!fdata_same_comp(fdata[FDI1-1],fdata[FDI2-1])) continue;
	   int EI=(*it).first; TopoDS_Edge E = TopoDS::Edge(indexedEdges->FindKey(EI));
           TopoDS_Face F1=TopoDS::Face(indexedFaces->FindKey(fdata[FDI1-1]->FI));
           TopoDS_Face F2=TopoDS::Face(indexedFaces->FindKey(fdata[FDI2-1]->FI));
	   if(tE[EI-1]==-1) { 
		changed=true;
		tE[EI-1]= (isTangentFaces(E, F1, F2) && edgeData[EI-1].BrCond.size()==0) ? 1:0; 
	   }
	   if(tE[EI-1]>0){
	         if(fdata[FDI1-1]->sfname > fdata[FDI2-1]->sfname) {fdata[FDI1-1]->sfname=fdata[FDI2-1]->sfname; changed=true;}
	    else if(fdata[FDI2-1]->sfname > fdata[FDI1-1]->sfname) {fdata[FDI2-1]->sfname=fdata[FDI1-1]->sfname; changed=true;}
           }
         }
      }
      typedef std::pair<std::string,std::string> StringPair;
      typedef std::map<std::string,int>  StringIndex;
      std::map<StringPair, StringIndex>  sfnameRemap;
      for(int I = 1; I <= NF; I++) {
	 std::string cmpMin=min(fdata[I-1]->cmp1, fdata[I-1]->cmp2);
	 std::string cmpMax=max(fdata[I-1]->cmp1, fdata[I-1]->cmp2);
	 StringPair cmpCouple(cmpMin,cmpMax);
	 sfnameRemap[cmpCouple][fdata[I-1]->sfname]=0;
      }
      typedef std::map<StringPair, StringIndex>::iterator cmpCoupleIt;
      typedef std::map<std::string,int>::iterator StringIndexIt;
      for (cmpCoupleIt cit=sfnameRemap.begin(); cit!= sfnameRemap.end(); cit++){
        int SFI=0;
        for (StringIndexIt sit=(*cit).second.begin(); sit!= (*cit).second.end(); sit++) (*sit).second=++SFI;
      }
      for(int I = 1; I <= NF; I++) {
	 std::string cmpMin=min(fdata[I-1]->cmp1, fdata[I-1]->cmp2);
	 std::string cmpMax=max(fdata[I-1]->cmp1, fdata[I-1]->cmp2);
	 StringPair cmpCouple(cmpMin,cmpMax);
	 int SFI=sfnameRemap[cmpCouple][fdata[I-1]->sfname];
	 fdata[I-1]->sfname=cmpMin+std::string("_")+cmpMax+std::string("_SF")+to_string(SFI);
	 int FI=fdata[I-1]->FI;
         std::string FPath=projectDir+std::string ("/interfaces/F");
         char tag[10]; sprintf(tag,"%d",FI); FPath+=tag;
         FPath=nativePath(FPath);
         FILE *fout=fopen(FPath.c_str(),"w");
	 assert(setLock(fout,"w")==0);
         fdata[I-1]->write(fout);
	 assert(releaseLock(fout)==0);
         fclose(fout);
      }

      for(int I = 1; I <= NF; I++) delete fdata[I-1];
      delete [] tE;
}


void MwOCAF::setSuperFaceSplitterMap()
{
   superfaceSplitterMap.clear();
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
     TDF_Label label1 = it.Value();
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;

//---
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue;
     if(vol->type==SPLITTER ){
       TopoDS_Shape S = shapeTool->GetShape(label);
       if(S.ShapeType()==TopAbs_FACE){
           TopoDS_Shape F=S;
           F.Orientation(TopAbs_FORWARD);
           int FI=indexedFaces->FindIndex(F);
           if(FI>0) if(faceData[FI-1].sfname!=std::string("-")) superfaceSplitterMap[faceData[FI-1].sfname]=vol->name;
       }
       if(S.ShapeType()==TopAbs_SHELL || S.ShapeType()==TopAbs_COMPOUND) for (TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
           TopoDS_Shape F = exp.Current(); 
           F.Orientation(TopAbs_FORWARD);
           int FI=indexedFaces->FindIndex(F);
           if(FI>0) if(faceData[FI-1].sfname!=std::string("-")) superfaceSplitterMap[faceData[FI-1].sfname]=vol->name;
       }
     }
   }
}

void MwOCAF::setSuperCurves()
{
   using namespace boost;
   typedef adjacency_list <vecS, vecS, undirectedS> Graph;

   int FNum=indexedFaces->Extent();
   int ENum=indexedEdges->Extent();
   int PNum=indexedVertices->Extent();
   std::vector<std::set<std::string>>  edgeComponents;
   std::vector<std::set<std::string>>  edgeSuperfaces;
   edgeComponents.resize(ENum);
   edgeSuperfaces.resize(ENum);

   for(int FI = 1; FI <= FNum; FI++) {
      if(faceData[FI-1].cmp1==faceData[FI-1].cmp2) continue;
      TopoDS_Shape F= indexedFaces->FindKey(FI);
      for (TopExp_Explorer expe(F,TopAbs_EDGE); expe.More(); expe.Next()) {
	   TopoDS_Shape E=expe.Current();
	   E.Orientation(TopAbs_FORWARD);
	   int EI=indexedEdges->FindIndex(E);
	   if(faceData[FI-1].cmp1!=std::string("-")) edgeComponents[EI-1].insert(faceData[FI-1].cmp1);
	   if(faceData[FI-1].cmp2!=std::string("-")) edgeComponents[EI-1].insert(faceData[FI-1].cmp2);
	   if(faceData[FI-1].sfname!=std::string("-")) edgeSuperfaces[EI-1].insert(faceData[FI-1].sfname);
      }
   }

   std::map<std::string,int>  compCurveI;
   std::vector<std::string>   edgeCompCurve;
   edgeCompCurve.resize(ENum);
   std::map<std::string,std::map<int,std::set<int>>> compCurveVertexEdges;
   std::map<std::string,std::vector<int>> compCurveEdges;
   for(int EI = extEdgeNum+1; EI <= ENum; EI++) {
      edgeCompCurve[EI-1]="-";
      if(edgeComponents[EI-1].size()<3) continue;
      if(edgeSuperfaces[EI-1].size()<2) continue;
      typedef std::set<std::string> ::iterator CompIt;
      for (CompIt it=edgeComponents[EI-1].begin(); it!= edgeComponents[EI-1].end(); it++){
	  if(it==edgeComponents[EI-1].begin()) edgeCompCurve[EI-1]=*it;
	  else{
	       edgeCompCurve[EI-1]+=std::string("_");
	       edgeCompCurve[EI-1]+=*it;
	  };
      }
      compCurveEdges[edgeCompCurve[EI-1]].push_back(EI);
      TopoDS_Shape eshp= indexedEdges->FindKey(EI);
      TopoDS_Edge E=TopoDS::Edge(eshp);
      TopoDS_Vertex V1,V2;
      TopExp::Vertices(E,V1,V2);
      int I1=indexedVertices->FindIndex(V1);
      int I2=indexedVertices->FindIndex(V1);
      int Ei=compCurveEdges[edgeCompCurve[EI-1]].size();
      compCurveVertexEdges[edgeCompCurve[EI-1]][I1-1].insert(Ei);
      compCurveVertexEdges[edgeCompCurve[EI-1]][I2-1].insert(Ei);
   }

   std::vector<int>  connectedComponent(ENum,0);
   std::map<int,int> EI2graphNode;
   std::vector<int>  graphNode2EI;
   for(std::map<std::string,std::map<int,std::set<int>>>::iterator it =compCurveVertexEdges.begin(); it !=compCurveVertexEdges.end(); it++){
     Graph G;
     for(std::map<int,std::set<int>>::iterator it2 =it->second.begin(); it2 !=it->second.end(); it2++)
        if(it2->second.size()>1) if(!ECvertex(it2->first)) if(!PMCvertex(it2->first)){
	   std::set<int>::iterator it3 =it2->second.begin(); int Ei1=*it3; it3++; int Ei2=*it3;
           if(EI2graphNode.find(Ei1)==EI2graphNode.end()){
		 EI2graphNode[Ei1]=graphNode2EI.size();
		 graphNode2EI.push_back(Ei1);
	   }
           if(EI2graphNode.find(Ei2)==EI2graphNode.end()){
		   EI2graphNode[Ei2]=EI2graphNode.size();
		   graphNode2EI.push_back(Ei2);
	   }
	   add_edge(EI2graphNode[Ei1],EI2graphNode[Ei1], G);
     }
     std::vector<int> component(num_vertices(G));
     int componentNum =connected_components(G, &component[0]);
     for(int i=0; i<num_vertices(G); i++){
	int EI=graphNode2EI[i]; connectedComponent[EI-1]=component[i];
     }
   }

   for(int EI = extEdgeNum+1; EI <= ENum; EI++) {
      if(edgeCompCurve[EI-1]==std::string("-")) continue;
      std::string EPath=projectDir+std::string("/interfaces/E");
      char tag[10]; sprintf(tag,"%d",EI); EPath+=tag;
      EPath=nativePath(EPath);
      FILE *fin=fopen(EPath.c_str(),"r");
      if(!fin){
         std::cout<< "missing file :"<< EPath.c_str() <<std::endl;
	 assert(false);
      }
      assert(setLock(fin,"r")==0);
      EdgeData *ed=new(EdgeData);
      ed->read(fin);
      assert(releaseLock(fin)==0);
      fclose(fin);
      if(ed->scname!=std::string("-")){ delete ed; continue; }
      ed->scname=edgeCompCurve[EI-1];
      if(connectedComponent[EI-1]>0) {
         char tag[10]; sprintf(tag,"_CC%d",connectedComponent[EI-1]);
	 ed->scname+=std::string(tag);
      }
      FILE *fout=fopen(EPath.c_str(),"w");
      assert(setLock(fout,"w")==0);
      ed->write(fout);
      delete ed;
      assert(releaseLock(fout)==0);
      fclose(fout);
   }

}


void MwOCAF::checkSuperFacesTEMnum()
{
//  ckeck TEM_Num (must be 0 for superface): 
      std::map<std::string, int, std::less<std::string> > SFindex;
      TopTools_IndexedMapOfShape  superF;
      BRep_Builder builder;
      int SFnum=0;
      int FNum=indexedFaces->Extent();
      for(int FI = 1; FI <= FNum; FI++) if(faceData[FI-1].cmp1!=faceData[FI-1].cmp2){
	if(SFindex.find(faceData[FI-1].sfname)==SFindex.end()) {
	  SFnum++;
	  SFindex[faceData[FI-1].sfname]=SFnum;
          TopoDS_Compound  S;
          builder.MakeCompound(S);
          superF.Add(S);
	 }
	 int SFI=SFindex[faceData[FI-1].sfname];
         TopoDS_Compound S=TopoDS::Compound(superF.FindKey(SFI));
         TopoDS_Shape F= indexedFaces->FindKey(FI);
         builder.Add(S,F);
      }
      for (int SFI=1; SFI<=superF.Extent(); SFI++){
        TopoDS_Compound S=TopoDS::Compound(superF.FindKey(SFI));
        int TEMnum=evalTEMnum(S,true);
	if(TEMnum>0) {prjStatus.linePorts=false; break;}
      }
}

void MwOCAF::setSuperCurveFaceData(
        std::map<std::string, std::set<std::string> > &SCSFlinks,
	std::map<std::string, bool > &SFhasPMC,
	std::map<std::string, bool > &SChasPMC
){
//   int I1=EmP->isTopLevel? I1 : extFaceNum+1;
      int FNum=indexedFaces->Extent();
      std::map<std::string, std::set<std::string> > VSClinks;
      for(int FI = 1; FI <= FNum; FI++) {
          if(EmP->assemblyType==COMPONENT) if(FI<=extFaceNum) continue;
          if(faceData[FI-1].sfname==std::string("-")) continue;
          TopoDS_Shape F= indexedFaces->FindKey(FI);
          for (TopExp_Explorer exp(F,TopAbs_EDGE); exp.More(); exp.Next()) {
	    TopoDS_Edge E=TopoDS::Edge(exp.Current());
	    E.Orientation(TopAbs_FORWARD);
	    int EI=indexedEdges->FindIndex(E);
            if(edgeData[EI-1].scname==std::string("-")) continue;
	    SCSFlinks[edgeData[EI-1].scname].insert(faceData[FI-1].sfname);
            TopoDS_Vertex V1,V2;
            bool  CumOri=false;
            TopExp::Vertices(E,V1,V2, CumOri);
            int PI1=indexedVertices->FindIndex(V1);
            int PI2=indexedVertices->FindIndex(V2);
	    VSClinks[vertexData[PI1-1].name].insert(edgeData[EI-1].scname);
	    VSClinks[vertexData[PI2-1].name].insert(edgeData[EI-1].scname);
          }
      }
      int PNum=indexedVertices->Extent();
      for(int PI = 1; PI <= PNum; PI++)
	 if(VSClinks.find(vertexData[PI-1].name)!=VSClinks.end())
            if(VSClinks[vertexData[PI-1].name].size()>1) {
               char tag[10]; sprintf(tag,"%d",PI);
               std::string PPath=projectDir+std::string ("/interfaces/P")+tag;
	       FILE *fout=fopen(PPath.c_str(),"w");
	       assert(setLock(fout,"w")==0);
	       vertexData[PI-1].shared=1;
	       vertexData[PI-1].write(fout);
	       fflush(fout);
	       assert(releaseLock(fout)==0);
               fclose(fout);
             }

      for(int FI = 1; FI <= FNum; FI++) {
          if(EmP->assemblyType==COMPONENT) if(FI<=extFaceNum) continue;
          if(faceData[FI-1].sfname==std::string("-")) continue;
          SFhasPMC[faceData[FI-1].sfname]==false;
          TopoDS_Shape F= indexedFaces->FindKey(FI);
          for (TopExp_Explorer exp(F,TopAbs_VERTEX); exp.More(); exp.Next()) {
	    TopoDS_Vertex V=TopoDS::Vertex(exp.Current());
	    int PI=indexedVertices->FindIndex(V);
            SFhasPMC[faceData[FI-1].sfname]+=vertexData[PI-1].shared;
            SFhasPMC[faceData[FI-1].sfname]+=PMCvertex(PI);
          }
      }
      int ENum=indexedEdges->Extent();
      for(int EI = 1; EI <= ENum; EI++) {
          if(EmP->assemblyType==COMPONENT) if(EI<=extEdgeNum) continue;
          if(edgeData[EI-1].scname==std::string("-")) continue;
          SChasPMC[edgeData[EI-1].scname]==false;
          TopoDS_Edge E= TopoDS::Edge(indexedEdges->FindKey(EI));
          TopoDS_Vertex V1,V2;
          bool  CumOri=false;
          TopExp::Vertices(E,V1,V2, CumOri);
          int PI1=indexedVertices->FindIndex(V1);
          int PI2=indexedVertices->FindIndex(V2);
          SChasPMC[edgeData[EI-1].scname]+=vertexData[PI1-1].shared;
          SChasPMC[edgeData[EI-1].scname]+=PMCvertex(PI1);
          SFhasPMC[edgeData[EI-1].scname]+=vertexData[PI2-1].shared;
          SChasPMC[edgeData[EI-1].scname]+=PMCvertex(PI2);
      }      
}

void MwOCAF::setSuperCurvesConstU(const char* projectDir,
		std::map<std::string, std::set<std::string> > &SCSFlinks,
		std::map<std::string, bool > &SFhasPMC,
		std::map<std::string, bool > &SChasPMC,
                std::map<std::string, bool>  &superCurveHasConstU
	     )
{
     using namespace boost::interprocess;
     using namespace boost;
     typedef adjacency_list <vecS, vecS, undirectedS> Graph;
     std::map<std::string, int> SFindex;
     int SFnum=0;
     Graph G;
     for(std::map<std::string, std::set<std::string> >::iterator cit =SCSFlinks.begin(); cit !=SCSFlinks.end(); cit++){
         for(std::set<std::string>::iterator fit =cit->second.begin(); fit !=cit->second.end(); fit++)  
		 if(SFindex.find(*fit)==SFindex.end()) SFindex[*fit]=SFnum++;
         for(std::set<std::string>::iterator fit1 =cit->second.begin(); fit1 !=cit->second.end(); fit1++){
	        std::set<std::string>::iterator fit2=fit1; fit2++; if(fit2==cit->second.end()) break;
	        int SFI1=SFindex[*fit1];
	        int SFI2=SFindex[*fit2];
	        add_edge(SFI1,SFI2, G);
         }
     }
     if(SFnum==0) return;
     std::vector<int> component(SFnum);
     int componentNum =connected_components(G, &component[0]);
     std::vector<bool> compHasU0(componentNum,true);
     for(std::map<std::string, int>::iterator sfit =SFindex.begin(); sfit !=SFindex.end(); sfit++) if(SFhasPMC[sfit->first]) compHasU0[SFindex[sfit->first]] =false;
     std::set<int> SFdone;
     for(std::map<std::string, std::set<std::string> >::iterator cit =SCSFlinks.begin(); cit !=SCSFlinks.end(); cit++){
	     std::set<std::string>::iterator sfit =cit->second.begin();
	     int icomp=component[SFindex[*sfit]];
	     superCurveHasConstU[cit->first] = ( SFdone.find(icomp)!=SFdone.end() || !compHasU0[icomp] ) && !SChasPMC[cit->first];
	     SFdone.insert(icomp);
     }
     bool writeToFile=false;
     if(!writeToFile) return;
     std::string fileName=std::string(projectDir)+std::string ("/superCurveHasConstU.dat");
     FILE *out=fopen(nativePath(fileName).c_str(), "w");
     if(out){
        for(std::map<std::string, bool >::iterator cit =superCurveHasConstU.begin(); cit !=superCurveHasConstU.end(); cit++){
         fprintf(out, "%s  ", cit->first.c_str() );
         fprintf(out, "%d  ", cit->second? 1:0 );
         fprintf(out, "\n");
        }
        fclose(out);
     }

}



void MwOCAF::writeSuperCurvesConstU(std::map<std::string, bool>  &superCurveHasConstU){
//   int I1=EmP->isTopLevel? I1 : extFaceNum+1;
      int ENum=indexedEdges->Extent();
      for(int EI = 1; EI <= ENum; EI++){
         if(edgeData[EI-1].scname==std::string("-")) continue;
	 if(superCurveHasConstU.find(edgeData[EI-1].scname)==superCurveHasConstU.end()) continue;
	 if(!superCurveHasConstU[edgeData[EI-1].scname]) continue;
         char tag[10]; sprintf(tag,"%d",EI);
         std::string EPath=projectDir+std::string ("/interfaces/E")+tag;
	 FILE *fout=fopen(EPath.c_str(),"w");
	 assert(setLock(fout,"w")==0);
	 edgeData[EI-1].hasConstU=1;
	 edgeData[EI-1].write(fout);
	 fflush(fout);
	 assert(releaseLock(fout)==0);
         fclose(fout);
       }
}



void MwOCAF::makeUPIF(TopoDS_Shape &IF)
{
  TopTools_IndexedMapOfShape IFaces;
  std::vector<int> FOrient;
  for (TopExp_Explorer ex1(theMergedSolid,TopAbs_FACE); ex1.More(); ex1.Next()){
         TopoDS_Shape F=ex1.Current();
	 int Fsign=(F.Orientation()==TopAbs_FORWARD) ? 1 : -1;
         F.Orientation(TopAbs_FORWARD);
	 int FI=IFaces.FindIndex(F);
	 if(FI<1) {IFaces.Add(F); FOrient.push_back(0); FI=FOrient.size(); }
         FOrient[FI-1]+=Fsign;
  }
  BRep_Builder builder;
  TopoDS_Shell shell;
  builder.MakeShell(shell);
  for (int FI=1; FI <=IFaces.Extent(); FI++) if(abs(FOrient[FI-1])==1) {
     TopoDS_Shape F =IFaces.FindKey(FI);
     if(FOrient[FI-1]<0) F.Orientation(TopAbs_REVERSED);
     builder.Add(IF,F);
  }
  IF=shell;
}

void MwOCAF::makeIF(TopoDS_Shape &IF)
{
  TopTools_IndexedMapOfShape IFaces;
  std::vector<int> FOrient;
  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
     TDF_Label label1 = it.Value();
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;
     bool skip=false;
     DB::Volume *vol=getLabelVol(label);
     if(vol) skip=(EmP->assemblyType==COMPONENT && vol->type!=DIELECTRIC);
     if(skip) continue;
     TopoDS_Shape S = shapeTool->GetShape(label);
     for (TopExp_Explorer ex1(S,TopAbs_FACE); ex1.More(); ex1.Next()){
         TopoDS_Shape F=ex1.Current();
	 int Fsign=(F.Orientation()==TopAbs_FORWARD) ? 1 : -1;
         F.Orientation(TopAbs_FORWARD);
	 int FI=IFaces.FindIndex(F);
	 if(FI<1) {IFaces.Add(F); FOrient.push_back(0); FI=FOrient.size(); }
         FOrient[FI-1]+=Fsign;
     }
  }

  if(EmP->assemblyType==COMPONENT)  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
     TDF_Label label1 = it.Value();
     TDF_Label label = getRefLabel(label1);
     if(label.IsNull()) label=label1;
     DB::Volume *vol=getLabelVol(label);
     if(!vol) continue;
     if(vol->type==BOUNDARYCOND) { //Remove BC faces from the interface
      TopoDS_Shape S = shapeTool->GetShape(label);
      Handle(XCAFDoc_Location) locAtt;
      for (TopExp_Explorer ex1(S,TopAbs_FACE); ex1.More(); ex1.Next()){
        TopoDS_Shape F=ex1.Current();
	int Fsign=(F.Orientation()==TopAbs_FORWARD) ? 1 : -1;
        F.Orientation(TopAbs_FORWARD);
	int FI=IFaces.FindIndex(F);
        if(FI>0) FOrient[FI-1]=0;
      }
     }
  }
  BRep_Builder builder;
  TopoDS_Shell shell;
  builder.MakeShell(shell);
  for (int FI=1; FI <=IFaces.Extent(); FI++) if(abs(FOrient[FI-1])==1) {
     TopoDS_Shape F =IFaces.FindKey(FI);
     if(FOrient[FI-1]<0) F.Orientation(TopAbs_REVERSED);
     builder.Add(shell,F);
  }
  IF=shell;

}

//=======================================================================
//function : void MakeSolid(const TopoDS_CompSolid& S, TopoDS_Shape &Solid)
//purpose  : replace BRepLib_MakeSolid(const TopoDS_CompSolid& S) 
//           which do not works when the resulting solid is coomposed of multiple shells
//           (solid with voids)
//=======================================================================

void MwOCAF::makeSolid(const TopoDS_CompSolid& S, TopoDS_Solid &Solid)
{
  BRep_Builder B;
  B.MakeSolid(Solid);

  TopExp_Explorer ex1, ex2;
  TopTools_MapOfShape aMapOfFaces;
  for (ex1.Init(S,TopAbs_SHELL); ex1.More(); ex1.Next()) {
    for (ex2.Init(ex1.Current(),TopAbs_FACE); ex2.More(); ex2.Next()) {
      if(!aMapOfFaces.Add(ex2.Current())) {
	aMapOfFaces.Remove(ex2.Current());
      }
    }    
  }

  TopTools_IndexedMapOfShape  faces;
  for(TopTools_MapIteratorOfMapOfShape amapIter(aMapOfFaces);amapIter.More(); amapIter.Next()) {
      TopoDS_Shape F=amapIter.Key();
      if(faces.FindIndex(F)<1) faces.Add(F);
  }

  int FNum=faces.Extent();
  TopTools_IndexedMapOfShape edges;
  TColStd_SequenceOfInteger E2F;
  for(int FI=1; FI<=FNum; FI++){
    TopoDS_Shape F=faces.FindKey(FI);
    for (ex1.Init(F,TopAbs_EDGE); ex1.More(); ex1.Next()){
          TopoDS_Shape E=ex1.Current();
//	  E.Orientation(TopAbs_FORWARD);
	  int EI=edges.FindIndex(E);
	  if(EI<1)    {edges.Add(E); E2F.Append(FI); E2F.Append(0);}
	  else        {E2F.SetValue(2*EI, FI); }
    }
  }
  int ENum=edges.Extent();;
  TColStd_Array1OfInteger F2Sh(1,FNum);
  for(int I=1; I<=FNum; I++) F2Sh(I)=I;
  for(int I=1; I<=ENum; I++) {
	  int FI1=E2F(2*I-1);
	  int FI2=E2F(2*I-0);
	  if(FI1>0 && FI2>0) F2Sh(FI1)=F2Sh(FI2)=min(F2Sh(FI1),F2Sh(FI2));
  }

  TColStd_IndexedMapOfInteger I2I;
  for(int I=1; I<=FNum; I++) I2I.Add(F2Sh(I));
  for(int I=1; I<=FNum; I++) F2Sh(I)=I2I.FindIndex(F2Sh(I));
  int ShellNum=I2I.Extent();

  for(int ShI=1; ShI<=ShellNum; ShI++){
    TopoDS_Shell aShell;
    B.MakeShell(aShell);
    for(int I=1; I<=FNum; I++)  if(F2Sh(I)==ShI) B.Add(aShell, faces(I));
    B.Add(Solid,aShell);
  }

}

bool  MwOCAF::importSTEP_OR_DXC( char* fileName, bool update )
{
	EmP->stepFilePath=fileName;
	EmP->hasGeo=true;
        bool fileExist=FileExists(fileName);
	if(update && !fileExist){
	   stepFileNotFound e(fileName);
	   throw(e);
	}
	bool reload=update && fileExist;
	bool reloaded=false;
	if(reload){
	         importSTEP(mainDoc, fileName);
		 setDocTools();
		 setMissingNames();
		 worksaveNeeded=true;
		 reloaded=true;
	} else  openDoc(nativePath(EmP->occFilePath).c_str());
	return reloaded;
}


