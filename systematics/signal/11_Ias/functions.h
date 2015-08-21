#ifndef FUNCTIONS_H
#define FUNCTIONS_H
//-----------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include "TVector2.h"
#include <vector>
#include "TTree.h"
#include "TFile.h"
#include <fstream>
#include "TROOT.h"
#include "TInterpreter.h"
#include "TApplication.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TKey.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMath.h"

#include "TVector2.h"
#include "TLorentzVector.h"
#include "TH3.h"
using namespace std;
using namespace evt;
//-----------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
TH3* loadDeDxTemplate(string path){
  TFile* InputFile = new TFile(path.c_str());
  TH3*  DeDxMap_;
  InputFile->GetObject("Charge_Vs_Path",DeDxMap_);
  if(!DeDxMap_){printf("dEdx templates in file %s can't be open\n", path.c_str()); exit(0);}

  TH3* Prob_ChargePath  = (TH3*)(DeDxMap_->Clone("Prob_ChargePath"));
  Prob_ChargePath->Reset();
  Prob_ChargePath->SetDirectory(0);

  for(int i=0;i<=Prob_ChargePath->GetXaxis()->GetNbins()+1;i++){        // pt dimension
    for(int j=0;j<=Prob_ChargePath->GetYaxis()->GetNbins()+1;j++){      // path dimension
      double Ni = 0;
      for(int k=0;k<=Prob_ChargePath->GetZaxis()->GetNbins()+1;k++){Ni+=DeDxMap_->GetBinContent(i,j,k);}  
      for(int k=0;k<=Prob_ChargePath->GetZaxis()->GetNbins()+1;k++){  // charge/path dimension 
	double tmp = 0;
	for(int l=0;l<=k;l++){ tmp+=DeDxMap_->GetBinContent(i,j,l);} // sum over all bins with dEdx equal or smaller than the current one
	if(Ni>0){
	  Prob_ChargePath->SetBinContent (i, j, k, tmp/Ni);
	}else{
	  Prob_ChargePath->SetBinContent (i, j, k, 0);
	}
      }
    }
  }
  InputFile->Close();
  return Prob_ChargePath;
}
//--------------------------------------------------------------------------------------------------
/* This is a sorting algorithm, which sorts the dE/dx entries of vector_prob according the radius 
   where they are deposited. The sorted vector is filled into vect_prob_sorted.
   The new vector vect_prob_sorted is sorted such that the dE/dx value deposited closest to the 
   beam pipe is the first entry. */
