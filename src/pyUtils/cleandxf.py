#!/usr/bin/python3

import sys,os
import ezdxf
import dxfdedup


argv=sys.argv
argl=len(argv)

fname=argv[argl-1]

# define the entity types that will be processed (optional)

entities = ['line', 'circle', 'lwpolyline', 'arc']

# create a Deduplicator object and optionally pass the entities list

d = dxfdedup.Deduplicator(entities)

# remove dupicates from a single file

d.dedup(fname)


