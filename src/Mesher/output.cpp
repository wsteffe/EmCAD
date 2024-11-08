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

extern bool REUSE_CIRCUITS;

#if defined(HAVE_OCE_CONFIG_H)
 #include <oce-config.h>
#elif defined(HAVE_OCC_CONFIG_H)
 #include <config.h>
#endif

#include "Context.h"
#include "GmshConfig.h"
#include "gmsh.h"
#include "GEntity.h"
#include "GModel.h"
#include "GFace.h"
#include "GEdge.h"
#include "MLine.h"
#include "MTriangle.h"
#include "MTetrahedron.h"
#include "MElement.h"

#include "GeomAPI_ProjectPointOnSurf.hxx"

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TCollection_AsciiString.hxx>

#include <TopExp_Explorer.hxx>
#include <TopoDS_Iterator.hxx>
#include <TopExp.hxx>
#include <BRep_Tool.hxx>
#include <gp_Dir.hxx>

#include <BRepAdaptor_Curve.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <GeomLProp_SLProps.hxx>

#include "assert.h"
#include <map>
#include <vector>
#include <algorithm>

#include "ocaf.h"
//#include "InputOutput.h"
#include "OStools.h"
#include "DataBase.h"
#include "mwm.h"

#ifndef WNT
#include <unistd.h>
#else
#include <windows.h>
#endif



#ifndef SIGN
#define SIGN(a)  (((a) > 0) ? 1 : -1)
#endif

#define DISTANCE(P1, P2) \
  sqrt( ((P1)[0]-(P2)[0])*((P1)[0]-(P2)[0]) + ((P1)[1]-(P2)[1])*((P1)[1]-(P2)[1]) + ((P1)[2]-(P2)[2])*((P1)[2]-(P2)[2]) )


#ifndef SIGN
#define SIGN(a)  (((a) > 0) ? 1 : -1)
#endif


void  curveTangent(Handle(Geom_Curve) GC, gp_Pnt GP, gp_Vec &T);

void  curveTangent(Handle(Geom_Curve) GC, gp_Pnt GP,
		     double &Tx, double &Ty, double &Tz
		     );

void  curvePointAndTangent(Handle(Geom_Curve) GC, gp_Pnt GP,
		     gp_Pnt &P, gp_Vec &T);

void  curvePointAndTangent(Handle(Geom_Curve) GC, gp_Pnt GP,
		     double &Px, double &Py, double &Pz,
		     double &Tx, double &Ty, double &Tz
		     );


void  supNormal(Handle(Geom_Surface) GS, 
		        double Px,  double Py,  double Pz,
		        double &Nx, double &Ny, double &Nz
			);


void  supPointAndNormal(Handle(Geom_Surface) GS, 
		        double Px,  double Py,  double Pz,
		        double &SPx, double &SPy, double &SPz,
		        double &Nx, double &Ny, double &Nz
			);

void  putPointOnSurf(Handle(Geom_Surface) GS, 
		        double Px,  double Py,  double Pz,
		        double &SPx, double &SPy, double &SPz
			);


typedef struct CompCurve{
    int linePort;
    int shared;
    int hasConstU;
    double cutoffref;
    std::vector<GEdge *> curves;
    CompCurve(){linePort=0; shared=0; hasConstU=0; cutoffref=1;}
} CompCurveType;


SPoint2 parFromVertex(MVertex *v, Handle(Geom_Surface) GS){
     SPoint2 UV;
     double U=0., V=0.;
     if(v->getParameter(0,U) && v->getParameter(1,V))
	  UV=SPoint2(U, V);
     else{
          gp_Pnt pnt(v->x(), v->y(), v->z());
          GeomAPI_ProjectPointOnSurf proj(pnt, GS);
          proj.LowerDistanceParameters(U,V);
          UV=SPoint2(U,V);
     }
     return UV;
}


int GfaceOCCfaceSign(GFace *gf){
//mesh orientation versus OCC face orientation
//gmsh model may reverse OCC face orientation
     TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
     Handle(Geom_Surface) GS =BRep_Tool::Surface(F);
     MTriangle *t = gf->triangles[0];
     MVertex *v=t->getVertex(0);
     double uv[2]={0.0,0.0};
     if(v->onWhat()->dim()==0){
	  GVertex *gv = (GVertex *)v->onWhat();
          TopoDS_Vertex V=* (TopoDS_Vertex *) gv->getNativePtr();
	  gp_Pnt2d aP2d = BRep_Tool::Parameters(V,F);
	  uv[0]=aP2d.X();
	  uv[1]=aP2d.Y();
     } else if(v->onWhat()->dim()==1){
	  GEdge *ge =(GEdge *)v->onWhat();
          TopoDS_Edge E=* (TopoDS_Edge *) ge->getNativePtr();
	  double u; v->getParameter(0,u);
	  Standard_Real first,last;
	  Handle(Geom2d_Curve) c2d=BRep_Tool::CurveOnSurface (E,F,first,last);
	  gp_Pnt2d aP2d = c2d->Value (u);
	  uv[0]=aP2d.X();
	  uv[1]=aP2d.Y();
     }
     else if(v->getParameter(0,uv[0]) && v->getParameter(1,uv[1])) ;
     else{
          gp_Pnt pnt(v->x(), v->y(), v->z());
          GeomAPI_ProjectPointOnSurf proj(pnt, GS);
          proj.LowerDistanceParameters(uv[0],uv[1]);
     }
     SPoint2 UV=SPoint2(uv[0],uv[1]);
     SVector3 n=gf->normal(UV);
     GeomLProp_SLProps props(GS,uv[0],uv[1],1,0.001);
     gp_Pnt P=props.Value();
     gp_Vec N=props.Normal();
     return SIGN(N.X()*n.x()+N.Y()*n.y()+N.Z()*n.z());
    }

int GmshOCCfaceSign(GFace *gf){
       TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
       Handle(Geom_Surface) GS = BRep_Tool::Surface(F);
       MTriangle *t = gf->triangles[0];
       MVertex *v=t->getVertex(0);
       SPoint2 uv=parFromVertex(v,GS);
       SVector3 n=gf->normal(uv);
       SVector3 A=SVector3(t->getVertex(0)->point(),t->getVertex(1)->point());
       SVector3 B=SVector3(t->getVertex(0)->point(),t->getVertex(2)->point());
       return SIGN(dot(n,crossprod(A,B)));
      }

int GedgeOCCedgeSign(GEdge *ge){
     TopoDS_Shape Esh=* (TopoDS_Shape *) ge->getNativePtr();
     Esh.Orientation(TopAbs_FORWARD);
     TopoDS_Edge E= TopoDS::Edge(Esh);
     TopoDS_Vertex V1,V2;
     bool  CumOri=false;
     TopExp::Vertices(E,V1,V2, CumOri);
     TopoDS_Shape V1sh=* (TopoDS_Shape *) ge->getBeginVertex()->getNativePtr();
     TopoDS_Shape V2sh=* (TopoDS_Shape *) ge->getEndVertex()->getNativePtr();
     TopoDS_Vertex V1_=TopoDS::Vertex(V1sh);
     TopoDS_Vertex V2_=TopoDS::Vertex(V2sh);
     return (V1.IsSame(V1_) && V2.IsSame(V2_)) ? 1 :-1;
}

namespace MWM {
  extern MWM::Mesh *mesh;
}

