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


#include <qwt_text.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_legend.h>
//#include <qwt_legend_item.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_engine.h>
#include <qwt_plot_grid.h>
#include <qwt_math.h>
#include <qwt_plot_item.h>
#include <qwt_symbol.h>

#include "plotter.h"
#include "OStools.h"

#include <QList>
#include <QString>
#include <QColor>
#include <QStringList>
#include <QDialog>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QMenuBar>
#include <QHeaderView>

#include "projectData.h"
#include "plotData.h"

#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#define QWT61 1

PlotData plotData;
extern QString    mainWorkPath;
extern ProjectData prjData;

void MwPlot::updateScale(int autoscale, double yscale[3])
{
 if(autoscale)  plot->setAxisAutoScale(QwtPlot::yLeft, true);
 else           plot->setAxisScale(QwtPlot::yLeft, yscale[0], yscale[1], yscale[2]);
 plot->replot();
}


void MwPlot::setFreqResponseScale()
{
 prjData.freqRespYscaleAuto=yscaleEdit.autoscale->checkState()==Qt::Checked;
 if(!prjData.freqRespYscaleAuto){
   prjData.freqRespYscale[0] =yscaleEdit.minval->text().toDouble();
   prjData.freqRespYscale[1] =yscaleEdit.maxval->text().toDouble();
   prjData.freqRespYscale[2] =yscaleEdit.step->text().toDouble();
 }
 updateScale(prjData.freqRespYscaleAuto, prjData.freqRespYscale);
 if(prjData.freqRespYscaleAuto) updateScaleEdit();
 prjData.saveSettings();
}

void MwPlot::setZeroPoleScale()
{
 prjData.zeropoleYscaleAuto=yscaleEdit.autoscale->checkState()==Qt::Checked;
 if(!prjData.zeropoleYscaleAuto){
   prjData.zeropoleYscale[0] =yscaleEdit.minval->text().toDouble();
   prjData.zeropoleYscale[1] =yscaleEdit.maxval->text().toDouble();
   prjData.zeropoleYscale[2] =yscaleEdit.step->text().toDouble();
 }
 updateScale(prjData.zeropoleYscaleAuto, prjData.zeropoleYscale);
 if(prjData.zeropoleYscaleAuto) updateScaleEdit();
 prjData.saveSettings();
}

void MwPlot::setScale()
{
 switch(plotType){
     case FREQRESP_PLOT:  setFreqResponseScale(); break;
     case ZEROPOLE_PLOT:  setZeroPoleScale(); break;
 }
}


void MwPlot::checkAutoscale(){
    yscaleEdit.minval->setReadOnly(yscaleEdit.autoscale->checkState()==Qt::Checked);
    yscaleEdit.maxval->setReadOnly(yscaleEdit.autoscale->checkState()==Qt::Checked);
    yscaleEdit.step->setReadOnly(yscaleEdit.autoscale->checkState()==Qt::Checked);
}


#ifdef QWT61
void MwPlot::updateScaleEdit(){
     QwtScaleDiv yScaleDiv=plot->axisScaleDiv(QwtPlot::yLeft);
     QList<double> yticks=yScaleDiv.ticks(QwtScaleDiv::MajorTick);
     yscaleEdit.minval->setText(QString("%1").arg(yScaleDiv.lowerBound(), 0, 'f', 3));
     yscaleEdit.maxval->setText(QString("%1").arg(yScaleDiv.upperBound(), 0, 'f', 3));
     if(!yticks.isEmpty()){
      double yStepSize=yticks[1]-yticks[0];
      yscaleEdit.step->setText(QString("%1").arg(yStepSize, 0, 'f', 3));
     }
}
#else
void MwPlot::updateScaleEdit(){
     QwtScaleDiv *yScaleDiv=plot->axisScaleDiv(QwtPlot::yLeft);
     QList<double> yticks=yScaleDiv->ticks(QwtScaleDiv::MajorTick);
     double yStepSize=yticks[1]-yticks[0];
     yscaleEdit.minval->setText(QString("%1").arg(yScaleDiv->lowerBound(), 0, 'f', 3));
     yscaleEdit.maxval->setText(QString("%1").arg(yScaleDiv->upperBound(), 0, 'f', 3));
     yscaleEdit.step->setText(QString("%1").arg(yStepSize, 0, 'f', 3));
}
#endif

