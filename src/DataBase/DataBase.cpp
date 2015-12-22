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


#include "DataBase.h"
#include "math.h"

DB::Units unit;

namespace DB {

Units::Units(){
  xm   =1e-3;  //[len]=mm
  xHz  =1e+9;  //[freq]=GHz
  xOhm =1;     //[R]=Ohm
}


void PhysicConstants::set(Units u){
   double c0_mks =299792458;
   double mu0_mks=4*PIG*1.e-7;
   double Z0_mks =mu0_mks*c0_mks;
   c0=c0_mks/(u.xm*u.xHz);
   Z0=Z0_mks/u.xOhm;
   mu0=Z0/c0;
   eps0=1.0/(Z0*c0);
}


//*** StringList_t:

StringList_t::StringList_t(){
  list=List_Create(6, 6, sizeof(str_t));
}
StringList_t::~StringList_t(){
 List_Delete(list);
}
void StringList_t::add(char *string){
 str_t str;
 strcpy(str,string);
 List_Add(list, str);
}
void StringList_t::get(int i, str_t str){
 List_Read(list, i, str);
}
int StringList_t::num(){
 return List_Nbr(list);
}
void StringList_t::reset(){
 List_Reset(list);
}


// ************
// Material:


Material::Material(){
  I=0;
  epsr=1.0;
  mur=1.0;
  etandelta=0;
  htandelta=0;
  econductivity=0;
  hconductivity=0;
  edispersive=0;
  hdispersive=0;
  Sresistance=0;
  freqBand[0]=freqBand[1]=0.0;
  epsLorentz.stride=3;
  muLorentz.stride=3;
  color[0]=49;  color[1]=79; color[2]=79; color[3]=255;
}
void Material::flush(){
  buff.epsLorentz.flush(&epsLorentz);
  buff.muLorentz.flush(&muLorentz);
}

Material & Material::operator =(const Material &rhs)
{
  if (this != &rhs) { 
//      strcpy(name, rhs.name);
      I = rhs.I;
      epsr=rhs.epsr;
      mur=rhs.mur;
      etandelta=rhs.etandelta;
      htandelta=rhs.htandelta;
      econductivity=rhs.econductivity;
      edispersive=rhs.edispersive;
      hdispersive=rhs.hdispersive;
      epsLorentz=rhs.epsLorentz;
      muLorentz=rhs.muLorentz;
      Sresistance=rhs.Sresistance;
      for(int i=0; i<4; i++) color[i]=rhs.color[i];
  }
  return *this;    // Return ref for multiple assignment
}


Transform::Transform(){
  trasl[0]=trasl[1]=trasl[2]=0;
  rotAxis[0]=rotAxis[1]=rotAxis[2]=0;
  rotOrigin[0]=rotOrigin[1]=rotOrigin[2]=0;
  rotAngle=0;
}


Transform& Transform::operator =(const Transform &rhs)
{
  if (this != &rhs) {
      rotAngle=rhs.rotAngle;
      for(int i=0; i<3; i++){
	   trasl[i]=rhs.trasl[i];
	   rotAxis[i]=rhs.rotAxis[i];
	   rotOrigin[i]=rhs.rotOrigin[i];
      }
  }
  return *this;    // Return ref for multiple assignment
}


Volume::Volume(){
  type=UNDEFINED;
  master=0;
  TEMportsNum=0;
  TEportsNum=0;
  TMportsNum=0;
  gridNum=0;
  PML=0;
  invariant=1;
  defined=1;
  disabled=0;
  compSolid=0;
  meshRefinement=1;
  strcpy(material,"?");
}
Volume & Volume::operator =(Volume const &rhs){
//	 strcpy(name, rhs.name);
	 type=rhs.type;
	 strcpy(material, rhs.material);
	 TEportsNum=rhs.TEportsNum;
	 TMportsNum=rhs.TMportsNum;
	 compSolid=rhs.compSolid;
	 return *this;
}



// ************
// fcmp_**


int fcmp_Material(const void *a, const void *b){
  return strcmp((*(Material **) a)->name, (*(Material **) b)->name);
}
int fcmp_Volume(const void *a, const void *b){
  return strcmp((*(Volume **) a)->name, (*(Volume **) b)->name);
}
int fcmp_Transform(const void *a, const void *b){
  return strcmp((*(Transform **) a)->name, (*(Transform **) b)->name);
}



void applyDelMat(void *a, void *b)
{
  Material *m = *(Material **) a;
  delete(m);
}
void applyDelVol(void *a, void *b)
{
  Volume *v = *(Volume **) a;
  delete(v);
}

// ************
// EmProblem:

EmProblem::EmProblem(){
  materials =Tree_Create(sizeof(Material *), fcmp_Material);
  volumes   =Tree_Create(sizeof(Volume *),   fcmp_Volume);
  invariants  =Tree_Create(sizeof(Transform *), fcmp_Transform);
  hasGeo=false;
  level=0;
  strcpy(defaultBC,"PEC");
  assemblyType=0;
  DB::Material* mat;
  mat=FindMaterial("PEC");
  if(!mat){
    mat =new DB::Material();
    strcpy(mat->name,"PEC");
    addMaterial(mat);
  }
  mat=FindMaterial("PMC");
  if(!mat){
    mat =new DB::Material();
    strcpy(mat->name,"PMC");
    addMaterial(mat);
  }
  mat=FindMaterial("PML");
  if(!mat){
    mat =new DB::Material();
    strcpy(mat->name,"PML");
    addMaterial(mat);
  }
}
EmProblem::~EmProblem(){
  Tree_Action(materials,  applyDelMat); Tree_Delete(materials);
  Tree_Action(volumes,    applyDelVol); Tree_Delete(volumes);
}

void EmProblem::addMaterial  (Material*  m){Tree_Add(materials, &m);}
void EmProblem::insertMaterial(Material* m){Tree_Insert(materials, &m);}
void EmProblem::delMaterial  (Material*  m){Tree_Suppress(materials, &m);}
void EmProblem::insertVolume (Volume*    v){Tree_Insert(volumes,   &v);}
void EmProblem::delVolume    (Volume*    v){Tree_Suppress(volumes, &v);}
void EmProblem::addInvariant  (Transform* t){Tree_Add(invariants, &t);}
void EmProblem::insertInvariant (Transform* t){Tree_Insert(invariants,   &t);}
void EmProblem::delInvariant    (Transform* t){Tree_Suppress(invariants, &t);}


Material *EmProblem::FindMaterial(const str_t name)
{
  Material M, *pM;
  pM = &M;
  strcpy(pM->name, name);
  if(Tree_Query(materials, &pM)) {
    return pM;
  }
  return NULL;
}

Volume *EmProblem::FindVolume(const str_t name)
{
  Volume V, *pV;
  pV = &V;
  strcpy(pV->name, name);
  if(Tree_Query(volumes, &pV)) {
    return pV;
  }
  return NULL;
}

Transform *EmProblem::FindInvariant(const str_t name)
{
  Transform T, *pT;
  pT = &T;
  strcpy(pT->name, name);
  if(Tree_Query(invariants, &pT)) {
    return pT;
  }
  return NULL;
}



static FILE *EMMFILE;

void _mwm_print_units(){
  fprintf(EMMFILE, "DEF UNITS MWM_Units {\n");
  fprintf(EMMFILE, "   length          %.3e\n", unit.xm);
  fprintf(EMMFILE, "}\n\n");
}

void _mwm_print_invariants(void *a, void *b)
{
  DB::Transform *invt;
  invt = *(DB::Transform **) a;
  fprintf(EMMFILE, "DEF %s  MWM_Invariant {\n",   invt->name);
  if(invt->rotAngle==0){
    fprintf(EMMFILE, " translation  [");
    for(int i=0; i<3; i++) fprintf(EMMFILE, "   %.20f ", invt->trasl[i]);
    fprintf(EMMFILE, "]\n");
  } else{
    fprintf(EMMFILE, " rotation angle    %.20f\n", invt->rotAngle);
    fprintf(EMMFILE, " rotation axis  [");
    for(int i=0; i<3; i++) fprintf(EMMFILE, "   %.20f ", invt->rotAxis[i]);
    fprintf(EMMFILE, "]\n");
    fprintf(EMMFILE, " rotation origin  [");
    for(int i=0; i<3; i++) fprintf(EMMFILE, "   %.20f ", invt->rotOrigin[i]);
    fprintf(EMMFILE, "]\n");
  }
  fprintf(EMMFILE, "}\n\n");
}

void _mwm_print_material(void *a, void *b)
{
  DB::Material *mat;
  mat = *(DB::Material **) a;
  fprintf(EMMFILE, "DEF %s  MWM_Material {\n",   mat->name);
   fprintf(EMMFILE, "   epsilonr          %g\n", mat->epsr);
   fprintf(EMMFILE, "   mur               %g\n", mat->mur);
   if(mat->econductivity >0)\
   fprintf(EMMFILE, "   eConductivity     %g\n", mat->econductivity);
   if(mat->hconductivity >0)\
   fprintf(EMMFILE, "   hConductivity     %g\n", mat->hconductivity);
   if(mat->etandelta >0)\
   fprintf(EMMFILE, "   eTanDelta     %g\n", mat->etandelta);
   if(mat->htandelta >0)\
   fprintf(EMMFILE, "   eTanDelta     %g\n", mat->htandelta);

   if(mat->freqBand[1]>0)\
   fprintf(EMMFILE,"   frequency band %.8e  %.8e\n", mat->freqBand[0], mat->freqBand[1]);
   
   if(mat->epsLorentz.n) {
    fprintf(EMMFILE, "   epsLorentz [\n");
    for(int i=0; i<mat->epsLorentz.n; i++){
     double *d=&mat->epsLorentz[i];
     fprintf(EMMFILE, "          ");
     fprintf(EMMFILE, "   %.8f ", d[0]);
     for(int j=1; j<mat->epsLorentz.stride; j++) fprintf(EMMFILE, "   %.8e ", d[j]);
     fprintf(EMMFILE, "\n");
    }
    fprintf(EMMFILE, "               ]\n");
   }

   if(mat->muLorentz.n) {
    fprintf(EMMFILE, "   muLorentz [\n");
    for(int i=0; i<mat->muLorentz.n; i++){
     double *d=&mat->muLorentz[i];
     fprintf(EMMFILE, "          ");
     fprintf(EMMFILE, "   %.8f ", d[0]);
     for(int j=1; j<mat->epsLorentz.stride; j++) fprintf(EMMFILE, "   %.8e ", d[j]);
     fprintf(EMMFILE, "\n");
    }
    fprintf(EMMFILE, "              ]\n");
   }

   if(mat->Sresistance >0)
      fprintf(EMMFILE, "  SurfaceResistance  %g\n",   mat->Sresistance);
      
   fprintf(EMMFILE, "   color [ %d  %d  %d  %d ]\n",   mat->color[0], mat->color[1], mat->color[2], mat->color[3]);

  fprintf(EMMFILE, "}\n\n");
}

bool printOnlyMeshData=false;
bool printOnlyWgData=false;


std::string WGPREFIX;

typedef std::map<int,int>::iterator ImapIt;

void _mwm_print_volume(void *a, void *b)
{
  DB::Volume *vol = *(DB::Volume **) a;
  if(vol->compSolid && printOnlyMeshData) return;
  if(vol->disabled && printOnlyMeshData) return;
  if(vol->type==GRID && (printOnlyMeshData ||printOnlyWgData) ) return;
  if(vol->type==LINEPORT && (printOnlyMeshData ||printOnlyWgData) ) return;
  if((vol->type==DIELECTRIC || vol->type==HOLE) && printOnlyWgData) return;
  if(vol->type==WAVEGUIDE) fprintf(EMMFILE, "DEF %s%s  MWM_Volume {\n", WGPREFIX.c_str(),vol->name);
  else                     fprintf(EMMFILE, "DEF %s  MWM_Volume {\n", vol->name);
  switch(vol->type){
     case DIELECTRIC:
	   fprintf(EMMFILE, "  type  Dielectric\n");
	   if(!printOnlyMeshData){
		fprintf(EMMFILE, "  meshRefinement  %f \n", vol->meshRefinement);
		fprintf(EMMFILE, "  compSolid  %d \n", vol->compSolid);
	   }
	   if(printOnlyMeshData) if(vol->master){
	      fprintf(EMMFILE, "  master  %d\n",vol->master);
	      char mname[100]; sprintf(mname,"Vol%d",vol->master);
	      if(strcmp(vol->name,mname)){
		fprintf(EMMFILE, "  face map [\n");
		for (ImapIt it=vol->Fmap.begin(); it!= vol->Fmap.end(); it++) fprintf(EMMFILE, "  %d  %d \n",(*it).first, abs((*it).second));
		fprintf(EMMFILE, "  ]\n");
		fprintf(EMMFILE, "  curve map [\n");
		for (ImapIt it=vol->Cmap.begin(); it!= vol->Cmap.end(); it++) fprintf(EMMFILE, "  %d  %d \n",(*it).first, (*it).second);
		fprintf(EMMFILE, "  ]\n");
	      }
	   }
	   break;
     case HOLE:
	   fprintf(EMMFILE, "  type  Hole\n");
	   if(!printOnlyMeshData){
		fprintf(EMMFILE, "  meshRefinement  %f \n", vol->meshRefinement);
		fprintf(EMMFILE, "  compSolid  %d \n", vol->compSolid);
	   }
	   break;
     case WAVEGUIDE:
	   fprintf(EMMFILE, "  type  WaveGuide\n");
	   if(!printOnlyMeshData){
	      fprintf(EMMFILE, "  meshRefinement  %f \n", vol->meshRefinement);
	   }
	   break;
     case LINEPORT:
	   fprintf(EMMFILE, "  type  LinePort\n");
	   break;
     case BOUNDARYCOND:
	   fprintf(EMMFILE, "  type  BoundaryCondition\n");
	   if(!printOnlyMeshData) fprintf(EMMFILE, "  meshRefinement  %f \n", vol->meshRefinement);
	   break;
     case SPLITTER:
	   fprintf(EMMFILE, "  type  Splitter\n");
	   break;
     case GRID:
	   fprintf(EMMFILE, "  type  Grid\n");
           fprintf(EMMFILE, "  gridNum    %d\n",  vol->gridNum);
           fprintf(EMMFILE, "  PML    %d\n",  vol->PML);
           fprintf(EMMFILE, "  invariant  %d\n",  vol->invariant);
	   break;
     case ASSEMBLY:
	   fprintf(EMMFILE, "  type  Assembly\n");
	   break;
     case UNDEFINED:
	   fprintf(EMMFILE, "  type  Undefined\n");
	   break;
  }
  if(strcmp(vol->material,"?"))
      fprintf(EMMFILE, "  material \"%s\"\n",  vol->material);
  if(vol->TEMportsNum>0)
      fprintf(EMMFILE, "  TEMportsNum    %d\n",  vol->TEMportsNum);
  if(vol->TEportsNum>0)
      fprintf(EMMFILE, "  TEportsNum    %d\n",  vol->TEportsNum);
  if(vol->TMportsNum>0)
      fprintf(EMMFILE, "  TMportsNum    %d\n",  vol->TMportsNum);
  if(vol->disabled)
      fprintf(EMMFILE, "  disabled\n");
  fprintf(EMMFILE, "}\n\n");
}


void EmProblem::save(FILE *fid, bool onlyMesh, bool onlyWG, std::string WGprefix)
{
// write emm file
   printOnlyMeshData=onlyMesh;
   printOnlyWgData=onlyWG;
   EMMFILE=fid;
   _mwm_print_units();
   if(!printOnlyMeshData) 
      Tree_Action(invariants,  _mwm_print_invariants);
   fprintf(EMMFILE, "\n");
   Tree_Action(materials,   _mwm_print_material);
   fprintf(EMMFILE, "\n");
   WGPREFIX=WGprefix;
   Tree_Action(volumes,     _mwm_print_volume);
}

void EmProblem::saveMaterials(FILE *fid)
{
// write emm file
   EMMFILE=fid;
   Tree_Action(materials,   _mwm_print_material);
}






};//namespace
 
 
