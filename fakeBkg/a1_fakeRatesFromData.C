#include "TROOT.h"
#include "TFile.h"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLorentzVector.h"
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
#include "TAxis.h"
#include "TGaxis.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include "../plotStyle.h"
#include "Classes.h"
#include "../inputAnalysisFiles.h"


TCanvas *drawRatioPlot(TH1D *prediction, TH1D *sr, TString xTitle, TString filename);
TCanvas *drawRatioPlotWithPurity(TH1D *prediction, TH1D *sr, TString xTitle, TString filename, double purity, bool LowECaloRegion);
void getPoissonIntervalls(double mu1, double mu2);

TLorentzVector lorentzVector(float pt, float eta, float phi, float energy){
  
  TLorentzVector v4;
  v4.SetPtEtaPhiM(pt,eta,phi,energy);
  
  return v4;
};



int a1_fakeRatesFromData(double ptCutMin, double ptCutMax, double ecaloCut){

  sample muFakeControlRegion;
  sample muControlRegion;
  sample eleFakeControlRegion;
  sample eleControlRegion;
  muFakeControlRegion.file  = new TFile(fileSingleMu,"READ");
  eleFakeControlRegion.file = new TFile(fileSingleElectron,"READ");
  muFakeControlRegion.file  -> GetObject("chiTrackspreselectionTightMuons/Variables",muFakeControlRegion.tree);
  muFakeControlRegion.file  -> GetObject("chiTracksnoSelectionTightMuons/Variables",muControlRegion.tree);
  eleFakeControlRegion.file -> GetObject("chiTrackspreselectionTightElectrons/Variables",eleFakeControlRegion.tree);
  eleFakeControlRegion.file -> GetObject("chiTracksnoSelectionTightElectrons/Variables",eleControlRegion.tree);

  muFakeControlRegion.getTreeVariables();
  muControlRegion.getTreeVariables();
  eleFakeControlRegion.getTreeVariables();
  eleControlRegion.getTreeVariables();

  muFakeControlRegion.histo  = new TH1D("muFakeControlRegion","muFakeControlRegion",1,0,1);
  muControlRegion.histo      = new TH1D("muControlRegion","muControlRegion",1,0,1);
  eleFakeControlRegion.histo = new TH1D("eleFakeControlRegion","eleFakeControlRegion",1,0,1);
  eleControlRegion.histo     = new TH1D("eleControlRegion","eleControlRegion",1,0,1);

  cout<<"Entries SingleMu Control region  = "<<muControlRegion.tree->GetEntries()<<endl;
  cout<<"Entries SingleEle Control region = "<<eleControlRegion.tree->GetEntries()<<endl;

  double muControlRegionInt     = muControlRegion.tree->GetEntries();
  double muControlRegionIntErr  = sqrt(muControlRegion.tree->GetEntries());
  double eleControlRegionInt    = eleControlRegion.tree->GetEntries();
  double eleControlRegionIntErr = sqrt(eleControlRegion.tree->GetEntries());

  double ControlRegionInt       = muControlRegionInt + eleControlRegionInt;
  double ControlRegionIntErr    = sqrt(pow(muControlRegionIntErr,2)  +  pow( eleControlRegionIntErr,2));

  std::map <TString, double> fakeRate;
  std::map <TString, double> fakeRateError;

  TGaxis::SetMaxDigits(3);
  TH1D* histoFakeRates = new TH1D("fakeRates","fake rates",1,0,1);
  TFile *out = new TFile("templates/fakeRatesDATA.root","RECREATE");
  

  muFakeControlRegion.Selection( 0 , 0 , true , ptCutMin , ptCutMax , true , true, ecaloCut);
  eleFakeControlRegion.Selection(0 , 0 , true , ptCutMin , ptCutMax , true , true, ecaloCut);
  
  double muFakeControlRegionIntErr  = 0;
  double muFakeControlRegionInt     = muFakeControlRegion.histo->IntegralAndError(1,muFakeControlRegion.histo->GetNbinsX(),muFakeControlRegionIntErr);
  double eleFakeControlRegionIntErr = 0;
  double eleFakeControlRegionInt    = eleFakeControlRegion.histo->IntegralAndError(1,eleFakeControlRegion.histo->GetNbinsX(),eleFakeControlRegionIntErr);
  
  TString key = Form("%.0f-%.0f_%.0f",ptCutMin,ptCutMax,ecaloCut);
  cout<<"key = "<<key<<endl;

  double FakeControlRegionInt    = muFakeControlRegionInt + eleFakeControlRegionInt;
  double FakeControlRegionIntErr = sqrt(pow(muFakeControlRegionIntErr,2)  +  pow( eleFakeControlRegionIntErr,2));
    
  fakeRate[key]      = FakeControlRegionInt/ControlRegionInt;
  fakeRateError[key] = fakeRate[key]*sqrt(pow(FakeControlRegionIntErr/FakeControlRegionInt,2) + pow(ControlRegionIntErr/ControlRegionInt,2));

  cout<<"###########################################################################"<<endl;
   cout.precision(3);
  cout<<"Fake rates in muon and electron sample:"<<endl;
  double muFakeRate      = muFakeControlRegionInt/muControlRegionInt;
  double muFakeRateError = muFakeRate * sqrt( pow(muFakeControlRegionIntErr/muFakeControlRegionInt,2) + pow(muControlRegionIntErr/muControlRegionInt,2) ) ;
  cout<<"Fake rate (from muon sample)         = "<<muFakeRate<<" +/- "<<muFakeRateError<<endl;
  cout<<"With: muFakeControlRegionInt = "<<muFakeControlRegionInt<<"   and  muControlRegionInt = "<<muControlRegionInt<<endl;
  double eleFakeRate      = eleFakeControlRegionInt/eleControlRegionInt;
  double eleFakeRateError = eleFakeRate * sqrt( pow(eleFakeControlRegionIntErr/eleFakeControlRegionInt,2) + pow(eleControlRegionIntErr/eleControlRegionInt,2) ) ;
  cout<<"With: eleFakeControlRegionInt = "<<eleFakeControlRegionInt<<"   and  eleControlRegionInt = "<<eleControlRegionInt<<endl;
  cout<<"Fake rate (from electron sample)     = "<<eleFakeRate<<" +/- "<<eleFakeRateError<<endl;
  cout<<"###########################################################################"<<endl;
  cout<<"Fake control region       = "<<FakeControlRegionInt<<" +/- "<<FakeControlRegionIntErr<<endl;
  cout<<"Control region            = "<<ControlRegionInt<<" +/- "<<ControlRegionIntErr<<endl;
  cout<<"fake rate                 = "<<fakeRate[key]<<" +/- "<<fakeRateError[key]<<endl;
  cout<<"###########################################################################"<<endl;

  histoFakeRates->Fill(key,0);
  int binNumber = histoFakeRates->GetXaxis()->FindBin(key);
  histoFakeRates->SetBinContent(binNumber,fakeRate[key]);
  histoFakeRates->SetBinError(binNumber,fakeRateError[key]);

  muFakeControlRegion.histo->Reset();
  eleFakeControlRegion.histo->Reset();
   
  out->cd();
  histoFakeRates->Write();
  out->Close();
 
  //TCanvas *c1 = drawRatioPlotWithPurity(muFakeControlRegion.histo,muFakeControlRegion.histo,"track Pdg Id","trackPdgId_cr.pdf",1,1);
    
  return 0;
}

