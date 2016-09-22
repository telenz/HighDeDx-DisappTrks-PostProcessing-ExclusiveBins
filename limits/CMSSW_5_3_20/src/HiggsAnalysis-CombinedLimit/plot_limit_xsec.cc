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
#include "TLine.h"
#include "TStyle.h"
#include "TLatex.h"
#include "/afs/desy.de/user/t/tlenz/Thesis/rootFiles/plotStyleThesis.h"

string dTs(double d){
 std::stringstream tmp;
 tmp<<d;
 string str=tmp.str();
 return str;

}

void make_plot(vector<double> mass, vector<double> xsecTheo, vector<double> xsecTheoErr, vector<double> massAll, vector<double> xsecTheoAll, vector<double> xsecTheoErrAll, vector<double> obs,vector<double> exp, vector<double> exp_1sig_up,  vector<double> exp_2sig_up,vector<double> exp_1sig_down,  vector<double> exp_2sig_down, int ctau);

void plot_limit_xsec(TString filename, int ctau){
  //this macro should read in limit results and make a nice plot out of it!
  //hard coded for asymptotic limit format for now

  TeresaPlottingStyle::init();
  gROOT->ForceStyle();

  TString ctauToString = Form("%icm",ctau);
  cout<<ctauToString<<endl;

  vector<double> obs;
  vector<double> exp;
  vector<double> exp_1sig_up;
  vector<double> exp_2sig_up;
  vector<double> exp_1sig_down;
  vector<double> exp_2sig_down;

  //to be nicend in the future;
  vector<double> mass;
  if(ctau==1){
    mass.push_back(100);
    mass.push_back(200);
  }
  else{
    mass.push_back(100);
    mass.push_back(200);
    mass.push_back(300);
    mass.push_back(400);
    mass.push_back(500);
    mass.push_back(600);
  }

  vector<double> xsecTheo;
  vector<double> xsecTheoErr;
  vector<double> massAll;
  vector<double> xsecTheoAll;
  vector<double> xsecTheoErrAll;
  // Read in theoretical cross-sections
  ifstream inputFile("xsectionsUpdatedAllMasses_AllProcesses.txt");
  //ifstream inputFile("xsectionsUpdatedAllMasses.txt");
  int it=1;
  double dataset, xsec,xsecErr;
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

  
  vector<string> files;
  if(ctau==1){
    cout<<"1cm!!!!!!!!!!!!!!!!"<<endl;
    files.push_back(Form("logFiles/" + filename + "/res_mass_100GeV_ctau_%icm.log",ctau));
    files.push_back(Form("logFiles/" + filename + "/res_mass_200GeV_ctau_%icm.log",ctau));
  }
  else{
    files.push_back(Form("logFiles/" + filename + "/res_mass_100GeV_ctau_%icm.log",ctau));
    files.push_back(Form("logFiles/" + filename + "/res_mass_200GeV_ctau_%icm.log",ctau));
    files.push_back(Form("logFiles/" + filename + "/res_mass_300GeV_ctau_%icm.log",ctau));
    files.push_back(Form("logFiles/" + filename + "/res_mass_400GeV_ctau_%icm.log",ctau));
    files.push_back(Form("logFiles/" + filename + "/res_mass_500GeV_ctau_%icm.log",ctau));
    files.push_back(Form("logFiles/" + filename + "/res_mass_600GeV_ctau_%icm.log",ctau));
  }

 
  for(unsigned int i=0; i<files.size(); i++){
    string file=files[i];
    cout<<file<<endl;
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
    
    else cout << "Unable to open file "; 
  }//loop over all files

  make_plot(mass,xsecTheo,xsecTheoErr,massAll,xsecTheoAll,xsecTheoErrAll,obs,exp,exp_1sig_up,exp_2sig_up,exp_1sig_down,exp_2sig_down,ctau);

  return;
}


