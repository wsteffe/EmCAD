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
#include <sys/stat.h>
#include <string.h>

MainWindow *mainWindow;

std::string account_filename;
std::string config_filename;
std::string botoConfig_filename;
std::map<std::string, std::string> emcadConfig;
std::map<std::string, std::string> emcadAccount;
std::map<std::string, std::string> botoConfig;

char emcadPath[256];
extern int useAWS;
extern int useServer;
extern int useAPI;
extern int useDOCKER;

extern int modeldebug;
extern int model_flex_debug;
extern int touchstonedebug;
extern int touchstone_flex_debug;

void WriteBoto();

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

    modeldebug=0;
    model_flex_debug=0;
    touchstonedebug=0;
    touchstone_flex_debug=0;
//    TCollection_ExtendedString currentdir("./");
//    myApp->SetDefaultFolder(currentdir.ToExtString());

    #ifdef WNT
    char *home=getenv("USERPROFILE");
    #else
    char *home=getenv("HOME");
    #endif

    if(home==NULL) return 1;

    std::string homedir=std::string(home);

    struct stat configInfo;
    config_filename=homedir+"/.emcad";
    if(!stat(config_filename.c_str(), &configInfo)) ReadFile(config_filename, &emcadConfig);

    struct stat accountInfo;
    account_filename=homedir+"/.emcad_account";
    if(!stat(account_filename.c_str(), &accountInfo)) ReadFile(account_filename, &emcadAccount);

    struct stat botoConfigInfo;
    botoConfig_filename=homedir+"/.boto";
    if(!stat(botoConfig_filename.c_str(), &botoConfigInfo)) ReadFile(botoConfig_filename, &botoConfig);

    useAWS=emcadConfig[std::string("modeler")]==std::string("aws");
    useAPI=emcadConfig[std::string("modeler")]==std::string("api");
    useServer=emcadConfig[std::string("modeler")]==std::string("lan");
    useDOCKER=useServer || emcadConfig[std::string("modeler")]==std::string("docker");

    if(emcadAccount[std::string("credit")].empty()) emcadAccount[std::string("credit")]="0";

#ifdef WNT
    std::string cacertsPath=emcadPath; 
    cacertsPath=cacertsPath.substr (0, cacertsPath.find("bin\\emcad.exe"));
    cacertsPath+="aws\\cacerts.txt";
    cacertsPath=nativePath(cacertsPath);
    if(botoConfig[std::string("ca_certificates_file")]!=cacertsPath){
      botoConfig[std::string("ca_certificates_file")]=cacertsPath;
      WriteBoto();
    }
#endif

    bool useGUI = true;
    QApplication app( argc, argv, useGUI);

    mainWindow=new MainWindow;
    mainWindow->show();
    int ierr= app.exec();
    delete mainWindow;
    return ierr;
}
