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


#define SIGN(X) ((X) < 0. ? -1 : ((X) > 0. ? 1 : 0.))

#if defined(HAVE_OCE_CONFIG_H)
 #include <oce-config.h>
#elif defined(HAVE_OCC_CONFIG_H)
 #include <config.h>
#endif

#include <QApplication>
#include <QMouseEvent>
#include <QColorDialog>
#include <QStyle>
#include <QStyleFactory>

#include "ocaf.h"

#include <AIS_ListOfInteractive.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>

#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <Aspect_Grid.hxx>

#include "ViewWidget.h"

#include <V3d_AmbientLight.hxx>
#include <V3d_DirectionalLight.hxx>

#include <gp_Pln.hxx>
#include <gp_Lin.hxx>

#if defined(WNT)
#include <WNT_Window.hxx>
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
#include <Cocoa_Window.hxx>
#else
#include <Xw_Window.hxx>
#endif

#include <IntAna_IntConicQuad.hxx>
#include <cmath>
#include <Standard_Version.hxx>



#ifndef MIN
#define MIN(a,b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b)  (((a) > (b)) ? (a) : (b))
#endif

extern MwOCAF *mainOCAF;

ViewWidget::ViewWidget( QWidget *parent) : QWidget( parent),
	myView          ( NULL ),
	myFirstView     (true),
	myRubberBand    ( NULL ),
	myMode		( CurAction3d_Undefined ),
	myGridSnap      ( Standard_False ),
	myButtonFlags	( Qt::NoButton ),
	myViewResized	( Standard_False ),
	myPrecision	( 0.001 ),
	myDetection	( AIS_SOD_Nothing ),
	myKeyboardFlags ( Qt::NoModifier )
{
        InitViewer();
	// Needed to generate mouse events
	setMouseTracking( true );

//	setMinimumSize ( 600, 500 );

	setBackgroundRole( QPalette::NoRole );//NoBackground );
	// set focus policy to threat QContextMenuEvent from keyboard  
	setFocusPolicy( Qt::StrongFocus );
	setAttribute(Qt::WA_PaintOnScreen);
        setAttribute(Qt::WA_NoSystemBackground);

}

/*!
\brief	Default destructor for ViewWidget.
		This should delete any memory and release any resources. No parameters
		required.
*/
ViewWidget::~ViewWidget()
{
	if ( myRubberBand )
	{
		delete myRubberBand;
	}
	if ( myView )
	{
//		delete myView;
	}
}




/*!
\brief	Returns a NULL QPaintEngine
		This should result in a minor performance benefit.
*/
QPaintEngine* ViewWidget::paintEngine() const
{
	return NULL;
}

/*!
\brief	Paint Event
		Called when the Widget needs to repaint itself
\param	e The event data.
*/
void ViewWidget::paintEvent ( QPaintEvent * e)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  if (myFirstView)
  {
    InitView();
    myFirstView = false;
  }
#endif
  if (!myView.IsNull()){
	 if ( myViewResized ) myView->MustBeResized();
	 myView->Redraw();
  }
}

/*!
\brief	Resize event.
		Called when the Widget needs to resize itself, but seeing as a paint
		event always follows a resize event, we'll move the work into the
		paint event.
\param	e The event data.
*/
void ViewWidget::resizeEvent ( QResizeEvent * e)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
  if (myFirstView)
  {
    InitView();
    myFirstView = false;
  }
#endif
  myViewResized = Standard_True;
}

/*!
\brief	Mouse press event
\param	e The event data.
*/
void ViewWidget::mousePressEvent( QMouseEvent* e )
{
	myButtonFlags = e->button();

	// Cache the keyboard flags for the whole gesture
	myKeyboardFlags = e->modifiers();

	// The button mappings can be used as a mask. This code prevents conflicts
	// when more than one button pressed simutaneously.
	if      ( e->button() & Qt::LeftButton)
	{
	        onLeftButtonDown  ( myKeyboardFlags, e->pos() );
	}
	else if ( e->button() & Qt::RightButton )
	{
		onRightButtonDown ( myKeyboardFlags, e->pos() );
	}
	else if ( e->button() & Qt::MidButton )
	{
		onMiddleButtonDown( myKeyboardFlags, e->pos() );
	}
}

