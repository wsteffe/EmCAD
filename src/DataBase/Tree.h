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

#ifndef _TREE_H_
#define _TREE_H_



#include "avl.h"
#include "List.h"

namespace DB {

typedef struct {
  int size;
  avl_tree *root;
} Tree_T;

Tree_T *Tree_Create(int size, int (*fcmp)(const void *a, const void *b));
void    Tree_Delete(Tree_T *Tree);
void    Tree_Delete(Tree_T *Tree, void (*freefn)(void * ) );
void    Tree_Add(Tree_T *tree, void *data);
void   *Tree_AddP(Tree_T *tree, void *data);
int     Tree_Nbr(Tree_T *Tree);
int     Tree_Insert(Tree_T *Tree, void *data);
int     Tree_Replace(Tree_T *Tree, void *data);
int     Tree_Search(Tree_T *Tree, void *data);
int     Tree_Query(Tree_T *Tree, void *data);
void   *Tree_PQuery(Tree_T *Tree, void *data);
int     Tree_Suppress(Tree_T *Tree, void *data);
int     Tree_Left(Tree_T *tree, void *data);
int     Tree_Right(Tree_T *tree, void *data);
int     Tree_Size(Tree_T *tree);

inline void Tree_Action(Tree_T *tree, void (*action) (void *data, void *dummy))
{
  if(!tree) return;
  avl_foreach(tree->root, action, AVL_FORWARD);
}

List_T *Tree2List(Tree_T *pTree);

}
#endif

