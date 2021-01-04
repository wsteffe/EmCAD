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

#include "mainwindow.h"
#include <QApplication>
#include "projectData.h"
#include "TreeWidget.h"
#include "ViewWidget.h"
#include "ocaf.h"
//#include "Tools.h"

#include <OStools.h>
#include <complex>

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


#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif


extern ViewWidget*   mainOCC;
extern MwOCAF*       mainOCAF;
extern MainWindow *mainWindow;
extern ProjectData prjData;
extern Documentation documentation;
extern QString    mainWorkPath;

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
      currentSubComp=0;

//      setFocusPolicy(Qt::StrongFocus);

      setColumnCount(1);
      setHeaderLabel("");
      maxIndex=0;
      indexedItem =(TreeWidgetItem**) NULL;

      aboutAction = new QAction(tr("&About"), this);
      aboutAction->setStatusTip(tr("Show the application's About box"));
      connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
//      connect(aboutAction, SIGNAL(triggered()), this, SIGNAL(buttonClicked()));

      openCompOrPartitionAction = new QAction(tr("&Open Component OR Partition"), this);
      connect(openCompOrPartitionAction, SIGNAL(triggered()), this, SLOT(openCompOrPartition()));

      openCompAndPartitionAction = new QAction(tr("&Open Component AND Partition"), this);
      connect(openCompAndPartitionAction, SIGNAL(triggered()), this, SLOT(openCompAndPartition()));

      assignLayerAction = new QAction(tr("&Set Layer"), this);
      assignLayerAction->setStatusTip(tr("Assign a Layer to the Selected Part "));
      connect(assignLayerAction, SIGNAL(triggered()), this, SLOT(assignLayerDialog()));

      assignMaterialAction = new QAction(tr("&Set Material"), this);
      assignMaterialAction->setStatusTip(tr("Assign a Material to the Selected Part "));
      connect(assignMaterialAction, SIGNAL(triggered()), this, SLOT(assignMaterialDialog()));

      importCompPropertiesAction = new QAction(tr("&Import Component Properties"), this);
      importCompPropertiesAction->setStatusTip(tr("Import properties of the Selected Component "));
      connect(importCompPropertiesAction, SIGNAL(triggered()), this, SLOT(importCompPropertiesDialog()));

      setCompPropertiesAction = new QAction(tr("&Set Object properties"), this);
      setCompPropertiesAction->setStatusTip(tr("Set properties of the Selected Object "));
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
     bool isShell=false;
     bool isShape=false;
     bool isGeomPart=false;
     bool isLayer=false;
     bool isLayers=false;
     bool isFace=false;
     bool isEdge=false;
     bool isVertex=false;
     bool isMaterial=false;
     bool TBD=false;
     DB::Volume *vol;
     if(item->getLabel(label)){ // item is defined from OCAF label
       getLabelName(label, qtext);
       const char* txt=qtext.toLatin1().data();
       isSolid=mainOCAF->isSolid(label);
       isShell=mainOCAF->isShell(label);
       isLayer=mainOCAF->isLayer(label);
       isLayers=mainOCAF->isLayers(label);
       isFace =mainOCAF->isFace(label);
       isEdge =mainOCAF->isEdge(label);
       isVertex =mainOCAF->isVertex(label);
       isShape =mainOCAF->isShape(label);
       isGeomPart=mainOCAF->isPart(label);
       if(isGeomPart) vol=mainOCAF->EmP->FindVolume(qtext.toLatin1().data());
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
	 QBrush redbrush=QBrush(QColor(255, 0, 0, 127));
	 if(vol->type==DIELECTRIC || vol->type==BOUNDARYCOND){
           qtext.append(" [ ");
	   if(strcmp(vol->material,"?")) qtext.append(vol->material);
           qtext.append(" ]");
	 }
//         if(vol->type==ASSEMBLY)     qtext.append("CMP");
//         if(vol->type==COMPONENT)    qtext.append("CMP");
//         if(vol->type==INTERFACE)    qtext.append("IF");
//         if(vol->type==WAVEGUIDE)    qtext.append("MAT ");
//         if(vol->type==LINEPORT)     qtext.append("LP ");
//         if(vol->type==DIELECTRIC)   qtext.append("MAT ");
//         if(vol->type==HOLE)         qtext.append("HOLE ");
//         if(vol->type==BOUNDARYCOND) qtext.append("MAT ");
         if(vol->type==SPLITTER)     qtext.append(" -|-");
         if(vol->type==GRID)         qtext.append(" |||");
         if(vol->type==GRID && vol->gridNum) qtext.append(QString(" %1").arg(vol->gridNum));
#if defined(EXPLICIT_INVARIANT)
         if(vol->type==GRID && vol->invariant) qtext.append(" INVAR");
#endif
         if(vol->type==GRID && vol->PML) qtext.append(" PML");
         if(vol->type!=SPLITTER)     if(!vol->defined) item->setBackground ( 0, redbrush );
	                             else              item->setBackground ( 0, Qt::NoBrush );
         if(vol->type==WAVEGUIDE)    if(!prjData.workStatus.decompositionNeeded && !vol->defined) item->setBackground ( 0, redbrush );
	                             else                                                         item->setBackground ( 0, Qt::NoBrush );
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
//         TBD=!mainOCAF->EmP->FindMaterial(qtext.toLatin1().data());
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
       QString qtext; getLabelName(label, qtext);
       skip = skip || (qtext.length()==0);
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
  DB::List_T *mats = Tree2List(mainOCAF->EmP->materials);
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

extern DB::Material theDefaultMaterial;

void TreeWidget::setDefaultBC(){
  if(currentMaterialName.isEmpty()) return;
  if(!strcmp(theDefaultMaterial.name,currentMaterialName.toLatin1().data())) return;
  DB::Material* mat = mainOCAF->EmP->FindMaterial(currentMaterialName.toLatin1().data());
  if(!mat) return;
  if( !strcmp(currentMaterialName.toLatin1().data(),"PEC") || 
        !strcmp(currentMaterialName.toLatin1().data(),"PMC") ||
	mat->Sresistance >0 ||
	mat->Sinductance >0
     ){
     theDefaultMaterial=*mat;
     strcpy(theDefaultMaterial.name,mat->name);
     strcpy(mainOCAF->EmP->defaultBC,currentMaterialName.toLatin1().data());
     prjData.workStatus.decompositionNeeded=true;
     mainWindow->recursiveAssignDefaultMaterial();
   }else
     mainWindow->temporaryMessage(currentMaterialName + tr(" can not be assigned as Default BC"));
}

void TreeWidget::assignMaterial(){
  DB::Volume *vol=mainOCAF->EmP->FindVolume(currentPartName.toLatin1().data());
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


SetDefaultBCDialog::SetDefaultBCDialog(TreeWidget *parent) : QDialog(parent)
{
     setModal(0);
     label2= new QLabel();
     lineEdit2 = new QLineEdit();
     lineEdit2->setReadOnly(true);
     assignButton = new QPushButton(tr("Assign"));
     assignButton->setShortcut(tr("Enter"));
     closeButton = new QPushButton(tr("Close"));
     layout = new QHBoxLayout(this);
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
     setWindowTitle(tr("Object Properties"));
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
     typeChooser = new QLineEdit();
     typeChooser->setReadOnly(true);

     typeChooserMap[COMPONENT]=std::string("Component");
     typeChooserMap[DIELECTRIC]=std::string("Dielectric");
     typeChooserMap[HOLE]=std::string("Hole");
     typeChooserMap[WAVEGUIDE]=std::string("WG Port");
     typeChooserMap[LINEPORT]=std::string("Line Port");
     typeChooserMap[SPLITTER]=std::string("Splitter");
     typeChooserMap[GRID]=std::string("Grid");
     typeChooserMap[BOUNDARYCOND]=std::string("Boundary Cond");

/*     
     int tI=0;
     if(mainOCAF->EmP->assemblyType==NET){
       typeChooser->addItem(tr("Component"));       typeChooserMap[tI++]=ASSEMBLY;
     }
     else if(mainOCAF->EmP->assemblyType==COMPONENT || mainOCAF->isPartition() || mainOCAF->EmP->assemblyType==INTERFACE){
       typeChooser->addItem(tr("Dielectric"));      typeChooserMap[tI++]=DIELECTRIC;
       typeChooser->addItem(tr("Hole"));            typeChooserMap[tI++]=HOLE;
       typeChooser->addItem(tr("Waveguide"));       typeChooserMap[tI++]=WAVEGUIDE;
       typeChooser->addItem(tr("Line-Port"));       typeChooserMap[tI++]=LINEPORT;
       typeChooser->addItem(tr("Splitter"));        typeChooserMap[tI++]=SPLITTER;
       typeChooser->addItem(tr("Grid"));            typeChooserMap[tI++]=GRID;
     }
     typeChooser->addItem(tr("Boundary Cond"));   typeChooserMap[tI++]=BOUNDARYCOND;

     connect(typeChooser, SIGNAL(  currentIndexChanged  (int) ), this, SLOT(updateType(int)) );
*/

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
     gridSpaceLabel->setText(tr("Division Number:"));
     gridNumSB = new QSpinBox();
     gridNumSB->setValue(1);
     gridNumSB->setMinimum(1);

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

     if(mainOCAF->EmP->assemblyType==NET) meshGroupBox->hide();


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


void SetCompPropertiesDialog::updateType(int t)
{
    typeChooser->setText(tr(typeChooserMap[t].c_str()));
    if(t==WAVEGUIDE){
  	 WgGroupBox->show();
  	 meshGroupBox->show();
	 w=400; h=420;
    }else{
  	 WgGroupBox->hide();
	 w=400; h=250;
    }
    if(t==DIELECTRIC){
  	 meshGroupBox->show();
	 w=400; h=250;
    }
    if(t==HOLE){
  	 meshGroupBox->show();
	 w=400; h=250;
    }
    if(t==LINEPORT){
  	 LPGroupBox->show();
	 w=400; h=250;
    }
    if(t==GRID){
  	 GridGroupBox->show();
	 w=400; h=290;
    }
    if(t!=WAVEGUIDE && t!=DIELECTRIC && t!=HOLE && t!=BOUNDARYCOND){
  	 meshGroupBox->hide();
    }
    QApplication::processEvents();
    window()->resize(w,h);
}

void SetCompPropertiesDialog::getVolumeData(QString volname){
  if(volname.isEmpty()) return;
  DB::Volume* vol = mainOCAF->EmP->FindVolume(volname.toLatin1().data());
  if(!vol) return;
  QString TEMnum; TEMnum.setNum(vol->TEMportsNum);
  TEMnumLE->setText(TEMnum);
  TEnumSB->setValue(vol->TEportsNum);
  TMnumSB->setValue(vol->TMportsNum);
  updateType(vol->type);
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
//  vol->type=typeChooserMap[typeChooser->currentIndex()];
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
           mainOCAF->setPartsStatus();
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
      	 mainOCAF->worksaveNeeded=true;
         mainWindow->checkActions();
  }
}

void SetCompPropertiesDialog::set(){
	QString volname=nameLineEdit->text();
	if(volname.isEmpty()) return;
	DB::Volume* vol =mainOCAF->EmP->FindVolume(volname.toLatin1().data());
	if(!vol) return; 
	setVolumeData(vol);
        treeWidget->updateText();
}

void SetCompPropertiesDialog::help()
{
    documentation.showDocumentation(QLatin1String("#2.4.1"));
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
     QGroupBox *nameGroupBox=new QGroupBox(tr(""));
     nameGroupBox->setLayout(nameLayout);


     dispersive=new QCheckBox("Dispersive Model", this);
     dispersive->setCheckState(Qt::Unchecked);

     roughSurfModel=new QCheckBox("Lossy Metal", this);
     roughSurfModel->setCheckState(Qt::Unchecked);

     boundaryModel=new QCheckBox("Surf Impedance Model", this);
     boundaryModel->setCheckState(Qt::Unchecked);

     QHBoxLayout *typeLayout = new QHBoxLayout();
     typeLayout->addWidget(dispersive);
     typeLayout->addWidget(roughSurfModel);
     typeLayout->addWidget(boundaryModel);

     QGroupBox *typeGroupBox=new QGroupBox(tr(""));
     typeGroupBox->setLayout(typeLayout);

//----------------
     dvalidator = new QDoubleValidator(this);
     dvalidator->setDecimals(1000); // (standard anyway)
     dvalidator->setNotation(QDoubleValidator::ScientificNotation);

//****************************************
//   main properties
//   electric:

     QString EPSR=QString::fromUtf8("\u03B5");
//     QString EPSR=tr("<font face='symbol'>e<font face='arial'>");
     EPSR.append(tr("<sub>r</sub>"));
     QLabel *epsLabel= new QLabel(); 
     epsLabel->setText(EPSR);
     epsLineEdit = new QLineEdit();
     epsLineEdit->setText(QString("%1").arg(1.0, 0, 'f', 5));
     epsLineEdit->setValidator(dvalidator);		
     epsLineEdit->resize(1,5);

     QString SIGMA=QString::fromUtf8("\u03C3");
     SIGMA.append(tr("<font face='arial'> [S/m]"));
//     QString SIGMA=tr("<font face='symbol'>s </font><font face='arial'> [S/m]");
     QLabel *sigmaLabel= new QLabel(); sigmaLabel->setAlignment( Qt::AlignRight);
     sigmaLabel->setText(SIGMA);
     sigmaLineEdit = new QLineEdit();
     sigmaLineEdit->setText(QString("%1").arg(0.0, 0, 'e', 5));
     sigmaLineEdit->setValidator(dvalidator);		
     sigmaLineEdit->resize(1,5);

//----------------------
//   magnetic:

     QString MUR=QString::fromUtf8("\u03BC");
     MUR.append(tr("<sub>r</sub>"));
     QLabel *muLabel= new QLabel(); 
     muLabel->setText(MUR);
     muLineEdit = new QLineEdit();
     muLineEdit->setText(QString("%1").arg(1.0, 0, 'f', 5));
     muLineEdit->setValidator(dvalidator);		
     muLineEdit->resize(1,5);

     QString SIGMAM=QString::fromUtf8("\u03C3");
     SIGMAM.append(tr("<font face='arial'><sub>m</sub>["));
     SIGMAM.append(QString::fromUtf8("\u03A9"));
     SIGMAM.append(  tr("/m]") );
     QLabel *sigmamLabel= new QLabel(); 
     sigmamLabel->setText(SIGMAM); sigmamLabel->setAlignment( Qt::AlignRight);
     sigmamLineEdit = new QLineEdit();
     sigmamLineEdit->setText(QString("%1").arg(0.0, 0, 'e', 8));
     sigmamLineEdit->setValidator(dvalidator);		
     sigmamLineEdit->resize(1,5);

     QString freqBandStr=QString("Frequency band [");
     freqBandStr+=QString(prjData.freqUnitName());
     freqBandStr+=QString("] :");
     QLabel *freqBandLabel= new QLabel();
     freqBandLabel->setText(freqBandStr);

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

     emGroupBox=new QGroupBox();
     emGroupBox->setLayout(emLayout);


//****************************************
//   electric dispersion
//
     eTanDelta=new QCheckBox("Loss Tangent", this);
     eTanDelta->setCheckState(Qt::Unchecked);
     eTanDeltaLineEdit = new QLineEdit();
     eTanDeltaLineEdit->setText(QString("%1").arg(0.0, 0, 'e', 8));
     eTanDeltaLineEdit->setValidator(dvalidator);
     eTanDeltaLineEdit->setReadOnly(true);

     hTanDelta=new QCheckBox("Loss Tangent", this);
     hTanDelta->setCheckState(Qt::Unchecked);
     hTanDeltaLineEdit = new QLineEdit();
     hTanDeltaLineEdit->setText(QString("%1").arg(0.0, 0, 'e', 8));
     hTanDeltaLineEdit->setValidator(dvalidator);
     hTanDeltaLineEdit->setReadOnly(true);

     connect(dispersive, SIGNAL( stateChanged (int) ), this, SLOT(updateDisp(int)) );
     connect(boundaryModel, SIGNAL( stateChanged (int) ), this, SLOT(updateDisp(int)) );
     connect(roughSurfModel, SIGNAL( stateChanged (int) ), this, SLOT(updateDisp(int)) );
     connect(eTanDelta, SIGNAL(  stateChanged (int) ), this, SLOT(updateETanDelta(int)) );
     connect(hTanDelta, SIGNAL(  stateChanged (int) ), this, SLOT(updateHTanDelta(int)) );

     QLabel *epsTermsNumLabel= new QLabel(); 
     epsTermsNumLabel->setText(tr("Lorentz Number of Terms:"));
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

     epsLorentz->setColumnWidth(0, 150);
     epsLorentz->setColumnWidth(1, 150);
     epsLorentz->setColumnWidth(2, 150);

     QTableWidgetItem *eheader1 = new QTableWidgetItem(tr("eps_r(freq=0)"));
     QTableWidgetItem *eheader2 = new QTableWidgetItem(tr("relax freq [Hz]"));
     QTableWidgetItem *eheader3 = new QTableWidgetItem(tr("reson freq [Hz]"));
//     header1->setTextFormat(Qt::RichText);
     epsLorentz->setHorizontalHeaderItem(0, eheader1);
     epsLorentz->setHorizontalHeaderItem(1, eheader2);
     epsLorentz->setHorizontalHeaderItem(2, eheader3);

     QGridLayout *eDispLayout = new QGridLayout();
     eDispLayout->addWidget(eTanDelta, 0, 0);
     eDispLayout->addWidget(eTanDeltaLineEdit, 0, 1);
     eDispLayout->addWidget(epsTermsNumLabel, 1, 0);
     eDispLayout->addWidget(epsTermsNumSB, 1, 1);
     eDispLayout->addWidget(epsLorentz, 2, 0, 1, 3);

     eDispGroupBox=new QGroupBox(tr("Electric Model "));
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
     muTermsNumLabel->setText(tr("Lorentz Number of Terms:"));
     muTermsNumSB = new QSpinBox();
     muTermsNumSB->setValue(0);
     muTermsNumSB->setMinimum(0);

     muLorentz = new QTableWidget(0, 3, this);
     #if defined(QT5)
      muLorentz->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     #elif defined(QT4)
      muLorentz->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
     #endif

     muLorentz->setColumnWidth(0, 150);
     muLorentz->setColumnWidth(1, 150);
     muLorentz->setColumnWidth(2, 150);
     QTableWidgetItem *mheader1 = new QTableWidgetItem(tr("mu_r(freq=0)"));
     QTableWidgetItem *mheader2 = new QTableWidgetItem(tr("relax freq [Hz]"));
     QTableWidgetItem *mheader3 = new QTableWidgetItem(tr("reson freq [Hz]"));
     muLorentz->setHorizontalHeaderItem(0, mheader1);
     muLorentz->setHorizontalHeaderItem(1, mheader2);
     muLorentz->setHorizontalHeaderItem(2, mheader3);

     QGridLayout *mDispLayout = new QGridLayout();
     mDispLayout->addWidget(hTanDelta, 0, 0);
     mDispLayout->addWidget(hTanDeltaLineEdit, 0, 1);
     mDispLayout->addWidget(muTermsNumLabel,1, 0);
     mDispLayout->addWidget(muTermsNumSB, 1, 1);
     mDispLayout->addWidget(muLorentz, 2, 0, 1, 3);

     mDispGroupBox=new QGroupBox(tr("Magnetic Model "));
     mDispGroupBox->setLayout(mDispLayout);
     mDispGroupBox->hide();


//****************************************
//   Surface Properties

     QString roughFreqStr=QString("Frequency [");
     roughFreqStr+=QString(prjData.freqUnitName());
     roughFreqStr+=QString("] :");
     QLabel *roughFreqLabel= new QLabel();
     roughFreqLabel->setText(roughFreqStr);

     roughFreqLineEdit = new QLineEdit();
     roughFreqLineEdit->setValidator(dvalidator);		

     lossFacValidator = new QDoubleValidator(this);
     lossFacValidator->setDecimals(1000); // (standard anyway)
     lossFacValidator->setNotation(QDoubleValidator::ScientificNotation);
     lossFacValidator->setBottom(1.0);

     QLabel *roughLossFactorLabel= new QLabel(); 
     roughLossFactorLabel->setText(tr("Loss Factor"));

     roughLossFactorLineEdit = new QLineEdit();
     roughLossFactorLineEdit->setText(QString("%1").arg(1.0, 0, 'f', 8));
     roughLossFactorLineEdit->setValidator(lossFacValidator);		

     QLabel *roughFitPolesNumLabel= new QLabel(); 
     roughFitPolesNumLabel->setText(tr("Number of Fitting Poles:"));
     roughFitPolesNumSB = new QSpinBox();
     roughFitPolesNumSB->setValue(1);
     roughFitPolesNumSB->setMinimum(1);

//----------------
     roughQvalidator = new QDoubleValidator(this);
     roughQvalidator->setDecimals(1000); // (standard anyway)
     roughQvalidator->setNotation(QDoubleValidator::ScientificNotation);
     roughQvalidator->setBottom(1.0);

     QLabel *roughImpedanceQLabel= new QLabel(); 
     roughImpedanceQLabel->setText(tr("Q Factor"));

     roughImpedanceQLineEdit = new QLineEdit();
     roughImpedanceQLineEdit->setText(QString("%1").arg(1.0, 0, 'f', 8));
     roughImpedanceQLineEdit->setValidator(roughQvalidator);		

     QGridLayout *roughSurfLayout = new QGridLayout();
     roughSurfLayout->addWidget(roughFreqLabel,0, 0);
     roughSurfLayout->addWidget(roughFreqLineEdit,0, 1);
     roughSurfLayout->addWidget(roughFitPolesNumLabel,0, 2);
     roughSurfLayout->addWidget(roughFitPolesNumSB,0, 3);

     roughSurfLayout->addWidget(roughLossFactorLabel,1, 0);
     roughSurfLayout->addWidget(roughLossFactorLineEdit,1, 1);
     roughSurfLayout->addWidget(roughImpedanceQLabel,1, 2);
     roughSurfLayout->addWidget(roughImpedanceQLineEdit,1, 3);

     sRoughGroupBox=new QGroupBox(tr("Lossy Metal Model"));
     sRoughGroupBox->setLayout(roughSurfLayout);
     sRoughGroupBox->hide();

//****************************************
//   Surface Impedance Pole expansion

     QString SRES=tr("<font face='arial'> Surf Resist (f=");
     SRES.append(QString::fromUtf8("\u221E"));
     SRES.append(tr("<font face='arial'>) ["));
     SRES.append(QString::fromUtf8("\u03A9"));
     SRES.append(tr("]"));

     QLabel *SresLabel= new QLabel();
     SresLabel->setText(SRES);
     SresLineEdit = new QLineEdit();
     SresLineEdit->setText(QString("%1").arg(0.0, 0, 'e', 8));
     SresLineEdit->setValidator(dvalidator);		
     SresLineEdit->resize(1,5);
     QLabel *SindLabel= new QLabel(); 
     QString SIND=tr("<font face='arial'> Surf Induct (f=");
     SIND.append(QString::fromUtf8("\u221E"));
     SIND.append(tr("<font face='arial'>) [H]"));
     SindLabel->setText(SIND);
     SindLineEdit = new QLineEdit();
     SindLineEdit->setText(QString("%1").arg(0.0, 0, 'e', 8));
     SindLineEdit->setValidator(dvalidator);		
     SindLineEdit->resize(1,5);

     QLabel *surfTermsNumLabel= new QLabel(); 
     surfTermsNumLabel->setText(tr("Number of Terms:"));
     surfTermsNumSB = new QSpinBox();
     surfTermsNumSB->setValue(0);
     surfTermsNumSB->setMinimum(0);

     surfPolesRes = new QTableWidget(0, 4, this);
     #if defined(QT5)
      surfPolesRes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     #elif defined(QT4)
      surfPolesRes->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
     #endif
     surfPolesRes->setColumnWidth(0, 150);
     surfPolesRes->setColumnWidth(1, 150);
     surfPolesRes->setColumnWidth(2, 150);
     surfPolesRes->setColumnWidth(3, 150);
     QTableWidgetItem *sheader1 = new QTableWidgetItem(tr("Rs_i [ohm]"));
     QTableWidgetItem *sheader2 = new QTableWidgetItem(tr("Ls_i [H]"));
     QTableWidgetItem *sheader3 = new QTableWidgetItem(tr("Relax time [s]"));
     QTableWidgetItem *sheader4 = new QTableWidgetItem(tr("Reson time [s]"));
     surfPolesRes->setHorizontalHeaderItem(0, sheader1);
     surfPolesRes->setHorizontalHeaderItem(1, sheader2);
     surfPolesRes->setHorizontalHeaderItem(2, sheader3);
     surfPolesRes->setHorizontalHeaderItem(3, sheader4);

     QGridLayout *surfDispLayout = new QGridLayout();
     surfDispLayout->addWidget(SresLabel,0, 0);
     surfDispLayout->addWidget(SresLineEdit,0, 1);
     surfDispLayout->addWidget(SindLabel,0, 2);
     surfDispLayout->addWidget(SindLineEdit,0, 3);
     surfDispLayout->addWidget(surfTermsNumLabel,1, 0);
     surfDispLayout->addWidget(surfTermsNumSB, 1, 1);
     surfDispLayout->addWidget(surfPolesRes, 2, 0, 1, 4);

     sPolesGroupBox=new QGroupBox(tr("Surf Impedance Poles Expansion"));
     sPolesGroupBox->setLayout(surfDispLayout);
     sPolesGroupBox->hide();


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
     connect(surfTermsNumSB,  SIGNAL( valueChanged (int) ), this, SLOT(setSurfTermsNum(int)) );

     setFocusPolicy(Qt::StrongFocus);

//--------------------------------
     mainLayout = new QVBoxLayout(this);
     mainLayout->addWidget(nameGroupBox);
     mainLayout->addWidget(typeGroupBox);
     mainLayout->addWidget(emGroupBox);
     mainLayout->addWidget(eDispGroupBox);
     mainLayout->addWidget(mDispGroupBox);
     mainLayout->addWidget(sRoughGroupBox);
     mainLayout->addWidget(sPolesGroupBox);
     mainLayout->addWidget(colorGroupBox);
     mainLayout->addWidget(buttonGroupBox);
}

void DefineMaterialDialog::updateETanDelta(int state)
{
   eTanDeltaLineEdit->setReadOnly(state!=Qt::Checked);
}

void DefineMaterialDialog::updateHTanDelta(int state)
{
   hTanDeltaLineEdit->setReadOnly(state!=Qt::Checked);
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
     w=400; h=300;
     int sPolesShow=0;
     int eDispShow=0;
     int mDispShow=0;
     int roughShow=0;
     if(boundaryModel->checkState()==Qt::Checked){
	   sPolesShow=1;
	   w=600; h+=200;
     }
     if(dispersive->checkState()==Qt::Checked){
  	   eDispShow=1;
  	   mDispShow=1;
	   w=600; h+=400;
     }
     if(roughSurfModel->checkState()==Qt::Checked){
	   roughShow=1;
	   w=600; h+=100;
     }

     sPolesShow? sPolesGroupBox->show():sPolesGroupBox->hide();
     eDispShow?  eDispGroupBox->show() :eDispGroupBox->hide();
     mDispShow?  mDispGroupBox->show() :mDispGroupBox->hide();
     roughShow?  sRoughGroupBox->show():sRoughGroupBox->hide();

     QApplication::processEvents();
     window()->resize(w,h);
}



void DefineMaterialDialog::help()
{
    documentation.showDocumentation(QLatin1String("#2.3"));
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
  DB::Material* mat = mainOCAF->EmP->FindMaterial(matname.toLatin1().data());
  if(!mat) return;
  nameLineEdit->setText(matname);
  epsLineEdit->setText(QString("%1").arg(mat->epsr, 0, 'f', 5));
  sigmaLineEdit->setText(QString("%1").arg(mat->econductivity, 0, 'e', 8));
  muLineEdit->setText(QString("%1").arg(mat->mur, 0, 'f', 5));
  sigmamLineEdit->setText(QString("%1").arg(mat->hconductivity, 0, 'e', 8));
  epsTermsNumSB->setValue(mat->epsLorentz.n);
  muTermsNumSB->setValue(mat->muLorentz.n);
  surfTermsNumSB->setValue(mat->surfPolesRes.n);
  eTanDeltaLineEdit->setText(QString("%1").arg(mat->etandelta, 0, 'e', 8));
  hTanDeltaLineEdit->setText(QString("%1").arg(mat->htandelta, 0, 'e', 8));
  eTanDelta->setCheckState(mat->etandelta>0 ? Qt::Checked : Qt::Unchecked);
  hTanDelta->setCheckState(mat->htandelta>0 ? Qt::Checked : Qt::Unchecked);
  roughFreqLineEdit->setText(QString("%1").arg(mat->roughSurfFreq/prjData.freqUnit(), 0, 'f', 5));
  dispersive->setCheckState(Qt::Unchecked);
  if(mat->epsLorentz.n>0 || mat->muLorentz.n>0){
     dispersive->setCheckState(Qt::Checked);
     for (int i = 0; i <mat->epsLorentz.n; ++i) for (int j = 0; j <3; ++j){
         QLineEdit* le = (QLineEdit*) epsLorentz->cellWidget(i,j);
         le->setText(QString("%1").arg((&mat->epsLorentz[i])[j], 0, 'e', 8));
     }
     for (int i = 0; i <mat->muLorentz.n; ++i) for (int j = 0; j <3; ++j){
         QLineEdit* le = (QLineEdit*) muLorentz->cellWidget(i,j);
         le->setText(QString("%1").arg((&mat->epsLorentz[i])[j], 0, 'e', 8));
     }
  }
  if(mat->surfPolesRes.n>0 || mat->Sresistance>1.e-30 || mat->Sinductance>1.e-30){
     boundaryModel->setCheckState(Qt::Checked);
     SresLineEdit->setText(QString("%1").arg(mat->Sresistance, 0, 'e', 8));
     SindLineEdit->setText(QString("%1").arg(mat->Sinductance, 0, 'e', 8));
     for (int i = 0; i <mat->surfPolesRes.n; ++i) for (int j = 0; j <4; ++j){
         QLineEdit* le = (QLineEdit*) surfPolesRes->cellWidget(i,j);
         le->setText(QString("%1").arg((&mat->surfPolesRes[i])[j], 0, 'e', 8));
     }
  }
  if(mat->roughSurfFreq>1.e-10){
     roughSurfModel->setCheckState(Qt::Checked);
     roughFreqLineEdit->setText(QString("%1").arg(mat->roughSurfFreq/prjData.freqUnit(), 0, 'f', 5));
     roughLossFactorLineEdit->setText(QString("%1").arg(mat->roughSurfLossFactor, 0, 'f', 8));
     roughImpedanceQLineEdit->setText(QString("%1").arg(mat->roughSurfImpedanceQ, 0, 'f', 8));
     roughFitPolesNumSB->setValue(mat->roughSurfFitPolesNum);
  }
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

extern char emcadPath[256];

QString nativePath(QString path);
void msleep(unsigned milliseconds);

void DefineMaterialDialog::setSurfHurayLorentz(DB::Material* mat){
   if(!mat) return;
   if(mat->roughSurfFreq<1.e-10) return;
   QString script=QString(emcadPath);
   #ifdef WNT
      script.chop(13);
      QString ext=".exe";
   #else
      script.chop(9);
      QString ext=".py";
   #endif
   script+="bin/roughSurfaceFit";
   script=nativePath(script+ext);

   QString circuitsDir=mainWorkPath+"/Data/Circuits";

   QString sigma; sigma.setNum(mat->econductivity,'f',10);
   QString mur;   mur.setNum(mat->mur,'f',10);
   QString freq1; freq1.setNum(prjData.freqBand[0]*prjData.freqUnit(),'f',10);
   QString freq2; freq2.setNum(prjData.freqBand[1]*prjData.freqUnit(),'f',10);
   QString matFreq; matFreq.setNum(mat->roughSurfFreq,'f',10);
   QString lossFactor; lossFactor.setNum(mat->roughSurfLossFactor,'f',5);
   QString impedanceQ; impedanceQ.setNum(mat->roughSurfImpedanceQ,'f',5);
   QString npoles;  npoles.setNum(mat->roughSurfFitPolesNum);
   QString nfreq;   nfreq.setNum(10*mat->roughSurfFitPolesNum);

   QProcess *proc=new QProcess;
   proc->setWorkingDirectory(nativePath(circuitsDir));
   QString app=script;
   QStringList args;
   args << QString("-sigma");
   args << sigma;
   args << QString("-mur");
   args << mur;
   args << QString("-freq");
   args << freq1;
   args << freq2;
   args << nfreq;
   args << QString("-matFreq");
   args << matFreq;
   args << QString("-lossFactor");
   args << lossFactor;
   args << QString("-impedanceQ");
   args << impedanceQ;
   args << QString("-npoles");
   args << npoles;

   QString Cmd=app+QString("  ")+args.join(QString(" "));
   char * cmd=Cmd.toLatin1().data();
   proc->start(app, args);
   proc->waitForStarted();
   proc->waitForFinished(-1);
   msleep(100);

   QString fname=circuitsDir+"/roughSurfaceFit.txt";
   FILE *fin= fopen(nativePath(fname).toLatin1().data(), "r");
   double Rinf, Linf;
   fscanf(fin, "%lf %lf", &Rinf, &Linf);
   int Npoles,NrealPoles;
   fscanf(fin, "%d %d", &Npoles,&NrealPoles);
   std::complex<double> poles[Npoles];
   for(int i=0; i<Npoles; i++){
	double d1,d2;
	fscanf(fin, "%lf %lf", &d1, &d2);
        poles[i]=std::complex<double>(d1,d2);
   }
   std::complex<double> residues[Npoles];
   for(int i=0; i<Npoles; i++){
	double d1,d2;
	fscanf(fin, "%lf %lf", &d1, &d2);
        residues[i]=std::complex<double>(d1,d2);
   }
   fclose(fin);


   int NcomplPoles=Npoles-NrealPoles;
   int Nterms=NrealPoles+(NcomplPoles)/2;
   mat->surfPolesRes.resize(Nterms);
   mat->Sresistance=Rinf;
   mat->Sinductance=Linf;
   double Ztol=1.e-8;
   int n=0;
   double omegaMax=2*M_PI*prjData.freqBand[1]*prjData.freqUnit();
   for (int i=0; i <NrealPoles; ++i) {
       int p=i;
       double R=-std::real(residues[p])/std::real(poles[p]);
       double L=0;
       double omegar=-std::real(poles[p]);
       double taur=2*M_PI/omegar;
       double tau0=0;
       if(fabs(R)<Ztol) continue;
       (&mat->surfPolesRes[n])[0]=R;
       (&mat->surfPolesRes[n])[1]=L;
       (&mat->surfPolesRes[n])[2]=taur;
       (&mat->surfPolesRes[n])[3]=tau0;
       n++;
   }
   for (int i=0; i <NcomplPoles/2; i++) {
       int p=NrealPoles+2*i;
       double R=-2*std::real(residues[p]/poles[p]);      
       double L=2*std::real(residues[p])/std::abs(poles[p]*poles[p]);
       double omegar=-abs(poles[p]*poles[p])/(2*std::real(poles[p]));
       double omega0=abs(poles[p]);
       double taur=2*M_PI/omegar;
       double tau0=2*M_PI/omega0;
       if(fabs(R)<Ztol) R=0;
       if(fabs(L*omegaMax)<Ztol) L=0;
       if(fabs(R)<Ztol && fabs(L*omegaMax)<Ztol) continue;
       (&mat->surfPolesRes[n])[0]=R;
       (&mat->surfPolesRes[n])[1]=L;
       (&mat->surfPolesRes[n])[2]=taur;
       (&mat->surfPolesRes[n])[3]=tau0;
       n++;
   }
   Nterms=n;
   mat->surfPolesRes.resize(Nterms);
   SresLineEdit->setText(QString("%1").arg(mat->Sresistance, 0, 'e', 8));
   SindLineEdit->setText(QString("%1").arg(mat->Sinductance, 0, 'e', 8));
   surfTermsNumSB->setValue(Nterms);
   for (int i = 0; i <mat->surfPolesRes.n; ++i) for (int j = 0; j <4; ++j){
       QLineEdit* le = (QLineEdit*) surfPolesRes->cellWidget(i,j);
       le->setText(QString("%1").arg((&mat->surfPolesRes[i])[j], 0, 'e', 8));
   }
   if(!strcmp(theDefaultMaterial.name,mat->name)) mainWindow->recursiveAssignDefaultMaterial();
}

void DefineMaterialDialog::setConsTanDeltaLorentz(char type){
 int N=(type=='e') ? epsTermsNumSB->value() : muTermsNumSB->value();
 double freqBand[2];
 freqBand[0]=prjData.freqBand[0]*prjData.freqUnit();
 freqBand[1]=prjData.freqBand[1]*prjData.freqUnit();
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
 double nu=1.0/(1-2*atan(tandelta)/M_PI);
 double a=0.0;
 for (int i = 0; i <N; ++i) {
    a+=(pow(k,i*nu-N/2)-tandelta*pow(k,i*(nu-1)))/(1+pow(k,2.0*i-N));
 }
 a=tandelta/a;
 double lorentz[N][3];
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
}


void DefineMaterialDialog::setMaterialData(DB::Material* mat){
  int etandeltaChanged=0;
  int htandeltaChanged=0;
  int roughModelChanged=0;
  mat->epsr=epsLineEdit->text().toDouble();
  mat->econductivity=sigmaLineEdit->text().toDouble();
  mat->mur=muLineEdit->text().toDouble();
  mat->hconductivity=sigmamLineEdit->text().toDouble();
  if(dispersive->checkState()==Qt::Checked){
     double etandelta=eTanDeltaLineEdit->text().toDouble();
     if(eTanDelta->checkState()==Qt::Checked && fabs(mat->etandelta-etandelta) >1.e-10){
       mat->etandelta=etandelta; etandeltaChanged=1;
     }
     double htandelta=hTanDeltaLineEdit->text().toDouble();
     if(hTanDelta->checkState()==Qt::Checked && fabs(mat->htandelta-htandelta) >1.e-10){
       mat->htandelta=htandelta; htandeltaChanged=1;
     }
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
   }
   if(roughSurfModel->checkState()==Qt::Checked){
     if(fabs(mat->roughSurfFreq-roughFreqLineEdit->text().toDouble()*prjData.freqUnit()) >1.e-10){
      mat->roughSurfFreq=roughFreqLineEdit->text().toDouble()*prjData.freqUnit();
      roughModelChanged=1;
     }
     if(fabs(mat->roughSurfLossFactor-roughLossFactorLineEdit->text().toDouble()) > 1.e-10){
      mat->roughSurfLossFactor=roughLossFactorLineEdit->text().toDouble();
      roughModelChanged=1;
     }
     if(fabs(mat->roughSurfImpedanceQ-roughImpedanceQLineEdit->text().toDouble()) > 1.e-10){
      mat->roughSurfImpedanceQ=roughImpedanceQLineEdit->text().toDouble();
      roughModelChanged=1;
     }
     if((mat->roughSurfFitPolesNum!=roughFitPolesNumSB->value())){
      mat->roughSurfFitPolesNum=roughFitPolesNumSB->value();
      roughModelChanged=1;
     }
   } 
   if(boundaryModel->checkState()==Qt::Checked){
     mat->Sresistance=SresLineEdit->text().toDouble();
     mat->Sinductance=SindLineEdit->text().toDouble();
     mat->surfPolesRes.resize(surfTermsNumSB->value());
     for (int i = 0; i <mat->surfPolesRes.n; ++i) for (int j = 0; j <4; ++j){
         QLineEdit* le = (QLineEdit*) surfPolesRes->cellWidget(i,j);
	 (&mat->surfPolesRes[i])[j]=le->text().toDouble();
     }
  } 
  color.getRgb(&mat->color[0],&mat->color[1],&mat->color[2],&mat->color[3]);

  if(eTanDelta->checkState()==Qt::Checked && etandeltaChanged) setConsTanDeltaLorentz('e');
  if(hTanDelta->checkState()==Qt::Checked && htandeltaChanged) setConsTanDeltaLorentz('h');
  if(roughSurfModel->checkState()==Qt::Checked) setSurfHurayLorentz(mat);
}

void DefineMaterialDialog::set(){
	QString matname=nameLineEdit->text();
	if(matname.isEmpty()) return;
	DB::Material* mat =mainOCAF->EmP->FindMaterial(matname.toLatin1().data());
	if(!mat){ 
	   mat =new DB::Material();
           strcpy(mat->name,matname.toLatin1().data());
	   mainOCAF->EmP->addMaterial(mat);
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
	DB::Material* mat =mainOCAF->EmP->FindMaterial(matname.toLatin1().data());
	if(mat) mainOCAF->EmP->delMaterial(mat);
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
}
void DefineMaterialDialog::setMuTermsNum(int n){
	muLorentz->setRowCount(n);
        for (int i = 0; i <n; ++i) for (int j = 0; j <3; ++j){
              QLineEdit* le = new QLineEdit();
	      le->setValidator(dvalidator);
              le->setReadOnly(hTanDelta->checkState()==Qt::Checked);
              muLorentz->setCellWidget(i,j,le);
	}
}
void DefineMaterialDialog::setSurfTermsNum(int n){
	surfPolesRes->setRowCount(n);
        for (int i = 0; i <n; ++i) for (int j = 0; j <4; ++j){
              QLineEdit* le = new QLineEdit();
	      le->setValidator(dvalidator);
              le->setReadOnly(eTanDelta->checkState()==Qt::Checked);
              surfPolesRes->setCellWidget(i,j,le);
	}
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

void ImportCompPropertiesDialog::getVolumeData(QString volname){
  if(volname.isEmpty()) return;
  DB::Volume* vol = mainOCAF->EmP->FindVolume(volname.toLatin1().data());
  if(!vol) return;
  nameLineEdit->setText(volname);
  QString filter1=genericName(volname)+".emc";
  QString filter2=genericName(volname)+"_*.emc";
  QString filters="("+filter1+" "+filter2+")";
  fdialog->setNameFilter(filters);
}

void ImportCompPropertiesDialog::help()
{
    documentation.showDocumentation(QLatin1String("#2.5.1"));
}

QSize  ImportCompPropertiesDialog::sizeHint() const
{
     QSize size = QSize(w,h);
     return size;
}

ImportCompPropertiesDialog::ImportCompPropertiesDialog(TreeWidget *parent) : QDialog(parent)
{
     setModal(0);
     setWindowTitle(tr("Import Component Properties"));
//
     QLabel *nameLabel= new QLabel(); nameLabel->setText(tr("Component Name"));
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
     fdialog->setOption(QFileDialog::DontUseNativeDialog);
     fdialog->setDirectory(QDir::currentPath());
     fdialog->setNameFilter("*.emc");
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


void ImportCompPropertiesDialog::importEMC(const QString & fileName){
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


void TreeWidget::importCompPropertiesDialog()
{    
   ImportCompPropertiesDialog *dialog=new ImportCompPropertiesDialog(this);
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
   currentSubComp=0;
   if(!label.IsNull()) if(mainOCAF->isPart(label)){
       currentPart=item;
       getLabelName(label, currentPartName);
       emit sendPartName(currentPartName);
       mainWindow->openCompOrPartitionAction->setEnabled(true);
       mainWindow->openCompAndPartitionAction->setEnabled(true);
       return;
   } 
   if(!label.IsNull()) if(mainOCAF->isLayer(label)){ 
       currentLayer=item;
       getLabelName(label, currentLayerName);
       emit sendLayerName(currentLayerName);
       mainWindow->openCompOrPartitionAction->setEnabled(false);
       mainWindow->openCompAndPartitionAction->setEnabled(false);
       return;
   }
   if(item->getMaterial(mat)){
      currentMaterial=item;
      currentMaterialName=mat->name;
      emit sendMaterialName(currentMaterialName);
      mainWindow->openCompOrPartitionAction->setEnabled(false);
      mainWindow->openCompAndPartitionAction->setEnabled(false);
      return;
   }
   if(!label.IsNull())
    if(mainOCAF->isPartition()){
      QString labelName;
      getLabelName(label, labelName);
      currentSubComp=mainOCAF->subCompNum>0? mainOCAF->partNameCompMap(labelName.toLatin1().data()) : 0;
      mainWindow->openCompOrPartitionAction->setEnabled(currentSubComp>0);
      mainWindow->openCompAndPartitionAction->setEnabled(currentSubComp>0);
      return;
   }
   mainWindow->openCompOrPartitionAction->setEnabled(false);
   mainWindow->openCompAndPartitionAction->setEnabled(false);
}

void TreeWidget::assignLayerDialog()
{    
//     bool ok;
//     QString text = QInputDialog::getText(this, tr("Setting Item Property"),
//                                          tr("enter property name:"), QLineEdit::Normal,
//                                          tr(""), &ok);
//     if(!ok) return;
     AssignDialog *dialog=new AssignDialog(this);
     dialog->label1->setText(tr("Object:"));
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
     dialog->label1->setText(tr("Object:"));
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

void TreeWidget::setDefaultBCdialog()
{    
     SetDefaultBCDialog *dialog=new SetDefaultBCDialog(this);
     dialog->label2->setText(tr("BC:"));
     dialog->lineEdit2->setText(mainOCAF->EmP->defaultBC);
     connect( this, SIGNAL( sendMaterialName( QString) ),\
	      dialog->lineEdit2, SLOT( setText( QString ) )  );
     connect(dialog, SIGNAL(assigned()), this, SLOT(setDefaultBC()));
     dialog->setWindowTitle(tr("Assign Default Boundary Condition"));
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
     for (int mt = 0; mt<3; ++mt)  while(!feof(in)) {
	 int Nm;
	 char tag[5];
	 if(fscanf(in, "%d %s", &Nm, tag)<2) break;
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



void TreeWidget::openCompAndPartition()
{    
     mainWindow->openCompAndPartition();
}
void TreeWidget::openCompOrPartition()
{    
     mainWindow->openCompOrPartition();
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
  (mainOCC->getContext())->Select(true); //to clear also the Viewer Selection
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
    if(!selectedLabel.IsNull()) if(!mainOCAF->subComp) 
    { 
      QString selectedLabelName;
      getLabelName(selectedLabel, selectedLabelName);
      char *ctext=selectedLabelName.toLatin1().data();
      DB::Volume *vol=mainOCAF->EmP->FindVolume(selectedLabelName.toLatin1().data());
      if(vol){ 
	 if(mainOCAF->EmP->assemblyType==COMPONENT || mainOCAF->EmP->assemblyType==INTERFACE) {
	     if (vol->type==DIELECTRIC || vol->type==BOUNDARYCOND) menu.addAction(assignMaterialAction);
             if (vol->type==WAVEGUIDE) menu.addAction(showWgModesAction);
             menu.addAction(setCompPropertiesAction);
         } else { 
	     menu.addAction(importCompPropertiesAction);
	 }
      }
      if(showlayers)      menu.addAction(assignLayerAction);
      if(mainOCAF->isPart(selectedLabel)){
	      menu.addAction(openCompOrPartitionAction);
	      menu.addAction(openCompAndPartitionAction);
      } else if(mainOCAF->isPartition()){
       int currentSubCmp=mainOCAF->subCompNum>0? mainOCAF->partNameCompMap(selectedLabelName.toLatin1().data()) : 0;
       if(currentSubCmp>0) {
	     menu.addAction(openCompOrPartitionAction);
	     menu.addAction(openCompAndPartitionAction);
       }
      }
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
