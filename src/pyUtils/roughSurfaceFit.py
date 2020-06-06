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


import shutil
import sys,os
import math
import cmath

import vectfit
import numpy as np
from scipy.optimize import fsolve

CHECK=True

argv=sys.argv
argl=len(argv)

argv=sys.argv

for i,arg in enumerate(argv):
    if arg=="-sigma":
       argv.pop(i)
       sigma=float(argv.pop(i))
       break

for i,arg in enumerate(argv):
    if arg=="-mur":
       argv.pop(i)
       mur=float(argv.pop(i))
       break

for i,arg in enumerate(argv):
    if arg=="-lossFactor":
       argv.pop(i)
       lossFactor=float(argv.pop(i))
       break


for i,arg in enumerate(argv):
    if arg=="-impedanceQ":
       argv.pop(i)
       impedanceQ=float(argv.pop(i))
       break


for i,arg in enumerate(argv):
    if arg=="-npoles":
       argv.pop(i)
       npoles=int(argv.pop(i))
       break

for i,arg in enumerate(argv):
    if arg=="-freq":
       argv.pop(i)
       f1=float(argv.pop(i))
       f2=float(argv.pop(i))
       nfreq=float(argv.pop(i))
       break


for i,arg in enumerate(argv):
    if arg=="-matFreq":
       argv.pop(i)
       matFreq=float(argv.pop(i))
       break


#npoles=3
#sigma=4.561e7
#mur=1
#nfreq=50
#f1=27.7e9
#f2=29.7e9
#matFreq=27.71e9
#lossFactor=0.0736585/0.0489696
#impedanceQ=0.33736218/0.0736585


mu0= 4*math.pi*1e-7

#Ni=11
#a=Rz/(4*math.sqrt(2/3)+2)
#Ahex=6*a**2*math.sqrt(3)*(1/math.sqrt(3)+1)**2
#Ki=6*math.pi*a**2*Ni/Ahex
#Fi=1/(math.pi*a**2*mu0*mur*sigma)


def ferror(variables) :
      [Ki,Fi] = variables
      ferror_cfac=1+ Ki/(1+cmath.sqrt(Fi/(2*matFreq*1j)))
      eq1=np.real(ferror_cfac)-np.imag(ferror_cfac)-lossFactor
      eq2=45*math.pi/180+cmath.phase(ferror_cfac)-math.atan(impedanceQ)
      return [eq1,eq2]

[Ki,Fi]=fsolve(ferror,[lossFactor, 10*f2])


f1=max(f1,f2/(2*nfreq))
freqs = np.linspace(f1, f2, nfreq)
s_f=np.array([2*math.pi*1j*f for f in freqs]) 


Zsmooth = lambda f : cmath.sqrt(2*math.pi*1j*f*mu0*mur/sigma)
Zsmooth_=np.vectorize(Zsmooth)

deltaFun = lambda f : 1/math.sqrt(math.pi*f*mu0*mur*sigma)
deltaFun_=np.vectorize(deltaFun)


Hci = lambda f : Ki/(1+cmath.sqrt(Fi/(2*f*1j)))

Zrough = lambda f : Zsmooth(f)*(1+Hci(f))
Zrough_=np.vectorize(Zrough)

Zrough_f=Zrough_(freqs)

niter=10

poles, residues, d, h = vectfit.vectfit_auto_rescale(Zrough_f, s_f, n_poles=npoles,n_iter=niter,show=False,inc_real=False)
nrealpoles=0
for pole in poles:
  if np.imag(pole)==0:
     nrealpoles+=1

npoles=len(poles)
outf=open("roughSurfaceFit.txt","w")
print(" %20.15g  %20.15g" % (d,h) , file=outf)
print("    %d  %d" % (npoles,nrealpoles), file=outf)
for pole in poles:
  print(" %20.15g  %20.15g" % (np.real(pole), np.imag(pole)), file=outf)

for res in residues:
  print(" %20.15g  %20.15g" % (np.real(res), np.imag(res)), file=outf)

outf.close()


if CHECK:
  outf=open("roughSurfaceFitCheck.txt","w")
  for f in freqs:
    s=2*math.pi*1j*f
    fit = sum(c/(s - a) for c, a in zip(residues, poles))
    fit += d + h*s
    print("freq=",f,"  Zrough=",Zrough(f), "  Zfit=",fit, file=outf)
  outf.close()


