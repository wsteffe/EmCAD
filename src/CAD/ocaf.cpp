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


#if defined(HAVE_OCE_CONFIG_H)
 #include <oce-config.h>
#elif defined(HAVE_OCC_CONFIG_H)
 #include <config.h>
#endif

#include <iostream>
#include <fstream>
#include <algorithm>

#include "ocaf.h"
#include "InputOutput.h"
#include "OStools.h"
#include "assert.h"

#include <fcntl.h> 

#include <XCAFDoc.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_Location.hxx>

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
#include "GEOMAlgo_Splitter.hxx"
//#include "GEOMAlgo_Gluer.hxx"
#include <TopExp_Explorer.hxx>
#include <TopoDS_Iterator.hxx>

#include <ShapeFix_Shape.hxx>
#include <ShapeFix_Wireframe.hxx>
#include <ShapeAnalysis_Wire.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepTools.hxx>
#include <BRepLib.hxx>
//#include <BRepLib_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopExp.hxx>

#include <BRepAdaptor_Surface.hxx>
#include <BRepAdaptor_HSurface.hxx>
#include <BRepTopAdaptor_TopolTool.hxx>
#include <LocalAnalysis_SurfaceContinuity.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <Geom2d_Curve.hxx>
#include <TopOpeBRepTool_TOOL.hxx>

#include <BRepCheck_Analyzer.hxx>
#include <ShapeAnalysis_ShapeContents.hxx>

#include <ShapeCustom.hxx>
#include <ShapeCustom_TrsfModification.hxx>
#include <BRepTools_Modifier.hxx>

#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomLProp_SLProps.hxx>

#include <BRepMesh.hxx>
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
#include <TColgp_HArray1OfPnt.hxx>

#include <BRepAdaptor_Curve.hxx>
#include <GCPnts_AbscissaPoint.hxx>

#include <omp.h>


#define USEXMLXCAF 0

#ifndef PIG
#define PIG 3.14159265
#endif

#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif
	      
#define CHECK_CREATED_SHAPES 0


ProjectStatus::ProjectStatus(){
   geometry=1;
   partTypes=0;
   partMaterials=0;
   linePorts=1;
}
int ProjectStatus::defined(){return geometry*partTypes*partMaterials*linePorts;}
void ProjectStatus::save(const char*filename){
   FILE *fid= fopen(nativePath(filename).c_str(), "w");
   fprintf(fid, "%d\n", geometry);
   fprintf(fid, "%d\n", partTypes);
   fprintf(fid, "%d\n", partMaterials);
   fprintf(fid, "%d\n", linePorts);
   fclose(fid);
}
int ProjectStatus::read(const char*filename){
   if(!FileExists(filename)) return 0;
   FILE *fid= fopen(nativePath(filename).c_str(), "r");
   fscanf(fid, "%d\n", &geometry);
   fscanf(fid, "%d\n", &partTypes);
   fscanf(fid, "%d\n", &partMaterials);
   fscanf(fid, "%d\n", &linePorts);
   fclose(fid);
   return 1;
}


MwOCAF::MwOCAF(Handle_AIS_InteractiveContext aContext){
   worksaveNeeded=false;
   partitionVolSaveNeeded=false;
   Imax=0;
   indexedShapes=NULL;
   indexedSolids=NULL;
   indexedFaces=NULL;
   indexedEdges=NULL;
   indexedVertices=NULL;
   extFaceNum=0;
   extEdgeNum=0;
   extVertexNum=0;
   shapeIndex2Labels =NULL;
   faceAdjParts=NULL;
   faceAdjBdrCond=NULL;

   theSelectedLabels=NULL;
   edgeData=NULL;
   faceData=NULL;
   vertexData=NULL;
   
   hasDownIF=false;
   hasIF=false;

//   partVertexRenum=NULL;
   hasUPIF=false;
   myContext=aContext;
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
  if(faceAdjBdrCond)       { delete [] faceAdjBdrCond;  faceAdjBdrCond=NULL;}
  if(faceData)             { delete [] faceData;   faceData=NULL;}
  if(edgeData)             { delete [] edgeData;   edgeData=NULL;}
  if(vertexData)           { delete [] vertexData; vertexData=NULL;}
}

bool isCompName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<4) return false;
  TCollection_AsciiString start=name.SubString(1,4);
  start.LowerCase();
  if (start!="cmp") return false;
  const char *str=name.ToCString();
  int l=3;
  for(; l<len && isdigit(str[l]); ) l++;
  if(l==len) return true;
  else return (str[l]=='_' || str[l]=='|' || str[l]=='^');
}


bool isBCondName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<2) return false;
  TCollection_AsciiString start=name.SubString(1,2);
  start.LowerCase();
  if (start!="bc") return false;
  const char *str=name.ToCString();
  int l=2;
  for(; l<len && isdigit(str[l]); ) l++;
  if(l==len) return true;
  else return (str[l]=='_' || str[l]=='|' || str[l]=='^');
}

bool isDielName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<4) return false;
  TCollection_AsciiString start=name.SubString(1,4);
  start.LowerCase();
  if (start!="diel") return false;
  const char *str=name.ToCString();
  int l=4;
  for(; l<len && isdigit(str[l]); ) l++;
  if(l==len) return true;
  else return (str[l]=='_' || str[l]=='|' || str[l]=='^');
}

bool isHoleName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<4) return false;
  TCollection_AsciiString start=name.SubString(1,4);
  start.LowerCase();
  if (start!="hole") return false;
  const char *str=name.ToCString();
  int l=4;
  for(; l<len && isdigit(str[l]); ) l++;
  if(l==len) return true;
  else return (str[l]=='_' || str[l]=='|' || str[l]=='^');
}


bool isSplitName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<5) return false;
  TCollection_AsciiString start=name.SubString(1,5);
  start.LowerCase();
  if (start!="split") return false;
  const char *str=name.ToCString();
  int l=5;
  for(; l<len && isdigit(str[l]); ) l++;
  if(l==len) return true;
  else return (str[l]=='_' || str[l]=='|' || str[l]=='^');
}

bool isGridName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<4) return false;
  TCollection_AsciiString start=name.SubString(1,4);
  start.LowerCase();
  if (start!="grid") return false;
  const char *str=name.ToCString();
  int l=4;
  for(; l<len && isdigit(str[l]); ) l++;
  if(l==len) return true;
  else return (str[l]=='_' || str[l]=='|' || str[l]=='^');
}

bool isWgName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<2) return false;
  TCollection_AsciiString start=name.SubString(1,2);
  start.LowerCase();
  if (start!="wg") return false;
  const char *str=name.ToCString();
  int l=2;
  for(; l<len && isdigit(str[l]); ) l++;
  if(l==len) return true;
  else return (str[l]=='_' || str[l]=='|' || str[l]=='^');
}

bool isLPName(TCollection_AsciiString &name)
{
  int len=name.Length();
  if(len<2) return false;
  TCollection_AsciiString start=name.SubString(1,2);
  start.LowerCase();
  if (start!="lp") return false;
  const char *str=name.ToCString();
  int l=2;
  for(; l<len && isdigit(str[l]); ) l++;
  if(l==len) return true;
  else return (str[l]=='_' || str[l]=='|' || str[l]=='^');
}

bool isValidMultibodyPartName(TCollection_AsciiString &name)
{
 if(isDielName(name))  return true; 
 if(isHoleName(name))  return true; 
 if(isBCondName(name)) return true; 
 if(isGridName(name))  return true; 
 if(isSplitName(name)) return true; 
 if(isWgName(name))    return true; 
 if(isLPName(name))    return true; 
 return false;
}


struct LabelAttributes
{
  bool hasName;
  bool hasLayers;
  bool hasColor;
  bool hasColorL;
  TCollection_AsciiString name;
  Handle_TColStd_HSequenceOfExtendedString layers;
  Quantity_Color color;
  TDF_Label colorL;
  LabelAttributes(){
    hasName=hasLayers=hasColor=hasColorL=false;
  }
};

void copyLabelAttributes( LabelAttributes &attr, TDF_Label &label,
			 Handle(XCAFDoc_ColorTool) colTool, 
			 Handle(XCAFDoc_LayerTool) layTool)
{
 Handle(TDataStd_Name)  nameAtt;
 attr.hasName =label.FindAttribute(TDataStd_Name::GetID(),nameAtt);
 if(attr.hasName) attr.name=nameAtt->Get();
 attr.hasLayers =layTool->GetLayers(label,attr.layers);
 attr.hasColor  =colTool->GetColor(label,XCAFDoc_ColorGen,attr.color);
 attr.hasColorL =colTool->GetColor(label,XCAFDoc_ColorGen,attr.colorL);
}