void print_mwm(GModel *gm,  MwOCAF* ocaf, bool meshIF, bool mesh3D, bool meshWG,
	      const char* dirName, const char* modelDir,
	      std::map< int, std::string > &solidNames,
	      std::vector<int> &Fmaster)
{

  FILE *fwg=NULL;
  if(meshWG){ //****************
   TCollection_AsciiString assName; ocaf->getAssName(assName);
   TCollection_AsciiString wgmodelFileName=TCollection_AsciiString(modelDir)+"/"+assName+"_WG.mwm";
   fwg=fopen(nativePath(wgmodelFileName.ToCString()).c_str(), "w"); if(!fwg) return;
   std::string WGprefix=(remove_SUB(assName)).ToCString()+std::string("__");
   ocaf->EmP->save(fwg, true, true, WGprefix);
  }
 
 
  FILE *fout=NULL;
  std::string WGprefix;
  if(mesh3D){ //****************
    TCollection_AsciiString assName; ocaf->getAssName(assName);
    TCollection_AsciiString modelFileName=TCollection_AsciiString(modelDir)+"/"+assName+".mwm";
    fout=fopen(nativePath(modelFileName.ToCString()).c_str(), "a"); if(!fout) return;
    WGprefix=(remove_SUB(assName)).ToCString()+std::string("__");
    ocaf->EmP->save(fout, true, false, WGprefix);


#if defined(GMSH3D)
    bool saveAll=true;
    int saveSinglePartition=0;
    bool renumber = true;

    std::vector<GEntity*> entities;
    gm->getEntities(entities);
    int numVertices = gm->indexMeshVertices(saveAll, saveSinglePartition, renumber);

//  fprintf(fout, "numberOfPoints  %d\n", numVertices);
   fprintf(fout, "points [\n");
   for(unsigned int i = 0; i < entities.size(); i++)
    for(unsigned int j = 0; j < entities[i]->mesh_vertices.size(); j++){
      MVertex *v=entities[i]->mesh_vertices[j];
//      fprintf(fout, "\t%d %.16g %.16g %.16g, \n", v->getIndex(), v->x(),v->y(), v->z());
      if(v->getIndex()>0) fprintf(fout, "\t%.16g %.16g %.16g,\n", v->x(),v->y(), v->z());
    }
   fprintf(fout, "]\n\n");
#endif

  }
//--- Curves:  
   for(GModel::eiter it = gm->firstEdge(); it != gm->lastEdge(); it++){
     GEdge *ge=(*it);
     unsigned int ElineNum=ge->getNumMeshElements();
     if(ElineNum==0) continue;
     TopoDS_Edge E=* (TopoDS_Edge *) ge->getNativePtr();
     int EI=ocaf->indexedEdges->FindIndex(E);
     if(EI<1) continue; 
     if(meshIF && ocaf->edgeData[EI-1].level <ocaf->EmP->level) continue;
     if(meshIF){
	if(ge->meshAttributes.method!=MESH_NONE){
	 std::string edgeFileName=dirName;
         char fname[50]; sprintf(fname,"E%d.mwm",EI);
         edgeFileName+="/interfaces/";
         edgeFileName+=fname;
	 fout=fopen(nativePath(edgeFileName).c_str(), "w");
	}
	if(!fout) continue;
     }
     if(fout) fprintf(fout, "DEF %s  MWM_Curve {\n", ocaf->edgeData[EI-1].name.c_str());
     if(fwg)  fprintf(fwg, "DEF %s  MWM_Curve {\n", ocaf->edgeData[EI-1].name.c_str());
//---------------------------
     if(fout) fprintf(fout, "  curveVertices [\n");
     if(fwg)  fprintf(fwg, "  curveVertices [\n");
      GVertex *gv1=ge->getBeginVertex();
      GVertex *gv2=ge->getEndVertex();
      TopoDS_Vertex V1= *(TopoDS_Vertex *) gv1->getNativePtr();
      int IP1=ocaf->indexedVertices->FindIndex(V1);
      TopoDS_Vertex V2= *(TopoDS_Vertex *) gv2->getNativePtr();
      int IP2=ocaf->indexedVertices->FindIndex(V2);
      int GMSH_Esign=ocaf->vertexData[IP1-1].name > ocaf->vertexData[IP2-1].name ? -1:1;
      if(GMSH_Esign<0) {int i=IP1; IP1=IP2; IP2=i;}
      fprintf(fout, "\t\"%s\"\n",  ocaf->vertexData[IP1-1].name.c_str());
      fprintf(fout, "\t\"%s\"\n",  ocaf->vertexData[IP2-1].name.c_str());
      if(mesh3D && fwg) {
        fprintf(fwg, "\t\"%s\"\n",  ocaf->vertexData[IP1-1].name.c_str());
        fprintf(fwg, "\t\"%s\"\n",  ocaf->vertexData[IP2-1].name.c_str());
      }
     if(fout) fprintf(fout, "  ]\n");
     if(fwg)  fprintf(fwg, "  ]\n");
//---------------------------
     if(fout) fprintf(fout, "  ECvertices [\n");
     if(fwg)  fprintf(fwg, "  ECvertices [\n");
      fprintf(fout, "\t%d\n", ocaf->ECvertex(IP1)); if(fwg) fprintf(fwg, "\t%d\n", ocaf->ECvertex(IP1));
      fprintf(fout, "\t%d\n", ocaf->ECvertex(IP2)); if(fwg) fprintf(fwg, "\t%d\n", ocaf->ECvertex(IP2));
     if(fout) fprintf(fout, "  ]\n");
     if(fwg)  fprintf(fwg, "  ]\n");
//---------------------------
    if(fout) fprintf(fout, "  PMCvertices [\n");
     if(fwg)  fprintf(fwg, "  PMvertices [\n");
      fprintf(fout, "\t%d\n", ocaf->PMCvertex(IP1)); if(fwg) fprintf(fwg, "\t%d\n", ocaf->PMCvertex(IP1));
      fprintf(fout, "\t%d\n", ocaf->PMCvertex(IP2)); if(fwg) fprintf(fwg, "\t%d\n", ocaf->PMCvertex(IP2));
     if(fout) fprintf(fout, "  ]\n");
     if(fwg)  fprintf(fwg, "  ]\n");
//---------------------------
    if(fout) fprintf(fout, "  SHAREDvertices [\n");
     if(fwg)  fprintf(fwg, "  SHAREDvertices [\n");
      fprintf(fout, "\t%d\n", ocaf->vertexData[IP1-1].shared); if(fwg) fprintf(fwg, "\t%d\n", ocaf->vertexData[IP1-1].shared);
      fprintf(fout, "\t%d\n", ocaf->vertexData[IP2-1].shared); if(fwg) fprintf(fwg, "\t%d\n", ocaf->vertexData[IP2-1].shared);
     if(fout) fprintf(fout, "  ]\n");
     if(fwg)  fprintf(fwg, "  ]\n");

//---------------------------
     BRepAdaptor_Curve C(E);
     double Elen = GCPnts_AbscissaPoint::Length (C);
     if(fout) fprintf(fout, "  length  %.16g\n", Elen);
     if(fwg)  fprintf(fwg,  "  length  %.16g\n", Elen);
//---------------------------
     MElement *l;
     MVertex *v;
#if defined(GMSH3D)
     if(fout && !meshIF) fprintf(fout, "  pointsI [\n");
     if(fwg)  fprintf(fwg, "  pointsI [\n");
      for(unsigned int i = 0; i <ElineNum;  ++i){
          l=ge->getMeshElement(GMSH_Esign>0 ? i:ElineNum-1-i);
          v = l->getVertex(GMSH_Esign>0 ? 0:1);
	  if(fout && !meshIF) fprintf(fout, "\t%d,\n", v->getIndex());
	  if(fwg) fprintf(fwg, "\t%d,\n", v->getIndex());
      }
      v = l->getVertex(GMSH_Esign>0 ? 1:0);
      if(fout && !meshIF) fprintf(fout, "\t%d,\n", v->getIndex());
      if(fwg ) fprintf(fwg, "\t%d,\n", v->getIndex());
      if(fout && !meshIF) fprintf(fout, "  ]\n");
      if(fwg ) fprintf(fwg, "  ]\n");
#endif
// ------------------------------------------
     if(fout) fprintf(fout, "  points [\n");
     if(fwg ) fprintf(fwg, "  points [\n");
     for(unsigned int i = 0; i <ElineNum;  ++i){
          l=ge->getMeshElement(GMSH_Esign>0 ? i:ElineNum-1-i);
          v = l->getVertex(GMSH_Esign>0 ? 0:1);
	  if(fout) fprintf(fout, "\t%.16g %.16g %.16g,\n", v->x(), v->y(), v->z());
	  if(fwg ) fprintf(fwg, "\t%.16g %.16g %.16g,\n", v->x(), v->y(), v->z());
     }
     v = l->getVertex(GMSH_Esign>0 ? 1:0);
     if(fout) fprintf(fout, "\t%.16g %.16g %.16g,\n", v->x(), v->y(), v->z());
     if(fwg ) fprintf(fwg, "\t%.16g %.16g %.16g,\n", v->x(), v->y(), v->z());
     if(fout) fprintf(fout, "  ]\n");
     if(fwg ) fprintf(fwg, "  ]\n");
// ------------------------------------------
/*
     if(fout) fprintf(fout, "  pointsU [\n");
     double u;
     for(unsigned int i = 0; i <ElineNum;  ++i){
          l=ge->getMeshElement(GMSH_Esign>0 ? i:ElineNum-1-i);
          v = l->getVertex(GMSH_Esign>0 ? 0:1);
	  v->getParameter(0,u);
	  if(fout) fprintf(fout, "\t%.16g,\n", u);
     }
     v = l->getVertex(GMSH_Esign>0 ? 1:0);
     v->getParameter(0,u);
     if(fout) fprintf(fout, "\t%.16g,\n", u);
     if(fout) fprintf(fout, "  ]\n");
*/
// ------------------------------------------
     if(fout) fprintf(fout, "  curveTangents [\n");
     if(fwg ) fprintf(fwg, "  curveTangents [\n");
      for(unsigned int i = 0; i <ElineNum;  ++i) {
        l=ge->getMeshElement(GMSH_Esign>0 ? i:ElineNum-1-i);
        SPoint3 P=l->getVertex(GMSH_Esign>0 ? 0:1)->point();
	double u=ge->parFromPoint(P);
	SVector3 T=ge->firstDer(u);
        T.normalize();
        if(GMSH_Esign<0) T*=-1;
        if(fout) fprintf(fout, "\t%.16f %.16f %.16f,\n", T.x(), T.y(), T.z());
        if(fwg ) fprintf(fwg, "\t%.16f %.16f %.16f,\n", T.x(), T.y(), T.z());
      }
      SPoint3 P=l->getVertex(GMSH_Esign>0 ? 1:0)->point();
      double u=ge->parFromPoint(P);
      SVector3 T=ge->firstDer(u);
      T.normalize();
      if(GMSH_Esign<0) T*=-1;
      if(fout) fprintf(fout, "\t%.16f %.16f %.16f,\n", T.x(), T.y(), T.z());
      if(fwg ) fprintf(fwg, "\t%.16f %.16f %.16f,\n", T.x(), T.y(), T.z());
     if(fout) fprintf(fout, "  ]\n");
     if(fwg ) fprintf(fwg, "  ]\n");
// ------------------------------------------
     if(fout) fprintf(fout, "}\n\n");
     if(fwg ) fprintf(fwg, "}\n\n");

//----------------------------------
     if(meshIF && fout){
	 fflush(fout);
         #ifndef WNT
           int fd = fileno(fout);
           fsync(fd);   
         #else   
           FlushFileBuffers(fout);   
         #endif   
	 fclose(fout);
	 fout=NULL;
	}
   }


  if(mesh3D){ //****************

//--- Line Ports:  
   std::map<std::string, CompCurveType > compCurves;
   for(GModel::eiter it = gm->firstEdge(); it != gm->lastEdge(); it++){
     GEdge *ge=(*it);
     unsigned int ElineNum=ge->getNumMeshElements();
     if(ElineNum==0) continue;
     int iv1 = ge->getMeshElement(0)->getVertex(0)->getIndex();
     int iv2 = ge->getMeshElement(ElineNum-1)->getVertex(1)->getIndex();
     if(iv1==iv2) if(ge->length()<1.e-8) continue;
     TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
     int EI=ocaf->indexedEdges->FindIndex(E);
     if(EI<1) continue;
     int linePort=ocaf->LPedge(EI);
     std::string name=linePort? WGprefix+ocaf->edgeData[EI-1].LPname : ocaf->edgeData[EI-1].name;
     if(name==std::string("-")) continue;
     compCurves[name].curves.push_back(ge);
     compCurves[name].linePort=linePort;
     if(ocaf->ECedge(EI)) continue;
     if(ocaf->PMCedge(EI)) continue;
     if(ocaf->edgeData[EI-1].scname==std::string("-")) continue;
     name=ocaf->edgeData[EI-1].scname;
     compCurves[name].curves.push_back(ge);
     compCurves[name].shared=1;
     compCurves[name].hasConstU=ocaf->edgeData[EI-1].hasConstU;
     compCurves[name].cutoffref=MAX(ocaf->edgeData[EI-1].cutoffref,compCurves[name].cutoffref);
   }
   typedef std::map<std::string, CompCurveType >::const_iterator CompCurveIt;
   for (CompCurveIt ccit=compCurves.begin(); ccit!= compCurves.end(); ccit++){
//sorting
     bool badCompCurve=false;
     double dist=0;
     int EN=(*ccit).second.curves.size();
     int *permu=new int[EN];
     int *ccEdgeSign=new int[EN];
     for (int ei=0; ei< EN; ei++) permu[ei]=0;
     std::map<int, int > vimult;
     for (int ei=0; ei< EN; ei++){
      GEdge *ge=(*ccit).second.curves[ei];
      unsigned int ElineNum=ge->getNumMeshElements();
      MVertex *v1 = ge->getMeshElement(0)->getVertex(0);
      MVertex *v2 = ge->getMeshElement(ElineNum-1)->getVertex(1);
      vimult[v1->getIndex()]++;
      vimult[v2->getIndex()]++;
     }
     int first=0,last=0,firstv=0,lastv=0,firstIP=0,lastIP=0;
     for (int ei=0; ei< EN; ei++){
      GEdge *ge=(*ccit).second.curves[ei];
      unsigned int ElineNum=ge->getNumMeshElements();
      GVertex *gv1=ge->getBeginVertex();
      GVertex *gv2=ge->getEndVertex();
      MVertex *v1 = gv1->getMeshElement(0)->getVertex(0);
      MVertex *v2 = gv2->getMeshElement(0)->getVertex(0);
      if(vimult[v1->getIndex()]==1) {
	   TopoDS_Vertex V= *(TopoDS_Vertex *) gv1->getNativePtr();
	   int IP=ocaf->indexedVertices->FindIndex(V);
	   if(!firstv){first=ei; firstv=v1->getIndex(); firstIP=IP;}
	   else       {last=ei; lastv=v1->getIndex(); lastIP=IP; break;}
      }
      if(vimult[v2->getIndex()]==1) {
	   TopoDS_Vertex V= *(TopoDS_Vertex *) gv2->getNativePtr();
	   int IP=ocaf->indexedVertices->FindIndex(V);
	   if(!firstv){first=ei; firstv=v2->getIndex(); firstIP=IP;;} 
	   else       {last=ei; lastv=v2->getIndex(); lastIP=IP; break;}
      }
     }
     if(firstIP>0 && lastIP>0) if(ocaf->vertexData[firstIP-1].name > ocaf->vertexData[lastIP-1].name){
	   int i;
	   i=first; first=last; last=i;
	   i=firstv; firstv=lastv; lastv=i;
	   i=firstIP; firstIP=lastIP; lastIP=i;
     }
     if(firstv==0) { //seem edge
      GEdge *ge=(*ccit).second.curves[0];
      MVertex *v1 = ge->getMeshElement(0)->getVertex(0);
      first=0; firstv=lastv=v1->getIndex();
     }
     {
      std::set<int> done;
      int last=0; permu[last]=first; 
      int lastv_=firstv;
      bool changed=true;
      while(changed){
       changed=false;
       for (int ei=0; ei< EN; ei++) if(done.find(ei)==done.end()){
        GEdge *ge=(*ccit).second.curves[ei];
        unsigned int ElineNum=ge->getNumMeshElements();
        MVertex *v1 = ge->getMeshElement(0)->getVertex(0);
        MVertex *v2 = ge->getMeshElement(ElineNum-1)->getVertex(1);
        GVertex *gv1=ge->getBeginVertex();
        GVertex *gv2=ge->getEndVertex();
        TopoDS_Vertex V1= *(TopoDS_Vertex *) gv1->getNativePtr();
	int IP1=ocaf->indexedVertices->FindIndex(V1);
        TopoDS_Vertex V2= *(TopoDS_Vertex *) gv2->getNativePtr();
	int IP2=ocaf->indexedVertices->FindIndex(V2);
	int GMSH_Esign=ocaf->vertexData[IP1-1].name > ocaf->vertexData[IP2-1].name ? -1 : 1;
        if     (v1->getIndex()==lastv_) {done.insert(ei); permu[last]=ei; ccEdgeSign[last++]=GMSH_Esign;  lastv_=v2->getIndex(); changed=true;}
	else if(v2->getIndex()==lastv_) {done.insert(ei); permu[last]=ei; ccEdgeSign[last++]=-GMSH_Esign; lastv_=v1->getIndex(); changed=true;}
       }
      }
      badCompCurve=(last!=EN);
      bool checkBadCompCurve=false;
      if(badCompCurve && checkBadCompCurve){
	  std::vector<int> vertices;
	  std::vector<int> curves;
          for (int ei=0; ei< EN; ei++) {
            GEdge *ge=(*ccit).second.curves[ei];
            TopoDS_Edge E=* (TopoDS_Edge *) ge->getNativePtr();
            int EI=ocaf->indexedEdges->FindIndex(E);
	    curves.push_back(EI);
            unsigned int ElineNum=ge->getNumMeshElements();
            MVertex *v1 = ge->getMeshElement(0)->getVertex(0);
            MVertex *v2 = ge->getMeshElement(ElineNum-1)->getVertex(1);
	    vertices.push_back(v1->getIndex());
	    vertices.push_back(v2->getIndex());
	  }
      }
     }
     //end sorting
     if(badCompCurve) continue;  //skip bad comp curve
     fprintf(fout, "DEF %s  MWM_CompCurve {\n",  (*ccit).first.c_str());
     if((*ccit).second.linePort) fprintf(fout, "  LinePort   %d\n", 1);
     if((*ccit).second.shared){
	  fprintf(fout, "  shared   %d\n", 1);
          fprintf(fout, "  cutoffRefinement  %f \n", (*ccit).second.cutoffref) ;
     }
     if((*ccit).second.hasConstU) fprintf(fout, "  addConstU\n");
     fprintf(fout, "  curves [\n");
     for (int ei_=0; ei_< EN; ei_++){
      int ei=permu[ei_];
      GEdge *ge=(*ccit).second.curves[ei];
      TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
      int EI=ocaf->indexedEdges->FindIndex(E);
      fprintf(fout, "\t\"%s\"\n", ocaf->edgeData[EI-1].name.c_str());
     }
     fprintf(fout, "  ]\n");
//----------------------------------
     fprintf(fout, "  curveSign [\n");
     for (int ei_=0; ei_< EN; ei_++) fprintf(fout, "\t%d,\n", ccEdgeSign[ei_]);
     fprintf(fout, "  ]\n");
     fprintf(fout, "  curveColor [\n");
     for (int ei_=0; ei_< EN; ei_++){
      fprintf(fout, "\t1,\n");
     }
     fprintf(fout, "  ]\n");
     fprintf(fout, "}\n\n");
     delete [] permu;
     delete [] ccEdgeSign;
   }  //CompCurve iteration

  }  //if mesh3D *****************
  

  if(!mesh3D && fout){
	fflush(fout);
        #ifndef WNT
          int fd = fileno(fout);
          fsync(fd);   
        #else   
          FlushFileBuffers(fout);   
        #endif   
	fclose(fout);
	fout=NULL;
  }

  for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
    GFace *gf=*fit;
    TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
    int OCC_Fsign=(F.Orientation()==TopAbs_FORWARD) ? 1 : -1;
    F.Orientation(TopAbs_FORWARD);
    int FI=ocaf->indexedFaces->FindIndex(F);
    if(!FI) continue;
    int internal=1;
    internal=FI>ocaf->extFaceNum;
    if(meshIF && ocaf->faceData[FI-1].level <ocaf->EmP->level) continue;
//    int TriSsign=GmshOCCfaceSign(gf);
//    assert(TriSsign==1);
    bool skipFaceMesh=REUSE_CIRCUITS;
    if(meshIF){
        skipFaceMesh=false;
	if(internal && gf->meshAttributes.method!=MESH_NONE){
	 std::string faceFileName=dirName;
         char fname[50]; sprintf(fname,"F%d.mwm",FI);
         faceFileName+="/interfaces/";
         faceFileName+=fname;
	 fout=fopen(nativePath(faceFileName).c_str(), "w");
	}
	if(!fout) continue;
    }
    int OCC_GMSH_Fsign=GfaceOCCfaceSign(gf);
    int GMSH_Fsign=OCC_GMSH_Fsign*OCC_Fsign;
    bool onWG=false;
    {
      if(mesh3D){
       std::string upFaceName="UF"+ocaf->faceData[FI-1].name;
       MWM::Surface *mwms=MWM::mesh->FindSurface(upFaceName.c_str());
       if(mwms){GMSH_Fsign=mwms->masterSign; OCC_Fsign=GMSH_Fsign*OCC_GMSH_Fsign;}
      }
      int reversed_vol=OCC_Fsign>0 ? 0 : 1;
      TCollection_AsciiString bdrname= (ocaf->faceData[FI-1].BrCond.size()>0) ? ocaf->faceData[FI-1].BrCond.begin()->c_str() : "-";
      TCollection_AsciiString vol1name;
      TCollection_AsciiString vol2name;
      if(mesh3D && ocaf->faceAdjParts){
        vol1name=ocaf->faceAdjParts[2*(FI-1)+reversed_vol];
        vol2name=ocaf->faceAdjParts[2*(FI-1)+1-reversed_vol];
      } else if(meshIF){
	vol1name=ocaf->faceData[FI-1].sfname.c_str();
	vol2name="-";
      }
      if(vol1name==TCollection_AsciiString("-") && vol2name==TCollection_AsciiString("-")) continue;
      DB::Volume *vol1=NULL;
      if(vol1name!=TCollection_AsciiString("-")) vol1=ocaf->EmP->FindVolume(vol1name.ToCString());
      if(vol1) if(vol1->disabled) vol1=NULL;
      DB::Volume *vol2=NULL;
      if(vol2name!=TCollection_AsciiString("-")) vol2=ocaf->EmP->FindVolume(vol2name.ToCString());
      if(vol2) if(vol2->disabled) vol2=NULL;
      if(mesh3D) if(ocaf->faceData[FI-1].cmp1!=ocaf->faceData[FI-1].cmp2
	 && ocaf->faceData[FI-1].cmp1!=std::string("-") && ocaf->faceData[FI-1].cmp2!=std::string("-")){
          if(!vol1) vol1name=TCollection_AsciiString("UF")+TCollection_AsciiString(ocaf->faceData[FI-1].name.c_str());
          if(!vol2) vol2name=TCollection_AsciiString("UF")+TCollection_AsciiString(ocaf->faceData[FI-1].name.c_str());
      }
      if(REUSE_CIRCUITS) if(abs(Fmaster[FI-1])==FI)  skipFaceMesh=false;
      if(skipFaceMesh && vol1){
	    char m1name[100]; sprintf(m1name,"Vol%d",vol1->master);
	    if(!vol1->master || !strcmp(vol1->name,m1name)) skipFaceMesh=false;
      }
      if(skipFaceMesh && vol2){
	    char m2name[100]; sprintf(m2name,"Vol%d",vol2->master);
	    if(!vol2->master || !strcmp(vol2->name,m2name)) skipFaceMesh=false;
      }
      if(  bdrname==TCollection_AsciiString("-")
        &&(vol1name==TCollection_AsciiString("-") || vol2name==TCollection_AsciiString("-"))
      ) bdrname=TCollection_AsciiString(ocaf->EmP->defaultBC);
      bool hasbdrc =(bdrname!=TCollection_AsciiString("-"))
	         &&(bdrname!=TCollection_AsciiString("WAVEGUIDE"));
      if(hasbdrc && (bdrname!=TCollection_AsciiString("PEC")) && (bdrname!=TCollection_AsciiString("PML")) ) skipFaceMesh=false;
      onWG =bdrname==TCollection_AsciiString("WAVEGUIDE");
      if(onWG || ocaf->faceData[FI-1].cmp1!=ocaf->faceData[FI-1].cmp2) skipFaceMesh=false;
      if(mesh3D)
        fprintf(fout, "DEF %s  MWM_Surface {\n", ocaf->faceData[FI-1].name.c_str());
      else
        fprintf(fout, "DEF UF%s  MWM_Surface {\n", ocaf->faceData[FI-1].name.c_str());
      if(REUSE_CIRCUITS){
        if(Fmaster[FI-1]<0)
         fprintf(fout, "  volumes  [\"%s\", \"%s\"]\n",  vol2name.ToCString(), vol1name.ToCString());
        else
         fprintf(fout, "  volumes  [\"%s\", \"%s\"]\n",  vol1name.ToCString(), vol2name.ToCString());
      } else
         fprintf(fout, "  volumes  [\"%s\", \"%s\"]\n",  vol1name.ToCString(), vol2name.ToCString());
      if(meshIF)
	 fprintf(fout, "  cutoffRefinement  %f \n", ocaf->faceData[FI-1].cutoffref);
      if(mesh3D){
        if(REUSE_CIRCUITS) if(Fmaster[FI-1]) fprintf(fout, "  master  %d\n", abs(Fmaster[FI-1]) );
        if(fwg && onWG){ fprintf(fwg, "DEF %s  MWM_Surface {\n", ocaf->faceData[FI-1].name.c_str());
                       fprintf(fwg, "  volumes  [\"%s\", \"%s\"]\n",  vol1name.ToCString(), vol2name.ToCString()); 
                       if(REUSE_CIRCUITS) if(Fmaster[FI-1]) fprintf(fwg, "  master  %d\n", abs(Fmaster[FI-1]) );
        }
        if(hasbdrc)              fprintf(fout, "  material \"%s\"\n",  bdrname.ToCString());
        if(ocaf->ECface(FI))	fprintf(fout, "  conductor %d\n",  ocaf->faceConductorMap[FI-1]);
      }
    }

    std::map<int, int> PI2FPI;
    std::map<int, int> TI2FPI;
    std::vector<int>  FPI2PI;
    std::vector<double> points;
    std::vector<double> normals;
//    std::vector<double> pointsUV;

    std::map<int,GEdge*>  indexedGEdges;
    std::vector<GEdge*>     Fedges_=gf->edges();
    for (std::vector<GEdge*>::const_iterator eit=Fedges_.begin(); eit!=Fedges_.end(); ++eit){
     GEdge *ge=*eit;
     assert(GedgeOCCedgeSign(ge)>0);
     TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
     E.Orientation(TopAbs_FORWARD);
     int EI=ocaf->indexedEdges->FindIndex(E);
     indexedGEdges[EI]=ge;
    }
/*
    std::list<int>        FedgesOrientation_=gf->edgeOrientations();
    std::list<int>::iterator esgnit=FedgesOrientation_.begin();
*/
    std::vector<GEdge*>   Fedges;
    std::vector<int>      OCCF_Esign;
    for(TopoDS_Iterator it1(F,TopAbs_WIRE); it1.More(); it1.Next()){
     TopoDS_Shape W=it1.Value();
     int OCC_Wsign=1;
     for(TopoDS_Iterator it2(W,TopAbs_EDGE); it2.More(); it2.Next()){
      TopoDS_Shape E=it2.Value();
      int Esign=E.Orientation()==TopAbs_FORWARD? OCC_Fsign*OCC_Wsign : -OCC_Fsign*OCC_Wsign;
      E.Orientation(TopAbs_FORWARD);
      int EI=ocaf->indexedEdges->FindIndex(E);
      GEdge *ge=indexedGEdges[EI];
      if(!ge) continue;
//      Esign*=GedgeOCCedgeSign(ge);
      GVertex *gv1=ge->getBeginVertex();
      GVertex *gv2=ge->getEndVertex();
      TopoDS_Vertex V1= *(TopoDS_Vertex *) gv1->getNativePtr();
      int IP1=ocaf->indexedVertices->FindIndex(V1);
      TopoDS_Vertex V2= *(TopoDS_Vertex *) gv2->getNativePtr();
      int IP2=ocaf->indexedVertices->FindIndex(V2);
      if(ocaf->vertexData[IP1-1].name>ocaf->vertexData[IP2-1].name) Esign*=-1;
      
      unsigned int ElineNum=ge->getNumMeshElements();
      if(ElineNum==0) continue;
      int iv1 = ge->getMeshElement(0)->getVertex(0)->getIndex();
      int iv2 = ge->getMeshElement(ElineNum-1)->getVertex(1)->getIndex();
      if(iv1==iv2) if(ge->length()<1.e-8) continue;

      Fedges.push_back(ge);
      OCCF_Esign.push_back(Esign);
     }
    }

//Fedges,OCCF_Esign vectors are reversed because edgeloop iterators
//seems to proceed in reverse order
    {
     std::reverse(Fedges.begin(),Fedges.end());
     std::reverse(OCCF_Esign.begin(),OCCF_Esign.end());
    }


    int FPNum=0;
    int FtriaNum=gf->triangles.size();

    if(REUSE_CIRCUITS) if(mesh3D) if(Fmaster[FI-1]<0) OCC_Fsign=1;

    if(!skipFaceMesh) {

    Handle(Geom_Surface) GS = BRep_Tool::Surface(F);
    std::map<int, std::pair<double, double> > faceContPar; 
    for(int k = 0; k <3; k++) for(int TI = 0; TI <FtriaNum; ++TI){
      MTriangle *t = gf->triangles[TI];
      SPoint3 CP(0.0,0.0,0.0);
      for(int j = 0; j < 3; ++j){
         MVertex *v=GMSH_Fsign<0 ? t->getVertex(3-j-1) : t->getVertex(j);
         int I = v->getIndex();
         if(PI2FPI.find(I)==PI2FPI.end()) if (v->onWhat()->dim()==k){
	    PI2FPI[I]=FPNum++;
	    FPI2PI.push_back(I);
	    points.push_back(v->x());
	    points.push_back(v->y());
	    points.push_back(v->z());
	    double uv[2]={0.0,0.0};
	    if(v->onWhat()->dim()==0){
	       GVertex *gv = (GVertex *)v->onWhat();
               TopoDS_Vertex V=* (TopoDS_Vertex *) gv->getNativePtr();
	       gp_Pnt2d aP2d = BRep_Tool::Parameters(V,F);
	       uv[0]=aP2d.X();
	       uv[1]=aP2d.Y();
	    } else if(v->onWhat()->dim()==1){
		GEdge *ge =(GEdge *)v->onWhat();
                TopoDS_Edge E=* (TopoDS_Edge *) ge->getNativePtr();
		double u; v->getParameter(0,u);
		Standard_Real first,last;
		Handle(Geom2d_Curve) c2d=BRep_Tool::CurveOnSurface (E,F,first,last);
		gp_Pnt2d aP2d = c2d->Value (u);
		uv[0]=aP2d.X();
		uv[1]=aP2d.Y();
	      }
	      else if(v->getParameter(0,uv[0]) && v->getParameter(1,uv[1])) ;
	      else if(faceContPar.find(I)==faceContPar.end()){
                  gp_Pnt pnt(v->x(), v->y(), v->z());
                  GeomAPI_ProjectPointOnSurf proj(pnt, GS);
                  proj.LowerDistanceParameters(uv[0],uv[1]);
//		  uv=gf->parFromPoint(v->point());
                  std::pair<double, double> p=std::make_pair(uv[0],uv[1]);
                  faceContPar[I]=p;
	      } else {
		   std::pair<double, double> p=faceContPar[I];
		   uv[0]=p.first; uv[1]=p.second;
	      }
              SPoint2 UV=SPoint2(uv[0],uv[1]);
/*	      
	      pointsUV.push_back(uv[0]);
	      pointsUV.push_back(uv[1]);
*/
              if(gf->geomType() != GEntity::Plane){
                 SVector3 n=GMSH_Fsign*gf->normal(UV);
	         normals.push_back(n.x());
	         normals.push_back(n.y());
	         normals.push_back(n.z());
/*
	     GeomLProp_SLProps props(GS,uv[0],uv[1],1,0.001);
	     gp_Pnt P=props.Value();
	     gp_Vec N=props.Normal();
	     gf_F_sign=SIGN(N.X()*n.x()+N.Y()*n.y()+N.Z()*n.z());
	     normals.push_back(N.X());
	     normals.push_back(N.Y());
	     normals.push_back(N.Z());
*/

	      }
        }
      }
    }
    for(int TI = 0; TI <FtriaNum; ++TI){
      MTriangle *t = gf->triangles[TI];
      SPoint2 CUV(0.0,0.0);
      SPoint3 cpnt(0,0,0);
      bool hasuv=true;
      double el_mean=0;
      for(int j = 0; j < 3; ++j){
         MVertex *v = t->getVertex(j);
	 SPoint3 vpnt=v->point();
         MVertex *v2 = t->getVertex((j+1)%3);
         el_mean+=vpnt.distance(v2->point());
         cpnt+=vpnt;
	 double uv[2]={0.0,0.0};
	 hasuv=hasuv && v->getParameter(0,uv[0]) && v->getParameter(1,uv[1]);
	 if(hasuv) CUV+=SPoint2(uv[0], uv[1]);
      }
      cpnt*=1.0/3;
      el_mean*=1.0/3;
      GPoint CP;
      bool goodpoint=false;
      if(hasuv){
	 CUV*=1.0/3;
	 CP=gf->point(CUV);
	 SPoint3 pnt(CP.x(), CP.y(), CP.z());
	 goodpoint=pnt.distance(cpnt)<el_mean;
      }
      if(!goodpoint){
        gp_Pnt P(cpnt.x(), cpnt.y(), cpnt.z());
        GeomAPI_ProjectPointOnSurf proj(P, GS);
	double u0=0., u1=0.;
        proj.LowerDistanceParameters(u0,u1);
        CUV=SPoint2(u0,u1);
	CP=gf->point(CUV);
      }
      TI2FPI[TI]=FPNum++;
      FPI2PI.push_back(0);
      points.push_back(CP.x());
      points.push_back(CP.y());
      points.push_back(CP.z());
/*
      pointsUV.push_back(CUV[0]);
      pointsUV.push_back(CUV[1]);
*/
      if(gf->geomType() != GEntity::Plane){
       SVector3 n=GMSH_Fsign*gf->normal(CUV);
       normals.push_back(n.x());
       normals.push_back(n.y());
       normals.push_back(n.z());
/*
       GeomLProp_SLProps props(GS,CUV.x(),CUV.y(),1,0.001);
       gp_Pnt P=props.Value();
       gp_Vec N=props.Normal();
       normals.push_back(N.X());
       normals.push_back(N.Y());
       normals.push_back(N.Z());
*/
       
      }
   }

  } //!skipFaceMesh 
//----------------------------------
  if(fout) fprintf(fout, "  curves [\n");
  if(fwg && onWG) fprintf(fwg, "  curves [\n");
  if(fout) for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
    int EI=ocaf->indexedEdges->FindIndex(E);
    fprintf(fout, "\t\"%s\"\n",  ocaf->edgeData[EI-1].name.c_str());
    if(fwg && onWG) fprintf(fwg, "\t\"%s\"\n", ocaf->edgeData[EI-1].name.c_str());
  }
  if(fout) fprintf(fout, "  ]\n");  
  if(fwg && onWG) fprintf(fwg, "  ]\n");