void MwPlot::setScaleDialog()
{
     QDialog *dialog=new QDialog(this);
     dialog->setWindowTitle(tr("Axis Scales"));

     QDoubleValidator *dvalidator = new QDoubleValidator(dialog);
     dvalidator->setDecimals(1000); // (standard anyway)
     dvalidator->setNotation(QDoubleValidator::ScientificNotation);

     QIntValidator *ivalidator = new QIntValidator(dialog);
     ivalidator->setRange(10,100);

     QLabel *autoscaleLabel= new QLabel();
     yscaleEdit.autoscale=new QCheckBox;
     autoscaleLabel->setText(tr("Autoscale:"));

     QLabel *minValLabel= new QLabel();
     yscaleEdit.minval = new QLineEdit();
     minValLabel->setText(tr("Min:"));
     yscaleEdit.minval->setValidator(dvalidator);		

     QLabel *maxValLabel= new QLabel();
     yscaleEdit.maxval = new QLineEdit();
     maxValLabel->setText(tr("Max:"));
     yscaleEdit.maxval->setValidator(dvalidator);

     QLabel *stepLabel= new QLabel();
     yscaleEdit.step = new QLineEdit();
     stepLabel->setText(tr("Step:"));
     
     yscaleEdit.step->setValidator(dvalidator);

     switch(plotType){
       case FREQRESP_PLOT:  
               updateScale(prjData.freqRespYscaleAuto, prjData.freqRespYscale);
               yscaleEdit.autoscale->setCheckState(prjData.freqRespYscaleAuto ? Qt::Checked : Qt::Unchecked);
	       break;
       case ZEROPOLE_PLOT:  
               updateScale(prjData.zeropoleYscaleAuto, prjData.zeropoleYscale);
               yscaleEdit.autoscale->setCheckState(prjData.zeropoleYscaleAuto ? Qt::Checked : Qt::Unchecked);
	       break;
     }

     updateScaleEdit();
     checkAutoscale();

     QPushButton *setButton = new QPushButton(tr("Set"));
     setButton->resize(1.5,3);
     QPushButton *closeButton =new QPushButton(tr("Close"));
     closeButton->resize(1.5,3);

     connect(yscaleEdit.autoscale, SIGNAL(clicked()), this, SLOT(checkAutoscale()));
     connect(setButton, SIGNAL(clicked()), this, SLOT(setScale()));
     connect(closeButton,  SIGNAL(clicked()), dialog, SLOT(accept()));

     QGridLayout *layout = new QGridLayout(dialog);
//     freqLayout->setColumnMinimumWidth(1,30);

     layout->addWidget(autoscaleLabel, 0, 0);
     layout->addWidget(yscaleEdit.autoscale,0, 1);
     layout->addWidget(minValLabel, 1, 0);
     layout->addWidget(yscaleEdit.minval,1, 1);
     layout->addWidget(maxValLabel, 2, 0);
     layout->addWidget(yscaleEdit.maxval,2, 1);
     layout->addWidget(stepLabel, 3, 0);
     layout->addWidget(yscaleEdit.step,3, 1);
     layout->addWidget(setButton, 4, 0);
     layout->addWidget(closeButton,4, 1);

     dialog->show();     
}



