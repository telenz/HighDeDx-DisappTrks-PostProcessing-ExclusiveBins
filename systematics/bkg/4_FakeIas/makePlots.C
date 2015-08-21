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
#include "/afs/desy.de/user/t/tlenz/myAnalysis/bkgEstimation/plotStyle.h"


TCanvas *drawRatioPlot(TH1D *prediction, TH1D *sr, TString xTitle, TString filename);
TCanvas *drawRatioPlotWithPurity(TH1D *prediction, TH1D *sr, TString xTitle, TString filename, double purity, bool LowECaloRegion=true);
void getPoissonIntervalls(double mu1, double mu2);

class sample{


public:

  TFile *file;
  TTree *tree;

  TH1D* histo;
  TH1D* histoASmiSR;
  TH1D* histoASmi;
  TH1D* histoNValidSR;
  TH1D* histoECalo;
  TH1D* histoPt;

  int    statistics;
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


  void Selection(bool doMatching, double ptCut, double ecaloCut){

    histo->Sumw2();
    histoASmiSR->Sumw2();
    histoASmi->Sumw2();
    histoNValidSR->Sumw2();
    histoECalo->Sumw2();
    histoPt->Sumw2();

    histo->SetTitle("");

    statistics = 0;
    purity=0;

    for(int n=0; n<tree->GetEntries(); n++){

      tree->GetEntry(n);

      if(met<0)                        continue;
      //if(leadingJetPt<70)               continue;


      for(unsigned int i=0; i<trackEta->size(); i++){

	if(abs(trackEta->at(i))>2.1)                                 continue;
	if(abs(trackEta->at(i))>1.42 && abs(trackEta->at(i))<1.65 )  continue;
	if(trackASmi->at(i)<0.0)           continue;
    
	if(trackPt->at(i)<ptCut)          continue;
 	if(trackCaloIso->at(i)>ecaloCut)  continue;

	
      
	// ****************************************** Particle Id **********************************
	if(doMatching){
	  purity+=1;
	  if(abs(trackPdgId->at(i))!=0)      continue; 
	  statistics +=1;
	}
	// ****************************************** Particle Id **********************************
	histo         -> Fill(leadingJetPt,weight_xsec_lumi*weight);
	histoASmiSR   -> Fill(trackASmi->at(i),weight_xsec_lumi*weight);
	histoASmi     -> Fill(trackASmi->at(i),weight_xsec_lumi*weight);
	histoNValidSR -> Fill(trackNValid->at(i),weight_xsec_lumi*weight);
	histoECalo    -> Fill(trackCaloIso->at(i),weight_xsec_lumi);
	histoPt       -> Fill(trackPt->at(i),weight_xsec_lumi);
      }
    }

    purity=statistics/purity;
  };
};


