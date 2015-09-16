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

//#include "fmesh.h"
#include "OStools.h"
#include "GmshConfig.h"
#include "Gmsh.h"
#include "GModel.h"
#include "GmshDefines.h"
#include "Context.h"
#include "meshGEdge.h"
#include "MLine.h"
#include "Field.h"
//#include "GModelIO_OCC.h"
#include "assert.h"
#include <string>

#include "mesher.h"
//#include "mymeshGFaceBamg.h"

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp_Explorer.hxx>
#include <TCollection_AsciiString.hxx>

#include <BRep_Builder.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>

#include <TDF_ChildIterator.hxx>
#include <TDF_Reference.hxx>

#include "DataBase.h"


#ifndef PIG
#define PIG 3.14159265
#endif

#ifndef SIGN
#define SIGN(a)  (((a) > 0) ? 1 : -1)
#endif

#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifdef WNT
#define snprintf _snprintf
#endif


bool mesh_aniso=false; 
bool useFaceAttractor=false;
bool useBoundaryLayer=false;

struct Reloc
{
  TopTools_IndexedMapOfShape  *indexedFaces;
  TopTools_IndexedMapOfShape  *indexedEdges;
  TopTools_IndexedMapOfShape  *indexedVertices;
} ;


void print_mwm(GModel *gm,  MwOCAF* ocaf, bool isPartition,
	      const char* dirName, const char* modelDir,
	      TopTools_IndexedMapOfShape *indexedFaces,
	      TopTools_IndexedMapOfShape *indexedEdges,
	      TopTools_IndexedMapOfShape *indexedVertices
	      );



void mesher_setLC(GModel *gm, double meshsize){
      int minCircPoints=10;
      for(GModel::eiter it = gm->firstEdge(); it != gm->lastEdge(); it++){
	      GEdge *e=(*it);
	      Range<double> range =e->parBounds(0);
	      GVertex * v1=e->getBeginVertex();
	      double curv1 =e->curvature(range.low());
	      double lc1 = curv1 > 0 ? 2 * PIG/curv1 /minCircPoints : MAX_LC;
	      lc1=min(lc1,meshsize);
	      GVertex * v2=e->getEndVertex();
	      double curv2 =e->curvature(range.high());
	      double lc2 = curv2 > 0 ? 2 * PIG/curv2 /minCircPoints : MAX_LC;
	      lc2=min(lc2,meshsize);
	      v1->setPrescribedMeshSizeAtVertex(min(v1->prescribedMeshSizeAtVertex(),lc1));
	      v2->setPrescribedMeshSizeAtVertex(min(v2->prescribedMeshSizeAtVertex(),lc2));
      }
}





double max_refri(GModel *gm, MwOCAF* ocaf, Reloc &reloc){
//      for(GModel::viter it = gm->firstVertex(); it != gm->lastVertex(); it++)
//	      (*it)->setPrescribedMeshSizeAtVertex(meshsize);
     double refri=1.0;
     for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
         GFace *gf=*fit;
         TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
         int FI=reloc.indexedFaces->FindIndex(F);
         assert(FI);
         if(!FI) continue;
         if(ocaf->faceAdjParts) for (int parti=0; parti< 2; parti++){
             TCollection_AsciiString volname=ocaf->faceAdjParts[2*(FI-1)+parti];
             if(volname!=TCollection_AsciiString("?")){
		 DB::Volume *vol=ocaf->EmP.FindVolume(volname.ToCString());
                 DB::Material* mat;
		 if(vol) mat= ocaf->EmP.FindMaterial(vol->material);
                 if(mat) refri=max(refri,sqrt(mat->epsr * mat->mur));
             }
	 }
     }
     return refri;
}