MwPlot::MwPlot(PlotType type) : QMainWindow()
{
  plotType=type;
  QWidget *mainWidget=new QWidget(this);
  mainLayout= new QVBoxLayout(mainWidget);
  plot=new QwtPlot();
  plot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

  mainLayout->addWidget(plot);
  if(plotType==FREQRESP_PLOT){
   curveTable=new QTableWidget();
   curveTable->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
   mainLayout->addWidget(curveTable);
  }
//  curveTable->setColumnWidth(0,150);
  mycurves=NULL;
  this->setCentralWidget(mainWidget);
  createActions();

}

void MwPlot::createActions(){
    updateAction = new QAction(tr("&Update"), this);
    connect(updateAction, SIGNAL(triggered()), this, SLOT(update()));
    updateAction->setEnabled(true);

    setScaleAction = new QAction(tr("&Set Scale"), this);
    connect(setScaleAction, SIGNAL(triggered()), this, SLOT(setScaleDialog()));
    setScaleAction->setEnabled(true);

    setReal_Action = new QAction(tr("Real"), this);
    setReal_Action->setCheckable (true);
    connect(setReal_Action, SIGNAL(triggered()), this, SLOT(setPart0()));
    setReal_Action->setChecked(true);

    setImag_Action = new QAction(tr("Imag"), this);
    setImag_Action->setCheckable (true);
    connect(setImag_Action, SIGNAL(triggered()), this, SLOT(setPart1()));
    setImag_Action->setChecked(prjData.freqRespParPart==1);

    setReIm_ActionGroup = new QActionGroup(this);
    setReIm_ActionGroup->addAction(setReal_Action);
    setReIm_ActionGroup->addAction(setImag_Action);

    setAmp_Action = new QAction(tr("Amplitude"), this);
    setAmp_Action->setCheckable (true);
    connect(setAmp_Action, SIGNAL(triggered()), this, SLOT(setPart0()));
    setAmp_Action->setChecked(prjData.freqRespParPart==0);

    setPh_Action = new QAction(tr("Phase"), this);
    setPh_Action->setCheckable (true);
    connect(setPh_Action, SIGNAL(triggered()), this, SLOT(setPart1()));
    setPh_Action->setChecked(false);

    setAmpPh_ActionGroup = new QActionGroup(this);
    setAmpPh_ActionGroup->addAction(setAmp_Action);
    setAmpPh_ActionGroup->addAction(setPh_Action);

    closeAction = new QAction(tr("&Close"), this);
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));


}

void MwPlot::setPart0(){ prjData.freqRespParPart=0; prjData.saveSettings(); plotFreqResponse(0); }
void MwPlot::setPart1(){ prjData.freqRespParPart=1; prjData.saveSettings(); plotFreqResponse(0); }


void MwPlot::createMenus(){
  menuBar()->clear();
  fileMenu = menuBar()->addMenu( tr("&File") );
     fileMenu->addAction( closeAction );
  editMenu = menuBar()->addMenu( tr("&Edit") );
     editMenu->addAction( setScaleAction );
     editMenu->addAction( updateAction );
     setAmpPhMenu = editMenu->addMenu( tr("&Par type") );
           if(!strcmp(plotData.format,"DB")){
		    setAmpPhMenu->addAction( setAmp_Action );
		    setAmpPhMenu->addAction( setPh_Action );
	   } else {
		    setAmpPhMenu->addAction( setReal_Action );
		    setAmpPhMenu->addAction( setImag_Action );
	   }

}

int readTouchstone(const char *fName);
int readSZP(const char* fName);

