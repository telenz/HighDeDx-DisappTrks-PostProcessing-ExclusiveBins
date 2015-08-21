#ifndef ANALYZERUTIL_H
#define ANALYZERUTIL_H
//-----------------------------------------------------------------------------
// -- Utilities
// Created: Tue Jul 21 14:55:47 2015 by mkanalyzer.py
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TMath.h"
#include "TString.h"
#include "treestream.h"
//-----------------------------------------------------------------------------

///
void
error(std::string message)
{
  std::cout << "** error ** " << message << std::endl;
  exit(0);
}

// string utils ==============================================================

///
std::string 
strip(std::string line)
{
  int l = line.size();
  if ( l == 0 ) return std::string("");
  int n = 0;
  while (((line[n] == 0)    ||
	  (line[n] == ' ' ) ||
	  (line[n] == '\n') ||
	  (line[n] == '\t')) && n < l) n++;

  int m = l-1;
  while (((line[m] == 0)    ||
	  (line[m] == ' ')  ||
	  (line[m] == '\n') ||
	  (line[m] == '\t')) && m > 0) m--;
  return line.substr(n,m-n+1);
}

///
void 
split(std::string str, std::vector<std::string>& vstr)
{
  vstr.clear();
  std::istringstream stream(str);
  while ( stream )
    {
      std::string str;
      stream >> str;
      if ( stream ) vstr.push_back(str);
    }
}

///
std::string
replace(std::string& str, std::string oldstr, std::string newstr)
{
  return std::string(TString(str).ReplaceAll(oldstr, newstr).Data());
}

///
std::string
nameonly(std::string filename)
{
  int i = filename.rfind("/");
  int j = filename.rfind(".");
  if ( j < 0 ) j = filename.size();
  return filename.substr(i+1,j-i-1);
}

///
std::string 
shell(std::string cmd)
{
  FILE* f = popen(cmd.c_str(),"r");
  int buffsize=8192;
  char s[8192];
  int n = fread(s,1,buffsize,f);
  pclose(f);
  std::string result = strip(std::string(s).substr(0,n));
  return result;
}

///
struct outputFile
{
///
  outputFile(std::string filename)
   : filename_(filename),
	 file_(new TFile(filename_.c_str(), "recreate")),
	 tree_(0),
	 b_weight_(0),
	 entry_(0),
	 SAVECOUNT_(50000)
  {
	file_->cd();
	hist_ = new TH1F("counts", "", 1,0,1);
	hist_->SetBit(TH1::kCanRebin);
	hist_->SetStats(0);
  }

///
  outputFile(std::string filename, itreestream& stream, int savecount=50000) 
   : filename_(filename),
	 file_(new TFile(filename.c_str(), "recreate")),
	 tree_(stream.tree()->CloneTree(0)),
	 b_weight_(tree_->Branch("eventWeight", &weight_, "eventWeight/D")),
	 entry_(0),
	 SAVECOUNT_(savecount)
  {
	std::cout << "events will be skimmed to file "
			  << filename_ << std::endl;
	file_->cd();
	hist_ = new TH1F("counts", "", 1,0,1);
	hist_->SetBit(TH1::kCanRebin);
	hist_->SetStats(0);
  }

///
  void addEvent(double weight=1)
  {
	if ( tree_ == 0 ) return;

	weight_ = weight;
	file_   = tree_->GetCurrentFile();
	file_->cd();
	tree_->Fill();

	entry_++;
	if ( entry_ % SAVECOUNT_ == 0 )
	  tree_->AutoSave("SaveSelf");
  }

///
  void count(std::string cond, double w=1)
  {
	hist_->Fill(cond.c_str(), w);
  }

///
  void close()
  {
	std::cout << "==> histograms saved to file " << filename_ << std::endl;
	if ( tree_ != 0 )
	  {
		std::cout << "==> events skimmed to file " << filename_ << std::endl;
		file_ = tree_->GetCurrentFile();
	  }
	file_->cd();
	//file_->Write("", TObject::kWriteDelete);
	file_->Write();
	file_->ls();
	file_->Close();
  }

  std::string filename_;  
  TFile* file_;
  TTree* tree_;
  TH1F*  hist_;
  TBranch* b_weight_;
  double     weight_;
  int    entry_;
  int    SAVECOUNT_;
};

///
struct commandLine
{
  std::string progname;
  std::string filelist;
  std::string outputfilename;
};

