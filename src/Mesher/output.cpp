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

#include "GmshConfig.h"
#include "Gmsh.h"
#include "GEntity.h"
#include "GModel.h"
#include "GFace.h"
#include "GEdge.h"
#include "MTriangle.h"
#include "MElement.h"

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

#include "assert.h"
#include <map>
#include <vector>
#include <list>
#include <algorithm>

#include "ocaf.h"
//#include "InputOutput.h"
#include "OStools.h"
#include "DataBase.h"

#ifndef PIG
#define PIG 3.14159265
#endif

#ifndef SIGN
#define SIGN(a)  (((a) > 0) ? 1 : -1)
#endif

#define DISTANCE(P1, P2) \
  sqrt( ((P1)[0]-(P2)[0])*((P1)[0]-(P2)[0]) + ((P1)[1]-(P2)[1])*((P1)[1]-(P2)[1]) + ((P1)[2]-(P2)[2])*((P1)[2]-(P2)[2]) )



#ifndef SIGN
#define SIGN(a)  (((a) > 0) ? 1 : -1)
#endif


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
    std::vector<GEdge *> curves;
    CompCurve(){linePort=0;}
} CompCurveType;


int GfaceOCCfaceSign(GFace *gf){
//mesh orientation versus OCC face orientation
//gmsh model may reverse OCC face orientation
     TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
     Handle(Geom_Surface) GS = BRep_Tool::Surface(F);
     MTriangle *t = gf->triangles[0];
     SPoint3 P=t->getVertex(0)->point();
     SVector3 n=gf->normal(gf->parFromPoint(P));
     double Nx, Ny, Nz;
     supNormal(GS, P.x(), P.y(), P.z(), Nx, Ny, Nz);
     SVector3 Fn=SVector3(Nx, Ny, Nz);
     return SIGN(dot(n,Fn));
    }

