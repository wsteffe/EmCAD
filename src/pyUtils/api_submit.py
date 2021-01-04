#! /usr/bin/env python3

# This file is part of the EmCAD program which constitutes the client
# side of an electromagnetic modeler delivered as a cloud based service.
# 
# Copyright (C) 2017  Walter Steffe
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



import os,sys,time,fnmatch
import json
import api_utils

argv=sys.argv

for i,arg in enumerate(argv):
    if arg=="-job":
       argv.pop(i)
       job=argv.pop(i)
       break

username=None
for i,arg in enumerate(argv):
    if arg=="-user":
       argv.pop(i)
       username=argv.pop(i)
       break

password=None
for i,arg in enumerate(argv):
    if arg=="-password":
       argv.pop(i)
       password=argv.pop(i)
       break

project=''
for i,arg in enumerate(argv):
    if arg=="-project":
       argv.pop(i)
       project=argv.pop(i)
       break

meshing=0
for i,arg in enumerate(argv):
    if arg=="-meshsize":
       meshing=1
       break

target_cktname=''
for i,arg in enumerate(argv):
    if arg=="-target_cktname":
       target_cktname=argv[i+1]
       break



folder=project


argv[0]=job

argl=len(argv)
fname=argv[argl-1]


res_url_fname=fname+'.res.url'
if(os.path.exists(res_url_fname)):
      os.remove(res_url_fname)

if job=="ideal_filter_tune":
  log_fname=fname+'_tuning.log'
else:
  log_fname=fname+'.log'

if(os.path.exists(log_fname)):
      os.remove(log_fname)

argvdata={'argv': argv }
with open('%s' % fname+'.json','w') as outfile:
    json.dump(argvdata,outfile)

res_fname=fname+'.res'
with open(res_fname, 'w') as f:
    f.write("-1\n")
api_utils.upload_project_file(username,password,res_fname,folder,False)


class JobExecutionException(Exception):
    def __init__(self, value):
         self.value = value
    def __str__(self):
         return repr(self.value)


def project_file_get(fname):
    return api_utils.download_project_file(username,password,folder,fname)


def project_file_put(fname,only_if_newer=True):
    if(not os.path.exists(fname)):
         return 1
    api_utils.upload_project_file(username,password,fname,folder,only_if_newer)
    return 0


def project_file_putall(job):
    project_file_put(fname+'.json')
    project_file_put(fname+'.res')
    if job=="modelize1" or job=="modelizep":
        if meshing==1:
            project_file_put(fname+'.tgz')
        else:
            project_file_put(fname+'.mwm')
    elif job=="reduce":
       for f in os.listdir("./"):
            if fnmatch.fnmatch(f, '*.JC'):
                   project_file_put(f)
       for f in os.listdir("./"):
            if fnmatch.fnmatch(f, '*_RM.JC'):
                   project_file_put(f)
    elif job=="analyse" or job=="analyseszp":
       project_file_put(fname+'.JC')
       project_file_put('extern_port_loads.JC')
    elif job=="ideal_filter_tune":
       project_file_put(fname+'.dat')
       project_file_put(fname+'_xpar.txt')
       project_file_put(target_cktname+'_xpar.txt')
       project_file_put('extern_port_loads.JC')
    elif job=="filtermap":
       pippo=1
    elif job=="symmfilter":
       project_file_put(fname+'.inp')
    else:
       raise JobExecutionException("not valid job: " +job)


def docker_run():
    cmd= "docker run --rm"\
         +" -v /home/walter/.aws:/home/mwserver/.aws"\
         +" --user mwserver"\
         +" mwm  job-processor -aws"\
         +" -user " +username\
         +" -folder " +folder\
         +" -filename "+fname
    exitcode=(os.system(cmd))>>8
    return exitcode


def wait_for_job_completed():
    signedUrl=None
    completed=False
    exitcode=-1
    while not completed:
       signedUrls=api_utils.download_project_file(username,password,folder,fname+'.res',signedUrl=signedUrl)
       if len(signedUrls)==1:
          signedUrl=signedUrls[0]
       with open(fname+'.res', 'r') as f:
          exitcode=int(f.readline())
          completed=exitcode != -1
    return exitcode


def project_file_getall(job):
    if(len(project_file_get(log_fname))!=1):
             sys.exit(255)  # log file is mandatory for all jobs
    if job=="modelize1":
             if meshing==1:
                project_file_get(fname+'.msh')
                project_file_get(fname+'_WG.mwm')
             project_file_get(fname+'_RM.JC')
             project_file_get(fname+'_RM.sp')
             project_file_get(fname+"*.port")
    elif job=="modelizep":
             project_file_get(fname+"*.port")
    elif job=="reduce":
             project_file_get(fname+'_RM.JC')
             project_file_get(fname+'_RM.sp')
    elif job=="analyse":
             project_file_get(fname+'.ts')
    elif job=="analyseszp":
             project_file_get(fname+'.SZP')
             project_file_get(fname+'_mapped_canonical.inp')
    elif job=="filtermap":
             pippo=1
    elif job=="ideal_filter_tune":
             project_file_get(fname+'.dat')
             project_file_get(fname+'_xpar.txt')
             project_file_get(fname+'.JC')
             project_file_get(fname+'.SZP')
    elif job=="symmfilter":
             project_file_get(fname+'.JC')
             project_file_get(fname+'.sp')
             project_file_get(fname+'_xpar.txt')
    else:
             raise JobExecutionException("not valid job: " +job)


exitcode=0
project_file_putall(job)

response=api_utils.submit_job(username,password,folder,fname,job)
if job!="modelize1" and job!="modelizep":
  exitcode=wait_for_job_completed()
  project_file_getall(job)

sys.exit(exitcode)


