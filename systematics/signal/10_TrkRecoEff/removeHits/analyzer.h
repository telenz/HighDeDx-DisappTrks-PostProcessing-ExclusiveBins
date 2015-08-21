#ifndef ANALYZER_H
#define ANALYZER_H
//-----------------------------------------------------------------------------
// File:        analyzer.h
// Description: Analyzer header for ntuples created by TheNtupleMaker
// Created:     Mon Jul 27 15:16:23 2015 by mkanalyzer.py
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
float	MET_energy;
float	MET_et;
float	MET_eta;
float	MET_phi;
float	MET_pt;
float	MET_pz;
std::vector<float>	MuonTracksAllHits_charge(200,0);
std::vector<float>	MuonTracksAllHits_chi2(200,0);
std::vector<float>	MuonTracksAllHits_eta(200,0);
std::vector<unsigned short>	MuonTracksAllHits_hitPattern_trackerLayersWithoutMeasurement(200,0);
std::vector<float>	MuonTracksAllHits_ndof(200,0);
std::vector<unsigned short>	MuonTracksAllHits_numberOfValidHits(200,0);
std::vector<float>	MuonTracksAllHits_phi(200,0);
std::vector<float>	MuonTracksAllHits_pt(200,0);
std::vector<float>	MuonTracksAllHits_ptError(200,0);
std::vector<float>	MuonTracksAllHits_px(200,0);
std::vector<float>	MuonTracksAllHits_py(200,0);
std::vector<float>	MuonTracksAllHits_pz(200,0);
std::vector<int>	MuonTracksAllHits_trackHighPurity(200,0);
std::vector<float>	MuonTracksAllHits_trackRelIso03(200,0);
std::vector<unsigned short>	MuonTracksAllHits_trackerExpectedHitsInner_numberOfLostHits(200,0);
std::vector<unsigned short>	MuonTracksAllHits_trackerExpectedHitsOuter_numberOfHits(200,0);
std::vector<float>	MuonTracksAllHits_vx(200,0);
std::vector<float>	MuonTracksAllHits_vy(200,0);
std::vector<float>	MuonTracksAllHits_vz(200,0);
std::vector<float>	TrackAllHits_charge(2000,0);
std::vector<float>	TrackAllHits_chi2(2000,0);
std::vector<float>	TrackAllHits_eta(2000,0);
std::vector<unsigned short>	TrackAllHits_hitPattern_trackerLayersWithoutMeasurement(2000,0);
std::vector<float>	TrackAllHits_ndof(2000,0);
std::vector<unsigned short>	TrackAllHits_numberOfValidHits(2000,0);
std::vector<float>	TrackAllHits_phi(2000,0);
std::vector<float>	TrackAllHits_pt(2000,0);
std::vector<float>	TrackAllHits_ptError(2000,0);
std::vector<float>	TrackAllHits_px(2000,0);
std::vector<float>	TrackAllHits_py(2000,0);
std::vector<float>	TrackAllHits_pz(2000,0);
std::vector<int>	TrackAllHits_trackHighPurity(2000,0);
std::vector<float>	TrackAllHits_trackRelIso03(2000,0);
std::vector<unsigned short>	TrackAllHits_trackerExpectedHitsInner_numberOfLostHits(2000,0);
std::vector<unsigned short>	TrackAllHits_trackerExpectedHitsOuter_numberOfHits(2000,0);
std::vector<float>	TrackAllHits_vx(2000,0);
std::vector<float>	TrackAllHits_vy(2000,0);
std::vector<float>	TrackAllHits_vz(2000,0);
std::vector<float>	Track_charge(2000,0);
std::vector<float>	Track_chi2(2000,0);
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
std::vector<float>	Track_trackRelIso03(2000,0);
std::vector<unsigned short>	Track_trackerExpectedHitsInner_numberOfLostHits(2000,0);
std::vector<unsigned short>	Track_trackerExpectedHitsOuter_numberOfHits(2000,0);
std::vector<float>	Track_vx(2000,0);
std::vector<float>	Track_vy(2000,0);
std::vector<float>	Track_vz(2000,0);
std::vector<float>	Vertex_ndof(100,0);
std::vector<float>	Vertex_position_rho(100,0);
std::vector<float>	Vertex_x(100,0);
std::vector<float>	Vertex_y(100,0);
std::vector<float>	Vertex_z(100,0);
int	edmEventHelper_bunchCrossing;
int	edmEventHelper_event;
int	edmEventHelper_isRealData;
int	edmEventHelper_luminosityBlock;
int	edmEventHelper_orbitNumber;
int	edmEventHelper_run;
int	nMuonTracksAllHits;
int	nTrack;
int	nTrackAllHits;
int	nVertex;

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
struct MuonTracksAllHits_s
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
  float	trackRelIso03;
};
std::vector<MuonTracksAllHits_s> MuonTracksAllHits(200);

