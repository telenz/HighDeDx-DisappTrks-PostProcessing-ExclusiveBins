#!/usr/bin/env/python

from decimal import Decimal
import sys
import math
import ROOT
import os.path

#metCut    = sys.argv[1]
#ptCut     = sys.argv[2]
#ecaloCut  = sys.argv[3]
#iasCut    = float(sys.argv[4])*100
mass       = sys.argv[1]
ctau       = sys.argv[2]
printTitle = sys.argv[3]
#ptCut=20
#iasCut=15
#mass=500
#ctau=5

fileOut = open("results.txt",'a')

##############################################################################################
def isfloat(value):
  try:
    float(value)
    return True
  except ValueError:
    return False
##############################################################################################
def getOneSidedUpperLimit(mu, cl):
    muUp = 1./2.*ROOT.TMath.ChisquareQuantile(cl,2.*(mu+1));
    return muUp;
#################################### function definition ####################################
def getYield(filename, type ):
    fileIn = open(filename,'r')
    lines = fileIn.read().split("\n")
    if type == "signal": column=1
    elif type == "fake": column=2
    elif type == "pion": column=3
    elif type == "elec": column=4
    elif type == "muon": column=5
    for n in range(len(lines)):
        if lines[n].find('rate') >= 0:
            columns = lines[n].split()
            #print columns
            break
    return float(columns[column])
##############################################################################################
def getUncertainties(filename, type):
    uncertainty=0
    lines = open(filename,'r').readlines()[10:-1]
    for n in range(len(lines)):
        if lines[n].find(type) >= 0:
            #print "line = " + lines[n]
            if lines[n].find('lnN') >= 0:
                aux = lines[n].split("lnN")[1]
                columns = aux.split()
                for i in range(len(columns)):
                    if isfloat(columns[i]):
                        uncertainty = uncertainty+pow((float(columns[i])-1)*getYield(filename,type),2)
                        #print "uncertainty lnN = " + str(uncertainty)
            if lines[n].find('gmN') >= 0:
                aux = lines[n].split("gmN")[1]
                columns = aux.split()
                nEvents = float(columns[0])
                #print nEvents
                statUnc = getOneSidedUpperLimit(nEvents,0.6827)-nEvents
                #print "stat Unc = " + str(statUnc)
                columns = columns[1:]
                for i in range(len(columns)):
                    if isfloat(columns[i]):
                        #print "scaling = " + columns[i]
                        uncertainty = uncertainty+pow(float(columns[i])*statUnc,2)
                        #print "uncertainty gmN = " + str(uncertainty)

            #print columns
            #print uncertainty
    return math.sqrt(uncertainty)
##############################################################################################
def getSignalUncertainties(filename):
    uncertainty=0
    lines = open(filename,'r').readlines()[38:]
    for n in range(len(lines)):
        if lines[n].find('xsec') >= 0: continue
        aux = lines[n].split("lnN")[1]
        columns = aux.split()
        for i in range(len(columns)):
            if isfloat(columns[i]):
                uncertainty = uncertainty+pow((float(columns[i])-1)*getYield(filename,"signal"),2)
                #print "uncertainty lnN = " + str(uncertainty)
    return math.sqrt(uncertainty)
##############################################################################################

# Get xsection
lines  = open("xsectionsUpdated.txt",'r').readlines()
for n in range(len(lines)):
    if lines[n].find("Madgraph_signal_mass_" + str(mass) + "_ctau_" + str(ctau) +"cm") >= 0:
        columns = lines[n].split()
        xsec = float(columns[1])


bestValuePtCut=0
bestValueIasCut=0
bestValueXsec=10000

# Loop over pt
for l in range(1,10):
    ptCut = 20+10*l
    for i in range(1,9):
        iasCut=00+i*5
        if not os.path.exists('datacards/datacard_metCutEq%s_ptCutEq%s_ECaloCutEq%s_IasCutEq0p%02.0f_mass_' %(100,ptCut,5,iasCut) + str(mass) + 'GeV_ctau_' + str(ctau) + 'cm.txt'): continue
        file    = 'datacards/datacard_metCutEq%s_ptCutEq%s_ECaloCutEq%s_IasCutEq0p%02.0f_mass_' %(100,ptCut,5,iasCut) + str(mass) + 'GeV_ctau_' + str(ctau) + 'cm.txt'
        #print file
        nSignal = getYield(file,"signal")
        if nSignal == 0: continue
        nFake   = getYield(file,"fake")
        nPion   = getYield(file,"pion")
        nElec   = getYield(file,"elec")
        nMuon   = getYield(file,"muon")
        nBkg    = nFake + nPion + nElec + nMuon  

        muonUnc = getUncertainties(file,"muon")
        #print "muonUnc = " + str(muonUnc)
        pionUnc = getUncertainties(file,"pion")
        #print "pionUnc = " + str(pionUnc)
        elecUnc = getUncertainties(file,"elec")
        #print "elecUnc = " + str(elecUnc)
        fakeUnc = getUncertainties(file,"fake")
        #print "fakeUnc = " + str(fakeUnc)


        # Add everthing together
        bkgUnc = math.sqrt(pow(fakeUnc,2) + pow(pionUnc,2) + pow(elecUnc,2) + pow(muonUnc,2) )
        #print "bkg = " + str(round(nBkg,2)) + "+/-" +str(round(bkgUnc,2))
        signalUnc = getSignalUncertainties(file)
        #print "signal = " + str(round(nSignal,2)) + "+/-" +str(round(signalUnc,2))


        # Now loop over different xsec and find xsection with which 5sigma discovery is possible
        for k in range(0,10000):
            sOverB = k/math.sqrt(pow(bkgUnc,2) + pow(signalUnc/nSignal*k,2))
            if(sOverB>5):
                #print "Discovery possible!"
                #print "k = " + str(k)
                #print "xsec = " + str(xsec)
                minExcludedXsec = k/nSignal*xsec
                if minExcludedXsec<bestValueXsec:
                    bestValueXsec=minExcludedXsec
                    bestValuePtCut=ptCut
                    bestValueIasCut="%02.0f" %(iasCut)
                    #print "minExcludedXsec = " + str(minExcludedXsec)
                break



format_string = "{0:<30}{1:<30}{2:<30}{3:<30}{4:<30}{5:<30}"
if printTitle=="1":
    print format_string.format("mass [GeV]", "lifetime [cm]", "optimal pt Cut", "optimal Ias Cut","min. excluded xsection [pb]" ,"actual xsection [pb]")
    fileOut.write(format_string.format("mass [GeV]", "lifetime [cm]", "optimal pt Cut", "optimal Ias Cut","min. excluded xsection [pb]" ,"actual xsection [pb]"))
    fileOut.write("\n")
print format_string.format(mass, ctau, bestValuePtCut, "0." + str(bestValueIasCut),"{0:.4f}".format(bestValueXsec) ,"{0:.4f}".format(xsec))
fileOut.write(format_string.format(mass, ctau, bestValuePtCut, "0." + str(bestValueIasCut),"{0:.4f}".format(bestValueXsec) ,"{0:.4f}".format(xsec)))
fileOut.write("\n")
#print "best pt cut = " + str(bestValuePtCut)
#print "best ias cut = " + str(bestValueIasCut)     
#print "excluded xsec = " + str(bestValueXsec)
#print "xsec = " + str(xsec)
#print " "