void MwPlot::loadFile(){
  QString fname;
  switch(plottedCircuit){
      case ELECROMAGNETICDEVICE: fname=prjData.mainAssName+"_RM"; break;
      case MAPPEDCIRCUIT:        fname=prjData.mainAssName+"_RM_mapped"; break;
      case IDEALCIRCUIT:         fname=prjData.mainAssName+"_ideal"; break;
  }
  switch(plotType){
       case FREQRESP_PLOT:  fname=fname+".ts"; break;
       case ZEROPOLE_PLOT:  fname=fname+".SZP"; break;
  }
  QString dataDir=mainWorkPath+"/Data/Circuits";
  QString fpath=dataDir+"/"+fname;
  if(!FileExists(fpath.toLatin1().data())) return;

  plotData.clear();
  const QwtScaleEngine *xscaleeng=plot->axisScaleEngine(QwtPlot::xBottom);
  QwtScaleDiv xScaleDiv,yScaleDiv;
  switch(plotType){
       case FREQRESP_PLOT: 
	     readTouchstone(nativePath(fpath.toLatin1().data()).c_str() ); 
             xScaleDiv=xscaleeng->divideScale(prjData.anaFreqBand[0], prjData.anaFreqBand[1], 10,10);
             plot->setAxisScaleDiv(QwtPlot::xBottom, xScaleDiv);
	     break;
       case ZEROPOLE_PLOT:
	     readSZP       (nativePath(fpath.toLatin1().data()).c_str() ); 
             plot->setAxisAutoScale(QwtPlot::yLeft, false);
             xScaleDiv=xscaleeng->divideScale(prjData.zpFreqBand[0], prjData.zpFreqBand[1], 10,10);
             plot->setAxisScaleDiv(QwtPlot::xBottom, xScaleDiv);
             const QwtScaleEngine *yscaleeng=plot->axisScaleEngine(QwtPlot::xBottom);
             yScaleDiv=yscaleeng->divideScale(prjData.zeropoleYscale[0], prjData.zeropoleYscale[1], 10,10);
             plot->setAxisScaleDiv(QwtPlot::yLeft, yScaleDiv);
	     break;
//       plot->setAxisAutoScale(QwtPlot::xBottom, false);
  }
  createMenus();
}

void MwPlot::update(){
  loadFile();
  switch(plotType){
       case FREQRESP_PLOT:  
	       plotFreqResponse(0); 
	       break;
       case ZEROPOLE_PLOT:  
	       plotZeroPole(0); 
	       break;
  }
}


void MwPlot::plotZeroPole(int initial){
  plot->detachItems(QwtPlotItem::Rtti_PlotItem,true);
  if(mycurves) {free(mycurves);  mycurves=NULL;}
  const QString xaxistitle="Im[s/(2 pig)]=freq ["+QString(plotData.frequnit)+"]";
  plot->setAxisTitle(QwtPlot::xBottom, xaxistitle);
  const QString yaxistitle="Re[s/(2 pig)] ["+QString(plotData.frequnit)+"]";
  plot->setAxisTitle(QwtPlot::yLeft, yaxistitle);
  updateScale(prjData.zeropoleYscaleAuto, prjData.zeropoleYscale);
  switch(plottedCircuit){
      case ELECROMAGNETICDEVICE: 
	   plot->setTitle("S Param Zeros and Poles");
	   break;
      case MAPPEDCIRCUIT:        
	   plot->setTitle("Mapped Filter S Param Zeros and Poles");
	   break;
      case IDEALCIRCUIT:        
	   plot->setTitle("Ideal Filter S Param Zeros and Poles");
	   break;
  }
  plot->updateAxes();

  grid = new QwtPlotGrid(); 
#ifdef QWT61
  grid->setMajorPen(QPen(Qt::DotLine));
#else
  grid->setMajPen(QPen(Qt::DotLine));
#endif
  grid->attach(plot);

// add curves

  int Nc=plotData.numberOfCurves;
  mycurves = (QwtPlotCurve**) malloc(Nc*sizeof(QwtPlotCurve*));
  for (int i=0; i< Nc; i++) mycurves[i]=new QwtPlotCurve;
  int QtColours[]= { 2,7,13,8,14,9, 15, 10, 16, 11, 17, 12, 18, 5, 4, 6, 19, 0, 1 };

  int ic=0;
  for ( PlotDataCurveMapIterator it=plotData.curveMap.begin(); it!= plotData.curveMap.end(); it++){
     int port1=(*it).first[0];
     int port2=(*it).first[1];
     QString tag;
     if(port1==0) tag="S_poles";
     else         tag="S("+tr("%1").arg(port1)+","+tr("%1").arg(port2)+")_zeros";
     mycurves[ic]->setTitle(tag);
     QColor color=Qt::GlobalColor(QtColours[ic%sizeof(QtColours)]);
     QwtSymbol *scatter_symbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(color), QPen(color), QSize(3, 3));
     int Np=(*it).second[0].size();
     mycurves[ic]->setSamples(&(*it).second[1][0],&(*it).second[0][0], Np);
     mycurves[ic]->attach(plot);
     mycurves[ic]->setStyle(QwtPlotCurve::NoCurve);
     mycurves[ic]->setPen(color, 1.0, Qt::SolidLine);
     mycurves[ic]->setSymbol(scatter_symbol);
     mycurves[ic]->hide();
     ic++;
  }

  legend = new QwtLegend;
