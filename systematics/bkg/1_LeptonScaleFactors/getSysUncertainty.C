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

  TH1D* histo;
  TH1D* histoASmiSR;
  TH1D* histoNValidSR;
  TH1D* histoECalo;
  TH1D* histoPt;

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
    histo=0;
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
    tree->SetBranchStatus("*",0);
    tree->SetBranchStatus("trackEta",1);
    tree->SetBranchAddress("trackEta",&trackEta);
    tree->SetBranchStatus("trackPt",1);
    tree->SetBranchAddress("trackPt",&trackPt);
    tree->SetBranchStatus("trackCaloIsolation",1);
    tree->SetBranchAddress("trackCaloIsolation",&trackCaloIso);
    tree->SetBranchStatus("trackNValid",1);
    tree->SetBranchAddress("trackNValid",&trackNValid);
    tree->SetBranchStatus("trackPdgId",1);
    tree->SetBranchAddress("trackPdgId",&trackPdgId);
    tree->SetBranchStatus("weight*",1);
    tree->SetBranchAddress("weight",&weight);
    tree->SetBranchAddress("weight_xsec_lumi",&weight_xsec_lumi);
  };

  void Selection(bool doMatching=false, bool doEcaloCut=false, int pdgId=13, double ptCutMin=20, double ptCutMax=50, double ecaloCut=5){

    statistics = 0;
    purity=0;
    double auxWeight = 0;
    


    histo= new TH1D("histo","histo",1,0,1);
    histo->Sumw2();

    //cout<<"N events = "<<tree->GetEntries()<<endl;
    
    for(int n=0; n<tree->GetEntries(); n++){

      tree->GetEntry(n);

      auxWeight = weight_xsec_lumi*weight;
      
      //if(trackEta->size()>1) cout<<"trackEta->size() = "<<trackEta->size()<<endl;
      for(unsigned int i=0; i<trackEta->size(); i++){


	if(trackNValid->at(i)<6)                                     continue;

	if(abs(trackEta->at(i))>2.1)                                 continue;
	if(abs(trackEta->at(i))>1.42 && abs(trackEta->at(i))<1.65 )  continue;
	
	if(trackPt->at(i)<ptCutMin)                                     continue;
	if(trackPt->at(i)>ptCutMax)                                     continue;
	if(doEcaloCut){
	  if(trackCaloIso->at(i)>ecaloCut)                             continue;
	}
	
	// ****************************************** Particle Id **********************************
	purity+=auxWeight;
	if(doMatching){

	  if(abs(trackPdgId->at(i))!=pdgId){
	    //cout<<"pdgId = "<<abs(trackPdgId->at(i))<<endl;
	    continue; 
	  }
	}
	statistics +=auxWeight;
	histo -> Fill(0.5,auxWeight);
	break;
	// ****************************************** Particle Id **********************************
      }

    }
    purity=statistics/purity;
    //cout<<"purity = "<<purity<<endl;

  };
};