void pasteLabelAttributes( LabelAttributes &attr, TDF_Label &label,
			 Handle(XCAFDoc_ColorTool) colTool, 
			 Handle(XCAFDoc_LayerTool) layTool)
{
       if(attr.hasName){
           Handle(TDataStd_Name)  nameAtt;
           if(!label.FindAttribute(TDataStd_Name::GetID(),nameAtt)){ 
             nameAtt= new TDataStd_Name();
             label.AddAttribute(nameAtt);
           }
           nameAtt->Set(attr.name);
       }
       if(attr.hasLayers) 
	       for (int i=1; i<=attr.layers->Length(); i++) layTool->SetLayer(label,attr.layers->Value(i));
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

TDF_Label MwOCAF::partsLabel() 
{
  TDF_Label L = XCAFDoc_DocumentTool::DocLabel(mainDoc->Main()).FindChild(6,Standard_False);
  return L;
}

TDF_Label MwOCAF::makePartsLabel() 
{
  TDF_Label L = XCAFDoc_DocumentTool::DocLabel(mainDoc->Main()).FindChild(6,Standard_True);
  return L;
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


TopLoc_Location MwOCAF::location()
{
  TopLoc_Location loc;
  {
     Handle(XCAFDoc_Location) locAtt;
     if(theParts.FindAttribute(XCAFDoc_Location::GetID(),locAtt)) loc=locAtt->Get();
  }
  return loc;
}

void MwOCAF::resetShapeTool(){
  TDF_Label L=XCAFDoc_DocumentTool::ShapesLabel(mainDoc->Main());
  if (L.FindAttribute (XCAFDoc_ShapeTool::GetID(), shapeTool)) L.ForgetAttribute(XCAFDoc_ShapeTool::GetID());
  shapeTool = new XCAFDoc_ShapeTool();
  L.AddAttribute(shapeTool);
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
  bool itis=isComponent(label) || isSolid(label);
  if(itis) itis=itis && EmP.FindVolume(name.ToCString());
  return itis;
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

void MwOCAF::setLabelShape(TDF_Label label, TopoDS_Shape shape){
 shapeTool->SetShape(label,shape);
}


//-----------------------------------------
//Names:

int MwOCAF::setLabelName(TDF_Label item, TCollection_AsciiString name){
  Handle(TDataStd_Name)  nameAtt;
  if(!item.FindAttribute(TDataStd_Name::GetID(),nameAtt)){ 
    nameAtt= new TDataStd_Name();
    item.AddAttribute(nameAtt);
  }
  nameAtt->Set(name);
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

bool MwOCAF::getLabelLayers(TDF_Label label, Handle_TColStd_HSequenceOfExtendedString &layers){
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
      return EmP.FindVolume(name.ToCString());
}

DB::Volume * MwOCAF::getLabelUpperVol(TDF_Label label, DB::EmProblem *upperEmP){
      Handle(TDataStd_Name)  nameAtt;
      if (!label.FindAttribute(TDataStd_Name::GetID(),nameAtt)) return NULL;
      TCollection_AsciiString name=nameAtt->Get();
      return upperEmP->FindVolume(name.ToCString());
}


//--------------------


void MwOCAF::updateAssembly(TDF_Label label){
 if(!label.IsNull()) shapeTool->UpdateAssembly(label);
}

void MwOCAF::setLabelIndex(TDF_Label item, int I){
  Handle(TDataStd_Integer) intAtt = new TDataStd_Integer();
  intAtt->Set(I);
  if (item.IsAttribute(TDataStd_Integer::GetID()))  item.ForgetAttribute(TDataStd_Integer::GetID());
  item.AddAttribute(intAtt);
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

void MwOCAF::assignMatColor(TDF_Label label){
  Handle(TPrsStd_AISPresentation)  prs;
  if(!label.FindAttribute(TPrsStd_AISPresentation::GetID(),prs)) return;
  Quantity_NameOfColor colName=Quantity_NOC_SALMON4;
  DB::Volume *vol=getLabelVol(label);
  if(vol){
     if(vol->type==SPLITTER || vol->type==GRID) colName=Quantity_NOC_SEAGREEN;
     else{
       DB::Material* mat = EmP.FindMaterial(vol->material);
       if(mat){
	 Quantity_Parameter r=mat->color[0]/255.0;
	 Quantity_Parameter g=mat->color[1]/255.0;
	 Quantity_Parameter b=mat->color[2]/255.0;
	 colName=Quantity_Color::Name(r,g,b);
       }
     } 
  }
  if(prs->HasOwnColor()) {
	  if(prs->Color()!=colName) 
	  prs->SetColor(colName);
  
  } else  
	  prs->SetColor(colName);
  prs->Update();
}

void MwOCAF::changeColor(TDF_Label label, Quantity_NameOfColor colName){
  Handle(TPrsStd_AISPresentation)  prs;
  if(!label.FindAttribute(TPrsStd_AISPresentation::GetID(),prs)) return;
  if(prs->HasOwnColor()) {
	  if(prs->Color()!=colName) 
	  prs->SetColor(colName);
  
  } else  
	  prs->SetColor(colName);
  prs->Update();
}



/*
void MwOCAF::assignMatColor(TDF_Label label){
  Quantity_Color col=Quantity_Color(Quantity_NOC_SALMON4);
  DB::Volume *vol=getLabelVol(label);
  if(vol){
      DB::Material* mat = EmP.FindMaterial(vol->material);
      if(mat){
	 Quantity_Parameter r=mat->color[0]/255.0;
	 Quantity_Parameter g=mat->color[1]/255.0;
	 Quantity_Parameter b=mat->color[2]/255.0;
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

Handle(TPrsStd_AISPresentation) MwOCAF::setLabelPresentation(TDF_Label label, Quantity_NameOfColor colName, double transp)
{ 

//  Quantity_Color col(colName);
//  colorTool->SetColor(label, col, XCAFDoc_ColorGen);
//  TDF_Label colorLabel;
//  if(!colorTool->FindColor(col,colorLabel)) colorLabel=colorTool->AddColor(col);

//  if(!colorTool->IsSet(label,XCAFDoc_ColorGen)) assignMatColor(label);

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

/*
  Handle(TPrsStd_AISPresentation)  prs;
  if(!label.FindAttribute(TPrsStd_AISPresentation::GetID(),prs)){ 
	 prs=TPrsStd_AISPresentation::Set(label,XCAFPrs_Driver::GetID());
  }
*/

 if(prs->HasOwnColor()) {if(prs->Color()!=colName) prs->SetColor(colName);}
 else                    prs->SetColor(colName);


//  prs->SetColor(colName);
  prs->SetMode(AIS_Shaded);
  prs->SetSelectionMode(0);
  if(transp>0 && transp<1) prs->SetTransparency(transp);
  prs->Update();
  TPrsStd_AISViewer::Update(mainDoc->GetData()->Root());
  return prs;
}



void MwOCAF::displayLabelShape(TDF_Label label, bool B)
{ 
  if(B) if(isDisabled(label)) return;
  if(!shapeTool->IsShape(label)) return;
  Handle(TPrsStd_AISPresentation)  prs;
  if(!label.FindAttribute(TPrsStd_AISPresentation::GetID(),prs)) prs=setLabelPresentation(label);
  if(prs.IsNull()) return;
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
		     myContext->Display(faceVSMesh);
		 }
          else   myContext->Erase(faceVSMesh);
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
    if(shapeTool->GetShape(root,shape)){
       if(!hasNameAtt){
        nameAtt= new TDataStd_Name();
        root.AddAttribute(nameAtt);
       }
       char cstr[100];
       int itag= root.Tag();
       if (shape.ShapeType() == TopAbs_COMPOUND){
	  if(isAssembly(root))  sprintf(cstr,"REMOVE");
	  else                  sprintf(cstr,"Comp%d",itag);                   
	  TCollection_AsciiString name(cstr);
          if(isAssembly(root)) nameAtt->Set(name);
       }else if (shape.ShapeType() == TopAbs_SOLID){
	  sprintf(cstr,"Solid%d",itag);
          TCollection_AsciiString name(cstr);
          nameAtt->Set(name);
       }else if (shape.ShapeType() == TopAbs_SHELL){
	  sprintf(cstr,"Shell%d",itag);
          TCollection_AsciiString name(cstr);
          nameAtt->Set(name);
       } else if (shape.ShapeType() == TopAbs_WIRE){
	  sprintf(cstr,"Wire%d",itag);
          TCollection_AsciiString name(cstr);
          nameAtt->Set(name);
       } else if (shape.ShapeType() == TopAbs_FACE){
	  sprintf(cstr,"F%d",itag);
          TCollection_AsciiString name(cstr);
          nameAtt->Set(name);
       } else if (shape.ShapeType() == TopAbs_EDGE){
	  sprintf(cstr,"E%d",itag);
          TCollection_AsciiString name(cstr);
          nameAtt->Set(name);
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
  TopoDS_Shape shape;
  if(shapeTool->GetShape(root,shape)){
       if(!hasName){
         nameAtt= new TDataStd_Name();
         root.AddAttribute(nameAtt);
       }
       char cstr[100];
       if (shape.ShapeType() == TopAbs_SOLID){
	  int I=indexedSolids->FindIndex(shape); 
	  sprintf(cstr,"Vol%d",I);
          TCollection_AsciiString name(cstr);
          if(!hasName) nameAtt->Set(name);
       } else if (shape.ShapeType() == TopAbs_FACE){
	  shape.Orientation(TopAbs_FORWARD);
	  int I=indexedFaces->FindIndex(shape); 
	  sprintf(cstr,"F%d",I);
          TCollection_AsciiString name(cstr);
          nameAtt->Set(name);
       } else if (shape.ShapeType() == TopAbs_EDGE){
	  shape.Orientation(TopAbs_FORWARD);
	  int I=indexedEdges->FindIndex(shape); 
	  sprintf(cstr,"E%d",I);
          TCollection_AsciiString name(cstr);
          nameAtt->Set(name);
       } else if (shape.ShapeType() == TopAbs_VERTEX){
	  shape.Orientation(TopAbs_FORWARD);
	  int I=indexedVertices->FindIndex(shape); 
	  sprintf(cstr,"P%d",I);
          TCollection_AsciiString name(cstr);
          nameAtt->Set(name);
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
//  Handle_AIS_InteractiveContext myContext=context->getContext();
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
     displayLabelShape(item, Standard_True);  //recursion;
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
     TDF_Label label1 = it.Value();
     Handle(TDF_Reference)  refAtt;
     assert(label1.FindAttribute(TDF_Reference::GetID(),refAtt));
     TDF_Label label = refAtt->Get();
     displayLabelShape(label, true);
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
     DB::Volume *vol=getLabelVol(label);
     if(vol) skip=(vol->type==WAVEGUIDE || vol->type==BOUNDARYCOND || vol->type==SPLITTER  || vol->type==GRID || vol->type==LINEPORT);
     if(skip) continue;
     TopoDS_Shape S = shapeTool->GetShape(label);
     if(!S.IsNull()){ 
	     if(S.ShapeType()==TopAbs_SOLID) builder.Add(theCompSolid,S);
	     else if(S.ShapeType()==TopAbs_COMPSOLID) 
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
 if(safc2.NbShells()-safc1.NbShells()) cout << "NbShells Diff=" << safc2.NbShells()-safc1.NbShells() <<"\n";
 if(safc2.NbFaces()-safc1.NbFaces())   cout << "NbFaces Diff=" << safc2.NbFaces()-safc1.NbFaces() <<"\n";
 if(safc2.NbWires()-safc1.NbWires())   cout << "NbWires Diff=" << safc2.NbWires()-safc1.NbWires() <<"\n";
 if(safc2.NbEdges()-safc1.NbEdges())   cout << "NbEdges Diff=" << safc2.NbEdges()-safc1.NbEdges() <<"\n";
 if(safc2.NbVertices()-safc1.NbVertices())   cout << "NbVertices Diff=" << safc2.NbVertices()-safc1.NbVertices() <<"\n";
 if(safc2.NbBigSplines()-safc1.NbBigSplines())   cout << "NbBigSplines Diff=" << safc2.NbBigSplines()-safc1.NbBigSplines() <<"\n";
 if(safc2.NbC0Surfaces()-safc1.NbC0Surfaces())   cout << "NbC0Surfaces Diff=" << safc2.NbC0Surfaces()-safc1.NbC0Surfaces() <<"\n";
 if(safc2.NbC0Curves()-safc1.NbC0Curves())   cout << "NbC0Curves Diff=" << safc2.NbC0Curves()-safc1.NbC0Curves() <<"\n";
 if(safc2.NbOffsetSurf()-safc1.NbOffsetSurf())   cout << "NbOffsetSurf Diff=" << safc2.NbOffsetSurf()-safc1.NbOffsetSurf() <<"\n";
 if(safc2.NbIndirectSurf()-safc1.NbIndirectSurf())   cout << "NbIndirectSurf Diff=" << safc2.NbIndirectSurf()-safc1.NbIndirectSurf() <<"\n";
 if(safc2.NbOffsetCurves()-safc1.NbOffsetCurves())   cout << "NbOffsetCurves Diff=" << safc2.NbOffsetCurves()-safc1.NbOffsetCurves() <<"\n";
 if(safc2.NbTrimmedCurve2d()-safc1.NbTrimmedCurve2d())   cout << "NbTrimmedCurve2d Diff=" << safc2.NbTrimmedCurve2d()-safc1.NbTrimmedCurve2d() <<"\n";
 if(safc2.NbTrimmedCurve3d()-safc1.NbTrimmedCurve3d())   cout << "NbTrimmedCurve3d Diff=" << safc2.NbTrimmedCurve3d()-safc1.NbTrimmedCurve3d() <<"\n";
 if(safc2.NbBSplibeSurf()-safc1.NbBSplibeSurf())   cout << "NbBSplibeSurf Diff=" << safc2.NbBSplibeSurf()-safc1.NbBSplibeSurf() <<"\n";
 if(safc2.NbBezierSurf()-safc1.NbBezierSurf())   cout << "NbBezierSurf Diff=" << safc2.NbBezierSurf()-safc1.NbBezierSurf() <<"\n";
 if(safc2.NbTrimSurf()-safc1.NbTrimSurf())   cout << "NbTrimSurf Diff=" << safc2.NbTrimSurf()-safc1.NbTrimSurf() <<"\n";
 if(safc2.NbWireWitnSeam()-safc1.NbWireWitnSeam())   cout << "NbWireWitnSeam Diff=" << safc2.NbWireWitnSeam()-safc1.NbWireWitnSeam() <<"\n";
 if(safc2.NbWireWithSevSeams()-safc1.NbWireWithSevSeams())   cout << "NbWireWithSevSeams Diff=" << safc2.NbWireWithSevSeams()-safc1.NbWireWithSevSeams() <<"\n";
 if(safc2.NbFaceWithSevWires()-safc1.NbFaceWithSevWires())   cout << "NbFaceWithSevWires Diff=" << safc2.NbFaceWithSevWires()-safc1.NbFaceWithSevWires() <<"\n";
 if(safc2.NbNoPCurve()-safc1.NbNoPCurve())   cout << "NbNoPCurve Diff=" << safc2.NbNoPCurve()-safc1.NbNoPCurve() <<"\n";
 if(safc2.NbFreeFaces()-safc1.NbFreeFaces())   cout << "NbFreeFaces Diff=" << safc2.NbFreeFaces()-safc1.NbFreeFaces() <<"\n";
 if(safc2.NbFreeWires()-safc1.NbFreeWires())   cout << "NbFreeWires Diff=" << safc2.NbFreeWires()-safc1.NbFreeWires() <<"\n";
 if(safc2.NbFreeEdges()-safc1.NbFreeEdges())   cout << "NbFreeEdges Diff=" << safc2.NbFreeEdges()-safc1.NbFreeEdges() <<"\n";
 if(safc2.NbSharedShells()-safc1.NbSharedShells())   cout << "NbSharedShells Diff=" << safc2.NbSharedShells()-safc1.NbSharedShells() <<"\n";
 if(safc2.NbSharedFaces()-safc1.NbSharedFaces())   cout << "NbSharedFaces Diff=" << safc2.NbSharedFaces()-safc1.NbSharedFaces() <<"\n";
 if(safc2.NbSharedWires()-safc1.NbSharedWires())   cout << "NbSharedWires Diff=" << safc2.NbSharedWires()-safc1.NbSharedWires() <<"\n";
 if(safc2.NbSharedEdges()-safc1.NbSharedEdges())   cout << "NbSharedEdges Diff=" << safc2.NbSharedEdges()-safc1.NbSharedEdges() <<"\n";
 if(safc2.NbSharedVertices()-safc1.NbSharedVertices())   cout << "NbSharedVertices Diff=" << safc2.NbSharedVertices()-safc1.NbSharedVertices() <<"\n";
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
     cout<< "Some edges in the wire need to be reordered"<<endl;
     cout<<"Please ensure that all the edges are correctly ordered before further analysis"<<endl;
     return;
    }
    if (saw.CheckSmall (precision)) 
      cout<<"Wire contains edge(s) shorter than "<<precision<<endl;
    if (saw.CheckConnected())
      cout<<"Wire is disconnected"<<endl;
    if (saw.CheckSelfIntersection())
      cout<<"Wire has self-intersecting or intersecting adjacent edges" << endl;
   }
  }

}

void MwOCAF::heal(Handle(ShapeBuild_ReShape) &reshape){

//----
//  initial healing:
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
  Handle(ShapeFix_Shape)     SFS= new ShapeFix_Shape();
  reshape = new ShapeBuild_ReShape();
  reshape->Apply(healedShape);
  SFS->Init(healedShape);
  SFS->SetContext(reshape);
  SFS->SetPrecision(1.e-4);
  SFS->SetMaxTolerance(1.e-4);
  SFS->FixWireTool()->FixRemovePCurveMode() =1;
  SFS->FixWireTool()->FixReorderMode()=1,
  SFS->FixWireTool()->FixSmallMode() =1;
  SFS->FixWireTool()->FixConnectedMode()=1,
  SFS->FixWireTool()->ModifyTopologyMode() =false;
  SFS->FixFaceTool()->FixSmallAreaWireMode() =1;
  SFS->Perform();

  Handle(ShapeFix_Wireframe) SFW = new ShapeFix_Wireframe;
  SFW->SetContext(reshape);
  SFW->Load(SFS->Shape());
  SFW->SetPrecision(1.e-4);
  SFW->SetMaxTolerance(1.e-4);
  SFW->ModeDropSmallEdges() = Standard_True;
  SFW->FixSmallEdges();
  SFW->FixWireGaps();
/*
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
           TopoDS_Shape newS=reshape->Apply(S);
           if(!newS.IsNull() && !newS.IsEqual(S)){ 
	     TDF_Label newlabel=replaceLabelShape(label,newS);
	     displayLabelShape(newlabel, true);
	     refAtt->Set(newlabel);
           } 
     } 
  }


}




TDF_Label MwOCAF::replaceLabelShape(TDF_Label label, TopoDS_Shape newS){
             Handle(TDataStd_Name)  nameAtt;
	     bool hasName =label.FindAttribute(TDataStd_Name::GetID(),nameAtt);
             Handle_TColStd_HSequenceOfExtendedString layers;
	     bool hasLayers=layerTool->GetLayers(label,layers);
             Quantity_Color color;
	     TDF_Label colorL;
	     XCAFDoc_ColorType ctype=XCAFDoc_ColorGen;
	     bool hasColor =colorTool->GetColor(label,ctype,color);
	     bool hasColorL=colorTool->GetColor(label,ctype,colorL);
             Handle(TPrsStd_AISPresentation)  prs;
	     bool hasprs=label.FindAttribute(TPrsStd_AISPresentation::GetID(),prs);
	     TDF_Label newlabel;
	     TDF_Label father=label.Father();
	     displayLabelShape(label, false);
             if(CHECK_CREATED_SHAPES) if(newS.ShapeType()==TopAbs_SOLID|| newS.ShapeType()==TopAbs_COMPSOLID){
	        BRepCheck_Analyzer ana (newS, true);
	        assert(ana.IsValid());
	     }
             if(isComponent(label)){
		  removeWithSubComponents(label);
		  newlabel=shapeTool->AddComponent(father,newS); 
	     } else{
		  shapeTool->RemoveShape(label);
	          newlabel=shapeTool->AddShape(newS);
	     }
	     if(!newlabel.IsNull()){
                  if(hasName){
                     Handle(TDataStd_Name)  newNameAtt;
                     assert(newlabel.FindAttribute(TDataStd_Name::GetID(),newNameAtt));
                     newNameAtt->Set(nameAtt->Get());
		  }
	          if(hasLayers) for (int i = 1; i <= layers->Length(); i++)
	                              layerTool->SetLayer(newlabel,layers->Value(i));
	          if(hasColor)  colorTool->SetColor(newlabel,color,ctype);
	          if(hasColorL) colorTool->SetColor(newlabel,colorL,ctype);
	     }
	     updateAssembly(father);
             if(hasprs) setLabelPresentation(newlabel,prs->Color(), prs->Transparency());
	     else       setLabelPresentation(newlabel);
	     return newlabel;
}



void MwOCAF::imprint(){

 if(hasUPIF || numberOfParts()>=1){ // else skip all imprinting:

// Define the imprinting parts
 TopTools_IndexedMapOfShape  imprintingParts;
 assert(hasParts());
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
     TDF_Label label1 = it.Value();
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
     TDF_Label label = refAtt->Get();
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue;
     if(vol->type==SPLITTER || vol->type==GRID) continue;
     if(!(thePartShapes.IsBound(label1.Tag()))){
        TopoDS_Shape S= shapeTool->GetShape(label);
        assert(!S.IsNull());
        if(S.ShapeType()==TopAbs_COMPOUND){
	   if(vol->type==WAVEGUIDE || vol->type==BOUNDARYCOND ){
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
        thePartShapes.Bind(label1.Tag(),S);
     }
//     S.Location (loc.Multiplied(S.Location()));
     imprintingParts.Add(thePartShapes(label1.Tag()));
  }


//  Imprinting:
  GEOMAlgo_Splitter PS;
//  PS.SetLimitMode(1);
  PS.SetLimit(TopAbs_COMPOUND);
 
  TopTools_MapOfShape ShapesMap, ToolsMap;
  for (int I=1; I<=imprintingParts.Extent(); I++){
        TopoDS_Shape S =imprintingParts.FindKey(I);
	if (ShapesMap.Add(S)) PS.AddArgument(S);
  }
  if(hasUPIF) for (int I = 1; I <=UPIFsubshapes.Extent() ; I++){
       TopoDS_Shape S =UPIFsubshapes.FindKey(I);
       if (!ShapesMap.Contains(S) && ToolsMap.Add(S)) PS.AddTool(S);
  }

  try {
    PS.Perform();
  } catch ( Standard_Failure ) {
	  throw ImprintFailure();
  }

  const BOPCol_IndexedDataMapOfShapeListOfShape& PSmap=PS.ImagesResult();

//  Replace imprinting result in UPIFsubshapes:
  if(hasUPIF){
    for (int I = 1; I <=UPIFsubshapes.Extent() ; I++)
    {
       TopoDS_Shape S =UPIFsubshapes.FindKey(I);
       TopoDS_Shape newS=S;
       const BOPCol_ListOfShape& RSlist = PSmap.FindFromKey(S);
       if(!RSlist.IsEmpty()){
         assert(RSlist.Extent()==1);
         BOPCol_ListIteratorOfListOfShape RSit(RSlist);
	 newS=RSit.Value();
	 UPIFsubshapes.Substitute(I,newS);
       }
    }
  }

//-----------------------------------------------------------------------------
//  Replace original parts with the imprinted parts:
  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
  {
     TDF_Label label1 = it.Value();
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
     DB::Volume *vol=getLabelVol(label1);
     if(vol) if(vol->type==SPLITTER || vol->type==GRID) continue;
//---
     Handle(TDataStd_Name)  nameAtt;
     TCollection_AsciiString name;
     if(label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) name=nameAtt->Get();
//---
     TDF_Label label = refAtt->Get();
     TopoDS_Shape S=thePartShapes(label1.Tag());
/*
     Handle(TNaming_NamedShape) NS;
     assert(label1.FindAttribute(TNaming_NamedShape::GetID(), NS) );
     TopoDS_Shape S = TNaming_Tool::GetShape(NS);
*/
     TopoDS_Shape S0 = shapeTool->GetShape(label);
/*
     Handle(XCAFDoc_Location) locAtt;
     assert(label1.FindAttribute(XCAFDoc_Location::GetID(),locAtt));
     TopLoc_Location loc=locAtt->Get();
     newS.Location (loc.Multiplied(S.Location()));
*/
     TopoDS_Shape newS=S;
     if(!newS.IsNull()) if(PSmap.Contains(S)) {
          const BOPCol_ListOfShape& RSlist = PSmap.FindFromKey(S);
//          const TopTools_ListOfShape& RSlist = PS.Modified(S);
          if(!RSlist.IsEmpty()){
              BOPCol_ListIteratorOfListOfShape RSit(RSlist);
//              TopTools_ListIteratorOfListOfShape RSit(RSlist);
	      newS=RSit.Value();
          }
     }
//Replace Shape with new shape:
     if(!newS.IsNull() && !newS.IsEqual(S0)){ 
	    TDF_Label newlabel=replaceLabelShape(label,newS);
	    displayLabelShape(newlabel, true);
	    refAtt->Set(newlabel);
     } 
  }

} //skip all imprinting:

  if(!hasUPIF){
//     makeTheCompSolid();
//     makeSolid(theCompSolid,theMergedSolid);
     makeIF(theIF);
     if(EmP.assemblyType==COMPONENT) makeSplitFaces();
  }

   resetShapeTool();

   generateSubshapes();
   newIndexedSubShapes();
   makeIF_IndexedSubShapes();
   makePartsIndexedSubShapes();
   setGlobalNames();
  
}


void MwOCAF::updatePartColors(){
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
   TDF_Label label1 = it.Value();
   DB::Volume *vol=getLabelVol(label1);
   if(vol) if(vol->type==DIELECTRIC || vol->type==HOLE || vol->type==BOUNDARYCOND || vol->type==WAVEGUIDE || vol->type==SPLITTER ){
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
//     label.ForgetAttribute(TPrsStd_AISPresentation::GetID());
     displayLabelShape(label, B);
   }
 }
}

void MwOCAF::updatePartitionVolumes(DB::EmProblem *upperEmP){
 if(hasParts()) for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
   TDF_Label label1 = it.Value();
   DB::Volume *vol=getLabelVol(label1);
   DB::Volume *upvol=getLabelUpperVol(label1,upperEmP);
   if(vol) if(vol->type==DIELECTRIC || vol->type==HOLE || vol->type==WAVEGUIDE || vol->type==BOUNDARYCOND ){
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
     TDF_Label label = refAtt->Get();
     if(upvol) if(strcmp(vol->material,upvol->material) || 
		  fabs(vol->meshRefinement-upvol->meshRefinement)>1.e-5)
     {
       strcpy(vol->material,upvol->material);
       vol->meshRefinement=upvol->meshRefinement;
       TopoDS_Shape S = shapeTool->GetShape(label);
       if(S.IsNull()) continue;
       if(S.ShapeType()==TopAbs_COMPSOLID){
        for (TDF_ChildIterator it2(label,Standard_False); it2.More(); it2.Next()){
          TDF_Label cellLabel = it2.Value();
          DB::Volume *cellvol=getLabelVol(cellLabel); 
          strcpy(cellvol->material,upvol->material);
          cellvol->meshRefinement=upvol->meshRefinement;
        }
       }
       partitionVolSaveNeeded=true;
       worksaveNeeded=true;
     }
   }
 }
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
	 if(vol->type==SPLITTER)  makeSplitFaces(label1);
         else if(vol->type==GRID) makeGridFaces(label1);
   }
 }
}

