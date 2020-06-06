#! /usr/bin/env python3

import shutil
import sys,os

argv=sys.argv
argl=len(argv)
fname=argv[argl-1]

if(fname.endswith('.ts')):
   froot=fname[:-3]
else:
   froot=fname
   fname = froot+'.ts'

f=open(fname)

for line in f:
   if   line.startswith('#'):
          param=line.split()[2]
   elif line.find('[Number of Ports]') >=0:
          ll=line[17:]
          nports=str(int(ll))
f.close()

ofname=froot
if   param=='Y':
        t1ext='.y'
elif param=='Z':
        t1ext='.z'
elif param=='S':
        t1ext='.s'
elif param=='W':
        t1ext='.w'


t1ext+=nports
t1ext+='p'
ofname+=t1ext

f=open(froot+'.ext','w')
f.write(t1ext)
f.close()


f = open(fname)
of = open(ofname,'w')


for line in f:
      if   line.find('[Version] 2.0') >=0:
          continue
      elif line.find('[Number of Ports]') >=0:
          continue
      elif line.find('[Number of Frequencies]') >=0:
          continue
      elif line.find('![Port Names]') >=0:
          continue
      elif line.find('[Two-Port Data Order]') >=0:
          continue
      elif line.find('[Network Data]') >=0:
          continue
      else:
          of.write(line) 

f.close()
of.close()