int makePlots(double ptCut, double ecaloCut, double iasCut){

  sample pred;
  sample SR;
  sample data;

  SR.file   = new TFile("/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_04_METGt0_JetPtGt70_trackPtGt20/results/analyzer/ntuples/input_weighted/wjets.root","READ");
  data.file = new TFile("/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_04_METGt0_JetPtGt70_trackPtGt20_FakeCS/results/analyzer/ntuples/input_weighted/data.root","READ");
  //pred.file = new TFile("/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_15_METGt0_JetPtGt70_trackPtGt20_Nod0Cut_FakeCS/results/analyzer/ntuples/input_weighted/wjets.root","READ");
  pred.file = new TFile("/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_15_METGt0_JetPtGt70_trackPtGt20_d0CutNotInverted_FakeCS/results/analyzer/ntuples/input_weighted/wjets.root","READ");
  //pred.file = new TFile("/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_04_METGt0_JetPtGt70_trackPtGt20_FakeCS/results/analyzer/ntuples/input_weighted/wjets.root","READ");
  
  //SR.file   = new TFile("/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_05_22_METGt0_JetPtGt0_TauPtGt30_SubJetPtGt30/results/analyzer/ntuples/input_weighted/wjets.root","READ");
  //pred.file = new TFile("/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_04_20_METGt0_JetPtGt70_FakeCS_withData/results/analyzer/ntuples/input_weighted/wjets.root","READ");
  //pred.file = new TFile("/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_05_08_METGt0_JetPtGt0_FakeCS/results/analyzer/ntuples/input_weighted/wjets.root","READ");
 
 


  /*
  pred.file = new TFile("~/myAnalysis/bkgEstimation/input_fakeCS/wjets.root","READ");
  SR.file   = new TFile("/afs/desy.de/user/t/tlenz/xxl-af-cms/ANALYSIS/workdir/analysis_2015_04_25_METGt0_JetPtGt70_withData_andSignal/results/analyzer/ntuples/input_weighted/wjets.root","READ");
  data.file = new TFile("~/myAnalysis/bkgEstimation/input_fakeCS/MET_Run2012_22Jan2013.root");
  */
  //  pred.file   -> GetObject("chiTrackspreselectionNoQCDCutsNoTrigger/Variables",pred.tree);
  //SR.file     -> GetObject("chiTrackspreselectionNoQCDCutsNoTrigger/Variables",SR.tree);
  //data.file   -> GetObject("chiTrackspreselection/Variables",data.tree);
  pred.file   -> GetObject("chiTrackspreselectionQCDsupression/Variables",pred.tree);
  SR.file     -> GetObject("chiTrackspreselectionQCDsupression/Variables",SR.tree);
  data.file   -> GetObject("chiTrackspreselection/Variables",data.tree);

  pred.getTreeVariables();
  SR.getTreeVariables();
  data.getTreeVariables();

  int nbins=15;
  double xmin=50;
  double xmax=300;

  pred.histo = new TH1D("CR","CR",nbins,xmin,xmax);
  SR.histo   = new TH1D("SR","SR",nbins,xmin,xmax);
  data.histo = new TH1D("data","data",nbins,xmin,xmax);


  Double_t xbinsASmiSR[3]  = {0,iasCut,1};
  Double_t xbinsASmi[5]  = {0,0.05,0.2,0.4,1};
  Double_t xbinsNHits[4] = {3,5,7,25};
  Double_t xbinsECalo[3]  = {0,2,200};
  Double_t xbinsPt[6]  = {0,20,30,70,150,200};
  pred.histoASmiSR   = new TH1D("MC_CR","MC CR",2,xbinsASmiSR);
  pred.histoASmi     = new TH1D("MC_CR","MC CR",4,xbinsASmi);
  pred.histoNValidSR = new TH1D("MC_CR","MC CR",3,xbinsNHits);
  pred.histoECalo    = new TH1D("MC_CR","MC CR",2,xbinsECalo);
  pred.histoPt       = new TH1D("MC_CR","MC CR",5,xbinsPt);
  SR.histoASmiSR     = new TH1D("MC_SR","MC SR",2,xbinsASmiSR);
  SR.histoASmi       = new TH1D("MC_SR","MC SR",4,xbinsASmi);
  SR.histoNValidSR   = new TH1D("MC_SR","MC SR",3,xbinsNHits); 
  SR.histoECalo      = new TH1D("MC_SR","MC SR",2,xbinsECalo);
  SR.histoPt         = new TH1D("MC_SR","MC SR",5,xbinsPt);
  data.histoASmiSR   = new TH1D("data_CR","data CR",2,xbinsASmiSR);
  data.histoASmi     = new TH1D("data_CR","data CR",4,xbinsASmi);
  data.histoNValidSR = new TH1D("data_CR","data CR",3,xbinsNHits); 
  data.histoECalo    = new TH1D("data_CR","data CR",2,xbinsECalo);
  data.histoPt       = new TH1D("data_CR","data CR",5,xbinsPt);
 
  pred.Selection(1,ptCut,ecaloCut);
  SR.Selection(1,ptCut,ecaloCut);
  data.Selection(0,ptCut,ecaloCut);
  cout<<"###########################################################################"<<endl;
  cout<<"Statisitcs:"<<endl;
  cout<<"Signal region  = "<<SR.statistics<<endl;
  cout<<"Control region = "<<pred.statistics<<endl;
  cout<<"Purity: Signal region  = "<<SR.purity<<endl;
  cout<<"Purity: Control region = "<<pred.purity<<endl;
  cout<<"###########################################################################"<<endl;

  cout<<"Integral SR   = "<<SR.histoNValidSR->Integral()<<endl;
  cout<<"Integral pred = "<<pred.histoNValidSR->Integral()<<endl;
  cout<<"SR entries    = "<<SR.histoNValidSR->GetEntries()<<endl;
  cout<<"pred entries  = "<<pred.histoNValidSR->GetEntries()<<endl;
  cout<<"data entries  = "<<data.histoNValidSR->GetEntries()<<endl;

  // ***** Calculate scaling factor  ************************************************************************
  double PredIntErr = 0;
  double SRIntErr   = 0;
  double dataIntErr = 0;
  
  double PredInt = pred.histoASmiSR->IntegralAndError(1,pred.histoASmiSR->GetNbinsX(),PredIntErr);
  double SRInt   = SR.histoASmiSR->IntegralAndError(1,SR.histoASmiSR->GetNbinsX(),SRIntErr);
  double dataInt = data.histoASmiSR->IntegralAndError(1,data.histoASmiSR->GetNbinsX(),dataIntErr);
  
  double scalingFactor=SRInt/PredInt;
  double scalingFactorGaussianError = scalingFactor*sqrt(pow(SRIntErr/SRInt,2) + pow(PredIntErr/PredInt,2));
  cout<<"scaling factor = "<<scalingFactor<<" +/- "<<scalingFactorGaussianError<<endl;
  cout<<"Scaling factor from NValid = "<<SR.histoNValidSR->Integral()/pred.histoNValidSR->Integral()<<endl;

  //getPoissonIntervalls(SRInt,PredInt);
  //getPoissonIntervalls(SR.histoASmiSR->GetEntries(),pred.histoASmiSR->GetEntries());
  // ***********************************************************************************************************

  pred.histo->Scale(SR.histo->Integral()/pred.histo->Integral());
  pred.histoASmiSR->Scale(SR.histoASmiSR->Integral()/pred.histoASmiSR->Integral());
  pred.histoASmi->Scale(SR.histoASmi->Integral()/pred.histoASmi->Integral());
  pred.histoNValidSR->Scale(SR.histoNValidSR->Integral()/pred.histoNValidSR->Integral());
  data.histoASmiSR->Scale(SR.histoASmiSR->Integral()/data.histoASmiSR->Integral());
  pred.histoECalo->Scale(SR.histoECalo->Integral()/pred.histoECalo->Integral());
  pred.histoPt->Scale(SR.histoPt->Integral()/pred.histoPt->Integral());


  TeresaPlottingStyle::init();
  gROOT->ForceStyle();

  //TCanvas *c0  = drawRatioPlot(pred.histo,SR.histo,"Leading Jet PT fakes","hLeadingJetPt_fakes.pdf");
  TString filename = "hASmi_SRbinning_d0Inverted_fakes_";
  filename += Form("ECalaoLe%.0f_",ecaloCut);
  filename += Form("trackPtGt%.0f_",ptCut);

  TCanvas *c1 = drawRatioPlotWithPurity(pred.histoASmiSR,SR.histoASmiSR,"I_{as} fakes","plotsLLMeeting/" + filename + pred.histoASmiSR->GetName() + "_"  + SR.histoASmiSR->GetName() + ".pdf" ,pred.purity);
  filename = "hASmi_d0Inverted_fakes_";
  filename += Form("ECalaoLe%.0f_",ecaloCut);
  filename += Form("trackPtGt%.0f_",ptCut);
  TCanvas *c2 = drawRatioPlotWithPurity(pred.histoASmi,SR.histoASmi,"I_{as} fakes","plotsLLMeeting/" + filename + pred.histoASmi->GetName() + "_"  + SR.histoASmi->GetName() + ".pdf" ,pred.purity);
  //TCanvas *c1a = drawRatioPlotWithPurity(pred.histoPt,SR.histoPt,"pt fakes","trackPt_fakes" ,pred.purity,LowECaloRegion);
    //TCanvas *c3 = drawRatioPlotWithPurity(data.histoASmiSR,SR.histoASmiSR,"I_{as} fakes",filename + data.histoASmiSR->GetName() + "_"  + SR.histoASmiSR->GetName() + ".pdf" ,pred.purity,LowECaloRegion);
  //TCanvas *c4  = drawRatioPlot(pred.histoECalo,SR.histoECalo,"track E_{calo}","plots/htrackCaloIsolation_fakes_ECaloEq2.pdf");
  //TCanvas *c3 = drawRatioPlotWithPurity(pred.histoNValidSR,SR.histoNValidSR,"n_{Hits} fakes",Form("plotsLLMeeting/hNValid_SRbinning_d0Inverted_fakes_trackPtGt%.0f.pdf",ptCut),pred.purity);
 
  return 0;
 
}

