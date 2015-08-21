//-----------------------------------------------------------------------------
// File:        analyzer.cc
// Description: Analyzer for ntuples created by TheNtupleMaker
// Created:     Thu Jul 24 15:23:41 2014 by mkanalyzer.py
// Author:      Teresa Lenz
//-----------------------------------------------------------------------------
#include "TVector2.h"
#include "TLorentzVector.h"
#include "analyzer.h"
using namespace std;
using namespace evt;
//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  // Get file list and histogram filename from command line

  commandLine cmdline;
  decodeCommandLine(argc, argv, cmdline);

  // Get names of ntuple files to be processed and open chain of ntuples

  vector<string> filenames = getFilenames(cmdline.filelist);
  itreestream stream(filenames, "Events");
  if ( !stream.good() ) error("unable to open ntuple file(s)");

  // Get number of events to be read

  int nevents = stream.size();
  cout << "Number of events: " << nevents << endl;

  // Select variables to be read

  selectVariables(stream);


  // The root application is needed to make canvases visible during
  // program execution. If this is not needed, just comment out the
  // following line

  TApplication app("analyzer", &argc, argv);

  /**
	 Notes 1
	 -------
	 1. Use
	   ofile = outputFile(cmdline.outputfile, stream)

	 to skim events to output file in addition to writing out histograms.

	 2. Use
	   ofile.addEvent(event-weight)

	 to specify that the current event is to be added to the output file.
	 If omitted, the event-weight is defaulted to 1.

	 3. Use
		ofile.count(cut-name, event-weight)

	 to keep track, in the count histogram, of the number of events
	 passing a given cut. If omitted, the event-weight is taken to be 1.
	 If you want the counts in the count histogram to appear in a given
	 order, specify the order, before entering the event loop, as in
	 the example below

		ofile.count("NoCuts", 0)
		ofile.count("GoodEvent", 0)
		ofile.count("Vertex", 0)
		ofile.count("MET", 0)

     Notes 2
	 -------
	 By default all variables are saved. Before the event loop, you can use
  
       select(objectname)
	  
     e.g.,
	
       select("GenParticle")
  
     to declare that you intend to select objects of this type. The
	 selection is done using

       select(objectname, index)
	  
     e.g.,
	  
       select("GenParticle", 3),
  
     which is called within the event loop. Call saveSelectedObjects()
	 before a call to addEvent if you wish to save the selected objects.
	 All other objects are saved by default.
	 
	 NB: If you declare your intention to select objects of a given type
	     by calling select(objectname), but subsequently fail to select
	     them using select(objectname, index) then none will be saved!
  */

  outputFile ofile(cmdline.outputfilename);

  //---------------------------------------------------------------------------
  // Declare histograms
  //---------------------------------------------------------------------------
  TH1D* hAllHits = new TH1D("hAllHits","hAllHits",100,0,500);
  TH1D* hHitsEq3 = new TH1D("hHitsEq3","hHitsEq1",100,0,500);
  TH1D* hRecEff  = new TH1D("hRecEff","hRecEff",1,0,1);
  TH1D* hdZ      = new TH1D("hdZ","hdZ",100,-10,10); 
  //---------------------------------------------------------------------------
  // Loop over events
  //---------------------------------------------------------------------------

  std::vector<TrackAllHits_s> TrackColl;

  for(int entry=0; entry < nevents; ++entry)
	{
	  // Read event into memory
	  stream.read(entry);

	  // NB: call to clear object selection map (indexmap)
	  initialize();
	  
	  // Uncomment the following line if you wish to copy variables into
	  // structs. See the header file analyzer.h to find out what structs
	  // are available. Alternatively, you can call individual fill functions.
	  fillObjects();


	  // ---------------------
	  // -- event selection --
	  // ---------------------

	  if(MuonTracksAllHits.size()==0) continue;
	  for(unsigned int i=0; i<MuonTracksAllHits.size(); i++){
    

	    if(MuonTracksAllHits_pt[i] < __ptCutMin__) continue;
	    if(MuonTracksAllHits_pt[i] > __ptCutMax__) continue;

	    // Muon quality cuts to ensure good tracks
	    if(abs(MuonTracksAllHits_eta[i])>2.1)           continue;
	    if(MuonTracksAllHits_numberOfValidHits[i] < 20) continue;
	    if(MuonTracksAllHits_hitPattern_trackerLayersWithoutMeasurement[i] != 0) continue;
	    if(MuonTracksAllHits_trackerExpectedHitsInner_numberOfLostHits[i]  != 0) continue;
	    if(MuonTracksAllHits_trackerExpectedHitsOuter_numberOfHits[i]      != 0) continue;
	    if(MuonTracksAllHits_chi2[i]/MuonTracksAllHits_ndof[i] > 5)  continue;


	    // Vertex requirement - loose vertex cuts to keep statistics high!
	    double _dvx = MuonTracksAllHits_vx[i] - Vertex[0].x;
	    double _dvy = MuonTracksAllHits_vy[i] - Vertex[0].y;
	    double d0   = abs( - _dvx*MuonTracksAllHits_py[i] + _dvy*MuonTracksAllHits_px[i])/MuonTracksAllHits_pt[i];
	    double _dvz = MuonTracksAllHits_vz[i] - Vertex[0].z;
	    double dZ   = _dvz - ( _dvx*MuonTracksAllHits_px[i] + _dvy*MuonTracksAllHits_py[i])/MuonTracksAllHits_pt[i] * (MuonTracksAllHits_pz[i]/MuonTracksAllHits_pt[i]);
	    
	    if(abs(d0)>0.02)                      continue;
	    if(abs(dZ)>20)                       continue;

	    hAllHits->Fill(1);

	    for(unsigned int j=0; j<Track.size(); j++){

	      // Matching
	      double dPhi = std::abs(TVector2::Phi_mpi_pi(MuonTracksAllHits_phi[i]-Track_phi[j]));
	      double dEta = std::abs(MuonTracksAllHits_eta[i]-Track_eta[j]);
	      double dR   = std::sqrt(dPhi*dPhi + dEta*dEta); 
	    
	      if(dR<0.01){
		hdZ->Fill(dZ);
		hHitsEq3->Fill(1);
		break;

	      }
	      //cout<<endl;
	    }

	  }

	  hRecEff->SetBinContent(1,hHitsEq3->Integral()/hAllHits->Integral());
	  
	  // ---------------------
	  // -- fill histograms --
	  // ---------------------	  

	}

  cout<<endl;
  cout.precision(0);
  double ratio      = hRecEff->GetBinContent(1);
  double ratioError = ratio*sqrt(pow(sqrt(hHitsEq3->Integral())/hHitsEq3->Integral(),2) + pow(sqrt(hAllHits->Integral())/hAllHits->Integral(),2) ) ;
  cout<<"n Tracks 3 hits   = "<<fixed<<hHitsEq3->Integral()<<endl;
  cout<<"n Tracks all hits = "<<fixed<<hAllHits->Integral()<<endl;
  cout.precision(3);
  cout<<"Track reconstruction efficiency = "<<fixed<<ratio<<" +/- "<<ratioError<<endl;
  cout<<endl;
  stream.close();
  ofile.close();
  return 0;
}
