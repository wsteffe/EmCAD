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


#ifdef INTERNAL_PYTHON
#include <Python.h>
#endif

#if defined(HAVE_OCE_CONFIG_H)
 #include <oce-config.h>
#elif defined(HAVE_OCC_CONFIG_H)
 #include <config.h>
#endif

#include "mainwindow.h"
#include "projectData.h"
#include "ocaf.h"
#include "Message.h"
//#include "Tools.h"

#include <TDataStd_Name.hxx>

#if defined(QT5)
 #include <QtGui>
#endif
#include <QStatusBar>
#include <QMenuBar>
#include <QMessageBox>
#include <QHeaderView>
#include <QFileDialog>
#include <QUrl>
#include <QDesktopServices>
#include <QtCore/QTextStream>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QDockWidget>
#include <QToolBar>
#include <QApplication>
#include <QTextDocument>
#include <QTextStream>
#include <QRegExp>
#include <QPen>
#include <QIcon>
#include <QPixmap>

//#include "PJTabScrollView.h"

#include <Aspect_RectangularGrid.hxx>

#include <ViewWidget.h>
#include <InputOutput.h>
#include <OStools.h>

#include <TDocStd_Document.hxx>
#include <XmlXCAFDrivers_DocumentRetrievalDriver.hxx>
#include <XmlXCAFDrivers_DocumentStorageDriver.hxx>

#include <V3d_View.hxx>
#include <AIS_Point.hxx>
#include <AIS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>

#include "DataBase.h"
#include "assert.h"
#include "stdlib.h"


#ifdef WNT
#include <io.h>
#endif

#include <string>

#include <omp.h>


#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifdef WNT
    #include <windows.h>
    void msleep(unsigned milliseconds)
    {
        Sleep(milliseconds);
    }
#else
    #include <unistd.h>
    void msleep(unsigned milliseconds)
    {
        usleep(milliseconds * 1000); // takes microseconds
    }
#endif

#define USEXMLXCAF 1
#define INVERTEDTEPORTS 1
#define INVERTEDTEMPORTS 1
#define INVERTEDTMPORTS 1

#define NPARJOBSONSERVER 12

DB::Material theDefaultMaterial;

extern std::string account_filename;
extern std::string config_filename;
extern std::string botoConfig_filename;

extern char emcadPath[256];
extern std::map<std::string, std::string> emcadConfig;
extern std::map<std::string, std::string> emcadAccount;
extern std::map<std::string, std::string> botoConfig;

int useAWS=0;
int useServer=0;
int useDOCKER=0;
int useAPI=0;



int MESH3D_ON_SERVER=1;

bool componentIsAssembly(){ return (emcadConfig[std::string("ComponentIsAssembly")]!=std::string("false"));}

ViewWidget*    mainOCC;
MwOCAF*       mainOCAF;
extern DB::Units unit;

QString  mainWorkPath;
ProjectData prjData;
Documentation documentation;
RunningStatus runStatus;


void ltrim(std::string& str) {
  std::string::size_type pos = 0;
  while (pos < str.size() && isspace(str[pos])) pos++;
  str.erase(0, pos);
}

void rtrim(std::string& str) {
  std::string::size_type pos = str.size();
  while (pos > 0 && isspace(str[pos - 1])) pos--;
  str.erase(pos);
}

void trim(std::string& str) {
  ltrim(str);
  rtrim(str);
}

QString nativePath(QString path){
return QString(nativePath(path.toLatin1().data()).c_str());
}

template<typename T>
void StringMap<T>::read(const char*filename)
{
    std::ifstream in;
    in.open(nativePath(filename).c_str());
    if(!in) return;
    std::string line;
    while (!getLine(in, line).eof())  {
	T n;
	std::string str;
	in>>n; getLine(in, str);
	if(map.find(line)==map.end()) if(!line.empty()) map[line]=n;
    }
    in.close();
}
template<typename T>
void StringMap<T>::save(const char*filename)
{
    std::ofstream out;
    out.open(nativePath(filename).c_str());
    typedef typename std::map<std::string, T, std::less<std::string> >::const_iterator MapIt;
    for ( MapIt it=map.begin(); it!= map.end(); it++) { 
		std::string tmp=(*it).first;
		out << tmp.c_str(); out <<std::endl;
		out <<(*it).second; out <<std::endl;
    }
    out.close();
}

enum PortType {PortTypeTE=1, PortTypeTM=2, PortTypeTEM=3};
PortType portTypeFromName(const char *name){
 int l;
 for(l=strlen(name)-1; l>0&&((!isalpha(name[l])||name[l]=='i')); ) l--;
 if( l>0 && !strncmp(&name[l-1],"TE", 2))  return PortTypeTE;
 if( l>0 && !strncmp(&name[l-1],"TM", 2))  return PortTypeTM;
 if( l>1 && !strncmp(&name[l-2],"TEM",3))  return PortTypeTEM;
 return 3;  //TEM is the default
};


int ReadFile(std::string fname, std::map<std::string, std::string> *m)
{
    std::ifstream fin(nativePath(fname).c_str(),std::ios::in);
    if (fin.fail()) return -1;
     
    m->clear();
    std::string line;
    int count = 0;
    while(std::getline(fin, line)){ 
	std::string::size_type pos = line.find_first_of('=');
	if(pos==std::string::npos) continue;
        std::string s1 = line.substr(0, pos); trim(s1);
	std::string s2 = line.substr(pos+1); trim(s2);
        (*m)[s1] = s2;
        count++;
    }
     
    fin.close();
    return count;
}

int WriteFile(std::string fname, std::map<std::string, std::string> *m){
    int count = 0;
    if (m->empty()) return 0;
    if (fname.empty()) return 0;

    FILE *fp = fopen(nativePath(fname).c_str(), "w");

    if (!fp) return -1;

    for(std::map<std::string, std::string>::iterator it = m->begin(); it != m->end(); it++) {
        fprintf(fp, "%s=%s\n", it->first.c_str(), it->second.c_str());
        count++;
    }

    fclose(fp);

    return count;
}

void printMapEntry(char *name, FILE *fp, std::map<std::string, std::string> &m)
{
    std::string key=name;
    std::map<std::string, std::string>::iterator it=m.find(key);
    if(it!=m.end())  fprintf(fp, "%s=%s\n", it->first.c_str(), it->second.c_str());
}


void WriteBoto(){
if (botoConfig_filename.empty()) return;
    FILE *fp = fopen(nativePath(botoConfig_filename).c_str(), "w");
    fprintf(fp,"[Credentials]\n");
    printMapEntry("aws_access_key_id", fp, botoConfig);
    printMapEntry("aws_secret_access_key", fp, botoConfig);
    fprintf(fp,"[Boto]\n");
    printMapEntry("ec2_region_name", fp, botoConfig);
    printMapEntry("ec2_region_endpoint", fp, botoConfig);
#ifdef WNT
    printMapEntry("ca_certificates_file", fp, botoConfig);
#endif
    fclose(fp);
}

void StringList::read(const char*filename)
{
    std::ifstream in;
    in.open(nativePath(filename).c_str());
    if(!in) return;
    std::string line;
    while (!getLine(in, line).eof())  if(list.find(line)==list.end()) if(!line.empty()) list.insert(line);
    in.close();
}
void StringList::save(const char*filename)
{
    std::ofstream out;
    out.open(nativePath(filename).c_str());
    typedef std::set<std::string, std::less<std::string> >::const_iterator StrIt;
    for ( StrIt it=list.begin(); it!= list.end(); it++) { 
		std::string tmp=*it;
		out << tmp.c_str(); out <<std::endl;
    }
    out.close();
}



#ifdef INTERNAL_PYTHON

void remove_dir(QString dir)
{
  QString py_import="import shutil\n";
  QString py_remove="shutil.rmtree('"+dir+"')\n";
  QString py_script=py_import+py_remove;
  Py_Initialize();
  PyRun_SimpleString(py_script.toLatin1().data());
  Py_Finalize();
}

void tar_extract(QString tarFile, QString path)
{
  QString py_import="import tarfile\n";
  QString py_tarfile="tar = tarfile.open('"+tarFile+"', 'r:gz')\n";
  QString py_extract="tar.extractall(path='"+path+"')\n";
  QString py_close="tar.close()\n";
  QString py_script=py_import+py_tarfile+py_extract+py_close;
  Py_Initialize();
  PyRun_SimpleString(py_script.toLatin1().data());
  Py_Finalize();
}

void tar_create(QString tarFile, QString wkDir, QString dir)
{
  QString py_import="import tarfile,os\n";
  QString py_startdir="start_dir=os.getcwd()\n";
  QString py_chdir="os.chdir('"+wkDir+"')\n";
  QString py_tarfile="tar = tarfile.open('"+tarFile+"', 'w:gz')\n";
  QString py_add="tar.add('"+dir+"')\n";
  QString py_close="tar.close()\n";
  QString py_backdir="os.chdir(start_dir)\n";
  QString py_script=py_import+py_startdir+py_chdir+py_tarfile+py_add+py_close+py_backdir;
  Py_Initialize();
  PyRun_SimpleString(py_script.toLatin1().data());
  Py_Finalize();
}

#else


void remove_dir(QString dir)
{
    QString script=QString(emcadPath);
    #ifdef WNT
       script.chop(13);
       QString ext=".exe";
    #else
       script.chop(9);
       QString ext=".py";
    #endif
    script+="bin/remove_dir";
    script=nativePath(script+ext);

    QProcess *proc=new QProcess;
    QString app=script;
    QStringList args;
    args << dir;
    QString Cmd=app+QString("  ")+args.join(QString(" "));
    char * cmd=Cmd.toLatin1().data();
    proc->start(app, args);
    proc->waitForStarted();
    proc->waitForFinished(-1);
    msleep(100);

}


void create_touchstone1(QString filepath)
{
    QString script=QString(emcadPath);
    #ifdef WNT
       script.chop(13);
       QString ext=".exe";
    #else
       script.chop(9);
       QString ext=".py";
    #endif
    script+="bin/touchstone2to1";
    script=nativePath(script+ext);

    QProcess *proc=new QProcess;
    QString app=script;
    QStringList args;
    args << filepath;
    QString Cmd=app+QString("  ")+args.join(QString(" "));
    char cmd[512]; strcpy(cmd,Cmd.toLatin1().data());
    proc->start(app, args);
    proc->waitForStarted();
    proc->waitForFinished(-1);
    msleep(100);

}



void tar_extract(QString tarFile, QString dir)
{
    QString script=QString(emcadPath);
    #ifdef WNT
         script.chop(13);
	 QString ext=".exe";
    #else
         script.chop(9);
	 QString ext=".py";
    #endif

    script+="bin/tar_extract";
    script=nativePath(script+ext);

    QProcess *proc=new QProcess;
    QString app=script;
    QStringList args;
    args << tarFile;
    args << dir;
    QString Cmd=app+QString("  ")+args.join(QString(" "));
    char * cmd=Cmd.toLatin1().data();
    proc->start(app, args);
    proc->waitForStarted();
    proc->waitForFinished(-1);
    msleep(100);
}

void tar_create(QString tarFile, QString path, QString dir)
{
    QString script;
    script=QString(emcadPath);
    #ifdef WNT
       script.chop(13);
       QString ext=".exe";
    #else
       script.chop(9);
       QString ext=".py";
    #endif
    script+="bin/tar_create";
    script=nativePath(script+ext);

    QProcess *proc=new QProcess;
    QString app=script;
    QStringList args;
    args << tarFile;
    args << path;
    args << dir;
    QString Cmd=app+QString("  ")+args.join(QString(" "));
    char * cmd=Cmd.toLatin1().data();
    proc->start(app, args);
    proc->waitForStarted();
    proc->waitForFinished(-1);
    msleep(100);

}

int getAssType(QString dir)
{
  QString asstypeFileName=dir+"/assemblyType";
  FILE *in= fopen(nativePath(asstypeFileName.toLatin1().data()).c_str(), "r");
  if(!in) return 0;
  int assType;
  fscanf(in, "%d", &assType);
  fclose(in);
  return assType;
}

int getSubCompNum(QString dir)
{
  QString asstypeFileName=dir+"/subcompNum";
  FILE *in= fopen(nativePath(asstypeFileName.toLatin1().data()).c_str(), "r");
  if(!in) return 0;
  int subcompNum;
  fscanf(in, "%d", &subcompNum);
  fclose(in);
  return subcompNum;
}


void tar_component(QString compDir, QString compName)
{
    QString script;
    script=QString(emcadPath);
    #ifdef WNT
       script.chop(13);
       QString ext=".exe";
    #else
       script.chop(9);
       QString ext=".py";
    #endif
    script+="bin/tar_component";
    script=nativePath(script+ext);

    int subCompNum=getSubCompNum(compDir);

    QString tarFileDir=mainWorkPath+QString("/Data/Circuits/");
    QString str_subCompNum; str_subCompNum.setNum(subCompNum);

    QProcess *proc=new QProcess;
    QString app=script;
    QStringList args;
    args << compName;
    args << compDir;
    args << str_subCompNum;
    args << tarFileDir;
    QString Cmd=app+QString("  ")+args.join(QString(" "));
    char * cmd=Cmd.toLatin1().data();
    proc->start(app, args);
    proc->waitForStarted();
    proc->waitForFinished(-1);
    msleep(100);

}


#endif

EMClogger::EMClogger(){
}
QSize  EMClogger::sizeHint() const
{
     QSize size = QPlainTextEdit::sizeHint();
     size.rheight() = 100;
//     size.rwidth() = qMax(size.width(), size.height());
     return size;
}


void AddVertex (double x, double y, double z, Handle(AIS_InteractiveContext) theContext)
{
	TopoDS_Vertex aVertex=BRepBuilderAPI_MakeVertex( gp_Pnt(x,y,z) );
	Handle(AIS_Shape) AISVertex = new AIS_Shape(aVertex);
	// context is the handle to an AIS_InteractiveContext object.
	theContext->Display(AISVertex,true); 
}
void ShowOrigin ( Handle(AIS_InteractiveContext) theContext )
{
	AddVertex ( 0.0, 0.0, 0.0, theContext);	
}


QString getFileName(const QString &fullFileName)
{
     return QFileInfo(fullFileName).fileName();
}
QString getFileBaseName(const QString &fullFileName)
{
     return QFileInfo(fullFileName).baseName();
}
QString getFilePath(const QString &fullFileName)
{
     return QFileInfo(fullFileName).path();
}

MainWindow::~MainWindow(){
// NB Qt objects are destructed automatically:
  delete mainOCAF;
  delete mainOCC;

  if (emhelp && emhelp->state() == QProcess::Running) {
         emhelp->terminate();
         emhelp->waitForFinished(3000);
  }
/*
  if (gmshview && gmshview->state() == QProcess::Running) {
         gmshview->terminate();
         gmshview->waitForFinished(3000);
  }
*/
  removeWorkDir();
}

void WorkStatus::reset(){
   reloadNeeded=0;
   decompositionNeeded=0;
   firstDecomposition=1;
   materialChanged=0;
   remeshNeeded=0;
   modelizationNeeded=0;
   componentsaveNeeded=0;
   reductionNeeded=0;
}

WorkStatus::WorkStatus(){
   reset();
}


ProjectData::ProjectData(){
       s2pFilePath="";
       network=0;
       strcpy(lengthUnitName,"MM");
       freqUnitE=9;
       meshPerWavelen=10;
       meshPerCircle=20;
       meshRefineMaxNum=2;
       meshMinEnergyRatio=30.0;
       localMeshing3d=1;
       freqBand[0]=freqBand[1]=0.0;
       refFreqBand[0]=refFreqBand[1]=0.0;
       filterPassBand[0]=filterPassBand[1]=0.0;
       mapFreqBand[0]=mapFreqBand[1]=0.0;
       filterOrder=0;
       anaFreqBand[0]=anaFreqBand[1]=0.0;
       zpFreqBand[0]=zpFreqBand[1]=0.0;
       zpWinRatio=1.0;
       cutoffRatio=2.0;
       zeropoleCurves.clear();
       freqRespParType=SPAR;
       freqRespParPart=0;
       MORFreqNum=4;
       KrylovOrder=1;
       anaMORFreqNum=10;
       anaFreqNum=100;
       filterTuneItermax=1000;
       idealFilterType=CHEBYSHEV;
       idealFilterTopology=SYMMETRIC_TRANSVERSE_LC;
       filtermapSource=ZEROPOLES;
       filtermapSymmetric=1;
       filtermapQfactor=0.0;
       filterTuneMethod=FIXEDGRADIENT;
       filterTuneRecomputeError=1;
       filterTuneRecomputeJaco=1;
       filterSymmetricTuning=1;
       filterTuneOnlyJt=0;
       filterTuneXtol=1.e-4;
       filterTuneTrustR=2.e-1;
       filterTuneGradDx=5.e-2;
       unit.xm=lengthUnit();
       unit.xHz=freqUnit();
       freqRespYscaleAuto=1;
       zeropoleYscaleAuto=1;
       freqRespYscale[0]=-50;
       freqRespYscale[1]= 0;
       freqRespYscale[2]= 5;
       autoFreqResponse=0;
       autoMappedFreqResponse=0;
       autoDesignWithMappedTz=0;
       autoIdealMappedTzFreqResponse=0;
       autoZeropole=0;
       autoMappedZeropole=0;
       autoFilterMapping=0;
       filterOrder=0;
       filterQfactor=0.0;
       filterInductiveSkin=0;
       filterPortImpedance=1.0;
       filterRetLoss=20.0;
       filterOutbandRetLoss=0.0;
       symmFilterResponse=0;
       predistortedFilter=0;
       predistFilterOptim=0;
       predistFilterOptimILpp=0.5;
       predistFilterOptimRL=15;
       predistFilterOptimIterMax=200;
       predistFilterOptimTrustR=0.005;
}

MainWindow::MainWindow()
: myLastFolder(tr(""))
{
	emhelp=NULL;
//	gmshview=NULL;
	runStatus.projectIsOpen=false;
	runStatus.runningDecomposer=false;
	runStatus.runningModeler=false;
	runStatus.runningMesher=false;
	runStatus.runningTuner=false;
//	isComponent=false;
//	isNetwork=false;

        project = new QTextEdit;

        setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );

// OCC Viewer	
	mainOCC = new ViewWidget(this);
        mainOCC->resize ( 600, 500 );
	this->setCentralWidget(mainOCC);
	theAISViewer=NULL;
	ShowOrigin ( mainOCC->getContext() );

// MwOCAF
        mainOCAF=new MwOCAF(mainOCC->getContext());

// Structure Tree	
	treeWidget = new TreeWidget();
	treeWidget->setColumnCount(1);
	treeWidget->setHeaderLabel("");
	treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
//        treeRoot   =new TreeWidgetItem(treeWidget);
        treeRoot =(TreeWidgetItem*) treeWidget->invisibleRootItem();
//	treeWidget->setHeaderItem(treeRoot);


//        connect(treeWidget, SIGNAL(AISchanged()), mainOCC, SLOT(updateContext()));
//        connect(treeWidget, SIGNAL(AISchanged()), mainOCC, SLOT(redraw()));
//        connect(treeWidget, SIGNAL(highlightObject(Handle(AIS_InteractiveObject))), mainOCC, SLOT( highlightObject(Handle(AIS_InteractiveObject))));

//      Plotter
        freqRespPlot = new MwPlot(FREQRESP_PLOT);
        mapped_freqRespPlot = new MwPlot(FREQRESP_PLOT);
        ideal_freqRespPlot = new MwPlot(FREQRESP_PLOT);
        imported_freqRespPlot = new MwPlot(FREQRESP_PLOT);
        ideal_mappedTZ_freqRespPlot = new MwPlot(FREQRESP_PLOT);
        zeroPolePlot = new MwPlot(ZEROPOLE_PLOT);
        mapped_zeroPolePlot = new MwPlot(ZEROPOLE_PLOT);
        ideal_zeroPolePlot = new MwPlot(ZEROPOLE_PLOT);

	
// Project
	projectBar = new QToolBar(this);
	addToolBar(projectBar);
        projectLabel=new QLabel;	
        projectBar->addWidget(projectLabel);
//        QString title= "<font color='red'> ? ";
        QString title;
        projectLabel->setText(title);

// Geometry
	geometry = new QDockWidget("Structure", this);
	geometry->setFeatures(QDockWidget::NoDockWidgetFeatures);
	geometry->setWidget(treeWidget);
	addDockWidget(Qt::LeftDockWidgetArea, geometry);

	
// Messages
        logger=new EMClogger;
	logs = new QDockWidget("logs", this);
	logs->setFeatures(QDockWidget::NoDockWidgetFeatures);
	logs->setWidget(logger);
	addDockWidget(Qt::BottomDockWidgetArea, logs);

        resize ( 800, 600 );

// Menus	
    createActions();
    createMenus();

// statusBar
    statusLabel = new QLabel();
    statusLabel->setFrameStyle(QFrame::Panel);
    statusLabel->setTextFormat(Qt::RichText);
    statusLabel->setFrameStyle(QFrame::Panel);
    statusLabel->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    statusBar()->addPermanentWidget(statusLabel);
    statusBar()->setMinimumHeight(40);

    setWindowTitle(tr("Electromagnetic CAD"));
    setMinimumSize(160, 160);

//	showMaximized();
//

}

void MainWindow::removeWorkDir()
{
  if(!mainWorkPath.isEmpty()){
    QString tmpdir=mainWorkPath;
    tmpdir.chop(5); //remove /work 
    if(FileExists(tmpdir.toLatin1().data())) remove_dir(nativePath(tmpdir));
  }
}


void MainWindow::setWorkProject(const QString wkpath)
{
    currentWorkPath =wkpath;
//    project->document()->setModified(false);
    setWindowModified(false);
//    checkActions();
    mainOCAF->EmP.occFilePath=wkpath.toLatin1().data();
    if(USEXMLXCAF)  mainOCAF->EmP.occFilePath+="/model.xml";
    else            mainOCAF->EmP.occFilePath+="/model.dxc";
    mainOCAF->setProjectDir(wkpath.toLatin1().data());
}

void MainWindow::clearWorkProject()
{
    currentWorkPath ="";
//    projectDir  ="";
    project->document()->setModified(false);
    setWindowModified(false);
    checkActions();
    mainOCAF->EmP.occFilePath="";
}

void MainWindow::setProjectTitle()
{
    QString title;
    if(runStatus.projectIsOpen){
      title +=prjData.projectName;
      QString level; level.setNum(mainOCAF->EmP.level);
      QDir maindir(mainWorkPath);
      QString projectPath=maindir.relativeFilePath(currentWorkPath);
      QString partition="Partition";
      if(projectPath.endsWith(partition)) projectPath.chop(partition.length()+1);
      if(mainOCAF->EmP.level) title +="/"+projectPath;
      if(mainOCAF->isPartition())     if(mainOCAF->subComp) {title += "/Subcomp "; char tag[10]; sprintf(tag,"_%d",mainOCAF->subComp); title +=tag;}
//      title+=";    Status:  ";
      if(!mainOCAF->EmP.hasGeo)                 title += "    Undefined  ";
      if(!mainOCAF->prjStatus.linePorts)        temporaryMessage("missing Line Port at some interface");
 /*     
      else{
       if(!mainOCAF->prjStatus.partMaterials)    title += " Part Materials TBD";
       if(mainOCAF->hasIF)     title += "   Connected";
       else          title += "   Unconnected";
      }
 */
    }

    projectLabel->setText(title);
}


