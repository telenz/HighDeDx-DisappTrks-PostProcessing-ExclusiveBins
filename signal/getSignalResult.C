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


int getSignalResult(double metCut, double jetptCut,  double ptCutMin,  double ptCutMax, double ecaloCut, double iasCutMin, double iasCutMax, TString region, TString name){

  vector<sample> samples;
  samples.push_back(name);

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
