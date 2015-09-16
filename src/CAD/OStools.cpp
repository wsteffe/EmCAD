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

#include<vector>
#include <stdio.h>
#include <boost/filesystem.hpp>

#if defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#endif

#include "OStools.h"

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

std::string convertFileNameToWindowsStyle(const std::string& fileName)
{
    std::string new_fileName(fileName);
    
    std::string::size_type slash = 0;
    while( (slash=new_fileName.find_first_of(UNIX_PATH_SEPARATOR,slash)) != std::string::npos)
    {
        new_fileName[slash]=WINDOWS_PATH_SEPARATOR;
    }
    return new_fileName;
}

std::string convertFileNameToUnixStyle(const std::string& fileName)
{
    std::string new_fileName(fileName);
    
    std::string::size_type slash = 0;
    while( (slash=new_fileName.find_first_of(WINDOWS_PATH_SEPARATOR,slash)) != std::string::npos)
    {
        new_fileName[slash]=UNIX_PATH_SEPARATOR;
    }

    return new_fileName;
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
  FILE *fp=fopenWithLock(to, "w");
  fcloseWithLock(fp);
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
  return convertFileNameToWindowsStyle(name);
#else
  return convertFileNameToUnixStyle(name);
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


#if defined(_WIN32) || defined(__WIN32__)
FILE * fopenWithLock(const char *filename, const char * mode)
{  
   FILE *fp;
   std::string native_filename=nativePath(filename);

   DWORD _mode;
   if (!strcmp(mode,"w")) _mode=GENERIC_WRITE;
   else if (!strcmp(mode,"r")) _mode=GENERIC_READ;
   else if (!strcmp(mode,"a")) _mode=GENERIC_WRITE | GENERIC_READ;
   while(1) {
    HANDLE h = CreateFile(native_filename.c_str(), _mode, 0, 0,
        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(h != INVALID_HANDLE_VALUE)
    {
        int fd = _open_osfhandle((intptr_t)h, _O_TEXT);
        if(fd != -1){
            FILE* fp = _fdopen(fd, "a+");
            if(fp != 0) break;
	}
    }
    Sleep(1000);
   }

   return fp;
}
#else
FILE * fopenWithLock(const char *filename, const char * mode)
{  
   FILE *fp;
   std::string native_filename=nativePath(filename);
   fp =fopen(native_filename.c_str(), mode);
   flockfile(fp);
   return fp;
}
#endif



void fcloseWithLock(FILE *fp){
#if !defined(_WIN32) || !defined(__WIN32__)
  funlockfile(fp);
#endif
  fclose(fp);
}



/*
void removedir(const char *dir)
{
  path p(dir);
  remove_all(p);
}
*/


