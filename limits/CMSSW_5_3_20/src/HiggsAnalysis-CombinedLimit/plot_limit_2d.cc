#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TAxis.h"
#include "TGaxis.h"
#include "TLine.h"
#include "TStyle.h"
#include "TF1.h"
#include "TH1F.h"
#include "TMarker.h"
#include "TFrame.h"
#include "TMath.h"
#include "TLatex.h"
#include "TFile.h"
#include "/afs/desy.de/user/t/tlenz/Thesis/rootFiles/plotStyleThesis.h"

string dTs(double d){
 std::stringstream tmp;
 tmp<<d;
 string str=tmp.str();
 return str;

}

double ctauMin=1;
double massMin=100;


double getIntersectionPoint(vector<double> mass, vector<double> xsecTheo, vector<double> xsecTheoErr, vector<double> massAll, vector<double> xsecTheoAll, vector<double> xsecTheoErrAll, vector<double> exp, TString title);

void plot_limit_2d(TString filename){
  //this macro should read in limit results and make a nice plot out of it!
  //hard coded for asymptotic limit format for now

  TeresaPlottingStyle::init();
  gStyle -> SetPadBottomMargin(0.17);
  gStyle -> SetPadLeftMargin(0.17);
  gStyle -> SetPadTopMargin(0.10);
  gStyle -> SetPadRightMargin(0.17);
  gStyle->SetTitleOffset(1.2,"XY");
  gStyle->SetPadTickY(0);
  gROOT->ForceStyle();

  vector<int> lifetimes;
  //lifetimes.push_back(1);
  lifetimes.push_back(2);
  lifetimes.push_back(3);
  lifetimes.push_back(4);
  lifetimes.push_back(5);
  lifetimes.push_back(6);
  lifetimes.push_back(7);
  lifetimes.push_back(8);
  lifetimes.push_back(9);
  lifetimes.push_back(10);
  lifetimes.push_back(20);
  lifetimes.push_back(30);
  lifetimes.push_back(40);
  lifetimes.push_back(50);
  lifetimes.push_back(60);
  lifetimes.push_back(70);
  lifetimes.push_back(80);
  lifetimes.push_back(90);
  lifetimes.push_back(100);
  lifetimes.push_back(200);
  lifetimes.push_back(300);
  lifetimes.push_back(400);
  lifetimes.push_back(500);
  lifetimes.push_back(600);
  lifetimes.push_back(700);
  lifetimes.push_back(800);
  lifetimes.push_back(900);
  lifetimes.push_back(1000);
  lifetimes.push_back(2000);
  lifetimes.push_back(3000);
  lifetimes.push_back(4000);
  lifetimes.push_back(5000);
  lifetimes.push_back(6000);
  lifetimes.push_back(7000);
  lifetimes.push_back(8000);
  lifetimes.push_back(9000);
  lifetimes.push_back(10000);

  int size = lifetimes.size()+2;

  double* ctau_2d    = new double[size];
  double* ctau_2d_cm = new double[size];
  double* ctau_Error_2d = new double[size];
  double* obs_2d = new double[size];
  double* exp_2d = new double[size];
  double* exp_1sig_up_2d = new double[size];
  double* exp_1sig_down_2d = new double[size];
  double* exp_2sig_up_2d = new double[size];
  double* exp_2sig_down_2d = new double[size];


  for(unsigned int ct=0; ct<lifetimes.size();ct++){

    cout<<endl<<"ctau = "<<lifetimes[ct]<<"cm :"<<endl;
    TString ctauToString = Form("%icm",lifetimes[ct]);
    //cout<<ctauToString<<endl;

    vector<double> obs;
    vector<double> exp;
    vector<double> exp_1sig_up;
    vector<double> exp_2sig_up;
    vector<double> exp_1sig_down;
    vector<double> exp_2sig_down;

    vector<double> xsecTheo;
    vector<double> xsecTheoErr;
    vector<double> massAll;
    vector<double> xsecTheoAll;
    vector<double> xsecTheoErrAll;
    // Read in theoretical cross-sections
    ifstream inputFile("xsectionsUpdated.txt");
    //ifstream inputFile("xsectionsUpdatedAllMasses_AllProcesses.txt");
    
    int it=1;
    double dataset,xsec,xsecErr;
    while(inputFile>>dataset>>xsec>>xsecErr){
      massAll.push_back(dataset);
      xsecTheoAll.push_back(xsec/1000.);
      xsecTheoErrAll.push_back(xsecErr/1000.);

      if(dataset!=100 && dataset!=200 && dataset!=300 && dataset!=400 && dataset!=500 && dataset!=600 ) continue;
      xsecTheo.push_back(xsec/1000.);
      xsecTheoErr.push_back(xsecErr/1000.);
      it++;
    }
    inputFile.close();


    //to be nicend in the future;
    vector<double> mass;
    mass.push_back(100);
    mass.push_back(200);
    mass.push_back(300);
    mass.push_back(400);
    mass.push_back(500);
    mass.push_back(600);
  
    vector<string> files;
    files.push_back(Form("logFiles/" + filename + "/res_mass_100GeV_ctau_%icm.log",lifetimes[ct]));
    files.push_back(Form("logFiles/" + filename + "/res_mass_200GeV_ctau_%icm.log",lifetimes[ct]));
    files.push_back(Form("logFiles/" + filename + "/res_mass_300GeV_ctau_%icm.log",lifetimes[ct]));
    files.push_back(Form("logFiles/" + filename + "/res_mass_400GeV_ctau_%icm.log",lifetimes[ct]));
    files.push_back(Form("logFiles/" + filename + "/res_mass_500GeV_ctau_%icm.log",lifetimes[ct]));
    files.push_back(Form("logFiles/" + filename + "/res_mass_600GeV_ctau_%icm.log",lifetimes[ct]));
 
    for(unsigned int i=0; i<files.size(); i++){
      string file=files[i];
      //cout<<file<<endl;
      string line;
      ifstream myfile(file.c_str());
    
      if (myfile.is_open())
	{
	  while ( myfile.good() )
	    {
	      getline (myfile,line);
	      std::string::size_type pos; 
	      bool take_line=true;
	      //pos = line.find("Observed");
	      //if(pos != std::string::npos)take_line=true;
	      //pos = line.find("xpected");
	      //cout<<"pos = "<<pos<<endl;
	      if(pos != std::string::npos)take_line=true;
	      if(take_line){
		std::istringstream ss(line);
		string s1,s2,s3,s4,s5,s6,s7;
		double value=-1;
		ss >> s1 >> s2 >> s3 >> s4 >> value;
		if(value!=-1){
		  if(s1=="Observed"){
		    obs.push_back(value);
		  }
		  if(s1=="Expected" && s2=="50.0%:"){
		    //cout<<"expected limit = "<<value<<endl;
		    exp.push_back(value);
		  }
		}
		if(value!=-1){
		  if(s2=="84.0%:") {
		    exp_1sig_up.push_back(value);
		  }
		  if(s2=="16.0%:"){
		    exp_1sig_down.push_back(value);
		  }
		  if(s2=="97.5%:") {
		    exp_2sig_up.push_back(value);
		  }
		  if(s2=="2.5%:"){
		    exp_2sig_down.push_back(value);
		  }
		}
	    
	      }
	      //std::istream_iterator<std::string> begin(ss), end;
	      //std::vector<std::string> arrayTokens(begin, end); 
	  
	    }
	  myfile.close();
	}
    }//loop over all files

    double intExp           = 0;
    double intExp_1sig_up   = 0;
    double intExp_1sig_down = 0;
    double intExp_2sig_up   = 0;
    double intExp_2sig_down = 0;
    double intObs           = 0;
    intExp = getIntersectionPoint(mass,xsecTheo,xsecTheoErr,massAll,xsecTheoAll,xsecTheoErrAll,exp,Form("expected_%icm",lifetimes[ct]));
    cout<<"intExp = "<<intExp<<endl;
    intExp_1sig_up = getIntersectionPoint(mass,xsecTheo,xsecTheoErr,massAll,xsecTheoAll,xsecTheoErrAll,exp_1sig_down,Form("expected_1sigma_up_%icm",lifetimes[ct]));
    cout<<"intExp_1sig_up = "<<intExp_1sig_up<<endl;
    intExp_1sig_down = getIntersectionPoint(mass,xsecTheo,xsecTheoErr,massAll,xsecTheoAll,xsecTheoErrAll,exp_1sig_up,Form("expected_1sigma_down_%icm",lifetimes[ct]));
    cout<<"intExp_1sig_down = "<<intExp_1sig_down<<endl;
    intExp_2sig_up = getIntersectionPoint(mass,xsecTheo,xsecTheoErr,massAll,xsecTheoAll,xsecTheoErrAll,exp_2sig_down,Form("expected_2sigma_up_%icm",lifetimes[ct]));
    cout<<"intExp_2sig_up = "<<intExp_2sig_up<<endl;
    intExp_2sig_down = getIntersectionPoint(mass,xsecTheo,xsecTheoErr,massAll,xsecTheoAll,xsecTheoErrAll,exp_2sig_up,Form("expected_2sigma_down_%icm",lifetimes[ct]));
    cout<<"intExp_2sig_down = "<<intExp_2sig_down<<endl;
    intObs = getIntersectionPoint(mass,xsecTheo,xsecTheoErr,massAll,xsecTheoAll,xsecTheoErrAll,obs,Form("observed_%icm",lifetimes[ct]));
    cout<<"intObs = "<<intObs<<endl;
    intExp = getIntersectionPoint(mass,xsecTheo,xsecTheoErr,massAll,xsecTheoAll,xsecTheoErrAll,exp,Form("expected_%icm",lifetimes[ct]));
    cout<<"intExp = "<<intExp<<endl;
    
    cout<<endl;

    cout<<"lifetimes["<<ct<<"] = "<<lifetimes[ct]<<endl;
    cout<<"intExp = "<<intExp<<endl;
    if(ct==0 && (intExp>200 || intExp==-1)){

      cout<<"wrong limit!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
      intExp=0;
    }

    if(ct==0){

      ctau_2d[ct]          = lifetimes[ct]/(100*TMath::C()*pow(10,-9));
      ctau_2d_cm[ct]       = lifetimes[ct];
      ctau_Error_2d[ct]    = 0;
      obs_2d[ct]           = 0;
      exp_2d[ct]           = 0;
      exp_1sig_up_2d[ct]   = 0;
      exp_1sig_down_2d[ct] = 0;
      exp_2sig_up_2d[ct]   = 0;
      exp_2sig_down_2d[ct] = 0;
    }
    
    
    ctau_2d[ct+1]          = lifetimes[ct]/(100*TMath::C()*pow(10,-9));
    ctau_2d_cm[ct+1]       = lifetimes[ct];
    ctau_Error_2d[ct+1]    = 0;
    obs_2d[ct+1]           = intObs;
    exp_2d[ct+1]           = intExp;
    exp_1sig_up_2d[ct+1]   = intExp_1sig_up;
    exp_1sig_down_2d[ct+1] = intExp_1sig_down;
    exp_2sig_up_2d[ct+1]   = intExp_2sig_up;
    exp_2sig_down_2d[ct+1] = intExp_2sig_down;
    
    /*
    cout<<"exp_2d["<<ct<<"] = "<<exp_2d[ct]<<endl;
    if(exp_2d[ct]==0){
      cout<<"in  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "<<endl;
      ctau_2d[ct]          = ctau_2d[ct+1];
      ctau_2d_cm[ct]       = ctau_2d_cm[ct+1];
      ctau_Error_2d[ct]    = 0;
      obs_2d[ct]           = massMin;
      exp_2d[ct]           = massMin;
      exp_1sig_up_2d[ct]   = massMin;
      exp_1sig_down_2d[ct] = massMin;
      exp_2sig_up_2d[ct]   = massMin;
      exp_2sig_down_2d[ct] = massMin;

    }
    */

    /*
    ctau_2d[ct] = lifetimes[ct];
    ctau_Error_2d[ct] = 0;
    obs_2d[ct] =intObs;
    exp_2d[ct] =intExp;
    exp_1sig_up_2d[ct] =intExp_1sig_up;
    exp_1sig_down_2d[ct] =intExp_1sig_down;
    exp_2sig_up_2d[ct] =intExp_2sig_up;
    exp_2sig_down_2d[ct] =intExp_2sig_down;
    */


  }


  ctau_2d[size-1] = ctau_2d[size-2];
  ctau_2d_cm[size-1] = ctau_2d_cm[size-2];
  ctau_Error_2d[size-1] = 0;
  obs_2d[size-1] = obs_2d[0];
  exp_2d[size-1] = exp_2d[0];
  exp_1sig_up_2d[size-1] = exp_1sig_up_2d[0];
  exp_1sig_down_2d[size-1] = exp_1sig_down_2d[0];
  exp_2sig_up_2d[size-1] = exp_2sig_up_2d[0];
  exp_2sig_down_2d[size-1] = exp_2sig_down_2d[0];


  TCanvas *c = new TCanvas("c","c",500,500);
  //c->SetLogy();

  gPad->SetBottomMargin(0.20);
  gPad->SetLeftMargin(0.20);
  
  TLegend *l1=new TLegend(0.15,0.65,0.45,0.85);
  l1->SetLineColor(0);
  l1->SetFillColor(0);
  l1->SetFillStyle(0);
  l1->SetTextSize(0.04);
  l1->SetBorderSize(0);

  int nBins = size;
  
  /*
  cout<<"exp_2sig_up_2d_2d[0] = "<<exp_2sig_up_2d[0]<<endl;
  cout<<"exp_2sig_up_2d_2d[1] = "<<exp_2sig_up_2d[1]<<endl;
  cout<<"exp_2sig_up_2d_2d[2] = "<<exp_2sig_up_2d[2]<<endl;
  cout<<"exp_2sig_up_2d_2d[3] = "<<exp_2sig_up_2d[3]<<endl;
  cout<<"exp_2sig_up_2d_2d[4] = "<<exp_2sig_up_2d[4]<<endl;

  cout<<"exp_2sig_down_2d_2d[0] = "<<exp_2sig_down_2d[0]<<endl;
  cout<<"exp_2sig_down_2d_2d[1] = "<<exp_2sig_down_2d[1]<<endl;
  cout<<"exp_2sig_down_2d_2d[2] = "<<exp_2sig_down_2d[2]<<endl;
  cout<<"exp_2sig_down_2d_2d[3] = "<<exp_2sig_down_2d[3]<<endl;
  cout<<"exp_2sig_down_2d_2d[4] = "<<exp_2sig_down_2d[4]<<endl;

  cout<<"ctau_2d[0] = "<<ctau_2d[0]<<endl;
  cout<<"ctau_2d[1] = "<<ctau_2d[1]<<endl;
  cout<<"ctau_2d[2] = "<<ctau_2d[2]<<endl;
  cout<<"ctau_2d[3] = "<<ctau_2d[3]<<endl;
  cout<<"ctau_2d[3] = "<<ctau_2d[4]<<endl;
  */
  TGraph* g_exp_2sig_up   = new TGraph(nBins, exp_2sig_up_2d,   ctau_2d);
  TGraph* g_exp_2sig_down = new TGraph(nBins, exp_2sig_down_2d, ctau_2d);
  TGraph* g_exp_1sig_up   = new TGraph(nBins, exp_1sig_up_2d,   ctau_2d);
  TGraph* g_exp_1sig_down = new TGraph(nBins, exp_1sig_down_2d, ctau_2d);
  TGraph* g_exp           = new TGraph(nBins, exp_2d          , ctau_2d);
  TGraph* g_obs           = new TGraph(nBins, obs_2d          , ctau_2d);

  TGraph* g_exp_2sig_up_cm   = new TGraph(nBins, exp_2sig_up_2d,   ctau_2d_cm);
  TGraph* g_exp_2sig_down_cm = new TGraph(nBins, exp_2sig_down_2d, ctau_2d_cm);
  TGraph* g_exp_1sig_up_cm   = new TGraph(nBins, exp_1sig_up_2d,   ctau_2d_cm);
  TGraph* g_exp_1sig_down_cm = new TGraph(nBins, exp_1sig_down_2d, ctau_2d_cm);
  TGraph* g_exp_cm           = new TGraph(nBins, exp_2d          , ctau_2d_cm);
  TGraph* g_obs_cm           = new TGraph(nBins, obs_2d          , ctau_2d_cm);


  g_exp_2sig_up->GetXaxis()->SetLimits(100,600);

  g_exp_2sig_up->SetMarkerStyle(8);
  g_exp_2sig_up->SetMarkerSize(1);
  g_exp_2sig_up->SetFillColor(kYellow);
  g_exp_2sig_up->SetLineWidth(9900);

  g_exp_2sig_down->SetMarkerStyle(8);
  g_exp_2sig_down->SetMarkerSize(1);
  g_exp_2sig_down->SetFillColor(kWhite);
  g_exp_2sig_down->SetLineWidth(9900);

  g_exp_1sig_up->SetMarkerStyle(8);
  g_exp_1sig_up->SetMarkerSize(1);
  g_exp_1sig_up->SetFillColor(kGreen);
  g_exp_1sig_up->SetLineWidth(9900);

  g_exp_1sig_down->SetMarkerStyle(8);
  g_exp_1sig_down->SetMarkerSize(1);
  g_exp_1sig_down->SetFillColor(kYellow);
  g_exp_1sig_down->SetLineWidth(9900);

  g_exp->SetLineWidth(2);
  g_exp->SetLineStyle(7);
  g_obs->SetLineWidth(2);

  g_exp_2sig_up  ->Draw("alf");
  g_exp_1sig_up->Draw("lf same");
  g_exp_1sig_down->Draw("lf same");
  g_exp_2sig_down->Draw("lf same");
  g_exp->Draw("l same");
  g_obs->Draw("l same");


  g_exp_2sig_up->GetXaxis()->SetTitle("mass_{#Chi^{#pm}} (GeV)");
  g_exp_2sig_up->GetYaxis()->SetTitle("c#tau_{#Chi^{#pm}} [ns]");
  g_exp_2sig_up->GetYaxis()->SetRangeUser(ctauMin,1100);
  g_exp_2sig_up->GetYaxis()->SetRangeUser(0.05,300);
  g_exp_2sig_up->GetYaxis()->SetRangeUser(1,10);
  g_exp_2sig_up->SetTitle();

  gPad->RedrawAxis();

  l1->AddEntry(g_obs,"Observed Limit","l");
  l1->AddEntry(g_exp,"Expected Limit","l");
  l1->AddEntry(g_exp_1sig_up,"#pm 1#sigma","f");
  l1->AddEntry(g_exp_2sig_up,"#pm 2#sigma","f");

  l1->Draw();
  
  //c->SetTickx();
  //c->SetTicky();
  c->Update();
  g_exp_2sig_up->GetYaxis()->SetRangeUser(0.05,300);
  c->SaveAs("LimitPlot_2d.pdf");
  c->SetLogy();
  g_exp_2sig_up->GetYaxis()->SetRangeUser(0.05,300);
  c->SaveAs("LimitPlot_2d_log.pdf");


  delete c;
  gStyle -> SetPadBottomMargin(0.15);
  gStyle -> SetPadLeftMargin(0.15);

  c = new TCanvas("c","c",500,500);
  g_exp_2sig_up_cm->GetXaxis()->SetLimits(100,600);

  g_exp_2sig_up_cm->SetMarkerStyle(8);
  g_exp_2sig_up_cm->SetMarkerSize(1);
  g_exp_2sig_up_cm->SetFillColor(kYellow);
  g_exp_2sig_up_cm->SetLineColor(kYellow);
  g_exp_2sig_up_cm->SetLineWidth(9900);

  g_exp_2sig_down_cm->SetMarkerStyle(8);
  g_exp_2sig_down_cm->SetMarkerSize(1);
  g_exp_2sig_down_cm->SetFillColor(kWhite);
  g_exp_2sig_down_cm->SetLineWidth(9900);

  g_exp_1sig_up_cm->SetMarkerStyle(8);
  g_exp_1sig_up_cm->SetMarkerSize(1);
  g_exp_1sig_up_cm->SetFillColor(kGreen);
  g_exp_1sig_up_cm->SetLineColor(kGreen);
  g_exp_1sig_up_cm->SetLineWidth(9900);

  g_exp_1sig_down_cm->SetMarkerStyle(8);
  g_exp_1sig_down_cm->SetMarkerSize(1);
  g_exp_1sig_down_cm->SetFillColor(kYellow);
  g_exp_1sig_down_cm->SetLineColor(kYellow);
  g_exp_1sig_down_cm->SetLineWidth(9900);

  g_exp_cm->SetLineWidth(2);
  //g_exp_cm->SetLineStyle(7);
  g_obs_cm->SetLineColor(kRed);
  g_obs_cm->SetLineWidth(2);

  g_exp_2sig_up_cm  ->Draw("alf");
  g_exp_1sig_up_cm->Draw("lf same");
  g_exp_1sig_down_cm->Draw("lf same");
  g_exp_2sig_down_cm->Draw("lf same");
  g_exp_cm->Draw("l same");
  g_obs_cm->Draw("l same");


  g_exp_2sig_up_cm->GetXaxis()->SetTitle("mass_{#Chi^{#pm}} [GeV]");
  g_exp_2sig_up_cm->GetYaxis()->SetTitle("c#tau_{#Chi^{#pm}} [cm]");
  g_exp_2sig_up_cm->GetYaxis()->SetRangeUser(ctauMin,1100);
  g_exp_2sig_up_cm->GetYaxis()->SetRangeUser(0.05,300);
  g_exp_2sig_up_cm->GetYaxis()->SetRangeUser(1,10);
  g_exp_2sig_up_cm->SetTitle();

  gPad->RedrawAxis();

  delete l1;
  l1=new TLegend(0.18,0.45,0.50,0.65);
  l1->SetTextSize(0.04);


  TGraph* g_obs_cmDummy = (TGraph*) g_obs_cm->Clone();
  g_obs_cmDummy->SetLineWidth(3);
  TGraph* g_exp_cmDummy = (TGraph*) g_exp_cm->Clone();
  g_exp_cmDummy->SetLineWidth(3);
  l1->AddEntry(g_obs_cmDummy,"Observed limit","l");
  l1->AddEntry(g_exp_cmDummy,"Expected limit","l");
  l1->AddEntry(g_exp_1sig_up_cm,"#pm 1#sigma exp. limit","f");
  l1->AddEntry(g_exp_2sig_up_cm,"#pm 2#sigma exp. limit","f");

  l1->Draw();
  double rightmax=50000;
  cout<<"gPad->GetUxmax() = "<<gPad->GetUxmax()<<endl;
  cout<<"gPad->GetUxmin() = "<<gPad->GetUxmin()<<endl;

  double lowEdge  = 1.7 / (100*TMath::C()*pow(10,-9));
  double highEdge = 9000 / (100*TMath::C()*pow(10,-9));

  TGaxis *axis = new TGaxis(600, 1.7, 600, 9000, lowEdge, highEdge,505,"+LG");
  axis->SetTitle("#tau_{#Chi^{#pm}} [ns]");
  axis->SetTitleOffset(1.35);
  //axis->SetLabelOffset(0.07);
  axis->SetTitleSize(0.06);
  axis->SetTitleFont(42);
  axis->SetLabelSize(0.05);
  axis->SetLabelFont(42);
  axis->Draw("same");
  
  TLatex*  info   = new TLatex();
  info-> SetNDC();
  info->SetTextSize(0.045);
  info->DrawLatex(0.55,0.91,"19.7 fb^{-1} (8 TeV)");

  //c->SetTickx();
  //c->SetTicky();
  c->Update();
  g_exp_2sig_up_cm->GetYaxis()->SetRangeUser(1.7,9000);
  c->SaveAs("LimitPlot_2d_cm.pdf");
  c->SetLogy();
  g_exp_2sig_up_cm->GetYaxis()->SetRangeUser(1.7,9000);
  c->SaveAs("LimitPlot_2d_log_cm.pdf");


  // Save root file with observed limit points

  TFile* f = new TFile("ObservedLimit.root","RECREATE");
  f->cd();
  g_obs_cm->Write("Limit_cm");
  g_obs   ->Write("Limit_ns");
  f->Close();

  return;
}

