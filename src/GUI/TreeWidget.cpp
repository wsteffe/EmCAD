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

#include "mainwindow.h"
#include <QApplication>
#include "projectData.h"
#include "TreeWidget.h"
#include "ViewWidget.h"
#include "ocaf.h"
//#include "Tools.h"


#include <QTreeWidget>
#include <QTreeWidgetItemIterator>
#include <QVariant>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QColorDialog>
#include <QPalette>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QSizePolicy>

#include <TDF_ChildIterator.hxx>

#include <QFocusEvent>

extern ViewWidget*   mainOCC;
extern MwOCAF*       mainOCAF;
extern MainWindow *mainWindow;
extern ProjectData prjData;
extern QString    mainWorkPath;

#ifndef PIG
#define PIG 3.14159265
#endif


/*
void TreeWidgetItem::setExpanded ( bool expand ) 
{
 QTreeWidgetItem::setExpanded ( expand );
 TDF_Label label; if(getLabel(label)) openLabel(label);
}
*/

TreeWidget::TreeWidget(){

      showlayers=false;

      currentItem=NULL;
      lastClicked=NULL;
      lastPressed=NULL;
      lastEntered=NULL;

//      setFocusPolicy(Qt::StrongFocus);

      setColumnCount(1);
      setHeaderLabel("");
      maxIndex=0;
      indexedItem =(TreeWidgetItem**) NULL;

      aboutAction = new QAction(tr("&About"), this);
      aboutAction->setStatusTip(tr("Show the application's About box"));
      connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
//      connect(aboutAction, SIGNAL(triggered()), this, SIGNAL(buttonClicked()));

      openSubAssAction = new QAction(tr("&Open subassembly"), this);
      connect(openSubAssAction, SIGNAL(triggered()), this, SLOT(openSubAssembly()));

      assignLayerAction = new QAction(tr("&Set Layer"), this);
      assignLayerAction->setStatusTip(tr("Assign a Layer to the Selected Part "));
      connect(assignLayerAction, SIGNAL(triggered()), this, SLOT(assignLayerDialog()));

      assignMaterialAction = new QAction(tr("&Set Material"), this);
      assignMaterialAction->setStatusTip(tr("Assign a Material to the Selected Part "));
      connect(assignMaterialAction, SIGNAL(triggered()), this, SLOT(assignMaterialDialog()));

      importPartPropertiesAction = new QAction(tr("&Import part properties"), this);
      importPartPropertiesAction->setStatusTip(tr("Import properties of the Selected Part "));
      connect(importPartPropertiesAction, SIGNAL(triggered()), this, SLOT(importPartPropertiesDialog()));

      setCompPropertiesAction = new QAction(tr("&Set part properties"), this);
      setCompPropertiesAction->setStatusTip(tr("Set properties of the Selected Part "));
      connect(setCompPropertiesAction, SIGNAL(triggered()), this, SLOT(setCompPropertiesDialog()));

      showWgModesAction = new QAction(tr("&Show Port Modes"), this);
      showWgModesAction->setStatusTip(tr("Show characteristic impedances and cut off frequencies of port modes"));
      connect(showWgModesAction, SIGNAL(triggered()), this, SLOT(showWgModes()));

/*
      showMeshAction = new QAction(tr("&Show Mesh"), this);
      showMeshAction->setStatusTip(tr("Displays Mesh of Selected Shape "));
      connect(showMeshAction, SIGNAL(triggered()), this, SLOT(displaySelectedMesh()));

      hideMeshAction = new QAction(tr("&Hide Mesh"), this);
      hideMeshAction->setStatusTip(tr("Hide Mesh of Selected Shape  "));
      connect(hideMeshAction, SIGNAL(triggered()), this, SLOT(hideSelectedMesh()));
*/

      setMouseTracking(true);
      connect( this, SIGNAL( itemClicked( QTreeWidgetItem *, int ) ),\
	       this, SLOT( setLastClicked( QTreeWidgetItem *, int ) )  );
      connect( this, SIGNAL( itemPressed( QTreeWidgetItem *, int ) ),\
	       this, SLOT( setLastPressed( QTreeWidgetItem *, int ) )  );
      connect( this, SIGNAL( itemEntered( QTreeWidgetItem *, int ) ),\
	       this, SLOT( setLastEntered( QTreeWidgetItem *, int ) )  );
      connect( this, SIGNAL( itemPressed( QTreeWidgetItem *, int ) ),\
	       this, SLOT( highlightItem( QTreeWidgetItem *, int ) )  );
      connect( this, SIGNAL( itemClicked( QTreeWidgetItem *, int ) ),\
	       this, SLOT( setCheck( QTreeWidgetItem *, int ) )  );
      connect( this, SIGNAL(itemExpanded( QTreeWidgetItem *) ),\
	       this, SLOT( openItem( QTreeWidgetItem *) )  );
      connect( this, SIGNAL(itemCollapsed( QTreeWidgetItem *) ),\
	       this, SLOT( closeItem( QTreeWidgetItem *) )  );
      connect( mainOCC, SIGNAL(selectedLabelChanged()),\
               this,   SLOT(getOcafSelection()) );

      connect( this, SIGNAL( itemClicked( QTreeWidgetItem *, int ) ),\
	       this, SLOT( setCurrentItem( QTreeWidgetItem *, int ) )  );

//      connect( this, SIGNAL( currentItemChanged ( QTreeWidgetItem * current, QTreeWidgetItem * previous ) ),\
//	       this, SLOT( setCurrentItem( QTreeWidgetItem * current, QTreeWidgetItem * previous ) )  );

}

TreeWidget::~TreeWidget(){
	if(indexedItem) free(indexedItem);
}


bool getLabelName(TDF_Label label, QString &name){
     TCollection_AsciiString TCname;
     if(!mainOCAF->getLabelName(label, TCname)) return false;
     name=QString(TCname.ToCString());
     return true;
}



void TreeWidget::setItemText(TreeWidgetItem * item){
//   set item name (as displayed in qtree):
     QString qtext;
     TDF_Label label;
     DB::Material* mat;
     bool isSolid=false;
     bool isShape=false;
     bool isGeomPart=false;
     bool isLayer=false;
     bool isLayers=false;
     bool isFace=false;
     bool isEdge=false;
     bool isMaterial=false;
     bool TBD=false;
     DB::Volume *vol;
     if(item->getLabel(label)){ // item is defined from OCAF label
       getLabelName(label, qtext);
       const char* txt=qtext.toLatin1().data();
       isSolid=mainOCAF->isSolid(label);
       isLayer=mainOCAF->isLayer(label);
       isLayers=mainOCAF->isLayers(label);
       isFace =mainOCAF->isFace(label);
       isEdge =mainOCAF->isEdge(label);
       isShape =mainOCAF->isShape(label);
       isGeomPart=mainOCAF->isPart(label);
       if(isGeomPart) vol=mainOCAF->EmP.FindVolume(qtext.toLatin1().data());
     } else if(item->getMaterial(mat)){ // item is defined from DataBase material
         qtext=mat->name;
	 isMaterial=true;
     }
//---------------------
     if(isGeomPart){
         Handle(TColStd_HSequenceOfExtendedString) layers;
//       bool shouldHaveLayers=  (mainOCAF->isTopLevel(label)&&mainOCAF->isSimpleShape(label))\
//                             ||(mainOCAF->isComponent(label)&&!mainOCAF->isAssembly(label));
         bool shouldHaveLayers=false;
         bool hasLayers=mainOCAF->getLabelLayers(label, layers);
         qtext.append(" [ ");
	 QBrush redbrush=QBrush(QColor(255, 0, 0, 127));
         if(vol->type==ASSEMBLY)     qtext.append("CMP");
         if(vol->type==WAVEGUIDE)    qtext.append("WG ");
         if(vol->type==LINEPORT)     qtext.append("LP ");
         if(vol->type==DIELECTRIC)   qtext.append("DIEL ");
         if(vol->type==HOLE)         qtext.append("HOLE ");
         if(vol->type==SPLITTER)     qtext.append("-|-");
         if(vol->type==GRID)         qtext.append("|||");
         if(vol->type==BOUNDARYCOND) qtext.append("BC ");
         if(vol->type==DIELECTRIC || vol->type==BOUNDARYCOND || vol->type==WAVEGUIDE || vol->type==HOLE && strcmp(vol->material,"?")) qtext.append(vol->material);
         if(vol->type==GRID && vol->gridNum) qtext.append(QString(" %1").arg(vol->gridNum));
#if defined(EXPLICIT_INVARIANT)
         if(vol->type==GRID && vol->invariant) qtext.append(" INVAR");
#endif
         if(vol->type==GRID && vol->PML) qtext.append(" PML");
         if(vol->type!=SPLITTER)     if(!vol->defined) item->setBackground ( 0, redbrush );
	                             else              item->setBackground ( 0, Qt::NoBrush );
         qtext.append(" ]");
         if(showlayers&&hasLayers){
           qtext.append(" - ");
           for (int i = 1; i <= layers->Length(); i++){
             TCollection_ExtendedString layer=layers->Value(i);
             int cstrlen= layer.LengthOfCString();
             char* cstr=(char *) malloc((cstrlen+1)*sizeof(char));
             layer.ToUTF8CString(cstr);
             qtext.append(cstr);
             if(i<layers->Length()) qtext.append(",");
             free(cstr);
           }
         }else if (shouldHaveLayers){
            QString qstr="  ( ? )";
	    qtext.append(qstr);
            TBD=true;
         }
//---------------------
     }else if(isLayers){
       item->setHidden(!showlayers);
     }else if(isLayer){
       item->setHidden(!showlayers);
//         TBD=!mainOCAF->EmP.FindMaterial(qtext.toLatin1().data());
//---------------------
     }else if(isFace||isEdge){
        TDF_LabelSequence *equivalents=mainOCAF->getEquivalents(label);
        if(equivalents){
         int Neq=equivalents->Length();
         char cstr[30];  
         sprintf(cstr,"  [ %d ]",Neq);
         qtext.append(cstr);
        }
//---------------------
     }else if(isMaterial){
         qtext=mat->name;
     }
//---------------------
     if(qtext.isEmpty()) return;
     item->setText(0,qtext);
     if(TBD) { QBrush brush(QColor(255, 0, 0, 127)); item->setForeground(0,brush);}
     else    { QBrush brush(QColor(  0, 0, 0, 255)); item->setForeground(0,brush);}
}


