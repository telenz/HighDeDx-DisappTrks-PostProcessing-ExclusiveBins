#include "TMath.h"
#include <iostream>
#include <cmath>
#include <iomanip>   




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

void calculateMassSplittingIter(double ctau, double mass){

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
      	cout<<setprecision(10)<<"deltaM = "<<deltaM<<endl;
	cout<<setprecision(10)<<"lifetimeGuess = "<<lifetimeGuessinCM<<endl;
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

   
}

/*
def calcMassSplitting(mass, lifetimeInSec, deltaM, prevDeltaMGuess):
    #    print "Debug:  running calcMassSplitting with mass=", mass, ", lifetimeInSec=", lifetimeInSec, ", deltaM=", deltaM, ", prevDeltaMGuess=", prevDeltaMGuess
    
    fPi = float(0.093)
    gF = 1.166379E-5   # units if GeV^-2
    mPi = 0.13957018  # units of GeV, from PDG
    hBarinGeVs = 6.582119E-25
    neuMass = float(mass) - float(deltaM)
 
    a = math.pow(float(fPi),2) * math.pow(float(gF),2) / (4 * math.pi * math.pow(float(mass),2))
    b = (math.sqrt((math.fabs(math.pow((math.pow((mass),2) + math.pow((neuMass),2) - math.pow((mPi),2)), 2) - (4) * (math.pow((mass),2)) * (math.pow((neuMass),2)) ))))/(2*(mass))  #old, mostly works
    c = math.pow(math.pow(mass, 2) - math.pow(neuMass, 2),2)
    d = float(math.pow(float(mPi),2))
 
    lifetimeGuess = hBarinGeVs / (4.0 * a * b * (c - d * deltaM))
    precision = 0.0001  # precision of desired result
    lowerlimit = mPi  # value below which deltaM is undefined
    if math.fabs((lifetimeInSec - lifetimeGuess) / lifetimeInSec) < precision:
        return (deltaM, prevDeltaMGuess)
    elif math.fabs((deltaM - prevDeltaMGuess) / deltaM) < precision / 10:
        #        print "Difference between deltaM=", deltaM, " and prevDeltaMGuess=", prevDeltaMGuess, " is less than precision = ', precision
        return (deltaM, prevDeltaMGuess)
    else:
        if (lifetimeInSec - lifetimeGuess) > 0:
            tmp = deltaM
            deltaM -= 0.5 * math.fabs(deltaM - prevDeltaMGuess)
            if deltaM < lowerlimit:
                deltaM = lowerlimit * (1.0 + precision)
            prevDeltaMGuess = tmp
            return calcMassSplitting(mass, lifetimeInSec, deltaM, prevDeltaMGuess)
        else:
            tmp = deltaM
            deltaM += 0.5 * math.fabs(deltaM - prevDeltaMGuess)
            if deltaM < lowerlimit:
                deltaM = lowerlimit * (1.0 + precision)
            prevDeltaMGuess = tmp
            return calcMassSplitting(mass, lifetimeInSec, deltaM, prevDeltaMGuess)
*/
