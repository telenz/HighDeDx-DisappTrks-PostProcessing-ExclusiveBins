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
#include "TVector2.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include "../../../plotStyle.h"

TCanvas *drawPlot(TH1D *histo, TString xTitle, TString filename);

class sample{


public:

  TFile *file;
  TTree *tree;

  TH1D* histoReco;
  TH1D* histoAll;
  TH1D* hNhits;

  double statistics;
  double purity;

  vector<double> *trackEta;
  vector<double> *trackPhi;
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

  std::vector<Double_t>  *muonPt;
  std::vector<Double_t>  *muonEt;
  std::vector<Double_t>  *muonPhi;
  std::vector<Double_t>  *muonEta;
  std::vector<Double_t>  *muonChi2;
  std::vector<Double_t>  *muonndof;
  std::vector<Double_t>  *muondB;
  std::vector<Double_t>  *muond0;
  std::vector<Double_t>  *muonVertex_z;
  std::vector<Double_t>  *muonNOfValidMuonHits;
  std::vector<Double_t>  *muonTrackerLayersWithMeas;
  std::vector<Double_t>  *muonNOfValidPixelHits;
  std::vector<Double_t>  *muonNOfMatchedMuonStations;
  std::vector<Double_t>  *muonChargedHadronIso;
  std::vector<Double_t>  *muonNeutralHadronIso;
  std::vector<Double_t>  *muonPhotonIso;
  std::vector<Double_t>  *muonpuChargedHadronIso;
  std::vector<Int_t>  *muonPdgId;
  std::vector<Int_t>  *muonStatus;


  sample(){

    tree=0;
    file=0;
    histoReco=0;
    histoAll=0;
    hNhits=0;
    statistics=0;
    purity=0;

    trackEta        = 0;
    trackPhi        = 0;
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
    muonPt = 0;
    muonEt = 0;
    muonPhi = 0;
    muonEta = 0;
    muonChi2 = 0;
    muonndof = 0;
    muondB = 0;
    muond0 = 0;
    muonVertex_z = 0;
    muonNOfValidMuonHits = 0;
    muonTrackerLayersWithMeas = 0;
    muonNOfValidPixelHits = 0;
    muonNOfMatchedMuonStations = 0;
    muonChargedHadronIso = 0;
    muonNeutralHadronIso = 0;
    muonPhotonIso = 0;
    muonpuChargedHadronIso = 0;
    muonPdgId = 0;
    muonStatus = 0;
  };

  void getTreeVariables(){
    tree->SetBranchAddress("trackEta",&trackEta);
    tree->SetBranchAddress("trackPhi",&trackPhi);
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
    tree->SetBranchAddress("muonPt",&muonPt);
    tree->SetBranchAddress("muonEt",&muonEt);
    tree->SetBranchAddress("muonPhi",&muonPhi);
    tree->SetBranchAddress("muonEta",&muonEta);
    tree->SetBranchAddress("muonChi2",&muonChi2);
    tree->SetBranchAddress("muonndof",&muonndof);
    tree->SetBranchAddress("muondB",&muondB);
    tree->SetBranchAddress("muond0",&muond0);
    tree->SetBranchAddress("muonVertex_z",&muonVertex_z);
    tree->SetBranchAddress("muonNOfValidMuonHits",&muonNOfValidMuonHits);
    tree->SetBranchAddress("muonTrackerLayersWithMeas",&muonTrackerLayersWithMeas);
    tree->SetBranchAddress("muonNOfValidPixelHits",&muonNOfValidPixelHits);
    tree->SetBranchAddress("muonNOfMatchedMuonStations",&muonNOfMatchedMuonStations);
    tree->SetBranchAddress("muonChargedHadronIso",&muonChargedHadronIso);
    tree->SetBranchAddress("muonNeutralHadronIso",&muonNeutralHadronIso);
    tree->SetBranchAddress("muonPhotonIso",&muonPhotonIso);
    tree->SetBranchAddress("muonpuChargedHadronIso",&muonpuChargedHadronIso);
    tree->SetBranchAddress("muonPdgId",&muonPdgId);
    tree->SetBranchAddress("muonStatus",&muonStatus);
  };


