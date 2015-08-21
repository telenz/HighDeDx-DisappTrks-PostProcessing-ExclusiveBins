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
#include "TPad.h"
#include "/afs/desy.de/user/t/tlenz/myAnalysis/bkgEstimation/plotStyle.h"


TCanvas *drawRatioPlot(TH1D *prediction, TH1D *sr, TH1D *data, TH1D *data2, TString xTitle, TString filename, double ecaloCut);

class sample{


public:

  TFile *file;
  TTree *tree;

  TH1D* histo;
  TH1D* histoECalo;
  TH1D* histoASmi;
  TH1D* histoNValid;
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

  sample(double iasCut, TString name){

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

    Double_t xbinsECalo[3]  = {0,10,200};
    Double_t xbinsASmi[3]   = {0,iasCut,1};
    //Double_t xbinsASmi[5]   = {0,0.05,0.1,0.3,1};
    Double_t xbinsNHits[4]  = {3,5,7,25};
    Double_t xbinsPt[6]     = {20,35,50,75,150,2000};

    histoECalo  = new TH1D("predECalo","predECalo",2,xbinsECalo);
    histoASmi   = new TH1D("ASmi " + name,"ASmi " +name,2,xbinsASmi);
    histoNValid = new TH1D("predNValid","predNValid",3,xbinsNHits);
    histoPt     = new TH1D("predPt","predPt",5,xbinsPt);
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


  void Selection(double metCut, double jetptCut, bool doMatching, int pdgId, double ptCut=35., double ecaloCut=5.){

    histo->Sumw2();
    histoECalo->Sumw2();
    histoASmi->Sumw2();
    histoNValid->Sumw2();
    histoPt->Sumw2();

    histo->SetTitle("");

    statistics = 0;
    purity     = 0;

    for(int n=0; n<tree->GetEntries(); n++){

      tree->GetEntry(n);

      if(met<metCut)                          continue;
      if(leadingJetPt<jetptCut)               continue;
      

      if(abs(trackEta->at(0))>2.1)                                 continue;
      if(abs(trackEta->at(0))>1.42 && abs(trackEta->at(0))<1.65 )  continue;
      if(trackPt->at(0)<ptCut)                                     continue;

      if(trackCaloIso->at(0)>ecaloCut)                                     continue;

      if(trackASmi->at(0)<0) continue;
      // ****************************************** Particle Id **********************************
      purity+=1;
      if(doMatching){
	if(abs(trackPdgId->at(0))!=pdgId)      continue; 
      }
      statistics +=1;
      // ****************************************** Particle Id **********************************

      histoECalo    -> Fill(trackCaloIso->at(0),weight_xsec_lumi*weight);
      histoASmi     -> Fill(trackASmi->at(0));
      histoNValid   -> Fill(trackNValid->at(0),weight_xsec_lumi*weight);
      histoPt       -> Fill(trackPt->at(0),weight_xsec_lumi*weight);
      histo         -> Fill(trackEta->at(0),weight_xsec_lumi*weight);

     }
    
    purity=statistics/purity;
  };
};


int compareDifferentCuts(double metCut, double jetptCut, int pdgId, double ptCut, double ecaloCut, double iasCut){

  sample pred(iasCut,"CR");
  sample predEcalo(iasCut,"CR No Ecalo Cut");
  sample SR(iasCut,"SR");
  sample SREcalo(iasCut,"SR No Ecalo Cut");
  sample data(iasCut,"data");



  TString process = "wjets";
  SR.file   = new TFile("~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_04_METGt0_JetPtGt70_trackPtGt20/results/analyzer/ntuples/input_weighted/wjets.root","READ");
  SREcalo.file   = new TFile("~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_04_METGt0_JetPtGt70_trackPtGt20/results/analyzer/ntuples/input_weighted/wjets.root","READ");

  if(pdgId==11){
    pred.file   = new TFile("~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_04_METGt0_JetPtGt70_trackPtGt20_ElectronCS/results/analyzer/ntuples/input_weighted/" + process + ".root","READ");
    predEcalo.file   = new TFile("~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_04_METGt0_JetPtGt70_trackPtGt20_ElectronCS/results/analyzer/ntuples/input_weighted/" + process + ".root","READ");
    data.file   = new TFile("~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_04_METGt0_JetPtGt70_trackPtGt20_ElectronCS/results/analyzer/ntuples/input_weighted/data.root","READ");
  }
  else if(pdgId==13){
    pred.file   = new TFile("~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_04_METGt0_JetPtGt70_trackPtGt20_MuonCS/results/analyzer/ntuples/input_weighted/" + process + ".root","READ");
    predEcalo.file   = new TFile("~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_04_METGt0_JetPtGt70_trackPtGt20_MuonCS/results/analyzer/ntuples/input_weighted/" + process + ".root","READ");
    data.file   = new TFile("~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_04_METGt0_JetPtGt70_trackPtGt20_MuonCS/results/analyzer/ntuples/input_weighted/data.root","READ");
  }
  else if(pdgId==211){
    pred.file   = new TFile("~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_04_METGt0_JetPtGt70_trackPtGt20_TauCS/results/analyzer/ntuples/input_weighted/" + process + ".root","READ");
    predEcalo.file   = new TFile("~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_04_METGt0_JetPtGt70_trackPtGt20_TauCS/results/analyzer/ntuples/input_weighted/" + process + ".root","READ");
    data.file   = new TFile("~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_06_04_METGt0_JetPtGt70_trackPtGt20_TauCS/results/analyzer/ntuples/input_weighted/data.root","READ");
  }
  else if(pdgId==0){
    pred.file   = new TFile("~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_05_08_METGt0_JetPtGt70_TauPtGt20_JetPtGt20_FakeCS/results/analyzer/ntuples/input_weighted/" + process + ".root","READ");
    predEcalo.file   = new TFile("~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_05_08_METGt0_JetPtGt70_TauPtGt20_JetPtGt20_FakeCS/results/analyzer/ntuples/input_weighted/" + process + ".root","READ");
    data.file   = new TFile("~/xxl-af-cms/ANALYSIS/workdir/analysis_2015_05_08_METGt0_JetPtGt70_TauPtGt20_JetPtGt20_FakeCS/results/analyzer/ntuples/input_weighted/data.root","READ");
  }
  
  TString selection = "chiTrackspreselectionNoQCDCutsNoTrigger";
  pred.file -> GetObject(selection + "/Variables",pred.tree);
  predEcalo.file -> GetObject(selection + "/Variables",predEcalo.tree);
  SR.file   -> GetObject(selection + "/Variables",SR.tree);
  SREcalo.file   -> GetObject(selection + "/Variables",SREcalo.tree);
  data.file -> GetObject("chiTrackspreselectionNoQCDCutsNoTrigger/Variables",data.tree);

  SR.getTreeVariables();
  SREcalo.getTreeVariables();
  pred.getTreeVariables();
  predEcalo.getTreeVariables();
  data.getTreeVariables();

  int nbins=20;
  double xmin=-2.4;
  double xmax=2.4;
  SR.histo   = new TH1D("SR","SR",nbins,xmin,xmax);
  SREcalo.histo   = new TH1D("SRNoEcaloCut","SRNoEcaloCut",nbins,xmin,xmax);
  pred.histo = new TH1D("pred","pred",nbins,xmin,xmax);
  predEcalo.histo = new TH1D("predNoEcaloCut","predNoEcaloCut",nbins,xmin,xmax);
  data.histo = new TH1D("data","data",nbins,xmin,xmax);


  SR.Selection(metCut  , jetptCut, 1, pdgId, ptCut, ecaloCut);
  SREcalo.Selection(metCut  , jetptCut, 1, pdgId, ptCut, 1000000);
  pred.Selection(metCut, jetptCut, 0, pdgId, ptCut, ecaloCut);
  predEcalo.Selection(metCut, jetptCut, 0, pdgId, ptCut, 1000000);
  data.Selection(metCut, jetptCut, 0, pdgId, ptCut, ecaloCut);

  cout<<"###########################################################################"<<endl;
  cout<<"Statisitcs:"<<endl;
  cout<<"Signal region  = "<<SR.statistics<<endl;
  cout<<"Control region = "<<pred.statistics<<endl;
  //cout<<"Purity: Signal region  = "<<SR.purity<<endl;
  cout<<"Purity: Control region = "<<pred.purity<<endl;
  cout<<"###########################################################################"<<endl;
  
  
  cout<<"Integral SR = "<<SR.histoNValid->Integral()<<endl;
  cout<<"Integral pred = "<<pred.histoPt->Integral()<<endl;
  cout<<"Integral data = "<<data.histoPt->Integral()<<endl;
  //cout<<"SR entries = "<<SR.histoNValid->GetEntries()<<endl;
  cout<<"pred entries = "<<pred.histoPt->GetEntries()<<endl;
  cout<<"data entries = "<<data.histoPt->GetEntries()<<endl;
  // ***** Calculate scaling factor  ************************************************************************
  double SRIntErr   = 0;
  double PredIntErr = 0;
  
  double SRInt   = SR.histoASmi->IntegralAndError(1,SR.histoASmi->GetNbinsX(),SRIntErr);
  double PredInt = pred.histoASmi->IntegralAndError(1,pred.histoASmi->GetNbinsX(),PredIntErr);
  
  double scalingFactor=SRInt/PredInt;
  double scalingFactorGaussianError = scalingFactor*sqrt(pow(SRIntErr/SRInt,2) + pow(PredIntErr/PredInt,2));
  cout<<"scaling factor for "<<pdgId<<" = "<<scalingFactor<<" +/- "<<scalingFactorGaussianError<<endl;
  // ***********************************************************************************************************

  //scalingFactor=0.0033;
  pred.histoECalo   -> Scale(scalingFactor);
  predEcalo.histoECalo   -> Scale(SRInt/predEcalo.histoECalo->Integral());
  cout<<"pred.histoECalo->Integral() = "<<pred.histoECalo->Integral()<<endl;
  cout<<"SR.histoECalo->Integral() = "<<SR.histoECalo->Integral()<<endl;
  pred.histoASmi    -> Scale(scalingFactor);
  predEcalo.histoASmi    -> Scale(SRInt/predEcalo.histoASmi->Integral());
  predEcalo.histo        -> Scale(SRInt/predEcalo.histo->Integral());
  pred.histoNValid  -> Scale(scalingFactor);
  //pred.histoPt      -> Scale(scalingFactor);
  pred.histo      -> Scale(scalingFactor);
  data.histo      -> Scale(SRInt/data.histo->GetEntries());
  data.histoASmi  -> Scale(SRInt/data.histoASmi->GetEntries());
  

  data.histoASmi       -> Scale(1./data.histoASmi->Integral());
  pred.histoASmi       -> Scale(1./pred.histoASmi->Integral());
  predEcalo.histoASmi  -> Scale(1./predEcalo.histoASmi->Integral());
  SR.histoASmi         -> Scale(1./SR.histoASmi->Integral());
  SREcalo.histoASmi         -> Scale(1./SREcalo.histoASmi->Integral());


 
  TeresaPlottingStyle::init();
  gROOT->ForceStyle();
 

  //TCanvas *c0 = drawRatioPlot(pred.histoECalo,SR.histoECalo,Form("E_{calo} pdgId=%i",pdgId),Form("hECalo_%i.pdf",pdgId));
  TCanvas *c1 = drawRatioPlot(pred.histoASmi,SR.histoASmi,predEcalo.histoASmi,SREcalo.histoASmi,Form("I_{as} pdgId=%i",pdgId),Form("plots/hASmi_%i_ASmiShape_EcaloComparison_ptCut%.0f_iasCut0p%.0f_EcaloLe%.0f.pdf",pdgId,ptCut,iasCut*100,ecaloCut), ecaloCut);

  
  //TCanvas *c2 = drawRatioPlot(pred.histoPt,data.histoPt,Form("track Pt pdgId=%i",pdgId),Form("hPt_%i.pdf",pdgId));
  //TCanvas *c3 = drawRatioPlot(pred.histoNValid,SR.histoNValid,Form("n_{Hits} pdgId=%i",pdgId),Form("hNValid_%i.pdf",pdgId));
  //TCanvas *c3 = drawRatioPlot(pred.histo,SR.histo,Form("n_{Hits} pdgId=%i",pdgId),Form("hNValid_%i.pdf",pdgId));

  //gDirectory->Delete(); 
 
  return 0;
 
}

//################################################################################################################################
//################################################################################################################################
//################################################################################################################################


TCanvas *drawRatioPlot(TH1D *prediction, TH1D *sr, TH1D *data,  TH1D *data2, TString xTitle, TString filename, double ecaloCut){

  gStyle -> SetPadLeftMargin(0.20);

  data->SetMarkerStyle(20);
  data->SetMarkerColor(kGreen);
  data->SetLineColor(kGreen);

  data2->SetMarkerStyle(20);
  data2->SetMarkerColor(kOrange);
  data2->SetLineColor(kOrange);


  
  TCanvas *c = new TCanvas("c"+filename,"c",0,0,500,500);

  float y = 0.3;

  TPad *pad1     = new TPad("pad1", "Control Plots 1", 0.01, y, 0.99, 0.99);
  TPad *padRatio = new TPad("rp1", "Ratio1", 0.01, 0.01, 0.99, y-0.01);

  pad1->SetNumber(100);
  pad1->SetTicks(0,1);
  cout<<"number pad1     = "<<pad1->GetNumber()<<endl;
  cout<<"number padRatio = "<<padRatio->GetNumber()<<endl;

  TH1D *ratio = 0;

  //ratio = (TH1D*) sr->Clone();
  //ratio->Divide(prediction);

  ratio = (TH1D*) prediction->Clone();
  ratio->Divide(data);



  for(int i=1; i<=ratio->GetNbinsX();i++){

    if(ratio->GetBinContent(i) != 0){
      cout<<"N in CR in "<<i<<". bin ="<<prediction->GetBinContent(i)<<endl;
      cout<<"N in SR in "<<i<<". bin ="<<sr->GetBinContent(i)<<endl;
      cout<<"Rel. difference in "<<i<<". bin ="<<(1./ratio->GetBinContent(i)-1.)*100<<"%"<<endl; 
    }
    else if(sr->GetBinContent(i) == 0 && prediction->GetBinContent(i) !=0)    cout<<"Scaling Factor in "<<i<<". bin <"<<prediction->GetBinContent(i)/1.15<<" +/- "<<ratio->GetBinError(i)<<endl;
    else if(sr->GetBinContent(i) != 0 && prediction->GetBinContent(i) ==0)    cout<<"Scaling Factor in "<<i<<". bin <"<<(sr->GetEntries()/prediction->GetEntries())/sr->GetBinContent(i)<<" +/- "<<ratio->GetBinError(i)<<endl;

  }

  ratio->GetYaxis()->SetTitle("#frac{CR (MC)}{CR (data)}");
  ratio->SetTitle("");
  ratio->SetLabelSize(0.1,"X");
  ratio->SetLabelSize(0.1,"Y");
  ratio->SetTitleOffset(0.5,"Y");
  ratio->SetTitleSize(0.15,"Y");


  padRatio->cd();
  //ratio->GetYaxis()->SetRangeUser(0,2);
  ratio->Draw("e");

  // Draw line at one!
  float xmin = ratio->GetXaxis()->GetXmin();
  float xmax = ratio->GetXaxis()->GetXmax();
  TLine *line = new TLine(xmin,1,xmax,1);
  line->SetLineWidth(2);
  line->Draw("same");
  padRatio->Modified();

  TLegend *leg = new TLegend(0.5,0.7,0.9,0.9);
  leg->AddEntry(sr,sr->GetName(),"pel"); 
  leg->AddEntry(prediction,prediction->GetName(),"pel"); 
 
  pad1->cd();
  pad1->SetLogy();
  //  pad1->SetLogx();

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

  double maximum = 0;
  double minimum = 1000000;
  if(sr->GetMinimum()!=0 && sr->GetMinimum()<minimum){
    minimum=sr->GetMinimum()*0.5;
  }
  if(prediction->GetMinimum()!=0 && prediction->GetMinimum()<minimum){
    minimum=prediction->GetMinimum()*0.5;
  }
  if(data->GetMinimum()!=0 && data->GetMinimum()<minimum){
    minimum=data->GetMinimum()*0.5;
  }
  if(sr->GetMaximum()>maximum){
    maximum=sr->GetMaximum()*2.5;
  }
  if(prediction->GetMaximum()>maximum){
    maximum=prediction->GetMaximum()*2.5;
  }
  if(data->GetMaximum()>maximum){
    maximum=data->GetMaximum()*2.5;
  }

  prediction->GetYaxis()->SetRangeUser(minimum,maximum);
  ratio->GetYaxis()->SetRangeUser(0,2);


  prediction->Draw("e");
  sr->Draw("e same");
  leg->AddEntry(data,data->GetName(),"pel"); 
  data->Draw("e same");
  leg->AddEntry(data2,data2->GetName(),"pel"); 
  data2->Draw("e same");

  
  leg->Draw("same");


  TLatex*  info1   = new TLatex();
  info1->SetTextFont(132);
  info1-> SetNDC();
  info1->SetTextSize(0.06);
  info1->DrawLatex(0.55,0.62,Form("E_{calo}<%.0fGeV",ecaloCut));

  // Draw both pads to canvas
  c->cd();
  pad1->Draw();
  padRatio->SetGridy();
  padRatio->Draw();

  c->SaveAs(filename);

  return c;

}
