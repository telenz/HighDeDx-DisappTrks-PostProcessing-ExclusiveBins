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
    tree->SetBranchStatus("MET",1);
    tree->SetBranchAddress("MET",&met);
    tree->SetBranchStatus("LeadingJetPt",1);
    tree->SetBranchAddress("LeadingJetPt",&leadingJetPt);
  };


  void Selection(bool doKinSelection=false, bool doTrkSelection=false, double ptCutMin=20, double ptCutMax=50, double ecaloCut=5){

    statistics = 0;
    purity=0;
    double auxWeight = 0;

    histo= new TH1D("count","count",1,0,1);
    histo->Sumw2();

    cout<<"N events = "<<tree->GetEntries()<<endl;
    
    for(int n=0; n<tree->GetEntries(); n++){

      tree->GetEntry(n);

      auxWeight = weight_xsec_lumi*weight;


      if(doKinSelection){
	if(met<0)                         continue;
	if(leadingJetPt<70)                continue;	
      }


      if(doTrkSelection){


	for(unsigned int i=0; i<trackEta->size(); i++){

	  if(abs(trackEta->at(i))>2.1)                                 continue;
	  if(abs(trackEta->at(i))>1.42 && abs(trackEta->at(i))<1.65 )  continue;
	
	  if(trackPt->at(i)<ptCutMin)                                  continue;
	  if(trackPt->at(i)>ptCutMax)                                  continue;
	  if(trackCaloIso->at(i)>ecaloCut)                             continue;
	
	  // ****************************************** Particle Id **********************************
	  purity+=auxWeight;
	  if(abs(trackPdgId->at(i))!=0)      continue; 
	  statistics +=auxWeight;
	  histo -> Fill(0.5,auxWeight);
	  break;
	  // ****************************************** Particle Id **********************************
	} 

      }
      else{
	//for(unsigned int i=0; i<trackEta->size(); i++){
	statistics +=auxWeight;
	histo -> Fill(0.5,auxWeight);
	//}
      }
      purity=statistics/purity;
    }
  };
};


