#ifndef INPUTANALYSISFILES_H
#define INPUTANALYSISFILES_H




TString fileSR          = "~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_19_METGt0_JetPt70_noBlinding/results/analyzer/ntuples/input_weighted/";
TString filePionCS      = "~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_19_METGt0_JetPt70_TauCS/results/analyzer/ntuples/input_weighted/";
TString fileElectronCS  = "~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_19_METGt0_JetPt70_ElectronCS/results/analyzer/ntuples/input_weighted/";
TString fileMuonCS      = "~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_19_METGt0_JetPt70_MuonCS/results/analyzer/ntuples/input_weighted/";
TString fileFakeCS      = "~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_19_METGt0_JetPt70_FakeCS/results/analyzer/ntuples/input_weighted/";

TString fileDYToLL         = "/afs/desy.de/user/t/tlenz/xxl-af-cms/DYTOLLANALYSIS/workdir/analysis_2015_06_16_newNtuples/results/analyzer/ntuples/input_weighted/";
TString fileSingleMu       = "/afs/desy.de/user/t/tlenz/xxl-af-cms/DYTOLLANALYSIS/workdir/analysis_2015_06_16_newNtuples/results/analyzer/ntuples/input_weighted/SingleMu.root";
TString fileSingleElectron = "/afs/desy.de/user/t/tlenz/xxl-af-cms/DYTOLLANALYSIS/workdir/analysis_2015_06_16_newNtuples/results/analyzer/ntuples/input_weighted/SingleElectron.root";


// For systematic uncertainties
// 1.) Bkg
TString inputScaleUnc         = "/afs/desy.de/user/t/tlenz/xxl-af-cms/DYTOLLANALYSIS/workdir/analysis_2015_06_25_TagAndProbe/results/analyzer/ntuples/input_weighted/";
TString inputScaleUncPion     = "/afs/desy.de/user/t/tlenz/xxl-af-cms/DYTOLLANALYSIS/workdir/analysis_2015_06_25_TagAndProbe_TauCS/results/analyzer/ntuples/input_weighted/";
TString inputScaleUncMuon     = "/afs/desy.de/user/t/tlenz/xxl-af-cms/DYTOLLANALYSIS/workdir/analysis_2015_06_25_TagAndProbe_MuonCS/results/analyzer/ntuples/input_weighted/";
TString inputScaleUncElectron = "/afs/desy.de/user/t/tlenz/xxl-af-cms/DYTOLLANALYSIS/workdir/analysis_2015_06_25_TagAndProbe_ElectronCS/results/analyzer/ntuples/input_weighted/";

//2.) Signal
TString inputISRcentral       = fileSR;
TString inputISRup            = "/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_06_ISRUnc_up/results/analyzer/ntuples/input_weighted/";
TString inputISRdown          = "/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_06_ISRUnc_down/results/analyzer/ntuples/input_weighted/";

TString inputTrigEffNominal  = fileSR;
TString inputTrigEffWeighted = "/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_06_TrigEffunc_weighted/results/analyzer/ntuples/input_weighted/";

TString inputJEScentral      = fileSR;
TString inputJESup           = "/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_06_JESunc_up/results/analyzer/ntuples/input_weighted/";
TString inputJESdown         = "/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_06_JESunc_down/results/analyzer/ntuples/input_weighted/";

TString inputJERcentral      = fileSR;
TString inputJERup           = "/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_06_JERunc_up/results/analyzer/ntuples/input_weighted/";
TString inputJERdown         = "/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_06_JERunc_down/results/analyzer/ntuples/input_weighted/";

TString inputPUcentral      = fileSR;
TString inputPUup           = "/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_06_PUunc_up/results/analyzer/ntuples/input_weighted/";
TString inputPUdown         = "/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_06_PUunc_down/results/analyzer/ntuples/input_weighted/";

TString inputEcaloUnc       = "/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_05_METGt0_JetPtGt0_FakeCS/results/analyzer/ntuples/input_weighted/";

TString inputNmissUnc       = "/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_04_METGt0_JetPt0_NmissUnc/results/analyzer/ntuples/input_weighted/";

TString inputPDFUnc         = "/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_08_10_PDFUnc/results/analyzer/ntuples/input_weighted/";

#endif
