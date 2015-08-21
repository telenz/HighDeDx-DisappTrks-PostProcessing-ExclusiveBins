#include "TROOT.h"
#include "TFile.h"
#include "TProfile.h"
#include "TH1.h"
#include "TMath.h"
#include "TGraph.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include <iostream>

TString idir="~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_15_METGt0_JetPtGt70_trackPtGt20/results/analyzer/ntuples/input_weighted/";
TString odir="output";

void makeROCplot(int ctau  = 5){


  int nStep = 10;


  TString bkg = "wjets";

  TFile *f = new TFile(idir + "/" + bkg + ".root","READ");
  TH1D* httbarNP=0;
  f->GetObject("chiTrackstriggerRequirements/htrackASmiNP",httbarNP);
  httbarNP->SetDirectory(0);
  f->Close();
  f = new TFile(idir + "/" + bkg + ".root","READ");
  TH1D* httbar=0;
  f->GetObject("chiTrackstriggerRequirements/htrackASmi",httbar);
  httbar->SetDirectory(0);
  f->Close();

  TFile *f1 = new TFile(idir + "/Madgraph_signal_mass_500_ctau_" + (long) ctau + "cm.root","READ");
  TH1D* hsignal500_0_NP=0;
  f1->GetObject("chiTrackstriggerRequirements/htrackASmiNP",hsignal500_0_NP);
  hsignal500_0_NP->SetDirectory(0);
  f1->Close();
  f1 = new TFile(idir + "/Madgraph_signal_mass_500_ctau_" + (long) ctau + "cm.root","READ");
  TH1D* hsignal500_0=0;
  f1->GetObject("chiTrackstriggerRequirements/htrackASmi",hsignal500_0);
  hsignal500_0->SetDirectory(0);
  f1->Close();

  f1 = new TFile(idir + "/Madgraph_signal_mass_500_ctau_50cm.root","READ");
  TH1D* hsignal500_1_NP=0;
  f1->GetObject("chiTrackstriggerRequirements/htrackASmiNP_3",hsignal500_1_NP);
  hsignal500_1_NP->SetDirectory(0);
  f1->Close();
  f1 = new TFile(idir + "/Madgraph_signal_mass_500_ctau_50cm.root","READ");
  TH1D* hsignal500_1=0;
  f1->GetObject("chiTrackstriggerRequirements/htrackASmi_3",hsignal500_1);
  hsignal500_1->SetDirectory(0);
  f1->Close();


  double bkgFullNP=0;
  double bkgCutNP=0;
  double signalFullNP_0=0;
  double signalCutNP_0=0;
  double signalFullNP_1=0;
  double signalCutNP_1=0;

  double bkgFull=0;
  double bkgCut=0;
  double signalFull_0=0;
  double signalCut_0=0;
  double signalFull_1=0;
  double signalCut_1=0;

  double *xNP;
  double *x;
  double *yNP_0;
  double *y_0;
  double *yNP_1;
  double *y_1;


  xNP    = new double[nStep];
  x      = new double[nStep];
  yNP_0  = new double[nStep];
  y_0    = new double[nStep];
  yNP_1  = new double[nStep];
  y_1    = new double[nStep];


  for(int i=0; i<nStep; i++){

    bkgFullNP=httbarNP->Integral(httbarNP->FindBin(0.),httbarNP->FindBin(1.));
    bkgCutNP=httbarNP->Integral(httbarNP->FindBin( (double) i/nStep),httbarNP->FindBin(1.));
    double bkgEffNP = bkgCutNP/bkgFullNP;
    xNP[i]=bkgEffNP;
   
    signalFullNP_0=hsignal500_0_NP->Integral(hsignal500_0_NP->FindBin(0.),hsignal500_0_NP->FindBin(1.));
    signalCutNP_0=hsignal500_0_NP->Integral(hsignal500_0_NP->FindBin( (double) i/nStep),hsignal500_0_NP->FindBin(1.));
    double signalEffNP_0 = signalCutNP_0/signalFullNP_0;
    yNP_0[i]=signalEffNP_0;
  
    signalFullNP_1=hsignal500_1_NP->Integral(hsignal500_1_NP->FindBin(0.),hsignal500_1_NP->FindBin(1.));
    signalCutNP_1=hsignal500_1_NP->Integral(hsignal500_1_NP->FindBin( (double) i/nStep),hsignal500_1_NP->FindBin(1.));
    double signalEffNP_1 = signalCutNP_1/signalFullNP_1;
    yNP_1[i]=signalEffNP_1;


    bkgFull=httbar->Integral(httbar->FindBin(0.),httbar->FindBin(1.));
    bkgCut=httbar->Integral(httbar->FindBin( (double) i/nStep),httbar->FindBin(1.));
    double bkgEff = bkgCut/bkgFull;
    x[i]=bkgEff;
    
    signalFull_0=hsignal500_0->Integral(hsignal500_0->FindBin(0.),hsignal500_0->FindBin(1.));
    signalCut_0=hsignal500_0->Integral(hsignal500_0->FindBin( (double) i/nStep),hsignal500_0->FindBin(1.));
    double signalEff_0 = signalCut_0/signalFull_0;
    y_0[i]=signalEff_0;

    signalFull_1=hsignal500_1->Integral(hsignal500_1->FindBin(0.),hsignal500_1->FindBin(1.));
    signalCut_1=hsignal500_1->Integral(hsignal500_1->FindBin( (double) i/nStep),hsignal500_1->FindBin(1.));
    double signalEff_1 = signalCut_1/signalFull_1;
    y_1[i]=signalEff_1;

    //    cout<<"xNP["<<i<<"] = "<<xNP[i]<<endl;
  
  }

  TCanvas *c = new TCanvas("c"+bkg,"c"+bkg,0,0,500,500);
  c->cd();
  c->SetLogx();
  TGraph *gNP_0 = new TGraph(nStep,xNP,yNP_0);
  TGraph *g_0 = new TGraph(nStep,x,y_0);
  TGraph *gNP_1 = new TGraph(nStep,xNP,yNP_1);
  TGraph *g_1 = new TGraph(nStep,x,y_1);

  g_0->SetMarkerStyle(20);
  g_1->SetMarkerStyle(20);
  gNP_0->SetMarkerStyle(20);
  gNP_1->SetMarkerStyle(20);
  g_0->SetMarkerColor(2);
  g_1->SetMarkerColor(3);
  gNP_0->SetMarkerColor(1);
  gNP_1->SetMarkerColor(5);
  g_0->SetTitle("");
  g_0->GetXaxis()->SetTitle("bkg efficiency (" + bkg + " only)");
  g_0->GetYaxis()->SetTitle("signal efficiency");
  g_0->GetXaxis()->SetTitleSize(0.04);
  g_0->GetYaxis()->SetTitleSize(0.04);
  g_0->Draw("AP");
  gNP_0->Draw("same P");
  //g_1->Draw("same P");
  //gNP_1->Draw("same P");


  TLegend* leg = new TLegend(0.1,0.7,0.65,0.9);
  leg->SetTextSize(0.037);
  leg->SetFillColor(kWhite);
  leg->AddEntry(g_0,"Pixel hits included","p");
  leg->AddEntry(gNP_0,"Without Pixel hits","p");
  //leg->AddEntry(g_1,"3 hits - Pixel hits included","p");
  //leg->AddEntry(gNP_1,"3 hits - Without Pixel hits","p");
  leg->Draw("same");

  TLatex* info = new TLatex();
  info->SetTextFont(132);
  info->SetNDC();
  info->SetTextSize(0.040);
  info->DrawLatex(0.7,0.3,"m=500GeV");
  info->DrawLatex(0.7,0.25,Form("c#tau=%icm",ctau));


  c->SaveAs(odir + "/rocplot_m500_" + (long) ctau + "cm_" + bkg + ".pdf");
  



}
