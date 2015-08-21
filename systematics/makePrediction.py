#!/usr/bin/env/python

from decimal import Decimal
import sys
import math
import ROOT
import os.path

mass=200
ctau=10

ptCutMin    = sys.argv[1]
ptCutMax    = sys.argv[2]
iasCutMin   = float(sys.argv[3])*100
iasCutMax   = float(sys.argv[4])*100


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
def getOneSidedLowerLimit(mu, cl):
    muLow = 1./2.*ROOT.TMath.ChisquareQuantile(1-cl,2.*mu);
    return muLow;
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
def getStatUncertainties(filename):
  uncertaintyUp=0
  uncertaintyDown=0
  lines = open(filename,'r').readlines()[10:-1]
  for type in ['fake','pion','elec','muon']:
    for n in range(len(lines)):
      if lines[n].find(type + '_stat') >= 0:
        if lines[n].find('lnN') >= 0:
          aux = lines[n].split("lnN")[1]
          columns = aux.split()
          for i in range(len(columns)):
            if isfloat(columns[i]):
              uncertaintyUp   = uncertaintyUp+pow((float(columns[i])-1)*getYield(filename,type),2)
              uncertaintyDown = uncertaintyDown+pow((float(columns[i])-1)*getYield(filename,type),2)
        if lines[n].find('gmN') >= 0:
          aux = lines[n].split("gmN")[1]
          columns = aux.split()
          nEvents = float(columns[0])
          statUncUp   = getOneSidedUpperLimit(nEvents,0.6827)-nEvents
          statUncDown = nEvents-getOneSidedLowerLimit(nEvents,0.6827)
          columns = columns[1:]
          for i in range(len(columns)):
            if isfloat(columns[i]):
              uncertaintyUp   = uncertaintyUp+pow(float(columns[i])*statUncUp,2)
              uncertaintyDown = uncertaintyDown+pow(float(columns[i])*statUncDown,2)
            
  return (math.sqrt(uncertaintyUp), math.sqrt(uncertaintyDown))
##############################################################################################
def getSysUncertainties(filename):
  uncertainty=0
  lines = open(filename,'r').readlines()[10:-1]
  for n in range(len(lines)):
    for type in ['fake','pion','elec','muon']:
      if lines[n].find(type + '_syst') >= 0:
        aux = lines[n].split("lnN")[1]
        columns = aux.split()
        for i in range(len(columns)):
          if isfloat(columns[i]):
            #print "getYield(filename,type) = " + str(getYield(filename,type))
            #print "(float(columns[i])-1) = " + str((float(columns[i])-1))
            uncertainty = uncertainty+pow((float(columns[i])-1)*getYield(filename,type),2)
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

file    = 'signal/datacards/datacard_metCutEq%s_ptGt%s_Le%s_ECaloCutEq%s_IasGt0p%02.0f_Le0p%02.0f_mass_' %(100,ptCutMin,ptCutMax,5,iasCutMin,iasCutMax) + str(mass) + 'GeV_ctau_' + str(ctau) + 'cm.txt'
nSignal = getYield(file,"signal")
nFake   = getYield(file,"fake")
nPion   = getYield(file,"pion")
nElec   = getYield(file,"elec")
nMuon   = getYield(file,"muon")
nBkg    = nFake + nPion + nElec + nMuon  


statUncUp,statUncDown = getStatUncertainties(file)
#print "statUncUp = " + str(statUncUp)
#print "statUncDown = " + str(statUncDown)
sysUnc=getSysUncertainties(file)
#print "sysUnc = " + str(sysUnc)
signalUnc = getSignalUncertainties(file)


# print Result
print ""
print str(ptCutMin) + "GeV<pt<" + str(ptCutMax) +"GeV, 0." + str('%02.0f' %(iasCutMin)) + "<Ias<0." + str('%02.0f' %(iasCutMax)) + ":"
print "Result = " +str("{0:.2f}".format(nBkg))+ " + " +"{0:.2f}".format(statUncUp) + " - "+ "{0:.2f}".format(statUncDown)+ " (stat)" + " +/- " + "{0:.2f}".format(sysUnc)+ " (sys)"
# print it for latex format
print "Pred. = " +str("{0:.2f}".format(nBkg))+ " $^{+ " +"{0:.2f}".format(statUncUp) + "} _{- "+ "{0:.2f}".format(statUncDown)+ "}$ (stat)" + " $\\pm$ " + "{0:.2f}".format(sysUnc)+ " (sys) \\\\" 
print ""

