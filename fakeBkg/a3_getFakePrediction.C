#include "TROOT.h"
#include "TFile.h"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLine.h"
#include "TTree.h"
#include "TChain.h"
#include "TRandom3.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include "../plotStyle.h"
#include "../inputAnalysisFiles.h"


class sample{


public:

  TFile *file;
  TTree *tree;

  TH1D* histo;
  TH1D* histoASmiSR;
  TH1D* histoECaloSR;

  int    statistics;
  double purity;

  double weight;
  double weight_xsec_lumi;
  double met;
  double leadingJetPt;


  sample(){

    tree=0;
    file=0;
    histo=0;
    statistics=0;
    purity=0;
    histoASmiSR=0;
    histoECaloSR=0;

  };

  void getTreeVariables(){
    tree->SetBranchAddress("MET",&met);
    tree->SetBranchAddress("LeadingJetPt",&leadingJetPt);
    tree->SetBranchAddress("weight",&weight);
    tree->SetBranchAddress("weight_xsec_lumi",&weight_xsec_lumi);
  };
  

  void Selection(double metCut, double jetptCut){

    if(histoASmiSR)  histoASmiSR  -> Sumw2();
    if(histoECaloSR) histoECaloSR -> Sumw2();
    
    statistics = 0;
    purity=0;


    cout<<"Entries = "<<tree->GetEntries()<<endl;
    for(int n=0; n<tree->GetEntries(); n++){

      tree->GetEntry(n);
      if(met<metCut)                     continue;
      if(leadingJetPt<jetptCut)               continue;
            
      histoASmiSR    -> Fill(0.5,weight_xsec_lumi*weight);
    
    }

    purity=statistics/purity;
  };
};