void mesher_refine(GEdge *ge, double edgeMeshSize){
  int N = ge->mesh_vertices.size();
  if(N<1) return;
  std::vector<MVertex*>  newmesh_vertices(N+2);
  Range<double> bounds = ge->parBounds(0);
  double t_begin = bounds.low();
  double t_end = bounds.high();
  for(int i = 0; i < N; i++){
    double t; ge->mesh_vertices[i]->getParameter(1,t);
    GPoint P = ge->point(t);
    if(i==0){ 
      double t2=(t+t_begin)/2;
      GPoint P2 = ge->point(t2);
      newmesh_vertices[0]=new MEdgeVertex(P2.x(), P2.y(), P2.z(), ge, t2, edgeMeshSize/2);
    }
    newmesh_vertices[i+1]=new MEdgeVertex(P.x(), P.y(), P.z(), ge, t, edgeMeshSize);
    if(i==N-1){ 
      double t2=(t+t_end)/2;
      GPoint P2 = ge->point(t2);
      newmesh_vertices[N+1]=new MEdgeVertex(P2.x(), P2.y(), P2.z(), ge, t2, edgeMeshSize/2);
    }
  }
  deMeshGEdge dem;
  dem(ge);
  ge->mesh_vertices.resize(N+2);
  for(unsigned int i = 0; i < newmesh_vertices.size(); i++)   ge->mesh_vertices[i]=newmesh_vertices[i];
  for(unsigned int i = 0; i < ge->mesh_vertices.size() + 1; i++){
    MVertex *v0 = (i == 0) ?
      ge->getBeginVertex()->mesh_vertices[0] : ge->mesh_vertices[i - 1];
    MVertex *v1 = (i == ge->mesh_vertices.size()) ?
      ge->getEndVertex()->mesh_vertices[0] : ge->mesh_vertices[i];
    ge->lines.push_back(new MLine(v0, v1));
  }
}


void mesher_setTags(GModel *gm, 
	            TopTools_IndexedMapOfShape *indexedFaces,
	            TopTools_IndexedMapOfShape *indexedEdges,
	            TopTools_IndexedMapOfShape *indexedVertices
	      )
{
      for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
        GFace *gf=*fit;
	TopoDS_Shape F=* (TopoDS_Face *) gf->getNativePtr();
        int FI=indexedFaces->FindIndex(F);
        gf->setTag(FI);
      }
      for(GModel::eiter it = gm->firstEdge(); it != gm->lastEdge(); it++){
	      GEdge *ge=(*it);
              TopoDS_Shape E=* (TopoDS_Edge *) ge->getNativePtr();
              int EI=indexedEdges->FindIndex(E);
              ge->setTag(EI);
      }
      for(GModel::viter it = gm->firstVertex(); it != gm->lastVertex(); it++){
	      GVertex *gv=(*it);
              TopoDS_Shape V=* (TopoDS_Vertex *) gv->getNativePtr();
              int VI=indexedVertices->FindIndex(V);
              gv->setTag(VI);
      }
}

std::string double_to_string(double d){
  char buffer[32];
  snprintf(buffer, 32, "%g", d);
  return std::string(buffer);
}

void mesher_setGlobalAniso(GModel *gm,
	            TopTools_IndexedMapOfShape *indexedEdges,
		    double meshsize, double refx, double refy, double refz,
		    int &fieldNum, std::list<int> &mshFieldList)
{
  FieldManager *fields = gm->getFields();
  fieldNum++;  mshFieldList.push_back(1+fieldNum);
  Field *f=fields->newField(1+fieldNum, "MathEvalAniso");
  double lx=meshsize/refx,ly=meshsize/refy,lz=meshsize/refz;
  SVector3 vx(1,0,0);
  SVector3 vy(0,1,0);
  SVector3 vz(0,0,1);
  SMetric3 m(1./(lx*lx),1./(ly*ly),1./(lz*lz),vx,vy,vz);
  f->options["m11"]->string(double_to_string(m(0,0)));
  f->options["m22"]->string(double_to_string(m(1,1)));
  f->options["m33"]->string(double_to_string(m(2,2)));
  f->options["m12"]->string(double_to_string(m(0,1)));
  f->options["m13"]->string(double_to_string(m(0,2)));
  f->options["m23"]->string(double_to_string(m(1,2)));
}