bool MwOCAF::updateSplitPlanes(){
 evalBBox();
 bool changed=false;
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
   TDF_Label label1 = it.Value();
   DB::Volume *vol=getLabelVol(label1);
   if(vol) if(vol->type==GRID) {
     TopoDS_Shape S1= shapeTool->GetShape(label1);
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

void MwOCAF::makeSplitFaces(TDF_Label label1){
     double BBX[2],BBY[2],BBZ[2];
     BBox.Get(BBX[0], BBY[0], BBZ[0], BBX[1], BBY[1], BBZ[1]);
     gp_Pnt CP = gp_Pnt((BBX[0]+BBX[1])/2,
		     (BBY[0]+BBY[1])/2,
		     (BBZ[0]+BBZ[1])/2
		    );
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) return;
     TDF_Label label = refAtt->Get();
     DB::Volume *vol=getLabelVol(label1);
     TopoDS_Shape S= shapeTool->GetShape(label1);
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
     TDF_Label newlabel=replaceLabelShape(label,newS);
     refAtt->Set(newlabel);
     setLabelPresentation(newlabel,Quantity_NOC_SEAGREEN, 0.5);
     displayLabelShape(newlabel, true);
     S= shapeTool->GetShape(newlabel); assert(S.IsEqual(newS));
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
     BRepAdaptor_Curve C(TopoDS::Edge(E));
     double Elen = GCPnts_AbscissaPoint::Length (C);
     int planeNum=Elen/vol->gridSpace-0.7;
     planeNum=max(0,planeNum);
     TopoDS_Shape S0= shapeTool->GetShape(label);
     TopTools_IndexedMapOfShape faces0;
     TopExp::MapShapes(S0,TopAbs_FACE,faces0);
     if (faces0.Extent()==planeNum)  return false;
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
     TopTools_IndexedMapOfShape splitVertices;
     Standard_Real X1, Y1, Z1;
     P1.Coord (X1, Y1, Z1);
     Standard_Real X2, Y2, Z2;
     P2.Coord (X2, Y2, Z2);
     for (int VI=1; VI<=planeNum; VI++){
         Standard_Real X=X1+(X2-X1)*VI/(planeNum+1);
         Standard_Real Y=Y1+(Y2-Y1)*VI/(planeNum+1);
         Standard_Real Z=Z1+(Z2-Z1)*VI/(planeNum+1);
         gp_Pnt P(X,Y,Z);
         TopoDS_Vertex V = BRepBuilderAPI_MakeVertex(P);
         splitVertices.Add(V);
     }
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




void MwOCAF::split(){
 TopTools_IndexedMapOfShape  originalParts;
 assert(hasParts());
 bool hasSplitter=false;
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
     TDF_Label label1 = it.Value();
     DB::Volume *vol=getLabelVol(label1);
     hasSplitter=hasSplitter|| vol->type==SPLITTER || vol->type==GRID;
 }
//----------------------------------------------------------------------------------
 if(hasSplitter){


// BRepLib::Precision(1.e-4);
 GEOMAlgo_Splitter PS;
// PS.SetTolerance(1.e-4);
// PS.SetLimitMode(1);
 PS.SetLimit(TopAbs_COMPOUND);

 TopTools_MapOfShape ShapesMap, ToolsMap;

 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
     TDF_Label label1 = it.Value();
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
     TDF_Label label = refAtt->Get();
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue; 
     TopoDS_Shape S= shapeTool->GetShape(label);
     assert(!S.IsNull());
     if(vol->type==DIELECTRIC || vol->type==HOLE) assert(S.ShapeType()==TopAbs_SOLID);
     if(vol->type==WAVEGUIDE) assert(S.ShapeType()==TopAbs_SHELL || S.ShapeType()==TopAbs_FACE);
     if(!S.IsNull()) if(vol->type!=SPLITTER && vol->type!=GRID)  if (ShapesMap.Add(S)) PS.AddArgument(S);
  }
  TopTools_IndexedMapOfShape  upIndexedFaces;
  for (int FI=1; FI<=indexedFaces->Extent(); FI++){
      TopoDS_Shape F = indexedFaces->FindKey(FI);
      if (ShapesMap.Add(F)) {
         upIndexedFaces.Add(F);
	 PS.AddArgument(F);
      }
  }
  TopTools_IndexedMapOfShape  upIndexedEdges;
  for (int EI=1; EI<=indexedEdges->Extent(); EI++){
      TopoDS_Shape E = indexedEdges->FindKey(EI);
      if (ShapesMap.Add(E)) {
        upIndexedEdges.Add(E);
	PS.AddArgument(E);
      }
  }
  TopTools_IndexedMapOfShape  upIndexedVertices;
  for (int PI=1; PI<=indexedVertices->Extent(); PI++){
      TopoDS_Shape P = indexedVertices->FindKey(PI);
      if (ShapesMap.Add(P)){
         upIndexedVertices.Add(P);
	 PS.AddArgument(P);
      }
  }

  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
     TDF_Label label1 = it.Value();
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
     TDF_Label label = refAtt->Get();
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue; 
     TopoDS_Shape S= shapeTool->GetShape(label);
     assert(!S.IsNull());
     if(!S.IsNull()) if(vol->type==SPLITTER || vol->type==GRID)
		              for (TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()) 
				      if (!ShapesMap.Contains(exp.Current()) && ToolsMap.Add(exp.Current())) PS.AddTool(exp.Current());
  }

  try {
     PS.Perform();
  } catch ( Standard_Failure ) {throw ImprintFailure();}

  const BOPCol_IndexedDataMapOfShapeListOfShape& PSmap=PS.ImagesResult();

//-----------------------------------------------------------------------------
//  Replace original parts with the splitted parts:
  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
  {
     TDF_Label label1 = it.Value();
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
//---
     Handle(TDataStd_Name)  nameAtt;
     if(label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)){
      TCollection_AsciiString name=nameAtt->Get();
     }
//---
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue;
     TDF_Label label = refAtt->Get();
     if(vol->type==SPLITTER || vol->type==GRID) {
	TDF_Label father=label.Father();
        removeWithSubComponents(label);
	updateAssembly(father);
	EmP.delVolume(vol);
	continue;
     }
     TopoDS_Shape S = shapeTool->GetShape(label);
//     TopoDS_Shape S = shapeTool->GetShape(label1);
     if(!S.IsNull()) if(PSmap.Contains(S)) {
      const BOPCol_ListOfShape &RSlist = PS.ImagesResult().FindFromKey(S);
     if(!RSlist.IsEmpty()) {
//   takes notice of the data stored in old Solid
//           S1<< the healed Solid
//	     TopoDS_Shape S1=S;
// 	     if(Context->IsRecorded(S)) S1=Context->Value(S);
//Builds the new shape in case shape is Solid
	     TopoDS_Shape newS;
	     if(S.ShapeType()==TopAbs_SOLID){
                TopoDS_CompSolid  newCompS;
                BRep_Builder builder;
                builder.MakeCompSolid(newCompS);
//              builder.MakeCompound(newCompS);
		int soNum=0;
		int shNum=0;
	        for (BOPCol_ListIteratorOfListOfShape RSit(RSlist); RSit.More(); RSit.Next()){
                  TopoDS_Shape Si=RSit.Value();
                  shNum++;
	  	  if(Si.ShapeType()==TopAbs_SOLID){
		      if(CHECK_CREATED_SHAPES){
			 BRepCheck_Analyzer ana (Si, true);
			 assert(ana.IsValid());
		      }
		      builder.Add(newCompS,Si); soNum++; 
		  }
                }
	        newS=newCompS;
	     } else if(S.ShapeType()==TopAbs_FACE  ){
                 TopoDS_Shell  newShell;
                 BRep_Builder builder;
                 builder.MakeShell(newShell);
	         for (BOPCol_ListIteratorOfListOfShape RSit(RSlist); RSit.More(); RSit.Next()){
	              if(RSit.Value().ShapeType()==TopAbs_FACE) builder.Add(newShell,TopoDS::Face(RSit.Value()));
                 }
	         newS =newShell;
             } else  {
		 assert(RSlist.Extent()==1);
		 BOPCol_ListIteratorOfListOfShape RSit(RSlist);
		 newS =RSit.Value();
	     }
//Replace Shape with new shape:
             if(!newS.IsNull()){ 
	        TDF_Label newlabel=replaceLabelShape(label,newS);
	        displayLabelShape(newlabel, true);
	        refAtt->Set(newlabel);
             } 
      }
      }
  }

//  resetShapeTool();
//  Handle(ShapeBuild_ReShape) reshape;
//  heal(reshape);
//  checkWires();
  hasUPIF=false;
  makeIF(theIF);
//  makeTheCompSolid();
//  makeSolid(theCompSolid,theMergedSolid);
  resetShapeTool();
  generateSubshapes();
  newIndexedSubShapes();
  makeIF_IndexedSubShapes();
  makePartsIndexedSubShapes();
  setGlobalNames();

//  inserts new Solids into database
  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
  {
     TDF_Label label1 = it.Value();
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue;
     TDF_Label label = refAtt->Get();
     if(vol->type!=DIELECTRIC) continue;
     TopoDS_Shape S = shapeTool->GetShape(label);
     if(S.IsNull()) continue;
     if(S.ShapeType()==TopAbs_COMPSOLID){
       for (TDF_ChildIterator it2(label,Standard_False); it2.More(); it2.Next()){
          TDF_Label cellLabel = it2.Value();
          TopoDS_Shape S2 = shapeTool->GetShape(cellLabel);
          if(S2.IsNull()) continue;
	  if(S2.ShapeType()!=TopAbs_SOLID) continue;
          if(CHECK_CREATED_SHAPES){
		BRepCheck_Analyzer ana (S2, true);
	        assert(ana.IsValid());
	  }
          TCollection_AsciiString cellName;
          if(getLabelName(cellLabel, cellName)){
	    DB::Volume *cellvol = new DB::Volume();
	    *cellvol=*vol;
	    strcpy(cellvol->name,cellName.ToCString()); cellvol->compSolid=0;
	    EmP.insertVolume(cellvol);
          }
        }
        vol->compSolid=1;
     }
  }

  int FNum=indexedFaces->Extent();
  splitFacesMap = new TColStd_HArray1OfInteger(1,FNum);
  for (int FI=1; FI<=FNum; FI++) splitFacesMap->SetValue(FI,0);
  int upFNum=upIndexedFaces.Extent();
  int upENum=upIndexedEdges.Extent();
  int upPNum=upIndexedVertices.Extent();
  for (int upFI=1; upFI<=upFNum; upFI++){
      TopoDS_Shape F = upIndexedFaces.FindKey(upFI);
      const BOPCol_ListOfShape& RSlist = PS.ImagesResult().FindFromKey(F);
      if(!RSlist.IsEmpty()){
	  for (BOPCol_ListIteratorOfListOfShape RSit(RSlist); RSit.More(); RSit.Next()){
//	       if (reshape->Status(RSit.Value(), Fi, Standard_True)>=0){
                 TopoDS_Shape Fi=RSit.Value();
	         Fi.Orientation(TopAbs_FORWARD);
	         if(indexedFaces->Contains(Fi)){
		   int FI = indexedFaces->FindIndex(Fi);
                   splitFacesMap->SetValue(FI,upFI);
	         }
//	       }
          }
      }
  }
  int ENum=indexedEdges->Extent();
  splitEdgesMap = new TColStd_HArray1OfInteger(1,ENum);
  for (int EI=1; EI<=ENum; EI++) splitEdgesMap->SetValue(EI,0);
  for (int upEI=1; upEI<=upENum; upEI++){
      TopoDS_Shape E = upIndexedEdges.FindKey(upEI);
      const BOPCol_ListOfShape& RSlist = PS.ImagesResult().FindFromKey(E);
      if(!RSlist.IsEmpty()){
	  for (BOPCol_ListIteratorOfListOfShape RSit(RSlist); RSit.More(); RSit.Next()) 
//		if (reshape->Status(RSit.Value(), Ei, Standard_True)>=0)
	  {
	      TopoDS_Shape Ei=RSit.Value();
	      Ei.Orientation(TopAbs_FORWARD);
	      if(indexedEdges->Contains(Ei)){
		   int EI = indexedEdges->FindIndex(Ei);
                   splitEdgesMap->SetValue(EI,upEI);
	      }
          }
      }
  }
  int PNum=indexedVertices->Extent();
  splitVerticesMap = new TColStd_HArray1OfInteger(1,PNum);
  for (int PI=1; PI<=PNum; PI++) splitVerticesMap->SetValue(PI,0);
  for (int upPI=1; upPI<=upPNum; upPI++){
      TopoDS_Shape P = upIndexedVertices.FindKey(upPI);
      const BOPCol_ListOfShape& RSlist = PS.ImagesResult().FindFromKey(P);
      if(!RSlist.IsEmpty()){
	  for (BOPCol_ListIteratorOfListOfShape RSit(RSlist); RSit.More(); RSit.Next())
//		  if (reshape->Status(RSit.Value(), Pi, Standard_True)>=0)
	  {
	      TopoDS_Shape Pi=RSit.Value();
	      Pi.Orientation(TopAbs_FORWARD);
	      if(indexedVertices->Contains(Pi)){
		   int PI = indexedVertices->FindIndex(Pi);
                   splitVerticesMap->SetValue(PI,upPI);
	      }
          }
      }
  }


  } else{  //no splitter
    int FNum=indexedFaces->Extent();
    int ENum=indexedEdges->Extent();
    int PNum=indexedVertices->Extent();
    splitFacesMap = new TColStd_HArray1OfInteger(1,FNum);
    splitEdgesMap = new TColStd_HArray1OfInteger(1,ENum);
    splitVerticesMap = new TColStd_HArray1OfInteger(1,PNum);
    for (int FI=1; FI<=FNum; FI++) splitFacesMap->SetValue(FI,FI);
    for (int EI=1; EI<=ENum; EI++) splitEdgesMap->SetValue(EI,EI);
    for (int PI=1; PI<=PNum; PI++) splitVerticesMap->SetValue(PI,PI);
  }
