#include "TMath.h"
#include "TFile.h"
#include "TGraph.h"
#include "TBox.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include <iostream>
#include <cmath>
#include <iomanip>   
#include <fstream>
#include <vector>
#include "/afs/desy.de/user/t/tlenz/Thesis/rootFiles/plotStyleThesis.h"


/*
double calculateMassSplitting(double ctau, double m_chi){

  
  double f_pi  = 0.093;   // GeV
  double G_F   = 1.166*pow(10,-5);  // in GeV-2
  double hbar  = 6.582*pow(10,-25);  // in GeV*s
  double m_pi    = 0.13957; // in GeV


  double lifetime = ctau/(TMath::C()*100.);
  
  double m_chi2  = pow(m_chi,2);
  double m_pi2   = pow(m_pi,2);
  

  double k_pi = 1./(2.*m_chi) * sqrt(  pow( 2 * m_chi2 + m_pi2 ,2) - 4. * m_chi2 * m_pi2 );
  
  double width = hbar/lifetime;

  double a = pow( f_pi * G_F , 2 ) / (TMath::Pi() * m_chi2);
  double x = pow( width*a  , 1./3);

  cout<<"x = "<<x<<endl;
  double deltaM = ( 2*m_chi - sqrt( 4*m_chi2 -4 * x  ))/2.;
  
  //double deltaM = sqrt ( width *  TMath::Pi() * m_chi2 / ( pow( f_pi * G_F , 2 ) * k_pi * ( 4. * m_chi2 - m_pi2  )) );


  return deltaM;

}
*/

double calculateLifetime(double deltaM, double m_chi){

  
  double f_pi  = 0.093;   // GeV
  double G_F   = 1.166379*pow(10,-5);  // in GeV-2
  double hbar  = 6.582119*pow(10,-25);  // in GeV*s
  double m_pi   = 0.13957018; // in GeV

  double m_chi0 = m_chi - deltaM; // in GeV
  

  double m_chi2  = pow(m_chi,2);
  double m_chi02 = pow(m_chi0,2);
  double m_pi2   = pow(m_pi,2);
  
  double k_pi = 1./(2.*m_chi)*sqrt(  pow( m_chi2 + m_chi02 - m_pi2 ,2) -4.*m_chi2*m_chi02 );
  


  double width = pow(f_pi,2)*pow(G_F,2)/(4*TMath::Pi()*m_chi2);
  width = width * k_pi;
  width = width * ( 4 * ( pow( m_chi2 - m_chi02 , 2 ) - m_pi2*pow( m_chi2 - m_chi02 , 2 ) )  );


  return hbar/width*TMath::C()*100.;
}

double calculateMassSplittingIter(double ctau, double mass){

  double fPi = 0.093;
  double gF  = 1.166379*pow(10,-5);
  double mPi = 0.13957018; 
  double  hBarinGeVs = 6.582119*pow(10,-25);

  double lifetimeInSec = ctau/(TMath::C()*100.);
  
  double a = pow(fPi,2) *pow(gF,2) / (4 * TMath::Pi() * pow(mass,2)) ; 

  double precision  = 0.00001;
  double lowerlimit = mPi; 
  double deltaM     = 0.160;
  double prevDeltaMGuess = 0;

  for(int i=0; i<100; i++){


    //cout<<"iteration "<<i<<endl;

    double neuMass = mass - deltaM;
    double b = (sqrt((abs(pow((pow((mass),2) + pow((neuMass),2) - pow((mPi),2)), 2) - (4) * (pow((mass),2)) * (pow((neuMass),2)) ))))/(2*(mass));
    double c = pow(pow(mass, 2) - pow(neuMass, 2),2);
    double d = pow(mPi,2);



    double lifetimeGuess     = (double) hBarinGeVs / (4.0 * a * b * (c - d * deltaM));
    double lifetimeGuessinCM = (double) lifetimeGuess*TMath::C()*100.;


    if(abs(1 -  lifetimeGuess / lifetimeInSec) < precision){
      //cout<<setprecision(10)<<"deltaM = "<<deltaM<<endl;
      //cout<<setprecision(10)<<"lifetimeGuess = "<<lifetimeGuessinCM<<endl;
	break;
      }
      else{
	
	if(lifetimeInSec - lifetimeGuess>0){
	  double tmp = deltaM;
	  deltaM -= 0.5 * abs(deltaM - prevDeltaMGuess);
	  if( deltaM < lowerlimit){
	    deltaM = lowerlimit * (1.0 + precision);
	  }
	  prevDeltaMGuess = tmp;
	}
	else{
	  double tmp = deltaM;
	  deltaM += 0.5 * abs(deltaM - prevDeltaMGuess);
	  if( deltaM < lowerlimit){
	    deltaM = lowerlimit * (1.0 + precision);
	  }
	  prevDeltaMGuess = tmp;
	}
      }
  }
  return deltaM;
   
}

void make2dLimitPlot(){

  TeresaPlottingStyle::init();
  gStyle->SetNdivisions(506,"Y");
  gROOT->ForceStyle();


  TCanvas *c = new TCanvas();
  c->cd();

  TFile* input = new TFile("ObservedLimit.root","READ");
  TGraph* excl;
  input -> GetObject("Graph",excl);
  excl->Draw("APL");

  double *mass = 0;
  double *ctau = 0;
  mass = excl->GetX();
  ctau = excl->GetY();

  double* deltaM  = new double[20];
  double* chiMass = new double[20];
  cout<<setprecision(1);
  for(int i=0; i<20; i++){

    if(i==0) chiMass[i] = 100;
    else     chiMass[i] = mass[i];
    deltaM[i] = calculateMassSplittingIter(ctau[i], mass[i])*1000;
    if(i==1) deltaM[i-1] = deltaM[i];
    cout<<left<<"point = "<<left<<setw(10)<<fixed<<chiMass[i-1]<<"   "<<fixed<<ctau[i]<<"   "<<fixed<<deltaM[i-1]<<endl;
  }

  TGraph* g_obs = new TGraph(19,chiMass,deltaM);

  double ymin = 130.;
  double ymax = 220.;
  // ****************************************************************************
  g_obs -> SetLineColor(kRed); 
  g_obs -> SetLineWidth(2); 
  g_obs -> GetXaxis() -> SetLimits(100,600); 
  g_obs -> GetYaxis() -> SetRangeUser(ymin,ymax); 
  g_obs -> GetXaxis() -> SetTitle("mass_{#Chi^{#pm}_{1}} [GeV]"); 
  g_obs -> GetYaxis() -> SetTitle("#Delta m_{#Chi^{#pm}_{1}#Chi^{0}_{1}} [MeV]"); 
  // ****************************************************************************

  g_obs->Draw("AL");


  TBox *b = new TBox(100, ymin, 600, 139.57); 
  b->SetFillColor(kGray+1);
  b->Draw("same");


  TLegend *l1=new TLegend(0.50,0.75,0.80,0.90);
  l1->SetTextSize(0.05);

  l1->AddEntry(g_obs,"Observed Limit","l");

  l1->Draw();

  TLatex*  info   = new TLatex();
  info -> SetNDC();
  info -> SetTextSize(0.04);
  info -> DrawLatex(0.45,0.20,"#Delta m_{#Chi^{#pm}_{1}#Chi^{0}_{1}} < m_{#pi}");

  c->RedrawAxis();
  c->SaveAs("MassSplittingLimitPlot.pdf");

  delete[] mass;
  delete[] ctau;

}