//----------------------------------
  if(fout) fprintf(fout, "  curveSign [\n");
  if(fwg && onWG) fprintf(fwg, "  curveSign [\n");
  for (int i=0; i<Fedges.size(); i++){
    if(fout) fprintf(fout, "\t%d,\n", OCCF_Esign[i]);
    if(fwg && onWG) fprintf(fwg, "\t%d,\n", OCCF_Esign[i]);
  }
  if(fout) fprintf(fout, "  ]\n");
  if(fwg && onWG) fprintf(fwg, "  ]\n");
  if(!mesh3D){
//----------------------------------
   if(fout) fprintf(fout, "  curveVertices [\n");
   if(fout) for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    GVertex *gv1=ge->getBeginVertex();
    GVertex *gv2=ge->getEndVertex();
    TopoDS_Vertex V1= *(TopoDS_Vertex *) gv1->getNativePtr();
    int IP1=ocaf->indexedVertices->FindIndex(V1);
    TopoDS_Vertex V2= *(TopoDS_Vertex *) gv2->getNativePtr();
    int IP2=ocaf->indexedVertices->FindIndex(V2);
    int Esign=ocaf->vertexData[IP1-1].name > ocaf->vertexData[IP2-1].name ? -1 : 1;
    if(Esign>0){
     fprintf(fout, "\t\"%s\"\n",  ocaf->vertexData[IP1-1].name.c_str());
     fprintf(fout, "\t\"%s\"\n",  ocaf->vertexData[IP2-1].name.c_str());
    }else{
     fprintf(fout, "\t\"%s\"\n",  ocaf->vertexData[IP2-1].name.c_str());
     fprintf(fout, "\t\"%s\"\n",  ocaf->vertexData[IP1-1].name.c_str());
    }
   }
   if(fout) fprintf(fout, "  ]\n");
