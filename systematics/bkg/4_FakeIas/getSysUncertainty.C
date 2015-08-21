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


  void Selection(bool doKinSelection=false, bool doTrkSelection=false, double ptCutMin=20, double ptCutMax=50, double ecaloCut=5, double iasCutMin=0.1, double iasCutMax=0.99){

    statistics = 0;
    purity=0;
    double auxWeight = 0;

    double bins[4] = {0,iasCutMin, iasCutMax,1};
    histo= new TH1D("hASmi","hASmi",3,bins);
    histo->Sumw2();

    cout<<"N events = "<<tree->GetEntries()<<endl;
    
    for(int n=0; n<tree->GetEntries(); n++){

      tree->GetEntry(n);

      auxWeight = weight_xsec_lumi*weight;

      if(doKinSelection){
	if(met<100)                        continue;
	if(leadingJetPt<110)               continue;
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
	  histo -> Fill(trackASmi->at(i),auxWeight);
	  // ****************************************** Particle Id **********************************
	} 

      }
      purity=statistics/purity;
    }
  };
};


int getSysUncertainty(double ptCutMin, double ptCutMax, double ecaloCut, double iasCutMin, double iasCutMax){


  // %%%%%%%%%%  Uncertainty in Ias of fake tracks from Wjets MC  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  sample fakeSR;
  sample fakeCR;

  fakeSR.file = new TFile(fileSR + "/wjets.root","READ");
  fakeSR.file -> GetObject("chiTrackspreselectionNoQCDCutsNoTrigger/Variables",fakeSR.tree);
  fakeSR.getTreeVariables();
  fakeCR.file = new TFile(fileFakeCS + "/wjets.root","READ");
  fakeCR.file -> GetObject("chiTrackspreselectionNoQCDCutsNoTrigger/Variables",fakeCR.tree);
  fakeCR.getTreeVariables();

  fakeSR.Selection(0, 1, ptCutMin, ptCutMax, ecaloCut, iasCutMin, iasCutMax);
  fakeCR.Selection(0, 1, ptCutMin, ptCutMax, ecaloCut, iasCutMin, iasCutMax);

  fakeCR.histo->Scale(fakeSR.histo->Integral()/fakeCR.histo->Integral());

  cout<<"###########################################################################"<<endl;
  cout.precision(2);

  double highIasSR      = fakeSR.histo->GetBinContent(2);
  double highIasErrorSR = fakeSR.histo->GetBinError(2);
  double highIasCR      = fakeCR.histo->GetBinContent(2);
  double highIasErrorCR = fakeCR.histo->GetBinError(2);

  cout<<"Bin Content high Ias in SR = "<<highIasSR<<" +/-"<<highIasErrorSR<<endl;
  cout<<"Bin Content high Ias in CR = "<<highIasCR<<" +/-"<<highIasErrorCR<<endl;

  double ratio      = highIasSR/highIasCR;
  double ratioError = ratio* sqrt(  pow( highIasErrorCR/highIasCR ,2 )  +  pow( highIasErrorSR/highIasSR ,2 ) );

  cout<<fixed<<"Ratio       = "<<ratio<<" +/- "<<ratioError<<" $\\rightarrow$ ";
  double uncertainty = (max(abs(ratio-ratioError-1),abs(ratio+ratioError-1)));
  cout<<fixed<<"Uncertainty = "<<uncertainty*100<<"\\%"<<endl;
  cout<<"###########################################################################"<<endl;

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //%%%%%%%%%%  Rel Difference between fake rate of the two samples  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  /*
  cout<<"###########################################################################"<<endl;
  cout<<"Fake rate from wjets MC            = "<<scientific<<fakeRateWjets<<" +/- "<<fakeRateWjetsError<<endl;
  cout<<"Fake rate from dytoll MC           = "<<scientific<<fakeRateDytoll<<" +/- "<<fakeRateDytollError<<endl;
  double ratio = fakeRateWjets/fakeRateDytoll;
  double ratioError = ratio* sqrt(  pow( fakeRateWjetsError/fakeRateWjets ,2 )  +  pow( fakeRateDytollError/fakeRateDytoll ,2 ) );
  cout<<"Ratio = "<<ratio<<" +/- "<<ratioError<<endl;
  cout<<"###########################################################################"<<endl;
  */
  cout.precision(5);
  cout<<"Final Uncertainty = "<<fixed<<uncertainty<<endl;
  return 0;
 
}

//################################################################################################################################
//################################################################################################################################
