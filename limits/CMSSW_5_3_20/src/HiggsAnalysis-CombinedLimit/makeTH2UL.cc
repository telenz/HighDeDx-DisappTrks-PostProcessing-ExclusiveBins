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
#include "TH2.h"
#include "TMarker.h"
#include "TFrame.h"
#include "TMath.h"
#include "TLatex.h"
#include "TFile.h"


void makeTH2UL(TString filename){

  double bins_cm[37] = {2,3,4,5,6,7,8,9,10,20,30,40,50,60,70,80,90,100,200,300,400,500,600,700,800,900,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,20000};
  TH2D* obsUL_cm  = new TH2D("obsUL_cm","obsUL_cm",6,100,700,36,bins_cm);

  double bins_ns[37];

  for(int i=0; i<37; i++){

    bins_ns[i] = bins_cm[i]/(100.*TMath::C())*pow(10,9);

  }

  TH2D* obsUL_ns  = new TH2D("obsUL_ns","obsUL_ns",6,100,700,36,bins_ns);

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


  for(unsigned int ct=0; ct<lifetimes.size();ct++){

    cout<<endl<<"ctau = "<<lifetimes[ct]<<"cm :"<<endl;
    TString ctauToString = Form("%icm",lifetimes[ct]);

    vector<double> obs;
    vector<double> exp;
    vector<double> exp_1sig_up;
    vector<double> exp_2sig_up;
    vector<double> exp_1sig_down;
    vector<double> exp_2sig_down;

    vector<double> xsecTheo;
    vector<double> xsecTheoErr;
    vector<double> mass;

    // Read in theoretical cross-sections
    ifstream inputFile("xsections_m100-m600.txt");
    
    int it=1;
    double dataset,xsec,xsecErr;
    while(inputFile>>dataset>>xsec>>xsecErr){
      mass.push_back(dataset);
      xsecTheo.push_back(xsec/1000.);
      xsecTheoErr.push_back(xsecErr/1000.);
      it++;
    }
    inputFile.close();


  
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


    vector<double> expTimesXsec;
    for(unsigned int j=0; j<xsecTheo.size();j++){
      
      expTimesXsec.push_back(exp[j]*xsecTheo[j]);
      obsUL_cm -> Fill(mass[j],lifetimes[ct],expTimesXsec[j]);
      obsUL_ns -> Fill(mass[j],lifetimes[ct]/(100.*TMath::C())*pow(10,9),expTimesXsec[j]);
    }


  }

  TFile* f = new TFile("fobsUL.root","RECREATE");
  obsUL_cm->Write();
  obsUL_ns->Write();
  f->Close();


}

