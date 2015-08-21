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
#include "TGraphAsymmErrors.h"

TCanvas *drawRatioPlot(TH1D *prediction, TH1D *sr, TString xTitle, TString filename);

int a1_getScalingFactors(int pdgId, double metCut, double jetptCut, bool doEcaloCut, double ptCutMin, double ptCutMax, double ecaloCut, TString region){

  TFile *out = new TFile(Form("templates/leptonScalingFactors_pdgId_%i_metCut_%.f_jetptCut_%.0f_ptCut_%.0f-%.0f",pdgId,metCut,jetptCut,ptCutMin,ptCutMax) + region + ".root","RECREATE");

  sample pred("wjets");
  sample SR("wjets");


  TString selection = "chiTrackspreselectionTriggerNoQCDCuts";
  SR.getTree(fileSR, selection);
  if(pdgId==211)        pred.getTree(filePionCS, selection);
  else if(pdgId==11)    pred.getTree(fileElectronCS, selection);
  else if(pdgId==13)    pred.getTree(fileMuonCS, selection);

  /*
 
  pred.file -> GetObject(selectionType,pred.tree);
  SR.file   -> GetObject(selectionType,SR.tree);
  */

  SR.getTreeVariables();
  pred.getTreeVariables();

  int nbins=5;
  double xmin=0;
  double xmax=10;
  SR.histo   = new TH1D("SR","SR",nbins,xmin,xmax);
  pred.histo = new TH1D("pred","pred",nbins,xmin,xmax);

  SR.histo->Sumw2();
  pred.histo->Sumw2();


  double scalingFactor;
  double scalingFactorErrorUp;
  double scalingFactorErrorLow;
  
  if(pdgId==211 || pdgId==13){
    SR.Selection(   1, pdgId, metCut, jetptCut, 20., 50000., 0. ,1. , 1,          ecaloCut, 1, region);
    pred.Selection( 0, pdgId, metCut, jetptCut, 20., 50000., 0. ,1. , doEcaloCut, ecaloCut, 1, region);
  }
  else{
    SR.Selection(   1, pdgId, metCut, jetptCut, ptCutMin, ptCutMax, 0., 1., 1,          ecaloCut, 1, region);
    pred.Selection( 0, pdgId, metCut, jetptCut, ptCutMin, ptCutMax, 0., 1., doEcaloCut, ecaloCut, 1, region);
  }

  
  double predIntError   =  0;
  double predInt        =  pred.histo->IntegralAndError(1,pred.histo->GetNbinsX(),predIntError);
  double SRInt          =  SR.histo->Integral(1,SR.histo->GetNbinsX()+1);
  double SRIntErrorUp   =  getOneSidedUpperLimit(SRInt,0.6827) -SRInt;
  double SRIntErrorLow  =  SRInt - getOneSidedLowerLimit(SRInt,0.6827);
      

  if(SRInt==0){
    /*
    SRInt        = getOneSidedUpperLimit(SRInt,0.6827) -SRInt;
    SRIntErrorUp   = 0.;
    SRIntErrorLow  = 0.;
    predIntError   = 0.;
    */

    SRIntErrorUp   = getOneSidedUpperLimit(SRInt,0.6827) -SRInt;
    SRIntErrorLow  = getOneSidedLowerLimit(SRInt,0.6827) -SRInt;

  }
  scalingFactor         = SRInt/predInt;
  scalingFactorErrorUp  = SRIntErrorUp/predInt;
  scalingFactorErrorLow = SRIntErrorLow/predInt;
  if(predInt==0){
    scalingFactor         = -1;
    scalingFactorErrorUp  = 0;
    scalingFactorErrorLow = 0;
  }

  cout<<"###########################################################################"<<endl;
  cout.precision(2);
  cout<<"Statisitcs for "<<pdgId<<" : "<<"pt="<<ptCutMin<<" - "<<ptCutMax<<" ECalo="<<ecaloCut<<endl;
  cout<<"Signal region          = "<<SR.statistics<<endl;
  cout<<"Control region         = "<<pred.statistics<<endl;
  cout<<"Purity in CR           = "<<fixed<<pred.purity*100<<"%"<<endl;
  cout<<"scaling factor         = "<<scientific<<scalingFactor<<" + "<<scalingFactorErrorUp<<" - "<<scalingFactorErrorLow<<endl;
  cout<<"###########################################################################"<<endl;

  TString name = Form("%.0f-%.0f_%.0f",ptCutMin,ptCutMax,ecaloCut);

  double x[1]    = {0.5};
  double xLow[1] = {0};
  double xUp[1]  = {0};

  double y[1]    = {scalingFactor};
  double yLow[1] = {scalingFactorErrorLow};
  double yUp[1]  = {scalingFactorErrorUp};

  TGraphAsymmErrors* GScalingFactors = new TGraphAsymmErrors(1,x,y,xLow,xUp,yLow,yUp);
  GScalingFactors -> SetMarkerStyle(20);

  out->cd();
  GScalingFactors->Write(name);
  out->Close();

  gDirectory->Delete(); 
 
  return 0;
 
}