//----------------------------------------------------------------------------------


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
     TDF_Label label1 = it.Value();
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) return;
     TDF_Label label = refAtt->Get();
     makeIndexedShapes(label);
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
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) return;
     TDF_Label label = refAtt->Get();
     bool skip=false;
     if(EmP.assemblyType==COMPONENT){
      DB::Volume *vol=getLabelVol(label);
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



int labelIndex(TDF_Label label){
 Handle(TDataStd_Name)  nameAtt;
 if(!label.FindAttribute(TDataStd_Name::GetID(),nameAtt)) return 0;
 TCollection_AsciiString name=nameAtt->Get();
 const char* cstr=name.ToCString();
 int cstrlen=strlen(cstr);
 int i; for(i=cstrlen-1; i>0&&isdigit(cstr[i]); ) i--;
 int n=0; if (i<cstrlen-1) sscanf (&cstr[i+1],"%d",&n);
 return n;
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
    Handle(TDF_Reference)  refAtt;
    if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) return;
    TDF_Label label = refAtt->Get();
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
      TDF_Label label1 = it.Value();
      Handle(TDF_Reference)  refAtt;
      if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) return;
      TDF_Label label = refAtt->Get();
      makeShapeIndex2Labels(label);
   }
}

void MwOCAF::makeFaceAdjCells(TDF_Label label){
    TopoDS_Shape S = shapeTool->GetShape(label);
    Handle(TDataStd_Name)  nameAtt;
    if(!label.FindAttribute(TDataStd_Name::GetID(),nameAtt)) return;
    TCollection_AsciiString volName=nameAtt->Get();
    DB::Volume *vol=getLabelVol(label);
    TCollection_AsciiString assName; getAssName(assName);
    TCollection_AsciiString partName;
    if(vol->type==WAVEGUIDE) partName=assName+TCollection_AsciiString("__")+volName;
    else                     partName=volName;
    if(EmP.assemblyType==NET || vol->type==DIELECTRIC || vol->type==WAVEGUIDE)  
    for (TopExp_Explorer it(S.Oriented(TopAbs_FORWARD),TopAbs_FACE); it.More(); it.Next()){
            TopoDS_Shape F = it.Current();
            int si=(F.Orientation()==TopAbs_FORWARD) ? 1 : 0; //first part is where face normal enters
            F.Orientation(TopAbs_FORWARD);
            int FI=indexedFaces->FindIndex(F);
            if(FI){
		 if(S.ShapeType()==TopAbs_SOLID) assert(faceAdjParts[2*(FI-1)+si]==TCollection_AsciiString("-"));
		 if(faceAdjParts[2*(FI-1)+si]!=TCollection_AsciiString("-")) si=1-si;
		 faceAdjParts[2*(FI-1)+si]=partName;
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
      Handle(TDF_Reference)  refAtt;
      if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
      TDF_Label label = refAtt->Get();
      Handle(TDataStd_Name)  nameAtt;
      if(!label.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
      TopoDS_Shape S = shapeTool->GetShape(label);
      if(S.IsNull())  continue;
      if(EmP.assemblyType==PARTITION && S.ShapeType()==TopAbs_COMPSOLID){
          for (TDF_ChildIterator it2(label,Standard_False); it2.More(); it2.Next()){
              TDF_Label solidL = it2.Value();
              TopoDS_Shape S2 = shapeTool->GetShape(solidL);
	      makeFaceAdjCells(solidL);
	  }
      } else if(S.ShapeType()==TopAbs_SOLID) makeFaceAdjCells(label);
   }
   if(EmP.assemblyType==NET) return;
// then waveguides are processed
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
      TDF_Label label1 = it.Value();
      Handle(TDF_Reference)  refAtt;
      if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
      TDF_Label label = refAtt->Get();
      Handle(TDataStd_Name)  nameAtt;
      if(!label.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
      DB::Volume *vol=getLabelVol(label);
      if(!vol) continue; 
      TopoDS_Shape S = shapeTool->GetShape(label);
      if(S.IsNull())  continue;
      if(vol->type==WAVEGUIDE)  makeFaceAdjCells(label);
   }

}


void MwOCAF::makeFaceAdjBdrCond(TDF_Label label){
    TopoDS_Shape S = shapeTool->GetShape(label);
    if(S.IsNull())  return;
    DB::Volume *vol=getLabelVol(label);
    for (TopExp_Explorer exp(S.Oriented(TopAbs_FORWARD),TopAbs_FACE); exp.More(); exp.Next()){
            TopoDS_Shape F = exp.Current();
            F.Orientation(TopAbs_FORWARD);
            int FI=indexedFaces->FindIndex(F);
            if(FI) if(vol->type==BOUNDARYCOND) faceAdjBdrCond[FI-1]=TCollection_AsciiString(vol->material);
            if(FI) if(vol->type==WAVEGUIDE)    faceAdjBdrCond[FI-1]=TCollection_AsciiString("WAVEGUIDE");
    }
}


void MwOCAF::makeFaceAdjBdrCond(){
   int FNum=indexedFaces->Extent();
   if(faceAdjBdrCond)  delete [] faceAdjBdrCond; 
   faceAdjBdrCond=new TCollection_AsciiString[FNum];
   for (int i = 0; i < FNum; i++)   faceAdjBdrCond[i]=TCollection_AsciiString("-");
// internally defined conditions;
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
      TDF_Label label1 = it.Value();
      Handle(TDF_Reference)  refAtt;
      if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
      TDF_Label label = refAtt->Get();
      DB::Volume *vol=getLabelVol(label); 
      if(vol) if(vol->type==BOUNDARYCOND || vol->type==WAVEGUIDE) makeFaceAdjBdrCond(label);
   }
// externally defined conditions 
   int *fmap=new int[FNum]; for (int FI =1; FI <= FNum; FI++) fmap[FI-1]=FI;
   if(EmP.assemblyType==PARTITION){
    std::string splitFaceFileName=projectDir+std::string("/partition.map");
    FILE *mapf=fopen(nativePath(splitFaceFileName).c_str(), "r");
    if(!mapf) return;
    int FNum_;
    fscanf(mapf, "%d", &FNum_); char cstr[100]; fgets(cstr,100,mapf); assert(FNum_==FNum);
    int I, UFI;
    for (int FI =1; FI <= FNum; FI++){ fscanf(mapf, "%d %d", &I, &UFI); fmap[FI-1]=UFI;}
    fclose(mapf);
   }
   std::map<int, std::string>  UFbdrc;
   for (int FI =1; FI <= FNum; FI++) if(fmap[FI-1]){
      int UFI=fmap[FI-1];
      if(UFbdrc.find(UFI)==UFbdrc.end() ){
	   char tag[10]; sprintf(tag,"%d",UFI);
           std::string SPath=projectDir+std::string("/../interfaces/F")+tag;
           SPath=nativePath(SPath);
           std::ifstream in;
           in.open(SPath.c_str());
	   if(!in){
             cout<< "missing file :"<< SPath.c_str() <<endl;
	     assert(false);
 	   }
	   FaceData fd; fd.read(in);
           std::string str;
           bool hasBdrCond=fd.BrCond.size()>0;
           in.close();
	   if(hasBdrCond) UFbdrc[UFI]=*fd.BrCond.begin();
	   else           UFbdrc[UFI]="-";
       }
       if(UFbdrc[UFI]!="-") faceAdjBdrCond[FI-1]=TCollection_AsciiString(UFbdrc[UFI].c_str());
   }
   delete [] fmap;
}



void MwOCAF::setFEproperties(){
   int FNum=indexedFaces->Extent();
   int ENum=indexedEdges->Extent();
   int PNum=indexedVertices->Extent();
   if(!faceData) faceData=new FaceData[FNum];
   if(!edgeData) edgeData=new EdgeData[ENum];
   if(!vertexData) vertexData=new VertexData[PNum];

   for (int FI = 1; FI <= FNum; FI++)  {
	   faceData[FI-1].level=EmP.level;
   }
   for (int EI = 1; EI <= ENum; EI++)  {
	   edgeData[EI-1].level=EmP.level;
   }
   for (int PI = 1; PI <= PNum; PI++)  {
	   vertexData[PI-1].level=EmP.level;
   }
   if(faceAdjBdrCond) for (int FI =1; FI <= FNum; FI++) if(faceAdjBdrCond[FI-1]!="-"){
        TopoDS_Shape F = indexedFaces->FindKey(FI);
        for (TopExp_Explorer exp(F,TopAbs_EDGE); exp.More(); exp.Next()){
          TopoDS_Shape E = exp.Current();
	  E.Orientation(TopAbs_FORWARD);
          int EI=indexedEdges->FindIndex(E);
          if(EI>0){
	        std::string str=std::string(faceAdjBdrCond[FI-1].ToCString());
	        if(edgeData[EI-1].BrCond.find(str)==edgeData[EI-1].BrCond.end()){
		     edgeData[EI-1].BrCond.insert(str);
	        }
	  }
        }
   }
// externally defined PEC/PMC edges 
   int *emap=new int[ENum]; for (int EI =1; EI <= ENum; EI++) emap[EI-1]=EI;
   int *fmap=new int[FNum]; for (int FI =1; FI <= FNum; FI++) fmap[FI-1]=FI;
   int *pmap=new int[PNum]; for (int PI =1; PI <= PNum; PI++) pmap[PI-1]=PI;

   int UFNum=FNum;
   int UENum=ENum;
   int UPNum=PNum;
   if(EmP.assemblyType==PARTITION) {
    std::string splitFaceFileName=projectDir+std::string("/partition.map");
    FILE *mapf=fopen(nativePath(splitFaceFileName).c_str(), "r");
    if(!mapf) return;
    char cstr[100];
    int FNum_, ENum_, PNum_, I, UFI, UEI, UPI;
    fscanf(mapf, "%d", &FNum_);  fgets(cstr,100,mapf); assert(FNum_==FNum);
    UFNum=0;
    for (int FI =1; FI <= FNum; FI++){ fscanf(mapf, "%d %d", &I, &UFI); fmap[FI-1]=UFI; UFNum=max(UFNum,UFI);}
    fscanf(mapf, "%d", &ENum_);  fgets(cstr,100,mapf); assert(ENum_==ENum);
    UENum=0;
    for (int EI =1; EI <= ENum; EI++){ fscanf(mapf, "%d %d", &I, &UEI); emap[EI-1]=UEI; UENum=max(UENum,UEI);}
    fscanf(mapf, "%d", &PNum_);  fgets(cstr,100,mapf); assert(PNum_==PNum);
    UPNum=0;
    for (int PI =1; PI <= PNum; PI++){ fscanf(mapf, "%d %d", &I, &UPI); pmap[PI-1]=UPI; UPNum=max(UPNum,UPI);}
    fclose(mapf);
   }
   FaceData *U_faceData =new FaceData[UFNum];
   EdgeData *U_edgeData =new EdgeData[UENum];
   VertexData *U_vertexData =new VertexData[UPNum];
   std::set<int> Iset;
   for (int FI =1; FI <= FNum; FI++) if(fmap[FI-1]){
      int UFI=fmap[FI-1];
      if(UFI) if(Iset.find(UFI)==Iset.end()) {
	Iset.insert(UFI);
        char tag[10]; sprintf(tag,"%d",UFI);
        std::string FPath;
	if(EmP.assemblyType==PARTITION)   FPath=projectDir+std::string ("/../interfaces/F")+tag;
	else                              FPath=projectDir+std::string ("/interfaces/F")+tag;
        FPath=nativePath(FPath);
        std::ifstream in;
        in.open(FPath.c_str());
	if(!in){
           cout<< "missing file :"<< FPath.c_str() <<endl;
	   assert(false);
 	}
        U_faceData[UFI-1].read(in);
        in.close();
      }
      faceData[FI-1]=U_faceData[UFI-1];
   }
   Iset.clear();
   for (int EI =1; EI <= ENum; EI++) if(emap[EI-1]){
      int UEI=emap[EI-1];
      if(UEI) if(Iset.find(UEI)==Iset.end()) {
	Iset.insert(UEI);
        char tag[10]; sprintf(tag,"%d",UEI);
        std::string EPath;
	if(EmP.assemblyType==PARTITION)  EPath=projectDir+std::string("/../interfaces/E")+tag;
	else                             EPath=projectDir+std::string("/interfaces/E")+tag;
        EPath=nativePath(EPath);
        std::ifstream in;
        in.open(EPath.c_str());
	if(!in){
           cout<< "missing file :"<< EPath.c_str() <<endl;
	   assert(false);
 	}
	U_edgeData[UEI-1].read(in);
        in.close();
      }
      edgeData[EI-1]=U_edgeData[UEI-1];
   }
   Iset.clear();
   for (int PI =1; PI <= PNum; PI++) if(pmap[PI-1]){
      int UPI=pmap[PI-1];
      if(UPI) if(Iset.find(UPI)==Iset.end()) {
	Iset.insert(UPI);
        char tag[10]; sprintf(tag,"%d",UPI);
        std::string PPath;
	if(EmP.assemblyType==PARTITION)  PPath=projectDir+std::string("/../interfaces/P")+tag;
	else                             PPath=projectDir+std::string("/interfaces/P")+tag;
        PPath=nativePath(PPath);
        std::ifstream in;
        in.open(PPath.c_str());
	if(!in){
           cout<< "missing file :"<< PPath.c_str() <<endl;
	   assert(false);
 	}
	U_vertexData[UPI-1].read(in);
        in.close();
      }
      vertexData[PI-1]=U_vertexData[UPI-1];
   }
   delete [] U_faceData;
   delete [] U_edgeData;
   delete [] U_vertexData;
   delete [] emap;
   delete [] fmap;
   delete [] pmap;

// internally defined refri 
   if(EmP.assemblyType!=PARTITION) return;
   int I=0;
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
      I++;
      TDF_Label label1 = it.Value();
      Handle(TDF_Reference)  refAtt;
      if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
      TDF_Label label = refAtt->Get();
      DB::Volume *vol=getLabelVol(label1); 
      if(!vol) continue;
      if(vol->type!=DIELECTRIC && vol->type!=WAVEGUIDE  && vol->type!=BOUNDARYCOND) continue;
      DB::Material* mat=NULL;
      mat= EmP.FindMaterial(vol->material);
      if(!mat) continue;
      TopoDS_Shape S = shapeTool->GetShape(label);
      if(S.IsNull()) continue;
      if(S.ShapeType()==TopAbs_COMPSOLID){
        for (TDF_ChildIterator it2(label,Standard_False); it2.More(); it2.Next()){
          TDF_Label cellLabel = it2.Value();
          TopoDS_Shape S2 = shapeTool->GetShape(cellLabel);
          DB::Volume *vol=getLabelVol(cellLabel);
          for (TopExp_Explorer exp(S2,TopAbs_FACE); exp.More(); exp.Next()){
            TopoDS_Shape F = exp.Current(); 
            F.Orientation(TopAbs_FORWARD);
            int FI=indexedFaces->FindIndex(F);
	    if(!FI) continue;
	    faceData[FI-1].epsr=max(faceData[FI-1].epsr,mat->epsr);
	    faceData[FI-1].mur=max(faceData[FI-1].mur,mat->mur);
	    faceData[FI-1].meshref=max(faceData[FI-1].meshref,vol->meshRefinement);
 	  }
	}
      }
      else{
	for (TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
            TopoDS_Shape F = exp.Current(); 
            F.Orientation(TopAbs_FORWARD);
            int FI=indexedFaces->FindIndex(F);
	    if(!FI) continue;
	    faceData[FI-1].epsr=max(faceData[FI-1].epsr,mat->epsr);
	    faceData[FI-1].mur=max(faceData[FI-1].mur,mat->mur);
	    faceData[FI-1].meshref=max(faceData[FI-1].meshref,vol->meshRefinement);
	}
      }

   }
   for (int FI=1; FI<=indexedFaces->Extent(); FI++){
      TopoDS_Shape F = indexedFaces->FindKey(FI);
      for (TopExp_Explorer exp(F,TopAbs_EDGE); exp.More(); exp.Next()){
        TopoDS_Shape E = exp.Current(); 
        E.Orientation(TopAbs_FORWARD);
        int EI=indexedEdges->FindIndex(E);
	if(!EI) continue;
	edgeData[EI-1].epsr=max(edgeData[EI-1].epsr,faceData[FI-1].epsr);
	edgeData[EI-1].mur=max(edgeData[EI-1].mur,faceData[FI-1].mur);
	edgeData[EI-1].meshref=max(edgeData[EI-1].meshref,faceData[FI-1].meshref);
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
   EmP.insertVolume(vol);
}
void MwOCAF::storeSolidsInDB(){
 if(thePartsLabels->Length()>0)
    for (int i = 1; i <= thePartsLabels->Length(); i++) storeSolidInDB(thePartsLabels->Value(i));
}
*/

void addToParts(Handle(XCAFDoc_ShapeTool) shapeTool, const TDF_Label &label){

   TDF_Label theParts = XCAFDoc_DocumentTool::DocLabel(label).FindChild(6,Standard_True);
   TDF_TagSource aTag;
   TDF_Label label1 = aTag.NewChild(theParts);

   Handle(TDF_Reference)  refAtt;
   if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)){
        refAtt= new TDF_Reference();
        label1.AddAttribute(refAtt);
   }
   refAtt->Set(label);
   Handle(XCAFDoc_Location) locAtt;
   if(!label1.FindAttribute(XCAFDoc_Location::GetID(),locAtt)){
            locAtt= new XCAFDoc_Location();
            label1.AddAttribute(locAtt);
   }
   TopLoc_Location loc=shapeTool->GetLocation(label);
   locAtt->Set(loc);
   Handle(TDataStd_Name)  nameAtt;
   if(label.FindAttribute(TDataStd_Name::GetID(),nameAtt)){
      Handle(TDataStd_Name)  nameAtt1;
      if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt1)){
              nameAtt1= new TDataStd_Name();
              label1.AddAttribute(nameAtt1);
      }
      nameAtt1->Set(nameAtt->Get());
   }
}

