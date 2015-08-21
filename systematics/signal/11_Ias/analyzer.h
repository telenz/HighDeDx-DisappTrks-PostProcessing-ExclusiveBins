#ifndef ANALYZER_H
#define ANALYZER_H
//-----------------------------------------------------------------------------
// File:        analyzer.h
// Description: Analyzer header for ntuples created by TheNtupleMaker
// Created:     Tue Jul 21 14:55:47 2015 by mkanalyzer.py
// Author:      Teresa Lenz
//-----------------------------------------------------------------------------
// -- System

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cmath>

#include "analyzerutil.h"
#include "treestream.h"
#include "pdg.h"

// -- Root

#include "TROOT.h"
#include "TApplication.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TKey.h"
#include "TH1F.h"
#include "TH2F.h"

namespace evt {
//-----------------------------------------------------------------------------
// --- Declare variables
//-----------------------------------------------------------------------------
std::vector<int>	PileupSummaryInfo_getBunchCrossing(10,0);
std::vector<int>	PileupSummaryInfo_getPU_NumInteractions(10,0);
std::vector<float>	PileupSummaryInfo_getTrueNumInteractions(10,0);
std::vector<float>	Track_charge(2000,0);
std::vector<float>	Track_chi2(2000,0);
std::vector<float>	Track_dEdxASmi(2000,0);
std::vector<float>	Track_dEdxNPASmi(2000,0);
std::vector<unsigned int>	Track_dEdxNPNoM(2000,0);
std::vector<unsigned int>	Track_dEdxNoM(2000,0);
std::vector<float>	Track_eta(2000,0);
std::vector<unsigned short>	Track_hitPattern_trackerLayersWithoutMeasurement(2000,0);
std::vector<float>	Track_ndof(2000,0);
std::vector<unsigned short>	Track_numberOfValidHits(2000,0);
std::vector<float>	Track_phi(2000,0);
std::vector<float>	Track_pt(2000,0);
std::vector<float>	Track_ptError(2000,0);
std::vector<float>	Track_px(2000,0);
std::vector<float>	Track_py(2000,0);
std::vector<float>	Track_pz(2000,0);
std::vector<int>	Track_trackHighPurity(2000,0);
std::vector<unsigned short>	Track_trackerExpectedHitsInner_numberOfLostHits(2000,0);
std::vector<unsigned short>	Track_trackerExpectedHitsOuter_numberOfHits(2000,0);
std::vector<float>	Track_vx(2000,0);
std::vector<float>	Track_vy(2000,0);
std::vector<float>	Track_vz(2000,0);
int	edmEventHelper_bunchCrossing;
int	edmEventHelper_event;
int	edmEventHelper_isRealData;
int	edmEventHelper_luminosityBlock;
int	edmEventHelper_orbitNumber;
int	edmEventHelper_run;
int	nPileupSummaryInfo;
int	nTrack;

//-----------------------------------------------------------------------------
// --- indexmap keeps track of which objects have been flagged for selection
// --- IMPORTANT: initialize must be called every event to clear selection
std::map<std::string, std::vector<int> > indexmap;
void initialize()
{
  for(std::map<std::string, std::vector<int> >::iterator
    item=indexmap.begin(); 
    item != indexmap.end();
	++item)
	item->second.clear();
}

void select(std::string objname)
{
  indexmap[objname] = std::vector<int>();
}

void select(std::string objname, int index)
{
  try
    {
      indexmap[objname].push_back(index);
    }
  catch (...)
    {
      std::cout << "*** perhaps you failed to call select for " 
                << objname << std::endl;
      assert(0);
    }
}

//-----------------------------------------------------------------------------
// --- Structs can be filled by calling fillObjects()
// --- after the call to stream.read(...)
//-----------------------------------------------------------------------------
struct PileupSummaryInfo_s
{
  int	getBunchCrossing;
  int	getPU_NumInteractions;
  float	getTrueNumInteractions;
};
std::vector<PileupSummaryInfo_s> PileupSummaryInfo(10);

std::ostream& operator<<(std::ostream& os, const PileupSummaryInfo_s& o)
{
  char r[1024];
  os << "PileupSummaryInfo" << std::endl;
  sprintf(r, "  %-32s: %f\n", "getBunchCrossing", (double)o.getBunchCrossing); os << r;
  sprintf(r, "  %-32s: %f\n", "getPU_NumInteractions", (double)o.getPU_NumInteractions); os << r;
  sprintf(r, "  %-32s: %f\n", "getTrueNumInteractions", (double)o.getTrueNumInteractions); os << r;
  return os;
}
//-----------------------------------------------------------------------------
struct Track_s
{
  float	pt;
  float	ptError;
  float	px;
  float	py;
  float	pz;
  float	phi;
  float	eta;
  float	vx;
  float	vy;
  float	vz;
  float	chi2;
  float	ndof;
  float	charge;
  unsigned short	numberOfValidHits;
  unsigned short	hitPattern_trackerLayersWithoutMeasurement;
  unsigned short	trackerExpectedHitsInner_numberOfLostHits;
  unsigned short	trackerExpectedHitsOuter_numberOfHits;
  int	trackHighPurity;
  float	dEdxNPASmi;
  float	dEdxASmi;
  unsigned int	dEdxNPNoM;
  unsigned int	dEdxNoM;
  double ASmi;
  double ASmiNP;

};
std::vector<Track_s> Track(2000);

std::ostream& operator<<(std::ostream& os, const Track_s& o)
{
  char r[1024];
  os << "Track" << std::endl;
  sprintf(r, "  %-32s: %f\n", "pt", (double)o.pt); os << r;
  sprintf(r, "  %-32s: %f\n", "ptError", (double)o.ptError); os << r;
  sprintf(r, "  %-32s: %f\n", "px", (double)o.px); os << r;
  sprintf(r, "  %-32s: %f\n", "py", (double)o.py); os << r;
  sprintf(r, "  %-32s: %f\n", "pz", (double)o.pz); os << r;
  sprintf(r, "  %-32s: %f\n", "phi", (double)o.phi); os << r;
  sprintf(r, "  %-32s: %f\n", "eta", (double)o.eta); os << r;
  sprintf(r, "  %-32s: %f\n", "vx", (double)o.vx); os << r;
  sprintf(r, "  %-32s: %f\n", "vy", (double)o.vy); os << r;
  sprintf(r, "  %-32s: %f\n", "vz", (double)o.vz); os << r;
  sprintf(r, "  %-32s: %f\n", "chi2", (double)o.chi2); os << r;
  sprintf(r, "  %-32s: %f\n", "ndof", (double)o.ndof); os << r;
  sprintf(r, "  %-32s: %f\n", "charge", (double)o.charge); os << r;
  sprintf(r, "  %-32s: %f\n", "numberOfValidHits", (double)o.numberOfValidHits); os << r;
  sprintf(r, "  %-32s: %f\n", "hitPattern_trackerLayersWithoutMeasurement", (double)o.hitPattern_trackerLayersWithoutMeasurement); os << r;
  sprintf(r, "  %-32s: %f\n", "trackerExpectedHitsInner_numberOfLostHits", (double)o.trackerExpectedHitsInner_numberOfLostHits); os << r;
  sprintf(r, "  %-32s: %f\n", "trackerExpectedHitsOuter_numberOfHits", (double)o.trackerExpectedHitsOuter_numberOfHits); os << r;
  sprintf(r, "  %-32s: %f\n", "trackHighPurity", (double)o.trackHighPurity); os << r;
  sprintf(r, "  %-32s: %f\n", "dEdxNPASmi", (double)o.dEdxNPASmi); os << r;
  sprintf(r, "  %-32s: %f\n", "dEdxASmi", (double)o.dEdxASmi); os << r;
  sprintf(r, "  %-32s: %f\n", "dEdxNPNoM", (double)o.dEdxNPNoM); os << r;
  sprintf(r, "  %-32s: %f\n", "dEdxNoM", (double)o.dEdxNoM); os << r;
  return os;
}
//-----------------------------------------------------------------------------

inline void fillPileupSummaryInfo()
{
  PileupSummaryInfo.resize(PileupSummaryInfo_getBunchCrossing.size());
  for(unsigned int i=0; i < PileupSummaryInfo.size(); ++i)
    {
      PileupSummaryInfo[i].getBunchCrossing	= PileupSummaryInfo_getBunchCrossing[i];
      PileupSummaryInfo[i].getPU_NumInteractions	= PileupSummaryInfo_getPU_NumInteractions[i];
      PileupSummaryInfo[i].getTrueNumInteractions	= PileupSummaryInfo_getTrueNumInteractions[i];
    }
}

inline void fillTrack()
{
  Track.resize(Track_pt.size());
  for(unsigned int i=0; i < Track.size(); ++i)
    {
      Track[i].pt	= Track_pt[i];
      Track[i].ptError	= Track_ptError[i];
      Track[i].px	= Track_px[i];
      Track[i].py	= Track_py[i];
      Track[i].pz	= Track_pz[i];
      Track[i].phi	= Track_phi[i];
      Track[i].eta	= Track_eta[i];
      Track[i].vx	= Track_vx[i];
      Track[i].vy	= Track_vy[i];
      Track[i].vz	= Track_vz[i];
      Track[i].chi2	= Track_chi2[i];
      Track[i].ndof	= Track_ndof[i];
      Track[i].charge	= Track_charge[i];
      Track[i].numberOfValidHits	= Track_numberOfValidHits[i];
      Track[i].hitPattern_trackerLayersWithoutMeasurement	= Track_hitPattern_trackerLayersWithoutMeasurement[i];
      Track[i].trackerExpectedHitsInner_numberOfLostHits	= Track_trackerExpectedHitsInner_numberOfLostHits[i];
      Track[i].trackerExpectedHitsOuter_numberOfHits	= Track_trackerExpectedHitsOuter_numberOfHits[i];
      Track[i].trackHighPurity	= Track_trackHighPurity[i];
      Track[i].dEdxNPASmi	= Track_dEdxNPASmi[i];
      Track[i].dEdxASmi	= Track_dEdxASmi[i];
      Track[i].dEdxNPNoM	= Track_dEdxNPNoM[i];
      Track[i].dEdxNoM	= Track_dEdxNoM[i];
    }
}


void fillObjects()
{
  fillPileupSummaryInfo();
  fillTrack();
}

//-----------------------------------------------------------------------------
// --- Call saveSelectedObjects() just before call to addEvent if
// --- you wish to save only the selected objects
//-----------------------------------------------------------------------------
// Select objects for which the select function was called
void saveSelectedObjects()
{
  int n = 0;

  n = 0;
  try
    {
       n = indexmap["PileupSummaryInfo"].size();
    }
  catch (...)
    {}
  if ( n > 0 )
    {
      std::vector<int>& index = indexmap["PileupSummaryInfo"];
      for(int i=0; i < n; ++i)
        {
          int j = index[i];
          PileupSummaryInfo_getBunchCrossing[i]	= PileupSummaryInfo_getBunchCrossing[j];
          PileupSummaryInfo_getPU_NumInteractions[i]	= PileupSummaryInfo_getPU_NumInteractions[j];
          PileupSummaryInfo_getTrueNumInteractions[i]	= PileupSummaryInfo_getTrueNumInteractions[j];
        }
      nPileupSummaryInfo = n;
    }

  n = 0;
  try
    {
       n = indexmap["Track"].size();
    }
  catch (...)
    {}
  if ( n > 0 )
    {
      std::vector<int>& index = indexmap["Track"];
      for(int i=0; i < n; ++i)
        {
          int j = index[i];
          Track_pt[i]	= Track_pt[j];
          Track_ptError[i]	= Track_ptError[j];
          Track_px[i]	= Track_px[j];
          Track_py[i]	= Track_py[j];
          Track_pz[i]	= Track_pz[j];
          Track_phi[i]	= Track_phi[j];
          Track_eta[i]	= Track_eta[j];
          Track_vx[i]	= Track_vx[j];
          Track_vy[i]	= Track_vy[j];
          Track_vz[i]	= Track_vz[j];
          Track_chi2[i]	= Track_chi2[j];
          Track_ndof[i]	= Track_ndof[j];
          Track_charge[i]	= Track_charge[j];
          Track_numberOfValidHits[i]	= Track_numberOfValidHits[j];
          Track_hitPattern_trackerLayersWithoutMeasurement[i]	= Track_hitPattern_trackerLayersWithoutMeasurement[j];
          Track_trackerExpectedHitsInner_numberOfLostHits[i]	= Track_trackerExpectedHitsInner_numberOfLostHits[j];
          Track_trackerExpectedHitsOuter_numberOfHits[i]	= Track_trackerExpectedHitsOuter_numberOfHits[j];
          Track_trackHighPurity[i]	= Track_trackHighPurity[j];
          Track_dEdxNPASmi[i]	= Track_dEdxNPASmi[j];
          Track_dEdxASmi[i]	= Track_dEdxASmi[j];
          Track_dEdxNPNoM[i]	= Track_dEdxNPNoM[j];
          Track_dEdxNoM[i]	= Track_dEdxNoM[j];
        }
      nTrack = n;
    }
}

//-----------------------------------------------------------------------------
// --- Select variables to be read
//-----------------------------------------------------------------------------
void selectVariables(itreestream& stream)
{
  stream.select("PileupSummaryInfo_addPileupInfo.getBunchCrossing", PileupSummaryInfo_getBunchCrossing);
  stream.select("PileupSummaryInfo_addPileupInfo.getPU_NumInteractions", PileupSummaryInfo_getPU_NumInteractions);
  stream.select("PileupSummaryInfo_addPileupInfo.getTrueNumInteractions", PileupSummaryInfo_getTrueNumInteractions);
  stream.select("recoTrackHelper_TrackRefitter.charge", Track_charge);
  stream.select("recoTrackHelper_TrackRefitter.chi2", Track_chi2);
  stream.select("recoTrackHelper_TrackRefitter.dEdxASmi", Track_dEdxASmi);
  stream.select("recoTrackHelper_TrackRefitter.dEdxNPASmi", Track_dEdxNPASmi);
  stream.select("recoTrackHelper_TrackRefitter.dEdxNPNoM", Track_dEdxNPNoM);
  stream.select("recoTrackHelper_TrackRefitter.dEdxNoM", Track_dEdxNoM);
  stream.select("recoTrackHelper_TrackRefitter.eta", Track_eta);
  stream.select("recoTrackHelper_TrackRefitter.hitPattern_trackerLayersWithoutMeasurement", Track_hitPattern_trackerLayersWithoutMeasurement);
  stream.select("recoTrackHelper_TrackRefitter.ndof", Track_ndof);
  stream.select("recoTrackHelper_TrackRefitter.numberOfValidHits", Track_numberOfValidHits);
  stream.select("recoTrackHelper_TrackRefitter.phi", Track_phi);
  stream.select("recoTrackHelper_TrackRefitter.pt", Track_pt);
  stream.select("recoTrackHelper_TrackRefitter.ptError", Track_ptError);
  stream.select("recoTrackHelper_TrackRefitter.px", Track_px);
  stream.select("recoTrackHelper_TrackRefitter.py", Track_py);
  stream.select("recoTrackHelper_TrackRefitter.pz", Track_pz);
  stream.select("recoTrackHelper_TrackRefitter.trackHighPurity", Track_trackHighPurity);
  stream.select("recoTrackHelper_TrackRefitter.trackerExpectedHitsInner_numberOfLostHits", Track_trackerExpectedHitsInner_numberOfLostHits);
  stream.select("recoTrackHelper_TrackRefitter.trackerExpectedHitsOuter_numberOfHits", Track_trackerExpectedHitsOuter_numberOfHits);
  stream.select("recoTrackHelper_TrackRefitter.vx", Track_vx);
  stream.select("recoTrackHelper_TrackRefitter.vy", Track_vy);
  stream.select("recoTrackHelper_TrackRefitter.vz", Track_vz);
  stream.select("edmEventHelper_info.bunchCrossing", edmEventHelper_bunchCrossing);
  stream.select("edmEventHelper_info.event", edmEventHelper_event);
  stream.select("edmEventHelper_info.isRealData", edmEventHelper_isRealData);
  stream.select("edmEventHelper_info.luminosityBlock", edmEventHelper_luminosityBlock);
  stream.select("edmEventHelper_info.orbitNumber", edmEventHelper_orbitNumber);
  stream.select("edmEventHelper_info.run", edmEventHelper_run);
  stream.select("nPileupSummaryInfo_addPileupInfo", nPileupSummaryInfo);
  stream.select("nrecoTrackHelper_TrackRefitter", nTrack);

}
}; // end namespace evt
#endif
