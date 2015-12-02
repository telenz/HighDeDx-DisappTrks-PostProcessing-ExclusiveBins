#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"
#include "../../../plotStyle.h"

TCanvas* plotTH2D(TH2* histo, TString xTitle, TString yTitle);
TH1D* getMomentumSlide(TH2D* histo, double ptmin, double ptmax, TString name);

void makeFit(double ptmin = 0.95, double ptmax = 1.00){

  //  gStyle->SetOptFit(1111);
  gStyle->SetOptStat("");

  TeresaPlottingStyle::init();
  gROOT->ForceStyle();


  TCanvas *c = new TCanvas("c","c",500,0,500,500);
  c->cd();

  //TString ana = "analysis_2015_07_22_withALCARECOdata";
  //TString ana = "analysis_2015_07_22_withMETdata";
  //TString ana = "analysis_2015_07_23_withVertexConditions";
  TString ana = "analysis_2015_07_24_WithVertexCondition_FinerBinning";
  //TString ana = "analysis_2015_07_23_NoHitCondition";
  //TString ana = "analysis_2015_07_23_oldMC_ALCARECO";

  TFile *fileMC   = new TFile("ANALYSES/workdir/" + ana + "/results/analyzer/ntuples/mc.root", "READ");
  TFile *fileData = new TFile("ANALYSES/workdir/" + ana + "/results/analyzer/ntuples/data.root", "READ");
  TH2D* histoIasPMC = 0;
  TH2D* histoIasPData = 0;
  fileMC    -> GetObject("hIasP",histoIasPMC);
  fileData  -> GetObject("hIasP",histoIasPData);

  TH1D* histoMC   = 0;
  TH1D* histoData = 0;
  //fileMC    -> GetObject("hIas",histoMC);
  //fileData  -> GetObject("hIas",histoData);

  // Get other momentum slides 
  histoData = getMomentumSlide(histoIasPData, ptmin, ptmax, "histoData");
  histoMC   = getMomentumSlide(histoIasPMC,   ptmin, ptmax, "histoMC");

  histoMC->Sumw2();
  histoData->Sumw2();
  // ********************************************************************************************************************************************************************************************************

  histoMC   -> SetLineColor(kRed);
  histoMC   -> SetMarkerColor(kRed);
  histoMC   -> SetMarkerSize(0.7);
  histoData -> SetMarkerSize(0.7);

  histoMC   -> Scale(1./histoMC->Integral());
  histoData -> Scale(1./histoData->Integral());

  histoMC   -> GetXaxis()->SetRangeUser(0.2,1.0);
  histoData -> GetXaxis()->SetRangeUser(0.2,1.0);
 

  double maximumMC = histoMC->GetXaxis()->GetBinCenter(histoMC->GetMaximumBin());
  cout<<"maximumMC = "<<maximumMC<<endl;
  double maximumData = histoData->GetXaxis()->GetBinCenter(histoData->GetMaximumBin());
  cout<<"maximumData = "<<maximumData<<endl;

  TF1* fMC   = new TF1("fMC","gaus",maximumMC-0.3,maximumMC+0.3);
  TF1* fData = new TF1("fData","gaus",maximumMC-0.3,maximumMC+0.3);

  fMC->SetLineColor(kRed);
  fMC->SetLineWidth(2);
  fData->SetLineWidth(2);

  //cout<<"MC:"<<endl;
  histoMC   -> Fit("fMC","RQ0");
  //cout<<endl<<"Data:"<<endl;
  histoData -> Fit("fData","RQ0");
  
  histoMC->SetTitle("");
  histoMC   -> Draw("e");
  histoMC->GetXaxis()->SetTitle("I_{as}");
  histoData -> Draw("same e");

  fMC   -> SetRange(fMC   -> GetParameter(1) - 0.2, fMC   -> GetParameter(1) +0.2);
  fData -> SetRange(fData   -> GetParameter(1) - 0.2, fData   -> GetParameter(1) +0.2);
  fMC   -> Draw("same");
  fData -> Draw("same");

  TLegend *leg = new TLegend(0.15,0.15,0.4,0.3);
  leg->AddEntry(histoData,"data","lpe"); 
  leg->AddEntry(histoMC,"mc","lpe"); 

  leg->Draw("same");

  c->SetLogy();
  c->GetPad(0)->SetTicks(0,1);


  TLatex*  info   = new TLatex();
  info->SetTextFont(132);
  info-> SetNDC();
  info->SetTextSize(0.04);
  TString AuxString = Form("%.2f GeV < pt < %.2f GeV ",ptmin,ptmax);
  info->DrawLatex(0.19,0.85,AuxString);



  // Get numbers out of the fit
  double meanData       = fData -> GetParameter(1);
  double meanDataError  = fData -> GetParError(1);
  double meanMC         = fMC   -> GetParameter(1);
  double meanMCError    = fMC   -> GetParError(1);
  double relDiffMean    = abs(meanMC/meanData-1.);

  double sigmaData      = fData -> GetParameter(2);
  double sigmaDataError = fData -> GetParError(2);
  double sigmaMC        = fMC   -> GetParameter(2);
  double sigmaMCError   = fMC   -> GetParError(2);
  double relDiffSigma   = abs(sigmaMC/sigmaData-1.);

  // Make Info box
  info->SetTextSize(0.04);
  info->DrawLatex(0.66,0.86,"Data 8TeV");
  info->DrawLatex(0.66,0.82,Form("#mu=%.2f",meanData));
  info->DrawLatex(0.66,0.78,Form("#sigma=%.2f",sigmaData));
  info->DrawLatex(0.66,0.74,"#color[2]{MC 8TeV}");
  info->DrawLatex(0.66,0.70,Form("#color[2]{#mu=%.2f}",meanMC));
  info->DrawLatex(0.66,0.66,Form("#color[2]{#sigma=%.2f}",sigmaMC));


  int ptminInt = (int) ptmin;
  int ptmaxInt = (int) ptmax;

  double max = histoMC->GetMaximum();
  histoMC->SetMaximum(100*max);

  c->SaveAs("plots/hIas_" + ana + Form("_ptmin%ip%.0f_ptmax%ip%.0f.pdf",ptminInt,(ptmin-ptminInt)*100,ptmaxInt,(ptmax-ptmaxInt)*100));


  cout<<endl;
  cout<<"###########################################################################"<<endl;
  cout.precision(3);
  cout<<"Ias of low protons in MC and data:"<<endl;
  cout<<"mean MC         = "<<meanMC<<" +/- "<<meanMCError<<endl;
  cout<<"mean Data       = "<<meanData<<" +/- "<<meanDataError<<endl;
  cout<<"-------------------------------------------------"<<endl;
  cout<<"rel diff mean (%)    = "<<relDiffMean*100<<"%"<<endl;
  cout<<endl;
  cout<<"sigma MC         = "<<sigmaMC<<" +/- "<<sigmaMCError<<endl;
  cout<<"sigma Data       = "<<sigmaData<<" +/- "<<sigmaDataError<<endl;
  cout<<"-------------------------------------------------"<<endl;
  cout<<"rel diff sigma (%)   = "<<relDiffSigma*100<<"%"<<endl;

  cout<<endl;
  cout<<"chi2/ndof data = "<<fData->GetChisquare()/fData->GetNDF()<<endl;
  cout<<"chi2/ndof MC   = "<<fMC->GetChisquare()/fMC->GetNDF()<<endl;
  cout.precision(5);
  double uncertainty = relDiffMean;
  cout<<endl<<"Final Uncertainty = "<<fixed<<uncertainty<<endl;
  cout<<"###########################################################################"<<endl;
  

  // plot other stuff

  

  /* 
  TCanvas *c1 = plotTH2D(histoIasPData,"p [GeV]","I_{as}");
  c1->SaveAs("plots/hIasP_" + ana + "_data.pdf");
  TCanvas *c2 = plotTH2D(histoIasPMC,"p [GeV]","I_{as}");
  c2->SaveAs("plots/hIasP_" + ana + "_mc.pdf");
  */
}

//####################################################################################################
TH1D* getMomentumSlide(TH2D* histo, double ptmin, double ptmax, TString name){

  TH1D* h = histo -> ProjectionY( name, histo->GetXaxis()->FindBin(ptmin), histo->GetXaxis()->FindBin(ptmax));
  
  return h;


}
//####################################################################################################

TCanvas* plotTH2D(TH2* histo, TString xTitle, TString yTitle){


  TCanvas *c = new TCanvas("c","c",500,0,500,500);
  c->cd();

  histo->Draw("COLZ");
  histo->SetTitle("");
  histo->GetXaxis()->SetTitle(xTitle);
  histo->GetYaxis()->SetTitle(yTitle);

  c->SetLogz();

  
  return c;

}
//####################################################################################################