///
void
decodeCommandLine(int argc, char** argv, commandLine& cl)
{
  cl.progname = std::string(argv[0]);

  // 1st (optional) argument
  if ( argc > 1 )
	cl.filelist = std::string(argv[1]);
  else
	cl.filelist = std::string("filelist.txt");

  // 2nd (optional) command line argument
  if ( argc > 2 ) 
	cl.outputfilename = std::string(argv[2]);
  else
	cl.outputfilename = cl.progname + std::string("_histograms");

  // Make sure extension is ".root"
  std::string name = cl.outputfilename;
  if ( name.substr(name.size()-5, 5) != std::string(".root") )
	cl.outputfilename += std::string(".root");
}

/// Read ntuple filenames from file list
std::vector<std::string>
getFilenames(std::string filelist)
{
  std::ifstream stream(filelist.c_str());
  if ( !stream.good() ) error("unable to open file: " + filelist);

  // Get list of ntuple files to be processed

  std::vector<std::string> v;
  std::string filename;
  while ( stream >> filename )
	if ( strip(filename) != "" ) v.push_back(filename);
  return v;
}

// physics utils ==============================================================

///
double
deltaPhi(double phi1, double phi2)
{
  double deltaphi = phi2 - phi1;
  if ( fabs(deltaphi) > M_PI ) deltaphi = 2 * M_PI - fabs(deltaphi);
  return deltaphi;
}

///
double
deltaR(double eta1, double phi1, double eta2, double phi2)
{
  double deltaeta = eta1 - eta2;
  double deltaphi = deltaPhi(phi1, phi2);
  return sqrt(deltaeta*deltaeta + deltaphi*deltaphi);
}

///
struct MatchedPair
{
  int first;
  int second;
  double distance;
  bool operator<(const MatchedPair& o) const 
  { return this->distance < o.distance; }
};

/// Collect together standard attributes and permit pT-sorting.
struct PtThing
{
  PtThing() {}
  PtThing(int index_, int id_,
          double pt_, double eta_, double phi_, std::string name_="")
   : index(index_), id(id_),
     pt(pt_), eta(eta_), phi(phi_), name(name_) {}		  
  ~PtThing() {}

  /// Copy constructor.
  PtThing(const PtThing& rhs)
  {
    *this = rhs; // this will call assignment operator
  }

  /// Assignment. 
  PtThing& operator=(const PtThing& rhs)
  {
    index = rhs.index;
    id    = rhs.id;
    pt    = rhs.pt;
    eta   = rhs.eta;
    phi   = rhs.phi;
	name  = rhs.name;
    var   = rhs.var;
    return *this;
  }

  /** Find $|Delta R = \sqrt{\Delta\phi^2+\Delta\eta^2}$ between this
      PtThing and the given.
  */
  double deltaR(PtThing& thing)
  {
    double deta = eta - thing.eta;
	double dphi = phi - thing.phi;
    
	// Make sure acute
	if ( fabs(dphi) > TMath::Pi() ) dphi = TMath::TwoPi() - fabs(dphi);
	return sqrt(deta*deta+dphi*dphi);
  }

  /// Compare direction of this PtThing with another using deltaR.
  bool matches(PtThing& thing, double drcut=0.4)
  {
    return deltaR(thing) < drcut;
  }

  int index;
  int id;
  double pt;
  double eta;
  double phi;
  std::string name;
    
  /// Map for additional variables.
  std::map<std::string, double> var;
    
  /// To sort PtThings in descending pt.
  bool operator<(const PtThing& o) const { return o.pt < this->pt; }
};

///
std::vector<MatchedPair>
deltaR(std::vector<PtThing>& v1, std::vector<PtThing>& v2)
{
  if ( v1.size() == 0 || v2.size() == 0 ) return std::vector<MatchedPair>();

  std::vector<MatchedPair> mp(v1.size(), MatchedPair());
  std::vector<MatchedPair> vp(v2.size(), MatchedPair());

  for(unsigned i=0; i < v1.size(); i++)
    {
      for(unsigned j=0; j < v2.size(); j++)
        {
          vp[j].first = i;
          vp[j].second = j;
          vp[j].distance = v1[i].deltaR(v2[j]);
        }
      std::sort(vp.begin(), vp.end());
      mp[i].first = i;
      mp[i].second = vp[0].second;
      mp[i].distance = vp[0].distance;
    }
  return mp;
}

#endif
