/******************************************************************
*
*	Microwave Modeler
*
*	Copyright (C) walter steffè  2005-2020
*
*	File:	filtmat2jc.cpp
*
******************************************************************/

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <Message.h>
#include <OStools.h>
#include <vector>
#include <map>

char fileNameRoot[256];
int FREQUNITE=0;
int makexpar=0;

int main(int argc, char **argv)
{

    char filename[256],ext[5];
//    char outfilename[256];
    for(int i=1; i<argc-1; i++) for(int l=0; l<strlen(argv[i]); l++) argv[i][l]=tolower(argv[i][l]); 

    int i = 1;
    while (i < argc-1) {
      if      (   !strcmp(argv[i], "-frequnit") )   {
	 i++;
         if      (!strcmp(argv[i],  "hz"))   {FREQUNITE=0; i++;}
         else if (!strcmp(argv[i], "khz"))   {FREQUNITE=3; i++;}
         else if (!strcmp(argv[i], "mhz"))   {FREQUNITE=6; i++;}
         else if (!strcmp(argv[i], "ghz"))   {FREQUNITE=9; i++;}

      }
      else if (   !strcmp(argv[i], "-makexpar") )   {
	 i++; makexpar=1;
      }
      else {
        fprintf(stderr, "\n  Sintax Error with the Agrument: \"%s\" \n\n", argv[i]);
        return 1;
      }
    }

    strncpy(filename,argv[argc-1],256-5);
    strncpy(ext, &filename[strlen(filename) - 4], 5);
    if(strcmp(ext, ".dat")) {
        fprintf(stderr, "\n  filename must have \".dat\" extension\n\n");
        return 1;
    }
    strncpy(fileNameRoot,filename,strlen(filename) - 4);

    char outfilename[256];
    strcpy(outfilename,fileNameRoot);
    strcat(outfilename,".JC");

    char xparfilename[256];
    strcpy(xparfilename,fileNameRoot);
    strcat(xparfilename,"_xpar.txt");

    std::map< std::pair<int,int>, double > idealFilterJ;
    std::map< std::pair<int,int>, double > idealFilterCK;
    std::map< std::pair<int,int>, double > idealFilterLK;
    std::map< std::pair<int,int>, double > idealFilterG;
    std::vector<double> idealFilterResonFreq;
    std::vector<double> idealFilterImpedance;
    std::vector<double > xpar;

    if(!FileExists(filename)){
        fprintf(stderr, "file %s not found\n", filename);
	return 1;
    }
    int resonNum,impedanceNum;
    {
     FILE *fid=fopen(nativePath(filename).c_str(), "r");
     char comment[10];
     fscanf(fid,"%d ",&resonNum);
     fscanf(fid,"%[^\n]s",comment);
     for (int i=0; i<resonNum; i++){
      double f;
      fscanf(fid,"%lf\n",&f);
      idealFilterResonFreq.push_back(f);
      xpar.push_back(f);
     }
     fscanf(fid," %d ",&impedanceNum);
     fscanf(fid,"%[^\n]s",comment);
     for (int i=0; i<impedanceNum; i++){
      double z;
      fscanf(fid," %lf\n",&z);
      idealFilterImpedance.push_back(z);
     }
     int Jnum; fscanf(fid,"%d ",&Jnum);
     fscanf(fid,"%[^\n]s",comment);
     for (int i=0; i<Jnum; i++){
      int ip,jp;
      double par;
      int ivar;
      fscanf(fid," %d %d %lf %d\n",&ip, &jp, &par, &ivar);
      idealFilterJ[std::pair<int,int>(ip,jp)]=par;
      if(makexpar){
       int nx=xpar.size();
       for (int j=nx; j<ivar; j++) xpar.push_back(0.0);
       if(ivar>0) xpar[ivar-1]=par;
      }
     }
     int CKnum; fscanf(fid," %d ",&CKnum);
     fscanf(fid,"%[^\n]s",comment);
     for (int i=0; i<CKnum; i++){
      int ip,jp;
      double par;
      int ivar;
      fscanf(fid," %d %d %lf %d\n",&ip, &jp, &par, &ivar);
      idealFilterCK[std::pair<int,int>(ip,jp)]=par;
      if(makexpar){
       int nx=xpar.size();
       for (int j=nx; j<ivar; j++) xpar.push_back(0.0);
       if(ivar>0) xpar[ivar-1]=par;
      }
     }
     int LKnum; fscanf(fid," %d ",&LKnum);
     fscanf(fid,"%[^\n]s",comment);
     for (int i=0; i<LKnum; i++){
      int ip,jp;
      double par;
      int ivar;
      fscanf(fid," %d %d %lf %d\n",&ip, &jp, &par, &ivar);
      idealFilterLK[std::pair<int,int>(ip,jp)]=par;
      if(makexpar){
       int nx=xpar.size();
       for (int j=nx; j<ivar; j++) xpar.push_back(0.0);
       if(ivar>0) xpar[ivar-1]=par;
      }
     }
     int Gnum; fscanf(fid,"%d ",&Gnum);
     fscanf(fid,"%[^\n]s",comment);
     for (int i=0; i<Gnum; i++){
      int ip,jp;
      double par;
      int ivar;
      fscanf(fid," %d %d %lf %d\n",&ip, &jp, &par, &ivar);
      idealFilterG[std::pair<int,int>(ip,jp)]=par;
      if(makexpar){
       int nx=xpar.size();
       for (int j=nx; j<ivar; j++) xpar.push_back(0.0);
       if(ivar>0) xpar[ivar-1]=par;
      }
     }
     fclose(fid);
    }
    {
     typedef std::map< std::pair<int, int>, double >::iterator SparseMatIterator;
     FILE *fid=fopen(nativePath(outfilename).c_str(), "w");
     fprintf(fid, " \n.subckt  ideal_filter  Wg1 Wg2\n");
     fprintf(fid, "\n");
     fprintf(fid, " Kt  Wg1  0.000\n");
     fprintf(fid, " Kt  Wg2  0.000\n");
     fprintf(fid, " Zc  Wg1  %lf\n", idealFilterImpedance[resonNum+0]);
     fprintf(fid, " Zc  Wg2  %lf\n", idealFilterImpedance[resonNum+1]);
     fprintf(fid, "\n");
     std::vector<double> C(resonNum),Ci(resonNum);
     for ( int i=0; i< resonNum; i++){
      double e=FREQUNITE;
      double CC=1.0/(2*M_PI*idealFilterResonFreq[i]*pow(10.0,e));
      C[i] =CC/sqrt(idealFilterImpedance[i]);
      Ci[i]=CC*sqrt(idealFilterImpedance[i]);
      fprintf(fid, " C  k%d k%d  %.10e\n", i+1, i+1, C[i]);
      fprintf(fid, " C  ki%d ki%d  %.10e\n", i+1, i+1, Ci[i]);
     }
     fprintf(fid, "\n");
     for ( int i=0; i< resonNum; i++){
      fprintf(fid, " J  k%d ki%d  1.0\n", i+1, i+1);
     }
     fprintf(fid, "\n");
     for ( SparseMatIterator it=idealFilterJ.begin(); it!= idealFilterJ.end(); it++){
      int i=it->first.first;
      int j=it->first.second;
      double Jinv=it->second;
      if(i<=impedanceNum) Jinv*=sqrt(idealFilterImpedance[i-1]);
      if(j<=impedanceNum) Jinv*=sqrt(idealFilterImpedance[j-1]);
      if(i<=resonNum && j<=resonNum) fprintf(fid, " J  k%d k%d  %.10g\n", i, j, Jinv);
      else if(i<=resonNum)           fprintf(fid, " J  k%d Wg%d  %.10g\n", i, j-resonNum, Jinv);
      else if(j<=resonNum)           fprintf(fid, " J  Wg%d k%d  %.10g\n", i-resonNum, j, Jinv);
     }
     if(idealFilterCK.size()>0 || idealFilterLK.size()>0) fprintf(fid, "\n");
     for ( SparseMatIterator it=idealFilterCK.begin(); it!= idealFilterCK.end(); it++){
      int i=it->first.first;
      int j=it->first.second;
      double Ck=(it->second);
      if(i<=resonNum) Ck*=sqrt(C[i-1]);
      if(j<=resonNum) Ck*=sqrt(C[j-1]);
      fprintf(fid, " C  k%d k%d  %.10e\n", i, j, Ck);
     }
     for ( SparseMatIterator it=idealFilterLK.begin(); it!= idealFilterLK.end(); it++){
      int i=it->first.first;
      int j=it->first.second;
      double Lk=(it->second);
      if(i<=resonNum) Lk*=sqrt(Ci[i-1]);
      if(j<=resonNum) Lk*=sqrt(Ci[j-1]);
      fprintf(fid, " C  ki%d ki%d  %.10e\n", i, j, Lk);
     }
     fprintf(fid, " \n.include  extern_port_loads.JC\n");
     fprintf(fid, " \n.include  port_impedance_circuits.JC\n");
     fprintf(fid, " \n.ends  symmfilter\n");
     fclose(fid);
     if(idealFilterG.size()>0) fprintf(fid, "\n");
     for ( SparseMatIterator it=idealFilterG.begin(); it!= idealFilterG.end(); it++){
      int i=it->first.first;
      int j=it->first.second;
      double G=it->second;
      if(i<=impedanceNum) G*=sqrt(idealFilterImpedance[i-1]);
      if(j<=impedanceNum) G*=sqrt(idealFilterImpedance[j-1]);
      if(i<=resonNum && j<=resonNum) fprintf(fid, " G  k%d k%d  %.10g\n", i, j, G);
      else if(i<=resonNum)           fprintf(fid, " G  k%d Wg%d  %.10g\n", i, j-resonNum, G);
      else if(j<=resonNum)           fprintf(fid, " G  Wg%d k%d  %.10g\n", i-resonNum, j, G);
     }
    }
    if(makexpar){
     typedef std::map< std::pair<int, int>, double >::iterator SparseMatIterator;
     FILE *fid=fopen(nativePath(xparfilename).c_str(), "w");
     for ( int i=0; i< xpar.size(); i++)
	 fprintf(fid," var_%d : %.10lf\n",i+1,xpar[i]);
    }
    return 0;
}



