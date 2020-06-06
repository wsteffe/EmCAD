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


#ifndef PORTABILITY_H
#define PORTABILITY_H

#include<string>
#include<istream>

std::istream& getLine( std::istream& is, std::string& str);
void toUnix(std::string &str);
bool FileExists(const char* filename);
bool FileIsSymlink(const char* filename);
void removeFile(const char* filename);
bool removeAllFilesInDir(const char *dirname);
void createLink(const char* to, const char* link);
std::string nativePath(const char *name);
std::string nativePath(std::string name);
std::string cleanWindowsPath(std::string name);
bool file1NewerThanFile2(const char *file1, const char *file2);
bool createdir(const char *dir);

int setLock(FILE * fh, const char *mode);
int releaseLock(FILE * fh);

//void removedir(const char *dir);

#endif
