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
#include "OCCEdge.h"
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
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TCollection_AsciiString.hxx>

#include <BRep_Builder.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>

#include <TDF_ChildIterator.hxx>
#include <TDF_Reference.hxx>

#include <gp_Ax1.hxx>
#include <gp_Trsf.hxx>

#include "DataBase.h"


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
  gp_Trsf trsf; 
  TopTools_IndexedMapOfShape  *indexedSolids;
  TopTools_IndexedMapOfShape  *indexedFaces;
  TopTools_IndexedMapOfShape  *indexedEdges;
  TopTools_IndexedMapOfShape  *indexedVertices;
} ;

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

void makeInvMasters(MwOCAF* ocaf, Reloc* reloc, EdgeData  *edgeData, 
		std::vector<int> &Emaster, std::vector<std::vector<double> > &ETransf,
	       	std::vector<int> &Fmaster, std::vector<std::vector<double> > &FTransf
		)
{
  std::vector<int> Vmaster;
  std::map< std::set<int>, int > PIset2EI;
  int NE=reloc->indexedEdges->Extent();
  int NF=reloc->indexedFaces->Extent();
  int NV=reloc->indexedSolids->Extent();
  Emaster.resize(NE,0); ETransf.resize(NE);
  Fmaster.resize(NF,0); FTransf.resize(NF);
  Vmaster.resize(NV,0); 
  for (int EI=1; EI<=NE; EI++){
     TopoDS_Edge E= TopoDS::Edge(reloc->indexedEdges->FindKey(EI));
     std::set<int> Iset;
     Iset.insert(-EI);
     TopoDS_Vertex V1,V2;
     TopExp::Vertices(E,V1,V2);
     int IV1=reloc->indexedVertices->FindIndex(V1); Iset.insert(IV1);
     int IV2=reloc->indexedVertices->FindIndex(V2); Iset.insert(IV2);
     PIset2EI[Iset]=EI;
  }
  std::map< std::set<int>, int > EIset2FI;
  for (int FI=1; FI<=reloc->indexedFaces->Extent(); FI++){
     TopoDS_Shape F = reloc->indexedFaces->FindKey(FI);
     std::set<int> Iset;
     for (TopExp_Explorer exp(F,TopAbs_EDGE); exp.More(); exp.Next()){
       TopoDS_Shape E = exp.Current();
       E.Orientation(TopAbs_FORWARD);
       int EI=ocaf->indexedEdges->FindIndex(E);
       Iset.insert(EI);
     }
     EIset2FI[Iset]=FI;
  }
  List_T *invariants=Tree2List(ocaf->EmP.invariants);
  std::map< std::set<int>, int > FIset2VI;
  typedef std::set<int >::iterator IsetIt;
  typedef std::map<int,int>::iterator ImapIt;
  std::vector< std::map<int,int> > VFmap, VEmap;
  VFmap.resize(NV); VEmap.resize(NV);
  std::vector< std::vector< std::map<int,int> > > VFmap_i, VEmap_i;
  VFmap_i.resize(List_Nbr(invariants)); VEmap_i.resize(List_Nbr(invariants));
  for(int inv=0; inv<List_Nbr(invariants); inv++){ VFmap_i[inv].resize(NV); VEmap_i[inv].resize(NV);}
  for (int VI=1; VI<=reloc->indexedSolids->Extent(); VI++){
     TopoDS_Shape V = reloc->indexedSolids->FindKey(VI);
     std::set<int> Iset;
     for (TopExp_Explorer exp(V,TopAbs_FACE); exp.More(); exp.Next()){
       TopoDS_Shape F = exp.Current();
       F.Orientation(TopAbs_FORWARD);
       int FI=ocaf->indexedFaces->FindIndex(F);
       Iset.insert(FI);
     }
     FIset2VI[Iset]=VI;
     for (IsetIt it=Iset.begin(); it!= Iset.end(); it++){
	 int FI=*it;
	 VFmap[VI-1][FI]=FI;
         for(int inv=0; inv<List_Nbr(invariants); inv++) VFmap_i[inv][VI-1][FI]=FI;
     }
     Iset.clear();
     for (TopExp_Explorer exp(V,TopAbs_EDGE); exp.More(); exp.Next()){
       TopoDS_Shape E = exp.Current();
       E.Orientation(TopAbs_FORWARD);
       int EI=ocaf->indexedEdges->FindIndex(E);
       Iset.insert(EI);
     }
     for (IsetIt it=Iset.begin(); it!= Iset.end(); it++){
	  int EI=*it;
	  VEmap[VI-1][EI]=EI;
          for(int inv=0; inv<List_Nbr(invariants); inv++) VEmap_i[inv][VI-1][EI]=EI;
     }
  }
  int inv=0;
  std::vector<std::vector<int> > Emaster_i; Emaster_i.resize(List_Nbr(invariants));
  std::vector<std::vector<int> > Fmaster_i; Fmaster_i.resize(List_Nbr(invariants));
  std::vector<std::vector<int> > Vmaster_i; Vmaster_i.resize(List_Nbr(invariants));
  std::vector<std::vector<std::vector<double> > > ETransf_i; ETransf_i.resize(List_Nbr(invariants));
  std::vector<std::vector<std::vector<double> > > FTransf_i; FTransf_i.resize(List_Nbr(invariants));
  for(int inv=0; inv<List_Nbr(invariants); inv++){
    Emaster_i[inv].resize(NE,0);
    Fmaster_i[inv].resize(NF,0);
    Vmaster_i[inv].resize(NV,0);
    ETransf_i[inv].resize(NE);
    FTransf_i[inv].resize(NF);
    std::map<int,int> PEmap_i;
    DB::Transform *invar;
    List_Read(invariants, inv, &invar);
    std::string str_name=std::string(invar->name);
    gp_Trsf trsf,itrsf;
    computeAffineTransf(invar, trsf);
    trsf=reloc->trsf*(trsf*reloc->trsf.Inverted());
    computeAffineTransf(invar, itrsf,-1);
    itrsf=reloc->trsf*(itrsf*reloc->trsf.Inverted());
    std::vector<double> tfo,itfo;
    gp_trsf_2_gmsh(trsf,tfo);
    gp_trsf_2_gmsh(itrsf,itfo);
    std::vector<double> tfo1,itfo1;
    computeAffineTransf_(invar, tfo1);
    computeAffineTransf_(invar, itfo1,-1);
    double tol=1.e-10;
    for (int PI=1; PI<=reloc->indexedVertices->Extent(); PI++){
      TopoDS_Vertex P = TopoDS::Vertex(reloc->indexedVertices->FindKey(PI));
      gp_Pnt gP = BRep_Tool::Pnt(P);
      double p[4] = {gP.X(), gP.Y(), gP.Z(),1};
      double p_m[3]={0,0,0};
      for (size_t i=0,ij=0;i<3;i++)
          for (size_t j=0;j<4;j++,ij++) p_m[i] += itfo[ij] * p[j];
      gp_Pnt gP_m =gp_Pnt(p_m[0],p_m[1],p_m[2]);
      for (int QI=1; QI<=reloc->indexedVertices->Extent(); QI++){
        TopoDS_Vertex Q = TopoDS::Vertex(reloc->indexedVertices->FindKey(QI));
        gp_Pnt gQ=BRep_Tool::Pnt(Q);
        gp_Vec PQ=gp_Vec(gP_m, gQ);
        if (PQ.Magnitude() < tol) {
		PEmap_i[PI]=QI;
		break;
        }
      }
    }
    for (int IE=1; IE<=reloc->indexedEdges->Extent(); IE++){
      TopoDS_Edge EI = TopoDS::Edge(reloc->indexedEdges->FindKey(IE));
      double ui1,ui2;
      Handle(Geom_Curve) gci=BRep_Tool::Curve (EI, ui1, ui2);
      gp_Pnt gP = gci->Value((ui1+ui2)/2);
      double p[4] = {gP.X(), gP.Y(), gP.Z(),1};
      double p_m[3]={0,0,0};
      for (size_t i=0,ij=0;i<3;i++)
          for (size_t j=0;j<4;j++,ij++) p_m[i] += itfo[ij] * p[j];
      gp_Pnt gP_m =gp_Pnt(p_m[0],p_m[1],p_m[2]);
      for (int JE=1; JE<=reloc->indexedEdges->Extent(); JE++){
        TopoDS_Edge EJ = TopoDS::Edge(reloc->indexedEdges->FindKey(JE));
        double uj1,uj2;
        Handle(Geom_Curve) gcj=BRep_Tool::Curve (EJ, uj1, uj2);
        gp_Pnt gQ = gcj->Value((uj1+uj2)/2);
        gp_Vec PQ=gp_Vec(gP_m, gQ);
        if (PQ.Magnitude() < tol) {
	    PEmap_i[-IE]=-JE;
	    break;
        }
      }
    }
    for (int IE=1; IE<=NE; IE++){
       double refri=sqrt(edgeData[IE-1].epsr * edgeData[IE-1].mur)*edgeData[IE-1].meshref;
       std::set<int> Iset;
       TopoDS_Edge E= TopoDS::Edge(reloc->indexedEdges->FindKey(IE));
       Iset.insert(PEmap_i[-IE]);
       TopoDS_Vertex V1,V2;
       TopExp::Vertices(E,V1,V2);
       int IV1=reloc->indexedVertices->FindIndex(V1); Iset.insert(PEmap_i[IV1]);
       int IV2=reloc->indexedVertices->FindIndex(V2); Iset.insert(PEmap_i[IV2]);
       if(PIset2EI.find(Iset)!=PIset2EI.end()){
	   int IE_m=PIset2EI[Iset];
	   double refri_m=sqrt(edgeData[IE_m-1].epsr * edgeData[IE_m-1].mur)*edgeData[IE_m-1].meshref;
	   if(fabs(refri_m-refri)<1.e-3*refri){
            TopoDS_Edge E_m= TopoDS::Edge(reloc->indexedEdges->FindKey(IE_m));
            TopoDS_Vertex V1_m,V2_m;
            TopExp::Vertices(E_m,V1_m,V2_m);
            int IV1_m=reloc->indexedVertices->FindIndex(V1_m);
	    int sgn=PEmap_i[IV1]==IV1_m? 1:-1;
	    Emaster_i[inv][IE-1]=IE_m*sgn;
	    ETransf_i[inv][IE-1]=tfo;
	   }
       }
    }
    for (int FI=1; FI<=reloc->indexedFaces->Extent(); FI++){
       TopoDS_Face F = TopoDS::Face(reloc->indexedFaces->FindKey(FI));
       std::set<int> Iset;
       for (TopExp_Explorer exp(F,TopAbs_EDGE); exp.More(); exp.Next()){
         TopoDS_Shape E = exp.Current();
         E.Orientation(TopAbs_FORWARD);
         int EI=reloc->indexedEdges->FindIndex(E);
         Iset.insert(abs(Emaster_i[inv][EI-1]));
       }
       if(EIset2FI.find(Iset)!=EIset2FI.end()){
	   int FIm=EIset2FI[Iset];
	   FTransf_i[inv][FI-1]=tfo;
           TopoDS_Face Fm = TopoDS::Face(reloc->indexedFaces->FindKey(FIm));
	   Handle(Geom_Surface) GS  = BRep_Tool::Surface(F);
	   Handle(Geom_Surface) GSm = BRep_Tool::Surface(Fm);
	   TopExp_Explorer vexp(F,TopAbs_VERTEX);
           TopoDS_Vertex P = TopoDS::Vertex(vexp.Current());
           gp_Pnt gP = BRep_Tool::Pnt(P);
	   gp_Vec N=supNormal(GS, gP);
           double p[4] = {gP.X(), gP.Y(), gP.Z(),1};
           double pm[3]={0,0,0};
           for (size_t i=0,ij=0;i<3;i++)
             for (size_t j=0;j<4;j++,ij++) pm[i] += itfo[ij] * p[j];
           gp_Pnt gPm =gp_Pnt(pm[0],pm[1],pm[2]);
	   gp_Vec Nm=supNormal(GSm, gPm);
           double n[4] = {N.X(), N.Y(), N.Z(),0};
           double tfn[3]={0,0,0};
           for (size_t i=0,ij=0;i<3;i++)
             for (size_t j=0;j<4;j++,ij++) tfn[i] += itfo[ij] * n[j];
	   gp_Vec tfN=gp_Vec(tfn[0],tfn[1],tfn[2]);
	   Fmaster_i[inv][FI-1]=FIm*SIGN(tfN*Nm);
       }
    }
    for (int VI=1; VI<=reloc->indexedSolids->Extent(); VI++){
       char vname[100]; sprintf(vname,"Vol%d",VI);
       DB::Volume* vol=ocaf->EmP.FindVolume(vname);
       TopoDS_Shape V = reloc->indexedSolids->FindKey(VI);
       std::set<int> Iset;
       for (TopExp_Explorer exp(V,TopAbs_FACE); exp.More(); exp.Next()){
         TopoDS_Shape F = exp.Current();
         F.Orientation(TopAbs_FORWARD);
         int FI=reloc->indexedFaces->FindIndex(F);
         Iset.insert(abs(Fmaster_i[inv][FI-1]));
       }
       if(FIset2VI.find(Iset)!=FIset2VI.end()){
	   int VIm=FIset2VI[Iset];
           char vmname[100]; sprintf(vmname,"Vol%d",VIm);
           DB::Volume* volm=ocaf->EmP.FindVolume(vmname);
	   if(!strcmp(volm->material,vol->material)){
	    Vmaster_i[inv][VI-1]=VIm;
            for (ImapIt it=VFmap_i[inv][VI-1].begin(); it!= VFmap_i[inv][VI-1].end(); it++)  (*it).second=Fmaster_i[inv][(*it).first-1];
            for (ImapIt it=VEmap_i[inv][VI-1].begin(); it!= VEmap_i[inv][VI-1].end(); it++)  (*it).second=Emaster_i[inv][(*it).first-1];
	   }
       }
    }
  }


  std::vector<std::set<int> > EeqivClass;
  EeqivClass.reserve(NE);
  for (int I=1; I<=NE; I++){
    std::set<int> eqi; eqi.insert(I);
    Emaster[I-1]=I;
    gp_Trsf trsf=gp_Trsf();
    gp_trsf_2_gmsh(trsf,ETransf[I-1]);
    EeqivClass.push_back(eqi);
  }
//  for (int I=1; I<=NE; I++){
//    int M=abs(Emaster[I-1]);
//    if(M>0) EeqivClass[M-1].insert(I);
//  }
  bool changed=true;
  while(changed){ 
     changed=false;
     escapeE: 
     for (int I1=1; I1<=NE; I1++)  if(EeqivClass[I1-1].size()>0)
     for (std::set<int>::const_iterator it=EeqivClass[I1-1].begin(); it!= EeqivClass[I1-1].end(); it++) {
	 int I=*it;
	 assert(abs(Emaster[I-1])==I1);
         gp_Trsf trsf;
         for(int inv=0; inv<List_Nbr(invariants); inv++){
          int I2=Emaster_i[inv][I-1]; I2=abs(I2);
          int I3=Emaster[I2-1]; I3=abs(I3);
          if(I2 && I3!=I1){  //I2 is not in the class I1
           gmsh_2_gp_trsf(ETransf[I-1],trsf);
	   trsf.Invert();
	   std::vector<double> invTfo_I;
           gp_trsf_2_gmsh(trsf,invTfo_I);
	   int sgnTfoI=SIGN(Emaster[I-1]);
	   std::vector<double> tfo2;
           multiplyAffineTransf(ETransf_i[inv][I-1],ETransf[I2-1],tfo2);
	   int sgnTfo2=SIGN(Emaster_i[inv][I-1])*SIGN(Emaster[I2-1]);
           for (std::set<int>::const_iterator jt=EeqivClass[I1-1].begin(); jt!= EeqivClass[I1-1].end(); jt++) {
	    int J=*jt;
	    int sgnTfoJ=SIGN(Emaster[J-1]);
	    std::vector<double> tmp,tmp2;
            multiplyAffineTransf(ETransf[J-1],invTfo_I, tmp);
            multiplyAffineTransf(tmp,tfo2,tmp2); 
	    ETransf[J-1]=tmp2;
	    Emaster[J-1]=I3*sgnTfoJ*sgnTfoI*sgnTfo2;
	   }
           for (std::set<int>::const_iterator jt=EeqivClass[I1-1].begin(); jt!= EeqivClass[I1-1].end(); jt++)  EeqivClass[I3-1].insert(*jt);
           EeqivClass[I1-1].clear();
           changed=true;
	   goto escapeE;
          }
         }
     }
  }

  std::vector<std::set<int> > FeqivClass;
  FeqivClass.reserve(NF);
  for (int I=1; I<=NF; I++){
    std::set<int> eqi; eqi.insert(I);
    Fmaster[I-1]=I;
    gp_Trsf trsf=gp_Trsf();
    gp_trsf_2_gmsh(trsf,FTransf[I-1]);
    FeqivClass.push_back(eqi);
  }
  changed=true;
  while(changed){ 
     changed=false;
     escapeF: 
         for (int I1=1; I1<=NF; I1++)  if(FeqivClass[I1-1].size()>0)
	 for (std::set<int>::const_iterator it=FeqivClass[I1-1].begin(); it!= FeqivClass[I1-1].end(); it++) {
	 int I=*it;
	 assert(abs(Fmaster[I-1])==I1);
         gp_Trsf trsf;
         for(int inv=0; inv<List_Nbr(invariants); inv++){
          int I2=Fmaster_i[inv][I-1]; I2=abs(I2);
          int I3=Fmaster[I2-1]; I3=abs(I3);
          if(I2 && I3!=I1){  //I2 is not in the class I1
           gmsh_2_gp_trsf(FTransf[I-1],trsf);
	   trsf.Invert();
	   std::vector<double> invTfo_I;
           gp_trsf_2_gmsh(trsf,invTfo_I);
	   int sgnTfoI=SIGN(Fmaster[I-1]);
	   std::vector<double> tfo2;
           multiplyAffineTransf(FTransf_i[inv][I-1],FTransf[I2-1],tfo2);
	   int sgnTfo2=SIGN(Fmaster_i[inv][I-1])*SIGN(Fmaster[I2-1]);
           for (std::set<int>::const_iterator jt=FeqivClass[I1-1].begin(); jt!= FeqivClass[I1-1].end(); jt++) {
	    int J=*jt;
	    int sgnTfoJ=SIGN(Fmaster[J-1]);
	    std::vector<double> tmp,tmp2;
            multiplyAffineTransf(FTransf[J-1],invTfo_I, tmp);
            multiplyAffineTransf(tmp,tfo2,tmp2); 
	    FTransf[J-1]=tmp2;
	    Fmaster[J-1]=I3*sgnTfoJ*sgnTfoI*sgnTfo2;
	   }
           for (std::set<int>::const_iterator jt=FeqivClass[I1-1].begin(); jt!= FeqivClass[I1-1].end(); jt++)  FeqivClass[I3-1].insert(*jt);
           FeqivClass[I1-1].clear();
           changed=true;
	   goto escapeF;
          }
         }
     }
  }



  std::vector<std::set<int> > VeqivClass;
  VeqivClass.reserve(NV);
  for (int I=1; I<=NV; I++){
    std::set<int> eqi; eqi.insert(I);
    Vmaster[I-1]=I;
    VeqivClass.push_back(eqi);
  }
  changed=true;
  while(changed){ 
     changed=false;
     escapeV: 
         for (int I1=1; I1<=NV; I1++)  if(VeqivClass[I1-1].size()>0)
	 for (std::set<int>::const_iterator it=VeqivClass[I1-1].begin(); it!= VeqivClass[I1-1].end(); it++) {
	 int I=*it;
	 assert(Vmaster[I-1]==I1);
         gp_Trsf trsf;
         for(int inv=0; inv<List_Nbr(invariants); inv++){
          int I2=Vmaster_i[inv][I-1];
          int I3=Vmaster[I2-1];
          if(I2 && I3!=I1){  //I2 is not in the class I1
	   std::map<int,int> invVFMap_I,invVEMap_I;
	   invertMap(VFmap[I-1],invVFMap_I);
	   invertMap(VEmap[I-1],invVEMap_I);
	   std::map<int,int> VFMap2,VEMap2;
           multiplyMaps(VFmap_i[inv][I-1],VFmap[I2-1],VFMap2);
           multiplyMaps(VEmap_i[inv][I-1],VEmap[I2-1],VEMap2);
           for (std::set<int>::const_iterator jt=VeqivClass[I1-1].begin(); jt!= VeqivClass[I1-1].end(); jt++) {
	    int J=*jt;
	    std::map<int,int> tmp,tmp2;
            multiplyMaps(VFmap[J-1],invVFMap_I, tmp);
            multiplyMaps(tmp,VFMap2,tmp2); 
	    VFmap[J-1]=tmp2;
	    tmp.clear();tmp2.clear();
            multiplyMaps(VEmap[J-1],invVEMap_I, tmp);
            multiplyMaps(tmp,VEMap2,tmp2); 
	    VEmap[J-1]=tmp2;
	    Vmaster[J-1]=I3;
	   }
           for (std::set<int>::const_iterator jt=VeqivClass[I1-1].begin(); jt!= VeqivClass[I1-1].end(); jt++)  VeqivClass[I3-1].insert(*jt);
           VeqivClass[I1-1].clear();
           changed=true;
	   goto escapeV;
          }
         }
     }
  }


  List_Delete(invariants);

  for (int I=1; I<=NE; I++) if(Emaster[I-1]==I) Emaster[I-1]=0;
  for (int I=1; I<=NF; I++) if(Fmaster[I-1]==I) Fmaster[I-1]=0;
  for (int I=1; I<=NV; I++) if(Vmaster[I-1]==I) Vmaster[I-1]=0;

  for (int I=1; I<=NE; I++) if(Emaster[I-1]) {
       int M=abs(Emaster[I-1]); Emaster[M-1]=M;
  }
  for (int I=1; I<=NF; I++) if(Fmaster[I-1]) {
       int M=abs(Fmaster[I-1]); Fmaster[M-1]=M;
  }
  for (int I=1; I<=NV; I++) if(Vmaster[I-1]) {
       int M=Vmaster[I-1];
       if(M) Vmaster[M-1]=M;
  }
  for (int I=1; I<=NV; I++) if(Vmaster[I-1]) {
       char vname[100]; sprintf(vname,"Vol%d",I);
       DB::Volume* vol=ocaf->EmP.FindVolume(vname);
       vol->master=Vmaster[I-1];
       vol->Fmap=VFmap[I-1];
       vol->Cmap=VEmap[I-1];
  }


}