//----------------------------------
  if(fout) fprintf(fout, "  curveLength [\n");
  if(fout) for (int i=0; i<Fedges.size(); i++){
     GEdge *ge=Fedges[i];
     TopoDS_Edge E=* (TopoDS_Edge *) ge->getNativePtr();
     BRepAdaptor_Curve C(E);
     double Elen = GCPnts_AbscissaPoint::Length (C);
     fprintf(fout, "\t%.16g,\n", Elen); if(fwg) fprintf(fwg,  "\t%.16g,\n", Elen);
   }
   if(fout) fprintf(fout, "  ]\n");
//----------------------------------
  }
  if(fout) fprintf(fout, "  PECcurves [\n");
  if(fwg && onWG) fprintf(fwg, "  PECcurves [\n");
  for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
    int EI=ocaf->indexedEdges->FindIndex(E);
    int PECedge=ocaf->PECedge(EI);
    if(fout) fprintf(fout, "\t%d,\n", PECedge);
    if(fwg && onWG) fprintf(fwg, "\t%d,\n", PECedge);
  }
  if(fout) fprintf(fout, "  ]\n");
  if(fwg && onWG) fprintf(fwg, "  ]\n");
//----------------------------------
  if(fout) fprintf(fout, "  ECcurves [\n");
  if(fwg && onWG) fprintf(fwg, "  ECcurves [\n");
  for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
    int EI=ocaf->indexedEdges->FindIndex(E);
    int ECedge=mesh3D? ocaf->edgeConductorMap[EI-1]: ocaf->ECedge(EI);
    if(fout) fprintf(fout, "\t%d,\n", ECedge);
    if(fwg && onWG) fprintf(fwg, "\t%d,\n", ECedge);
  }
  if(fout) fprintf(fout, "  ]\n");
  if(fwg && onWG) fprintf(fwg, "  ]\n");
