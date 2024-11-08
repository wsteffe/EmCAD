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


#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#define OCAF_LABEL      1
#define DB_MATERIAL     2

#if defined(HAVE_OCE_CONFIG_H)
 #include <oce-config.h>
#elif defined(HAVE_OCC_CONFIG_H)
 #include <config.h>
#endif

#include <TDF_Label.hxx>
#include <QTreeWidget>
#include <QAction>
#include <AIS_InteractiveContext.hxx>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QShortcut>
#include <QGroupBox>
#include <QTableWidget>
#include <QSpinBox>
#include <QCheckBox>
#include <QDoubleValidator>
#include <QComboBox>
#include <QFrame>
#include <QFileDialog>
#include <QPlainTextEdit>

#include "DataBase.h"
#include <map>

class TreeWidgetItem : public QTreeWidgetItem 
{
   public:
     int index;
     TreeWidgetItem ( int type )                             : QTreeWidgetItem ( type )        {index=0;}
     TreeWidgetItem ( const QStringList & strings, int type) : QTreeWidgetItem (strings, type) {index=0;}
     TreeWidgetItem ( QTreeWidget * parent, int type)        : QTreeWidgetItem (parent, type ) {index=0;}
     TreeWidgetItem ( QTreeWidget * parent)                  : QTreeWidgetItem (parent)        {index=0;}
     TreeWidgetItem ( TreeWidgetItem * parent)               : QTreeWidgetItem (parent)        {index=0;}
     TreeWidgetItem ( const TreeWidgetItem & other )         : QTreeWidgetItem ( other )       {index=0;}
     bool getLabel(TDF_Label &label);
     int  dataType();
     bool getMaterial(DB::Material* &mat);
     void displayShape(bool B);
     void displayMesh(bool B);
     void setCheck(int column );
};



class TreeWidget : public QTreeWidget
{
   Q_OBJECT

   public:
     TreeWidget();
     ~TreeWidget();
     void makeSubTree(TreeWidgetItem * qroot, TDF_Label root);
     void makeMaterials(TreeWidgetItem * troot);
     void updateText();
     void updateIndex();
     void clear();
     void setItem(TreeWidgetItem * qitem, TDF_Label label);
     void setItemText(TreeWidgetItem * item);
     void setItemLink(TreeWidgetItem * item, TDF_Label label);
     void setItemLink(TreeWidgetItem * item, DB::Material *mat);
     bool getLabel(TreeWidgetItem * item, TDF_Label &label);
     bool getMaterial(TreeWidgetItem * item, DB::Material* &mat);
     TreeWidgetItem* getItemFromLabel(TDF_Label label);
     TreeWidgetItem* getItemFromMaterial(DB::Material* mat);

   public:
      int isLastClicked(QTreeWidgetItem * qitem) {return(lastClicked==((TreeWidgetItem*)qitem));}
      int isLastPressed(QTreeWidgetItem * qitem) {return(lastPressed==((TreeWidgetItem*)qitem));}
      int isLastEntered(QTreeWidgetItem * qitem) {return(lastEntered==((TreeWidgetItem*)qitem));}

   protected:
     void contextMenuEvent(QContextMenuEvent *event);
//     void mousePressEvent( QMouseEvent * event );
     void leaveEvent ( QEvent * event );

   public slots:
      void setLastClicked(QTreeWidgetItem * qitem, int column ) {lastClicked=(TreeWidgetItem*) qitem;}
      void setLastPressed(QTreeWidgetItem * qitem, int column ) {lastPressed=(TreeWidgetItem*) qitem;}
      void setLastEntered(QTreeWidgetItem * qitem, int column ) {lastEntered=(TreeWidgetItem*) qitem;}
      void about();
      void setCurrentItem(QTreeWidgetItem * qitem, int column );
      void openComp();
      void viewSubCompConvergence();
      void atImportedPartProperties();
      void assignLayer();
      void assignMaterial();
      void setDefaultBC();
      void assignLayerDialog();
      void setCompPropertiesDialog();
      void importCompPropertiesDialog();
      void defineMaterialDialog();
      void assignMaterialDialog();
      void setDefaultBCdialog();
      void showWgModes();
      void displaySelectedMesh();
      void hideSelectedMesh();
      void highlightItem (QTreeWidgetItem * qitem, int column );
      void openItem (QTreeWidgetItem * qitem );
      void closeItem (QTreeWidgetItem * qitem );
      void displayItemShape(QTreeWidgetItem * qitem );
      void displayItemMesh(QTreeWidgetItem * qitem, bool B );
      void unhighlightCurrentItem ();
      void setCheck(QTreeWidgetItem * item, int column );
      void getOcafSelection();
      void showLayers(bool);
//      void regainFocus();

