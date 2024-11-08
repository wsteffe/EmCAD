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


#include <stdarg.h>

#define FATAL          1  // Fatal error (causes program to exit)
#define FATAL1         2  // First part of a multiline FATAL message 
#define FATAL2         3  // Middle part of a multiline FATAL message
#define FATAL3         4  // Last part of a multiline FATAL message  

#define ERROR          5  // Error (but program can live with it)
#define ERROR1         6  // First part of a multiline ERROR message 
#define ERROR2         7  // Middle part of a multiline ERROR message
#define ERROR3         8  // Last part of a multiline ERROR message  

#define WARNING        9  // Warning
#define WARNING1      10  // First part of a multiline WARNING message 
#define WARNING2      11  // Middle part of a multiline WARNING message
#define WARNING3      12  // Last part of a multiline WARNING message  

#define INFO          13  // Long informations
#define INFO1         14  // First part of a multiline INFO message 
#define INFO2         15  // Middle part of a multiline INFO message
#define INFO3         16  // Last part of a multiline INFO message  


#define DIRECT        30  // Direct message (no special formatting)

#define FATAL_STR          "Fatal Error : "
#define FATAL_NIL          "            : "
#define ERROR_STR          "Error : "
#define ERROR_NIL          "      : "
#define WARNING_STR        "Warning : "
#define WARNING_NIL        "        : "
#define INFO_STR           "Info : "
#define INFO_NIL           "     : "
#define DEBUG_STR          "Debug : "
#define DEBUG_NIL          "      : "

namespace DB {

void  Msg(int level, char *fmt, ...);

}