//----------------------------------
  if(fout) fprintf(fout, "  PMCcurves [\n");
  if(fwg && onWG)  fprintf(fwg, "  PMCcurves [\n");
  for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
    int EI=ocaf->indexedEdges->FindIndex(E);
    int PMCedge=ocaf->PMCedge(EI);
    if(fout) fprintf(fout, "\t%d,\n", PMCedge);
    if(fwg && onWG) fprintf(fwg, "\t%d,\n", PMCedge);
  }
  if(fout) fprintf(fout, "  ]\n");
  if(fwg && onWG) fprintf(fwg, "  ]\n");
//----------------------------------
  if(!mesh3D){
   if(fout) fprintf(fout, "  LPcurves [\n");
   if(fout) for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
    int EI=ocaf->indexedEdges->FindIndex(E);
    int LPedge=ocaf->LPedge(EI);
    fprintf(fout, "\t%d,\n", LPedge);
   }
   if(fout) fprintf(fout, "  ]\n");
  }

   if(!skipFaceMesh) {
#if defined(GMSH3D)
//----------------------------------
   if(mesh3D){
    fprintf(fout, "  pointsI [\n");
    for(int i = 0; i< FPNum; i++) fprintf(fout, "\t%d,\n", FPI2PI[i] );
    fprintf(fout, "  ]\n");
    if(fwg && onWG){
       fprintf(fwg, "  pointsI [\n");
       for(int i = 0; i< FPNum; i++) fprintf(fwg, "\t%d,\n", FPI2PI[i] );
       fprintf(fwg, "  ]\n");
    }
   }
#endif
//----------------------------------
   if(fout){
     fprintf(fout, "  points [\n");
     if(fout) for(int i = 0; i< FPNum; i++){
	fprintf(fout, "\t%.16g %.16g %.16g,\n",
	                  points[3*i +0], 
	                  points[3*i +1], 
	                  points[3*i +2]
        );
     }
     fprintf(fout, "  ]\n");
   }
   if(fwg && onWG){
     fprintf(fwg, "  points [\n");
     for(int i = 0; i< FPNum; i++){
	fprintf(fwg, "\t%.16g %.16g %.16g,\n",
	                  points[3*i +0], 
	                  points[3*i +1], 
	                  points[3*i +2]
        );
     }
     fprintf(fwg, "  ]\n");
   }
/*
   if(fout && meshIF){
     fprintf(fout, "  pointsUV [\n");
     for(int i = 0; i< FPNum; i++){
	fprintf(fout, "\t%.16g %.16g,\n",
	                  pointsUV[2*i +0], 
	                  pointsUV[2*i +1]
        );
     }
     fprintf(fout, "  ]\n");
   }
*/
//----------------------------------
   if(fout && gf->geomType() != GEntity::Plane){
       fprintf(fout, "  normals [\n");
       for(int i=0; i<FPNum; i++) 
         fprintf(fout, "\t%.16f %.16f %.16f,\n",
	         normals[3*i +0], 
	         normals[3*i +1], 
	         normals[3*i +2]
         );
       fprintf(fout, "  ]\n");
   }
   if(fwg && onWG && gf->geomType() != GEntity::Plane){
       fprintf(fwg, "  normals [\n");
       for(int i=0; i<FPNum; i++)
	 fprintf(fwg, "\t%.16f %.16f %.16f,\n",
	         normals[3*i +0], 
	         normals[3*i +1], 
	         normals[3*i +2]
         );
       fprintf(fwg, "  ]\n");
   }
//----------------------------------
 if(fout) fprintf(fout, "  curvePoints [\n");
 if(fwg && onWG) fprintf(fwg, "  curvePoints [\n");
 for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    GVertex *gv1=ge->getBeginVertex();
    GVertex *gv2=ge->getEndVertex();
    TopoDS_Vertex V1= *(TopoDS_Vertex *) gv1->getNativePtr();
    int IP1=ocaf->indexedVertices->FindIndex(V1);
    TopoDS_Vertex V2= *(TopoDS_Vertex *) gv2->getNativePtr();
    int IP2=ocaf->indexedVertices->FindIndex(V2);
    int Esign=ocaf->vertexData[IP1-1].name > ocaf->vertexData[IP2-1].name ? -1: 1;
    unsigned int ElineNum=ge->getNumMeshElements();
    MElement *l;
    for(unsigned int i = 0; i <ElineNum;  ++i){
      l=ge->getMeshElement(Esign>0 ? i: ElineNum-1-i);
      if(fout) fprintf(fout, "\t%d,\n", PI2FPI[l->getVertex(Esign>0 ? 0:1)->getIndex()]);
      if(fwg && onWG) fprintf(fwg, "\t%d,\n", PI2FPI[l->getVertex(Esign>0 ? 0:1)->getIndex()]);
    }
    if(fout) fprintf(fout, "\t%d,\n", PI2FPI[l->getVertex(Esign>0 ? 1:0)->getIndex()]);
    if(fwg && onWG) fprintf(fwg, "\t%d,\n", PI2FPI[l->getVertex(Esign>0 ? 1:0)->getIndex()]);
    if(fout) fprintf(fout, "\t-1,\n");
    if(fwg && onWG) fprintf(fwg, "\t-1,\n");
 }
 if(fout) fprintf(fout, "  ]\n");
 if(fwg && onWG) fprintf(fwg, "  ]\n");
