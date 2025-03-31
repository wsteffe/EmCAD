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


#if defined(HAVE_OCC_CONFIG_H)
 #include <config.h>
#endif

//#include "fmesh.h"
#include "OStools.h"
#include "GmshConfig.h"
#include "GmshMessage.h"
#include "gmsh.h"
#include "GmshGlobal.h"
#include "GModel.h"
#include "GModelIO_OCC.h"
#include "GRegion.h"
#include "MPrism.h"
#include "MPyramid.h"
#include "MHexahedron.h"
#include "MQuadrangle.h"
#include "MTetrahedron.h"
#include "MTriangle.h"
#include "MPoint.h"
#include "OpenFile.h"
#include "PView.h"
#include "PViewDataGModel.h"
#include "PViewDataList.h"
#include "GmshDefines.h"
#include "Context.h"
#include "meshGEdge.h"
#include "meshGFace.h"
#include "OCCEdge.h"
#include "MLine.h"
#include "Field.h"
//#include "GModelIO_OCC.h"
#include "assert.h"
#include <string>

#include "mesher.h"
#include "mwm.h"
//#include "mymeshGFaceBamg.h"

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <TCollection_AsciiString.hxx>

#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>

#include <TDF_ChildIterator.hxx>
#include <TDF_Reference.hxx>

#include <gp_Ax1.hxx>
#include <gp_Trsf.hxx>

#include "DataBase.h"

#ifndef WNT
#include <unistd.h>
#else
#include <windows.h>
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

#define DOT(v1, v2) \
  ((v1)[0] * (v2)[0] + (v1)[1] * (v2)[1] + (v1)[2] * (v2)[2])

#ifdef WNT
#define snprintf _snprintf
#endif

int loadMwm(const char *fName);

namespace MWM {
  extern MWM::Mesh *mesh;
}

int GfaceOCCfaceSign(GFace *gf);

bool mesh_aniso=false; 
bool useFaceAttractor=false;
bool useEdgeAttractor=true;
bool useBoundaryLayer=false;

extern bool conformalMeshIF;


void gp_trsf_2_gmsh(gp_Trsf& trsf,std::vector<double>& tfo)
{
 tfo.resize(16,0);
 for (int i = 0; i < 3; i++) 
   for (int j = 0; j < 4; j++) 
     tfo[4*i+j] =trsf.Value(i+1,j+1);
 tfo[15]=1;
}

void gmsh_2_gp_trsf(std::vector<double>& tfo, gp_Trsf& trsf)
{
 trsf.SetValues(
	        tfo[4*0+0],tfo[4*0+1],tfo[4*0+2],tfo[4*0+3],
	        tfo[4*1+0],tfo[4*1+1],tfo[4*1+2],tfo[4*1+3],
	        tfo[4*2+0],tfo[4*2+1],tfo[4*2+2],tfo[4*2+3]
	       );
}

void computeAffineTransf(DB::Transform *invt, gp_Trsf& trsf, int sgn=1)
{
  sgn=SIGN(sgn);
  if(invt->rotAngle!=0){
    gp_Pnt O =gp_Pnt(invt->rotOrigin[0],invt->rotOrigin[1],invt->rotOrigin[2]);
    gp_Dir D =gp_Dir(invt->rotAxis[0],invt->rotAxis[1],invt->rotAxis[2]);
    gp_Ax1 A=gp_Ax1(O, D);
    trsf.SetRotation(A,sgn*invt->rotAngle);
  } else{
    gp_Vec T =gp_Vec(sgn*invt->trasl[0],sgn*invt->trasl[1],sgn*invt->trasl[2]);
    trsf.SetTranslation(T);
  }
}

void computeAffineTransf_(DB::Transform *invt, std::vector<double>& tfo, int sgn=1)
{

  sgn=SIGN(sgn);
  double ca = cos(invt->rotAngle);
  double sa = sin(invt->rotAngle)*sgn;

  double ux = invt->rotAxis[0];
  double uy = invt->rotAxis[1];
  double uz = invt->rotAxis[2];

  tfo.resize(16,0.0);

  tfo[0*4+0] = ca + ux*ux*(1.-ca);
  tfo[0*4+1] = ux*uy*(1.-ca) - uz * sa;
  tfo[0*4+2] = ux*uz*(1.-ca) + uy * sa;

  tfo[1*4+0] = ux*uy*(1.-ca) + uz * sa;
  tfo[1*4+1] = ca + uy*uy*(1.-ca);
  tfo[1*4+2] = uy*uz*(1.-ca) - ux * sa;

  tfo[2*4+0] = ux*uz*(1.-ca) - uy * sa;
  tfo[2*4+1] = uy*uz*(1.-ca) + ux * sa;
  tfo[2*4+2] = ca + uz*uz*(1.-ca);

  int idx = 0;
  for (size_t i = 0; i < 3; i++,idx++) {
    int tIdx = i*4+3;
    tfo[tIdx] = invt->rotOrigin[i] + sgn*invt->trasl[i];
    for (int j = 0; j < 3; j++,idx++) tfo[tIdx] -= tfo[idx] * invt->rotOrigin[j];
  }

  for (int i = 0; i < 4; i++) tfo[12+i] = 0;
  tfo[15] = 1;
}

void multiplyAffineTransf(const std::vector<double> &a, const std::vector<double> &b, std::vector<double> &c)
{
    c.resize(16,0.0);
    for(int i = 0; i < 4; i++)
      for(int j = 0; j < 4; j++)
        for(int k = 0; k < 4; k++)
          c[i+4 * j] += a[i+4*k] * b[k+4*j];
}

void multiplyMaps(const std::map<int,int> &a, const std::map<int,int> &b, std::map<int,int> &c){
    for (std::map<int,int>::const_iterator it=a.begin(); it!= a.end(); it++){ 
	  std::map<int,int>::const_iterator jt=b.find(abs((*it).second));
	  assert(jt!=b.end());
	  c[(*it).first] = ((*jt).second) * SIGN((*it).second);
    }
}

void invertMap(const std::map<int,int> &a, std::map<int,int> &b){
     for (std::map<int,int>::const_iterator it=a.begin(); it!= a.end(); it++) b[abs((*it).second)]=(*it).first*SIGN((*it).second);
}



gp_Vec  supNormal(Handle(Geom_Surface) GS, gp_Pnt GP);


void print_mwm(GModel *gm,  MwOCAF* ocaf, bool meshIF, bool mesh3D, bool meshWG,
	      const char* dirName, const char* modelDir,
	      std::map< int, std::string > &solidNames,
	      std::vector<int> &Fmaster);


void mesher_setLC(GModel *gm, double meshsize){
      int minCircPoints=10;
      for(GModel::eiter it = gm->firstEdge(); it != gm->lastEdge(); it++){
	      GEdge *e=(*it);
	      Range<double> range =e->parBounds(0);
	      GVertex * v1=e->getBeginVertex();
	      double curv1 =e->curvature(range.low());
	      double lc1 = curv1 > 0 ? 2 * M_PI/curv1 /minCircPoints : MAX_LC;
	      lc1=min(lc1,meshsize);
	      GVertex * v2=e->getEndVertex();
	      double curv2 =e->curvature(range.high());
	      double lc2 = curv2 > 0 ? 2 * M_PI/curv2 /minCircPoints : MAX_LC;
	      lc2=min(lc2,meshsize);
	      v1->setPrescribedMeshSizeAtVertex(min(v1->prescribedMeshSizeAtVertex(),lc1));
	      v2->setPrescribedMeshSizeAtVertex(min(v2->prescribedMeshSizeAtVertex(),lc2));
      }
}





