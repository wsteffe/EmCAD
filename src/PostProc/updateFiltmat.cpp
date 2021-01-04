/******************************************************************
*
*	Microwave Modeler
*
*	Copyright (C) walter steffè  2005-2007
*
*	File:	updateFiltmat.cpp
*
******************************************************************/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <OStools.h>
#include <vector>
#include <map>

char fileNameRoot[256];
int FREQUNITE=0;

int main(int argc, char **argv)
{

    char filename[256],ext[5];
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
    char varfilename[256];
    strcpy(varfilename,fileNameRoot);
    strcat(varfilename,"_xpar.txt");

    char tmpfilename[256];
    strcpy(tmpfilename,fileNameRoot);
    strcat(tmpfilename,".tmp");


    std::vector<double > variables;
    if(!FileExists(varfilename)){
        fprintf(stderr, "file %s not found\n", varfilename);
	return 1;
    }
    if(!FileExists(filename)){
        fprintf(stderr, "file %s not found\n", filename);
	return 1;
    }

    {
     FILE *fid=fopen(nativePath(varfilename).c_str(), "r");
     char a;
     while(fscanf(fid,"%c",&a)!=EOF){
      char varname[10]; double var;
      fscanf(fid,"%s : %lf\n",varname,&var);
      variables.push_back(var);
     }
     fclose(fid);
    }
    {
     FILE *fid=fopen(nativePath(filename).c_str(), "r");
     FILE *fod=fopen(nativePath(tmpfilename).c_str(), "w");
     char comment[10];
     int resonNum;
     fscanf(fid," %d",&resonNum);
     fprintf(fod," %d",resonNum);
     fscanf(fid,"%s",comment);
     fprintf(fod," %s\n",comment);
     for (int i=0; i<resonNum; i++){
      double f;
      fscanf(fid," %lf\n",&f);
      if(variables.size()>i) f=variables[i];
      fprintf(fod," %.10lf\n",f);
     }
     int impedanceNum;
     fscanf(fid," %d",&impedanceNum);
     fprintf(fod," %d",impedanceNum);
     fscanf(fid,"%s",comment);
     fprintf(fod," %s\n",comment);
     for (int i=0; i<impedanceNum; i++){
      double z;
      fscanf(fid," %lf\n",&z);
      fprintf(fod," %.10lf\n",z);
     }
     int Jnum; fscanf(fid," %d",&Jnum);
     fprintf(fod," %d",Jnum);
     fscanf(fid,"%s",comment);
     fprintf(fod," %s\n",comment);
     for (int i=0; i<Jnum; i++){
      int ip,jp;
      double par;
      int ivar;
      fscanf(fid," %d %d %lf %d\n",&ip, &jp, &par, &ivar);
      if(ivar>0 && variables.size()>ivar-1) par=variables[ivar-1];
      fprintf(fod," %d %d %.10lf %d\n",ip, jp, par, ivar);
     }
     int CKnum; fscanf(fid," %d",&CKnum);
     fprintf(fod," %d",CKnum);
     fscanf(fid,"%s",comment);
     fprintf(fod," %s\n",comment);
     for (int i=0; i<CKnum; i++){
      int ip,jp;
      double par;
      int ivar;
      fscanf(fid," %d %d %lf %d\n",&ip, &jp, &par, &ivar);
      if(ivar>0 && variables.size()>ivar-1) par=variables[ivar-1];
      fprintf(fod," %d %d %.10lf %d\n",ip, jp, par, ivar);
     }
     int LKnum; fscanf(fid,"%d",&LKnum);
     fprintf(fod," %d",LKnum);
     fscanf(fid,"%s",comment);
     fprintf(fod," %s\n",comment);
     for (int i=0; i<LKnum; i++){
      int ip,jp;
      double par;
      int ivar;
      fscanf(fid," %d %d %lf %d\n",&ip, &jp, &par, &ivar);
      if(ivar>0 && variables.size()>ivar-1) par=variables[ivar-1];
      fprintf(fod," %d %d %.10lf %d\n",ip, jp, par, ivar);
     }
     int Gnum; fscanf(fid," %d",&Gnum);
     fprintf(fod," %d",Gnum);
     fscanf(fid,"%s",comment);
     fprintf(fod," %s\n",comment);
     for (int i=0; i<Gnum; i++){
      int ip,jp;
      double par;
      int ivar;
      fscanf(fid," %d %d %lf %d\n",&ip, &jp, &par, &ivar);
      if(ivar>0 && variables.size()>ivar-1) par=variables[ivar-1];
      fprintf(fod," %d %d %.10lf %d\n",ip, jp, par, ivar);
     }
     fclose(fid);
     fclose(fod);
    }
    char cmd[256];
    strcpy(cmd,"mv ");
    strcat(cmd, tmpfilename);
    strcat(cmd, " ");
    strcat(cmd, filename);
    system(cmd);
    return 0;
}



