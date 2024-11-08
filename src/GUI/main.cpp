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


#include <QApplication>

#include <iostream>	

#include "mainwindow.h"
#include "DataBase.h"
#include "OStools.h"

#include <Standard.hxx>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

MainWindow *mainWindow;

std::string configdir;

std::string proxy_filepath;
std::string api_pem_filepath;
std::string account_filepath;
std::string config_filepath;
std::string EmCADuser_filepath;
std::map<std::string, std::string> emcadConfig;
std::map<std::string, std::string> emcadAccount;
std::map<std::string, std::string> EmCADuser;

char emcadPath[256];
int useAWS=0;
int useServer=0;
int useAPI=1;
int useDOCKER=0;

extern int modeldebug;
extern int model_flex_debug;
extern int touchstonedebug;
extern int touchstone_flex_debug;
extern int projectDatadebug;

namespace DB {
 char yyFileName[256];
 int yyLineNum;
 void yyMsg(int type, const char *fmt, ...){
  va_list args;
  char tmp[1024];
  va_start (args, fmt);
  vsprintf (tmp, fmt, args);
  va_end (args);
  Msg(type, "'%s', line %d : %s", yyFileName, yyLineNum, tmp);
 }
}

bool userHasCredit=false;

extern void api_renew_if_expired(bool wait=true);

#ifdef WNT
#include <windows.h>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    int argc=0; char **argv=NULL;
    GetModuleFileName(NULL, emcadPath, 256);

	/*
	char* casroot = getenv("CASROOT");
	char resource[1000];
	strcpy(resource, casroot);
	strcat(resource, "\\src\\StdResource");
	_putenv_s("CSF_StandardDefaults", resource);
	_putenv_s("CSF_XCAFDefaults", resource);
	_putenv_s("CSF_PluginDefaults", resource);
	*/

#else
#include "unistd.h"
int main(int argc, char *argv[])
{
    readlink("/proc/self/exe", emcadPath, 256);
#endif

    projectDatadebug=0;
    modeldebug=0;
    model_flex_debug=0;
    touchstonedebug=0;
    touchstone_flex_debug=0;
//    TCollection_ExtendedString currentdir("./");
//    myApp->SetDefaultFolder(currentdir.ToExtString());

    #ifdef WNT
    char *home=getenv("USERPROFILE");
    configdir=std::string(home)+std::string("/EmCAD");
    #else
    char *home=getenv("HOME");
    configdir=std::string(home)+std::string("/.config/EmCAD");
    #endif

    if(home==NULL) return 1;

    if(!FileExists(configdir.c_str())) createdir(configdir.c_str());

    config_filepath=configdir+"/emcad_config";
    if(FileExists(config_filepath.c_str())) ReadFile(config_filepath, &emcadConfig);

    proxy_filepath=configdir+"/proxy.pac";
    api_pem_filepath=configdir+"/api-hierarchical-electromagnetics-com-bundle.pem";

    account_filepath=configdir+"/emcad_account";
    if(FileExists(account_filepath.c_str())){
	ReadFile(account_filepath, &emcadAccount);
        if(emcadAccount[std::string("credit")].empty()) emcadAccount[std::string("credit")]="0";
        float credit=std::stof(emcadAccount[std::string("credit")]);
        userHasCredit=credit>1.0;
    }

    EmCADuser_filepath=configdir+"/emcad_user";
    if(FileExists(EmCADuser_filepath.c_str())) ReadFile(EmCADuser_filepath, &EmCADuser);

    useAPI=1; useAWS=0;
    if(emcadConfig.find(std::string("modeler"))!=emcadConfig.end()){
      useAPI=emcadConfig[std::string("modeler")]==std::string("api");
      useAWS=emcadConfig[std::string("modeler")]==std::string("aws");
      useServer=emcadConfig[std::string("modeler")]==std::string("lan");
      useDOCKER=useServer || emcadConfig[std::string("modeler")]==std::string("docker");
    }

    bool useGUI = true;
    QApplication app( argc, argv, useGUI);

    mainWindow=new MainWindow;
    mainWindow->show();
    if(useAPI && FileExists(EmCADuser_filepath.c_str())){
	mainWindow->api_renew_if_expired(false);
    }
    int ierr= app.exec();
    delete mainWindow;
    return ierr;
}
