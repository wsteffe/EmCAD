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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <ViewerContext.h>
#include "TreeWidget.h"
#include "plotter.h"
#include <QTextEdit>
#include <QAbstractItemDelegate>
#include <QProcess>
#include <QThread>
#include <QListWidget>

#include <TPrsStd_AISViewer.hxx>

#include <set>

class QAction;
class QLabel;
class QMenu;
class QToolBar;
class Assistant;

#include <V3d_Coordinate.hxx>

#define UPDATEMATERIAL        1
#define DECOMPOSITION         2
#define MESHING               3


class MwOCAF;


int ReadFile(std::string fname, std::map<std::string, std::string> *m);

int WriteFile(std::string fname, std::map<std::string, std::string> *m);

enum modelerTask { CIRCUITS, PORTS, FREQANA, ZEROPOLE, FILTERMAP, UPDATE};


class Modeler : public QThread
{
  Q_OBJECT

  public:
    Modeler(){failure=0; }
    enum modelerTask task;
    bool failure;
    std::map<std::string, int> modeledComponent;
    std::map<std::string, int> modeledWgComponent;
    void run();
    void modelize(std::string compNameStr);
    void aws_getmodel(std::string compNameStr);
    void aws_getp(std::string compNameStr);
    void analize_ports(std::string compNameStr);
    void initModeledComponent();
    void initModeledComponentWg();
    void mainReduce();
    void freqAna();
    void zeropoleAna();
    void filterMap();
    QObject *receiver;
  signals:
    void modelerEnd();
    void freqAnaEnd();
    void zeropoleEnd();
    void filterMapEnd();
    void sendLogMessageSignal(const QString &);
    void sendStatusMessageSignal(const QString);
  private:
    void sendLogMessage ( const QString & text) {emit sendLogMessageSignal(text);}
    void sendStatusMessage ( const QString & text) {emit sendStatusMessageSignal(text);}
};




class Preprocessor : public QThread
{
  Q_OBJECT
  public:
    Preprocessor(){extproc=true; failure=0; }
    bool extproc;
    bool failure;
    int requiredOperation;
    QString  currentWorkPath;
    QString  assName;
    void run();
    void imprintSubass(QString dir, QString assName, bool toplevel, bool upwards);
    void splitComponents(QString dir, QString assName);
    void setMaterialData(QString dir);
    void setMaterialData();
    void updateMaterialData();
    void setSuperFaces(QString dir);
    void decompose();
    void meshModel(QString dir, QString assName, int assType);
    void mesh(QString dir, QString assName, int level);
    void mesh();
    void worksaveComponents(QString dir);
    void worksaveAllComponents();
    void saveMainCirc();
    QObject *receiver;
  signals:
    void decomposerEnd();
    void mesherEnd();
    void sendLogMessageSignal(const QString &);
    void sendStatusMessageSignal(const QString);
  private slots:
    void setFailure(){failure=1;};
  private:
    void sendLogMessage ( const QString & text) {emit sendLogMessageSignal(text);}
    void sendStatusMessage ( const QString & text) {emit sendStatusMessageSignal(text);}
};





class EMClogger : public QPlainTextEdit
{
Q_OBJECT
public:
 EMClogger();
 QSize sizeHint() const;
};

struct RunningStatus
{
  bool projectIsOpen;
  bool runningDecomposer;
  bool runningModeler;
  bool runningMesher;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:

    MainWindow();
    ~MainWindow();

    void showDocumentation(const QString sectionID);
    void setProjectTitle();
//    bool isComponent;
//    bool isNetwork;


    void saveMaterials();
    void checkActions();
    void startAna(modelerTask task);
    void startOperation();
    void importEMC(QString partName, QString fileName);

public slots:
    void openPart();
    void workopen(QString wkprojpath);
    void updatePartitionVolumes();
    void updateModels();
    void closeDoc();

    void temporaryMessage (const QString aMessage);
    void statusMessage (const QString aMessage);
    void atDecomposerEnd();
    void atModelerEnd();
    void atMesherEnd();
    void updateAllWidgets();
    void updateAccountCredit();

private slots:
    void open();
    void meshView();
    void responsePlot();
    void mapped_responsePlot();
    void zeropolePlot();
    void mapped_zeropolePlot();
    void accountStatus();
    void openParent();
    void closeAll();
    void importGeometry();
    void importMaterial();
    void importIdealCircuit();
    void exportMaterial();
    void exportSpiceCircuit();
    void exportMappedCircuit();
    void importDataDir();
    void importADSprj();
    void saveAs();
    void save();
    void newProject();
    void makeSubDirs();
    void setDefaultPECbc();
    void setDefaultPMCbc();
    void setGlobals();
    void setConfig();
    void print();
    void about();
    void decompose();
    void mesh();
    void reload();
    void modelize();
    void portModes();
    void freqAna();
    void zeropoleAna();
    void filterMap();
    void aboutQt();
    void xyzPosition (V3d_Coordinate X, 
       	              V3d_Coordinate Y, 
		      V3d_Coordinate Z);
    void showDocumentation();



signals:
    void enterPressed();


private:
    void openAllDoc(QString dir);
    bool worksave();
    void worksaveComponents(QString dir, bool toplevel);
    void worksaveAllComponents();
    void splitComponents(QString dir);
    void meshModel(QString dir, QString assName, int assType);
    void createActions();
    void createMenus();
    bool maybeSave();
    void setWorkProject(const QString fileName);
    void removeWorkDir();
    void clearWorkProject();
    void makeTree();
    void saveStatus();
    void propagateStatus(QString dir);
    void setEMC(QString prjDir, QString inDir);

