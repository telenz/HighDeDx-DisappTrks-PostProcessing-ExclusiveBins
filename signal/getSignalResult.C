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
#include "Classes.h"


int getSignalResult(double metCut, double jetptCut,  double ptCutMin,  double ptCutMax, double ecaloCut, double iasCutMin, double iasCutMax, TString region){

  vector<sample> samples;
  
  
  sample sig_m100_ct1("Madgraph_signal_mass_100_ctau_1cm");
  samples.push_back(sig_m100_ct1);
  sample sig_m100_ct5("Madgraph_signal_mass_100_ctau_5cm");
  samples.push_back(sig_m100_ct5);
  sample sig_m100_ct10("Madgraph_signal_mass_100_ctau_10cm");
  samples.push_back(sig_m100_ct10);
  sample sig_m100_ct50("Madgraph_signal_mass_100_ctau_50cm");
  samples.push_back(sig_m100_ct50);
  sample sig_m100_ct100("Madgraph_signal_mass_100_ctau_100cm");
  samples.push_back(sig_m100_ct100);
  sample sig_m100_ct1000("Madgraph_signal_mass_100_ctau_1000cm");
  samples.push_back(sig_m100_ct1000);

  sample sig_m200_ct1("Madgraph_signal_mass_200_ctau_1cm");
  samples.push_back(sig_m200_ct1);
  sample sig_m200_ct5("Madgraph_signal_mass_200_ctau_5cm");
  samples.push_back(sig_m200_ct5);
  sample sig_m200_ct10("Madgraph_signal_mass_200_ctau_10cm");
  samples.push_back(sig_m200_ct10);
  sample sig_m200_ct50("Madgraph_signal_mass_200_ctau_50cm");
  samples.push_back(sig_m200_ct50);
  sample sig_m200_ct100("Madgraph_signal_mass_200_ctau_100cm");
  samples.push_back(sig_m200_ct100);
  sample sig_m200_ct1000("Madgraph_signal_mass_200_ctau_1000cm");
  samples.push_back(sig_m200_ct1000);

  sample sig_m300_ct1("Madgraph_signal_mass_300_ctau_1cm");
  samples.push_back(sig_m300_ct1);
  sample sig_m300_ct5("Madgraph_signal_mass_300_ctau_5cm");
  samples.push_back(sig_m300_ct5);
  sample sig_m300_ct10("Madgraph_signal_mass_300_ctau_10cm");
  samples.push_back(sig_m300_ct10);
  sample sig_m300_ct50("Madgraph_signal_mass_300_ctau_50cm");
  samples.push_back(sig_m300_ct50);
  sample sig_m300_ct100("Madgraph_signal_mass_300_ctau_100cm");
  samples.push_back(sig_m300_ct100);
  sample sig_m300_ct1000("Madgraph_signal_mass_300_ctau_1000cm");
  samples.push_back(sig_m300_ct1000);

  sample sig_m400_ct1("Madgraph_signal_mass_400_ctau_1cm");
  samples.push_back(sig_m400_ct1);
  sample sig_m400_ct5("Madgraph_signal_mass_400_ctau_5cm");
  samples.push_back(sig_m400_ct5);
  sample sig_m400_ct10("Madgraph_signal_mass_400_ctau_10cm");
  samples.push_back(sig_m400_ct10);
  sample sig_m400_ct50("Madgraph_signal_mass_400_ctau_50cm");
  samples.push_back(sig_m400_ct50);
  sample sig_m400_ct100("Madgraph_signal_mass_400_ctau_100cm");
  samples.push_back(sig_m400_ct100);
  sample sig_m400_ct1000("Madgraph_signal_mass_400_ctau_1000cm");
  samples.push_back(sig_m400_ct1000);

  sample sig_m500_ct1("Madgraph_signal_mass_500_ctau_1cm");
  samples.push_back(sig_m500_ct1);
  sample sig_m500_ct5("Madgraph_signal_mass_500_ctau_5cm");
  samples.push_back(sig_m500_ct5);
  sample sig_m500_ct10("Madgraph_signal_mass_500_ctau_10cm");
  samples.push_back(sig_m500_ct10);
  sample sig_m500_ct50("Madgraph_signal_mass_500_ctau_50cm");
  samples.push_back(sig_m500_ct50);
  sample sig_m500_ct100("Madgraph_signal_mass_500_ctau_100cm");
  samples.push_back(sig_m500_ct100);
  sample sig_m500_ct1000("Madgraph_signal_mass_500_ctau_1000cm");
  samples.push_back(sig_m500_ct1000);


  TFile *out = 0;
  TH1D *histoSignal = 0;
  TString filename = "";

  for(unsigned int i=0; i<samples.size(); i++){

    filename = "results/Signal_" + samples[i].filename  + Form("_metCutEq%.0f_ptGt%.0f_Le%.0f_ECaloCutEq%.0f_IasGt0p%02.0f_Le0p%02.0f.root",metCut,ptCutMin,ptCutMax,ecaloCut,iasCutMin*100,iasCutMax*100);
    cout<<"filename = "<<filename<<endl;
    out = new TFile(filename,"RECREATE");
    histoSignal = new TH1D(samples[i].filename,samples[i].filename,1,0,1);

    histoSignal -> Fill(region,0);
    

    samples[i].getTree(fileSR);
    samples[i].getTreeVariables();
    samples[i].histo = new TH1D(samples[i].filename,samples[i].filename,1,0,1);
    samples[i].histo->Sumw2();
    samples[i].Selection(  0, 0, metCut, jetptCut, ptCutMin, ptCutMax, iasCutMin, iasCutMax, 1,          ecaloCut, 0, region);
    double nSRError  = 0;
    double nSR       =  samples[i].histo->IntegralAndError(1,samples[i].histo->GetNbinsX()+1,nSRError);

    histoSignal->SetBinContent(1,nSR);
    histoSignal->SetBinError(1,nSRError);
    out->cd();
    histoSignal->Write();
    out->Close();
    cout<<"Yield "<<samples[i].filename<<" = "<<nSR<<" +/- "<<nSRError<<endl;
    cout<<"Precision                               = "<<nSRError/nSR*100<<"%"<<endl;
    cout<<"NEvents                                 = "<<samples[i].histo->GetEntries()<<endl;
    cout<<"Average event weight                    = "<<samples[i].histo->GetSumOfWeights()/samples[i].histo->GetEntries()<<endl<<endl;

  }
  return 0;
 
}

//################################################################################################################################
//################################################################################################################################
