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


#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "ListUtils.h"
#include "TreeUtils.h"

namespace DB {

int fcmp_int(const void *a, const void *b);
int fcmp_absint(const void *a, const void *b);
int fcmp_double(const void *a, const void *b);

List_T *Tree2List(Tree_T *pTree) ;

Tree_T *Tree_Duplique(Tree_T *pTree) ;
Tree_T *Tree_Union       (Tree_T *pTreeA, Tree_T *pTreeB) ;
Tree_T *Tree_Soustraction(Tree_T *pTreeA, Tree_T *pTreeB) ;
Tree_T *Tree_Intersection(Tree_T *pTreeA, Tree_T *pTreeB) ;

void Tree_Unit      (Tree_T *pTreeA, Tree_T *pTreeB) ;
void Tree_Soustrait (Tree_T *pTreeA, Tree_T *pTreeB) ;
void Tree_Intersecte(Tree_T *pTreeA, Tree_T *pTreeB) ;

}

#endif