void TreeWidget::setItemLink(TreeWidgetItem * qitem, TDF_Label label){
//   set a link to the OCAF label:
     int myDataRole=32;
     WidgetItemData d;
     d.type=OCAF_LABEL;
     d.docLabel=label;
     QVariant qdata;
     qdata.setValue(d);
     qitem->setData(0,myDataRole,qdata);
//   set a link from the OCAF label to the qitem index:
     qitem->index=++maxIndex;
     mainOCAF->setLabelIndex(label, qitem->index);
}

void TreeWidget::setItemLink(TreeWidgetItem * qitem, DB::Material *mat){
//   set a link to the OCAF label:
     int myDataRole=32;
     WidgetItemData d;
     d.type=DB_MATERIAL;
     d.data=(void *) mat;
     QVariant qdata;
     qdata.setValue(d);
     qitem->setData(0,myDataRole,qdata);
     mat->I=qitem->index=++maxIndex;
}

void TreeWidget::setItem(TreeWidgetItem * item, TDF_Label label){
  setItemLink(item, label);
  setItemText(item);
}

void TreeWidget::showLayers(bool show)
{
  if(showlayers!=show){
      showlayers=show;
      updateText();
  }
}


bool TreeWidgetItem::getLabel(TDF_Label &label){
//   get the OCAF label:
    int myDataRole=32;
    QVariant qdata = this->data(0,myDataRole);
//    if(!qdata.isNull()){
    if(qdata.canConvert<WidgetItemData>()){
      WidgetItemData d=qdata.value<WidgetItemData>();
      if(d.type==OCAF_LABEL){ 
	 label=d.docLabel;
	 return true;
      }
    }
    return false;
}
int TreeWidgetItem::dataType(){
    int myDataRole=32;
    QVariant qdata = this->data(0,myDataRole);
    if(qdata.canConvert<WidgetItemData>()){
      WidgetItemData d=qdata.value<WidgetItemData>();
      return d.type; 
    }
    return 0;
}
bool TreeWidgetItem::getMaterial(DB::Material* &mat){
//   get the OCAF label:
    int myDataRole=32;
    QVariant qdata = this->data(0,myDataRole);
//    if(!qdata.isNull()){
    if(qdata.canConvert<WidgetItemData>()){
      WidgetItemData d=qdata.value<WidgetItemData>();
      if(d.type==DB_MATERIAL){ 
	 mat=(DB::Material *) d.data;
	 return true;
      }
    }
    return false;
}

bool TreeWidget::getLabel(TreeWidgetItem * item, TDF_Label &label){
     return item->getLabel(label);
}
bool TreeWidget::getMaterial(TreeWidgetItem * item, DB::Material* &mat){
     return item->getMaterial(mat);
}


void TreeWidget::makeSubTree(TreeWidgetItem * troot, TDF_Label root)
{
 for (TDF_ChildIterator it(root,Standard_False); it.More(); it.Next()) 
 {
     TDF_Label label = it.Value();
     if (label.HasAttribute()){
//       bool skip=mainOCAF->hasAssembly()&&mainOCAF->isTopLevel(label) && mainOCAF->isSimpleShape(label);
//       bool skip=mainOCAF->hasAssembly()&&mainOCAF->isTopLevel(label)&&(!mainOCAF->isFree(label) ||mainOCAF->isFree(label) && mainOCAF->isSimpleShape(label));
       bool skip= mainOCAF->isShape(label) && mainOCAF->isTopLevel(label) && !mainOCAF->isFree(label);
       skip=skip || mainOCAF->isMaterial(label) || mainOCAF->isDGTs(label) || mainOCAF->isParts(label) || mainOCAF->isDisabled(label);
       if (!skip) {
         TreeWidgetItem *item =new TreeWidgetItem(troot);
         setItemLink(item,label);
         setItemText(item);
         if (mainOCAF->isShape(label)){
//     	     item->setFlags(item->flags()|Qt::ItemIsUserCheckable|Qt::ItemIsTristate);
     	     item->setFlags(item->flags()|Qt::ItemIsUserCheckable);
             if(mainOCAF->volType(label)==GRID) item->setCheckState (0, Qt::Unchecked );
	     else                               item->setCheckState (0, Qt::Checked );
         }
         makeSubTree(item,label);  //recursion
       }
//     item->setText(0,QString("item: %1").arg(i));
     }
  }
}

void TreeWidget::makeMaterials(TreeWidgetItem * troot)
{
//  int N=troot->childCount();
//  for(int i = N; i > 0; i--) troot->removeChild(troot->child(i));
  troot->takeChildren();
  DB::List_T *mats = Tree2List(mainOCAF->EmP.materials);
  for(int i = 0; i < List_Nbr(mats); i++) {
    DB::Material *mat;
    List_Read(mats, i, &mat);
    TreeWidgetItem *item =new TreeWidgetItem(troot);
    setItemLink(item,mat);
    setItemText(item);
  }
  List_Delete(mats);
}

void TreeWidget::updateText(){
   if(indexedItem) indexedItem=(TreeWidgetItem**) realloc(indexedItem,maxIndex*sizeof(TreeWidgetItem *));
   else            indexedItem=(TreeWidgetItem**) malloc(             maxIndex*sizeof(TreeWidgetItem *));
   QTreeWidgetItemIterator qit(this);
   while (*qit) {
     TreeWidgetItem* it=(TreeWidgetItem *) (*qit);
     ++qit;
     setItemText(it);
   }
}

void TreeWidget::updateIndex(){
   if(indexedItem) indexedItem=(TreeWidgetItem**) realloc(indexedItem,maxIndex*sizeof(TreeWidgetItem *));
   else            indexedItem=(TreeWidgetItem**) malloc(             maxIndex*sizeof(TreeWidgetItem *));
   QTreeWidgetItemIterator qit(this);
   while (*qit) {
     TreeWidgetItem* it=(TreeWidgetItem *) (*qit);
     if(it->index) indexedItem[it->index-1]=it;
     ++qit;
   }
}

TreeWidgetItem* TreeWidget::getItemFromLabel(TDF_Label label){
 int lI=mainOCAF->getLabelIndex(label);
 if(indexedItem && lI && lI <=maxIndex ) return indexedItem[lI-1];
 else return NULL;
}

TreeWidgetItem* TreeWidget::getItemFromMaterial(DB::Material *mat){
 if(indexedItem && mat->I && mat->I <=maxIndex ) return indexedItem[mat->I-1];
 else return NULL;
}



void TreeWidget::about()
{
    QMessageBox::about(this, tr("About Menu"),
            tr("The <b>Qt OpenCASCADE</b> example shows how to create a simple Qt4 Viewer."));
}



void TreeWidget::assignLayer(){
  TCollection_ExtendedString layer=TCollection_ExtendedString(currentLayerName.toLatin1().data());
  TDF_Label label; currentPart->getLabel(label);
  mainOCAF->setLabelLayer(label, layer);
  setItemText(currentPart);
}

void TreeWidget::assignMaterial(){
  DB::Volume *vol=mainOCAF->EmP.FindVolume(currentPartName.toLatin1().data());
  if(!vol) return; 
  if( ( 
        !strcmp(currentMaterialName.toLatin1().data(),"PEC") || 
        !strcmp(currentMaterialName.toLatin1().data(),"PMC")
      ) 
      && vol->type!=BOUNDARYCOND && vol->type!=HOLE
    )
  {
       mainWindow->temporaryMessage(currentMaterialName + tr(" can only be assigned to parts declared as Boundary Condition or Hole"));
       return;
  }
  if(strcmp(vol->material,currentMaterialName.toLatin1().data())){
    strcpy(vol->material,currentMaterialName.toLatin1().data());
    mainOCAF->worksaveNeeded=true;
    mainOCAF->partitionVolSaveNeeded=true;
    prjData.workStatus.materialChanged=true;
    mainOCAF->setPartsStatus();
    mainWindow->setProjectTitle();
    setItemText(currentPart);
    mainOCAF->updatePartColors();
    mainWindow->checkActions();
  }
}

//void TreeWidget::regainFocus(){setFocusProxy(0);}

AssignDialog::AssignDialog(TreeWidget *parent) : QDialog(parent)
{
     setModal(0);
     label1= new QLabel();
     lineEdit1 = new QLineEdit();
     lineEdit1->setReadOnly(true);
     label2= new QLabel();
     lineEdit2 = new QLineEdit();
     lineEdit2->setReadOnly(true);
     assignButton = new QPushButton(tr("Assign"));
     assignButton->setShortcut(tr("Enter"));
     closeButton = new QPushButton(tr("Close"));
     layout = new QHBoxLayout(this);
     layout->addWidget(label1);
     layout->addWidget(lineEdit1);
     layout->addWidget(label2);
     layout->addWidget(lineEdit2);
     layout->addWidget(assignButton);
     layout->addWidget(closeButton);
     connect(assignButton, SIGNAL(clicked()), this, SLOT(assign()));
     connect(parent, SIGNAL(enterPressed()), this, SLOT(assign()));
     connect(closeButton,  SIGNAL(clicked()), this, SLOT(accept()));
//     connect(this,  SIGNAL(accepted()), parent, SLOT(regainFocus()));
     setFocusPolicy(Qt::StrongFocus);
}

