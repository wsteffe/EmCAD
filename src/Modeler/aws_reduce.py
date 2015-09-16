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

import sys,os,time
import fnmatch
import json, boto
from boto.sqs.message import Message
from boto.exception import SQSError, S3ResponseError
#import uuid

argv=sys.argv
#argv=["-project", "net", "modelizeb", "-k", "2", "-freq", "0.00000", "12.00000", "GHz", "20", "NET_ASM"]

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

argv[0]="reduce"

polling_wait_time=10 #sec

argl=len(argv)
fname=argv[argl-1]
MJC_fname=fname+'.MJC'
log_fname=fname+'.log'
RM_JC_fname=fname+'_RM.JC'
RM_SP_fname=fname+'_RM.sp'
res_fname=fname+'.res'

sqs_conn = boto.connect_sqs()
#q = sqs_conn.get_queue(quename)
q = boto.sqs.queue.Queue(sqs_conn, que_url)

s3_conn = boto.connect_s3()
bucket = s3_conn.get_bucket(bucketname)
folder=project

files=os.listdir('.')

for f in files:
    if fnmatch.fnmatch(f, '*.MJC') or fnmatch.fnmatch(f, '*.bin'):
        key=bucket.get_key(folder+'/'+f)
        if key is None:
            key=bucket.new_key(folder+'/'+f)
            try:
                key.set_contents_from_filename(f)
                bucket.set_acl('public-read', key.name)
            except S3ResponseError as e:
                sys.exit(-1)


#mwmkey.add_user_grant('READ', mwserverID)


reskey=bucket.get_key(folder+'/'+res_fname)
if reskey is not None:
   reskey.delete()

mtxt=json.dumps({'bucket': bucket.name, 'folder': folder, 'argv': argv })
m = Message()
m.set_body(mtxt)
status = q.write(m)

reskey=bucket.get_key(folder+'/'+res_fname)
while reskey is None: 
  time.sleep(polling_wait_time)
  reskey=bucket.get_key(folder+'/'+res_fname)

reskey.get_contents_to_filename(res_fname)
res_f = open(res_fname, 'r')
exitcode=int(res_f.readline())
res_f.close()

logkey=bucket.get_key(folder+'/'+log_fname)
if logkey is not None:
  logkey.get_contents_to_filename(log_fname)

RM_JC_key=bucket.get_key(folder+'/'+RM_JC_fname)
if RM_JC_key is not None:
  RM_JC_key.get_contents_to_filename(RM_JC_fname)

RM_SP_key=bucket.new_key(folder+'/'+RM_SP_fname)
if RM_SP_key is not None:
  RM_SP_key.get_contents_to_filename(RM_SP_fname)


sys.exit(exitcode)


