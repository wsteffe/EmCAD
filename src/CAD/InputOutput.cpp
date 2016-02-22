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

#include<string>

#include "InputOutput.h"
#include "OStools.h"


#include <TDataStd_Name.hxx>

#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Solid.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopTools_DataMapOfShapeInteger.hxx>

#include <Geom_Line.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>

#include <TopTools_HSequenceOfShape.hxx>

#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>

#include <BRepTools.hxx>
#include <ShapeSchema.hxx>
#include <TCollection_AsciiString.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#include <TColStd_SequenceOfAsciiString.hxx>
#include <TCollection_HAsciiString.hxx>
#include <TColStd_SequenceOfHAsciiString.hxx>

#include <TNaming_NamedShape.hxx>


// specific STEP & IGES
#include <STEPCAFControl_Reader.hxx>
#include <STEPControl_Controller.hxx>
#include <STEPCAFControl_Controller.hxx>
#include <IGESCAFControl_Reader.hxx>
#include <IGESControl_Controller.hxx>
#include <Interface_Static.hxx>

#include <StepBasic_Product.hxx>
#include <StepRepr_ProductDefinitionShape.hxx>
#include <StepRepr_RepresentationItem.hxx>
#include <StepShape_ShapeRepresentation.hxx>
#include <StepShape_AdvancedBrepShapeRepresentation.hxx>
#include <StepShape_ManifoldSurfaceShapeRepresentation.hxx>
#include <StepShape_GeometricallyBoundedSurfaceShapeRepresentation.hxx>
#include <StepShape_OpenShell.hxx>
#include <StepGeom_CompositeCurve.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <Transfer_Binder.hxx>
#include <Transfer_TransientProcess.hxx>
#include <TransferBRep.hxx>
//#include <StepRepr_ProductDefinitionShape.hxx>
//#include <UnitsMethods.hxx>

#include <StepData_StepModel.hxx>
#include <XSControl_WorkSession.hxx>
#include <XSControl_TransferReader.hxx>

#include <StepShape_ManifoldSolidBrep.hxx>
#include <StepShape_ShellBasedSurfaceModel.hxx>
#include <TDF_ChildIterator.hxx>

// specific CSFDB
#include <FSD_File.hxx>
#include <MgtBRep.hxx>
#include <MgtTopLoc.hxx>
#include <PTopLoc_Location.hxx>
#include <PTopLoc_ItemLocation.hxx>
#include <PTColStd_PersistentTransientMap.hxx>
#include <PTColStd_TransientPersistentMap.hxx>
#include <PTopoDS_HShape.hxx>
#include <Storage_Data.hxx>
#include <Storage_Error.hxx>
#include <Storage_HSeqOfRoot.hxx>
#include <Storage_Root.hxx>
#include <ShapeSchema.hxx>



/******************************************************************
 * EXPORT FUNCTIONALITY
 ******************************************************************/

/*
Standard_Boolean ReadNamesBis(const Handle(XSControl_WorkSession)& WS,
                                    Handle(TDocStd_Document)& doc)
{
  Handle(Interface_InterfaceModel) Model = WS->Model(); // Is there any way to access to the WorkSession 
  Handle(XSControl_TransferReader) TR = WS->TransferReader();
  Handle(Transfer_TransientProcess) TP = TR->TransientProcess();
  Handle(XCAFDoc_ShapeTool) STool = XCAFDoc_DocumentTool::ShapeTool( doc->Main() );
  if ( STool.IsNull() ) return Standard_False;
  Standard_Integer nb = Model->NbEntities();
  Handle(TCollection_HAsciiString) name;

  for (Standard_Integer i = 1; i <= nb; i ++) {
    Handle(Standard_Transient) enti = Model->Value(i);
     
    if ( ! enti->DynamicType()->SubType("StepRepr_RepresentationItem")) continue;
    Handle(StepRepr_RepresentationItem) SRRI = Handle(StepRepr_RepresentationItem)::DownCast(enti);
    Handle(TCollection_HAsciiString) hName = SRRI->Name();
    TCollection_ExtendedString myName ( hName->String() );
     
    Handle(Transfer_Binder) binder = TP->Find( enti);
    if ( binder.IsNull() || ! binder->HasResult() ) continue;

    TopoDS_Shape S = TransferBRep::ShapeResult ( TP, binder );
    if ( S.IsNull() ) continue;
     
    TDF_Label shL;
//    if ( ! STool->Search ( S, shL, Standard_True, Standard_True, Standard_True ) ) continue;
    std::string name=std::string(hName->ToCString());
    int i=1;
  }
  return Standard_True;
}
*/