SetCompPropertiesDialog::SetCompPropertiesDialog(TreeWidget *parent) : QDialog(parent)
{
     setModal(0);
     setWindowTitle(tr("Part Properties"));
/*
     QSizePolicy sizep;
     sizep.setVerticalPolicy(QSizePolicy::Maximum);
     setSizePolicy(sizep);
*/
//****************************************
//   material name
//
     QLabel *nameLabel= new QLabel(); nameLabel->setText(tr("Name"));
     nameLineEdit = new QLineEdit();
     nameLineEdit->setReadOnly(true);
     w=400; h=250;
     window()->resize(w,h);

     QLabel *typeLabel= new QLabel(); typeLabel->setText(tr("Type"));
     typeChooser = new QComboBox();

     int tI=0;
     if(mainOCAF->EmP.assemblyType==NET){
       typeChooser->addItem(tr("Component"));       typeChooserMap[tI++]=ASSEMBLY;
     }
     else if(mainOCAF->EmP.assemblyType==COMPONENT || mainOCAF->EmP.assemblyType==PARTITION){
       typeChooser->addItem(tr("Dielectric"));      typeChooserMap[tI++]=DIELECTRIC;
       typeChooser->addItem(tr("Hole"));            typeChooserMap[tI++]=HOLE;
       typeChooser->addItem(tr("Waveguide"));       typeChooserMap[tI++]=WAVEGUIDE;
       typeChooser->addItem(tr("Line-Port"));       typeChooserMap[tI++]=LINEPORT;
       typeChooser->addItem(tr("Splitter"));        typeChooserMap[tI++]=SPLITTER;
       typeChooser->addItem(tr("Grid"));            typeChooserMap[tI++]=GRID;
     }
     typeChooser->addItem(tr("Boundary Cond"));   typeChooserMap[tI++]=BOUNDARYCOND;

     connect(typeChooser, SIGNAL(  currentIndexChanged  (int) ), this, SLOT(updateType(int)) );

     QGridLayout *nameLayout = new QGridLayout();
     nameLayout->setColumnMinimumWidth(1,20);
     nameLayout->addWidget(nameLabel, 0, 0);
     nameLayout->addWidget(nameLineEdit,0, 2);
     nameLayout->addWidget(typeLabel, 1, 0);
     nameLayout->addWidget(typeChooser, 1, 2);

     nameGroupBox=new QGroupBox(tr(""));
     nameGroupBox->setLayout(nameLayout);


     meshRefValidator = new QDoubleValidator(1.0, 50.0, 5, this);
     meshRefValidator->setNotation(QDoubleValidator::StandardNotation);

     positiveDoubleValidator = new QDoubleValidator(this);
     positiveDoubleValidator->setBottom(0.0);

//****************************************
//   Waveguide Ports


     QLabel *WgLabel= new QLabel();
     WgLabel->setText(tr("Port Modes Number:"));

     QLabel *TEMnumLabel= new QLabel(); 
     TEMnumLabel->setText(tr("TEM:"));
     TEMnumLE = new QLineEdit();
     TEMnumLE->setReadOnly(true);
     TEMnumLE->setText(tr("0"));

     QLabel *TEnumLabel= new QLabel(); 
     TEnumLabel->setText(tr("TE :"));
     TEnumSB = new QSpinBox();
     TEnumSB->setValue(0);
     TEnumSB->setMinimum(0);

     QLabel *TMnumLabel= new QLabel(); 
     TMnumLabel->setText(tr("TM :"));
     TMnumSB = new QSpinBox();
     TMnumSB->setValue(0);
     TMnumSB->setMinimum(0);

     QGridLayout *WgLayout = new QGridLayout();
     WgLayout->addWidget(WgLabel,     0, 0);
     WgLayout->addWidget(TEMnumLabel, 1, 0);
     WgLayout->addWidget(TEMnumLE,    1, 1);
     WgLayout->addWidget(TEnumLabel,  2, 0);
     WgLayout->addWidget(TEnumSB,     2, 1);
     WgLayout->addWidget(TMnumLabel,  3, 0);
     WgLayout->addWidget(TMnumSB,     3, 1);
     
     WgGroupBox=new QGroupBox();
     WgGroupBox->setLayout(WgLayout);


//****************************************
//   Line Ports
     QLabel *LPLabel= new QLabel();
     LPLabel->setText(tr("Load Impedence [ohm]:"));
     LinePortZcLineEdit=new QLineEdit();
     double LPimpedance=(prjData.portloads.map.find(LPname)==prjData.portloads.map.end())? 50.0 : prjData.portloads.map[LPname];
     LinePortZcLineEdit->setText(QString("%1").arg(LPimpedance));
     LinePortZcLineEdit->setValidator(positiveDoubleValidator);

     QGridLayout *LPLayout = new QGridLayout();
     LPLayout->addWidget(LPLabel,     0, 0);
     LPLayout->addWidget(LinePortZcLineEdit, 1, 0);

     LPGroupBox=new QGroupBox();
     LPGroupBox->setLayout(LPLayout);


//****************************************
//   Grid

     QDoubleValidator *gridSpaceValidator = new QDoubleValidator(this);
     gridSpaceValidator->setNotation(QDoubleValidator::StandardNotation);
     gridSpaceValidator->setBottom(0);
     gridSpaceValidator->setDecimals(5);

     QLabel *gridSpaceLabel= new QLabel(); 
     gridSpaceLabel->setText(tr("Plane Number:"));
     gridNumSB = new QSpinBox();
     gridNumSB->setValue(2);
     gridNumSB->setMinimum(2);

     PML=new QCheckBox("Perfectly Matched Layer", this);
     PML->setCheckState(Qt::Unchecked);

#if defined(EXPLICIT_INVARIANT)
     invariant=new QCheckBox("Invariant Direction", this);
     invariant->setCheckState(Qt::Checked);
#endif

     QGridLayout *GridLayout = new QGridLayout();
     GridLayout->addWidget(gridSpaceLabel, 0, 0);
     GridLayout->addWidget(gridNumSB, 0, 1);
     GridLayout->addWidget(PML,1, 0);
#if defined(EXPLICIT_INVARIANT)
     GridLayout->addWidget(invariant,2, 0);
#endif
     GridGroupBox=new QGroupBox(tr(""));
     GridGroupBox->setLayout(GridLayout);


//****************************************
//   meshRefinemente
     QLabel *meshRefLabel= new QLabel(); 
     meshRefLabel->setText(tr("Mesh refinement"));
     meshRefLineEdit = new QLineEdit();
     meshRefLineEdit->setText(QString("%1").arg(1.0));
     meshRefLineEdit->setValidator(meshRefValidator);		
     meshRefLineEdit->resize(1,5);

     QGridLayout *meshLayout = new QGridLayout();
     meshLayout->addWidget(meshRefLabel, 0, 0);
     meshLayout->addWidget(meshRefLineEdit,0, 1);

     meshGroupBox=new QGroupBox(tr(""));
     meshGroupBox->setLayout(meshLayout);

     if(mainOCAF->EmP.assemblyType==NET) meshGroupBox->hide();


//****************************************
//   control buttons:
//
     QPushButton *setButton = new QPushButton(tr("Set"));
     QPushButton *closeButton =new QPushButton(tr("Close"));
     QPushButton *helpButton  = new QPushButton(tr("Help"));


     QHBoxLayout *buttonLayout = new QHBoxLayout();
     buttonLayout->addWidget(setButton);
     buttonLayout->addWidget(closeButton);
     buttonLayout->addWidget(helpButton);
     buttonGroupBox=new QGroupBox(tr(""));
     buttonGroupBox->setLayout(buttonLayout);

     connect(setButton, SIGNAL(clicked()), this, SLOT(set()));
     connect(parent, SIGNAL(enterPressed()), this, SLOT(set()));
     connect(closeButton,  SIGNAL(clicked()), this, SLOT(accept()));
     connect(helpButton,  SIGNAL(clicked()), this, SLOT(help()));

     setFocusPolicy(Qt::StrongFocus);

//--------------------------------
     WgGroupBox->hide();
     LPGroupBox->hide();
     GridGroupBox->hide();
//--------------------------------
     mainLayout = new QVBoxLayout(this);
     mainLayout->addWidget(nameGroupBox);
     mainLayout->addWidget(WgGroupBox);
     mainLayout->addWidget(LPGroupBox);
     mainLayout->addWidget(meshGroupBox);
     mainLayout->addWidget(GridGroupBox);
     mainLayout->addWidget(buttonGroupBox);

}

void SetCompPropertiesDialog::updateType(int i)
{
    if(typeChooserMap[i]==WAVEGUIDE){
  	 WgGroupBox->show();
  	 meshGroupBox->show();
	 w=400; h=420;
    }else{
  	 WgGroupBox->hide();
	 w=400; h=250;
    }
    if(typeChooserMap[i]==DIELECTRIC){
  	 meshGroupBox->show();
	 w=400; h=250;
    }
    if(typeChooserMap[i]==HOLE){
  	 meshGroupBox->show();
	 w=400; h=250;
    }
    if(typeChooserMap[i]==LINEPORT){
  	 LPGroupBox->show();
	 w=400; h=250;
    }
    if(typeChooserMap[i]==GRID){
  	 GridGroupBox->show();
	 w=400; h=290;
    }
    if(typeChooserMap[i]!=WAVEGUIDE && typeChooserMap[i]!=DIELECTRIC && typeChooserMap[i]!=HOLE && typeChooserMap[i]!=BOUNDARYCOND){
  	 meshGroupBox->hide();
    }
    QApplication::processEvents();
    window()->resize(w,h);
}


void SetCompPropertiesDialog::getVolumeData(QString volname){
  if(volname.isEmpty()) return;
  DB::Volume* vol = mainOCAF->EmP.FindVolume(volname.toLatin1().data());
  if(!vol) return;
  QString TEMnum; TEMnum.setNum(vol->TEMportsNum);
  TEMnumLE->setText(TEMnum);
  TEnumSB->setValue(vol->TEportsNum);
  TMnumSB->setValue(vol->TMportsNum);
  for (int i = 0; i <typeChooser->count(); ++i)
	  if(typeChooserMap[i]==vol->type)  typeChooser->setCurrentIndex(i);
  if(vol->type==DIELECTRIC || vol->type==HOLE || vol->type==WAVEGUIDE || vol->type==BOUNDARYCOND)  meshRefLineEdit->setText(QString("%1").arg(vol->meshRefinement));
  if(vol->type==LINEPORT){
	TCollection_AsciiString assName; mainOCAF->getAssName(assName);
	std::string volName=vol->name;
	LPname=std::string(assName.ToCString())+std::string("__")+volName+std::string("_v");
	double LPimpedance=(prjData.portloads.map.find(LPname)==prjData.portloads.map.end())? 50.0 : prjData.portloads.map[LPname];
	LinePortZcLineEdit->setText(QString("%1").arg(LPimpedance));
  }
  if (vol->type==GRID){
   gridNumSB->setValue(vol->gridNum);
   if(vol->PML) PML->setCheckState(Qt::Checked); else PML->setCheckState(Qt::Unchecked);
#if defined(EXPLICIT_INVARIANT)
   if(vol->invariant) invariant->setCheckState(Qt::Checked); else invariant->setCheckState(Qt::Unchecked);
#endif
  }
  nameLineEdit->setText(volname);
}