void checkMasterEdges(
		GModel *gm,
		TopTools_IndexedMapOfShape *indexedEdges,
	       	std::vector<int> &Emaster,
		std::vector<std::vector<double> > &ETransf
	   )
{
     
    for(GModel::eiter eit = gm->firstEdge(); eit != gm->lastEdge(); ++eit){
         GEdge *ge=*eit;
         TopoDS_Edge E1=* (TopoDS_Edge *) ge->getNativePtr();
         int I1=indexedEdges->FindIndex(E1);
         assert(I1);
         TopoDS_Vertex V1_1,V1_2;
         TopExp::Vertices(E1,V1_1,V1_2);
	 int I2=Emaster[I1-1];
	 int sgn2=SIGN(I2); I2=abs(I2);
         TopoDS_Edge E2= TopoDS::Edge(indexedEdges->FindKey(I2));
         TopoDS_Vertex V2_1,V2_2;
         TopExp::Vertices(E2,V2_1,V2_2);
         double tol=1.e-10;
         gp_Pnt gP = BRep_Tool::Pnt(V2_1);
         double p[4] = {gP.X(), gP.Y(), gP.Z(),1};
         double p_t[3]={0,0,0};
         std::vector<double> tf1=ETransf[I1-1];
         for (size_t i=0,ij=0;i<3;i++)
           for (size_t j=0;j<4;j++,ij++) p_t[i] += tf1[ij] * p[j];
         gp_Pnt gP_t =gp_Pnt(p_t[0],p_t[1],p_t[2]);
         gp_Pnt gQ = sgn2>0 ? BRep_Tool::Pnt(V1_1):BRep_Tool::Pnt(V1_2) ;
         gp_Vec PQ=gp_Vec(gP_t, gQ);
         assert (PQ.Magnitude() < tol);
    }

}

