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


#define QWT61 1

#include <qwt_plot.h>
#include <QMainWindow>
#include <qwt_plot_item.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <QMenu>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTableWidget>

#if defined(QT5)
 #include <QtGui>
#endif
#include <QCheckBox>

struct ScaleEdit{
 QCheckBox *autoscale;
 QLineEdit *minval;
 QLineEdit *maxval;
 QLineEdit *step;
};

#ifndef _SELECTEDCIRCUIT_H_
#define _SELECTEDCIRCUIT_H_
enum SelectedCircuit {ELECROMAGNETICDEVICE=0, MAPPEDCIRCUIT=1};
#endif

enum PlotType {FREQRESP_PLOT=0, ZEROPOLE_PLOT=1};

class QAction;
class MwPlot : public QMainWindow
{

Q_OBJECT

public:
MwPlot(PlotType type);
QwtPlot *plot;
PlotType plotType;
SelectedCircuit plottedCircuit;
void loadFile();
void plotFreqResponse(int initial=1);
void plotZeroPole(int initial=1);
public slots:
#ifdef QWT61
void showCurve(const QVariant & itemInfo, bool on, int 	index);
#else
void showCurve(QwtPlotItem *item, bool on);
#endif

private:
void createActions();
void createMenus();
void updateScaleEdit();

QMenu *fileMenu;
QMenu *editMenu;
QMenu *setReImagMenu;
QMenu *setAmpPhMenu;

QVBoxLayout *mainLayout;
QTableWidget *curveTable;
QwtPlotGrid *grid;
QwtPlotCurve **mycurves;
QwtLegend *legend;


QAction *updateAction;
QAction *setScaleAction;
ScaleEdit yscaleEdit;

QAction *setReal_Action;
QAction *setImag_Action;
QActionGroup *setReIm_ActionGroup;

QAction *setAmp_Action;
QAction *setPh_Action;
QActionGroup *setAmpPh_ActionGroup;

QAction *closeAction;

void setFreqResponseScale();
void setZeroPoleScale();

private slots:
void setScaleDialog();
void setScale();
void updateScale(int autoscale, double yscale[3]);
void setPart0();
void setPart1();
void update();
void checkAutoscale();

};

