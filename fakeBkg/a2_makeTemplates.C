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
#include "../inputAnalysisFiles.h"


TCanvas *drawRatioPlot(TH1D *prediction, TH1D *sr, TString xTitle, TString filename);
TCanvas *drawRatioPlotWithPurity(TH1D *prediction, TH1D *sr, TString xTitle, TString filename, double purity, bool LowECaloRegion);
void getPoissonIntervalls(double mu1, double mu2);

int makeIasTemplates(double metCut, double jetptCut, double ptCutMin, double ptCutMax, double ecaloCut, double iasCutMin, double iasCutMax=0.3){

  sample data;

  data.file = new TFile(fileFakeCS + "data.root","READ");
  
  data.file   -> GetObject("chiTrackspreselection/Variables",data.tree);
  data.getTreeVariables();

  TString filename = Form("templates/IasTemplate_METGt%.0f_JetPtGt%.0f_trackPtGt%.0f_Le%.0f_ECaloEq%.0f.root",metCut,jetptCut,ptCutMin,ptCutMax,ecaloCut);
  TFile *out  = new TFile(filename,"RECREATE");
  
  bool LowECaloRegion=true;
  
  for(int j=0; j<2; j++){

    if(j==0) LowECaloRegion=false;
    else     LowECaloRegion=true;
    LowECaloRegion=true;
    Double_t xbinsASmi[4]  = {0,iasCutMin,iasCutMax,1};

    TString histoName = (TString) Form("fake_bkg_data_IascutMin_0p%02.0f_IascutMax_0p%02.0f_EcaloLow_%i",iasCutMin*100,iasCutMax*100,j);
    cout<<"histoName = "<<histoName<<endl;
    data.histoASmi   = new TH1D(histoName, histoName, 3, xbinsASmi);
    data.Selection(metCut, jetptCut , true, ptCutMin, 10000, true , LowECaloRegion , ecaloCut);
    cout<<"look here:"<<data.histoASmi->GetEntries()<<endl;
      // ***** Calculate scaling factor  ************************************************************************
    data.histoASmi->Scale(1./data.histoASmi->Integral());
    // ***********************************************************************************************************
    
    out->cd();
    data.histoASmi->Write();
        
    delete data.histoASmi;
    
  }

  out->Close();
 
  return 0;
}

int makeECaloTemplates(double metCut, double jetptCut, double ptCutMin, double ptCutMax, double ecaloCut){

  sample data;
  data.file = new TFile(fileFakeCS + "/data.root");
  data.file = new TFile(fileFakeCS + "data.root");

  data.file   -> GetObject("chiTrackspreselection/Variables",data.tree);
  data.getTreeVariables();

  TString filename = Form("templates/ECaloTemplate_METGt%.0f_JetPtGt%.0f_trackPtGt%.0f_Le%.0f.root",metCut,jetptCut,ptCutMin,ptCutMax);
  TFile *out = new TFile(filename,"RECREATE");
  
  Double_t xbinsECalo[3]  = {0,ecaloCut,1000.};
  TString histoName = (TString) Form("fake_bkg_data_ECaloCut_%.0f",ecaloCut);
  data.histoECalo   = new TH1D(histoName, histoName, 2, xbinsECalo);
 
  
  data.Selection(metCut , jetptCut , true , ptCutMin , ptCutMax , false, 0, 0);

  // ***** Calculate scaling factor  ************************************************************************
  data.histoECalo->Scale(1./data.histoECalo->Integral());
  // ***********************************************************************************************************
  
  out->cd();
  data.histoECalo->Write();
        
  delete data.histoECalo;
  
  out->Close();
 
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
  pad1->SetLogy();

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
  pad1->SetLogy();

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



  leg->Draw("same");


  //***********************
  TLatex*  info   = new TLatex();
  info->SetTextFont(132);
  info-> SetNDC();
  info->SetTextSize(0.06);
  TString AuxString = Form("Purity of fakes in CR = %4.1f ",purity*100);
  info->DrawLatex(0.4,0.7,AuxString);

  TLatex*  info1   = new TLatex();
  info1->SetTextFont(132);
  info1-> SetNDC();
  info1->SetTextSize(0.06);
  if(LowECaloRegion) AuxString = "E_{calo}<10GeV";
  else               AuxString = "E_{calo}>10GeV"; 
  info1->DrawLatex(0.2,0.82,AuxString);
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
