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

#include<vector>
#include <stdio.h>
#include <boost/filesystem.hpp>


#include <sys/file.h>
#include "OStools.h"
#include <iostream>

void toUnix(std::string &str){
 if(str.size()) if (str[str.size() - 1] == '\r') str.resize(str.size() - 1);
}
std::istream& getLine( std::istream& is, std::string& str){
 getline(is,str,'\n');
 toUnix(str);
 return is;
}

const char UNIX_PATH_SEPARATOR = '/';
const char WINDOWS_PATH_SEPARATOR = '\\';

std::string convertPathToWindowsStyle(std::string fileName)
{
    
    std::string::size_type slash = 0;
    while( (slash=fileName.find_first_of(UNIX_PATH_SEPARATOR,slash)) != std::string::npos)
    {
        fileName[slash]=WINDOWS_PATH_SEPARATOR;
    }
    return fileName;
}

std::string convertPathToUnixStyle(std::string fileName)
{
    std::string::size_type slash = 0;
    while( (slash=fileName.find_first_of(WINDOWS_PATH_SEPARATOR,slash)) != std::string::npos)
    {
        fileName[slash]=UNIX_PATH_SEPARATOR;
    }

    return fileName;
}

std::string cleanWindowsPath(std::string fileName)
{
    std::string::size_type p=0;
    int l=fileName.length();
    for (int i=l-2; i>=0; i--)
       if(fileName[i]==WINDOWS_PATH_SEPARATOR && fileName[i+1]==WINDOWS_PATH_SEPARATOR){ 
          l--;
   	  for (int j=i+1; j< l; j++) fileName[j]=fileName[j+1];
        }
    fileName=fileName.substr(0,l);
    return fileName;
}



using namespace boost::filesystem;

bool FileExists(const char* filename) { 
 path p(filename);
 file_status s =status(p);
 return exists(s);
}

bool FileIsSymlink(const char* filename) {
 path p(filename);
 file_status s =symlink_status(p);
 return is_symlink(s);
}

void createLink(const char* to, const char* link) {
#ifdef USE_SYMBOLIC_LINKS
 create_symlink(to, link);
#else
 path pto(to);
 file_status s =status(pto);
 if(!exists(s)){
  FILE *fp=fopen(to, "w");
  fclose(fp);
 }
 create_hard_link(to, link);
#endif
}

void removeFile(const char* filename)
{
 path p(filename);
 remove(p);
}
bool removeAllFilesInDir(const char *dirname)
{
    path p(dirname);
    if (!exists(p)){
        fprintf(stderr, "Null Directory Error\n");
        return false;
    }
    if (is_directory(p)){
       typedef std::vector<path> vec;     // store paths,
       vec v;                       // so we can sort them later
       copy(directory_iterator(p), directory_iterator(), back_inserter(v));
       for (vec::const_iterator it(v.begin()), it_end(v.end()); it != it_end; ++it) if (!is_directory(*it)) remove(*it);
    }
    return true;
}


std::string nativePath(std::string name)
{
#if defined(_WIN32) || defined(__WIN32__)
  return convertPathToWindowsStyle(name);
#else
  return convertPathToUnixStyle(name);
#endif
}

std::string nativePath(const char *name)
{
  std::string sname=name;
  return nativePath(sname);
}

/*
std::string nativePath(const char *name)
{
  path p(name);
#ifdef WIN32
  p.make_preferred();
#endif
  return p.string();
}

std::string nativePath(std::string name)
{
  path p(name.c_str());
#ifdef WIN32
  p.make_preferred();
#endif
  return p.string();
}
*/

bool file1NewerThanFile2(const char *file1, const char *file2)
{
  path p1(file1);
  path p2(file2);
  return difftime(last_write_time(p1),last_write_time(p2))>0;
}

bool createdir(const char *dir)
{
  path p(dir);
  return create_directory(p);
}




#if defined(_MINGW)
  #include <windows.h>
  #include <errno.h>
  #include <io.h>


  int setLock(FILE * fh, const char *mode)
  {
    int fd = fileno(fh);
    HANDLE h = (HANDLE) _get_osfhandle (fd);

    DWORD size_lower=1;
    DWORD size_upper=0;

    OVERLAPPED ovlp;
    ovlp.Offset=0;
    ovlp.OffsetHigh=0;
    ovlp.hEvent=NULL;

    if(mode[0]=='w'||mode[0]=='a') {
	   if(!LockFileEx (h, LOCKFILE_EXCLUSIVE_LOCK, 0, size_lower, size_upper, &ovlp)){ 
		 std::cerr << "ERROR: could not obtain write lock \n";
		 return -1;
	   }
    } else if(mode[0]=='r') {
	   if(!LockFileEx (h, 0, 0, size_lower, size_upper, &ovlp)){
		 std::cerr << "ERROR: could not obtain read lock \n";
		 return -1;
	   }
    } else {
	    std::cerr << "ERROR: invalid mode " << mode <<"\n";
	    return -1;
    }
    return 0;
}

int releaseLock(FILE * fh){
    int fd = fileno(fh);
    HANDLE h = (HANDLE) _get_osfhandle (fd);

    DWORD size_lower=1;
    DWORD size_upper=0;
    DWORD OffsetLow=0;
    DWORD OffsetHigh=0;

    if(!UnlockFile(h, OffsetLow, OffsetHigh, size_lower, size_upper)) {
      std::cerr << "ERROR: could not remove the lock \n";
      return -1;
    }
    
    return 0;
}


#else


#include <fcntl.h>
int setLock(FILE * fh, const char *mode)
{
    int fd = fileno(fh);
    struct flock fl;
    fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
    fl.l_start  = 0;        /* Offset from l_whence         */
    fl.l_len    = 0;        /* length, 0 = to EOF           */
    fl.l_pid    = getpid(); /* our PID                      */
    
    if(mode[0]=='w'||mode[0]=='a') {
           fl.l_type= F_WRLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK    */
	   if(fcntl(fd, F_SETLKW, &fl) == -1){ 
		 std::cerr << "ERROR: could not obtain write lock \n";
		 return -1;
	   }
    } else if(mode[0]=='r') {
           fl.l_type= F_RDLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK    */
	   if(fcntl(fd, F_SETLKW, &fl) == -1){
		 std::cerr << "ERROR: could not obtain read lock \n";
		 return -1;
	   }
    } else if(mode[0]=='0'){
           fl.l_type= F_UNLCK;  /* tell it to unlock the region */
	   if(fcntl(fd, F_SETLK, &fl) == -1) {
		 std::cerr << "ERROR: could not remove the lock \n";
		 return -1;
	   }
    } else {
	    std::cerr << "ERROR: invalid mode " << mode <<"\n";
	    return -1;
    }
    return 0;
}

int releaseLock(FILE * fh){setLock(fh, "0");}


#endif


/*
void removedir(const char *dir)
{
  path p(dir);
  remove_all(p);
}
*/


