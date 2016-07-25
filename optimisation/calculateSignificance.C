#include "TROOT.h"
#include "TFile.h"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2D.h"
#include "TMath.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TROOT.h"
#include "TGaxis.h"
#include "TAxis.h"
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
//#include "../plotStyle.h"
#include "plotStyleThesis.h"
#include "../poisson/getPoissonCL.h"

//TString folder = "NLostOuterGe0";
//TString folder = "";

int calculateSignificance(double metCut, double ecaloCut, TString region, TString folder = "NLostOuterGe0"){

  
  TString nlost = "";
  if(folder.Contains("NLostOuterGe0")) nlost="0";
  else if(folder.Contains("NLostOuterGe1")) nlost="1";
  else if(folder.Contains("NLostOuterGe2")) nlost="2";
  else if(folder.Contains("NLostOuterGe3")) nlost="3";

  gStyle->SetOptStat(0);
  TeresaPlottingStyle2d::init();
  gStyle -> SetTitleSize(0.05,"Z");
  gStyle -> SetTitleOffset(1.2,"Z");
  gStyle -> SetOptTitle(1);
  gStyle -> SetNdivisions(505,"Z");

  gStyle->SetTitleStyle(1); 
  gStyle->SetStatStyle(0);
  gROOT->ForceStyle(); 

  //gStyle->SetTitleBorderSize(0);
  //gStyle -> SetPadRightMargin(0.20);
  //gStyle->SetTitleFont(42,"");

  vector<TString> samples;
  vector<TString> titles;
  TString sig_m100_ct1 = "Madgraph_signal_mass_100_ctau_1cm";
  samples.push_back(sig_m100_ct1);
  titles.push_back("mass=100GeV, c#tau=1cm");
  TString sig_m100_ct5 = "Madgraph_signal_mass_100_ctau_5cm";
  samples.push_back(sig_m100_ct5);
  titles.push_back("mass=100GeV, c#tau=5cm");
  TString sig_m100_ct10 = "Madgraph_signal_mass_100_ctau_10cm";
  samples.push_back(sig_m100_ct10);
  titles.push_back("mass=100GeV, c#tau=10cm");
  TString sig_m100_ct50 = "Madgraph_signal_mass_100_ctau_50cm";
  samples.push_back(sig_m100_ct50);
  titles.push_back("mass=100GeV, c#tau=50cm");
  TString sig_m200_ct1 = "Madgraph_signal_mass_200_ctau_1cm";
  //samples.push_back(sig_m200_ct1);
  TString sig_m200_ct5 = "Madgraph_signal_mass_200_ctau_5cm";
  //samples.push_back(sig_m200_ct5);
  TString sig_m200_ct50 = "Madgraph_signal_mass_200_ctau_50cm";
  //samples.push_back(sig_m200_ct50);
  TString sig_m300_ct5 = "Madgraph_signal_mass_300_ctau_5cm";
  //samples.push_back(sig_m300_ct5);
  TString sig_m300_ct10 = "Madgraph_signal_mass_300_ctau_10cm";
  //samples.push_back(sig_m300_ct10);
  TString sig_m300_ct50 = "Madgraph_signal_mass_300_ctau_50cm";
  //samples.push_back(sig_m300_ct50);
  TString sig_m500_ct1 = "Madgraph_signal_mass_500_ctau_1cm";
  //samples.push_back(sig_m500_ct1);
  //titles.push_back("mass=500GeV, c#tau=1cm");
  TString sig_m500_ct5 = "Madgraph_signal_mass_500_ctau_5cm";
  samples.push_back(sig_m500_ct5);
  titles.push_back("mass=500GeV, c#tau=5cm");
  TString sig_m500_ct10 = "Madgraph_signal_mass_500_ctau_10cm";
  samples.push_back(sig_m500_ct10);
  titles.push_back("mass=500GeV, c#tau=10cm");
  TString sig_m500_ct50 = "Madgraph_signal_mass_500_ctau_50cm";
  samples.push_back(sig_m500_ct50);
  titles.push_back("mass=500GeV, c#tau=50cm");

  
  for(unsigned int i=0; i<samples.size(); i++){

    
    TH2D* sig1 = new TH2D("significance",samples[i] + ": s/#Delta b_{stat}",9,20,65,9,0.00,0.45);
    sig1->GetXaxis()->SetTitle("p_{T} cut [GeV]");
    sig1->GetYaxis()->SetTitle("I_{as} cut");
    //sig1->GetZaxis()->SetTitle("minimum possible x-sec to discover [pb]");
    sig1->GetZaxis()->SetTitle("minimal discoverable x-sec [pb]");
    sig1->GetZaxis()->CenterTitle();
    sig1->SetTitle(titles[i]);

    //    TH2D* sig2 = new TH2D("significance",samples[i]+ ": s/#sqrt((#Delta b_{stat})^{2} + (#Delta b_{sys}^{fake})^{2} + (10 #upoint #Delta b_{sys}^{lepton})^{2})",7,30,65,9,0.00,0.45);
    TH2D* sig2 = new TH2D("significance",samples[i]+ ": s/#Delta b_{stat + sys}",9,20,65,9,0.00,0.45);
    sig2->GetXaxis()->SetTitle("p_{T} cut [GeV]");
    sig2->GetYaxis()->SetTitle("I_{as} cut");
    sig2->GetZaxis()->SetTitle("minimal discoverable x-sec [pb]");
    sig2->GetZaxis()->CenterTitle();
    //sig2->SetTitle(titles[i] + ", N_{lost}^{outer}#geq" + nlost);
    sig2->SetTitle(titles[i]);
    sig2->GetZaxis()->SetNoExponent(kTRUE);
    //sig2->GetZaxis()->SetRangeUser(0.5,2.5);

    TH2D* hBkgYield = new TH2D("hBkgYield","Bkg Yield",9,20,65,9,0.00,0.45);
    hBkgYield -> GetXaxis()->SetTitle("p_{T} cut [GeV]");
    hBkgYield -> GetYaxis()->SetTitle("I_{as} cut");
    hBkgYield -> GetZaxis()->SetTitle("bkg yield");

    TH2D* hBkgUnc = new TH2D("hBkgUnc","Bkg Unc",9,20,65,9,0.00,0.45);
    hBkgUnc -> GetXaxis()->SetTitle("p_{T} cut [GeV]");
    hBkgUnc -> GetYaxis()->SetTitle("I_{as} cut");
    hBkgUnc -> GetZaxis()->SetTitle("bkg uncertainty [%]");

    TH2D* hSignalYield = new TH2D("hSignalYield",samples[i] + ": Signal Yield",9,20,65,9,0.00,0.45);
    hSignalYield -> GetXaxis()->SetTitle("p_{T} cut [GeV]");
    hSignalYield -> GetYaxis()->SetTitle("I_{as} cut");
    hSignalYield -> GetZaxis()->SetTitle("signal yield");
    hSignalYield -> SetTitle(titles[i]);

    //----------------------------------------------------------------------------------------------------------
    // Get theory cross-section:
    cout<<"######################################################"<<endl;
    cout<<"Get theory cross section:"<<endl;
    ifstream inputFile("xsectionsUpdated.txt");
    int it=0;
    TString name;
    double xsec,err;
    while(inputFile>>name>>xsec>>err){
      TString aux =  "mass_" + name;
      cout<<"aux = "<<aux<<endl;
      if ( samples[i].Contains(aux) ){
	cout<<endl<<name<<": "<<xsec<<" pb"<<endl<<endl;
	break;
      }
      it++;
    }
    inputFile.close();
    xsec = xsec/1000.;   // xsection in pb
    cout<<"######################################################"<<endl;
    //----------------------------------------------------------------------------------------------------------

    for(int l=0; l<9; l++){
      //      for(int l=0; l<9; l++){

      double ptCut = 20 + l*5;

      //for(int j=0; j<9; j++){
      for(int j=0; j<9; j++){
  

	double iasCut = 0.00 + j*0.05;

	cout<<"---------------------------------------------"<<""<<endl;
	cout<<"pt cut = "<<ptCut<<", Ias Cut = "<<iasCut<<endl;
	//TFile *fBkg        = new TFile(Form("totalBkg" + folder + "/TotalBkg_metCutEq%.0f_ptCutEq%0.f_ECaloCutEq%.0f_IasCutEq0p%02.0f_",metCut,ptCut,ecaloCut,iasCut*100) + region +".root" ,"READ");
	//TString filename   = "signal" + folder + "/Signal_" + samples[i]  + Form("_metCutEq%.0f_ptCutEq%.0f_ECaloCutEq%.0f_IasCutEq0p%02.0f.root",metCut,ptCut,ecaloCut,iasCut*100);
	TFile *fBkg        = new TFile(Form("totalBkg" + folder + "/TotalBkg_metCutEq%.0f_ptGt%.0f_Le50000_ECaloCutEq%.0f_IasGt0p%02.0f_Le0p99_",metCut,ptCut,ecaloCut,iasCut*100) + region +".root" ,"READ");
	TString filename   = "signal" + folder + "/Signal_" + samples[i]  + Form("_metCutEq%.0f_ptGt%.0f_Le50000_ECaloCutEq%.0f_IasGt0p%02.0f_Le0p99.root",metCut,ptCut,ecaloCut,iasCut*100);
	TFile *fSignal     = new TFile(filename,"READ");
	TH1D  *hBkg        = 0;
	TH1D  *hLepton     = 0;
	TH1D  *hFake       = 0;
	TH1D  *hSignal     = 0;
	fBkg       -> GetObject("totalBkg",hBkg);
	fBkg       -> GetObject("leptonBkg",hLepton);
	fBkg       -> GetObject("fakeBkg",hFake);
	fSignal    -> GetObject(samples[i],hSignal);

	cout.precision(3);
	double nAll         = 0;
	double nAllErrorUp  = 0;
	double nAllErrorLow = 0;
	double n, nErrorUp, nErrorLow;
 
	TString eq = "=";
    
	int bin         = hBkg->GetXaxis()->FindBin(region);
	n = hBkg->IntegralAndError(bin,bin,nErrorUp);
	nErrorLow = nErrorUp;
	if(nErrorUp==0) eq = "<";
	else            eq = "=";
	nAll         = n ; 
	nAllErrorUp  = nErrorUp;
	nAllErrorLow = nErrorLow;
      
	cout<<"nAll        = "<<nAll<<endl;
	cout<<"nAllErrorUp = "<<nAllErrorUp<<endl;
	cout<<"One Sided Upper 68% limit of bkg = "<<getOneSidedUpperLimit(nAll,0.6827)-nAll<<endl;
    
    	cout<<"Total bkg                          = "<<nAll<<" + "<<nAllErrorUp<<" - "<<nAllErrorLow<<""<<endl;
	cout<<samples[i]<<" = "<<hSignal->GetBinContent(1)<<" +/- "<<hSignal->GetBinError(1)<<endl;
	cout<<"Statistics of signal = "<<pow(hSignal->GetBinContent(1),2)/pow(hSignal->GetBinError(1),2)<<endl;

	hBkgYield    -> SetBinContent(l+1,j+1,n);
	hBkgUnc      -> SetBinContent(l+1,j+1,nErrorUp/n);
	hSignalYield -> SetBinContent(l+1,j+1,hSignal->GetBinContent(1));

	double sOverDeltabUp    = hSignal->GetBinContent(1)/(getOneSidedUpperLimit(nAll,0.6827)-nAll);
	double sOverDeltabError = 0;
	sig1->SetBinContent(l+1,j+1,sOverDeltabUp);
	sig1->SetBinError(l+1,j+1,sOverDeltabError);


	double leptonStatError = hLepton -> GetBinError(bin);
	double fakeStatError   = hFake   -> GetBinError(bin);
	cout<<"leptonStatError  = "<<leptonStatError<<endl;
	cout<<"fakeStatError    = "<<fakeStatError<<endl;

	double leptonSysError = hLepton -> GetBinContent(bin)*1.0;  // 100% error
	double fakeSysError   = hFake   -> GetBinContent(bin)*0.2;  // 20% error
	cout<<"leptonSysError  = "<<leptonSysError<<endl;
	cout<<"fakeSysError    = "<<fakeSysError<<endl;
	  
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// Vary now signal x-sec and don't take the one given from theory -> model independent optimisation
	double sOverDeltabUpStatPlusSys          = 0;
	double sOverDeltabUpStatPlusSysError     = 0;
	double minExcludedXsec                   = 0;
	for(int k=0; k<1000; k++){

	  sOverDeltabUpStatPlusSys            = k/sqrt( pow( getOneSidedUpperLimit(nAll,0.6827)-nAll ,2) + pow(fakeStatError ,2) + pow(leptonStatError ,2) + pow(fakeSysError ,2) + pow(leptonSysError ,2));
	   

	  if(sOverDeltabUpStatPlusSys>5){
	    cout<<"Dicovery possible !!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;

	    minExcludedXsec =1.*k/hSignal->GetBinContent(1)*xsec;
	    cout<<"minimal xsec = "<<minExcludedXsec<<endl;
	    cout<<"theory xsec  = "<<xsec<<endl; 
	    break;

	  }

	  
	  
	}

	sig2->SetBinContent(l+1,j+1,minExcludedXsec);
	//	sig2->SetBinError(l+1,j+1,sOverDeltabUpStatPlusSysError);

	cout<<"s/(Delta b_stat)                          = "<<sOverDeltabUp<<" +/- "<<sOverDeltabError<<endl;
	cout<<"s/sqrt(Delta b_stat**2 + (Delta b_sys**2) = "<<sOverDeltabUpStatPlusSys<<" +/- "<<sOverDeltabUpStatPlusSysError<<endl;
    
	//fBkg->Close();
	//fSignal->Close();

	for(int k=0; k<1000; k++){

	  sOverDeltabUp                       = k/sqrt( pow( getOneSidedUpperLimit(nAll,0.6827)-nAll ,2));
	  
	  if(sOverDeltabUp>5){
	    cout<<"Dicovery possible !!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;

	    minExcludedXsec =1.*k/hSignal->GetBinContent(1)*xsec;
	    cout<<"minimal xsec = "<<minExcludedXsec<<endl;
	    cout<<"theory xsec  = "<<xsec<<endl; 
	    break;

	  }
	  
	}


	sig1->SetBinContent(l+1,j+1,minExcludedXsec);

	cout<<"s/(Delta b_stat)                          = "<<sOverDeltabUp<<" +/- "<<sOverDeltabError<<endl;
	cout<<"s/sqrt(Delta b_stat**2 + (Delta b_sys**2) = "<<sOverDeltabUpStatPlusSys<<" +/- "<<sOverDeltabUpStatPlusSysError<<endl;
    
	fBkg->Close();
	fSignal->Close();

      }
    }
    TCanvas *c = new TCanvas(samples[i],samples[i],500,500);
    sig1->Draw("COLZ");
    c->SaveAs(Form("plots" + folder + "/" + samples[i] + "_ECaloLe%.0f" +  "_SOverDeltaBStat.pdf", ecaloCut));

    TCanvas *c1 = new TCanvas(samples[i],samples[i],500,500);
    sig2->Draw("COLZ");
    //c1->SaveAs(Form("plots" + folder + "/" + samples[i] + "_ECaloLe%.0f" + "_SOverDeltaBStatPlusSys_" + nlost+ ".pdf",ecaloCut));
    c1->SaveAs(Form("plots" + folder + "/" + samples[i] + "_ECaloLe%.0f" + "_SOverDeltaBStatPlusSys.pdf",ecaloCut));

    TCanvas *c2 = new TCanvas(samples[i],samples[i],500,500);
    hBkgYield->Draw("COLZ");
    c2->SaveAs(Form("plots" + folder + "/BkgYield" + "_ECaloLe%.0f" + ".pdf",ecaloCut));

    TCanvas *c3 = new TCanvas(samples[i],samples[i],500,500);
    hBkgUnc->Draw("COLZ");
    c3->SaveAs(Form("plots" + folder + "/BkgUncertainty" + "_ECaloLe%.0f" + ".pdf",ecaloCut));

    TCanvas *c4 = new TCanvas(samples[i],samples[i],500,500);
    hSignalYield->Draw("COLZ");
    c4->SaveAs(Form("plots" + folder + "/" + samples[i] + "_ECaloLe%.0f" + "_SignalYield.pdf",ecaloCut));

    

  }
  
  return 0;
 
}

//################################################################################################################################
//################################################################################################################################