/*!
\brief	Mouse release event
\param	e The event data.
*/
void ViewWidget::mouseReleaseEvent(QMouseEvent* e)
{
	myButtonFlags = Qt::NoButton;
	redraw();							// Clears up screen when menu selected but not used.
	hideRubberBand();
	if ( e->button() & Qt::LeftButton )
	{
		onLeftButtonUp  ( myKeyboardFlags, e->pos() );
	}
	else if ( e->button() & Qt::RightButton )
	{
		onRightButtonUp ( myKeyboardFlags, e->pos() );
	}
	else if ( e->button() & Qt::MidButton )
	{
		onMiddleButtonUp( myKeyboardFlags, e->pos() );
	}
}

/*!
\brief	Mouse move event, driven from application message loop
\param	e The event data.
*/
void ViewWidget::mouseMoveEvent(QMouseEvent* e)
{
	Standard_Real X, Y, Z;
	
	myCurrentPoint = e->pos();
	//Check if the grid is active and that we're snapping to it
	if( myContext->CurrentViewer()->Grid()->IsActive() && myGridSnap )
	{
		myView->ConvertToGrid( myCurrentPoint.x(),
							   myCurrentPoint.y(),
							   myV3dX,
							   myV3dY,
							   myV3dZ );
		emit mouseMoved( myV3dX, myV3dY, myV3dZ );
	}
	else //	this is the standard case
	{
		if (convertToPlane( myCurrentPoint.x(),
							myCurrentPoint.y(),
							X, Y, Z ) )
		{
			myV3dX = precision( X );
			myV3dY = precision( Y );
			myV3dZ = precision( Z );
			emit mouseMoved( X, Y, Z );
		}
		else
		{
			emit sendStatus ( tr("Indeterminate Point") );
		}
	}

	onMouseMove( e->buttons(), myKeyboardFlags, e->pos() );
}

void  ViewWidget::wheelEvent(QWheelEvent *e)
{
     double degrees = e->delta() / 8.0;
     onWheel(degrees);
}

/*!
\brief	A leave event is sent to the widget when the mouse cursor leaves
		the widget.
		This sub-classed event handler fixes redraws when gestures are interrupted
		by use of parent menus etc. (Candidate for change)
\param	e	The event data
*/
void ViewWidget::leaveEvent ( QEvent* e )
{
	myButtonFlags = Qt::NoButton;
}

void ViewWidget::updateContext(  )
{
     myContext->UpdateCurrentViewer();
}

/*!
\brief	Go idle
		This called from various locations, and also exposed as a slot.
*/
void ViewWidget::idle( )
{
	setMode( CurAction3d_Nothing );
}

/*!
\brief	The main redraw function
		This called from various locations, and also exposed as a slot.
*/
void ViewWidget::redraw( void )
{
	if (!myView.IsNull()) myView->Redraw();
}

/*!
\brief	Just fits the current window
		This function just fits the current objects to the window, without
		either reducing or increasing the Z extents. This can cause clipping
		of the objects after rotation.
\return	Nothing
*/
void ViewWidget::fit( void )
{
	if (!myView.IsNull())  myView->FitAll();
}

/*!
\brief	Fits the model to view extents
		This function fits the current objects to the window,  
		reducing or increasing the Z extents as needed. 
\return	Nothing
*/
void ViewWidget::fitAll( void )
{
	if (!myView.IsNull())

	{
		myView->ZFitAll();
		myView->FitAll(0.1);
	}
}

//-----------------------------------------------------------------------------
/*!
\brief	Sets up the view for a rubberband window zoom
\return	Nothing
*/
void ViewWidget::fitArea( void )
{
    setMode( CurAction3d_WindowZooming );
}

/*!
\brief	Sets up the view for a dynamic zoom
\return	Nothing
*/
void ViewWidget::zoom( void )
{
    setMode( CurAction3d_DynamicZooming );
}

/*!
\brief	Sets up the view for panning
\return	Nothing
*/
void ViewWidget::pan( void )
{
    setMode( CurAction3d_DynamicPanning );
}

/*!
\brief	Sets up the view for dynamic rotation
\return	Nothing
*/
void ViewWidget::rotation( void )
{
    setMode( CurAction3d_DynamicRotation );
}

