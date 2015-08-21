#!/usr/bin/env/python

from decimal import Decimal
import sys
import math


# Calculate Final Uncertainty
fIn = open('result_data.log','r')
lines = fIn.read().split("\n")

for n in range(len(lines)):
    if lines[n].find('Track reconstruction efficiency ') >= 0:
        aux = lines[n].split(' = ')[1]
        uncertaintyData = float(aux.split(' +/- ')[0]) 
        uncertaintyDataError = float(aux.split(' +/- ')[1])
        print "data  = " + str(uncertaintyData) + " +/- " + str(uncertaintyDataError)
        break


fIn = open('result_mc.log','r')
lines = fIn.read().split("\n")

for n in range(len(lines)):
    if lines[n].find('Track reconstruction efficiency ') >= 0:
        aux = lines[n].split(' = ')[1]
        uncertaintyMC = float(aux.split(' +/- ')[0]) 
        uncertaintyMCError = float(aux.split(' +/- ')[1])
        print "mc    = " + str(uncertaintyMC) + " +/- " + str(uncertaintyMCError)
        break


# Calculate Final Uncertainty

ratio=uncertaintyMC/uncertaintyData
ratioError=ratio*math.sqrt(math.pow(uncertaintyMCError/uncertaintyMC,2) + math.pow(uncertaintyDataError/uncertaintyData,2))

finalUncertainty = max(abs(ratio-ratioError-1),abs(ratio+ratioError-1))

print "ratio = " + '%.3f'%ratio + " +/- " + '%.3f'%ratioError
print "Final Uncertainty = " + '%.3f'%finalUncertainty

#(max(abs(ratio-ratioError-1),abs(ratio+ratioError-1)))
