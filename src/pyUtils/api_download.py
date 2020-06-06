#! /usr/bin/env python3

# This file is part of the EmCAD program which constitutes the client
# side of an electromagnetic modeler delivered as a cloud based service.
# 
# Copyright (C) 2015-2020  Walter Steffe
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


import os,sys,time
import json
import api_utils

argv=sys.argv

username=None
for i,arg in enumerate(argv):
    if arg=="-user":
       argv.pop(i)
       username=argv.pop(i)
       break
if username is None:
    sys.exit(1)

password=None
for i,arg in enumerate(argv):
    if arg=="-password":
       argv.pop(i)
       password=argv.pop(i)
       break
if password is None:
    sys.exit(1)

folder=None
for i,arg in enumerate(argv):
    if arg=="-folder":
       argv.pop(i)
       folder=argv.pop(i)
       break
if folder is None:
    sys.exit(1)

saveUrl=False
for i,arg in enumerate(argv):
    if arg=="-saveUrl":
       argv.pop(i)
       saveUrl=True
       break


argl=len(argv)
filename=argv[argl-1]

signedUrl=None
if os.path.exists (filename+".url"):
   with open(filename+".url", 'r') as f:
       signedUrl=json.load(f)


signedUrls=api_utils.download_project_file(username,password,folder,filename,signedUrl=signedUrl)
if saveUrl and len(signedUrls)==1:
    if signedUrls[0]!=signedUrl:
       with open(filename+".url", 'w') as f:
           json.dump(signedUrls[0], f)