/*!
\brief	Sets up the view for global panning, whatever that is!
\return	Nothing
*/
void ViewWidget::globalPan()
{
	if (!myView.IsNull())
	{
		// save the current zoom value
		myCurZoom = myView->Scale();
		// Do a Global Zoom
		myView->FitAll();
		// Set the mode
		setMode( CurAction3d_GlobalPanning );
	}
}

/*!
\brief	This aligns the view to the current privilege plane.
		Not so interesting at the moment, but wait until custom
		grid planes are implemented!
\return	Nothing
*/
void ViewWidget::viewGrid()
{
	if(!myView.IsNull())
	{
		myView->SetFront();
	}
}

/*!
\brief	View from canonical "front".
\return	Nothing
*/
void ViewWidget::viewFront()
{
	if(!myView.IsNull())
	{
	    myView->SetProj( V3d_Xpos );
	    myView->FitAll();
	}
}

/*!
\brief	View from canonical "back".
\return	Nothing
*/
void ViewWidget::viewBack()
{
	if(!myView.IsNull())
	{
	    myView->SetProj( V3d_Xneg );
	    myView->FitAll();
	}
}


/*!
\brief	View from canonical "top".
		This is traditional XOY axis.
\return	Nothing
*/
void ViewWidget::viewTop()
{
	if(!myView.IsNull())
	{
	    myView->SetProj( V3d_Zpos );
	    myView->FitAll();
	}
}

/*!
\brief	View from canonical "bottom".
\return	Nothing
*/
void ViewWidget::viewBottom()
{
	if(!myView.IsNull())
	{
	    myView->SetProj( V3d_Zneg );
	    myView->FitAll();
	}
}
/*!
\brief	View from canonical "left".
\return	Nothing
*/
void ViewWidget::viewLeft()
{
	if(!myView.IsNull())
	{
	    myView->SetProj( V3d_Ypos );
	    myView->FitAll();
	}
}
/*!
\brief	View from canonical "right".
\return	Nothing
*/
void ViewWidget::viewRight()
{
	if(!myView.IsNull())
	{
	    myView->SetProj( V3d_Yneg );
	    myView->FitAll();
	}
}
/*!
\brief	View using axonometric projection.
\return	Nothing
*/

void ViewWidget::viewAxo()
{
	if(!myView.IsNull())
	{
	    myView->SetProj( V3d_XposYnegZpos );
	    myView->FitAll();
	}
}

void ViewWidget::viewReset()
{
	if(!myView.IsNull())
	{
	    myView->Reset();
	}
}

void ViewWidget::hiddenLineOff()
{
	if(!myView.IsNull())
	{
	    QApplication::setOverrideCursor( Qt::WaitCursor );
		myView->SetComputedMode( Standard_False );
		QApplication::restoreOverrideCursor();
	}
}

void ViewWidget::hiddenLineOn()
{
	if(!myView.IsNull())
	{
	    QApplication::setOverrideCursor( Qt::WaitCursor );
		myView->SetComputedMode( Standard_True );
		QApplication::restoreOverrideCursor();
	}
}

void ViewWidget::background()
{
    QColor aColor ;
    Standard_Real R1;
    Standard_Real G1;
    Standard_Real B1;
    myView->BackgroundColor(Quantity_TOC_RGB,R1,G1,B1);
    aColor.setRgb(R1*255, G1*255, B1*255);

    QColor aRetColor = QColorDialog::getColor(aColor);

    if( aRetColor.isValid() )
	{
        R1 = aRetColor.red()/255.;
        G1 = aRetColor.green()/255.;
        B1 = aRetColor.blue()/255.;
        myView->SetBackgroundColor(Quantity_TOC_RGB,R1,G1,B1);
    }
    redraw();
}

void ViewWidget::setReset ()
{
	if(!myView.IsNull())
	{
		myView->SetViewOrientationDefault() ;
	}
}



void ViewWidget::highlightTreeObject(){
  myContext->InitDetected();
  while (myContext->MoreDetected()) {
    if (myContext->HasDetectedShape()){
     TopoDS_Shape ashape = myContext->DetectedShape();
   // to be able to use the picked shape
    }
    else{
     Handle(AIS_InteractiveObject) aniobj = myContext->SelectedInteractive();
    // to be able to use the picked interactive object
    }
    myContext->NextDetected(); 
  }
}