//----------------------------------
 if(!mesh3D){
   if(fout) fprintf(fout, "  curveTangents [\n");
   for (int i=0; i<Fedges.size(); i++){
     GEdge *ge=Fedges[i];
     GVertex *gv1=ge->getBeginVertex();
     GVertex *gv2=ge->getEndVertex();
     TopoDS_Vertex V1= *(TopoDS_Vertex *) gv1->getNativePtr();
     int IP1=ocaf->indexedVertices->FindIndex(V1);
     TopoDS_Vertex V2= *(TopoDS_Vertex *) gv2->getNativePtr();
     int IP2=ocaf->indexedVertices->FindIndex(V2);
     int Esign=ocaf->vertexData[IP1-1].name > ocaf->vertexData[IP2-1].name ? -1: 1;
     TopoDS_Edge E=* (TopoDS_Edge *) ge->getNativePtr();
     int EI=ocaf->indexedEdges->FindIndex(E);
     unsigned int ElineNum=ge->getNumMeshElements();
     MElement *l;
     for(unsigned int i = 0; i <ElineNum;  ++i) {
        l=ge->getMeshElement(Esign>0 ? i:ElineNum-1-i);
        SPoint3 P=l->getVertex(Esign>0 ? 0:1)->point();
	double u=ge->parFromPoint(P);
	SVector3 T=ge->firstDer(u);
        T.normalize();
        if(Esign<0) T*=-1;
        if(fout) fprintf(fout, "\t%.16f %.16f %.16f,\n", T.x(), T.y(), T.z());
        if(fwg ) fprintf(fwg, "\t%.16f %.16f %.16f,\n", T.x(), T.y(), T.z());
     }
     SPoint3 P=l->getVertex(Esign>0 ? 1:0)->point();
     double u=ge->parFromPoint(P);
     SVector3 T=ge->firstDer(u);
     T.normalize();
     if(Esign<0) T*=-1;
     if(fout) fprintf(fout, "\t%.16f %.16f %.16f,\n", T.x(), T.y(), T.z());
     if(fout) fprintf(fout, "\t0 0 0,\n");
   }
   if(fout) fprintf(fout, "  ]\n");
 }
//----------------------------------
  if(fout){
    fprintf(fout, "  triangles [\n");
    for(int TI = 0; TI <FtriaNum;  ++TI){
       MTriangle *t = gf->triangles[TI];
       fprintf(fout, "\t");
       for(int j = 0; j < 3; ++j){ 
          MVertex *v = GMSH_Fsign<0 ? t->getVertex(3-j-1) : t->getVertex(j);
          fprintf(fout, "%d,", PI2FPI[v->getIndex()]);
       }
       fprintf(fout, "-1,\n");
    }
    fprintf(fout, "  ]\n");
  }
  if(fwg && onWG){
    fprintf(fwg, "  triangles [\n");
    for(int TI = 0; TI <FtriaNum;  ++TI){
       MTriangle *t = gf->triangles[TI];
       fprintf(fwg, "\t");
       for(int j = 0; j < 3; ++j){
          MVertex *v = GMSH_Fsign<0 ? t->getVertex(3-j-1) : t->getVertex(j);
	  fprintf(fwg, "%d,", PI2FPI[v->getIndex()]);
       }
       fprintf(fwg, "-1,\n");
    }
    fprintf(fwg, "  ]\n");
  }
//----------------------------------
   if(fout){
     fprintf(fout, "  triaCenters [\n");
     for(int TI = 0; TI <FtriaNum;  ++TI) fprintf(fout, "\t%d,\n", TI2FPI[TI]);
     fprintf(fout, "  ]\n");
   }
   if(fwg && onWG){
     fprintf(fwg, "  triaCenters [\n");
     for(int TI = 0; TI <FtriaNum;  ++TI) fprintf(fwg, "\t%d,\n", TI2FPI[TI]);
     fprintf(fwg, "  ]\n");
   }

   } //!skipFaceMesh
//----------------------------------
    if(fout) fprintf(fout, "}\n\n"); 
    if(fwg && onWG) fprintf(fwg, "}\n\n");
    if(fout && !mesh3D){
	 fflush(fout);
         #ifndef WNT
           int fd = fileno(fout);
           fsync(fd);   
         #else   
           FlushFileBuffers(fout);   
         #endif   
	 fclose(fout);
	 fout=NULL;
	}
  }
#if defined(GMSH3D)
  if(mesh3D) {
     int tetNumber=0;
     for(GModel::riter it = gm->firstRegion(); it != gm->lastRegion(); ++it)  {
       GRegion *gr=*it;
       TopoDS_Solid V=* (TopoDS_Solid *) gr->getNativePtr();
       int VI=ocaf->indexedSolids->FindIndex(V);
       assert(VI);
       assert((*it)->tetrahedra.size()>0);
       fprintf(fout, "\ntetsI OF %s  [\n", solidNames[VI].c_str());
       for(unsigned int i = 0; i < (*it)->tetrahedra.size(); i++){
	 MElement *t=(*it)->tetrahedra[i];
         fprintf(fout, "\t%d,\n", t->getNum());
       }
       fprintf(fout, "]\n");
       fprintf(fout, "\ntetrahedras OF %s [\n", solidNames[VI].c_str());
       for(unsigned int i = 0; i < (*it)->tetrahedra.size(); i++){
	 MElement *t=(*it)->tetrahedra[i];
         std::vector<int> verts; 
	 t->getVerticesIdForMSH(verts);
         for(int j = 0; j < t->getNumVerticesForMSH(); j++) fprintf(fout, " %d", verts[j]);
         fprintf(fout, " -1\n");
       }
       fprintf(fout, "]\n");
       tetNumber+=(*it)->tetrahedra.size();
     }
     fflush(fout);
     #ifndef WNT
        int fd = fileno(fout);
        fsync(fd);   
     #else   
        FlushFileBuffers(fout);   
     #endif   
     fclose(fout);
     if(fwg) fclose(fwg);
     TCollection_AsciiString assName; ocaf->getAssName(assName);
     TCollection_AsciiString tetnFileName=TCollection_AsciiString(modelDir)+"/"+assName+".tetn";
     FILE *ftetN=fopen(nativePath(tetnFileName.ToCString()).c_str(), "w"); 
     if(ftetN){
        fprintf(ftetN, "%d\n", tetNumber);
	fclose(ftetN);
     }
  }
#endif

}