void  importSTEP( Handle(TDocStd_Document) doc, char* fileName, char *lengthUnitName )
{

	STEPCAFControl_Reader reader;
	if(!Interface_Static::SetIVal("read.stepcaf.subshapes.name",0)) cout<<"error";
	if(!Interface_Static::SetIVal("read.step.product.mode",1)) cout<<"error";
	if(!Interface_Static::SetIVal("read.step.product.context",1))  cout<<"error";
	if(!Interface_Static::SetIVal("read.step.shape.repr",1)) cout<<"error";
	if(!Interface_Static::SetIVal("read.step.assembly.level",2))  cout<<"error";
        if(!Interface_Static::SetIVal("read.step.shape.aspect",1)) cout<<"error";
        if(!Interface_Static::SetIVal("read.step.shape.relationship",1)) cout<<"error";
	if(lengthUnitName){
		Standard_CString LU=lengthUnitName;
	        if(!Interface_Static::SetCVal("step.cascade.unit",LU)) cout<<"error";
	}

        IFSelect_ReturnStatus status = reader.ReadFile(fileName);
	bool ok= status==IFSelect_RetDone ;
	Standard_Integer NbRoots = reader.NbRootsForTransfer();
        if(ok){       
//	    Standard_Boolean ok = reader.EmCAD_Transfer(doc);
	    Standard_Boolean ok = reader.Transfer(doc);
        }
	Handle(XSControl_WorkSession) WS=reader.Reader().WS();
	Handle(StepData_StepModel) model = reader.Reader().StepModel();
        Standard_Integer nb = model->NbEntities();

        Handle(Interface_InterfaceModel) Model = WS->Model(); // Is there any way to access to the WorkSession 
        Handle(XSControl_TransferReader) TR = WS->TransferReader();
        Handle(Transfer_TransientProcess) TP = TR->TransientProcess();
        Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool( doc->Main() );

        for (int i = 1; i <= nb; i ++) {
            Handle(Standard_Transient) enti = model->Value(i);
	    TCollection_AsciiString entityName;
/*
            if (enti->DynamicType()->SubType("StepRepr_RepresentationItem")){
              Handle(StepRepr_RepresentationItem) SRRI = Handle(StepRepr_RepresentationItem)::DownCast(enti);
              entityName =SRRI->Name()->String();
	    }
*/ 
	    int t=0;
	    if ( enti->IsKind( STANDARD_TYPE(StepShape_ManifoldSolidBrep) ) ){
               Handle(StepShape_ManifoldSolidBrep) MSB = Handle(StepShape_ManifoldSolidBrep)::DownCast(enti);
               entityName =MSB->Name()->String();
	       t=1;
/*	    } else if ( enti->IsKind( STANDARD_TYPE(StepShape_ShellBasedSurfaceModel) ) ){
              Handle(StepShape_ShellBasedSurfaceModel) ShBSM = Handle(StepShape_ShellBasedSurfaceModel)::DownCast(enti);
              entityName =ShBSM->Name()->String();
	      t=2;
*/
	    } else if ( enti->IsKind( STANDARD_TYPE(StepShape_OpenShell) ) ){
              Handle(StepShape_OpenShell) OSh = Handle(StepShape_OpenShell)::DownCast(enti);
              entityName =OSh->Name()->String();
	      t=3;
	    } else if ( enti->IsKind( STANDARD_TYPE(StepShape_AdvancedBrepShapeRepresentation) ) ){
              Handle(StepShape_AdvancedBrepShapeRepresentation) REP = Handle(StepShape_AdvancedBrepShapeRepresentation)::DownCast(enti);
//              entityName =REP->Name()->String();
              entityName =TCollection_AsciiString("REMOVE");
	      t=4;
	    } else if ( enti->IsKind( STANDARD_TYPE(StepShape_ManifoldSurfaceShapeRepresentation) ) ){
              Handle(StepShape_ManifoldSurfaceShapeRepresentation) REP = Handle(StepShape_ManifoldSurfaceShapeRepresentation)::DownCast(enti);
//              entityName =REP->Name()->String();
              entityName =TCollection_AsciiString("REMOVE");
	      t=5;
	    } else if ( enti->IsKind( STANDARD_TYPE(StepShape_GeometricallyBoundedSurfaceShapeRepresentation) ) ){
              Handle(StepShape_GeometricallyBoundedSurfaceShapeRepresentation) REP = Handle(StepShape_GeometricallyBoundedSurfaceShapeRepresentation)::DownCast(enti);
//              entityName =REP->Name()->String();
              entityName =TCollection_AsciiString("REMOVE");
	      t=6;
	    } else if ( enti->IsKind( STANDARD_TYPE(StepGeom_CompositeCurve) ) ){
              Handle(StepGeom_CompositeCurve) REP = Handle(StepGeom_CompositeCurve)::DownCast(enti);
              entityName =REP->Name()->String();
//              entityName =TCollection_AsciiString("REMOVE");
	      t=7;
	    }
	    if(entityName.IsEmpty()) continue;
	    Handle(Transfer_Binder) binder = TP->Find( enti); if ( binder.IsNull() || ! binder->HasResult() ) continue;
            TopoDS_Shape S = TransferBRep::ShapeResult ( TP, binder ); if ( S.IsNull() ) continue;
            if((t==1) && (S.ShapeType()!=TopAbs_SOLID)) continue;
            if((t==3) && (S.ShapeType()!=TopAbs_SHELL)) continue;
            if((t==4) && (S.ShapeType()!=TopAbs_COMPOUND)) continue;
            if((t==5) && (S.ShapeType()!=TopAbs_COMPOUND)) continue;
            if((t==6) && (S.ShapeType()!=TopAbs_COMPOUND)) continue;
            if((t==7) && (S.ShapeType()!=TopAbs_WIRE)) continue;
            TDF_Label shL=shapeTool->FindShape( S,Standard_False); 
	    if(shL.IsNull()) continue;
            Handle(TDataStd_Name)  nameAtt;  TCollection_AsciiString name;
            if(!shL.FindAttribute(TDataStd_Name::GetID(),nameAtt)){
		   nameAtt= new TDataStd_Name();
		   shL.AddAttribute (nameAtt);
            }
            name=nameAtt->Get();
            if(name==entityName) continue;
	    nameAtt->Set(entityName);
	 }



}


