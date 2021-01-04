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


#ifndef _SELECTEDCIRCUIT_H_
#define _SELECTEDCIRCUIT_H_
enum SelectedCircuit {ELECROMAGNETICDEVICE=0, MAPPEDCIRCUIT=1, IDEALCIRCUIT=2, IMPORTEDCIRCUIT=3, IDEALCIRCUITMAPPEDTZ=4, IMPORTEDRESPONSE=5};
#endif

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

enum modelerTask { CIRCUITS, PORTS, FREQANA, ZEROPOLE, FILTERDESIGN, FILTERDESIGNMAPPEDTXZ, FILTERMAP, UPDATE};


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
    void api_get_component_results(modelerTask operation, std::string compNameStr);
    void server_secrets(int put);
    void analize_ports(std::string compNameStr);
    void initModeledComponent();
    void initModeledComponentWg();
    void mainReduce();
    void freqAna();
    void zeropoleAna();
    void filterMap();
    void filterDesign(bool mappedTxZeros=false);
    void idealFilterTune(QString targetCkt, QString tunedCkt);
    QObject *receiver;
  
  signals:
    void modelerEnd();
    void freqAnaEnd();
    void zeropoleEnd();
    void filterDesignEnd();
    void filterMapEnd();
    void idealFilterTuneEnd();
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
    Preprocessor(){extproc=true; decompose_failure=0; mesh_failure=0;}
    bool extproc;
    bool decompose_failure;
    bool mesh_failure;
    bool failure(){return decompose_failure || mesh_failure; }
    int requiredOperation;
    QString  currentWorkPath;
    QString  assName;
    void run();
    int maxDepth(QString dir, int l=0);
    void upImprintSubass(QString dir, QString assName, int l=0);
    void downImprintSubass(QString dir, QString assName, int l=0);
    void splitComponents(QString dir, QString assName);
    void setMaterialData(QString dir);
    void setMaterialData();
    void updateMaterialData();
    void setSuperFaces(QString dir);
    void decompose();
    void meshModel(QString dir, QString assPath, int assType);
    void mesh(QString dir, QString assPath, int level, int l=0);
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

class FilterTuner: public QThread
{
 Q_OBJECT

 public:
      FilterTuner();
      void run();
      int check();
      bool failure;
      QObject *receiver;

  signals:
    void tunerEnd();

  signals:
    void sendLogMessageSignal(const QString &);
    void sendStatusMessageSignal(const QString);
  private:
    void sendLogMessage ( const QString & text) {emit sendLogMessageSignal(text);}
    void sendStatusMessage ( const QString & text) {emit sendStatusMessageSignal(text);}

};

class FilterTunerDialog : public QDialog
{
 Q_OBJECT

 public:
     FilterTunerDialog(FilterTuner *t, QWidget * p=0, Qt::WindowFlags f=0 );
     void init();

 public slots:
      void set();
      void start();
      void help();
      void atMethodChanged(int);
      void atFilterTuneEnd();
      void varFileChooser();
//      void updateParType(int i);

 public:
      FilterTuner *tuner;
      EMClogger  *logger;
      QWidget * parent;
      QComboBox  *tuneMethodChooser;
      QPushButton *setButton;
      QPushButton *startButton;
      QPushButton *closeButton;
      QLineEdit *varFileLineEdit;
      QLineEdit *maxIterLineEdit;
      QLineEdit *xTolLineEdit;
      QLineEdit *trustRadiusLineEdit;
      QLineEdit *gradDxLineEdit;
      QVBoxLayout *mainLayout;
      QCheckBox *recomputeError;
      QCheckBox *recomputeJaco;
      void run();
      int check();
      bool failure;

};



struct RunningStatus
{
  bool projectIsOpen;
  bool runningDecomposer;
  bool runningModeler;
  bool runningMesher;
  bool runningTuner;
};

class Documentation : public QObject
{
    Q_OBJECT
    public:
    void showDocumentation(const QString sectionID);
    public slots:
    void showDocumentation();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
    MainWindow();
    ~MainWindow();

    void setProjectTitle();
//    bool isComponent;
//    bool isNetwork;


    void saveMaterials();
    void checkActions();
    void startTask(modelerTask task);
    void startOperation();
    void importEMC(QString partName, QString fileName);

public slots:
    void openComp();
    void openCompAndPartition();
    void openCompOrPartition();
    void workopen(QString wkprojpath, int subcomp=0);
    void updateModels();
    void closeDoc();

