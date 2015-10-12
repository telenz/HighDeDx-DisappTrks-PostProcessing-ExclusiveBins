#!/usr/bin/env/python

from decimal import Decimal
import sys

metCut    = sys.argv[1]
ptCutMin  = sys.argv[2]
ptCutMax  = sys.argv[3]
ecaloCut  = sys.argv[4]
iasCutMin = float(sys.argv[5])*100
iasCutMax = float(sys.argv[6])*100
mass      = sys.argv[7]
ctau      = sys.argv[8]

format_string = "{0:<40}{1:<5}{2:<10}{3:<40}{4:<20}{5:<20}{6:<20}{7:<20}"

fIn = open('../leptonicBkg/datacards/datacard_metCutEq%s_ptGt%s_Le%s_ECaloCutEq%s_IasGt0p%02.0f_Le0p%02.0f.txt' %(metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin,iasCutMax), 'r')
linesOriginal = fIn.read().split("\n")


fIn = open('logFiles/signal_mass_%sGeV_ctau_%scm.log' %(mass,ctau),'r')
lines = fIn.read().split("\n")

fout = open('datacards/datacard_metCutEq%s_ptGt%s_Le%s_ECaloCutEq%s_IasGt0p%02.0f_Le0p%02.0f_mass_' %(metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin,iasCutMax) + str(mass) + 'GeV_ctau_' + str(ctau) + 'cm.txt', 'w')

#Read log files
nEvents=0
pred=0
scaling=0
for n in range(len(lines)):
    if lines[n].find('Yield Madgraph_signal_mass_') >= 0:
        pred = lines[n].split()[3]
        n2 = n+2
        nEvents = lines[n2].split()[2]

        if float(nEvents) == 0:
            scaling = 0.0001
            nEvents = 1
        else:
            scaling = float(pred)/float(nEvents)
        
# Write two datacard
for line in linesOriginal:
    if line.find('CharginoPair_mass_100GeV_ctau_5cm') >= 0:
        line = line.replace('CharginoPair_mass_100GeV_ctau_5cm', str('CharginoPair_mass_%.0fGeV_ctau_%.0fcm') %(float(mass),float(ctau)) )
    if line.find('rate') >= 0:
        line = line.replace('x.xxxx', str("%.3f" % float(pred)));
    fout.write(line)
    fout.write('\n')
    if line.find('muon_stat') >= 0:
        break
#print "mass:" + str(mass)+ ", ctau: " + str(ctau) + "cm  = " + str(pred)
fout.write(format_string.format('signal_stat_' + str(ptCutMin) + str('_0p%02.0f' %(iasCutMin)), 'gmN', str(nEvents), str("%.5f" % scaling) , '-', '-', '-', '-'))
fout.write('\n')
            



