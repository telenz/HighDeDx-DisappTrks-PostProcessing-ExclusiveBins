#ifndef INPUTANALYSISFILES_H
#define INPUTANALYSISFILES_H



TString fileSR          = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_10_11_METGt0_JetPt70_noBlinding/results/analyzer/ntuples/input_weighted/";
TString filePionCS      = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_08_19_METGt0_JetPt70_TauCS/results/analyzer/ntuples/input_weighted/";
TString fileElectronCS  = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_08_19_METGt0_JetPt70_ElectronCS/results/analyzer/ntuples/input_weighted/";
TString fileMuonCS      = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_08_19_METGt0_JetPt70_MuonCS/results/analyzer/ntuples/input_weighted/";
TString fileFakeCS      = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_08_19_METGt0_JetPt70_FakeCS/results/analyzer/ntuples/input_weighted/";

TString fileDYToLL         = "/nfs/dust/cms/user/tlenz/DYTOLLANALYSIS/workdir/analysis_2015_09_18_FilesOnDcache/results/analyzer/ntuples/input_weighted/";
TString fileSingleMu       = "/nfs/dust/cms/user/tlenz/DYTOLLANALYSIS/workdir/analysis_2015_09_18_FilesOnDcache/results/analyzer/ntuples/input_weighted/SingleMu.root";
TString fileSingleElectron = "/nfs/dust/cms/user/tlenz/DYTOLLANALYSIS/workdir/analysis_2015_09_18_FilesOnDcache/results/analyzer/ntuples/input_weighted/SingleElectron.root";



// For systematic uncertainties
// 1.) Bkg
TString inputScaleUnc         = "/nfs/dust/cms/user/tlenz/DYTOLLANALYSIS/workdir/analysis_2015_09_18_FilesOnDcache/results/analyzer/ntuples/input_weighted/";
TString inputScaleUncPion     = "/nfs/dust/cms/user/tlenz/DYTOLLANALYSIS/workdir/analysis_2015_06_25_TagAndProbe_TauCS/results/analyzer/ntuples/input_weighted/";
TString inputScaleUncMuon     = "/nfs/dust/cms/user/tlenz/DYTOLLANALYSIS/workdir/analysis_2015_06_25_TagAndProbe_MuonCS/results/analyzer/ntuples/input_weighted/";
TString inputScaleUncElectron = "/nfs/dust/cms/user/tlenz/DYTOLLANALYSIS/workdir/analysis_2015_06_25_TagAndProbe_ElectronCS/results/analyzer/ntuples/input_weighted/";

//2.) Signal
TString inputISRcentral       = fileSR;
TString inputISRup            = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_09_18_ISRunc_up/results/analyzer/ntuples/input_weighted/";
TString inputISRdown          = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_09_18_ISRunc_down/results/analyzer/ntuples/input_weighted/";

TString inputTrigEffNominal  = fileSR;
TString inputTrigEffWeighted = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_09_18_TrigEffunc_weighted/results/analyzer/ntuples/input_weighted/";

TString inputJEScentral      = fileSR;
TString inputJESup           = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_09_18_JESunc_up/results/analyzer/ntuples/input_weighted/";
TString inputJESdown         = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_09_18_JESunc_down/results/analyzer/ntuples/input_weighted/";

TString inputJERcentral      = fileSR;
TString inputJERup           = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_09_18_JERunc_up/results/analyzer/ntuples/input_weighted/";
TString inputJERdown         = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_09_18_JERunc_down/results/analyzer/ntuples/input_weighted/";

TString inputPUcentral      = fileSR;
TString inputPUup           = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_10_02_PUunc_up/results/analyzer/ntuples/input_weighted/";
TString inputPUdown         = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_10_02_PUunc_down/results/analyzer/ntuples/input_weighted/";


TString inputEcaloUnc       = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_08_05_METGt0_JetPtGt0_FakeCS/results/analyzer/ntuples/input_weighted/";
TString inputNmissUnc       = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_08_04_METGt0_JetPt0_NmissUnc/results/analyzer/ntuples/input_weighted/";
TString inputPDFUnc         = "/nfs/dust/cms/user/tlenz/ANALYSIS/workdir/analysis_2015_09_18_PDFunc/results/analyzer/ntuples/input_weighted/";

#endif
