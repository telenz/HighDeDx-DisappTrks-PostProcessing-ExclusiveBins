#!/bin/bash

#source "../variablesSetting.sh"

echo ""
echo "%%%%%%%%%%%%%%%%%%%%%%% Make fake background estimates %%%%%%%%%%%%%%%%%%%%%%%"
root -l -b -q a1_fakeRatesFromData.C+"(${ptCutMin},${ptCutMax},${ecaloCut})" 

root -l -b << EOF
   .L a2_makeTemplates.C+
   makeIasTemplates(${metCut},${jetptCut},${ptCutMin},${ptCutMax},${ecaloCut},${iasCutMin},${iasCutMax})
   makeECaloTemplates(${metCut},${jetptCut},${ptCutMin},${ptCutMax},${ecaloCut})
EOF

root -l -b -q a3_getFakePrediction.C+"(${metCut}, ${jetptCut} , ${ecaloCut} , ${iasCutMin} , ${iasCutMax} , ${ptCutMin}, ${ptCutMax})" 


rm *~