void setMasterEdges(
		GModel *gm,
		TopTools_IndexedMapOfShape *indexedEdges,
		std::map< int, GEdge * > &indexedGmshEdges,
	       	std::vector<int> &Emaster,
		std::vector<std::vector<double> > &ETransf
	   )
{
     
    for(GModel::eiter eit = gm->firstEdge(); eit != gm->lastEdge(); ++eit){
         GEdge *ge=*eit;
         TopoDS_Edge E=* (TopoDS_Edge *) ge->getNativePtr();
         int EI=indexedEdges->FindIndex(E);
         assert(EI);
         if(EI) if(Emaster[EI-1]) if(Emaster[EI-1] !=EI) {
	  int ori=SIGN(Emaster[EI-1]);
	  int EI_m=abs(Emaster[EI-1]);
          GEdge *ge_m=indexedGmshEdges[EI_m];
	  ge->setMeshMaster(ge_m,ETransf[EI-1]);
	 }
    }

}


void setMasterFaces(
		GModel *gm,
		TopTools_IndexedMapOfShape *indexedFaces,
		std::map< int, GFace * > &indexedGmshFaces,
	       	std::vector<int> &Fmaster,
		std::vector<std::vector<double> > &FTransf
	   )
{
    for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
         GFace *gf=*fit;
         TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
         int FI=indexedFaces->FindIndex(F);
         assert(FI);
         if(FI) if(Fmaster.size()>FI-1) if(Fmaster[FI-1]) if(Fmaster[FI-1] !=FI) {
	  int FI_m=abs(Fmaster[FI-1]);
          GFace *gf_m=indexedGmshFaces[FI_m];
	  gf->setMeshMaster(gf_m,FTransf[FI-1]);
	 }
    }

}