void make_plot(vector<double> mass, vector<double> xsecTheo, vector<double> xsecTheoErr, vector<double> massAll, vector<double> xsecTheoAll, vector<double> xsecTheoErrAll, vector<double> obs,vector<double> exp, vector<double> exp_1sig_up,  vector<double> exp_2sig_up,vector<double> exp_1sig_down,  vector<double> exp_2sig_down, int ctau){
  TCanvas *c1 = new TCanvas;
  c1->SetLogy();
  //gStyle->SetPadBottomMargin(10.5);
  gPad->SetBottomMargin(0.14);
  gPad->SetLeftMargin(0.14);
  

  //TGraphAsymmErrors *exp_1sig= new TGraphAsymmErrors(mass,exp,exp_1sig_up,exp_1sig_down);
  const int nn = 11;
  

  double x[nn];
  double xUpperErr[nn];
  double xLowerErr[nn];
  double yUpperErr[nn];
  double yLowerErr[nn];
  double yUpperErr2[nn];
  double yLowerErr2[nn];
  double *xAll    = new double[xsecTheoAll.size()];
  double *xsec    = new double[xsecTheoAll.size()];
  double *xsecErr = new double[xsecTheoAll.size()];


  double yexp[nn];
  double yobs[nn];
  int nBins = mass.size();
  cout<<"Sizes of various vectors = "<<mass.size()<<" "<<exp.size()<<" "<< exp_1sig_up.size()<<" "<<exp_2sig_down.size()<<endl;
  for(unsigned int j=0; j<mass.size(); j++){
    x[j]=mass[j];
    yexp[j]=exp[j]*xsecTheo[j];          // Something not working
    yobs[j]=obs[j]*xsecTheo[j];
    xUpperErr[j]=0;
    xLowerErr[j]=0;
    yUpperErr[j]=(exp_1sig_up[j]-exp[j])*xsecTheo[j];
    yLowerErr[j]=(exp[j]-exp_1sig_down[j])*xsecTheo[j];
    yUpperErr2[j]=(exp_2sig_up[j]-exp[j])*xsecTheo[j];
    yLowerErr2[j]=(exp[j]-exp_2sig_down[j])*xsecTheo[j];
    //cout<<exp_1sig_down[j]<<" "<<exp_2sig_down[j]<<endl;
  }
  for(unsigned int j=0; j<xsecTheoAll.size(); j++){
    xAll[j]=massAll[j];
    xsec[j]=xsecTheoAll[j];
    xsecErr[j]=xsecTheoErrAll[j];
  }
  TGraph *expL= new TGraph(nBins,x,yexp);
  TGraph *obsL= new TGraph(nBins,x,yobs);
  TGraphErrors *xsecL= new TGraphErrors(xsecTheoAll.size(),xAll,xsec,xLowerErr,xsecErr);
  TGraphAsymmErrors *exp_1sig = new TGraphAsymmErrors(nBins, x, yexp, xLowerErr, xUpperErr, yLowerErr, yUpperErr);
  TGraphAsymmErrors *exp_2sig = new TGraphAsymmErrors(nBins, x, yexp, xLowerErr, xUpperErr, yLowerErr2, yUpperErr2);
  //exp_1sig->Draw("AF");
  expL->Draw("AL");
  exp_2sig->Draw("same l3");
  exp_1sig->Draw("same l3");


  obsL->SetLineColor(kRed);
  obsL->SetLineWidth(2);
  xsecL->SetLineColor(867);
  xsecL->SetFillColor(867);
  xsecL->SetLineWidth(2);

  //expL->SetMarkerStyle(22);
  expL->SetLineColor(kBlack);
  expL->SetLineWidth(2);
  expL->GetXaxis()->SetTitleSize(0.05);
  expL->GetYaxis()->SetTitleSize(0.05);
  expL->GetXaxis()->SetTitle("mass_{#Chi^{#pm}} [GeV]");
  expL->GetYaxis()->SetTitle("95% CL limit on #sigma [pb]");
  expL->GetXaxis()->SetRangeUser(50,650);
  expL->GetYaxis()->SetRangeUser(0.005,20);
  expL->SetTitle();

  exp_1sig->SetFillColor(kGreen);
  exp_2sig->SetFillColor(kYellow);
  exp_1sig->SetLineColor(kGreen);
  exp_2sig->SetLineColor(kYellow);
  expL->Draw("L same");
  obsL->Draw("L same");
  xsecL->Draw("L same 3");

  gPad->RedrawAxis();

  TLegend *l1=new TLegend(0.6,0.65,0.90,0.90);
  //l1=new TLegend(0.7,0.65,0.95,0.9);
  //l1->SetLineColor(0);
  //l1->SetFillColor(0);
  //l1->SetFillStyle(0);
  //l1->SetTextFont(42);
  l1->SetTextSize(0.04);
  //l1->SetBorderSize(0);


  TGraphErrors *xsecLDummy = (TGraphErrors*) xsecL->Clone();
  xsecLDummy->SetLineWidth(3);
  TGraphErrors *expLDummy = (TGraphErrors*) expL->Clone();
  expLDummy->SetLineWidth(3);
  TGraphErrors *obsLDummy = (TGraphErrors*) obsL->Clone();
  obsLDummy->SetLineWidth(3);
  l1->AddEntry(obsLDummy,"Observed limit","l");
  l1->AddEntry(expLDummy,"Expected limit","l");
  l1->AddEntry(exp_1sig,"#pm 1#sigma exp. limit","f");
  l1->AddEntry(exp_2sig,"#pm 2#sigma exp. limit","f");
  l1->AddEntry(xsecLDummy,"#sigma^{theo}","l");

  //l1->AddEntry(xsecL,"Theoretical x-section","l");

  l1->Draw();

  TLatex*  info   = new TLatex();
  info-> SetNDC();
  //info->SetTextSize(0.04);
  TString AuxString = Form("c#tau = %icm",ctau);
  //info->DrawLatex(0.62,0.61,AuxString);
  info->DrawLatex(0.2,0.22,AuxString);

  info->SetTextSize(0.045);
  info->DrawLatex(0.65,0.95,"19.7 fb^{-1} (8 TeV)");
  info->SetTextSize(0.040);
  //info->DrawLatex(0.65,0.50,"pp #rightarrow #chi^{#pm}#chi^{#pm}");
  //info->DrawLatex(0.65,0.45,"pp #rightarrow #chi^{#pm}#chi^{0}");

  c1->SetTickx();
  c1->SetTicky();
  c1->Update();
  c1->SaveAs(Form("test_ctau%icm.pdf",ctau));

  
  return;

}