/*!
\brief	This function handles left button down events from the mouse.
*/
void ViewWidget::checkSelection()
{
	AIS_StatusOfPick pick = myContext->Select(true);
	if ( pick != AIS_SOP_NothingSelected )
	{
	   mainOCAF->setSelectedLabels();
	   emit selectedLabelChanged();
	   emit selectionChanged();
	}
}

void ViewWidget::onLeftButtonDown(  Qt::KeyboardModifiers nFlags, const QPoint point )
{
    myStartPoint = point;

    switch ( myMode )
    {
     case CurAction3d_Nothing:
       checkSelection();
       if(nFlags==Qt::ShiftModifier)
        setMode( CurAction3d_DynamicPanning );
       else{
        myView->StartRotation( myStartPoint.x(), myStartPoint.y() );
        setMode( CurAction3d_DynamicRotation );
       }
           break;
      case CurAction3d_DynamicZooming:
           break;
      case CurAction3d_WindowZooming:
           break;
      case CurAction3d_DynamicPanning:
           break;
      case CurAction3d_GlobalPanning:
           break;
      case CurAction3d_DynamicRotation:
           break;
      default:
	         Standard_Failure::Raise( "incompatible Current Mode" );
           break;
    }
}


/*!
\brief	This function handles middle button down events from the mouse.
*/
void ViewWidget::onMiddleButtonDown(  Qt::KeyboardModifiers nFlags, const QPoint point )
{
	myStartPoint = point;
//    if ( nFlags & CASCADESHORTCUTKEY )
	setMode( CurAction3d_DynamicPanning );
}

/*!
\brief	This function handles right button down events from the mouse.
*/
void ViewWidget::onRightButtonDown(  Qt::KeyboardModifiers nFlags, const QPoint point )
{
    myStartPoint = point;
    if ( nFlags & CASCADESHORTCUTKEY )
    {
//		setMode( CurAction3d_DynamicRotation );
//		myView->StartRotation( point.x(), point.y() );
    }
    else
    {
        emit popupMenu ( this, point );
    }
}

/*!
\brief	This function handles left button up events from the mouse.
		This marks the end of the gesture.
*/


void ViewWidget::onLeftButtonUp(  Qt::KeyboardModifiers nFlags, const QPoint point )
{
	myCurrentPoint = point;
	if ( nFlags & CASCADESHORTCUTKEY )
        {
		// Deactivates dynamic zooming
		setMode( CurAction3d_Nothing );
	}
	else
	{
		switch( myMode )
		{

			case CurAction3d_Nothing:
//				break;
//
//			case CurAction3d_Picking:
				if ( myCurrentPoint == myStartPoint )
				{
					inputEvent( nFlags & MULTISELECTIONKEY );
				}
				else
				{
					dragEvent( myStartPoint,
						       myCurrentPoint,
							   nFlags & MULTISELECTIONKEY );
				}
				break;

			case CurAction3d_DynamicZooming:
		                setMode( CurAction3d_Nothing );
				break;

			case CurAction3d_WindowZooming:
				if ( (abs( myCurrentPoint.x() - myStartPoint.x() ) > ValZWMin ) ||
					 (abs( myCurrentPoint.y() - myStartPoint.y() ) > ValZWMin ) )
				{
					myView->WindowFitAll( myStartPoint.x(),
										  myStartPoint.y(),
										  myCurrentPoint.x(),
										  myCurrentPoint.y() );
				}
		                setMode( CurAction3d_Nothing );
				break;

			case CurAction3d_DynamicPanning:
		                setMode( CurAction3d_Nothing );
				break;

			case CurAction3d_GlobalPanning :
				myView->Place( myCurrentPoint.x(), myCurrentPoint.y(), myCurZoom );
		                setMode( CurAction3d_Nothing );
				break;

			case CurAction3d_DynamicRotation:
		                setMode( CurAction3d_Nothing );
				break;

			default:
				Standard_Failure::Raise(" Incompatible Current Mode ");
				break;
		}
	}
    emit selectionChanged();
}
/*!
\brief	Middle button up event handler.
		This marks the end of the gesture.
*/
void ViewWidget::onMiddleButtonUp(  Qt::KeyboardModifiers nFlags, const QPoint point )
{
    setMode( CurAction3d_Nothing );
}

