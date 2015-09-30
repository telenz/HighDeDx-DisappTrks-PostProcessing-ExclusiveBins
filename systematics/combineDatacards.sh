ini5
ini6
cd ../limits/CMSSW_5_3_20/src/HiggsAnalysis-CombinedLimit/
cmsenv
cd -

folder="signal/datacards"

source ../variablesSetting.sh

for m in "${mass[@]}"
do
    for ct in "${ctau[@]}"
    do
	if [ "$ct" == "1" ] && ([ "$m" == "300" ] || [ "$m" == "400" ] || [ "$m" == "500" ] || [ "$m" == "600" ])
	then
	    continue
	fi
      combineCards.py ${folder}/datacard_metCutEq100_ptGt30_Le50_ECaloCutEq5_IasGt0p05_Le0p30_mass_${m}GeV_ctau_${ct}cm.txt ${folder}/datacard_metCutEq100_ptGt30_Le50_ECaloCutEq5_IasGt0p30_Le0p99_mass_${m}GeV_ctau_${ct}cm.txt ${folder}/datacard_metCutEq100_ptGt50_Le50000_ECaloCutEq5_IasGt0p05_Le0p30_mass_${m}GeV_ctau_${ct}cm.txt ${folder}/datacard_metCutEq100_ptGt50_Le50000_ECaloCutEq5_IasGt0p30_Le0p99_mass_${m}GeV_ctau_${ct}cm.txt   > datacardsCombined/datacard_mass_${m}GeV_ctau_${ct}cm.txt

    done
done

python makePrediction.py 30 50 0.05 0.30 ${folder}
python makePrediction.py 30 50 0.30 0.99 ${folder}
python makePrediction.py 50 50000 0.05 0.30 ${folder}
python makePrediction.py 50 50000 0.30 0.99 ${folder}

