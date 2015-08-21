#!bin/bash

#source "../variablesSetting.sh"

root -l -b -q getSignalResult.C+"(${metCut},${jetptCut},${ptCutMin},${ptCutMax},${ecaloCut},${iasCutMin},${iasCutMax},\"${region}\")" > signal.log
cat signal.log

rm *~
#rm *_C.d
#rm *_C.so