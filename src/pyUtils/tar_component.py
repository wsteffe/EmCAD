#! /usr/bin/env python3

# This file is part of the EmCAD program which constitutes the client
# side of an electromagnetic modeler delivered as a cloud based service.
# 
# Copyright (C) 2015-2020  Walter Steffe`
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


import sys
import tarfile,os

cmp_name=sys.argv[1]
cmp_path=sys.argv[2]
subcmpNum=sys.argv[3]
tar_dir=sys.argv[4]

start_dir=os.getcwd()
os.chdir(cmp_path)
if int(subcmpNum) == 0:
  tar = tarfile.open(tar_dir+"/"+cmp_name+".tgz", 'w:gz', compresslevel=9)
  tar.add("interfaces")
  tar.add("assemblyType")
  tar.add("Partition/interfaces")
  tar.add("Partition/partition.map")
  tar.add("Partition/model.em")
  tar.add("Partition/model.xml")
  tar.close()

if int(subcmpNum) > 0:
  for I in range(int(subcmpNum)):
    subcmpI=str(I+1)
    tarI = tarfile.open(tar_dir+"/"+cmp_name+"_SUB"+subcmpI+".tgz", 'w:gz', compresslevel=9)
    tarI.add("interfaces")
    tarI.add("Partition/interfaces")
    tarI.add("Partition/partition.map")
    tarI.add("Partition/partition_"+subcmpI+".map")
    tarI.add("Partition/model.em")
    tarI.add("Partition/model_"+subcmpI+".xml")
    tarI.close()

os.chdir(start_dir)