double max_refri(GModel *gm, MwOCAF* ocaf){
//      for(GModel::viter it = gm->firstVertex(); it != gm->lastVertex(); it++)
//	      (*it)->setPrescribedMeshSizeAtVertex(meshsize);
     double refri=1.0;
     for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
         GFace *gf=*fit;
         TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
         int FI=ocaf->indexedFaces->FindIndex(F);
         assert(FI);
         if(!FI) continue;
         if(ocaf->faceAdjParts) for (int parti=0; parti< 2; parti++){
             TCollection_AsciiString volname=ocaf->faceAdjParts[2*(FI-1)+parti];
             if(volname!=TCollection_AsciiString("?")){
		 DB::Volume *vol=ocaf->EmP->FindVolume(volname.ToCString());
                 DB::Material* mat;
		 if(vol) mat= ocaf->EmP->FindMaterial(vol->material);
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

MVertex * MWM::Curve::mesh_vertices(int i){
 int ii = masterSign>0? i : points.n-1-i;
 GEdge *me =dynamic_cast<GEdge *>(ge->getMeshMaster());
 if(!me || me==ge) return NULL;
 if(ii==0)                    return me->getBeginVertex()->mesh_vertices[0]; 
 if(ii==points.n-1)           return me->getEndVertex()->mesh_vertices[0]; 
 if(ii>0 && ii<points.n-1)    return me->mesh_vertices[ii-1];
 return NULL;
}

MVertex * MWM::Curve::master_mesh_vertices(int i){
 int ii = masterSign>0? i : points.n-1-i;
 GEdge *me =dynamic_cast<GEdge *>(ge->getMeshMaster());
 if(!me || me==ge) return NULL;
 if(ii==0)                    return me->getBeginVertex()->mesh_vertices[0]; 
 if(ii==points.n-1)           return me->getEndVertex()->mesh_vertices[0]; 
 if(ii>0 && ii<points.n-1)    return me->mesh_vertices[ii-1];
 return NULL;
}


void MWM::Curve::setGEdge(GEdge * ge_, int GMSH_Esign){
  ge=ge_;
  masterSign=GMSH_Esign;
}



void MWM::Curve::setMesh(){
  int N = points.n;
  if(N<3) return;
  GVertex *gv;
  gv=ge->getBeginVertex();
  if(gv->mesh_vertices.empty()){
     gv->mesh_vertices.push_back(new MVertex(gv->x(), gv->y(), gv->z(), gv));
     MVertex *mshv=new MVertex(gv->point().x(), gv->point().y(), gv->point().z(), gv);
     gv->mesh_vertices.push_back(mshv);
  }
  gv=ge->getEndVertex();
  if(gv->mesh_vertices.empty()){
     gv->mesh_vertices.push_back(new MVertex(gv->x(), gv->y(), gv->z(), gv));
     MVertex *mshv=new MVertex(gv->point().x(), gv->point().y(), gv->point().z(), gv);
     gv->mesh_vertices.push_back(mshv);
  }

/*
  ge->mesh_vertices.resize(N-2);
  Range<double> bounds = ge->parBounds(0);
  double u_begin = bounds.low();
  double u_end = bounds.high();
  GVertex *gv1=ge->getBeginVertex();
  if(gv1->mesh_vertices.size()==0){
       MVertex *v1 = new MVertex(gv1->x(),gv1->y(),gv1->z(),gv1,gv1->prescribedMeshSizeAtVertex());
       gv1->addMeshVertex(v1);
  }
*/

/*  
  GVertex *gv1=ge->getBeginVertex();
  GPoint P1=gv1->point();
  GPoint CP1((&points[0])[0],(&points[0])[1],(&points[0])[2]);

  GVertex *gv2=ge->getEndVertex();
  GPoint P2=gv2->point();
  GPoint CP2((&points[N-1])[0],(&points[N-1])[1],(&points[N-1])[2]);
  double d11=P1.distance(CP1);
  double d12=P1.distance(CP2);
  double d21=P2.distance(CP1);
  double d22=P2.distance(CP2);
  assert(min(d11,d12)<1.e-5);
  assert(min(d21,d22)<1.e-5);
  int GMSH_Esign=d11<d12 ? 1 : -1;
*/

  deMeshGEdge dem;
  dem(ge);
  ge->mesh_vertices.clear();
  Range<double> bounds = ge->parBounds(0);
/*
  double u_min=min(pointsU[0],pointsU[N-1]);
  double u_max=max(pointsU[0],pointsU[N-1]);
  double to_u_min = bounds.low();
  double to_u_max = bounds.high();
*/
  for(int i = 1; i < N-1; i++){
    int ii=masterSign > 0 ? i : N-1-i;
/*
    double u=pointsU[ii];
    double newu= (GMSH_Esign > 0) ? (u - u_min + to_u_min) : (u_max - u + to_u_min);;
    GPoint P = ge->point(newu);
*/
    SPoint3 P((&points[ii])[0],(&points[ii])[1],(&points[ii])[2]);
    double u = ge->parFromPoint(P);
    MVertex *v=new MEdgeVertex(P.x(),P.y(),P.z(), ge, u, ge->meshAttributes.meshSize);
    ge->mesh_vertices.push_back(v);
  }
  for(int i = 0; i <  points.n-1; i++)
    ge->lines.push_back(new MLine(mesh_vertices(i), mesh_vertices(i+1)));

  ge->setFixedMeshIF(true);
}


void MWM::Curve::setMasterMesh(){
  GEdge *me = dynamic_cast<GEdge *>(ge->getMeshMaster());
  if(!me || me==ge) return;
  int N = points.n;
  if(N<3) return;

  deMeshGEdge dem;
  dem(me);
  me->mesh_vertices.clear();
  Range<double> bounds = me->parBounds(0);
/*
  double u_min=min(pointsU[0],pointsU[N-1]);
  double u_max=max(pointsU[0],pointsU[N-1]);
  double to_u_min = bounds.low();
  double to_u_max = bounds.high();
*/
  for(int i = 1; i < N-1; i++){
    int ii=masterSign > 0 ? i : N-1-i;
/*
    double u=pointsU[ii];
    double newu= (GMSH_Esign > 0) ? (u - u_min + to_u_min) : (u_max - u + to_u_min);;
    GPoint P = ge->point(newu);
*/
    SPoint3 P((&points[ii])[0],(&points[ii])[1],(&points[ii])[2]);
    double u = me->parFromPoint(P);
    MVertex *v=new MEdgeVertex(P.x(),P.y(),P.z(), me, u, ge->meshAttributes.meshSize);
    me->mesh_vertices.push_back(v);
  }
  for(int i = 0; i <  points.n-1; i++)
    me->lines.push_back(new MLine(master_mesh_vertices(i), master_mesh_vertices(i+1)));

  me->setVisibility(false);
  me->setFixedMeshIF(true);
}

int MWM::Surface::GFaceSign(GFace *gf){
    SPoint3 P[3];
    for(int j=0; j<3; j++){
       int i=(&(triangles[0]))[j];
       P[j]=SPoint3((&points[i])[0],(&points[i])[1],(&points[i])[2]);
    }
    SPoint2 uv = gf->parFromPoint(P[0], true, true);
    SVector3 n=gf->normal(uv);
    SVector3 A=SVector3(P[0],P[1]);
    SVector3 B=SVector3(P[0],P[2]);
    return SIGN(dot(n,crossprod(A,B)));
}

void MWM::Surface::setGFace(GFace * gf_){
  gf=gf_;
  masterSign=GFaceSign(gf);
}


void MWM::Surface::setMesh(){

  int N = points.n;
  bool checkMesh=true;

  deMeshGFace dem;
  dem(gf);
  for(int ic=0; ic< NumOfCurves; ic++)
    for(int j=0; j< curvePoints[ic].size(); j++)
       mesh_vertices[curvePoints[ic][j]]=curves[ic]->mesh_vertices(j);

  for(int i = 0; i < N; i++) if(!mesh_vertices[i]){
    SPoint3 P((&points[i])[0],(&points[i])[1],(&points[i])[2]);
    SPoint2 uv = gf->parFromPoint(P, true, true);
    MVertex *v = new MFaceVertex(P.x(),P.y(),P.z(),gf,uv.x(), uv.y());
    mesh_vertices[i]=v;
    gf->mesh_vertices.push_back(v);
  }
  for(int itr=0; itr<triangles.n; itr++){
    MVertex *tv[3];
    for(int j=0; j<3; j++){
       int i=(&(triangles[itr]))[masterSign<0 ? (3-j-1) : j];
       tv[j]=mesh_vertices[i];
/*
       int I=pointsI[iS];
       MVertex *v=getMeshVertexByTag(I);
*/
    }
    gf->triangles.push_back(new MTriangle(tv[0],tv[1],tv[2]));
    if(checkMesh) for(int j=0; j<3; j++){
       int i=(&(triangles[itr]))[masterSign<0 ? (3-j-1) : j];
       SPoint3 TP=mesh_vertices[i]->point();
       SPoint3 SP=gf->triangles[itr]->getVertex(j)->point();
       assert(TP.distance(SP)<1.e-5);
     }
  }
  
  gf->setFixedMeshIF(true);
  
}

void MWM::Surface::checkMesh(){
  int N = points.n;
  for(int itr=0; itr<triangles.n; itr++){
    for(int j=0; j<3; j++){
       int i=(&(triangles[itr]))[masterSign<0 ? (3-j-1) : j];
       SPoint3 TP=mesh_vertices[i]->point();
       SPoint3 SP=gf->triangles[itr]->getVertex(j)->point();
       assert(TP.distance(SP)<1.e-5);
     }
  }
}



void MWM::Surface::setMasterMesh(){
  GFace *mf = dynamic_cast<GFace *>(gf->getMeshMaster());
  if(!mf || mf==gf) return;

  int N = points.n;
  bool checkMesh=true;

  deMeshGFace dem;
  dem(mf);
  for(int ic=0; ic< NumOfCurves; ic++)
    for(int j=0; j< curvePoints[ic].size(); j++)
       mesh_vertices[curvePoints[ic][j]]=curves[ic]->mesh_vertices(j);

  for(int i = 0; i < N; i++) if(!mesh_vertices[i]){
    SPoint3 P((&points[i])[0],(&points[i])[1],(&points[i])[2]);
    SPoint2 uv = mf->parFromPoint(P, true, true);
    MVertex *v = new MFaceVertex(P.x(),P.y(),P.z(),mf,uv.x(), uv.y());
    mesh_vertices[i]=v;
    mf->mesh_vertices.push_back(v);
  }
  for(int itr=0; itr<triangles.n; itr++){
    MVertex *tv[3];
    for(int j=0; j<3; j++){
       int i=(&(triangles[itr]))[masterSign<0 ? (3-j-1) : j];
       tv[j]=mesh_vertices[i];
/*
       int I=pointsI[iS];
       MVertex *v=getMeshVertexByTag(I);
*/
    }
    mf->triangles.push_back(new MTriangle(tv[0],tv[1],tv[2]));
    if(checkMesh) for(int j=0; j<3; j++){
       int i=(&(triangles[itr]))[masterSign<0 ? (3-j-1) : j];
       SPoint3 TP=mesh_vertices[i]->point();
       SPoint3 SP=mf->triangles[itr]->getVertex(j)->point();
       assert(TP.distance(SP)<1.e-5);
     }
  }
  
  mf->setVisibility(false);
  mf->setFixedMeshIF(true);
}

void MWM::Surface::checkMasterMesh(){
  GFace *mf = dynamic_cast<GFace *>(gf->getMeshMaster());
  if(!mf || mf==gf) return;
  int N = points.n;
  for(int itr=0; itr<triangles.n; itr++){
    for(int j=0; j<3; j++){
       int i=(&(triangles[itr]))[masterSign<0 ? (3-j-1) : j];
       SPoint3 TP=mesh_vertices[i]->point();
       SPoint3 SP=mf->triangles[itr]->getVertex(j)->point();
       assert(TP.distance(SP)<1.e-5);
     }
  }
}

void MWM::Surface::setCorrespondingTriangles(std::vector<int> &correspondingTriangles){
  GFace *mf = dynamic_cast<GFace *>(gf->getMeshMaster());
  if(!mf || mf==gf) return;
  std::map<std::set<MVertex *>, int> mtv2I;
  for(int I = 0; I <mf->triangles.size(); ++I){
      MTriangle *mt = mf->triangles[I];
      std::set<MVertex *> mtv;
      for(int j = 0; j < 3; ++j) mtv.insert(mt->getVertex(j));
      mtv2I[mtv]=I;
  }
  for(int I = 0; I <gf->triangles.size(); ++I){
      MTriangle *gt = gf->triangles[I];
      std::set<MVertex *> mtv;
      for(int j = 0; j < 3; ++j) mtv.insert(gf->correspondingVertices[gt->getVertex(j)]);
      correspondingTriangles[mtv2I[mtv]]=I;
  }
}



namespace MWM {
  void setMasterMesh(int dim, GModel *gm){
     DB::List_T  *curvs=DB::Tree2List(MWM::mesh->curves);
     int nc=DB::List_Nbr(curvs);
     int ns=0; DB::List_T  *surfs=NULL;
     std::vector<std::pair<int, int> > inDimTags, outDimTags;
     if(dim==1){
      for(int ic=0; ic< nc; ic++){
        MWM::Curve *c;  DB::List_Read(curvs,ic,&c);
        inDimTags.push_back(std::pair<int,int>(c->ge->dim(),c->ge->tag()));
      }
     } else {
       surfs=DB::Tree2List(MWM::mesh->surfaces);
       ns=DB::List_Nbr(surfs);
       for(int is=0; is< ns; is++){
         MWM::Surface *s;  DB::List_Read(surfs,is,&s);
         inDimTags.push_back(std::pair<int,int>(s->gf->dim(),s->gf->tag()));
       }
     }
     bool r = gm->getOCCInternals()->copy(inDimTags, outDimTags);
     GModel::current()->getOCCInternals()->synchronize(GModel::current());
     std::vector<double> tfo={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
     if(dim==1) for(int ic=0; ic< nc; ic++){
         MWM::Curve *c;  DB::List_Read(curvs,ic,&c);
	 GEdge *me=gm->getEdgeByTag(outDimTags[ic].second);
         c->ge->setMeshMaster(me,tfo);
     } else for(int is=0; is< ns; is++){
         MWM::Surface *s;  DB::List_Read(surfs,is,&s);
	 GFace *mf=gm->getFaceByTag(outDimTags[is].second);
         s->gf->setMeshMaster(mf,tfo);
     }
     std::set<GVertex *> verts;
     for(int ic=0; ic< nc; ic++){
         MWM::Curve *c;  DB::List_Read(curvs,ic,&c);
         verts.insert(c->ge->getBeginVertex());
         verts.insert(c->ge->getEndVertex());
     }
     for (std::set<GVertex *>::iterator it=verts.begin(); it!= verts.end(); it++){
	  GVertex *gv=*it;
          GVertex *mv = dynamic_cast<GVertex *>(gv->getMeshMaster());
	  if(!mv || mv==gv) continue;
	  if(mv->mesh_vertices.empty()){
            MVertex *mshv=new MVertex(mv->point().x(), mv->point().y(), mv->point().z(), mv);
	    mv->mesh_vertices.push_back(mshv);
            mv->setVisibility(false);
	  }
     }
     for(int ic=0; ic< nc; ic++){
          MWM::Curve *c;  DB::List_Read(curvs,ic,&c);
          c->setMasterMesh();
     }
     if(dim>1) for(int is=0; is< ns; is++){
           MWM::Surface *s;  DB::List_Read(surfs,is,&s);
           s->setMasterMesh();
     }
     List_Delete(curvs);
     if(dim>1) List_Delete(surfs);
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
		    std::list<int> &mshFieldList)
{
  FieldManager *fields = gm->getFields();
  int fieldTag=fields->newId(); mshFieldList.push_back(fieldTag);
  Field *f=fields->newField(fieldTag, "MathEvalAniso");
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


void mesher_setSingularEdges(GModel *gm, MwOCAF* ocaf)
{
  std::map<int,int> Emult;
  std::set<int> BDRedges;
  typedef std::map<int,int>::const_iterator IntMapIt;
  std::map<int,std::pair<gp_Vec, gp_Vec > > Enormals;
  for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
     GFace *gf=*fit;
     if(gf->meshAttributes.method==MESH_NONE) continue;
     TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
     F.Orientation(TopAbs_FORWARD);
     int FI=ocaf->indexedFaces->FindIndex(F);
     TCollection_AsciiString bdrname= (ocaf->faceData[FI-1].BrCond.size()>0) ? ocaf->faceData[FI-1].BrCond.begin()->c_str() : "-";
     TCollection_AsciiString vol1name=ocaf->faceAdjParts[2*(FI-1)];
     TCollection_AsciiString vol2name=ocaf->faceAdjParts[2*(FI-1)+1];
     DB::Volume *vol1=NULL;
     if(vol1name!=TCollection_AsciiString("-")) vol1=ocaf->EmP->FindVolume(vol1name.ToCString());
     DB::Volume *vol2=NULL;
     if(vol2name!=TCollection_AsciiString("-")) vol2=ocaf->EmP->FindVolume(vol2name.ToCString());
     int FDsign=1;
     bool isBoundaryFace =false;
     if(vol1) if(vol1->type==DIELECTRIC){
	     if(vol2) isBoundaryFace=vol2->type==BOUNDARYCOND;
	     else     isBoundaryFace=true;
	     FDsign=1;
     }
     if(vol2) if(vol2->type==DIELECTRIC){
	     if(vol1) isBoundaryFace=vol1->type==BOUNDARYCOND;
	     else     isBoundaryFace=true;
	     FDsign=-1;
     }
     bool isSheet =false;
     if(bdrname!=TCollection_AsciiString("-")) if(vol1) if(vol2) if(vol1->type==DIELECTRIC) if(vol2->type==DIELECTRIC) isSheet=true;
     if(isSheet){
        for(TopExp_Explorer exp(F,TopAbs_WIRE); exp.More(); exp.Next()){
          TopoDS_Wire W=TopoDS::Wire(exp.Current());
          for(BRepTools_WireExplorer wexp(W); wexp.More(); wexp.Next()){
	    TopoDS_Edge E=wexp.Current();
	    E.Orientation(TopAbs_FORWARD);
            int EI=ocaf->indexedEdges->FindIndex(E);
	    Emult[EI]++;
	  }
        }
     }
     if(isBoundaryFace) {
	 Handle(Geom_Surface) gs = BRep_Tool::Surface(TopoDS::Face(F));
         for(TopExp_Explorer exp(F,TopAbs_WIRE); exp.More(); exp.Next()){
	 TopoDS_Wire W = TopoDS::Wire(exp.Current());
	 BRepTools_WireExplorer wexp;
         for (wexp.Init(W); wexp.More(); wexp.Next()) {
	     TopoDS_Edge E=wexp.Current();
             int EFsign=(E.Orientation()==TopAbs_FORWARD)? 1:-1;
	     E.Orientation(TopAbs_FORWARD);
             TopoDS_Vertex V1,V2;
             TopExp::Vertices(E,V1,V2, false);
             gp_Pnt GP1 = BRep_Tool::Pnt(V1);
             gp_Vec FN=supNormal(gs, GP1);
             double u1,u2;
             Handle(Geom_Curve) gec=BRep_Tool::Curve(E, u1, u2);
	     if(gec.IsNull()) continue;
             gp_Pnt CP1; gp_Vec ET1;
	     gec->D1(u1, CP1,ET1); ET1/=ET1.Magnitude(); ET1*=EFsign;
             gp_Vec BN=ET1.Crossed(FN);
	     gp_Vec DN=FN*FDsign;
             int EI=ocaf->indexedEdges->FindIndex(E);
             BDRedges.insert(EI);
	     if(Enormals.find(EI)==Enormals.end()){
                  Enormals[EI]=std::pair<gp_Vec, gp_Vec >(DN,BN);
	     } else {
                  Enormals[EI].first+=DN;
                  Enormals[EI].second+=BN;
		  double dd=Enormals[EI].first.Dot(Enormals[EI].second);
		  if(dd>0.2 ) ocaf->edgeData[EI-1].singular=1;
             }
	 }
         }
     }
  }
  for (IntMapIt it=Emult.begin(); it!= Emult.end(); it++) if((*it).second==1) 
	  if(BDRedges.find((*it).first)==BDRedges.end()) ocaf->edgeData[(*it).first-1].singular=2;

}

void mesher_setSingularVertices(GModel *gm, MwOCAF* ocaf,
		    double meshsize, std::list<int> &mshFieldList)
{
  std::map<int,int> Pmult;
  typedef std::map<int,int>::const_iterator IntMapIt;
  std::map<int,std::pair<gp_Vec, gp_Vec > > Pnormals;
  for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
     GFace *gf=*fit;
     if(gf->meshAttributes.method==MESH_NONE) continue;
     TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
     F.Orientation(TopAbs_FORWARD);
     int FI=ocaf->indexedFaces->FindIndex(F);
     if(!ocaf->faceData[FI-1].Shared()&&ocaf->faceData[FI-1].BrCond.find(std::string("WAVEGUIDE"))==ocaf->faceData[FI-1].BrCond.end()) continue;
     Handle(Geom_Surface) gs = BRep_Tool::Surface(TopoDS::Face(F));
     for(TopExp_Explorer exp(F,TopAbs_WIRE); exp.More(); exp.Next()){
        TopoDS_Wire W=TopoDS::Wire(exp.Current());
        for(BRepTools_WireExplorer wexp(W); wexp.More(); wexp.Next()){
	    TopoDS_Edge E=wexp.Current();
            int Esign=(E.Orientation()==TopAbs_FORWARD)? 1:-1;
	    E.Orientation(TopAbs_FORWARD);
            int EI=ocaf->indexedEdges->FindIndex(E);
	    if(ocaf->edgeData[EI-1].BrCond.size()==0) continue;
            TopoDS_Vertex V1,V2;
            TopExp::Vertices(E,V1,V2, true);
            int VI1=ocaf->indexedVertices->FindIndex(V1);
            int VI2=ocaf->indexedVertices->FindIndex(V2);
	    Pmult[VI1]++;
	    Pmult[VI2]++;
	    gp_Pnt GP1 = BRep_Tool::Pnt(V1);
	    gp_Pnt GP2 = BRep_Tool::Pnt(V2);
            gp_Vec SN1=supNormal(gs, GP1);
            gp_Vec SN2=supNormal(gs, GP2);
            double u1,u2;
            Handle(Geom_Curve) gec=BRep_Tool::Curve(E, u1, u2);
            gp_Pnt CP1; gp_Vec ET1;
            gp_Pnt CP2; gp_Vec ET2;
	    gec->D1(u1,CP1,ET1); ET1/=ET1.Magnitude();
	    gec->D1(u2,CP2,ET2); ET2/=ET2.Magnitude();
            gp_Vec BN1=SN1.Crossed(ET1)*Esign;
            gp_Vec BN2=SN2.Crossed(ET2)*Esign;
	    if(Pnormals.find(VI1)==Pnormals.end()){
                  Pnormals[VI1]=std::pair<gp_Vec, gp_Vec >(-ET1,BN1);
	    } else {
                  Pnormals[VI1].first-=ET1;
                  Pnormals[VI1].second+=BN1;
		  double dd=Pnormals[VI1].first.Dot(Pnormals[VI1].second);
		  if(dd>0.2 ) ocaf->vertexData[VI1-1].singular=1;
		  if(Pnormals[VI1].second.Magnitude()<0.001) ocaf->vertexData[VI1-1].singular=1;

            }
	    if(Pnormals.find(VI2)==Pnormals.end()){
                  Pnormals[VI2]=std::pair<gp_Vec, gp_Vec >(ET2,BN2);
	    } else {
                  Pnormals[VI2].first+=ET2;
                  Pnormals[VI2].second+=BN2;
		  double dd=Pnormals[VI2].first.Dot(Pnormals[VI2].second);
		  if(dd>0.2 ) ocaf->vertexData[VI2-1].singular=1;
		  if(Pnormals[VI2].second.Magnitude()<0.001) ocaf->vertexData[VI2-1].singular=1;
            }
	 }
     }
  }
  for (IntMapIt it=Pmult.begin(); it!= Pmult.end(); it++) if(Pmult[(*it).second]==1) ocaf->vertexData[Pmult[(*it).first]-1].singular=2;


  std::list<int> Vl;
  for(GModel::viter it = gm->firstVertex(); it != gm->lastVertex(); it++){
	  GVertex *gv=(*it); 
          TopoDS_Shape V=* (TopoDS_Shape *) gv->getNativePtr();
          int VI=ocaf->indexedVertices->FindIndex(V);
	  if (ocaf->vertexData[VI-1].singular)  Vl.push_back(gv->tag());
  }
  if(!Vl.size()) return;
  FieldManager *fields = gm->getFields();
  int fieldTag1=fields->newId(); 
  Field *distance=fields->newField(fieldTag1, "Distance");
  distance->options["PointsList"]->list(Vl); 
  int fieldTag=fields->newId(); mshFieldList.push_back(fieldTag);
  Field *thsld=fields->newField(fieldTag, "Threshold");
  thsld->options["InField"]->numericalValue(fieldTag1);
  if(mesh_aniso){
        thsld->options["DistMin"]->numericalValue(meshsize*0.6);
        thsld->options["DistMax"]->numericalValue(meshsize*2.5);
        thsld->options["SizeMin"]->numericalValue(meshsize/3);
        thsld->options["SizeMax"]->numericalValue(meshsize);
  }else{
        thsld->options["DistMin"]->numericalValue(meshsize/50);
        thsld->options["DistMax"]->numericalValue(meshsize);
        thsld->options["SizeMin"]->numericalValue(meshsize/50);
        thsld->options["SizeMax"]->numericalValue(meshsize);
  }
}


void mesher_setRefinedVertices(GModel *gm,
	            TopTools_IndexedMapOfShape *indexedVertices,
		    double meshsize, std::list<int> &mshFieldList)
{
  std::list<int> Vl;
  for(GModel::viter it = gm->firstVertex(); it != gm->lastVertex(); it++){
	  GVertex *gv=(*it); 
          TopoDS_Shape V=* (TopoDS_Shape *) gv->getNativePtr();
	  Vl.push_back(gv->tag());
  }
  if(!Vl.size()) return;
  FieldManager *fields = gm->getFields();
  int fieldTag1=fields->newId();
  Field *distance=fields->newField(fieldTag1, "Distance");
  distance->options["PointsList"]->list(Vl);
  int fieldTag=fields->newId(); mshFieldList.push_back(fieldTag);
  Field *thsld=fields->newField(fieldTag, "Threshold");
  thsld->options["InField"]->numericalValue(fieldTag1);
  thsld->options["DistMin"]->numericalValue(0);
  thsld->options["DistMax"]->numericalValue(meshsize*0.3);
  thsld->options["SizeMin"]->numericalValue(meshsize*0.3);
  thsld->options["SizeMax"]->numericalValue(meshsize);
}

class myVertex{
 private:
  int _tag;
  double _x, _y, _z;
 public:
  myVertex(int tag, double x, double y, double z)
    : _tag(tag), _x(x), _y(y), _z(z) {}
  int tag() const { return _tag; }
  double x() const { return _x; }
  double y() const { return _y; }
  double z() const { return _z; }
  double distance(const myVertex &other) const
  {
    return sqrt(std::pow(x() - other.x(), 2) +
                std::pow(y() - other.y(), 2) +
                std::pow(z() - other.z(), 2));
  }
};


extern double det3x3(double mat[3][3]);

class myElement{
 private:
  int _tag;
  std::vector<myVertex*> _nodes;
 public:
  myElement(int tag, const std::vector<myVertex*> &nodes)
    : _tag(tag), _nodes(nodes) {}
  int tag() const { return _tag; }
  const std::vector<myVertex*> &nodes() const { return _nodes; }
  int nodesNum() const { return _nodes.size();}
  double minRefEdge(std::map< std::pair<int,int>, double> &refmap) const
  {
    double l12,l13,l23,l14,l24,l34;
    if(_nodes.size() >=2){
      std::pair<int,int> p12(min(_nodes[0]->tag(),_nodes[1]->tag()),max(_nodes[0]->tag(),_nodes[1]->tag()));
      l12 = _nodes[0]->distance(*_nodes[1])/max(refmap[p12],1.0);
      if(_nodes.size()==2) return l12;
    }
    if(_nodes.size() >= 3){
      std::pair<int,int> p13(min(_nodes[0]->tag(),_nodes[2]->tag()),max(_nodes[0]->tag(),_nodes[2]->tag()));
      l13 = _nodes[0]->distance(*_nodes[2])/max(refmap[p13],1.0);
      std::pair<int,int> p23(min(_nodes[1]->tag(),_nodes[2]->tag()),max(_nodes[1]->tag(),_nodes[2]->tag()));
      l23 = _nodes[1]->distance(*_nodes[2])/max(refmap[p23],1.0);
      if(_nodes.size()==3) return min(l12, min(l13, l23));
    }
    if(_nodes.size() >= 4){
      std::pair<int,int> p14(min(_nodes[0]->tag(),_nodes[3]->tag()),max(_nodes[0]->tag(),_nodes[3]->tag()));
      l14 = _nodes[0]->distance(*_nodes[3])/max(refmap[p14],1.0);
      std::pair<int,int> p24(min(_nodes[1]->tag(),_nodes[3]->tag()),max(_nodes[1]->tag(),_nodes[3]->tag()));
      l24 = _nodes[1]->distance(*_nodes[3])/max(refmap[p24],1.0);
      std::pair<int,int> p34(min(_nodes[2]->tag(),_nodes[3]->tag()),max(_nodes[2]->tag(),_nodes[3]->tag()));
      l34 = _nodes[2]->distance(*_nodes[3])/max(refmap[p34],1.0);
      return min(min(l12, min(l13, l23)),min(l14, min(l24, l34)));
    }
  }
  double maxRefEdge(std::map< std::pair<int,int>, double> &refmap) const
  {
    double l12,l13,l23,l14,l24,l34;
    if(_nodes.size() >=2){
      std::pair<int,int> p12(min(_nodes[0]->tag(),_nodes[1]->tag()),max(_nodes[0]->tag(),_nodes[1]->tag()));
      l12 = _nodes[0]->distance(*_nodes[1])/max(refmap[p12],1.0);
      if(_nodes.size()==2) return l12;
    }
    if(_nodes.size() >= 3){
      std::pair<int,int> p13(min(_nodes[0]->tag(),_nodes[2]->tag()),max(_nodes[0]->tag(),_nodes[2]->tag()));
      l13 = _nodes[0]->distance(*_nodes[2])/max(refmap[p13],1.0);
      std::pair<int,int> p23(min(_nodes[1]->tag(),_nodes[2]->tag()),max(_nodes[1]->tag(),_nodes[2]->tag()));
      l23 = _nodes[1]->distance(*_nodes[2])/max(refmap[p23],1.0);
      if(_nodes.size()==3) return max(l12, min(l13, l23));
    }
    if(_nodes.size() >= 4){
      std::pair<int,int> p14(min(_nodes[0]->tag(),_nodes[3]->tag()),max(_nodes[0]->tag(),_nodes[3]->tag()));
      l14 = _nodes[0]->distance(*_nodes[3])/max(refmap[p14],1.0);
      std::pair<int,int> p24(min(_nodes[1]->tag(),_nodes[3]->tag()),max(_nodes[1]->tag(),_nodes[3]->tag()));
      l24 = _nodes[1]->distance(*_nodes[3])/max(refmap[p24],1.0);
      std::pair<int,int> p34(min(_nodes[2]->tag(),_nodes[3]->tag()),max(_nodes[2]->tag(),_nodes[3]->tag()));
      l34 = _nodes[2]->distance(*_nodes[3])/max(refmap[p34],1.0);
      return max(max(l12, max(l13, l23)),max(l14, max(l24, l34)));
    }
  }
  void minRefEdge(std::map< std::pair<int,int>, double> &refmap, double *Psize) const
  {
    double l12,l13,l23,l14,l24,l34;
    if(_nodes.size() >=2){
      std::pair<int,int> p12(min(_nodes[0]->tag(),_nodes[1]->tag()),max(_nodes[0]->tag(),_nodes[1]->tag()));
      l12 = _nodes[0]->distance(*_nodes[1])/max(refmap[p12],1.0);
      Psize[0]=l12;
      Psize[1]=l12;
    }
    if(_nodes.size() >= 3){
      std::pair<int,int> p13(min(_nodes[0]->tag(),_nodes[2]->tag()),max(_nodes[0]->tag(),_nodes[2]->tag()));
      l13 = _nodes[0]->distance(*_nodes[2])/max(refmap[p13],1.0);
      std::pair<int,int> p23(min(_nodes[1]->tag(),_nodes[2]->tag()),max(_nodes[1]->tag(),_nodes[2]->tag()));
      l23 = _nodes[1]->distance(*_nodes[2])/max(refmap[p23],1.0);
      Psize[0]=min(l12,l13);
      Psize[1]=min(l12,l23);
      Psize[2]=min(l13,l23);
    }
    if(_nodes.size() >= 4){
      std::pair<int,int> p14(min(_nodes[0]->tag(),_nodes[3]->tag()),max(_nodes[0]->tag(),_nodes[3]->tag()));
      l14 = _nodes[0]->distance(*_nodes[3])/max(refmap[p14],1.0);
      std::pair<int,int> p24(min(_nodes[1]->tag(),_nodes[3]->tag()),max(_nodes[1]->tag(),_nodes[3]->tag()));
      l24 = _nodes[1]->distance(*_nodes[3])/max(refmap[p24],1.0);
      std::pair<int,int> p34(min(_nodes[2]->tag(),_nodes[3]->tag()),max(_nodes[2]->tag(),_nodes[3]->tag()));
      l34 = _nodes[2]->distance(*_nodes[3])/max(refmap[p34],1.0);
      Psize[0]=min(l14, Psize[0]);
      Psize[1]=min(l24, Psize[1]);
      Psize[2]=min(l34, Psize[2]);
      Psize[3]=min(l14, min(l24,l34));
    }
  }
  double meanRefEdge(std::map< std::pair<int,int>, double> &refmap) const
  {
    double l12,l13,l23,l14,l24,l34;
    if(_nodes.size() >=2){
      std::pair<int,int> p12(min(_nodes[0]->tag(),_nodes[1]->tag()),max(_nodes[0]->tag(),_nodes[1]->tag()));
      l12 = _nodes[0]->distance(*_nodes[1])/max(refmap[p12],1.0);
      if(_nodes.size()==2) return l12;
    }
    if(_nodes.size() >= 3){
      std::pair<int,int> p13(min(_nodes[0]->tag(),_nodes[2]->tag()),max(_nodes[0]->tag(),_nodes[2]->tag()));
      l13 = _nodes[0]->distance(*_nodes[2])/max(refmap[p13],1.0);
      std::pair<int,int> p23(min(_nodes[1]->tag(),_nodes[2]->tag()),max(_nodes[1]->tag(),_nodes[2]->tag()));
      l23 = _nodes[1]->distance(*_nodes[2])/max(refmap[p23],1.0);
      if(_nodes.size()==3) return 3/(1/l12+1/l13+1/l23);
    }
    if(_nodes.size() >= 4){
      std::pair<int,int> p14(min(_nodes[0]->tag(),_nodes[3]->tag()),max(_nodes[0]->tag(),_nodes[3]->tag()));
      l14 = _nodes[0]->distance(*_nodes[3])/max(refmap[p14],1.0);
      std::pair<int,int> p24(min(_nodes[1]->tag(),_nodes[3]->tag()),max(_nodes[1]->tag(),_nodes[3]->tag()));
      l24 = _nodes[1]->distance(*_nodes[3])/max(refmap[p24],1.0);
      std::pair<int,int> p34(min(_nodes[2]->tag(),_nodes[3]->tag()),max(_nodes[2]->tag(),_nodes[3]->tag()));
      l34 = _nodes[2]->distance(*_nodes[3])/max(refmap[p34],1.0);
      return 6/(1/l12+1/l13+1/l23+1/l14+1/l24+1/l34);
    }
  }
  void meanRefEdge(std::map< std::pair<int,int>, double> &refmap, double *Psize) const
  {
    double l12,l13,l23,l14,l24,l34;
    if(_nodes.size() >=2){
      std::pair<int,int> p12(min(_nodes[0]->tag(),_nodes[1]->tag()),max(_nodes[0]->tag(),_nodes[1]->tag()));
      l12 = _nodes[0]->distance(*_nodes[1])/max(refmap[p12],1.0);
      Psize[0]=1/l12;
      Psize[1]=1/l12;
    }
    if(_nodes.size() >= 3){
      std::pair<int,int> p13(min(_nodes[0]->tag(),_nodes[2]->tag()),max(_nodes[0]->tag(),_nodes[2]->tag()));
      l13 = _nodes[0]->distance(*_nodes[2])/max(refmap[p13],1.0);
      std::pair<int,int> p23(min(_nodes[1]->tag(),_nodes[2]->tag()),max(_nodes[1]->tag(),_nodes[2]->tag()));
      l23 = _nodes[1]->distance(*_nodes[2])/max(refmap[p23],1.0);
      Psize[0]+=1/l13;
      Psize[1]+=1/l23;
      Psize[2]=1/l13+1/l23;
    }
    if(_nodes.size() >= 4){
      std::pair<int,int> p14(min(_nodes[0]->tag(),_nodes[3]->tag()),max(_nodes[0]->tag(),_nodes[3]->tag()));
      l14 = _nodes[0]->distance(*_nodes[3])/max(refmap[p14],1.0);
      std::pair<int,int> p24(min(_nodes[1]->tag(),_nodes[3]->tag()),max(_nodes[1]->tag(),_nodes[3]->tag()));
      l24 = _nodes[1]->distance(*_nodes[3])/max(refmap[p24],1.0);
      std::pair<int,int> p34(min(_nodes[2]->tag(),_nodes[3]->tag()),max(_nodes[2]->tag(),_nodes[3]->tag()));
      l34 = _nodes[2]->distance(*_nodes[3])/max(refmap[p34],1.0);
      Psize[0]+=1/l14;
      Psize[1]+=1/l24;
      Psize[2]+=1/l34;
      Psize[3]=1/l14+1/l24+1/l34;
    }
    for(int i=0; i<_nodes.size(); i++) Psize[i]=(_nodes.size()-1)/Psize[i]; 
  }
  void getMat(double mat[3][3]) const
  {
    mat[0][0] = _nodes[1]->x() - _nodes[0]->x();
    mat[0][1] = _nodes[2]->x() - _nodes[0]->x();
    mat[0][2] = _nodes[3]->x() - _nodes[0]->x();
    mat[1][0] = _nodes[1]->y() - _nodes[0]->y();
    mat[1][1] = _nodes[2]->y() - _nodes[0]->y();
    mat[1][2] = _nodes[3]->y() - _nodes[0]->y();
    mat[2][0] = _nodes[1]->z() - _nodes[0]->z();
    mat[2][1] = _nodes[2]->z() - _nodes[0]->z();
    mat[2][2] = _nodes[3]->z() - _nodes[0]->z();
  }
  double meanEdgeSize() const
  {
    double l12,l13,l23,l14,l24,l34;
    if(_nodes.size() >=2){
      std::pair<int,int> p12(min(_nodes[0]->tag(),_nodes[1]->tag()),max(_nodes[0]->tag(),_nodes[1]->tag()));
      l12 = _nodes[0]->distance(*_nodes[1]);
      if(_nodes.size()==2) return l12;
    }
    if(_nodes.size() >= 3){
      std::pair<int,int> p13(min(_nodes[0]->tag(),_nodes[2]->tag()),max(_nodes[0]->tag(),_nodes[2]->tag()));
      l13 = _nodes[0]->distance(*_nodes[2]);
      std::pair<int,int> p23(min(_nodes[1]->tag(),_nodes[2]->tag()),max(_nodes[1]->tag(),_nodes[2]->tag()));
      l23 = _nodes[1]->distance(*_nodes[2]);
      if(_nodes.size()==3) return (l12+l13+l23)/3;
//      if(_nodes.size()==3) return 3/(1/l12+1/l13+1/l23);
//      if(_nodes.size()==3) return sqrt(3/(1/(l12*l12)+1/(l13*l13)+1/(l23*l23)));
    }
    if(_nodes.size() >= 4){
      std::pair<int,int> p14(min(_nodes[0]->tag(),_nodes[3]->tag()),max(_nodes[0]->tag(),_nodes[3]->tag()));
      l14 = _nodes[0]->distance(*_nodes[3]);
      std::pair<int,int> p24(min(_nodes[1]->tag(),_nodes[3]->tag()),max(_nodes[1]->tag(),_nodes[3]->tag()));
      l24 = _nodes[1]->distance(*_nodes[3]);
      std::pair<int,int> p34(min(_nodes[2]->tag(),_nodes[3]->tag()),max(_nodes[2]->tag(),_nodes[3]->tag()));
      l34 = _nodes[2]->distance(*_nodes[3]);
      return (l12+l13+l23+l14+l24+l34)/6;
//      return 6/(1/l12+1/l13+1/l23+1/l14+1/l24+1/l34);
//      return sqrt(6/(1/(l12*l12)+1/(l13*l13)+1/(l23*l23)+1/(l14*l14)+1/(l24*l24)+1/(l34*l34)));
    }
  }
  double meanDihedralEdgeSize(int i) const
  {
    double l12,l13,l23,l14,l24,l34;
    int iV=i+1;
    if(_nodes.size() >= 4){
      if(iV==1||iV==2){
       std::pair<int,int> p12(min(_nodes[0]->tag(),_nodes[1]->tag()),max(_nodes[0]->tag(),_nodes[1]->tag()));
       l12 = _nodes[0]->distance(*_nodes[1]);
      } else l12=0;
      if(iV==1||iV==3){
       std::pair<int,int> p13(min(_nodes[0]->tag(),_nodes[2]->tag()),max(_nodes[0]->tag(),_nodes[2]->tag()));
       l13 = _nodes[0]->distance(*_nodes[2]);
      } else l13=0;
      if(iV==2||iV==3){
       std::pair<int,int> p23(min(_nodes[1]->tag(),_nodes[2]->tag()),max(_nodes[1]->tag(),_nodes[2]->tag()));
       l23 = _nodes[1]->distance(*_nodes[2]);
      } else l23=0;
      if(iV==1||iV==4){
       std::pair<int,int> p14(min(_nodes[0]->tag(),_nodes[3]->tag()),max(_nodes[0]->tag(),_nodes[3]->tag()));
       l14 = _nodes[0]->distance(*_nodes[3]);
      } else l14=0;
      if(iV==2||iV==4){
       std::pair<int,int> p24(min(_nodes[1]->tag(),_nodes[3]->tag()),max(_nodes[1]->tag(),_nodes[3]->tag()));
       l24 = _nodes[1]->distance(*_nodes[3]);
      } else l24=0;
      if(iV==3||iV==4){
       std::pair<int,int> p34(min(_nodes[2]->tag(),_nodes[3]->tag()),max(_nodes[2]->tag(),_nodes[3]->tag()));
       l34 = _nodes[2]->distance(*_nodes[3]);
      } else l34=0;
      return (l12+l13+l23+l14+l24+l34)/3;
    } else return 0;
  }
  double minDihedralEdgeSize(int i) const
  {
    int iV=i+1;
    if(_nodes.size() >= 4){
      double retval=-1;
      if(iV==1||iV==2){
       std::pair<int,int> p12(min(_nodes[0]->tag(),_nodes[1]->tag()),max(_nodes[0]->tag(),_nodes[1]->tag()));
       double l12 = _nodes[0]->distance(*_nodes[1]);
       retval=retval<0 ? l12 : min(retval,l12);
      }
      if(iV==1||iV==3){
       std::pair<int,int> p13(min(_nodes[0]->tag(),_nodes[2]->tag()),max(_nodes[0]->tag(),_nodes[2]->tag()));
       double l13 = _nodes[0]->distance(*_nodes[2]);
       retval=retval<0 ? l13 : min(retval,l13);
      };
      if(iV==2||iV==3){
       std::pair<int,int> p23(min(_nodes[1]->tag(),_nodes[2]->tag()),max(_nodes[1]->tag(),_nodes[2]->tag()));
       double l23 = _nodes[1]->distance(*_nodes[2]);
       retval=retval<0 ? l23 : min(retval,l23);
      }
      if(iV==1||iV==4){
       std::pair<int,int> p14(min(_nodes[0]->tag(),_nodes[3]->tag()),max(_nodes[0]->tag(),_nodes[3]->tag()));
       double l14 = _nodes[0]->distance(*_nodes[3]);
       retval=retval<0 ? l14 : min(retval,l14);
      }
      if(iV==2||iV==4){
       std::pair<int,int> p24(min(_nodes[1]->tag(),_nodes[3]->tag()),max(_nodes[1]->tag(),_nodes[3]->tag()));
       double l24 = _nodes[1]->distance(*_nodes[3]);
       retval=retval<0 ? l24 : min(retval,l24);
      }
      if(iV==3||iV==4){
       std::pair<int,int> p34(min(_nodes[2]->tag(),_nodes[3]->tag()),max(_nodes[2]->tag(),_nodes[3]->tag()));
       double l34 = _nodes[2]->distance(*_nodes[3]);
       retval=retval<0 ? l34 : min(retval,l34);
      }
      return retval;
    } else return 0;
  }
  double maxEdgeSize() const
  {
    double l12,l13,l23,l14,l24,l34;
    if(_nodes.size() >=2){
      std::pair<int,int> p12(min(_nodes[0]->tag(),_nodes[1]->tag()),max(_nodes[0]->tag(),_nodes[1]->tag()));
      l12 = _nodes[0]->distance(*_nodes[1]);
      if(_nodes.size()==2) return l12;
    }
    if(_nodes.size() >= 3){
      std::pair<int,int> p13(min(_nodes[0]->tag(),_nodes[2]->tag()),max(_nodes[0]->tag(),_nodes[2]->tag()));
      l13 = _nodes[0]->distance(*_nodes[2]);
      std::pair<int,int> p23(min(_nodes[1]->tag(),_nodes[2]->tag()),max(_nodes[1]->tag(),_nodes[2]->tag()));
      l23 = _nodes[1]->distance(*_nodes[2]);
      if(_nodes.size()==3) return max(l12,max(l13,l23));
    }
    if(_nodes.size() >= 4){
      std::pair<int,int> p14(min(_nodes[0]->tag(),_nodes[3]->tag()),max(_nodes[0]->tag(),_nodes[3]->tag()));
      l14 = _nodes[0]->distance(*_nodes[3]);
      std::pair<int,int> p24(min(_nodes[1]->tag(),_nodes[3]->tag()),max(_nodes[1]->tag(),_nodes[3]->tag()));
      l24 = _nodes[1]->distance(*_nodes[3]);
      std::pair<int,int> p34(min(_nodes[2]->tag(),_nodes[3]->tag()),max(_nodes[2]->tag(),_nodes[3]->tag()));
      l34 = _nodes[2]->distance(*_nodes[3]);
      return max(l12,max(l13,max(l23,max(l14,max(l24,l34)))));
    }
  }
  double volume()
  {
    if(_nodes.size() < 4) return 0;
    double mat[3][3];
    getMat(mat);
    return det3x3(mat)/6.;
  }
/*
  double meanRefTetSize(std::map< std::size_t, double> &refmap){
     double tetref=refmap[tag()];
     double tetsize=meanEdgeSize()/tetref;
     return tetsize;
  }
  double meanRefTetSize(std::map< std::size_t, double> &refmap){
     double tetref=refmap[tag()];
     double tetsize=pow(volume()*6*sqrt(2.0),1.0/3) /tetref;
     return tetsize;
  }
  double maxRefTetSize(std::map< std::size_t, double> &refmap){
     double tetref=refmap[tag()];
     double tetsize=maxEdgeSize()/tetref;
     return tetsize;
  }
*/
};



static std::string mesher_getEntityName(int dim, int tag)
{
  std::stringstream stream;
  switch(dim) {
  case 0: stream << "Point "; break;
  case 1: stream << "Curve "; break;
  case 2: stream << "Surface "; break;
  case 3: stream << "Volume "; break;
  }
  stream << tag;
  return stream.str();
}


static void
mesher_getEntitiesForElementTypes(GModel *gm, std::map<int, std::vector<GEntity *> > &typeEnt)
{
  std::vector<GEntity *> entities;  gm->getEntities(entities, -1);
  for(std::size_t i = 0; i < entities.size(); i++) {
    GEntity *ge = entities[i];
    switch(ge->dim()) {
    case 0: {
      GVertex *v = static_cast<GVertex *>(ge);
      if(v->points.size())
        typeEnt[v->points.front()->getTypeForMSH()].push_back(ge);
      break;
    }
    case 1: {
      GEdge *e = static_cast<GEdge *>(ge);
      if(e->lines.size())
        typeEnt[e->lines.front()->getTypeForMSH()].push_back(ge);
      break;
    }
    case 2: {
      GFace *f = static_cast<GFace *>(ge);
      if(f->triangles.size())
        typeEnt[f->triangles.front()->getTypeForMSH()].push_back(ge);
      if(f->quadrangles.size())
        typeEnt[f->quadrangles.front()->getTypeForMSH()].push_back(ge);
      break;
    }
    case 3: {
      GRegion *r = static_cast<GRegion *>(ge);
      if(r->tetrahedra.size())
        typeEnt[r->tetrahedra.front()->getTypeForMSH()].push_back(ge);
      if(r->hexahedra.size())
        typeEnt[r->hexahedra.front()->getTypeForMSH()].push_back(ge);
      if(r->prisms.size())
        typeEnt[r->prisms.front()->getTypeForMSH()].push_back(ge);
      if(r->pyramids.size())
        typeEnt[r->pyramids.front()->getTypeForMSH()].push_back(ge);
      break;
    }
    }
  }
}


void mesher_getNodes(GModel *gm, 
	  std::vector<std::size_t> &nodeTags,
          std::vector<double> &coord
	 )
{
  nodeTags.clear();
  coord.clear();
  std::vector<GEntity *> entities;
  gm->getEntities(entities, -1);
  for(std::size_t i = 0; i < entities.size(); i++) {
    GEntity *ge = entities[i];
    for(std::size_t j = 0; j < ge->mesh_vertices.size(); j++) {
      MVertex *v = ge->mesh_vertices[j];
      nodeTags.push_back(v->getNum());
      coord.push_back(v->x());
      coord.push_back(v->y());
      coord.push_back(v->z());
    }
  }
}

void mesher_getElements(GModel *gm, 
  std::vector<int> &elementTypes,
  std::vector<std::vector<std::size_t> > &elementTags,
  std::vector<std::vector<std::size_t> > &nodeTags)
{
  elementTypes.clear();
  elementTags.clear();
  nodeTags.clear();
  std::map<int, std::vector<GEntity *> > typeEnt;
  mesher_getEntitiesForElementTypes(gm, typeEnt);
  for(std::map<int, std::vector<GEntity *> >::const_iterator it = typeEnt.begin(); it != typeEnt.end(); it++) {
    elementTypes.push_back(it->first);
    elementTags.push_back(std::vector<std::size_t>());
    nodeTags.push_back(std::vector<std::size_t>());
    int elementType = it->first;
    for(std::size_t i = 0; i < it->second.size(); i++) {
      GEntity *ge = it->second[i];
      for(std::size_t j = 0; j < ge->getNumMeshElements(); j++) {
        MElement *e = ge->getMeshElement(j);
        if(e->getTypeForMSH() == elementType) {
          elementTags.back().push_back(e->getNum());
          for(std::size_t k = 0; k < e->getNumVertices(); k++) {
            nodeTags.back().push_back(e->getVertex(k)->getNum());
          }
        }
      }
    }
  }
}


class myMesh{
 private:
  std::map<std::size_t, myVertex*> _nodes;
  std::map<std::size_t, myElement*> _elements;
 public:
  myMesh(GModel *gm)
  {
    std::vector<std::size_t> vtags;
    std::vector<double> vxyz; mesher_getNodes(gm, vtags, vxyz);
    std::vector<int> etypes;
    std::vector<std::vector<std::size_t> > etags, evtags;
    mesher_getElements(gm, etypes, etags, evtags);
    for(unsigned int i = 0; i < vtags.size(); i++){
      _nodes[vtags[i]] = new myVertex
        (vtags[i], vxyz[3*i], vxyz[3*i+1], vxyz[3*i+2]);
    }
    for(unsigned int i = 0; i < etypes.size(); i++){
      int nev = evtags[i].size() / etags[i].size();
      for(unsigned int j = 0; j < etags[i].size(); j++){
        std::vector<myVertex*> ev;
        for(unsigned int k = nev * j; k < nev * (j + 1); k++)
          ev.push_back(_nodes[evtags[i][k]]);
        std::vector<double> qx, qy, qz, qdet, qjac;
        _elements[etags[i][j]] = new myElement(etags[i][j], ev);
      }
    }
  }
  const std::map<std::size_t, myVertex*> &nodes() const { return _nodes; }
  const std::map<std::size_t, myElement*> &elements() const { return _elements; }
};




void computeNodeSizeField(const myMesh &mesh,
		      std::map< std::size_t, double> &refmap,
                      std::map<std::size_t, double> &sf_node
		      )
{
  std::map<std::size_t, int> sf_counter;
//  std::map<std::size_t, double> ref_node;
  for(std::map<std::size_t, myElement*>::const_iterator it = mesh.elements().begin(); it != mesh.elements().end(); it++) if(it->second->nodes().size()==4){
//     double tetsize=it->second->meanEdgeSize()/(refmap[it->second->tag()]);
     for (int i=0; i<4; i++){
       assert(refmap[it->second->tag()]>0);
       double tetref=refmap[it->second->tag()];
       double tetsize=it->second->meanDihedralEdgeSize(i)/tetref;
//       double tetsize=it->second->meanDihedralEdgeSize(i);
       int iP=it->second->nodes()[i]->tag();
//       ref_node[iP]=(ref_node.find(iP)==ref_node.end()) ? tetref: max(tetref,ref_node[iP]);
       sf_node[iP]=(sf_node.find(iP)==sf_node.end()) ? tetsize: min(tetsize,sf_node[iP]);
/*
       if(ref_node.find(iP)==ref_node.end()){
         ref_node[iP]=tetref; sf_node[iP]=tetsize;
       } else {
         ref_node[iP]+=tetref; sf_node[iP]+=tetsize;
       }
*/
/*
       if(sf_counter.find(iP)==sf_counter.end()){
         sf_counter[iP]=1; sf_node[iP]=tetsize/tetref;
       } else {
         sf_counter[iP]+=1; sf_node[iP]+=tetsize/tetref;
       }
*/
     }
  }
//  for(std::map<std::size_t, int>::const_iterator it = sf_counter.begin(); it != sf_counter.end(); it++)  sf_node[it->first]/=it->second;
//  for(std::map<std::size_t, double>::const_iterator it = ref_node.begin(); it != ref_node.end(); it++)  sf_node[it->first]/=(it->second);
}


void computeNodeSizeField2(const myMesh &mesh,
		      std::map< std::size_t, double> &refmap,
                      std::map<std::size_t, double> &sf_node
		      )
{
  std::map<std::size_t, int> nodeTetCounter;
  for(std::map<std::size_t, myElement*>::const_iterator it = mesh.elements().begin(); it != mesh.elements().end(); it++) if(it->second->nodes().size()==4){
     double tetvol=it->second->volume();
     for (int i=0; i<4; i++){
       double tetsize=it->second->meanDihedralEdgeSize(i)/(refmap[it->second->tag()]);
       int iP=it->second->nodes()[i]->tag();
       if(sf_node.find(iP)==sf_node.end()){ sf_node[iP]=tetsize; nodeTetCounter[iP]=1;}
       else                               { sf_node[iP]+=tetsize; nodeTetCounter[iP]++;}
     }
  }
  for(std::map<std::size_t, double>::const_iterator it = sf_node.begin(); it != sf_node.end(); it++){
	 int iP=it->first;
	 sf_node[iP]=sf_node[iP]/nodeTetCounter[iP];
  }
}


void computeElementSizeField1(const myMesh &mesh,
                      std::map<std::size_t, double> &sf_ele, 
		      std::map< std::pair<int,int>, double> &refmap)
{
  for(std::map<std::size_t, myElement*>::const_iterator it = mesh.elements().begin(); it != mesh.elements().end(); it++) 
	  if(it->second->nodesNum()==4) sf_ele[it->first] = it->second->meanRefEdge(refmap);
}


void computeElementSizeField(const myMesh &mesh,
                      std::map<std::size_t, double> &sf_ele, 
		      std::map<std::size_t, double> &refmap)
{
  for(std::map<std::size_t, myElement*>::const_iterator it = mesh.elements().begin(); it != mesh.elements().end(); it++) 
	  if(it->second->nodesNum()==4) sf_ele[it->first] = it->second->meanEdgeSize()/(refmap[it->second->tag()]);
}

void getKeysValues(const std::map<std::size_t, double> &f,
                   std::vector<std::size_t> &keys,
                   std::vector<std::vector<double> > &values)
{
  keys.clear();
  values.clear();
  for(std::map<std::size_t, double>::const_iterator it = f.begin();
      it != f.end(); it++){
    keys.push_back(it->first);
    values.push_back(std::vector<double>(1, it->second));
  }
}

void read_meshref1(std::string &mshrefFilePath, std::map< std::pair<int,int>, double> &refmap)
{

 FILE *f_ref;
 if(!(f_ref = fopen(mshrefFilePath.c_str(), "r")))  std::cout<< "Cannot open file "<< mshrefFilePath.c_str() << std::endl;

 int ne; fscanf(f_ref , "%d\n", &ne);
 for(int i=0; i<ne; i++){
  int IP1,IP2;
  double ref;
  fscanf(f_ref , "%d  %d  %lf\n", &IP1, &IP2, &ref);
  std::pair<int,int> p(IP1,IP2);
  refmap[p]=ref;
 }

 fclose(f_ref);
}



void read_meshref(std::string &mshrefFilePath, std::map<std::size_t, double> &refmap)
{

 FILE *f_ref;
 if(!(f_ref = fopen(mshrefFilePath.c_str(), "r")))  std::cout<< "Cannot open file "<< mshrefFilePath.c_str() << std::endl;

 int ntet; fscanf(f_ref , "%d\n", &ntet);
 for(int i=0; i<ntet; i++){
  int Itet;
  double ref;
  fscanf(f_ref , "%d  %lf\n", &Itet, &ref);
  refmap[Itet]=ref;
 }

 fclose(f_ref);
}


int mesher_setMeshSizeView(GModel *gm0, std::string mshFilePath)
{
  gm0->readMSH(mshFilePath+".msh");
  myMesh mesh(gm0);
  
  std::string mshrefFilePath=mshFilePath+".mshref";
//  std::map< std::pair<int,int>, double> refmap;
  std::map<std::size_t, double> refmap;
  read_meshref(mshrefFilePath,refmap);

  std::vector<std::size_t> keys;
  std::vector<std::vector<double> > values;

  int nodeData=1;

  if(nodeData) {
   std::map<std::size_t, double> sf_node;
   computeNodeSizeField(mesh, refmap, sf_node);
   getKeysValues(sf_node, keys, values);
  } else {
   std::map<std::size_t, double> sf_ele;
   computeElementSizeField(mesh, sf_ele,refmap);
   getKeysValues(sf_ele, keys, values);
  }

/*
  PViewDataList *d=new PViewDataList();
  d->setName("mesh size field");
*/

  int viewTag = gmsh::view::add("mesh size");
  
  if(nodeData)
   gmsh::view::addModelData(viewTag, 0, "backgroundmesh", "NodeData", keys, values);
  else
   gmsh::view::addModelData(viewTag, 0, "backgroundmesh", "ElementData", keys, values);

/*
  const std::vector<double> listdata;
  int nelem=keys.size();
  for(int i=0; i<nelem ; i++){
    int etag=keys[i];
    for(int j=0; j<mesh.elements()[etag]->nodesNum(); j++) listdata.push_back(mesh.elements()[etag]->nodes()[j]->x());
    for(int j=0; j<mesh.elements()[etag]->nodesNum(); j++) listdata.push_back(mesh.elements()[etag]->nodes()[j]->y());
    for(int j=0; j<mesh.elements()[etag]->nodesNum(); j++) listdata.push_back(mesh.elements()[etag]->nodes()[j]->z());
    listdata.insert(listdata.end(),values[i].begin(),values[i].end());
  }
  gmsh::view::addListData(viewTag, "SS", nelem, listdata);
*/

  int dumpfiles=1;
  if(dumpfiles) gmsh::view::write(viewTag, mshFilePath+"_size.msh");
  return viewTag;

/*
  PViewDataGModel::DataType type;
  type=PViewDataGModel::ElementData;
  PViewDataGModel *dg = new PViewDataGModel(type);
  dg->addData(gm0, keys, values, 0, 0, 0, -1);
  dg->setName("mesh size field");

  std::string posFilePath=mshFilePath+"sz";
  bool binary=false;
  bool parsed=true;
  bool append=false;
  dg->writePOS(posFilePath,binary,parsed,append);

  delete dg;
*/
  
/*
  std::vector<std::vector<double> > dvec;
  dg->toVector(dvec);
  delete dg;
*/
  


}

void mesher_setMeshSizeField(GModel *gm, int meshSizeViewTag, std::list<int> &mshFieldList)
{

  FieldManager *fields = gm->getFields();
//  int fieldTag=mshFieldList.size()+2;
  int fieldTag=1;
  Field *size_f=fields->newField(fieldTag, "PostView");
  size_f->options["ViewTag"]->numericalValue(meshSizeViewTag);
//  size_f->options["IView"]->numericalValue(PView::list.size()-1);
  mshFieldList.push_back(fieldTag);
//  fields->setBackgroundFieldId(fieldTag);

}

double singularEdgeMeshRefinement[2]={4,6};

void mesher_setEdgeMeshAttribute(GModel *gm, bool mesh3D, 
	            TopTools_IndexedMapOfShape *indexedEdges,
		    double meshsize,  EdgeData  *edgeData, 
		    std::list<int> &mshFieldList,
		    int useEdgeAttractor)
{
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
	      if(mesh3D) if (edgeData[EI-1].singular){
	          int singmshrefI=10*edgeRef*singularEdgeMeshRefinement[edgeData[EI-1].singular-1]+0.2;
		  singElist[singmshrefI].push_back(ge->tag());
	      }
	      ge->meshAttributes.meshSize=min(ge->meshAttributes.meshSize,emeshSize);
             /*	      
	      GVertex * v1=ge->getBeginVertex();
	      GVertex * v2=ge->getEndVertex();
	      v1->setPrescribedMeshSizeAtVertex(min(v1->prescribedMeshSizeAtVertex(),ge->meshAttributes.meshSize));
	      v2->setPrescribedMeshSizeAtVertex(min(v2->prescribedMeshSizeAtVertex(),ge->meshAttributes.meshSize));
	     */
      }
      if(!useEdgeAttractor) return;
      typedef std::map<int, std::list<int> >::const_iterator ElistIt;
      if(mesh_aniso)   
//      if(true)   
	      for (ElistIt it=singElist.begin(); it!= singElist.end(); it++){
	  int mshrefI=(*it).first;
	  double emeshSize=10*meshsize/mshrefI;
	  int fieldTag=fields->newId(); mshFieldList.push_back(fieldTag);
	  {
             Field *attractor=fields->newField(fieldTag, "AttractorAnisoCurve");
             attractor->options["EdgesList"]->list((*it).second);
             attractor->options["NNodesByEdge"]->numericalValue(40);
             attractor->options["dMin"]->numericalValue(emeshSize*0.1);
             attractor->options["dMax"]->numericalValue(meshsize*3.0);
             attractor->options["lMinTangent"]->numericalValue(emeshSize/2.0);
             attractor->options["lMinNormal"]->numericalValue(emeshSize/2.0);
             attractor->options["lMaxTangent"]->numericalValue(meshsize);
             attractor->options["lMaxNormal"]->numericalValue(meshsize);
	  }
      }
      else for (ElistIt it=singElist.begin(); it!= singElist.end(); it++){
	  int mshrefI=(*it).first;
	  double emeshSize=10*meshsize/mshrefI;
	  {
	     int fieldTag1=fields->newId();
             Field *distance=fields->newField(fieldTag1, "Distance");
             distance->options["EdgesList"]->list((*it).second);
             distance->options["Sampling"]->numericalValue(50);
	     int fieldTag=fields->newId(); mshFieldList.push_back(fieldTag);
             Field *thsld=fields->newField(fieldTag, "Threshold");
             thsld->options["IField"]->numericalValue(fieldTag1);
             thsld->options["DistMin"]->numericalValue(emeshSize);
             thsld->options["DistMax"]->numericalValue(meshsize/2);
             thsld->options["SizeMin"]->numericalValue(emeshSize);
             thsld->options["SizeMax"]->numericalValue(meshsize);
	  }
      }

}




void mesher_setBackgroundField(GModel *gm, std::list<int> &mshFieldList){
      if(mshFieldList.size()>0){
          FieldManager *fields = gm->getFields();
          int fieldTag;
          if(mshFieldList.size()>1){
           fieldTag=fields->newId();
//           Field *bkgfield= fields->newField(fieldTag, "MinAniso");
           Field *bkgfield= mesh_aniso ?  fields->newField(fieldTag, "MinAniso") : fields->newField(fieldTag, "Min");
           bkgfield->options["FieldsList"]->list(mshFieldList);
	  } else fieldTag=*(mshFieldList.begin());
          fields->setBackgroundFieldId(fieldTag);
      }
}

void mesher_setRegionMeshAttribute(GModel *gm, MwOCAF* ocaf, 
		    double meshsize,
		    std::map< int, std::string > &solidNames
	      )
{
      for(GModel::riter it = gm->firstRegion(); it != gm->lastRegion(); ++it) {
              GRegion *gr=*it;
              TopoDS_Shape S=* (TopoDS_Shape *) gr->getNativePtr();
              int VI=ocaf->indexedSolids->FindIndex(S);
	      if(VI<1) continue;
	      DB::Volume *vol=ocaf->EmP->FindVolume(solidNames[VI].c_str());
	      if(vol==NULL) continue;
              DB::Material* mat= ocaf->EmP->FindMaterial(vol->material);
	      if(mat==NULL) continue;
	      double refri=sqrt(mat->epsr * mat->mur);
	      gr->meshAttributes.meshSize = meshsize/(refri*vol->meshRefinement);
      }
}


void mesher_setFaceMeshAttribute(GModel *gm,
	            TopTools_IndexedMapOfShape *indexedFaces,
		    double meshsize, double sharedMeshsize, FaceData  *faceData,
		    bool onlyOnWG=false
	      )
{
      for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
              GFace *gf=*fit;
              TopoDS_Shape F=* (TopoDS_Shape *) gf->getNativePtr();
              int FI=indexedFaces->FindIndex(F);
	      if(onlyOnWG && !faceData[FI-1].Shared()) continue;
	      double refri=sqrt(faceData[FI-1].epsr * faceData[FI-1].mur);
	      gf->meshAttributes.meshSize = meshsize/(refri*faceData[FI-1].meshref);
	      if(faceData[FI-1].cmp1!=faceData[FI-1].cmp2){
		     if(sharedMeshsize>0) gf->meshAttributes.meshSize=MIN(gf->meshAttributes.meshSize,sharedMeshsize/refri);
		     std::vector<GEdge*> Fedges=gf->edges();
                     for (std::vector<GEdge*>::const_iterator eit=Fedges.begin(); eit!=Fedges.end(); ++eit){
                       GEdge *ge=*eit;
		       ge->meshAttributes.meshSize=min(ge->meshAttributes.meshSize,gf->meshAttributes.meshSize);
		       /*
	               GVertex * v1=ge->getBeginVertex();
	               GVertex * v2=ge->getEndVertex();
	               v1->setPrescribedMeshSizeAtVertex(min(v1->prescribedMeshSizeAtVertex(),ge->meshAttributes.meshSize));
	               v2->setPrescribedMeshSizeAtVertex(min(v2->prescribedMeshSizeAtVertex(),ge->meshAttributes.meshSize));
		       */
                     }
	      }
      }
}


void mesher_setFaceMeshAttribute(GModel *gm,
	            TopTools_IndexedMapOfShape *indexedFaces,
		    double meshsize, double sharedMeshsize, FaceData  *faceData,
		    std::list<int> &mshFieldList)
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
	   int mshrefI=(*it).first;
	   double fmeshSize=10*meshsize/mshrefI;
	   int fieldTag1=fields->newId(); 
           Field *attractor=fields->newField(fieldTag1, "Attractor");
           attractor->options["FacesList"]->list((*it).second);
	   int fieldTag=fields->newId(); mshFieldList.push_back(fieldTag);
           Field *thsld=fields->newField(fieldTag, "Threshold");
           thsld->options["IField"]->numericalValue(fieldTag1);
           thsld->options["DistMin"]->numericalValue(0);
           thsld->options["DistMax"]->numericalValue(meshsize);
           thsld->options["LcMin"]->numericalValue(fmeshSize);
           thsld->options["LcMax"]->numericalValue(meshsize);
      }
}