void SetCompPropertiesDialog::setVolumeData(DB::Volume* vol){
  vol->type=typeChooserMap[typeChooser->currentIndex()];
  bool changed=false;
  if(vol->type==DIELECTRIC || vol->type==HOLE ||vol->type==WAVEGUIDE || vol->type==BOUNDARYCOND) 
   if(fabs(vol->meshRefinement-meshRefLineEdit->text().toDouble())>1.e-5){
     vol->meshRefinement=meshRefLineEdit->text().toDouble();
     changed=true;
     prjData.workStatus.decompositionNeeded=true;
     prjData.workStatus.remeshNeeded=true;
   }
  if(vol->type==WAVEGUIDE){
        bool portChanged=false;	
	if(vol->TEportsNum!=TEnumSB->value())  {vol->TEportsNum=TEnumSB->value(); portChanged=changed=true;}
	if(vol->TMportsNum!=TMnumSB->value())  {vol->TMportsNum=TMnumSB->value(); portChanged=changed=true;}
	if(portChanged) {
           prjData.workStatus.decompositionNeeded=true;
        }
  }
  if(vol->type==LINEPORT)
   if(fabs(prjData.portloads.map[LPname]-LinePortZcLineEdit->text().toDouble())>1.e-5){
     prjData.portloads.map[LPname]=LinePortZcLineEdit->text().toDouble();
     prjData.savePortLoads();
  }
  if (vol->type==GRID){
	if(vol->gridNum!=gridNumSB->value())  {
             vol->gridNum=gridNumSB->value(); changed=true;
             prjData.workStatus.reloadNeeded=!prjData.workStatus.firstDecomposition;
             prjData.workStatus.decompositionNeeded=true;
             mainOCAF->setPartsStatus();
	}
	int pml=PML->checkState()==Qt::Checked;
        if(vol->PML!=pml){
	   vol->PML=pml;
	   changed=true;	    
           prjData.workStatus.reloadNeeded=!prjData.workStatus.firstDecomposition;
           prjData.workStatus.decompositionNeeded=true;
	}
#if defined(EXPLICIT_INVARIANT)
	int invDir=invariant->checkState()==Qt::Checked;
        if(vol->invariant!=invDir){
	   vol->invariant=invDir;
	   changed=true;	    
           prjData.workStatus.reloadNeeded=!prjData.workStatus.firstDecomposition;
           prjData.workStatus.decompositionNeeded=true;
	}
#endif
  }
  if(changed){
         mainOCAF->partitionVolSaveNeeded=true;
      	 mainOCAF->worksaveNeeded=true;
         mainWindow->checkActions();
  }
}

void SetCompPropertiesDialog::set(){
	QString volname=nameLineEdit->text();
	if(volname.isEmpty()) return;
	DB::Volume* vol =mainOCAF->EmP.FindVolume(volname.toLatin1().data());
	if(!vol) return; 
	setVolumeData(vol);
        treeWidget->updateText();
}

void SetCompPropertiesDialog::help()
{
    mainWindow->showDocumentation(QLatin1String("#2.4.1"));
}


