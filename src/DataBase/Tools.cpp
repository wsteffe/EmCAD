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
#include <math.h>
#include "Tools.h"

// Comparison functions
namespace DB {

int fcmp_int(const void *a, const void *b)
{
  return (*(int *)a - *(int *)b);
}

int fcmp_absint(const void *a, const void *b)
{
  return (abs(*(int *)a) - abs(*(int *)b));
}

int fcmp_double(const void *a, const void *b)
{
  double cmp;

  cmp = *(double *)a - *(double *)b;
  if(cmp > 1.e-16)
    return 1;
  else if(cmp < -1.e-16)
    return -1;
  else
    return 0;
}

// Tree -> List transfer

List_T *pListeTransfert;

void TransfereListe(void *a, void *b)
{
  List_Add(pListeTransfert, a);
}

List_T *Tree2List(Tree_T * pTree)
{
  int Nb;
  Nb = Tree_Nbr(pTree);
  if(Nb == 0)
    Nb = 1;
  pListeTransfert = List_Create(Nb, Nb, Tree_Size(pTree));
  Tree_Action(pTree, TransfereListe);
  return (pListeTransfert);
}

// Algebraic utilities

Tree_T *pTreeTransfert;
Tree_T *pTreeTransfert2;

void DupliqueArbre(void *a, void *b)
{
  Tree_Add(pTreeTransfert, a);
}

Tree_T *Tree_Duplique(Tree_T * pTree)
{
  pTreeTransfert = Tree_Create(pTree->size, pTree->root->compar);
  Tree_Action(pTree, DupliqueArbre);
  return (pTreeTransfert);
}

void UnitArbre(void *a, void *b)
{
  Tree_Replace(pTreeTransfert, a);
}

Tree_T *Tree_Union(Tree_T * pTreeA, Tree_T * pTreeB)
{
  pTreeTransfert = Tree_Duplique(pTreeA);
  Tree_Action(pTreeB, UnitArbre);
  return (pTreeTransfert);
}

void Tree_Unit(Tree_T * pTreeA, Tree_T * pTreeB)
{
  pTreeTransfert = pTreeA;
  Tree_Action(pTreeB, UnitArbre);
}

void SoustraitArbre(void *a, void *b)
{
  Tree_Suppress(pTreeTransfert, a);
}

Tree_T *Tree_Soustraction(Tree_T * pTreeA, Tree_T * pTreeB)
{
  pTreeTransfert = Tree_Duplique(pTreeA);
  Tree_Action(pTreeB, SoustraitArbre);
  return (pTreeTransfert);
}

void Tree_Soustrait(Tree_T * pTreeA, Tree_T * pTreeB)
{
  pTreeTransfert = pTreeA;
  Tree_Action(pTreeB, SoustraitArbre);
}

void IntersecteArbre(void *a, void *b)
{
  if(Tree_Query(pTreeTransfert, a))
    Tree_Add(pTreeTransfert2, a);
}

Tree_T *Tree_Intersection(Tree_T * pTreeA, Tree_T * pTreeB)
{
  pTreeTransfert = Tree_Duplique(pTreeA);
  pTreeTransfert2 = Tree_Create(pTreeA->size, pTreeA->root->compar);
  Tree_Action(pTreeB, IntersecteArbre);
  Tree_Delete(pTreeTransfert);
  return (pTreeTransfert2);
}

void Tree_Intersecte(Tree_T * pTreeA, Tree_T * pTreeB)
{
  pTreeTransfert2 = pTreeA;
  pTreeTransfert = Tree_Create(pTreeA->size, pTreeA->root->compar);
  Tree_Action(pTreeB, IntersecteArbre);
  pTreeA = pTreeTransfert2;
  Tree_Delete(pTreeA);
}

}
