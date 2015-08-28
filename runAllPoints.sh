#!/bin/bash

source "variablesSetting.sh"

metCut=100
jetptCut=110
ecaloCut=5
region="SR"


ptMinArray=(30 50)
ptMaxArray=(50 50000)
iasMinArray=(0.05 0.30)
iasMaxArray=(0.30 0.99)


find . -type f -name "*.log" -not -path "./limits/*" -not -path  "./systematics/signal/12_xsec/*" -not -path  "./systematics/signal/1_Luminosity/*" -not -path  "./systematics/signal/10_TrkRecoEff/*" -exec rm -f {} \;
rm systematics/signal/datacards/*
for ((i=0; i < ${#ptMinArray[@]}; i++))
do
    ptCutMin=${ptMinArray[$i]}
    ptCutMax=${ptMaxArray[$i]}
    for ((j=0; j < ${#iasMinArray[@]}; j++))
    do
	iasCutMin=${iasMinArray[$j]}
	iasCutMax=${iasMaxArray[$j]}
	echo ""
	echo " ************************************************** Pt  Cut = ${ptCutMin} - ${ptCutMax}    **************************************************"
	echo " ************************************************** Ias Cut = ${iasCutMin} - ${iasCutMax}  **************************************************"
	echo ""
	cd fakeBkg
	source estimateFakeBkg.sh
	cd ../leptonicBkg
	source estimateLeptonicBkg.sh
	python makeDatacard.py ${metCut} ${ptCutMin} ${ptCutMax} ${ecaloCut} "${iasCutMin}" "${iasCutMax}"
	cd ../signal
	source estimateSignal.sh
	cd ../systematics/bkg
	source estimateBkgUncertainties.sh
	cd ../signal
	source estimateSignalUncertainties.sh
	cd ../..
    done
done

var=$(date +"%F"_"%H"-"%M"-"%S")
cp -rp systematics/signal/datacards systematics/datacards_${var}


rm *~