bool isGridFace(GFace *gf)
{
   std::vector<GEdge*>   Fedges=gf->edges();
   std::set<GEdge*> edgeSet;
   for (std::vector<GEdge*>::const_iterator eit=Fedges.begin(); eit!=Fedges.end(); ++eit){
     GEdge *ge=*eit;
     edgeSet.insert(ge);
   }
   int coupledSize=0;
   for (std::vector<GEdge*>::const_iterator eit=Fedges.begin(); eit!=Fedges.end(); ++eit){
     GEdge *ge=*eit;
     GEdge *gem = dynamic_cast<GEdge*> (ge->getMeshMaster());
     if (gem != ge) if(edgeSet.find(gem)!=edgeSet.end()) coupledSize+=2;
   }
   return coupledSize==edgeSet.size();
}

void laplaceSmoothing(GFace *gf);



void writeIFmap(GModel *gm,  MwOCAF* ocaf, const char* dirName){

  std::string IFfileName=std::string(dirName)+std::string("/interfaces/IF.map");
  FILE *fIF=NULL;
  std::set<int> FIset;
  {
   for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
    GFace *gf=*fit;
    TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
    F.Orientation(TopAbs_FORWARD);
    int FI=ocaf->indexedFaces->FindIndex(F);
    if(!FI) continue;
    if(ocaf->faceData[FI-1].sfname==std::string("-")) continue;
    FIset.insert(FI);
   }
   int IFfaceNum=FIset.size();
   fIF=fopen(nativePath(IFfileName).c_str(), "w");
   fprintf(fIF, "%d  faceNum \n", IFfaceNum );
  }
  FIset.clear();

  for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
    GFace *gf=*fit;
    TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
    int Ssign=(F.Orientation()==TopAbs_FORWARD) ? 1 : -1;
    F.Orientation(TopAbs_FORWARD);
    int FI=ocaf->indexedFaces->FindIndex(F);
    if(!FI) continue;
    if(ocaf->faceData[FI-1].sfname!=std::string("-") ){
	if(FIset.find(FI)==FIset.end()) {fprintf(fIF, "%d  %s\n", FI, ocaf->faceData[FI-1].sfname.c_str()); FIset.insert(FI);}
    }
  }
  fclose(fIF);
}