DefineMaterialDialog::DefineMaterialDialog(TreeWidget *parent) : QDialog(parent)
{
     setModal(0);
     setWindowTitle(tr("Material Create/Modify"));

/*
     QSizePolicy sizep;
     sizep.setVerticalPolicy(QSizePolicy::Maximum);
     setSizePolicy(sizep);
*/
//****************************************
//   material name
//
     QLabel *nameLabel= new QLabel(); nameLabel->setText(tr("Material Name"));
     nameLineEdit = new QLineEdit();
     nameLineEdit->setReadOnly(false);
     w=400; h=300;
     window()->resize(w,h);
     QHBoxLayout *nameLayout = new QHBoxLayout();
     nameLayout->addWidget(nameLabel);
     nameLayout->addWidget(nameLineEdit);
     nameGroupBox=new QGroupBox(tr(""));
     nameGroupBox->setLayout(nameLayout);

     dvalidator = new QDoubleValidator(this);
     dvalidator->setDecimals(1000); // (standard anyway)
     dvalidator->setNotation(QDoubleValidator::ScientificNotation);

//****************************************
//   main properties
//   electric:

     QString EPSR=tr("<font face='symbol'>e<font face='arial'>");
     EPSR.append(tr("<sub>r</sub>"));
/*
     EPSR.append(tr("(f="));
     EPSR.append(tr("<font face='symbol'>"));
     EPSR.append(QChar(0x0221E));
     EPSR.append(tr("<font face='arial'>"));
     EPSR.append(tr(")"));
*/
     QLabel *epsLabel= new QLabel(); 
     epsLabel->setText(EPSR);
     epsLineEdit = new QLineEdit();
     epsLineEdit->setText(QString("%1").arg(1.0, 0, 'f', 5));
     epsLineEdit->setValidator(dvalidator);		
     epsLineEdit->resize(1,5);

     QString SIGMA=tr("<font face='symbol'>s </font><font face='arial'> [S/m]");
     QLabel *sigmaLabel= new QLabel(); sigmaLabel->setAlignment( Qt::AlignRight);
     sigmaLabel->setText(SIGMA);
     sigmaLineEdit = new QLineEdit();
     sigmaLineEdit->setText(QString("%1").arg(0.0, 0, 'e', 5));
     sigmaLineEdit->setValidator(dvalidator);		
     sigmaLineEdit->resize(1,5);

//----------------------
//   magnetic:

     QString MUSR=tr("<font face='symbol'>m</font><font face='arial'>");
     MUSR.append(tr("<sub>r</sub>"));
/*     
     MUSR.append(tr("(f="));
     MUSR.append(tr("<font face='symbol'>"));
     MUSR.append(QChar(0x0221E));
     MUSR.append(tr("<font face='arial'>"));
     MUSR.append(tr(")"));
*/
     QLabel *muLabel= new QLabel(); 
     muLabel->setText(MUSR);
     muLineEdit = new QLineEdit();
     muLineEdit->setText(QString("%1").arg(1.0, 0, 'f', 5));
     muLineEdit->setValidator(dvalidator);		
     muLineEdit->resize(1,5);

     QString SIGMAM =tr("<font face='symbol'>s</font><font face='arial'><sub>m</sub>[");
     SIGMAM.append(  tr("<font face='symbol'>") );
     SIGMAM.append( QChar(0x2126));
     SIGMAM.append(  tr("</font><font face='arial'>/m]") );
     QLabel *sigmamLabel= new QLabel(); 
     sigmamLabel->setText(SIGMAM); sigmamLabel->setAlignment( Qt::AlignRight);
     sigmamLineEdit = new QLineEdit();
     sigmamLineEdit->setText(QString("%1").arg(0.0, 0, 'e', 5));
     sigmamLineEdit->setValidator(dvalidator);		
     sigmamLineEdit->resize(1,5);

     dispersive=new QCheckBox("Dispersive Model", this);
     dispersive->setCheckState(Qt::Unchecked);

     QString freqBandStr=QString("Frequency band [");
     freqBandStr+=QString(prjData.freqUnitName());
     freqBandStr+=QString("] :");
     QLabel *freqBandLabel= new QLabel();
     freqBandLabel->setText(freqBandStr);
     f1LineEdit = new QLineEdit();
     f1LineEdit->setValidator(dvalidator);		
     f2LineEdit = new QLineEdit();
     f2LineEdit->setValidator(dvalidator);


     boundaryCond=new QCheckBox("Boundary", this);
     boundaryCond->setCheckState(Qt::Unchecked);
     connect(boundaryCond, SIGNAL(  stateChanged (int) ), this, SLOT(updateBoundaryCond(int)) );


     QGridLayout *emLayout = new QGridLayout();
     emLayout->setColumnMinimumWidth(2,30);
     emLayout->addWidget(epsLabel, 0, 0);
     emLayout->addWidget(epsLineEdit,0, 1);
     emLayout->addWidget(sigmaLabel, 0, 2);
     emLayout->addWidget(sigmaLineEdit,0, 3);

     emLayout->addWidget(muLabel,  1,  0);
     emLayout->addWidget(muLineEdit,1, 1);
     emLayout->addWidget(sigmamLabel, 1, 2);
     emLayout->addWidget(sigmamLineEdit,1, 3);

     emLayout->addWidget(dispersive,2, 1);
     emLayout->addWidget(boundaryCond,2, 2);

     QLabel *bandSepLabel= new QLabel();
     emLayout->addWidget(freqBandLabel, 3, 1);
     emLayout->addWidget(f1LineEdit,3, 2);
     emLayout->addWidget(f2LineEdit,3, 3);

     emGroupBox=new QGroupBox();
     emGroupBox->setLayout(emLayout);


//****************************************
//   electric dispersion
//
     eTanDelta=new QCheckBox("Loss Tangent", this);
     eTanDelta->setCheckState(Qt::Unchecked);
     eTanDeltaLineEdit = new QLineEdit();
     eTanDeltaLineEdit->setText(QString("%1").arg(0.0, 0, 'f', 5));
     eTanDeltaLineEdit->setValidator(dvalidator);
     eTanDeltaLineEdit->setReadOnly(true);

     hTanDelta=new QCheckBox("Loss Tangent", this);
     hTanDelta->setCheckState(Qt::Unchecked);
     hTanDeltaLineEdit = new QLineEdit();
     hTanDeltaLineEdit->setText(QString("%1").arg(0.0, 0, 'f', 5));
     hTanDeltaLineEdit->setValidator(dvalidator);
     hTanDeltaLineEdit->setReadOnly(true);

     connect(dispersive, SIGNAL(  stateChanged (int) ), this, SLOT(updateDisp(int)) );
     connect(eTanDelta, SIGNAL(  stateChanged (int) ), this, SLOT(updateETanDelta(int)) );
     connect(hTanDelta, SIGNAL(  stateChanged (int) ), this, SLOT(updateHTanDelta(int)) );
     connect(f1LineEdit, SIGNAL( textEdited (const QString & ) ), this, SLOT(updateTanDelta(const QString & )) );
     connect(f2LineEdit, SIGNAL( textEdited (const QString & ) ), this, SLOT(updateTanDelta(const QString & )) );
     connect(eTanDeltaLineEdit, SIGNAL( textEdited (const QString & ) ), this, SLOT(updateETanDelta(const QString & )) );
     connect(hTanDeltaLineEdit, SIGNAL( textEdited (const QString & ) ), this, SLOT(updateHTanDelta(const QString & )) );
     connect(epsLineEdit, SIGNAL( textEdited (const QString & ) ), this, SLOT(updateETanDelta(const QString & )) );
     connect(muLineEdit,  SIGNAL( textEdited (const QString & ) ), this, SLOT(updateHTanDelta(const QString & )) );


     QLabel *epsTermsNumLabel= new QLabel(); 
     epsTermsNumLabel->setText(tr("Terms Number:"));
     epsTermsNumSB = new QSpinBox();
     epsTermsNumSB->setValue(0);
     epsTermsNumSB->setMinimum(0);

     epsLorentz= new QTableWidget(0, 3, this);
//     epsLorentz->horizontalHeader()->setMinimumSectionSize(200);
     #if defined(QT5)
      epsLorentz->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     #elif defined(QT4)
      epsLorentz->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
     #endif

/*
     epsLorentz->setColumnWidth(0, 150);
     epsLorentz->setColumnWidth(1, 150);
     epsLorentz->setColumnWidth(2, 150);
*/
     QTableWidgetItem *eheader1 = new QTableWidgetItem(tr("De"));
     QTableWidgetItem *eheader2 = new QTableWidgetItem(tr("relax freq [Hz]"));
     QTableWidgetItem *eheader3 = new QTableWidgetItem(tr("reson freq [Hz]"));
     QFont symb("Symbol", 10);
//     header1->setTextFormat(Qt::RichText);
     eheader1->setFont(symb);
     epsLorentz->setHorizontalHeaderItem(0, eheader1);
     epsLorentz->setHorizontalHeaderItem(1, eheader2);
     epsLorentz->setHorizontalHeaderItem(2, eheader3);

     QGridLayout *eDispLayout = new QGridLayout();
     eDispLayout->addWidget(eTanDelta, 0, 0);
     eDispLayout->addWidget(eTanDeltaLineEdit, 0, 1);
     eDispLayout->addWidget(epsTermsNumLabel, 1, 0);
     eDispLayout->addWidget(epsTermsNumSB, 1, 1);
     eDispLayout->addWidget(epsLorentz, 2, 0, 1, 3);

     eDispGroupBox=new QGroupBox(tr("Electric Dispersion"));
     eDispGroupBox->setLayout(eDispLayout);
     eDispGroupBox->hide();

/*
     tt=tr("<font face='symbol'>de<font face='arial'>");
     tt.append(tr("<sub>r</sub>(f=0)"));
     QLabel *epsInfLabel= new QLabel(); epsInfLabel->setText(tt);
     QLabel *epsFrLabel= new QLabel();  epsFrLabel->setText(tr("relax freq [Hz]"));
     QLabel *epsF0Label= new QLabel();  epsF0Label->setText(tr("reson freq [Hz"));
     epsInfLineEdit = new QLineEdit();
     epsFrLineEdit = new QLineEdit();
     epsF0LineEdit = new QLineEdit();
*/

//****************************************
//   magnetic dispersion
//
     QLabel *muTermsNumLabel= new QLabel(); 
     muTermsNumLabel->setText(tr("Terms Number:"));
     muTermsNumSB = new QSpinBox();
     muTermsNumSB->setValue(0);
     muTermsNumSB->setMinimum(0);

     muLorentz = new QTableWidget(0, 3, this);
     #if defined(QT5)
      muLorentz->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     #elif defined(QT4)
      muLorentz->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
     #endif
/*
     muLorentz->setColumnWidth(0, 150);
     muLorentz->setColumnWidth(1, 150);
     muLorentz->setColumnWidth(2, 150);
*/
     QTableWidgetItem *mheader1 = new QTableWidgetItem(tr("Dm"));
     QTableWidgetItem *mheader2 = new QTableWidgetItem(tr("relax freq [Hz]"));
     QTableWidgetItem *mheader3 = new QTableWidgetItem(tr("reson freq [Hz]"));
     mheader1->setFont(symb);
     muLorentz->setHorizontalHeaderItem(0, mheader1);
     muLorentz->setHorizontalHeaderItem(1, mheader2);
     muLorentz->setHorizontalHeaderItem(2, mheader3);

     QGridLayout *mDispLayout = new QGridLayout();
     mDispLayout->addWidget(hTanDelta, 0, 0);
     mDispLayout->addWidget(hTanDeltaLineEdit, 0, 1);
     mDispLayout->addWidget(muTermsNumLabel,1, 0);
     mDispLayout->addWidget(muTermsNumSB, 1, 1);
     mDispLayout->addWidget(muLorentz, 2, 0, 1, 3);

     mDispGroupBox=new QGroupBox(tr("Magnetic Dispersion"));
     mDispGroupBox->setLayout(mDispLayout);
     mDispGroupBox->hide();


//****************************************
//   Surface Resistence

     QLabel *SresLabel= new QLabel(); 
     SresLabel->setText(tr("Resistance [ohm]"));
     SresLineEdit = new QLineEdit();
     SresLineEdit->setText(QString("%1").arg(0.0, 0, 'e', 5));
     SresLineEdit->setValidator(dvalidator);		
     SresLineEdit->resize(1,5);

     QHBoxLayout *SresLayout = new QHBoxLayout();
     SresLayout->addWidget(SresLabel);
     SresLayout->addSpacing(20);
     SresLayout->addWidget(SresLineEdit);
     SresGroupBox=new QGroupBox();
     SresGroupBox->setLayout(SresLayout);


/*
     tt=tr("<font face='symbol'>dm<font face='arial'>");
     tt.append(tr("<sub>r</sub>(f=0)"));
     QLabel *muInfLabel= new QLabel(); muInfLabel->setText(tt);
     QLabel *muFrLabel= new QLabel();  muFrLabel->setText(tr("relaxation freq"));
     QLabel *muF0Label= new QLabel();  muF0Label->setText(tr("resonant freq"));
     muInfLineEdit = new QLineEdit();
     muFrLineEdit = new QLineEdit();
     muF0LineEdit = new QLineEdit();
*/

//****************************************
//   control buttons:

     color=Qt::red;
     colorFrame=new QFrame();
     colorFrame->setFrameStyle(QFrame::Box);
//     colorFrame->resize(5,1.5);
     colorpal=colorFrame->palette();
     QBrush brush=QBrush(color);
     colorpal.setColor(QPalette::Background,color);
     colorpal.setBrush(QPalette::Background,brush);
     colorFrame->setAutoFillBackground(true);
     colorFrame->setPalette(colorpal);

     QPushButton *colorButton = new QPushButton(tr("Change Color"));
     QHBoxLayout *colorLayout = new QHBoxLayout();
     colorLayout->addWidget(colorButton);
     colorLayout->addWidget(colorFrame);
     colorGroupBox=new QGroupBox(tr(""));
     colorGroupBox->setLayout(colorLayout);

     QPushButton *delButton = new QPushButton(tr("Delete"));
     QPushButton *setButton = new QPushButton(tr("Set"));
     QPushButton *closeButton =new QPushButton(tr("Close"));
     QPushButton *helpButton  = new QPushButton(tr("Help"));

     QHBoxLayout *buttonLayout = new QHBoxLayout();
     buttonLayout->addWidget(setButton);
     buttonLayout->addWidget(delButton);
     buttonLayout->addWidget(closeButton);
     buttonLayout->addWidget(helpButton);
     buttonGroupBox=new QGroupBox(tr(""));
     buttonGroupBox->setLayout(buttonLayout);

     connect(colorButton, SIGNAL(clicked()), this, SLOT(setColorDialog()));

     connect(setButton, SIGNAL(clicked()), this, SLOT(set()));
     connect(delButton, SIGNAL(clicked()), this, SLOT(del()));
     connect(parent, SIGNAL(enterPressed()), this, SLOT(set()));
     connect(closeButton,  SIGNAL(clicked()), this, SLOT(accept()));
     connect(helpButton,  SIGNAL(clicked()), this, SLOT(help()));
     connect(epsTermsNumSB, SIGNAL( valueChanged (int) ), this, SLOT(setEpsTermsNum(int)) );
     connect(muTermsNumSB,  SIGNAL( valueChanged (int) ), this, SLOT(setMuTermsNum(int)) );

     setFocusPolicy(Qt::StrongFocus);

     SresGroupBox->hide();
//--------------------------------
     mainLayout = new QVBoxLayout(this);
     mainLayout->addWidget(nameGroupBox);
     mainLayout->addWidget(emGroupBox);
     mainLayout->addWidget(eDispGroupBox);
     mainLayout->addWidget(mDispGroupBox);
     mainLayout->addWidget(SresGroupBox);
     mainLayout->addWidget(colorGroupBox);
     mainLayout->addWidget(buttonGroupBox);
}


void DefineMaterialDialog::setColorDialog()
{
color=QColorDialog::getColor(color);
colorpal=colorFrame->palette();
QBrush brush=QBrush(color);
colorpal.setColor(QPalette::Background,color);
colorpal.setBrush(QPalette::Background,brush);
colorFrame->setAutoFillBackground(true);
colorFrame->setPalette(colorpal);
}

