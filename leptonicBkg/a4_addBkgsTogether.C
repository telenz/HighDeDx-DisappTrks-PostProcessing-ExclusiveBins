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
#include "TGraphAsymmErrors.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>


int a4_addBkgsTogether(double metCut, double ptCutMin, double ptCutMax, double ecaloCut, double iasCutMin, double iasCutMax, TString region){

  ofstream outputFile("thesisOutput.txt");

  TFile *fPions     = new TFile(Form("results/Bkg211_metCutEq%.0f_ptGt%0.f_Le%0.f_ECaloCutEq%.0f_IasGt0p%02.0f_Le0p%02.0f_",metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin*100,iasCutMax*100) + region + ".root", "READ");
  TFile *fElectrons = new TFile(Form("results/Bkg11_metCutEq%.0f_ptGt%0.f_Le%0.f_ECaloCutEq%.0f_IasGt0p%02.0f_Le0p%02.0f_",metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin*100,iasCutMax*100) + region + ".root","READ");
  TFile *fMuons     = new TFile(Form("results/Bkg13_metCutEq%.0f_ptGt%0.f_Le%0.f_ECaloCutEq%.0f_IasGt0p%02.0f_Le0p%02.0f_",metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin*100,iasCutMax*100) + region + ".root","READ");
  TFile *fFakes     = new TFile(Form("../fakeBkg/results/fakeBkg_metCutEq%.0f_ptGt%0.f_Le%0.f_ECaloCutEq%.0f_IasGt0p%02.0f_Le0p%02.0f.root",metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin*100,iasCutMax*100),"READ");
  TFile *fData      = new TFile(Form("results/DataYield_metCutEq%.0f_ptGt%0.f_Le%0.f_ECaloCutEq%.0f_IasGt0p%02.0f_Le0p%02.0f_",metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin*100,iasCutMax*100) + region + ".root","READ");

  TGraphAsymmErrors * gPions     = 0;
  TGraphAsymmErrors * gElectrons = 0;
  TGraphAsymmErrors * gMuons     = 0;
  TH1D* hFakes     = 0;
  TH1D* hData      = 0;

  fPions     -> GetObject("pions", gPions);
  fElectrons -> GetObject("electrons", gElectrons);
  fMuons     -> GetObject("muons", gMuons);
  fFakes     -> GetObject("fakes",hFakes);
  fData      -> GetObject("data",hData);

  cout.precision(3);
  outputFile.precision(2);
  double nAll         = 0;
  double nAllErrorUp  = 0;
  double nAllErrorLow = 0;
  double nLeptons         = 0;
  double nLeptonsErrorUp  = 0;
  double nLeptonsErrorLow = 0;
  double nFakes         = 0;
  double nFakesErrorUp  = 0;
  double nFakesErrorLow = 0;
  double n, nErrorUp, nErrorLow;
  double aux;

  TString eq = "=";

  gElectrons              -> GetPoint(0,aux,n);
  nErrorUp  = gElectrons  -> GetErrorYhigh(0);
  nErrorLow = gElectrons  -> GetErrorYlow(0);
  if(nErrorUp==0) eq = "<";
  else            eq = "=";
  cout<<fixed<<"Electron bkg     "<<eq<<" "<<n<<" + "<<nErrorUp<<" - "<<nErrorLow<<"\\\\"<<endl;
  outputFile<<fixed<<setw(10)<<"Electrons"<<setw(10)<<" &$"<<setw(3)<<n<<"^{+"<<setw(3)<<nErrorUp<<"}_{-"<<setw(3)<<nErrorLow<<"}$"<<setw(20)<<"  & \\\\"<<endl;
  nAll         += n ; 
  nAllErrorUp  += pow(nErrorUp,2);
  nAllErrorLow += pow(nErrorLow,2);
  nLeptons         += n ; 
  nLeptonsErrorUp  += pow(nErrorUp,2);
  nLeptonsErrorLow += pow(nErrorLow,2);

  gMuons              -> GetPoint(0,aux,n);
  nErrorUp  = gMuons  -> GetErrorYhigh(0);
  nErrorLow = gMuons  -> GetErrorYlow(0);
  if(nErrorUp==0) eq = "<";
  else            eq = "=";
  cout<<fixed<<"Muon bkg         "<<eq<<" "<<n<<" + "<<nErrorUp<<" - "<<nErrorLow<<"\\\\"<<endl;
  outputFile<<fixed<<setw(10)<<"Muons"<<setw(10)<<" &$"<<setw(3)<<n<<"^{+"<<setw(3)<<nErrorUp<<"}_{-"<<setw(3)<<nErrorLow<<"}$"<<setw(20)<<"  & \\\\"<<endl;
  nAll         += n ; 
  nAllErrorUp  += pow(nErrorUp,2);
  nAllErrorLow += pow(nErrorLow,2);
  nLeptons         += n ; 
  nLeptonsErrorUp  += pow(nErrorUp,2);
  nLeptonsErrorLow += pow(nErrorLow,2);

  gPions              -> GetPoint(0,aux,n);
  nErrorUp  = gPions  -> GetErrorYhigh(0);
  nErrorLow = gPions  -> GetErrorYlow(0);
  if(nErrorUp==0) eq = "<";
  else            eq = "=";
  cout<<fixed<<"Pion bkg         "<<eq<<" "<<n<<" + "<<nErrorUp<<" - "<<nErrorLow<<"\\\\"<<endl;
  outputFile<<fixed<<setw(10)<<"Taus"<<setw(10)<<" &$"<<setw(3)<<n<<"^{+"<<setw(3)<<nErrorUp<<"}_{-"<<setw(3)<<nErrorLow<<"}$"<<setw(20)<<"  & \\\\"<<endl;
  nAll         += n ; 
  nAllErrorUp  += pow(nErrorUp,2);
  nAllErrorLow += pow(nErrorLow,2);
  nLeptons         += n ; 
  nLeptonsErrorUp  += pow(nErrorUp,2);
  nLeptonsErrorLow += pow(nErrorLow,2);

  int bin         = hFakes->GetXaxis()->FindBin(region);
  n = hFakes->IntegralAndError(bin,bin,nErrorUp);
  nErrorLow = nErrorUp;
  if(nErrorUp==0) eq = "<";
  else            eq = "=";
  cout<<"Fake bkg         "<<eq<<" "<<n<<" + "<<nErrorUp<<" - "<<nErrorUp<<"\\\\"<<endl;
  outputFile<<fixed<<setw(10)<<"Fakes"<<setw(10)<<" &$"<<setw(3)<<n<<"^{+"<<setw(3)<<nErrorUp<<"}_{-"<<setw(3)<<nErrorLow<<"}$"<<setw(20)<<"  & \\\\"<<endl;
  nAll         += n ; 
  nAllErrorUp  += pow(nErrorUp,2);
  nAllErrorLow += pow(nErrorLow,2);
  nFakes         += n ; 
  nFakesErrorUp  += pow(nErrorUp,2);
  nFakesErrorLow += pow(nErrorLow,2);
  cout<<"---------------------------------------------"<<"\\\\"<<endl;


  nAllErrorUp  = sqrt(nAllErrorUp);
  nAllErrorLow = sqrt(nAllErrorLow);
  nLeptonsErrorUp  = sqrt(nLeptonsErrorUp);
  nLeptonsErrorLow = sqrt(nLeptonsErrorLow);
  nFakesErrorUp  = sqrt(nFakesErrorUp);
  nFakesErrorLow = sqrt(nFakesErrorLow);
  cout<<"Total bkg        = "<<nAll<<" + "<<nAllErrorUp<<" - "<<nAllErrorLow<<"\\\\"<<endl;
  outputFile<<fixed<<setw(10)<<"Total bkg"<<setw(10)<<" &$"<<setw(3)<<nAll<<"^{+"<<setw(3)<<nAllErrorUp<<"}_{-"<<setw(3)<<nAllErrorLow<<"}$"<<setw(20);

  //*********** save total bkg in histogram ***********
  TH1D* total  = new TH1D("totalBkg","total bkg",1,0,1);
  TH1D* lepton = new TH1D("leptonBkg","total bkg",1,0,1);
  TH1D* fake   = new TH1D("fakeBkg","total bkg",1,0,1);
  total->Fill("SR",0);
  total->SetBinContent(1,nAll);
  total->SetBinError(1,nAllErrorUp);
  lepton->Fill("SR",0);
  lepton->SetBinContent(1,nLeptons);
  lepton->SetBinError(1,nLeptonsErrorUp);
  fake->Fill("SR",0);
  fake->SetBinContent(1,nFakes);
  fake->SetBinError(1,nFakesErrorUp);

  TFile *out = new TFile(Form("results/TotalBkg_metCutEq%.0f_ptGt%0.f_Le%.0f_ECaloCutEq%.0f_IasGt0p%02.0f_Le0p%02.0f_",metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin*100,iasCutMax*100) + region + ".root","RECREATE");
  total  -> Write();
  lepton -> Write();
  fake   -> Write();
  out->Close();

  //if(region=="SR") return 0;

  n = hData->Integral();
  cout.precision(0);
  outputFile.precision(0);
  cout<<endl<<"Data Yield = "<<n<<"\\\\"<<endl<<endl;
  outputFile<<"  &"<<n<<" \\\\"<<endl;


  return 0;
 
}

//################################################################################################################################
//################################################################################################################################