int GmshOCCfaceSign(GFace *gf){
       MTriangle *t = gf->triangles[0];
       SVector3 n=gf->normal(gf->parFromPoint(t->getVertex(0)->point()));
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

void print_mwm(GModel *gm,  MwOCAF* ocaf, bool isPartition,
	      const char* dirName, const char* modelDir,
	      TopTools_IndexedMapOfShape *indexedFaces,
	      TopTools_IndexedMapOfShape *indexedEdges,
	      TopTools_IndexedMapOfShape *indexedVertices)
{

  FILE *fout;
  FILE *fwg=NULL;
  if(isPartition){
  TCollection_AsciiString assName; ocaf->getAssName(assName);
  TCollection_AsciiString modelFileName=TCollection_AsciiString(modelDir)+"/"+assName+".mwm";
  fout=fopen(nativePath(modelFileName.ToCString()).c_str(), "a"); if(!fout) return;
  std::string WGprefix=assName.ToCString()+std::string("__");
  ocaf->EmP.save(fout, true, false, WGprefix);
  TCollection_AsciiString wgmodelFileName=TCollection_AsciiString(modelDir)+"/"+assName+"_WG.mwm";
  fwg=fopen(nativePath(wgmodelFileName.ToCString()).c_str(), "w"); if(!fwg) return;
  ocaf->EmP.save(fwg, true, true, WGprefix);
//--- Line Ports:  
  std::map<std::string, CompCurveType > compCurves;
  for(GModel::eiter it = gm->firstEdge(); it != gm->lastEdge(); it++){
     GEdge *ge=(*it);
     unsigned int ElineNum=ge->getNumMeshElements();
     int iv1 = ge->getMeshElement(0)->getVertex(0)->getIndex();
     int iv2 = ge->getMeshElement(ElineNum-1)->getVertex(1)->getIndex();
     if(iv1==iv2) if(ge->length()<1.e-8) continue;
     TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
     int EI=indexedEdges->FindIndex(E);
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
     int EI=indexedEdges->FindIndex(E);
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
     int EI=indexedEdges->FindIndex(E);
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
     int VI1=indexedVertices->FindIndex(V1);
     int VI2=indexedVertices->FindIndex(V2);
     if(isPartition) {
       fprintf(fout, "\t\"P%d\"\n", VI1);
       fprintf(fout, "\t\"P%d\"\n", VI2);
     }else{
       fprintf(fout, "\t\"%s\"\n",  ocaf->vertexData[VI1-1].name.c_str());
       fprintf(fout, "\t\"%s\"\n",  ocaf->vertexData[VI2-1].name.c_str());
     }
    }
    fprintf(fout, "  ]\n");
//----------------------------------
    fprintf(fout, "  points [\n");
    for (int ei_=0; ei_< EN; ei_++){
     int ei=permu[ei_];
     GEdge *ge=(*ccit).second.curves[ei];
     unsigned int ElineNum=ge->getNumMeshElements();
     MElement *l;
     int sgn=esign[ei_];
     int inv= sgn>0 ? 0:1;
     for(unsigned int i = 0; i <ElineNum;  ++i){
          l=ge->getMeshElement(inv*(ElineNum-1)+sgn*i);
          MVertex *v = l->getVertex(inv);
	  fprintf(fout, "\t%.16g %.16g %.16g,\n", v->x(), v->y(), v->z());
     }
     if(ei_==EN-1){
      MVertex *v = l->getVertex(1-inv);
      fprintf(fout, "\t%.16g %.16g %.16g,\n", v->x(), v->y(), v->z());
     }
    }
     fprintf(fout, "  ]\n");
     fprintf(fout, "  curvePoints [\n");
    int i0=0;
    for (int ei_=0; ei_< EN; ei_++){
     int ei=permu[ei_];
     GEdge *ge=(*ccit).second.curves[ei];
     unsigned int ElineNum=ge->getNumMeshElements();
     int sgn=esign[ei_];
     int inv=sgn>0 ? 0:1;
     for(unsigned int i = 0; i <ElineNum+1;  ++i) fprintf(fout, "\t%d,\n", i0+inv*ElineNum+sgn*i);
     fprintf(fout, "\t-1,\n");
     i0+=ElineNum;
    }
    fprintf(fout, "  ]\n");
    fprintf(fout, "  curveTangents [\n");
    for (int ei_=0; ei_< EN; ei_++){
     int ei=permu[ei_];
     GEdge *ge=(*ccit).second.curves[ei];
     unsigned int ElineNum=ge->getNumMeshElements();
     int CEsign;
     {//checks edges orientation versus curve orientation
       SPoint3 P1=ge->getMeshElement(0)->getVertex(0)->point();
       SVector3 T1=ge->firstDer(ge->parFromPoint(P1)); T1.normalize();
       SPoint3 P2=ge->getMeshElement(0)->getVertex(1)->point();
       SVector3 T2=ge->firstDer(ge->parFromPoint(P2)); T2.normalize();
       SVector3 T=SVector3(P1,P2);
       CEsign=SIGN(dot(T,T1+T2));
       assert(CEsign>0);
      }
      SVector3 T;
      MElement *l;
      for(unsigned int i = 0; i <ElineNum;  ++i){
        l=ge->getMeshElement(i);
        T=ge->firstDer(ge->parFromPoint(l->getVertex(0)->point()));
        T.normalize();
        fprintf(fout, "\t%.16f %.16f %.16f,\n", T.x(), T.y(), T.z());
      }
      T=ge->firstDer(ge->parFromPoint(l->getVertex(1)->point()));
      T.normalize();
      fprintf(fout, "\t%.16f %.16f %.16f,\n", T.x(), T.y(), T.z());
      fprintf(fout, "\t0 0 0,\n");
    }
    fprintf(fout, "  ]\n");
    fprintf(fout, "  curveColor [\n");
    for (int ei_=0; ei_< EN; ei_++){
      fprintf(fout, "\t1,\n");
    }
    fprintf(fout, "  ]\n");
    fprintf(fout, "}\n\n");
    delete [] permu;
    delete [] esign;
  }  //port iteration
  }  //if partition
  
  for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
    GFace *gf=*fit;
    if(gf->meshAttributes.method==MESH_NONE) continue;
    TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
    assert(F.Orientation()==TopAbs_FORWARD);
    int FI=indexedFaces->FindIndex(F);
    if(!FI) continue;
    if(!isPartition && ocaf->faceData[FI-1].shared && ocaf->faceData[FI-1].level !=ocaf->EmP.level) continue;
    int Ssign=GmshOCCfaceSign(gf);
//    int Ssign=GfaceOCCfaceSign(gf);
    assert(Ssign>0);
    if(!isPartition){
	std::string faceFileName=dirName;
        char fname[50]; sprintf(fname,"F%d.mwm",FI);
        faceFileName+="/interfaces/";
        faceFileName+=fname;
	fout=fopen(nativePath(faceFileName).c_str(), "w"); if(!fout) continue;
    }
    bool onWG=false;
    if(isPartition && ocaf->faceAdjParts) {
      int rev=Ssign>0 ? 0 : 1;
      TCollection_AsciiString bdrname=ocaf->faceAdjBdrCond[FI-1];
      TCollection_AsciiString vol1name=ocaf->faceAdjParts[2*(FI-1)+rev];
      TCollection_AsciiString vol2name=ocaf->faceAdjParts[2*(FI-1)+1-rev];
      if(vol1name==TCollection_AsciiString("-") && vol2name==TCollection_AsciiString("-")) continue;
      if(ocaf->faceData[FI-1].shared){
       DB::Volume *vol1=NULL;
       if(vol1name!=TCollection_AsciiString("-")) vol1=ocaf->EmP.FindVolume(vol1name.ToCString());
       if(vol1) if(vol1->disabled) vol1=NULL;
       if(!vol1) vol1name=TCollection_AsciiString("UF")+TCollection_AsciiString(ocaf->faceData[FI-1].name.c_str());
       DB::Volume *vol2=NULL;
       if(vol2name!=TCollection_AsciiString("-")) vol2=ocaf->EmP.FindVolume(vol2name.ToCString());
       if(vol2) if(vol2->disabled) vol2=NULL;
       if(!vol2) vol2name=TCollection_AsciiString("UF")+TCollection_AsciiString(ocaf->faceData[FI-1].name.c_str());
      }
      if(  bdrname==TCollection_AsciiString("-")
        &&(vol1name==TCollection_AsciiString("-") || vol2name==TCollection_AsciiString("-"))
      ) bdrname=TCollection_AsciiString(ocaf->EmP.defaultBC);
      bool hasbdrc =(bdrname!=TCollection_AsciiString("-"))
	         &&(bdrname!=TCollection_AsciiString("WAVEGUIDE"))
	         &&(bdrname!=TCollection_AsciiString("INTERFACE"));
      onWG =bdrname==TCollection_AsciiString("WAVEGUIDE");
      fprintf(fout, "DEF S%d  MWM_Surface {\n", FI);
      fprintf(fout, "  volumes  [\"%s\", \"%s\"]\n",  vol1name.ToCString(), vol2name.ToCString());
      if(fwg && onWG){ fprintf(fwg, "DEF S%d  MWM_Surface {\n", FI);
                       fprintf(fwg, "  volumes  [\"%s\", \"%s\"]\n",  vol1name.ToCString(), vol2name.ToCString()); }
      if(hasbdrc) fprintf(fout, "  material \"%s\"\n",  bdrname.ToCString());
    }

    std::map<int, int> PI2FPI;
    std::map<int, int> TI2FPI;
    std::vector<double> points;
    std::vector<double> normals;

    std::map<int,GEdge*>  indexedGEdges;
    std::list<GEdge*>     Fedges_=gf->edges();
    for (std::list<GEdge*>::const_iterator eit=Fedges_.begin(); eit!=Fedges_.end(); ++eit){
     GEdge *ge=*eit;
     assert(GedgeOCCedgeSign(ge)>0);
     TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
     E.Orientation(TopAbs_FORWARD);
     int EI=indexedEdges->FindIndex(E);
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
    int Wsgn=(W.Orientation()==TopAbs_FORWARD) ? 1 : -1;    
    for(TopoDS_Iterator it2(W,TopAbs_EDGE); it2.More(); it2.Next()){
      TopoDS_Shape E=it2.Value();
      int Esgn=(E.Orientation()==TopAbs_FORWARD) ? 1 : -1;
      E.Orientation(TopAbs_FORWARD);
      int EI=indexedEdges->FindIndex(E);
      GEdge *ge=indexedGEdges[EI];
      unsigned int ElineNum=ge->getNumMeshElements();
      int iv1 = ge->getMeshElement(0)->getVertex(0)->getIndex();
      int iv2 = ge->getMeshElement(ElineNum-1)->getVertex(1)->getIndex();
      if(iv1==iv2) if(ge->length()<1.e-8) continue;
      Fedges.push_back(ge);
      FedgesSign.push_back(Esgn);
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

    for(int k = 0; k <2; k++) for(int TI = 0; TI <FtriaNum; ++TI){
      MTriangle *t = gf->triangles[TI];
      SPoint3 CP(0.0,0.0,0.0);
      for(int j = 0; j < 3; ++j){
        MVertex *v = t->getVertex(j);
        int I = v->getIndex();
        if(PI2FPI.find(I)==PI2FPI.end())
        if (v->onWhat()->dim() < 2 || k==1){
	   PI2FPI[I]=FPNum++;
	   points.push_back(v->x());
	   points.push_back(v->y());
	   points.push_back(v->z());
           if(gf->geomType() != GEntity::Plane){
             SVector3 n=gf->normal(gf->parFromPoint(v->point()));
	     normals.push_back(n.x());
	     normals.push_back(n.y());
	     normals.push_back(n.z());
	   }
        }
      }
    }
    Handle(Geom_Surface) GS = BRep_Tool::Surface(F);
    for(int TI = 0; TI <FtriaNum; ++TI){
      MTriangle *t = gf->triangles[TI];
      SPoint3 CP(0.0,0.0,0.0);
      for(int j = 0; j < 3; ++j){
       MVertex *v = t->getVertex(j);
       CP+=v->point();
      }
      CP/=3.0;
      double SCPx, SCPy, SCPz;
      double CNx, CNy, CNz;
      supPointAndNormal(GS, CP.x(), CP.y(), CP.z(), SCPx, SCPy, SCPz, CNx, CNy, CNz);
      TI2FPI[TI]=FPNum++;
      points.push_back(SCPx);
      points.push_back(SCPy);
      points.push_back(SCPz);
      if(gf->geomType() != GEntity::Plane){
       normals.push_back(CNx*Ssign);
       normals.push_back(CNy*Ssign);
       normals.push_back(CNz*Ssign);
/*
       SPoint3 SCP(SCPx, SCPy, SCPz);
       SVector3 n=gf->normal(gf->parFromPoint(CP));
       normals.push_back(n.x());
       normals.push_back(n.y());
       normals.push_back(n.z());
*/
      }
     // checks orientation
      if(TI==0){
//      SVector3 n=gf->normal(gf->parFromPoint(CP));
       SVector3 n=SVector3(CNx*Ssign, CNy*Ssign, CNz*Ssign);
       SVector3 A=SVector3(t->getVertex(0)->point(),t->getVertex(1)->point());
       SVector3 B=SVector3(t->getVertex(0)->point(),t->getVertex(2)->point());
       if (dot(n,crossprod(A,B))<0) cout<< "Wrong orientation of face "<< FI<<endl;
      }
   }
//----------------------------------
  fprintf(fout, "  curves [\n");
  if(fwg && onWG) fprintf(fwg, "  curves [\n");
  for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
    int EI=indexedEdges->FindIndex(E);
    if(isPartition) {fprintf(fout, "\t\"C%d\"\n", EI); if(fwg && onWG) fprintf(fwg, "\t\"C%d\"\n", EI);}
    else            fprintf(fout, "\t\"%s\"\n",  ocaf->edgeData[EI-1].name.c_str());
    
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
//----------------------------------
   fprintf(fout, "  curveVertices [\n");
   if(fwg && onWG) fprintf(fwg, "  curveVertices [\n");
   for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    TopoDS_Shape V1sh=* (TopoDS_Shape *) ge->getBeginVertex()->getNativePtr();
    TopoDS_Shape V2sh=* (TopoDS_Shape *) ge->getEndVertex()->getNativePtr();
    TopoDS_Vertex V1=TopoDS::Vertex(V1sh);
    TopoDS_Vertex V2=TopoDS::Vertex(V2sh);
    int VI1=indexedVertices->FindIndex(V1);
    int VI2=indexedVertices->FindIndex(V2);
    if(isPartition) {
      fprintf(fout, "\t\"P%d\"\n", VI1); if(fwg && onWG) fprintf(fwg, "\t\"P%d\"\n", VI1);
      fprintf(fout, "\t\"P%d\"\n", VI2); if(fwg && onWG) fprintf(fwg, "\t\"P%d\"\n", VI2);
    }else{
      fprintf(fout, "\t\"%s\"\n",  ocaf->vertexData[VI1-1].name.c_str());
      fprintf(fout, "\t\"%s\"\n",  ocaf->vertexData[VI2-1].name.c_str());
    }
   }
   fprintf(fout, "  ]\n");
   if(fwg && onWG) fprintf(fwg, "  ]\n");
//----------------------------------
  fprintf(fout, "  PECcurves [\n");
  if(fwg && onWG) fprintf(fwg, "  PECcurves [\n");
  for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
    int EI=indexedEdges->FindIndex(E);
    int PECedge=ocaf->PECedge(EI);
    fprintf(fout, "\t%d,\n", PECedge);
    if(fwg && onWG) fprintf(fwg, "\t%d,\n", PECedge);
  }
  fprintf(fout, "  ]\n");
  if(fwg && onWG) fprintf(fwg, "  ]\n");
//----------------------------------
  fprintf(fout, "  ECcurves [\n");
  if(fwg && onWG) fprintf(fwg, "  ECcurves [\n");
  for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
    int EI=indexedEdges->FindIndex(E);
    int ECedge=ocaf->ECedge(EI);
    fprintf(fout, "\t%d,\n", ECedge);
    if(fwg && onWG) fprintf(fwg, "\t%d,\n", ECedge);
  }
  fprintf(fout, "  ]\n");
  if(fwg && onWG) fprintf(fwg, "  ]\n");