bool MainWindow::maybeSave()
{
     if (project->document()->isModified()) {
         QMessageBox::StandardButton ret;
         ret = QMessageBox::warning(this, tr("Application"),
                      tr("The project has been modified.\n"
                         "Do you want to save your changes?"),
                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
         if (ret == QMessageBox::Save)
             return worksave();
         else if (ret == QMessageBox::Cancel)
             return false;
     }
     return true;
}

//int loadProject(const char *fName);
int loadModel(MwOCAF* ocaf, const char *fName, bool update=false);




void MainWindow::makeTree()
{
	mainOCAF->setMissingNames();
	mainOCAF->makeShapeIndex2Labels();
        TDF_Label mainLabel=mainOCAF->mainDoc->Main();
//	treeWidget->setItem(treeRoot,mainLabel);
	treeWidget->clear();
	treeWidget->makeSubTree(treeRoot,mainLabel);
	treeWidget->updateIndex();
//	treeRoot->setExpanded(true);
        if(!mainOCAF->theParts.IsNull()){
            TreeWidgetItem* shapesit=treeWidget->getItemFromLabel(mainOCAF->theShapes);
	    shapesit->setExpanded(true);
	    mainOCAF->displayAllParts();
	}
	else if(!mainOCAF->theShapes.IsNull()){
            TreeWidgetItem* shapesit=treeWidget->getItemFromLabel(mainOCAF->theShapes);
	    treeWidget->displayItemShape(shapesit);
	}
//        treeWidget->updateText();
}


void MainWindow::open()
{
/*
        QString fullPathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
			                         QDir::currentPath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
*/
	QString	path;
        QStringList filters;
	QFileDialog dialog(this,tr("Open"));
	dialog.setNameFilter("*.emc");
	dialog.setDirectory(QDir::currentPath());
	dialog.setFileMode(QFileDialog::ExistingFile);
	if(dialog.exec()){
           QStringList pathList;
	   pathList = dialog.selectedFiles();
	   path= pathList.at(0);
	}
	if(path.isEmpty()) return;
	savePath=path; 
	QFileInfo fileinfo(path);
        QDir::setCurrent(fileinfo.dir().absolutePath());


//	char tmp[]={"/mnt/vdisk/emwork/EmCAD-XXXXXX"};
	QString tmppath;
        #ifdef WNT
          std::string tmp_str=getenv("TEMP");
	  tmp_str+=std::string("\\EmCAD-XXXXXX");
//	  char tmp[]={"C:\\Temp\\EmCAD-XXXXXX"};
          int l=tmp_str.size()+1;
          auto tmp=new char[l]; strcpy (tmp,tmp_str.c_str());
          int res = _mktemp_s(tmp, l);
          if (res != 0) return;
	  tmppath=tmp;
          QDir tmpdir(tmppath);
          tmpdir.mkdir(tmppath);
          delete [] tmp;
        #else
	  char tmp[]={"/tmp/EmCAD-XXXXXX"};
	  tmppath=mkdtemp(tmp);
        #endif
	if(!FileExists(tmppath.toLatin1().data())) return;
	tar_extract(savePath, tmppath);
        mainWorkPath=tmppath+"/Work";

        runStatus.projectIsOpen=true;

	prjData.readPorts();
	prjData.readPortLoads();

        QString compFileName=mainWorkPath+"/components";
        prjData.components.read(compFileName.toLatin1().data());
        QString wgcompFileName=mainWorkPath+"/wgcomponents";
        prjData.wgcomponents.read(wgcompFileName.toLatin1().data());
        prjData.readWorkStatus();
//	openAllDoc(mainWorkPath);
	prjData.readSettings();
        QFileInfo fileInfo(savePath); 
	prjData.projectName=fileInfo.fileName();
        prjData.projectName.chop(4);
       	workopen(mainWorkPath);
        if(mainOCAF->EmP.hasGeo) updateAllWidgets();
        statusMessage(QString("Ready"));
}


extern int loadProblem(DB::EmProblem* EmP, const char *fName);



bool readStringlist(QString fname, QStringList &subdirs){
 QFile file(fname);
 if (!file.open(QFile::ReadOnly)) return false;
 QTextStream sfin(&file);
 QString line;
 do {
   line = sfin.readLine();
   if (!line.isNull()) subdirs<<line;
 } while (!line.isNull());
 file.close();
 return true;
}

QString genericName(QString name)
{
QString name1=name;
while(name1.size()>1 && name1.at(name1.size()-1).isDigit() || name1.endsWith("_",Qt::CaseInsensitive)) name1.chop(1);
return name1;
}


void MainWindow::setEMC(QString prjDir, QString inDir)
{
  int assType=getAssType(inDir);
  if(assType==NET){
   QMap<QString, int> map,gmap;
   QStringList prjSubdirs, inSubdirs;
   if(!readStringlist(prjDir+"/subdirs", prjSubdirs)) return;
   if(!readStringlist(inDir+"/subdirs", inSubdirs)) return;
   for (int i = 0; i < inSubdirs.size(); ++i){
      QString name=inSubdirs.at(i);
      QString gname=genericName(name);
      map[name]=i+1;
      gmap[gname]=i+1;
   }
   for (int i = 0; i < prjSubdirs.size(); ++i){
      QString subPrjDir=prjDir+"/"+prjSubdirs.at(i);
      QString name=prjSubdirs.at(i);
      QString gname=genericName(name);
      int j=map[name]; if(j<=0) j=gmap[gname];
      if(j>0){
        QString subInDir=inDir+"/"+inSubdirs.at(j-1);;
        setEMC(subPrjDir, subInDir);
      }
   }
  } else if(assType==COMPONENT){
/*
    DB::EmProblem prjEmP;
    QString prj_emfname=prjDir+"/model.em";
    loadProblem(&prjEmP, prj_emfname.toLatin1().data());
*/
    QMap<QString, int> map,gmap;
    MwOCAF* ocaf=new MwOCAF();
    ocaf->workopen(prjDir.toLatin1().data());
    QString in_emfname=inDir+"/model.em";
    DB::EmProblem inEmP;
    loadProblem(&inEmP, nativePath(in_emfname).toLatin1().data());
    DB::List_T *inVols=Tree2List(inEmP.volumes);
    int ninVols=List_Nbr(inVols);
    for(int i=0; i<ninVols; i++){
      DB::Volume *invol;
      List_Read(inVols, i, &invol);
      QString name=invol->name;
      QString gname=genericName(name);
      map[name]=i+1;
      gmap[gname]=i+1;
    }
    DB::List_T *prjVols=Tree2List(ocaf->EmP.volumes);
    int nprjVols=List_Nbr(prjVols);
    for(int i=0; i<nprjVols; i++){
      DB::Volume *vol;
      List_Read(prjVols, i, &vol);
      QString name=vol->name;
      QString gname=genericName(name);
      int j=map[name]; if(j<=0) j=gmap[gname];
      if(j>0) 
      {
        DB::Volume *invol;
        List_Read(inVols, j-1, &invol);
	*vol=*invol;
	if(vol->type==DIELECTRIC || vol->type==WAVEGUIDE || vol->type==BOUNDARYCOND)
	   vol->meshRefinement=invol->meshRefinement;
	if(vol->type==DIELECTRIC || vol->type==BOUNDARYCOND){
	   DB::Material* mat= ocaf->EmP.FindMaterial(invol->material);
	   if(!mat){
	     DB::Material* inmat= inEmP.FindMaterial(invol->material);
             mat= new DB::Material();
	     *mat=*inmat;
	     strcpy(mat->name, inmat->name);
	     ocaf->EmP.addMaterial(mat);
	     DB::Material* mainmat=mainOCAF->EmP.FindMaterial(invol->material);
	     if(!mainmat){ mainmat=new DB::Material();
	      *mainmat= *mat; strcpy(mainmat->name, mat->name);
	      mainOCAF->EmP.addMaterial(mainmat);
	     }
	   }
	} else if(vol->type==GRID){
	   vol->gridNum=invol->gridNum;
	   vol->invariant=invol->invariant;
	   vol->PML=invol->PML;
	}
      }
    }
    ocaf->updatePartColors();
    ocaf->setPartsStatus();
    ocaf->worksave();
    saveMaterials();
    ocaf->closeDoc();
    delete ocaf;
  }
}

void MainWindow::importEMC(QString partName, QString fileName)
{
  QString tmppath;
  #ifdef WNT
//     char tmp[]={"C:\\Temp\\EmCAD-XXXXXX"};
     std::string tmp_str=getenv("TEMP");
     tmp_str+=std::string("\\EmCAD-XXXXXX");
     int l=tmp_str.size()+1;
     auto tmp=new char[l]; strcpy (tmp,tmp_str.c_str());
     int res = _mktemp_s(tmp, l);
     if (res != 0) return;
     tmppath=tmp;
     QDir tmpdir(tmppath);
     tmpdir.mkdir(tmppath);
     delete [] tmp;
  #else
     char tmp[]={"/tmp/EmCAD-XXXXXX"};
     tmppath=mkdtemp(tmp);
  #endif
  if(!FileExists(tmppath.toLatin1().data())) return;
  tar_extract(fileName, tmppath);
  QString inDir=tmppath+"/Work";
  QString prjDir=currentWorkPath+"/"+partName;

  setEMC(prjDir, inDir);

  if(FileExists(tmppath.toLatin1().data()))  remove_dir(nativePath(tmppath));
  saveStatus();
  checkActions();

  mainOCAF->readPartsStatus();
  if(mainOCAF->EmP.hasGeo) updateAllWidgets();
}



bool geoIsModified;
void MainWindow::reload()
{
	preproc.decompose_failure=0;
	preproc.mesh_failure=0;
        modeler.failure=0;
        if(mainOCAF->worksaveNeeded) worksave();
	bool decomposed=!prjData.workStatus.decompositionNeeded;
        closeDoc();
        QString emFileName=mainWorkPath+"/model.em";
        assert(FileExists(emFileName.toLatin1().data()));
	bool reloaded;
	try{
          mainOCAF->newDoc();
          reloaded=loadModel(mainOCAF, nativePath(emFileName).toLatin1().data(), true);
	} 
	catch (stepFileNotFound &e) {
	   workopen(mainWorkPath);
	   QString message="file: \"";
	   message+=e.filename.c_str();
	   message+="\" was not found";
           temporaryMessage(message);
	   reloaded=false;
	}
	geoIsModified=reloaded;
	if(reloaded){
	   bool compIsAss=componentIsAssembly();
	   mainOCAF->saveImportedStruct(mainWorkPath.toLatin1().data(),compIsAss);
           closeDoc();
	   workopen(mainWorkPath);
           saveStatus();
	   updateAllWidgets();
	}
	if(reloaded && mainOCAF->prjStatus.partMaterials){
         TCollection_AsciiString assName; mainOCAF->getAssName(assName);
         QString meshFile=mainWorkPath+"/Data/Circuits/"+QString(assName.ToCString())+".msh";
         meshFile=nativePath(meshFile);
	 bool meshed=FileExists(meshFile.toLatin1().data());
	 if(meshed){
             prjData.workStatus.decompositionNeeded=1;
//             workStatus.remeshNeeded=1;
	     mesh();
	 } else if(decomposed) {
             prjData.workStatus.decompositionNeeded=1;
	     decompose();
	 }
	}
}



int jumpDirectlyToPartition=1;

void MainWindow::openComp()
{
     QString path=currentWorkPath;
     if(!mainOCAF->isPartition()){
       if(mainOCAF->EmP.assemblyType==NET){
	  path+="/";
	  path+=treeWidget->currentPartName;
          QDir directory(path);
	  if(jumpDirectlyToPartition)
            if(directory.exists(QString("Partition"))) path+=QString("/Partition");
       }
       if(mainOCAF->EmP.assemblyType==COMPONENT){
           QDir directory(currentWorkPath);
	   if(!directory.exists(QString("Partition"))) return;
           path=path+QString("/Partition");
       }
       if(prjData.workStatus.componentsaveNeeded) worksaveAllComponents();
     } else if(!treeWidget->currentSubComp) return;
     if(mainOCAF->worksaveNeeded) worksave();
     closeDoc();
     workopen(path, treeWidget->currentSubComp);
//     if(!mainOCAF->hasPartsData()) mainOCAF->makePartsData();
     if(mainOCAF->EmP.hasGeo){
	 updateAllWidgets();
//         mainOCAF->updatePartColors();
    }
}

void MainWindow::openComp_()
{
 jumpDirectlyToPartition=1;
// jumpDirectlyToPartition=1;
 openComp();
}

void MainWindow::closeComp()
{
     QDir dir(currentWorkPath);  QString currentFolder=dir.dirName();
     QString upProjectPath =currentWorkPath;
     if(!mainOCAF->subComp){
	 upProjectPath.chop(currentFolder.size()+1);
	 if(!upProjectPath.endsWith(QString("Work"))) {
	   QDir updir(upProjectPath); QString upProjectDir=updir.dirName();
           upProjectPath.chop(upProjectDir.size()+1);
         }
     }
//     if(mainOCAF->isPartition()) if(mainOCAF->worksaveNeeded) worksave();
     if(mainOCAF->EmP.assemblyType==COMPONENT) if(mainOCAF->worksaveNeeded) worksave();
     saveStatus();
     closeDoc();
     treeWidget->currentSubComp=0;
     workopen(upProjectPath);
     if(mainOCAF->EmP.hasGeo){
	 updateAllWidgets();
//         mainOCAF->updatePartColors();
     }
}


void setModified(QString dir){
   QString fname=dir+"/modified";
   FILE *fd= fopen(nativePath(fname.toLatin1().data()).c_str(), "w");
   fclose(fd);
}
void rmModified(QString dir){
 QString fname=dir+"/modified";
 remove(nativePath(fname.toLatin1().data()).c_str());
}
bool isModified(QString dir){
 QString fname=dir+"/modified";
 bool itis=FileExists(fname.toLatin1().data());
/*
 fname=dir+"/model.dxc.modified";
 itis=itis||FileExists(fname.toLatin1().data());
*/
 return itis;
}



void MainWindow::workopen(QString wkprojpath, int subcomp)
{
  setWindowModified(false);
  runStatus.projectIsOpen=true;
  currentWorkPath =wkprojpath;
  mainOCAF->workopen(wkprojpath.toLatin1().data(), subcomp);
  if(mainOCAF->EmP.hasGeo && geoIsModified) mainOCAF->updatePartColors();
  checkActions();
  setProjectTitle();
  QString IFfileName=wkprojpath+"/model.brep";
}

void MainWindow::updateAllWidgets()
{
	if(TPrsStd_AISViewer::Find(mainOCAF->mainDoc->Main().Root(),theAISViewer))
	    theAISViewer->SetInteractiveContext ( mainOCC->getContext());
        else
	    theAISViewer = TPrsStd_AISViewer::New(mainOCAF->mainDoc->Main(), mainOCC->getContext());

	QString qstepFilePath=mainOCAF->EmP.stepFilePath.c_str();
	treeWidget->setHeaderLabel(getFileName(qstepFilePath));
	makeTree();
	mainOCC->fitAll();
	if(!mainOCAF->theMaterials.IsNull()){
            TreeWidgetItem* matit=treeWidget->getItemFromLabel(mainOCAF->theMaterials);
	    if(matit) treeWidget->makeMaterials(matit);
	}
	setProjectTitle();
	treeWidget-> updateText();
        checkActions();
}

void MainWindow::closeAll(){
 closeDoc();
 removeWorkDir();
 prjData.components.list.clear();
 prjData.wgcomponents.list.clear();
 logger->clear();
 runStatus.projectIsOpen=false;
 checkActions();
}



void MainWindow::closeDoc()
{
//	isNetwork=false;
//	isComponent=false;
        clearWorkProject();
	setProjectTitle();
	treeWidget->clear();
	treeWidget->setHeaderLabel(tr(""));
	mainOCC->deleteAllObjects();
	mainOCAF->closeDoc();
	delete mainOCAF;
        mainOCAF=new MwOCAF(mainOCC->getContext());
        checkActions();
}


void MainWindow::importDataDir()
{
        QString dataPathName = QFileDialog::getExistingDirectory(this, tr("Import Data Directory"),
			                         currentWorkPath,
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
	if(dataPathName.isEmpty()) return;

        QString localDataPathName=currentWorkPath;
        for (int i = 0; i < mainOCAF->EmP.level; ++i) localDataPathName+="/..";
        localDataPathName+="/Data";
        QString copyDataDir="cp -R " + dataPathName + " " + localDataPathName;
        system(copyDataDir.toLatin1().data());
}

void MainWindow::importGeometry()
{
/*
	QString fileName = QFileDialog::getOpenFileName(
          this,
          tr("Import"),
          QDir::currentPath(),
          tr("*.step *.stp *.mat  *.dxc")
	);
*/
        QStringList filters;
	QFileDialog dialog(this,tr("Import Geometry"));
	dialog.setNameFilter(tr("CAD (*.step *.stp)"));
	dialog.setViewMode(QFileDialog::List);
	QString	fileName;
	if(dialog.exec()){
	   QStringList fileNames = dialog.selectedFiles();
	   fileName= fileNames.at(0);
	   QFileInfo fileinfo(fileName);
           QDir::setCurrent(fileinfo.dir().absolutePath());
	}
	bool isSTEP;
	bool isDXC;
	if (!fileName.isEmpty()){
	   QFileInfo fileInfo;
	   fileInfo.setFile(fileName);
	   QString fileType = fileInfo.suffix();
	   isDXC  =fileType.toLower() == tr("dxc"); 
	   isSTEP =fileType.toLower() == tr("step") || fileType.toLower() == tr("stp");
	   bool isGEO=isSTEP||isDXC;
	   if(!isGEO) return;
	   try {
	      QApplication::setOverrideCursor( Qt::WaitCursor );
	      if (isDXC) {
		   mainOCAF->EmP.occFilePath=fileName.toLatin1().data();
                   mainOCAF->openDoc(mainOCAF->EmP.occFilePath.c_str());
	           if(TPrsStd_AISViewer::Find(mainOCAF->mainDoc->Main().Root(),theAISViewer))
		       theAISViewer->SetInteractiveContext ( mainOCC->getContext());
		   else
	               theAISViewer = TPrsStd_AISViewer::New(mainOCAF->mainDoc->Main(), mainOCC->getContext());
	               treeWidget->setHeaderLabel(getFileName(fileName));

	      }
	      else if (isSTEP)
	      {
                   if(!mainOCAF->EmP.stepFilePath.empty()){
                      temporaryMessage("Step file can not be imported because the geometry is already defined");
	              return;
	           }
		   mainOCAF->newDoc();
		   mainOCAF->EmP.stepFilePath=fileName.toLatin1().data();
	           mainOCAF->EmP.hasGeo=true;
	           treeWidget->setHeaderLabel(getFileName(fileName));
		   importSTEP(mainOCAF->mainDoc, fileName.toLatin1().data(), prjData.lengthUnitName );
		   mainOCAF->setDocTools();
	           if(runStatus.projectIsOpen){
		       setModified(currentWorkPath);
		       mainOCAF->worksaveNeeded=true;
                       prjData.workStatus.decompositionNeeded=1;
                       prjData.workStatus.reductionNeeded=1;
		   }
	           if(TPrsStd_AISViewer::Find(mainOCAF->mainDoc->Main().Root(),theAISViewer))
		          theAISViewer->SetInteractiveContext ( mainOCC->getContext());
		   else
	                  theAISViewer = TPrsStd_AISViewer::New(mainOCAF->mainDoc->Main(), mainOCC->getContext());
                   //------
	      }
	      myLastFolder = fileInfo.absolutePath();
	   }catch (ImportFailure ex) {
	     QApplication::restoreOverrideCursor();
             temporaryMessage("import failure");
             return;
           }
	   mainOCC->updateContext();
           if(isSTEP){
	      bool compIsAss=componentIsAssembly();
              mainOCAF->saveImportedStruct(mainWorkPath.toLatin1().data(),compIsAss);
              closeDoc();
              workopen(mainWorkPath.toLatin1().data());
              if(mainOCAF->EmP.assemblyType==NET) prjData.network=1;
//	      mainOCAF->setMissingNames();
	   }
	   makeTree();
	   mainOCC->fitAll();
	   if (isSTEP){ 
//	      mainOCAF->putVolumesInDB();
	      geoIsModified=true;
	      mainOCAF->loadGlobalMat(mainWorkPath.toLatin1().data(),false);
              if (!mainOCAF->theMaterials.IsNull()){
                 TreeWidgetItem* matit=treeWidget->getItemFromLabel(mainOCAF->theMaterials);
	         if(matit) treeWidget->makeMaterials(matit);
              }
	      treeWidget->updateText();
	   }
	   QApplication::restoreOverrideCursor();
	   setProjectTitle();
           if(isSTEP){
              TCollection_AsciiString name;
              mainOCAF->getAssName(name);
              prjData.mainAssName=name.ToCString();
              prjData.saveSettings();
              updateAllWidgets();
	   }
	}
        checkActions();
}


void MainWindow::importMaterial()
{
        QStringList filters;
	QFileDialog dialog(this,tr("Import Materials"));
	dialog.setDirectory(QDir::currentPath());
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(tr("Material (*.mat)"));
	dialog.setViewMode(QFileDialog::List);
	QString	fileName;
	if(dialog.exec()){
	   QStringList fileNames = dialog.selectedFiles();
	   fileName= fileNames.at(0);
	   QFileInfo fileinfo(fileName);
           QDir::setCurrent(fileinfo.dir().absolutePath());
	}
	if (!fileName.isEmpty()){
	   QFileInfo fileInfo;
	   fileInfo.setFile(fileName);
	   QString fileType = fileInfo.suffix();
	   bool isMAT  =fileType.toLower() == tr("mat");
	   if(!isMAT) return;
	   try {
	      QApplication::setOverrideCursor( Qt::WaitCursor );
              loadModel(mainOCAF, nativePath(fileName).toLatin1().data() );
	      if (!mainOCAF->theMaterials.IsNull()){
                     TreeWidgetItem* matit=treeWidget->getItemFromLabel(mainOCAF->theMaterials);
	             treeWidget->makeMaterials(matit);
                     saveMaterials();
		     mainOCAF->worksaveNeeded=true;
	      }
	      myLastFolder = fileInfo.absolutePath();
	   }catch (ImportFailure ex) {
	     QApplication::restoreOverrideCursor();
             temporaryMessage("import failure");
             return;
           }
	   mainOCC->updateContext();
	   QApplication::restoreOverrideCursor();
	}
        checkActions();
}


void MainWindow::importIdealFilterCircuit()
{
        QStringList filters;
	QFileDialog dialog(this,tr("Import Ideal Filter Circuit"));
//	dialog.setDirectory(QDir::currentPath());
	dialog.setNameFilter(tr("Circuit (*.JC)"));
	dialog.setViewMode(QFileDialog::List);
	QString	fileName;
	if(dialog.exec()){
	   QStringList fileNames = dialog.selectedFiles();
	   fileName= fileNames.at(0);
	   QFileInfo fileinfo(fileName);
           QDir::setCurrent(fileinfo.dir().absolutePath());
	}
	if (!fileName.isEmpty()){
	   QFileInfo fileInfo;
	   fileInfo.setFile(fileName);
	   QString fileType = fileInfo.suffix();
	   bool isJC  =fileType.toLower() == tr("jc");
	   if(!isJC) return;
           QString idealJCpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter.JC");
	   if(QFile::exists(idealJCpath)) QFile::remove (idealJCpath);
	   QFile::copy(fileName, idealJCpath);
	   QApplication::restoreOverrideCursor();
	}
        checkActions();
}


void MainWindow::importIdealFilterPar()
{
        QStringList filters;
	QFileDialog dialog(this,tr("Import Ideal Filter Parameters"));
//	dialog.setDirectory(QDir::currentPath());
	dialog.setNameFilter(tr("Circuit (*.txt)"));
	dialog.setViewMode(QFileDialog::List);
	QString	fileName;
	if(dialog.exec()){
	   QStringList fileNames = dialog.selectedFiles();
	   fileName= fileNames.at(0);
	   QFileInfo fileinfo(fileName);
           QDir::setCurrent(fileinfo.dir().absolutePath());
	}
	if (!fileName.isEmpty()){
	   QFileInfo fileInfo;
	   fileInfo.setFile(fileName);
	   QString fileType = fileInfo.suffix();
	   bool istxt  =fileType.toLower() == tr("txt");
	   if(!istxt) return;
           QString idealFilterParPath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter_xpar.txt");
	   if(QFile::exists(idealFilterParPath)) QFile::remove (idealFilterParPath);
	   QFile::copy(fileName, idealFilterParPath);
	   QApplication::restoreOverrideCursor();
	}
        checkActions();
}


void MainWindow::importFilterCircuit()
{
        QStringList filters;
	QFileDialog dialog(this,tr("Import Ideal Filter Circuit"));
//	dialog.setDirectory(QDir::currentPath());
	dialog.setNameFilter(tr("Circuit (*.JC)"));
	dialog.setViewMode(QFileDialog::List);
	QString	fileName;
	if(dialog.exec()){
	   QStringList fileNames = dialog.selectedFiles();
	   fileName= fileNames.at(0);
	   QFileInfo fileinfo(fileName);
           QDir::setCurrent(fileinfo.dir().absolutePath());
	}
	if (!fileName.isEmpty()){
	   QFileInfo fileInfo;
	   fileInfo.setFile(fileName);
	   QString fileType = fileInfo.suffix();
	   bool isJC  =fileType.toLower() == tr("jc");
	   if(!isJC) return;
           QString importedParPath=nativePath(mainWorkPath+"/Data/Circuits/imported_circuit.JC");
	   if(QFile::exists(importedParPath)) QFile::remove (importedParPath);
	   QFile::copy(fileName, importedParPath);
	   QApplication::restoreOverrideCursor();
	}
        checkActions();
}


void MainWindow::importFilterPar()
{
        QStringList filters;
	QFileDialog dialog(this,tr("Import Mapped Circuit Parameters"));
//	dialog.setDirectory(QDir::currentPath());
	dialog.setNameFilter(tr("Circuit (*.txt)"));
	dialog.setViewMode(QFileDialog::List);
	QString	fileName;
	if(dialog.exec()){
	   QStringList fileNames = dialog.selectedFiles();
	   fileName= fileNames.at(0);
	   QFileInfo fileinfo(fileName);
           QDir::setCurrent(fileinfo.dir().absolutePath());
	}
	if (!fileName.isEmpty()){
	   QFileInfo fileInfo;
	   fileInfo.setFile(fileName);
	   QString fileType = fileInfo.suffix();
	   bool istxt  =fileType.toLower() == tr("txt");
	   if(!istxt) return;
           QString importedParPath=nativePath(mainWorkPath+"/Data/Circuits/imported_circuit_xpar.txt");
	   if(QFile::exists(importedParPath)) QFile::remove (importedParPath);
	   QFile::copy(fileName, importedParPath);
	   QApplication::restoreOverrideCursor();
	}
        checkActions();
}




void MainWindow::importS2P()
{
        QStringList filters;
	QFileDialog dialog(this,tr("Import Touchstone 2 Ports"));
//	dialog.setDirectory(QDir::currentPath());
	dialog.setNameFilter(tr("Touchstone 2 Ports (*.S2P *.s2p)"));
	dialog.setViewMode(QFileDialog::List);
	char *s2pFilePath=prjData.s2pFilePath.toLatin1().data();
	if(prjData.s2pFilePath.length()>0) if(QFile::exists(prjData.s2pFilePath)) dialog.selectFile(prjData.s2pFilePath);
	QString	fileName;
	QString	filePath;
	if(dialog.exec()){
	   QStringList fileNames = dialog.selectedFiles();
	   fileName= fileNames.at(0);
	   QFileInfo fileinfo(fileName);
           QDir::setCurrent(fileinfo.dir().absolutePath());
	   filePath=fileinfo.absoluteFilePath();
	}
	if (!fileName.isEmpty()){
	   QFileInfo fileInfo;
	   fileInfo.setFile(fileName);
	   QString fileType = fileInfo.suffix();
	   bool isS2P  =fileType.toLower() == tr("s2p");
	   if(!isS2P) return;
           QString S2P_path=nativePath(mainWorkPath+"/Data/Circuits/imported_response.s2p");
	   if(QFile::exists(S2P_path)) QFile::remove (S2P_path);
	   QFile::copy(fileName, S2P_path);
	   if(prjData.s2pFilePath!=filePath){
	       prjData.s2pFilePath=filePath;
	       prjData.saveSettings();
	   }
	   QApplication::restoreOverrideCursor();
	}
        checkActions();
}




void MainWindow::importADSprj()
{
// TBD
}

void MainWindow::saveStatus(){
  QString prjstatusFile=mainWorkPath+"/model.status";
  if(mainOCAF->EmP.assemblyType==NET){	
    propagateStatus(mainWorkPath);
    mainOCAF->prjStatus.read(prjstatusFile.toLatin1().data());
  } else {
   mainOCAF->prjStatus.save(prjstatusFile.toLatin1().data());
  }
}


void MainWindow::propagateStatus(QString dir)
{
  QString asstypeFileName=dir+"/assemblyType";
  FILE *in= fopen(nativePath(asstypeFileName.toLatin1().data()).c_str(), "r");
  if(!in) return;
  int assType;
  fscanf(in, "%d", &assType);
  fclose(in);
  if(assType==NET){
   QStringList subdirs;
   if(readStringlist(dir+"/subdirs", subdirs)){
     for (int i = 0; i < subdirs.size(); ++i){
       QString subDir=dir+"/"+subdirs.at(i);
       char subdirname[101]; if(strlen(subDir.toLatin1().data())< 100 ) strcpy(subdirname,subDir.toLatin1().data());
       propagateStatus(subDir);
     }
     QString prjstatusFile=dir+"/model.status";
     ProjectStatus prjstatus;
     if(!prjstatus.read(prjstatusFile.toLatin1().data())) return;
     prjstatus.geometry=1;
     prjstatus.partTypes=1;
     prjstatus.partMaterials=1;
     for (int i = 0; i < subdirs.size(); ++i){
       ProjectStatus subStatus;
       QString subStatusFile=dir+"/"+subdirs.at(i)+"/model.status";
       char fname[101]; if(strlen(subStatusFile.toLatin1().data())< 100 ) strcpy(fname,subStatusFile.toLatin1().data());
       if(!subStatus.read(subStatusFile.toLatin1().data())) return;
       prjstatus.geometry*=subStatus.geometry;
       prjstatus.partTypes*=subStatus.partTypes;
       prjstatus.partMaterials*=subStatus.partMaterials;
     }
     prjstatus.save(prjstatusFile.toLatin1().data());
   }
  }
}

int loadModel(MwOCAF* ocaf, const char *fName, bool update=false);


void MainWindow::recursiveAssignDefaultMaterial(QString dir, bool toplevel)
{
  char dirname[101]; if(strlen(dir.toLatin1().data())< 100 ) strcpy(dirname,dir.toLatin1().data());
  int assType=mainOCAF->EmP.assemblyType;
  MwOCAF* ocaf=mainOCAF;
  QString emFileName=dir+"/model.em";
  if(!toplevel){
    ocaf=new MwOCAF;
    loadModel(ocaf, nativePath(emFileName).toLatin1().data(), false);
  }
  {
    if(strlen(theDefaultMaterial.name)>0){
       strcpy(ocaf->EmP.defaultBC,theDefaultMaterial.name);
       DB::Material *mat = new DB::Material();
       *mat=theDefaultMaterial; strcpy(mat->name,theDefaultMaterial.name);
       ocaf->EmP.replaceMaterial(mat);
    }
    FILE *fid= fopen(nativePath(emFileName).toLatin1().data(), "w");
    fprintf(fid, "Level  %d\n", ocaf->EmP.level);
    fprintf(fid, "AssemblyType  %d\n", ocaf->EmP.assemblyType);
    fprintf(fid, "DefaultBoundCond  \"%s\"\n", ocaf->EmP.defaultBC);
    if(!ocaf->EmP.stepFilePath.empty()) fprintf(fid, "\nImport  \"%s\"\n\n", ocaf->EmP.stepFilePath.c_str());
    ocaf->EmP.save(fid);
    fclose(fid);
  }
  if(!toplevel){
     assType=ocaf->EmP.assemblyType;
     ocaf->closeDoc();
     delete ocaf;
  }
  if(assType==NET){
   QStringList subdirs;
   if(readStringlist(dir+"/subdirs", subdirs)) for (int i = 0; i < subdirs.size(); ++i){
      QString subDir=dir+"/"+subdirs.at(i);
      char subdirname[101]; if(strlen(subDir.toLatin1().data())< 100 ) strcpy(subdirname,subDir.toLatin1().data());
      recursiveAssignDefaultMaterial(subDir, false);
   }
  }
}

void MainWindow::recursiveAssignDefaultMaterial()
{
  recursiveAssignDefaultMaterial(currentWorkPath, true);
}



void MainWindow::worksaveComponents(QString dir, bool toplevel)
{
  char dirname[101]; if(strlen(dir.toLatin1().data())< 100 ) strcpy(dirname,dir.toLatin1().data());
  if(!toplevel){ //toplevel is dealt above in worksaveAllComponents()
    workopen(dir);
  }
  int assType=mainOCAF->EmP.assemblyType;
  if(assType==COMPONENT){ 
	  worksave();
  }
  closeDoc();
  if(assType==NET){
   QStringList subdirs;
   if(readStringlist(dir+"/subdirs", subdirs)) for (int i = 0; i < subdirs.size(); ++i){
      QString subDir=dir+"/"+subdirs.at(i);
      char subdirname[101]; if(strlen(subDir.toLatin1().data())< 100 ) strcpy(subdirname,subDir.toLatin1().data());
      worksaveComponents(subDir, false);
   }
  }
}

void MainWindow::worksaveAllComponents()
{
  QString currentWorkPath0=currentWorkPath;
  worksaveComponents(currentWorkPath0, true);
  workopen(currentWorkPath0);
  if(mainOCAF->EmP.hasGeo) updateAllWidgets();
  prjData.workStatus.componentsaveNeeded=false;
}


void MainWindow::save(){
 prjData.savePorts();
 prjData.savePortLoads();
 prjData.saveWorkStatus();
 saveStatus();
 if(prjData.workStatus.componentsaveNeeded) worksaveAllComponents();
 if(mainOCAF->worksaveNeeded) worksave();
 QString tmpdir=mainWorkPath;
 QString Work="Work";
 tmpdir.chop(5); //remove /work 
 tar_create(savePath, tmpdir, Work);
}

void MainWindow::saveMaterials()
{
   QString globalMatName=currentWorkPath;
   for (int i = 0; i < mainOCAF->EmP.level; ++i) globalMatName+="/..";
   globalMatName+="/Data/materials.mat";
   FILE *matfid= fopen(nativePath(globalMatName.toLatin1().data()).c_str(), "w");
   mainOCAF->EmP.saveMaterials(matfid);
   fclose(matfid);
}
bool MainWindow::worksave()
{
   if(!runStatus.projectIsOpen) return false;
   mainOCAF->worksave();
   return true;
}

void ProjectData::saveSettings(){
   if(!runStatus.projectIsOpen) return;
   QString globalFileName=mainWorkPath+"/settings.dat";
   FILE *fid= fopen(nativePath(globalFileName.toLatin1().data()).c_str(), "w");
   fprintf(fid, "mainAssName \"%s\"\n", prjData.mainAssName.toLatin1().data());
   fprintf(fid, "varFilePath \"%s\"\n", prjData.varFilePath.toLatin1().data());
   fprintf(fid, "s2pFilePath \"%s\"\n", prjData.s2pFilePath.toLatin1().data());
   fprintf(fid, "network %d\n", network);
   fprintf(fid, "length unit \"%s\"\n", lengthUnitName);
   fprintf(fid, "freq unit exp %d\n", freqUnitE);
   fprintf(fid, "mesh wavelength ratio %d\n", meshPerWavelen);
   fprintf(fid, "mesh circle ratio %d\n", meshPerCircle);
   fprintf(fid, "mesh refine max num %d\n", meshRefineMaxNum);
   fprintf(fid, "mesh min energy ratio %10.3f\n", meshMinEnergyRatio);
   fprintf(fid, "local meshing3d %d\n", localMeshing3d);
   fprintf(fid, "mor freq band %10.5f %10.5f\n", freqBand[0], freqBand[1]);
   fprintf(fid, "refine freq band %10.5f %10.5f\n", refFreqBand[0], refFreqBand[1]);
   fprintf(fid, "mapping freq band %10.5f %10.5f\n", mapFreqBand[0], mapFreqBand[1]);
   fprintf(fid, "filter pass band %14.8f %14.8f\n", filterPassBand[0], filterPassBand[1]);
   fprintf(fid, "filter order %d\n", filterOrder);
   fprintf(fid, "filter returnloss %10.5f\n", filterRetLoss);
   fprintf(fid, "filter outband returnloss %10.5f\n", filterOutbandRetLoss);
   fprintf(fid, "filter Qfactor %10.5f\n", filterQfactor);
   fprintf(fid, "filter inductive skin %d\n", filterInductiveSkin);
   fprintf(fid, "filter port impedance %10.5f\n", filterPortImpedance);
   fprintf(fid, "symmetric filter response %d\n", symmFilterResponse);
   fprintf(fid, "predistorted filter %d\n", predistortedFilter);
   fprintf(fid, "predistorted filter optimize %d\n", predistFilterOptim);
   fprintf(fid, "predistorted filter ilosspp %10.5f\n", predistFilterOptimILpp);
   fprintf(fid, "predistorted filter returnloss %10.5f\n", predistFilterOptimRL);
   fprintf(fid, "predistorted filter itermax %d\n", predistFilterOptimIterMax);
   fprintf(fid, "predistorted filter trustR %10.5f\n", predistFilterOptimTrustR);
   fprintf(fid, "mor freq num %d\n", MORFreqNum);
   fprintf(fid, "mor krylov order %d\n", KrylovOrder);
   fprintf(fid, "ana freq band %10.5f %10.5f\n", anaFreqBand[0], anaFreqBand[1]);
   fprintf(fid, "ana freq num %d\n", anaFreqNum);
   fprintf(fid, "ana mor freq num %d\n", anaMORFreqNum);
   fprintf(fid, "cutoff ratio %10.5f\n", cutoffRatio);
   fprintf(fid, "zero pole freq band %10.5f %10.5f\n", zpFreqBand[0], zpFreqBand[1]);
   fprintf(fid, "zero pole window ratio %10.5f\n", zpWinRatio);
   fprintf(fid, "ideal filter type %d\n", idealFilterType);
   fprintf(fid, "ideal filter topology %d\n", idealFilterTopology);
   fprintf(fid, "filter mapping source %d\n", filtermapSource);
   fprintf(fid, "filter mapping symmetric %d\n", filtermapSymmetric);
   fprintf(fid, "filter mapping Qfactor %10.5f\n", filtermapQfactor);
   fprintf(fid, "filter tuning method %d\n", filterTuneMethod);
   fprintf(fid, "filter tuning recompute jacobian %d\n", filterTuneRecomputeJaco);
   fprintf(fid, "filter tuning recompute error %d\n", filterTuneRecomputeError);
   fprintf(fid, "filter symmetric tuning %d\n", filterSymmetricTuning);
   fprintf(fid, "filter tuning only transversej %d\n", filterTuneOnlyJt);
   fprintf(fid, "filter tuning itermax %d\n", filterTuneItermax);
   fprintf(fid, "filter tuning xtol %8.2e\n", filterTuneXtol);
   fprintf(fid, "filter tuning trustd %8.2e\n", filterTuneTrustR);
   fprintf(fid, "filter tuning graddx %8.2e\n", filterTuneGradDx);
   fprintf(fid, "freq response type %d\n", freqRespParType);
   fprintf(fid, "freq response part %d\n", freqRespParPart);
   fprintf(fid, "freq response yscale auto %d\n", freqRespYscaleAuto);
   fprintf(fid, "freq response yscale %10.5f %10.5f %10.5f\n", freqRespYscale[0], freqRespYscale[1], freqRespYscale[2]);
   fprintf(fid, "zero pole yscale auto %d\n", zeropoleYscaleAuto);
   fprintf(fid, "zero pole yscale %10.5f %10.5f %10.5f\n", zeropoleYscale[0], zeropoleYscale[1], zeropoleYscale[2]);
   for ( ZeroPoleCurvesIterator it=zeropoleCurves.begin(); it!= zeropoleCurves.end(); it++) 
     fprintf(fid, "zero pole curve %d %d\n", (*it).first, (*it).second);
   fprintf(fid, "automatic freq response %d\n", autoFreqResponse);
   fprintf(fid, "automatic design mapped pole %d\n", autoDesignWithMappedTz);
   fprintf(fid, "automatic mapped freq response %d\n", autoMappedFreqResponse);
   fprintf(fid, "automatic ideal mapped pole freq response %d\n", autoIdealMappedTzFreqResponse);
   fprintf(fid, "automatic zero pole  %d\n", autoZeropole);
   fprintf(fid, "automatic mapped zero pole  %d\n", autoMappedZeropole);
   fprintf(fid, "automatic filter mapping  %d\n", autoFilterMapping);
   if(filterZeros.size()>0){
     fprintf(fid, "filter tx zeros [\n");
	for (int i=0; i< filterZeros.size(); i++) fprintf(fid, " %14.8f\n", filterZeros[i]);
     fprintf(fid, "]\n");
   }
   fclose(fid);
}

void ProjectData::saveWorkStatus(){
   QString wkstatusFileName=mainWorkPath+"/workstatus.dat";
   FILE *fid= fopen(nativePath(wkstatusFileName).toLatin1().data(), "w");
   fprintf(fid, "decomposition needed  %d\n", workStatus.decompositionNeeded);
   fprintf(fid, "first decomposition %d\n", workStatus.firstDecomposition);
   fprintf(fid, "material changed %d\n", workStatus.materialChanged);
   fprintf(fid, "remesh needed  %d\n", workStatus.remeshNeeded);
   fprintf(fid, "modelization needed  %d\n", workStatus.modelizationNeeded);
   fprintf(fid, "component save needed %d\n", workStatus.componentsaveNeeded);
   fprintf(fid, "reduction needed %d\n", workStatus.reductionNeeded);
   fprintf(fid, "reload needed %d\n", workStatus.reloadNeeded);
   fclose(fid);
}

void ProjectData::readPorts(){
    QString portsFileName=mainWorkPath+"/ports";
    ports.read(portsFileName.toLatin1().data());
}
void ProjectData::readPortLoads(){
    QString LPimpFileName=mainWorkPath+"/portloads";
    portloads.read(LPimpFileName.toLatin1().data());
}
void ProjectData::savePorts(){
    QString portsFileName=mainWorkPath+"/ports";
    ports.save(portsFileName.toLatin1().data());
}
void ProjectData::savePortLoads(){
    QString LPimpFileName=mainWorkPath+"/portloads";
    portloads.save(LPimpFileName.toLatin1().data());
    QString portLoadJCfileName=mainWorkPath+"/Data/Circuits/extern_port_loads.JC";
    FILE *f_JC= fopen(nativePath(portLoadJCfileName.toLatin1().data()).c_str(), "w");
    typedef std::map<std::string, double, std::less<std::string> >::const_iterator MapIt;
    for ( MapIt it=prjData.portloads.map.begin(); it!= prjData.portloads.map.end(); it++) { 
        fprintf(f_JC, "Zc  %s%d  %.8e\n", (*it).first.c_str(), 1, (*it).second);
    }
    fclose(f_JC);
}


extern int loadPrjData(const char *fName);

void ProjectData::readSettings(){
   if(!runStatus.projectIsOpen) return;
   QString globalFileName=mainWorkPath+"/settings.dat";
   if(!FileExists(globalFileName.toLatin1().data())) return;
   loadPrjData(globalFileName.toLatin1().data());
   unit.xm=lengthUnit();
   unit.xHz=freqUnit();
}

void ProjectData::readWorkStatus(){
   QString wkstatusFileName=mainWorkPath+"/workstatus.dat";
   if(!FileExists(wkstatusFileName.toLatin1().data())) return;
   loadPrjData(wkstatusFileName.toLatin1().data());
}


void MainWindow::saveAs()
{
	QString fnameHint=prjData.mainAssName;
	if(fnameHint.length()==0) fnameHint=prjData.projectName;
        if(fnameHint.endsWith("_ASM",Qt::CaseInsensitive)) fnameHint.chop(4);
        fnameHint+=".emc";
        QFileDialog dialog(this,tr("Save As"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter(tr("*.emc"));
	dialog.selectFile(fnameHint);
	dialog.setLabelText(QFileDialog::Accept,tr("Save"));
//	dialog.setFileMode ( QFileDialog::DirectoryOnly );
	QString	path;
	if(dialog.exec()){
           QStringList pathList= dialog.selectedFiles();
	   path= pathList.at(0);
	}


	if( !path.isEmpty() )
	{
	       savePath=path;
	       if(!savePath.endsWith(".emc")) savePath+=".emc";
	       save();
               QFileInfo fileInfo(savePath); 
	       prjData.projectName=fileInfo.fileName();
               prjData.projectName.chop(4);
	       setProjectTitle(); 
               checkActions();
	}
}


void MainWindow::exportMaterial()
{
        QFileDialog dialog(this,tr("Export Material File"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter(tr("*.mat"));
	QString	fname="materials.mat";
	dialog.selectFile(fname);
	dialog.setDirectory(QDir::currentPath());
	dialog.setLabelText(QFileDialog::Accept,tr("Save"));
//	dialog.setFileMode ( QFileDialog::DirectoryOnly );
	QString	path;
	if(dialog.exec()){
           QStringList pathList= dialog.selectedFiles();
	   path= pathList.at(0);
	}

	if( !path.isEmpty() )
	{
	       if(!path.endsWith(".mat")) path+=".mat";
	       FILE *matfid= fopen(nativePath(path.toLatin1().data()).c_str(), "w");
	       mainOCAF->EmP.saveMaterials(matfid);
               fclose(matfid);
	}
}


void MainWindow::exportSpice()
{
        QFileDialog dialog(this,tr("Export Spice Circuit"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter(tr("*.sp"));
	QString	fname=prjData.mainAssName+"_RM.sp";
	dialog.selectFile(fname);
	dialog.setLabelText(QFileDialog::Accept,tr("Save"));
//	dialog.setFileMode ( QFileDialog::DirectoryOnly );
	QString	path;
	if(dialog.exec()){
           QStringList pathList= dialog.selectedFiles();
	   path= pathList.at(0);
	}
	if( !path.isEmpty() )
	{
	     if(!path.endsWith(".sp")) path+=".sp";
             QString spiceCircPath=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM.sp");
             if(!FileExists(spiceCircPath.toLatin1().data())) return;
	     if(QFile::exists(path)) QFile::remove (path);
	     QFile::copy(spiceCircPath, path);
	}
}


void MainWindow::exportMappedJC()
{
        QFileDialog dialog(this,tr("Export Mapped JC Circuit"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter(tr("*.JC"));
        QString circuitName;
	if(prjData.filtermapSource==ZEROPOLES) circuitName=prjData.mainAssName+"_RM_mapped";
        if(prjData.filtermapSource==IMPORTED_RESPONSE) circuitName="imported_response_mapped";
	QString	fname=circuitName+".JC";
	dialog.selectFile(fname);
	dialog.setLabelText(QFileDialog::Accept,tr("Save"));
//	dialog.setFileMode ( QFileDialog::DirectoryOnly );
	QString	path;
	if(dialog.exec()){
           QStringList pathList= dialog.selectedFiles();
	   path= pathList.at(0);
	}
	if( !path.isEmpty() )
	{
	     if(!path.endsWith(".JC")) path+=".JC";
             QString mappedJCpath=nativePath(mainWorkPath+"/Data/Circuits/"+circuitName+".JC");
             if(!FileExists(mappedJCpath.toLatin1().data())) return;
	     if(QFile::exists(path)) QFile::remove (path);
	     QFile::copy(mappedJCpath, path);
	}
}

void MainWindow::exportMappedSpice()
{
        QFileDialog dialog(this,tr("Export Mapped Spice Circuit"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter(tr("*.JC"));
        QString circuitName;
	if(prjData.filtermapSource==ZEROPOLES) circuitName=prjData.mainAssName+"_RM_mapped";
        if(prjData.filtermapSource==IMPORTED_RESPONSE) circuitName="imported_response_mapped";
	QString	fname=circuitName+".sp";
	dialog.selectFile(fname);
	dialog.setLabelText(QFileDialog::Accept,tr("Save"));
//	dialog.setFileMode ( QFileDialog::DirectoryOnly );
	QString	path;
	if(dialog.exec()){
           QStringList pathList= dialog.selectedFiles();
	   path= pathList.at(0);
	}
	if( !path.isEmpty() )
	{
	     if(!path.endsWith(".sp")) path+=".sp";
             QString mappedSPpath=nativePath(mainWorkPath+"/Data/Circuits/"+circuitName+".sp");
             if(!FileExists(mappedSPpath.toLatin1().data())) return;
	     if(QFile::exists(path)) QFile::remove (path);
	     QFile::copy(mappedSPpath, path);
	}
}


void MainWindow::exportIdealJC()
{
        QFileDialog dialog(this,tr("Export Ideal JC Circuit"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter(tr("*.JC"));
	QString	fname=prjData.projectName+"_ideal.JC";
	dialog.selectFile(fname);
	dialog.setLabelText(QFileDialog::Accept,tr("Save"));
//	dialog.setFileMode ( QFileDialog::DirectoryOnly );
	QString	path;
	if(dialog.exec()){
           QStringList pathList= dialog.selectedFiles();
	   path= pathList.at(0);
	}
	if( !path.isEmpty() )
	{
	     if(!path.endsWith(".JC")) path+=".JC";
             QString mappedJCpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter.JC");
             if(!FileExists(mappedJCpath.toLatin1().data())) return;
	     if(QFile::exists(path)) QFile::remove (path);
	     QFile::copy(mappedJCpath, path);
	}
}

int getFileLine(FILE *fin, std::string &str);

void MainWindow::exportIdealResponse()
{

	QString idealRespPath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter.ts");
	QString t1extPath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter.ext");
	if(!QFile::exists(t1extPath)) 
		create_touchstone1(idealRespPath);
	else if(file1NewerThanFile2(idealRespPath.toLatin1().data(), t1extPath.toLatin1().data()) )  
		create_touchstone1(idealRespPath);

	FILE *fext= fopen(nativePath(t1extPath.toLatin1().data()).c_str(), "r");
	std::string ext;
        getFileLine(fext, ext);
	QString t1ext=QString(ext.c_str());
        QString t1Path=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter"+t1ext);	

        QFileDialog dialog(this,tr("Export Ideal Filter Response"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter("*"+t1ext);
	QString	fname=prjData.projectName+"_ideal"+t1ext;
	dialog.selectFile(fname);
	dialog.setLabelText(QFileDialog::Accept,tr("Save"));
//	dialog.setFileMode ( QFileDialog::DirectoryOnly );

	QString	savepath;
	if(dialog.exec()){
           QStringList pathList= dialog.selectedFiles();
	   savepath= pathList.at(0);
	}
	if( !savepath.isEmpty() )
	{
	     if(!savepath.endsWith(t1ext)) savepath+=t1ext;
             if(!FileExists(t1Path.toLatin1().data())) return;
	     if(QFile::exists(savepath)) QFile::remove (savepath);
	     QFile::copy(t1Path, savepath);
	}
}

void MainWindow::exportFreqResponse()
{

	QString freqRespPath=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM.ts");
	QString t1extPath=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM.ext");
	if(!QFile::exists(t1extPath)) 
		create_touchstone1(freqRespPath);
	else if(file1NewerThanFile2(freqRespPath.toLatin1().data(), t1extPath.toLatin1().data()) )  
		create_touchstone1(freqRespPath);

	FILE *fext= fopen(nativePath(t1extPath.toLatin1().data()).c_str(), "r");
	std::string ext;
        getFileLine(fext, ext);
	QString t1ext=QString(ext.c_str());
        QString t1Path=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM"+t1ext);	

        QFileDialog dialog(this,tr("Export Frequency Response"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter("*"+t1ext);
	QString	fname=prjData.mainAssName+t1ext;
	dialog.selectFile(fname);
	dialog.setLabelText(QFileDialog::Accept,tr("Save"));
//	dialog.setFileMode ( QFileDialog::DirectoryOnly );

	QString	savepath;
	if(dialog.exec()){
           QStringList pathList= dialog.selectedFiles();
	   savepath= pathList.at(0);
	}
	if( !savepath.isEmpty() )
	{
	     if(!savepath.endsWith(t1ext)) savepath+=t1ext;
             if(!FileExists(t1Path.toLatin1().data())) return;
	     if(QFile::exists(savepath)) QFile::remove (savepath);
	     QFile::copy(t1Path, savepath);
	}
}


void MainWindow::exportMappedResponse()
{

	QString freqRespPath=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM_mapped.ts");
	QString t1extPath=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM_mapped.ext");
	if(!QFile::exists(t1extPath)) 
		create_touchstone1(freqRespPath);
	else if(file1NewerThanFile2(freqRespPath.toLatin1().data(), t1extPath.toLatin1().data()) )  
		create_touchstone1(freqRespPath);

	FILE *fext= fopen(nativePath(t1extPath.toLatin1().data()).c_str(), "r");
	std::string ext;
        getFileLine(fext, ext);
	QString t1ext=QString(ext.c_str());
        QString t1Path=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM_mapped"+t1ext);	

        QFileDialog dialog(this,tr("Export Mapped Circuit Frequency Response"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter("*"+t1ext);
	QString	fname=prjData.mainAssName+"_mapped"+t1ext;
	dialog.selectFile(fname);
	dialog.setLabelText(QFileDialog::Accept,tr("Save"));
//	dialog.setFileMode ( QFileDialog::DirectoryOnly );

	QString	savepath;
	if(dialog.exec()){
           QStringList pathList= dialog.selectedFiles();
	   savepath= pathList.at(0);
	}
	if( !savepath.isEmpty() )
	{
	     if(!savepath.endsWith(t1ext)) savepath+=t1ext;
             if(!FileExists(t1Path.toLatin1().data())) return;
	     if(QFile::exists(savepath)) QFile::remove (savepath);
	     QFile::copy(t1Path, savepath);
	}
}




void MainWindow::exportIdealSpice()
{
        QFileDialog dialog(this,tr("Export Ideal Spice Circuit"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter(tr("*.JC"));
	QString	fname=prjData.projectName+"_ideal.sp";
	dialog.selectFile(fname);
	dialog.setLabelText(QFileDialog::Accept,tr("Save"));
//	dialog.setFileMode ( QFileDialog::DirectoryOnly );
	QString	path;
	if(dialog.exec()){
           QStringList pathList= dialog.selectedFiles();
	   path= pathList.at(0);
	}
	if( !path.isEmpty() )
	{
	     if(!path.endsWith(".sp")) path+=".sp";
             QString mappedSPpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter.sp");
             if(!FileExists(mappedSPpath.toLatin1().data())) return;
	     if(QFile::exists(path)) QFile::remove (path);
	     QFile::copy(mappedSPpath, path);
	}
}


void MainWindow::exportIdealMappedTzJC()
{
        QFileDialog dialog(this,tr("Export Ideal JC Circuit with mapped Tz"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter(tr("*.JC"));
	QString	fname=prjData.projectName+"_ideal_mappedTZ.JC";
	dialog.selectFile(fname);
	dialog.setLabelText(QFileDialog::Accept,tr("Save"));
//	dialog.setFileMode ( QFileDialog::DirectoryOnly );
	QString	path;
	if(dialog.exec()){
           QStringList pathList= dialog.selectedFiles();
	   path= pathList.at(0);
	}
	if( !path.isEmpty() )
	{
	     if(!path.endsWith(".JC")) path+=".JC";
             QString mappedJCpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter_mappedTZ.JC");
             if(!FileExists(mappedJCpath.toLatin1().data())) return;
	     if(QFile::exists(path)) QFile::remove (path);
	     QFile::copy(mappedJCpath, path);
	}
}


void MainWindow::exportIdealMappedTzSpice()
{
        QFileDialog dialog(this,tr("Export Ideal Spice Circuit with mapped Tz"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter(tr("*.JC"));
	QString	fname=prjData.projectName+"_ideal_mappedTZ.sp";
	dialog.selectFile(fname);
	dialog.setLabelText(QFileDialog::Accept,tr("Save"));
//	dialog.setFileMode ( QFileDialog::DirectoryOnly );
	QString	path;
	if(dialog.exec()){
           QStringList pathList= dialog.selectedFiles();
	   path= pathList.at(0);
	}
	if( !path.isEmpty() )
	{
	     if(!path.endsWith(".sp")) path+=".sp";
             QString mappedSPpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter_mappedTZ.sp");
             if(!FileExists(mappedSPpath.toLatin1().data())) return;
	     if(QFile::exists(path)) QFile::remove (path);
	     QFile::copy(mappedSPpath, path);
	}
}



void MainWindow::exportFilterTunePar()
{
        QFileDialog dialog(this,tr("Export Tunable Parameters of Filter Circuit"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter(tr("*.txt"));
	QString	fname=prjData.projectName+"_ideal_xpar.txt";
	dialog.selectFile(fname);
	dialog.setLabelText(QFileDialog::Accept,tr("Save"));
//	dialog.setFileMode ( QFileDialog::DirectoryOnly );
	QString	path;
	if(dialog.exec()){
           QStringList pathList= dialog.selectedFiles();
	   path= pathList.at(0);
	}
	if( !path.isEmpty() )
	{
	     if(!path.endsWith(".txt")) path+=".txt";
             QString mappedJCpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter_xpar.txt");
             if(!FileExists(mappedJCpath.toLatin1().data())) return;
	     if(QFile::exists(path)) QFile::remove (path);
	     QFile::copy(mappedJCpath, path);
	}
}


void MainWindow::newProject()
{
//	char tmp[]={"/mnt/vdisk/emwork/EmCAD-XXXXXX"};
	QString tmppath;
        #ifdef WNT
//           char tmp[]={"C:\\Temp\\EmCAD-XXXXXX"};
           std::string tmp_str=getenv("TEMP");
	   tmp_str+=std::string("\\EmCAD-XXXXXX");
           int l=tmp_str.size()+1;
           auto tmp=new char[l]; strcpy (tmp,tmp_str.c_str());
           int res = _mktemp_s(tmp, l);
           if (res != 0) return;
           tmppath=tmp;
           QDir tmpdir(tmppath);
           tmpdir.mkdir(tmppath);
           delete [] tmp;
        #else
           char tmp[]={"/tmp/EmCAD-XXXXXX"};
           tmppath=mkdtemp(tmp);
        #endif
	if(!FileExists(tmppath.toLatin1().data())) return;
	if( !tmppath.isEmpty() )
	{
               prjData.projectName="Untitled";
               QDir tmpdir(tmppath);
               tmpdir.mkdir("Work");
               mainWorkPath=tmppath+"/Work";
               QDir wkdir(mainWorkPath);
               wkdir.mkdir("Data");
               wkdir.mkdir(nativePath("Data/Circuits").c_str());
               QString matFileName=mainWorkPath+"/Data/materials.mat";
               FILE *fid= fopen(nativePath(matFileName.toLatin1().data()).c_str(), "w");
               fprintf(fid,"\n");
	       fclose(fid);
               QString gmshOptionFile=mainWorkPath+"/gmsh.opt";
               fid= fopen(nativePath(gmshOptionFile.toLatin1().data()).c_str(), "w");
               fprintf(fid,"Geometry.Points = 1;\n");
               fprintf(fid,"Mesh.Points = 0;\n");
               fprintf(fid,"Mesh.Lines = 1;\n");
               fprintf(fid,"Mesh.LineWidth = 2;\n");
               fprintf(fid,"Mesh.SurfaceFaces = 1;\n");
               fprintf(fid,"Mesh.VolumeEdges = 1;\n");
               fprintf(fid,"Mesh.VolumeFaces = 1;\n");
               fprintf(fid,"Mesh.ColorCarousel = 1;\n");
	       fclose(fid);
               setWorkProject(mainWorkPath);
               setModified(mainWorkPath);
	       runStatus.projectIsOpen=true;
	       mainOCAF->EmP.level=0;
               checkActions();
	       setProjectTitle();
	       prjData.saveSettings();
               prjData.workStatus.reset();
	}
}


bool isDownModified(QString dir, QStringList &subdirs){
 bool itis=false;
 for (int i = 0; i < subdirs.size(); ++i){
     QString fname=dir+"/"+subdirs.at(i)+"/model.brep.modified";
     fname=nativePath(fname);
     itis=itis||FileExists(fname.toLatin1().data());
 }
 return itis;
}
void rmDownModified(QString dir, QStringList &subdirs){
 bool itis=false;
 for (int i = 0; i < subdirs.size(); ++i){
     QString rmfile=dir+"/"+subdirs.at(i)+"/model.brep.modified";
     rmfile=nativePath(rmfile);
     remove(nativePath(rmfile.toLatin1().data()).c_str());
 }
}
bool isUpModified(QString dir){
   QString fname=dir+".brep.modified";
   fname=nativePath(fname);
   return FileExists(fname.toLatin1().data());
}
void rmUpModified(QString dir){
   QString rmfile=dir+".brep.modified";
   rmfile=nativePath(rmfile);
   remove(nativePath(rmfile.toLatin1().data()).c_str());
}


int Preprocessor::maxDepth(QString dir, int l)
{
  int md=l;
  QStringList subdirs;
  if(readStringlist(dir+"/subdirs", subdirs)){
#pragma omp parallel for shared(md,l,dir,subdirs)
   for (int i = 0; i < subdirs.size(); ++i){
      QString subDir=dir+"/"+subdirs.at(i);
      md=max(md,maxDepth(subDir, l+1));
   }
  }
  return md;

}


void Preprocessor::upImprintSubass(QString dir, QString assName, int l)
{

  QStringList subdirs;
  if(readStringlist(dir+"/subdirs", subdirs)){
     if(l==0) {
#pragma omp parallel for shared(l,dir,subdirs,assName)
       for (int i = 0; i < subdirs.size(); ++i){
         QString subDir=dir+"/"+subdirs.at(i);
         QString subAssName=assName+"__"+subdirs.at(i);
         upImprintSubass(subDir,subAssName, l+1);
       }
     } else {
       for (int i = 0; i < subdirs.size(); ++i){
         QString subDir=dir+"/"+subdirs.at(i);
         QString subAssName=assName+"__"+subdirs.at(i);
         upImprintSubass(subDir,subAssName, l+1);
        }
     }
  }

  QString dataDir=mainWorkPath+"/Data";
  QString app= QString("emcad_imprint");
  bool modified=isModified(dir);

  if(modified || isDownModified(dir, subdirs) ) {
   if(extproc){
      QStringList args;
      QString str_downImprint; 
      str_downImprint.setNum(0);
      args << str_downImprint;
      args << dir;
      QString command=app+"  "+str_downImprint+"  "+dir;
      char cmd[512]; strcpy(cmd,command.toLatin1().data());
      QProcess *proc=new QProcess;
      proc->setWorkingDirectory(nativePath(dataDir));
      proc->start(app, args);
      proc->waitForStarted();
      QByteArray outdata;
      while(proc->waitForReadyRead()) outdata.append(proc->readAllStandardOutput());
      proc->waitForFinished(-1); 
      msleep(100);
      QString mssg;
      if(proc->exitStatus()==QProcess::NormalExit){
	  mssg="Completed Up Imprinting of \"" ; mssg+=assName; mssg+="\"";
      }else{
	  mssg="Failed Up Imprinting of \"" ; mssg+=assName; mssg+="\"\n";
          QString outdataAsString=QString(outdata);
	  mssg+=outdataAsString;
	  decompose_failure=1;
      }
      sendLogMessage(mssg);
      proc->terminate();
//      delete procLog;
   }else{
      char dirname[101]; if(strlen(dir.toLatin1().data())< 100 ) strcpy(dirname,dir.toLatin1().data());
      std::cout <<"Starting upward imprinting of " << dirname <<"\n";
      MwOCAF* ocaf=new MwOCAF();
      ocaf->workopen(dir.toLatin1().data());
      if (ocaf->EmP.assemblyType==COMPONENT||ocaf->hasDownIF)  ocaf->imprint();
      ocaf->saveIF();
      if(!ocaf->EmP.level) { 
         if(ocaf->EmP.assemblyType==NET) ocaf->savePartsIF();
         ocaf->initFEPdataStruct();
	 if(ocaf->EmP.assemblyType==COMPONENT){
//	      ocaf->makeFaceAdjCells();
	      ocaf->setFaceComp();
	 }
      } 
      if(ocaf->EmP.assemblyType==COMPONENT) ocaf->updatePartColors();
      ocaf->worksave();
      ocaf->closeDoc();
      delete ocaf;
      QString mssg="Completed Up Imprinting of \""; mssg+=assName; mssg+="\"";
      sendLogMessage(mssg);
    }
/*---------------------------------------------------------------------------*/
      if(modified) rmModified(dir);
      rmDownModified(dir, subdirs);
      return;
  }



}


void Preprocessor::downImprintSubass(QString dir, QString assName, int l)
{

  QString dataDir=mainWorkPath+"/Data";
  QString app= QString("emcad_imprint");
  if(l>0){
   if(extproc){
      QStringList args;
      QString str_downImprint; 
      str_downImprint.setNum(1);
      args << str_downImprint;
      args << dir;
      QString command=app+"  "+str_downImprint+"  "+dir;
      char cmd[512]; strcpy(cmd,command.toLatin1().data());
      QProcess *proc=new QProcess;
      proc->setWorkingDirectory(nativePath(dataDir));
      proc->start(app, args);
      proc->waitForStarted();
      QByteArray outdata;
      while(proc->waitForReadyRead()) outdata.append(proc->readAllStandardOutput());
      proc->waitForFinished(-1);
      msleep(100);
      QString mssg;
      if(proc->exitStatus()==QProcess::NormalExit){
	  mssg="Completed Down Imprinting of \"" ; mssg+=assName; mssg+="\"";
      }else{
	  mssg="Failed Down Imprinting of \"" ; mssg+=assName; mssg+="\"\n";
          QString outdataAsString=QString(outdata);
	  mssg+=outdataAsString;
	  decompose_failure=1;
      }
      sendLogMessage(mssg);
      proc->terminate();
//      delete procLog;
   }else{
      char dirname[101]; if(strlen(dir.toLatin1().data())< 100 ) strcpy(dirname,dir.toLatin1().data());
      MwOCAF* ocaf=new MwOCAF();
      ocaf->workopen(dir.toLatin1().data());
      if (ocaf->EmP.assemblyType==COMPONENT||ocaf->hasDownIF)  ocaf->imprint();
      ocaf->worksave();
      if(ocaf->EmP.assemblyType==NET) ocaf->savePartsIF();
      ocaf->initFEPdataStruct();
      if(ocaf->EmP.assemblyType==COMPONENT){
//	   ocaf->makeFaceAdjCells();
	   ocaf->setFaceComp();
      }
      ocaf->closeDoc();
      delete ocaf;
      QString mssg="Completed Down Imprinting of \""; mssg+=assName; mssg+="\"";
      sendLogMessage(mssg);
   }
/*-----------------------------------------------------------------------------------------*/
  }


  QStringList subdirs;
  if(readStringlist(dir+"/subdirs", subdirs)){
     if(l==0) {
#pragma omp parallel for shared(l,dir,subdirs,assName)
       for (int i = 0; i < subdirs.size(); ++i){
         QString subDir=dir+"/"+subdirs.at(i);
         QString subAssName=assName+"__"+subdirs.at(i);
         downImprintSubass(subDir,subAssName, l+1);
       }
     } else {
       for (int i = 0; i < subdirs.size(); ++i){
         QString subDir=dir+"/"+subdirs.at(i);
         QString subAssName=assName+"__"+subdirs.at(i);
         downImprintSubass(subDir,subAssName, l+1);
        }
     }
  }



}



void Preprocessor::splitComponents(QString dir, QString assName )
{
  int assType=getAssType(dir);
  QString app= QString("emcad_split");
  QString dataDir=mainWorkPath+"/Data";
  if(assType==COMPONENT){
      QStringList args;
      args << dir;
      QString command=app+"  "+dir;
      char cmd[512]; strcpy(cmd,command.toLatin1().data());
      QProcess *proc=new QProcess;
      proc->setWorkingDirectory(nativePath(dataDir));
      proc->start(app, args);
      proc->waitForFinished(-1); 
      msleep(100);
      QString mssg;
      if(proc->exitStatus()==QProcess::NormalExit){
	  mssg="Completed Splitting of\"" ; mssg+=assName; mssg+="\"";
      }else{
	  mssg="Failed Splitting of\"" ; mssg+=assName; mssg+="\"";
	  decompose_failure=1;
      }
      sendLogMessage(mssg);
      proc->terminate();
   }

   if(decompose_failure) return;

   QStringList subdirs;
   if(assType==NET) if(readStringlist(dir+"/subdirs", subdirs)){
#pragma omp parallel for shared(dir,subdirs,assName)
    for (int i = 0; i < subdirs.size(); ++i){
      QString subDir=dir+"/"+subdirs.at(i);
      QString subAssName=assName+"__"+subdirs.at(i);
      splitComponents(subDir,subAssName);
    }
   }

}



void Preprocessor::worksaveComponents(QString dir)
{
  int assType=getAssType(dir);
  if(assType==COMPONENT){
    MwOCAF* ocaf=new MwOCAF();
    ocaf->workopen(dir.toLatin1().data());
    ocaf->worksave();
    ocaf->closeDoc();
    delete ocaf;
  }
  if(assType==NET){
   QStringList subdirs;
   if(readStringlist(dir+"/subdirs", subdirs)) for (int i = 0; i < subdirs.size(); ++i){
      QString subDir=dir+"/"+subdirs.at(i);
      char subdirname[101]; if(strlen(subDir.toLatin1().data())< 100 ) strcpy(subdirname,subDir.toLatin1().data());
      worksaveComponents(subDir);
   }
  }
}

void Preprocessor::worksaveAllComponents()
{
  worksaveComponents(currentWorkPath);
  prjData.workStatus.componentsaveNeeded=false;
}



void Preprocessor::setMaterialData(QString dir)
{
  MwOCAF* ocaf=new MwOCAF();
  ocaf->workopen(dir.toLatin1().data());
  if(ocaf->EmP.assemblyType==COMPONENT){
      ocaf->regenerateIndexedSubShapes();
      ocaf->setFEproperties();
      ocaf->setTEMnum();
      ocaf->addPorts(prjData.ports.map, prjData.portloads.map);
      if(ocaf->worksaveNeeded) ocaf->worksave();
      prjData.savePorts();
      QDir directory(dir);
      if(directory.exists(QString("Partition"))){
        QString partitionDir=dir+"/Partition";
        MwOCAF* subocaf=new MwOCAF();
        subocaf->workopen(partitionDir.toLatin1().data());
        subocaf->regenerateIndexedSubShapes();
        subocaf->setFEproperties();
        if(subocaf->worksaveNeeded) subocaf->worksave();
        subocaf->closeDoc();
        delete subocaf;
      }
  }
  ocaf->closeDoc();
  delete ocaf;

  QStringList subdirs;
  if(readStringlist(dir+"/subdirs", subdirs)) for (int i = 0; i < subdirs.size(); ++i){
      QString subDir=dir+"/"+subdirs.at(i);
      subDir=nativePath(subDir);
      char subdirname[101]; if(strlen(subDir.toLatin1().data())< 100 ) strcpy(subdirname,subDir.toLatin1().data());
      setMaterialData(subDir);
  }

}

void Preprocessor::setMaterialData(){
 setMaterialData(currentWorkPath);
 prjData.workStatus.materialChanged=false;
}

void Preprocessor::setSuperFaces(QString dir)
{
  char dir_[201]; strcpy(dir_,dir.toLatin1().data());
  int assType=getAssType(dir);
  if(assType==COMPONENT){
     MwOCAF* ocaf=new MwOCAF();
     ocaf->workopen(dir.toLatin1().data());
     ocaf->regenerateIndexedSubShapes();
     ocaf->readFEproperties();
     ocaf->setSuperFaces();
//     ocaf->checkSuperFacesTEMnum();
     ocaf->closeDoc();
     delete ocaf;
     QDir directory(dir);
     if(directory.exists(QString("Partition"))){
       QString partitionDir=dir+"/Partition";
       ocaf=new MwOCAF();
       ocaf->workopen(partitionDir.toLatin1().data());
       if(ocaf->subCompNum==0) ocaf->addToComponentLists(&(prjData.components.list),&(prjData.wgcomponents.list));
       int subCompNum=ocaf->subCompNum;
       ocaf->closeDoc();
       delete ocaf;
       for (int subcmpI=1; subcmpI<=subCompNum; subcmpI++) {
         ocaf=new MwOCAF();
         ocaf->workopen(partitionDir.toLatin1().data(),subcmpI);
	 ocaf->addToComponentLists(&(prjData.components.list),&(prjData.wgcomponents.list));
         ocaf->closeDoc();
         delete ocaf;
      }
    }
  }
  QStringList subdirs;
  if(readStringlist(dir+"/subdirs", subdirs)) for (int i = 0; i < subdirs.size(); ++i){
      QString subDir=dir+"/"+subdirs.at(i);
      setSuperFaces(subDir);
  }
}


void Preprocessor::meshModel(QString dir, QString assPath, int assType)
{
    if(prjData.freqBand[1]<1.e-4) return;
  
    QString meshPath=dir+".mwm";
    QString dxcPath;
    if(USEXMLXCAF) dxcPath=dir+"/model.xml";
    else           dxcPath=dir+"/model.dxc";

    if(!FileExists(dxcPath.toLatin1().data())) return;

    QString str_assType; str_assType.setNum(assType);

    double c0_mks =299792458;
    double waveLen=c0_mks/(prjData.freqBand[1]*prjData.freqUnit());
    double meshsize=waveLen/prjData.lengthUnit()/prjData.meshPerWavelen;
    QString str_meshsize; str_meshsize.setNum(meshsize,'f',5);
    QString str_meshpercircle; str_meshpercircle.setNum(prjData.meshPerCircle);
    QString str_onServer; str_onServer.setNum(0);
    QString str_subcompI; str_subcompI.setNum(0);
    QString str_mesh3DonClient; str_mesh3DonClient.setNum(prjData.localMeshing3d);

    QString subprjDir=dir;
    QString dataDir=mainWorkPath+"/Data";
    QString mwmDir=dataDir+"/Circuits";

    QString app= QString("emcad_mesh");
    QStringList args;
    args << QString("-asstype");
    args << str_assType;
    args << QString("-meshsize");
    args << str_meshsize;
    args << QString("-meshpercircle");
    args << str_meshpercircle;
    args << QString("-subcomp");
    args << str_subcompI;
    args << QString("-dir");
    args << nativePath(subprjDir);
    args << QString("-outdir");
    args << nativePath(mwmDir);
    args << QString("-onserver");
    args << str_onServer;
    args << QString("-mesh3DonClient");
    args << str_mesh3DonClient;
    QProcess *proc=new QProcess;
    proc->setWorkingDirectory(nativePath(dataDir));
    
    QString qcmd=app+QString("  ")+args.join(QString(" "));
    char cmd[512]; strcpy(cmd,qcmd.toLatin1().data());
    proc->start(app, args);
    proc->waitForFinished(-1); 
    msleep(300);
    if(proc->exitStatus()==QProcess::NormalExit){
       QString mssg="Completed Meshing of \"" ; mssg+=assPath; mssg+="\""; sendLogMessage(mssg);
    }else{
       QString mssg="Failed Meshing of \"" ; mssg+=assPath; mssg+=QString("\t"); mssg+=qcmd;
       mssg+="\""; sendLogMessage(mssg);
       mesh_failure=1;
    }
    
}


void Preprocessor::mesh(QString dir, QString assPath, int level, int l)
{
// before entering doc must be closed //
  QString asstypeFileName=dir+"/assemblyType";
  char fname[264]; strcpy(fname,asstypeFileName.toLatin1().data());
  FILE *in= fopen(nativePath(asstypeFileName.toLatin1().data()).c_str(), "r");
  int assType;
  fscanf(in, "%d", &assType);
  fclose(in);
  char dirname[101]; if(strlen(dir.toLatin1().data())< 100 ) strcpy(dirname,dir.toLatin1().data());
  if (l==level){
         int subCompNum=0;
	 if(assType==COMPONENT) subCompNum=getSubCompNum(dir);
//	 if(assType!=COMPONENT || subCompNum>0 || prjData.localMeshing3d) meshModel(dir, assPath, assType);
	 meshModel(dir, assPath, assType);

         QString compName;
         if(dir.endsWith(QString("Work"))) compName=assName;
	 else{
          QStringList compDirParts=dir.split('/');
          compName=compDirParts.takeLast();
	 }
	 if(assType==COMPONENT) tar_component(dir, compName);
	 return;
  }
  if(level>l && assType==NET){
   msleep(1000);
   QStringList subdirs;
   if(readStringlist(dir+"/subdirs", subdirs)){
      if(level==l+1){
          #pragma omp parallel for shared(l,dir,subdirs,assPath)
          for (int i = 0; i < subdirs.size(); ++i){
            QString subdir=dir+"/"+subdirs.at(i);
            QString subAssName=assPath+"__"+subdirs.at(i);
            mesh(subdir,subAssName, level, l+1);
          }
      } else
        for (int i = 0; i < subdirs.size(); ++i){
            QString subdir=dir+"/"+subdirs.at(i);
            QString subAssName=assPath+"__"+subdirs.at(i);
            mesh(subdir,subAssName, level, l+1);
        }

   }
  }
}

void Preprocessor::mesh()
{
  
  if(prjData.network || prjData.components.list.size()>1) saveMainCirc();
  int depth=maxDepth(currentWorkPath);
  mesh_failure=0;
  for (int level =0; level <= depth; ++level) 
      mesh(currentWorkPath, assName, level);
  prjData.workStatus.remeshNeeded=0;
  prjData.saveWorkStatus();
}

void Preprocessor::decompose(){
   int depth=maxDepth(currentWorkPath);
   upImprintSubass(currentWorkPath, assName);
   if(decompose_failure) return;
   downImprintSubass(currentWorkPath, assName);
   if(decompose_failure) return;
   splitComponents(currentWorkPath, assName);
   if(decompose_failure) return;
   prjData.ports.clear();
   prjData.components.list.clear();
   prjData.wgcomponents.list.clear();
   setSuperFaces(currentWorkPath);
   setMaterialData();
   QString compFileName=mainWorkPath+"/components";
   prjData.components.save(compFileName.toLatin1().data());
   QString wgcompFileName=mainWorkPath+"/wgcomponents";
   prjData.wgcomponents.save(wgcompFileName.toLatin1().data());
   prjData.workStatus.decompositionNeeded=0;
   prjData.workStatus.firstDecomposition=0;
}


void Preprocessor::run(){
  QString mssg;
  if(prjData.workStatus.componentsaveNeeded) worksaveAllComponents();
  if(prjData.workStatus.decompositionNeeded){
     mainOCAF->closeDoc();
     sendStatusMessage(QString("Decomposing Geom Objects"));
     decompose();
     if(!decompose_failure) sendStatusMessage(QString("Completed Decomposition"));
     else                   sendStatusMessage(QString("Failed Decomposition"));
     mainOCC->deleteAllObjects();
     mainOCAF->workopen(currentWorkPath.toLatin1().data());
     emit(decomposerEnd());
  }
  if(!decompose_failure && prjData.workStatus.materialChanged) updateMaterialData();
  if(requiredOperation<=DECOMPOSITION){
     sendStatusMessage(QString("Ready"));
     exit();
     return;
  }
  if(decompose_failure){
     sendStatusMessage(QString("Ready"));
     emit(mesherEnd());
     exit();
     return;
  }
  msleep(100);
  sendStatusMessage(QString("Meshing"));
  mesh();
  mssg="Completed Meshing\"";
  sendStatusMessage(mssg);
  emit(mesherEnd());
  sendStatusMessage(QString("Ready"));
  exit();
}



void MainWindow::openAllDoc(QString dir)
{
  Handle(TDocStd_Document) doc;
  QString occFilePath=dir;
  if(USEXMLXCAF) dir+="/model.xml";
  else           dir+="/model.dxc";

  PCDM_ReaderStatus status;
  TCollection_ExtendedString Oname=TCollection_ExtendedString(occFilePath.toLatin1().data());
  Handle(XCAFApp_Application) XCAFapp=XCAFApp_Application::GetApplication();
  if(USEXMLXCAF)  XCAFapp->DefineFormat ("XmlXCAF", "Xml XCAF Document", "xml",
                                         new XmlXCAFDrivers_DocumentRetrievalDriver, 
                                         new XmlXCAFDrivers_DocumentStorageDriver ("Copyright: Open Cascade, 2001-2002"));
  int I=XCAFapp->IsInSession(Oname);
  if(I>0) XCAFapp->GetDocument(I,doc);
  else    status=XCAFapp->Open(Oname,doc);
  QString asstypeFilePath=dir+"/assemblyType";
  char fname[264]; strcpy(fname,asstypeFilePath.toLatin1().data());
  FILE *in= fopen(nativePath(asstypeFilePath.toLatin1().data()).c_str(), "r");
  int assType;
  fscanf(in, "%d", &assType);
  fclose(in);
  if(assType==NET){
   QStringList subdirs;
   if(readStringlist(dir+"/subdirs", subdirs)) for (int i = 0; i < subdirs.size(); ++i) openAllDoc(dir+"/"+subdirs.at(i));
  }
}


void MainWindow::startOperation(){
  if(prjData.workStatus.reloadNeeded){
      prjData.workStatus.reloadNeeded=0;
      reload();
  }
  preproc.currentWorkPath=currentWorkPath;
  TCollection_AsciiString name;
  mainOCAF->getAssName(name);
  preproc.assName=name.ToCString();
  preproc.receiver=this;

  preproc.start();

}


void MainWindow::decompose()
{
  if(!prjData.workStatus.decompositionNeeded) return;
  if(!mainOCAF->prjStatus.partMaterials) return;
  if(mainOCAF->worksaveNeeded) worksave();

  runStatus.runningDecomposer=true; checkActions();

  preproc.requiredOperation=DECOMPOSITION;
  startOperation();

}


void MainWindow::mesh(){
  if(!mainOCAF->prjStatus.partMaterials) return;
  if(mainOCAF->worksaveNeeded) worksave();

  runStatus.runningMesher=true; checkActions();

  preproc.requiredOperation=MESHING;
  startOperation();

}


void MainWindow::updateAccountCredit()
{
    QString script;
    if(useAWS){
       script=QString(emcadPath);
       #ifdef WNT
         script.chop(13);
	 QString ext=".exe";
       #else
         script.chop(9);
	 QString ext=".py";
       #endif
       script+="bin/aws_getcredit";
       script=nativePath(script+ext);
    }else{
       return;
    }

    QProcess *proc=new QProcess;
    QString app=script;
    QStringList args;
    args << QString("-bucket");
    args << QString(emcadConfig[std::string("AWS_bucket")].c_str());
    args << QString("-account_filename");
    args << QString(account_filename.c_str());
    QString Cmd=app+QString("  ")+args.join(QString(" "));
    char * cmd=Cmd.toLatin1().data();
    proc->start(app, args);
    proc->waitForStarted();
    proc->waitForFinished(-1);
    msleep(100);
    QString mssg;
    if(proc->exitCode()==1){
       mssg="Could not update credit" ;
       temporaryMessage(mssg);
    }
    proc->terminate();
}


void Preprocessor::saveMainCirc(){

   QString dataDir=mainWorkPath+"/Data";
   typedef std::map<std::string, int, std::less<std::string> >::const_iterator MapIt;
#if defined(GMSH3D)
   QString mjcPath=dataDir+"/Circuits/"+prjData.mainAssName+".JC";
   FILE *ckfid= fopen(nativePath(mjcPath.toLatin1().data()).c_str(), "w");
   fprintf(ckfid, "\n.subckt  %s",  prjData.mainAssName.toLatin1().data());
   for ( MapIt it=prjData.ports.map.begin(); it!= prjData.ports.map.end(); it++){
      PortType pt=portTypeFromName((*it).first.c_str());
      bool inverted;
      if(pt==PortTypeTE)  inverted=INVERTEDTEPORTS;
      if(pt==PortTypeTM)  inverted=INVERTEDTMPORTS;
      if(pt==PortTypeTEM) inverted=INVERTEDTEMPORTS;
      for (int i=0; i< (*it).second; i++) 
	  if(inverted) fprintf(ckfid, "\n+   %si%d", (*it).first.c_str(), i+1);
	  else         fprintf(ckfid, "\n+   %s%d", (*it).first.c_str(), i+1);
   }
#else
   QString mjcPath=dataDir+"/Circuits/"+prjData.mainAssName+".MJC";
   FILE *ckfid= fopen(nativePath(mjcPath.toLatin1().data()).c_str(), "w");
   fprintf(ckfid, "\n.Msubckt  %s",  prjData.mainAssName.toLatin1().data());
   for ( MapIt it=prjData.ports.map.begin(); it!= prjData.ports.map.end(); it++)
     if((*it).second) fprintf(ckfid, "\n+   %s  %d", (*it).first.c_str(), (*it).second);
#endif
   fprintf(ckfid, "\n\n");
   typedef std::set<std::string, std::less<std::string> >::const_iterator StrIt;
   for ( StrIt it=prjData.components.list.begin(); it!= prjData.components.list.end(); it++){
      QString compFileName=it->c_str();
#if defined(GMSH3D)
      compFileName+="_RM.JC";
#else
      compFileName+=".MJC";
#endif
      fprintf(ckfid, ".include  %s\n", compFileName.toLatin1().data());
   }
   fprintf(ckfid, "\n");

   fprintf(ckfid, "\n.ends  %s\n\n", prjData.mainAssName.toLatin1().data());

   fclose(ckfid);

}


void Preprocessor::updateMaterialData()
{
  if(prjData.workStatus.componentsaveNeeded) worksaveAllComponents();
  prjData.ports.clear();
  setMaterialData(currentWorkPath);
  if(prjData.components.list.size()>1) saveMainCirc();
  prjData.workStatus.materialChanged=0;
  prjData.workStatus.remeshNeeded=1;
}

void Modeler::initModeledComponent()
{
    QString workDir=mainWorkPath+"/Data/Circuits";
    modeledComponent.clear();
    typedef std::set<std::string, std::less<std::string> >::const_iterator StrIt;
    for ( StrIt it=prjData.components.list.begin(); it!= prjData.components.list.end(); it++){
      QString compName=(*it).c_str();
      QString infilePath=workDir+"/"+compName;
      if(MESH3D_ON_SERVER)  infilePath+=".tgz";
      else                  infilePath+=".mwm";
      QString logName=compName+".log";
      QString logPath=workDir+"/"+logName;
      if(!FileExists(infilePath.toLatin1().data())) continue;
      else if(prjData.workStatus.modelizationNeeded) modeledComponent[*it]=0;
      else if(!FileExists(logPath.toLatin1().data())) modeledComponent[*it]=0;
      else if( file1NewerThanFile2(infilePath.toLatin1().data(), logPath.toLatin1().data()) )  modeledComponent[*it]=0;
   }
}

void Modeler::initModeledComponentWg()
{
    QString workDir=mainWorkPath+"/Data/Circuits";
    modeledWgComponent.clear();
    typedef std::set<std::string, std::less<std::string> >::const_iterator StrIt;
    for ( StrIt it=prjData.wgcomponents.list.begin(); it!= prjData.wgcomponents.list.end(); it++){
      QString compName=(*it).c_str();
      compName+="_WG";
      QString infilePath=workDir+"/"+compName;
      if(MESH3D_ON_SERVER)  infilePath+=".tgz";
      else                  infilePath+=".mwm";
      QString logName=compName+".log";
      QString logPath=workDir+"/"+logName;
      if(!FileExists(infilePath.toLatin1().data())) continue;
      if(!FileExists(logPath.toLatin1().data())) modeledWgComponent[*it]=0;
      else if( file1NewerThanFile2(infilePath.toLatin1().data(),logPath.toLatin1().data()) )  modeledWgComponent[*it]=0;
   }
}


void Modeler::server_secrets(int put)
{
     QString script=QString(emcadPath);
     #ifdef WNT
        script.chop(13);
        QString ext=".exe";
     #else
        script.chop(9);
        QString ext=".py";
     #endif
       script="mwdocker_run";
     QString app;
     QStringList args;
     app= script;
     args << QString("-job");
     if(put) args << QString("secrets_put");
     else    args << QString("secrets_delete");
     args << QString("-project");
     args << prjData.projectName;
     if(emcadConfig.find(std::string("server"))!=emcadConfig.end()) if(emcadConfig[std::string("server")].length()>0) {
            args << QString("-server");
	    args << QString(emcadConfig[std::string("server")].c_str());
     }
     QString Cmd=app+QString("  ")+args.join(QString(" "));
     char * cmd=Cmd.toLatin1().data();
     QString mssg;
     QProcess *proc=new QProcess;
     QString workDir=mainWorkPath+"/Data/Circuits";
     proc->setWorkingDirectory(nativePath(workDir));
     proc->setStandardErrorFile(workDir+QString("/secrets_error.log"));
     proc->start(app, args);
     proc->waitForFinished(-1);
     msleep(100);
}


int name2subCompI(const char *name){
 int l=strlen(name)-1; if(!isdigit(name[l])) return 0;
 for(l=strlen(name)-1; l>0&&((!isalpha(name[l])||name[l]=='i')); ) l--;
 int subCompI=0;
 if( l>1 && !strncmp(&name[l-2],"SUB",3))  sscanf(&name[l+1], "%d", &subCompI);
 return subCompI;
}

void Modeler::modelize(std::string compNameStr)
{
     QString compName=compNameStr.c_str();
     QString script=QString(emcadPath);
     #ifdef WNT
        script.chop(13);
        QString ext=".exe";
     #else
        script.chop(9);
        QString ext=".py";
     #endif
     if(useAPI){
       script+="bin/api_submit";
       script=nativePath(script+ext);
     }else if(useDOCKER){
       script="mwdocker_run";
     }else{
#if defined(GMSH3D)
       script="modelize1";
#else
       if(prjData.network) script="modelizeb";
       else                script="modelize1";
#endif
     }
     QString app;
     QStringList args;
     app= script;
     QString freq1; freq1.setNum(prjData.freqBand[0],'f',5);
     QString freq2; freq2.setNum(prjData.freqBand[1],'f',5);
     QString rfreq1; rfreq1.setNum(prjData.refFreqBand[0],'f',5);
     QString rfreq2; rfreq2.setNum(prjData.refFreqBand[1],'f',5);
     QString cutoff; cutoff.setNum(prjData.cutoffRatio,'f',5);
     QString KrylovOrder; KrylovOrder.setNum(prjData.KrylovOrder);
     QString fnum; fnum.setNum(prjData.MORFreqNum);
     if(useAPI){ 
       args << QString("-user");
       args << QString(emcadConfig[std::string("user")].c_str());
       args << QString("-password");
       args << QString(emcadConfig[std::string("password")].c_str());
       args << QString("-project");
       args << prjData.projectName;
       args << QString("-job");
#if defined(GMSH3D)
       args << QString("modelize1");
#else
       if(prjData.network) args << QString("modelizeb");
       else                args << QString("modelize1");
#endif
     } else if(useDOCKER){
       args << QString("-job");
#if defined(GMSH3D)
       args << QString("modelize1");
#else
       if(prjData.network) args << QString("modelizeb");
       else                args << QString("modelize1");
#endif
       if(useServer){
          args << QString("-project");
          args << prjData.projectName;
          if(emcadConfig.find(std::string("server"))!=emcadConfig.end()) if(emcadConfig[std::string("server")].length()>0) {
                 args << QString("-server");
		 args << QString(emcadConfig[std::string("server")].c_str());
	  }
       }
     }
     if((!prjData.network && prjData.components.list.size()==1)|| (!INVERTEDTEPORTS && !INVERTEDTMPORTS && !INVERTEDTEMPORTS) )  args << QString("-inv");
     if(!prjData.network){
      args << QString("-include");
      args << QString("extern_port_loads.JC");
      args << QString("-include");
      args << QString("port_impedance_circuits.JC");
     }
     if(MESH3D_ON_SERVER){
      double c0_mks =299792458;
      double waveLen=c0_mks/(prjData.freqBand[1]*prjData.freqUnit());
      double meshsize=waveLen/prjData.lengthUnit()/prjData.meshPerWavelen;
      QString str_meshsize; str_meshsize.setNum(meshsize,'f',5);
      QString str_meshRefineMaxNum; str_meshRefineMaxNum.setNum(prjData.meshRefineMaxNum);
      QString str_meshMinEnergyRatio; str_meshMinEnergyRatio.setNum(prjData.meshMinEnergyRatio,'f',5);
      QString str_meshpercircle; str_meshpercircle.setNum(prjData.meshPerCircle);
      int subcompI=name2subCompI(compName.toLatin1().data());
      QString str_subcompI; str_subcompI.setNum(subcompI);
      args << QString("-meshsize");
      args << str_meshsize;
      args << QString("-meshrefine");
      args << rfreq1;
      args << rfreq2;
      args << prjData.freqUnitName();
      args << str_meshRefineMaxNum;
      args << str_meshMinEnergyRatio;
      args << QString("-meshpercircle");
      args << str_meshpercircle;
      args << QString("-subcomp");
      args << str_subcompI;
     }
#if defined(GMSH3D)
     if(prjData.components.list.size()>1) args << QString("-call");
#endif
     args << QString("-k");
     args << KrylovOrder;
     args << QString("-freq");
     args << freq1;
     args << freq2;
     args << prjData.freqUnitName();
     args << fnum;
     args << QString("-cutoff");
     args << cutoff;
     args << compName;
     QString Cmd=app+QString("  ")+args.join(QString(" "));
     char * cmd=Cmd.toLatin1().data();
     QString mssg;
     QProcess *proc=new QProcess;
     QString workDir=mainWorkPath+"/Data/Circuits";
     proc->setWorkingDirectory(nativePath(workDir));
     proc->setStandardErrorFile(workDir+QString("/modelize_error.log"));
     proc->start(app, args);
//     mssg="Started modelization of \""; mssg+=compName; mssg+="\"";
//     sendLogMessage(mssg);
     proc->waitForFinished(-1);
     msleep(100);
     if(proc->exitCode()==0){
        if(useAPI)    {mssg="Completed Uploading of \"" ; mssg+=compName; mssg+="\"";}
	else          {mssg="Completed Modelization of \"" ; mssg+=compName; mssg+="\"";}
        sendLogMessage(mssg);
     }else if(proc->exitCode()>0){
        modeledComponent[compNameStr]=1;
        failure=1;
        if(useAPI)
	 {mssg="Failed Uploading of \"" ; mssg+=compName; mssg+="\"";}
	else {
	  if(proc->exitCode()==1) {mssg="Failed Modelization of \"" ; mssg+=compName; mssg+="\"";}
	  if(proc->exitCode()==2) {mssg="Failed Model Order Reduction  of \"" ; mssg+=compName; mssg+="\"";}
	  QString logFilePath=workDir+"/"+compName+".log";
	  QFile logfile(logFilePath);
	  if(logfile.open(QIODevice::ReadOnly | QIODevice::Text)){
	    QTextStream logtext(&logfile);
	    QString logline=logtext.readLine();
	    while(!logline.isNull()){
	      mssg+="\n\t"+logline;
	      logline=logtext.readLine();
	    }
            logfile.close();
	  }
	}
        sendLogMessage(mssg);
     }
     proc->terminate();
}

 
int read_result_file(QString resultFilePath)
{
    QFile resfile(resultFilePath);
    int res=-1;
    if(resfile.open(QIODevice::ReadOnly | QIODevice::Text)){
	   QTextStream restext(&resfile);
	   QString resline=restext.readLine();
	   res=resline.toInt();
           resfile.close();
    }
    return res;
}

QString read_string_from_file(QString filePath)
{
    QFile file(filePath);
    QString string="";
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
	 QTextStream in(&file);
//	 in.setCodec("UTF-8");
	 string=in.readLine();
         file.close();
    }
    return string;
}

void Modeler::api_get_component_results(modelerTask operation, std::string compNameStr)
{
     QString compName=compNameStr.c_str();
     QString app;
     app=QString(emcadPath);
     #ifdef WNT
         app.chop(13);
	 QString ext=".exe";
     #else
         app.chop(9);
	 QString ext=".py";
     #endif
     app+="bin/api_download";
     app=nativePath(app+ext);
     QStringList resultFiles;
#if defined(GMSH3D)
     if(operation==CIRCUITS){
       resultFiles << compName+QString(".res");
       resultFiles << compName+QString(".log");
       resultFiles << compName+QString("_RM.JC");
       resultFiles << compName+QString("_RM.sp");
     }else if (operation==PORTS){
       resultFiles << compName+QString(".res");
       resultFiles << compName+QString(".log");
     }
#else
     if(operation==CIRCUITS)
       if(prjData.network){
         resultFiles << compName+QString(".res");
         resultFiles << compName+QString(".log");
         resultFiles << compName+QString("_RM.MJC");
         resultFiles << compName+QString("_RM.bin");
       } else {
         resultFiles << compName+QString(".res");
         resultFiles << compName+QString(".log");
         resultFiles << compName+QString("_RM.JC");
         resultFiles << compName+QString("_RM.sp");
       }
     }else if (operation==PORTS){
       resultFiles << compName+QString(".res");
       resultFiles << compName+QString(".log");
       resultFiles << compName+QString("*.port");
     }
#endif
     int exitCode=-1;
     QString mssg;
     for (QStringList::iterator it=resultFiles.begin(); it != resultFiles.end(); ++it) {
         QString resultFile = *it;
         QStringList args;
         args << QString("-user");
         args << QString(emcadConfig[std::string("user")].c_str());
         args << QString("-password");
         args << QString(emcadConfig[std::string("password")].c_str());
         args << QString("-folder");
         args << prjData.projectName;
	 if(it==resultFiles.begin()) args << QString("-saveUrl");
         args << resultFile;
         QString Cmd=app+QString("  ")+args.join(QString(" "));
         char * cmd=Cmd.toLatin1().data();
         QProcess *proc=new QProcess;
         QString workDir=mainWorkPath+"/Data/Circuits";
         proc->setWorkingDirectory(nativePath(workDir));
	 proc->setStandardErrorFile(workDir+QString("/get_component_results_error.log"));
         proc->start(app, args);
         proc->waitForFinished(-1);
         msleep(100);
         if(proc->exitCode()>0){
           if(operation==CIRCUITS) exitCode=1;
           if(operation==PORTS)    exitCode=2;
           break;
         }
         if(it==resultFiles.begin()){ //first resultfile must be compName+".res"
           QString resultFilePath=workDir+"/"+resultFile;
           if(FileExists(resultFilePath.toLatin1().data())) exitCode=read_result_file(resultFilePath);
	   if(exitCode==-1) break; //   result not yet ready
	 }
     }
     if(exitCode>0){
          if(operation==CIRCUITS){
            if(exitCode==1) {mssg="Failed Modelization of \"" ; mssg+=compName; mssg+="\"";}
            if(exitCode==2) {mssg="Failed Model Order Reduction  of \"" ; mssg+=compName; mssg+="\"";}
	  }
          if(operation==PORTS){
            mssg="Failed Port Modes of \"" ; mssg+=compName; mssg+="\"";
	  }
          failure=1;
          modeledComponent[compNameStr]=1;
          sendLogMessage(mssg);
      }
      else if(exitCode==0){
          if(operation==CIRCUITS) {mssg="Completed Modelization of \"" ; mssg+=compName; mssg+="\"";}
          if(operation==PORTS) {mssg="Computed Port Modes of \"" ; mssg+=compName; mssg+="\"";}
          modeledComponent[compNameStr]=1;
          sendLogMessage(mssg);
      }    
}



void Modeler::mainReduce(){
     QString script=QString(emcadPath);
     #ifdef WNT
       script.chop(13);
       QString ext=".exe";
     #else
       script.chop(9);
       QString ext=".py";
     #endif
     if(useAPI){
       script+="bin/api_submit";
       script=nativePath(script+ext);
     }else if(useDOCKER){
       script="mwdocker_run";
     }else{
       script="reduce";
     }
     QString app=script;
     QStringList args;
     #if not defined(GMSH3D)
        args << QString("-mjc");
     #endif	
     QString freq1; freq1.setNum(prjData.anaFreqBand[0],'f',5);
     QString freq2; freq2.setNum(prjData.anaFreqBand[1],'f',5);
     QString fnum;  fnum.setNum(prjData.anaMORFreqNum);
     if(useAPI){
       args << QString("-user");
       args << QString(emcadConfig[std::string("user")].c_str());
       args << QString("-password");
       args << QString(emcadConfig[std::string("password")].c_str());
       args << QString("-job");
       args << QString("reduce");
       args << QString("-project");
       args << prjData.projectName;
     } else if(useDOCKER){
       args << QString("-job");
       args << QString("reduce");
       if(useServer){
          args << QString("-project");
          args << prjData.projectName;
          if(emcadConfig.find(std::string("server"))!=emcadConfig.end()) if(emcadConfig[std::string("server")].length()>0) {
                 args << QString("-server");
		 args << QString(emcadConfig[std::string("server")].c_str());
	  }
       }
     }
     args << QString("-include");
     args << QString("extern_port_loads.JC");
     args << QString("-include");
     args << QString("port_impedance_circuits.JC");
     args << QString("-k");
     args << QString("1");
     args << QString("-freq");
     args << QString(freq1);
     args << QString(freq2);
     args << prjData.freqUnitName();
     args << QString(fnum);
     args << prjData.mainAssName;
     QString Cmd=app+QString("  ")+args.join(QString(" "));
     if(useServer) server_secrets(1);
     char cmd[512];
     strcpy(cmd,Cmd.toLatin1().data());
     QProcess *proc=new QProcess;
     QString workDir=mainWorkPath+"/Data/Circuits";
     proc->setWorkingDirectory(nativePath(workDir));
     proc->start(app, args);
     bool started=proc->waitForStarted();
     proc->waitForFinished(-1);
     msleep(100);
     QString mssg;
     if(useServer) server_secrets(0);
     if(proc->exitCode()==0){
        mssg="Completed Reduction of \"" ; mssg+=prjData.mainAssName; mssg+="\"";
        prjData.workStatus.reductionNeeded=0;
        prjData.saveWorkStatus();
     }else {
        mssg="Failed Reduction of \"" ; mssg+=prjData.mainAssName; mssg+="\"";
        failure=1;
        QString logFilePath=workDir+"/"+prjData.mainAssName+".log";
        QFile logfile(logFilePath);
	if(logfile.open(QIODevice::ReadOnly | QIODevice::Text)){
	   QTextStream logtext(&logfile);
	   QString logline=logtext.readLine();
	   while(!logline.isNull()){
	     mssg+="\n\t"+logline;
	     logline=logtext.readLine();
	   }
           logfile.close();
	}
     }
     sendLogMessage(mssg);
}



void Modeler::freqAna()
{
    QString script=QString(emcadPath);
    #ifdef WNT
       script.chop(13);
       QString ext=".exe";
    #else
       script.chop(9);
       QString ext=".py";
    #endif
    if(useAPI){
       script+="bin/api_submit";
       script=nativePath(script+ext);
    }else if(useDOCKER){
       script="mwdocker_run";
    }else{
       script="analyse";
    }
    QString circuitName;
    switch(prjData.analysedCircuit){
	 case ELECROMAGNETICDEVICE: 
            circuitName=prjData.mainAssName+"_RM";
            break;
	 case MAPPEDCIRCUIT:
	    if(prjData.filtermapSource==ZEROPOLES) circuitName=prjData.mainAssName+"_RM_mapped";
            if(prjData.filtermapSource==IMPORTED_RESPONSE) circuitName="imported_response_mapped";
            if(prjData.filtermapSource==IMPORTED_CIRCUIT)  circuitName="imported_circuit";
            break;
	 case IDEALCIRCUIT:
            circuitName="ideal_filter";
            break;
	 case  IDEALCIRCUITMAPPEDTZ:
            circuitName="ideal_filter_mappedTZ";
            break;
    }
    QString workDir=mainWorkPath+"/Data/Circuits";
    QString JCpath=workDir+"/"+circuitName+".JC";
    char *fname=JCpath.toLatin1().data();
    if(!FileExists(JCpath.toLatin1().data())) return;

    QProcess *proc=new QProcess;
    proc->setWorkingDirectory(nativePath(workDir));

    QString app=script;
    QStringList args;
    if(useAPI){
       args << QString("-user");
       args << QString(emcadConfig[std::string("user")].c_str());
       args << QString("-password");
       args << QString(emcadConfig[std::string("password")].c_str());
       args << QString("-project");
       args << prjData.projectName;
       args << QString("-job");
       args << QString("analyse");
    } else if(useDOCKER){
       args << QString("-job");
       args << QString("analyse");
       if(useServer){
          args << QString("-project");
          args << prjData.projectName;
          if(emcadConfig.find(std::string("server"))!=emcadConfig.end()) if(emcadConfig[std::string("server")].length()>0) {
                 args << QString("-server");
		 args << QString(emcadConfig[std::string("server")].c_str());
	  }
       }
    }
    QString freq1,freq2,fnum;
    freq1.setNum(prjData.anaFreqBand[0],'f',5);
    freq2.setNum(prjData.anaFreqBand[1],'f',5);
    fnum.setNum(prjData.anaFreqNum);
    switch(prjData.freqRespParType){
	 case SPAR: args << QString("-s"); break;
	 case ZPAR: args << QString("-z"); args << QString("-ri"); break;
	 case YPAR: args << QString("-y"); args << QString("-ri"); break;
    }
    args << QString("-freq");
    args << freq1;
    args << freq2;
    args << prjData.freqUnitName();
    args << fnum;
    args << circuitName;
    QString Cmd=app+QString("  ")+args.join(QString(" "));
    char * cmd=Cmd.toLatin1().data();
    if(useServer) server_secrets(1);
    proc->start(app, args);
    proc->waitForStarted();
    proc->waitForFinished(-1);
    if(useServer) server_secrets(0);
    msleep(100);
    QString mssg;
    if(proc->exitCode()==0){
       mssg="Completed Freq Response Analysis of \"" ; mssg+=circuitName; mssg+="\"";
    }else if(proc->exitCode()==1){
       mssg="Failed Freq Response Analysis of \"" ; mssg+=circuitName; mssg+="\"";
    }
    sendLogMessage(mssg);
    emit(freqAnaEnd());
    exit();
}


void Modeler::zeropoleAna()
{
//generates XXX_RM.SZP and completes symmfilter.inp
    QString script=QString(emcadPath);
    #ifdef WNT
      script.chop(13);
      QString ext=".exe";
    #else
      script.chop(9);
      QString ext=".py";
    #endif
    if(useAPI){
       script+="bin/api_submit";
       script=nativePath(script+ext);
    }else if(useDOCKER){
       script="mwdocker_run";
    }else{
       script="analyseszp";
    }
    QString app=script;
    QStringList args;
    QString circuitName;
    switch(prjData.analysedCircuit){
	 case ELECROMAGNETICDEVICE: 
            circuitName=prjData.mainAssName+"_RM";
            args << QString("-symminput");
            break;
	 case MAPPEDCIRCUIT:
	    if(prjData.filtermapSource==ZEROPOLES) circuitName=prjData.mainAssName+"_RM_mapped";
            if(prjData.filtermapSource==IMPORTED_RESPONSE) circuitName="imported_response_mapped";
            if(prjData.filtermapSource==IMPORTED_CIRCUIT)  circuitName="imported_circuit";
            break;
	 case IDEALCIRCUIT:
            circuitName="ideal_filter";
            break;
    }
    QString workDir=mainWorkPath+"/Data/Circuits";
    QString JCpath=workDir+"/"+circuitName+".JC";
    if(!FileExists(JCpath.toLatin1().data())) return;

    QProcess *proc=new QProcess;
    proc->setWorkingDirectory(nativePath(workDir));

    if(useAPI){
       args << QString("-user");
       args << QString(emcadConfig[std::string("user")].c_str());
       args << QString("-password");
       args << QString(emcadConfig[std::string("password")].c_str());
       args << QString("-project");
       args << prjData.projectName;
       args << QString("-job");
       args << QString("analyseszp");
    } else if(useDOCKER){
       args << QString("-job");
       args << QString("analyseszp");
       if(useServer){
          args << QString("-project");
          args << prjData.projectName;
          if(emcadConfig.find(std::string("server"))!=emcadConfig.end()) if(emcadConfig[std::string("server")].length()>0) {
                 args << QString("-server");
		 args << QString(emcadConfig[std::string("server")].c_str());
	  }
       }
    }
    QString freq1,freq2,winRatio,fnum;
    freq1.setNum(prjData.zpFreqBand[0],'f',5);
    freq2.setNum(prjData.zpFreqBand[1],'f',5);
    winRatio.setNum(prjData.zpWinRatio,'f',5);

    fnum.setNum(50);
    args << QString("-freq");
    args << freq1;
    args << freq2;
    args << prjData.freqUnitName();
    args << fnum;
    QString cnum; cnum.setNum(prjData.zeropoleCurves.size()); args << cnum;
    for ( ZeroPoleCurvesIterator it=prjData.zeropoleCurves.begin(); it!= prjData.zeropoleCurves.end(); it++){
     QString port1; port1.setNum((*it).first); args << port1;
     QString port2; port2.setNum((*it).second); args << port2;
     QString freq1; freq1.setNum(prjData.zpFreqBand[0],'f',5); args << freq1;
     QString freq2; freq2.setNum(prjData.zpFreqBand[1],'f',5); args << freq2;
    }
    args << prjData.freqUnitName();
    args << QString("-zpwinratio");
    args << winRatio;
    args << circuitName;
    QString Cmd=app+QString("  ")+args.join(QString(" "));
    char *cmd=Cmd.toLatin1().data();
    if(useServer) server_secrets(1);
    proc->start(app, args);
    proc->waitForStarted();
    proc->waitForFinished(-1);
    msleep(100);
    if(useServer) server_secrets(0);
    QString mssg;
    if(proc->exitCode()==0){
       mssg="Completed Zero Pole Analysis of \"" ; mssg+=circuitName; mssg+="\"";
    }else{
       mssg="Failed Zero Pole Analysis of \"" ; mssg+=circuitName; mssg+="\"";
    }
    sendLogMessage(mssg);
    emit(zeropoleEnd());
    exit();
}


void Modeler::filterDesign(bool mappedTxZeros)
{
    QString script=QString(emcadPath);
    #ifdef WNT
       script.chop(13);
       QString ext=".exe";
    #else
       script.chop(9);
       QString ext=".py";
    #endif
    if(useAPI){
       script+="bin/api_submit";
       script=nativePath(script+ext);
    }else if(useDOCKER){
       script="mwdocker_run";
    }else{
       script="symmfilter";
    }
    QString workDir=mainWorkPath+"/Data/Circuits";

    QString cktName;

    char f_unit[5];
    int fI=prjData.freqUnitE/3;
    switch(fI){
	 case 0: strcpy(f_unit,"Hz"); break;
	 case 1: strcpy(f_unit,"KHz"); break;
	 case 2: strcpy(f_unit,"MHz"); break;
	 case 3: strcpy(f_unit,"GHz"); break;
    }

    if(!mappedTxZeros) {
      cktName=QString("ideal_filter");
      FILE *mufid;
      QString MUpath=workDir+"/"+cktName+".inp";
      if(!(mufid = fopen(MUpath.toLatin1().data(), "w"))){
        emit(filterDesignEnd());
        exit();
      }
      fprintf(mufid , "//Ideal Filter Design:\n\n");
      fprintf(mufid,  "f_unit:=\"%s\":\n",f_unit);
      fprintf(mufid,  "filterType:= %d:\n",prjData.idealFilterType);

      int NZ=prjData.filterZeros.size();
      if(prjData.symmFilterResponse){
       fprintf(mufid , "nzeri_1_2:= %d:\n",2*NZ);
      } else {
       fprintf(mufid , "nzeri_1_2:= %d:\n",NZ);
      }
      fprintf(mufid , "zeri_1_2_f:= [ ");
      for(int i=0; i<NZ; i++){
	  fprintf(mufid , "%f",i,prjData.filterZeros[i]);
	  if(i<NZ-1) fprintf(mufid , ", ");
	  if(i && !(i%5) ) fprintf(mufid , "\n");
      }
      fprintf(mufid , " ]:\n");
      fprintf(mufid,  "Q:= %f:\n",prjData.filterQfactor);
      if(prjData.filterInductiveSkin){
	fprintf(mufid,  "inductiveSkin:= TRUE:\n");
      } else {
	fprintf(mufid,  "inductiveSkin:= FALSE:\n");
      }
      if(prjData.filterSymmetricTuning){
	  fprintf(mufid,  "SYMMETRIC_TUNING:= TRUE:\n");
      } else {
	  fprintf(mufid,  "SYMMETRIC_TUNING:= FALSE:\n");
      }
      if(prjData.predistortedFilter){
	  fprintf(mufid,  "predistortedFilter:= TRUE:\n");
      } else {
	  fprintf(mufid,  "predistortedFilter:= FALSE:\n");
      }
      if(prjData.predistFilterOptim){
	  fprintf(mufid,  "predistFilterOptim:= TRUE:\n");
	  fprintf(mufid,  "predistFilterOptimRL:= %f:\n",prjData.predistFilterOptimRL);
	  fprintf(mufid,  "predistFilterOptimILpp:= %f:\n",prjData.predistFilterOptimILpp);
	  fprintf(mufid,  "predistFilterOptimIterMax:= %d:\n",prjData.predistFilterOptimIterMax);
	  fprintf(mufid,  "predistFilterOptimTrustR:= %f:\n",prjData.predistFilterOptimTrustR);
      } else {
	  fprintf(mufid,  "predistFilterOptim:= FALSE:\n");
      }
      fclose(mufid);
    }

    QProcess *proc=new QProcess;
    proc->setWorkingDirectory(nativePath(workDir));


    QString nresonator,f_min_pass,f_max_pass,rloss,outrloss,portimp;
    nresonator.setNum(prjData.filterOrder);
    f_min_pass.setNum(prjData.filterPassBand[0],'f',10);
    f_max_pass.setNum(prjData.filterPassBand[1],'f',10);
    rloss.setNum(prjData.filterRetLoss,'f',5);
    outrloss.setNum(prjData.filterOutbandRetLoss,'f',5);
    portimp.setNum(prjData.filterPortImpedance,'f',5);

    QString app=script;
    QStringList args;

    args << QString("-idealFilterDesign");
    args << QString("-nresonator");
    args << nresonator;
    args << QString("-rloss");
    args << rloss;
    args << QString("-outbandRloss");
    args << outrloss;
    args << QString("-passband");
    args << f_min_pass;
    args << f_max_pass;
    args << QString("-portimpedance");
    args << portimp;
    if(prjData.idealFilterTopology==SYMMETRIC_ONLY_LC){
      args << QString("-onlyLC");
    } else if(prjData.idealFilterTopology==SYMMETRIC_WITH_MAGICT){
      args << QString("-withMagicT");
    } else if(prjData.idealFilterTopology==SYMMETRIC_TRANSVERSE_LC){
      args << QString("-transverseLC");
    } else if (prjData.idealFilterTopology==SYMMETRIC_TRANSVERSE_JC){
      args << QString("-transverseJC");
    }
    if(mappedTxZeros) {
      cktName=QString("ideal_filter_mappedTZ");
      args << QString("-mapTxZerosFromAna");
      QString dataFile=prjData.mainAssName+"_RM_mapped.inp";
      args << QString("-dataFile");
      args << dataFile;
    }

    if(useAPI){
       args << QString("-user");
       args << QString(emcadConfig[std::string("user")].c_str());
       args << QString("-password");
       args << QString(emcadConfig[std::string("password")].c_str());
       args << QString("-project");
       args << prjData.projectName;
       args << QString("-job");
       args << QString("symmfilter");
    } else if(useDOCKER){
       args << QString("-job");
       args << QString("symmfilter");
       if(useServer){
          args << QString("-project");
          args << prjData.projectName;
          if(emcadConfig.find(std::string("server"))!=emcadConfig.end()) if(emcadConfig[std::string("server")].length()>0) {
                 args << QString("-server");
		 args << QString(emcadConfig[std::string("server")].c_str());
	  }
       }
    }
    args << cktName;
    QString Cmd=app+QString("  ")+args.join(QString(" "));
    char *cmd=Cmd.toLatin1().data();
    if(useServer) server_secrets(1);
    proc->start(app, args);
    proc->waitForStarted();
    proc->waitForFinished(-1);
    msleep(100);
    if(useServer) server_secrets(0);
    QString mssg;
    if(proc->exitCode()==0){
       mssg=mappedTxZeros? "Completed Filter Design with Mapped Zeros" : "Completed Ideal Filter Design";
    }else{
       mssg=mappedTxZeros? "Failed Filter Design with Mapped Zeros" : "Failed Ideal Filter Design";
    }
    sendLogMessage(mssg);
    emit(filterDesignEnd());
    exit();
}


void Modeler::filterMap()
{
    QString script=QString(emcadPath);
    #ifdef WNT
       script.chop(13);
       QString ext=".exe";
    #else
       script.chop(9);
       QString ext=".py";
    #endif
    if(useAPI){
       script+="bin/api_submit";
       script=nativePath(script+ext);
    }else if(useDOCKER){
	 script="mwdocker_run";
    }else{
	 script="symmfilter";
    }
    QString workDir=mainWorkPath+"/Data/Circuits";

    QString inputData;
    if(prjData.filtermapSource==ZEROPOLES) inputData=prjData.mainAssName+"_RM_mapped.inp";
    if(prjData.filtermapSource==IMPORTED_RESPONSE) inputData="imported_response.s2p";
    QString inputDataPath=workDir+"/"+inputData;
    if(!FileExists(inputDataPath.toLatin1().data())){
	    emit(filterMapEnd());
            exit();
    }

    char f_unit[5];
    int fI=prjData.freqUnitE/3;
    switch(fI){
	 case 0: strcpy(f_unit,"Hz"); break;
	 case 1: strcpy(f_unit,"KHz"); break;
	 case 2: strcpy(f_unit,"MHz"); break;
	 case 3: strcpy(f_unit,"GHz"); break;
    }

    QString nresonator,ntxzeros,f_min_pass,f_max_pass,f_min_map,f_max_map,Qfactor,portimp;
    nresonator.setNum(prjData.filterOrder);
    if(prjData.symmFilterResponse){
      ntxzeros.setNum(2*prjData.filterZeros.size());
    } else {
      ntxzeros.setNum(prjData.filterZeros.size());
    }
    f_min_pass.setNum(prjData.filterPassBand[0],'f',10);
    f_max_pass.setNum(prjData.filterPassBand[1],'f',10);
    f_min_map.setNum(prjData.mapFreqBand[0],'f',10);
    f_max_map.setNum(prjData.mapFreqBand[1],'f',10);
    Qfactor.setNum(prjData.filtermapQfactor,'f',10);
    portimp.setNum(prjData.filterPortImpedance,'f',5);

    QProcess *proc=new QProcess;
    proc->setWorkingDirectory(nativePath(workDir));

    QStringList args;
    args << QString("-passband");
    args << f_min_pass;
    args << f_max_pass;
    args << QString("-portimpedance");
    args << portimp;
    if(prjData.filtermapSource==ZEROPOLES) args << QString("-mapFilterFromAna");
    if(prjData.filtermapSource==IMPORTED_RESPONSE){
	   args << QString("-mapFilterFromS2P");
           if(!prjData.filtermapSymmetric) args << QString("-asymmetric");
           if(prjData.filtermapQfactor>1.e-10){
		 args << QString("-Q");
		 args << Qfactor;
           }
           args << QString("-mapband");
           args << f_min_map;
           args << f_max_map;
           args << QString("-f_unit");
           args << QString(f_unit);
           args << QString("-nresonator");
           args << nresonator;
           args << QString("-ntxzeros");
           args << ntxzeros;
    }
    if(prjData.idealFilterTopology==SYMMETRIC_WITH_MAGICT)      args << QString("-withMagicT");
    else if(prjData.idealFilterTopology==SYMMETRIC_TRANSVERSE_LC)  args << QString("-transverseLC");
    else if (prjData.idealFilterTopology==SYMMETRIC_TRANSVERSE_JC){
	     args << QString("-transverseJC");
             if(prjData.filterTuneOnlyJt) args << QLatin1String("-tuneOnlyJt");
    }
    if(useAPI){
       args << QString("-user");
       args << QString(emcadConfig[std::string("user")].c_str());
       args << QString("-password");
       args << QString(emcadConfig[std::string("password")].c_str());
       args << QString("-project");
       args << prjData.projectName;
       args << QString("-job");
       args << QString("symmfilter");
    } else if(useDOCKER){
       args <<  QString("-job");
       args << QString("symmfilter");
       if(useServer){
           args << QString("-project");
           args << prjData.projectName;
           if(emcadConfig.find(std::string("server"))!=emcadConfig.end()) if(emcadConfig[std::string("server")].length()>0) {
                args << QString("-server");
		args << QString(emcadConfig[std::string("server")].c_str());
	   }
       }
    }
    if(prjData.filtermapSource==ZEROPOLES){
      QString cktName=prjData.mainAssName+"_RM_mapped";
      args << cktName;
    }
    if(prjData.filtermapSource==IMPORTED_RESPONSE){
      QString fileName="imported_response";
      args << fileName;
    }
    QString Cmd=script+QString("  ")+args.join(QString(" "));
    char *cmd=Cmd.toLatin1().data();
    if(useServer) server_secrets(1);
    proc->start(script, args);
    proc->waitForStarted();
    proc->waitForFinished(-1);
    msleep(100);
    if(useServer) server_secrets(0);
    QString mssg;
    if(proc->exitCode()==0){
       mssg="Completed Filter Mapping of \"" ; mssg+=inputData; mssg+="\"";
    }else if(proc->exitCode()==1){
       mssg="Failed Filter Mapping of \"" ; mssg+=inputData; mssg+="\"";
    }
    sendLogMessage(mssg);
    emit(filterMapEnd());
    exit();
}



void Modeler::analize_ports(std::string compNameStr)
{
     QString compName=compNameStr.c_str();
     QString script=QString(emcadPath);
     #ifdef WNT
        script.chop(13);
        QString ext=".exe";
     #else
        script.chop(9);
        QString ext=".py";
     #endif
     if(useAPI){
       script+="bin/api_submit";
       script=nativePath(script+ext);
     }else if(useDOCKER){
       script="mwdocker_run";
     }else{
       script="modelizep";
     }
     compName+="_WG";
     QString app=script;
     QStringList args;
     if(useAPI){
       args << QString("-user");
       args << QString(emcadConfig[std::string("user")].c_str());
       args << QString("-password");
       args << QString(emcadConfig[std::string("password")].c_str());
       args << QString("-project");
       args << prjData.projectName;
       args <<  QString("-job");
       args <<  QString("modelizep");
     } else if(useDOCKER){
       args <<  QString("-job");
       args <<  QString("modelizep");
     }
     args << compName;
     QString Cmd=app+QString("  ")+args.join(QString(" "));
     char * cmd=Cmd.toLatin1().data();
     QString mssg;
     QProcess *proc=new QProcess;
     QString workDir=mainWorkPath+"/Data/Circuits";
     proc->setWorkingDirectory(nativePath(workDir));
     proc->start(app, args);
     proc->waitForFinished(-1);
     msleep(100);
     if(proc->exitCode()==0){
        if(useAPI) {mssg="Completed Uploading of \"" ; mssg+=compName; mssg+="\"";}
	else       {mssg="Computed Port Modes of \"" ; mssg+=compName; mssg+="\"";}
        sendLogMessage(mssg);
     }else if(proc->exitCode()>0){
        modeledComponent[compNameStr]=1;
        failure=1;
        if(useAPI) {mssg="Failed Uploading of \"" ; mssg+=compName; mssg+="\"";}
	else       {mssg="Failed Port Modes of \"" ; mssg+=compName; mssg+="\"";}
        sendLogMessage(mssg);
     }
}

bool allTrue(std::map<std::string, int> &strmap)
{
typedef std::map<std::string, int>::const_iterator StrMapIt;
for ( StrMapIt it=strmap.begin(); it!= strmap.end(); it++) if(!(*it).second)  return false;
return true;
}	


void Modeler::run()
{
   QString mssg="Modeling";
   sendStatusMessage(mssg);
   typedef std::map<std::string, int>::const_iterator StrMapIt;
   if(task==CIRCUITS || task==UPDATE)    initModeledComponent();
   else if(task==PORTS)                  initModeledComponentWg();
   if(useAPI){
      if(task==CIRCUITS || task==UPDATE){
         for ( StrMapIt it=modeledComponent.begin(); it!= modeledComponent.end(); it++) modelize((*it).first);
         do { 
           msleep(1000); 
           for ( StrMapIt it=modeledComponent.begin(); it!= modeledComponent.end(); it++) if(!(*it).second) api_get_component_results(CIRCUITS, (*it).first);
         } while(!allTrue(modeledComponent));
      }else if(task==PORTS){
         for ( StrMapIt it=modeledWgComponent.begin(); it!= modeledWgComponent.end(); it++) analize_ports((*it).first);
         do { 
           msleep(1000); 
           for ( StrMapIt it=modeledWgComponent.begin(); it!= modeledWgComponent.end(); it++) if(!(*it).second) api_get_component_results(PORTS,((*it).first));
         } while(!allTrue(modeledWgComponent));
      }  
   } else if(useServer) {
      int nparjobs=NPARJOBSONSERVER;
      server_secrets(1);
      if(task==CIRCUITS || task==UPDATE){
          std::vector<std::string> modeledComponentVec;
          for ( StrMapIt it=modeledComponent.begin(); it!= modeledComponent.end(); it++){ modeledComponentVec.push_back((*it).first); }
          #pragma omp parallel for num_threads(nparjobs) shared(modeledComponentVec) 
          for (int i=0; i < modeledComponentVec.size(); ++i) modelize( modeledComponentVec[i]);
      }
      else if(task==PORTS){
          std::vector<std::string> modeledWgComponentVec;
          for ( StrMapIt it=modeledWgComponent.begin(); it!= modeledWgComponent.end(); it++){ modeledWgComponentVec.push_back((*it).first); }
          #pragma omp parallel for num_threads(nparjobs) shared(modeledWgComponentVec)
	  for (int i = 0; i < modeledWgComponentVec.size(); ++i) analize_ports(modeledWgComponentVec[i]);
      }
      server_secrets(0);
   } else {
      if(task==CIRCUITS || task==UPDATE)    for ( StrMapIt it=modeledComponent.begin(); it!= modeledComponent.end(); it++){  modelize((*it).first); if(failure) break; }
      else if(task==PORTS)                  for ( StrMapIt it=modeledWgComponent.begin(); it!=modeledWgComponent.end(); it++) analize_ports((*it).first);
   }
   if(failure){
       sendStatusMessage(QString("Ready"));
       emit(modelerEnd());
       return;
   }
   prjData.workStatus.modelizationNeeded=0;
   prjData.workStatus.checkReduction();
   if(task==FILTERDESIGN ){
        msleep(1000);
        filterDesign();
   }
   if(task==FILTERDESIGNMAPPEDTXZ ){
        msleep(1000);
        filterDesign(true);
   }
   if(task==FREQANA ){
        msleep(1000); 
        if(prjData.network || prjData.components.list.size()>1) if(prjData.workStatus.reductionNeeded && prjData.analysedCircuit==ELECROMAGNETICDEVICE)  mainReduce();
        freqAna();
   }
   if(task==ZEROPOLE ){
        msleep(1000); 
        if(prjData.network || prjData.components.list.size()>1) if(prjData.workStatus.reductionNeeded && prjData.analysedCircuit==ELECROMAGNETICDEVICE) mainReduce();
        zeropoleAna();
   }
   if(task==FILTERMAP ){
        msleep(1000);
        filterMap();
   }
   if(task== UPDATE && prjData.autoFreqResponse){
        msleep(1000);
        if(prjData.network || prjData.components.list.size()>1) if(prjData.workStatus.reductionNeeded) mainReduce();
        prjData.analysedCircuit=ELECROMAGNETICDEVICE;
        freqAna();
        if(failure){
          sendStatusMessage(QString("Ready"));
          emit(modelerEnd());
          return;
        }
   }
   if(task== UPDATE && prjData.autoZeropole){
        msleep(1000);
        if(prjData.network || prjData.components.list.size()>1) if(prjData.workStatus.reductionNeeded) mainReduce();
        prjData.analysedCircuit=ELECROMAGNETICDEVICE;
        zeropoleAna();
        if(failure){
          sendStatusMessage(QString("Ready"));
          emit(modelerEnd());
          return;
        }
   }
   if(task== UPDATE && prjData.autoFilterMapping ){
        msleep(1000);
        filterMap();
        if(failure){
          sendStatusMessage(QString("Ready"));
          emit(modelerEnd());
          return;
        }
   }
   if(task== UPDATE && prjData.autoMappedFreqResponse){
        msleep(1000);
        prjData.analysedCircuit=MAPPEDCIRCUIT;
        freqAna();
        if(failure){
          sendStatusMessage(QString("Ready"));
          emit(modelerEnd());
          return;
        }
   }
   if(task== UPDATE && prjData.autoIdealMappedTzFreqResponse){
        msleep(1000);
        prjData.analysedCircuit=IDEALCIRCUITMAPPEDTZ;
        freqAna();
        if(failure){
          sendStatusMessage(QString("Ready"));
          emit(modelerEnd());
          return;
        }
   }
   if(task== UPDATE && prjData.autoMappedZeropole){
        msleep(1000);
        prjData.analysedCircuit=MAPPEDCIRCUIT;
        zeropoleAna();
        if(failure){
          sendStatusMessage(QString("Ready"));
          emit(modelerEnd());
          return;
        }
   }
   sendStatusMessage(QString("Ready"));
   emit(modelerEnd());
}


void MainWindow::startTask(modelerTask task){
    modeler.task=task;
    runStatus.runningModeler=true;
    QString msg="Analysing"; 
    switch(task){
	 case PORTS: msg="WG Ports Analysis Running"; break;
	 case FILTERDESIGN: msg="Filter Design Running"; break;
	 case FILTERDESIGNMAPPEDTXZ: msg="Filter Design with Mapped TX Zeros Running"; break;
	 case FREQANA: msg="Freq Domain Ana Running";; break;
	 case ZEROPOLE: msg="Zero Pole Ana Running";; break;
	 case FILTERMAP: msg="Filter Map Running"; break;
	 case UPDATE: msg="Updating all results"; break;
        }
    statusMessage(msg);
    checkActions();
    modeler.receiver=this;
    modeler.start();
}


void MainWindow::updateModels()
{
    modeler.task=UPDATE;
    runStatus.runningModeler=true;
    QString mssg="Updating Electromagnetic Models\""; statusMessage(mssg);
    checkActions();
    modeler.receiver=this;
    modeler.start();
}

void MainWindow::modelize(){
    modeler.task=CIRCUITS;
    runStatus.runningModeler=true; checkActions();
    modeler.receiver=this;
    modeler.start();
}


void MainWindow::portModes(){
    modeler.task=PORTS;
    runStatus.runningModeler=true; checkActions();
    modeler.receiver=this;
    modeler.start();
}

void WorkStatus::checkReduction(){
    if(prjData.components.list.size()>1){
      QString circDir=mainWorkPath+"/Data/Circuits";
      QString main_JC_path=circDir+"/"+prjData.mainAssName+"_RM.JC";
      reductionNeeded=!FileExists(main_JC_path.toLatin1().data()) || prjData.workStatus.reductionNeeded;
      typedef std::set<std::string, std::less<std::string> >::const_iterator StrIt;
      if(!reductionNeeded) for ( StrIt it=prjData.components.list.begin(); it!= prjData.components.list.end(); it++){
        QString compName=it->c_str();
#if defined(GMSH3D)
        QString comp_JC_path=circDir+"/"+compName+"_RM.JC";
	if( file1NewerThanFile2(comp_JC_path.toLatin1().data(), main_JC_path.toLatin1().data()))  {reductionNeeded=1; break;}
#else
        QString comp_MJC_path=circDir+"/"+compName+".MJC";
	if( file1NewerThanFile2(comp_MJC_path.toLatin1().data(), main_JC_path.toLatin1().data()))  {reductionNeeded=1; break;}
#endif
      }
    }
}


void MainWindow::atModelerEnd(){
   runStatus.runningModeler=false; checkActions();
}

void MainWindow::atTunerEnd(){
   runStatus.runningTuner=false; checkActions();
}



void MainWindow::freqAna(){
  FreqAnaDialog *dialog=new FreqAnaDialog(this);
  connect(&modeler, SIGNAL(freqAnaEnd()), dialog, SLOT(atFreqAnaEnd()));
  dialog->show();
}

void MainWindow::zeropoleAna(){
  ZeroPoleDialog *dialog=new ZeroPoleDialog(this);
  connect(&modeler, SIGNAL(zeropoleEnd()), dialog, SLOT(atZeroPoleEnd()));
  dialog->show();
}


void MainWindow::filterMap(){
  FilterMapDialog *dialog=new FilterMapDialog(this);
  connect(&modeler, SIGNAL(filterMapEnd()), dialog, SLOT(atFilterMapEnd()));
  dialog->show();
}

void MainWindow::filterDesign(){
  FilterDesignDialog *dialog=new FilterDesignDialog(this);
  connect(&modeler, SIGNAL(filterDesignEnd()), dialog, SLOT(atFilterDesignEnd()));
  dialog->show();
}

void MainWindow::filterDesignWithTzMap(){
  FilterDesignWithTzMapDialog *dialog=new FilterDesignWithTzMapDialog(this);
  connect(&modeler, SIGNAL(filterDesignEnd()), dialog, SLOT(atFilterDesignWithTzMapEnd()));
  dialog->show();
}


FilterTuner::FilterTuner()
{
     failure=0;
}

FilterTunerDialog::FilterTunerDialog(FilterTuner *t, QWidget * p=0, Qt::WindowFlags f=0 ) : QDialog(p, f)
{
     tuner=t;
     parent=p;
     failure=0;
     setModal(0);

     setWindowTitle("Filter Tuning Wizard");
     
     QLabel *varFileLabel= new QLabel();
     varFileLabel->setText(tr("Variables File:"));
     varFileLineEdit = new QLineEdit();
     
     QPushButton *varFileButton  = new QPushButton(tr("Browse"));
     varFileButton->resize(1.5,3);
     connect(varFileButton, SIGNAL(clicked()), this, SLOT(varFileChooser()));

     QLabel *tuneMethodChooserLabel= new QLabel();
     tuneMethodChooserLabel->setText(tr("Solver Method:"));

     tuneMethodChooser = new QComboBox();
     tuneMethodChooser->addItem(tr("Broyden"));
//     tuneMethodChooser->addItem(tr("Modified Powell"));
//     connect(tuneMethodChooser, SIGNAL(currentIndexChanged(int)), this, SLOT(atMethodChanged(int)));

     QIntValidator *iterNumValidator = new QIntValidator(this);
     iterNumValidator->setBottom(1);

     QDoubleValidator *dvalidator = new QDoubleValidator(this);
     dvalidator->setDecimals(1000); // (standard anyway)
     dvalidator->setNotation(QDoubleValidator::ScientificNotation);

     QLabel *maxIterLabel= new QLabel();
     maxIterLabel->setText(tr("Max Iteration Num:"));

     maxIterLineEdit = new QLineEdit();
     maxIterLineEdit->setValidator(iterNumValidator);

     QLabel *xTolLabel= new QLabel();
     xTolLabel->setText(tr("X Tolerance:"));

     xTolLineEdit = new QLineEdit();
     xTolLineEdit->setValidator(dvalidator);
     xTolLineEdit->setText(QString("%1").arg(prjData.filterTuneXtol, 0, 'e', 1));

     QLabel *trustRadiusLabel= new QLabel();
     trustRadiusLabel->setText(tr("Trust Region Radius:"));

     trustRadiusLineEdit = new QLineEdit();
     trustRadiusLineEdit->setValidator(dvalidator);
     trustRadiusLineEdit->setText(QString("%1").arg(prjData.filterTuneTrustR, 0, 'e', 1));


     QLabel *gradDxLabel= new QLabel();
     gradDxLabel->setText(tr("Gradient dX:"));

     gradDxLineEdit = new QLineEdit();
     gradDxLineEdit->setValidator(dvalidator);
     gradDxLineEdit->setText(QString("%1").arg(prjData.filterTuneGradDx, 0, 'e', 1));



//   Automatic
     recomputeJaco=new QCheckBox("Recompute Jacobian", this);
     if(prjData.filterTuneRecomputeJaco)
         recomputeJaco->setCheckState(Qt::Checked);
     else                            
         recomputeJaco->setCheckState(Qt::Unchecked);

     recomputeError=new QCheckBox("Recompute Error", this);
     if(prjData.filterTuneRecomputeError)
         recomputeError->setCheckState(Qt::Checked);
     else                            
         recomputeError->setCheckState(Qt::Unchecked);



     QGridLayout *tuneSettingsLayout = new QGridLayout();

     tuneSettingsLayout->addWidget(varFileLabel, 0, 0);
     tuneSettingsLayout->addWidget(varFileLineEdit, 0, 1);
     tuneSettingsLayout->addWidget(varFileButton, 0, 2);
     tuneSettingsLayout->addWidget(tuneMethodChooserLabel, 1, 0);
     tuneSettingsLayout->addWidget(tuneMethodChooser, 1, 1);
     tuneSettingsLayout->addWidget(recomputeJaco, 2, 1);
     tuneSettingsLayout->addWidget(recomputeError, 2, 2);
     tuneSettingsLayout->addWidget(maxIterLabel, 3, 0);
     tuneSettingsLayout->addWidget(maxIterLineEdit, 3, 1);
     tuneSettingsLayout->addWidget(xTolLabel, 3, 2);
     tuneSettingsLayout->addWidget(xTolLineEdit, 3, 3);
     tuneSettingsLayout->addWidget(gradDxLabel, 4, 0);
     tuneSettingsLayout->addWidget(gradDxLineEdit, 4, 1);
     tuneSettingsLayout->addWidget(trustRadiusLabel, 4, 2);
     tuneSettingsLayout->addWidget(trustRadiusLineEdit, 4, 3);

     QGroupBox *tuneSettingsGroupBox=new QGroupBox();
     tuneSettingsGroupBox->setLayout(tuneSettingsLayout);



//****************************************
//   control buttons:
//
     setButton = new QPushButton(tr("Set"));
     setButton->resize(1.5,3);
     startButton =new QPushButton(tr("Start"));
     startButton->resize(1.5,3);
     closeButton =new QPushButton(tr("Close"));
     closeButton->resize(1.5,3);
     QPushButton *helpButton  = new QPushButton(tr("Help"));
     helpButton->resize(1.5,3);

     QHBoxLayout *buttonLayout = new QHBoxLayout();
     buttonLayout->addWidget(setButton);
     buttonLayout->addWidget(startButton);
     buttonLayout->addWidget(closeButton);
     buttonLayout->addWidget(helpButton);

     QGroupBox *buttonGroupBox=new QGroupBox(tr(""));
     buttonGroupBox->setLayout(buttonLayout);

//******
     connect(setButton, SIGNAL(clicked()), this, SLOT(set()));
     connect(parent, SIGNAL(enterPressed()), this, SLOT(set()));
     connect(startButton,  SIGNAL(clicked()), this, SLOT(start()));
     connect(closeButton,  SIGNAL(clicked()), this, SLOT(accept()));
     connect(helpButton,  SIGNAL(clicked()), this, SLOT(help()));
   

     setFocusPolicy(Qt::StrongFocus);

     mainLayout = new QVBoxLayout(this);
     mainLayout->addWidget(tuneSettingsGroupBox);
     mainLayout->addWidget(buttonGroupBox);

     setFocusPolicy(Qt::StrongFocus);
}

void FilterTunerDialog::atMethodChanged(int i){
}

//*************
void FilterTunerDialog::init()
{
     failure=0;
     varFileLineEdit->setText(prjData.varFilePath);
     maxIterLineEdit->setText(QString("%1").arg(prjData.filterTuneItermax));
     int err=tuner->check();
     startButton->setEnabled(!err);
}
void FilterTunerDialog::varFileChooser()
{
	QString	path;
        QStringList filters;
	QFileDialog dialog(this,tr("Open"));
	dialog.setNameFilter("*.txt");
	dialog.setDirectory(QDir::currentPath());
	dialog.setFileMode(QFileDialog::ExistingFile);
	if(dialog.exec()){
           QStringList pathList;
	   pathList = dialog.selectedFiles();
	   path= pathList.at(0);
	}
	if(path.isEmpty()) return;
	varFileLineEdit->setText(path); 
	QFileInfo fileinfo(path);
        QDir::setCurrent(fileinfo.dir().absolutePath());

}


void FilterTunerDialog::set(){
    bool changed;
    if(prjData.varFilePath!=varFileLineEdit->text()) {
	changed=true;
	prjData.varFilePath=varFileLineEdit->text();
    }
    if(prjData.filterTuneMethod!=tuneMethodChooser->currentIndex()) {
	changed=true;
	prjData.filterTuneMethod = tuneMethodChooser->currentIndex();
    }
    int recompJaco=(recomputeJaco->checkState()==Qt::Checked)? 1 :0;
    if(prjData.filterTuneRecomputeJaco!=recompJaco) {changed=true; prjData.filterTuneRecomputeJaco=recompJaco;}
    int recompError=(recomputeError->checkState()==Qt::Checked)? 1 :0;
    if(prjData.filterTuneRecomputeError!=recompError) {changed=true; prjData.filterTuneRecomputeError=recompError;}
    int maxit=maxIterLineEdit->text().toInt();
    if(prjData.filterTuneItermax!=maxit) { changed=true; prjData.filterTuneItermax=maxit;}

    double tmp=xTolLineEdit->text().toDouble();
    if(fabs(prjData.filterTuneXtol-tmp)>1.e-8) { changed=true; prjData.filterTuneXtol=tmp;}

    tmp=trustRadiusLineEdit->text().toDouble();
    if(fabs(prjData.filterTuneTrustR-tmp)>1.e-8) { changed=true; prjData.filterTuneTrustR=tmp;}

    tmp=gradDxLineEdit->text().toDouble();
    if(fabs(prjData.filterTuneGradDx-tmp)>1.e-8) { changed=true; prjData.filterTuneGradDx=tmp;}

    if(changed) prjData.saveSettings();
    int err=tuner->check();
    startButton->setEnabled(!err);
}

void FilterTunerDialog::start(){
 set();
 setButton->setEnabled(false);
 startButton->setEnabled(false);
 closeButton->setEnabled(false);
 tuner->receiver=parent;
 tuner->start();
 close();
}

void FilterTunerDialog::atFilterTuneEnd(){
 setButton->setEnabled(true);
 startButton->setEnabled(true);
 closeButton->setEnabled(true);
}

void FilterTunerDialog::help()
{
    documentation.showDocumentation(QLatin1String("#page=49"));
}


void MainWindow::filterTune(){	
  FilterTunerDialog *tunerDialog=new FilterTunerDialog(&tuner, this);
  tunerDialog->init();
  tunerDialog->show();
}

void FilterTuner::run(){
    QString script=QString(emcadPath);
    #ifdef WNT
       script.chop(13);
       QString ext=".exe";
    #else
       script.chop(9);
       QString ext=".py";
    #endif
    script+="bin/filter_tune";
    script=nativePath(script+ext);

    QString ideal_cktName="ideal_filter";
    QString mapped_cktName=prjData.mainAssName;
    if(prjData.filtermapSource==ZEROPOLES) mapped_cktName=prjData.mainAssName+"_RM_mapped";
    if(prjData.filtermapSource==IMPORTED_RESPONSE) mapped_cktName="imported_response_mapped";
    if(prjData.filtermapSource==IMPORTED_CIRCUIT) mapped_cktName="imported_circuit";


    QString workDir=mainWorkPath+"/Data/Circuits";
    QProcess *proc=new QProcess;
    proc->setWorkingDirectory(nativePath(workDir));

    QString logFilePath=nativePath(workDir+QString("/")+QString("filter_tune.log"));
    proc->setStandardErrorFile(logFilePath);

    QStringList args;
    args << QLatin1String("-varfile");
    args << prjData.varFilePath;
    QString tunerMethod;
    tunerMethod.setNum(prjData.filterTuneMethod);
    args << QLatin1String("-method");
    args << tunerMethod;
    QString filterTuneItermax; 
    filterTuneItermax.setNum(prjData.filterTuneItermax);
    args << QLatin1String("-maxiter");
    args << filterTuneItermax;
    QString xtol,trustd,graddx;
    xtol.setNum(prjData.filterTuneXtol,'e',3);
    trustd.setNum(prjData.filterTuneTrustR,'e',3);
    graddx.setNum(prjData.filterTuneGradDx,'e',3);
    args << QLatin1String("-xtol");
    args << xtol;
    args << QLatin1String("-trustd");
    args << trustd;
    args << QLatin1String("-graddx");
    args << graddx;
    if(prjData.filterTuneRecomputeJaco) args << QLatin1String("-recomputejaco");
    if(prjData.filterTuneRecomputeError) args << QLatin1String("-recomputeerror");
    args << QLatin1String("-ideal_cktname");
    args << ideal_cktName;
    args << QLatin1String("-mapped_cktname");
    args << mapped_cktName;
    QString Cmd=script+QString("  ")+args.join(QString(" "));
    char *cmd=Cmd.toLatin1().data();
/*
*/
    proc->start(script, args);
    runStatus.runningTuner=true;
    proc->waitForStarted();
    proc->waitForFinished(-1);
    msleep(100);
    QString mssg;
    if(proc->exitCode()==0){
       mssg="Completed Filter Tuning";
    }else if(proc->exitCode()==1){
       mssg="Failed Filter Tuning" ;
    }
    emit(tunerEnd());
}
    

int FilterTuner::check()
{
    QString script=QString(emcadPath);
    #ifdef WNT
       script.chop(13);
       QString ext=".exe";
    #else
       script.chop(9);
       QString ext=".py";
    #endif
    script+="bin/filter_tune";
    script=nativePath(script+ext);

    QString ideal_cktName="ideal_filter";
    QString mapped_cktName=prjData.mainAssName;
    if(prjData.filtermapSource==ZEROPOLES) mapped_cktName=prjData.mainAssName+"_RM_mapped";
    if(prjData.filtermapSource==IMPORTED_RESPONSE) mapped_cktName="imported_response_mapped";
    if(prjData.filtermapSource==IMPORTED_CIRCUIT)  mapped_cktName="imported_circuit";

    QString workDir=mainWorkPath+"/Data/Circuits";

    if(!FileExists(prjData.varFilePath.toLatin1().data())) return -2;
    QString idealParPath=nativePath(workDir+QString("/")+ideal_cktName+QString("_xpar.txt"));
    if(!FileExists(idealParPath.toLatin1().data())) return -1;

    QProcess *proc=new QProcess;
    proc->setWorkingDirectory(nativePath(workDir));

    QString logFilePath=nativePath(workDir+QString("/")+QString("filter_tune.log"));
    proc->setStandardErrorFile(logFilePath);

    QStringList args;
    args << QLatin1String("-varfile");
    args << prjData.varFilePath;
    args << QLatin1String("-check");

    args << QLatin1String("-ideal_cktname");
    args << ideal_cktName;
    args << QLatin1String("-mapped_cktname");
    args << mapped_cktName;

    QString Cmd=QString("cd  ")+workDir+QString("; ");
    Cmd=Cmd+script+QString("  ")+args.join(QString(" "));
    char *cmd=Cmd.toLatin1().data();
    proc->start(script, args);
    proc->waitForStarted();
    proc->waitForFinished(-1);
    msleep(100);
    int ecode=proc->exitCode();
    if(ecode==-1){
       QString mssg="Ideal Filter not found. Please run \"Ideal Filter\" from \"Design\" menu";
       sendLogMessage(mssg);
    }else if(ecode==1){
       QString mssg="Number of variables is lower than Number of tunable Circuit Parameters";
       sendLogMessage(mssg);
    }
    return ecode;
}

void MainWindow::atDecomposerEnd()
{
  runStatus.runningDecomposer=false; checkActions();
  if(mainOCAF->EmP.hasGeo) updateAllWidgets();
}

void MainWindow::atMesherEnd() {
  runStatus.runningMesher=false; checkActions();
}



void MainWindow::setGlobals()
{
  SetGlobalsDialog *dialog=new SetGlobalsDialog(this);
  dialog->setWindowTitle("Project Options");
  dialog->show();
}

void MainWindow::setConfig()
{
  ConfigDialog *dialog=new ConfigDialog(this);
  dialog->show();
}




void MainWindow::print()
{
}



void MainWindow::about()
{
    QMessageBox::about(this, tr("About Menu"),
            tr("The <b>Electromagnetic CAD</b> is an user interface "
                "for the definition of an electromagnetic problem in a file format "
		"compatible with the Hierachical Electromagnetic Modeler.\n"
		"The geometrical structure should have been previously defined "
	        "by means of a CAD tool and imported in this framework as a STEP file.\n"
		"Here the user should specify all the data (material properties, "
		"excitation ports, frequency bands..) which are required for "
		"the proper definitopn of the electromagnetic problem."
                ));
}

void MainWindow::aboutQt()
{
}


void MainWindow::xyzPosition (V3d_Coordinate X,   V3d_Coordinate Y,  V3d_Coordinate Z)
{
	QString aString;
	QTextStream ts(&aString);
	ts << X << "," << Y << "," << Z;
//	statusBar()->showMessage(aString);
}


void MainWindow::temporaryMessage (const QString aMessage)
{
	statusBar()->showMessage(aMessage);
}

void MainWindow::statusMessage (const QString aMessage)
{
	statusLabel->setText(aMessage);
}

/*
void MainWindow::errorMessage (const QString aMessage)
{
    statusBar()->showMessage(aMessage);
    QString fmt_message="<font color='red'>";
    fmt_message.append(aMessage);
    error->setText(fmt_message);
}
*/



void Documentation::showDocumentation(const QString sectionID)
{

    QString manualPath=emcadPath;
    #ifdef WNT
       manualPath.chop(13);
    #else
       manualPath.chop(9);
    #endif
    manualPath+="doc/manual";
    manualPath+=".pdf";
    manualPath+=sectionID;

    QString manualLink="file:///"+manualPath;
    QDesktopServices::openUrl(QUrl(manualLink, QUrl::TolerantMode));

}
void Documentation::showDocumentation()
{
  showDocumentation(QLatin1String(""));
}

void MainWindow::meshView()
{
/*
    bool running=(gmshview)? running=gmshview->state()!=QProcess::NotRunning : false;
    if(running) {
        gmshview->terminate();
        gmshview->waitForFinished(3000);
    }
*/


    QProcess *gmshview = new QProcess;
    QString gmshapp=QLatin1String("gmsh");
    QStringList gmshargs;
    QString gmshOptionFile=nativePath(mainWorkPath+"/gmsh.opt");
    gmshargs << QString("-option")
             << gmshOptionFile;

    QString circDir=mainWorkPath+"/Data/Circuits/";
    TCollection_AsciiString assName; mainOCAF->getAssName(assName);
    QString meshFile=circDir+QString(assName.ToCString())+".msh";
    meshFile=nativePath(meshFile);

    gmshargs << meshFile;

    QString Cmd=gmshapp+QString("  ")+gmshargs.join(QString(" "));
    char * cmd=Cmd.toLatin1().data();

    gmshview->start(gmshapp, gmshargs);
    if (!gmshview->waitForStarted()) return;
}


void MainWindow::responsePlot()
{
  freqRespPlot->plottedCircuit=ELECROMAGNETICDEVICE;
  freqRespPlot->loadFile();
  freqRespPlot->plotFreqResponse();
  freqRespPlot->window()->resize(800,500);
  freqRespPlot->show();
}

void MainWindow::ideal_responsePlot()
{
  ideal_freqRespPlot->plottedCircuit=IDEALCIRCUIT;
  ideal_freqRespPlot->loadFile();
  ideal_freqRespPlot->plotFreqResponse();
  ideal_freqRespPlot->window()->resize(800,500);
  ideal_freqRespPlot->show();
}

void MainWindow::imported_responsePlot()
{
  imported_freqRespPlot->plottedCircuit=IMPORTEDRESPONSE;
  imported_freqRespPlot->loadFile();
  imported_freqRespPlot->plotFreqResponse();
  imported_freqRespPlot->window()->resize(800,500);
  imported_freqRespPlot->show();
}


void MainWindow::ideal_mappedTZ_responsePlot()
{
  ideal_mappedTZ_freqRespPlot->plottedCircuit=IDEALCIRCUITMAPPEDTZ;
  ideal_mappedTZ_freqRespPlot->loadFile();
  ideal_mappedTZ_freqRespPlot->plotFreqResponse();
  ideal_mappedTZ_freqRespPlot->window()->resize(800,500);
  ideal_mappedTZ_freqRespPlot->show();
}


void MainWindow::mapped_responsePlot()
{
  mapped_freqRespPlot->plottedCircuit=MAPPEDCIRCUIT;
  mapped_freqRespPlot->loadFile();
  mapped_freqRespPlot->plotFreqResponse();
  mapped_freqRespPlot->window()->resize(800,500);
  mapped_freqRespPlot->show();
}

void MainWindow::zeropolePlot()
{
  zeroPolePlot->plottedCircuit=ELECROMAGNETICDEVICE;
  zeroPolePlot->loadFile();
  zeroPolePlot->plotZeroPole();
  zeroPolePlot->window()->resize(800,500);
  zeroPolePlot->show();
}

void MainWindow::ideal_zeropolePlot()
{
  ideal_zeroPolePlot->plottedCircuit=IDEALCIRCUIT;
  ideal_zeroPolePlot->loadFile();
  ideal_zeroPolePlot->plotZeroPole();
  ideal_zeroPolePlot->window()->resize(800,500);
  ideal_zeroPolePlot->show();
}

void MainWindow::mapped_zeropolePlot()
{
  mapped_zeroPolePlot->plottedCircuit=MAPPEDCIRCUIT;
  mapped_zeroPolePlot->loadFile();
  mapped_zeroPolePlot->plotZeroPole();
  mapped_zeroPolePlot->window()->resize(800,500);
  mapped_zeroPolePlot->show();
}



void MainWindow::accountStatus()
{
  AccountStatusDialog *dialog=new AccountStatusDialog(this);
  dialog->show();
}


void MainWindow::createActions()
{

    openAction = new QAction(tr("&Open"), this);
//    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setStatusTip(tr("Open an existing project"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAsAction = new QAction(tr("&save As"), this);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    saveAsAction->setEnabled(false);

    closeAction = new QAction(tr("&Close"), this);
    closeAction->setEnabled(false);
    connect(closeAction, SIGNAL(triggered()), this, SLOT(closeAll()));

    closeCompAction = new QAction(tr("&Close Component"), this);
    closeCompAction->setShortcut(tr("Ctrl+U"));
    closeCompAction->setStatusTip(tr("Close Component"));
    connect(closeCompAction, SIGNAL(triggered()), this, SLOT(closeComp()));
    closeCompAction->setEnabled(false);

    openCompAction = new QAction(tr("&Open Component"), this);
    openCompAction->setShortcut(tr("Ctrl+O"));
    connect(openCompAction, SIGNAL(triggered()), this, SLOT(openComp()));
    openCompAction->setEnabled(false);

    openCompAction_ = new QAction(tr("&Open Original SubAssembly"), this);
    openCompAction_->setShortcut(tr("Ctrl+Shift+O"));
    connect(openCompAction_, SIGNAL(triggered()), this, SLOT(openComp_()));
    openCompAction_->setEnabled(false);

    importGeometryAction = new QAction(tr("&Import Geometry"), this);
    importGeometryAction->setShortcut(tr("Ctrl+I"));
    importGeometryAction->setEnabled(false);
//    importGeometryAction->setEnabled(false);
    connect(importGeometryAction, SIGNAL(triggered()), this, SLOT(importGeometry()));

    importMaterialAction = new QAction(tr("&Import Materials"), this);
    importMaterialAction->setShortcut(tr("Ctrl+I"));
    importMaterialAction->setEnabled(false);
//    importMaterialAction->setEnabled(false);
    connect(importMaterialAction, SIGNAL(triggered()), this, SLOT(importMaterial()));

    importIdealFilterCircuitAction = new QAction(tr("&Import Ideal Filter Circuit"), this);
    importIdealFilterCircuitAction->setShortcut(tr("Ctrl+I"));
    importIdealFilterCircuitAction->setEnabled(false);
    connect(importIdealFilterCircuitAction, SIGNAL(triggered()), this, SLOT(importIdealFilterCircuit()));

    importIdealFilterParAction = new QAction(tr("&Import Ideal Filter Par"), this);
    importIdealFilterParAction->setShortcut(tr("Ctrl+I"));
    importIdealFilterParAction->setEnabled(true);
    connect(importIdealFilterParAction, SIGNAL(triggered()), this, SLOT(importIdealFilterPar()));

    importFilterCircuitAction = new QAction(tr("&Import Circuit"), this);
    importFilterCircuitAction->setShortcut(tr("Ctrl+I"));
    importFilterCircuitAction->setEnabled(true);
    connect(importFilterCircuitAction, SIGNAL(triggered()), this, SLOT(importFilterCircuit()));

    importFilterParAction = new QAction(tr("&Import Mapped Filter Par"), this);
    importFilterParAction->setShortcut(tr("Ctrl+I"));
    importFilterParAction->setEnabled(true);
    connect(importFilterParAction, SIGNAL(triggered()), this, SLOT(importFilterPar()));



    importS2P_Action = new QAction(tr("&Import S2P touchstone file"), this);
    importS2P_Action->setEnabled(false);
    connect(importS2P_Action, SIGNAL(triggered()), this, SLOT(importS2P()));


    exportMaterialAction = new QAction(tr("&Export Materials"), this);
    exportMaterialAction->setEnabled(false);
//    exportMaterialAction->setEnabled(false);
    connect(exportMaterialAction, SIGNAL(triggered()), this, SLOT(exportMaterial()));

    exportSpiceAction = new QAction(tr("&Spice Circuit"), this);
    exportSpiceAction->setEnabled(false);
    connect(exportSpiceAction, SIGNAL(triggered()), this, SLOT(exportSpice()));

    exportMappedJCAction = new QAction(tr("&Mapped JC Circuit"), this);
    exportMappedJCAction->setEnabled(false);
    connect(exportMappedJCAction, SIGNAL(triggered()), this, SLOT(exportMappedJC()));

    exportMappedSpiceAction = new QAction(tr("&Mapped Spice Circuit"), this);
    exportMappedSpiceAction->setEnabled(false);
    connect(exportMappedSpiceAction, SIGNAL(triggered()), this, SLOT(exportMappedSpice()));

    exportIdealJCAction = new QAction(tr("&Ideal JC Circuit"), this);
    exportIdealJCAction->setEnabled(false);
    connect(exportIdealJCAction, SIGNAL(triggered()), this, SLOT(exportIdealJC()));

    exportFreqResponseAction = new QAction(tr("&Frequency Response"), this);
    exportFreqResponseAction->setEnabled(false);
    connect(exportFreqResponseAction, SIGNAL(triggered()), this, SLOT(exportFreqResponse()));

    exportIdealResponseAction = new QAction(tr("&Ideal Filter Response"), this);
    exportIdealResponseAction->setEnabled(false);
    connect(exportIdealResponseAction, SIGNAL(triggered()), this, SLOT(exportIdealResponse()));

    exportMappedResponseAction = new QAction(tr("&Mapped Circuit Response"), this);
    exportMappedResponseAction->setEnabled(false);
    connect(exportMappedResponseAction, SIGNAL(triggered()), this, SLOT(exportMappedResponse()));

    exportIdealSpiceAction = new QAction(tr("&Ideal Spice Circuit"), this);
    exportIdealSpiceAction->setEnabled(false);
    connect(exportIdealSpiceAction, SIGNAL(triggered()), this, SLOT(exportIdealSpice()));

    exportJC_IdealMappedTZ_Action = new QAction(tr("&Ideal JC Circuit with mapped TZ"), this);
    exportJC_IdealMappedTZ_Action->setEnabled(false);
    connect(exportJC_IdealMappedTZ_Action, SIGNAL(triggered()), this, SLOT(exportIdealMappedTzJC()));

    exportSpice_IdealMappedTZ_Action = new QAction(tr("&Ideal Spice Circuit with mapped TZ"), this);
    exportSpice_IdealMappedTZ_Action->setEnabled(false);
    connect(exportSpice_IdealMappedTZ_Action, SIGNAL(triggered()), this, SLOT(exportIdealMappedTzSpice()));


    exportFilterTuneParAction = new QAction(tr("&Filter Tuning Parameters"), this);
    exportFilterTuneParAction->setEnabled(false);
    connect(exportFilterTuneParAction, SIGNAL(triggered()), this, SLOT(exportFilterTunePar()));


    importADSprjAction = new QAction(tr("&Import ADS project"), this);
    connect(importADSprjAction, SIGNAL(triggered()), this, SLOT(importADSprj()));

/*
    importDataDirAction = new QAction(tr("&Import Data Dir"), this);
    importDataDirAction->setEnabled(false);
    connect(importDataDirAction, SIGNAL(triggered()), this, SLOT(importDataDir()));
*/

    newProjectAction = new QAction(tr("New"), this);
    newProjectAction->setEnabled(true);
    connect(newProjectAction, SIGNAL(triggered()), this, SLOT(newProject()));

    defaultBC_Action = new QAction(tr("Default Boundary Condition"), this);
    defaultBC_Action->setEnabled(true);
    connect(defaultBC_Action, SIGNAL(triggered()), treeWidget, SLOT(setDefaultBCdialog()));

    saveAction = new QAction(tr("Save"), this);
    saveAction->setEnabled(false);
    saveAction->setShortcut(tr("Ctrl+S"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    printAction = new QAction(tr("&Print..."), this);
    printAction->setShortcut(tr("Ctrl+P"));
//    printAction->setStatusTip(tr("Print the document"));
    connect(printAction, SIGNAL(triggered()), this, SLOT(print()));

    exitAction = new QAction(tr("&Exit"), this);
//    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));


    decomposeAction = new QAction(tr("&Decompose"), this);
    decomposeAction->setShortcut(tr("Ctrl+X"));
    decomposeAction->setStatusTip(tr("Performs Domain Decomposition and Imprinting of adjacent faces/edges"));
    decomposeAction->setEnabled(false);
    connect(decomposeAction, SIGNAL(triggered()), this, SLOT(decompose()));

    meshAction = new QAction(tr("&Mesh"), this);
    meshAction->setShortcut(tr("Ctrl+M"));
    meshAction->setStatusTip(tr("Mesh all the subdomains interfaces"));
    meshAction->setEnabled(false);
    connect(meshAction, SIGNAL(triggered()), this, SLOT(mesh()));

    reloadAction = new QAction(tr("&Reload CAD"), this);
    reloadAction->setShortcut(tr("Ctrl+R"));
    reloadAction->setStatusTip(tr("Reload CAD file and remesh"));
    reloadAction->setEnabled(false);
    connect(reloadAction, SIGNAL(triggered()), this, SLOT(reload()));

    modelizeAction = new QAction(tr("&Modelize"), this);
    modelizeAction->setShortcut(tr("Ctrl+E"));
    modelizeAction->setStatusTip(tr("Generate Electromagnetic Models"));
    modelizeAction->setEnabled(true);
    connect(modelizeAction, SIGNAL(triggered()), this, SLOT(modelize()));


    portModesAction = new QAction(tr("&Port Modes"), this);
    portModesAction->setStatusTip(tr("Compute Port Modes"));
    portModesAction->setEnabled(true);
    connect(portModesAction, SIGNAL(triggered()), this, SLOT(portModes()));

    freqAnaAction = new QAction(tr("&Freq Domain Analysis"), this);
    freqAnaAction->setStatusTip(tr("Analyze circuit response in frequency domain"));
    freqAnaAction->setEnabled(true);
    connect(freqAnaAction, SIGNAL(triggered()), this, SLOT(freqAna()));

    zeropoleAnaAction = new QAction(tr("&S Param Zeros/Poles"), this);
    zeropoleAnaAction->setStatusTip(tr("Compute zeros and poles of S parameter Freq Response over Laplace plane"));
    zeropoleAnaAction->setEnabled(true);
    connect(zeropoleAnaAction, SIGNAL(triggered()), this, SLOT(zeropoleAna()));

    filterMapAction = new QAction(tr("&Filter Mapping"), this);
    filterMapAction->setStatusTip(tr("Synthesize a resonator circuit matching computed position of zeros and poles"));
    filterMapAction->setEnabled(false);
    connect(filterMapAction, SIGNAL(triggered()), this, SLOT(filterMap()));

    filterTuneAction = new QAction(tr("&Filter Tuning"), this);
    filterTuneAction->setStatusTip(tr("Tunes the geometrical variables of the EM device in order to match the ideal circuit"));
    filterTuneAction->setEnabled(false);
    connect(filterTuneAction, SIGNAL(triggered()), this, SLOT(filterTune()));


    filterDesignAction = new QAction(tr("&Ideal Filter"), this);
    filterDesignAction->setStatusTip(tr("Design an ideal filter circuit with a specified frequency response"));
    filterDesignAction->setEnabled(false);
    connect(filterDesignAction, SIGNAL(triggered()), this, SLOT(filterDesign()));

    filterDesignWithTzMapAction = new QAction(tr("&Ideal Filter with mapped Tx zeros"), this);
    filterDesignWithTzMapAction->setStatusTip(tr("Design an ideal filter circuit with with mapped Tx zeros"));
    filterDesignWithTzMapAction->setEnabled(false);
    connect(filterDesignWithTzMapAction, SIGNAL(triggered()), this, SLOT(filterDesignWithTzMap()));

    updateAction = new QAction(tr("&Update"), this);
    updateAction->setStatusTip(tr("Update electromagnetic models and analyses"));
    updateAction->setEnabled(true);
    connect(updateAction, SIGNAL(triggered()), this, SLOT(updateModels()));

    meshViewAction = new QAction(tr("&Mesh View"), this);
    meshViewAction = new QAction(tr("&Mesh View"), this);
    meshViewAction->setStatusTip(tr("View the model Mesh"));
    meshViewAction->setEnabled(false);
    connect(meshViewAction, SIGNAL(triggered()), this, SLOT(meshView()));

    responsePlotAction = new QAction(tr("&Electromanetic Model"), this);
    responsePlotAction->setStatusTip(tr("Plots the frequency response of the electromagnetic device"));
    responsePlotAction->setEnabled(false);
    connect(responsePlotAction, SIGNAL(triggered()), this, SLOT(responsePlot()));

    mapped_responsePlotAction = new QAction(tr("&Mapped Filter"), this);
    mapped_responsePlotAction->setStatusTip(tr("Plots the frequency response of the mapped filter circuit"));
    mapped_responsePlotAction->setEnabled(false);
    connect(mapped_responsePlotAction, SIGNAL(triggered()), this, SLOT(mapped_responsePlot()));

    ideal_responsePlotAction = new QAction(tr("&Ideal Filter"), this);
    ideal_responsePlotAction->setStatusTip(tr("Plots the frequency response of the ideal filter"));
    ideal_responsePlotAction->setEnabled(false);
    connect(ideal_responsePlotAction, SIGNAL(triggered()), this, SLOT(ideal_responsePlot()));

    imported_responsePlotAction = new QAction(tr("&Imported Response"), this);
    imported_responsePlotAction->setStatusTip(tr("Plots the imported frequency response"));
    imported_responsePlotAction->setEnabled(false);
    connect(imported_responsePlotAction, SIGNAL(triggered()), this, SLOT(imported_responsePlot()));

    ideal_mappedTZ_responsePlotAction = new QAction(tr("&Ideal Filter with mapped Tx Zeros"), this);
    ideal_mappedTZ_responsePlotAction->setStatusTip(tr("Plots the frequency response of the ideal filter with mapped Tx Zeros"));
    ideal_mappedTZ_responsePlotAction->setEnabled(false);
    connect(ideal_mappedTZ_responsePlotAction, SIGNAL(triggered()), this, SLOT(ideal_mappedTZ_responsePlot()));

    zeropolePlotAction = new QAction(tr("&Electromanetic Model"), this);
    zeropolePlotAction->setStatusTip(tr("Plots the Zeros/Poles pattern of S parameters of the electromagnetic device"));
    zeropolePlotAction->setEnabled(false);
    connect(zeropolePlotAction, SIGNAL(triggered()), this, SLOT(zeropolePlot()));

    mapped_zeropolePlotAction = new QAction(tr("&Mapped Filter"), this);
    mapped_zeropolePlotAction->setStatusTip(tr("Plot the Zeros/Poles pattern of S parameters of the mapped filter circuit"));
    mapped_zeropolePlotAction->setEnabled(false);
    connect(mapped_zeropolePlotAction, SIGNAL(triggered()), this, SLOT(mapped_zeropolePlot()));

    ideal_zeropolePlotAction = new QAction(tr("&Ideal Filter"), this);
    ideal_zeropolePlotAction->setStatusTip(tr("Plot the Zeros/Poles pattern of S parameters of the ideal filter"));
    ideal_zeropolePlotAction->setEnabled(false);
    connect(ideal_zeropolePlotAction, SIGNAL(triggered()), this, SLOT(ideal_zeropolePlot()));

    accountStatusAction = new QAction(tr("&Account Status"), this);
    accountStatusAction->setStatusTip(tr("View Account Status"));
    accountStatusAction->setEnabled(true);
    connect(accountStatusAction, SIGNAL(triggered()), this, SLOT(accountStatus()));


    setCompPropertiesAction = new QAction(tr("&Set Part Properties"), this);
    connect(setCompPropertiesAction, SIGNAL(triggered()), treeWidget, SLOT(setCompPropertiesDialog()));

    defineMaterialAction = new QAction(tr("&Create/Modify"), this);
    connect(defineMaterialAction, SIGNAL(triggered()), treeWidget, SLOT(defineMaterialDialog()));

    assignMaterialAction = new QAction(tr("&Assign Material"), this);
    assignMaterialAction->setStatusTip(tr("Assign a Material to the Selected Item "));
    connect(assignMaterialAction, SIGNAL(triggered()), treeWidget, SLOT(assignMaterialDialog()));

    assignLayerAction = new QAction(tr("&Assign Layer"), this);
    assignLayerAction->setStatusTip(tr("Assign a Layer to the Selected Solid "));
    connect(assignLayerAction, SIGNAL(triggered()), treeWidget, SLOT(assignLayerDialog()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(aboutQtAction, SIGNAL(triggered()), this, SLOT(aboutQt()));

    helpAction = new QAction(tr("Help Contents"), this);
    helpAction->setShortcut(tr("F1"));
    connect(helpAction, SIGNAL(triggered()), &documentation, SLOT(showDocumentation()));

	// Now for the ViewWidget slots.

    fitAction = new QAction(tr("&Fit Window"), this);
    fitAction->setShortcut(tr("Ctrl+F"));
    fitAction->setStatusTip(tr("Fit to window"));
    connect(fitAction, SIGNAL(triggered()), mainOCC, SLOT(fit()));

    fitAllAction = new QAction(tr("Fit &All"), this);
    fitAllAction->setShortcut(tr("Ctrl+A"));
    fitAllAction->setStatusTip(tr("Fit contents to viewport"));
    connect(fitAllAction, SIGNAL(triggered()), mainOCC, SLOT(fitAll()));

    zoomAction = new QAction(tr("&Zoom"), this);
    zoomAction->setStatusTip(tr("Zoom in window"));
    connect(zoomAction, SIGNAL(triggered()), mainOCC, SLOT(fitArea()));

    panAction = new QAction(tr("&Pan"), this);
    panAction->setStatusTip(tr("Window panning"));
    connect(panAction, SIGNAL(triggered()), mainOCC, SLOT(pan()));

    rotAction = new QAction(tr("&Rotate"), this);
    rotAction->setShortcut(tr("Ctrl+R"));
    rotAction->setStatusTip(tr("Window rotation"));
    connect(rotAction, SIGNAL(triggered()), mainOCC, SLOT(rotation()));

    gridOnAction = new QAction(tr("&Grid On"), this);
    gridOnAction->setShortcut(tr("Ctrl+G"));
    gridOnAction->setStatusTip(tr("Turn the grid on"));
    connect(gridOnAction, SIGNAL(triggered()), mainOCC, SLOT(gridOn()));

    prjOptionsAction = new QAction(tr("&Project options"), this);
    prjOptionsAction->setStatusTip(tr("Set project options"));
    connect(prjOptionsAction, SIGNAL(triggered()), this, SLOT(setGlobals()));

    configAction = new QAction(tr("&Configuration"), this);
    configAction->setStatusTip(tr("Emcad configuration"));
    connect(configAction, SIGNAL(triggered()), this, SLOT(setConfig()));

    showLayersAction = new QAction(tr("&Show imported layers"), this);
    showLayersAction->setCheckable (true);
    showLayersAction->setStatusTip(tr("Show shape layers defined in the step file"));
    connect(showLayersAction, SIGNAL(toggled(bool)), treeWidget, SLOT(showLayers(bool)));

    gridOffAction = new QAction(tr("Gri&d Off"), this);
    gridOffAction->setShortcut(tr("Ctrl+D"));
    gridOffAction->setStatusTip(tr("Turn the grid on"));
    connect(gridOffAction, SIGNAL(triggered()), mainOCC, SLOT(gridOff()));

    gridXYAction = new QAction(tr("XY Grid"), this);
    gridXYAction->setStatusTip(tr("Grid on XY Plane"));
    //gridOffAction->setShortcut(tr("Ctrl+Z"));
    connect(gridXYAction, SIGNAL(triggered()), mainOCC, SLOT(gridXY()));

    gridXZAction = new QAction(tr("XZ Grid"), this);
    gridXZAction->setStatusTip(tr("Grid on XZ Plane"));
    //gridXZAction->setShortcut(tr("Ctrl+Y"));
    connect(gridXZAction, SIGNAL(triggered()), mainOCC, SLOT(gridXZ()));

    gridYZAction = new QAction(tr("YZ Grid"), this);
    gridYZAction->setStatusTip(tr("Grid on YZ Plane"));
    //gridOffAction->setShortcut(tr("Ctrl+Z"));
    connect(gridYZAction, SIGNAL(triggered()), mainOCC, SLOT(gridYZ()));

    gridRectAction = new QAction(tr("Rectangular"), this);
    gridRectAction->setStatusTip(tr("Retangular grid"));
    //gridOffAction->setShortcut(tr("Ctrl+Z"));
    connect(gridRectAction, SIGNAL(triggered()), mainOCC, SLOT(gridRect()));

    gridCircAction = new QAction(tr("Circular"), this);
    gridCircAction->setStatusTip(tr("Circular grid"));
    //gridOffAction->setShortcut(tr("Ctrl+Z"));
    connect(gridCircAction, SIGNAL(triggered()), mainOCC, SLOT(gridCirc()));
	
	// Standard View

    viewFrontAction = new QAction(tr("Front"), this);
    viewFrontAction->setStatusTip(tr("View From Front"));
    connect(viewFrontAction, SIGNAL(triggered()), mainOCC, SLOT(viewFront()));

    viewBackAction = new QAction(tr("Back"), this);
    viewBackAction->setStatusTip(tr("View From Back"));
    connect(viewBackAction, SIGNAL(triggered()), mainOCC, SLOT(viewBack()));

    viewTopAction = new QAction(tr("Top"), this);
    viewTopAction->setStatusTip(tr("View From Top"));
    connect(viewTopAction, SIGNAL(triggered()), mainOCC, SLOT(viewTop()));

    viewBottomAction = new QAction(tr("Bottom"), this);
    viewBottomAction->setStatusTip(tr("View From Bottom"));
    connect(viewBottomAction, SIGNAL(triggered()), mainOCC, SLOT(viewBottom()));

    viewLeftAction = new QAction(tr("Left"), this);
    viewLeftAction->setStatusTip(tr("View From Left"));
    connect(viewLeftAction, SIGNAL(triggered()), mainOCC, SLOT(viewLeft()));

    viewRightAction = new QAction(tr("Right"), this);
    viewRightAction->setStatusTip(tr("View From Right"));
    connect(viewRightAction, SIGNAL(triggered()), mainOCC, SLOT(viewRight()));

    viewAxoAction = new QAction(tr("Axonometric"), this);
    viewAxoAction->setStatusTip(tr("Axonometric view"));
    connect(viewAxoAction, SIGNAL(triggered()), mainOCC, SLOT(viewAxo()));

    viewGridAction = new QAction(tr("Grid"), this);
    viewGridAction->setStatusTip(tr("View from grid"));
    connect(viewGridAction, SIGNAL(triggered()), mainOCC, SLOT(viewGrid()));

    viewResetAction = new QAction(tr("Reset"), this);
    viewResetAction->setStatusTip(tr("Reset the view"));
    connect(viewResetAction, SIGNAL(triggered()), mainOCC, SLOT(viewReset()));

    backgroundAction = new QAction( tr("&Background"), this );
    backgroundAction->setStatusTip(tr("Change the background colour"));
    connect(backgroundAction, SIGNAL(triggered()), mainOCC, SLOT(background()));
	
	// The co-ordinates from the view
//    connect( mainOCC, SIGNAL(mouseMoved(V3d_Coordinate,V3d_Coordinate,V3d_Coordinate)),
//		     this,   SLOT(xyzPosition(V3d_Coordinate,V3d_Coordinate,V3d_Coordinate)) );

//    connect( mainOCC, SIGNAL(selectedLabelChanged()),\
//            this,   SLOT(getOcafSelection()) );

   connect( mainOCC, SIGNAL(sendStatus(const QString)),\
             this,  SLOT  (temporaryMessage(const QString)) );

   connect(&modeler, SIGNAL(modelerEnd()), this, SLOT(atModelerEnd()));
   connect(&modeler, SIGNAL(sendLogMessageSignal(const QString &)), logger, SLOT(appendPlainText(const QString &)));
   connect(&modeler, SIGNAL(sendStatusMessageSignal(const QString)), this, SLOT(statusMessage(const QString)));
   connect(&preproc, SIGNAL(decomposerEnd()), this, SLOT(atDecomposerEnd()));
   connect(&preproc, SIGNAL(mesherEnd()), this, SLOT(atMesherEnd()));
   connect(&preproc, SIGNAL(sendLogMessageSignal(const QString &)), logger, SLOT(appendPlainText(const QString &)));
   connect(&preproc, SIGNAL(sendStatusMessageSignal(const QString)), this, SLOT(statusMessage(const QString)));
   connect(&tuner, SIGNAL(sendLogMessageSignal(const QString &)), logger, SLOT(appendPlainText(const QString &)));
   connect(&tuner, SIGNAL(sendStatusMessageSignal(const QString)), this, SLOT(statusMessage(const QString)));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( tr("&File") );
	fileMenu->addAction( newProjectAction );
	fileMenu->addAction( openAction );
	fileMenu->addAction( saveAsAction );
	fileMenu->addAction( saveAction );
	importMenu=fileMenu->addMenu(tr("&Import"));
		importMenu->addAction( importGeometryAction );
		importMenu->addAction( importMaterialAction );
		importMenu->addAction( importS2P_Action );
		importMenu->addAction( importIdealFilterCircuitAction );
		importMenu->addAction( importIdealFilterParAction );
		importMenu->addAction( importFilterCircuitAction );
		importMenu->addAction( importFilterParAction );
	exportMenu=fileMenu->addMenu(tr("&Export"));
		exportMenu->addAction( exportMaterialAction );
		exportMenu->addAction( exportSpiceAction );
		exportMenu->addAction( exportFreqResponseAction );
		exportMenu->addAction( exportIdealResponseAction );
		exportMenu->addAction( exportMappedResponseAction );
		exportMenu->addAction( exportIdealJCAction );
		exportMenu->addAction( exportIdealSpiceAction );
		exportMenu->addAction( exportMappedJCAction );
		exportMenu->addAction( exportMappedSpiceAction );
		exportMenu->addAction( exportJC_IdealMappedTZ_Action );
		exportMenu->addAction( exportSpice_IdealMappedTZ_Action );
		exportMenu->addAction( exportFilterTuneParAction );
	fileMenu->addAction( closeCompAction );
	fileMenu->addAction( openCompAction );
	fileMenu->addAction( openCompAction_ );
	fileMenu->addAction( closeAction );
//	fileMenu->addAction( printAction );

	fileMenu->addSeparator();
	fileMenu->addAction( exitAction );


	viewMenu = menuBar()->addMenu( tr("&View") );
		viewGeomMenu = viewMenu->addMenu( tr("&Geometry") );
			viewGeomMenu->addAction( viewFrontAction );
			viewGeomMenu->addAction( viewBackAction );
			viewGeomMenu->addAction( viewTopAction );
			viewGeomMenu->addAction( viewBottomAction );
			viewGeomMenu->addAction( viewLeftAction );
			viewGeomMenu->addAction( viewRightAction );
			viewGeomMenu->addSeparator();
			viewGeomMenu->addAction( viewAxoAction );
			viewGeomMenu->addAction( viewGridAction );
			viewGeomMenu->addSeparator();
			viewGeomMenu->addAction( viewResetAction );
			viewGeomMenu->addAction( fitAction );
			viewGeomMenu->addAction( fitAllAction );
			viewGeomMenu->addSeparator();
			viewGeomMenu->addAction( backgroundAction );
//			viewGeomMenu->addAction( zoomAction );
//			viewGeomMenu->addAction( panAction );
//			viewGeomMenu->addAction( rotAction );
		gridMenu = viewMenu->addMenu( tr("&Grid") );
			gridMenu->addAction( gridOnAction );
			gridMenu->addAction( gridOffAction );
			gridMenu->addSeparator();
			gridMenu->addAction( gridXYAction );
			gridMenu->addAction( gridXZAction );
			gridMenu->addAction( gridYZAction );
			gridMenu->addSeparator();
			gridMenu->addAction( gridRectAction );
			gridMenu->addAction( gridCircAction );
		viewMenu->addAction( showLayersAction );
		viewMenu->addSeparator();
		viewMenu->addAction( meshViewAction );
		plotsMenu = viewMenu->addMenu( tr("&Plots") );
		plotFreqResponseMenu = plotsMenu->addMenu( tr("&Frequency Response") );
		plotZeroPoleMenu = plotsMenu->addMenu( tr("&Zeros and Poles") );
		        plotFreqResponseMenu->addAction( responsePlotAction );
		        plotFreqResponseMenu->addAction( mapped_responsePlotAction );
		        plotFreqResponseMenu->addAction( imported_responsePlotAction );
		        plotFreqResponseMenu->addAction( ideal_responsePlotAction );
		        plotFreqResponseMenu->addAction( ideal_mappedTZ_responsePlotAction );
		        plotZeroPoleMenu->addAction( zeropolePlotAction );
		        plotZeroPoleMenu->addAction( mapped_zeropolePlotAction );
		        plotZeroPoleMenu->addAction( ideal_zeropolePlotAction );
		viewMenu->addSeparator();
		viewMenu->addAction( accountStatusAction );

        editMenu = menuBar()->addMenu( tr("&Edit ") );
		editMenu->addAction( prjOptionsAction );
		editMenu->addSeparator();
		editMaterialsMenu = editMenu->addMenu( tr("&Materials") );
			editMaterialsMenu->addAction( defineMaterialAction );
			editMaterialsMenu->addAction( assignMaterialAction );
		        editMaterialsMenu->addAction( defaultBC_Action );
		editLayersMenu = editMenu->addMenu( tr("&Layers") );
			editLayersMenu->addAction( assignLayerAction );
		editMenu->addSeparator();
		editMenu->addAction( decomposeAction );
		editMenu->addAction( meshAction );
		editMenu->addAction( reloadAction );
		editMenu->addSeparator();
		editMenu->addAction( modelizeAction );
                designsMenu = editMenu->addMenu( tr("&Designs ") );
		       designsMenu->addAction( filterDesignAction );
		       designsMenu->addAction( filterDesignWithTzMapAction );
		       designsMenu->addAction( filterTuneAction );
                analysesMenu = editMenu->addMenu( tr("&Analyses ") );
		       analysesMenu->addAction( portModesAction );
		       analysesMenu->addAction( freqAnaAction );
		       analysesMenu->addAction( zeropoleAnaAction );
		       analysesMenu->addAction( filterMapAction );
		editMenu->addAction( updateAction );
	editMenu->addSeparator();
	editMenu->addAction( configAction );


    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::checkActions()
{
  QString RM_JCpath=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM.JC");
  QString RM_SZPpath=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM.SZP");
  QString RM_TSpath=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM.ts");
  QString RM_SPpath=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM.sp");
  QString MAT_path =nativePath(mainWorkPath+"/Data/materials.mat");
  bool hasRM_JC=FileExists(RM_JCpath.toLatin1().data());
  bool hasRM_SZP=FileExists(RM_SZPpath.toLatin1().data());
  bool hasRM_TS=FileExists(RM_TSpath.toLatin1().data());
  bool hasRM_SP=FileExists(RM_SPpath.toLatin1().data());
  bool hasMAT=FileExists(MAT_path.toLatin1().data());
  QString mappedCircuitName;
  if(prjData.filtermapSource==ZEROPOLES) mappedCircuitName=prjData.mainAssName+"_RM_mapped";
  if(prjData.filtermapSource==IMPORTED_RESPONSE) mappedCircuitName="imported_response_mapped";
  if(prjData.filtermapSource==IMPORTED_CIRCUIT)  mappedCircuitName="imported_circuit";
  QString mappedJCpath=nativePath(mainWorkPath+"/Data/Circuits/"+mappedCircuitName+".JC");
  QString mappedSPpath=nativePath(mainWorkPath+"/Data/Circuits/"+mappedCircuitName+".sp");
  QString mappedSZPpath=nativePath(mainWorkPath+"/Data/Circuits/"+mappedCircuitName+".SZP");
//  QString mappedTSpath=nativePath(mainWorkPath+"/Data/Circuits/"+mappedCircuitName+".ts");
  QString idealJCpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter.JC");
  QString idealSPpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter.sp");
  QString idealS2Ppath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter.s2p");
  QString idealJCmappedTZpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter_mappedTZ.JC");
  QString idealSPmappedTZpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter_mappedTZ.sp");
  QString idealSZPpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter.SZP");
  QString idealTSpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter.ts");
  QString freqTSpath=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM.ts");
  QString mappedTSpath=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM_mapped.ts");
  QString importedS2Ppath=nativePath(mainWorkPath+"/Data/Circuits/imported_response.s2p");
  QString ideal_mappedTZ_TSpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter_mappedTZ.ts");
  QString filtInpPath;
  if(prjData.filtermapSource==ZEROPOLES) filtInpPath=nativePath(mainWorkPath+"/Data/Circuits/"+mappedCircuitName+".inp");
  if(prjData.filtermapSource==IMPORTED_RESPONSE) filtInpPath=importedS2Ppath;
  bool hasMappedJC=FileExists(mappedJCpath.toLatin1().data());
  bool hasMappedSP=FileExists(mappedSPpath.toLatin1().data());
  bool hasMappedSZP=FileExists(mappedSZPpath.toLatin1().data());
  bool hasIdealJC=FileExists(idealJCpath.toLatin1().data());
  bool hasIdealSP=FileExists(idealSPpath.toLatin1().data());
  bool hasIdealJCmappedTZ=FileExists(idealJCmappedTZpath.toLatin1().data());
  bool hasIdealSPmappedTZ=FileExists(idealSPmappedTZpath.toLatin1().data());
  bool hasIdealSZP=FileExists(idealSZPpath.toLatin1().data());
  bool hasIdealTS=FileExists(idealTSpath.toLatin1().data());
  bool hasFreqTS=FileExists(freqTSpath.toLatin1().data());
  bool hasMappedTS=FileExists(mappedTSpath.toLatin1().data());
  bool hasImportedTS=FileExists(importedS2Ppath.toLatin1().data());
  bool hasIdeal_mappedTZ_TS=FileExists(ideal_mappedTZ_TSpath.toLatin1().data());
  bool hasFiltInp=FileExists(filtInpPath.toLatin1().data());
  bool hasFiltSpec=prjData.filterOrder>0 && prjData.filterRetLoss>0 && prjData.filterPassBand[0]>0 && prjData.filterPassBand[1] >0;
  newProjectAction->setEnabled(!runStatus.projectIsOpen);
//  saveAction->setEnabled(runStatus.projectIsOpen);
  saveAsAction->setEnabled(runStatus.projectIsOpen &&!runStatus.runningDecomposer &&!runStatus.runningModeler &&!runStatus.runningMesher);
  saveAction->setEnabled(runStatus.projectIsOpen && prjData.projectName!=QString("Untitled") &&!runStatus.runningModeler &&!runStatus.runningMesher);
  closeAction->setEnabled(runStatus.projectIsOpen &&!runStatus.runningDecomposer &&!runStatus.runningModeler &&!runStatus.runningMesher);
  openAction->setEnabled(!runStatus.projectIsOpen &&!runStatus.runningDecomposer &&!runStatus.runningModeler &&!runStatus.runningMesher);
  closeCompAction->setEnabled(runStatus.projectIsOpen && (mainOCAF->EmP.level || mainOCAF->isPartition()));
  openCompAction_->setEnabled(runStatus.projectIsOpen && mainOCAF->EmP.assemblyType==NET);
//  importDataDirAction->setEnabled(projectIsOpen);
  importGeometryAction->setEnabled(runStatus.projectIsOpen);
  importMaterialAction->setEnabled(runStatus.projectIsOpen);
  importS2P_Action->setEnabled(runStatus.projectIsOpen);
  importIdealFilterCircuitAction->setEnabled(runStatus.projectIsOpen);
  exportMaterialAction->setEnabled(runStatus.projectIsOpen && hasMAT);
  exportSpiceAction->setEnabled(runStatus.projectIsOpen && hasRM_SP);
  exportIdealJCAction->setEnabled(runStatus.projectIsOpen && hasIdealJC);
  exportIdealSpiceAction->setEnabled(runStatus.projectIsOpen && hasIdealSP);
  exportIdealResponseAction->setEnabled(runStatus.projectIsOpen && hasIdealTS);
  exportFreqResponseAction->setEnabled(runStatus.projectIsOpen && hasFreqTS);
  exportMappedResponseAction->setEnabled(runStatus.projectIsOpen && hasMappedTS);
  exportJC_IdealMappedTZ_Action->setEnabled(runStatus.projectIsOpen && hasIdealJCmappedTZ);
  exportSpice_IdealMappedTZ_Action->setEnabled(runStatus.projectIsOpen && hasIdealSPmappedTZ);
  exportMappedJCAction->setEnabled(runStatus.projectIsOpen && hasMappedJC);
  exportMappedSpiceAction->setEnabled(runStatus.projectIsOpen && hasMappedSP);
  decomposeAction->setEnabled(runStatus.projectIsOpen &&!runStatus.runningDecomposer &&!runStatus.runningModeler &&!runStatus.runningMesher  && !mainOCAF->EmP.level && mainOCAF->prjStatus.partMaterials);
  meshAction->setEnabled(runStatus.projectIsOpen &&!runStatus.runningDecomposer &&!runStatus.runningModeler &&!runStatus.runningMesher && !mainOCAF->EmP.level && mainOCAF->prjStatus.partMaterials);
  reloadAction->setEnabled(runStatus.projectIsOpen &&!runStatus.runningDecomposer &&!runStatus.runningModeler &&!runStatus.runningMesher &&mainOCAF->EmP.hasGeo );
  meshViewAction->setEnabled(runStatus.projectIsOpen  &&!runStatus.runningDecomposer &&!runStatus.runningMesher);
  responsePlotAction->setEnabled(runStatus.projectIsOpen && hasRM_TS);
  zeropolePlotAction->setEnabled(runStatus.projectIsOpen && hasRM_SZP);
  mapped_responsePlotAction->setEnabled(runStatus.projectIsOpen && hasMappedTS);
  mapped_zeropolePlotAction->setEnabled(runStatus.projectIsOpen && hasMappedSZP);
  ideal_responsePlotAction->setEnabled(runStatus.projectIsOpen && hasIdealTS);
  imported_responsePlotAction->setEnabled(runStatus.projectIsOpen && hasImportedTS);
  ideal_mappedTZ_responsePlotAction->setEnabled(runStatus.projectIsOpen && hasIdeal_mappedTZ_TS);
  ideal_zeropolePlotAction->setEnabled(runStatus.projectIsOpen && hasIdealSZP);
  modelizeAction->setEnabled (runStatus.projectIsOpen &&!runStatus.runningDecomposer &&!runStatus.runningModeler &&!runStatus.runningMesher   && mainOCAF->prjStatus.partMaterials &&!preproc.failure());
  portModesAction->setEnabled (runStatus.projectIsOpen &&!runStatus.runningDecomposer &&!runStatus.runningModeler &&!runStatus.runningMesher   && mainOCAF->prjStatus.partMaterials);
  freqAnaAction->setEnabled (runStatus.projectIsOpen &&!runStatus.runningDecomposer &&!runStatus.runningModeler  &&!runStatus.runningMesher );
  zeropoleAnaAction->setEnabled (runStatus.projectIsOpen &&!runStatus.runningDecomposer &&!runStatus.runningModeler  &&!runStatus.runningMesher);
  filterDesignAction->setEnabled(runStatus.projectIsOpen && !runStatus.runningDecomposer &&!runStatus.runningModeler &&!runStatus.runningMesher);
  filterMapAction->setEnabled(runStatus.projectIsOpen && !runStatus.runningDecomposer &&!runStatus.runningModeler &&!runStatus.runningMesher);
  filterDesignWithTzMapAction->setEnabled(runStatus.projectIsOpen && !runStatus.runningDecomposer &&!runStatus.runningModeler &&!runStatus.runningMesher && hasFiltInp);
  filterTuneAction->setEnabled(runStatus.projectIsOpen && !runStatus.runningDecomposer &&!runStatus.runningModeler &&!runStatus.runningMesher && (prjData.filtermapSource==IMPORTED_CIRCUIT) || hasFiltInp);
  updateAction->setEnabled (runStatus.projectIsOpen &&!runStatus.runningDecomposer &&!runStatus.runningModeler  &&!runStatus.runningMesher &&!preproc.failure());
  exitAction->setEnabled(!runStatus.runningDecomposer &&!runStatus.runningModeler &&!runStatus.runningMesher );
}

/*
void RTextItemDelegate::paint ( QPainter * p, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{

   p->save();
   QTextDocument *td=new QTextDocument();
   QString text=index.data(Qt::DisplayRole).toString();
   td->setHtml(text);
   QPoint P=option.rect.topLeft();
   p->translate(P.x(), P.y()-5);
   td->drawContents ( p);
//   p->drawText(option.rect, text);
   p->restore();
}

RTextComboBox::RTextComboBox(int size) : QComboBox(){
   rtd=new RTextItemDelegate(this);
   fixedSize=size;
   setFixedWidth (size);
   popv=new QListWidget(this);
   popv->setMaximumSize(size, 500);
   popv->setFixedWidth (size);
   popv->setItemDelegate(rtd);
   setModel(popv->model());
   setView (popv );
 }

void RTextComboBox::paintEvent( QPaintEvent* e){
   QPen pen(Qt::black);
   pen.setWidth(3);

   QStylePainter sp(this);
//   sp.setPen(this->palette().color(QPalette().Text()))
   sp.setPen(pen);

   QString text=this->itemData(this->currentIndex(),Qt::DisplayRole).toString();

   QStyleOptionComboBox opt=QStyleOptionComboBox();
   this->initStyleOption(&opt);
   sp.drawComplexControl(QStyle::CC_ComboBox, opt);

   QPainter p(this);
   p.setPen(pen);

   QRect rect = sp.style()->subElementRect(QStyle::SE_ComboBoxFocusRect, &opt, this);
//   rect.adjust(+5, 0, -5, 0);

   p.save();
   QTextDocument *td=new QTextDocument();
   td->setHtml(text);
   QPoint P=rect.topLeft();
   p.translate(P.x(), P.y()-5);
   td->drawContents (&p);
   p.restore();
}



QSize RTextItemDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex & ) const
{
     return QSize(hintSize, hintSize);
}

*/


QIcon  richTextIcon(QString text, int w, int h){
      QPoint point;
      QTextDocument richTextLabel;
      richTextLabel.setHtml(text);
      double s=h-1; s/=richTextLabel.size().height();
      QPixmap richTextPixmap(w, h);
      richTextPixmap.fill(Qt::transparent);
      QPainter p(&richTextPixmap);
      p.translate(0.5*(w-richTextLabel.size().width()),0);
      p.scale(s,s);
      richTextLabel.drawContents(&p, richTextPixmap.rect());
      QIcon icon(richTextPixmap);
      return icon;

}

void RTextComboBox::addRtItem ( const QString & text){
  addItem(richTextIcon(text, width, height-2),tr(""));
}


SetGlobalsDialog::SetGlobalsDialog(MainWindow * parent, Qt::WindowFlags f ) : QDialog(parent, f)
{
     mainw=parent;
     setModal(0);
     QDoubleValidator *dvalidator = new QDoubleValidator(this);
     dvalidator->setDecimals(1000); // (standard anyway)
     dvalidator->setNotation(QDoubleValidator::ScientificNotation);


//****************************************
//   Units

     QLabel *unitsLabel= new QLabel(); unitsLabel->setText(tr("Units:"));
//     RTextComboBox* lUnitChooser = new RTextComboBox(200);
     lUnitChooser = new RTextComboBox(100,30);
     fUnitChooser = new RTextComboBox(100,30);

//     fUnitChooser->resize(1,5);
//     lUnitChooser->resize(1,5);

     QString micron=tr("<font face='symbol'>m </font><font face='arial'>m");
//     lUnitChooser->view()->verticalHeader()->setItemDelegate(rtd0);
     if(!mainOCAF->EmP.hasGeo || !strcmp(prjData.lengthUnitName,"M"))  lUnitChooser->addRtItem(tr("m"));
     if(!mainOCAF->EmP.hasGeo || !strcmp(prjData.lengthUnitName,"MM")) lUnitChooser->addRtItem(tr("mm"));
     if(!mainOCAF->EmP.hasGeo || !strcmp(prjData.lengthUnitName,"UM")) lUnitChooser->addRtItem(micron);
     if(!mainOCAF->EmP.hasGeo){
       int lI;
       if(!strcmp(prjData.lengthUnitName,"M"))  lI=0;
       else  if(!strcmp(prjData.lengthUnitName,"MM")) lI=1;
       else if(!strcmp(prjData.lengthUnitName,"UM")) lI=2;
       lUnitChooser->setCurrentIndex(lI);
     }else lUnitChooser->setCurrentIndex(0);
     fUnitChooser->addRtItem(tr("Hz"));
     fUnitChooser->addRtItem(tr("KHz"));
     fUnitChooser->addRtItem(tr("MHz"));
     fUnitChooser->addRtItem(tr("GHz"));
     int fI=prjData.freqUnitE/3;
     fUnitChooser->setCurrentIndex(fI);

     QGridLayout *unitsLayout = new QGridLayout();
     unitsLayout->addWidget(unitsLabel, 0, 0);
     unitsLayout->addWidget(lUnitChooser,0, 1);
     unitsLayout->addWidget(fUnitChooser,0, 2);

     QGroupBox *unitsGroupBox=new QGroupBox();
     unitsGroupBox->setLayout(unitsLayout);

//****************************************
//   Frequencies
     QLabel *MORFreqNumLabel= new QLabel();
     MORFreqNumLabel->setText(tr("MOR Frequency Num:"));
     QLabel *freqBandLabel= new QLabel();
     freqBandLabel->setText(tr("Modelization Frequency Band:"));
     QLabel *bandSepLabel= new QLabel();
     bandSepLabel->setText(tr("  :  "));

     f1LineEdit = new QLineEdit();
     f1LineEdit->setText(QString("%1").arg(prjData.freqBand[0], 0, 'f', 5));
     f1LineEdit->setValidator(dvalidator);		
     f2LineEdit = new QLineEdit();
     f2LineEdit->setText(QString("%1").arg(prjData.freqBand[1], 0, 'f', 5));
     f2LineEdit->setValidator(dvalidator);

     QLabel *rFreqBandLabel= new QLabel();
     rFreqBandLabel->setText(tr("Refinement Frequency Band:"));

     bool changed=false;
     if(prjData.refFreqBand[0]<prjData.freqBand[0]){
	 prjData.refFreqBand[0]=prjData.freqBand[0];
         changed=true;
     }
     if(prjData.refFreqBand[1]>prjData.freqBand[1]){
	 prjData.refFreqBand[1]=prjData.freqBand[1];
         changed=true;
     }
     if(prjData.refFreqBand[1]<prjData.refFreqBand[0]){
	 prjData.refFreqBand[1]=prjData.freqBand[1];
         changed=true;
     }
     if(changed) prjData.saveSettings();
     rf1LineEdit = new QLineEdit();
     rf1LineEdit->setText(QString("%1").arg(prjData.refFreqBand[0], 0, 'f', 5));
     rf1LineEdit->setValidator(dvalidator);		
     rf2LineEdit = new QLineEdit();
     rf2LineEdit->setText(QString("%1").arg(prjData.refFreqBand[1], 0, 'f', 5));
     rf2LineEdit->setValidator(dvalidator);

   
//****************************************
//   Mesh
     QLabel *meshSizeLabel= new QLabel();
     meshSizeLabel->setText(tr("Mesh/Wavelength:"));
     QIntValidator *meshPerLenValidator = new QIntValidator(this);
     meshPerLenValidator->setRange(5,10000);
     meshSizeLineEdit = new QLineEdit();
     meshSizeLineEdit->setText(QString("%1").arg(prjData.meshPerWavelen));
     meshSizeLineEdit->setValidator(meshPerLenValidator);
     meshSizeLineEdit->setFixedWidth(150);

     localMeshing3dCB=new QCheckBox("Local 3D Meshing", this);
     if(prjData.localMeshing3d)  localMeshing3dCB->setCheckState(Qt::Checked);
     else                        localMeshing3dCB->setCheckState(Qt::Unchecked);

     QLabel *meshPerCircleLabel= new QLabel();
     meshPerCircleLabel->setText(tr("Mesh/Circle:"));
     QIntValidator *meshPerCircleValidator = new QIntValidator(this);
     meshPerCircleValidator->setRange(5,30);
     meshPerCircleLineEdit = new QLineEdit();
     meshPerCircleLineEdit->setText(QString("%1").arg(prjData.meshPerCircle));
     meshPerCircleLineEdit->setValidator(meshPerCircleValidator);
     meshPerCircleLineEdit->setFixedWidth(150);

     QLabel *meshRefineMaxNumLabel= new QLabel();
     meshRefineMaxNumLabel->setText(tr("Mesh Refinements Max:"));
     QIntValidator *meshRefineMaxNumValidator = new QIntValidator(this);
     meshRefineMaxNumValidator->setRange(0,30);
     meshRefineMaxNumLineEdit = new QLineEdit();
     meshRefineMaxNumLineEdit->setText(QString("%1").arg(prjData.meshRefineMaxNum));
     meshRefineMaxNumLineEdit->setValidator(meshRefineMaxNumValidator);
     meshRefineMaxNumLineEdit->setFixedWidth(150);

     QLabel *meshMinEnergyRatioLabel= new QLabel();
     meshMinEnergyRatioLabel->setText(tr("Mesh Refinement Threshold [dB]:"));
     QDoubleValidator *meshMinEnergyRatioValidator = new QDoubleValidator(this);
     meshMinEnergyRatioValidator->setRange(0,50,1);
     meshMinEnergyRatioLineEdit = new QLineEdit();
     meshMinEnergyRatioLineEdit->setText(QString("%1").arg(prjData.meshMinEnergyRatio, 0, 'f', 1));
     meshMinEnergyRatioLineEdit->setValidator(meshMinEnergyRatioValidator);
     meshMinEnergyRatioLineEdit->setFixedWidth(150);


//****************************************
//   cutoff
     QLabel *cutoffLabel= new QLabel();
     cutoffLabel->setText(tr("Interface Cutoff Ratio:"));

     QDoubleValidator *cutoffValidator = new QDoubleValidator(this);
     cutoffValidator->setRange(1,20,1);

     cutoffLineEdit = new QLineEdit();
     cutoffLineEdit->setFixedWidth(150);
     cutoffLineEdit->setText(QString("%1").arg(prjData.cutoffRatio, 0, 'f', 1));
     cutoffLineEdit->setValidator(cutoffValidator);


//****************************************
//   MOR
     QIntValidator *morFreqNumValidator = new QIntValidator(this);
     morFreqNumValidator->setRange(2,500);
     MORFreqNumLineEdit = new QLineEdit();
     MORFreqNumLineEdit->setText(QString("%1").arg(prjData.MORFreqNum));
     MORFreqNumLineEdit->setValidator(morFreqNumValidator);
     MORFreqNumLineEdit->setFixedWidth(150);

     QLabel *krylovLabel= new QLabel();
     krylovLabel->setText(tr("MOR Krylov Order:"));

     QIntValidator *krylovOrderValidator = new QIntValidator(this);
     krylovOrderValidator->setRange(1,6);
     krylovLineEdit = new QLineEdit();
     krylovLineEdit->setFixedWidth(150);
     krylovLineEdit->setText(QString("%1").arg(prjData.KrylovOrder));
     krylovLineEdit->setValidator(krylovOrderValidator);

     f1LineEdit->setFixedWidth(150);
     f2LineEdit->setFixedWidth(150);

     rf1LineEdit->setFixedWidth(150);
     rf2LineEdit->setFixedWidth(150);

     QGridLayout *freqLayout = new QGridLayout();
//     freqLayout->setColumnMinimumWidth(1,30);

     freqLayout->addWidget(freqBandLabel, 0, 0);
     freqLayout->addWidget(f1LineEdit,1, 0);
     freqLayout->addWidget(bandSepLabel,1, 1);
     freqLayout->addWidget(f2LineEdit,1, 2);
     freqLayout->setRowMinimumHeight(2,10);
     
     freqLayout->addWidget(rFreqBandLabel, 3, 0);
     freqLayout->addWidget(rf1LineEdit,4, 0);
     freqLayout->addWidget(bandSepLabel,4, 1);
     freqLayout->addWidget(rf2LineEdit,4, 2);
     freqLayout->setRowMinimumHeight(5,10);
     freqLayout->addWidget(MORFreqNumLabel, 6, 0);
     freqLayout->addWidget(MORFreqNumLineEdit, 6, 2);
     freqLayout->addWidget(krylovLabel, 7, 0);
     freqLayout->addWidget(krylovLineEdit, 7, 2);


     QGridLayout *otherLayout = new QGridLayout();

     otherLayout->addWidget(cutoffLabel, 0, 0);
     otherLayout->addWidget(cutoffLineEdit, 0, 2);
     otherLayout->addWidget(meshSizeLabel, 1, 0);
     otherLayout->addWidget(meshSizeLineEdit, 1, 2);
     otherLayout->addWidget(meshPerCircleLabel, 2, 0);
     otherLayout->addWidget(meshPerCircleLineEdit, 2, 2);
     otherLayout->addWidget(meshRefineMaxNumLabel, 3, 0);
     otherLayout->addWidget(meshRefineMaxNumLineEdit, 3, 2);
     otherLayout->addWidget(meshMinEnergyRatioLabel, 4, 0);
     otherLayout->addWidget(meshMinEnergyRatioLineEdit, 4, 2);
     otherLayout->addWidget(localMeshing3dCB, 5, 0);
     otherLayout->setRowMinimumHeight(2,30);

     QGroupBox *freqGroupBox=new QGroupBox();
     freqGroupBox->setLayout(freqLayout);

     QGroupBox *otherGroupBox=new QGroupBox();
     otherGroupBox->setLayout(otherLayout);


//****************************************
//   control buttons:
//
     QPushButton *setButton = new QPushButton(tr("Set"));
     setButton->resize(1.5,3);
     QPushButton *closeButton =new QPushButton(tr("Close"));
     closeButton->resize(1.5,3);
     QPushButton *helpButton  = new QPushButton(tr("Help"));
     helpButton->resize(1.5,3);

     QHBoxLayout *buttonLayout = new QHBoxLayout();
     buttonLayout->addWidget(setButton);
     buttonLayout->addWidget(closeButton);
     buttonLayout->addWidget(helpButton);

     QGroupBox *buttonGroupBox=new QGroupBox(tr(""));
     buttonGroupBox->setLayout(buttonLayout);

     connect(setButton, SIGNAL(clicked()), this, SLOT(set()));
     connect(parent, SIGNAL(enterPressed()), this, SLOT(set()));
     connect(closeButton,  SIGNAL(clicked()), this, SLOT(accept()));
     connect(helpButton,  SIGNAL(clicked()), this, SLOT(help()));

     setFocusPolicy(Qt::StrongFocus);

     QVBoxLayout *mainLayout = new QVBoxLayout(this);
     mainLayout->addWidget(unitsGroupBox);
     mainLayout->addWidget(freqGroupBox);
     mainLayout->addWidget(otherGroupBox);
     mainLayout->addWidget(buttonGroupBox);

     setFocusPolicy(Qt::StrongFocus);
}

void SetGlobalsDialog::help()
{
    documentation.showDocumentation(QLatin1String("#page=27"));
}

void SetGlobalsDialog::set(){
      bool changed=false;
      if(!mainOCAF->EmP.hasGeo){  //length unit can not be changed after importing geometry
        int lI=lUnitChooser->currentIndex();
        char lengthUnitName1[5];
        switch(lI){
	 case 0: strcpy(lengthUnitName1,"M"); break;
	 case 1: strcpy(lengthUnitName1,"MM"); break;
	 case 2: strcpy(lengthUnitName1,"UM"); break;
        }
        if(!strcmp(prjData.lengthUnitName,lengthUnitName1)) {
	      strcpy(prjData.lengthUnitName,lengthUnitName1); 
	      changed=true;
              unit.xm=prjData.lengthUnit();
        }
      }
      int fI=3*fUnitChooser->currentIndex();
      if(prjData.freqUnitE!=fI) { 
	      prjData.freqUnitE=fI; 
              unit.xHz=prjData.freqUnit();
	      changed=true; 
	      prjData.workStatus.remeshNeeded=1;
      }

      int localMeshing3d=(localMeshing3dCB->checkState()==Qt::Checked)? 1:0;
      if(localMeshing3d!=prjData.localMeshing3d) {prjData.localMeshing3d=localMeshing3d; changed=true;}

      int ms=meshSizeLineEdit->text().toInt();
      if(prjData.meshPerWavelen!=ms) { prjData.meshPerWavelen=ms; changed=true; prjData.workStatus.remeshNeeded=1;}

      int mc=meshPerCircleLineEdit->text().toInt();
      if(prjData.meshPerCircle!=mc) { prjData.meshPerCircle=mc; changed=true; prjData.workStatus.remeshNeeded=1;}

      int mrefnum=meshRefineMaxNumLineEdit->text().toInt();
      if(prjData.meshRefineMaxNum!=mrefnum) { prjData.meshRefineMaxNum=mrefnum; changed=true; prjData.workStatus.remeshNeeded=1;}

      double mrefratio=meshMinEnergyRatioLineEdit->text().toDouble();
      if(fabs(prjData.meshMinEnergyRatio-mrefratio)>1.e-4) {prjData.meshMinEnergyRatio=mrefratio; changed=true; prjData.workStatus.remeshNeeded=1;}

      int morfn=MORFreqNumLineEdit->text().toInt();
      if(prjData.MORFreqNum!=morfn) { prjData.MORFreqNum=morfn; changed=true; prjData.workStatus.modelizationNeeded=1;}
      
      int k=krylovLineEdit->text().toInt();
      if(prjData.KrylovOrder!=k) { prjData.KrylovOrder=k; changed=true; prjData.workStatus.modelizationNeeded=1;}

      double tmp=f1LineEdit->text().toDouble();
      if(fabs(prjData.freqBand[0]-tmp)>1.e-4) { prjData.freqBand[0]=tmp; changed=true; prjData.workStatus.remeshNeeded=1;}
      tmp=f2LineEdit->text().toDouble();
      if(fabs(prjData.freqBand[1]-tmp)>1.e-4) { prjData.freqBand[1]=tmp; changed=true; prjData.workStatus.remeshNeeded=1;}
      
      double f1=f1LineEdit->text().toDouble();
      double rf1=rf1LineEdit->text().toDouble();
      rf1LineEdit->setText(QString("%1").arg(max(rf1,f1), 0, 'f', 5));
      double f2=f2LineEdit->text().toDouble();
      double rf2=rf2LineEdit->text().toDouble();
      rf2LineEdit->setText(QString("%1").arg(min(rf2,f2), 0, 'f', 5));
      tmp=rf1LineEdit->text().toDouble();
      if(fabs(prjData.refFreqBand[0]-tmp)>1.e-4) { prjData.refFreqBand[0]=tmp; changed=true; prjData.workStatus.remeshNeeded=1;}
      tmp=rf2LineEdit->text().toDouble();
      if(fabs(prjData.refFreqBand[1]-tmp)>1.e-4) { prjData.refFreqBand[1]=tmp; changed=true; prjData.workStatus.remeshNeeded=1;}
      
      tmp=cutoffLineEdit->text().toDouble();
      if(fabs(prjData.cutoffRatio-tmp)>1.e-4) { prjData.cutoffRatio=tmp; changed=true; prjData.workStatus.modelizationNeeded=1;}

      if(changed) prjData.saveSettings();
}

QSize ConfigDialog::sizeHint() const
{
     QSize size = QSize(700,300);
     return size;
}


ConfigDialog::ConfigDialog(MainWindow * parent, Qt::WindowFlags f ) : QDialog(parent, f)
{
     setWindowTitle("EmCAD Configuration");
     mainw=parent;
     QLabel *AWS_bucket_Label= new QLabel();
     AWS_bucket_Label->setText(tr("AWS bucket :"));
     AWS_bucket_LineEdit = new QLineEdit();
     AWS_bucket_LineEdit->setText(QString(emcadConfig[std::string("AWS_bucket")].c_str()));

     QLabel *AWS_access_key_id_Label= new QLabel();
     AWS_access_key_id_Label->setText(tr("AWS access key :"));
     AWS_access_key_id_LineEdit = new QLineEdit();
     AWS_access_key_id_LineEdit->setText(QString(botoConfig[std::string("aws_access_key_id")].c_str()));

     QLabel *AWS_secret_access_key_Label= new QLabel();
     AWS_secret_access_key_Label->setText(tr("AWS secret access key :"));
     AWS_secret_access_key_LineEdit = new QLineEdit();
     AWS_secret_access_key_LineEdit->setText(QString(botoConfig[std::string("aws_secret_access_key")].c_str()));

     componentIsAssembly=new QCheckBox("Components defined as CAD assemblies", this);
     if(emcadConfig[std::string("ComponentIsAssembly")]==std::string("true"))  componentIsAssembly->setCheckState(Qt::Checked);
     else                                                                      componentIsAssembly->setCheckState(Qt::Unchecked);

     QGridLayout *configLayout = new QGridLayout();
     configLayout->addWidget(AWS_bucket_Label, 0, 0);
     configLayout->addWidget(AWS_bucket_LineEdit,0, 1);
     configLayout->addWidget(AWS_access_key_id_Label, 1, 0);
     configLayout->addWidget(AWS_access_key_id_LineEdit,1, 1);
     configLayout->addWidget(AWS_secret_access_key_Label, 2, 0);
     configLayout->addWidget(AWS_secret_access_key_LineEdit,2, 1);
     QGroupBox *configGroupBox=new QGroupBox();
     configGroupBox->setLayout(configLayout);

//****************************************
//   control buttons:
//
     QPushButton *setButton = new QPushButton(tr("Set"));
     setButton->resize(1.5,3);
     QPushButton *closeButton =new QPushButton(tr("Close"));
     closeButton->resize(1.5,3);
     QPushButton *helpButton  = new QPushButton(tr("Help"));
     helpButton->resize(1.5,3);

     QHBoxLayout *buttonLayout = new QHBoxLayout();
     buttonLayout->addWidget(setButton);
     buttonLayout->addWidget(closeButton);
     buttonLayout->addWidget(helpButton);

     QGroupBox *buttonGroupBox=new QGroupBox(tr(""));
     buttonGroupBox->setLayout(buttonLayout);

     connect(setButton, SIGNAL(clicked()), this, SLOT(set()));
     connect(parent, SIGNAL(enterPressed()), this, SLOT(set()));
     connect(closeButton,  SIGNAL(clicked()), this, SLOT(accept()));
     connect(helpButton,  SIGNAL(clicked()), this, SLOT(help()));

     setFocusPolicy(Qt::StrongFocus);

     QVBoxLayout *mainLayout = new QVBoxLayout(this);
     mainLayout->addWidget(configGroupBox);
     mainLayout->addWidget(componentIsAssembly);
     mainLayout->addWidget(buttonGroupBox);
}

void ConfigDialog::help()
{
    documentation.showDocumentation(QLatin1String("#1"));
}

void ConfigDialog::set(){
  bool emcad_changed=false;
  bool boto_changed=false;
  std::string tmp;

  tmp=std::string(AWS_bucket_LineEdit->text().toLatin1());
  if(tmp!=emcadConfig[std::string("AWS_bucket")]) { emcadConfig[std::string("AWS_bucket")]=tmp; emcad_changed=true;}

  tmp=(componentIsAssembly->checkState()==Qt::Checked)? std::string("true"): std::string("false");
  if(tmp!=emcadConfig[std::string("ComponentIsAssembly")]) {emcadConfig[std::string("ComponentIsAssembly")]=tmp; emcad_changed=true;}

  if(emcad_changed) WriteFile(config_filename, &emcadConfig);

  tmp=std::string(AWS_access_key_id_LineEdit->text().toLatin1());
  if(tmp!=botoConfig[std::string("aws_access_key_id")]) { botoConfig[std::string("aws_access_key_id")]=tmp; boto_changed=true;}

  tmp=std::string(AWS_secret_access_key_LineEdit->text().toLatin1());
  if(tmp!=botoConfig[std::string("aws_secret_access_key")]) { botoConfig[std::string("aws_secret_access_key")]=tmp; boto_changed=true;}

  if(boto_changed) WriteBoto();
}


AccountStatusDialog::AccountStatusDialog(MainWindow * parent, Qt::WindowFlags f ) : QDialog(parent, f)
{
     mainw=parent;
     setWindowTitle("Account Status");

     QLabel *credit_Label= new QLabel();
     credit_Label->setText(tr("Credit :"));
     credit_LineEdit = new QLineEdit();
     credit_LineEdit->setReadOnly(true);
     credit_LineEdit->setText(QString(emcadAccount[std::string("credit")].c_str())+QString(" $"));

     QGridLayout *configLayout = new QGridLayout();
     configLayout->addWidget(credit_Label, 0, 0);
     configLayout->addWidget(credit_LineEdit,0, 1);
     QGroupBox *configGroupBox=new QGroupBox();
     configGroupBox->setLayout(configLayout);

//****************************************
//   control buttons:
//
     QPushButton *updateButton = new QPushButton(tr("Update"));
     updateButton->resize(1.5,3);
     QPushButton *closeButton =new QPushButton(tr("Close"));
     closeButton->resize(1.5,3);
     QPushButton *helpButton  = new QPushButton(tr("Help"));
     helpButton->resize(1.5,3);

     QHBoxLayout *buttonLayout = new QHBoxLayout();
     buttonLayout->addWidget(updateButton);
     buttonLayout->addWidget(closeButton);
     buttonLayout->addWidget(helpButton);

     QGroupBox *buttonGroupBox=new QGroupBox(tr(""));
     buttonGroupBox->setLayout(buttonLayout);

     connect(updateButton, SIGNAL(clicked()), this, SLOT(updateCredit()));
     connect(closeButton,  SIGNAL(clicked()), this, SLOT(accept()));
     connect(helpButton,  SIGNAL(clicked()), this, SLOT(help()));

     setFocusPolicy(Qt::StrongFocus);

     QVBoxLayout *mainLayout = new QVBoxLayout(this);
     mainLayout->addWidget(configGroupBox);
     mainLayout->addWidget(buttonGroupBox);
}

void AccountStatusDialog::help()
{
    documentation.showDocumentation(QLatin1String("#1"));
}

void AccountStatusDialog::updateCredit(){
  mainw->updateAccountCredit();
  if(FileExists(account_filename.c_str())) ReadFile(account_filename, &emcadAccount);
  credit_LineEdit->setText(QString(emcadAccount[std::string("credit")].c_str())+QString(" $"));
}

//void FreqAnaDialog::updateParType(int i) {freqRespParType=(FreqRespParType) i;}



//*************
FreqAnaDialog::FreqAnaDialog(MainWindow * parent, Qt::WindowFlags f ) : QDialog(parent, f)
{
     mainw=parent;
     setModal(0);
     QDoubleValidator *dvalidator = new QDoubleValidator(this);
     dvalidator->setDecimals(1000); // (standard anyway)
     dvalidator->setNotation(QDoubleValidator::ScientificNotation);

     QIntValidator *anaFreqNumValidator = new QIntValidator(this);
     anaFreqNumValidator->setRange(2,100000);

     QIntValidator *morFreqNumValidator = new QIntValidator(this);
     morFreqNumValidator->setRange(2,100);

//****************************************
//   Frequencies
     QLabel *freqBandLabel= new QLabel();
     freqBandLabel->setText(tr("Frequency band:"));

     QLabel *anaFreqNumLabel= new QLabel();
     anaFreqNumLabel->setText(tr("Freq Points Number:"));

     QLabel *MORFreqNumLabel= new QLabel();
     MORFreqNumLabel->setText(tr("MOR Freq Number:"));
     
     QLabel *freqRespParLabel= new QLabel();
     freqRespParLabel->setText(tr("Parameter Type:"));

     parTypeChooser = new QComboBox();
     parTypeChooser->addItem(tr("S"));
     parTypeChooser->addItem(tr("Z"));
     parTypeChooser->addItem(tr("Y"));

     parTypeChooser->setCurrentIndex(prjData.freqRespParType);

//     connect(parTypeChooser, SIGNAL(  currentIndexChanged  (int) ), this, SLOT(updateParType(int)) );

     f1LineEdit = new QLineEdit();
     f1LineEdit->setText(QString("%1").arg(prjData.anaFreqBand[0], 0, 'f', 5));
     f1LineEdit->setValidator(dvalidator);		
     f2LineEdit = new QLineEdit();
     f2LineEdit->setText(QString("%1").arg(prjData.anaFreqBand[1], 0, 'f', 5));
     f2LineEdit->setValidator(dvalidator);

     anaFreqNumLineEdit = new QLineEdit();
     anaFreqNumLineEdit->setText(QString("%1").arg(prjData.anaFreqNum));
     anaFreqNumLineEdit->setValidator(anaFreqNumValidator);

     MORFreqNumLineEdit = new QLineEdit();
     MORFreqNumLineEdit->setText(QString("%1").arg(prjData.anaMORFreqNum));
     MORFreqNumLineEdit->setValidator(morFreqNumValidator);

     f1LineEdit->setFixedWidth(150);
     f2LineEdit->setFixedWidth(150);
     anaFreqNumLineEdit->setFixedWidth(150);
     MORFreqNumLineEdit->setFixedWidth(150);

     QGridLayout *freqLayout = new QGridLayout();
//     freqLayout->setColumnMinimumWidth(1,30);

     QLabel *bandSepLabel= new QLabel();
     bandSepLabel->setText(tr("  :  "));
     freqLayout->addWidget(freqBandLabel, 0, 0);
     freqLayout->addWidget(f1LineEdit,1, 0);
     freqLayout->addWidget(bandSepLabel,1, 1);
     freqLayout->addWidget(f2LineEdit,1, 2);

     freqLayout->setRowMinimumHeight(2,25);

     freqLayout->addWidget(anaFreqNumLabel, 4, 0);
     freqLayout->addWidget(anaFreqNumLineEdit, 4, 2);
     freqLayout->addWidget(freqRespParLabel, 5, 0);
     freqLayout->addWidget(parTypeChooser, 5, 2);

     if(prjData.components.list.size()>1){
       freqLayout->addWidget(MORFreqNumLabel, 6, 0);
       freqLayout->addWidget(MORFreqNumLineEdit, 6, 2);
     }

     QGroupBox *freqGroupBox=new QGroupBox();
     freqGroupBox->setLayout(freqLayout);

//   Fitermap selection

     QLabel *circuitLabel= new QLabel();
     circuitLabel->setText(tr("Circuit:"));
     circuitChooser = new QComboBox();
     circuitChooser->addItem(tr("Elecromagnetic Model"));
     circuitChooser->addItem(tr("Mapped circuit"));
     circuitChooser->addItem(tr("Ideal Filter"));
     circuitChooser->addItem(tr("Ideal Filter Mapped Tx Zeros"));
 

     QHBoxLayout *circuitLayout = new QHBoxLayout();
     circuitLayout->addWidget(circuitLabel);
     circuitLayout->addWidget(circuitChooser);
     QGroupBox *circuitGroupBox=new QGroupBox();
     circuitGroupBox->setLayout(circuitLayout);

     circuitChooser->setCurrentIndex(ELECROMAGNETICDEVICE);
     prjData.analysedCircuit=ELECROMAGNETICDEVICE;

//   Automatic
     automatic=new QCheckBox("Automatic update", this);
     QHBoxLayout *automaticLayout = new QHBoxLayout();
     automaticLayout->addWidget(automatic);
     QGroupBox *automaticGroupBox=new QGroupBox();
     automaticGroupBox->setLayout(automaticLayout);


//****************************************
//   control buttons:
//
     setButton = new QPushButton(tr("Set"));
     setButton->resize(1.5,3);
     startButton =new QPushButton(tr("Start"));
     startButton->resize(1.5,3);
     closeButton =new QPushButton(tr("Close"));
     closeButton->resize(1.5,3);
     QPushButton *helpButton  = new QPushButton(tr("Help"));
     helpButton->resize(1.5,3);

     QHBoxLayout *buttonLayout = new QHBoxLayout();
     buttonLayout->addWidget(setButton);
     buttonLayout->addWidget(startButton);
     buttonLayout->addWidget(closeButton);
     buttonLayout->addWidget(helpButton);

     QGroupBox *buttonGroupBox=new QGroupBox(tr(""));
     buttonGroupBox->setLayout(buttonLayout);

     onCircuitChanged(circuitChooser->currentIndex());
     connect(setButton, SIGNAL(clicked()), this, SLOT(set()));
     connect(parent, SIGNAL(enterPressed()), this, SLOT(set()));
     connect(startButton,  SIGNAL(clicked()), this, SLOT(start()));
     connect(closeButton,  SIGNAL(clicked()), this, SLOT(accept()));
     connect(helpButton,  SIGNAL(clicked()), this, SLOT(help()));
     connect(circuitChooser, SIGNAL(currentIndexChanged(int)), this, SLOT(onCircuitChanged(int)));

     setFocusPolicy(Qt::StrongFocus);

     mainLayout = new QVBoxLayout(this);
     mainLayout->addWidget(freqGroupBox);
     mainLayout->addWidget(circuitGroupBox);
     mainLayout->addWidget(automaticGroupBox);
     mainLayout->addWidget(buttonGroupBox);

     setFocusPolicy(Qt::StrongFocus);
}

void FreqAnaDialog::onCircuitChanged(int circi)
{
     selectedCircuit = (SelectedCircuit) circi;
     QString JCpath;
     switch(circi){
	 case ELECROMAGNETICDEVICE:
           JCpath=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM.JC");
	   break;
	 case MAPPEDCIRCUIT:
             if(prjData.filtermapSource==ZEROPOLES) JCpath=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM_mapped.JC");
             if(prjData.filtermapSource==IMPORTED_RESPONSE) JCpath=nativePath(mainWorkPath+"/Data/Circuits/imported_response_mapped.JC");
             if(prjData.filtermapSource==IMPORTED_CIRCUIT) JCpath=nativePath(mainWorkPath+"/Data/Circuits/imported_circuit.JC");
	   break;
	 case IDEALCIRCUIT:
           JCpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter.JC");
	   break;
	 case IDEALCIRCUITMAPPEDTZ:
           JCpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter_mappedTZ.JC");
	   break;
     }
     char * jcp=JCpath.toLatin1().data();
     if(circi==ELECROMAGNETICDEVICE) 
	 startButton->setEnabled(true);
     else
         startButton->setEnabled(FileExists(JCpath.toLatin1().data()));
     if((circi==ELECROMAGNETICDEVICE && prjData.autoFreqResponse) || (circi==MAPPEDCIRCUIT && prjData.autoMappedFreqResponse) )
         automatic->setCheckState(Qt::Checked);
     else                            
         automatic->setCheckState(Qt::Unchecked);
}

void FreqAnaDialog::help()
{
    documentation.showDocumentation(QLatin1String("#page=36"));
}

void FreqAnaDialog::set(){

      bool changed;
      int anafn=anaFreqNumLineEdit->text().toInt();
      if(prjData.anaFreqNum!=anafn) { changed=true; prjData.anaFreqNum=anafn;}

      int morfn=MORFreqNumLineEdit->text().toInt();
      if(prjData.anaMORFreqNum!=morfn) { changed=true; prjData.anaMORFreqNum=morfn; prjData.workStatus.reductionNeeded=1;}

      double tmp=f1LineEdit->text().toDouble();
      if(fabs(prjData.anaFreqBand[0]-tmp)>1.e-4) { changed=true; prjData.anaFreqBand[0]=tmp; prjData.workStatus.reductionNeeded=1;}
      tmp=f2LineEdit->text().toDouble();
      if(fabs(prjData.anaFreqBand[1]-tmp)>1.e-4) { changed=true; prjData.anaFreqBand[1]=tmp; prjData.workStatus.reductionNeeded=1;}
     
      if(prjData.freqRespParType!=parTypeChooser->currentIndex()) {
	    changed=true;
	    prjData.freqRespParType = (FreqRespParType) parTypeChooser->currentIndex();
            prjData.freqRespYscaleAuto=1;
            switch(prjData.freqRespParType){
	       case SPAR: prjData.freqRespParPart=0; break;
	       case ZPAR: prjData.freqRespParPart=1; break;
	       case YPAR: prjData.freqRespParPart=1; break;
            }
      }

      prjData.analysedCircuit = selectedCircuit;
      int autostate=(automatic->checkState()==Qt::Checked)? 1 :0;
      switch(prjData.analysedCircuit){
         case ELECROMAGNETICDEVICE:
             if( prjData.autoFreqResponse!=autostate) {changed=true; prjData.autoFreqResponse=autostate;}
	     break;
	 case MAPPEDCIRCUIT:
             if( prjData.autoMappedFreqResponse!=autostate) {changed=true; prjData.autoMappedFreqResponse=autostate;}
	   break;
	 case IDEALCIRCUITMAPPEDTZ:
           if( prjData.autoIdealMappedTzFreqResponse!=autostate) {changed=true; prjData.autoIdealMappedTzFreqResponse=autostate;}
	   break;
      }
      if(changed) prjData.saveSettings();
}

void FreqAnaDialog::start(){
 set();
 setButton->setEnabled(false);
 startButton->setEnabled(false);
 closeButton->setEnabled(false);
 mainw->startTask(FREQANA);
}

void FreqAnaDialog::atFreqAnaEnd(){
 setButton->setEnabled(true);
 startButton->setEnabled(true);
 closeButton->setEnabled(true);
}



//*************
ZeroPoleDialog::ZeroPoleDialog(MainWindow * parent, Qt::WindowFlags f ) : QDialog(parent, f){
     mainw=parent;
     setModal(0);
     setWindowTitle(tr("Zero Pole Analysis"));

     QDoubleValidator *freqvalidator = new QDoubleValidator(this);
     freqvalidator->setDecimals(1000); // (standard anyway)
     freqvalidator->setNotation(QDoubleValidator::ScientificNotation);

     QLabel *freqMinLabel= new QLabel(); freqMinLabel->setText(tr("Freq min:"));
     QLabel *freqMaxLabel= new QLabel(); freqMaxLabel->setText(tr("Freq max:"));

     f1LineEdit = new QLineEdit();
     f1LineEdit->setValidator(freqvalidator);		
     f2LineEdit = new QLineEdit();
     f2LineEdit->setValidator(freqvalidator);
     f1LineEdit->setFixedWidth(150);
     f2LineEdit->setFixedWidth(150);

     QDoubleValidator *winRatioValidator = new QDoubleValidator(this);
     winRatioValidator->setDecimals(10); // (standard anyway)
     winRatioValidator->setNotation(QDoubleValidator::StandardNotation);
     winRatioValidator->setBottom(0.01);

     QLabel *winRatioLabel= new QLabel(); winRatioLabel->setText(tr("Freq Window Ratio H/W:"));
     winRatioLineEdit = new QLineEdit();
     winRatioLineEdit->setFixedWidth(150);
     winRatioLineEdit->setValidator(winRatioValidator);


     QGridLayout *freqLayout = new QGridLayout();
     freqLayout->addWidget(freqMinLabel,0, 0);
     freqLayout->addWidget(freqMaxLabel,0, 1);
     freqLayout->addWidget(f1LineEdit,1, 0);
     freqLayout->addWidget(f2LineEdit,1, 1);
     freqLayout->addWidget(winRatioLabel,2, 0);
     freqLayout->addWidget(winRatioLineEdit,2, 1);
     freqLayout->setRowMinimumHeight(2,25);

     QGroupBox *freqBox=new QGroupBox();
     freqBox->setLayout(freqLayout);
     freqBox->show();

     QLabel *zeroParNumLabel= new QLabel(); 
     zeroParNumLabel->setText(tr("Number of curves:"));
     SijCurveNumSB = new QSpinBox();

     zeroCurvesTW= new QTableWidget(0, 2, this);
     #if defined(QT5)
      zeroCurvesTW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     #elif defined(QT4)
      zeroCurvesTW->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
     #endif
     QTableWidgetItem *Sij_header1 = new QTableWidgetItem(tr("port1"));
     QTableWidgetItem *Sij_header2 = new QTableWidgetItem(tr("port2"));
     zeroCurvesTW->setHorizontalHeaderItem(0, Sij_header1);
     zeroCurvesTW->setHorizontalHeaderItem(1, Sij_header2);
     zeroCurvesTW->verticalHeader()->hide();
     
     int SijCurveNum=prjData.zeropoleCurves.size()-1;
     SijCurveNumSB->setValue(SijCurveNum); 

     QGridLayout *SijLayout = new QGridLayout();
     SijLayout->addWidget(zeroParNumLabel, 0, 0);
     SijLayout->addWidget(SijCurveNumSB,   0, 1);
     SijLayout->addWidget(zeroCurvesTW,    1, 0, 1, 2);

     getPrjData();

     QGroupBox *SijBox=new QGroupBox(tr("S Par Zeros Selection:"));
     SijBox->setLayout(SijLayout);

//   Fitermap selection

     QLabel *circuitLabel= new QLabel();
     circuitLabel->setText(tr("Circuit:"));
     circuitChooser = new QComboBox();
     circuitChooser->addItem(tr("Elecromagnetic Model"));
     circuitChooser->addItem(tr("Mapped circuit"));
     circuitChooser->addItem(tr("Ideal circuit"));


     QHBoxLayout *circuitLayout = new QHBoxLayout();
     circuitLayout->addWidget(circuitLabel);
     circuitLayout->addWidget(circuitChooser);
     QGroupBox *circuitGroupBox=new QGroupBox();
     circuitGroupBox->setLayout(circuitLayout);

     circuitChooser->setCurrentIndex(ELECROMAGNETICDEVICE);
     prjData.analysedCircuit=ELECROMAGNETICDEVICE;

//   Automatic
     automatic=new QCheckBox("Automatic update", this);
     QHBoxLayout *automaticLayout = new QHBoxLayout();
     automaticLayout->addWidget(automatic);
     QGroupBox *automaticGroupBox=new QGroupBox();
     automaticGroupBox->setLayout(automaticLayout);


//****************************************
//   control buttons:
//
     setButton = new QPushButton(tr("Set"));
     setButton->resize(1.5,3);
     startButton =new QPushButton(tr("Start"));
     startButton->resize(1.5,3);
     closeButton =new QPushButton(tr("Close"));
     closeButton->resize(1.5,3);
     QPushButton *helpButton  = new QPushButton(tr("Help"));
     helpButton->resize(1.5,3);

     QHBoxLayout *buttonLayout = new QHBoxLayout();
     buttonLayout->addWidget(setButton);
     buttonLayout->addWidget(startButton);
     buttonLayout->addWidget(closeButton);
     buttonLayout->addWidget(helpButton);

     QGroupBox *buttonGroupBox=new QGroupBox(tr(""));
     buttonGroupBox->setLayout(buttonLayout);

     onCircuitChanged(circuitChooser->currentIndex());

     connect(setButton, SIGNAL(clicked()), this, SLOT(set()));
     connect(parent, SIGNAL(enterPressed()), this, SLOT(set()));
     connect(startButton,  SIGNAL(clicked()), this, SLOT(start()));
     connect(closeButton,  SIGNAL(clicked()), this, SLOT(accept()));
     connect(helpButton,  SIGNAL(clicked()), this, SLOT(help()));
     connect(SijCurveNumSB, SIGNAL( valueChanged (int) ), this, SLOT(setSijCurveNum(int)) );
     connect(circuitChooser, SIGNAL(currentIndexChanged(int)), this, SLOT(onCircuitChanged(int)));

     setFocusPolicy(Qt::StrongFocus);


     mainLayout = new QVBoxLayout(this);
//     mainLayout->addWidget(freqGroupBox);
     mainLayout->addWidget(freqBox);
     mainLayout->addWidget(SijBox);
     mainLayout->addWidget(circuitGroupBox);
     mainLayout->addWidget(automaticGroupBox);
     mainLayout->addWidget(buttonGroupBox);

}	

void ZeroPoleDialog::onCircuitChanged(int circi)
{
     selectedCircuit = (SelectedCircuit) circi;

     QString JCpath;
     switch(circi){
	 case ELECROMAGNETICDEVICE:
           JCpath=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM.JC");
	   break;
	 case MAPPEDCIRCUIT:
             if(prjData.filtermapSource==ZEROPOLES) JCpath=nativePath(mainWorkPath+"/Data/Circuits/"+prjData.mainAssName+"_RM_mapped.JC");
             if(prjData.filtermapSource==IMPORTED_RESPONSE) JCpath=nativePath(mainWorkPath+"/Data/Circuits/imported_response_mapped.JC");
	   break;
	 case IDEALCIRCUIT:
           JCpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter.JC");
	   break;
	 case IDEALCIRCUITMAPPEDTZ:
           JCpath=nativePath(mainWorkPath+"/Data/Circuits/ideal_filter_mappedTZ.JC");
	   break;
     }
     bool dataExist=FileExists(JCpath.toLatin1().data());
     startButton->setEnabled(dataExist);
     SijCurveNumSB->setEnabled(dataExist);

     setSijCurveNum(SijCurveNumSB->value());

     if((circi==ELECROMAGNETICDEVICE && prjData.autoZeropole) || (circi==MAPPEDCIRCUIT && prjData.autoMappedZeropole) )
         automatic->setCheckState(Qt::Checked);
     else                            
         automatic->setCheckState(Qt::Unchecked);
}


void ZeroPoleDialog::setSijCurveNum(int n){
	zeroCurvesTW->setRowCount(n);
        for (int i = 0; i <n; ++i){
           for (int j = 0; j <2; ++j){
              QSpinBox *spb = new QSpinBox();
              spb->setMinimum(1);
	      if(selectedCircuit==MAPPEDCIRCUIT && prjData.filtermapSource==IMPORTED_RESPONSE) spb->setMaximum(2);
	      else if(selectedCircuit==IDEALCIRCUIT) spb->setMaximum(2);
	      else spb->setMaximum(prjData.ports.map.size());
              zeroCurvesTW->setCellWidget(i,j,spb);
  	   }
	}
	getPrjData();
/*
	int n1=min(n,prjData.zeropoleCurves.size()-definedpole);
        for (int i = n1; i <n; ++i){
           for (int j = 0; j <2; ++j){
              QLineEdit* fle = (QLineEdit*) zeroCurvesTW->cellWidget(i,j);
              fle->setText(QString("%1").arg(prjData.anaFreqBand[j], 0, 'f', 5));
  	   }
	}
*/
}

void ZeroPoleDialog::getPrjData(){

     if(prjData.zpFreqBand[0]==0 && prjData.zpFreqBand[1]==0){
	 prjData.zpFreqBand[0]=prjData.anaFreqBand[0];
	 prjData.zpFreqBand[1]=prjData.anaFreqBand[1];
     }
     f1LineEdit->setText(QString("%1").arg(prjData.zpFreqBand[0], 0, 'f', 5));
     f2LineEdit->setText(QString("%1").arg(prjData.zpFreqBand[1], 0, 'f', 5));
     winRatioLineEdit->setText(QString("%1").arg(prjData.zpWinRatio, 0, 'f', 5));

     std::pair<int, int> S_ports(0,0);
     int ic=0;
     for ( ZeroPoleCurvesIterator it=prjData.zeropoleCurves.begin(); it!= prjData.zeropoleCurves.end(); it++) 
      if(ic<zeroCurvesTW->rowCount()) if((*it).first!=0 && (*it).second!=0){
        for (int j = 0; j <2; ++j){
         QSpinBox *spb = (QSpinBox*) zeroCurvesTW->cellWidget(ic,j);
	 if(j==0) spb->setValue((*it).first);
	 if(j==1) spb->setValue((*it).second);
        }
        ic++;
      }
}

void ZeroPoleDialog::setInitialScale(){
     prjData.zeropoleYscaleAuto=0;
     double span=(prjData.zpFreqBand[1]-prjData.zpFreqBand[0])*prjData.zpWinRatio;
     prjData.zeropoleYscale[0]= -span/2;
     prjData.zeropoleYscale[1]=  span/2;
     prjData.zeropoleYscale[2]=  span/10;
}

void ZeroPoleDialog::set(){
     int changed=0;
     std::set< std::pair<int,int> > zeropoleCurves;

     double tmp=f1LineEdit->text().toDouble();
     if(fabs(prjData.zpFreqBand[0]-tmp)>1.e-4) { changed=true; prjData.zpFreqBand[0]=tmp; }
     tmp=f2LineEdit->text().toDouble();
     if(fabs(prjData.zpFreqBand[1]-tmp)>1.e-4) { changed=true; prjData.zpFreqBand[1]=tmp;}

     tmp=winRatioLineEdit->text().toDouble();
     if(fabs(prjData.zpWinRatio-tmp)>1.e-4) { changed=true; prjData.zpWinRatio=tmp;}
     
     std::pair<int, int> S_ports(0,0);
     zeropoleCurves.insert(S_ports);
     changed=changed || prjData.zeropoleCurves.find(S_ports)==prjData.zeropoleCurves.end();
     for (int i = 0; i <zeroCurvesTW->rowCount(); ++i){
        for (int j = 0; j <2; ++j){
            QSpinBox *spb = (QSpinBox*) zeroCurvesTW->cellWidget(i,j);
	    if(j==0) S_ports.first=spb->value();
	    if(j==1) S_ports.second=spb->value();
  	}
	if(S_ports.first==0 || S_ports.second==0) continue;
	if(S_ports.first>S_ports.second) {int tmp=S_ports.first; S_ports.first=S_ports.second; S_ports.second=tmp;}
        changed=changed || prjData.zeropoleCurves.find(S_ports)==prjData.zeropoleCurves.end();
        zeropoleCurves.insert(S_ports);
     }
     prjData.analysedCircuit = (SelectedCircuit) circuitChooser->currentIndex();
     changed=changed||(zeropoleCurves.size()!=prjData.zeropoleCurves.size());
     int autostate=(automatic->checkState()==Qt::Checked)? 1 :0;
     switch(prjData.analysedCircuit){
	 case ELECROMAGNETICDEVICE:
           if( prjData.autoZeropole!=autostate) {changed=true; prjData.autoZeropole=autostate;}
	   break;
	 case MAPPEDCIRCUIT:
           if( prjData.autoMappedZeropole!=autostate) {changed=true; prjData.autoMappedZeropole=autostate;}
	   break;
     }
     setInitialScale();
     if(changed){
	   prjData.zeropoleCurves=zeropoleCurves;
	   prjData.saveSettings();
     }
}

void ZeroPoleDialog::start(){
 set();
 setButton->setEnabled(false);
 startButton->setEnabled(false);
 closeButton->setEnabled(false);
 mainw->startTask(ZEROPOLE);
}

void ZeroPoleDialog::atZeroPoleEnd(){
 setButton->setEnabled(true);
 startButton->setEnabled(true);
 closeButton->setEnabled(true);
}

void ZeroPoleDialog::help()
{
    documentation.showDocumentation(QLatin1String("#page=46"));
}



//*************
FilterDesignDialog::FilterDesignDialog(MainWindow * parent, Qt::WindowFlags f ) : QDialog(parent, f)
{
     mainw=parent;
     setModal(0);
     setWindowTitle(tr("Ideal Filter Design"));

     QIntValidator *iterNumValidator = new QIntValidator(this);
     iterNumValidator->setBottom(1);

     QLabel *filterTypeChooserLabel= new QLabel();
     filterTypeChooserLabel->setText(tr("Filter Type:"));
     filterTypeChooser = new QComboBox();
     filterTypeChooser->addItem(tr("Chebyshev"));
     filterTypeChooser->addItem(tr("Maximally Flat"));
     filterTypeChooser->setCurrentIndex(prjData.idealFilterType);


     QLabel *filterTopologyChooserLabel= new QLabel();
     filterTopologyChooserLabel->setText(tr("Filter Topology:"));
     filterTopologyChooser = new QComboBox();
     filterTopologyChooser->addItem(tr("Transverse LC"));
     filterTopologyChooser->addItem(tr("Transverse JC"));
     filterTopologyChooser->addItem(tr("Only LC"));
     filterTopologyChooser->addItem(tr("With magic T"));
     filterTopologyChooser->setCurrentIndex(prjData.idealFilterTopology);
   
     symmResponseCB=new QCheckBox("Symmetric Response", this);
     if(prjData.symmFilterResponse) symmResponseCB->setCheckState(Qt::Checked);
     else                           symmResponseCB->setCheckState(Qt::Unchecked);

     symmetricTuningCB=new QCheckBox("Symmetric Tuning", this);
     if(prjData.filterSymmetricTuning)
         symmetricTuningCB->setCheckState(Qt::Checked);
     else                            
         symmetricTuningCB->setCheckState(Qt::Unchecked);

     tuneOnlyJtCB=new QCheckBox("Tune Only Transverse J", this);
     if(prjData.filterTuneOnlyJt)
         tuneOnlyJtCB->setCheckState(Qt::Checked);
     else                            
         tuneOnlyJtCB->setCheckState(Qt::Unchecked);

     QLabel *passBandLabel= new QLabel();
     passBandLabel->setText(tr("Pass Band:"));
     int fI=prjData.freqUnitE/3;
     switch(fI){
	  case 0: passBandLabel->setText(tr("Pass Band [Hz]:")); break;
	  case 1: passBandLabel->setText(tr("Pass Band [KHz]:")); break;
	  case 2: passBandLabel->setText(tr("Pass Band [MHz]:")); break;
	  case 3: passBandLabel->setText(tr("Pass Band [GHz]:")); break;
     }

     QLabel *bandSepLabel= new QLabel();
     bandSepLabel->setText(tr("  :  "));

     freqvalidator = new QDoubleValidator(this);
     freqvalidator->setDecimals(1000); // (standard anyway)
     freqvalidator->setNotation(QDoubleValidator::ScientificNotation);
     freqvalidator->setBottom(0);

     QIntValidator *filterOrderValidator = new QIntValidator(this);
     filterOrderValidator->setRange(2,500);

     f1LineEdit = new QLineEdit();
     f1LineEdit->setText(QString("%1").arg(prjData.filterPassBand[0], 0, 'f', 8));
     f1LineEdit->setValidator(freqvalidator);		
     f2LineEdit = new QLineEdit();
     f2LineEdit->setText(QString("%1").arg(prjData.filterPassBand[1], 0, 'f', 8));
     f2LineEdit->setValidator(freqvalidator);

     f1LineEdit->setFixedWidth(150);
     f2LineEdit->setFixedWidth(150);

     QDoubleValidator *rlvalidator = new QDoubleValidator(this);
     rlvalidator->setDecimals(1000); // (standard anyway)
     rlvalidator->setNotation(QDoubleValidator::ScientificNotation);
     rlvalidator->setBottom(0.001);

     QLabel *retLossLabel= new QLabel(); 
     retLossLabel->setText(tr("In Band Ret Loss [dB]:"));
     retLossLineEdit = new QLineEdit();
     retLossLineEdit->setText(QString("%1").arg(prjData.filterRetLoss, 0, 'g', -1));
     retLossLineEdit->setValidator(rlvalidator);		

     QLabel *outRetLossLabel= new QLabel(); 
     outRetLossLabel->setText(tr("Far Out Band Ret Loss [dB]:"));
     outRetLossLineEdit = new QLineEdit();
     outRetLossLineEdit->setText(QString("%1").arg(prjData.filterOutbandRetLoss, 0, 'g', -1));
     outRetLossLineEdit->setValidator(rlvalidator);		

     predistortedFilterCB=new QCheckBox("Predistorted Filter", this);
     if(prjData.predistortedFilter) predistortedFilterCB->setCheckState(Qt::Checked);
     else                    predistortedFilterCB->setCheckState(Qt::Unchecked);

     predistFilterOptimCB=new QCheckBox("Optimize Predistorted Filter", this);
     if(prjData.predistFilterOptim) predistFilterOptimCB->setCheckState(Qt::Checked);
     else                    predistFilterOptimCB->setCheckState(Qt::Unchecked);

     QLabel *RLossOptimLabel= new QLabel(); 
     RLossOptimLabel->setText(tr("Goal Return Loss [dB]:"));
     RLossOptimLineEdit = new QLineEdit();
     RLossOptimLineEdit->setText(QString("%1").arg(prjData.predistFilterOptimRL, 0, 'g', -1));
     RLossOptimLineEdit->setValidator(rlvalidator);		

     QLabel *ILppOptimLabel= new QLabel(); 
     ILppOptimLabel->setText(tr("Goal Insertion Loss Flatness [dBpp]:"));
     ILppOptimLineEdit = new QLineEdit();
     ILppOptimLineEdit->setText(QString("%1").arg(prjData.predistFilterOptimILpp, 0, 'g', -1));
     ILppOptimLineEdit->setValidator(rlvalidator);		


     QLabel *optimIterMaxLabel= new QLabel(); 
     optimIterMaxLabel->setText(tr("Optimization Max Iter:"));
     optimIterMaxLineEdit=new QLineEdit();
     optimIterMaxLineEdit->setText(QString("%1").arg(prjData.predistFilterOptimIterMax, 0, 'g', -1));
     QIntValidator *optimIterMaxValidator = new QIntValidator(this);
     optimIterMaxValidator->setRange(100,10000);
     optimIterMaxLineEdit->setValidator(optimIterMaxValidator);		

     QLabel *optimTrustRLabel= new QLabel(); 
     optimTrustRLabel->setText(tr("Optimization Trust Radius:"));
     optimTrustRLineEdit=new QLineEdit();
     optimTrustRLineEdit->setText(QString("%1").arg(prjData.predistFilterOptimTrustR, 0, 'g', -1));
     QDoubleValidator *optimTrustRvalidator = new QDoubleValidator(this);
     optimTrustRvalidator->setNotation(QDoubleValidator::ScientificNotation);
     optimTrustRvalidator->setRange(0.0001,0.01);
     optimTrustRLineEdit->setValidator(optimTrustRvalidator);		


     QLabel *filterOrderLabel= new QLabel();
     filterOrderLabel->setText(tr("Filter Order:"));
     filterOrderSB = new QSpinBox();
     filterOrderSB->setValue(prjData.filterOrder);
     filterOrderSB->setMinimum(0);

     QLabel *txZerosNumLabel= new QLabel(); 
     txZerosNumLabel->setText(tr("Number of Specified Tx Zeros:"));
     txZerosNumSB = new QSpinBox();
     txZerosNumSB->setMinimum(0);
     txZerosNumSB->setValue(prjData.filterZeros.size());

     txZeros= new QTableWidget(0, 1, this);
     #if defined(QT5)
      txZeros->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     #elif defined(QT4)
      txZeros->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
     #endif
     QTableWidgetItem *Zheader = new QTableWidgetItem();
     switch(fI){
	  case 0: Zheader->setText(tr("Tx Zeros [Hz]:")); break;
	  case 1: Zheader->setText(tr("Tx Zeros [KHz]:")); break;
	  case 2: Zheader->setText(tr("Tx Zeros [MHz]:")); break;
	  case 3: Zheader->setText(tr("Tx Zeros [GHz]:")); break;
     }
     txZeros->setHorizontalHeaderItem(0, Zheader);
     txZeros->setRowCount(prjData.filterZeros.size());
     for (int i = 0; i <prjData.filterZeros.size(); ++i) {
           QLineEdit* le = new QLineEdit();
	   le->setValidator(freqvalidator);
           le->setText(QString("%1").arg(prjData.filterZeros[i], 0, 'f', 8));
           txZeros->setCellWidget(i,0,le);
     }


     QLabel *portImpedanceLabel= new QLabel();
     portImpedanceLabel->setText(tr("Port Impedance [ohm]:"));
     
     QDoubleValidator *portImpedanceValidator = new QDoubleValidator(this);
     portImpedanceValidator->setDecimals(1000); // (standard anyway)
     portImpedanceValidator->setNotation(QDoubleValidator::ScientificNotation);
     portImpedanceValidator->setBottom(0.01);
     portImpedanceLineEdit = new QLineEdit();
     portImpedanceLineEdit->setText(QString("%1").arg(prjData.filterPortImpedance, 0, 'g', -1));
     portImpedanceLineEdit->setValidator(portImpedanceValidator);		


     QLabel *QfactorLabel= new QLabel();
     QfactorLabel->setText(tr("Resonators Q factor:"));
     
     QDoubleValidator *qvalidator = new QDoubleValidator(this);
     qvalidator->setDecimals(1000); // (standard anyway)
     qvalidator->setNotation(QDoubleValidator::ScientificNotation);
     qvalidator->setBottom(0);
     QfactorLineEdit = new QLineEdit();
     QfactorLineEdit->setText(QString("%1").arg(prjData.filterQfactor, 0, 'g', -1));
     QfactorLineEdit->setValidator(qvalidator);		

     inductiveSkinCB=new QCheckBox("Inductive Skin", this);
     if(prjData.filterInductiveSkin) inductiveSkinCB->setCheckState(Qt::Checked);
     else                            inductiveSkinCB->setCheckState(Qt::Unchecked);

     QGridLayout *designSettingsLayout = new QGridLayout();
     QGridLayout *bandLayout = new QGridLayout();
     QGridLayout *lossyLayout = new QGridLayout();
     QGridLayout *nLayout = new QGridLayout();
     QGridLayout *zerosLayout = new QGridLayout();
     QGridLayout *qfactorLayout = new QGridLayout();
//     nLayout->setColumnMinimumWidth(1,30);

     designSettingsLayout->addWidget(filterTypeChooserLabel, 0, 0);
     designSettingsLayout->addWidget(filterTypeChooser, 0, 1);
     designSettingsLayout->addWidget(filterTopologyChooserLabel, 1, 0);
     designSettingsLayout->addWidget(filterTopologyChooser, 1, 1);
     designSettingsLayout->addWidget(symmResponseCB, 2, 0);
     designSettingsLayout->addWidget(symmetricTuningCB, 2, 1);
     designSettingsLayout->addWidget(tuneOnlyJtCB, 2, 2);
     designSettingsLayout->addWidget(portImpedanceLabel, 3, 0);
     designSettingsLayout->addWidget(portImpedanceLineEdit, 3, 1);


     bandLayout->addWidget(passBandLabel, 0, 0);
     bandLayout->addWidget(f1LineEdit,0, 1);
     bandLayout->addWidget(bandSepLabel,0, 2);
     bandLayout->addWidget(f2LineEdit,0, 3);
     bandLayout->setRowMinimumHeight(0,25);
     bandLayout->addWidget(retLossLabel,1, 0);
     bandLayout->addWidget(retLossLineEdit,1, 1);
     bandLayout->addWidget(outRetLossLabel,1, 3);
     bandLayout->addWidget(outRetLossLineEdit,1, 4);

     lossyLayout->addWidget(RLossOptimLabel,0, 0);
     lossyLayout->addWidget(RLossOptimLineEdit,0, 1);
     lossyLayout->addWidget(ILppOptimLabel,0, 2);
     lossyLayout->addWidget(ILppOptimLineEdit,0, 3);
     lossyLayout->addWidget(optimIterMaxLabel,1, 0);
     lossyLayout->addWidget(optimIterMaxLineEdit,1, 1);
     lossyLayout->addWidget(optimTrustRLabel,1, 2);
     lossyLayout->addWidget(optimTrustRLineEdit,1, 3);

     nLayout->addWidget(filterOrderLabel, 0, 0);
     nLayout->addWidget(filterOrderSB, 0, 1);

     nLayout->addWidget(txZerosNumLabel, 1, 0);
     nLayout->addWidget(txZerosNumSB, 1, 1);

     zerosLayout->addWidget(txZeros, 0, 0);

     qfactorLayout->addWidget(QfactorLabel, 1, 0);
     qfactorLayout->addWidget(QfactorLineEdit, 1, 1);
     qfactorLayout->addWidget(inductiveSkinCB, 1, 2);
     qfactorLayout->addWidget(predistortedFilterCB, 2, 0);
     qfactorLayout->addWidget(predistFilterOptimCB, 2, 1);

     QGroupBox *designSettingsGroupBox=new QGroupBox();     
     designSettingsGroupBox->setLayout(designSettingsLayout);

     predistOptimGroupBox=new QGroupBox();
     predistOptimGroupBox->setLayout(lossyLayout);
     predistOptimGroupBox->hide();

     QGroupBox *bandGroupBox=new QGroupBox();
     bandGroupBox->setLayout(bandLayout);

     QGroupBox *nGroupBox=new QGroupBox();
     nGroupBox->setLayout(nLayout);

     QGroupBox *zerosGroupBox=new QGroupBox();
     zerosGroupBox->setLayout(zerosLayout);

     qfactorGroupBox=new QGroupBox();
     qfactorGroupBox->setLayout(qfactorLayout);

//****************************************
//   control buttons:
//
     setButton = new QPushButton(tr("Set"));
     setButton->resize(1.5,3);
     startButton =new QPushButton(tr("Start"));
     startButton->resize(1.5,3);
     closeButton =new QPushButton(tr("Close"));
     closeButton->resize(1.5,3);
     QPushButton *helpButton  = new QPushButton(tr("Help"));
     helpButton->resize(1.5,3);

     QHBoxLayout *buttonLayout = new QHBoxLayout();
     buttonLayout->addWidget(setButton);
     buttonLayout->addWidget(startButton);
     buttonLayout->addWidget(closeButton);
     buttonLayout->addWidget(helpButton);

     QGroupBox *buttonGroupBox=new QGroupBox(tr(""));
     buttonGroupBox->setLayout(buttonLayout);

     updateSymmResponse(symmResponseCB->checkState());
     updatePredistorted(predistortedFilterCB->checkState());
     updatePredistOptim(predistFilterOptimCB->checkState());
//******
     connect(symmResponseCB, SIGNAL( stateChanged (int) ), this, SLOT(updateSymmResponse(int)) );
     connect(predistortedFilterCB, SIGNAL( stateChanged (int) ), this, SLOT(updatePredistorted(int)) );
     connect(predistFilterOptimCB, SIGNAL( stateChanged (int) ), this, SLOT(updatePredistOptim(int)) );
     connect(setButton, SIGNAL(clicked()), this, SLOT(set()));
     connect(parent, SIGNAL(enterPressed()), this, SLOT(set()));
     connect(startButton,  SIGNAL(clicked()), this, SLOT(start()));
     connect(closeButton,  SIGNAL(clicked()), this, SLOT(accept()));
     connect(helpButton,  SIGNAL(clicked()), this, SLOT(help()));
     connect(filterOrderSB, SIGNAL( valueChanged (int) ), this, SLOT(atFilterOrder(int)) );
     connect(txZerosNumSB, SIGNAL( valueChanged (int) ), this, SLOT(setTxZerosNum(int)) );

     setFocusPolicy(Qt::StrongFocus);

     mainLayout = new QVBoxLayout(this);
     mainLayout->addWidget(designSettingsGroupBox);
     mainLayout->addWidget(bandGroupBox);
     mainLayout->addWidget(nGroupBox);
     mainLayout->addWidget(zerosGroupBox);
     mainLayout->addWidget(qfactorGroupBox);
     mainLayout->addWidget(predistOptimGroupBox);
     mainLayout->addWidget(buttonGroupBox);

     setFocusPolicy(Qt::StrongFocus);

}

void FilterDesignDialog::updatePredistorted(int state)
{
     if(state==Qt::Unchecked)  predistFilterOptimCB->setCheckState(Qt::Unchecked);
     predistFilterOptimCB->setEnabled(state==Qt::Checked);
}

void FilterDesignDialog::updatePredistOptim(int state)
{
     if(state==Qt::Checked){
//	   qfactorGroupBox->hide();
  	   predistOptimGroupBox->show();
//	   w=550; h=800;
     }else if(state==Qt::Unchecked){
//	   qfactorGroupBox->show();
	   predistOptimGroupBox->hide();
//           w=400; h=300;
     }
     QApplication::processEvents();
//     window()->resize(w,h);
}

void FilterDesignDialog::updateSymmResponse(int state)
{
     int n=filterOrderSB->value();
     if(state==Qt::Checked){
       txZerosNumSB->setMaximum((n-1)/2);
     }else if(state==Qt::Unchecked){
       txZerosNumSB->setMaximum(n-1);
     }
     QApplication::processEvents();
//     window()->resize(w,h);
}


void FilterDesignDialog::atFilterOrder(int n){
   int symmResponse=(symmResponseCB->checkState()==Qt::Checked) ? 1:0;
   if(symmResponse)
      txZerosNumSB->setMaximum((n-1)/2);
   else
      txZerosNumSB->setMaximum(n-1);
}

void FilterDesignDialog::setTxZerosNum(int n){
	int n0=txZeros->rowCount();
	txZeros->setRowCount(n);
        for (int i = n0; i <n; ++i){
              QLineEdit* le = new QLineEdit();
	      le->setValidator(freqvalidator);
              txZeros->setCellWidget(i,0,le);
	}
}

void FilterDesignDialog::set(){
    bool changed=false;
    if(prjData.idealFilterType!=filterTypeChooser->currentIndex()) {
	changed=true;
	prjData.idealFilterType =filterTypeChooser->currentIndex();
    }
    if(prjData.idealFilterTopology!=filterTopologyChooser->currentIndex()) {
	changed=true;
	prjData.idealFilterTopology =filterTopologyChooser->currentIndex();
    }

    double tmp;
    int symmResponse=(symmResponseCB->checkState()==Qt::Checked) ? 1:0;
    if(prjData.symmFilterResponse!=symmResponse) {prjData.symmFilterResponse=symmResponse; changed=true;}
    int predistortedFilter=(predistortedFilterCB->checkState()==Qt::Checked) ? 1:0;
    if(prjData.predistortedFilter!=predistortedFilter) {prjData.predistortedFilter=predistortedFilter; changed=true;}
    int predistFilterOptim=(predistFilterOptimCB->checkState()==Qt::Checked) ? 1:0;
    if(prjData.predistFilterOptim!=predistFilterOptim) {prjData.predistFilterOptim=predistFilterOptim; changed=true;}
    if(predistFilterOptim){
     tmp=RLossOptimLineEdit->text().toDouble();
     if(fabs(prjData.predistFilterOptimRL-tmp)>1.e-4) { prjData.predistFilterOptimRL=tmp; changed=true;}
     tmp=ILppOptimLineEdit->text().toDouble();
     if(fabs(prjData.predistFilterOptimILpp-tmp)>1.e-4) { prjData.predistFilterOptimILpp=tmp; changed=true;}
     tmp=optimTrustRLineEdit->text().toDouble();
     if(fabs(prjData.predistFilterOptimTrustR-tmp)>1.e-5) { prjData.predistFilterOptimTrustR=tmp; changed=true;}
     int itmax=optimIterMaxLineEdit->text().toInt();
     if(prjData.predistFilterOptimIterMax!=itmax) { prjData.predistFilterOptimIterMax=itmax; changed=true;}
    }
    
    int inductiveSkin=(inductiveSkinCB->checkState()==Qt::Checked) ? 1:0;
    if(prjData.filterInductiveSkin!=inductiveSkin) {prjData.filterInductiveSkin=inductiveSkin; changed=true;}

    int symmTuning=(symmetricTuningCB->checkState()==Qt::Checked)? 1 :0;
    if(prjData.filterSymmetricTuning!=symmTuning) {changed=true; prjData.filterSymmetricTuning=symmTuning;}

    int onlyJt=(tuneOnlyJtCB->checkState()==Qt::Checked)? 1 :0;
    if(prjData.filterTuneOnlyJt!=onlyJt) {changed=true; prjData.filterTuneOnlyJt=onlyJt;}

    tmp=f1LineEdit->text().toDouble();
    if(fabs(prjData.filterPassBand[0]-tmp)>1.e-7) { prjData.filterPassBand[0]=tmp; changed=true;}
    tmp=f2LineEdit->text().toDouble();
    if(fabs(prjData.filterPassBand[1]-tmp)>1.e-7) { prjData.filterPassBand[1]=tmp; changed=true;}

    tmp=retLossLineEdit->text().toDouble();
    if(fabs(prjData.filterRetLoss-tmp)>1.e-2) { prjData.filterRetLoss=tmp; changed=true;}

    tmp=outRetLossLineEdit->text().toDouble();
    if(fabs(prjData.filterOutbandRetLoss-tmp)>1.e-2) { prjData.filterOutbandRetLoss=tmp; changed=true;}

    if(filterOrderSB->value() != prjData.filterOrder){
	changed=true;
	prjData.filterOrder=filterOrderSB->value();
    }


    bool zerosChanged=false;
    if(changed) prjData.saveSettings();
    std::vector<double> filterZeros;
    for (int i = 0; i <txZerosNumSB->value(); ++i) {
       QLineEdit* le = (QLineEdit*) txZeros->cellWidget(i,0);
       filterZeros.push_back(le->text().toDouble());
    }
    if(txZerosNumSB->value()!=prjData.filterZeros.size()) zerosChanged=true;
    else for (int i = 0; i < filterZeros.size(); ++i)
              if(fabs(filterZeros[i])>0) 
	         if(fabs(1-prjData.filterZeros[i]/filterZeros[i])>1.e-8) zerosChanged=true;

    if(zerosChanged){
     prjData.filterZeros=filterZeros;
     changed=true;
    }

    tmp=portImpedanceLineEdit->text().toDouble();
    if(fabs(prjData.filterPortImpedance-tmp)>1.e-4) { prjData.filterPortImpedance=tmp; changed=true;}

    tmp=QfactorLineEdit->text().toDouble();
    if(fabs(prjData.filterQfactor-tmp)>1.e-4) { prjData.filterQfactor=tmp; changed=true;}

    if(changed) prjData.saveSettings();
}

void FilterDesignDialog::start(){
 set();
 setButton->setEnabled(false);
 startButton->setEnabled(false);
 closeButton->setEnabled(false);
 mainw->startTask(FILTERDESIGN);
}

void FilterDesignDialog::atFilterDesignEnd(){
 setButton->setEnabled(true);
 startButton->setEnabled(true);
 closeButton->setEnabled(true);
}


void FilterDesignDialog::help()
{
    documentation.showDocumentation(QLatin1String("#page=41"));
}



//*************
FilterMapDialog::FilterMapDialog(MainWindow * parent, Qt::WindowFlags f ) : QDialog(parent, f)
{
     mainw=parent;
     setModal(0);

     symmetricFilter = new QCheckBox("Symmetric Filter", this);
     if(prjData.filtermapSymmetric) symmetricFilter->setCheckState(Qt::Checked);
     else                           symmetricFilter->setCheckState(Qt::Unchecked);

     QLabel *mapSourceChooserLabel= new QLabel();
     mapSourceChooserLabel->setText(tr("Mapping Source:"));

     mapSourceChooser = new QComboBox();
     mapSourceChooser->addItem(tr("Computed zero poles"));
     mapSourceChooser->addItem(tr("Imported response"));
     mapSourceChooser->addItem(tr("Imported Mapped circuit"));
     mapSourceChooser->setCurrentIndex(prjData.filtermapSource);

     connect(mapSourceChooser, SIGNAL(currentIndexChanged(int)), this, SLOT(atSourceChanged(int)));

     QHBoxLayout *mapSourceChooserLayout = new QHBoxLayout();
     mapSourceChooserLayout->addWidget(mapSourceChooserLabel);
     mapSourceChooserLayout->addWidget(mapSourceChooser);
     QGroupBox *mapSourceChooserGroupBox=new QGroupBox();
     mapSourceChooserGroupBox->setLayout(mapSourceChooserLayout);

     QLabel *freqRangeLabel= new QLabel();
     freqRangeLabel->setText(tr("Map Frequency Range:"));
     QLabel *bandSepLabel= new QLabel();
     bandSepLabel->setText(tr("  :  "));

     QDoubleValidator *dvalidator = new QDoubleValidator(this);
     dvalidator->setDecimals(1000); // (standard anyway)
     dvalidator->setNotation(QDoubleValidator::ScientificNotation);
     f1LineEdit = new QLineEdit();
     f1LineEdit->setText(QString("%1").arg(prjData.mapFreqBand[0], 0, 'f', 5));
     f1LineEdit->setValidator(dvalidator);		
     f2LineEdit = new QLineEdit();
     f2LineEdit->setText(QString("%1").arg(prjData.mapFreqBand[1], 0, 'f', 5));
     f2LineEdit->setValidator(dvalidator);

     QHBoxLayout *freqLayout = new QHBoxLayout();
     freqLayout->addWidget(freqRangeLabel);
     freqLayout->addWidget(f1LineEdit);
     freqLayout->addWidget(bandSepLabel);
     freqLayout->addWidget(f2LineEdit);
     freqGroupBox=new QGroupBox();
     freqGroupBox->setLayout(freqLayout);

     QLabel *QfactorLabel= new QLabel();
     QfactorLabel->setText(tr("Q factor:"));
     
     QDoubleValidator *qvalidator = new QDoubleValidator(this);
     qvalidator->setDecimals(1000); // (standard anyway)
     qvalidator->setNotation(QDoubleValidator::ScientificNotation);
     qvalidator->setBottom(0);
     QfactorLineEdit = new QLineEdit();
     QfactorLineEdit->setText(QString("%1").arg(prjData.filtermapQfactor, 0, 'g', -1));
     QfactorLineEdit->setValidator(qvalidator);		

     QVBoxLayout *mapSettingsLayout = new QVBoxLayout();
     QHBoxLayout *qfactorLayout = new QHBoxLayout();
     qfactorLayout->addWidget(QfactorLabel);
     qfactorLayout->addWidget(QfactorLineEdit);
     qfactorGroupBox=new QGroupBox();
     qfactorGroupBox->setLayout(qfactorLayout);

     mapSettingsLayout->addWidget(mapSourceChooserGroupBox);
     mapSettingsLayout->addWidget(symmetricFilter);
     mapSettingsLayout->addWidget(freqGroupBox);
     mapSettingsLayout->addWidget(qfactorGroupBox);

     symmetricFilter->setEnabled(mapSourceChooser->currentIndex()==1);

     QGroupBox *mapSettingsGroupBox=new QGroupBox();
     mapSettingsGroupBox->setLayout(mapSettingsLayout);

//   Automatic
     automatic=new QCheckBox("Automatic update", this);
     QHBoxLayout *automaticLayout = new QHBoxLayout();
     automaticLayout->addWidget(automatic);
     QGroupBox *automaticGroupBox=new QGroupBox();
     automaticGroupBox->setLayout(automaticLayout);

     if(prjData.autoFilterMapping) automatic->setCheckState(Qt::Checked);
     else                          automatic->setCheckState(Qt::Unchecked);


//****************************************
//   control buttons:
//
     setButton = new QPushButton(tr("Set"));
     setButton->resize(1.5,3);
     startButton =new QPushButton(tr("Start"));
     startButton->resize(1.5,3);
     closeButton =new QPushButton(tr("Close"));
     closeButton->resize(1.5,3);
     QPushButton *helpButton  = new QPushButton(tr("Help"));
     helpButton->resize(1.5,3);

     QHBoxLayout *buttonLayout = new QHBoxLayout();
     buttonLayout->addWidget(setButton);
     buttonLayout->addWidget(startButton);
     buttonLayout->addWidget(closeButton);
     buttonLayout->addWidget(helpButton);

     QGroupBox *buttonGroupBox=new QGroupBox(tr(""));
     buttonGroupBox->setLayout(buttonLayout);

     atSourceChanged(mapSourceChooser->currentIndex());
//******
     connect(setButton, SIGNAL(clicked()), this, SLOT(set()));
     connect(parent, SIGNAL(enterPressed()), this, SLOT(set()));
     connect(startButton,  SIGNAL(clicked()), this, SLOT(start()));
     connect(closeButton,  SIGNAL(clicked()), this, SLOT(accept()));
     connect(helpButton,  SIGNAL(clicked()), this, SLOT(help()));

     setFocusPolicy(Qt::StrongFocus);

     mainLayout = new QVBoxLayout(this);
     mainLayout->addWidget(mapSettingsGroupBox);
     mainLayout->addWidget(automaticGroupBox);
     mainLayout->addWidget(buttonGroupBox);

     setFocusPolicy(Qt::StrongFocus);

}

void FilterMapDialog::atMethodChanged(int i){
}
void FilterMapDialog::atSourceChanged(int i){
     if(prjData.autoFilterMapping) automatic->setCheckState(Qt::Checked);
     else                          automatic->setCheckState(Qt::Unchecked);
     automatic->setEnabled(i!=IMPORTED_CIRCUIT);
     symmetricFilter->setEnabled(i==IMPORTED_RESPONSE);
     if(mapSourceChooser->currentIndex()==IMPORTED_CIRCUIT){
  	qfactorGroupBox->hide();
	freqGroupBox->hide();
        startButton->setEnabled(false);
     } else if (mapSourceChooser->currentIndex()==ZEROPOLES){
  	qfactorGroupBox->hide();
	freqGroupBox->hide();
        QString mappedCircuitName=prjData.mainAssName+"_RM_mapped";
        QString filtInpPath=nativePath(mainWorkPath+"/Data/Circuits/"+mappedCircuitName+".inp");
        bool hasFiltInp=FileExists(filtInpPath.toLatin1().data());
        startButton->setEnabled(hasFiltInp);
     } else if (mapSourceChooser->currentIndex()==IMPORTED_RESPONSE){
  	qfactorGroupBox->show();
	freqGroupBox->show();
        QString importedS2Ppath=nativePath(mainWorkPath+"/Data/Circuits/imported_response.s2p");
        bool hasImportedTS=FileExists(importedS2Ppath.toLatin1().data());
        startButton->setEnabled(hasImportedTS);
     }
}

void FilterMapDialog::set(){
    bool changed;
    if(prjData.filtermapSource!=mapSourceChooser->currentIndex()) {
	changed=true;
	prjData.filtermapSource = mapSourceChooser->currentIndex();
    }
    int symmetric=(symmetricFilter->checkState()==Qt::Checked)? 1 :0;
    if( prjData.filtermapSymmetric!=symmetric) {changed=true; prjData.filtermapSymmetric=symmetric;}

    double tmp=f1LineEdit->text().toDouble();
    if(fabs(prjData.mapFreqBand[0]-tmp)>1.e-4) { prjData.mapFreqBand[0]=tmp; changed=true; prjData.workStatus.remeshNeeded=1;}
    tmp=f2LineEdit->text().toDouble();
    if(fabs(prjData.mapFreqBand[1]-tmp)>1.e-4) { prjData.mapFreqBand[1]=tmp; changed=true; prjData.workStatus.remeshNeeded=1;}

    tmp=QfactorLineEdit->text().toDouble();
    if(fabs(prjData.filtermapQfactor-tmp)>1.e-4) { prjData.filtermapQfactor=tmp; changed=true;}

    int autostate=(automatic->checkState()==Qt::Checked)? 1 :0;
    if( prjData.autoFilterMapping!=autostate) {changed=true; prjData.autoFilterMapping=autostate;}
    if(changed) prjData.saveSettings();
}

void FilterMapDialog::start(){
 set();
 setButton->setEnabled(false);
 startButton->setEnabled(false);
 closeButton->setEnabled(false);
 mainw->startTask(FILTERMAP);
}

void FilterMapDialog::atFilterMapEnd(){
 setButton->setEnabled(true);
 startButton->setEnabled(mapSourceChooser->currentIndex()!=IMPORTED_CIRCUIT);
 closeButton->setEnabled(true);
}


void FilterMapDialog::help()
{
    documentation.showDocumentation(QLatin1String("#page=48"));
}


//*************
FilterDesignWithTzMapDialog::FilterDesignWithTzMapDialog(MainWindow * parent, Qt::WindowFlags f ) : QDialog(parent, f)
{
     mainw=parent;
     setModal(0);


//   Automatic
     automatic=new QCheckBox("Automatic update", this);
     QHBoxLayout *automaticLayout = new QHBoxLayout();
     automaticLayout->addWidget(automatic);
     QGroupBox *automaticGroupBox=new QGroupBox();
     automaticGroupBox->setLayout(automaticLayout);

     if(prjData.autoFilterMapping) automatic->setCheckState(Qt::Checked);
     else                          automatic->setCheckState(Qt::Unchecked);


//****************************************
//   control buttons:
//
     setButton = new QPushButton(tr("Set"));
     setButton->resize(1.5,3);
     startButton =new QPushButton(tr("Start"));
     startButton->resize(1.5,3);
     closeButton =new QPushButton(tr("Close"));
     closeButton->resize(1.5,3);
     QPushButton *helpButton  = new QPushButton(tr("Help"));
     helpButton->resize(1.5,3);

     QHBoxLayout *buttonLayout = new QHBoxLayout();
     buttonLayout->addWidget(setButton);
     buttonLayout->addWidget(startButton);
     buttonLayout->addWidget(closeButton);
     buttonLayout->addWidget(helpButton);

     QGroupBox *buttonGroupBox=new QGroupBox(tr(""));
     buttonGroupBox->setLayout(buttonLayout);

//******
     connect(setButton, SIGNAL(clicked()), this, SLOT(set()));
     connect(parent, SIGNAL(enterPressed()), this, SLOT(set()));
     connect(startButton,  SIGNAL(clicked()), this, SLOT(start()));
     connect(closeButton,  SIGNAL(clicked()), this, SLOT(accept()));
     connect(helpButton,  SIGNAL(clicked()), this, SLOT(help()));

     setFocusPolicy(Qt::StrongFocus);

     mainLayout = new QVBoxLayout(this);
     mainLayout->addWidget(automaticGroupBox);
     mainLayout->addWidget(buttonGroupBox);

     setFocusPolicy(Qt::StrongFocus);

}



void FilterDesignWithTzMapDialog::set(){
    bool changed;
    int autostate=(automatic->checkState()==Qt::Checked)? 1 :0;
    if( prjData.autoDesignWithMappedTz!=autostate) {changed=true; prjData.autoDesignWithMappedTz=autostate;}
    if(changed) prjData.saveSettings();
}


void FilterDesignWithTzMapDialog::start(){
 set();
 setButton->setEnabled(false);
 startButton->setEnabled(false);
 closeButton->setEnabled(false);
 mainw->startTask(FILTERDESIGNMAPPEDTXZ);
}

void FilterDesignWithTzMapDialog::atFilterDesignWithTzMapEnd(){
 setButton->setEnabled(true);
 startButton->setEnabled(true);
 closeButton->setEnabled(true);
}


void FilterDesignWithTzMapDialog::help()
{
    documentation.showDocumentation(QLatin1String("#1"));
}



