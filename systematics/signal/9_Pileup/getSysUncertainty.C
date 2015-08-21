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


  void Selection(double ptCutMin=20, double ptCutMax=50., double ecaloCut=5, double iasCutMin=0.1, double iasCutMax=0.99){

    statistics = 0;
    purity=0;
    double auxWeight = 0;
    


    histo= new TH1D("histo" + (TString) file->GetName(),"histo",1,0,1);
    histo->Sumw2();

    //cout<<"N events = "<<tree->GetEntries()<<endl;
    
    for(int n=0; n<tree->GetEntries(); n++){

      tree->GetEntry(n);

      auxWeight = weight_xsec_lumi*weight;


      if(met<100)           continue;
      if(leadingJetPt<110)  continue;
	
      for(unsigned int i=0; i<trackEta->size(); i++){

	if(abs(trackEta->at(i))>2.1)                                   continue;
	if(abs(trackEta->at(i))>1.42 && abs(trackEta->at(i))<1.65 )    continue;
	
	if(trackPt->at(i)<ptCutMin)                                    continue;
	if(trackPt->at(i)>ptCutMax)                                    continue;
	if(trackCaloIso->at(i)>ecaloCut)                               continue;
	if(trackASmi->at(i)<iasCutMin)                                 continue;
	if(trackASmi->at(i)>iasCutMax)                                 continue;
	
	
	histo -> Fill(0.5,auxWeight);
	break;
      } 

    }
  };
};


int getSysUncertainty(double ptCutMin, double ptCutMax, double ecaloCut, double iasCutMin, double iasCutMax, TString inputSample){

  TString particle = "";

  // %%%%%%%%%%  Uncertainty of leptonic scale factors  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  sample up;
  sample down;
  sample central;


  up.file         =  new TFile(inputPUup      + "/" + inputSample,"READ");
  down.file       =  new TFile(inputPUdown    + "/" + inputSample,"READ");
  central.file    =  new TFile(inputPUcentral + "/" + inputSample,"READ");
  
  TString select = "chiTrackspreselectionTrigger/Variables";

  up.file       -> GetObject(select , up.tree);
  down.file     -> GetObject(select , down.tree);
  central.file  -> GetObject(select , central.tree);
  up.getTreeVariables();
  down.getTreeVariables();
  central.getTreeVariables();

  up.Selection(     ptCutMin, ptCutMax, ecaloCut, iasCutMin, iasCutMax);
  down.Selection(   ptCutMin, ptCutMax, ecaloCut, iasCutMin, iasCutMax);
  central.Selection( ptCutMin, ptCutMax, ecaloCut, iasCutMin, iasCutMax);
  

  cout<<"up.histo->GetBinContent(1)      = "<<up.histo->GetBinContent(1)<<endl;
  cout<<"down.histo->GetBinContent(1)    = "<<down.histo->GetBinContent(1)<<endl;
  cout<<"central.histo->GetBinContent(1) = "<<central.histo->GetBinContent(1)<<endl;
  

  double nUp               =  up.histo->GetBinContent(1);
  double nUpError          =  up.histo->GetBinError(1);
  double nDown             =  down.histo->GetBinContent(1);
  double nDownError        =  down.histo->GetBinError(1);
  double nCentral          =  central.histo->GetBinContent(1);
  double nCentralError     =  central.histo->GetBinError(1);

  double upUnc           = abs(nUp/nCentral - 1);
  double upUncError      = upUnc * sqrt( pow(nUpError/nUp , 2)  +  pow(nCentralError/nCentral , 2) );
  double downUnc         = abs(nDown/nCentral - 1);
  double downUncError    = downUnc * sqrt( pow(nDownError/nDown , 2)  +  pow(nCentralError/nCentral , 2) );


  cout<<"###########################################################################"<<endl;
  cout.precision(1);


  cout<<fixed<<"upward uncertainty     = "<<upUnc*100<<" +/- "<<upUncError*100<<" %\\\\"<<endl;
  cout<<fixed<<"downward uncertainty   = "<<downUnc*100<<" +/- "<<downUncError*100<<" %\\\\"<<endl;

  cout.precision(3);
  cout<<"upUnc = "<<upUnc<<endl;
  cout<<"upUncError = "<<upUncError<<endl;
  double Upuncertainty = (max(abs(upUnc-upUncError),abs(upUnc+upUncError)));
  cout<<fixed<<"Uncertainty upward     = "<<Upuncertainty*100<<"\\%"<<"  \\\\"<<endl<<endl;
 
  cout<<"downUnc = "<<downUnc<<endl;
  cout<<"downUncError = "<<downUncError<<endl;
  double Downuncertainty = (max(abs(downUnc-downUncError),abs(downUnc+downUncError)));
  cout<<fixed<<"Uncertainty downward     = "<<Downuncertainty*100<<"\\%"<<"  \\\\"<<endl<<endl;

  double uncertainty = (Downuncertainty+Upuncertainty)/2.;
  cout<<"Final Uncertainty = "<<fixed<<uncertainty<<endl;
  cout<<"###########################################################################"<<endl;

  return 0;
 
}

//################################################################################################################################
//################################################################################################################################