void MESHER::meshModel(MwOCAF* ocaf, bool meshIF, bool mesh3D, bool meshWG, double meshsize, double sharedMeshsize, int meshpercircle, const char* dirName, const char* modelDir)
{
    bool CHECK=false;

    if(mesh3D || meshWG) ocaf->makeFaceAdjCells();
//    ocaf->makeTheCompound();


    TCollection_AsciiString assName; ocaf->getAssName(assName);
    std::string mshFilePath=std::string(modelDir)+"/"+std::string(assName.ToCString());
    mshFilePath=nativePath(mshFilePath);

//------------------

    BRep_Builder builder;
    TopoDS_Compound     theShapes;
    builder.MakeCompound(theShapes);

#if defined(GMSH3D)
    for (int VI=1; VI<=ocaf->indexedSolids->Extent(); VI++){
      TopoDS_Shape V = ocaf->indexedSolids->FindKey(VI);
      if(!V.IsNull()) builder.Add(theShapes,V);
    }
#endif

    for (int FI=1; FI<=ocaf->indexedFaces->Extent(); FI++){
      TopoDS_Shape F = ocaf->indexedFaces->FindKey(FI);
      if(!F.IsNull()) builder.Add(theShapes,F);
    }

    bool print_brep=false;
    if(print_brep){
      char brepFileName[256];
      strcpy(brepFileName, "MODEL.brep");
      BRepTools::Write(theShapes, brepFileName);
    }


    std::vector<int> Fmaster;

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
//      GmshSetOption("Mesh", "Algorithm", ALGO_2D_DELAUNAY);
//      GmshSetOption("Mesh", "Algorithm", ALGO_2D_FRONTAL);


      gmsh::initialize();
      Msg::SetVerbosity(0);
      CTX::instance()->geom.tolerance=1.e-6;


      std::string mshrefFilePath=mshFilePath+".mshref";
      int meshSizeViewTag=-1;
      if(FileExists(mshrefFilePath.c_str())){
        gmsh::model::add("backgroundmesh");
        gmsh::model::setCurrent("backgroundmesh");
        GModel *gm0=GModel::current();
        gm0->importOCCShape(&theShapes);
        meshSizeViewTag=mesher_setMeshSizeView(gm0, mshFilePath);
      }
      gmsh::model::add(assName.ToCString());
      gmsh::model::setCurrent(assName.ToCString());
      GModel *gm=GModel::current();
      std::list<int> mshFieldList;
      FieldManager *fields = gm->getFields(); fields->reset();
      gm->importOCCShape(&theShapes);
//      setTags is deactivated because, by breaking the ordering of edges structure (a std::set),
//      it may lead to a crash of gmsh.
//      mesher_setTags(gm, ocaf->indexedFaces, ocaf->indexedEdges, ocaf->indexedVertices);
//

      std::map< int, std::string > solidNames;
      if(mesh3D){
      int SI=0;
      for (TDF_ChildIterator it(ocaf->theParts,Standard_False); it.More(); it.Next()) {
        TDF_Label label1 = it.Value();
        Handle(TDF_Reference)  refAtt;
        if(!label1.FindAttribute(TDF_Reference::GetID(),refAtt)) continue;
        TDF_Label label = refAtt->Get();
        TopoDS_Shape S = ocaf->shapeTool->GetShape(label);
        if(!S.IsNull()){
           DB::Volume *vol=ocaf->getLabelVol(label);
	   if(S.ShapeType()==TopAbs_SOLID){
              int VI=ocaf->indexedSolids->FindIndex(S);
	      solidNames[VI]=std::string(vol->name);
	   } else if(S.ShapeType()==TopAbs_COMPSOLID || S.ShapeType()==TopAbs_COMPOUND) for (TopExp_Explorer exp(S,TopAbs_SOLID); exp.More(); exp.Next()){ 
	      TopoDS_Shape Si=exp.Current();
	      if(Si.ShapeType()==TopAbs_SOLID){
                  int VIi=ocaf->indexedSolids->FindIndex(Si);
	          solidNames[VIi]=std::string(vol->name);
	      }
           }
        }
      }
      }
//
      std::map< int, GEdge * > indexedGmshEdges;
      for(GModel::eiter eit = gm->firstEdge(); eit != gm->lastEdge(); ++eit){
         GEdge *ge=*eit;
	 assert(ge->getNativeType()==GEntity::OpenCascadeModel);
//         OCCEdge *occe= dynamic_cast<OCCEdge*> (ge);
         TopoDS_Edge E=* (TopoDS_Edge *) ge->getNativePtr();
         int EI=ocaf->indexedEdges->FindIndex(E);
         assert(EI);
         if(!EI) continue;
	 indexedGmshEdges[EI]=ge;
      }

#if defined(GMSH3D)
//      if(!mesh3D) meshsize_=meshsize*0.7;
#endif
      if(mesh3D)  mesher_setSingularEdges(gm, ocaf);
      if(meshSizeViewTag<0){
         mesher_setEdgeMeshAttribute(gm, mesh3D, ocaf->indexedEdges, 0.9*meshsize, ocaf->edgeData,mshFieldList, useEdgeAttractor);
//       mesher_setGlobalAniso(gm, ocaf->indexedEdges, meshsize, 1.0, 1.0, 2.0, fieldNum, mshFieldList);
//         if(!mesh3D)  
		 mesher_setSingularVertices(gm, ocaf, meshsize, mshFieldList);
      }
      if(useFaceAttractor)
         mesher_setFaceMeshAttribute(gm, ocaf->indexedFaces, 0.9*meshsize, sharedMeshsize, ocaf->faceData, mshFieldList);
      else{
//	 bool onlyOnWG=meshSizeViewTag>=0;
	 bool onlyOnWG=false;
         mesher_setFaceMeshAttribute(gm, ocaf->indexedFaces, 0.9*meshsize, sharedMeshsize, ocaf->faceData, onlyOnWG);
      }
      if(mesh3D) mesher_setRegionMeshAttribute(gm, ocaf, meshsize, solidNames);

      for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
         GFace *gf=*fit;
	 TopoDS_Shape F=* (TopoDS_Face *) gf->getNativePtr();
         int FI=ocaf->indexedFaces->FindIndex(F);
         if(mesh3D){
//           gf->meshAttributes.method= isGridFace(gf) ? MESH_TRANSFINITE: MESH_UNSTRUCTURED;
           gf->meshAttributes.method=MESH_UNSTRUCTURED;
//	   if(gf->geomType()==GEntity::Plane) gf->setMeshingAlgo(ALGO_2D_MESHADAPT);
//	   else                               gf->setMeshingAlgo(ALGO_2D_MESHADAPT);
	 } else if(ocaf->faceData[FI-1].cmp1!=ocaf->faceData[FI-1].cmp2 
		&& ocaf->faceData[FI-1].level>=ocaf->EmP->level
		&& ocaf->faceData[FI-1].cmp1!=std::string("-")
		&& ocaf->faceData[FI-1].cmp2!=std::string("-")
		)
           gf->meshAttributes.method=MESH_UNSTRUCTURED;
	 else
           gf->meshAttributes.method=MESH_NONE;
      }
      if(mesh_aniso)  
	 CTX::instance()->mesh.algo2d=ALGO_2D_BAMG;
