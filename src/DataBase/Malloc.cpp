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


#include <stdio.h>
#include <stdlib.h>

#include "Malloc.h"
#include "Message.h"

namespace DB {

void *Malloc(size_t size)
{
  void *ptr;

  if(!size)
    return (NULL);
  ptr = malloc(size);
  if(ptr == NULL)
    Msg(FATAL, "Out of memory (buy some more RAM!)");
  return (ptr);
}

void *Calloc(size_t num, size_t size)
{
  void *ptr;

  if(!size)
    return (NULL);
  ptr = calloc(num, size);
  if(ptr == NULL)
    Msg(FATAL, "Out of memory (buy some more RAM!)");
  return (ptr);
}

void *Realloc(void *ptr, size_t size)
{
  if(!size)
    return (NULL);
  ptr = realloc(ptr, size);
  if(ptr == NULL)
    Msg(FATAL, "Out of memory (buy some more RAM!)");
  return (ptr);
}

void Free(void *ptr)
{
  if(ptr == NULL)
    return;
  free(ptr);
}

}
