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
import fnmatch
import json, boto
from boto.sqs.message import Message
from boto.exception import SQSError, S3ResponseError

open('s3.lock','w').close()

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

argv[0]="modelizep"

polling_wait_time=10 #sec


#argl=len(sys.argv)
#fname=sys.argv[argl-1]
argl=len(argv)
fname=argv[argl-1]
mwm_fname=fname+'.mwm'
log_fname=fname+'.log'
res_fname=fname+'.res'

sqs_conn = boto.connect_sqs()
#q = sqs_conn.get_queue(quename)
q = boto.sqs.queue.Queue(sqs_conn, que_url)

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
    os.remove('s3.lock')

bucket.set_acl('public-read', mwm_key.name)
os.remove('s3.lock')

#mwm_key.add_user_grant('READ', mwserverID)

res_key=bucket.get_key(folder+'/'+res_fname)
if res_key is not None:
   res_key.delete()

mtxt=json.dumps({'bucket': bucket.name, 'folder': folder, 'argv': argv })
m = Message()
m.set_body(mtxt)
status = q.write(m)

res_key=bucket.get_key(folder+'/'+res_fname)
while res_key is None: 
  time.sleep(polling_wait_time)
  res_key=bucket.get_key(folder+'/'+res_fname)

res_key.get_contents_to_filename(res_fname)
res_f = open(res_fname, 'r')
exitcode=int(res_f.readline())
res_f.close()

log_key=bucket.get_key(folder+'/'+log_fname)
if log_key is not None:
  log_key.get_contents_to_filename(log_fname)

for port_fname in os.listdir('.'):
  if fnmatch.fnmatch(port_fname, '*.port'):
    port_key=bucket.get_key(folder+'/'+port_fname)
    if port_key is None:
       port_key.get_contents_to_filename(port_fname)



sys.exit(exitcode)