std::ostream& operator<<(std::ostream& os, const MuonTracksAllHits_s& o)
{
  char r[1024];
  os << "MuonTracksAllHits" << std::endl;
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
  sprintf(r, "  %-32s: %f\n", "trackRelIso03", (double)o.trackRelIso03); os << r;
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
  float	trackRelIso03;
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
  sprintf(r, "  %-32s: %f\n", "trackRelIso03", (double)o.trackRelIso03); os << r;
  return os;
}
//-----------------------------------------------------------------------------
struct TrackAllHits_s
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
  float	trackRelIso03;
};
std::vector<TrackAllHits_s> TrackAllHits(2000);

std::ostream& operator<<(std::ostream& os, const TrackAllHits_s& o)
{
  char r[1024];
  os << "TrackAllHits" << std::endl;
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
  sprintf(r, "  %-32s: %f\n", "trackRelIso03", (double)o.trackRelIso03); os << r;
  return os;
}
//-----------------------------------------------------------------------------
struct Vertex_s
{
  float	ndof;
  float	x;
  float	y;
  float	z;
  float	position_rho;
};
std::vector<Vertex_s> Vertex(100);

std::ostream& operator<<(std::ostream& os, const Vertex_s& o)
{
  char r[1024];
  os << "Vertex" << std::endl;
  sprintf(r, "  %-32s: %f\n", "ndof", (double)o.ndof); os << r;
  sprintf(r, "  %-32s: %f\n", "x", (double)o.x); os << r;
  sprintf(r, "  %-32s: %f\n", "y", (double)o.y); os << r;
  sprintf(r, "  %-32s: %f\n", "z", (double)o.z); os << r;
  sprintf(r, "  %-32s: %f\n", "position_rho", (double)o.position_rho); os << r;
  return os;
}
//-----------------------------------------------------------------------------

