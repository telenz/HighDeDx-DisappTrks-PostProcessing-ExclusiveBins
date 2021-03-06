#!bin/bash

#source "../variablesSetting.sh"
echo ""
echo "%%%%%%%%%%%%%%%%%%%%%%% signal yield estimation %%%%%%%%%%%%%%%%%%%%%%%"
for m in "${mass[@]}"
do
    for ct in "${ctau[@]}"
    do
	if [ "$ct" == "1" ] && ([ "$m" == "300" ] || [ "$m" == "400" ] || [ "$m" == "500" ] || [ "$m" == "600" ])
	then
	    continue
	fi
	root -l -b -q getSignalResult.C+"(${metCut},${jetptCut},${ptCutMin},${ptCutMax},${ecaloCut},${iasCutMin},${iasCutMax},\"${region}\",\"Madgraph_signal_mass_${m}_ctau_${ct}cm\")" > logFiles/signal_mass_${m}GeV_ctau_${ct}cm.log
	python makeSignalDatacard.py ${metCut} ${ptCutMin} ${ptCutMax} ${ecaloCut} "${iasCutMin}" "${iasCutMax}" ${m} ${ct}
    done
done
rm *~
#rm *_C.d
#rm *_C.so