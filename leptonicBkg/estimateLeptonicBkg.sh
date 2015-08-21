#!bin/bash

#source "../variablesSetting.sh"

echo ""
echo "%%%%%%%%%%%%%%%%%%%%%%% Make leptonic background estimates %%%%%%%%%%%%%%%%%%%%%%%"

rm *log
#rm datacards/*

## 1.) Get scaling factors s = N_MC^SR/N_MC^CR
root -l -b -q a1_getScalingFactors.C+"(211,${metCut},${jetptCut},0,${ptCutMin},${ptCutMax},${ecaloCut},\"${region}\")" > logFiles/pion.log
root -l -b -q a1_getScalingFactors.C+"(11,${metCut},${jetptCut},0,${ptCutMin},${ptCutMax},${ecaloCut},\"${region}\")"  > logFiles/elec.log
root -l -b -q a1_getScalingFactors.C+"(13,${metCut},${jetptCut},1,${ptCutMin},${ptCutMax},${ecaloCut},\"${region}\")"  > logFiles/muon.log

## 1a.) Get Ias template
root -l -b -q a2_makeTemplates.C+"(211,${metCut},${jetptCut},${ecaloCut},${iasCutMin},${iasCutMax},\"${region}\")" >> logFiles/pion.log
root -l -b -q a2_makeTemplates.C+"(11,${metCut},${jetptCut},${ecaloCut},${iasCutMin},${iasCutMax},\"${region}\")"  >> logFiles/elec.log
root -l -b -q a2_makeTemplates.C+"(13,${metCut},${jetptCut},${ecaloCut},${iasCutMin},${iasCutMax},\"${region}\")"  >> logFiles/muon.log


## 2.) Get leptonic bkg prediction
root -l -b -q a3_getLeptonPrediction.C+"(211,${metCut},${jetptCut},${ptCutMin},${ptCutMax},${iasCutMin},${iasCutMax},0,${ecaloCut},\"${region}\")" >> logFiles/pion.log
cat logFiles/pion.log
root -l -b -q a3_getLeptonPrediction.C+"(11,${metCut},${jetptCut},${ptCutMin},${ptCutMax},${iasCutMin},${iasCutMax},0,${ecaloCut},\"${region}\")" >> logFiles/elec.log
cat logFiles/elec.log
root -l -b -q a3_getLeptonPrediction.C+"(13,${metCut},${jetptCut},${ptCutMin},${ptCutMax},${iasCutMin},${iasCutMax},1,${ecaloCut},\"${region}\")" >> logFiles/muon.log
cat logFiles/muon.log


## 3.) Add all bks together
root -l -b -q   a4_addBkgsTogether.C+"(${metCut},${ptCutMin},${ptCutMax},${ecaloCut},${iasCutMin},${iasCutMax},\"${region}\")" > logFiles/results.log
cat logFiles/results.log


#rm a1_getScalingFactors_C.*
#rm a2_getLeptonPrediction_C.*
#rm a3_addBkgsTogether_C.*
#rm *_C.d
#rm *_C.so
rm *~