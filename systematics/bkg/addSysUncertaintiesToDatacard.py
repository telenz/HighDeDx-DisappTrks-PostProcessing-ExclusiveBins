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


fIn = open('../../signal/datacards/datacard_metCutEq%s_ptGt%s_Le%s_ECaloCutEq%s_IasGt0p%02.0f_Le0p%02.0f_mass_%sGeV_ctau_%scm.txt' %(metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin,iasCutMax,mass,ctau), 'r')
linesOriginal = fIn.read().split("\n")

# Write old stuff to datacard
fout = open('datacards/datacard_metCutEq%s_ptGt%s_Le%s_ECaloCutEq%s_IasGt0p%02.0f_Le0p%02.0f_mass_' %(metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin,iasCutMax) + str(mass) + 'GeV_ctau_' + str(ctau) + 'cm.txt', 'w')
for line in linesOriginal:
    fout.write(line)
    fout.write('\n')


# Add new stuff - pions
fIn = open('1_LeptonScaleFactors/logFiles/pion.log','r')
lines = fIn.read().split("\n")

for n in range(len(lines)):

    if lines[n].find('Final Uncertainty ') >= 0:
        uncertainty = float(lines[n].split(' = ')[1]) + 1
        #print 'uncertainty = ' + str(uncertainty)
        
fout.write("\n# SYSTEMATIC UNCERTAINTIES \n\n")

format_string = "{0:<40}{1:<5}{2:<10}{3:<40}{4:<30}{5:<30}{6:<30}{7:<30}"

fout.write(format_string.format("pion_syst_rate", "lnN", "", "-", "-", uncertainty,"-", "-"))
fout.write('\n')

# Add new stuff - electrons
fIn = open('1_LeptonScaleFactors/logFiles/electron_ptGt%s_Le%s_ecaloCutEq%s.log' %(ptCutMin,ptCutMax,ecaloCut),'r')
lines = fIn.read().split("\n")

for n in range(len(lines)):

    if lines[n].find('Final Uncertainty ') >= 0:
        uncertainty = float(lines[n].split(' = ')[1]) + 1
        #print 'uncertainty = ' + str(uncertainty)
        
format_string = "{0:<40}{1:<5}{2:<10}{3:<40}{4:<30}{5:<30}{6:<30}{7:<30}"
fout.write(format_string.format("elec_syst_rate_" + str(ptCutMin) , "lnN", "", "-", "-", "-",uncertainty, "-"))
fout.write('\n')

# Add new stuff - muons
fIn = open('1_LeptonScaleFactors/logFiles/muon_ecaloCutEq%s.log' %(ecaloCut) ,'r')
lines = fIn.read().split("\n")

for n in range(len(lines)):

    if lines[n].find('Final Uncertainty ') >= 0:
        uncertainty = float(lines[n].split(' = ')[1]) + 1.
        #print 'uncertainty = ' + str(uncertainty)
        
format_string = "{0:<40}{1:<5}{2:<10}{3:<40}{4:<30}{5:<30}{6:<30}{7:<30}"
fout.write(format_string.format("muon_syst_rate", "lnN", "", "-","-", "-", "-", uncertainty))
fout.write('\n')

####################################################################################################
# Add new stuff - pions
fIn = open('2_LeptonIas/logFiles/pion_iasGt0p%02.0f_Le0p%02.0f.log' %(iasCutMin,iasCutMax),'r')
lines = fIn.read().split("\n")

for n in range(len(lines)):

    if lines[n].find('Final Uncertainty ') >= 0:
        uncertainty = float(lines[n].split(' = ')[1]) + 1
        #print 'uncertainty = ' + str(uncertainty)
        

format_string = "{0:<40}{1:<5}{2:<10}{3:<40}{4:<30}{5:<30}{6:<30}{7:<30}"
fout.write(format_string.format("pion_syst_ias" + str('_0p%02.0f' %(iasCutMin)), "lnN", "", "-", "-", uncertainty,"-", "-"))
fout.write('\n')

# Add new stuff - electrons
fIn = open('2_LeptonIas/logFiles/electron_iasGt0p%02.0f_Le0p%02.0f.log' %(iasCutMin,iasCutMax),'r')
lines = fIn.read().split("\n")

for n in range(len(lines)):

    if lines[n].find('Final Uncertainty ') >= 0:
        uncertainty = float(lines[n].split(' = ')[1]) + 1
        #print 'uncertainty = ' + str(uncertainty)
        
format_string = "{0:<40}{1:<5}{2:<10}{3:<40}{4:<30}{5:<30}{6:<30}{7:<30}"
fout.write(format_string.format("elec_syst_ias" + str('_0p%02.0f' %(iasCutMin)), "lnN", "", "-", "-", "-",uncertainty, "-"))
fout.write('\n')

# Add new stuff - muons
fIn = open('2_LeptonIas/logFiles/muon_iasGt0p%02.0f_Le0p%02.0f.log' %(iasCutMin,iasCutMax),'r')
lines = fIn.read().split("\n")

for n in range(len(lines)):

    if lines[n].find('Final Uncertainty ') >= 0:
        uncertainty = float(lines[n].split(' = ')[1]) + 1
        #print 'uncertainty = ' + str(uncertainty)
        
format_string = "{0:<40}{1:<5}{2:<10}{3:<40}{4:<30}{5:<30}{6:<30}{7:<30}"
fout.write(format_string.format("muon_syst_ias" + str('_0p%02.0f' %(iasCutMin)), "lnN", "", "-","-", "-", "-", uncertainty))
fout.write('\n')

####################################################################################################
# Add new stuff - fake rate
fIn = open('3_FakeRate/logFiles/fake_ptGt%s_Le%s_ecaloCutEq%s.log' %(ptCutMin,ptCutMax,ecaloCut) ,'r')
lines = fIn.read().split("\n")

for n in range(len(lines)):

    if lines[n].find('Final Uncertainty ') >= 0:
        uncertainty = float(lines[n].split(' = ')[1]) + 1
        #print 'uncertainty = ' + str(uncertainty)
        
format_string = "{0:<40}{1:<5}{2:<10}{3:<40}{4:<30}{5:<30}{6:<30}{7:<30}"
fout.write(format_string.format("fake_syst_rate_" + str(ptCutMin), "lnN", "", "-", uncertainty,"-", "-", "-"))
fout.write('\n')
####################################################################################################
# Add new stuff - fake ias
fIn = open('4_FakeIas/logFiles/fake_ptGt%s_Le%s_ecaloCutEq%s_iasGt0p%02.0f_Le0p%02.0f.log' %(ptCutMin,ptCutMax,ecaloCut,iasCutMin,iasCutMax) ,'r')
lines = fIn.read().split("\n")

for n in range(len(lines)):

    if lines[n].find('Final Uncertainty ') >= 0:
        uncertainty = float(lines[n].split(' = ')[1]) + 1
        #print 'uncertainty = ' + str(uncertainty)
        
format_string = "{0:<40}{1:<5}{2:<10}{3:<40}{4:<30}{5:<30}{6:<30}{7:<30}"
fout.write(format_string.format("fake_syst_ias_" + str(ptCutMin) + str('_0p%02.0f' %(iasCutMin)), "lnN", "", "-", uncertainty,"-", "-", "-"))
fout.write('\n')
####################################################################################################
