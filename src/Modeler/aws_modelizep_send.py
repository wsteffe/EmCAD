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


#quename='EmCAD'

import os,sys,time
import json, boto
from boto.sqs.message import Message
from boto.exception import SQSError, S3ResponseError


argv=sys.argv

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

for i,arg in enumerate(argv):
    if arg=="-queue":
       argv.pop(i)
       que_url=argv.pop(i)
       break


argv[0]="modelizep"


argl=len(argv)
fname=argv[argl-1]
mwm_fname=fname+'.mwm'
res_fname=fname+'.res'

sqs_conn = boto.connect_sqs()
if sqs_conn is None:
    sys.exit(1)

que = boto.sqs.queue.Queue(sqs_conn, que_url)
if que is None:
    sys.exit(1)


s3_conn = boto.connect_s3()
bucket = s3_conn.get_bucket(bucketname)
folder=project

mwm_key=bucket.get_key(folder+'/'+mwm_fname)
if mwm_key is None:
    mwm_key=bucket.new_key(folder+'/'+mwm_fname)

try:
    mwm_key.set_contents_from_filename(mwm_fname)
except S3ResponseError as e:
    sys.exit(-1)

bucket.set_acl('public-read', mwm_key.name)

#mwm_key.add_user_grant('READ', 'hierarchical-electromagnetics')

res_key=bucket.get_key(folder+'/'+res_fname)
if res_key is not None:
   res_key.delete()

mtxt=json.dumps({'bucket': bucket.name, 'folder': folder, 'argv': argv })
m = Message()
m.set_body(mtxt)
try:
  status = que.write(m)
except SQSError as e:
  sys.exit(1)

if(not status):
  sys.exit(1)