int getSysUncertainty(double ptCutMin, double ptCutMax, double ecaloCut){


  // %%%%%%%%%%  Fake rate from Wjets MC  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Uncertainty estimation is done by comparing fake rate (=#fakes/#events) in wjets mc and z->ll mc
  sample wjetsKin;
  sample wjetsDT;

  wjetsKin.file = new TFile(fileSR + "/wjets.root","READ");
  //wjetsKin.file -> GetObject("chiTracksQCDsupression/Variables",wjetsKin.tree);
  wjetsKin.file -> GetObject("chiTrackstriggerRequirements/Variables",wjetsKin.tree);
  wjetsKin.getTreeVariables();
  wjetsKin.file -> GetObject("chiTrackspreselectionNoQCDCuts/Variables",wjetsDT.tree);
  //wjetsKin.file -> GetObject("chiTrackspreselection/Variables",wjetsDT.tree);
  wjetsDT.getTreeVariables();

  wjetsKin.Selection(1,0);
  wjetsDT.Selection(1,1, ptCutMin, ptCutMax, ecaloCut);

  
  cout<<"###########################################################################"<<endl;
  cout.precision(2);
  double intKinError = 0;
  double intKin      = wjetsKin.histo -> IntegralAndError(1,1,intKinError);
  double intDTError = 0;
  double intDT      = wjetsDT.histo -> IntegralAndError(1,1,intDTError);
  cout<<scientific<<"Number of events in kin. selection = "<<intKin<<" +/- "<<intKinError<<endl;
  cout<<            "wjets: Number of fakes             = "<<intDT<<" +/- "<<intDTError<<endl;
  if(intDT==0)       intDT = 1.15*14.52;
  double fakeRateWjets      = intDT/intKin; 
  double fakeRateWjetsError = fakeRateWjets * sqrt( pow(intKinError/intKin, 2)  +  pow(intDTError/intDT, 2) ); 
  cout<<"Fake rate from wjets MC            = "<<scientific<<fakeRateWjets<<" +/- "<<fakeRateWjetsError<<endl;
  cout<<"###########################################################################"<<endl;


  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //%%%%%%%%%%  Fake rate from Zmumu MC  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //
  sample dytomumuKin;
  sample dytomumuDT;

  dytomumuKin.file = new TFile(fileDYToLL + "/dytollAODSIM.root","READ");
  dytomumuKin.file -> GetObject("chiTracksnoSelectionTightMuons/Variables",dytomumuKin.tree);
  dytomumuKin.getTreeVariables();
  dytomumuKin.file -> GetObject("chiTrackspreselectionTightMuons/Variables",dytomumuDT.tree);
  dytomumuDT.getTreeVariables();

  dytomumuKin.Selection(0, 0);
  dytomumuDT.Selection(0, 1, ptCutMin, ptCutMax, ecaloCut);

  cout<<"###########################################################################"<<endl;
  cout.precision(2);
  double intKinErrorMuMu = 0;
  double intKinMuMu      = dytomumuKin.histo -> IntegralAndError(1,1,intKinErrorMuMu);
  double intDTErrorMuMu = 0;
  double intDTMuMu      = dytomumuDT.histo -> IntegralAndError(1,1,intDTErrorMuMu);
  cout<<scientific<<"Number of events in kin. selection = "<<intKinMuMu<<" +/- "<<intKinErrorMuMu<<endl;
  cout<<            "dytomumu: Number of fakes             = "<<intDTMuMu<<" +/- "<<intDTErrorMuMu<<endl;
  if(intDT==0)       intDT = 1.15*14.52;
  double fakeRateDytomumu      = intDTMuMu/intKinMuMu; 
  double fakeRateDytomumuError = intDTMuMu/intKinMuMu * sqrt( pow(intKinErrorMuMu/intKinMuMu, 2)  +  pow(intDTErrorMuMu/intDTMuMu, 2) ); 
  cout<<"Fake rate from dytomumu MC            = "<<scientific<<fakeRateDytomumu<<" +/- "<<fakeRateDytomumuError<<endl;
  cout<<"###########################################################################"<<endl;
  //%%%%%%%%%%  Fake rate from Zee MC   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // 
  sample dytoeeKin;
  sample dytoeeDT;

  dytoeeKin.file = new TFile(fileDYToLL + "/dytollAODSIM.root","READ");
  dytoeeKin.file -> GetObject("chiTracksnoSelectionTightElectrons/Variables",dytoeeKin.tree);
  dytoeeKin.getTreeVariables();
  dytoeeKin.file -> GetObject("chiTrackspreselectionTightElectrons/Variables",dytoeeDT.tree);
  dytoeeDT.getTreeVariables();

  dytoeeKin.Selection();
  dytoeeDT.Selection(0, 1, ptCutMin, ptCutMax, ecaloCut);

  cout<<"###########################################################################"<<endl;
  cout.precision(2);
  double intKinErrorEE = 0;
  double intKinEE      = dytoeeKin.histo -> IntegralAndError(1,1,intKinErrorEE);
  double intDTErrorEE  = 0;
  double intDTEE       = dytoeeDT.histo -> IntegralAndError(1,1,intDTErrorEE);
  cout<<scientific<<"Number of events in kin. selection = "<<intKinEE<<" +/- "<<intKinErrorEE<<endl;
  cout<<            "dytoee: Number of fakes            = "<<intDTEE<<" +/- "<<intDTErrorEE<<endl;
  if(intDT==0)       intDT = 1.15*14.52;
  double fakeRateDytoee      = intDTEE/intKinEE; 
  double fakeRateDytoeeError = intDTEE/intKinEE * sqrt( pow(intKinErrorEE/intKinEE, 2)  +  pow(intDTErrorEE/intDTEE, 2) ); 
  cout<<            "Fake rate from dytoee MC           = "<<scientific<<fakeRateDytoee<<" +/- "<<fakeRateDytoeeError<<endl;
  cout<<"###########################################################################"<<endl;

  //%%%%%%%%%%  Z->mumu  and Z->ee together  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  
  double dytollKin     = intKinMuMu + intKinEE;
  double dytollKinErr  = sqrt( pow( intKinErrorMuMu , 2)  +  pow( intKinErrorEE , 2) ) ;
  double dytollDT      = intDTMuMu + intDTEE;
  double dytollDTErr   = sqrt( pow( intDTErrorMuMu , 2)  +  pow( intDTErrorEE , 2) ) ;

  double fakeRateDytoll      = dytollDT/dytollKin;
  double fakeRateDytollError = dytollDT/dytollKin * sqrt( pow(dytollKinErr/dytollKin, 2)  +  pow(dytollDTErr/dytollDT, 2) ); 
  
  cout<<"###########################################################################"<<endl;
  cout.precision(2);
  cout<<scientific<<"Number of events in kin. selection = "<<dytollKin<<" +/- "<<dytollKinErr<<endl;
  cout<<            "dytoll: Number of fakes            = "<<dytollDT<< " +/-$ "<<dytollDTErr<<endl;
  cout<<            "Fake rate from dytoll MC           = "<<scientific<<fakeRateDytoll<<" +/- "<<fakeRateDytollError<<endl;
  cout<<"###########################################################################"<<endl;
  

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //%%%%%%%%%%  Rel Difference between fake rate of the two samples  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  cout<<"###########################################################################"<<endl;
  cout<<"Fake rate from wjets MC            = ("<<fixed<<fakeRateWjets*pow(10,6)<<" $\\pm$ "<<fakeRateWjetsError*pow(10,6)<<") $10^{-6}$"<<endl;
  cout<<"Fake rate from dytoll MC           = ("<<fixed<<fakeRateDytoll*pow(10,6)<<" $\\pm$ "<<fakeRateDytollError*pow(10,6)<<") $10^{-6}$"<<endl;
  double ratio = fakeRateWjets/fakeRateDytoll;
  double ratioError = ratio* sqrt(  pow( fakeRateWjetsError/fakeRateWjets ,2 )  +  pow( fakeRateDytollError/fakeRateDytoll ,2 ) );
  cout<<fixed<<"Ratio       = "<<ratio<<" +/- "<<ratioError<<" $\\rightarrow$ ";
  double uncertainty = (max(abs(ratio-ratioError-1),abs(ratio+ratioError-1)));
  cout<<fixed<<"Uncertainty = "<<uncertainty*100<<"\\%"<<endl;
  cout<<"###########################################################################"<<endl;
  cout.precision(5);
  cout<<"Final Uncertainty = "<<fixed<<uncertainty<<endl;
  
  return 0;
 
}

//################################################################################################################################
//################################################################################################################################
