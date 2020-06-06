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


#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#if defined(HAVE_OCE_CONFIG_H)
 #include <oce-config.h>
#elif defined(HAVE_OCC_CONFIG_H)
 #include <config.h>
#endif

#include <TopoDS_Shape.hxx>
#include <TDocStd_Document.hxx>
#include <TopTools_IndexedMapOfShape.hxx>


class AIS_InteractiveContext;
class TopTools_HSequenceOfShape;

class ImportFailure{};


void importSTEP(Handle(TDocStd_Document) doc, char* fileName, char *lengthUnitName=NULL );
void importIGES(Handle(TDocStd_Document) doc, char* fileName );

bool  replaceIfDiff(const char* newfile, const char *oldfile);
bool  openShape ( const char* fileName, TopoDS_Shape &shape);
bool  openShapes ( const char* fileName, TopTools_IndexedMapOfShape &shapes);
bool  saveShape ( const char* fileName, TopoDS_Shape  shape);
bool  saveShapes ( const char* fileName, TopTools_IndexedMapOfShape &shapes);
void  saveShapeIfDiff(const char* file, TopoDS_Shape shape, const char *tmpfile, bool createModified=false);
void  saveShapesIfDiff(const char* file, TopTools_IndexedMapOfShape &shapes, const char *tmpfile, bool createModified=false);
bool  openLocation( const char* fileName,  TopLoc_Location  &loc);
bool  saveLocation ( const char* fileName, TopLoc_Location loc);
void saveLocationIfDiff(const char* file,  TopLoc_Location loc, const char *tmpfile);

bool checkFacetedBrep( const Handle(TopTools_HSequenceOfShape)& );

#endif