void  importIGES( Handle(TDocStd_Document) doc, char* fileName )
{
	IGESControl_Controller::Init();
	IGESCAFControl_Reader reader;
//	if(!Interface_Static::SetIVal("read.step.assembly.level",1))  cout<<"error";
        IFSelect_ReturnStatus status = reader.ReadFile(fileName);
	bool ok= status==IFSelect_RetDone ;
        if(ok){       
	    reader.SetColorMode(Standard_True);
	    reader.SetNameMode(Standard_True);
	    reader.SetLayerMode(Standard_True);
	    Standard_Boolean ok = reader.Transfer(doc);
        }
}


FSD_File *fileDriver=NULL;


bool openShape( const char* fileName, TopoDS_Shape &shape)
{
    // Check file type
    if(!fileDriver) fileDriver=new FSD_File();
    if ( FSD_File::IsGoodFileType( fileName ) != Storage_VSOk )  return false;

    TCollection_AsciiString aName( fileName );
    if ( fileDriver->Open( aName, Storage_VSRead ) != Storage_VSOk ) return false;

    Handle(ShapeSchema) schema = new ShapeSchema();
    Handle(Storage_Data) data  = schema->Read( *fileDriver );
    if ( data->ErrorStatus() != Storage_VSOk ) return false;

    fileDriver->Close();

    Handle(Storage_HSeqOfRoot) roots = data->Roots();
    Handle(Storage_Root) r = roots->Value( 1 );
    Handle(Standard_Persistent) p = r->Object();
    Handle(PTopoDS_HShape) aPShape = Handle(PTopoDS_HShape)::DownCast(p);
    if ( aPShape.IsNull() ) return 1;
    PTColStd_PersistentTransientMap aMap;
    MgtBRep::Translate( aPShape, aMap, shape, MgtBRep_WithoutTriangle );
    return true;

/*
    for ( int i = 0; i < roots->Length() ; i++ )
    {
        Handle(Storage_Root) r = roots->Value( i+1 );
        Handle(Standard_Persistent) p = r->Object();
        Handle(PTopoDS_HShape) aPShape = Handle(PTopoDS_HShape)::DownCast(p);
        if ( !aPShape.IsNull() )
        {
	    PTColStd_PersistentTransientMap aMap;
	    TopoDS_Shape aTShape;
            MgtBRep::Translate( aPShape, aMap, aTShape, MgtBRep_WithoutTriangle );
            myOCAF->theSolids.Append( aTShape );
        }
    }
    BRep_Builder builder;
    builder.MakeCompSoloid(myOCAF->theCompSolid);
    for(int i=0; i<myOCAF->theSolids.Length(); i++)  builder.Add(myOCAF->theCompSolid,myOCAF->theSolids.Value(i+1));
*/

}


