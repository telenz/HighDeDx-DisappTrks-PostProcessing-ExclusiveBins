#!bin/bash

#ptCut=60

###########################################################################################################################
# edit analyzer.cc file and run analysis


if [ ! -f "logFiles/uncertainty_ptGt${ptCutMin}_Le${ptCutMax}.log" ] 
then
    echo ""
    echo "%%%%%%%%% logFiles/uncertainty_ptGt${ptCutMin}_Le${ptCutMax}.log does not exists: Estimate trk reco eff uncertainty: "
    echo ""

    cd removeHits
    sed -e "s/MuonTracksAllHits_pt\[i\] < __ptCutMin__/MuonTracksAllHits_pt\[i\] < ${ptCutMin}/" analyzer.cc > analyzerAux.cc
    sed -e "s/MuonTracksAllHits_pt\[i\] > __ptCutMax__/MuonTracksAllHits_pt\[i\] > ${ptCutMax}/" analyzerAux.cc > analyzerAux2.cc
    mv analyzerAux2.cc analyzer.cc
    rm analyzerAux.cc
    #make clean
    make
    echo "before analysis"
    ./analyzer filelist_data.txt analyzer_histogram_data.root > ../result_data.log
    ./analyzer filelist_mc.txt   analyzer_histogram_mc.root   > ../result_mc.log
    echo "after analysis"
    sed -e "s/MuonTracksAllHits_pt\[i\] < ${ptCutMin}/MuonTracksAllHits_pt[i] < __ptCutMin__/" analyzer.cc > analyzerAux.cc
    sed -e "s/MuonTracksAllHits_pt\[i\] > ${ptCutMax}/MuonTracksAllHits_pt[i] > __ptCutMax__/" analyzerAux.cc > analyzerAux2.cc
    mv analyzerAux2.cc analyzer.cc
    rm analyzerAux.cc
    cd ..
    python getSysUncertainty.py > logFiles/uncertainty_ptGt${ptCutMin}_Le${ptCutMax}.log
fi
cat logFiles/uncertainty_ptGt${ptCutMin}_Le${ptCutMax}.log
###########################################################################################################################


rm *~
