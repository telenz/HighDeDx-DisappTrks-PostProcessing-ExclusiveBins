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
#include "TF1.h"
#include "TH1F.h"
#include "TMarker.h"
#include "TFrame.h"

string dTs(double d){
 std::stringstream tmp;
 tmp<<d;
 string str=tmp.str();
 return str;

}

double ctauMin=1;
double massMin=100;

void make_plot(vector<double> mass, vector<double> xsecTheo, vector<double> xsecTheoErr, vector<double> obs,vector<double> exp, vector<double> exp_1sig_up,  vector<double> exp_2sig_up,vector<double> exp_1sig_down,  vector<double> exp_2sig_down, int ctau);

double getIntersectionPoint(vector<double> mass, vector<double> xsecTheo, vector<double> xsecTheoErr, vector<double> exp, TString title);

void plot_limit_2d(TString filename){
  //this macro should read in limit results and make a nice plot out of it!
  //hard coded for asymptotic limit format for now

  vector<int> lifetimes;
  lifetimes.push_back(5);
  lifetimes.push_back(10);
  lifetimes.push_back(50);
  lifetimes.push_back(100);

  int size = lifetimes.size()+2;

  double* ctau_2d = new double[size];
  double* ctau_Error_2d = new double[size];
  double* obs_2d = new double[size];
  double* exp_2d = new double[size];
  double* exp_1sig_up_2d = new double[size];
  double* exp_1sig_down_2d = new double[size];
  double* exp_2sig_up_2d = new double[size];
  double* exp_2sig_down_2d = new double[size];


  for(unsigned int ct=0; ct<lifetimes.size();ct++){

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
    // Read in theoretical cross-sections
    ifstream inputFile("xsectionsUpdated.txt");
    int lines;
    int it=1;
    TString dataset;
    double xsec,xsecErr;
    while(inputFile>>dataset>>xsec>>xsecErr){
      if(!dataset.Contains(ctauToString)) continue;
      //cout<<"name = "<<dataset<<endl;
      //cout<<"xsec = "<<xsec<<endl;
      xsecTheo.push_back(xsec);
      xsecTheoErr.push_back(xsecErr);
      it++;
    }
    lines=it;
    inputFile.close();


    //to be nicend in the future;
    vector<double> mass;
    mass.push_back(100);
    mass.push_back(200);
    mass.push_back(300);
    mass.push_back(400);
    mass.push_back(500);
  
    vector<string> files;
    files.push_back(Form("logFiles/" + filename + "/res_mass_100GeV_ctau_%icm.log",lifetimes[ct]));
    files.push_back(Form("logFiles/" + filename + "/res_mass_200GeV_ctau_%icm.log",lifetimes[ct]));
    files.push_back(Form("logFiles/" + filename + "/res_mass_300GeV_ctau_%icm.log",lifetimes[ct]));
    files.push_back(Form("logFiles/" + filename + "/res_mass_400GeV_ctau_%icm.log",lifetimes[ct]));
    files.push_back(Form("logFiles/" + filename + "/res_mass_500GeV_ctau_%icm.log",lifetimes[ct]));

    //cout<<files.size()<<endl;
 
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
		    //cout<<"expected limit = "<<value<<endl;
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

  
    

    double intExp = getIntersectionPoint(mass,xsecTheo,xsecTheoErr,exp,Form("expected_%icm",lifetimes[ct]));
    cout<<"intExp = "<<intExp<<endl;
    double intExp_1sig_up = getIntersectionPoint(mass,xsecTheo,xsecTheoErr,exp_1sig_down,Form("expected_1sigma_up_%icm",lifetimes[ct]));
    cout<<"intExp_1sig_up = "<<intExp_1sig_up<<endl;
    double intExp_1sig_down = getIntersectionPoint(mass,xsecTheo,xsecTheoErr,exp_1sig_up,Form("expected_1sigma_down_%icm",lifetimes[ct]));
    cout<<"intExp_1sig_down = "<<intExp_1sig_down<<endl;
    double intExp_2sig_up = getIntersectionPoint(mass,xsecTheo,xsecTheoErr,exp_2sig_down,Form("expected_2sigma_up_%icm",lifetimes[ct]));
    cout<<"intExp_2sig_up = "<<intExp_2sig_up<<endl;
    double intExp_2sig_down = getIntersectionPoint(mass,xsecTheo,xsecTheoErr,exp_2sig_up,Form("expected_2sigma_down_%icm",lifetimes[ct]));
    cout<<"intExp_2sig_down = "<<intExp_2sig_down<<endl;
    double intObs = getIntersectionPoint(mass,xsecTheo,xsecTheoErr,obs,Form("observed_%icm",lifetimes[ct]));
    cout<<"intObs = "<<intObs<<endl;
    cout<<endl;

    
    if(ct==0){

      ctau_2d[ct]          = ctauMin;
      ctau_Error_2d[ct]    = 0;
      obs_2d[ct]           = massMin;
      exp_2d[ct]           = massMin;
      exp_1sig_up_2d[ct]   = massMin;
      exp_1sig_down_2d[ct] = massMin;
      exp_2sig_up_2d[ct]   = massMin;
      exp_2sig_down_2d[ct] = massMin;
    }
    
    
    ctau_2d[ct+1]          = lifetimes[ct];
    ctau_Error_2d[ct+1]    = 0;
    obs_2d[ct+1]           = intObs;
    exp_2d[ct+1]           = intExp;
    exp_1sig_up_2d[ct+1]   = intExp_1sig_up;
    exp_1sig_down_2d[ct+1] = intExp_1sig_down;
    exp_2sig_up_2d[ct+1]   = intExp_2sig_up;
    exp_2sig_down_2d[ct+1] = intExp_2sig_down;
    

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
    make_plot(mass,xsecTheo,xsecTheoErr,obs,exp,exp_1sig_up,exp_2sig_up,exp_1sig_down,exp_2sig_down,lifetimes[ct]);    

  }


  ctau_2d[size-1] = ctau_2d[size-2];
  ctau_Error_2d[size-1] = 0;
  obs_2d[size-1] = obs_2d[0];
  exp_2d[size-1] = exp_2d[0];
  exp_1sig_up_2d[size-1] = exp_1sig_up_2d[0];
  exp_1sig_down_2d[size-1] = exp_1sig_down_2d[0];
  exp_2sig_up_2d[size-1] = exp_2sig_up_2d[0];
  exp_2sig_down_2d[size-1] = exp_2sig_down_2d[0];


  TCanvas *c = new TCanvas;
  //c->SetLogy();

  gPad->SetBottomMargin(0.14);
  gPad->SetLeftMargin(0.14);
  
  TLegend *l1=new TLegend(0.15,0.65,0.45,0.85);
  l1->SetLineColor(0);
  l1->SetFillColor(0);
  l1->SetFillStyle(0);
  l1->SetTextFont(42);
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
  TGraph* g_exp           = new TGraph(nBins, exp_2d, ctau_2d);
  TGraph* g_obs           = new TGraph(nBins, obs_2d, ctau_2d);


  g_exp_2sig_up->GetXaxis()->SetLimits(100,500);

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


  g_exp_2sig_up->GetXaxis()->SetTitleSize(0.05);
  g_exp_2sig_up->GetYaxis()->SetTitleSize(0.05);
  g_exp_2sig_up->GetXaxis()->SetTitle("mass_{#Chi^{#pm}} (GeV)");
  g_exp_2sig_up->GetYaxis()->SetTitle("c#tau_{#Chi^{#pm}} [cm]");
  g_exp_2sig_up->GetYaxis()->SetRangeUser(ctauMin,110);
  g_exp_2sig_up->SetTitle();

  gPad->RedrawAxis();

  l1->AddEntry(g_obs,"Observed Limit","l");
  l1->AddEntry(g_exp,"Expected Limit","l");
  l1->AddEntry(g_exp_1sig_up,"#pm 1#sigma","f");
  l1->AddEntry(g_exp_2sig_up,"#pm 2#sigma","f");

  l1->Draw();

  c->SetTickx();
  c->SetTicky();
  c->Update();
  c->SaveAs("LimitPlot_2d.pdf");

  return;
}