//----------------------------------
  fprintf(fout, "  PMCcurves [\n");
  if(fwg && onWG)  fprintf(fwg, "  PMCcurves [\n");
  for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
    int EI=indexedEdges->FindIndex(E);
    int PMCedge=ocaf->PMCedge(EI);
    fprintf(fout, "\t%d,\n", PMCedge);
    if(fwg && onWG) fprintf(fwg, "\t%d,\n", PMCedge);
  }
  fprintf(fout, "  ]\n");
  if(fwg && onWG) fprintf(fwg, "  ]\n");
//----------------------------------
  if(!isPartition){
   fprintf(fout, "  LPcurves [\n");
   for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    TopoDS_Shape E=* (TopoDS_Shape *) ge->getNativePtr();
    int EI=indexedEdges->FindIndex(E);
    int LPedge=ocaf->LPedge(EI);
    fprintf(fout, "\t%d,\n", LPedge);
   }
   fprintf(fout, "  ]\n");
  }
//----------------------------------
   fprintf(fout, "  points [\n");
   for(int i = 0; i< FPNum; i++){
	fprintf(fout, "\t%.16g %.16g %.16g,\n",
	                  points[3*i +0], 
	                  points[3*i +1], 
	                  points[3*i +2]
        );
   }
   fprintf(fout, "  ]\n");
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
   if(fwg && onWG){
     if(gf->geomType() != GEntity::Plane){
       fprintf(fwg, "  normals [\n");
       for(int i=0; i<FPNum; i++) 
         fprintf(fwg, "\t%.16f %.16f %.16f,\n",
	         normals[3*i +0], 
	         normals[3*i +1], 
	         normals[3*i +2]
         );
       fprintf(fwg, "  ]\n");
     }
   }