//         CTX::instance()->mesh.algo2d=ALGO_2D_MESHADAPT;
      else            
//        CTX::instance()->mesh.algo2d=(meshSizeViewTag>=0)? ALGO_2D_DELAUNAY : ALGO_2D_FRONTAL;
//        CTX::instance()->mesh.algo2d=(meshSizeViewTag>=0)? ALGO_2D_DELAUNAY : ALGO_2D_MESHADAPT;
//        if(meshIF)  CTX::instance()->mesh.algo2d=conformalMeshIF? ALGO_2D_DELAUNAY:ALGO_2D_MESHADAPT;
//        else        CTX::instance()->mesh.algo2d=conformalMeshIF? ALGO_2D_DELAUNAY:ALGO_2D_MESHADAPT;
      CTX::instance()->mesh.algo2d=ALGO_2D_DELAUNAY;
      CTX::instance()->mesh.lcFromPoints=1;
      CTX::instance()->mesh.lcFromCurvature=1;
      CTX::instance()->mesh.lcExtendFromBoundary=0;
      if(meshIF){
        CTX::instance()->mesh.minCircleNodes=meshpercircle;
        CTX::instance()->mesh.lcFromCurvature=meshpercircle;
      }else{
        CTX::instance()->mesh.minCircleNodes=meshpercircle;
        CTX::instance()->mesh.lcFromCurvature=meshpercircle;
      }
      CTX::instance()->mesh.anisoMax=10000;
