ini6
cmsenv

metCut=100
ecaloCut=5
ptCut=50
iasCutTimes100=30

#cp ../../../../systematics/bkg/datacards/* datacards/.

# Run all limits
#massTable=("200" "300" "400" "500")         #please specifiy in GeV
#decaywidthTable=("1" "5" "10" "50" "100")   #please specifiy in cm

#massTable=("100" "200" "300" "400" "500")         #please specifiy in GeV
#decaywidthTable=("1" "5" "10" "50" "100")   #please specifiy in cm


massTable=("500")         #please specifiy in GeV
decaywidthTable=("100")   #please specifiy in cm


for ctau in "${decaywidthTable[@]}"
do
    echo "ctau = " $ctau
    for mass in "${massTable[@]}"
    do
	echo "mass = " $mass

	combine -M Asymptotic --run expected -C 0.95 -t -1 --minimizerStrategy 2 datacards/datacard_metCutEq${metCut}_ptCutEq${ptCut}_ECaloCutEq${ecaloCut}_IasCutEq0p${iasCutTimes100}_mass_${mass}GeV_ctau_${ctau}cm.txt > logFiles/res_metCutEq${metCut}_ptCutEq${ptCut}_ECaloCutEq${ecaloCut}_IasCutEq0p${iasCutTimes100}_mass_${mass}GeV_ctau_${ctau}cm.log
	cat logFiles/res_metCutEq${metCut}_ptCutEq${ptCut}_ECaloCutEq${ecaloCut}_IasCutEq0p${iasCutTimes100}_mass_${mass}GeV_ctau_${ctau}cm.log

	echo ""
	echo ""

    done
    root -l -b -q plot_limit.cc+"(${metCut},${ptCut},${ecaloCut},${iasCutTimes100},${ctau})"
    mv test_ctau${ctau}cm.pdf limits/LimitPlot_ctau${ctau}cm_iasGt0p${iasCutTimes100}_ptGt${ptCut}.pdf
done

rm *.root
rm *.~





