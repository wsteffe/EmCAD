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

import sys
import boto
from boto.exception import SQSError, S3ResponseError
#import uuid

argv=sys.argv

for i,arg in enumerate(argv):
    if arg=="-bucket":
       argv.pop(i)
       bucketname=argv.pop(i)
       break

for i,arg in enumerate(argv):
    if arg=="-account_filename":
       argv.pop(i)
       local_account_fname=argv.pop(i)
       break

s3_conn = boto.connect_s3()
bucket = s3_conn.get_bucket(bucketname)


accountkey=bucket.get_key('emcad_account')
if accountkey is None:
  exitcode=1
else:
  accountkey.get_contents_to_filename(local_account_fname)
  exitcode=0

sys.exit(exitcode)