//----------------------------------
 fprintf(fout, "  curvePoints [\n");
 if(fwg && onWG) fprintf(fwg, "  curvePoints [\n");
 for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    unsigned int ElineNum=ge->getNumMeshElements();
    MElement *l;
    for(unsigned int i = 0; i <ElineNum;  ++i){
      l=ge->getMeshElement(i);
      fprintf(fout, "\t%d,\n", PI2FPI[l->getVertex(0)->getIndex()]);
      if(fwg && onWG) fprintf(fwg, "\t%d,\n", PI2FPI[l->getVertex(0)->getIndex()]);
    }
    fprintf(fout, "\t%d,\n", PI2FPI[l->getVertex(1)->getIndex()]);
    if(fwg && onWG) fprintf(fwg, "\t%d,\n", PI2FPI[l->getVertex(1)->getIndex()]);
    fprintf(fout, "\t-1,\n");
    if(fwg && onWG) fprintf(fwg, "\t-1,\n");
 }
 fprintf(fout, "  ]\n");
 if(fwg && onWG) fprintf(fwg, "  ]\n");
//----------------------------------
 fprintf(fout, "  curveTangents [\n");
 if(fwg && onWG) fprintf(fwg, "  curveTangents [\n");
 for (int i=0; i<Fedges.size(); i++){
    GEdge *ge=Fedges[i];
    unsigned int ElineNum=ge->getNumMeshElements();
    int CEsign;
    {//checks edges orientation versus curve orientation
     SPoint3 P1=ge->getMeshElement(0)->getVertex(0)->point();
     SVector3 T1=ge->firstDer(ge->parFromPoint(P1)); T1.normalize();
     SPoint3 P2=ge->getMeshElement(0)->getVertex(1)->point();
     SVector3 T2=ge->firstDer(ge->parFromPoint(P2)); T2.normalize();
     SVector3 T=SVector3(P1,P2);
     CEsign=SIGN(dot(T,T1+T2));
     assert(CEsign>0);
    }
    MElement *l;
    SVector3 T;
    for(unsigned int i = 0; i <ElineNum;  ++i){
      l=ge->getMeshElement(i);
      T=ge->firstDer(ge->parFromPoint(l->getVertex(0)->point()));
      T.normalize();
      fprintf(fout, "\t%.16f %.16f %.16f,\n", T.x(), T.y(), T.z());
      if(fwg && onWG) fprintf(fwg, "\t%.16f %.16f %.16f,\n", T.x(), T.y(), T.z());
    }
    T=ge->firstDer(ge->parFromPoint(l->getVertex(1)->point()));
    T.normalize();
    fprintf(fout, "\t%.16f %.16f %.16f,\n", T.x(), T.y(), T.z());
    if(fwg && onWG) fprintf(fwg, "\t%.16f %.16f %.16f,\n", T.x(), T.y(), T.z());
    fprintf(fout, "\t0 0 0,\n");
    if(fwg && onWG) fprintf(fwg, "\t0 0 0,\n");
 }
 fprintf(fout, "  ]\n");
 if(fwg && onWG) fprintf(fwg, "  ]\n");