void checkMasterFaces(
		GModel *gm,
		TopTools_IndexedMapOfShape *indexedFaces,
		TopTools_IndexedMapOfShape *indexedEdges,
		std::map< int, GFace * > &indexedGmshFaces,
	       	std::vector<int> &Fmaster,
	       	std::vector<int> &Emaster
	   )
{
      for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
         GFace *gf=*fit;
         TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
         int FI=indexedFaces->FindIndex(F);
         assert(FI);
         if(FI) if(Fmaster.size()>FI-1) if(Fmaster[FI-1]) if(Fmaster[FI-1] !=FI) {
	  int FI_m=abs(Fmaster[FI-1]);
          GFace *gf_m=indexedGmshFaces[FI_m];
	  std::set<int> gf_m_EI;
	  int fenum=gf_m->edges().size();
	  std::list<GEdge*> gf_m_edges = gf_m->edges();
          for(std::list<GEdge*>::const_iterator eit = gf_m_edges.begin(); eit != gf_m_edges.end(); ++eit){
            GEdge *ge=*eit;
            TopoDS_Edge E=* (TopoDS_Edge *) ge->getNativePtr();
            E.Orientation(TopAbs_FORWARD);
            int EI=indexedEdges->FindIndex(E);
            gf_m_EI.insert(abs(Emaster[EI-1]));
	  }
	  std::list<GEdge*> gf_edges = gf->edges();
          for(std::list<GEdge*>::const_iterator eit = gf_edges.begin(); eit != gf_edges.end(); ++eit){
            GEdge *ge=*eit;
            TopoDS_Edge E=* (TopoDS_Edge *) ge->getNativePtr();
            E.Orientation(TopAbs_FORWARD);
            int EI=indexedEdges->FindIndex(E);
            assert(gf_m_EI.find(abs(Emaster[EI-1]))!=gf_m_EI.end());
	  }
         }

      }

}