  void Selection(double ptCut=20){

    statistics = 0;
    purity=0;
    double auxWeight = 0;
    
    histoReco= new TH1D("histoReco","histoReco",1,0,1);
    histoReco->Sumw2();
    histoAll= new TH1D("histoAll","histoAll",1,0,1);
    histoAll->Sumw2();
    hNhits= new TH1D("hNHits","hNhits",25,1,25);
    hNhits->Sumw2();

    //cout<<"N events = "<<tree->GetEntries()<<endl;

    for(int n=0; n<tree->GetEntries(); n++){

      tree->GetEntry(n);

      auxWeight = weight_xsec_lumi*weight;
      //auxWeight = 1;

      if(muonPt->at(0)<ptCut)                                       continue;

      double dRsaved = 10000;
      int idx =-10;
      for(unsigned int i=0; i<trackEta->size(); i++){

	if(abs(trackEta->at(i))>2.1)                                   continue;
	if(abs(trackEta->at(i))>1.42 && abs(trackEta->at(i))<1.65 )    continue;
	
	if(trackPt->at(i)<ptCut)                                       continue;

	// Try to match muon to trk
	double dPhi = std::abs(TVector2::Phi_mpi_pi(trackPhi->at(i) - muonPhi->at(0)));
	double dEta = std::abs(trackEta->at(i) - muonEta->at(0));
	double dR   = std::sqrt(dPhi*dPhi + dEta*dEta); 

	if(dR<dRsaved){

	  dRsaved = dR;
	  idx     = i;

	}
	
      }
      
      if(dRsaved < 0.15){
	histoReco    -> Fill(0.5, auxWeight);
	hNhits       -> Fill(trackNValid->at(idx), auxWeight);
      }
      histoAll -> Fill(0.5, auxWeight);
      
    }

    histoReco    -> Divide(histoAll);

  };
};


int getSysUncertainty(double ptCut){

  TString particle = "";

  // %%%%%%%%%%  Uncertainty of leptonic scale factors  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  sample data;
  sample wjets;

  wjets.file      =  new TFile("/afs/desy.de/user/t/tlenz/xxl-af-cms/DYTOLLANALYSIS/workdir/analysis_2015_07_07_trkRecEffUnc/results/analyzer/ntuples/input_weighted/dytollAODSIM.root","READ");
  data.file       =  new TFile("/afs/desy.de/user/t/tlenz/xxl-af-cms/DYTOLLANALYSIS/workdir/analysis_2015_07_07_trkRecEffUnc/results/analyzer/ntuples/input_weighted/SingleMu.root","READ");
  
  TString select = "ReconstructionEfficiencyTagAndProbe/Variables";

  wjets.file    -> GetObject(select , wjets.tree);
  data.file     -> GetObject(select , data.tree);
  wjets.getTreeVariables();
  data.getTreeVariables();

  wjets.Selection(  ptCut );
  data.Selection(   ptCut );


  
  double recoEffMC             =  wjets.histoReco->GetBinContent(1);
  double recoEffMCError        =  wjets.histoReco->GetBinError(1);
  double recoEffData           =  data.histoReco->GetBinContent(1);
  double recoEffDataError      =  data.histoReco->GetBinError(1);

  double ratio             = recoEffData/recoEffMC;
  double ratioError        = recoEffData/recoEffMC * sqrt( pow(recoEffDataError/recoEffData,2)  + pow(recoEffMCError/recoEffMC,2) ) ;

  double uncertainty = (max(abs(ratio-ratioError-1),abs(ratio+ratioError-1)));
  cout<<"###########################################################################"<<endl;
  cout.precision(3);
  cout<<"Track reconstruction efficiency in MC and data:"<<endl;
  cout<<"MC         = "<<recoEffMC<<" +/- "<<recoEffMCError<<endl;
  cout<<"Data       = "<<recoEffData<<" +/- "<<recoEffDataError<<endl;
  cout<<"-------------------------------------------------"<<endl;
  cout<<"Data/MC    = "<<ratio<<" +/- "<<ratioError<<endl;
  cout.precision(5);
  cout<<endl<<"Final Uncertainty = "<<fixed<<uncertainty<<endl;
  cout<<"###########################################################################"<<endl;
  
  TCanvas *c = drawPlot(wjets.hNhits, "n_{hits}", "NumberOfHits_wjets.pdf");
  
  return 0;
}

//################################################################################################################################
//################################################################################################################################
TCanvas *drawPlot(TH1D *histo, TString xTitle, TString filename){

  gStyle -> SetPadLeftMargin(0.20);

  
  TCanvas *c = new TCanvas("c"+filename,"c",0,0,500,500);
  c->cd();

  histo->GetYaxis()->SetTitle(xTitle);
  histo->SetTitle("");
  //histo->SetLabelSize(0.1,"X");
  //histo->SetLabelSize(0.1,"Y");
  histo->SetTitleOffset(0.5,"Y");
  histo->SetTitleSize(0.15,"Y");


  histo->Draw("e");


  c->SaveAs(filename);

  return c;

}
//################################################################################################################################
//################################################################################################################################