//################################################################################################################################
//################################################################################################################################
//################################################################################################################################


TCanvas *drawRatioPlot(TH1D *prediction, TH1D *sr, TString xTitle, TString filename){

  
  TCanvas *c = new TCanvas("c"+filename,"c",0,0,500,500);

  float y = 0.3;

  TPad *pad1     = new TPad("pad1", "Control Plots 1", 0.01, y, 0.99, 0.99);
  TPad *padRatio = new TPad("rp1", "Ratio1", 0.01, 0.01, 0.99, y-0.01);

  TH1D *ratio = 0;

  //ratio = (TH1D*) sr->Clone();
  //ratio->Divide(prediction);

  ratio = (TH1D*) prediction->Clone();
  ratio->Divide(sr);


  for(int i=1; i<=ratio->GetNbinsX();i++){

    cout<<"Scaling Factor in "<<i<<". bin ="<<ratio->GetBinContent(i)<<" +/- "<<ratio->GetBinError(i)<<endl; 

  }

  ratio->GetYaxis()->SetTitle("Pred/Bkg");
  ratio->SetTitle("");
  ratio->SetLabelSize(0.1,"X");
  ratio->SetLabelSize(0.1,"Y");
  ratio->SetTitleOffset(0.5,"Y");
  ratio->SetTitleSize(0.15,"Y");


  padRatio->cd();
  //ratio->GetYaxis()->SetRangeUser(0,2);
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
  leg->AddEntry(prediction,"bkg prediction","pel"); 
  pad1->cd();
  pad1->SetLogy();

  sr->SetLineColor(kRed);
  sr->SetMarkerColor(kRed);
  sr->SetMarkerStyle(20);
  sr->SetTitle("");
  prediction->SetMarkerStyle(20);
  prediction->SetTitle("");
  prediction->GetXaxis()->SetTitle(xTitle);
  sr->GetXaxis()->SetTitle(xTitle);

  prediction->SetTitleSize(0.07,"X");
  prediction->GetXaxis()->SetTitleOffset(0.7);
  sr->SetTitleSize(0.07,"X");
  sr->GetXaxis()->SetTitleOffset(0.7);

  prediction->Draw("e");
  sr->Draw("e same");

  double maximum =prediction->GetMaximum()*2.5;
  double minimum = 0.00001;
  if(sr->GetMinimum()!=0) minimum=sr->GetMinimum()*0.5;
  if(prediction->GetMinimum()!=0) minimum=prediction->GetMinimum()*0.5;
  if(sr->GetMaximum()>prediction->GetMaximum()){

    maximum=sr->GetMaximum()*2.5;
  }

  prediction->GetYaxis()->SetRangeUser(minimum,maximum);

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
//################################################################################################################################
