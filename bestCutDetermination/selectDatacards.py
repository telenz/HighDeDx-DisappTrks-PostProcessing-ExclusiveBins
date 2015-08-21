#!/usr/bin/env/python

from decimal import Decimal
import sys
import math
import ROOT
import os.path
import shutil

input       = sys.argv[1]
output      = sys.argv[2]

print "input folder = " + input
print "output folder = " + output 
filename = output

if not os.path.exists(filename):
    os.makedirs(filename)
else:
    shutil.rmtree(filename)
    os.makedirs(filename)




lines = open("resultsOutOf4Sets.txt",'r').readlines()[1:]

for n in range(len(lines)):
    columns = lines[n].split()
    mass = columns[0]
    #print "mass = " + str(mass)
    ctau = columns[1]
    #print "ctau = " + str(ctau)
    ptCut = columns[2]
    if float(ptCut)==0:
        ptCut='30'
    iasCut = columns[3]
    if float(iasCut)==0:
        iasCut='0.05'
    iasCut = iasCut[2:]
    
    
    for fn in os.listdir('./' + input + '/.'):
        #print fn
        if fn.find("mass_" + mass) >= 0:
            if fn.find(ctau + "cm") >= 0:
                if fn.find("ptCutEq" + ptCut) >=0:
                    if fn.find("IasCutEq0p" + iasCut) >=0:
                        shutil.copy2(input +"/" + fn, filename + "/" + fn)
                        print fn