bool openShapes( const char* fileName, TopTools_IndexedMapOfShape &shapes)
{
    // Check file type
    if ( FSD_File::IsGoodFileType( fileName ) != Storage_VSOk )  return false;

    if(!fileDriver) fileDriver=new FSD_File();
    TCollection_AsciiString aName( fileName );
    if ( fileDriver->Open( aName, Storage_VSRead ) != Storage_VSOk ) return false;

    Handle(ShapeSchema) schema = new ShapeSchema();
    Handle(Storage_Data) data  = schema->Read( *fileDriver );
    if ( data->ErrorStatus() != Storage_VSOk ) return false;

    fileDriver->Close();

    Handle(Storage_HSeqOfRoot) roots = data->Roots();
    shapes.Clear();
    int N=roots->Length();
    int *permu=new int[N];
    for ( int i = 1; i <= N; i++ ) {
        Handle(Storage_Root) r=roots->Value( i );
	permu[atoi(r->Name().ToCString())-1]=i;
    }
    for ( int i = 1; i <= N; i++ )
    {
        Handle(Storage_Root) r=roots->Value( permu[i-1] );
        Handle(PTopoDS_HShape) aPShape = Handle(PTopoDS_HShape)::DownCast(r->Object());
        if ( !aPShape.IsNull() )
        {
	    PTColStd_PersistentTransientMap aMap;
	    TopoDS_Shape S;
            MgtBRep::Translate( aPShape, aMap, S, MgtBRep_WithoutTriangle );
	    shapes.Add(S);
        }
    }
    delete [] permu;

    return true;

}


bool saveShape( const char* fileName, TopoDS_Shape shape)
{

    Handle(ShapeSchema) schema = new ShapeSchema();
    Handle(Storage_Data) data  = new Storage_Data();
    data->ClearErrorStatus();

    data->SetApplicationName( TCollection_ExtendedString( "MwCAE" ) );
    data->SetApplicationVersion( "1" );
    data->SetDataType( TCollection_ExtendedString( "Shapes" ) );
    data->AddToUserInfo( "Storing the shape in a flat file" );
    data->AddToComments( TCollection_ExtendedString( "Application is based on CasCade 6.8" ) );

    if(!fileDriver) fileDriver=new FSD_File();
    if ( fileDriver->Open( fileName, Storage_VSWrite ) != Storage_VSOk ) return false;

    PTColStd_TransientPersistentMap aMap;
    Handle(PTopoDS_HShape) pshape = MgtBRep::Translate(shape, aMap, MgtBRep_WithoutTriangle );
    data->AddRoot( pshape );

    schema->Write( *fileDriver, data );
//    fileDriver->myStream.flush();
    fflush(NULL);
    fileDriver->Close();

    return ( data->ErrorStatus() == Storage_VSOk );
}