inline void fillMuonTracksAllHits()
{
  MuonTracksAllHits.resize(MuonTracksAllHits_pt.size());
  for(unsigned int i=0; i < MuonTracksAllHits.size(); ++i)
    {
      MuonTracksAllHits[i].pt	= MuonTracksAllHits_pt[i];
      MuonTracksAllHits[i].ptError	= MuonTracksAllHits_ptError[i];
      MuonTracksAllHits[i].px	= MuonTracksAllHits_px[i];
      MuonTracksAllHits[i].py	= MuonTracksAllHits_py[i];
      MuonTracksAllHits[i].pz	= MuonTracksAllHits_pz[i];
      MuonTracksAllHits[i].phi	= MuonTracksAllHits_phi[i];
      MuonTracksAllHits[i].eta	= MuonTracksAllHits_eta[i];
      MuonTracksAllHits[i].vx	= MuonTracksAllHits_vx[i];
      MuonTracksAllHits[i].vy	= MuonTracksAllHits_vy[i];
      MuonTracksAllHits[i].vz	= MuonTracksAllHits_vz[i];
      MuonTracksAllHits[i].chi2	= MuonTracksAllHits_chi2[i];
      MuonTracksAllHits[i].ndof	= MuonTracksAllHits_ndof[i];
      MuonTracksAllHits[i].charge	= MuonTracksAllHits_charge[i];
      MuonTracksAllHits[i].numberOfValidHits	= MuonTracksAllHits_numberOfValidHits[i];
      MuonTracksAllHits[i].hitPattern_trackerLayersWithoutMeasurement	= MuonTracksAllHits_hitPattern_trackerLayersWithoutMeasurement[i];
      MuonTracksAllHits[i].trackerExpectedHitsInner_numberOfLostHits	= MuonTracksAllHits_trackerExpectedHitsInner_numberOfLostHits[i];
      MuonTracksAllHits[i].trackerExpectedHitsOuter_numberOfHits	= MuonTracksAllHits_trackerExpectedHitsOuter_numberOfHits[i];
      MuonTracksAllHits[i].trackHighPurity	= MuonTracksAllHits_trackHighPurity[i];
      MuonTracksAllHits[i].trackRelIso03	= MuonTracksAllHits_trackRelIso03[i];
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
      Track[i].trackRelIso03	= Track_trackRelIso03[i];
    }
}

inline void fillTrackAllHits()
{
  TrackAllHits.resize(TrackAllHits_pt.size());
  for(unsigned int i=0; i < TrackAllHits.size(); ++i)
    {
      TrackAllHits[i].pt	= TrackAllHits_pt[i];
      TrackAllHits[i].ptError	= TrackAllHits_ptError[i];
      TrackAllHits[i].px	= TrackAllHits_px[i];
      TrackAllHits[i].py	= TrackAllHits_py[i];
      TrackAllHits[i].pz	= TrackAllHits_pz[i];
      TrackAllHits[i].phi	= TrackAllHits_phi[i];
      TrackAllHits[i].eta	= TrackAllHits_eta[i];
      TrackAllHits[i].vx	= TrackAllHits_vx[i];
      TrackAllHits[i].vy	= TrackAllHits_vy[i];
      TrackAllHits[i].vz	= TrackAllHits_vz[i];
      TrackAllHits[i].chi2	= TrackAllHits_chi2[i];
      TrackAllHits[i].ndof	= TrackAllHits_ndof[i];
      TrackAllHits[i].charge	= TrackAllHits_charge[i];
      TrackAllHits[i].numberOfValidHits	= TrackAllHits_numberOfValidHits[i];
      TrackAllHits[i].hitPattern_trackerLayersWithoutMeasurement	= TrackAllHits_hitPattern_trackerLayersWithoutMeasurement[i];
      TrackAllHits[i].trackerExpectedHitsInner_numberOfLostHits	= TrackAllHits_trackerExpectedHitsInner_numberOfLostHits[i];
      TrackAllHits[i].trackerExpectedHitsOuter_numberOfHits	= TrackAllHits_trackerExpectedHitsOuter_numberOfHits[i];
      TrackAllHits[i].trackHighPurity	= TrackAllHits_trackHighPurity[i];
      TrackAllHits[i].trackRelIso03	= TrackAllHits_trackRelIso03[i];
    }
}

inline void fillVertex()
{
  Vertex.resize(Vertex_ndof.size());
  for(unsigned int i=0; i < Vertex.size(); ++i)
    {
      Vertex[i].ndof	= Vertex_ndof[i];
      Vertex[i].x	= Vertex_x[i];
      Vertex[i].y	= Vertex_y[i];
      Vertex[i].z	= Vertex_z[i];
      Vertex[i].position_rho	= Vertex_position_rho[i];
    }
}