void DefineMaterialDialog::updateDisp(int state)
{
     if(state==Qt::Checked){
  	   eDispGroupBox->show();
  	   mDispGroupBox->show();
	   w=550; h=800;
     }else if(state==Qt::Unchecked){
	   eDispGroupBox->hide();
	   mDispGroupBox->hide();
           w=400; h=300;
     }
     QApplication::processEvents();
     window()->resize(w,h);
}

void DefineMaterialDialog::updateTanDelta(const QString & text)
{
 updateETanDelta(text);
 updateHTanDelta(text);
}
void DefineMaterialDialog::updateETanDelta(const QString & text)
{
 if(eTanDelta->checkState()==Qt::Checked) setConsTanDeltaLorentz('e');
}
void DefineMaterialDialog::updateHTanDelta(const QString & text)
{
 if(hTanDelta->checkState()==Qt::Checked) setConsTanDeltaLorentz('h');
}

void DefineMaterialDialog::updateETanDelta(int state)
{
   eTanDeltaLineEdit->setReadOnly(state!=Qt::Checked);
   for (int i = 0; i <epsTermsNumSB->value(); ++i) for (int j = 0; j <3; ++j){
       QLineEdit* le = (QLineEdit*) epsLorentz->cellWidget(i,j);
       le->setReadOnly(state==Qt::Checked);
   }
   if(state==Qt::Checked) setConsTanDeltaLorentz('e');
}

void DefineMaterialDialog::updateHTanDelta(int state)
{
   hTanDeltaLineEdit->setReadOnly(state!=Qt::Checked);
   for (int i = 0; i <muTermsNumSB->value(); ++i) for (int j = 0; j <3; ++j){
       QLineEdit* le = (QLineEdit*) muLorentz->cellWidget(i,j);
       le->setReadOnly(state==Qt::Checked);
   }
   if(state==Qt::Checked) setConsTanDeltaLorentz('h');
}



void DefineMaterialDialog::updateBoundaryCond(int state)
{
     if(state==Qt::Checked){
           SresGroupBox->show();
     }else if(state==Qt::Unchecked){
           SresGroupBox->hide();
     }
     QApplication::processEvents();
     window()->resize(w,h);
}


void DefineMaterialDialog::help()
{
    mainWindow->showDocumentation(QLatin1String("#2.3"));
/*
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText("Material setup");
    msgBox.setInformativeText(
            "The <b>Electromagnetic CAD</b> is an user interface "
                "for the definition of an electromagnetic problem in a file format "
		"compatible with the Hierachical Electromagnetic Modeler.\n"
		"The geometrical structure should have been previously defined "
	        "by means of a CAD tool and imported in this framework as a STEP file.\n"
		"Here the user should specify all the data (material properties, "
		"excitation ports, frequency bands..) which are required for "
		"the proper definitopn of the electromagnetic problem."
               );
    msgBox.exec();
*/
}

void DefineMaterialDialog::getMaterialData(QString matname){
  if(matname.isEmpty()) return;
  DB::Material* mat = mainOCAF->EmP.FindMaterial(matname.toLatin1().data());
  if(!mat) return;
  nameLineEdit->setText(matname);
  epsLineEdit->setText(QString("%1").arg(mat->epsr, 0, 'f', 5));
  sigmaLineEdit->setText(QString("%1").arg(mat->econductivity, 0, 'e', 5));
  muLineEdit->setText(QString("%1").arg(mat->mur, 0, 'f', 5));
  sigmamLineEdit->setText(QString("%1").arg(mat->hconductivity, 0, 'e', 5));
  epsTermsNumSB->setValue(mat->epsLorentz.n);
  muTermsNumSB->setValue(mat->muLorentz.n);
  eTanDeltaLineEdit->setText(QString("%1").arg(mat->etandelta, 0, 'f', 5));
  hTanDeltaLineEdit->setText(QString("%1").arg(mat->htandelta, 0, 'f', 5));
  eTanDelta->setCheckState(mat->etandelta>0 ? Qt::Checked : Qt::Unchecked);
  hTanDelta->setCheckState(mat->htandelta>0 ? Qt::Checked : Qt::Unchecked);
  f1LineEdit->setText(QString("%1").arg(mat->freqBand[0]/prjData.freqUnit(), 0, 'f', 5));
  f2LineEdit->setText(QString("%1").arg(mat->freqBand[1]/prjData.freqUnit(), 0, 'f', 5));
  if(mat->epsLorentz.n>0 || mat->muLorentz.n>0){
     dispersive->setCheckState(Qt::Checked);
     for (int i = 0; i <mat->epsLorentz.n; ++i) for (int j = 0; j <3; ++j){
         QLineEdit* le = (QLineEdit*) epsLorentz->cellWidget(i,j);
         le->setText(QString("%1").arg((&mat->epsLorentz[i])[j], 0, 'e', 5));
     }
     for (int i = 0; i <mat->muLorentz.n; ++i) for (int j = 0; j <3; ++j){
         QLineEdit* le = (QLineEdit*) muLorentz->cellWidget(i,j);
         le->setText(QString("%1").arg((&mat->epsLorentz[i])[j], 0, 'e', 5));
     }
  } else dispersive->setCheckState(Qt::Unchecked);
  SresLineEdit->setText(QString("%1").arg(mat->Sresistance, 0, 'f', 5));
  color.setRgb(mat->color[0],mat->color[1],mat->color[2],mat->color[3] );
  colorpal=colorFrame->palette();
  QBrush brush=QBrush(color);
  colorpal.setColor(QPalette::Background,color);
  colorpal.setBrush(QPalette::Background,brush);
  colorFrame->setAutoFillBackground(true);
  colorFrame->setPalette(colorpal);
//  epsNumLineEdit->setText(QString("%1").arg(mat->epsLorentz.n));
//  muNumLineEdit->setText(QString("%1").arg(mat->muLorentz.n));
}


void DefineMaterialDialog::setConsTanDeltaLorentz(char type){
 int N=epsTermsNumSB->value();
 double freqBand[2];
 freqBand[0]=f1LineEdit->text().toDouble()*prjData.freqUnit();
 freqBand[1]=f2LineEdit->text().toDouble()*prjData.freqUnit();
 if(freqBand[1]<=0) return;
 if(freqBand[2]<=0) return;
 double tandelta;
 double r;
 if(type=='e'){
       tandelta=eTanDeltaLineEdit->text().toDouble();
       r=epsLineEdit->text().toDouble();
 }else{
       tandelta=hTanDeltaLineEdit->text().toDouble();
       r=muLineEdit->text().toDouble();
 }
 double k=pow(freqBand[1]/freqBand[0],1.0/N);
 double nu=1.0/(1-2*atan(tandelta)/PIG);
 double a=0.0;
 for (int i = 0; i <N; ++i) {
    a+=(pow(k,i*nu-N/2)-tandelta*pow(k,i*(nu-1)))/(1+pow(k,2.0*i-N));
 }
 a=tandelta/a;
 auto lorentz=new double[N][3]();
 for (int i = 0; i <N; ++i) {
     double term[3];
     lorentz[i][0] =a*r*pow(k,i*(nu-1));                      // depsr/dmur
     lorentz[i][1] =freqBand[1]/pow(k,i*nu);                  //frelax
     lorentz[i][2] =0.0;                                      //freson
 }
 for (int i = 0; i <N; ++i) {
     for (int j = 0; j <3; ++j){
      QLineEdit* le;
      if(type=='e') le= (QLineEdit*)  epsLorentz->cellWidget(i,j);
      else          le= (QLineEdit*)  muLorentz->cellWidget(i,j);
      le->setText(QString("%1").arg(lorentz[i][j], 0, 'e', 5));
     }
 }
 delete[] lorentz;
}

void DefineMaterialDialog::setMaterialData(DB::Material* mat){
  mat->epsr=epsLineEdit->text().toDouble();
  mat->econductivity=sigmaLineEdit->text().toDouble();
  mat->mur=muLineEdit->text().toDouble();
  mat->hconductivity=sigmamLineEdit->text().toDouble();
  mat->freqBand[0]=f1LineEdit->text().toDouble()*prjData.freqUnit();
  mat->freqBand[1]=f2LineEdit->text().toDouble()*prjData.freqUnit();
  if(dispersive->checkState()==Qt::Checked){
     mat->etandelta=eTanDelta->checkState()==Qt::Checked ? eTanDeltaLineEdit->text().toDouble() : 0.0;
     mat->htandelta=hTanDelta->checkState()==Qt::Checked ? hTanDeltaLineEdit->text().toDouble() : 0.0;
     mat->epsLorentz.resize(epsTermsNumSB->value());
     for (int i = 0; i <mat->epsLorentz.n; ++i) for (int j = 0; j <3; ++j){
         QLineEdit* le = (QLineEdit*) epsLorentz->cellWidget(i,j);
	 (&mat->epsLorentz[i])[j]=le->text().toDouble();
     }
     mat->muLorentz.resize(muTermsNumSB->value());
     for (int i = 0; i <mat->muLorentz.n; ++i) for (int j = 0; j <3; ++j){
         QLineEdit* le = (QLineEdit*) muLorentz->cellWidget(i,j);
	 (&mat->muLorentz[i])[j]=le->text().toDouble();
     }
  } else{
    mat->etandelta=0.0;
    mat->htandelta=0.0;
  }
  mat->Sresistance=SresLineEdit->text().toDouble();
  color.getRgb(&mat->color[0],&mat->color[1],&mat->color[2],&mat->color[3]);
}

void DefineMaterialDialog::set(){
	QString matname=nameLineEdit->text();
	if(matname.isEmpty()) return;
	DB::Material* mat =mainOCAF->EmP.FindMaterial(matname.toLatin1().data());
	if(!mat){ 
	   mat =new DB::Material();
           strcpy(mat->name,matname.toLatin1().data());
	   mainOCAF->EmP.addMaterial(mat);
           TreeWidgetItem* matit=treeWidget->getItemFromLabel(mainOCAF->theMaterials);
	   if(matit) treeWidget->makeMaterials(matit);
	}
	setMaterialData(mat);
	mainWindow->saveMaterials();
	mainOCAF->updatePartColors();
        prjData.workStatus.materialChanged=1;
        prjData.workStatus.componentsaveNeeded=1;
}