//      CTX::instance()->mesh.refineSteps=2;
//      CTX::instance()->mesh.nbSmoothing=(meshSizeViewTag>=0)? 0 : 2;
      CTX::instance()->mesh.nbSmoothing=4;
      CTX::instance()->mesh.smoothRatio=1.5;
      CTX::instance()->mesh.lcIntegrationPrecision=1.e-3;
      CTX::instance()->mesh.lcMax=meshsize;
      CTX::instance()->mesh.lcMin=meshsize/100;

#if defined(GMSH3D)
      if(mesh_aniso)
            CTX::instance()->mesh.algo3d=ALGO_3D_DELAUNAY;
//            CTX::instance()->mesh.algo3d=ALGO_3D_FRONTAL;
//	    CTX::instance()->mesh.algo3d=ALGO_3D_MMG3D;
      else            
            CTX::instance()->mesh.algo3d=ALGO_3D_DELAUNAY;
//            CTX::instance()->mesh.algo3d= (meshSizeViewTag>=0)? ALGO_3D_DELAUNAY : ALGO_3D_FRONTAL;
//          CTX::instance()->mesh.algo3d=ALGO_3D_HXT;
//          CTX::instance()->mesh.algo3d=ALGO_3D_RTREE;
      CTX::instance()->mesh.optimizeThreshold=0.6;
      CTX::instance()->mesh.optimize=5;
