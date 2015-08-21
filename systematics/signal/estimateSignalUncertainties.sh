#!bin/bash

#source "../../variablesSetting.sh"

echo -e "\n####################################################################################################################################################"
echo -e "##################################### Estimation of signal systematic uncertainties ################################################################"
echo -e "####################################################################################################################################################\n"

mass=("100" "200" "300" "400" "500")
ctau=("1" "5" "10" "50" "100")                           #please specifiy in cm

##########################################################################################################################################################
echo -e "\n#################################################### Ecalo uncertainty ####################################################\n"
cd 7_Ecalo
if [ ! -f "logFiles/uncertainty_ptGt${ptCutMin}_Le${ptCutMax}_ecaloCutEq${ecaloCut}.log" ] 
then
    root -l -b -q getSysUncertainty.C+"(${ptCutMin},${ptCutMax},${ecaloCut})"  > logFiles/uncertainty_ptGt${ptCutMin}_Le${ptCutMax}_ecaloCutEq${ecaloCut}.log
    cat logFiles/uncertainty_ptGt${ptCutMin}_Le${ptCutMax}_ecaloCutEq${ecaloCut}.log
fi
cd ..
echo -e "\n#################################################### n_miss_middle ####################################################\n"
cd 8_NInnerMiddleLost
if [ ! -f "logFiles/uncertainty_Middle_ptGt${ptCutMin}_Le${ptCutMax}.log" ] 
then
    root -l -b -q getSysUncertaintyMiddle.C+"(${ptCutMin},${ptCutMax})"  > logFiles/uncertainty_Middle_ptGt${ptCutMin}_Le${ptCutMax}.log
fi
cat logFiles/uncertainty_Middle_ptGt${ptCutMin}_Le${ptCutMax}.log
cd ..
echo -e "\n#################################################### n_miss_inner ####################################################\n"
cd 8_NInnerMiddleLost
if [ ! -f "logFiles/uncertainty_Inner_ptGt${ptCutMin}_Le${ptCutMax}.log" ] 
then
    root -l -b -q getSysUncertaintyInner.C+"(${ptCutMin},${ptCutMax})"  > logFiles/uncertainty_Inner_ptGt${ptCutMin}_Le${ptCutMax}.log
fi
cat logFiles/uncertainty_Inner_ptGt${ptCutMin}_Le${ptCutMax}.log
cd ..
echo -e "\n#################################################### trk reco eff ####################################################\n"
cd 10_TrkRecoEff
source runSystematicUncertainty.sh
cat logFiles/uncertainty_ptGt${ptCutMin}_Le${ptCutMax}.log
cd ..
echo -e "\n#################################################### Ias ####################################################\n"
cd 11_Ias
root -l -b -q makeFit.C+"()"  > logFiles/uncertainty.log
cat logFiles/uncertainty.log
cd ..

##########################################################################################################################################################

for m in "${mass[@]}"
do
    echo "m="$m
    ct=100

    echo -e "\n#################################################### Estimate ISR uncertainty ####################################################\n"
    cd 2_ISR
    root -l -b -q getSysUncertainty.C+"(${ptCutMin},${ptCutMax},${ecaloCut},\"Madgraph_signal_mass_${m}_ctau_100cm.root\")"  > logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
    cat logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
    cd ..
    echo -e "\n#################################################### Estimate Trigger uncertainty ####################################################\n"
    cd 3_TriggerEfficiency
    root -l -b -q getSysUncertainty.C+"(${ptCutMin},${ptCutMax},${ecaloCut},\"Madgraph_signal_mass_${m}_ctau_100cm.root\")"  > logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
    cat logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
    cd ..
    echo -e "\n#################################################### JES uncertainty ####################################################\n"
    cd 4_JetEnergyScale
    root -l -b -q getSysUncertainty.C+"(${ptCutMin},${ptCutMax},${ecaloCut},\"Madgraph_signal_mass_${m}_ctau_100cm.root\")"  > logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
    cat logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
    cd ..
    echo -e "\n#################################################### JER uncertainty ####################################################\n"
    cd 5_JetEnergyResolution
    root -l -b -q getSysUncertainty.C+"(${ptCutMin},${ptCutMax},${ecaloCut},\"Madgraph_signal_mass_${m}_ctau_100cm.root\")"  > logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
    cat logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
    cd ..
    echo -e "\n#################################################### PDF uncertainty ####################################################\n"
    cd 6_PDF
    root -l -b -q getSysUncertainty.C+"(${ptCutMin},${ptCutMax},${ecaloCut},\"Madgraph_signal_mass_${m}_ctau_100cm.root\")"  > logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
    cat logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
    cd ..

    for ct in "${ctau[@]}"
    do
	echo "ctau="$ct

	echo -e "\n#################################################### Estimate ISR uncertainty ####################################################\n"
	cd 2_ISR
	cp logFiles/Madgraph_signal_mass_${m}_ctau_100cm.log logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
	cd ..
	echo -e "\n#################################################### Estimate Trigger uncertainty ####################################################\n"
	cd 3_TriggerEfficiency
	cp logFiles/Madgraph_signal_mass_${m}_ctau_100cm.log logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
	cd ..
	echo -e "\n#################################################### JES uncertainty ####################################################\n"
	cd 4_JetEnergyScale
	cp logFiles/Madgraph_signal_mass_${m}_ctau_100cm.log logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
	cd ..
	echo -e "\n#################################################### JER uncertainty ####################################################\n"
	cd 5_JetEnergyResolution
	cp logFiles/Madgraph_signal_mass_${m}_ctau_100cm.log logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
	cd ..
	echo -e "\n#################################################### PDF uncertainty ####################################################\n"
	cd 6_PDF
	cp logFiles/Madgraph_signal_mass_${m}_ctau_100cm.log logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
	cd ..
	echo -e "\n#################################################### PU uncertainty ####################################################\n"
	cd 9_Pileup
	root -l -b -q getSysUncertainty.C+"(${ptCutMin},${ptCutMax},${ecaloCut},${iasCutMin},${iasCutMax},\"Madgraph_signal_mass_${m}_ctau_${ct}cm.root\")"  > logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
	cat logFiles/Madgraph_signal_mass_${m}_ctau_${ct}cm.log
	cd ..

	python addSysUncertaintiesToDatacard.py ${metCut} ${ptCutMin} ${ptCutMax} ${ecaloCut} "${iasCutMin}" "${iasCutMax}" "${m}" "${ct}"


    done
done