/*!
\brief	Right button up event handler.
		This marks the end of the gesture.
*/
void ViewWidget::onRightButtonUp(  Qt::KeyboardModifiers nFlags, const QPoint point )
{
	myCurrentPoint = point;
	if ( nFlags & CASCADESHORTCUTKEY )
    {
		setMode( CurAction3d_Nothing );
	}
	else
	{
		if ( myMode == CurAction3d_Nothing )
		{
			emit popupMenu ( this, point );
		}
		else
		{
			setMode( CurAction3d_Nothing );
		}
	}
}

/*!
\brief	Mouse move event handler.
\param	buttons
\param	nFlags
\param	point
\return Nothing
*/
void ViewWidget::onMouseMove( Qt::MouseButtons buttons,
			      Qt::KeyboardModifiers nFlags,
			      const QPoint point )
{
	myCurrentPoint = point;

	if ( buttons & Qt::LeftButton  ||
			buttons & Qt::RightButton ||
				buttons & Qt::MidButton )
	{
		switch ( myMode )
		{
			case CurAction3d_Nothing:
				break;

			case CurAction3d_Picking:
				// Shouldn't get here yet
				drawRubberBand ( myStartPoint, myCurrentPoint );
				dragEvent( myStartPoint, myCurrentPoint, nFlags & MULTISELECTIONKEY );
				break;

			case CurAction3d_DynamicZooming:
				myView->Zoom(	myStartPoint.x(),
						myStartPoint.y(),
						myCurrentPoint.x(),
						myCurrentPoint.y() );
				myStartPoint = myCurrentPoint;
				break;

			case CurAction3d_WindowZooming:
				drawRubberBand ( myStartPoint, myCurrentPoint );
				break;

			case CurAction3d_DynamicPanning:
				myView->Pan( myCurrentPoint.x() - myStartPoint.x(),
						     myStartPoint.y() - myCurrentPoint.y() );
				myStartPoint = myCurrentPoint;
				break;

			case CurAction3d_GlobalPanning:
				break;

			case CurAction3d_DynamicRotation:
				myView->Rotation( myCurrentPoint.x(), myCurrentPoint.y() );
				break;

			default:
				Standard_Failure::Raise( "Incompatible Current Mode" );
				break;
		}
	}
	else
	{
        moveEvent( myCurrentPoint );
    }
}





void ViewWidget::onWheel( double degrees)
{
 double factor=1.0+degrees/90.0;
 myView->SetScale( myView->Scale()*factor );
 myCurZoom=myView->Scale();
}



/*!
\brief	Move event detection handler
*/
AIS_StatusOfDetection ViewWidget::moveEvent( QPoint point )
{
	AIS_StatusOfDetection status;
	status = myContext->MoveTo( point.x(), point.y(), myView, true );
	return status;
}

/*!
\brief	Drag event handler.
\param	startPoint	The gesture start point.
\param	endPoint	The gesture end point.
\param	multi		Allows selection of multiple objects.
\return The status of pick.
*/
AIS_StatusOfPick ViewWidget::dragEvent( const QPoint startPoint, const QPoint endPoint, const bool multi )
{
	AIS_StatusOfPick pick = AIS_SOP_NothingSelected;
	if (multi)
	{
		pick = myContext->ShiftSelect( MIN (startPoint.x(), endPoint.x()),
									   MIN (startPoint.y(), endPoint.y()),
									   MAX (startPoint.x(), endPoint.x()),
									   MAX (startPoint.y(), endPoint.y()),
									   myView, true );
	}
	else
	{
		pick = myContext->Select( MIN (startPoint.x(), endPoint.x()),
								  MIN (startPoint.y(), endPoint.y()),
								  MAX (startPoint.x(), endPoint.x()),
								  MAX (startPoint.y(), endPoint.y()),
								  myView, true );
	}
        emit selectionChanged();
	return pick;
}
/*!
\brief	This handles object highlighting during movement of the mouse across the view.
\param	multi	Selects multiple objects if true (default false).
\return The status of the objects under the cursor
*/
AIS_StatusOfPick ViewWidget::inputEvent( bool multi )
{
	AIS_StatusOfPick pick = AIS_SOP_NothingSelected;

	if (multi)
	{
		pick = myContext->ShiftSelect(true);
	}
	else
	{
	    pick = myContext->Select(true);
	}
	if ( pick != AIS_SOP_NothingSelected )  checkSelection();
	return pick;
}