    Modeler modeler;

    QProcess *emhelp;
//    QProcess *gmshview;

	
    QMenu *fileMenu;
    QMenu *importMenu;
    QMenu *exportMenu;
    QMenu *viewMenu;
    QMenu *editMenu;
    QMenu *analysesMenu;
    QMenu *helpMenu;

    QMenu *viewGeomMenu;
    QMenu *plotsMenu;
    QMenu *gridMenu;
    
    QMenu *setCircTypeMenu;
    QMenu *setDefaultBCMenu;
    QMenu *editVolumesMenu;
    QMenu *editMaterialsMenu;
    QMenu *editLayersMenu;

    QAction *openAction;
    QAction *openParentAction;
    QAction *openSubAssAction;
    QAction *closeAction;
    QAction *importGeometryAction;
    QAction *importMaterialAction;
    QAction *importIdealCircuitAction;
    QAction *importDataDirAction;
    QAction *importADSprjAction;
    QAction *exportMaterialAction;
    QAction *exportSpiceCircuitAction;
    QAction *exportMappedCircuitAction;
    QAction *newProjectAction;
    QActionGroup *assemblyTypeActionGroup;
    QActionGroup *defaultBC_ActionGroup;
    QAction *assemblyTypeAction;
    QAction *componentTypeAction;
    QAction *defaultPEC_Action;
    QAction *defaultPMC_Action;

    QAction *saveAsAction;
    QAction *saveAction;
    QAction *printAction;
    QAction *exitAction;
    QAction *aboutAction;
    QAction *helpAction;


    QAction *assignLayerAction;
    QAction *setCompPropertiesAction;
    QAction *defineMaterialAction;
    QAction *assignMaterialAction;
    QAction *decomposeAction;
    QAction *meshAction;
    QAction *reloadAction;
    QAction *modelizeAction;
    QAction *meshViewAction;
    QAction *responsePlotAction;
    QAction *mapped_responsePlotAction;
    QAction *zeropolePlotAction;
    QAction *mapped_zeropolePlotAction;
    QAction *accountStatusAction;
    QAction *portModesAction;
    QAction *freqAnaAction;
    QAction *zeropoleAnaAction;
    QAction *filterMapAction;
    QAction *updateAction;

    QAction *fitAction;
    QAction *fitAllAction;
    QAction *zoomAction;
    QAction *panAction;
    QAction *rotAction;
	
    QAction *gridXYAction;
    QAction *gridXZAction;
    QAction *gridYZAction;
    QAction *gridOnAction;
    QAction *gridOffAction;
    QAction *gridRectAction;
    QAction *gridCircAction;

    QAction *prjOptionsAction;
    QAction *configAction;
    QAction *showLayersAction;

    QAction *viewFrontAction;
    QAction *viewBackAction;
    QAction *viewTopAction;
    QAction *viewBottomAction;
    QAction *viewLeftAction;
    QAction *viewRightAction;
    QAction *viewAxoAction;
    QAction *viewResetAction;
    QAction *viewGridAction; /** Action for grid view */

    QAction *backgroundAction;
	
    QAction *aboutQtAction;


   // The OpenCASCADE Qt widget and context;
//   ViewerContext*  myVC; 

   QString myLastFolder;

   // Toolbars
   QAction *addPointAction;
	

   Handle(TPrsStd_AISViewer) viewer;
   TreeWidget *treeWidget;
   TreeWidgetItem *treeRoot;


   // Docking widgets
   QDockWidget *geometry;
   EMClogger  *logger;
   QDockWidget *logs;
   QLabel *statusLabel;


   //--------------------------------------------    

   public:
   Preprocessor  preproc;

   private:
   QTextEdit *project;
   QToolBar  *projectBar;
   QString    savePath;
   QString    currentWorkPath;
   QLabel    *projectLabel;
   FILE      *projectFile;

//   Plotter *plot;
   MwPlot *freqRespPlot;
   MwPlot *zeroPolePlot;
   MwPlot *mapped_freqRespPlot;
   MwPlot *mapped_zeroPolePlot;

   void keyPressEvent(QKeyEvent *event) {
        switch (event->key()) {
        case Qt::Key_Enter:
            emit(enterPressed());
	    break;
        case Qt::Key_Return:
            emit(enterPressed());
	    break;
        }
	QMainWindow::keyPressEvent(event);
   }



};