void DefineMaterialDialog::del(){
	QString matname=nameLineEdit->text();
	if(matname.isEmpty()) return;
	DB::Material* mat =mainOCAF->EmP.FindMaterial(matname.toLatin1().data());
	if(mat) mainOCAF->EmP.delMaterial(mat);
	else {
	     mainWindow->temporaryMessage(tr("Material ") + matname + tr(" is not present"));
             return;
  	}
        TreeWidgetItem* matit=treeWidget->getItemFromLabel(mainOCAF->theMaterials);
	if(matit) treeWidget->makeMaterials(matit);
}

void DefineMaterialDialog::setEpsTermsNum(int n){
	epsLorentz->setRowCount(n);
        for (int i = 0; i <n; ++i) for (int j = 0; j <3; ++j){
              QLineEdit* le = new QLineEdit();
	      le->setValidator(dvalidator);
              le->setReadOnly(eTanDelta->checkState()==Qt::Checked);
              epsLorentz->setCellWidget(i,j,le);
	}
	if(eTanDelta->checkState()==Qt::Checked) setConsTanDeltaLorentz('e');
}
void DefineMaterialDialog::setMuTermsNum(int n){
	muLorentz->setRowCount(n);
        for (int i = 0; i <n; ++i) for (int j = 0; j <3; ++j){
              QLineEdit* le = new QLineEdit();
	      le->setValidator(dvalidator);
              le->setReadOnly(hTanDelta->checkState()==Qt::Checked);
              muLorentz->setCellWidget(i,j,le);
	}
	if(hTanDelta->checkState()==Qt::Checked) setConsTanDeltaLorentz('h');
}


void TreeWidget::setCompPropertiesDialog()
{    
     SetCompPropertiesDialog *dialog=new SetCompPropertiesDialog(this);
     dialog->treeWidget=this;
     dialog->getVolumeData(currentPartName);
     connect( this, SIGNAL( sendPartName( QString) ),\
	      dialog, SLOT( getVolumeData( QString) )  );
     dialog->show();
}




QString genericName(QString name);

void ImportPartPropertiesDialog::getVolumeData(QString volname){
  if(volname.isEmpty()) return;
  DB::Volume* vol = mainOCAF->EmP.FindVolume(volname.toLatin1().data());
  if(!vol) return;
  nameLineEdit->setText(volname);
  QString filter1=genericName(volname)+".emc";
  QString filter2=genericName(volname)+"_*.emc";
  QString filters="("+filter1+" "+filter2+")";
  fdialog->setNameFilter(filters);
}

void ImportPartPropertiesDialog::help()
{
    mainWindow->showDocumentation(QLatin1String("#2.5.1"));
}

QSize  ImportPartPropertiesDialog::sizeHint() const
{
     QSize size = QSize(w,h);
     return size;
}

ImportPartPropertiesDialog::ImportPartPropertiesDialog(TreeWidget *parent) : QDialog(parent)
{
     setModal(0);
     setWindowTitle(tr("Import Part Properties"));
//
     QLabel *nameLabel= new QLabel(); nameLabel->setText(tr("Part Name"));
     nameLineEdit = new QLineEdit();
     nameLineEdit->setReadOnly(true);
     w=800; h=800;
     window()->resize(w,h);

     QGridLayout *nameLayout = new QGridLayout();
     nameLayout->setColumnMinimumWidth(1,20);
     nameLayout->addWidget(nameLabel, 0, 0);
     nameLayout->addWidget(nameLineEdit,0, 2);

     nameGroupBox=new QGroupBox(tr(""));
     nameGroupBox->setLayout(nameLayout);

//****************************************
//   input file:

     fdialog= new QFileDialog(this);
     fdialog->setDirectory(QDir::currentPath());
     fdialog->setNameFilter(tr("EMCAD (*.emc)"));
     fdialog->setViewMode(QFileDialog::List);
     QString	fileName;
/*     if(fdialog->exec()){
	  QStringList fileNames = fdialog->selectedFiles();
	  fileName= fileNames.at(0);
	  QFileInfo fileinfo(fileName);
     }
*/
     QVBoxLayout *fileLayout = new QVBoxLayout();
     fileLayout->addWidget(fdialog);
     fileGroupBox=new QGroupBox(tr(""));
     fileGroupBox->setLayout(fileLayout);

//****************************************
//   control buttons:
//
     QPushButton *helpButton  = new QPushButton(tr("Help"));


     QHBoxLayout *buttonLayout = new QHBoxLayout();
     buttonLayout->addWidget(helpButton);
     buttonGroupBox=new QGroupBox(tr(""));
     buttonGroupBox->setLayout(buttonLayout);

     connect(fdialog, SIGNAL(fileSelected (const QString &)), this, SLOT(importEMC(const QString &)));
     connect(fdialog, SIGNAL(accepted()), this, SLOT(accept()));
     connect(fdialog, SIGNAL(rejected()), this, SLOT(reject()));
     connect(helpButton,   SIGNAL(clicked()), this, SLOT(help()));

     setFocusPolicy(Qt::StrongFocus);

//--------------------------------
     mainLayout = new QVBoxLayout(this);
     mainLayout->addWidget(nameGroupBox);
     mainLayout->addWidget(fileGroupBox);
     mainLayout->addWidget(buttonGroupBox);

}


void ImportPartPropertiesDialog::importEMC(const QString & fileName){
  QString partName=nameLineEdit->text();
  mainWindow->importEMC(partName, fileName);
  prjData.workStatus.materialChanged=true;
  mainOCAF->setPartsStatus();
  QFileInfo fileinfo(fileName);
  QDir::setCurrent(fileinfo.dir().absolutePath());
  accept();
}

void TreeWidget::atImportedPartProperties()
{    
  setItemText(currentPart);
}


void TreeWidget::importPartPropertiesDialog()
{    
   ImportPartPropertiesDialog *dialog=new ImportPartPropertiesDialog(this);
   dialog->treeWidget=this;
   dialog->getVolumeData(currentPartName);
   connect( this, SIGNAL( sendPartName( QString) ),\
	      dialog, SLOT( getVolumeData( QString) )  );
   connect(dialog, SIGNAL(accepted()), this, SLOT(atImportedPartProperties()));
   dialog->show();
}


void TreeWidget::defineMaterialDialog()
{    
     DefineMaterialDialog *dialog=new DefineMaterialDialog(this);
     dialog->treeWidget=this;
     dialog->getMaterialData(currentMaterialName);
     connect( this, SIGNAL( sendMaterialName( QString) ),\
	      dialog, SLOT( getMaterialData( QString) )  );
     dialog->show();
//     setFocusProxy(dialog);
}


void TreeWidget::setCurrentItem(QTreeWidgetItem * qitem, int column){
   TreeWidgetItem *item= (TreeWidgetItem *) qitem;
   DB::Material* mat;
   TDF_Label label;
   item->getLabel(label);
   if(!label.IsNull()) if(mainOCAF->isPart(label)){
       currentPart=item;
       getLabelName(label, currentPartName);
       emit sendPartName(currentPartName);
       return;
   } 
   if(!label.IsNull()) if(mainOCAF->isLayer(label)){ 
       currentLayer=item;
       getLabelName(label, currentLayerName);
       emit sendLayerName(currentLayerName);
       return;
   }
   if(item->getMaterial(mat)){
      currentMaterial=item;
      currentMaterialName=mat->name;
      emit sendMaterialName(currentMaterialName);
      return;
   }
}
void TreeWidget::setCurrentItem(QTreeWidgetItem * current, QTreeWidgetItem * previous){
   TreeWidgetItem *item= (TreeWidgetItem *) current;
   DB::Material* mat;
   TDF_Label label;
   item->getLabel(label);
   if(!label.IsNull()) if(mainOCAF->isComponent(label) || mainOCAF->isSolid(label)){
       currentPart=item;
       getLabelName(label, currentPartName);
       emit sendPartName(currentPartName);
       return;
   } 
   if(!label.IsNull()) if(mainOCAF->isLayer(label)){ 
       currentLayer=item;
       getLabelName(label, currentLayerName);
       emit sendLayerName(currentLayerName);
       return;
   }
   if(item->getMaterial(mat)){
      currentMaterial=item;
      currentMaterialName=mat->name;
      emit sendMaterialName(currentMaterialName);
      return;
   }
}

void TreeWidget::assignLayerDialog()
{    
//     bool ok;
//     QString text = QInputDialog::getText(this, tr("Setting Item Property"),
//                                          tr("enter property name:"), QLineEdit::Normal,
//                                          tr(""), &ok);
//     if(!ok) return;
     AssignDialog *dialog=new AssignDialog(this);
     dialog->label1->setText(tr("Solid:"));
     dialog->label2->setText(tr("Layer:"));
     connect( this, SIGNAL( sendLayerName( QString) ),\
	      dialog->lineEdit2, SLOT( setText( QString ) )  );
     connect(dialog, SIGNAL(assigned()), this, SLOT(assignLayer()));
     dialog->setWindowTitle(tr("Assign Layer"));
     dialog->show();
//     setFocusProxy(dialog);
}

void TreeWidget::assignMaterialDialog()
{    
     AssignDialog *dialog=new AssignDialog(this);
     dialog->label1->setText(tr("Solid:"));
     dialog->label2->setText(tr("Material:"));
     dialog->lineEdit1->setText(currentPartName);
     connect( this, SIGNAL( sendPartName( QString) ),\
	      dialog->lineEdit1, SLOT(setText( QString ) )  );
     connect( this, SIGNAL( sendMaterialName( QString) ),\
	      dialog->lineEdit2, SLOT( setText( QString ) )  );
     connect(dialog, SIGNAL(assigned()), this, SLOT(assignMaterial()));
     dialog->setWindowTitle(tr("Assign Material"));
     dialog->show();
//     setFocusProxy(dialog);
}