void  copyMultiBodyRep( 
		   Handle(XCAFDoc_ShapeTool) STool1, Handle(XCAFDoc_ColorTool) ColTool1, Handle(XCAFDoc_LayerTool) layTool1, const TDF_Label &label1, 
		   Handle(XCAFDoc_ShapeTool) STool2, Handle(XCAFDoc_ColorTool) ColTool2, Handle(XCAFDoc_LayerTool) layTool2, const TDF_Label &label2)
{
        TDF_Label label2ref=label2;
        if(STool2->IsReference(label2)) STool2->GetReferredShape(label2,label2ref);
        Handle(XCAFDoc_Location) locAtt;
	TopLoc_Location loc2;
        if(label2.FindAttribute(XCAFDoc_Location::GetID(),locAtt)) loc2=locAtt->Get(); 
	std::map<std::string, std::set<int> > nameTags;
//	TopTools_SequenceOfShape compounds;
	int nameI=0;
	for (TDF_ChildIterator it(label2ref, Standard_False); it.More(); it.Next()){
		 TDF_Label subl2=it.Value();
                 TDF_Label subl2ref=subl2;
                 if(STool2->IsReference(subl2)) STool2->GetReferredShape(subl2,subl2ref);
                 Handle(TDataStd_Name)  nameAtt;
                 subl2ref.FindAttribute(TDataStd_Name::GetID(),nameAtt);
                 TCollection_AsciiString subl2name=nameAtt->Get();
		 if(isValidMultibodyPartName(subl2name)){
                     std::string str=subl2name.ToCString();
                     nameTags[str].insert(subl2.Tag());
		 }
	}
        BRep_Builder builder;
        typedef std::map<std::string, std::set<int> > ::iterator NameIt;
	for (NameIt nit=nameTags.begin(); nit!= nameTags.end(); nit++){
	   TopoDS_Shape subS2;
           LabelAttributes attr;
	   if((*nit).second.size()>1){
             TopoDS_Compound  Cmp;  builder.MakeCompound(Cmp);
	     for (std::set<int> ::iterator tit=(*nit).second.begin(); tit!= (*nit).second.end(); tit++){
	       TDF_Label subl2=label2ref.FindChild(*tit,Standard_False);
               TDF_Label subl2ref=subl2;
               if(STool2->IsReference(subl2)) STool2->GetReferredShape(subl2,subl2ref);
               TopoDS_Shape S = STool2->GetShape(subl2);
               builder.Add(Cmp,S);
               if(!attr.hasName) copyLabelAttributes(attr, subl2ref, ColTool2, layTool2);
	     }
             subS2=Cmp;
	   } else {
	      std::set<int> ::iterator tit=(*nit).second.begin();
	      TDF_Label subl2=label2ref.FindChild(*tit,Standard_False);
              TDF_Label subl2ref=subl2;
              if(STool2->IsReference(subl2)) STool2->GetReferredShape(subl2,subl2ref);
              subS2 = STool2->GetShape(subl2);
              copyLabelAttributes(attr, subl2ref, ColTool2, layTool2);
	   }
	   TopoDS_Shape S2=subS2;
           S2.Location (loc2.Multiplied(subS2.Location()));
	   TDF_Label sublabel1=STool1->AddComponent(label1,S2);
           pasteLabelAttributes(attr, sublabel1, ColTool1, layTool1);
	   addToParts(STool1, sublabel1);
	}

}




void  copySubAss(
	           Handle(XCAFDoc_ShapeTool) STool1, Handle(XCAFDoc_ColorTool) ColTool1, Handle(XCAFDoc_LayerTool) layTool1, const TDF_Label &label1, 
		   Handle(XCAFDoc_ShapeTool) STool2, Handle(XCAFDoc_ColorTool) ColTool2, Handle(XCAFDoc_LayerTool) layTool2, const TDF_Label &label2,
		   int toplevel, int assemblytype
		 )
{
   TDF_Label label2ref=label2;
   if(STool2->IsReference(label2)) STool2->GetReferredShape(label2,label2ref);
   LabelAttributes attr;
   copyLabelAttributes(attr, label2ref, ColTool2, layTool2);
//---
//---
   TopoDS_Shape S2 = STool2->GetShape(label2); if(S2.IsNull()) return;
   TDF_Label sublabel1;
   if(toplevel){
/*
  If S2 is top level shape in the current document and current document not main document label2 holds a referred not located shape.
  In this case the actual location of S2 is stored in the upper document (in a sublabel of the theParts label).
*/	TDF_TagSource aTag;
        sublabel1 = aTag.NewChild(label1);
	STool1->SetShape(sublabel1,S2);
        if(!sublabel1.IsNull()){
	   pasteLabelAttributes(attr, sublabel1, ColTool1, layTool1);
	   STool1->UpdateAssembly(sublabel1);
           STool1->UpdateAssembly(label1);
	}
        for (TDF_ChildIterator it(label2ref, Standard_False); it.More(); it.Next())  
	                copySubAss(STool1,ColTool1,layTool1,sublabel1, STool2, ColTool2,layTool2, it.Value(), 0, assemblytype);
   }
   if(!toplevel){     
	bool expandLabel2=(S2.ShapeType() == TopAbs_COMPOUND) && attr.name==TCollection_AsciiString("REMOVE"); 
	if(expandLabel2) 
//           expand Compounds originated from different Step Representations (hybrid Models):
	     copyMultiBodyRep(STool1,ColTool1,layTool1,label1, STool2, ColTool2,layTool2, label2);
        else{
	    sublabel1=STool1->AddComponent(label1,S2);
            if(!sublabel1.IsNull()){
	      pasteLabelAttributes(attr, sublabel1, ColTool1, layTool1);
              STool1->UpdateAssembly(label1);
	    }
	    addToParts(STool1, sublabel1);
	}
   }
}


void MwOCAF::generateSubshapes(const TDF_Label &label, Handle(XCAFDoc_ShapeTool) shtool){
   TopoDS_Shape Sh = shtool->GetShape(label);
   Handle(TPrsStd_AISPresentation)  prs;
   bool hasprs=label.FindAttribute(TPrsStd_AISPresentation::GetID(),prs);
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
              if(hasprs) setLabelPresentation(label1,prs->Color(), prs->Transparency());
	      else       setLabelPresentation(label1);
	      generateSubshapes(label1);
	    }
	 } else if(Sh.ShapeType()==TopAbs_SOLID || Sh.ShapeType()==TopAbs_SHELL|| Sh.ShapeType()==TopAbs_WIRE || Sh.ShapeType()==TopAbs_COMPOUND){
	    bool skip=false;
	    bool lp=false;
            DB::Volume *vol=getLabelVol(label);
            if(vol){ skip=(vol->type==SPLITTER || vol->type==GRID); lp=vol->type==LINEPORT;}
            if(!skip){
             if(lp) for(exp1.Init(Sh, TopAbs_EDGE); exp1.More(); exp1.Next()){
               TopoDS_Shape E=exp1.Current();
	       TDF_TagSource aTag;
               TDF_Label label1 = aTag.NewChild(label);
	       shtool->SetShape(label1,E);
	       generateSubshapes(label1);
             }
	     else for(exp1.Init(Sh, TopAbs_FACE); exp1.More(); exp1.Next()){
               TopoDS_Shape F=exp1.Current();
	       TDF_TagSource aTag;
               TDF_Label label1 = aTag.NewChild(label);
	       shtool->SetShape(label1,F);
//              Quantity_Color col(Quantity_NOC_SALMON4);
//              colorTool->SetColor(label1, col, XCAFDoc_ColorGen);
               if(hasprs) setLabelPresentation(label1,prs->Color(), prs->Transparency());
	       else       setLabelPresentation(label1);
	       generateSubshapes(label1);
	     }
	    }
	 }else if(Sh.ShapeType()==TopAbs_FACE){
            for(exp1.Init(Sh, TopAbs_EDGE); exp1.More(); exp1.Next()){
               TopoDS_Shape E=exp1.Current();
	       TDF_TagSource aTag;
               TDF_Label label1 = aTag.NewChild(label);
	       shtool->SetShape(label1,E);
	       generateSubshapes(label1);
            }
	 }else if(Sh.ShapeType()==TopAbs_EDGE){
            for(exp1.Init(Sh, TopAbs_VERTEX); exp1.More(); exp1.Next()){
               TopoDS_Shape V=exp1.Current();
	       TDF_TagSource aTag;
               TDF_Label label1 = aTag.NewChild(label);
	       shtool->SetShape(label1,V);
            }
         }
	 shtool->UpdateAssembly(label);
   } else  for (TDF_ChildIterator it1(label, Standard_False); it1.More(); it1.Next()){
       TDF_Label label1 = it1.Value();
       generateSubshapes(label1, shtool);
   }
}


void MwOCAF::generateSubshapes(const TDF_Label &label){
 generateSubshapes(label, shapeTool);
}


void MwOCAF::generateSubshapes(){
 for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
 {
     TDF_Label label1 = it.Value();
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) return;
     TDF_Label label = refAtt->Get();
     generateSubshapes(label);
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
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) return;
     TDF_Label label = refAtt->Get();
     TopoDS_Shape S= shapeTool->GetShape(label);
     int NF=0; for(TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()) NF++;
     cout<< "part :"<< partName.ToCString() <<endl;
     cout<< "NF :" << NF <<endl;
 }
}



void MwOCAF::regenerate(){
// storeSolidsInDB();
 generateSubshapes();
 setMissingNames();
 makeShapeIndex2Labels();
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
  if(USEXMLXCAF)  XCAFapp->NewDocument("XmlXCAF",mainDoc);
  else            XCAFapp->NewDocument("MDTV-XCAF",mainDoc);
  return true;
}
bool MwOCAF::openDoc(const char* fileName)
{
    Handle(XCAFApp_Application) XCAFapp=XCAFApp_Application::GetApplication();
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
  return true;
}


int loadModel(MwOCAF* ocaf, const char *fName, bool update=false);


void MwOCAF::loadGlobalMat(const char* wkprojpath, bool onlyIfModified){
 std::string matFileName=wkprojpath;
 for (int i = 0; i < EmP.level; ++i) matFileName+="/..";
 matFileName+="/Data/material.dat";
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
 if(doit) loadModel(this, nativePath(matFileName).c_str() );
}

void MwOCAF::workopen(const char* wkprojpath)
{
//        mutex.lock();
        EmP.occFilePath=wkprojpath;
        if(USEXMLXCAF) EmP.occFilePath+="/model.xml";
	else           EmP.occFilePath+="/model.dxc";
        projectDir=wkprojpath;
        std::string prjstatusFileName=projectDir+"/model.status";
        prjstatusFileName=nativePath(prjstatusFileName);
        prjStatus.read(prjstatusFileName.c_str());
        std::string emFileName=projectDir+"/model.em";
        emFileName=nativePath(emFileName);
        assert(FileExists(emFileName.c_str()));
        loadModel(this, emFileName.c_str(), false);
        if(FileExists(EmP.occFilePath.c_str())){
	   openDoc(EmP.occFilePath.c_str());
	   EmP.hasGeo=true;
	}
        std::string UPIFfileName=projectDir+".brep";
        UPIFfileName=nativePath(UPIFfileName);
	if(FileExists(UPIFfileName.c_str())) openUPIF(UPIFfileName.c_str());
	hasDownIF=false;
	if(EmP.assemblyType==NET) hasDownIF=openParts();
	if(EmP.assemblyType==NET) readPartsStatus();
	else if(EmP.assemblyType==COMPONENT) setPartsStatus();
	std::string IFfileName=projectDir+"/model.brep";
        IFfileName=nativePath(IFfileName);
        hasIF=FileExists(IFfileName.c_str());
        loadGlobalMat(nativePath(wkprojpath).c_str(), true);
}

void MwOCAF::worksave(const char* wkprojpath)
{
   save(EmP.occFilePath.c_str());
   std::string prjstatusFileName=wkprojpath; prjstatusFileName+="/model.status";
   prjstatusFileName=nativePath(prjstatusFileName);
   prjStatus.save(prjstatusFileName.c_str());
   std::string emFileName=wkprojpath; emFileName+="/model.em";
   FILE *fid= fopen(nativePath(emFileName).c_str(), "w");
   fprintf(fid, "Level  %d\n", EmP.level);
   fprintf(fid, "AssemblyType  %d\n", EmP.assemblyType);
   fprintf(fid, "DefaultBoundCond  \"%s\"\n", EmP.defaultBC);
   if(!EmP.stepFilePath.empty()) fprintf(fid, "\nImport  \"%s\"\n\n", EmP.stepFilePath.c_str());
   EmP.save(fid);
   fclose(fid);
   std::string asstypeFileName=wkprojpath; asstypeFileName+="/assemblyType";
   FILE *assfid= fopen(nativePath(asstypeFileName).c_str(), "w");
   fprintf(assfid, "%d\n", EmP.assemblyType);
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



void MwOCAF::save(const char* fileName)
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



void MwOCAF::saveSplitFaceMap(){
// save superfaces_face_lists:
     std::string splitFaceFileName=projectDir+std::string ("/Partition/partition.map");
     FILE *out=fopen(nativePath(splitFaceFileName).c_str(), "w");
     if(out){
      fprintf(out, "%d  faceNum \n", indexedFaces->Extent() );
      for (int FI =1; FI <= splitFacesMap->Length(); FI++) fprintf(out, "%d  %d\n", FI, splitFacesMap->Value(FI));
      fprintf(out, "%d  edgeNum \n", indexedEdges->Extent() );
      for (int EI =1; EI <= splitEdgesMap->Length(); EI++) fprintf(out, "%d  %d\n", EI, splitEdgesMap->Value(EI));
      fprintf(out, "%d  pointNum \n", indexedVertices->Extent() );
      for (int PI =1; PI <= splitVerticesMap->Length(); PI++) fprintf(out, "%d  %d\n", PI, splitVerticesMap->Value(PI));
      fclose(out);
     }
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
     Handle(XCAFDoc_Location) locAtt;
     assert(label1.FindAttribute(XCAFDoc_Location::GetID(),locAtt));
     TopLoc_Location loc=locAtt->Get();
     TopoDS_Shape S2=S1; S2.Location (loc.Multiplied(S1.Location()));
     thePartShapes.Bind(label1.Tag(),S2);
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
     if(vol->type==SPLITTER || vol->type==GRID ) continue;
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
   if (theParts.IsNull()) { prjStatus.geometry=false; return;}
   prjStatus.partTypes=true;
   if(EmP.assemblyType==COMPONENT) prjStatus.partMaterials=true;
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
     TDF_Label label1 = it.Value();
     Handle(TDataStd_Name)  nameAtt;
     if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
     TCollection_AsciiString partName=nameAtt->Get();
     DB::Volume* vol = EmP.FindVolume(partName.ToCString());
     if(!vol) continue;
     vol->defined=1;
     if(vol->type==UNDEFINED) {prjStatus.partTypes=false; vol->defined=0;}
     if(vol->type==DIELECTRIC || vol->type==BOUNDARYCOND || vol->type==WAVEGUIDE)  
	     if(!strcmp(vol->material,"?")) {prjStatus.partMaterials=false; vol->defined=0; }
   }
}


TopAbs_ShapeEnum shapeType(TopoDS_Shape S)
{
 if(S.ShapeType()!=TopAbs_COMPOUND) return S.ShapeType();
 TopAbs_ShapeEnum type=TopAbs_SHAPE;
 for (TopoDS_Iterator it(S); it.More(); it.Next()) type=min(type,it.Value().ShapeType());
 return type;
}