void mesher_setSingularVertices(GModel *gm,
	            TopTools_IndexedMapOfShape *indexedVertices,
		    double meshsize,  VertexData  *vertexData,
		    int &fieldNum, std::list<int> &mshFieldList)
{
  std::list<int> Vl;
  for(GModel::viter it = gm->firstVertex(); it != gm->lastVertex(); it++){
	  GVertex *gv=(*it); 
          TopoDS_Shape V=* (TopoDS_Shape *) gv->getNativePtr();
          int VI=indexedVertices->FindIndex(V);
	  if (vertexData[VI-1].singular)  Vl.push_back(gv->tag());
  }
  if(!Vl.size()) return;
  FieldManager *fields = gm->getFields();
  fieldNum++;
  Field *attractor=fields->newField(1+fieldNum, "Attractor");
  FieldOption *option = attractor->options["NodesList"]; 
  option->list(Vl);
  attractor->options["NNodesByEdge"]->numericalValue(100);
  fieldNum++; mshFieldList.push_back(1+fieldNum);
  Field *thsld=fields->newField(1+fieldNum, "Threshold");
  thsld->options["IField"]->numericalValue(fieldNum);
  thsld->options["LcMax"]->numericalValue(meshsize);
  if(mesh_aniso){
        thsld->options["DistMin"]->numericalValue(meshsize*0.6);
        thsld->options["DistMax"]->numericalValue(meshsize*2.5);
	thsld->options["LcMin"]->numericalValue(meshsize/3);
  }else{
        thsld->options["DistMin"]->numericalValue(meshsize*0.6);
        thsld->options["DistMax"]->numericalValue(meshsize*3.0);
        thsld->options["LcMin"]->numericalValue(meshsize*0.3);
  }
}


void mesher_setRefinedVertices(GModel *gm,
	            TopTools_IndexedMapOfShape *indexedVertices,
		    double meshsize, int &fieldNum, std::list<int> &mshFieldList)
{
  std::list<int> Vl;
  for(GModel::viter it = gm->firstVertex(); it != gm->lastVertex(); it++){
	  GVertex *gv=(*it); 
          TopoDS_Shape V=* (TopoDS_Shape *) gv->getNativePtr();
	  Vl.push_back(gv->tag());
  }
  if(!Vl.size()) return;
  FieldManager *fields = gm->getFields();
  fieldNum++;
  Field *attractor=fields->newField(1+fieldNum, "Attractor");
  FieldOption *option = attractor->options["NodesList"]; 
  option->list(Vl);
  attractor->options["NNodesByEdge"]->numericalValue(100);
  fieldNum++; mshFieldList.push_back(1+fieldNum);
  Field *thsld=fields->newField(1+fieldNum, "Threshold");
  thsld->options["IField"]->numericalValue(fieldNum);
  thsld->options["DistMin"]->numericalValue(0);
  thsld->options["DistMax"]->numericalValue(meshsize*0.3);
  thsld->options["LcMin"]->numericalValue(meshsize*0.3);
  thsld->options["LcMax"]->numericalValue(meshsize);
}