   signals:
      void sendPartName(QString text);
      void sendMaterialName(QString text);
      void sendLayerName(QString text);
      void AISchanged();
      void highlightObject(Handle_AIS_InteractiveObject iobj);
      void enterPressed();
//      void itemEntered(QTreeWidgetItem,int);

   public:
      TreeWidgetItem * currentItem;
      TreeWidgetItem * currentPart;
      TreeWidgetItem * currentMaterial;
      TreeWidgetItem * currentLayer;

      QString     currentPartName;
      QString     currentMaterialName;
      QString     currentLayerName;
      int         currentSubComp;

   private:
      QAction *aboutAction;
      QAction *openComponentAction;
      QAction *viewSubCompConvergenceAction;
      QAction *assignLayerAction;
      QAction *assignMaterialAction;
      QAction *showWgModesAction;
      QAction *setCompPropertiesAction;
      QAction *importCompPropertiesAction;
      QAction *showMeshAction;
      QAction *hideMeshAction;

      TreeWidgetItem * lastClicked;
      TreeWidgetItem * lastPressed;
      TreeWidgetItem * lastEntered;

    // The OpenCASCADE Qt widget and context;
      TreeWidgetItem**   indexedItem;

   public:
      int  maxIndex;
      bool showlayers;

//      QShortcut *enter;

    void keyPressEvent(QKeyEvent *event) {
        switch (event->key()) {
        case Qt::Key_Enter:
            emit(enterPressed());
	    break;
        case Qt::Key_Return:
            emit(enterPressed());
	    break;
        }
	QTreeWidget::keyPressEvent(event);
    }
};

struct WidgetItemData
 {
     int type;
     TDF_Label docLabel;
     void *data;
 };


Q_DECLARE_METATYPE(WidgetItemData)


class AssignDialog : public QDialog
{
 Q_OBJECT

 public:
     AssignDialog(TreeWidget *parent = 0);

 signals:
     void assigned();

 public slots:
//    void accept();
      void assign(){emit assigned();}

 public:
      QLabel  *label1;
      QLineEdit *lineEdit1;
      QLabel  *label2;
      QLineEdit *lineEdit2;
      QPushButton *assignButton;
      QPushButton *closeButton;
      QHBoxLayout *layout;

};


class SetDefaultBCDialog : public QDialog
{
 Q_OBJECT

 public:
     SetDefaultBCDialog(TreeWidget *parent = 0);

 signals:
     void assigned();

 public slots:
//    void accept();
      void assign(){emit assigned();}

 public:
      QLabel  *label2;
      QLineEdit *lineEdit2;
      QPushButton *assignButton;
      QPushButton *closeButton;
      QHBoxLayout *layout;

};

class SetCompPropertiesDialog : public QDialog
{
 Q_OBJECT
 public:
     SetCompPropertiesDialog(TreeWidget *parent = 0);
 
 public slots:
     void set();
     void help();
     void getVolumeData(QString matname);
     void setVolumeData(DB::Volume* vol);
     void updateType(int i);

 public:
     int w,h;
     TreeWidget *treeWidget;
     QDoubleValidator *positiveDoubleValidator;
     QLineEdit  *nameLineEdit;
     QLineEdit  *meshRefLineEdit;
     QLineEdit  *cutoffRefLineEdit;
     QSpinBox   *gridNumSB;
     QLineEdit  *typeChooser;
     QLineEdit  *LinePortZcLineEdit;
     std::map<int, std::string> typeChooserMap;
     QLineEdit *TEMnumLE;
     QSpinBox  *TEnumSB;
     QSpinBox  *TMnumSB;
     QComboBox *TEM_TabularOrder;
     QCheckBox *PML;
#if defined(EXPLICIT_INVARIANT)
     QCheckBox *invariant;
#endif
     QGroupBox *nameGroupBox;
     QGroupBox *WgGroupBox;
     QGroupBox *LPGroupBox;
     QGroupBox *GridGroupBox;
     QGroupBox *buttonGroupBox;
     QGroupBox *meshGroupBox;
     QGroupBox *cutoffGroupBox;
     QVBoxLayout *mainLayout;
private:
     std::string LPname;

};

class ImportCompPropertiesDialog : public QDialog
{
 Q_OBJECT
 public:
     ImportCompPropertiesDialog(TreeWidget *parent = 0);
 