int defaultPartType(TopoDS_Shape S, TCollection_AsciiString partName)
{
// Defaults from Names
     if(isSplitName(partName))          return SPLITTER;
     else if(isGridName(partName))      return GRID;
     else if(isWgName(partName))        return WAVEGUIDE;
     else if(isLPName(partName))        return LINEPORT;
     else if(isDielName(partName))      return DIELECTRIC;
     else if(isHoleName(partName))      return HOLE;
     else if(isBCondName(partName))     return BOUNDARYCOND;
// Defaults from Geometries
     if(!hasSolid(S))                   return BOUNDARYCOND;
     else                               return DIELECTRIC;
}


void MwOCAF::setDefaultPartTypes(bool onlyUndefined)
{
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
     TDF_Label label1 = it.Value();
     Handle(TDataStd_Name)  nameAtt;
     if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
     TCollection_AsciiString partName=nameAtt->Get();
     DB::Volume* vol = EmP.FindVolume(partName.ToCString());
     if(!vol) continue;
     if(onlyUndefined && vol->type!=UNDEFINED) continue;
     if(EmP.assemblyType==NET)   vol->type=ASSEMBLY;
     else if(EmP.assemblyType==COMPONENT) {
           Handle(TDF_Reference)  refAtt;
           if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
           TDF_Label label = refAtt->Get();
           TopoDS_Shape S;
           if(!shapeTool->GetShape(label,S)) vol->type=0;
	   else vol->type=defaultPartType(S,partName);
     }
   }
   prjStatus.partTypes=true;
}


void TransformShape(TopoDS_Shape &S, gp_Trsf T)
{
         Handle(ShapeCustom_TrsfModification) TM = new ShapeCustom_TrsfModification(T);
         TopTools_DataMapOfShapeShape context;
         BRepTools_Modifier MD;
         TopoDS_Shape res = ShapeCustom::ApplyModifier (S, TM, context, MD );
	 S=res;
}



void putVolumesInDB_(DB::EmProblem *emP, Handle(XCAFDoc_ShapeTool) shapeTool, TDF_Label access){
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
	   vol = new DB::Volume();
	   strcpy(vol->name,partName.ToCString());
	   emP->insertVolume(vol);
           TopoDS_Shape S;
	   if(!shapeTool->GetShape(label,S))     vol->type=0;
	   else if(emP->assemblyType==NET)       vol->type=ASSEMBLY;
           else if(emP->assemblyType==COMPONENT) vol->type=defaultPartType(S,partName);
     }
 }
 DB::Volume* vol;
 vol=emP->FindVolume("PEC");
 if(!vol){
  vol =new DB::Volume();
  strcpy(vol->name,"PEC");
  strcpy(vol->material,"PEC");
  vol->type=BOUNDARYCOND;
  emP->insertVolume(vol);
 }
 vol=emP->FindVolume("PMC");
 if(!vol){
  vol =new DB::Volume();
  strcpy(vol->name,"PMC");
  strcpy(vol->material,"PMC");
  vol->type=BOUNDARYCOND;
  emP->insertVolume(vol);
 }

}

void MwOCAF::putVolumesInDB(){
  putVolumesInDB_(&EmP, shapeTool, mainDoc->Main());
}

inline void setModified(const char *dir){
   std::string modified=std::string(dir)+std::string("/modified");
   FILE *fid= fopen(nativePath(modified).c_str(), "w");
   fclose(fid);
}

bool MwOCAF::hasMultibodyPart(Handle(XCAFDoc_ShapeTool) shapeTool, TDF_Label ass)
{
//   TDF_LabelSequence components;
//   shapeTool->GetComponents(ass,components);
   bool haselement=false;
   for (TDF_ChildIterator it(ass, Standard_False); it.More(); it.Next()){
//   for (int I = 1; I <=components.Length() ; I++){
        TDF_Label li = it.Value();
//        TDF_Label li=components.Value(I);
        TDF_Label rli=li;
        if(shapeTool->IsReference(li)) shapeTool->GetReferredShape(li,rli);
        Handle(TDataStd_Name)  nameAtt;
        TCollection_AsciiString name;
        if(rli.FindAttribute(TDataStd_Name::GetID(),nameAtt)) name=nameAtt->Get();
        if(isValidMultibodyPartName(name)) {haselement=true; break;}
   }
   return haselement;
}