void mesher_setEdgeMeshAttribute(GModel *gm, bool isPartition, 
	            TopTools_IndexedMapOfShape *indexedEdges,
		    double meshsize,  EdgeData  *edgeData, 
		    int &fieldNum, std::list<int> &mshFieldList)
{
      std::map<int, std::list<int> > refElist;
      std::map<int, std::list<int> > singElist;
      FieldManager *fields = gm->getFields();
      for(GModel::eiter it = gm->firstEdge(); it != gm->lastEdge(); it++){
	      GEdge *ge=(*it);
              TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
              int EI=indexedEdges->FindIndex(E);
	      double refri=sqrt(edgeData[EI-1].epsr * edgeData[EI-1].mur);
	      double edgeRef=refri*edgeData[EI-1].meshref;
	      int mshrefI=10*edgeRef+0.2;
	      double emeshSize=10*meshsize/mshrefI;
	      if(isPartition){ 
		      if (edgeData[EI-1].singular) 
		              singElist[mshrefI].push_back(ge->tag());
		      else                         
			      refElist[mshrefI].push_back(ge->tag());
	      }
	      GVertex * v1=ge->getBeginVertex();
	      GVertex * v2=ge->getEndVertex();
	      v1->setPrescribedMeshSizeAtVertex(min(v1->prescribedMeshSizeAtVertex(),emeshSize/edgeRef));
	      v2->setPrescribedMeshSizeAtVertex(min(v2->prescribedMeshSizeAtVertex(),emeshSize/edgeRef));

      }
      typedef std::map<int, std::list<int> >::const_iterator ElistIt;
      for (ElistIt it=refElist.begin(); it!= refElist.end(); it++){
	  int mshrefI=(*it).first;
	  double emeshSize=10*meshsize/mshrefI;
	  fieldNum++; mshFieldList.push_back(1+fieldNum);
          if(useBoundaryLayer){
             Field *attractor=fields->newField(1+fieldNum, "BoundaryLayer");
             attractor->options["EdgesList"]->list((*it).second);
             attractor->options["hwall_n"]->numericalValue(emeshSize/8);
             attractor->options["hwall_t"]->numericalValue(emeshSize/2);
             attractor->options["hfar"]->numericalValue(meshsize);
             attractor->options["Quads"]->numericalValue(1);
             attractor->options["ratio"]->numericalValue(2);
             attractor->options["IntersectMetrics"]->numericalValue(0);
             attractor->options["thickness"]->numericalValue(emeshSize/50);
          }else{
             Field *attractor=fields->newField(1+fieldNum, "AttractorAnisoCurve");
             attractor->options["EdgesList"]->list((*it).second);
             attractor->options["NNodesByEdge"]->numericalValue(100);
             attractor->options["dMin"]->numericalValue(emeshSize*0.7);
             attractor->options["dMax"]->numericalValue(emeshSize*0.9);
             attractor->options["lMinTangent"]->numericalValue(emeshSize/1.5);
             attractor->options["lMinNormal"]->numericalValue(emeshSize/2);
             attractor->options["lMaxTangent"]->numericalValue(meshsize);
             attractor->options["lMaxNormal"]->numericalValue(meshsize);
	  }
      }
      for (ElistIt it=singElist.begin(); it!= singElist.end(); it++){
	  int mshrefI=(*it).first;
	  double emeshSize=10*meshsize/mshrefI;
	  fieldNum++; mshFieldList.push_back(1+fieldNum);
	  {
             Field *attractor=fields->newField(1+fieldNum, "AttractorAnisoCurve");
             attractor->options["EdgesList"]->list((*it).second);
             attractor->options["NNodesByEdge"]->numericalValue(40);
             attractor->options["dMin"]->numericalValue(emeshSize*0.7);
             attractor->options["dMax"]->numericalValue(meshsize*2.5);
             attractor->options["lMinTangent"]->numericalValue(emeshSize/1.5);
             attractor->options["lMinNormal"]->numericalValue(emeshSize/2.5);
             attractor->options["lMaxTangent"]->numericalValue(meshsize);
             attractor->options["lMaxNormal"]->numericalValue(meshsize);
	  }
      }
}




void mesher_setBackgroundField(GModel *gm, std::list<int> &mshFieldList){
      if(mshFieldList.size()>0){
          FieldManager *fields = gm->getFields();
          Field *bkgfield=fields->newField(1, "MinAniso");
          bkgfield->options["FieldsList"]->list(mshFieldList);
          fields->setBackgroundFieldId(1);
      }
}


void mesher_setFaceMeshAttribute(GModel *gm,
	            TopTools_IndexedMapOfShape *indexedFaces,
		    double meshsize, FaceData  *faceData
	      )
{
      for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
              GFace *gf=*fit;
              TopoDS_Shape F=* (TopoDS_Shape *) gf->getNativePtr();
              int FI=indexedFaces->FindIndex(F);
	      double refri=sqrt(faceData[FI-1].epsr * faceData[FI-1].mur);
	      gf->meshAttributes.meshSize = meshsize/(refri*faceData[FI-1].meshref);
      }
}


void mesher_setFaceMeshAttribute(GModel *gm,
	            TopTools_IndexedMapOfShape *indexedFaces,
		    double meshsize, FaceData  *faceData,
		    int &fieldNum, std::list<int> &mshFieldList)
{
      std::map<int, std::list<int> > refFlist;
      FieldManager *fields = gm->getFields();
      for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
              GFace *gf=*fit;
              TopoDS_Shape F=* (TopoDS_Shape *) gf->getNativePtr();
              int FI=indexedFaces->FindIndex(F);
	      double refri=sqrt(faceData[FI-1].epsr * faceData[FI-1].mur);
	      double faceRef=refri*faceData[FI-1].meshref;
	      int mshrefI=10*faceRef+0.2;
	      if(mshrefI!=10)  refFlist[mshrefI].push_back(gf->tag());
      }
      typedef std::map<int, std::list<int> >::const_iterator refFlistIt;
      for (refFlistIt it=refFlist.begin(); it!= refFlist.end(); it++){
	   fieldNum++;
	   int mshrefI=(*it).first;
	   double fmeshSize=10*meshsize/mshrefI;
           Field *attractor=fields->newField(1+fieldNum, "Attractor");
           attractor->options["FacesList"]->list((*it).second);
           fieldNum++; mshFieldList.push_back(1+fieldNum);
           Field *thsld=fields->newField(1+fieldNum, "Threshold");
           thsld->options["IField"]->numericalValue(fieldNum);
           thsld->options["DistMin"]->numericalValue(0);
           thsld->options["DistMax"]->numericalValue(meshsize);
           thsld->options["LcMin"]->numericalValue(fmeshSize);
           thsld->options["LcMax"]->numericalValue(meshsize);
      }
}





