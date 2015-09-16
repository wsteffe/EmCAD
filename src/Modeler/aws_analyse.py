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

import sys,time
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

anatype="0"
for i,arg in enumerate(argv):
    if arg=="-szp":
       argv.pop(i)
       anatype="szp"
       break

symminput=False
for i,arg in enumerate(argv):
    if arg=="-symminput":
       symminput=True
       break

argl=len(argv)
if anatype=="0":
  argv[0]="analyse"
elif anatype=="szp":
  argv[0]="analyseszp"

polling_wait_time=10 #sec

fname=argv[argl-1]
JC_fname=fname+'.JC'
loadsJC_fname='extern_port_loads.JC'
log_fname=fname+'.log'
ts_fname=fname+'.ts'
szp_fname=fname+'.SZP'
res_fname=fname+'.res'
symm_fname='symmfilter.inp'

sqs_conn = boto.connect_sqs()
#q = sqs_conn.get_queue(quename)
q = boto.sqs.queue.Queue(sqs_conn, que_url)

s3_conn = boto.connect_s3()
bucket = s3_conn.get_bucket(bucketname)
folder=project

JC_key=bucket.get_key(folder+'/'+JC_fname)
if JC_key is None:
    JC_key=bucket.new_key(folder+'/'+JC_fname)
    try:
        JC_key.set_contents_from_filename(JC_fname)
    except S3ResponseError as e:
        sys.exit(-1)
    bucket.set_acl('public-read', JC_key.name)

loadsJC_key=bucket.get_key(folder+'/'+loadsJC_fname)
if loadsJC_key is None:
    loadsJC_key=bucket.new_key(folder+'/'+loadsJC_fname)
    try:
        loadsJC_key.set_contents_from_filename(loadsJC_fname)
    except S3ResponseError as e:
        sys.exit(-1)
    bucket.set_acl('public-read', loadsJC_key.name)


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

if anatype=="0":
  ts_key=bucket.get_key(folder+'/'+ts_fname)
  if ts_key is not None:
    ts_key.get_contents_to_filename(ts_fname)
elif anatype=="szp":
  szp_key=bucket.get_key(folder+'/'+szp_fname)
  if szp_key is not None:
    szp_key.get_contents_to_filename(szp_fname)
  if symminput:
    symm_key=bucket.get_key(folder+'/'+symm_fname)
    if symm_key is not None:
       symm_key.get_contents_to_filename(symm_fname)

sys.exit(exitcode)