void fillObjects()
{
  fillMuonTracksAllHits();
  fillTrack();
  fillTrackAllHits();
  fillVertex();
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
       n = indexmap["MuonTracksAllHits"].size();
    }
  catch (...)
    {}
  if ( n > 0 )
    {
      std::vector<int>& index = indexmap["MuonTracksAllHits"];
      for(int i=0; i < n; ++i)
        {
          int j = index[i];
          MuonTracksAllHits_pt[i]	= MuonTracksAllHits_pt[j];
          MuonTracksAllHits_ptError[i]	= MuonTracksAllHits_ptError[j];
          MuonTracksAllHits_px[i]	= MuonTracksAllHits_px[j];
          MuonTracksAllHits_py[i]	= MuonTracksAllHits_py[j];
          MuonTracksAllHits_pz[i]	= MuonTracksAllHits_pz[j];
          MuonTracksAllHits_phi[i]	= MuonTracksAllHits_phi[j];
          MuonTracksAllHits_eta[i]	= MuonTracksAllHits_eta[j];
          MuonTracksAllHits_vx[i]	= MuonTracksAllHits_vx[j];
          MuonTracksAllHits_vy[i]	= MuonTracksAllHits_vy[j];
          MuonTracksAllHits_vz[i]	= MuonTracksAllHits_vz[j];
          MuonTracksAllHits_chi2[i]	= MuonTracksAllHits_chi2[j];
          MuonTracksAllHits_ndof[i]	= MuonTracksAllHits_ndof[j];
          MuonTracksAllHits_charge[i]	= MuonTracksAllHits_charge[j];
          MuonTracksAllHits_numberOfValidHits[i]	= MuonTracksAllHits_numberOfValidHits[j];
          MuonTracksAllHits_hitPattern_trackerLayersWithoutMeasurement[i]	= MuonTracksAllHits_hitPattern_trackerLayersWithoutMeasurement[j];
          MuonTracksAllHits_trackerExpectedHitsInner_numberOfLostHits[i]	= MuonTracksAllHits_trackerExpectedHitsInner_numberOfLostHits[j];
          MuonTracksAllHits_trackerExpectedHitsOuter_numberOfHits[i]	= MuonTracksAllHits_trackerExpectedHitsOuter_numberOfHits[j];
          MuonTracksAllHits_trackHighPurity[i]	= MuonTracksAllHits_trackHighPurity[j];
          MuonTracksAllHits_trackRelIso03[i]	= MuonTracksAllHits_trackRelIso03[j];
        }
      nMuonTracksAllHits = n;
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
          Track_trackRelIso03[i]	= Track_trackRelIso03[j];
        }
      nTrack = n;
    }

  n = 0;
  try
    {
       n = indexmap["TrackAllHits"].size();
    }
  catch (...)
    {}
  if ( n > 0 )
    {
      std::vector<int>& index = indexmap["TrackAllHits"];
      for(int i=0; i < n; ++i)
        {
          int j = index[i];
          TrackAllHits_pt[i]	= TrackAllHits_pt[j];
          TrackAllHits_ptError[i]	= TrackAllHits_ptError[j];
          TrackAllHits_px[i]	= TrackAllHits_px[j];
          TrackAllHits_py[i]	= TrackAllHits_py[j];
          TrackAllHits_pz[i]	= TrackAllHits_pz[j];
          TrackAllHits_phi[i]	= TrackAllHits_phi[j];
          TrackAllHits_eta[i]	= TrackAllHits_eta[j];
          TrackAllHits_vx[i]	= TrackAllHits_vx[j];
          TrackAllHits_vy[i]	= TrackAllHits_vy[j];
          TrackAllHits_vz[i]	= TrackAllHits_vz[j];
          TrackAllHits_chi2[i]	= TrackAllHits_chi2[j];
          TrackAllHits_ndof[i]	= TrackAllHits_ndof[j];
          TrackAllHits_charge[i]	= TrackAllHits_charge[j];
          TrackAllHits_numberOfValidHits[i]	= TrackAllHits_numberOfValidHits[j];
          TrackAllHits_hitPattern_trackerLayersWithoutMeasurement[i]	= TrackAllHits_hitPattern_trackerLayersWithoutMeasurement[j];
          TrackAllHits_trackerExpectedHitsInner_numberOfLostHits[i]	= TrackAllHits_trackerExpectedHitsInner_numberOfLostHits[j];
          TrackAllHits_trackerExpectedHitsOuter_numberOfHits[i]	= TrackAllHits_trackerExpectedHitsOuter_numberOfHits[j];
          TrackAllHits_trackHighPurity[i]	= TrackAllHits_trackHighPurity[j];
          TrackAllHits_trackRelIso03[i]	= TrackAllHits_trackRelIso03[j];
        }
      nTrackAllHits = n;
    }

  n = 0;
  try
    {
       n = indexmap["Vertex"].size();
    }
  catch (...)
    {}
  if ( n > 0 )
    {
      std::vector<int>& index = indexmap["Vertex"];
      for(int i=0; i < n; ++i)
        {
          int j = index[i];
          Vertex_ndof[i]	= Vertex_ndof[j];
          Vertex_x[i]	= Vertex_x[j];
          Vertex_y[i]	= Vertex_y[j];
          Vertex_z[i]	= Vertex_z[j];
          Vertex_position_rho[i]	= Vertex_position_rho[j];
        }
      nVertex = n;
    }
}

