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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Message.h"


namespace DB {


// General purpose message routine

void Debug()
{
  ;
}

void Msg(int level, char *fmt, ...)
{
  va_list args;
  int abort = 0, verb = 0, window = -1, log = 1;
  char *str = NULL;

  // *INDENT-OFF*
  switch(level){
    case DIRECT   : verb = 2; break ;

    case FATAL    : str = FATAL_STR; abort = 1; break ;
    case FATAL1   : str = FATAL_STR; break ;
    case FATAL2   : str = FATAL_NIL; break ;
    case FATAL3   : str = FATAL_NIL; abort = 1; break ;
				     		  
    case ERROR    : 		     		  
    case ERROR1   : str = ERROR_STR; break ;
    case ERROR2   : 		     
    case ERROR3   : str = ERROR_NIL; break ;
				     	  
    case WARNING  : 		     	  
    case WARNING1 : str = WARNING_STR; verb = 1; break ;
    case WARNING2 : 		     	  
    case WARNING3 : str = WARNING_NIL; verb = 1; break ;
				     	  
    case INFO     :		     	  
    case INFO1    : str = INFO_STR; verb = 2; break ;
    case INFO2    :		     	  
    case INFO3    : str = INFO_NIL; verb = 2; break ;
				     	  
    default : return;
  }
  // *INDENT-ON*

#define BUFFSIZE 1024

  static char buff1[BUFFSIZE], buff2[BUFFSIZE];

    va_start(args, fmt);
      strcpy(buff1, "@C1");
      if(str) strncat(buff1, str, BUFFSIZE-4);

      vsprintf(buff2, fmt, args);

      strncat(buff1, buff2, BUFFSIZE-strlen(buff1));
      fprintf(stderr, "\n%s\n", &buff1[3]);
    va_end(args);

    fflush(stderr);

  if(abort) {
    Debug(); exit(1);
  }

}


}