//################################################################################################################################
//################################################################################################################################
//################################################################################################################################
TCanvas *drawRatioPlot(TH1D *prediction, TH1D *sr, TString xTitle, TString filename){

  
  TCanvas *c = new TCanvas("c"+filename,"c",0,0,500,500);
  c->cd();
  float y = 0.3;

  TPad *pad1     = new TPad("pad1", "Control Plots 1", 0.01, y, 0.99, 0.99);
  TPad *padRatio = new TPad("rp1", "Ratio1", 0.01, 0.01, 0.99, y-0.01);

  TH1D *ratio = 0;

  ratio = (TH1D*) sr->Clone();
  ratio->Divide(prediction);
  ratio->GetYaxis()->SetTitle((TString) sr->GetName() + "/" + (TString) prediction->GetName());
  ratio->SetTitle("");
  ratio->SetLabelSize(0.1,"X");
  ratio->SetLabelSize(0.1,"Y");
  ratio->SetTitleOffset(0.5,"Y");
  ratio->SetTitleSize(0.15,"Y");


  padRatio->cd();
  ratio->GetYaxis()->SetRangeUser(0,2);
  ratio->Draw();

  // Draw line at one!
  float xmin = ratio->GetXaxis()->GetXmin();
  float xmax = ratio->GetXaxis()->GetXmax();
  TLine *line = new TLine(xmin,1,xmax,1);
  line->SetLineWidth(2);
  line->Draw("same");
  padRatio->Modified();

  TLegend *leg = new TLegend(0.6,0.8,0.9,0.9);
  leg->AddEntry(sr,"bkg","l"); 
  leg->AddEntry(prediction,prediction->GetName(),"pel"); 
  pad1->cd();
  //pad1->SetLogy();

  sr->SetLineColor(kRed);
  sr->SetMarkerColor(kRed);
  sr->SetTitle("");
  prediction->SetTitle("");
  prediction->GetXaxis()->SetTitle(xTitle);
  sr->GetXaxis()->SetTitle(xTitle);

  prediction->SetTitleSize(0.07,"X");
  prediction->GetXaxis()->SetTitleOffset(0.7);
  sr->SetTitleSize(0.07,"X");
  sr->GetXaxis()->SetTitleOffset(0.7);

  prediction->Draw("e");
  sr->Draw("e same");

  if(sr->GetMaximum()>prediction->GetMaximum()){

    sr->Draw("e");
    prediction->Draw("e same");

    }

  leg->Draw("same");




  // Draw both pads to canvas
  c->cd();
  pad1->Draw();
  padRatio->SetGridy();
  padRatio->Draw();

  c->SaveAs(filename);

  return c;

}