bool ViewWidget::dump(Standard_CString theFile)
{
  redraw();
  return myView->Dump(theFile);
}

/*!
	\brief This function sets the current cursor for the given interraction mode.
	\param mode		The interraction mode
*/
void ViewWidget::setMode( const CurrentAction3d mode )
{
	if ( mode != myMode )
	{
		switch( mode )
		{
			case CurAction3d_DynamicPanning:
				setCursor( Qt::SizeAllCursor );
				break;
			case CurAction3d_DynamicZooming:
				setCursor( Qt::CrossCursor );
				break;
			case CurAction3d_DynamicRotation:
				setCursor( Qt::ArrowCursor );
//				setCursor( Qt::CrossCursor );
				break;
			case CurAction3d_GlobalPanning:
				setCursor( Qt::CrossCursor );
				break;
			case CurAction3d_WindowZooming:
				setCursor( Qt::PointingHandCursor );
				break;
			case CurAction3d_Nothing:
				//setCursor( myCrossCursor );
				setCursor( Qt::ArrowCursor );
				break;
			default:
				setCursor( Qt::ArrowCursor );
				break;
		}
		myMode = mode;
	}
}

/*!
	\brief This is a Utility function for rounding the input value to a specific DP
*/
Standard_Real ViewWidget::precision( Standard_Real aReal )
{
	Standard_Real preciseReal;
	if ( myPrecision != 0.0 )
	{
		preciseReal =  SIGN(aReal) * floor((std::abs(aReal)) / myPrecision) * myPrecision;
	}
	else
	{
		preciseReal = aReal;
	}
	return preciseReal;
}

/*!
\brief	ConvertToPlane convert 2d window position to 3d point on priviledged plane.
		This routine was provided by Matra Datavision during Foundation training.
		There still appears to be a pixel error in y co-ordinate transforms.
\param	Xs				The screen's x co-ordinate (in)
\param	Ys				The screen's y co-ordinate (in)
\param	X				The output x position on the privileged plane (out)
\param	Y				The output y position on the privileged plane (out)
\param	Z				The output z position on the privileged plane (out)
\return	Nothing
*/
Standard_Boolean ViewWidget::convertToPlane(const Standard_Integer Xs,
											     const Standard_Integer Ys,
												 Standard_Real& X,
												 Standard_Real& Y,
												 Standard_Real& Z)
{
	Standard_Real Xv, Yv, Zv;
	Standard_Real Vx, Vy, Vz;
	gp_Pln aPlane(myView->Viewer()->PrivilegedPlane());
	myView->Convert( Xs, Ys, Xv, Yv, Zv );
	myView->Proj( Vx, Vy, Vz );
	gp_Lin aLine(gp_Pnt(Xv, Yv, Zv), gp_Dir(Vx, Vy, Vz));
	IntAna_IntConicQuad theIntersection( aLine, aPlane, Precision::Angular() );
	if (theIntersection.IsDone())
	{
		if (!theIntersection.IsParallel())
		{
			if (theIntersection.NbPoints() > 0)
			{
				gp_Pnt theSolution(theIntersection.Point(1));
				X = theSolution.X();
				Y = theSolution.Y();
				Z = theSolution.Z();
				return Standard_True;
			}
		}
	}
	return Standard_False;
}
/*!
\brief	Draws the rubberband box
		This function is designed to reduce "flicker" as the box is redrawn,
		especially when the origin in the bottom corner of the window
\param	origin		A QPoint defining the screen origin
\param	position	A QPoint defining the current cursor screen location
*/
void ViewWidget::drawRubberBand( const QPoint origin, const QPoint position )
{
	if ( myRubberBand )
	{
		redraw();
		hideRubberBand();
		myRubberBand->setGeometry( QRect( origin, position ).normalized() );
		showRubberBand();
	}
}
/*!
\brief	Shows the rubberband box
*/
void ViewWidget::showRubberBand( void )
{
	if ( myRubberBand )
	{
		myRubberBand->show();
	}
}
/*!
\brief	Hides the rubberband box
*/
void ViewWidget::hideRubberBand( void )
{
	if ( myRubberBand )
	{
		myRubberBand->hide();
	}
}