void sortVectorAccordingRadius(vector<double> *vect_prob_sorted, vector<double> *vect_prob, vector<double> *vect_R, vector<double> *vect_dx, double* dedx1=0, double* dedx2=0, double* dedx3=0, double* dedx4=0, double* dx1=0, double* dx2=0, double* dx3=0, double* dx4=0)
{
  int *idx = new int[vect_prob->size()];
  vector<double> aux_R (*vect_R);
  vector<double> aux_dx_sorted;

  for(unsigned int j=0; j<vect_R->size(); j++)
    {
      double aux = 10000;
      for(unsigned int i=0; i<aux_R.size(); i++)
	{ 
	  if(aux_R[i]<aux)
	    {
	      aux=aux_R[i];
	      idx[j] = i;
	    }
	}
      aux_R[idx[j]]=1000000.;
    }

  for(unsigned int j=0; j<vect_prob->size(); j++){
    vect_prob_sorted->push_back((*vect_prob)[idx[j]]);
    aux_dx_sorted.push_back((*vect_dx)[idx[j]]);
  }

  if(dedx1 && vect_R->size()>0){
    *dedx1=(*vect_prob_sorted)[0];
    *dx1  =aux_dx_sorted[0];
  }
  if(dedx2 && vect_R->size()>0){
    *dedx2=(*vect_prob_sorted)[1];
    *dx2  =aux_dx_sorted[1];
  }
  if(dedx3 && vect_R->size()>0){
    *dedx3=(*vect_prob_sorted)[2];
    *dx3  =aux_dx_sorted[2];
  }
  if(dedx4 && vect_R->size()>0){
    *dedx4=(*vect_prob_sorted)[3];
    *dx4  =aux_dx_sorted[3];
  }

  delete[] idx;
}
//--------------------------------------------------------------------------------------------------
double dEdxOnTheFly(std::vector<double> *HitsDeDx, std::vector<int> *HitsShapetest, std::vector<double> *HitsPathlength, std::vector<int> *HitsSubdetid, std::vector<double> *HitsTransverse, bool isData, TH3 *templateHistoStrip, TH3 *templateHistoPixel, bool usePixel, int nHits=0, double* dedx1=0, double* dedx2=0, double* dedx3=0, double* dedx4=0, double* dx1=0, double* dx2=0, double* dx3=0, double* dx4=0, int* measSize=0)
{


  const double globalPixelMC    = 3.50843;
  const double globalPixelData  = 3.5090;
  const double globalStrip      = 3.3027;
  
  int numStripMeas = 0;
  int numPixelMeas = 0;
  
  std::vector<double> vect_probs_strip;
  std::vector<double> vect_probs_pixel;
  std::vector<double> vect_probs;
  std::vector<double> vect_R;
  std::vector<double> vect_pathlength;
    
  double scaleFactorStrip = 1.;
  double scaleFactorPixel = 0.;
  if(isData) scaleFactorPixel = globalStrip/globalPixelData;
  else       scaleFactorPixel = globalStrip/globalPixelMC;

  scaleFactorPixel = scaleFactorPixel/265.;
  for(unsigned int j=0;j<(*HitsDeDx).size();j++){
    if((*HitsSubdetid)[j]>2 && !(*HitsShapetest)[j]){continue;}
    if(!usePixel && (*HitsSubdetid)[j]<=2) continue;

    double ProbStrip = 1.;
    double ProbPixel = 1.;
    
    // Strip
    if((*HitsSubdetid)[j]>2){
      if(templateHistoStrip){
	numStripMeas += 1;
	int    BinX   = templateHistoStrip->GetXaxis()->FindBin(50.0); // momentum bin -> not important
	int    BinY   = templateHistoStrip->GetYaxis()->FindBin((*HitsPathlength)[j]);
	int    BinZ   = templateHistoStrip->GetZaxis()->FindBin(scaleFactorStrip*(*HitsDeDx)[j]/(*HitsPathlength)[j]);
	ProbStrip     = templateHistoStrip->GetBinContent(BinX,BinY,BinZ);
      }
      vect_probs_strip.push_back(ProbStrip);
      vect_probs.push_back(ProbStrip);
      vect_R.push_back((*HitsTransverse)[j]);
      vect_pathlength.push_back((*HitsPathlength)[j]);
    }
    else{
      if(templateHistoPixel){
	numPixelMeas += 1;
	int    BinX   = templateHistoPixel->GetXaxis()->FindBin(50.0); // momentum bin -> not important
	int    BinY   = templateHistoPixel->GetYaxis()->FindBin((*HitsPathlength)[j]);
	int    BinZ   = templateHistoPixel->GetZaxis()->FindBin(scaleFactorPixel*(*HitsDeDx)[j]/(*HitsPathlength)[j]);
	ProbPixel     = templateHistoPixel->GetBinContent(BinX,BinY,BinZ);
      }
      vect_probs_pixel.push_back(ProbPixel);
      vect_probs.push_back(ProbPixel);
      vect_R.push_back((*HitsTransverse)[j]);
      vect_pathlength.push_back((*HitsPathlength)[j]);
    }
  }
  
  int size      = vect_probs.size();

  //if(size<=5 && dedx1){
  //  vector<double> vector_prob_sorted;
  //  sortVectorAccordingRadius(&vector_prob_sorted, &vect_probs, &vect_R, &vect_pathlength, dedx1, dedx2, dedx3, dedx4, dx1, dx2, dx3, dx4);
  //  if(measSize) *measSize=size;
  //}

  //vect_prob_sorted.erase(vect_prob_sorted.begin()+vect_prob_sorted.size()-1,vect_prob_sorted.end());

  if(nHits !=0  && size+nHits>=0){
    if(nHits<0)   size=size+nHits;
    else{
      if(size>nHits) size=nHits;
    }
    vect_probs.erase(vect_probs.begin()+size,vect_probs.end());
  }
 
  //Ias pixel + strip
  double P = 1.0/(12*size);

  // Why do I need to sort here? This makes a difference -> Yes it does
  std::sort(vect_probs.begin(), vect_probs.end(), std::less<double>() );
  for(int i=1;i<=size;i++){
    P += vect_probs[i-1] * pow(vect_probs[i-1] - ((2.0*i-1.0)/(2.0*size)),2);
    //P += vector_prob_sorted[i-1] * pow(vector_prob_sorted[i-1] - ((2.0*i-1.0)/(2.0*size)),2);
  }

  P *= (3.0/size);
  if(size<=0) P=-1;
  
  return P;
}
//--------------------------------------------------------------------------------------------------
#endif