 public slots:
     void importEMC(const QString & name);
     void help();
     void getVolumeData(QString matname);

 public:
     int w,h;
     QString  fileName;
     QFileDialog *fdialog;
     TreeWidget *treeWidget;
     QLineEdit  *nameLineEdit;
     QGroupBox *nameGroupBox;
     QGroupBox *fileGroupBox;
     QGroupBox *buttonGroupBox;
     QVBoxLayout *mainLayout;
     QSize sizeHint() const;
};

class DefineMaterialDialog : public QDialog
{
 Q_OBJECT

 public:
     DefineMaterialDialog(TreeWidget *parent = 0);


 public slots:
//    void accept();
      void set();
      void del();
      void help();
      void getMaterialData(QString matname);
      void setMaterialData(DB::Material* mat);
      void setEpsTermsNum(int n);
      void setMuTermsNum(int n);
      void setSurfTermsNum(int n);
      void setColorDialog();
      void updateETanDelta(int state);
      void updateHTanDelta(int state);
      void updateDisp(int state);
      void statusMessage (const QString aMessage);

 public:

      int w,h;
      QDoubleValidator *dvalidator;
      QDoubleValidator *roughQvalidator;
      QDoubleValidator *lossFacValidator;
      QDoubleValidator *roughBallRadiusValidator;
      QDoubleValidator *roughSurfRatioValidator;
      QDoubleValidator *roughSurfRzValidator;
      QDoubleValidator *roughSurfRqValidator;

      TreeWidget *treeWidget;

      QComboBox *roughModelChooser;
      QGroupBox *emGroupBox;
      QGroupBox *eDispGroupBox;
      QGroupBox *mDispGroupBox;
      QGroupBox *sRoughChooserGroupBox;
      QGroupBox *sRoughHurayGroupBox;
      QGroupBox *sRoughFactorsGroupBox;
      QGroupBox *sRoughFittingGroupBox;
      QGroupBox *sRoughRzGroupBox;
      QGroupBox *sRoughRqGroupBox;
      QGroupBox *sPolesGroupBox;
      QGroupBox *colorGroupBox;
      QGroupBox *buttonGroupBox;
      QVBoxLayout *mainLayout;

      QLineEdit  *nameLineEdit;
      QLineEdit  *epsLineEdit;
      QSpinBox   *epsTermsNumSB;
      QSpinBox   *muTermsNumSB;
      QSpinBox   *surfTermsNumSB;
      QLineEdit  *sigmaLineEdit;
      QLineEdit  *muLineEdit;
      QLineEdit  *sigmamLineEdit;
      QLineEdit  *roughFreqLineEdit;
      QLineEdit  *roughLossFactorLineEdit;
      QLineEdit  *roughQFactorLineEdit;
      QLineEdit  *roughBallRadiusLineEdit;
      QLineEdit  *roughSurfRatioLineEdit;
      QLineEdit  *roughSurfRzLineEdit;
      QLineEdit  *roughSurfRqLineEdit;
      QSpinBox   *roughFitPolesNumSB;

      QCheckBox  *dispersive;
      QCheckBox  *eTanDelta;
      QCheckBox  *hTanDelta;
      QCheckBox  *boundaryModel;
      QCheckBox  *roughSurfModel;
      QFrame    *colorFrame;
      QPalette    colorpal;
//      QLineEdit  *epsNumLineEdit;
//      QLineEdit  *muNumLineEdit;
/*
      QLineEdit  *epsInfLineEdit;
      QLineEdit  *epsFrLineEdit;
      QLineEdit  *epsF0LineEdit;
      QLineEdit  *muInfLineEdit;
      QLineEdit  *muFrLineEdit;
      QLineEdit  *muF0LineEdit;
*/
      QLineEdit *eTanDeltaLineEdit;
      QLineEdit *hTanDeltaLineEdit;

      QTableWidget *epsLorentz;
      QTableWidget *muLorentz;
      QTableWidget *surfPolesRes;
      QColor color;

      QLineEdit  *SresLineEdit;
      QLineEdit  *SindLineEdit;

      void setConsTanDeltaLorentz(char type, DB::Material* mat);
      void setSurfHurayLorentz(DB::Material* mat);

      QLabel *statusLabel;

};

class ShowWgModesDialog : public QDialog
{
 Q_OBJECT

 public:
   ShowWgModesDialog (TreeWidget *parent = 0);
   QPlainTextEdit *textEdit;
   QPushButton *closeButton;
   QVBoxLayout *layout;
};

#endif