void ViewWidget::InitViewer( )
{
      Handle(Aspect_DisplayConnection)  aDisplayConnection=new Aspect_DisplayConnection();
      static Handle(OpenGl_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver (aDisplayConnection);

#if OCC_VERSION_HEX > 0x060901
      myViewer = new V3d_Viewer (graphicDriver);
#else
      const Quantity_Length ViewSize=1000.0;
      myViewer = new V3d_Viewer(
		                  graphicDriver,
				  TCollection_AsciiString a3DName ("Visu3D"),"",
				  ViewSize,
				  V3d_XposYnegZpos,      //ViewProj
				  Quantity_NOC_GRAY50,   //BackgroundColor
                    		  V3d_ZBUFFER,           //TypeOfVisualization          
				  V3d_GOURAUD,            //TypeOfShadingModel
				  V3d_WAIT, 
				  Standard_True, 
				  Standard_False
		               );
#endif

       myViewer->SetLightOff();
       Handle(V3d_AmbientLight) L1 = new V3d_AmbientLight(myViewer,Quantity_NOC_WHITE) ;
       myViewer->SetLightOn(L1);
       Handle(V3d_DirectionalLight) L2 = new V3d_DirectionalLight(myViewer,V3d_XnegYnegZneg,Quantity_NOC_WHITE) ;
       myViewer->SetLightOn(L2);
       Handle(V3d_DirectionalLight) L3 = new V3d_DirectionalLight(myViewer,V3d_XnegYnegZpos,Quantity_NOC_WHITE) ;
       myViewer->SetLightOn(L3);
       Handle(V3d_DirectionalLight) L5 = new V3d_DirectionalLight(myViewer,V3d_XnegYpos,Quantity_NOC_WHITE) ;
       myViewer->SetLightOn(L5);

//	myViewer->SetDefaultVisualization(Quantity_NOC_GRAY50);

//	myViewer->SetDefaultLights();
       myContext = new AIS_InteractiveContext( myViewer );
       myContext->SetDisplayMode(AIS_Shaded, true);
       Standard_Integer selmode=6;
//	myContext->ActivateStandardMode(TopAbs_SOLID);

       myGridType = Aspect_GT_Rectangular;
	//myGridMode = Aspect_GDM_Lines;
       myGridMode       = Aspect_GDM_Points;
       myGridColor      = Quantity_NOC_RED4;
       myGridTenthColor = Quantity_NOC_MATRAGRAY;

       setGridOffset (0.0);
       gridXY();
       gridOff();


}

/*!
\brief	The Initialize procedure.
		This function creates the widget's view
		It also creates a trihedron object in the lower left corner
	         - this will eventually be managed by an external system setting.
\param	aContext Handle to the AIS Interactive Context managing the view
\return	nothing
*/


void ViewWidget::InitView()
{
  if (myView.IsNull())
    myView = myViewer->CreateView();

#ifdef _WIN32 || defined(__WIN32__)
   Aspect_Handle aWindowHandle = (Aspect_Handle) winId();
   Handle(WNT_Window) aWnd = new WNT_Window (aWindowHandle);
#if OCC_VERSION_HEX <= 0x060901
   myViewer->GetView()->SetZClippingDepth (0.5);
   myViewer->GetView()->SetZClippingWidth (0.5);
#endif
#elif defined (__APPLE__) && !defined (MACOSX_USE_GLX)
   NSView* aViewHandle = (NSView*)winId();
   Handle(Aspect_Window) aWnd = new Cocoa_Window (aViewHandle);
#else
   Window aWindowHandle = (Window ) winId();
   Handle(Aspect_DisplayConnection) aDispConnection = myContext->CurrentViewer()->Driver()->GetDisplayConnection();
   Handle(Xw_Window) aWnd = new Xw_Window (aDispConnection, aWindowHandle);
#endif

   myView->SetWindow (aWnd);
   if (!aWnd->IsMapped()) aWnd->Map();

   myView->SetBackgroundColor (Quantity_NOC_GRAY70);
   myView->MustBeResized();

   // Set up axes (Trihedron) in
   // lower left corner.
   myView->TriedronDisplay( Aspect_TOTP_LEFT_LOWER, Quantity_NOC_RED,0.08);

   // Create a rubber band box for later mouse activity
   myRubberBand = new QRubberBand( QRubberBand::Rectangle, this );
   if (myRubberBand){
	// If you don't set a style, QRubberBand doesn't work properly
	// take these lines out if you don't believe me.
	QStyle* ps = QStyleFactory::create("fusion");
	myRubberBand->setStyle( ps );
   }

   // Choose a "nicer" intial scale, and a view from the top, i.e. XY plane
   myView->SetScale( 2 );
   viewAxo();

   // Force a redraw to the new window on next paint event

   // Set default cursor as a cross
   setMode( CurAction3d_Nothing );

}




/*! 
\brief	Deletes all objects.

		This function deletes all dispayed objects from the AIS context.
		No parameters.
*/
void ViewWidget::deleteAllObjects()
{
        myContext->RemoveAll(true);
/*
	AIS_ListOfInteractive aList;
	myContext->DisplayedObjects( aList );
	AIS_ListIteratorOfListOfInteractive aListIterator;
	for ( aListIterator.Initialize( aList ); aListIterator.More(); aListIterator.Next() )
	{
		myContext->Remove( aListIterator.Value() );
	}
*/
}
/*! 
\brief	Sets the privileged plane to the XY Axis.  
*/
void ViewWidget::gridXY  ( void )
{
	myGridColor      = Quantity_NOC_RED4;
	myViewer->Grid()->SetColors( myGridColor, myGridTenthColor );
	gp_Ax3 aPlane(gp_Pnt( 0., 0., 0. ),gp_Dir(0., 0., 1.));
	myViewer->SetPrivilegedPlane( aPlane );
}
/*!
\brief	Sets the privileged plane to the XZ Axis.

		Note the negative direction of the Y axis.
		This is corrrect for a right-handed co-ordinate set.
*/
void ViewWidget::gridXZ  ( void )
{
	myGridColor      = Quantity_NOC_BLUE4;
	myViewer->Grid()->SetColors( myGridColor, myGridTenthColor );
	gp_Ax3 aPlane( gp_Pnt(0., 0., 0.),gp_Dir(0., -1., 0.) );
	myViewer->SetPrivilegedPlane( aPlane );
}
/*! 
\brief	Sets the privileged plane to the XY Axis.
 */
void ViewWidget::gridYZ  ( void )
{
	myGridColor      = Quantity_NOC_GREEN4;
	myViewer->Grid()->SetColors( myGridColor, myGridTenthColor );
	gp_Ax3 aPlane( gp_Pnt( 0., 0., 0.), gp_Dir( 1., 0., 0. ) );
	myViewer->SetPrivilegedPlane( aPlane );
}

/*!
\brief	Turn the grid on.
 */
void ViewWidget::gridOn  ( void )
{
	myViewer->ActivateGrid( myGridType , myGridMode );
	myViewer->SetGridEcho ( Standard_True );
}

/*! 
\brief	Turn the grid off.
*/
void ViewWidget::gridOff ( void )
{
	myViewer->DeactivateGrid();
	myViewer->SetGridEcho( Standard_False );
}

/*!
\brief	Select a rectangular grid
*/
void ViewWidget::gridRect ( void )
{
	myGridType = Aspect_GT_Rectangular;
	myViewer->ActivateGrid( myGridType , myGridMode );
	myViewer->Grid()->SetColors( myGridColor, myGridTenthColor );
}
/*! 
\brief	Select a circular grid.
*/
void ViewWidget::gridCirc ( void )
{
	myGridType = Aspect_GT_Circular;
	myViewer->ActivateGrid( myGridType , myGridMode );
	myViewer->Grid()->SetColors( myGridColor, myGridTenthColor );
}

void ViewWidget::setGridOffset (Quantity_Length offset)
{
	Quantity_Length radius;
	Quantity_Length xSize, ySize;
	Quantity_Length oldOffset;
	
	myViewer->CircularGridGraphicValues( radius, oldOffset );
	myViewer->SetCircularGridGraphicValues( radius, offset);

	myViewer->RectangularGridGraphicValues(xSize, ySize, oldOffset);
	myViewer->SetRectangularGridGraphicValues(xSize, ySize, offset);
}