//################################################################################################################################
//################################################################################################################################
//################################################################################################################################
TCanvas *drawRatioPlot(TH1D *prediction, TH1D *sr, TString xTitle, TString filename){

  
  TCanvas *c = new TCanvas("c"+filename,"c",0,0,500,500);

  float y = 0.3;

  TPad *pad1     = new TPad("pad1", "Control Plots 1", 0.01, y, 0.99, 0.99);
  TPad *padRatio = new TPad("rp1", "Ratio1", 0.01, 0.01, 0.99, y-0.01);

  TH1D *ratio = 0;

  ratio = (TH1D*) sr->Clone();
  ratio->Divide(prediction);
  ratio->GetYaxis()->SetTitle((TString) sr->GetTitle() + "/" + (TString) prediction->GetTitle());
  ratio->SetTitle("");
  ratio->SetLabelSize(0.1,"X");
  ratio->SetLabelSize(0.1,"Y");
  ratio->SetTitleOffset(0.5,"Y");
  ratio->SetTitleSize(0.15,"Y");


  padRatio->cd();
  ratio->GetYaxis()->SetRangeUser(0,2);
  ratio->Draw();

  // Draw line at one!
  float xmin = ratio->GetXaxis()->GetXmin();
  float xmax = ratio->GetXaxis()->GetXmax();
  TLine *line = new TLine(xmin,1,xmax,1);
  line->SetLineWidth(2);
  line->Draw("same");
  padRatio->Modified();

  TLegend *leg = new TLegend(0.6,0.8,0.9,0.9);
  leg->AddEntry(sr,"MC SR","l"); 
  leg->AddEntry(prediction,"MC CR","pel"); 
  pad1->cd();
  //pad1->SetLogy();

  sr->SetLineColor(kRed);
  sr->SetMarkerColor(kRed);
  sr->SetMarkerStyle(20);
  sr->SetTitle("");
  prediction->SetMarkerStyle(20);
  prediction->SetTitle("");
  prediction->GetXaxis()->SetTitle(xTitle);
  sr->GetXaxis()->SetTitle(xTitle);

  prediction->SetTitleSize(0.07,"X");
  prediction->GetXaxis()->SetTitleOffset(0.7);
  sr->SetTitleSize(0.07,"X");
  sr->GetXaxis()->SetTitleOffset(0.7);

  prediction->Draw("e");
  sr->Draw("e same");

  if(sr->GetMaximum()>prediction->GetMaximum()){

    sr->Draw("e");
    prediction->Draw("e same");

    }

  leg->Draw("same");




  // Draw both pads to canvas
  c->cd();
  pad1->Draw();
  padRatio->SetGridy();
  padRatio->Draw();

  c->SaveAs(filename);

  return c;

}