//      CTX::instance()->mesh.optimizeNetgen=(meshSizeViewTag>=0)? 1 : 2;
      CTX::instance()->mesh.optimizeNetgen=0;
#endif

/*
      GmshSetOption("Mesh", "Algorithm", (double) ALGO_2D_BAMG);
      GmshSetOption("Mesh", "lcFromPoints", 1.0);
      GmshSetOption("Mesh", "lcFromCurvature", 1.0);
      GmshSetOption("Mesh", "minCircPoints", 10.0);
      GmshSetOption("Mesh", "anisoMax", 100.0);
*/
//      CTX::instance()->mesh.smoothRatio=0.0;
//

      if(meshSizeViewTag>=0){
	 mesher_setMeshSizeField(gm, meshSizeViewTag, mshFieldList);
         CTX::instance()->mesh.lcFromPoints=0;
//         CTX::instance()->mesh.lcExtendFromBoundary=0;
      }
    

      mesher_setBackgroundField(gm, mshFieldList);

      std::map< int, GFace * > indexedGmshFaces;
      for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
         GFace *gf=*fit;
         TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
         int FI=ocaf->indexedFaces->FindIndex(F);
         assert(FI);
         if(!FI) continue;
	 indexedGmshFaces[FI]=gf;
      }



     if(conformalMeshIF) MWM::newMesh();


     if(conformalMeshIF) if(ocaf->EmP->level>0 || ocaf->subComp) for(GModel::eiter it = gm->firstEdge(); it != gm->lastEdge(); it++){
        GEdge *ge=(*it);
        unsigned int ElineNum=ge->getNumMeshElements();
        TopoDS_Edge E=* (TopoDS_Edge *) ge->getNativePtr();
        int EI=ocaf->indexedEdges->FindIndex(E);
        if(ge->meshAttributes.method!=MESH_NONE){
	   std::string edgeFileName=dirName;
	   int UEI=ocaf->subComp? ocaf->subSplitEdgesMap[EI-1] : EI;
           char ename[50]; sprintf(ename,"E%d.mwm",UEI);
           edgeFileName+="/interfaces/";
           edgeFileName+=ename;
	   if(!FileExists(nativePath(edgeFileName).c_str())) continue;
	   loadMwm(nativePath(edgeFileName).c_str());
           GVertex *gv1=ge->getBeginVertex();
           GVertex *gv2=ge->getEndVertex();
           TopoDS_Vertex V1= *(TopoDS_Vertex *) gv1->getNativePtr();
           int IP1=ocaf->indexedVertices->FindIndex(V1);
           TopoDS_Vertex V2= *(TopoDS_Vertex *) gv2->getNativePtr();
           int IP2=ocaf->indexedVertices->FindIndex(V2);
           int GMSH_Esign=ocaf->vertexData[IP1-1].name < ocaf->vertexData[IP2-1].name ? 1:-1;
	   MWM::Curve *mwmc=MWM::mesh->FindCurve(ocaf->edgeData[EI-1].name.c_str());
           if(mwmc) mwmc->setGEdge(ge,GMSH_Esign);
	}
     }
     

      if(conformalMeshIF) if(mesh3D) for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
         GFace *gf=*fit;
	 if(gf->meshAttributes.method!=MESH_NONE){
           TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
           int Ssign=(F.Orientation()==TopAbs_FORWARD) ? 1 : -1;
           F.Orientation(TopAbs_FORWARD);
           int FI=ocaf->indexedFaces->FindIndex(F);
           if(!FI) continue;
	   std::string faceFileName=dirName;
	   int UFI=ocaf->subComp? ocaf->subSplitFacesMap[FI-1] : FI;
           char fname[50]; sprintf(fname,"F%d.mwm",UFI);
           faceFileName+="/interfaces/";
           faceFileName+=fname;
	   if(!FileExists(nativePath(faceFileName).c_str())) continue;
	   loadMwm(nativePath(faceFileName).c_str());
	   std::string upFaceName="UF"+ocaf->faceData[FI-1].name;
	   MWM::Surface *mwms=MWM::mesh->FindSurface(upFaceName.c_str());
           if(mwms) mwms->setGFace(gf); 
	 }
      }

      if(conformalMeshIF){
       if(mesh3D)                                         MWM::setMasterMesh(2,gm);
       else if(ocaf->EmP->level>0 || ocaf->subComp)       MWM::setMasterMesh(1,gm);
      }

      gm->mesh(1);
      gm->mesh(2);


      bool checkMesh2D=false;
      if(conformalMeshIF) if(checkMesh2D){
	 if(mesh3D) for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
            GFace *gf=*fit;
	    if(gf->meshAttributes.method!=MESH_NONE){
               TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
               int Ssign=(F.Orientation()==TopAbs_FORWARD) ? 1 : -1;
               F.Orientation(TopAbs_FORWARD);
               int FI=ocaf->indexedFaces->FindIndex(F);
               if(!FI) continue;
	       std::string upFaceName="UF"+ocaf->faceData[FI-1].name;
	       MWM::Surface *mwms=MWM::mesh->FindSurface(upFaceName.c_str());
               if(mwms) mwms->checkMasterMesh();
            }
	 }
         gm->writeMSH(mshFilePath+".msh");
	 if(meshIF){
             print_mwm(gm, ocaf, meshIF, mesh3D, meshWG,
		dirName, modelDir,
		solidNames,
		Fmaster
	     );
         }
         if(ocaf->EmP->assemblyType==COMPONENT && !ocaf->subComp) writeIFmap(gm, ocaf, dirName);
         gmsh::finalize();
	 return;
      }