TF1* f1, *f2, *fint;
double finter(double *x, double*par) {
  //return 10;
  return TMath::Abs(f1->EvalPar(x,par) - f2->EvalPar(x,par));
}

double getIntersectionPoint(vector<double> mass, vector<double> xsecTheo, vector<double> xsecTheoErr, vector<double> massAll, vector<double> xsecTheoAll, vector<double> xsecTheoErrAll, vector<double> exp, TString title){

  vector<double> xsecTheoLow;
  vector<double> expTimesXsec;
  for(unsigned int i=0; i<xsecTheo.size();i++){
    //cout<<"massAll["<<i<<"] = "<<massAll[i]<<endl;
    //cout<<"exp["<<i<<"] = "<<exp[i]*xsecTheo[i]<<endl;
    //cout<<"xsecTheo["<<i<<"] = "<<xsecTheo[i]<<endl;
    expTimesXsec.push_back(log(exp[i]*xsecTheo[i]));
  }

  for(unsigned int i=0; i<xsecTheoAll.size();i++){
    xsecTheoLow.push_back(log(xsecTheoAll[i]-xsecTheoErrAll[i]));
  }


  TCanvas *c3=0;
  // f1 = theoretical cross-section
  // f2 = xsection upper limit
  for(unsigned int j=xsecTheo.size()-2; j>=0;j--){

    cout<<"j = "<<j<<endl;
    cout<<"mass["<<j<<"] = "<<mass[j]<<endl;
    cout<<"mass["<<j+1<<"] = "<<mass[j+1]<<endl;
    //cout<<"mass["<<j+1<<"] = "<<mass[j+1]<<endl;
    f2 = new TF1("f2","[0]*x+[1]",mass[j],mass[j+1]);
    //f2 = new TF1("f2","[0]*TMath::Exp([1]*x)",mass[j],mass[j+1]);
    f2->SetRange(mass[j],mass[j+1]);
    f2->SetLineColor(kGreen);
    cout<<"expTimesXsec["<<j<<"] = "<<expTimesXsec[j]<<endl;
    cout<<"expTimesXsec["<<j+1<<"] = "<<expTimesXsec[j+1]<<endl;
    double a = (expTimesXsec[j]-expTimesXsec[j+1])/(mass[j]-mass[j+1]);
    double b = expTimesXsec[j]-a*mass[j];
    //double b = TMath::Log((expTimesXsec[j]/expTimesXsec[j+1]))/(mass[j]-mass[j+1]);
    //double a = expTimesXsec[j]/TMath::Exp(b*mass[j]);
    f2->SetParameter(0,a);
    f2->SetParameter(1,b);

    for(unsigned int i=xsecTheoLow.size()-2; i>=0;i--){

      if(massAll[i+1]>mass[j+1] && j!=xsecTheo.size()-2) continue;
      if(massAll[i]<mass[j] /*&& j+2 != xsecTheo.size()*/){
	//
	//cout<<"mass["<<j+1<<"] = "<<mass[j+1]<<endl;
	break;
      }

      cout<<"i = "<<i<<endl;
      cout<<"xsecTheoLow["<<i<<"] = "<<xsecTheoLow[i]<<endl;
      cout<<"xsecTheoLow["<<i+1<<"] = "<<xsecTheoLow[i+1]<<endl;
      if(massAll[i]<mass[j] /*&& j+2 != xsecTheo.size()*/){
	//
	//cout<<"mass["<<j+1<<"] = "<<mass[j+1]<<endl;
	break;
      }
      f1 = new TF1("f1","[0]*x+[1]",massAll[i],massAll[i+1]);
      double a1 = (xsecTheoLow[i]-xsecTheoLow[i+1])/(massAll[i]-massAll[i+1]);
      double b1 = xsecTheoLow[i]-a1*massAll[i];
      f1->SetParameter(0,a1);
      f1->SetParameter(1,b1);
      double xmin, xmax;
      f1->GetRange(xmin,xmax);
      fint = new TF1("fint_"+title,finter,xmin,xmax,0);
      double xint = fint->GetMinimumX();
      cout<<"xint = "<<xint<<endl;
      fint->SetLineColor(2);
      c3= new TCanvas("c3_"+title,title);
      c3->cd();
      TLegend *leg = new TLegend(0.1,0.7,0.48,0.9);
      leg->AddEntry(f2,"xsec upper limit","l");
      leg->AddEntry(f1,"theory xsec","l");
      //c3->SetLogy();
      f2->SetMaximum(10);
      f2->SetMinimum(0.0);
      f1->SetTitle(title);
      //f1->Draw();
      f2->Draw();
      f1->Draw("same");
      leg->Draw("same");
      fint->Draw("lsame");
    
      if(xint!=massAll[i] && xint !=massAll[i+1]){
	//c3->SaveAs("intersection_"+title+".pdf");
	return xint;
      }
      delete c3;
    }

  }
  return -1;

}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
