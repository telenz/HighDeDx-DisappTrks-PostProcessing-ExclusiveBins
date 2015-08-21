#!/usr/bin/env/python

from decimal import Decimal
import sys

metCut    = sys.argv[1]
ptCutMin  = sys.argv[2]
ptCutMax  = sys.argv[3]
ecaloCut  = sys.argv[4]
iasCutMin = float(sys.argv[5])*100
iasCutMax = float(sys.argv[6])*100

fIn = open('../leptonicBkg/datacards/datacard_metCutEq%s_ptGt%s_Le%s_ECaloCutEq%s_IasGt0p%02.0f_Le0p%02.0f.txt' %(metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin,iasCutMax), 'r')
linesOriginal = fIn.read().split("\n")


fIn = open('signal.log','r')
lines = fIn.read().split("\n")


for n in range(len(lines)):


    if lines[n].find('Yield Madgraph_signal_mass_') >= 0:
        mass = lines[n].split('_')[3]
        print 'mass = ' + str(mass)
        ctau = lines[n].split('_')[5]
        ctau = ctau.split('cm')[0]
        print 'ctau = ' + str(ctau)
        pred = lines[n].split()[3]
        print "pred = " + str(pred)
        n2 = n+2
        nEvents = lines[n2].split()[2]
        print "nEvents = " + str(nEvents)
        print ''
        
        # Write two datacard
        fout = open('datacards/datacard_metCutEq%s_ptGt%s_Le%s_ECaloCutEq%s_IasGt0p%02.0f_Le0p%02.0f_mass_' %(metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin,iasCutMax) + str(mass) + 'GeV_ctau_' + str(ctau) + 'cm.txt', 'w')
        for line in linesOriginal:

            if line.find('CharginoPair_mass_100GeV_ctau_5cm') >= 0:
                line = line.replace('CharginoPair_mass_100GeV_ctau_5cm', str('CharginoPair_mass_%.0fGeV_ctau_%.0fcm') %(float(mass),float(ctau)) )

            if line.find('rate') >= 0:
                line = line.replace('x.xxxx', str("%.4f" % float(pred)));

            if line.find('signal_stat') >= 0:
                print nEvents
                if float(nEvents) == 0:
                    scaling = 0.000
                else:
                    scaling = float(pred)/float(nEvents)
                print scaling
                line = line.replace('x.xxxx', str("%.4f" % scaling));
                line = line.replace('x', str(nEvents));


            fout.write(line)
            fout.write('\n')
            



