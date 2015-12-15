#!/bin/bash

metCut=100
jetptCut=110
ecaloCut=5
region="SR"

ptMinArray=(20    25    30    35    40    45    50    55    60)
ptMaxArray=(50000 50000 50000 50000 50000 50000 50000 50000 50000)
iasMinArray=(0.00 0.05 0.10 0.15 0.20 0.25 0.30 0.35 0.40)
iasMaxArray=(0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99)

mass=("100" "200" "300" "400" "500")
ctau=("1" "5" "10" "50" "100")

#mass=("500")
#ctau=("1")

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
	
	cd ../fakeBkg
	source estimateFakeBkg.sh

	cd ../leptonicBkg
	source estimateLeptonicBkg.sh
	python makeDatacard.py ${metCut} ${ptCutMin} ${ptCutMax} ${ecaloCut} "${iasCutMin}" "${iasCutMax}"

	cd ../signal
	source estimateSignal.sh

    done
done

cd ../optimisation
#if [ ! -d "totalBkg" ]; then  mkdir totalBkg;fi;
#cp -rp ../leptonicBkg/results/Total*.root totalBkg/.
#if [ ! -d "signal" ]; then  mkdir signal;fi;
#cp -rp ../signal/results/*.root signal/.

#root -l -b -q calculateSignificance.C+"(${metCut}, ${ecaloCut}, \"${region}\")"

rm *~
rm *_C.d
rm *_C.so


