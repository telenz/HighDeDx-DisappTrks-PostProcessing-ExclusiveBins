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
#include "Classes.h"


int a3_getLeptonPrediction(int pdgId, double metCut, double jetptCut, double ptCutMin, double ptCutMax, double iasCutMin, double iasCutMax, bool doEcaloCut, double ecaloCut, TString region){

  gDirectory->Delete(); 


  cout<<endl<<"Get leptonic bkg prediction in "<<region<<"."<<endl;
  // %%%%%%%%%%%%%%%%%%%   Get correct scaling factors %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  TFile *in = new TFile(Form("templates/leptonScalingFactors_pdgId_%i_metCut_%.f_jetptCut_%.0f_ptCut_%.0f-%.0f",pdgId,metCut,jetptCut,ptCutMin,ptCutMax) + region + ".root","READ");
  TGraphAsymmErrors *GScalingFactors = 0;
  TString name                       = Form("%.0f-%.0f_%.0f",ptCutMin,ptCutMax,ecaloCut);
  in                                 -> GetObject(name,GScalingFactors);

  if(GScalingFactors==0){
    cout<<"No Graph available!"<<endl;
    return 1;
  }

  double aux;
  double scalingFactor;
  GScalingFactors->GetPoint(0,aux,scalingFactor);
  double scalingFactorErrorUp  = GScalingFactors->GetErrorYhigh(0);
  double scalingFactorErrorLow = GScalingFactors->GetErrorYlow(0);
  cout.precision(3);
  cout<<"scaling factor         = "<<scientific<<scalingFactor<<" + "<<scalingFactorErrorUp<<" - "<<scalingFactorErrorLow<<endl;
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


  // %%%%%%%%%%%%%%%%%%%   Get number of Events in CR_lepton from data %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  sample data("data");
  sample sr("data");

  sr.getTree(fileSR);
  if(pdgId==211)        data.getTree(filePionCS);
  else if(pdgId==11)    data.getTree(fileElectronCS);
  else if(pdgId==13)    data.getTree(fileMuonCS);


  data.getTreeVariables();
  sr.getTreeVariables();

  data.histo = new TH1D("data","data",1,0,1);
  data.histo->Sumw2();
  sr.histo = new TH1D("sr","sr",1,0,1);
  sr.histo->Sumw2();
  
  data.Selection(0, pdgId, metCut, jetptCut, ptCutMin, ptCutMax, 0.0      , iasCutMax, doEcaloCut, ecaloCut, 1, region);
  sr.Selection(  0, pdgId, metCut, jetptCut, ptCutMin, ptCutMax, iasCutMin, iasCutMax, 1         , ecaloCut, 0, region);


  double nSRInclusive         =  data.histo->Integral(1,data.histo->GetNbinsX()+1);
  double nSRErrorUpInclusive  =  nSRInclusive;
  double nSRErrorLowInclusive =  nSRInclusive;

  cout<<"Data Yield (CR)        = "<<fixed<<nSRInclusive<<endl;

  if(nSRInclusive==0){
    /*
    nSR         = getOneSidedUpperLimit(nSR , 0.6827) - nSR;
    nSRErrorUp  = 0;
    nSRErrorLow = 0;
    */

    nSRInclusive         = 0;
    nSRErrorUpInclusive  = getOneSidedUpperLimit(nSRInclusive , 0.6827);
    nSRErrorLowInclusive = getOneSidedLowerLimit(nSRInclusive , 0.6827);


  }
    
  nSRInclusive         =  nSRInclusive          * scalingFactor;
  nSRErrorUpInclusive  =  nSRErrorUpInclusive   * scalingFactorErrorUp;
  nSRErrorLowInclusive =  nSRErrorLowInclusive  * scalingFactorErrorLow;
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  cout<<"nSRInclusive  ("<<pdgId<<")     = "<<nSRInclusive<<" + "<<nSRErrorUpInclusive<<" - "<<nSRErrorLowInclusive<<endl;
  //cout<<"Data Yield (CR)           = "<<sr.histo->Integral()<<endl;
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  Get Ias template from MC %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  TString filename         =   Form("templates/IasTemplate_METGt%.0f_JetPtGt%.0f_ECaloEq%.0f_pdgId%i.root",metCut,jetptCut,ecaloCut,pdgId);
  cout<<"Ias template from : "<<filename<<endl;
  TFile *inIas             =   new TFile(filename,"READ");
  TH1D* histoIas           = 0;
  TString histoName        = Form("lepton_bkg_data_Iascut_0p%02.0f-0p%02.0f",iasCutMin*100,iasCutMax*100);
  cout<<"histogram         = "<<histoName<<endl; 
  inIas                   ->  GetObject(histoName,histoIas);


  double Ias    = 0;
  double IasErr = 0;

  if(region=="SR" || region=="CR1"){
    Ias       = histoIas->GetBinContent(2);
    IasErr    = histoIas->GetBinError(2);
  }
  else if(region=="CR2" || region=="CR3"){
    Ias       = histoIas->GetBinContent(1);
    IasErr    = histoIas->GetBinError(1);
  }
  

  cout<<"histoIas->GetBinContent(1) = "<<histoIas->GetBinContent(1)<<endl;
  cout<<"histoIas->GetBinContent(2) = "<<histoIas->GetBinContent(2)<<endl;


  int nEntries = histoIas->GetEntries();
  cout<<"nEntries = "<<nEntries<<endl;
  double a = nEntries/(histoIas->Integral());

  double IasErrUp  = (getOneSidedUpperLimit(a*Ias , 0.6827) - a*Ias)/a;
  double IasErrLow = (a*Ias - getOneSidedLowerLimit(a*Ias , 0.6827))/a;

  if(Ias==0){

    double up   = getOneSidedUpperLimit(0 , 0.6827);
    double low  = getOneSidedLowerLimit(0 , 0.6827);
    cout<<"Upper limit = "<<up<<endl;
    IasErrUp  = up/nEntries;
    IasErrLow = low/nEntries;
    cout<<"IasErrUp = "<<IasErrUp<<endl;

  }

  cout<<"Ias = "<<Ias<<" +/- "<<IasErr<<endl;

  double nSR          = nSRInclusive*Ias;
  double nSRErrorUp   = sqrt( pow( Ias*nSRErrorUpInclusive  ,2) + pow( nSRInclusive*IasErrUp ,2) );
  double nSRErrorLow  = sqrt( pow( Ias*nSRErrorLowInclusive ,2) + pow( nSRInclusive*IasErrLow ,2) );

  cout<<"###########################################################################"<<endl;
  cout.precision(2);
  cout<<"Statisitcs for "<<pdgId<<" : "<<"pt="<<ptCutMin<<" - "<<ptCutMax<<" ECalo="<<ecaloCut<<endl;
  cout<<"NEntries in Ias histo  = "<<histoIas->Integral()*a<<endl;
  cout<<"NEntries in high Ias = "<<Ias*a<<" +/- "<<IasErr<<endl;
  //cout<<"scaling factor         = "<<scientific<<scalingFactor<<" + "<<scalingFactorErrorUp<<" - "<<scalingFactorErrorLow<<endl;
  cout<<"###########################################################################"<<endl;

  cout<<"nSR  ("<<pdgId<<")             = "<<nSR<<" + "<<nSRErrorUp<<" - "<<nSRErrorLow<<endl;
  //%%%%%%%%%%%%%%%%%%%%  Write output %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  filename = Form("results/Bkg%i_metCutEq%.0f_ptGt%.0f_Le%.0f_ECaloCutEq%.0f_IasGt0p%02.0f_Le0p%02.0f_",pdgId,metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin*100,iasCutMax*100) + region + ".root";
  TFile *out = new TFile(filename,"RECREATE");
  
  if(pdgId==11)        name = "electrons";
  else if(pdgId==13)   name = "muons";
  else if(pdgId==211)  name = "pions";
 

  double x[1]    = {0.5};
  double xLow[1] = {0};
  double xUp[1]  = {0};
  double y[1]    = {nSR};
  double yLow[1] = {nSRErrorLow};
  double yUp[1]  = {nSRErrorUp};


  TGraphAsymmErrors* GnSR = new TGraphAsymmErrors(1,x,y,xLow,xUp,yLow,yUp);
  GnSR -> SetMarkerStyle(20);

  GnSR -> Write(name);
  out->Close();

 
  if(pdgId==211){
    filename = Form("results/DataYield_metCutEq%.0f_ptGt%.0f_Le%.0f_ECaloCutEq%.0f_IasGt0p%02.0f_Le0p%02.0f_",metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin*100,iasCutMax*100) + region + ".root";
    TFile *outData = new TFile(filename,"RECREATE");
    name="data";
    TH1D *histoData = new TH1D(name,name,1,0,1);
    histoData->Fill("CR1",0);
    histoData->Fill("CR2",0);
    histoData->Fill("CR3",0);
    histoData->Fill("SR",0);

    int bin = histoData->GetXaxis()->FindBin("SR");
    histoData->SetBinContent(bin,sr.histo->Integral());
    histoData->SetBinError(bin,sqrt(sr.histo->Integral()));

    histoData->Write();
    outData->Close();
    delete outData;
  }
  
  return 0;
 
}

//################################################################################################################################
//################################################################################################################################