#ifdef QWT61
  legend->setDefaultItemMode(QwtLegendData::Checkable);
  plot->insertLegend(legend, QwtPlot::RightLegend);
  connect(legend, SIGNAL(checked(const QVariant & , bool, int))	, this, SLOT(showCurve(const QVariant & , bool, int)));
#else
  legend->setItemMode(QwtLegend::CheckableItem);
  plot->insertLegend(legend, QwtPlot::RightLegend);
  connect(plot, SIGNAL(legendChecked(QwtPlotItem *, bool)), this, SLOT(showCurve(QwtPlotItem *, bool)));
#endif
  plot->replot();

}

void MwPlot::plotFreqResponse(int initial)
{
  plot->detachItems(QwtPlotItem::Rtti_PlotItem,true);
  if(mycurves) {free(mycurves);  mycurves=NULL;}
  const QString xaxistitle="freq ["+QString(plotData.frequnit)+"]";
  plot->setAxisTitle(QwtPlot::xBottom, xaxistitle);
  updateScale(prjData.freqRespYscaleAuto, prjData.freqRespYscale);
  QString title;
  switch(plotData.param){
	  case 'S': 
		  if(prjData.freqRespParPart==0) title="Ampl(S)  [dB]";
		  else                           title="Phase(S) [deg]";
		  break;
	  case 'Z': 
		  if(prjData.freqRespParPart==0) title="Re(Z) [ohm]"; 
		  else                           title="Imag(Z) [ohm]";
		  break;
	  case 'Y':
		  if(prjData.freqRespParPart==0) title="Re(Y) [1/ohm]"; 
		  else                           title="Imag(Y) [1/ohm]"; 
		  break;
  }
  if(plottedCircuit==MAPPEDCIRCUIT){QString tag="Mapped Filter "; title=tag+title;}
  if(plottedCircuit==IDEALCIRCUIT){QString tag="Ideal Filter "; title=tag+title;}
  plot->setTitle(title);
  plot->updateAxes();

  grid = new QwtPlotGrid();
#ifdef QWT61
  grid->setMajorPen(QPen(Qt::DotLine));
#else
  grid->setMajPen(QPen(Qt::DotLine));
#endif
  grid->attach(plot);

// add curves

  int Np=plotData.numberOfPorts;
  int Nc=plotData.numberOfCurves;
  int Nf=plotData.numberOfFreq;
  mycurves = (QwtPlotCurve**) malloc(Nc*sizeof(QwtPlotCurve*));
  for (int i=0; i< Nc; i++) mycurves[i]=new QwtPlotCurve;
  int QtColours[]= { 2,7,13,8,14,9, 15, 10, 16, 11, 17, 12, 18, 5, 4, 6, 19, 0, 1 };
  int coli=0;
  curveTable->setRowCount(Np);
  curveTable->setColumnCount(1);
  curveTable->setFixedHeight(curveTable->verticalHeader()->length()+20);
  for (int i=0; i< Np; i++) {
     QTableWidgetItem *nameItem = new QTableWidgetItem(plotData.portnames.at(i));
     curveTable->setItem(i, 0, nameItem);
  }
  curveTable->setShowGrid(false);
  curveTable->horizontalHeader()->hide();
  curveTable->resizeColumnToContents(0);


  for (int i=0; i< Np; i++) for (int j=0; j< Np; j++)
  {
    int ij=i+Np*j;
    QString I; I.setNum(i+1);
    QString J; I.setNum(j+1);
    QChar param=plotData.param;
    QString tag=param;
    tag=tag+"("+tr("%1").arg((i+1))+","+tr("%1").arg((j+1))+")";
    mycurves[ij]->setTitle(tag);
    if(coli == sizeof(QtColours)) coli = 0;
    QColor color=Qt::GlobalColor(QtColours[coli]);
    mycurves[ij]->setPen(color, 1.0, Qt::SolidLine);
    mycurves[ij]->setSamples(&plotData.frequencies[0],&plotData.curveArray[(2*Nf)*ij+Nf*prjData.freqRespParPart],plotData.numberOfFreq);
    mycurves[ij]->attach(plot);
//    if(initial) mycurves[ij]->hide();
    mycurves[ij]->hide();
    mycurves[ij]->setStyle(QwtPlotCurve::Lines);
    coli++;
  }
  plot->show();


  legend = new QwtLegend;
#ifdef QWT61
  legend->setDefaultItemMode(QwtLegendData::Checkable);
  plot->insertLegend(legend, QwtPlot::RightLegend);
  connect(legend, SIGNAL(checked(const QVariant & , bool, int))	, this, SLOT(showCurve(const QVariant & , bool, int)));
#else
  legend->setItemMode(QwtLegend::CheckableItem);
  plot->insertLegend(legend, QwtPlot::RightLegend);
  connect(plot, SIGNAL(legendChecked(QwtPlotItem *, bool)), this, SLOT(showCurve(QwtPlotItem *, bool)));
#endif
  plot->replot();
}