void print_mwm(GModel *gm,  MwOCAF* ocaf, bool isPartition,
	      const char* dirName, const char* modelDir,
	      TopTools_IndexedMapOfShape *indexedFaces,
	      TopTools_IndexedMapOfShape *indexedEdges,
	      TopTools_IndexedMapOfShape *indexedVertices,
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


bool isGridFace(GFace *gf)
{
   std::list<GEdge*>   Fedges=gf->edges();
   std::set<GEdge*> edgeSet;
   for (std::list<GEdge*>::const_iterator eit=Fedges.begin(); eit!=Fedges.end(); ++eit){
     GEdge *ge=*eit;
     edgeSet.insert(ge);
   }
   int coupledSize=0;
   for (std::list<GEdge*>::const_iterator eit=Fedges.begin(); eit!=Fedges.end(); ++eit){
     GEdge *ge=*eit;
     GEdge *gem = dynamic_cast<GEdge*> (ge->meshMaster());
     if (gem != ge) if(edgeSet.find(gem)!=edgeSet.end()) coupledSize+=2;
   }
   return coupledSize==edgeSet.size();
}

void laplaceSmoothing(GFace *gf);

void MESHER::meshModel(MwOCAF* ocaf, double meshsize, const char* dirName, const char* modelDir)
{
    bool CHECK=false;

    CTX::instance()->geom.tolerance=1.e-6;

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
    reloc.trsf=loc.Transformation();
    reloc.indexedSolids=new TopTools_IndexedMapOfShape;
    for (int VI=1; VI<=ocaf->indexedSolids->Extent(); VI++){
      TopoDS_Shape V = ocaf->indexedSolids->FindKey(VI);
      V.Move(loc);
      reloc.indexedSolids->Add(V);
    }
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

    bool print_brep=false;
    if(print_brep){
      char brepFileName[256];
      strcpy(brepFileName, "MODEL.brep");
      BRepTools::Write(theFaces, brepFileName);
    }


    std::vector<int> Emaster;
    std::vector<std::vector<double> > ETransf;
    std::vector<int> Fmaster;
    std::vector<std::vector<double> > FTransf;
    if(isPartition) makeInvMasters(ocaf,&reloc, ocaf->edgeData, Emaster,ETransf,Fmaster,FTransf);


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
//


//
      std::map< int, GEdge * > indexedGmshEdges;
      for(GModel::eiter eit = gm->firstEdge(); eit != gm->lastEdge(); ++eit){
         GEdge *ge=*eit;
	 assert(ge->getNativeType()==GEntity::OpenCascadeModel);
//         OCCEdge *occe= dynamic_cast<OCCEdge*> (ge);
         TopoDS_Edge E=* (TopoDS_Edge *) ge->getNativePtr();
         int EI=reloc.indexedEdges->FindIndex(E);
         assert(EI);
         if(!EI) continue;
	 indexedGmshEdges[EI]=ge;
      }

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

      if(isPartition){
        if(CHECK) checkMasterEdges(gm, reloc.indexedEdges, Emaster, ETransf);
        setMasterEdges(gm, reloc.indexedEdges, indexedGmshEdges, Emaster, ETransf);
      }

      for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
         GFace *gf=*fit;
	 TopoDS_Shape F=* (TopoDS_Face *) gf->getNativePtr();
         int FI=reloc.indexedFaces->FindIndex(F);
         if(isPartition)
//           gf->meshAttributes.method= isGridFace(gf) ? MESH_TRANSFINITE: MESH_UNSTRUCTURED;
           gf->meshAttributes.method=MESH_UNSTRUCTURED;
	 else if(ocaf->faceData[FI-1].shared )
           gf->meshAttributes.method=MESH_UNSTRUCTURED;
	 else
           gf->meshAttributes.method=MESH_NONE;
      }
      if(mesh_aniso)  CTX::instance()->mesh.algo2d=ALGO_2D_BAMG;
//      else            CTX::instance()->mesh.algo2d=ALGO_2D_MESHADAPT;
      else            CTX::instance()->mesh.algo2d=ALGO_2D_FRONTAL;
//      else            CTX::instance()->mesh.algo2d=ALGO_2D_DELAUNAY;
//      else  CTX::instance()->mesh.algo2d=ALGO_2D_MESHADAPT_OLD;
      CTX::instance()->mesh.lcFromPoints=1;
      CTX::instance()->mesh.lcFromCurvature=1;
      CTX::instance()->mesh.lcExtendFromBoundary=0;
      CTX::instance()->mesh.minCircPoints=isNET? 7: 7;
      CTX::instance()->mesh.anisoMax=500000;
      CTX::instance()->mesh.nbSmoothing=2;
      CTX::instance()->mesh.smoothRatio=5.0;
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


      std::map< int, GFace * > indexedGmshFaces;
      for(GModel::fiter fit = gm->firstFace(); fit != gm->lastFace(); ++fit){
         GFace *gf=*fit;
         TopoDS_Face F=* (TopoDS_Face *) gf->getNativePtr();
         int FI=reloc.indexedFaces->FindIndex(F);
         assert(FI);
         if(!FI) continue;
	 indexedGmshFaces[FI]=gf;
      }


      gm->mesh(1);

      if(CHECK) for(GModel::eiter eit = gm->firstEdge(); eit != gm->lastEdge(); ++eit){
        GEdge *ge=*eit;
	if (ge->meshMaster() != ge){
	   GEdge *gem = dynamic_cast<GEdge*> (ge->meshMaster());
	   assert(ge->mesh_vertices.size()==gem->mesh_vertices.size());
        }
      }


      if(CHECK) checkMasterFaces(gm, reloc.indexedFaces, reloc.indexedEdges, indexedGmshFaces, Fmaster, Emaster);
      setMasterFaces(gm, reloc.indexedFaces, indexedGmshFaces, Fmaster, FTransf);

      gm->mesh(2);

      if(CHECK) for(GModel::eiter eit = gm->firstEdge(); eit != gm->lastEdge(); ++eit){
        GEdge *ge=*eit;
	if (ge->meshMaster() != ge){
	   GEdge *gem = dynamic_cast<GEdge*> (ge->meshMaster());
	   assert(ge->mesh_vertices.size()==gem->mesh_vertices.size());
        }
      }

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
		reloc.indexedVertices,
		Fmaster
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