void print_mwm3D(GModel *gm,  MwOCAF* ocaf,
	      const char* dirName, const char* modelDir,
	      std::map< int, std::string > &solidNames
	      )
{
  FILE *fout;
  FILE *fwg=NULL;
   TCollection_AsciiString assName; ocaf->getAssName(assName);
   TCollection_AsciiString modelFileName=TCollection_AsciiString(modelDir)+"/"+assName+".mwm";
   fout=fopen(nativePath(modelFileName.ToCString()).c_str(), "a"); if(!fout) return;
   std::string WGprefix=assName.ToCString()+std::string("__");
   ocaf->EmP->save(fout, true, false, WGprefix);
   TCollection_AsciiString wgmodelFileName=TCollection_AsciiString(modelDir)+"/"+assName+"_WG.mwm";
   fwg=fopen(nativePath(wgmodelFileName.ToCString()).c_str(), "w"); if(!fwg) return;
   ocaf->EmP->save(fwg, true, true, WGprefix);

   bool saveAll=true;
   int saveSinglePartition=0;
   bool renumber = true;

   std::vector<GEntity*> entities;
   gm->getEntities(entities);
   int numVertices = gm->indexMeshVertices(saveAll, saveSinglePartition, renumber);

//  fprintf(fout, "numberOfPoints  %d\n", numVertices);
  fprintf(fout, "points [\n");
  for(unsigned int i = 0; i < entities.size(); i++)
    for(unsigned int j = 0; j < entities[i]->mesh_vertices.size(); j++){
      MVertex *v=entities[i]->mesh_vertices[j];
//      fprintf(fout, "\t%d %.16f %.16f %.16f, \n", v->getIndex(), v->x(),v->y(), v->z());
      fprintf(fout, "\t%.16f %.16f %.16f,\n", v->x(),v->y(), v->z());
    }
  fprintf(fout, "]\n\n");


//--- Line Ports:  
  std::map<std::string, CompCurveType > compCurves;
  for(GModel::eiter it = gm->firstEdge(); it != gm->lastEdge(); it++){
     GEdge *ge=(*it);
     unsigned int ElineNum=ge->getNumMeshElements();
     int iv1 = ge->getMeshElement(0)->getVertex(0)->getIndex();
     int iv2 = ge->getMeshElement(ElineNum-1)->getVertex(1)->getIndex();
     if(iv1==iv2) if(ge->length()<1.e-8) continue;
     TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
     int EI=ocaf->indexedEdges->FindIndex(E);
     int linePort=ocaf->LPedge(EI);
     std::string name=linePort? WGprefix+ocaf->edgeData[EI-1].LPname : ocaf->edgeData[EI-1].name;
     if(name==std::string("-")) continue;
     compCurves[name].curves.push_back(ge);
     compCurves[name].linePort=linePort;
  }
  typedef std::map<std::string, CompCurveType >::const_iterator CompCurveIt;
  for (CompCurveIt ccit=compCurves.begin(); ccit!= compCurves.end(); ccit++){
//sorting
    double dist=0;
    int EN=(*ccit).second.curves.size();
    int *permu=new int[EN];
    int *esign=new int[EN];
    for (int ei=0; ei< EN; ei++) permu[ei]=0;
    int ei_,lastv,lastesign;
    for (int ei=0; ei< EN; ei++){
     GEdge *ge=(*ccit).second.curves[ei];
     TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
     int EI=ocaf->indexedEdges->FindIndex(E);
     unsigned int ElineNum=ge->getNumMeshElements();
     MVertex *v1 = ge->getMeshElement(0)->getVertex(0);
     MVertex *v2 = ge->getMeshElement(ElineNum-1)->getVertex(1);
     double P1[3]; P1[0]=v1->x(); P1[1]=v1->y(); P1[2]=v1->z();
     double P2[3]; P2[0]=v2->x(); P2[1]=v2->y(); P2[2]=v2->z();
     double d=DISTANCE(ocaf->edgeData[EI-1].P1,P1);
     if(ei==0 || dist>d) {dist=d; ei_=ei; lastesign=1; lastv=v2->getIndex();};
     d=DISTANCE(ocaf->edgeData[EI-1].P1,P2);
     if(dist>d) {dist=d; ei_=ei; lastesign=-1; lastv=v1->getIndex();};
    }
    std::set<int> done;
    int last=0;
    permu[last]=ei_; esign[last]=lastesign; done.insert(ei_);
    bool changed=true;
    while(changed){
      changed=false;
      for (int ei=0; ei< EN; ei++) if(done.find(ei)==done.end()){
        GEdge *ge=(*ccit).second.curves[ei];
        unsigned int ElineNum=ge->getNumMeshElements();
        MVertex *v1 = ge->getMeshElement(0)->getVertex(0);
        MVertex *v2 = ge->getMeshElement(ElineNum-1)->getVertex(1);
        if     (v1->getIndex()==lastv) {last++; done.insert(ei); permu[last]=ei; esign[last]=1;  lastv=v2->getIndex(); changed=true;}
	else if(v2->getIndex()==lastv) {last++; done.insert(ei); permu[last]=ei; esign[last]=-1; lastv=v1->getIndex(); changed=true;}
      }
    }
//end sorting
    fprintf(fout, "DEF %s  MWM_CompCurve {\n",  (*ccit).first.c_str());
    if((*ccit).second.linePort) fprintf(fout, "  LinePort   %d\n", 1);
    fprintf(fout, "  curves [\n");
    for (int ei_=0; ei_< EN; ei_++){
     int ei=permu[ei_];
     GEdge *ge=(*ccit).second.curves[ei];
     TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
     int EI=ocaf->indexedEdges->FindIndex(E);
     fprintf(fout, "\t\"C%d\"\n", EI);
    }
    fprintf(fout, "  ]\n");
//----------------------------------
    fprintf(fout, "  curveVertices [\n");
    for (int ei_=0; ei_< EN; ei_++){
     int ei=permu[ei_];
     GEdge *ge=(*ccit).second.curves[ei];
     TopoDS_Shape V1sh=* (TopoDS_Shape *) ge->getBeginVertex()->getNativePtr();
     TopoDS_Shape V2sh=* (TopoDS_Shape *) ge->getEndVertex()->getNativePtr();
     TopoDS_Vertex V1=TopoDS::Vertex(V1sh);
     TopoDS_Vertex V2=TopoDS::Vertex(V2sh);
     int VI1=ocaf->indexedVertices->FindIndex(V1);
     int VI2=ocaf->indexedVertices->FindIndex(V2);
     fprintf(fout, "\t\"%s\"\n",  ocaf->vertexData[VI1-1].name.c_str());
     fprintf(fout, "\t\"%s\"\n",  ocaf->vertexData[VI2-1].name.c_str());
    }
    fprintf(fout, "  ]\n");
    
    fprintf(fout, "}\n\n");
    delete [] permu;
    delete [] esign;

  }  //port iteration

//----------------------------------
  for(GModel::eiter it = gm->firstEdge(); it != gm->lastEdge(); ++it){
    GEdge *ge=(*it);      
    TopoDS_Edge E=* (TopoDS_Edge *) ge->getNativePtr();
    int EI=ocaf->indexedEdges->FindIndex(E);
    double u1,u2;
    Handle(Geom_Curve) gec=BRep_Tool::Curve(E, u1, u2);
    char Cname[200];
    bool onWG=false;
    sprintf(Cname, "C%d", EI);
    fprintf(fout, "DEF %s  MWM_Curve {\n", Cname);
    if(fwg && onWG) fprintf(fwg, "  DEF %s  MWM_Curve {\n", Cname);

    if(ocaf->PECedge(EI)){
     fprintf(fout, "\tIsOnPEC\n");
     if(fwg && onWG) fprintf(fout, "\tIsOnPEC\n");
    }
    if(ocaf->PMCedge(EI)){
     fprintf(fout, "\tIsOnPMC\n");
     if(fwg && onWG) fprintf(fout, "\tIsOnPMC\n");
    }
    if(ocaf->ECedge(EI)){
     fprintf(fout, "\tIsOnEC\n");
     if(fwg && onWG) fprintf(fout, "\tIsOnEC\n");
    }
    if(ocaf->LPedge(EI)){
     fprintf(fout, "\tIsOnLP\n");
     if(fwg && onWG) fprintf(fout, "\tIsOnLP\n");
    }
//----------------------------------
   TopoDS_Shape V1sh=* (TopoDS_Shape *) ge->getBeginVertex()->getNativePtr();
   TopoDS_Shape V2sh=* (TopoDS_Shape *) ge->getEndVertex()->getNativePtr();
   TopoDS_Vertex V1=TopoDS::Vertex(V1sh);
   TopoDS_Vertex V2=TopoDS::Vertex(V2sh);
   int VI1=ocaf->indexedVertices->FindIndex(V1);
   int VI2=ocaf->indexedVertices->FindIndex(V2);
   fprintf(fout, "  vertices   \"P%d\"  \"P%d\"\n", VI1, VI2); 
   if(fwg && onWG) fprintf(fwg, "  vertices   \"P%d\"  \"P%d\"\n", VI1, VI2);
//----------------------------------
    fprintf(fout, "  pointsI [\n");
    if(fwg && onWG) fprintf(fwg, "  pointsI [\n");
    unsigned int ElineNum=ge->getNumMeshElements();
    MElement *l;
    for(unsigned int i = 0; i <ElineNum;  ++i){
      l=ge->getMeshElement(i);
      for(int j=0; j<3; j+=2){
       fprintf(fout, "\t%d,\n", l->getVertex(j)->getIndex());
       if(fwg && onWG) fprintf(fwg, "\t%d,\n", l->getVertex(j)->getIndex());
      }
    }
    fprintf(fout, "\t%d\n", l->getVertex(1)->getIndex());
    if(fwg && onWG) fprintf(fwg, "\t%d\n", l->getVertex(1)->getIndex());
    fprintf(fout, "  ]\n");
    if(fwg && onWG) fprintf(fwg, "  ]\n");
//----------------------------------
    fprintf(fout, "  tangents [\n");
    if(fwg && onWG) fprintf(fwg, "  tangents [\n");
    int CEsign;
    {//checks edges orientation versus curve orientation
     SPoint3 P1=ge->getMeshElement(0)->getVertex(0)->point();
     gp_Pnt GP1(P1.x(),P1.y(),P1.z());
     SPoint3 P2=ge->getMeshElement(0)->getVertex(1)->point();
     gp_Pnt GP2(P2.x(),P2.y(),P2.z());
     gp_Pnt CP1; gp_Vec T1;
     curvePointAndTangent(gec, GP1, CP1, T1);
     gp_Pnt CP2; gp_Vec T2;
     curvePointAndTangent(gec, GP2, CP2, T2);
     gp_Vec T=gp_Vec(GP1,GP2);
     CEsign=SIGN(T*(T1+T2));
     assert(CEsign>0);
/*
     double p1=ge->parFromPoint(P1);
     GPoint P1_ =ge->point(p1);
     SVector3 T1=ge->firstDer(p1); T1.normalize();
     SPoint3 P2=ge->getMeshElement(0)->getVertex(1)->point();
     double p2=ge->parFromPoint(P2);
     GPoint P2_ =ge->point(p2);
     SVector3 T2=ge->firstDer(p2); T2.normalize();
     SVector3 T=SVector3(P1,P2);
     CEsign=SIGN(dot(T,T1+T2));
     assert(CEsign>0);
*/
    }
    gp_Vec T;
    for(unsigned int i = 0; i <ElineNum;  ++i) {
      l=ge->getMeshElement(i);
      for(int j=0; j<3; j+=2){
       SPoint3 P=l->getVertex(j)->point();
       gp_Pnt GP(P.x(),P.y(),P.z());
       curveTangent(gec, GP, T);
       T.Normalize();
       fprintf(fout, "\t%.16f %.16f %.16f,\n", T.X(), T.Y(), T.Z());
       if(fwg && onWG) fprintf(fwg, "\t%.16f %.16f %.16f,\n", T.X(), T.Y(), T.Z());
      }
    }
    SPoint3 P=l->getVertex(1)->point();
    gp_Pnt GP(P.x(),P.y(),P.z());
    curveTangent(gec, GP, T);
    T.Normalize();
    fprintf(fout, "\t%.16f %.16f %.16f\n", T.X(), T.Y(), T.Z());
    if(fwg && onWG) fprintf(fwg, "\t%.16f %.16f %.16f\n", T.X(), T.Y(), T.Z());
    fprintf(fout, "  ]\n");
    if(fwg && onWG) fprintf(fwg, "  ]\n");
    fprintf(fout, "}\n\n");
    if(fwg && onWG) fprintf(fwg, "}\n\n");
 }



  for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
    GFace *gf=*fit;
    if(gf->meshAttributes.method==MESH_NONE) continue;
    TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
    assert(F.Orientation()==TopAbs_FORWARD);
    int FI=ocaf->indexedFaces->FindIndex(F);
    if(!FI) continue;
    int TriSsign=GmshOCCfaceSign(gf);
    int Ssign=GfaceOCCfaceSign(gf);
    assert(TriSsign==Ssign);
    assert(Ssign>0);
    bool skipFaceMesh=false;
    bool onWG=false;
    if(ocaf->faceAdjParts) {
      int rev=Ssign>0 ? 0 : 1;
      TCollection_AsciiString bdrname= (ocaf->faceData[FI-1].BrCond.size()>0) ? ocaf->faceData[FI-1].BrCond.begin()->c_str() : "-";
      TCollection_AsciiString vol1name=ocaf->faceAdjParts[2*(FI-1)+rev];
      TCollection_AsciiString vol2name=ocaf->faceAdjParts[2*(FI-1)+1-rev];
      if(vol1name==TCollection_AsciiString("-") && vol2name==TCollection_AsciiString("-")) continue;
      DB::Volume *vol1=NULL;
      if(vol1name!=TCollection_AsciiString("-")) vol1=ocaf->EmP->FindVolume(vol1name.ToCString());
      if(vol1) if(vol1->disabled) vol1=NULL;
//      if(vol1) if(vol1->type==DIELECTRIC) vol1name=TCollection_AsciiString("-");
      DB::Volume *vol2=NULL;
      if(vol2name!=TCollection_AsciiString("-")) vol2=ocaf->EmP->FindVolume(vol2name.ToCString());
      if(vol2) if(vol2->disabled) vol2=NULL;
//      if(vol2) if(vol2->type==DIELECTRIC) vol2name=TCollection_AsciiString("-");
      if(ocaf->faceData[FI-1].cmp1!=ocaf->faceData[FI-1].cmp1){
       if(!vol1) vol1name=TCollection_AsciiString("UF")+TCollection_AsciiString(ocaf->faceData[FI-1].name.c_str());
       if(!vol2) vol2name=TCollection_AsciiString("UF")+TCollection_AsciiString(ocaf->faceData[FI-1].name.c_str());
      }
      if(  bdrname==TCollection_AsciiString("-")
        &&(vol1name==TCollection_AsciiString("-") || vol2name==TCollection_AsciiString("-"))
      ) bdrname=TCollection_AsciiString(ocaf->EmP->defaultBC);
      bool hasbdrc =(bdrname!=TCollection_AsciiString("-"))
	         &&(bdrname!=TCollection_AsciiString("WAVEGUIDE"));
      if(hasbdrc && (bdrname!=TCollection_AsciiString("PEC")) && (bdrname!=TCollection_AsciiString("PML")) ) skipFaceMesh=false;
      onWG =bdrname==TCollection_AsciiString("WAVEGUIDE");
      if(onWG || ocaf->faceData[FI-1].cmp1!=ocaf->faceData[FI-1].cmp1) skipFaceMesh=false;
      fprintf(fout, "DEF S%d  MWM_Surface {\n", FI);
      fprintf(fout, "  volumes  [\"%s\", \"%s\"]\n",  vol1name.ToCString(), vol2name.ToCString());
      if(fwg && onWG){ fprintf(fwg, "DEF S%d  MWM_Surface {\n", FI);
                       fprintf(fwg, "  volumes  [\"%s\", \"%s\"]\n",  vol1name.ToCString(), vol2name.ToCString()); 
      }
      if(hasbdrc) fprintf(fout, "  material \"%s\"\n",  bdrname.ToCString());
    }

    int reversedFace=0;  // reversedFace is meant to take into account a reversed master face

    std::map<int, int> PI2FPI;
    std::vector<int>  FPI2PI;
    std::vector<double> normals;

    std::map<int,GEdge*>  indexedGEdges;
    std::vector<GEdge*>     Fedges_=gf->edges();
    for (std::vector<GEdge*>::const_iterator eit=Fedges_.begin(); eit!=Fedges_.end(); ++eit){
     GEdge *ge=*eit;
     assert(GedgeOCCedgeSign(ge)>0);
     TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
     E.Orientation(TopAbs_FORWARD);
     int EI=ocaf->indexedEdges->FindIndex(E);
     indexedGEdges[EI]=ge;
    }
/*
    std::list<int>        FedgesOrientation_=gf->edgeOrientations();
    std::list<int>::iterator esgnit=FedgesOrientation_.begin();
*/
    std::vector<GEdge*>   Fedges;
    std::vector<int>      FedgesSign;
    for(TopoDS_Iterator it1(F.Oriented(TopAbs_FORWARD),TopAbs_WIRE); it1.More(); it1.Next()){
    TopoDS_Shape W=it1.Value();
//    int Wsgn=(W.Orientation()==TopAbs_FORWARD) ? 1 : -1;    
    for(TopoDS_Iterator it2(W,TopAbs_EDGE); it2.More(); it2.Next()){
      TopoDS_Shape E=it2.Value();
      int Esgn=(E.Orientation()==TopAbs_FORWARD) ? 1 : -1;
      if(reversedFace) Esgn=-Esgn;
      E.Orientation(TopAbs_FORWARD);
      int EI=ocaf->indexedEdges->FindIndex(E);
      GEdge *ge=indexedGEdges[EI];
      if(ge) {
       unsigned int ElineNum=ge->getNumMeshElements();
       int iv1 = ge->getMeshElement(0)->getVertex(0)->getIndex();
       int iv2 = ge->getMeshElement(ElineNum-1)->getVertex(1)->getIndex();
       if(iv1==iv2) if(ge->length()<1.e-8) continue;
       Fedges.push_back(ge);
       FedgesSign.push_back(Esgn);
      }
    }
    }

//Fedges,FedgesSign vectors are reversed because edgeloop iterators
//seems to proceed in reverse order
    {
     std::reverse(Fedges.begin(),Fedges.end());
     std::reverse(FedgesSign.begin(),FedgesSign.end());
    }


    int FPNum=0;
    int FtriaNum=gf->triangles.size();


    if(!skipFaceMesh) {

     for(int k = 0; k <2; k++) for(int TI = 0; TI <FtriaNum; ++TI){
      MTriangle *t = gf->triangles[TI];
      SPoint3 CP(0.0,0.0,0.0);
      for(int l = 0; l < 2; ++l) for(int j = 0; j < 3; ++j) {
        MVertex *v= reversedFace? t->getVertex(l*3+(3-j)%3) : t->getVertex(l*3+j);
        int I = v->getIndex();
        if(PI2FPI.find(I)==PI2FPI.end())
        if (v->onWhat()->dim() < 2 || k==1){
	   PI2FPI[I]=FPNum++;
	   FPI2PI.push_back(I);
           if(gf->geomType() != GEntity::Plane){
            SVector3 n=gf->normal(gf->parFromPoint(v->point()));
            if(reversedFace) n=-n;
	    normals.push_back(n.x());
	    normals.push_back(n.y());
	    normals.push_back(n.z());
           }
        }
      }
     }

  } //!skipFaceMesh 
//----------------------------------
  fprintf(fout, "  curves [\n");
  if(fwg && onWG) fprintf(fwg, "  curves [\n");
  for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
    int EI=ocaf->indexedEdges->FindIndex(E);
    fprintf(fout, "\t\"C%d\"\n", EI); if(fwg && onWG) fprintf(fwg, "\t\"C%d\"\n", EI);
    
  }
  fprintf(fout, "  ]\n");  
  if(fwg && onWG) fprintf(fwg, "  ]\n");