//################################################################################################################################
 // Get the 68% confindence interval of mu1/mu2
void getPoissonIntervalls(double mu1, double mu2){

  double result = mu1/mu2;
  TH1D *expHist = new TH1D("mu1/mu2","mu1/mu2",1000,0,1);
  TRandom3 rand1(0);
  TRandom3 rand2(0);

  cout<<"mu1 = "<<mu1<<endl;
  cout<<"mu2 = "<<mu2<<endl;
  cout<<"ratio = "<<result<<endl;

  for(int i=0; i<10000000; i++){

    expHist->Fill(1.*rand1.Poisson(mu1)/rand2.Poisson(mu2));
  }

  // Get now the 68% upper and lower interval
  double errUp  = 0;
  double errLow = 0;
  for(int i=1; i<=expHist->GetNbinsX();i++){

    double upperIntegral = expHist->Integral(i,expHist->GetNbinsX());
    double lowerIntegral = expHist->Integral(1,i);

    if(abs(upperIntegral/expHist->Integral()-0.32)<0.02){
      cout<<"upper bound = "<<expHist->GetBinCenter(i)<<endl;
      errUp = expHist->GetBinCenter(i)-result;
    }
    if(abs(lowerIntegral/expHist->Integral()-0.32)<0.02){
      cout<<"lower bound = "<<expHist->GetBinCenter(i)<<endl;
      errLow = result - expHist->GetBinCenter(i);
    }
  }


  cout<<"error Up = "<<errUp<<endl;
  cout<<"error Low = "<<errLow<<endl;

  TCanvas *c = new TCanvas("cExp","cExp",0,0,500,500);
  c->cd();
  expHist->Draw();
}    
//################################################################################################################################
TCanvas *drawRatioPlotWithPurity(TH1D *prediction, TH1D *sr, TString xTitle, TString filename, double purity, bool LowECaloRegion){
  
  TCanvas *c = new TCanvas("c"+filename,"c",0,0,500,500);
  c->cd();
  float y = 0.3;

  TPad *pad1     = new TPad("pad1", "Control Plots 1", 0.01, y, 0.99, 0.99);
  TPad *padRatio = new TPad("rp1", "Ratio1", 0.01, 0.01, 0.99, y-0.01);

  TH1D *ratio = 0;

  ratio = (TH1D*) sr->Clone();
  ratio->Divide(prediction);
  ratio->GetYaxis()->SetTitle((TString) sr->GetName() + "/" + (TString) prediction->GetName());
  ratio->SetTitle("");
  ratio->SetLabelSize(0.1,"X");
  ratio->SetLabelSize(0.1,"Y");
  ratio->SetTitleOffset(0.5,"Y");
  ratio->SetTitleSize(0.11,"Y");
  ratio->SetLineColor(kBlack);
  ratio->SetMarkerColor(kBlack);


  padRatio->cd();
  ratio->GetYaxis()->SetRangeUser(0,2);
  ratio->Draw();

  // Draw line at one!
  float xmin = ratio->GetXaxis()->GetXmin();
  float xmax = ratio->GetXaxis()->GetXmax();
  TLine *line = new TLine(xmin,1,xmax,1);
  line->SetLineWidth(2);
  line->Draw("same");
  padRatio->Modified();

  TLegend *leg = new TLegend(0.5,0.8,0.85,0.9);
  leg->AddEntry(sr,sr->GetName(),"l"); 
  leg->AddEntry(prediction,prediction->GetName(),"pel"); 
  pad1->cd();
  //pad1->SetLogy();

  sr->SetLineColor(kRed);
  sr->SetMarkerColor(kRed);
  prediction->SetLineColor(kBlack);
  prediction->SetMarkerColor(kBlack);
  sr->SetTitle("");
  prediction->SetTitle("");
  prediction->GetXaxis()->SetTitle(xTitle);
  sr->GetXaxis()->SetTitle(xTitle);

  prediction->SetTitleSize(0.07,"X");
  prediction->GetXaxis()->SetTitleOffset(0.7);
  sr->SetTitleSize(0.07,"X");
  sr->GetXaxis()->SetTitleOffset(0.7);

  prediction->Draw("e");
  sr->Draw("e same");

  double maximum =prediction->GetMaximum()*1.2;
  double minimum = 0.00001;
  if(sr->GetMinimum()!=0) minimum=sr->GetMinimum()*0.9;
  if(prediction->GetMinimum()!=0) minimum=prediction->GetMinimum()*0.9;
  if(sr->GetMaximum()>prediction->GetMaximum()){

    maximum=sr->GetMaximum()*1.2;
  }



  //leg->Draw("same");


  //***********************
  TLatex*  info   = new TLatex();
  info->SetTextFont(132);
  info-> SetNDC();
  info->SetTextSize(0.06);
  TString AuxString = Form("Purity of fakes in CR = %4.1f ",purity*100);
  //info->DrawLatex(0.4,0.7,AuxString);

  TLatex*  info1   = new TLatex();
  info1->SetTextFont(132);
  info1-> SetNDC();
  info1->SetTextSize(0.06);
  if(LowECaloRegion) AuxString = "E_{calo}<10GeV";
  else               AuxString = "E_{calo}>10GeV"; 
  //info1->DrawLatex(0.2,0.82,AuxString);
  //***********************

  // Draw both pads to canvas
  c->cd();
  pad1->Draw();
  padRatio->SetGridy();
  c -> SetBottomMargin(0.55);
  //c->Modified();
  padRatio->Draw();

  c->SaveAs(filename);

  return c;

}
