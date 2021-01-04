#!/usr/bin/python3

import sys,os
from shutil import copyfile
import ezdxf


argv=sys.argv
argl=len(argv)

f0name,fext=os.path.splitext(argv[argl-1])
if fext  != '.dxf':
    print(argv[argl-1]+' is not a dxf file')
    exit()
    

doc0 = ezdxf.readfile(f0name+".dxf")

layer_names = []
for layer in doc0.layers:
    if layer.dxf.name != '0':
        layer_names.append(layer.dxf.name)

#layer_names_lower = {lname.lower() for lname in layer_names}

for lname in layer_names:
   fname = f0name+'_'+lname
   copyfile(f0name+".dxf", fname+".dxf")
   doc = ezdxf.readfile(fname+".dxf")
   msp = doc.modelspace()
   delete_entities = [entity for entity in msp if entity.dxf.layer != lname]
   for entity in delete_entities:
      msp.unlink_entity(entity)
   for dlname in layer_names:
      if dlname != lname:
         doc.layers.remove(dlname)
   doc.save()