void laplaceSmoothing(GFace *gf);

void MESHER::meshModel(MwOCAF* ocaf, double meshsize, const char* dirName, const char* modelDir)
{

    ocaf->regenerateIndexedSubShapes();
    bool isPartition=ocaf->EmP.assemblyType==PARTITION;
    bool isNET=ocaf->EmP.assemblyType==NET;
    if(isPartition){
     ocaf->makeFaceAdjCells();
     ocaf->makeFaceAdjBdrCond();
    }
    ocaf->setFEproperties();
//    ocaf->makeTheCompound();

// relocates faces with respect to upper assembly reference system
    Reloc reloc;
    TopLoc_Location loc=ocaf->location();
    reloc.indexedFaces=new TopTools_IndexedMapOfShape;
    for (int FI=1; FI<=ocaf->indexedFaces->Extent(); FI++){
      TopoDS_Shape F = ocaf->indexedFaces->FindKey(FI);
      F.Move(loc);
      reloc.indexedFaces->Add(F);
    }
    reloc.indexedEdges=new TopTools_IndexedMapOfShape;
    for (int EI=1; EI<=ocaf->indexedEdges->Extent(); EI++){
      TopoDS_Shape E = ocaf->indexedEdges->FindKey(EI);
      E.Move(loc);
      reloc.indexedEdges->Add(E);
    }
    reloc.indexedVertices=new TopTools_IndexedMapOfShape;
    for (int VI=1; VI<=ocaf->indexedVertices->Extent(); VI++){
      TopoDS_Shape V = ocaf->indexedVertices->FindKey(VI);
      V.Move(loc);
      reloc.indexedVertices->Add(V);
    }

//------------------

    BRep_Builder builder;
    TopoDS_Compound     theFaces;
    builder.MakeCompound(theFaces);


    for (int FI=1; FI<=reloc.indexedFaces->Extent(); FI++){
      TopoDS_Shape F = reloc.indexedFaces->FindKey(FI);
      if(!F.IsNull()) builder.Add(theFaces,F);
    }

/*
    int SI=0;
    for (TDF_ChildIterator it(ocaf->thePartsData,Standard_False); it.More(); it.Next()) {
       TDF_Label label1 = it.Value();
       Handle(TDF_Reference)  refAtt;
       if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
       TDF_Label label = refAtt->Get();
       TopoDS_Shape S = ocaf->shapeTool->GetShape(label);
       DB::Volume *vol=getLabelVol(label);
       ++SI;
       for (TopExp_Explorer it1(S.Oriented(TopAbs_FORWARD),TopAbs_FACE); it1.More(); it1.Next()){
         TopoDS_Face F = TopoDS::Face(it1.Current());
         int si=(F.Orientation()==TopAbs_FORWARD) ? 0 : 1;
         F.Orientation(TopAbs_FORWARD);
         int FI=ocaf->indexedFaces->FindIndex(F);
       }
     }
*/

//      int argc=3;
//      char *argv[argc];
//      char str1[]="-algo";  argv[1]=str1;
//      char str2[]="bamg";  argv[2]=str2;
//      char str2[]="del2d";  argv[2]=str2;
//      char str2[]="bds";  argv[2]=str2;
//      char str2[]="front2d";  argv[2]=str2;
//      char str3[]="optimize_lloyd"; argv[3]=str3;
//      char str3[]="smooth"; argv[3]=str3;
//      char str4[]="10";      argv[4]=str4;
//      char str3[]="anisoMax"; argv[3]=str3;
//      char str4[]="1.5";      argv[4]=str4;
//      GmshInitialize(argc, argv);
      GmshInitialize();
//      GmshSetOption("Mesh", "Algorithm", ALGO_2D_DELAUNAY);
//      GmshSetOption("Mesh", "Algorithm", ALGO_2D_FRONTAL);
      GModel *gm=new GModel();
      gm->importOCCShape(&theFaces);
//      setTags is deactivated because, by breaking the ordering of edges structure (a std::set),
//      it may lead to a crash of gmsh.
//      mesher_setTags(gm, reloc.indexedFaces, reloc.indexedEdges, reloc.indexedVertices);

      int fieldNum=0;
      std::list<int> mshFieldList;
      FieldManager *fields = gm->getFields(); fields->reset();
      mesher_setEdgeMeshAttribute(gm, isPartition, reloc.indexedEdges, meshsize, ocaf->edgeData,fieldNum, mshFieldList);
//      mesher_setGlobalAniso(gm, reloc.indexedEdges, meshsize, 1.0, 1.0, 2.0, fieldNum, mshFieldList);
      mesher_setSingularVertices(gm, reloc.indexedVertices, meshsize, ocaf->vertexData, fieldNum, mshFieldList);
      mesher_setBackgroundField(gm, mshFieldList);
      if(useFaceAttractor)
       mesher_setFaceMeshAttribute(gm, reloc.indexedFaces, meshsize, ocaf->faceData, fieldNum, mshFieldList);
      else
       mesher_setFaceMeshAttribute(gm, reloc.indexedFaces, meshsize, ocaf->faceData);

      for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
         GFace *gf=*fit;
	 TopoDS_Shape F=* (TopoDS_Face *) gf->getNativePtr();
         int FI=reloc.indexedFaces->FindIndex(F);
         if(ocaf->EmP.assemblyType!=PARTITION && !ocaf->faceData[FI-1].shared )
           gf->meshAttributes.method=MESH_NONE;
	 else
//           gf->meshAttributes.method=MESH_TRANSFINITE;
           gf->meshAttributes.method=MESH_UNSTRUCTURED;
      }
      if(mesh_aniso)  CTX::instance()->mesh.algo2d=ALGO_2D_BAMG;
      else            CTX::instance()->mesh.algo2d=ALGO_2D_MESHADAPT;