bool saveShapes( const char* fileName, TopTools_IndexedMapOfShape &shapes)
{

    Handle(ShapeSchema) schema = new ShapeSchema();
    Handle(Storage_Data) data  = new Storage_Data();
    data->ClearErrorStatus();

    data->SetApplicationName( TCollection_ExtendedString( "MwCAE" ) );
    data->SetApplicationVersion( "1" );
    data->SetDataType( TCollection_ExtendedString( "Shapes" ) );
    data->AddToUserInfo( "Storing the shape in a flat file" );
    data->AddToComments( TCollection_ExtendedString( "Application is based on CasCade 6.2" ) );

    if(!fileDriver) fileDriver=new FSD_File();
    if ( fileDriver->Open( fileName, Storage_VSWrite ) != Storage_VSOk ) return false;

    PTColStd_TransientPersistentMap aMap;
//---------
    for (Standard_Integer I=1;I<=shapes.Extent();I++){
      TCollection_AsciiString name=I;
      TopoDS_Shape S =shapes.FindKey(I);
      Handle(PTopoDS_HShape) pS = MgtBRep::Translate(S, aMap, MgtBRep_WithoutTriangle );
      data->AddRoot(name, pS);
    }
//---------
    schema->Write( *fileDriver, data );
    fileDriver->Close();

    return ( data->ErrorStatus() == Storage_VSOk );
}

void touchModified(const char* file){
std::string fname=file;
fname+=".modified";
FILE * f = fopen (fname.c_str(),"w");
fputs (" ",f);
fclose(f);
/*
    int len=strlen(file)+30;
    char *tch=(char*) malloc(len * sizeof(char));
    strcpy(tch, "touch ");
    strcat(tch, file);
    strcat(tch, ".modified");
    system(tch);
    free(tch);
*/
}

bool replaceIfDiff(const char* newfile, const char *oldfile){
    int len=strlen(newfile)+strlen(oldfile)+20;
    char *diff=(char*) malloc(len * sizeof(char));
    char *mv=(char*) malloc(len * sizeof(char));
    char *rm=(char*) malloc(len * sizeof(char));
    strcpy(diff, "diff ");
    strcat(diff, newfile);
    strcat(diff, "  ");
    strcat(diff, oldfile);
    strcpy(mv, "mv ");
    strcat(mv, newfile);
    strcat(mv, "  ");
    strcat(mv, oldfile);
    strcpy(rm, "rm ");
    strcat(rm, newfile);
    bool differ=system(diff);
    if(differ)  system(mv);
    else        system(rm);
    free(diff);
    free(mv);
    free(rm);
    return differ;
}


void saveShapeIfDiff(const char* file, TopoDS_Shape shape, const char *tmpfile, bool createModified){
    if(!FileExists(file)) { 
	saveShape( file, shape ); 
	if (createModified) touchModified(file); 
	return; 
    }
    if(!saveShape( tmpfile, shape )) return;
    bool differ=replaceIfDiff(tmpfile, file);
    if (differ && createModified) touchModified(file);
}


void saveShapesIfDiff(const char* file, TopTools_IndexedMapOfShape &shapes, const char *tmpfile,
	              bool createModified
		      )
{
    if(!FileExists(file)) { 
	  saveShapes( file, shapes ); 
	  if (createModified) touchModified(file); 
	  return; 
    }
    if(!saveShapes( tmpfile, shapes )) return;
    bool differ=replaceIfDiff(tmpfile, file);
    if (differ && createModified) touchModified(file);
}



