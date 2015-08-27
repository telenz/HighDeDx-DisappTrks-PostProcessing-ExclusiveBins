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
fIn = open('../bkg/datacards/datacard_metCutEq%s_ptGt%s_Le%s_ECaloCutEq%s_IasGt0p%02.0f_Le0p%02.0f_mass_%sGeV_ctau_%scm.txt' %(metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin,iasCutMax,mass,ctau), 'r')
linesOriginal = fIn.read().split("\n")

# Write old stuff to datacard
fout = open('datacards/datacard_metCutEq%s_ptGt%s_Le%s_ECaloCutEq%s_IasGt0p%02.0f_Le0p%02.0f_mass_' %(metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin,iasCutMax) + str(mass) + 'GeV_ctau_' + str(ctau) + 'cm.txt', 'w')
for line in linesOriginal:
    fout.write(line)
    fout.write('\n')

#################################### function definition ####################################
def getUncertainty(filename, stringname ):
    uncertainty=0.0
    fIn = open(filename,'r')
    lines = fIn.read().split("\n")
    for n in range(len(lines)):
        if lines[n].find('Final Uncertainty ') >= 0:
            uncertainty = float(lines[n].split(' = ')[1]) + 1
            #print 'uncertainty = ' + str(uncertainty)
    fout.write(format_string.format(stringname, "lnN", "", "{0:.4f}".format(uncertainty),"-", "-","-", "-"))
    fout.write('\n')
#################################### function definition ####################################




####################################################################################################
getUncertainty('1_Luminosity/logFiles/uncertainty.log','lumi')
getUncertainty('2_ISR/logFiles/Madgraph_signal_mass_' + str(mass) + '_ctau_' + str(ctau) + 'cm.log','ISR')
getUncertainty('3_TriggerEfficiency/logFiles/Madgraph_signal_mass_' + str(mass) + '_ctau_' + str(ctau) + 'cm.log','trigger')
getUncertainty('4_JetEnergyScale/logFiles/Madgraph_signal_mass_' + str(mass) + '_ctau_' + str(ctau) + 'cm.log','JES')
getUncertainty('5_JetEnergyResolution/logFiles/Madgraph_signal_mass_' + str(mass) + '_ctau_' + str(ctau) + 'cm.log','JER')
getUncertainty('6_PDF/logFiles/Madgraph_signal_mass_' + str(mass) + '_ctau_' + str(ctau) + 'cm.log','PDF')
getUncertainty('7_Ecalo/logFiles/uncertainty_ptGt' +str(ptCutMin) + '_Le' + str(ptCutMax) + '_ecaloCutEq' + str(ecaloCut) + '.log','ECalo')
getUncertainty('8_NInnerMiddleLost/logFiles/uncertainty_Middle_ptGt' +str(ptCutMin) + '_Le' + str(ptCutMax)+ '.log','HitsLostMiddle')
getUncertainty('8_NInnerMiddleLost/logFiles/uncertainty_Inner_ptGt' +str(ptCutMin) + '_Le' + str(ptCutMax)+ '.log','HitsLostInner')
getUncertainty('9_Pileup/logFiles/Madgraph_signal_mass_' + str(mass) + '_ctau_' + str(ctau) + 'cm.log','PU')
getUncertainty('10_TrkRecoEff/logFiles/uncertainty_ptGt' +str(ptCutMin) + '_Le' + str(ptCutMax) + '.log','TrkRecoEff')
getUncertainty('11_Ias/logFiles/uncertainty.log','Ias')
#getUncertainty('12_xsec/logFiles/Madgraph_signal_mass_' + str(mass) + '.log','xsec')
####################################################################################################