#ifdef QWT61
void MwPlot::showCurve(const QVariant & itemInfo, bool on, int 	index)
{
   QwtPlotItem *item=plot->infoToItem(itemInfo);
   item->setVisible(on);
//   QWidget *w = plot->legend()->find(item);
   plot->replot();
}
#else
void MwPlot::showCurve(QwtPlotItem *item, bool on)
{
   item->setVisible(on);
//   QWidget *w = plot->legend()->find(item);
   plot->replot();
}
#endif

/*
class MyPlot : public QwtPlot
{
public:
  MyPlot( QWidget *parent=0, char *name=0 ) : QwtPlot( parent, name )
  {
    // Mostra un titolo
    setTitle( "This is an Example" );

    // Mostra delle spiegazioni
    setAutoLegend( true );
    setLegendPos( Qwt::Bottom );

    // Mostra gli assi
    setAxisTitle( xBottom, "x" );
    setAxisTitle( yLeft, "y" );

    // Inserisci due curve e ottieni il loro ID
    long cSin = insertCurve( "y = sin(x)" );
    long cSign = insertCurve( "y = sign(sin(x))" );

    // Calcola i dati, 500 punti ciascuno
    const int points = 500;
    double x[ points ];
    double sn[ points ];
    double sg[ points ];

    for( int i=0; i<points; i++ )
    {
      x[i] = (3.0*3.14/double(points))*double(i);
    
      sn[i] = 2.0*sin( x[i] );
      sg[i] = (sn[i]>0)?1:((sn[i]<0)?-1:0);
    }

    // Copia i dati nel diagramma
    setCurveData( cSin, x, sn, points );
    setCurveData( cSign, x, sg, points );

    // Imposta lo stile delle curve
    setCurvePen( cSin, QPen( blue ) );
    setCurvePen( cSign, QPen( green, 3 ) );

    // Mostra i diagrammi
    replot();
  }
};
*/