ShowWgModesDialog::ShowWgModesDialog(TreeWidget *parent) : QDialog(parent)
{
     setModal(0);
     textEdit=new QPlainTextEdit();
     textEdit->setReadOnly(true);
     closeButton = new QPushButton(tr("Close"));
     connect(closeButton,  SIGNAL(clicked()), this, SLOT(accept()));
     layout = new QVBoxLayout(this);
     layout->addWidget(textEdit);
     layout->addWidget(closeButton);
     setFocusPolicy(Qt::StrongFocus);
}

extern QString nativePath(QString path);
void  TreeWidget::showWgModes()
{    
     std::string assName; mainOCAF->getAssName(assName);
     QString fname=mainWorkPath+"/Data/Circuits/"+assName.c_str()+"__"+currentPartName+".port";
     char cfname[256];
     strcpy(cfname,fname.toLatin1().data());
     FILE *in= fopen(nativePath(fname).toLatin1().data(), "r");
     if(!in) return;
     ShowWgModesDialog *dialog=new ShowWgModesDialog(this);
     QString title="Port Modes of "+currentPartName;
     dialog->setWindowTitle(title);
     for (int mt = 0; mt<3; ++mt)  {
	 int Nm;
	 char tag[5];
	 fscanf(in, "%d %s", &Nm, tag);
	 bool isTEM=!strcmp(tag,"TEM");
	 QString line;
	 if(isTEM)  line="\n"+QString(tag)+" MODES IMPEDANCES:";
	 else	    line="\n"+QString(tag)+" MODES CUT OFF FREQUENCIES:";
         if(Nm>0) dialog->textEdit->appendPlainText ( line );
         for (int i = 0; i<Nm; ++i){
	   double val; fscanf(in, "%lf", &val);
	   if(isTEM){
              line=QString("   %1: ").arg(i+1)+QString("%1 ").arg(val, 0, 'f', 5)+QString("ohm");
	   } else {
	      val/=prjData.freqUnit();
              line=QString("   %1: ").arg(i+1)+QString("%1 ").arg(val, 0, 'f', 5)+prjData.freqUnitName();
	   }
           dialog->textEdit->appendPlainText ( line );
         }
     }
     fclose(in);
     dialog->textEdit->show();
     dialog->show();
//     setFocusProxy(dialog);
}



void TreeWidget::openSubAssembly()
{    
     mainWindow->openPart();
}

void TreeWidget::displaySelectedMesh()
{
     QTreeWidgetItemIterator qit(this, QTreeWidgetItemIterator::Selected);
     while (*qit) {
         TreeWidgetItem* it=(TreeWidgetItem *) (*qit);
	 displayItemMesh(it, true);
         ++qit;
     }
}


void TreeWidget::hideSelectedMesh()
{
     QTreeWidgetItemIterator qit(this, QTreeWidgetItemIterator::Selected);
     while (*qit) {
         TreeWidgetItem* it=(TreeWidgetItem *) (*qit);
	 displayItemMesh(it, false);
         ++qit;
     }
}




void TreeWidgetItem::displayShape(bool B)
{
 TDF_Label label;
 if(getLabel(label)) mainOCAF->displayLabelShape(label, B);
}


void TreeWidgetItem::displayMesh(bool B)
{
 TDF_Label label;
 if(getLabel(label)) mainOCAF->displayLabelMesh(label, B);
}




void TreeWidget::clear()
{
// QTreeWidget::clear();
 invisibleRootItem()->takeChildren();
 currentItem=NULL;
 lastClicked=NULL;
 lastPressed=NULL;
 lastEntered=NULL;
 maxIndex=0;
 currentMaterialName=tr("");
 currentPartName=tr("");
 currentLayerName=tr("");
}


void TreeWidget::unhighlightCurrentItem (){
  TDF_Label label;
  if(getLabel(currentItem,label)) mainOCAF->highlightLabel(label, false);
  (mainOCC->getContext())->Select(); //to clear also the Viewer Selection
}


void TreeWidget::highlightItem (QTreeWidgetItem * qitem, int column ){
  if(currentItem)  unhighlightCurrentItem();
  TDF_Label label;
  TreeWidgetItem * item =(TreeWidgetItem *) qitem;
  if(getLabel(item,label)) mainOCAF->highlightLabel(label, true);
  currentItem=item;
}


void TreeWidget::leaveEvent ( QEvent * event ){
 if(currentItem)  unhighlightCurrentItem ();
}

bool neverDisplay(TDF_Label label){
     return !mainOCAF->theParts.IsNull()
         && ( label==mainOCAF->theShapes || label.Father()==mainOCAF->theShapes && !mainOCAF->isPart(label) );
}

void TreeWidget::openItem (QTreeWidgetItem * qitem){
  TreeWidgetItem * item =(TreeWidgetItem *) qitem;
  TDF_Label label;
  if(!getLabel(item,label)) return;
  if (!mainOCAF->isShape(label)) return;
  if(neverDisplay(label)) return;
  if(!mainOCAF->isFace(label)) item->displayShape(false);
  for (int i = 0; i < item->childCount(); ++i)  {
        TreeWidgetItem * child =(TreeWidgetItem *) item->child(i);
        Qt::CheckState state = child->checkState(0);
        TDF_Label clabel;
        if(getLabel(child,clabel))
	  if(state==Qt::Checked) mainOCAF->displayLabelShape(clabel, true);
  }
  mainOCC->redraw();
//  emit AISchanged();
}

void TreeWidget::displayItemShape(QTreeWidgetItem * qitem){
  int done=0;
  TreeWidgetItem * item =(TreeWidgetItem *) qitem;
  TDF_Label label;
  if(getLabel(item,label))
     if (mainOCAF->isShape(label))	{mainOCAF->displayLabelShape(label, true); done=1;}
  if(!done)  
     for (int i = 0; i < item->childCount(); ++i)  displayItemShape(item->child(i));
  mainOCC->redraw();
//  emit AISchanged();
}

void TreeWidget::displayItemMesh(QTreeWidgetItem * qitem, bool B){
  int done=0;
  TreeWidgetItem * item =(TreeWidgetItem *) qitem;
  TDF_Label label;
  if(getLabel(item,label))
     if (mainOCAF->isShape(label))
       if (mainOCAF->isFace(label))    {mainOCAF->displayLabelMesh(label, B); done=1;}
  if(!done)  
     for (int i = 0; i < item->childCount(); ++i)  displayItemMesh(item->child(i), B);
  mainOCC->redraw();
}

void TreeWidget::closeItem (QTreeWidgetItem * qitem){
  TreeWidgetItem * item =(TreeWidgetItem *) qitem;
  Qt::CheckState state = item->checkState(0);
  TDF_Label label;
  if(!getLabel(item,label)) return;
  if (!mainOCAF->isShape(label)) return;
  if(neverDisplay(label)) return;
  if(state==Qt::Checked) item->displayShape(true);
  for (int i = 0; i < item->childCount(); ++i)  {
        TreeWidgetItem * child =(TreeWidgetItem *) item->child(i);
        child->displayShape(false);
  }
  mainOCC->redraw();
//  emit AISchanged();
}


void TreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
//    menu.addAction(aboutAction);
    TDF_Label selectedLabel;
    if(lastClicked)  lastClicked->getLabel(selectedLabel);
    if(!selectedLabel.IsNull()) if(mainOCAF->EmP.assemblyType!=PARTITION) if(mainOCAF->isComponent(selectedLabel) || mainOCAF->isSolid(selectedLabel)) 
    { 
      QString qtext;
      getLabelName(selectedLabel, qtext);
      char *ctext=qtext.toLatin1().data();
      DB::Volume *vol=mainOCAF->EmP.FindVolume(qtext.toLatin1().data());
      if(vol){ 
	 if(mainOCAF->EmP.assemblyType==COMPONENT) {
	     if (vol->type==DIELECTRIC || vol->type==BOUNDARYCOND || vol->type==WAVEGUIDE) menu.addAction(assignMaterialAction);
             menu.addAction(setCompPropertiesAction);
             if (vol->type==WAVEGUIDE) menu.addAction(showWgModesAction);
         } else { 
	     menu.addAction(importPartPropertiesAction);
	 }
      }
      if(showlayers)                        menu.addAction(assignLayerAction);
      if(mainOCAF->EmP.assemblyType==NET)   menu.addAction(openSubAssAction);
      menu.exec(event->globalPos());
    }
//    menu.addAction(showMeshAction);
//    menu.addAction(hideMeshAction);
}

void TreeWidgetItem::setCheck(int column )
{
   Qt::CheckState state = checkState(column);
   TDF_Label label; this->getLabel(label);
   if(label.IsNull()) return;
   if(neverDisplay(label) || (isExpanded())) return;
   displayShape(state==Qt::Checked);
};


void TreeWidget::setCheck(QTreeWidgetItem * qitem, int column )
{
   Qt::CheckState state = qitem->checkState(column);
   TreeWidgetItem* item=(TreeWidgetItem*) qitem;
   item->setCheck(column);
   mainOCC->redraw();
};

void TreeWidget::getOcafSelection ()
{
    TCollection_AsciiString name;
    QList<QTreeWidgetItem *> slist=selectedItems();
    for (int i = 0; i < slist.size(); ++i) slist.at(i)->setSelected(false);
    if(mainOCAF->theSelectedLabels){
       int Neq=mainOCAF->theSelectedLabels->Length();
       for (int i = 1; i <= mainOCAF->theSelectedLabels->Length(); i++){
        TDF_Label leq=mainOCAF->theSelectedLabels->Value(i);
        TreeWidgetItem *item= getItemFromLabel(leq);
//        setCurrentItem(item);
        if(item) item->setSelected(true);
       }
    }
}



/*
void TreeWidget::mousePressEvent( QMouseEvent * event )
{
 QModelIndex index=indexAt(event->pos());
 if(!index.isvalid()) clickedItem=itemFromIndex(index);
 QTreeWidget::mousePressEvent(event );
}
*/