TF1* f1, *f2, *fint;
double finter(double *x, double*par) {
  //return 10;
  return TMath::Abs(f1->EvalPar(x,par) - f2->EvalPar(x,par));
}

double getIntersectionPoint(vector<double> mass, vector<double> xsecTheo, vector<double> xsecTheoErr, vector<double> exp, TString title){

  vector<double> xsecTheoLow;
  vector<double> expTimesXsec;
  for(unsigned int i=0; i<xsecTheo.size();i++){
    xsecTheoLow.push_back(xsecTheo[i]-xsecTheoErr[i]);
    expTimesXsec.push_back(exp[i]*xsecTheo[i]);
  }


  TCanvas *c3=0;
  // Get TF1 between xSecTheoLow  points
  for(unsigned int i=0; i<xsecTheoLow.size()-1;i++){
    f1 = new TF1("f1","[0]*x+[1]",mass[i],mass[i+1]);
    f1->SetRange(mass[i],mass[i+1]);
    double a = (xsecTheoLow[i]-xsecTheoLow[i+1])/(mass[i]-mass[i+1]);
    double b = xsecTheoLow[i]-a*mass[i];
    f1->SetParameter(0,a);
    f1->SetParameter(1,b);
   
   
    f2 = new TF1("f2","[0]*x+[1]",mass[i],mass[i+1]);
    f2->SetRange(mass[i],mass[i+1]);
    a = (expTimesXsec[i]-expTimesXsec[i+1])/(mass[i]-mass[i+1]);
    b = expTimesXsec[i]-a*mass[i];
    f2->SetParameter(0,a);
    f2->SetParameter(1,b);
  
  
    double xmin, xmax;
    f1->GetRange(xmin,xmax);
    fint = new TF1("fint_"+title,finter,xmin,xmax,0);
    double xint = fint->GetMinimumX();
    fint->SetLineColor(1);
    /*
    c3= new TCanvas("c3_"+title,title);
    c3->cd();
    //c3->SetLogy();
    f1->SetMaximum(1);
    f1->SetMinimum(0.0);
    f1->SetTitle(title);
    f1->Draw();
    f2->Draw("same");
    fint->Draw("lsame");
    */
    if(xint!=mass[i] && xint !=mass[i+1]){
      //c3->SaveAs("intersection_"+title+".pdf");
      return xint;
    }
    delete c3;
  }


  return -1;

}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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
  expL->GetXaxis()->SetRangeUser(100,500);
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