int getSysUncertainty(int pdgId, double ptCutMin, double ptCutMax, double ecaloCut){

  TString particle = "";

  // %%%%%%%%%%  Uncertainty of leptonic scale factors  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  sample dataSR;
  sample dataCR;
  sample mcSR;
  sample mcCR;

  sample wjetsSR;
  sample wjetsCR;

  if(pdgId==11){

    dataSR.file  =  new TFile(inputScaleUnc + "/SingleElectron.root","READ");
    dataCR.file  =  new TFile(inputScaleUncElectron + "/SingleElectron.root","READ");
    mcCR.file    =  new TFile(inputScaleUncElectron + "/dytollAODSIM.root","READ");
    wjetsCR.file =  new TFile(inputScaleUncElectron + "/wjetsAODSIM.root","READ");
    particle     =  "Electron";

  }
  else if(pdgId==13){

    dataSR.file  =  new TFile(inputScaleUnc + "SingleMu.root","READ");
    dataCR.file  =  new TFile(inputScaleUncMuon + "/SingleMu.root","READ");
    mcCR.file    =  new TFile(inputScaleUncMuon + "/dytollAODSIM.root","READ");
    wjetsCR.file =  new TFile(inputScaleUncMuon + "/wjetsAODSIM.root","READ");
    particle     =  "Muon";

  }
  else if(pdgId==211){

    dataSR.file  =  new TFile(inputScaleUnc + "/SingleMu.root","READ");
    dataCR.file  =  new TFile(inputScaleUncPion + "/SingleMu.root","READ");
    mcCR.file    =  new TFile(inputScaleUncPion + "/dytollAODSIM.root","READ");
    wjetsCR.file =  new TFile(inputScaleUncPion + "/wjetsAODSIM.root","READ");
    particle     =  "Tau";
    
  }
  else{

    cout<<" Unknown lepton !"<<endl;
    return 1;

  }
  
  TString select = "chiTrackspreselectionTagAndProbe" + particle + "/Variables";

  dataSR.file -> GetObject(select , dataSR.tree);
  dataSR.getTreeVariables();
  dataCR.file -> GetObject(select , dataCR.tree);
  dataCR.getTreeVariables();

  mcSR.file = new TFile(inputScaleUnc + "/dytollAODSIM.root","READ");
  mcSR.file -> GetObject(select , mcSR.tree);
  mcSR.getTreeVariables();

  mcCR.file -> GetObject(select , mcCR.tree);
  mcCR.getTreeVariables();

  wjetsSR.file = new TFile(inputScaleUnc + "/wjetsAODSIM.root","READ");
  wjetsSR.file -> GetObject(select , wjetsSR.tree);
  wjetsSR.getTreeVariables();

  wjetsCR.file -> GetObject(select , wjetsCR.tree);
  wjetsCR.getTreeVariables();

  // Different selections
  // For electrons and taus also the ecalo cut is removed in CRs
  
  cout<<endl<<"Selection data SR:"<<endl;
  dataSR.Selection(0, 1, pdgId, ptCutMin, ptCutMax, ecaloCut);
  
  cout<<endl<<"Selection data CR:"<<endl;
  if(pdgId==13) dataCR.Selection(0, 1, pdgId, ptCutMin, ptCutMax, ecaloCut);
  else          dataCR.Selection(0, 0, pdgId, ptCutMin, ptCutMax, ecaloCut);
  
  cout<<endl<<"Selection DytoLL MC SR:"<<endl;
  mcSR.Selection(  1, 1, pdgId, ptCutMin, ptCutMax, ecaloCut);
  
  cout<<endl<<"Selection DytoLL MC CR:"<<endl;
  if(pdgId==13) mcCR.Selection(  1, 1, pdgId, ptCutMin, ptCutMax, ecaloCut);
  else          mcCR.Selection(  1, 0, pdgId, ptCutMin, ptCutMax, ecaloCut);
  
  cout<<endl<<"Selection Wjets MC SR:"<<endl;
  wjetsSR.Selection(  0, 1, pdgId, ptCutMin, ptCutMax, ecaloCut);
  
  cout<<endl<<"Selection Wjets MC CR:"<<endl;
  if(pdgId==13) wjetsCR.Selection(  0, 1, pdgId, ptCutMin, ptCutMax, ecaloCut);
  else          wjetsCR.Selection(  0, 0, pdgId, ptCutMin, ptCutMax, ecaloCut);

  
  
  double nSRData           =  dataSR.histo->GetBinContent(1);
  double nSRDataError      =  dataSR.histo->GetBinError(1);
  double nCRData           =  dataCR.histo->GetBinContent(1);
  double nCRDataError      =  dataCR.histo->GetBinError(1);
  double nSRMC             =  mcSR.histo->GetBinContent(1);
  double nSRMCError        =  mcSR.histo->GetBinError(1);
  double nCRMC             =  mcCR.histo->GetBinContent(1);
  double nCRMCError        =  mcCR.histo->GetBinError(1);
  double nSRWjets          =  wjetsSR.histo->GetBinContent(1);
  double nSRWjetsError     =  wjetsSR.histo->GetBinError(1);
  double nCRWjets          =  wjetsCR.histo->GetBinContent(1);
  double nCRWjetsError     =  wjetsCR.histo->GetBinError(1);

  // Remove contamination from other particles in data:
  //nSRData = nSRData*mcSR.purity;
  //nCRData = nCRData*mcCR.purity;


  double sData          = nSRData/nCRData;
  double sDataError     = sData * sqrt( pow(nSRDataError/nSRData , 2)  +  pow(nCRDataError/nCRData , 2) );
  double sMC            = nSRMC/nCRMC;
  double sMCError       = sMC * sqrt( pow(nSRMCError/nSRMC , 2)  +  pow(nCRMCError/nCRMC , 2) );

  double ratio      = sMC/sData;
  double ratioError = ratio * sqrt( pow(sMCError/sMC , 2)  +  pow(sDataError/sData , 2) );


  cout<<"###########################################################################"<<endl;
  cout.precision(1);


  cout<<fixed<<"n SR Data     = "<<nSRData<<" +/- "<<nSRDataError<<"  \\\\"<<endl;
  cout<<fixed<<"n CR Data     = "<<nCRData<<" +/- "<<nCRDataError<<"  \\\\"<<endl;
  cout<<fixed<<"n SR DytoLL   = "<<nSRMC  <<" +/- "<<nSRMCError  <<"  \\\\"<<endl;
  cout<<fixed<<"n CR DytoLL   = "<<nCRMC  <<" +/- "<<nCRMCError  <<"  \\\\"<<endl;
  cout<<fixed<<"n SR Wjets    = "<<nSRWjets  <<" +/- "<<nSRWjetsError  <<"  \\\\"<<endl;
  cout<<fixed<<"n CR Wjets    = "<<nCRWjets  <<" +/- "<<nCRWjetsError  <<"  \\\\"<<endl<<endl;


  cout.precision(2);
  cout<<scientific<<"s Data   = "<<sData<<" +/-"<<sDataError <<"  \\\\"<<endl;
  cout<<scientific<<"s DytoLL = "<<sMC  <<" +/-"<<sMCError   <<"  \\\\"<<endl<<endl;
  
  cout<<fixed<<"rel. Difference = "<<ratio<<" +/- "<<ratioError<<"  \\\\"<<endl;
  double uncertainty = (max(abs(ratio-ratioError-1),abs(ratio+ratioError-1)));
  cout<<fixed<<"Uncertainty     = "<<uncertainty*100<<"\\%"<<"  \\\\"<<endl<<endl;

  cout.precision(0);
  cout<<fixed<<"Purity in DytoLL SR = "<<mcSR.purity*100<<"\\%"<<"  \\\\"<<endl;
  cout<<fixed<<"Purity in DytoLL CR = "<<mcCR.purity*100<<"\\%"<<"  \\\\"<<endl;
  cout<<"###########################################################################"<<endl;

  cout.precision(5);
  cout<<"Final Uncertainty = "<<fixed<<uncertainty<<endl;
  return 0;
 
}

//################################################################################################################################
//################################################################################################################################