//-----------------------------------------------------------------------------
// --- Select variables to be read
//-----------------------------------------------------------------------------
void selectVariables(itreestream& stream)
{
  stream.select("recoPFMET_pfMet.energy", MET_energy);
  stream.select("recoPFMET_pfMet.et", MET_et);
  stream.select("recoPFMET_pfMet.eta", MET_eta);
  stream.select("recoPFMET_pfMet.phi", MET_phi);
  stream.select("recoPFMET_pfMet.pt", MET_pt);
  stream.select("recoPFMET_pfMet.pz", MET_pz);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.charge", MuonTracksAllHits_charge);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.chi2", MuonTracksAllHits_chi2);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.eta", MuonTracksAllHits_eta);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.hitPattern_trackerLayersWithoutMeasurement", MuonTracksAllHits_hitPattern_trackerLayersWithoutMeasurement);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.ndof", MuonTracksAllHits_ndof);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.numberOfValidHits", MuonTracksAllHits_numberOfValidHits);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.phi", MuonTracksAllHits_phi);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.pt", MuonTracksAllHits_pt);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.ptError", MuonTracksAllHits_ptError);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.px", MuonTracksAllHits_px);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.py", MuonTracksAllHits_py);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.pz", MuonTracksAllHits_pz);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.trackHighPurity", MuonTracksAllHits_trackHighPurity);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.trackRelIso03", MuonTracksAllHits_trackRelIso03);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.trackerExpectedHitsInner_numberOfLostHits", MuonTracksAllHits_trackerExpectedHitsInner_numberOfLostHits);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.trackerExpectedHitsOuter_numberOfHits", MuonTracksAllHits_trackerExpectedHitsOuter_numberOfHits);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.vx", MuonTracksAllHits_vx);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.vy", MuonTracksAllHits_vy);
  stream.select("recoTrackHelper_globalMuonsAllHitsReduced.vz", MuonTracksAllHits_vz);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.charge", TrackAllHits_charge);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.chi2", TrackAllHits_chi2);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.eta", TrackAllHits_eta);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.hitPattern_trackerLayersWithoutMeasurement", TrackAllHits_hitPattern_trackerLayersWithoutMeasurement);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.ndof", TrackAllHits_ndof);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.numberOfValidHits", TrackAllHits_numberOfValidHits);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.phi", TrackAllHits_phi);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.pt", TrackAllHits_pt);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.ptError", TrackAllHits_ptError);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.px", TrackAllHits_px);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.py", TrackAllHits_py);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.pz", TrackAllHits_pz);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.trackHighPurity", TrackAllHits_trackHighPurity);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.trackRelIso03", TrackAllHits_trackRelIso03);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.trackerExpectedHitsInner_numberOfLostHits", TrackAllHits_trackerExpectedHitsInner_numberOfLostHits);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.trackerExpectedHitsOuter_numberOfHits", TrackAllHits_trackerExpectedHitsOuter_numberOfHits);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.vx", TrackAllHits_vx);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.vy", TrackAllHits_vy);
  stream.select("recoTrackHelper_generalTracksAllHitsReduced.vz", TrackAllHits_vz);
  stream.select("recoTrackHelper_generalTracksReduced.charge", Track_charge);
  stream.select("recoTrackHelper_generalTracksReduced.chi2", Track_chi2);
  stream.select("recoTrackHelper_generalTracksReduced.eta", Track_eta);
  stream.select("recoTrackHelper_generalTracksReduced.hitPattern_trackerLayersWithoutMeasurement", Track_hitPattern_trackerLayersWithoutMeasurement);
  stream.select("recoTrackHelper_generalTracksReduced.ndof", Track_ndof);
  stream.select("recoTrackHelper_generalTracksReduced.numberOfValidHits", Track_numberOfValidHits);
  stream.select("recoTrackHelper_generalTracksReduced.phi", Track_phi);
  stream.select("recoTrackHelper_generalTracksReduced.pt", Track_pt);
  stream.select("recoTrackHelper_generalTracksReduced.ptError", Track_ptError);
  stream.select("recoTrackHelper_generalTracksReduced.px", Track_px);
  stream.select("recoTrackHelper_generalTracksReduced.py", Track_py);
  stream.select("recoTrackHelper_generalTracksReduced.pz", Track_pz);
  stream.select("recoTrackHelper_generalTracksReduced.trackHighPurity", Track_trackHighPurity);
  stream.select("recoTrackHelper_generalTracksReduced.trackRelIso03", Track_trackRelIso03);
  stream.select("recoTrackHelper_generalTracksReduced.trackerExpectedHitsInner_numberOfLostHits", Track_trackerExpectedHitsInner_numberOfLostHits);
  stream.select("recoTrackHelper_generalTracksReduced.trackerExpectedHitsOuter_numberOfHits", Track_trackerExpectedHitsOuter_numberOfHits);
  stream.select("recoTrackHelper_generalTracksReduced.vx", Track_vx);
  stream.select("recoTrackHelper_generalTracksReduced.vy", Track_vy);
  stream.select("recoTrackHelper_generalTracksReduced.vz", Track_vz);
  stream.select("recoVertex_offlinePrimaryVertices.ndof", Vertex_ndof);
  stream.select("recoVertex_offlinePrimaryVertices.position_rho", Vertex_position_rho);
  stream.select("recoVertex_offlinePrimaryVertices.x", Vertex_x);
  stream.select("recoVertex_offlinePrimaryVertices.y", Vertex_y);
  stream.select("recoVertex_offlinePrimaryVertices.z", Vertex_z);
  stream.select("edmEventHelper_info.bunchCrossing", edmEventHelper_bunchCrossing);
  stream.select("edmEventHelper_info.event", edmEventHelper_event);
  stream.select("edmEventHelper_info.isRealData", edmEventHelper_isRealData);
  stream.select("edmEventHelper_info.luminosityBlock", edmEventHelper_luminosityBlock);
  stream.select("edmEventHelper_info.orbitNumber", edmEventHelper_orbitNumber);
  stream.select("edmEventHelper_info.run", edmEventHelper_run);
  stream.select("nrecoTrackHelper_globalMuonsAllHitsReduced", nMuonTracksAllHits);
  stream.select("nrecoTrackHelper_generalTracksReduced", nTrack);
  stream.select("nrecoTrackHelper_generalTracksAllHitsReduced", nTrackAllHits);
  stream.select("nrecoVertex_offlinePrimaryVertices", nVertex);

}
}; // end namespace evt
#endif
