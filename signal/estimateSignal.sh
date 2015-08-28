#!bin/bash

#source "../variablesSetting.sh"

for ct in "${ctau[@]}"
  do
    for m in "${mass[@]}"
    do
	root -l -b -q getSignalResult.C+"(${metCut},${jetptCut},${ptCutMin},${ptCutMax},${ecaloCut},${iasCutMin},${iasCutMax},\"${region}\",\"Madgraph_signal_mass_${m}_ctau_${ct}cm\")" > logFiles/signal_mass_${m}GeV_ctau_${ct}cm.log
	python makeSignalDatacard.py ${metCut} ${ptCutMin} ${ptCutMax} ${ecaloCut} "${iasCutMin}" "${iasCutMax}" ${m} ${ct}
    done
done
rm *~
#rm *_C.d
#rm *_C.so