    void temporaryMessage (const QString aMessage);
    void statusMessage (const QString aMessage);
    void atDecomposerEnd();
    void atModelerEnd();
    void atTunerEnd();
    void atMesherEnd();
    void updateAllWidgets();
    void updateAccountCredit();

private slots:
    void open();
    void meshView();
    void responsePlot();
    void ideal_responsePlot();
    void imported_responsePlot();
    void importedcircuit_responsePlot();
    void ideal_mappedTZ_responsePlot();
    void mapped_responsePlot();
    void zeropolePlot();
    void ideal_zeropolePlot();
    void mapped_zeropolePlot();
    void accountStatus();
    void closeComp();
    void closeCompAndPartition();
    void closeCompOrPartition();
    void closeAll();
    void importGeometry();
    void importMaterial();
    void importIdealFilterCircuit();
    void importIdealFilterPar();
    void importFilterCircuit();
    void importFilterPar();
    void importS2P();
    void exportMaterial();
    void exportSpice();
    void exportIdealResponse();
    void exportFreqResponse();
    void exportMappedResponse();
    void exportMappedJC();
    void exportMappedSpice();
    void exportIdealJC();
    void exportIdealSpice();
    void exportIdealMappedTzJC();
    void exportIdealMappedTzSpice();
    void exportFilterTunePar();
    void importDataDir();
    void importADSprj();
    void saveAs();
    void save();
    void newProject();
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
    void filterTune();
    void filterDesign();
    void filterDesignWithTzMap();
    void aboutQt();
    void xyzPosition (V3d_Coordinate X, 
       	              V3d_Coordinate Y, 
		      V3d_Coordinate Z);



signals:
    void enterPressed();


public:
    void worksaveAllComponents();
    void recursiveAssignDefaultMaterial();

private:
    void openAllDoc(QString dir);
    bool worksave();
    void worksaveComponents(QString dir, bool toplevel);
    void recursiveAssignDefaultMaterial(QString dir, bool toplevel);
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
    FilterTuner tuner;
    public:

    QAction *openCompAndPartitionAction;
    QAction *openCompOrPartitionAction;
    
    private:
    QProcess *emhelp;
//    QProcess *gmshview;

	
    QMenu *fileMenu;
    QMenu *importMenu;
    QMenu *exportMenu;
    QMenu *viewMenu;
    QMenu *editMenu;
    QMenu *analysesMenu;
    QMenu *designsMenu;
    QMenu *helpMenu;

    QMenu *viewGeomMenu;
    QMenu *plotsMenu;
    QMenu *plotFreqResponseMenu;
    QMenu *plotZeroPoleMenu;
    QMenu *gridMenu;
    
    QMenu *setCircTypeMenu;
    QMenu *setDefaultBCMenu;
    QMenu *editVolumesMenu;
    QMenu *editMaterialsMenu;
    QMenu *editLayersMenu;

    QAction *openAction;
    QAction *closeCompAndPartitionAction;
    QAction *closeCompOrPartitionAction;
    QAction *closeAction;
    QAction *importGeometryAction;
    QAction *importMaterialAction;
    QAction *importIdealFilterCircuitAction;
    QAction *importIdealFilterParAction;
    QAction *importFilterCircuitAction;
    QAction *importFilterParAction;
    QAction *importS2P_Action;
    QAction *importDataDirAction;
    QAction *importADSprjAction;
    QAction *exportMaterialAction;
    QAction *exportSpiceAction;
    QAction *exportIdealResponseAction;
    QAction *exportFreqResponseAction;
    QAction *exportMappedResponseAction;
    QAction *exportMappedJCAction;
    QAction *exportMappedSpiceAction;
    QAction *exportIdealJCAction;
    QAction *exportIdealSpiceAction;
    QAction *exportJC_IdealMappedTZ_Action;
    QAction *exportSpice_IdealMappedTZ_Action;
    QAction *exportFilterTuneParAction;
    QAction *newProjectAction;
    QActionGroup *assemblyTypeActionGroup;
    QAction *assemblyTypeAction;
    QAction *componentTypeAction;
    QAction *defaultBC_Action;

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
    QAction *ideal_responsePlotAction;
    QAction *imported_responsePlotAction;
    QAction *importedcircuit_responsePlotAction;
    QAction *ideal_mappedTZ_responsePlotAction;
    QAction *zeropolePlotAction;
    QAction *mapped_zeropolePlotAction;
    QAction *ideal_zeropolePlotAction;
    QAction *accountStatusAction;
    QAction *portModesAction;
    QAction *freqAnaAction;
    QAction *zeropoleAnaAction;
    QAction *filterMapAction;
    QAction *filterTuneAction;
    QAction *filterDesignAction;
    QAction *filterDesignWithTzMapAction;
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
	

   Handle(TPrsStd_AISViewer) theAISViewer;
   TreeWidget *treeWidget;
   TreeWidgetItem *treeRoot;


   // Docking widgets
   QDockWidget *geometry;
   QDockWidget *logs;
   QLabel *statusLabel;


   //--------------------------------------------    

   public:
   EMClogger  *logger;
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
   MwPlot *ideal_freqRespPlot;
   MwPlot *ideal_zeroPolePlot;
   MwPlot *ideal_mappedTZ_freqRespPlot;
   MwPlot *imported_freqRespPlot;

   public:

   private:
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
      QCheckBox  *localMeshing3dCB;
      QLineEdit *meshSizeLineEdit;
      QLineEdit *meshPerCircleLineEdit;
      QLineEdit *meshRefineMaxNumLineEdit;
      QLineEdit *meshMinEnergyRatioLineEdit;
      QLineEdit *MORFreqNumLineEdit;
      QLineEdit *f1LineEdit;
      QLineEdit *f2LineEdit;
      QLineEdit *rf1LineEdit;
      QLineEdit *rf2LineEdit;
      QLineEdit *cutoffLineEdit;
      QLineEdit *krylovLineEdit;
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
      void onCircuitChanged(int circi);
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
      SelectedCircuit selectedCircuit;
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
      void onCircuitChanged(int circi);