bool openLocation( const char* fileName,  TopLoc_Location  &loc)
{
    // Check file type
    if ( FSD_File::IsGoodFileType( fileName ) != Storage_VSOk )  return false;

    if(!fileDriver) fileDriver=new FSD_File();
    TCollection_AsciiString aName( fileName );
    if ( fileDriver->Open( aName, Storage_VSRead ) != Storage_VSOk ) return false;

    Handle(ShapeSchema) schema = new ShapeSchema();
    Handle(Storage_Data) data  = schema->Read( *fileDriver );
    if ( data->ErrorStatus() != Storage_VSOk ) return false;

    fileDriver->Close();

    Handle(Storage_HSeqOfRoot) roots = data->Roots();
    Handle(Storage_Root) r = roots->Value( 1 );
    Handle(Standard_Persistent) p = r->Object();
    Handle(PTopLoc_ItemLocation) pItemLoc = Handle(PTopLoc_ItemLocation)::DownCast(p);
    if ( pItemLoc.IsNull() ) return 1;
    PTColStd_PersistentTransientMap aMap;
    PTopLoc_Location ploc;  ploc._CSFDB_SetPTopLoc_LocationmyData(pItemLoc);
    loc=MgtTopLoc::Translate( ploc, aMap);
    return true;


}


bool saveLocation( const char* fileName,  TopLoc_Location loc)
{

    Handle(ShapeSchema) schema = new ShapeSchema();
    Handle(Storage_Data) data  = new Storage_Data();
    data->ClearErrorStatus();

    data->SetApplicationName( TCollection_ExtendedString( "MwCAE" ) );
    data->SetApplicationVersion( "1" );
    data->SetDataType( TCollection_ExtendedString( "Shapes" ) );
    data->AddToUserInfo( "Storing the CAD Solids in a flat file" );
    data->AddToComments( TCollection_ExtendedString( "Application is based on CasCade 6.2" ) );

    if(!fileDriver) fileDriver=new FSD_File();
    if ( fileDriver->Open( fileName, Storage_VSWrite ) != Storage_VSOk ) return false;

    PTColStd_TransientPersistentMap aMap;
    PTopLoc_Location ploc = MgtTopLoc::Translate(loc, aMap);
    Handle(PTopLoc_ItemLocation) pItemLoc=ploc._CSFDB_GetPTopLoc_LocationmyData();
    data->AddRoot(pItemLoc);

    schema->Write( *fileDriver, data );
    fileDriver->Close();

    return ( data->ErrorStatus() == Storage_VSOk );
}


void saveLocationIfDiff(const char* file,  TopLoc_Location loc, const char *tmpfile){
    if(!FileExists(file)) { saveLocation( file, loc ); return; }
    if(!saveLocation( tmpfile, loc )) return;
    replaceIfDiff(tmpfile, file);
}




bool checkFacetedBrep( const Handle_TopTools_HSequenceOfShape& shapes )
{
	bool err = false;
	for ( int i = 1; i <= shapes->Length(); i++ )
	{
	    TopoDS_Shape shape = shapes->Value( i );
        for ( TopExp_Explorer fexp( shape, TopAbs_FACE ); fexp.More() && !err; fexp.Next() )
		{
		    Handle(Geom_Surface) surface = BRep_Tool::Surface( TopoDS::Face( fexp.Current() ) );
		    if ( !surface->IsKind( STANDARD_TYPE( Geom_Plane ) ) )
		        err = true;
		}
        for ( TopExp_Explorer eexp( shape, TopAbs_EDGE ); eexp.More() && !err; eexp.Next() )
		{
		    Standard_Real fd, ld;
		    Handle(Geom_Curve) curve = BRep_Tool::Curve( TopoDS::Edge( eexp.Current() ), fd, ld );
		    if ( !curve->IsKind( STANDARD_TYPE( Geom_Line ) ) )
		        err = true;
		}
	}
	return !err;
}



