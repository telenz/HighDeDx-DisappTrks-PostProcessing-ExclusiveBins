#!/usr/bin/env/python

from decimal import Decimal
import sys
import glob


def scanDatacards( searchstring, stringname ):
     uncertaintyMax = 0
     uncertaintyMin = 10000
     filenameMax=""
     for filename in glob.iglob('datacards/*.txt'):
          fIn   = open(filename, 'r')
          lines = fIn.read().split("\n")
          for n in range(len(lines)):
               if lines[n].find(searchstring) >= 0:
                    aux = lines[n].split('lnN')[1]
                    uncertaintyAux = float(aux.split(' - ')[0]) -1.
                    if(float(uncertaintyAux) > float(uncertaintyMax)):
                         uncertaintyMax = uncertaintyAux
                         filenameMax = filename
                    if(float(uncertaintyAux) < float(uncertaintyMin)):
                         uncertaintyMin = uncertaintyAux
     fout.write(format_string.format(stringname,str('&') + str(round(float(uncertaintyMin*100),1)) , str('&') + str(round(float(uncertaintyMax*100),1)) ))
     fout.write('\\\\ \n')
     print stringname + " = " + str(uncertaintyMax*100)
     print filenameMax
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
scanDatacards("ISR","ISR")
scanDatacards("trigger","Trigger efficiency")
scanDatacards("JES","JES")
scanDatacards("JER","JER")
scanDatacards("PDF","PDF")
scanDatacards("ECalo","Simulation of calorimeter isolation")
scanDatacards("HitsLostMiddle","Simulation of missing middle hits")
scanDatacards("HitsLostInner","Simulation of missing inner hits")
scanDatacards("PU","PU")
scanDatacards("TrkRecoEff","Track reconstruction efficiency")
scanDatacards("Ias","Simulation of Ias")
scanDatacards("xsec","Theoretical x-section")
##########################################################################################################################################################################################
fout.write('\\hline')
fout.write('\\end{tabular}  \n')
fout.write('\\end{table} \n')