bool MwOCAF::hasSubAssembly(Handle(XCAFDoc_ShapeTool) shapeTool, TDF_Label ass)
{
//   TDF_LabelSequence components;
//   shapeTool->GetComponents(ass,components);
   bool hassubass=false;
   for (TDF_ChildIterator it(ass, Standard_False); it.More(); it.Next()){
//   for (int I = 1; I <=components.Length() ; I++){
        TDF_Label li = it.Value();
//        TDF_Label li=components.Value(I);
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

bool COMPONENTISASSEMBLY=false;

void MwOCAF::saveImportedStruct(const char *dir,
		int level,
		TCollection_AsciiString assName,
	       	TopLoc_Location assLoc,
		TDF_Label ass,
		std::set<std::string, std::less<std::string> > *componentlist,
		std::set<std::string, std::less<std::string> > *wgcomponentlist,
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

   DB::EmProblem *subEmP=new DB::EmProblem();
   strcpy(subEmP->defaultBC,EmP.defaultBC);

   if(isCompName(assName))
      subEmP->assemblyType=COMPONENT;
   else if(COMPONENTISASSEMBLY)
      subEmP->assemblyType= (hasSubAssembly(shapeTool, ass) && !hasMultibodyPart(shapeTool, ass)) ? NET: COMPONENT;
   else
      subEmP->assemblyType= shapeTool->IsAssembly(ass)? NET: COMPONENT;

// Copy the subassembly structure into the subdocument:
   copySubAss(subShapeTool,subColTool,subLayTool,subShapes, shapeTool, colorTool,layerTool, ass, 1, subEmP->assemblyType);
   putVolumesInDB_(subEmP, subShapeTool, subDoc->Main());

// set subass name and location into the related parts label:
   Handle(XCAFDoc_Location) locAtt;
   TDF_Label subPL = XCAFDoc_DocumentTool::DocLabel(subDoc->Main()).FindChild(6,Standard_True);
   TDataStd_Name::Set(subPL, assName);
   if(!subPL.FindAttribute(XCAFDoc_Location::GetID(),locAtt)){
        locAtt= new XCAFDoc_Location();
        subPL.AddAttribute(locAtt);
   }
   locAtt->Set(assLoc);



// *** make main directory and the interfaces directory for subassembly
   if(!FileExists(dir)) createdir(dir);
   std::string interfaces=std::string(dir)+std::string("/interfaces");
   interfaces=nativePath(interfaces);
   if(!FileExists(interfaces.c_str()))
       createdir(interfaces.c_str());
   else
       removeAllFilesInDir(interfaces.c_str());

// If considered subassembly is a  COMPONENT creates the Partition dir
   std::string partition=std::string(dir)+std::string("/Partition");
   partition=nativePath(partition);
   if(subEmP->assemblyType==COMPONENT){
      std::string cmp=assName.ToCString();
      if(componentlist) if(componentlist->find(cmp)==componentlist->end()) componentlist->insert(cmp);
      if(!FileExists(partition.c_str())) createdir(partition.c_str());
      else removeAllFilesInDir(partition.c_str());
      TDF_Label subParts = XCAFDoc_DocumentTool::DocLabel(subDoc->Main()).FindChild(6,Standard_True);
      if(wgcomponentlist) for (TDF_ChildIterator it(subParts,Standard_False); it.More(); it.Next()) 
      {
          TDF_Label label1 = it.Value();
          Handle(TDataStd_Name)  nameAtt;
          if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
          TCollection_AsciiString partName=nameAtt->Get();
          DB::Volume* vol = subEmP->FindVolume(partName.ToCString());
          if(vol->type==WAVEGUIDE) {
              if(wgcomponentlist->find(cmp)==wgcomponentlist->end()) wgcomponentlist->insert(cmp);
	      break;
	  }
      }
   }
   std::string prjstatusFileName=std::string(dir)+std::string("/model.status");
   prjstatusFileName=nativePath(prjstatusFileName);
   if(!FileExists(prjstatusFileName.c_str())) prjStatus.save(prjstatusFileName.c_str());

// Set subdirs list.  Remove downIF (part.brep)
   std::string subdfname=std::string(dir)+std::string("/subdirs");
   if(subEmP->assemblyType==NET) {
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
           if(vol->type!=ASSEMBLY) continue;
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
   if(subEmP->assemblyType==COMPONENT) { setModified(partition.c_str()); }

   std::string occFileName=std::string(dir);
   if(USEXMLXCAF) occFileName+=std::string("/model.xml");
   else           occFileName+=std::string("/model.dxc");
   occFileName=nativePath(occFileName);
   XCAFapp->SaveAs(subDoc,TCollection_AsciiString(occFileName.c_str()));
//   checkPartNF(subDoc);
   XCAFapp->Close(subDoc);

// ** Writes initial model.em and assemblyType.
   std::string  emFileName=std::string (dir)+std::string("/model.em");
   if(!FileExists(emFileName.c_str())) { 
       FILE *fid= fopen(nativePath(emFileName).c_str(), "w");
       fprintf(fid, "Level  %d\n", level);
       fprintf(fid, "AssemblyType  %d\n", subEmP->assemblyType);
       fprintf(fid, "DefaultBoundCond  \"%s\"\n", subEmP->defaultBC);
       if(addImport) if(!EmP.stepFilePath.empty()) fprintf(fid, "\nImport  \"%s\"\n\n", EmP.stepFilePath.c_str());
       subEmP->save(fid);
       fclose(fid);
       std::string asstypeFileName=std::string(dir)+std::string("/assemblyType");
       FILE *assfid= fopen(nativePath(asstypeFileName).c_str(), "w");
       fprintf(assfid, "%d\n", subEmP->assemblyType);
       fclose(assfid);
   }
// If it is a COMPONENT writes an initial model.em also in the Partition dir
   if(subEmP->assemblyType==COMPONENT){ 
      std::string emFileName=partition+std::string("/model.em");
      FILE *emfid= fopen(nativePath(emFileName).c_str(), "w");
      fprintf(emfid, "Level  %d\n", level+1);
      fclose(emfid);
   }

   delete(subEmP);

// Recursion on sublevels
   if(subEmP->assemblyType==NET) for (TDF_ChildIterator it(ass, Standard_False); it.More(); it.Next()){
     TDF_Label label = it.Value();
     Handle(XCAFDoc_Location) locAtt;
     if(!label.FindAttribute(XCAFDoc_Location::GetID(),locAtt)) continue;
     TDF_Label referred=label;
     if(shapeTool->IsReference(label)) shapeTool->GetReferredShape(label,referred);
     Handle(TDataStd_Name)  nameAtt;
     if(!referred.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
     TCollection_AsciiString subAssName=nameAtt->Get();
     std::string partDir=std::string (dir)+std::string ("/")+subAssName.ToCString();
     partDir=nativePath(partDir);
     TopLoc_Location subAssLoc=locAtt->Get(); subAssLoc=assLoc.Multiplied(subAssLoc);
     saveImportedStruct(partDir.c_str(), level+1, subAssName.ToCString(), subAssLoc, referred, componentlist, wgcomponentlist);
   }
}

void MwOCAF::saveImportedStruct(
		const char *dir,
                bool compIsAss,
		std::set<std::string, std::less<std::string> > *componentlist,
		std::set<std::string, std::less<std::string> > *wgcomponentlist
		)
{
   COMPONENTISASSEMBLY=compIsAss;
   TDF_LabelSequence FreeLabels;
   shapeTool->GetFreeShapes(FreeLabels);
   assert(FreeLabels.Length()>0);
   TDF_Label ass=FreeLabels.Value(1);
   TopLoc_Location assLoc; assLoc.Identity();
   TCollection_AsciiString assName;
   Handle(TDataStd_Name)  nameAtt;
   if(ass.FindAttribute(TDataStd_Name::GetID(),nameAtt)) assName=nameAtt->Get();

   saveImportedStruct(dir, 0, assName.ToCString(), assLoc, ass, componentlist, wgcomponentlist, true);
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
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
     Handle(XCAFDoc_Location) locAtt;
     if(!label1.FindAttribute(XCAFDoc_Location::GetID(),locAtt)) continue;
     std::string  fileName=partDir+std::string (".brep");
     fileName=nativePath(fileName);
     TDF_Label label = refAtt->Get();
     assert(shapeTool->IsShape(label));
     TopoDS_Shape S= shapeTool->GetShape(label);
     TopLoc_Location loc=locAtt->Get().Inverted();
     BRepBuilderAPI_Transform trs(loc.Transformation());
     TopTools_IndexedMapOfShape Fmap,Emap,Pmap,subShapes;
     makeISubShapes(&Fmap,&Emap,&Pmap,S);
     int FNum=Fmap.Extent();
     for (int FI=1; FI<=FNum; FI++){
       TopoDS_Shape F = Fmap.FindKey(FI);
       trs.Perform(F, Standard_False);
       if(subShapes.FindIndex(F) < 1) subShapes.Add(trs.ModifiedShape(F));
     }
     int ENum=Emap.Extent();
     for (int EI=1; EI<=ENum; EI++){
       TopoDS_Shape E = Emap.FindKey(EI);
       trs.Perform(E, Standard_False);
       if(subShapes.FindIndex(E) < 1) subShapes.Add(trs.ModifiedShape(E));
     }
     int PNum=Pmap.Extent();
     for (int PI=1; PI<=PNum; PI++){
       TopoDS_Shape P = Pmap.FindKey(PI);
       trs.Perform(P, Standard_False);
       if(subShapes.FindIndex(P) < 1) subShapes.Add(trs.ModifiedShape(P));
     }
     saveShapesIfDiff( fileName.c_str(), subShapes, tmpbrep.c_str(), true);
   }
   return true;
}


void MwOCAF::addComponent(std::set<std::string, std::less<std::string> > &list){
   TCollection_AsciiString assName; getAssName(assName);
   std::string line=assName.ToCString();
   if(list.find(line)==list.end()) if(!line.empty()) list.insert(line);
}


void MwOCAF::addPorts(std::map<std::string, int, std::less<std::string> > &ports, std::map<std::string, double, std::less<std::string> > &portloads)
{
//   int I1=EmP.isTopLevel? I1 : extFaceNum+1;
   int I=0;
   std::set<int> initializedF;
   TCollection_AsciiString assName; getAssName(assName);
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
      TDF_Label label1 = it.Value();
      Handle(TDataStd_Name)  nameAtt;
      if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) return;
      TCollection_AsciiString volName=nameAtt->Get();
      DB::Volume *vol=EmP.FindVolume(volName.ToCString());
      if(vol->disabled) continue;
      if(vol->type!=WAVEGUIDE && vol->type!=LINEPORT) continue;
      TCollection_AsciiString partName=assName+TCollection_AsciiString("__")+volName;
      std::string name=partName.ToCString();
      if(vol->type==WAVEGUIDE){
        std::string TEMport,TEport,TMport;
        TEMport=name+"_TEM";
        TEport=name+"_TE";
        TMport=name+"_TM";
        if(ports.find(TEMport)==ports.end()) ports[TEMport]=vol->TEMportsNum;
        if(ports.find(TEport)==ports.end()) ports[TEport]=vol->TEportsNum;
        if(ports.find(TMport)==ports.end()) ports[TMport]=vol->TMportsNum;
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

bool FileIsTop(std::string fname)
{
std::string ftop=fname+std::string(".top");
return FileExists(ftop.c_str());
}

void MwOCAF::saveFEPlinks(){
// *******
// Mapping of part entities to assembly entities:
// *******
  TopTools_IndexedMapOfShape  bdrFaces;
    if(EmP.assemblyType==COMPONENT){
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
  int I=0;
  for (TDF_ChildIterator Vit(theParts,Standard_False); Vit.More(); Vit.Next()) 
  {
     I++;
     partFacesMap[I-1]   =NULL;
     partEdgesMap[I-1]   =NULL;
     partVerticesMap[I-1]=NULL;
     TDF_Label label1 = Vit.Value();
     Handle(TDF_Reference)  refAtt;
     if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
     DB::Volume *vol=getLabelVol(label1);
     if(!vol) continue;
     if(vol->type==SPLITTER || vol->type==GRID) continue;
     TDF_Label label = refAtt->Get();
     assert(shapeTool->IsShape(label));
     TopoDS_Shape S= shapeTool->GetShape(label);
     TopTools_IndexedMapOfShape Fmap,Emap,Pmap;
     makeISubShapes(&Fmap,&Emap,&Pmap,S);
     int FNum=Fmap.Extent();
     if(FNum>0) partFacesMap[I-1] = new TColStd_HArray1OfInteger(1,FNum);
     for (int FI=1; FI<=FNum; FI++){
       TopoDS_Shape F = Fmap.FindKey(FI);
       int newFI=indexedFaces->FindIndex(F);
       partFacesMap[I-1]->SetValue(FI,newFI);
     }
     int ENum=Emap.Extent();
     if(ENum>0) partEdgesMap[I-1] = new TColStd_HArray1OfInteger(1,ENum);
     for (int EI=1; EI<=ENum; EI++){
       TopoDS_Shape E = Emap.FindKey(EI);
       int newEI=indexedEdges->FindIndex(E);
       partEdgesMap[I-1]->SetValue(EI,newEI);
     }
     int PNum=Pmap.Extent();
     partVerticesMap[I-1] = new TColStd_HArray1OfInteger(1,PNum);
     if(PNum>0) for (int PI=1; PI<=PNum; PI++){
       TopoDS_Shape P = Pmap.FindKey(PI);
       int newPI=indexedVertices->FindIndex(P);
       partVerticesMap[I-1]->SetValue(PI,newPI);
     }
  }

// ************  save Face Links
   TCollection_AsciiString assName; getAssName(assName);
   I=0;
   std::set<int> initializedF;
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
   // link assembly faces to part faces 
      I++;
      TDF_Label label1 = it.Value();
      Handle(TDataStd_Name)  nameAtt;
      if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) return;
      TCollection_AsciiString partName=nameAtt->Get();
      DB::Volume *vol=EmP.FindVolume(partName.ToCString());
      if(vol->type==SPLITTER || vol->type==GRID) continue;
      Handle(XCAFDoc_Location) locAtt;
      if(!label1.FindAttribute(XCAFDoc_Location::GetID(),locAtt)) continue;
      TopLoc_Location partloc=locAtt->Get();
      std::string partDir=projectDir+std::string("/")+partName.ToCString();
      partDir=nativePath(partDir);
      if(!partFacesMap[I-1].IsNull()) for (int FI =1; FI <= partFacesMap[I-1]->Length(); FI++){
	 int UFI=partFacesMap[I-1]->Value(FI);
	 if(!UFI) continue;
	 char tag[10]; sprintf(tag,"%d",UFI);
	 std::string UFPath=projectDir+std::string("/interfaces/F")+tag;
         std::string UFRPath=std::string("../../interfaces/F")+tag;
	 UFPath=nativePath(UFPath);
	 UFRPath=nativePath(UFRPath);
	 if(initializedF.find(UFI)==initializedF.end()) if(!FileExists(UFPath.c_str())){
	     FaceData FD;
	     FD.level=EmP.level; 
	     FD.epsr=1.0; 
	     FD.mur=1.0; 
	     FD.meshref=1.0; 
	     FD.shared=UFI>extFaceNum && EmP.assemblyType==NET;
	     FD.cmp1="-"; FD.cmp2="-";
	     char cname[20]; sprintf(cname,"L%dF%d",EmP.level,UFI);
	     FD.name=cname;
             FD.sfname=std::string("_");
	     std::ofstream out;
	     out.open(UFPath.c_str());
	     FD.write(out);
             out.close();
	     initializedF.insert(UFI);
	 #ifndef USE_SYMBOLIC_LINKS
	     FileSetTop(UFPath);
	 #endif
	 }
         if(EmP.assemblyType!=COMPONENT){
	    sprintf(tag,"%d",FI);
            std::string  FPath=partDir+std::string ("/interfaces/F")+tag;
	    FPath=nativePath(FPath);
	   #ifdef USE_SYMBOLIC_LINKS
	    createLink(UFRPath.c_str(), FPath.c_str());
	   #else
	    createLink(UFPath.c_str(), FPath.c_str());
	   #endif
	    std::ifstream in;
            in.open(UFPath.c_str());
	    if(!in){
              cout<< "missing file :"<< UFPath.c_str() <<endl;
	      assert(false);
 	    }
	    FaceData fd;
	    fd.read(in);
	    in.close();
            if(fd.shared){
	     std::string FPath_mwm  =FPath+std::string(".mwm");
	     #ifdef USE_SYMBOLIC_LINKS
	      std::string UFRPath_mwm=UFRPath+std::string(".mwm");
	      createLink(UFRPath_mwm.c_str(), FPath_mwm.c_str());
	     #else
	      std::string UFPath_mwm=UFPath+std::string(".mwm");
	      createLink(UFPath_mwm.c_str(), FPath_mwm.c_str());
	     #endif
	    }
	 }
         // set faces boundary conditions:
         DB::Volume *vol=EmP.FindVolume(partName.ToCString());
         if(vol->type==BOUNDARYCOND || vol->type==WAVEGUIDE) {
	   std::string str;
	   if(vol->type==BOUNDARYCOND ) str=std::string(vol->material);
	   if(vol->type==WAVEGUIDE ) str="WAVEGUIDE";
	   std::fstream out;
           out.open(UFPath.c_str(), std::ios::out | std::ios::app);
           out << str; out << endl;
           out.close();
         }
      }
   }

// ************  save Vertex Links:
   I=0;
   std::set<int> initializedP;
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
      I++;
      TDF_Label label1 = it.Value();
      Handle(TDataStd_Name)  nameAtt;
      if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
      TCollection_AsciiString partName=nameAtt->Get();
      DB::Volume *vol=EmP.FindVolume(partName.ToCString());
      if(!vol) continue;
      if(vol->type==SPLITTER || vol->type==GRID) continue;
      std::string partDir=projectDir+std::string("/")+partName.ToCString();
      partDir=nativePath(partDir);
      if(!partVerticesMap[I-1].IsNull()) for (int PI =1; PI <= partVerticesMap[I-1]->Length(); PI++){
	 int UPI=partVerticesMap[I-1]->Value(PI);
	 if(!UPI) continue;
	 char tag[10]; sprintf(tag,"%d",UPI);
         std::string UPPath=projectDir+std::string("/interfaces/P")+tag;
         std::string UPRPath=std::string("../../interfaces/P")+tag;
         UPPath=nativePath(UPPath);
         UPRPath=nativePath(UPRPath);
	 if(initializedP.find(UPI)==initializedP.end()) if(!FileExists(UPPath.c_str())){
	     VertexData pd;
	     pd.epsr=1.0; 
	     pd.mur=1.0; 
	     pd.meshref=1.0; 
	     pd.level=EmP.level;
	     char cname[20]; sprintf(cname,"L%dP%d",EmP.level,UPI);
	     pd.name=cname;
	     std::ofstream out;
             out.open(UPPath.c_str());
	     pd.write(out); 
             out.close();
	     initializedP.insert(UPI);
	 #ifndef USE_SYMBOLIC_LINKS
	     FileSetTop(UPPath);
	 #endif
	 }
	 if(EmP.assemblyType!=COMPONENT){
            std::string  PPath=partDir+std::string ("/interfaces/P"); 
	    sprintf(tag,"%d",PI); PPath+=tag;
            PPath=nativePath(PPath);
	    #ifdef USE_SYMBOLIC_LINKS
	     createLink(UPRPath.c_str(), PPath.c_str());
	    #else
	     createLink(UPPath.c_str(), PPath.c_str());
	    #endif
	 }
         // set points boundary conditions:
	 DB::Volume *vol=EmP.FindVolume(partName.ToCString());
	 if(vol->type==BOUNDARYCOND) if(FileExists(UPPath.c_str())){
	   std::ifstream in;
           in.open(UPPath.c_str());
//	   if(!in) continue;
	   if(!in){
             cout<< "missing file :"<< UPPath.c_str() <<endl;
	     assert(false);
 	   }
	   VertexData pd;
	   pd.read(in);
           in.close();
	   std::string str=std::string(vol->material);
	   if(pd.BrCond.find(str)==pd.BrCond.end()){
	      pd.BrCond.insert(str);
	      std::ofstream out;
              out.open(UPPath.c_str());
	      pd.write(out);
              out.close();
	   }	   
	 }
      }
   }


// ************  save Edge Links:
   TopLoc_Location loc=location();
   I=0;
   std::set<int> initializedE;
   int *sharedE;
   sharedE=new int[indexedEdges->Extent()];
   for (int EI=1; EI<=indexedEdges->Extent(); EI++) sharedE[EI-1]=0;
   if(EmP.assemblyType==NET) for (int FI=extFaceNum+1; FI<=indexedFaces->Extent(); FI++){
      TopoDS_Shape F = indexedFaces->FindKey(FI);
      for (TopExp_Explorer it(F,TopAbs_EDGE); it.More(); it.Next()){
        TopoDS_Shape E = it.Current();
        E.Orientation(TopAbs_FORWARD);
        int EI=indexedEdges->FindIndex(E);
	if (EI>0) sharedE[EI-1]=1;
      }
   }
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
      I++;
      TDF_Label label1 = it.Value();
      Handle(TDataStd_Name)  nameAtt;
      if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
      TCollection_AsciiString partName=nameAtt->Get();
      Handle(TDF_Reference)  refAtt;
      if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
      TDF_Label label = refAtt->Get();
      DB::Volume *vol=EmP.FindVolume(partName.ToCString());
      if(!vol) continue;
      if(vol->type==SPLITTER || vol->type==GRID) continue;
      std::string partDir=projectDir+std::string("/")+partName.ToCString();
      partDir=nativePath(partDir);
      assert(!partEdgesMap[I-1].IsNull());
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
	    int UEI=indexedEdges->FindIndex(E);
	    if((bdrFaces.Contains(F))) Emult[UEI]+=2;
	    else if(vol->type==BOUNDARYCOND) Emult[UEI]++;
	    if(Enormals.find(UEI)==Enormals.end()){
		  gp_Vec n1=supNormal(GS, P1);
		  gp_Vec n2=gp_Vec(0.0,0.0,0.0);
                  Enormals[UEI]=std::pair<gp_Vec, gp_Vec >(n1,n2);
	    } else {
                  Enormals[UEI].second=supNormal(GS, P1);
            }

	  }
	 }
      }
      if(!partEdgesMap[I-1].IsNull()) for (int EI =1; EI <= partEdgesMap[I-1]->Length(); EI++){
	 int UEI=partEdgesMap[I-1]->Value(EI);
	 if(!UEI) continue;
	 char tag[10]; sprintf(tag,"%d",UEI);
         std::string UEPath=projectDir+std::string("/interfaces/E")+tag;
         std::string UERPath=std::string("../../interfaces/E")+tag;
         UEPath=nativePath(UEPath);
         UERPath=nativePath(UERPath);
	 if(initializedE.find(UEI)==initializedE.end()) if(!FileExists(UEPath.c_str())){
             TopoDS_Edge E = TopoDS::Edge(indexedEdges->FindKey(UEI));
             bool  CumOri=false;
             TopoDS_Vertex V1,V2;
             TopExp::Vertices(E,V1,V2, CumOri);
             V1.Move(loc);
	     EdgeData ed;
	     ed.level=EmP.level;
             gp_Pnt P1 = BRep_Tool::Pnt(V1);
	     ed.P1[0] =P1.X();
	     ed.P1[1] =P1.Y();
	     ed.P1[2] =P1.Z();
	     ed.epsr=1.0; 
	     ed.mur=1.0;
	     ed.meshref=1.0; 
	     char cname[20]; sprintf(cname,"L%dE%d",EmP.level,UEI);
	     ed.name=cname;
	     std::ofstream out;
             out.open(UEPath.c_str());
	     ed.write(out);
             out.close();
	     initializedE.insert(UEI);
	 #ifndef USE_SYMBOLIC_LINKS
	     FileSetTop(UEPath);
	 #endif
	 }
	 if(EmP.assemblyType!=COMPONENT){
            std::string EPath=partDir+std::string("/interfaces/E"); 
	    sprintf(tag,"%d",EI); EPath+=tag;
            EPath=nativePath(EPath);
	    #ifdef USE_SYMBOLIC_LINKS
	     createLink(UERPath.c_str(), EPath.c_str());
	    #else
	     createLink(UEPath.c_str(), EPath.c_str());
	    #endif
	 }
         // set edges boundary conditions:
	 DB::Volume *vol=EmP.FindVolume(partName.ToCString());
	 if(vol->type==BOUNDARYCOND || vol->type==LINEPORT) if(FileExists(UEPath.c_str())){
	    std::ifstream in;
            in.open(UEPath.c_str());
//	    if(!in) continue;
	    if(!in){
              cout<< "missing file :"<< UEPath.c_str() <<endl;
	      assert(false);
 	    }
	    EdgeData ed;
	    ed.read(in);
            in.close();
	    int edChanged=0;
	    if(vol->type==BOUNDARYCOND) if(ed.BrCond.find(std::string(vol->material))==ed.BrCond.end()){
	        ed.BrCond.insert(std::string(vol->material));
		edChanged=1;
            }
	    if(vol->type==LINEPORT) if(ed.LPname!=std::string(vol->name)){
	        ed.LPname=std::string(vol->name);
		edChanged=1;
            }
//       Defines singular edges and singular points
            bool singularE=(Emult[UEI]==1);
	    if(Emult[UEI]>1 ) if(Enormals.find(UEI)!=Enormals.end()) 
		    singularE=singularE || (Enormals[UEI].first.CrossMagnitude(Enormals[UEI].second)>0.7);
	    if(singularE){
	       ed.singular=1; edChanged=1;
               TopoDS_Edge E = TopoDS::Edge(indexedEdges->FindKey(UEI));
               TopoDS_Vertex VV[2];
               bool  CumOri=false;
               TopExp::Vertices(E,VV[0],VV[1], CumOri);
               for (int i =0; i <2; i++){
                  int UPI=indexedVertices->FindIndex(VV[i]);
		  if(UPI<=extVertexNum){
                    char tag[10]; sprintf(tag,"%d",UPI);
                    std::string UPPath=projectDir+std::string("/interfaces/P")+tag;
                    UPPath=nativePath(UPPath);
                    std::ifstream in;
                    in.open(UPPath.c_str());
		    VertexData pd;
	            pd.read(in);
                    in.close();
		    pd.singular=1;
	            std::ofstream out;
                    out.open(UPPath.c_str());
	            pd.write(out); 
                    out.close();
	          }
	       }
	     }
	     if(edChanged){
	        std::ofstream out;
                out.open(UEPath.c_str());
	        ed.write(out);
                out.close();
	     }	   
	  }
      }
   }
   delete [] sharedE;

}

