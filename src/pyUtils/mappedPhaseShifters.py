#! /usr/bin/env python3

# This file is part of the EmCAD program which constitutes the client
# side of an electromagnetic modeler delivered as a cloud based service.
# 
# Copyright (C) 2015-2024  Walter Steffe
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


import sys,os
import skrf as rf
from scipy.optimize import minimize
import numpy as np
import cmath

argv=sys.argv

freqResponsePath=None
for i,arg in enumerate(argv):
    if arg=="-freqResponsePath":
       argv.pop(i)
       freqResponsePath=argv.pop(i)
       break

mappedResponsePath=None
for i,arg in enumerate(argv):
    if arg=="-mappedResponsePath":
       argv.pop(i)
       mappedResponsePath=argv.pop(i)
       break


t =rf.Touchstone(freqResponsePath)
f,S =t.get_sparameter_arrays()
t =rf.Touchstone(mappedResponsePath)
fm,Sm =t.get_sparameter_arrays()

if len(fm) != len(f):
    print('Different frequencies in full wave and mapped responses')
    sys.exit(1)
if fm[0] != f[0]:
    print('Different frequencies in full wave and mapped responses')
    sys.exit(1)
if fm[-1] != f[-1]:
    print('Different frequencies in full wave and mapped responses')
    sys.exit(1)


def cost(phi):
   S_f=np.copy(S[ifreq])
   Sm_f=np.copy(Sm[ifreq])
   for l in range(2):
      for m in range(2):
         Sm_f[l,m]=Sm_f[l,m]*cmath.exp(complex(0.0,phi[l]+phi[m]))
   cost=0
   for l in range(2):
      for m in range(2):
         cost=cost+(abs(Sm_f[l,m]-S_f[l,m]))**2
   return cost


def jaco(phi):
   S_f=np.copy(S[ifreq])
   Sm_f=np.copy(Sm[ifreq])
   for l in range(2):
      for m in range(2):
         Sm_f[l,m]=Sm_f[l,m]*cmath.exp(complex(0.0,phi[l]+phi[m]))
   Jac=np.array([0.0,0.0])
   for l in range(2):
      for m in range(2):
         for i in range(2):
            Jac[i]=Jac[i]-2*(Sm_f[l,m]*np.conjugate(Sm_f[l,m]-S_f[l,m])).imag
   return Jac


def phaseMin(phi):
    if phi>cmath.pi:
         return phi-2*cmath.pi
    elif phi<-cmath.pi:
         return phi+2*cmath.pi
    else:
        return phi


def estimatePhi(phi0,phi):
   if ifreq==0 or abs(S[ifreq][0,0]) >0.01 and abs(Sm[ifreq][0,0]) >0.01:
      phisum=cmath.phase(S[ifreq][0,0]/Sm[ifreq][0,0])/2 \
            +cmath.phase(S[ifreq][1,1]/Sm[ifreq][1,1])/2
      phidif=cmath.phase(S[ifreq][0,0]/Sm[ifreq][0,0])/2 \
            -cmath.phase(S[ifreq][1,1]/Sm[ifreq][1,1])/2
   else:
      phisum=phi0[0]+phi0[1]  
      phidif=phi0[0]-phi0[1]  
   if abs(S[ifreq][0,1]) >0.01 and abs(Sm[ifreq][0,1])>0.01:
      phisum2=cmath.phase(S[ifreq][0,1]/Sm[ifreq][0,1])
      if abs(phaseMin(phisum-phisum2))>cmath.pi/2:
        phidif+=np.sign(phisum2-phisum)*cmath.pi
        phisum=phisum2
      else:
        phisum=phisum2      
   phi[0]=phaseMin((phisum+phidif)/2)
   phi[1]=phaseMin((phisum-phidif)/2)
   if ifreq>0 and \
      abs(phaseMin(phi[0]-phi0[0]))>cmath.pi/2 and \
      abs(phaseMin(phi[1]-phi0[1]))>cmath.pi/2:
        phi[0]=phaseMin(phi[0]+cmath.pi)
        phi[1]=phaseMin(phi[1]+cmath.pi)



phi0=np.array([0.0,0.0])
shift1s=[]
shift2s=[]
for ifreq in range(len(f)):
   phi=np.array([0.0,0.0])
   estimatePhi(phi0,phi)
#   if ifreq==0:
#     res=minimize(cost, phi, method="Nelder-Mead")
#   else:
#     res=minimize(cost, phi, method="Newton-CG", jac=jaco)
#   for l in range(2):
#     phi[l]=res.x[l]
#   delta0=phaseMin(phi[0]-phi0[0])
#   delta1=phaseMin(phi[1]-phi0[1])
#   if abs(S[ifreq][0,1]) <=0.01 or abs(Sm[ifreq][0,1]) <=0.01:
#       if abs(delta0)>cmath.pi/2:
#          phi[0]+=cmath.pi
#       if abs(delta1)>cmath.pi/2:
#          phi[1]+=cmath.pi
#   elif abs(delta0)>cmath.pi/2 and abs(delta1)>cmath.pi/2:
#        phi[0]+=cmath.pi
#        phi[1]+=cmath.pi
   residualCost=cost(phi)
   if residualCost>0.2:
#     import pdb
#     pdb.set_trace()
#     residualCost=cost(phi)
     print("failed evaluation of mapped phase shifter. Residual cost=", residualCost)
     sys.exit(1)
   s11=0
   s22=0
   s12=cmath.exp(complex(0,phi[0]))
   s21=s12
   shift1s.append(np.array(([s11,s12],[s21,s22])))
   s12=cmath.exp(complex(0,phi[1]))
   s21=s12
   shift2s.append(np.array(([s11,s12],[s21,s22])))
   for l in range(2):
     phi0[l]=phi[l]


mappedResponsePath,ext=os.path.splitext(mappedResponsePath)
shifter1name=mappedResponsePath+'_shifter1'
shifter2name=mappedResponsePath+'_shifter2'

shifter1=rf.Network(name=shifter1name,s=shift1s,frequency=f*1e-9, z0=50)
shifter2=rf.Network(name=shifter2name,s=shift2s,frequency=f*1e-9, z0=50)
shifter1.write_touchstone(form='ma')
shifter2.write_touchstone(form='ma')