//################################################################################################################################
 // Get the 68% confindence interval of mu1/mu2
void getPoissonIntervalls(double mu1, double mu2){

  double result = mu1/mu2;
  TH1D *expHist = new TH1D("mu1/mu2","mu1/mu2",1000,0,1);
  TRandom3 rand1(0);
  TRandom3 rand2(0);

  cout<<"mu1 = "<<mu1<<endl;
  cout<<"mu2 = "<<mu2<<endl;
  cout<<"ratio = "<<result<<endl;

  for(int i=0; i<10000000; i++){

    expHist->Fill(1.*rand1.Poisson(mu1)/rand2.Poisson(mu2));
  }

  // Get now the 68% upper and lower interval
  double errUp  = 0;
  double errLow = 0;
  for(int i=1; i<=expHist->GetNbinsX();i++){

    double upperIntegral = expHist->Integral(i,expHist->GetNbinsX());
    double lowerIntegral = expHist->Integral(1,i);

    if(abs(upperIntegral/expHist->Integral()-0.32)<0.02){
      cout<<"upper bound = "<<expHist->GetBinCenter(i)<<endl;
      errUp = expHist->GetBinCenter(i)-result;
    }
    if(abs(lowerIntegral/expHist->Integral()-0.32)<0.02){
      cout<<"lower bound = "<<expHist->GetBinCenter(i)<<endl;
      errLow = result - expHist->GetBinCenter(i);
    }
  }


  cout<<"error Up = "<<errUp<<endl;
  cout<<"error Low = "<<errLow<<endl;

  TCanvas *c = new TCanvas("cExp","cExp",0,0,500,500);
  expHist->Draw();
}    
//################################################################################################################################
TCanvas *drawRatioPlotWithPurity(TH1D *prediction, TH1D *sr, TString xTitle, TString filename, double purity, bool LowECaloRegion){
  
  TCanvas *c = new TCanvas("c"+filename,"c",0,0,500,500);

  float y = 0.3;

  TPad *pad1     = new TPad("pad1", "Control Plots 1", 0.01, y, 0.99, 0.99);
  TPad *padRatio = new TPad("rp1", "Ratio1", 0.01, 0.01, 0.99, y-0.01);

  TH1D *ratio = 0;

  ratio = (TH1D*) sr->Clone();
  ratio->Divide(prediction);
  ratio->GetYaxis()->SetTitle((TString) sr->GetTitle() + "/" + (TString) prediction->GetTitle());
  ratio->SetTitle("");
  ratio->SetLabelSize(0.1,"X");
  ratio->SetLabelSize(0.1,"Y");
  ratio->SetTitleOffset(0.5,"Y");
  ratio->SetTitleSize(0.11,"Y");
  ratio->SetLineColor(kBlack);
  ratio->SetMarkerColor(kBlack);


  padRatio->cd();
  ratio->GetYaxis()->SetRangeUser(0,2);
  ratio->Draw();

  // Draw line at one!
  float xmin = ratio->GetXaxis()->GetXmin();
  float xmax = ratio->GetXaxis()->GetXmax();
  TLine *line = new TLine(xmin,1,xmax,1);
  line->SetLineWidth(2);
  line->Draw("same");
  padRatio->Modified();

  TLegend *leg = new TLegend(0.5,0.75,0.85,0.9);
  leg->SetTextSize(0.05);
  leg->AddEntry(sr,sr->GetTitle(),"l"); 
  leg->AddEntry(prediction,prediction->GetTitle(),"pel"); 
  pad1->cd();
  pad1->SetLogy();

  sr->SetLineColor(kRed);
  sr->SetMarkerColor(kRed);
  sr->SetMarkerStyle(20);
  prediction->SetLineColor(kBlack);
  prediction->SetMarkerColor(kBlack);
  prediction->SetMarkerStyle(20);
  sr->SetTitle("");
  prediction->SetTitle("");
  prediction->GetXaxis()->SetTitle(xTitle);
  sr->GetXaxis()->SetTitle(xTitle);

  prediction->SetTitleSize(0.07,"X");
  prediction->GetXaxis()->SetTitleOffset(0.7);
  sr->SetTitleSize(0.07,"X");
  sr->GetXaxis()->SetTitleOffset(0.7);

  prediction->Draw("e");
  sr->Draw("e same");

  double maximum =prediction->GetMaximum()*1.2;
  double minimum = 0.00001;
  if(sr->GetMinimum()!=0) minimum=sr->GetMinimum()*0.9;
  if(prediction->GetMinimum()!=0) minimum=prediction->GetMinimum()*0.9;
  if(sr->GetMaximum()>prediction->GetMaximum()){

    maximum=sr->GetMaximum()*1.2;
  }



  leg->Draw("same");


  //***********************
  TLatex*  info   = new TLatex();
  info->SetTextFont(132);
  info-> SetNDC();
  info->SetTextSize(0.06);
  TString AuxString = Form("Purity of fakes in CR = %4.1f ",purity*100);
  info->DrawLatex(0.4,0.68,AuxString);

  TLatex*  info1   = new TLatex();
  info1->SetTextFont(132);
  info1-> SetNDC();
  info1->SetTextSize(0.06);
  if(LowECaloRegion) AuxString = "E_{calo}<5GeV";
  else               AuxString = "E_{calo}>5GeV"; 
  info1->DrawLatex(0.2,0.82,AuxString);
  //***********************

  // Draw both pads to canvas
  c->cd();
  pad1->Draw();
  padRatio->SetGridy();
  c -> SetBottomMargin(0.55);
  //c->Modified();
  padRatio->Draw();

  c->SaveAs(filename);

  return c;

}
