//-----------------------------------------------------------------------------
// File:        analyzer.cc
// Description: Analyzer for ntuples created by TheNtupleMaker
// Created:     Thu Jul 24 15:23:41 2014 by mkanalyzer.py
// Author:      Teresa Lenz
//-----------------------------------------------------------------------------
#include "analyzerWithVertexInformation.h"
//#include "analyzer.h"
#include "functions.h"
#include "hitInformation.h"
#include <vector>
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


  bool isData = false;
  if(filenames[0].find("Run2012") != std::string::npos) isData = true;

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
  TH1D *hIasNP          = new TH1D("hIasNP","hIasNP",100,0,1);
  TH2D *hIasPNP         = new TH2D("hIasPNP","hIasPNP",150,0,1.5,100,0,1.);
  TH1D *hIas            = new TH1D("hIas","hIas",100,0,1);
  TH2D *hIasP           = new TH2D("hIasP","hIasP",150,0,1.5,100,0,1.);
  TH1D *hHarm2NP        = new TH1D("hHarm2NP","hHarm2NP",100,0,14);
  TH2D *hHarm2PNP       = new TH2D("hHarm2PNP","hHarm2PNP",150,0,1.5,100,0,14.);
  TH1D *hHarm2          = new TH1D("hHarm2","hHarm2",100,0,14);
  TH2D *hHarm2P         = new TH2D("hHarm2P","hHarm2P",150,0,1.5,100,0,14.);

  //-------------------------------------------------------------------------------------
  // Declare additional branch addresses for hit information
  //-------------------------------------------------------------------------------------
  stream._chain->SetBranchAddress("recoTrackHelper_TrackRefitter_HitsDeDx",&HitsDeDx);
  stream._chain->SetBranchAddress("recoTrackHelper_TrackRefitter_HitsPathlength",&HitsPathlength);
  stream._chain->SetBranchAddress("recoTrackHelper_TrackRefitter_HitsShapetest",&HitsShapetest);
  stream._chain->SetBranchAddress("recoTrackHelper_TrackRefitter_HitsSubdetId",&HitsSubdetid);
  //stream._chain->SetBranchAddress("recoTrackHelper_TrackRefitter_HitsEta",&HitsEta);
  //stream._chain->SetBranchAddress("recoTrackHelper_TrackRefitter_HitsPhi",&HitsPhi);
  stream._chain->SetBranchAddress("recoTrackHelper_TrackRefitter_HitsTransverse",&HitsTransverse);

  //-------------------------------------------------------------------------------------
  // Read discriminator templates
  //-------------------------------------------------------------------------------------
  cout<<endl<<endl<<"------------ Is it data   : "<<isData<<" --------------"<<endl<<endl;

  if(isData){
    template_pixel = loadDeDxTemplate("/afs/desy.de/user/t/tlenz/HighDeDx-DisappTrks-Analyzer/data/Discrim_template_pixel_data_2012.root");
    template_strip = loadDeDxTemplate("/afs/desy.de/user/t/tlenz/HighDeDx-DisappTrks-Analyzer/data/Data7TeV_Deco_SiStripDeDxMip_3D_Rcd.root");
  }
  else{
    template_pixel = loadDeDxTemplate("/afs/desy.de/user/t/tlenz/HighDeDx-DisappTrks-Analyzer/data/Discrim_template_pixel_mc_2012.root");
    template_strip = loadDeDxTemplate("/afs/desy.de/user/t/tlenz/HighDeDx-DisappTrks-Analyzer/data/Discrim_Templates_MC_2012.root");
  }
  //---------------------------------------------------------------------------
  // Loop over events
  //---------------------------------------------------------------------------


  for(int entry=0; entry < nevents; ++entry)
    {
      // Read event into memory
      stream.read(entry);
      // This is important for hit information
      stream._chain->GetEntry(entry);

      // NB: call to clear object selection map (indexmap)
      initialize();
	  
      // Uncomment the following line if you wish to copy variables into
      // structs. See the header file analyzer.h to find out what structs
      // are available. Alternatively, you can call individual fill functions.
      fillObjects();


      // ---------------------
      // -- event selection --
      // ---------------------
      for(unsigned int i=0; i<Track.size(); i++){

      //------------- Calculate track dependent variables from hits and save them in trk coll ----

	double ASmiOnTheFly            = dEdxOnTheFly(&(*HitsDeDx)[i], &(*HitsShapetest)[i], &(*HitsPathlength)[i], &(*HitsSubdetid)[i], &(*HitsTransverse)[i], edmEventHelper_isRealData, template_strip, template_pixel,1,0); 
	double ASmiNPOnTheFly          = dEdxOnTheFly(&(*HitsDeDx)[i], &(*HitsShapetest)[i], &(*HitsPathlength)[i], &(*HitsSubdetid)[i], &(*HitsTransverse)[i], edmEventHelper_isRealData, template_strip, template_pixel,0); 
	Track[i].ASmi           = ASmiOnTheFly;
       	Track[i].ASmiNP         = ASmiNPOnTheFly;
	
	    
	if(Track[i].numberOfValidHits<8)   continue;
	if(!Track[i].trackHighPurity)      continue;
	if(abs(Track[i].eta)>2.1)          continue;
	//if(Track[i].chi2/Track[i].ndof>5)  continue;
	//cout<<"Vertex[0] = "<<Vertex[0]<<endl;
	
	double _dvx = Track[i].vx - Vertex[0].x;
	double _dvy = Track[i].vy - Vertex[0].y;
	double d0 = abs( - _dvx*Track[i].py + _dvy*Track[i].px)/Track[i].pt;
	if(abs(d0)>0.02)                    continue;
	    
	double _dvz = Track[i].vz - Vertex[0].z;
	double dZ = _dvz - ( _dvx*Track[i].px + _dvy*Track[i].py)/Track[i].pt * (Track[i].pz/Track[i].pt);
	if(abs(dZ)>0.5)  continue;
	

	double p = sqrt( pow(Track[i].pt,2) + pow(Track[i].pz,2) );
	hIasPNP   -> Fill(p, Track[i].ASmiNP);
	hIasP     -> Fill(p, Track[i].ASmi);
	
	if(p<0.95 || p>1.) continue;
	hIasNP -> Fill(Track[i].ASmiNP);
	hIas   -> Fill(Track[i].ASmi);
	    
      }


	  
      // ---------------------
      // -- fill histograms --
      // ---------------------	  

    }

  stream.close();
  ofile.close();
  return 0;
}