 public:
      MainWindow * mainw;
      QVBoxLayout *mainLayout;
      QLineEdit *f1LineEdit;
      QLineEdit *f2LineEdit;
      QLineEdit *winRatioLineEdit;
      QSpinBox   *SijCurveNumSB;
      QTableWidget *zeroCurvesTW;
      QComboBox  *circuitChooser;
      QPushButton *setButton;
      QPushButton *startButton;
      QPushButton *closeButton;
      QCheckBox *automatic;
      SelectedCircuit selectedCircuit;
 private:


};

class IdealFilterDialog : public QDialog
{
 Q_OBJECT

 public:
     IdealFilterDialog(MainWindow * parent = 0, Qt::WindowFlags f = 0 );

 public slots:
       void set();
       void getPrjData();
     void help();
     void setJnum(int n);
     void setCKnum(int n);
     void setLKnum(int n);
     void setGnum(int n);

 public:
     QSpinBox   *JnumSB;
     QSpinBox   *CKnumSB;
     QSpinBox   *LKnumSB;
     QSpinBox   *GnumSB;
     QTableWidget *resonFreqTW;
     QTableWidget *impedanceTW;
     QTableWidget *JinvTW;
     QTableWidget *CKTW;
     QTableWidget *LKTW;
     QTableWidget *GTW;
     QGroupBox *resonFreqBox;
     QGroupBox *impedanceBox;
     QGroupBox *JBox;
     QGroupBox *CKBox;
     QGroupBox *LKBox;
     QGroupBox *GBox;

     QVBoxLayout *mainLayout;

     QPushButton *setButton;
     QPushButton *closeButton;
     
     void setResonNum(int n);
};


class FilterDesignDialog : public QDialog
{
 Q_OBJECT

 public:
     FilterDesignDialog(MainWindow * parent = 0, Qt::WindowFlags f = 0 );

 public slots:
      void set();
      void start();
      void help();
      void atFilterOrder(int n);
      void setTxZerosNum(int n);
      void atFilterDesignEnd();
      void updateSymmResponse(int state);
      void updatePredistOptim(int state);
      void updatePredistorted(int state);
      void updateCustomIdealFilter(int state);
      void setIdealFilter();
//      void updateParType(int i);

 public:
      QDoubleValidator *freqvalidator;
      QComboBox  *filterTypeChooser;
      QComboBox  *filterTopologyChooser;
      QCheckBox *symmResponseCB;
      QCheckBox *customIdealFilterCB;
      QCheckBox *predistortedFilterCB;
      QCheckBox *predistFilterOptimCB;
      QCheckBox *symmetricTuningCB;
      QCheckBox *tuneOnlyJtCB;
      QSpinBox *filterOrderSB;
      QLineEdit *retLossLineEdit;
      QLineEdit *outRetLossLineEdit;
      QLineEdit *RLossOptimLineEdit;
      QLineEdit *ILppOptimLineEdit;
      QLineEdit *optimIterMaxLineEdit;
      QLineEdit *optimTrustRLineEdit;
      QLineEdit *portImpedanceLineEdit;
      QLineEdit *QfactorLineEdit;
      QLineEdit *targetQfactorLineEdit;
      QCheckBox *inductiveSkinCB;
      QSpinBox   *txZerosNumSB;
      QLineEdit *f1LineEdit;
      QLineEdit *f2LineEdit;
      QTableWidget *txZeros;
      MainWindow * mainw;
      QPushButton *setIdealCircButton;
      QPushButton *setButton;
      QPushButton *startButton;
      QPushButton *closeButton;
      QLineEdit *iterNumLineEdit;
      QVBoxLayout *mainLayout;
      QGroupBox *predistOptimGroupBox;
      QGroupBox *qfactorGroupBox;
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
      void atSourceChanged(int);
      void atFilterMapEnd();
//      void updateParType(int i);

 public:
      MainWindow * mainw;
      QCheckBox  *symmetricFilter;
      QComboBox  *mapSourceChooser;
      QLineEdit *f1LineEdit;
      QLineEdit *f2LineEdit;
      QLineEdit *QfactorLineEdit;
      QGroupBox *freqGroupBox;
      QGroupBox *qfactorGroupBox;
      QPushButton *setButton;
      QPushButton *startButton;
      QPushButton *closeButton;
      QVBoxLayout *mainLayout;
      QCheckBox *automatic;
};



class FilterDesignWithTzMapDialog : public QDialog
{
 Q_OBJECT

 public:
     FilterDesignWithTzMapDialog(MainWindow * parent = 0, Qt::WindowFlags f = 0 );

 public slots:
      void set();
      void start();
      void help();
      void atFilterDesignWithTzMapEnd();
//      void updateParType(int i);

 public:
      MainWindow * mainw;
      QPushButton *setButton;
      QPushButton *startButton;
      QPushButton *closeButton;
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
