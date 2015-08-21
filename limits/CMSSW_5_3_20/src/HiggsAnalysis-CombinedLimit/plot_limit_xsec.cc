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

string dTs(double d){
 std::stringstream tmp;
 tmp<<d;
 string str=tmp.str();
 return str;

}

void make_plot(vector<double> mass, vector<double> xsecTheo, vector<double> xsecTheoErr, vector<double> obs,vector<double> exp, vector<double> exp_1sig_up,  vector<double> exp_2sig_up,vector<double> exp_1sig_down,  vector<double> exp_2sig_down, int ctau);

void plot_limit_xsec(TString filename, int ctau){
  //this macro should read in limit results and make a nice plot out of it!
  //hard coded for asymptotic limit format for now

  TString ctauToString = Form("%icm",ctau);
  cout<<ctauToString<<endl;

  vector<double> obs;
  vector<double> exp;
  vector<double> exp_1sig_up;
  vector<double> exp_2sig_up;
  vector<double> exp_1sig_down;
  vector<double> exp_2sig_down;

  vector<double> xsecTheo;
  vector<double> xsecTheoErr;
  // Read in theoretical cross-sections
  ifstream inputFile("xsectionsUpdated.txt");
  int lines;
  int it=1;
  TString dataset;
  double xsec,xsecErr;
  while(inputFile>>dataset>>xsec>>xsecErr){
    if(!dataset.Contains(ctauToString)) continue;
    cout<<"name = "<<dataset<<endl;
    cout<<"xsec = "<<xsec<<endl;
    xsecTheo.push_back(xsec);
    xsecTheoErr.push_back(xsecErr);
    it++;
  }
  lines=it;
  inputFile.close();
  cout<<"haha2"<<endl;


  //to be nicend in the future;
  vector<double> mass;
  mass.push_back(100);
  mass.push_back(200);
  mass.push_back(300);
  mass.push_back(400);
  mass.push_back(500);
  
  vector<string> files;
  files.push_back(Form("logFiles/" + filename + "/res_mass_100GeV_ctau_%icm.log",ctau));
  files.push_back(Form("logFiles/" + filename + "/res_mass_200GeV_ctau_%icm.log",ctau));
  files.push_back(Form("logFiles/" + filename + "/res_mass_300GeV_ctau_%icm.log",ctau));
  files.push_back(Form("logFiles/" + filename + "/res_mass_400GeV_ctau_%icm.log",ctau));
  files.push_back(Form("logFiles/" + filename + "/res_mass_500GeV_ctau_%icm.log",ctau));

  cout<<files.size()<<endl;
 
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
	    bool done=false;
	    std::istringstream ss(line);
	    string s1,s2,s3,s4,s5,s6,s7;
	    double value=-1;
	    ss >> s1 >> s2 >> s3 >> s4 >> value;
	    if(value!=-1){
	      if(s1=="Observed"){
		obs.push_back(value);
		done=true;
	      }
	      if(s1=="Expected" && s2=="50.0%:"){
		cout<<"expected limit = "<<value<<endl;
		exp.push_back(value);
		done=true;
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

  
  make_plot(mass,xsecTheo,xsecTheoErr,obs,exp,exp_1sig_up,exp_2sig_up,exp_1sig_down,exp_2sig_down,ctau);

  return;
}


void make_plot(vector<double> mass, vector<double> xsecTheo, vector<double> xsecTheoErr, vector<double> obs,vector<double> exp, vector<double> exp_1sig_up,  vector<double> exp_2sig_up,vector<double> exp_1sig_down,  vector<double> exp_2sig_down, int ctau){
  TCanvas *c1 = new TCanvas;
  c1->SetLogy();

  //gStyle->SetPadBottomMargin(10.5);
  gPad->SetBottomMargin(0.14);
  gPad->SetLeftMargin(0.14);
  

  //TGraphAsymmErrors *exp_1sig= new TGraphAsymmErrors(mass,exp,exp_1sig_up,exp_1sig_down);
  const int nn = 11;
  
  TLegend *l1=new TLegend(0.15,0.65,0.45,0.85);
  //l1=new TLegend(0.7,0.65,0.95,0.9);
  l1->SetLineColor(0);
  l1->SetFillColor(0);
  l1->SetFillStyle(0);
  l1->SetTextFont(42);
  l1->SetTextSize(0.04);
  l1->SetBorderSize(0);



  double x[nn];
  double xUpperErr[nn];
  double xLowerErr[nn];
  double yUpperErr[nn];
  double yLowerErr[nn];
  double yUpperErr2[nn];
  double yLowerErr2[nn];
  double xsec[nn];
  double xsecErr[nn];


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
    xsec[j]=xsecTheo[j];
    xsecErr[j]=xsecTheoErr[j];
    //cout<<exp_1sig_down[j]<<" "<<exp_2sig_down[j]<<endl;
  }
  TGraph *expL= new TGraph(nBins,x,yexp);
  TGraph *obsL= new TGraph(nBins,x,yobs);
  TGraphErrors *xsecL= new TGraphErrors(nBins,x,xsec,xLowerErr,xsecErr);

  TGraphAsymmErrors *exp_1sig = new TGraphAsymmErrors(nBins, x, yexp, xLowerErr, xUpperErr, yLowerErr, yUpperErr);
  TGraphAsymmErrors *exp_2sig = new TGraphAsymmErrors(nBins, x, yexp, xLowerErr, xUpperErr, yLowerErr2, yUpperErr2);
  //exp_1sig->Draw("AF");
  expL->Draw("ALP");
  exp_2sig->Draw("same l3");
  exp_1sig->Draw("same l3");


  obsL->SetLineColor(kBlack);
  obsL->SetLineWidth(2);
  xsecL->SetLineColor(kGreen+1);
  xsecL->SetFillColor(kGreen+1);
  xsecL->SetLineWidth(2);

  //expL->SetMarkerStyle(22);
  expL->SetLineColor(kRed);
  expL->SetLineWidth(2);
  expL->GetXaxis()->SetTitleSize(0.05);
  expL->GetYaxis()->SetTitleSize(0.05);
  expL->GetXaxis()->SetTitle("mass_{#Chi^{#pm}} (GeV)");
  expL->GetYaxis()->SetTitle("r = #sigma_{expected} / #sigma_{theo}");
  expL->GetXaxis()->SetRangeUser(50,550);
  expL->GetYaxis()->SetRangeUser(0.01,10);
  expL->SetTitle();

  exp_1sig->SetFillColor(kGreen);
  exp_2sig->SetFillColor(kYellow);
  expL->Draw("LP same");
  obsL->Draw("LP same");
  xsecL->Draw("LP same 3");

  gPad->RedrawAxis();

  l1->AddEntry(obsL,"Observed Limit","l");
  l1->AddEntry(expL,"Expected Limit","l");
  l1->AddEntry(exp_1sig,"#pm 1#sigma","f");
  l1->AddEntry(exp_2sig,"#pm 2#sigma","f");
  l1->AddEntry(xsecL,"Theoretical x-section","l");

  l1->Draw();

  TLine *line = new TLine(60,1,540,1);
  line->SetLineColor(kBlack);
  line->SetLineWidth(2);
  //line->Draw("same");

  c1->SetTickx();
  c1->SetTicky();
  c1->Update();
  c1->SaveAs(Form("test_ctau%icm.pdf",ctau));

  
  return;

}