//################################################################################################################################
//################################################################################################################################
//################################################################################################################################
int a3_getFakePrediction(double metCut, double jetptCut, double ecaloCut=5., double iasCutMin=0.05, double iasCutMax=0.3,double ptCutMin=30,double ptCutMax=50){

  TString filename  = "";
  TString histoName = "";

  sample dataMET;
  dataMET.file = new TFile(fileSR + "data.root","READ");
  dataMET.file   -> GetObject("chiTracksQCDsupression/Variables",dataMET.tree);
  dataMET.getTreeVariables();


  dataMET.histoASmiSR   = new TH1D("nSRCR3","nSRCR3",1,0,1);
  //dataMET.Selection(metCut, jetptCut);
    
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Fake Prediction in CR1+CR2 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  TFile *inFakeRate = new TFile("templates/fakeRatesDATA.root","READ");
  TH1D* fakeRates = 0;
  inFakeRate->GetObject("fakeRates",fakeRates);

  int bin          = fakeRates->GetXaxis()->FindBin(Form("%.0f-%.0f_%.0f",ptCutMin,ptCutMax,ecaloCut));
  cout<<"fake rate = "<<fakeRates->GetBinContent(bin)<<endl;
  //double integralError = 0;
  //double integral = dataMET.histoASmiSR->IntegralAndError(1,dataMET.histoASmiSR->GetNbinsX(),integralError);
  double integral      = dataMET.tree->GetEntries(Form("MET>%f && LeadingJetPt>%f",metCut,jetptCut));
  double integralError = sqrt(integral);
  cout<<"#entries kinematic selection = "<<integral<<endl;
  double nSRCR3    = integral*fakeRates->GetBinContent(bin);
  double nSRCR3Err = sqrt( pow( integral*fakeRates->GetBinError(bin) ,2) + pow( fakeRates->GetBinContent(bin)*integralError  ,2)  );
  cout<<"nSRCR3 = "<<nSRCR3<<" +/- "<<nSRCR3Err<<endl;

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Fake Prediction in CR1+CR2 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  cout<<"ptCutMin = "<<ptCutMin<<endl;
  cout<<"ptCutMax = "<<ptCutMax<<endl;
  filename = Form("templates/ECaloTemplate_METGt%.0f_JetPtGt%.0f_trackPtGt%.0f_Le%0.f.root",metCut,jetptCut,ptCutMin,ptCutMax);
  cout<<"ECalo template from :"<<filename<<endl;
  TFile *inCaloIso = new TFile(filename,"READ");
  TH1D* histoECalo = 0;
  histoName = Form("fake_bkg_data_ECaloCut_%.0f",ecaloCut);
  cout<<"histogram = "<<histoName<<endl; 
  inCaloIso->GetObject(histoName,histoECalo);

  double lowCaloRegion     = histoECalo->GetBinContent(1);
  double lowCaloRegionErr  = histoECalo->GetBinError(1);
  double highCaloRegion    = histoECalo->GetBinContent(2);
  double highCaloRegionErr = histoECalo->GetBinError(2);

  double nCR1CR2    = nSRCR3*highCaloRegion/lowCaloRegion;
  double nCR1CR2Err = nCR1CR2 * sqrt( pow( nSRCR3Err/nSRCR3 ,2) + pow( highCaloRegionErr/highCaloRegion ,2) + pow( lowCaloRegionErr/lowCaloRegion ,2) );
  cout<<"nCR1CR2 = "<<nCR1CR2<<" +/- "<<nCR1CR2Err<<endl;
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Fake Prediction in All regions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  filename             =   Form("templates/IasTemplate_METGt%.0f_JetPtGt%.0f_trackPtGt%0.f_Le%0.f_ECaloEq%.0f.root",metCut,jetptCut,ptCutMin,ptCutMax,ecaloCut);
  cout<<"Ias template from :"<<filename<<endl;
  TFile *inIas         =   new TFile(filename,"READ");
  TH1D* histoLowECalo  = 0;
  TH1D* histoHighECalo = 0;
  histoName            = Form("fake_bkg_data_IascutMin_0p%02.0f_IascutMax_0p%02.0f_EcaloLow_1",iasCutMin*100,iasCutMax*100);
  cout<<"histogram low ECalo = "<<histoName<<endl; 
  inIas  ->  GetObject(histoName,histoLowECalo);
  histoName            = Form("fake_bkg_data_IascutMin_0p%02.0f_IascutMax_0p%02.0f_EcaloLow_0",iasCutMin*100,iasCutMax*100);
  cout<<"histogram high ECalo = "<<histoName<<endl; 
  inIas  ->  GetObject(histoName,histoHighECalo);

  // Low ECalo region
  cout<<"Low Ecalo = "<<endl;
  cout<<"hello"<<endl;
  cout<<histoLowECalo->Integral()<<endl;
  double lowIas     = histoLowECalo->GetBinContent(1);
  double lowIasErr  = histoLowECalo->GetBinError(1);
  cout<<"lowIas = "<<lowIas<<" +/- "<<lowIasErr<<endl;
  double highIas    = histoLowECalo->GetBinContent(2);
  double highIasErr = histoLowECalo->GetBinError(2);
  cout<<"highIas = "<<highIas<<" +/- "<<highIasErr<<endl;
  cout<<"relation = "<<lowIas/highIas<<endl;


  double nSR     = nSRCR3*highIas;
  double nSRErr  = sqrt( pow( highIas*nSRCR3Err ,2) + pow( nSRCR3*highIasErr ,2) );
  double nCR3    = nSRCR3*lowIas;
  double nCR3Err = sqrt( pow( lowIas*nSRCR3Err ,2) + pow( nSRCR3*lowIasErr ,2) );

  // High ECalo region
  cout<<"High Ecalo = "<<endl;
  lowIas     = histoHighECalo->GetBinContent(1);
  lowIasErr  = histoHighECalo->GetBinError(1);
  highIas    = histoHighECalo->GetBinContent(2);
  highIasErr = histoHighECalo->GetBinError(2);
  cout<<"lowIas = "<<lowIas<<" +/- "<<lowIasErr<<endl;
  cout<<"highIas = "<<highIas<<" +/- "<<highIasErr<<endl;
  cout<<"relation = "<<lowIas/highIas<<endl;

  double nCR1     = nCR1CR2*highIas;
  double nCR1Err  = sqrt( pow( highIas*nCR1CR2Err ,2) + pow( nCR1CR2*highIasErr ,2) );
  double nCR2     = nCR1CR2*lowIas;
  double nCR2Err  = sqrt( pow( lowIas*nCR1CR2Err ,2) + pow( nCR1CR2*lowIasErr ,2) );


  //%%%%%%%%%%%%%%%%%%%%  Write output %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  filename = Form("results/fakeBkg_metCutEq%.0f_ptGt%.0f_Le%.0f_ECaloCutEq%.0f_IasGt0p%02.0f_Le0p%02.0f.root",metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin*100,iasCutMax*100);
  TFile *out = new TFile(filename,"RECREATE");
  TH1D *histoBkg = new TH1D("fakes","fakes",1,0,1);
  histoBkg->Fill("CR1",0);
  histoBkg->Fill("CR2",0);
  histoBkg->Fill("CR3",0);
  histoBkg->Fill("SR",0);

  bin = histoBkg->GetXaxis()->FindBin("CR1");
  histoBkg->SetBinContent(bin,nCR1);
  histoBkg->SetBinError(bin,nCR1Err);
  bin = histoBkg->GetXaxis()->FindBin("CR2");
  histoBkg->SetBinContent(bin,nCR2);
  histoBkg->SetBinError(bin,nCR2Err);
  bin = histoBkg->GetXaxis()->FindBin("CR3");
  histoBkg->SetBinContent(bin,nCR3);
  histoBkg->SetBinError(bin,nCR3Err);
  bin = histoBkg->GetXaxis()->FindBin("SR");
  histoBkg->SetBinContent(bin,nSR);
  histoBkg->SetBinError(bin,nSRErr);

  histoBkg->Write();
  out->Close();

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  cout<<"nCR1 = "<<nCR1<<" +/- "<<nCR1Err<<endl;
  cout<<"nCR2 = "<<nCR2<<" +/- "<<nCR2Err<<endl;
  cout<<"nCR3 = "<<nCR3<<" +/- "<<nCR3Err<<endl;
  cout<<"nSR  = "<<nSR<<" +/- "<<nSRErr<<endl;
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  return 0;
}

//################################################################################################################################
//################################################################################################################################
//################################################################################################################################