class RTextComboBox: public QComboBox{
Q_OBJECT
public:
 RTextComboBox(int w, int h) : QComboBox(){
   width=w;
   height=h;
   setFixedWidth (w);
   setFixedHeight(h);
   setIconSize(QSize(w,h-2));
 }
 void addRtItem ( const QString & text);
 private:
     int width;
     int height;
};


class SetGlobalsDialog : public QDialog
{
 Q_OBJECT

 public:
     SetGlobalsDialog(MainWindow * parent = 0, Qt::WindowFlags f = 0 );

 public slots:
      void set();
      void help();

 public:
      MainWindow * mainw;
      RTextComboBox* lUnitChooser;
      RTextComboBox* fUnitChooser;
      QLineEdit *meshSizeLineEdit;
      QLineEdit *MORFreqNumLineEdit;
      QLineEdit *f1LineEdit;
      QLineEdit *f2LineEdit;
};

class ConfigDialog : public QDialog
{
 Q_OBJECT

 public:
     ConfigDialog(MainWindow * parent = 0, Qt::WindowFlags f = 0 );
     QSize sizeHint() const;

 public slots:
      void set();
      void help();

 public:
      MainWindow * mainw;
      QLineEdit *AWS_bucket_LineEdit;
      QLineEdit *AWS_access_key_id_LineEdit;
      QLineEdit *AWS_secret_access_key_LineEdit;
      QLineEdit *EmCAD_queue_LineEdit;
      QCheckBox  *componentIsAssembly;
};

class AccountStatusDialog : public QDialog
{
 Q_OBJECT

 public:
     AccountStatusDialog(MainWindow * parent = 0, Qt::WindowFlags f = 0 );

 public slots:
      void updateCredit();
      void help();

 public:
      MainWindow * mainw;
      QLineEdit *credit_LineEdit;
};



class FreqAnaDialog : public QDialog
{
 Q_OBJECT

 public:
     FreqAnaDialog(MainWindow * parent = 0, Qt::WindowFlags f = 0 );

 public slots:
      void set();
      void start();
      void help();
      void atFreqAnaEnd();
      void updateAutomatic(int circi);
//      void updateParType(int i);

 public:
      MainWindow * mainw;
      QPushButton *setButton;
      QPushButton *startButton;
      QPushButton *closeButton;
      QLineEdit *anaFreqNumLineEdit;
      QLineEdit *MORFreqNumLineEdit;
      QComboBox  *parTypeChooser;
      QComboBox  *circuitChooser;
      QLineEdit *f1LineEdit;
      QLineEdit *f2LineEdit;
      QVBoxLayout *mainLayout;
      QCheckBox *automatic;
};

class ZeroPoleDialog : public QDialog
{
 Q_OBJECT

 public:
     ZeroPoleDialog(MainWindow * parent = 0, Qt::WindowFlags f = 0 );

      void setInitialScale();
 public slots:
      void set();
      void getPrjData();
      void start();
      void help();
      void atZeroPoleEnd();
      void setSijCurveNum(int n);
      void updateAutomatic(int circi);

 public:
      MainWindow * mainw;
      QVBoxLayout *mainLayout;
      QLineEdit *f1LineEdit;
      QLineEdit *f2LineEdit;
      QSpinBox   *SijCurveNumSB;
      QTableWidget *zeroCurvesTW;
      QComboBox  *circuitChooser;
      QPushButton *setButton;
      QPushButton *startButton;
      QPushButton *closeButton;
      QCheckBox *automatic;
 private:
      QDoubleValidator *freqvalidator;


};


class FilterMapDialog : public QDialog
{
 Q_OBJECT

 public:
     FilterMapDialog(MainWindow * parent = 0, Qt::WindowFlags f = 0 );

 public slots:
      void set();
      void start();
      void help();
      void atMethodChanged(int);
      void atFilterMapEnd();
//      void updateParType(int i);

 public:
      MainWindow * mainw;
      QComboBox  *mapMethodChooser;
      QPushButton *setButton;
      QPushButton *startButton;
      QPushButton *closeButton;
      QLineEdit *iterNumLineEdit;
      QVBoxLayout *mainLayout;
      QCheckBox *automatic;
};


/*
class RTextItemDelegate: public QAbstractItemDelegate{
Q_OBJECT
public:
 RTextItemDelegate(QObject *parent) : QAbstractItemDelegate(parent){hintSize=22;}	
 QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index ) const;
 void paint ( QPainter * p, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
// void drawDisplay  ( QPainter * p, const QStyleOptionViewItem & option, const QString & text ) const;
 private:
     int hintSize;
};

class RTextComboBox: public QComboBox{
Q_OBJECT
public:
 RTextComboBox(int size);
 void paintEvent( QPaintEvent* pEvent);
 int computeWidthHint();
 private:
     RTextItemDelegate *rtd;
     int fixedSize;
     QListWidget *popv;
};
*/

#endif
