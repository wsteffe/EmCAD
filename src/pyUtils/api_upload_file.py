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
import api_proxy

argv=sys.argv

token=None
for i,arg in enumerate(argv):
    if arg=="-token":
       argv.pop(i)
       token=argv.pop(i)
       break
if token is None:
    sys.exit(1)

folder=None
for i,arg in enumerate(argv):
    if arg=="-folder":
       argv.pop(i)
       folder=argv.pop(i)
       break
if folder is None:
    sys.exit(1)

for i,arg in enumerate(argv):
    if arg=="-proxyPacFile":
       argv.pop(i)
       api_proxy.getPac(argv.pop(i))
       break

api_proxy.verify=True
for i,arg in enumerate(argv):
    if arg=="-apiPemFile":
       argv.pop(i)
       api_proxy.verify=argv.pop(i)
       break

argl=len(argv)
fname=argv[argl-1]

if(os.path.exists(fname)):
   api_utils.upload_project_file(token,fname,folder,False)



