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
#include "../../../plotStyle.h"
#include "../../../inputAnalysisFiles.h"


class sample{


public:

  TFile *file;
  TTree *tree;

  TH1D* histoBefore;
  TH1D* histoAfter;

  double statistics;
  double purity;

  vector<double> *trackEta;
  vector<double> *trackPt;
  vector<double> *trackP;
  vector<double> *trackGenPt;
  vector<double> *trackASmi;
  vector<double> *trackHarm2;
  vector<double> *trackCaloIso;
  vector<double> *trackMass;
  vector<double> *trackIsolation;
  vector<double> *trackGenE;
  vector<double> *trackGenEt;
  vector<double> *trackEndVertexRho ;
  vector<double> *trackChi2;
  vector<double> *trackNdof;
  vector<double> *trackHCALRp5Isolation;
  vector<double> *trackECALRp5Isolation;
  vector<double> *trackHCALRp4Isolation;
  vector<double> *trackECALRp4Isolation;
  vector<double> *trackHCALRp3Isolation;
  vector<double> *trackECALRp3Isolation;
  vector<int>    *trackNLostOuter;
  vector<int>    *trackNLostMiddle;
  vector<int>    *trackNLostInner;
  vector<int>    *trackNValid;
  vector<int>    *trackPdgId;
  double weight;
  double weight_xsec_lumi;
  double met;
  double leadingJetPt;

  vector<double> *trackDeDx1;
  vector<double> *trackDeDx2;
  vector<double> *trackDeDx3;
  vector<double> *trackDeDx4;
  vector<double> *trackDx1;
  vector<double> *trackDx2;
  vector<double> *trackDx3;
  vector<double> *trackDx4;

  vector<int>    *trackMeasSize;

  sample(){

    tree=0;
    file=0;
    histoBefore=0;
    histoAfter=0;
    statistics=0;
    purity=0;

    trackEta        = 0;
    trackEta        = 0;
    trackPt         = 0;
    trackP          = 0;
    trackGenPt      = 0;
    trackASmi       = 0;
    trackHarm2      = 0;
    trackCaloIso    = 0;
    trackMass       = 0;
    trackIsolation  = 0;
    trackGenE       = 0;
    trackGenEt      = 0;
    trackEndVertexRho = 0;
    trackChi2 = 0;
    trackNdof = 0;
    trackHCALRp5Isolation = 0;
    trackECALRp5Isolation = 0;
    trackHCALRp4Isolation = 0;
    trackECALRp4Isolation = 0;
    trackHCALRp3Isolation = 0;
    trackECALRp3Isolation = 0;
    trackNLostOuter = 0;
    trackNLostMiddle = 0;
    trackNLostInner = 0;
    trackNValid     = 0;
    trackPdgId      = 0;
    trackDeDx1   = 0;
    trackDeDx2   = 0;
    trackDeDx3   = 0;
    trackDeDx4   = 0;
    trackDx1   = 0;
    trackDx2   = 0;
    trackDx3   = 0;
    trackDx4   = 0;
    trackMeasSize= 0;
  };

  void getTreeVariables(){
    tree->SetBranchAddress("trackEta",&trackEta);
    tree->SetBranchAddress("trackPt",&trackPt);
    tree->SetBranchAddress("trackP",&trackP);
    tree->SetBranchAddress("trackGenPt",&trackGenPt);
    tree->SetBranchAddress("trackDeDxASmi",&trackASmi);
    tree->SetBranchAddress("trackDeDxHarm2",&trackHarm2);
    tree->SetBranchAddress("trackCaloIsolation",&trackCaloIso);
    tree->SetBranchAddress("trackMass",&trackMass);
    tree->SetBranchAddress("trackIsolation",&trackIsolation);
    tree->SetBranchAddress("trackGenE",&trackGenE);
    tree->SetBranchAddress("trackGenEt",&trackGenEt);
    tree->SetBranchAddress("trackEndVertexRho",&trackEndVertexRho);
    tree->SetBranchAddress("trackChi2",&trackChi2);
    tree->SetBranchAddress("trackNdof",&trackNdof);
    tree->SetBranchAddress("trackNLostOuter",&trackNLostOuter);
    tree->SetBranchAddress("trackNLostInner",&trackNLostInner);
    tree->SetBranchAddress("trackNLostMiddle",&trackNLostMiddle);
    tree->SetBranchAddress("trackNValid",&trackNValid);
    tree->SetBranchAddress("trackHCALRp5Isolation",&trackHCALRp5Isolation);
    tree->SetBranchAddress("trackECALRp5Isolation",&trackECALRp5Isolation);
    tree->SetBranchAddress("trackHCALRp4Isolation",&trackHCALRp4Isolation);
    tree->SetBranchAddress("trackECALRp4Isolation",&trackECALRp4Isolation);
    tree->SetBranchAddress("trackHCALRp3Isolation",&trackHCALRp3Isolation);
    tree->SetBranchAddress("trackECALRp3Isolation",&trackECALRp3Isolation);
    tree->SetBranchAddress("trackPdgId",&trackPdgId);
    tree->SetBranchAddress("MET",&met);
    tree->SetBranchAddress("LeadingJetPt",&leadingJetPt);
    tree->SetBranchAddress("weight",&weight);
    tree->SetBranchAddress("weight_xsec_lumi",&weight_xsec_lumi);
    tree->SetBranchAddress("trackDeDx1",&trackDeDx1);
    tree->SetBranchAddress("trackDeDx2",&trackDeDx2);
    tree->SetBranchAddress("trackDeDx3",&trackDeDx3);
    tree->SetBranchAddress("trackDeDx4",&trackDeDx4);
    tree->SetBranchAddress("trackDx1",&trackDx1);
    tree->SetBranchAddress("trackDx2",&trackDx2);
    tree->SetBranchAddress("trackDx3",&trackDx3);
    tree->SetBranchAddress("trackDx4",&trackDx4);
    tree->SetBranchAddress("trackMeasSize",&trackMeasSize);
  };


