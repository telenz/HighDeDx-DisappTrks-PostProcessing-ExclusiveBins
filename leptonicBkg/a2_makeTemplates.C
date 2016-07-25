#include "TLegend.h"
#include "TLatex.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLine.h"
#include "TTree.h"
#include "TChain.h"
#include "TRandom3.h"
#include "TH1D.h"
#include "TFile.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include "../plotStyle.h"
#include "Classes.h"



int a2_makeTemplates(int pdgId, double metCut, double jetptCut, double ecaloCut, double iasCutMin, double iasCutMax, TString region){

  sample mc("wjets");
  mc.getTree(fileSR,"chiTrackspreselectionNoQCDCutsNoTrigger");
  mc.getTreeVariables();

  TString filename = Form("templates/IasTemplate_METGt%.0f_JetPtGt%.0f_ECaloEq%.0f_pdgId%i.root",metCut,jetptCut,ecaloCut,pdgId);
  TFile *out       = new TFile(filename,"RECREATE");

  Double_t xbinsASmi[4]  = {0,iasCutMin,iasCutMax,1};
  TString histoName = (TString) Form("lepton_bkg_data_Iascut_0p%02.0f-0p%02.0f",iasCutMin*100,iasCutMax*100);
  cout<<"histoName = "<<histoName<<endl;
  mc.histoASmi   = new TH1D(histoName, histoName, 2, xbinsASmi);
  mc.histoASmi->Sumw2();

  mc.Selection(1, pdgId, 0, 0 , 20.,5000000, 0.0 , 1.0, 0 , ecaloCut, 1, region);

  cout<<"Entries: "<<mc.histoASmi->GetEntries()<<endl;
  cout<<"2nd bin: "<<mc.histoASmi->GetBinContent(2)<<" +/- "<<mc.histoASmi->GetBinError(2)<<endl;

  // ***** Calculate scaling factor  ************************************************************************
  mc.histoASmi->Scale(1./mc.histoASmi->Integral());
  // ***********************************************************************************************************

  out->cd();
  mc.histoASmi->Write();
  
  delete mc.histoASmi;
      
  out->Close();
 
  return 0;
}

//################################################################################################################################
//################################################################################################################################
//################################################################################################################################