//----------------------------------
  fprintf(fout, "  triangles [\n");
  for(int TI = 0; TI <FtriaNum;  ++TI){
     MTriangle *t = gf->triangles[TI];
     fprintf(fout, "\t");
     for(int j = 0; j < 3; ++j) fprintf(fout, "%d,", PI2FPI[t->getVertex(j)->getIndex()]);
     fprintf(fout, "-1,\n");
  }
  fprintf(fout, "  ]\n");
  if(fwg && onWG){
    fprintf(fwg, "  triangles [\n");
    for(int TI = 0; TI <FtriaNum;  ++TI){
       MTriangle *t = gf->triangles[TI];
       fprintf(fwg, "\t");
       for(int j = 0; j < 3; ++j) fprintf(fwg, "%d,", PI2FPI[t->getVertex(j)->getIndex()]);
       fprintf(fwg, "-1,\n");
    }
    fprintf(fwg, "  ]\n");
  }
//----------------------------------
   fprintf(fout, "  triaCenters [\n");
   for(int TI = 0; TI <FtriaNum;  ++TI) fprintf(fout, "\t%d,\n", TI2FPI[TI]);
   fprintf(fout, "  ]\n");
   if(fwg && onWG){
     fprintf(fwg, "  triaCenters [\n");
     for(int TI = 0; TI <FtriaNum;  ++TI) fprintf(fwg, "\t%d,\n", TI2FPI[TI]);
     fprintf(fwg, "  ]\n");
   }

//----------------------------------
    if(isPartition)  { fprintf(fout, "}\n\n"); if(fwg && onWG) fprintf(fwg, "}\n\n");  }
    else             fclose(fout);
  }
  if(isPartition) { fclose(fout);  if(fwg) fclose(fwg); }

}