//      else            CTX::instance()->mesh.algo2d=ALGO_2D_DELAUNAY;
//      else            CTX::instance()->mesh.algo2d=ALGO_2D_FRONTAL;
//      else  CTX::instance()->mesh.algo2d=ALGO_2D_MESHADAPT_OLD;
      CTX::instance()->mesh.lcFromPoints=1;
      CTX::instance()->mesh.lcFromCurvature=1;
      CTX::instance()->mesh.lcExtendFromBoundary=0;
      CTX::instance()->mesh.minCircPoints=isNET? 7: 7;
      CTX::instance()->mesh.anisoMax=500000;
      CTX::instance()->mesh.nbSmoothing=0;
      CTX::instance()->mesh.smoothRatio=10.0;
      CTX::instance()->mesh.lcIntegrationPrecision=1.e-3;
      CTX::instance()->mesh.lcMax=meshsize*10;
      CTX::instance()->mesh.lcMin=meshsize/50;
/*
      GmshSetOption("Mesh", "Algorithm", (double) ALGO_2D_BAMG);
      GmshSetOption("Mesh", "lcFromPoints", 1.0);
      GmshSetOption("Mesh", "lcFromCurvature", 1.0);
      GmshSetOption("Mesh", "minCircPoints", 10.0);
      GmshSetOption("Mesh", "anisoMax", 100.0);
*/
//      CTX::instance()->mesh.smoothRatio=0.0;
//

      gm->mesh(1);

      gm->mesh(2);
/*
      for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
         GFace *gf=*fit;
	 if(gf->meshAttributes.Method ==MESH_TRANSFINITE) continue;
	 mymeshGFaceBamg(gf);
//         for(int i = 0; i < CTX::instance()->mesh.nbSmoothing; i++) laplaceSmoothing(gf);
      }
*/

      std::string outFileName=dirName;
      outFileName+="/model.msh";
      GmshWriteFile(outFileName);

      print_mwm(gm, ocaf, isPartition, 
		dirName, modelDir,
		reloc.indexedFaces,
		reloc.indexedEdges,
		reloc.indexedVertices
	       );

      delete gm;
      GmshFinalize();
      delete reloc.indexedFaces;
      delete reloc.indexedEdges;
      delete reloc.indexedVertices;

}


