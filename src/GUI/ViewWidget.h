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


#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#if defined(HAVE_OCE_CONFIG_H)
 #include <oce-config.h>
#elif defined(HAVE_OCC_CONFIG_H)
 #include <config.h>
#endif

/** the key for multi selection */
#define MULTISELECTIONKEY  Qt::ShiftModifier   

/** The key for shortcut ( use to activate dynamic rotation, panning ) */
#define CASCADESHORTCUTKEY Qt::ControlModifier 

#define ValZWMin 1 /** For elastic bean selection */

#if defined(WNT)
 #include <QWidget>
#elif defined(QT54)
 #include <QOpenGLWidget>
#elif defined(QT4)
 #include <QGLWidget>
#endif


#include <QRubberBand>

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

#include <Quantity_Factor.hxx>
#include <Aspect_GridType.hxx>
#include <V3d_Coordinate.hxx>
#include <Aspect_GridDrawMode.hxx>
#include <TDF_Label.hxx>

#if defined(WNT)
 class ViewWidget: public QWidget
#elif defined(QT54)
 class ViewWidget: public QOpenGLWidget
#elif defined(QT4)
 class ViewWidget: public QGLWidget
#endif
{
	Q_OBJECT

public:

	enum CurrentAction3d {	CurAction3d_Undefined,
							CurAction3d_Nothing, 
							CurAction3d_Picking,
							CurAction3d_DynamicZooming,
							CurAction3d_WindowZooming, 
							CurAction3d_DynamicPanning,
							CurAction3d_GlobalPanning, 
							CurAction3d_DynamicRotation };
/*
	enum ViewAction {	ViewFitAllId, 
						ViewFitAreaId, 
						ViewZoomId, 
						ViewPanId, 
						ViewGlobalPanId,
						ViewFrontId, 
						ViewBackId, 
						ViewTopId, 
						ViewBottomId, 
						ViewLeftId, 
						ViewRightId,
						ViewAxoId, 
						ViewRotationId, 
						ViewResetId, 
						ViewHlrOffId, 
						ViewHlrOnId };
*/
public:

	ViewWidget(QWidget *parent = 0, Qt::WindowFlags wflags = 0);
        ~ViewWidget();

	void createViewer();
	void createView();

	Handle_AIS_InteractiveContext	getContext( void ) { return myContext; }
	Handle_V3d_View			getView( void )    { return myView; }
	Handle_V3d_Viewer&              getViewer()        { return myViewer; };

	//Overrides
	QPaintEngine* paintEngine() const;



	void deleteAllObjects();

	void setGridOffset (Quantity_Length offset);


signals:
	void initialized();
	void selectionChanged();
	void selectedLabelChanged();
	void mouseMoved   ( V3d_Coordinate X, V3d_Coordinate Y, V3d_Coordinate Z );
	void pointClicked ( V3d_Coordinate X, V3d_Coordinate Y, V3d_Coordinate Z );
	void sendStatus   ( const QString aMessage );
	//! Just a placeholder for now
	void popupMenu ( const ViewWidget* aView, const QPoint aPoint ); 
	void error ( int errorCode, QString& errorDescription );

public slots:
    void updateContext( void );
    void idle( void );
    void fit( void );
    void fitAll( void );
    void fitArea( void );
    void zoom( void );
    void pan( void );
    void globalPan( void );
    void rotation( void );
    void hiddenLineOn( void );
    void hiddenLineOff( void );
    void background( void );
    void redraw( void );
    void viewFront( void );
    void viewBack( void );
    void viewTop( void );
    void viewBottom( void );
    void viewLeft( void );
    void viewRight( void );
    void viewAxo( void );
    void viewGrid( void );
    void viewReset( void );
    void setReset ();
    void highlightTreeObject();

    void gridXY   ( void );
    void gridXZ   ( void );
    void gridYZ   ( void );
    void gridOn   ( void );
    void gridOff  ( void );
    void gridRect ( void );
    void gridCirc ( void );


protected: // methods

    virtual void paintEvent        ( QPaintEvent* e );
    virtual void resizeEvent       ( QResizeEvent* e );
    virtual void mousePressEvent   ( QMouseEvent* e );
    virtual void mouseReleaseEvent ( QMouseEvent* e );
    virtual void mouseMoveEvent    ( QMouseEvent* e );
    virtual void wheelEvent        ( QWheelEvent* e );

    virtual void leaveEvent	   ( QEvent * );

private: // members
        TDF_Label theSelectedShape;

	Handle_V3d_View                 myView;
	Handle_V3d_Viewer               myViewer;
	Handle_AIS_InteractiveContext   myContext;

	Standard_Boolean		myViewResized;
        CurrentAction3d                 myMode;
        Quantity_Factor                 myCurZoom;
        Standard_Boolean		myGridSnap;
	AIS_StatusOfDetection		myDetection;

	V3d_Coordinate			myV3dX,   
					myV3dY,   
					myV3dZ;
		
	QRubberBand*			myRubberBand;
	QPoint				myStartPoint;
	QPoint				myCurrentPoint;
	
	Standard_Real			myPrecision;
	Standard_Boolean		myMapIsValid;
	Qt::KeyboardModifiers		myKeyboardFlags;
	Qt::MouseButton			myButtonFlags;
	QCursor				myCrossCursor;

	Aspect_GridType			myGridType;
	Aspect_GridDrawMode		myGridMode;
	Quantity_NameOfColor		myGridColor;
	Quantity_NameOfColor		myGridTenthColor;


private: // methods
    void checkSelection();
    void onLeftButtonDown  ( Qt::KeyboardModifiers nFlags, const QPoint point );
    void onMiddleButtonDown( Qt::KeyboardModifiers nFlags, const QPoint point );
    void onRightButtonDown ( Qt::KeyboardModifiers nFlags, const QPoint point );
    void onLeftButtonUp    ( Qt::KeyboardModifiers nFlags, const QPoint point );
    void onMiddleButtonUp  ( Qt::KeyboardModifiers nFlags, const QPoint point );
    void onRightButtonUp   ( Qt::KeyboardModifiers nFlags, const QPoint point );
    void onWheel           ( double degrees );
    
    void onMouseMove  ( Qt::MouseButtons buttons, 
			Qt::KeyboardModifiers nFlags, 
			const QPoint point );

    AIS_StatusOfPick		dragEvent ( const QPoint startPoint, const QPoint endPoint, const bool multi = false );
    AIS_StatusOfPick		inputEvent( const bool multi = false );
    AIS_StatusOfDetection	moveEvent ( const QPoint point );
    
    void setMode( const CurrentAction3d mode );
    Standard_Real precision( Standard_Real aReal );
    void drawRubberBand( const QPoint origin, const QPoint position );
    void showRubberBand( void );
    void hideRubberBand( void );

    Standard_Boolean convertToPlane(const Standard_Integer Xs, 
				    const Standard_Integer Ys, 
				    Standard_Real& X,
				    Standard_Real& Y,
				    Standard_Real& Z);
										  

public:

	bool dump(Standard_CString theFile);

};

#endif // ViewWidget_H