  void Selection(double ptCutMin=20, double ptCutMax=50.){

    statistics = 0;
    purity=0;
    double auxWeight = 0;
    
    histoBefore= new TH1D("histoBefore" + (TString) file->GetName(), "histoBefore",1,0,1);
    histoBefore->Sumw2();
    histoAfter= new TH1D("histoAfter" + (TString) file->GetName(), "histoAfter",1,0,1);
    histoAfter->Sumw2();

    //cout<<"N events = "<<tree->GetEntries()<<endl;
    
    for(int n=0; n<tree->GetEntries(); n++){

      tree->GetEntry(n);

      auxWeight = weight_xsec_lumi*weight;
      auxWeight = 1;


      //if(met<100)           continue;
      //if(leadingJetPt<110)  continue;
	
      for(unsigned int i=0; i<trackEta->size(); i++){

	if(abs(trackEta->at(i))>2.1)                                   continue;
	if(abs(trackEta->at(i))>1.42 && abs(trackEta->at(i))<1.65 )    continue;
	
	if(trackPt->at(i)<ptCutMin)                                    continue;
	if(trackPt->at(i)>ptCutMax)                                    continue;
			
	if(trackNLostMiddle->at(i)   >0)                                    continue;
	
	histoBefore->Fill(0.5,auxWeight);
	if(trackNLostInner->at(i)  >0)                                    continue;
	histoAfter->Fill(0.5,auxWeight);

	break;
      } 

    }
  };
};


int getSysUncertaintyInner(double ptCutMin, double ptCutMax){

  TString particle = "";

  // %%%%%%%%%%  Uncertainty of leptonic scale factors  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  sample data;
  sample wjets;

  wjets.file      =  new TFile(inputNmissUnc + "/wjets.root","READ");
  data.file       =  new TFile(inputNmissUnc + "/data.root","READ");
  
  TString select = "chiTrackspreselection/Variables";

  wjets.file    -> GetObject(select , wjets.tree);
  data.file     -> GetObject(select , data.tree);
  wjets.getTreeVariables();
  data.getTreeVariables();

  wjets.Selection(  ptCutMin, ptCutMax );
  data.Selection(   ptCutMin, ptCutMax );


  cout<<"wjets.histoBefore->GetBinContent(1)      = "<<wjets.histoBefore->GetBinContent(1)<<endl;
  cout<<"wjets.histoAfter->GetBinContent(1)       = "<<wjets.histoAfter->GetBinContent(1)<<endl;
  cout<<"data.histoBefore->GetBinContent(1)       = "<<data.histoBefore->GetBinContent(1)<<endl;
  cout<<"data.histoAfter->GetBinContent(1)        = "<<data.histoAfter->GetBinContent(1)<<endl;

  
  
  
  double effMC             =  wjets.histoAfter->GetBinContent(1)/wjets.histoBefore->GetBinContent(1);
  double effMCError        =  effMC * sqrt( pow( wjets.histoBefore->GetBinError(1)/wjets.histoBefore->GetBinContent(1) , 2)  +  pow(wjets.histoAfter->GetBinError(1)/wjets.histoAfter->GetBinContent(1) , 2) );
  double effData           =  data.histoAfter->GetBinContent(1)/data.histoBefore->GetBinContent(1);
  double effDataError      =  effMC * sqrt( pow( data.histoBefore->GetBinError(1)/data.histoBefore->GetBinContent(1) , 2)  +  pow(data.histoAfter->GetBinError(1)/data.histoAfter->GetBinContent(1) , 2) );

  double ratio             = effData/effMC;
  double ratioError        = effData/effMC * sqrt( pow(effDataError/effData,2)  + pow(effMCError/effMC,2) ) ;

  double uncertainty = (max(abs(ratio-ratioError-1),abs(ratio+ratioError-1)));
  cout<<"###########################################################################"<<endl;
  cout.precision(3);
  cout<<"Efficiency of cut on missing inner hits:"<<endl;
  cout<<"MC         = "<<effMC<<" +/- "<<effMCError<<endl;
  cout<<"Data       = "<<effData<<" +/- "<<effDataError<<endl;
  cout<<"-------------------------------------------------"<<endl;
  cout<<"Data/MC    = "<<ratio<<" +/- "<<ratioError<<endl;
  cout.precision(5);
  cout<<endl<<"Final Uncertainty = "<<fixed<<uncertainty<<endl;
  cout<<"###########################################################################"<<endl;
  
  return 0;
}

//################################################################################################################################
//################################################################################################################################
