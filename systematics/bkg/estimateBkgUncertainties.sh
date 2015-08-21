#!bin/bash

#source "../../variablesSetting.sh"

echo -e "\n####################################################################################################################################################"
echo -e "##################################### Estimation of bkg systematic uncertainties ###################################################################"
echo -e "####################################################################################################################################################\n"

##########################################################################################################################################################
echo -e "\n#################################################### Estimate leptonic scale factor uncertainty ####################################################\n"
cd 1_LeptonScaleFactors

if [ ! -f "logFiles/pion.log" ] 
then
    echo ""
    echo "%%%%%%%%% pion.log does not exists: Estimate pion uncertainty: "
    echo ""
    root -l -b -q getSysUncertainty.C+"(211,20,500000,5000)"   > logFiles/pion.log
    cat logFiles/pion.log
fi
if [ ! -f "logFiles/electron_ptGt${ptCutMin}_Le${ptCutMax}_ecaloCutEq${ecaloCut}.log" ] 
then
    echo ""
    echo "%%%%%%%%% electron_ptGt${ptCutMin}_Le${ptCutMax}_ecaloCutEq${ecaloCut}.log does not exists: Estimate electron uncertainty: "
    echo ""
    root -l -b -q getSysUncertainty.C+"(11,${ptCutMin},${ptCutMax},${ecaloCut})" > logFiles/electron_ptGt${ptCutMin}_Le${ptCutMax}_ecaloCutEq${ecaloCut}.log
    cat logFiles/electron_ptGt${ptCutMin}_Le${ptCutMax}_ecaloCutEq${ecaloCut}.log
fi
if [ ! -f "logFiles/muon_ecaloCutEq${ecaloCut}.log" ] 
then
    echo ""
    echo "%%%%%%%%% muon_ecaloCutEq${ecaloCut}.log does not exists: Estimate muon uncertainty: "
    echo ""
    root -l -b -q getSysUncertainty.C+"(13,20,500000,${ecaloCut})"       > logFiles/muon_ecaloCutEq${ecaloCut}.log
    cat logFiles/muon_ecaloCutEq${ecaloCut}.log
fi
cd ../
##########################################################################################################################################################
echo -e "\n#################################################### Estimate leptonic I_as uncertainty         ####################################################\n"
cd 2_LeptonIas

iasCutMin2=`bc << EOF
$iasCutMin * 100
EOF
`
iasCutMin2=${iasCutMin2%.*}
iasCutMin2=$(printf %02d $iasCutMin2)

iasCutMax2=`bc << EOF
$iasCutMax * 100
EOF
`
iasCutMax2=${iasCutMax2%.*}
iasCutMax2=$(printf %02d $iasCutMax2)

echo $iasCut2

if [ ! -f "logFiles/pion_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log" ] 
then
    echo ""
    echo "%%%%%%%%% pion_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log does not exists: Estimate pion uncertainty: "
    echo ""
    root -l -b -q getSysUncertainty.C+"(211,5000,${iasCutMin},${iasCutMax})"   > logFiles/pion_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log
    cat logFiles/pion_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log
fi
if [ ! -f "logFiles/electron_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log" ] 
then
    echo ""
    echo "%%%%%%%%% electron_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log does not exists: Estimate electron uncertainty: "
    echo ""
    root -l -b -q getSysUncertainty.C+"(11,5000,${iasCutMin},${iasCutMax})"   > logFiles/electron_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log
    cat logFiles/electron_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log
fi
if [ ! -f "logFiles/muon_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log" ] 
then
    echo ""
    echo "%%%%%%%%% muon_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log does not exists: Estimate muon uncertainty: "
    echo ""
    root -l -b -q getSysUncertainty.C+"(13,5000,${iasCutMin},${iasCutMax})"        > logFiles/muon_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log
    cat logFiles/muon_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log
fi
cd ../
##########################################################################################################################################################
echo -e "\n#################################################### Estimate fake rate uncertainty             ####################################################\n"
cd 3_FakeRate

if [ ! -f "logFiles/fake_ptGt${ptCutMin}_Le${ptCutMax}_ecaloCutEq${ecaloCut}.log" ] 
then
    echo ""
    echo "%%%%%%%%% fake_ptGt${ptCutMin}_Le${ptCutMax}_ecaloCutEq${ecaloCut}.log does not exists: Estimate fake uncertainty: "
    echo ""
    root -l -b -q getSysUncertainty.C+"(${ptCutMin},${ptCutMax},${ecaloCut})"   > logFiles/fake_ptGt${ptCutMin}_Le${ptCutMax}_ecaloCutEq${ecaloCut}.log
    cat logFiles/fake_ptGt${ptCutMin}_Le${ptCutMax}_ecaloCutEq${ecaloCut}.log
fi

cd ../
##########################################################################################################################################################
echo -e "\n#################################################### Estimate fake Ias uncertainty              ####################################################\n"
cd 4_FakeIas

if [ ! -f "logFiles/fake_ptGt${ptCutMin}_Le${ptCutMax}_ecaloCutEq${ecaloCut}_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log" ] 
then
    echo ""
    echo "%%%%%%%%% fake_ptGt${ptCutMin}_Le${ptCutMax}_ecaloCutEq${ecaloCut}_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log does not exists: Estimate fake uncertainty: "
    echo ""
    root -l -b -q getSysUncertainty.C+"(${ptCutMin}, ${ptCutMax}, ${ecaloCut}, ${iasCutMin}, ${iasCutMax})"   > logFiles/fake_ptGt${ptCutMin}_Le${ptCutMax}_ecaloCutEq${ecaloCut}_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log
    cat logFiles/fake_ptGt${ptCutMin}_Le${ptCutMax}_ecaloCutEq${ecaloCut}_iasGt0p${iasCutMin2}_Le0p${iasCutMax2}.log
fi
cd ../
##########################################################################################################################################################
rm *~

mass=("100" "200" "300" "400" "500")
ctau=("1" "5" "10" "50" "100")                           #please specifiy in cm

for ct in "${ctau[@]}"
  do
    for m in "${mass[@]}"
    do
	echo "m="$m
	echo "ctau="$ct
	python addSysUncertaintiesToDatacard.py ${metCut} ${ptCutMin} ${ptCutMax} ${ecaloCut} "${iasCutMin}" "${iasCutMax}" "${m}" "${ct}"
    done
done