void MwOCAF::saveFEproperties()
{
//   int I1=EmP.isTopLevel? I1 : extFaceNum+1;
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
      DB::Volume *vol=EmP.FindVolume(partName.ToCString());
      if(EmP.assemblyType==COMPONENT && vol->type!=DIELECTRIC && vol->type!=WAVEGUIDE  && vol->type!=BOUNDARYCOND) continue;
      DB::Material* mat= EmP.FindMaterial(vol->material);
      Handle(TDF_Reference)  refAtt;
      if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
      TDF_Label label = refAtt->Get();
      TopoDS_Shape S = shapeTool->GetShape(label);
      for(TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
	 TopoDS_Shape F=exp.Current();
	 F.Orientation(TopAbs_FORWARD);
	 int UFI=indexedFaces->FindIndex(F);
	 if(UFI==0) continue;
	 char tag[10]; sprintf(tag,"%d",UFI);
	 std::string UFPath=projectDir+std::string("/interfaces/F")+tag;
         UFPath=nativePath(UFPath);
         // set faces refraction index (maximum among adjiacent diel):
	#ifdef USE_SYMBOLIC_LINKS
	 bool reset=!FileIsSymlink(UFPath.c_str()) && initializedF.find(UFI)==initializedF.end();
	#else
	 bool reset=initializedF.find(UFI) == initializedF.end() && FileIsTop(UFPath.c_str());
	#endif
         std::ifstream in;
         in.open(UFPath.c_str());
	 if(!in){
           cout<< "missing file :"<< UFPath.c_str() <<endl;
	   assert(false);
	 }
	 FaceData FD;
	 FD.read(in);
         in.close();
	 if(reset){
	    FD.epsr=FD.mur=FD.meshref=1.0;
            initializedF.insert(UFI);
	 }
	 if(mat){
	  FD.epsr   =max(FD.epsr,   mat->epsr);
	  FD.mur    =max(FD.mur,    mat->mur);
	  FD.meshref=max(FD.meshref,vol->meshRefinement);
	 }
	 std::ofstream out;
         out.open(UFPath.c_str());
	 FD.write(out);
         out.close();
      }
      std::set<int> doneE;
      for(TopExp_Explorer exp(S,TopAbs_EDGE); exp.More(); exp.Next()){
	 TopoDS_Shape E=exp.Current();
	 E.Orientation(TopAbs_FORWARD);
	 int UEI=indexedEdges->FindIndex(E);
	 if(UEI==0) continue;
	 if(doneE.find(UEI)!=doneE.end()) continue;
	 doneE.insert(UEI);
	 char tag[10]; sprintf(tag,"%d",UEI);
         std::string  UEPath=projectDir+std::string ("/interfaces/E")+tag;
         UEPath=nativePath(UEPath);
         // set edges refraction index (maximum among adjiacent diel):
	#ifdef USE_SYMBOLIC_LINKS
	 bool reset=!FileIsSymlink(UEPath.c_str()) && initializedE.find(UEI)==initializedE.end();
	#else
	 bool reset=initializedE.find(UEI) == initializedE.end() && FileIsTop(UEPath.c_str());
	#endif
         std::ifstream in;
         in.open(UEPath.c_str());
	 if(!in){
           cout<< "missing file :"<< UEPath.c_str() <<endl;
	   assert(false);
	 }
	 EdgeData ED;
	 ED.read(in);
	 if(reset){ 
	     ED.epsr=ED.mur=ED.meshref=1.0;  
	     initializedE.insert(UEI);
	 }
	 if(mat){
	  ED.epsr   =max(ED.epsr,   mat->epsr);
	  ED.mur    =max(ED.mur,    mat->mur);
	  ED.meshref=max(ED.meshref,vol->meshRefinement);
	 }
         in.close();
	 std::ofstream out;
         out.open(UEPath.c_str());
	 ED.write(out);
         out.close();
      }
      std::set<int> doneP;
      for(TopExp_Explorer exp(S,TopAbs_VERTEX); exp.More(); exp.Next()){
	 TopoDS_Shape V=exp.Current();
	 V.Orientation(TopAbs_FORWARD);
	 int UPI=indexedVertices->FindIndex(V);
	 if(UPI==0) continue;
	 if(doneP.find(UPI)!=doneP.end()) continue;
	 doneP.insert(UPI);
	 char tag[10]; sprintf(tag,"%d",UPI);
         std::string UPPath=projectDir+std::string("/interfaces/P")+tag;
         UPPath=nativePath(UPPath);
         // set points refraction index (maximum among adjiacent diel):
	#ifdef USE_SYMBOLIC_LINKS
	 bool reset=!FileIsSymlink(UPPath.c_str()) && initializedP.find(UPI)==initializedP.end();
	#else
	 bool reset=initializedP.find(UPI) == initializedP.end() && FileIsTop(UPPath.c_str());
	#endif
         std::ifstream in;
         in.open(UPPath.c_str());
	 if(!in){
           cout<< "missing file :"<< UPPath.c_str() <<endl;
	   assert(false);
	 }
	 VertexData PD;
	 PD.read(in);
	 if(reset){
	    PD.epsr=PD.mur=PD.meshref=1.0; 
	    initializedP.insert(UPI);
	 }
	 if(mat){
	  PD.epsr   =max(PD.epsr,   mat->epsr);
	  PD.mur    =max(PD.mur,    mat->mur);
	  PD.meshref=max(PD.meshref,vol->meshRefinement);
	 }
         in.close();
	 std::ofstream out;
         out.open(UPPath.c_str());
	 PD.write(out);
         out.close();
      }
   }
}


void MwOCAF::setDisabledVolumes()
{
   if(EmP.assemblyType==COMPONENT)
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
   // link assembly faces to part faces 
      TDF_Label label1 = it.Value();
      Handle(TDataStd_Name)  nameAtt;
      if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) continue;
      TCollection_AsciiString partName=nameAtt->Get();
      DB::Volume *vol=EmP.FindVolume(partName.ToCString());
      if(vol->type!=WAVEGUIDE) continue;
      Handle(TDF_Reference)  refAtt;
      if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
      TDF_Label label = refAtt->Get();
      TopoDS_Shape S = shapeTool->GetShape(label);
      for(TopExp_Explorer exp(S,TopAbs_FACE); exp.More(); exp.Next()){
	 TopoDS_Shape F=exp.Current();
	 F.Orientation(TopAbs_FORWARD);
	 int UFI=indexedFaces->FindIndex(F);
	 if(UFI==0) continue;
	 char tag[10]; sprintf(tag,"%d",UFI);
	 std::string UFPath=projectDir+std::string("/interfaces/F")+tag;
         UFPath=nativePath(UFPath);
         std::ifstream in;
         in.open(UFPath.c_str());
	 if(!in){
           cout<< "missing file :"<< UFPath.c_str() <<endl;
	   assert(false);
	 }
	 FaceData FD;
	 FD.read(in);
         in.close();
	 if(FD.shared) {vol->disabled=1; break;}
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
	   if(!LPedge(EI)){
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
	   if(LPedge(EI)) 
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
//   int I1=EmP.isTopLevel? I1 : extFaceNum+1;
   for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) 
   {
   // link assembly faces to part faces 
      TDF_Label label1 = it.Value();
      Handle(TDataStd_Name)  nameAtt;
      if(!label1.FindAttribute(TDataStd_Name::GetID(),nameAtt)) return;
      TCollection_AsciiString partName=nameAtt->Get();
      DB::Volume *vol=EmP.FindVolume(partName.ToCString());
      if(vol->type!=WAVEGUIDE) continue;
      if(vol->disabled) continue;
      Handle(TDF_Reference)  refAtt;
      if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
      TDF_Label label = refAtt->Get();
      TopoDS_Shape S = shapeTool->GetShape(label);
      int TEMnum=evalTEMnum(S, false);
      if(vol->TEMportsNum!=TEMnum) {vol->TEMportsNum=TEMnum; worksaveNeeded=true;}
//--------
   }
}

void MwOCAF::getAssName(TCollection_AsciiString &assName){
    Handle(TDataStd_Name)  nameAtt;
    if(theParts.FindAttribute(TDataStd_Name::GetID(),nameAtt)) assName=nameAtt->Get();
}
void MwOCAF::getAssName(std::string &assName){
  TCollection_AsciiString name; getAssName(name);
  assName=std::string(name.ToCString());
}


void MwOCAF::setFaceDataLock(){
 std::string lockFileName=projectDir;
 for (int i = 0; i < EmP.level; ++i) lockFileName+="/..";
 lockFileName+="/Data/faceData.lock";
 faceDataLock=fopenWithLock(lockFileName.c_str(), "w");
}
void MwOCAF::freeFaceDataLock(){
 fcloseWithLock(faceDataLock);
}

void MwOCAF::setFaceComp(){
    setFaceDataLock();
    TCollection_AsciiString assName;
    getAssName(assName);
    if(hasUPIF) for (int I = 1; I <=UPIFsubshapes.Extent() ; I++){
        TopoDS_Shape F =UPIFsubshapes.FindKey(I);
        if(F.ShapeType()!=TopAbs_FACE) continue;
        F.Orientation(TopAbs_FORWARD);
        int UFI=indexedFaces->FindIndex(F);
	if(UFI<0) continue;
	std::string UFPath=projectDir+std::string("/interfaces/F");
        char tag[10]; sprintf(tag,"%d",UFI); UFPath+=tag;
        UFPath=nativePath(UFPath);
        std::ifstream in;
        in.open(UFPath.c_str());
	if(!in){
           cout<< "missing file :"<< UFPath.c_str() <<endl;
	   assert(false);
	}
	FaceData FD;
	FD.read(in);
        in.close();
	std::ofstream out;
        out.open(UFPath.c_str());
//        if(faceAdjParts[2*(UFI-1)+0]==TCollection_AsciiString("-") || faceAdjParts[2*(UFI-1)+1]==TCollection_AsciiString("-")){
	   if(FD.cmp1==std::string("-")) FD.cmp1=assName.ToCString();
	   else if(FD.cmp2==std::string("-")) {
	        FD.cmp2=assName.ToCString();
		if(FD.cmp1>FD.cmp2){ std::string tmp=FD.cmp1; FD.cmp1=FD.cmp2; FD.cmp2=tmp;}
	   }
//	}
	FD.write(out);
        out.close();
    }
    freeFaceDataLock();

}

void MwOCAF::setDefaultFEPBC()
{
//   int I1=EmP.isTopLevel? I1 : extFaceNum+1;
      std::set<int> defBCedges;
      std::set<int> defBCvertices;
      for(int UFI = 1; UFI <= extFaceNum; UFI++) {
        std::string UFPath=projectDir+std::string("/interfaces/F");
        char tag[10]; sprintf(tag,"%d",UFI); UFPath+=tag;
        UFPath=nativePath(UFPath);
        std::ifstream in;
        in.open(UFPath.c_str());
	if(!in){
           cout<< "missing file :"<< UFPath.c_str() <<endl;
	   assert(false);
	}
	FaceData fd;
	fd.read(in);
        in.close();
        bool hasBdrCond=fd.BrCond.size()>0;
        if(hasBdrCond || fd.Shared()) continue;
        std::fstream out;
        out.open(UFPath.c_str(), std::ios::out | std::ios::app);
        out << std::string(EmP.defaultBC) <<"\n";
        out.close();
        TopoDS_Shape F = indexedFaces->FindKey(UFI);
        for (TopExp_Explorer exp(F,TopAbs_EDGE); exp.More(); exp.Next()){
          TopoDS_Shape E = exp.Current();
	  E.Orientation(TopAbs_FORWARD);
          int EI=indexedEdges->FindIndex(E);
          if(EI>0) if(defBCedges.find(EI)==defBCedges.end()) defBCedges.insert(EI);
        }
        for (TopExp_Explorer exp(F,TopAbs_VERTEX); exp.More(); exp.Next()){
          TopoDS_Shape V = exp.Current();
	  V.Orientation(TopAbs_FORWARD);
          int PI=indexedVertices->FindIndex(V);
          if(PI>0) if(defBCvertices.find(PI)==defBCvertices.end()) defBCvertices.insert(PI);
        }
      }
      typedef std::set<int>::const_iterator IsetIt;
      for (IsetIt it=defBCedges.begin(); it!= defBCedges.end(); it++){
	 int UEI=*it;
         std::string  UEPath=projectDir+std::string ("/interfaces/E");
	 char tag[10];
	 sprintf(tag,"%d",UEI); UEPath+=tag;
         UEPath=nativePath(UEPath);
	 std::ifstream in;
         in.open(UEPath.c_str());
	 if(!in){
           cout<< "missing file :"<< UEPath.c_str() <<endl;
	   assert(false);
	 }
	 if(!in) continue;
	 EdgeData ed;
	 ed.read(in);
         in.close();
	 std::string str=std::string(EmP.defaultBC);
	 if(ed.BrCond.find(str)==ed.BrCond.end()){
	    ed.BrCond.insert(str);
	    std::ofstream out;
            out.open(UEPath.c_str());
	    ed.write(out);
            out.close();
 	 }	   
      }
      for (IsetIt it=defBCvertices.begin(); it!= defBCvertices.end(); it++){
	 int UPI=*it;
         std::string UPPath=projectDir+std::string("/interfaces/P");
	 char tag[10];
	 sprintf(tag,"%d",UPI); UPPath+=tag;
         UPPath=nativePath(UPPath);
	 std::ifstream in;
         in.open(UPPath.c_str());
	 if(!in){
           cout<< "missing file :"<< UPPath.c_str() <<endl;
	   assert(false);
	 }
	 VertexData pd;
	 pd.read(in);
         in.close();
	 std::string str=std::string(EmP.defaultBC);
	 if(pd.BrCond.find(str)==pd.BrCond.end()){
	    pd.BrCond.insert(str);
	    std::ofstream out;
            out.open(UPPath.c_str());
	    pd.write(out);
            out.close();
 	 }	   
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
  Handle(BRepAdaptor_HSurface)     aBAHS1      = new BRepAdaptor_HSurface(aBAS1);
  Handle(BRepAdaptor_HSurface)     aBAHS2      = new BRepAdaptor_HSurface(aBAS2);
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
//   int I1=EmP.isTopLevel? I1 : extFaceNum+1;
      std::vector<FaceData_ *> fdata;
      int ENum=indexedEdges->Extent();
      int      *tE=new int[ENum];
      for(int i =0; i < ENum; i++) tE[i]=-1;
      for(int FI = 1; FI <= extFaceNum; FI++) {
        std::string FPath=projectDir+std::string("/interfaces/F");
        char tag[10]; sprintf(tag,"%d",FI); FPath+=tag;
        FPath=nativePath(FPath);
        std::ifstream in;
        in.open(FPath.c_str());
	if(!in){
           cout<< "missing file :"<< FPath.c_str() <<endl;
	   assert(false);
	}
	FaceData_ *fd=new(FaceData_);
        fd->read(in);
	if(fd->cmp1!=fd->cmp2 && fd->cmp1!=std::string("-") && fd->cmp2!=std::string("-")){
	  fd->FI=FI;
          fd->sfname=fd->name;
	  fdata.push_back(fd);
	}else{
	  delete fd;
	}
        in.close();
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
      int linkEdgeNum=EFlinks.size();
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
	   if(tE[EI-1]==-1) { changed=true; tE[EI-1]=isTangentFaces(E, F1, F2)? 1:0; }
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
	 StringPair cmpCouple(fdata[I-1]->cmp1, fdata[I-1]->cmp2);
	 sfnameRemap[cmpCouple][fdata[I-1]->sfname]=0;
      }
      typedef std::map<StringPair, StringIndex>::iterator cmpCoupleIt;
      typedef std::map<std::string,int>::iterator StringIndexIt;
      for (cmpCoupleIt cit=sfnameRemap.begin(); cit!= sfnameRemap.end(); cit++){
        int SFI=0;
        for (StringIndexIt sit=(*cit).second.begin(); sit!= (*cit).second.end(); sit++) (*sit).second=++SFI;
      }
      for(int I = 1; I <= NF; I++) {
	 StringPair cmpCouple(fdata[I-1]->cmp1, fdata[I-1]->cmp2);
	 int SFI=sfnameRemap[cmpCouple][fdata[I-1]->sfname];
	 fdata[I-1]->sfname=fdata[I-1]->cmp1+std::string("_")+fdata[I-1]->cmp2+std::string("_SF")+std::to_string(SFI);
	 int FI=fdata[I-1]->FI;
         std::string FPath=projectDir+std::string ("/interfaces/F");
         char tag[10]; sprintf(tag,"%d",FI); FPath+=tag;
         FPath=nativePath(FPath);
         std::ofstream out;
         out.open(FPath.c_str());
         fdata[I-1]->write(out);
         out.close();
      }

      for(int I = 1; I <= NF; I++) delete fdata[I-1];
      delete [] tE;
}

void MwOCAF::checkSuperFacesTEMnum()
{
//  ckeck TEM_Num (must be 0 for superface): 
      std::map<std::string, int, std::less<std::string> > SFindex;
      TopTools_IndexedMapOfShape  superF;
      BRep_Builder builder;
      int SFnum=0;
      int FNum=indexedFaces->Extent();
      for(int FI = 1; FI <= FNum; FI++) if(faceData[FI-1].shared){
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


void MwOCAF::makeIF(TopoDS_Compound &IF)
{
  TopTools_MapOfShape aMapOfFaces;
  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
     TDF_Label label1 = it.Value();
     Handle(TDF_Reference)  refAtt;
     assert(label1.FindAttribute(TDF_Reference::GetID(),refAtt));
     TDF_Label label = refAtt->Get();
     bool skip=false;
     DB::Volume *vol=getLabelVol(label);
     if(vol) skip=(EmP.assemblyType==COMPONENT && vol->type!=DIELECTRIC);
     if(skip) continue;
     TopoDS_Shape S = shapeTool->GetShape(label);
     for (TopExp_Explorer ex1(S,TopAbs_FACE); ex1.More(); ex1.Next())
         if(!aMapOfFaces.Add(ex1.Current()))  aMapOfFaces.Remove(ex1.Current());
  }

  if(EmP.assemblyType==COMPONENT)  for (TDF_ChildIterator it(theParts,Standard_False); it.More(); it.Next()) {
     TDF_Label label1 = it.Value();
     Handle(TDF_Reference)  refAtt;
     assert(label1.FindAttribute(TDF_Reference::GetID(),refAtt));
     TDF_Label label = refAtt->Get();
     DB::Volume *vol=getLabelVol(label);
     if(!vol) continue;
     if(vol->type==BOUNDARYCOND) { //Remove BC faces from the interface
      TopoDS_Shape S = shapeTool->GetShape(label);
      for (TopExp_Explorer ex1(S,TopAbs_FACE); ex1.More(); ex1.Next())
        if(aMapOfFaces.Contains(ex1.Current())) aMapOfFaces.Remove(ex1.Current());
     }
  }
  BRep_Builder builder;
  builder.MakeCompound(IF);
  for(TopTools_MapIteratorOfMapOfShape amapIter(aMapOfFaces);amapIter.More(); amapIter.Next()) {
      TopoDS_Shape F=amapIter.Key();
      builder.Add(IF,F);
  }

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
	EmP.stepFilePath=fileName;
	EmP.hasGeo=true;
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
	} else  openDoc(EmP.occFilePath.c_str());
	return reloaded;
}


