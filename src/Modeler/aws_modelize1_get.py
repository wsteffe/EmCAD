#! /usr/bin/env python

# This file is part of the EmCAD program which constitutes the client
# side of an electromagnetic modeler delivered as a cloud based service.
# 
# Copyright (C) 2015  Walter Steffe
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
import boto
from boto.exception import S3ResponseError

argv=sys.argv
#argv=["-project", "net", "modelizeb", "-k", "2", "-freq", "0.00000", "12.00000", "GHz", "5", "NET_ASM__CONNECTOR_INST1"]

for i,arg in enumerate(argv):
    if arg=="-project":
       argv.pop(i)
       project=argv.pop(i)
       break

for i,arg in enumerate(argv):
    if arg=="-bucket":
       argv.pop(i)
       bucketname=argv.pop(i)
       break


argl=len(argv)
fname=argv[argl-1]
log_fname=fname+'.log'
RM_JC_fname=fname+'_RM.JC'
RM_SP_fname=fname+'_RM.sp'
res_fname=fname+'.res'


s3_conn = boto.connect_s3()
bucket = s3_conn.get_bucket(bucketname)
folder=project


res_key=bucket.get_key(folder+'/'+res_fname)
if res_key is None:
   sys.exit(255)

res_key.get_contents_to_filename(res_fname)
res_f = open(res_fname, 'r')
exitcode=int(res_f.readline())
res_f.close()

log_key=bucket.get_key(folder+'/'+log_fname)
if log_key is not None:
  log_key.get_contents_to_filename(log_fname)

RM_JC_key=bucket.get_key(folder+'/'+RM_JC_fname)
if RM_JC_key is not None:
  RM_JC_key.get_contents_to_filename(RM_JC_fname)

RM_SP_key=bucket.new_key(folder+'/'+RM_SP_fname)
if RM_SP_key is not None:
  RM_SP_key.get_contents_to_filename(RM_SP_fname)


sys.exit(exitcode)