#if defined(GMSH3D)
//      if(meshSizeViewTag>=0) CTX::instance()->mesh.nbSmoothing=0;
      CTX::instance()->mesh.lcExtendFromBoundary=(meshSizeViewTag>=0)? 0 : 1;
      if(mesh3D) gm->mesh(3);
#endif


//      GmshWriteFile(mshFilePath);

      gm->writeMSH(mshFilePath+".msh");

      if(ocaf->EmP->assemblyType==COMPONENT && !ocaf->subComp) writeIFmap(gm, ocaf, dirName);

      if(mesh3D) addIF(ocaf, dirName, modelDir);
      
      print_mwm(gm, ocaf, meshIF, mesh3D, meshWG,
		dirName, modelDir,
		solidNames,
		Fmaster
	       );

      if(mesh3D) for(GModel::riter it = gm->firstRegion(); it != gm->lastRegion(); ++it)  {
        GRegion *gr=*it;
        TopoDS_Solid V=* (TopoDS_Solid *) gr->getNativePtr();
        int VI=ocaf->indexedSolids->FindIndex(V);
        assert(VI);
        assert((*it)->tetrahedra.size()>0);
        for(unsigned int i = 0; i < (*it)->tetrahedra.size(); i++){
 	  MElement *t=(*it)->tetrahedra[i];
          if(t->gammaShapeMeasure()<1.e-5){
             fprintf(stderr, "Bad tetrahedra.\n");
	     exit(13);
          }
        }
      }
      
      gmsh::finalize();

}


void MESHER::addIF(MwOCAF* ocaf, const char* dirName, const char* modelDir)
{

    ocaf->readSuperfaceSplitterMap();

    TCollection_AsciiString assName; ocaf->getAssName(assName);
    TCollection_AsciiString modelFileName=TCollection_AsciiString(modelDir)+"/"+assName+".mwm";

    FILE *fout=fopen(nativePath(modelFileName.ToCString()).c_str(), "w"); if(!fout) return;

    ocaf->regenerateIndexedSubShapes();
    ocaf->readFEproperties();

    std::map<std::string,std::vector<int>, std::less<std::string> > WGIF_FI;
    std::string IFfileName=std::string(dirName)+std::string("/interfaces/IF.map");
    FILE *fIFmap=fopen(nativePath(IFfileName).c_str(), "r");
    int UFNum; char cstr[100];
    fscanf(fIFmap, "%d", &UFNum);  fgets(cstr,100,fIFmap);
    int FI; char str[100];
    for (int i =0; i < UFNum; i++){ fscanf(fIFmap, "%d %s", &FI, str); WGIF_FI[std::string(str)].push_back(FI);}
    fclose(fIFmap);

    std::set<std::string> WGIF;
    typedef std::set<std::string>::const_iterator WGIFIt;

    for (int FI=1; FI<=ocaf->indexedFaces->Extent(); FI++)  
	if(ocaf->faceData[FI-1].Shared())
            if(WGIF.find(ocaf->faceData[FI-1].sfname)==WGIF.end()) WGIF.insert(ocaf->faceData[FI-1].sfname);
    for (WGIFIt it=WGIF.begin(); it!= WGIF.end(); it++){
       std::string sfname=(*it);
       fprintf(fout, "DEF %s  MWM_Volume {\n",  sfname.c_str());
       fprintf(fout, "  type  WaveGuide\n");
       std::map<std::string,std::string>::const_iterator sfit=ocaf->superfaceSplitterMap.find(sfname);
       if(sfit!=ocaf->superfaceSplitterMap.end()){
         DB::Volume *vol=ocaf->EmP->FindVolume(sfit->second.c_str());
	 if(vol){
  	   if(vol->orientation>0)       fprintf(fout, "  orientation  %d\n",  vol->orientation);
  	   if(vol->TEM_TabularOrder1>0) fprintf(fout, "  TEMtabularOrder1  %d\n",  vol->TEM_TabularOrder1);
           if(vol->TEM_TabularOrder2>0) fprintf(fout, "  TEMtabularOrder2  %d\n",  vol->TEM_TabularOrder2);
           if(vol->TEM_refCond>0)       fprintf(fout, "  TEMrefConductor  %d\n",  vol->TEM_refCond);
           if(vol->disconnectedTEM>0)   fprintf(fout, "  disconnectedTEM  %d\n",  vol->disconnectedTEM);
           if(vol->subcircuitIndex>1)   fprintf(fout, "  subcircuitIndex  %d\n",  vol->subcircuitIndex);
           if(vol->subcircuitIFindex>1) fprintf(fout, "  subcircuitIFindex  %d\n",  vol->subcircuitIFindex);
	 }
       }
       fprintf(fout, "}\n\n");
       std::vector<int> FI=WGIF_FI[sfname];
       for (int j=0; j<FI.size(); j++){
         char fname[50]; sprintf(fname,"F%d",FI[j]);
         std::string faceFileName=dirName;
         faceFileName+="/interfaces/";
         faceFileName+=fname;

         std::string mwmFileName=faceFileName+std::string(".mwm");
         std::ifstream mwmin;
         mwmin.open(mwmFileName.c_str()); if(!mwmin) continue;

         FILE *fp=fopen(faceFileName.c_str(),"r+");
         FaceData FD;
         FD.read(fp);
         fclose(fp);

         std::string str;
         while (!getLine(mwmin, str).eof()) fprintf(fout, "%s\n", str.c_str());
         mwmin.close();
 /* ---*/
         fprintf(fout, "DEF mat_%s  MWM_Material {\n",   FD.name.c_str());
         fprintf(fout, "   epsilonr          %g\n", FD.epsr );
         fprintf(fout, "   mur               %g\n", FD.mur);
         fprintf(fout, "}\n");

         fprintf(fout, "DEF UF%s  MWM_Volume {\n",  FD.name.c_str());
         fprintf(fout, "  type  Upperface\n");
         fprintf(fout, "  material \"mat_%s\"\n",   FD.name.c_str());
         fprintf(fout, "}\n\n");
       }
    }
     fflush(fout);
     #ifndef WNT
       int fd = fileno(fout);
       fsync(fd);   
     #else   
       FlushFileBuffers(fout);   
     #endif   
     fclose(fout);

}

