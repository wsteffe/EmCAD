#!/usr/bin/python3

import sys,os
from shutil import copyfile
import ezdxf
from ezdxf import disassemble
from ezdxf.addons import Importer

argv=sys.argv
argl=len(argv)

fname,fext=os.path.splitext(argv[argl-1])
if fext  != '.dxf':
    print(argv[argl-1]+' is not a dxf file')
    exit()
    

sdoc = ezdxf.readfile(fname+".dxf")
tdoc = ezdxf.new()
importer = Importer(sdoc,tdoc)
smsp= sdoc.modelspace()
msp = tdoc.modelspace()
exploded=disassemble.recursive_decompose(smsp)
importer.import_entities(exploded)
tdoc.saveas(fname+"_flat.dxf")



