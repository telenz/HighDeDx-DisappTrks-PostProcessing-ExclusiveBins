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


int SignalResults(double metCut, double ptCut, double ecaloCut, double iasCut, TString region){

  vector<sample> samples;
 
  sample sig_m100_ct1("Madgraph_signal_mass_100_ctau_1cm");
  samples.push_back(sig_m100_ct1);
  sample sig_m100_ct2("Madgraph_signal_mass_100_ctau_2cm");
  //samples.push_back(sig_m100_ct2);
  sample sig_m300_ct5("Madgraph_signal_mass_300_ctau_5cm");
  samples.push_back(sig_m300_ct5);
  sample sig_m500_ct25("Madgraph_signal_mass_500_ctau_25cm");
  samples.push_back(sig_m500_ct25); 


  for(unsigned int i=0; i<samples.size(); i++){


    samples[i].getTree(fileSR);
    samples[i].getTreeVariables();
    samples[i].histo = new TH1D(samples[i].filename,samples[i].filename,1,0,1);
    samples[i].histo->Sumw2();
    samples[i].Selection(  0, 0, metCut, 110., ptCut, iasCut, 1,          ecaloCut, 0, region);
    cout<<"after slection "<<endl;
    double nSRError  = 0;
    double nSR       =  samples[i].histo->IntegralAndError(1,samples[i].histo->GetNbinsX()+1,nSRError);
    cout<<"Yield "<<samples[i].filename<<" = "<<nSR<<" +/- "<<nSRError<<endl;
    cout<<"Yield "<<samples[i].filename<<" = "<<samples[i].histo->GetSumOfWeights()<<" +/- "<<nSRError<<endl;
    cout<<"Precision                               = "<<nSRError/nSR*100<<"%"<<endl;
    cout<<"Entries                                 = "<<samples[i].histo->GetEntries()<<endl;
    cout<<"Average event weight                    = "<<samples[i].histo->GetSumOfWeights()/samples[i].histo->GetEntries()<<endl<<endl;

  }
  return 0;
 
}

//################################################################################################################################
//################################################################################################################################
