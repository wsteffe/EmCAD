/******************************************************************
*
*	MWM for C++
*
*	Copyright (C) Walter Steffe` 2004-2004
*
*	File:	mwm.cpp
*
******************************************************************/
 


#include<mwm.h>
#include<DataBase.h>
#include<string.h>
#include<assert.h>


//#include<patch.h>

#ifndef MIN
#define MIN(a,b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b)  (((a) > (b)) ? (a) : (b))
#endif


#define DOT(v1, v2) \
  ((v1)[0] * (v2)[0] + (v1)[1] * (v2)[1] + (v1)[2] * (v2)[2])

#define CROSS(v1, v2, n) \
  (n)[0] =   (v1)[1] * (v2)[2] - (v2)[1] * (v1)[2];\
  (n)[1] = -((v1)[0] * (v2)[2] - (v2)[0] * (v1)[2]);\
  (n)[2] =   (v1)[0] * (v2)[1] - (v2)[0] * (v1)[1]

#ifndef SIGN
#define SIGN(a)  (((a) > 0) ? 1 : -1)
#endif

int EBASETYPE= (TETMODELTYPE=='y')? 1:0;


namespace MWM {

MWM::Mesh *mesh=NULL;

void newMesh(){
    if(mesh){delete mesh;mesh=NULL;}
    mesh=new MWM::Mesh();
}

// ************
// fcmp_**

int fcmp_Surface(const void *a, const void *b){
  return strcmp((*(Surface **) a)->name, (*(Surface **) b)->name);
}

int fcmp_Curve(const void *a, const void *b){
  return strcmp((*(Curve **) a)->name, (*(Curve **) b)->name);
}

int fcmp_Vertex(const void *a, const void *b){
  return strcmp((*(Vertex **) a)->name, (*(Vertex **) b)->name);
}



int permu_sign(int n, int *permu ) // permu indexes starting at 0
{
 int psign=1;
 for( int i=0; i<n; i++){
   if(permu[i]!=i) for( int j=i+1; j<n; j++) if(permu[j]==i) {permu[j]=permu[i]; permu[i]=i; psign=-psign; break;}
 }
 return psign;
}





// ************
// Surface:


Surface::Surface(){
  points.stride=3;
//  pointsUV.stride=2;
  curves  =(Curve **) DB::Malloc(sizeof(Curve *));
  triaVnum=triaVinum=0;
  triaEdges=NULL;
  curveP=NULL;
  gf=NULL;
  masterSign=1;
}

Surface::~Surface(){
  DB::Free(curves);
  if(curveP) { delete [] curveP; curveP=NULL;}
}


// ************
// Curve:

Curve::Curve(const DB::str_t cname){
  points.stride=3;
//  pointsU.stride=1;
  strcpy(name,cname);
  onPEC=0;
  onPMC=0;
  onWG=0;
  onEC=0;
  onLP=0;
  shared=0;
  triaVNum=0;
  length=0;
  ge=NULL;
  masterSign=1;
}



// ************
// Vertex:

Vertex::Vertex(const DB::str_t vname){
  strcpy(name,vname);
  onPEC=0;
  onPMC=0;
  onEC=0;
  shared=0;
}



int fcmp_3d(const void *a, const void *b){
  double *P1=*(double **)a, *P2=*(double **)b;
  double cmp;
  for(int i = 0; i < 3; i++){
    cmp = P1[i]-P2[i];
    if(cmp > GEOMTOLERANCE)
      return 1;
    else if(cmp < -GEOMTOLERANCE)
      return -1;
  }
  return 0;
}



int fcmp_CurveByPoints(const void *a, const void *b){
  int    cmp;
  Curve *ca=(*(Curve **) a);
  Curve *cb=(*(Curve **) b);
// compare the number of points
  cmp=ca->points.n -cb->points.n;
  if(cmp) return(cmp>0? 1:-1);
// compare the two extreme points
  double *Pa, *Pb;
  Pa=&ca->points[0];
  Pb=&cb->points[0];
  if(cmp=fcmp_3d(&Pa, &Pb)) return(cmp);
  Pa=&ca->points[ca->points.n-1];
  Pb=&cb->points[ca->points.n-1];
  if(cmp=fcmp_3d(&Pa, &Pb)) return(cmp);
// compare the internal points
  for(int i = 1; i < ca->points.n-1; i++){
   Pa=&ca->points[i];
   Pb=&cb->points[i];
   if(cmp=fcmp_3d(&Pa, &Pb)) return(cmp);
  }
  return 0;
}

// ************
// Mesh:

void applyDelSurf(void *a, void *b)
{
  Surface *s = *(Surface **) a;
  delete(s);
}
void applyDelCurve(void *a, void *b)
{
  Curve *c = *(Curve **) a;
  delete(c);
}
void applyDelVertex(void *a, void *b)
{
  Vertex *p = *(Vertex **) a;
  delete(p);
}

Mesh::Mesh(){
  surfaces  =DB::Tree_Create(sizeof(Surface  *),  fcmp_Surface);
  curves    =DB::Tree_Create(sizeof(Curve  *),    fcmp_Curve);
  vertices  =DB::Tree_Create(sizeof(Vertex  *),   fcmp_Vertex);
  MaxVertexIndex=0; MaxCurveIndex=0; MaxSurfaceIndex=0; NumOfVertexV=0;
}
Mesh::~Mesh(){
  Tree_Action(surfaces,   applyDelSurf);
  Tree_Action(curves,     applyDelCurve);
  Tree_Action(vertices,   applyDelVertex);
  Tree_Delete(surfaces);
  Tree_Delete(curves);
  Tree_Delete(vertices);
}

void Mesh::addSurface   (Surface*   s){Tree_Add(surfaces,  &s); s->Index=++MaxSurfaceIndex;}
void Mesh::addCurve     (Curve*     c){Tree_Add(curves,    &c);}


Vertex  *Mesh::insertVertex  (const DB::str_t name){
   Vertex *P=new Vertex(name);
   void *ptr = Tree_PQuery(vertices, &P);
   Vertex *P0 = ptr? *(Vertex **) ptr : NULL;
   if(P0) {
       delete P;
       return P0;
   }else{
     Tree_Add(vertices, &P);
     P->Index=++MaxVertexIndex;
     return  P;
   }
}

Curve  *Mesh::insertCurve  (const DB::str_t name, const DB::str_t beg, const DB::str_t end ){
   Curve *c=new Curve(name);
   void *ptr = Tree_PQuery(curves, &c);
   Curve *c0 = ptr? *(Curve **) ptr : NULL;
   if(c0){
       delete c;
       return c0;
   }else{
      Tree_Add(curves, &c);
      c->Index=++MaxCurveIndex;
      if(beg) c->beg=insertVertex(beg);
      if(end) c->end=insertVertex(end);
      return  c;
   }
}

Surface *Mesh::FindSurface(const DB::str_t name)
{
  Surface S, *pS;
  pS = &S;
  strcpy(pS->name, name);
  if(Tree_Query(surfaces, &pS)) {
    return pS;
  }
  return NULL;
}

Curve *Mesh::FindCurve(const DB::str_t name)
{
  Curve C(name);
  Curve *pC;
  pC = &C;
  if(Tree_Query(curves, &pC)) {
    return pC;
  }
  return NULL;
}





}//namespace
 
 
