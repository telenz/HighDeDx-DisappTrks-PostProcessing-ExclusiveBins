#!/usr/bin/env/python

from decimal import Decimal
import sys
import glob
import math


def scanDatacards( searchstring, stringname ):
     uncertaintyMax = 0
     uncertaintyMin = 10000
     filenameMax=""
     filenameMin=""
     for filename in glob.iglob('datacards/*.txt'):
          fIn   = open(filename, 'r')
          aux   = filename.split("mass_")[1]
          mass  = aux.split("GeV")[0]
          aux   = filename.split("ctau_")[1]
          ctau  = aux.split("cm")[0]
          lines = fIn.read().split("\n")
          for n in range(len(lines)):
               if lines[n].find(searchstring) >= 0:
                    aux = lines[n].split('lnN')[1]
                    uncertaintyAux = float(aux.split(' - ')[0]) -1.
                    if(float(uncertaintyAux) > float(uncertaintyMax)):
                         if float(ctau)<0.855*math.exp(0.0085*float(mass)):
                              continue
                         if float(ctau)>20138.*math.exp(-0.007*float(mass)):
                              continue
                         if float(mass)>550.:
                              continue
                         uncertaintyMax = uncertaintyAux
                         filenameMax = filename
                    if(float(uncertaintyAux) < float(uncertaintyMin)):
                         uncertaintyMin = uncertaintyAux
                         filenameMin = filename
     fout.write(format_string.format(stringname,str('&') + str(round(float(uncertaintyMin*100),1)) , str('&') + str(round(float(uncertaintyMax*100),1)) ))
     fout.write('\\\\ \n')
     print stringname + " = " + str(uncertaintyMax*100)
     print "Maximal value from " + filenameMax
     print "Minimal value from " + filenameMin
     print ""

fout = open('signalSystematicsOverview.txt', 'w')
fout.write("Summary table of all systematic uncertainties\\\\")
fout.write('\n\n')
fout.write('\\begin{table}[H] \n')
fout.write('\\begin{tabular}{|l|c|c|}  \\hline \n')
format_string = "{0:<40}{1:<20}{2:<20}"
fout.write(format_string.format("Uncertainty","&Min [\\%]","&Max [\\%]"))
fout.write('\\\\ \\hline \n')
format_string = "{0:<40}{1:<20}{2:<20}"



##########################################################################################################################################################################################
scanDatacards("lumi","Luminosity")
scanDatacards("ISR","Simulation of ISR")
scanDatacards("trigger","Simulation of trigger efficiency")
scanDatacards("JES","JES")
scanDatacards("JER","JER")
scanDatacards("PDF","Simulation of PDF")
scanDatacards("PU","Pile-up reweighting")
scanDatacards("ECalo","Simulation of calorimeter isolation")
scanDatacards("HitsLostMiddle","Simulation of missing middle hits")
scanDatacards("HitsLostInner","Simulation of missing inner hits")
scanDatacards("Ias","Simulation of Ias")
scanDatacards("TrkRecoEff","Simulation of track reconstruction efficiency")
##########################################################################################################################################################################################
fout.write('\\hline')
fout.write('\\end{tabular}  \n')
fout.write('\\end{table} \n')

