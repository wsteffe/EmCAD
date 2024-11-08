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

from mpmath import mp, hyp2f1, hyp3f2
from numpy import ndarray, pi, sqrt, exp
from scipy.constants import mu_0, epsilon_0

mp.dps=8

CHECK=True
GRADIENT_TO_HURAY=False

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

lossFactor=None
for i,arg in enumerate(argv):
    if arg=="-lossFactor":
       argv.pop(i)
       lossFactor=float(argv.pop(i))
       break

impedanceQ=None
for i,arg in enumerate(argv):
    if arg=="-impedanceQ":
       argv.pop(i)
       impedanceQ=float(argv.pop(i))
       break

ballRadius=None
for i,arg in enumerate(argv):
    if arg=="-ballRadius":
       argv.pop(i)
       ballRadius=float(argv.pop(i))
       break

surfRatio=None
for i,arg in enumerate(argv):
    if arg=="-surfRatio":
       argv.pop(i)
       surfRatio=float(argv.pop(i))
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
       nfreq=int(argv.pop(i))
       break

matFreq=None
for i,arg in enumerate(argv):
    if arg=="-matFreq":
       argv.pop(i)
       matFreq=float(argv.pop(i))
       break


Rq=None
for i,arg in enumerate(argv):
    if arg=="-Rq":
       argv.pop(i)
       Rq=float(argv.pop(i))
       break



def mag_field(x, f, rq, sigma0=5.8e7):
    """Calculate the magnetic field tangential to the metal surface.
    Args:
        x: position relative to surface, in units [m]
        f: frequency, in units [Hz]
        rq: rms surface roughness, in units [m]
        sigma0: dc conductivity, optional, default is 5.8e7, in units [S]

    Returns:
        magnetic field
    """
    x_is_array = isinstance(x, ndarray) and len(x) > 1
    f_is_array = isinstance(f, ndarray) and len(f) > 1
    if x_is_array and f_is_array:
        print("x and f can't both be arrays")
        raise ValueError
    # Constants
    xi = 0.5
    chi = sqrt(2) * rq
    # Angular frequency
    w = 2 * pi * f
    # Eqns 15 and 21 from Grujic 2018
    alpha = (1 + 1j) / 2 * rq * sqrt(mu_0 * w * sigma0)
    beta = 0.5 * (sqrt(1 + 4 * alpha ** 2) - 1)
    # Eqn 32 from Grujic 2018
    zeta = 1 / (1 + exp(2 * (x / chi + xi)))
    # Coefficients
    a1 = alpha + beta
    a2 = alpha - beta - 1
    b1 = 1 + 2 * alpha
    # Eqn 31 from Grujic 2018
    if isinstance(x, ndarray) and len(x) > 1:
        mag = np.empty_like(x, dtype=complex)
        for i, _z in np.ndenumerate(zeta):
            mag[i] = _z ** alpha
            mag[i] *= hyp2f1(a1, a2, b1, _z)
    elif isinstance(f, ndarray) and len(f) > 1:
        mag = np.empty_like(f, dtype=complex)
        for i in range(len(f)):
            try:
               mag[i] = zeta ** alpha[i] * hyp2f1(a1[i], a2[i], b1[i], zeta)
            except ValueError:
               mag[i]=float('inf')
    else:
        mag = zeta ** alpha * hyp2f1(a1, a2, b1, zeta)
    return mag


def surface_impedance(f, rq, x0=None, sigma0=5.8e7):
    """Calculate the surface impedance of a rough metal.
    Args:
        f: frequency, in units [Hz]
        rq: rms surface roughness, in units [m]
        x0: starting point for integral, optional, default is -5*rq,
            in units [m]
        sigma0: dc conductivity, optional, default is 5.8e7, in units [S]

    Returns:
        surface impedance
    """
    f_is_array = isinstance(f, ndarray) and len(f) > 1
    # Constants
    xi = 0.5
    chi = sqrt(2) * rq
    if x0 is None:
        x0 = -5 * rq
    # Angular frequency
    w = 2 * pi * f
    # Eqns 15 and 21
    alpha = (1 + 1j) / 2 * rq * sqrt(mu_0 * w * sigma0)
    beta = 0.5 * (sqrt(1 + 4 * alpha ** 2) - 1)
    # Eqn 32
    zeta = 1 / (1 + exp(2 * (x0 / chi + xi)))
    # Magnetic field, mag
    mag = mag_field(x0, f, rq, sigma0=sigma0)
    # Anti-derivative, bb
    # Eqn 40 and 41 in Grujic 2018
    a1 = 1 + alpha - beta
    a2 = 2 + alpha + beta
    a3 = alpha
    b1 = 1 + 2 * alpha
    b2 = 1 + alpha
    if f_is_array:
        f0 = np.empty_like(f, dtype=complex)
        f1 = np.empty_like(f, dtype=complex)
        for i in range(len(f)):
            f1[i] = hyp3f2(a1[i], a2[i], a3[i] + 1, b1[i], b2[i] + 1, zeta)
            f0[i] = hyp3f2(a1[i], a2[i], a3[i], b1[i], b2[i], zeta)
        bb = chi / 2 * (zeta ** alpha) * (zeta / (1 + alpha) * f1 - f0 / alpha)
    else:
        f1 = hyp3f2(a1, a2, a3 + 1, b1, b2 + 1, zeta)
        f0 = hyp3f2(a1, a2, a3, b1, b2, zeta)
        bb = chi / 2 * (zeta ** alpha) * (zeta / (1 + alpha) * f1 - f0 / alpha)
    return -1j * mu_0 * w * bb / mag




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

if ballRadius!=None and surfRatio!=None:
  Fi=1/(math.pi*ballRadius**2*mu0*mur*sigma)
  Ki=3/2*surfRatio
elif Rq !=None and GRADIENT_TO_HURAY:
  matFreq=(f1+f2)/2
  Zs=surface_impedance(matFreq, Rq, x0=None, sigma0=sigma)
  Zs0=cmath.sqrt(2*math.pi*1j*matFreq*mu0*mur/sigma)
  lossFactor=float(np.real(Zs)/np.real(Zs0))
  impedanceQ=float(np.imag(Zs)/np.real(Zs))


if lossFactor!=None and impedanceQ!=None:
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

if Rq !=None and not GRADIENT_TO_HURAY:
  if Rq>0:
    Zrough_f=surface_impedance(freqs, Rq, x0=None, sigma0=sigma)
  else:
    Zrough = lambda f : Zsmooth(f)
    Zrough_=np.vectorize(Zrough)
    Zrough_f=Zrough_(freqs)
else:
  Hci = lambda f : Ki/(1+cmath.sqrt(Fi/(2*f*1j)))
  Zrough = lambda f : Zsmooth(f)*(1+Hci(f))
  Zrough_=np.vectorize(Zrough)
  Zrough_f=Zrough_(freqs)

#import pdb
#pdb.set_trace()

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
  i=0
  for f in freqs:
    s=2*math.pi*1j*f
    fit = sum(c/(s - a) for c, a in zip(residues, poles))
    fit += d + h*s
    print("freq=",f,"  Zrough=",Zrough_f[i], "  Zfit=",fit, file=outf)
    i=i+1
  outf.close()