//----------------------------------
  fprintf(fout, "  curveSign [\n");
  if(fwg && onWG) fprintf(fwg, "  curveSign [\n");
  for (int i=0; i<Fedges.size(); i++){
    fprintf(fout, "\t%d,\n", FedgesSign[i]*Ssign);
    if(fwg && onWG) fprintf(fwg, "\t%d,\n", FedgesSign[i]*Ssign);
  }
  fprintf(fout, "  ]\n");
  if(fwg && onWG) fprintf(fwg, "  ]\n");


   if(!skipFaceMesh) {
//----------------------------------
     fprintf(fout, "  pointsI [\n");
     for(int i = 0; i< FPNum; i++) fprintf(fout, "\t%d,\n", FPI2PI[i] );
     fprintf(fout, "  ]\n");
     if(fwg && onWG){
       fprintf(fwg, "  pointsI [\n");
       for(int i = 0; i< FPNum; i++) fprintf(fwg, "\t%d,\n", FPI2PI[i] );
       fprintf(fwg, "  ]\n");
     }
//----------------------------------
     if(gf->geomType() != GEntity::Plane){
      fprintf(fout, "  normals [\n");
      for(int i=0; i<FPNum; i++) 
         fprintf(fout, "\t%.16f %.16f %.16f,\n",
	         normals[3*i +0], 
	         normals[3*i +1], 
	         normals[3*i +2]
         );
      fprintf(fout, "  ]\n");
     }
     if(fwg && onWG && gf->geomType() != GEntity::Plane){
      fprintf(fwg, "  normals [\n");
      for(int i=0; i<FPNum; i++)
	 fprintf(fwg, "\t%.16f %.16f %.16f,\n",
	         normals[3*i +0], 
	         normals[3*i +1], 
	         normals[3*i +2]
         );
      fprintf(fwg, "  ]\n");
     }
//----------------------------------
    fprintf(fout, "  triangles [\n");
    for(int TI = 0; TI <FtriaNum;  ++TI){
      MTriangle *t = gf->triangles[TI];
      fprintf(fout, "\t");
      for(int j = 0; j < 3; ++j){ 
       MVertex *v = reversedFace ? t->getVertex((3-j)%3) : t->getVertex(j);
       fprintf(fout, "%d,", PI2FPI[v->getIndex()]);
      }
     fprintf(fout, "-1,\n");
  }
  fprintf(fout, "  ]\n");
  if(fwg && onWG){
    fprintf(fwg, "  triangles [\n");
    for(int TI = 0; TI <FtriaNum;  ++TI){
       MTriangle *t = gf->triangles[TI];
       fprintf(fwg, "\t");
       for(int j = 0; j < 3; ++j){
          MVertex *v = reversedFace ? t->getVertex((3-j)%3) : t->getVertex(j);
	  fprintf(fwg, "%d,", PI2FPI[v->getIndex()]);
       }
       fprintf(fwg, "-1,\n");
    }
    fprintf(fwg, "  ]\n");
  }
//----------------------------------
    fprintf(fout, "  triaSUP [\n");
    for(int TI = 0; TI <FtriaNum;  ++TI){
      MTriangle *t = gf->triangles[TI];
      fprintf(fout, "\t");
      for(int j = 0; j < 3; ++j){ 
       MVertex *v = reversedFace ? t->getVertex(3+(3-j)%3) : t->getVertex(3+j);
       fprintf(fout, "%d,", PI2FPI[v->getIndex()]);
      }
     fprintf(fout, "-1,\n");
  }
  fprintf(fout, "  ]\n");
  if(fwg && onWG){
    fprintf(fwg, "  triaSUP [\n");
    for(int TI = 0; TI <FtriaNum;  ++TI){
       MTriangle *t = gf->triangles[TI];
       fprintf(fwg, "\t");
       for(int j = 0; j < 3; ++j){
          MVertex *v = reversedFace ? t->getVertex(3+(3-j)%3) : t->getVertex(3+j);
	  fprintf(fwg, "%d,", PI2FPI[v->getIndex()]);
       }
       fprintf(fwg, "-1,\n");
    }
    fprintf(fwg, "  ]\n");
  }
//----------------------------------
   fprintf(fout, "  triaCenters [\n");
   for(int TI = 0; TI <FtriaNum;  ++TI) fprintf(fout, "\t%d,\n", FPNum-FtriaNum+TI );
   fprintf(fout, "  ]\n");
   if(fwg && onWG){
     fprintf(fwg, "  triaCenters [\n");
     for(int TI = 0; TI <FtriaNum;  ++TI) fprintf(fwg, "\t%d,\n", FPNum-FtriaNum+TI);
     fprintf(fwg, "  ]\n");
   }
//----------------------------------

  } //!skipFaceMesh 


   fprintf(fout, "}\n\n");
   if(fwg && onWG) fprintf(fwg, "}\n\n");
  
  }  

   int num=0;
   for(GModel::riter it = gm->firstRegion(); it != gm->lastRegion(); ++it){
     GRegion *gr=*it;
     TopoDS_Solid V=* (TopoDS_Solid *) gr->getNativePtr();
     int VI=ocaf->indexedSolids->FindIndex(V);
     assert(VI);
     fprintf(fout, "tetrahedras2 OF %s [\n", solidNames[VI].c_str());
     for(unsigned int i = 0; i < (*it)->tetrahedra.size(); i++){
	 MElement *t=(*it)->tetrahedra[i];
         std::vector<int> verts; 
	 t->getVerticesIdForMSH(verts);
         for(int j = 0; j < t->getNumVerticesForMSH(); j++) fprintf(fout, " %d", verts[j]);
         fprintf(fout, "\n");
     }
     fprintf(fout, "]\n");
   }


   fflush(fout);
   #ifndef WNT
     int fd = fileno(fout);
     fsync(fd);   
   #else   
     FlushFileBuffers(fout);   
   #endif   
   fclose(fout);
   if(fwg) fclose(fwg);

}


