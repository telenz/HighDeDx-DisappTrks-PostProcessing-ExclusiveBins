#!/bin/bash

source variablesSetting.sh

#rm leptonicBkg/datacards/*
rm signal/datacards/*
rm systematics/bkg/datacards/*

cd fakeBkg
source estimateFakeBkg.sh
cd ../leptonicBkg
source estimateLeptonicBkg.sh
python makeDatacard.py ${metCut} ${ptCutMin} ${ptCutMax} ${ecaloCut} "${iasCutMin}" "${iasCutMax}"
cd ../signal
source estimateSignal.sh
python makeSignalDatacard.py ${metCut} ${ptCutMin} ${ptCutMax} ${ecaloCut} "${iasCutMin}" "${iasCutMax}"
cd ../systematics/bkg
source estimateBkgUncertainties.sh
cd ../signal
source estimateSignalUncertainties.sh

cd ../..

rm *~
#rm */*.d
#rm */*.so