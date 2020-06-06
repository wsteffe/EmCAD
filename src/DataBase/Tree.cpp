
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


#include <stdlib.h>
#include <string.h>

#include "Malloc.h"
#include "Tree.h"
#include "Message.h"

namespace DB {

Tree_T *Tree_Create(int size, int (*fcmp) (const void *a, const void *b))
{
  Tree_T *tree;

  tree = (Tree_T *) Malloc(sizeof(Tree_T));

  tree->size = size;
  tree->root = avl_init_table(fcmp);

  return (tree);
}

void Tree_Delete(Tree_T * tree)
{
  if(!tree)
    return;
  avl_free_table(tree->root, Free, 0);
  Free(tree);
}

void Tree_Add(Tree_T * tree, void *data)
{
  void *ptr;

  if(!tree)
    Msg(ERROR, "Impossible to add in unallocated tree");
  else {
    ptr = Malloc(tree->size);
    memcpy(ptr, data, tree->size);
    avl_insert(tree->root, ptr, ptr);
  }
}

void *Tree_AddP(Tree_T * tree, void *data)
{
  void *ptr;

  if(!tree)
    Msg(FATAL, "Impossible to add in unallocated tree");
  ptr = Malloc(tree->size);
  memcpy(ptr, data, tree->size);
  avl_insert(tree->root, ptr, ptr);
  return ptr;
}

int Tree_Nbr(Tree_T * tree)
{
  if(!tree)
    return 0;
  return (avl_count(tree->root));
}

int Tree_Insert(Tree_T * tree, void *data)
{
  if(Tree_Search(tree, data) == 0){
    Tree_Add(tree, data);
    return 1;
  }
  else{
    return 0;
  }
}

int Tree_Replace(Tree_T * tree, void *data)
{
  void *ptr;
  int state;

  if(!tree) {
    Msg(ERROR, "Impossible to replace in unallocated tree");
    return (0);
  }
  state = avl_lookup(tree->root, data, &ptr);
  if(state == 0) {
    Tree_Add(tree, data);
    return (0);
  }
  else {
    memcpy(ptr, data, tree->size);
    return (1);
  }
}

int Tree_Search(Tree_T * tree, void *data)
{
  void *ptr;

  if(!tree)
    return 0;
  return (avl_lookup(tree->root, data, &ptr));
}

int Tree_Query(Tree_T * tree, void *data)
{
  void *ptr;
  int state;

  if(!tree)
    return 0;

  state = avl_lookup(tree->root, data, &ptr);

  if(state == 0)
    return (0);

  memcpy(data, ptr, tree->size);

  return (1);
}

void *Tree_PQuery(Tree_T * tree, void *data)
{
  void *ptr;
  int state;

  if(!tree)
    return 0;

  state = avl_lookup(tree->root, data, &ptr);

  if(state == 0)
    return (NULL);
  return (ptr);
}

int Tree_Suppress(Tree_T * tree, void *data)
{
  void *ptr;
  int state;

  if(!tree)
    return 0;

  ptr = data;
  state = avl_delete(tree->root, &ptr, &ptr);
  if(state == 0)
    return (0);

  Free(ptr);
  return (1);
}

int Tree_Left(Tree_T * tree, void *data)
{
  void *ptr;
  int state;

  if(!tree)
    return 0;

  state = avl_extremum(tree->root, AVL_MOST_LEFT, &ptr);

  if(state == 0)
    return (0);

  memcpy(data, ptr, tree->size);

  return (1);
}

int Tree_Right(Tree_T * tree, void *data)
{
  void *ptr;
  int state;

  if(!tree)
    return 0;

  state = avl_extremum(tree->root, AVL_MOST_RIGHT, &ptr);

  if(state == 0)
    return (0);

  memcpy(data, ptr, tree->size);

  return (1);
}

int Tree_Size(Tree_T * tree)
{
  if(!tree)
    return 0;

  return (tree->size);
}

}