void MESHER::addIF(MwOCAF* ocaf, const char* dirName, const char* modelDir)
{

    ocaf->regenerateIndexedSubShapes();
    ocaf->setFEproperties();

    TCollection_AsciiString assName; ocaf->getAssName(assName);
    TCollection_AsciiString modelFileName=TCollection_AsciiString(modelDir)+"/"+assName+".mwm";

    FILE *fout=fopen(nativePath(modelFileName.ToCString()).c_str(), "w"); if(!fout) return;

    std::map<std::string, int, std::less<std::string> > WGIF;
    std::vector<DB::Material> WGIFmat;
    std::vector<double> WGIFarea;
    typedef std::map<std::string, int, std::less<std::string> >::const_iterator WGIFIt;

    int WGIFnum=0;
    for (int FI=1; FI<=ocaf->indexedFaces->Extent(); FI++)  if(ocaf->faceData[FI-1].Shared()){
      TopoDS_Shape F = ocaf->indexedFaces->FindKey(FI);
      GProp_GProps FProps;
      BRepGProp::SurfaceProperties(F, FProps);
      double area=fabs(FProps.Mass());
      if(WGIF.find(ocaf->faceData[FI-1].sfname)==WGIF.end()){
	WGIF[ocaf->faceData[FI-1].sfname]=WGIFnum++;
        DB::Material mat;
	strcpy(mat.name, ocaf->faceData[FI-1].sfname.c_str());
	mat.epsr=0.0;
	mat.mur=0.0;
	WGIFmat.push_back(mat);
	WGIFarea.push_back(0.0);
      }
      int wgi=WGIF[ocaf->faceData[FI-1].sfname];
      WGIFmat[wgi].epsr+=area*ocaf->faceData[FI-1].epsr;
      WGIFmat[wgi].mur+=area*ocaf->faceData[FI-1].mur;
      WGIFarea[wgi]+=area;
    }
    for (WGIFIt it=WGIF.begin(); it!= WGIF.end(); it++){
        int wgi=(*it).second;
	double epsr=WGIFmat[wgi].epsr/WGIFarea[wgi];
	double mur =WGIFmat[wgi].mur/WGIFarea[wgi];
        fprintf(fout, "DEF mat_%s  MWM_Material {\n",   WGIFmat[wgi].name);
        fprintf(fout, "   epsilonr          %g\n", epsr );
        fprintf(fout, "   mur               %g\n", mur);
        fprintf(fout, "}\n");
        fprintf(fout, "DEF %s  MWM_Volume {\n",  (*it).first.c_str());
        fprintf(fout, "  type  WaveGuide\n");
        fprintf(fout, "  material \"mat_%s\"\n",  WGIFmat[wgi].name);
        fprintf(fout, "}\n\n");
    }

    for (int FI=1; FI<=ocaf->indexedFaces->Extent(); FI++) if(ocaf->faceData[FI-1].Shared()){
      std::string faceFileName=dirName;
      char fname[50]; sprintf(fname,"F%d.mwm",FI);
      faceFileName+="/interfaces/";
      faceFileName+=fname;
      std::ifstream in;
      in.open(faceFileName.c_str()); if(!in) continue;

      fprintf(fout, "DEF UF%s  MWM_Surface {\n", ocaf->faceData[FI-1].name.c_str());
// Volumes Data associated with the UpperFace are the Current Component (indicated as '-') and the Superface.
// The two volumes are ordered according to the UpperFace orientation.
      int foreward= (ocaf->faceData[FI-1].cmp1 == std::string(assName.ToCString())) ? 1: 0;
      if(foreward)
       fprintf(fout, "  volumes  [\"-\", \"%s\"]\n",  ocaf->faceData[FI-1].sfname.c_str());
      else
       fprintf(fout, "  volumes  [\"%s\", \"-\"]\n",  ocaf->faceData[FI-1].sfname.c_str());
      std::string str;
      while (!getLine(in, str).eof()) fprintf(fout, "%s\n", str.c_str());
      fprintf(fout, "\tcolor 0.878 0.000 0.000\n");
      fprintf(fout, "}\n\n");
      in.close();
 /* ---*/
      fprintf(fout, "DEF UF%s  MWM_Volume {\n",  ocaf->faceData[FI-1].name.c_str());
      fprintf(fout, "  type  Upperface\n");
      fprintf(fout, "}\n\n");
     }
     fclose(fout);

}

