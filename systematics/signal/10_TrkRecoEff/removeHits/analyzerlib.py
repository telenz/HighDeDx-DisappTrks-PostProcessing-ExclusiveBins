# -----------------------------------------------------------------------------
#  File:        analyzerlib.py
#  Description: Analyzer for ntuples created by TheNtupleMaker
#  Created:     Mon Jul 27 15:16:23 2015 by mkanalyzer.py
#  Author:      Teresa Lenz
# -----------------------------------------------------------------------------
import os, sys, re
from ROOT import *
from string import atof, atoi, split, strip, replace
from array import array
from math import *
from time import sleep
from sys import exit
# -----------------------------------------------------------------------------
gSystem.AddIncludePath("-Iinclude")
gROOT.ProcessLine(".L src/treestream.cc+")
gROOT.ProcessLine(".L src/pdg.cc+")
# -----------------------------------------------------------------------------
# -- Classes, procedures and functions
# -----------------------------------------------------------------------------
class outputFile:

	def __init__(self, filename, stream=None, savecount=50000):
		if stream != None:
			print "events will be skimmed to file", filename			
			self.tree = stream.tree().CloneTree(0)
			self.weight = Double()
			self.b_weight = self.tree.Branch("eventWeight", self.weight,
											 "eventWeight/D")
			self.SAVECOUNT = savecount
		else:
			self.tree = None
			self.b_weight = None

		self.entry = 0

		self.filename = filename
		self.file = TFile(filename, "recreate")

		self.hist = TH1F("counts", "", 1, 0, 1)
		self.hist.SetBit(TH1.kCanRebin)
		self.hist.SetStats(0)

		self.b_weight = 0

	def addEvent(self, weight=1.0):
		if self.tree == None: return

		self.file = self.tree.GetCurrentFile()
		self.file.cd()
		self.tree.Fill()

		self.entry += 1		
		if self.entry % self.SAVECOUNT == 0:
			self.tree.AutoSave("SaveSelf")

	def count(self, cond, w=1.0):
		self.hist.Fill(cond, w)

	def close(self):
		print "==> histograms saved to file", self.filename
		if self.tree != None:			
			print "==> events skimmed to file", self.filename
			self.file = self.tree.GetCurrentFile()

		self.file.cd()
		#self.file.Write("", TObject.kOverwrite)
		self.file.Write()
		self.file.ls()
		self.file.Close()
# -----------------------------------------------------------------------------
class commandLine:
	def __init__(self):
		pass

def decodeCommandLine():
	argv = sys.argv
	argc = len(argv)

	cl = commandLine()
	cl.progname = split(os.path.basename(argv[0]),'.')[0]

	if argc > 1:
		cl.filelist = argv[1]
	else:
		cl.filelist = "filelist.txt"

	if argc > 2: 
		cl.outputfilename = argv[2] # 2nd (optional) command line argument
	else:
		cl.outputfilename = cl.progname + "_histograms"

	# Make sure extension is ".root"
	if cl.outputfilename[:-5] != ".root": cl.outputfilename += ".root"
	print "==> output to:", cl.outputfilename

	return cl
# -----------------------------------------------------------------------------
def error(message):
	print "** error ** " + message
	sys.exit(0)
# -----------------------------------------------------------------------------
#  Read ntuple filenames from file list

def getFilenames(filelist):
	if not os.path.exists(filelist):
		error("unable to open file: " + filelist)

	# Get ntuple file names
	filenames = filter(lambda x: x != "",
					   map(strip, open(filelist).readlines()))
	v = vector("string")()
	for filename in filenames: v.push_back(filename)
	return v
# -----------------------------------------------------------------------------
TEXTFONT = 42
TEXTSIZE = 0.031
#------------------------------------------------------------------------------
def setStyle():
	gROOT.SetStyle("Pub")
	style = gROOT.GetStyle("Pub")
	style.SetPalette(1)

	# For the canvas
	style.SetCanvasBorderMode(0)
	style.SetCanvasColor(kWhite)
	style.SetCanvasDefH(500)
	style.SetCanvasDefW(500)
	style.SetCanvasDefX(0)
	style.SetCanvasDefY(0)

	# For the pad
	style.SetPadBorderMode(0)
	style.SetPadColor(kWhite)
	style.SetPadGridX(kFALSE)
	style.SetPadGridY(kTRUE)
	style.SetGridColor(kGreen)
	style.SetGridStyle(3)
	style.SetGridWidth(1)

	# For the frame
	style.SetFrameBorderMode(0)
	style.SetFrameBorderSize(1)
	style.SetFrameFillColor(0)
	style.SetFrameFillStyle(0)
	style.SetFrameLineColor(1)
	style.SetFrameLineStyle(1)
	style.SetFrameLineWidth(1)

	# For the histogram
	style.SetHistLineColor(1)
	style.SetHistLineStyle(0)
	style.SetHistLineWidth(1)
	style.SetEndErrorSize(2)
	#style.SetErrorX(0.)
	style.SetMarkerSize(0.1)
	style.SetMarkerStyle(20)

	# For the fit/function
	style.SetOptFit(1)
	style.SetFitFormat("5.4g")
	style.SetFuncColor(2)
	style.SetFuncStyle(1)
	style.SetFuncWidth(1)

	#For the date
	style.SetOptDate(0)

	# For the statistics box
	style.SetOptFile(0)
	style.SetOptStat("")
	# To display the mean and RMS
	#style.SetOptStat("mr") 
	style.SetStatColor(kWhite)
	style.SetStatFont(TEXTFONT)
	style.SetStatFontSize(TEXTSIZE)
	style.SetStatTextColor(1)
	style.SetStatFormat("6.4g")
	style.SetStatBorderSize(1)
	style.SetStatH(0.2)
	style.SetStatW(0.3)

	# Margins
	style.SetPadTopMargin(0.05)
	style.SetPadBottomMargin(0.16)
	style.SetPadLeftMargin(0.16)
	style.SetPadRightMargin(0.16)

	# For the global title
	style.SetOptTitle(0)
	style.SetTitleFont(TEXTFONT)
	style.SetTitleColor(1)
	style.SetTitleTextColor(1)
	style.SetTitleFillColor(10)
	style.SetTitleFontSize(TEXTSIZE*1.1)

	# For the axis titles
	style.SetTitleColor(1, "XYZ")
	style.SetTitleFont(TEXTFONT, "XYZ")
	style.SetTitleSize(TEXTSIZE*1.2, "XYZ") # 0,05
	style.SetTitleXOffset(1.25) # 0.9
	style.SetTitleYOffset(1.25) # 1.25

	# For the axis labels
	style.SetLabelColor(1, "XYZ")
	style.SetLabelFont(TEXTFONT, "XYZ")
	style.SetLabelOffset(0.006, "XYZ")
	style.SetLabelSize(TEXTSIZE*1.2, "XYZ")

	# For the axis
	style.SetAxisColor(1, "XYZ")
	style.SetStripDecimals(kTRUE)
	style.SetTickLength(0.03, "XYZ")
	style.SetNdivisions(505, "XYZ")

	# To get tick marks on the opposite side of the frame
	style.SetPadTickX(1)  
	style.SetPadTickY(1)

	# Change for log plots
	style.SetOptLogx(0)
	style.SetOptLogy(0)
	style.SetOptLogz(0)

	# Postscript options
	style.SetPaperSize(20.,20.)

	style.cd()
# -----------------------------------------------------------------------------
#  Define variables to be read
# -----------------------------------------------------------------------------
MET_energy	= Double()
MET_et	= Double()
MET_eta	= Double()
MET_phi	= Double()
MET_pt	= Double()
MET_pz	= Double()
MuonTracksAllHits_charge	= vector("float")(200)
MuonTracksAllHits_chi2	= vector("float")(200)
MuonTracksAllHits_eta	= vector("float")(200)
MuonTracksAllHits_hitPattern_trackerLayersWithoutMeasurement	= vector("unsigned short")(200)
MuonTracksAllHits_ndof	= vector("float")(200)
MuonTracksAllHits_numberOfValidHits	= vector("unsigned short")(200)
MuonTracksAllHits_phi	= vector("float")(200)
MuonTracksAllHits_pt	= vector("float")(200)
MuonTracksAllHits_ptError	= vector("float")(200)
MuonTracksAllHits_px	= vector("float")(200)
MuonTracksAllHits_py	= vector("float")(200)
MuonTracksAllHits_pz	= vector("float")(200)
MuonTracksAllHits_trackHighPurity	= vector("int")(200)
MuonTracksAllHits_trackRelIso03	= vector("float")(200)
MuonTracksAllHits_trackerExpectedHitsInner_numberOfLostHits	= vector("unsigned short")(200)
MuonTracksAllHits_trackerExpectedHitsOuter_numberOfHits	= vector("unsigned short")(200)
MuonTracksAllHits_vx	= vector("float")(200)
MuonTracksAllHits_vy	= vector("float")(200)
MuonTracksAllHits_vz	= vector("float")(200)
TrackAllHits_charge	= vector("float")(2000)
TrackAllHits_chi2	= vector("float")(2000)
TrackAllHits_eta	= vector("float")(2000)
TrackAllHits_hitPattern_trackerLayersWithoutMeasurement	= vector("unsigned short")(2000)
TrackAllHits_ndof	= vector("float")(2000)
TrackAllHits_numberOfValidHits	= vector("unsigned short")(2000)
TrackAllHits_phi	= vector("float")(2000)
TrackAllHits_pt	= vector("float")(2000)
TrackAllHits_ptError	= vector("float")(2000)
TrackAllHits_px	= vector("float")(2000)
TrackAllHits_py	= vector("float")(2000)
TrackAllHits_pz	= vector("float")(2000)
TrackAllHits_trackHighPurity	= vector("int")(2000)
TrackAllHits_trackRelIso03	= vector("float")(2000)
TrackAllHits_trackerExpectedHitsInner_numberOfLostHits	= vector("unsigned short")(2000)
TrackAllHits_trackerExpectedHitsOuter_numberOfHits	= vector("unsigned short")(2000)
TrackAllHits_vx	= vector("float")(2000)
TrackAllHits_vy	= vector("float")(2000)
TrackAllHits_vz	= vector("float")(2000)
Track_charge	= vector("float")(2000)
Track_chi2	= vector("float")(2000)
Track_eta	= vector("float")(2000)
Track_hitPattern_trackerLayersWithoutMeasurement	= vector("unsigned short")(2000)
Track_ndof	= vector("float")(2000)
Track_numberOfValidHits	= vector("unsigned short")(2000)
Track_phi	= vector("float")(2000)
Track_pt	= vector("float")(2000)
Track_ptError	= vector("float")(2000)
Track_px	= vector("float")(2000)
Track_py	= vector("float")(2000)
Track_pz	= vector("float")(2000)
Track_trackHighPurity	= vector("int")(2000)
Track_trackRelIso03	= vector("float")(2000)
Track_trackerExpectedHitsInner_numberOfLostHits	= vector("unsigned short")(2000)
Track_trackerExpectedHitsOuter_numberOfHits	= vector("unsigned short")(2000)
Track_vx	= vector("float")(2000)
Track_vy	= vector("float")(2000)
Track_vz	= vector("float")(2000)
Vertex_ndof	= vector("float")(100)
Vertex_position_rho	= vector("float")(100)
Vertex_x	= vector("float")(100)
Vertex_y	= vector("float")(100)
Vertex_z	= vector("float")(100)
edmEventHelper_bunchCrossing	= Long()
edmEventHelper_event	= Long()
edmEventHelper_isRealData	= Long()
edmEventHelper_luminosityBlock	= Long()
edmEventHelper_orbitNumber	= Long()
edmEventHelper_run	= Long()
nMuonTracksAllHits	= Long()
nTrack	= Long()
nTrackAllHits	= Long()
nVertex	= Long()

stream.select("recoPFMET_pfMet.energy", MET_energy)
stream.select("recoPFMET_pfMet.et", MET_et)
stream.select("recoPFMET_pfMet.eta", MET_eta)
stream.select("recoPFMET_pfMet.phi", MET_phi)
stream.select("recoPFMET_pfMet.pt", MET_pt)
stream.select("recoPFMET_pfMet.pz", MET_pz)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.charge", MuonTracksAllHits_charge)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.chi2", MuonTracksAllHits_chi2)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.eta", MuonTracksAllHits_eta)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.hitPattern_trackerLayersWithoutMeasurement", MuonTracksAllHits_hitPattern_trackerLayersWithoutMeasurement)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.ndof", MuonTracksAllHits_ndof)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.numberOfValidHits", MuonTracksAllHits_numberOfValidHits)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.phi", MuonTracksAllHits_phi)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.pt", MuonTracksAllHits_pt)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.ptError", MuonTracksAllHits_ptError)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.px", MuonTracksAllHits_px)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.py", MuonTracksAllHits_py)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.pz", MuonTracksAllHits_pz)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.trackHighPurity", MuonTracksAllHits_trackHighPurity)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.trackRelIso03", MuonTracksAllHits_trackRelIso03)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.trackerExpectedHitsInner_numberOfLostHits", MuonTracksAllHits_trackerExpectedHitsInner_numberOfLostHits)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.trackerExpectedHitsOuter_numberOfHits", MuonTracksAllHits_trackerExpectedHitsOuter_numberOfHits)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.vx", MuonTracksAllHits_vx)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.vy", MuonTracksAllHits_vy)
stream.select("recoTrackHelper_globalMuonsAllHitsReduced.vz", MuonTracksAllHits_vz)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.charge", TrackAllHits_charge)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.chi2", TrackAllHits_chi2)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.eta", TrackAllHits_eta)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.hitPattern_trackerLayersWithoutMeasurement", TrackAllHits_hitPattern_trackerLayersWithoutMeasurement)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.ndof", TrackAllHits_ndof)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.numberOfValidHits", TrackAllHits_numberOfValidHits)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.phi", TrackAllHits_phi)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.pt", TrackAllHits_pt)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.ptError", TrackAllHits_ptError)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.px", TrackAllHits_px)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.py", TrackAllHits_py)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.pz", TrackAllHits_pz)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.trackHighPurity", TrackAllHits_trackHighPurity)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.trackRelIso03", TrackAllHits_trackRelIso03)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.trackerExpectedHitsInner_numberOfLostHits", TrackAllHits_trackerExpectedHitsInner_numberOfLostHits)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.trackerExpectedHitsOuter_numberOfHits", TrackAllHits_trackerExpectedHitsOuter_numberOfHits)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.vx", TrackAllHits_vx)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.vy", TrackAllHits_vy)
stream.select("recoTrackHelper_generalTracksAllHitsReduced.vz", TrackAllHits_vz)
stream.select("recoTrackHelper_generalTracksReduced.charge", Track_charge)
stream.select("recoTrackHelper_generalTracksReduced.chi2", Track_chi2)
stream.select("recoTrackHelper_generalTracksReduced.eta", Track_eta)
stream.select("recoTrackHelper_generalTracksReduced.hitPattern_trackerLayersWithoutMeasurement", Track_hitPattern_trackerLayersWithoutMeasurement)
stream.select("recoTrackHelper_generalTracksReduced.ndof", Track_ndof)
stream.select("recoTrackHelper_generalTracksReduced.numberOfValidHits", Track_numberOfValidHits)
stream.select("recoTrackHelper_generalTracksReduced.phi", Track_phi)
stream.select("recoTrackHelper_generalTracksReduced.pt", Track_pt)
stream.select("recoTrackHelper_generalTracksReduced.ptError", Track_ptError)
stream.select("recoTrackHelper_generalTracksReduced.px", Track_px)
stream.select("recoTrackHelper_generalTracksReduced.py", Track_py)
stream.select("recoTrackHelper_generalTracksReduced.pz", Track_pz)
stream.select("recoTrackHelper_generalTracksReduced.trackHighPurity", Track_trackHighPurity)
stream.select("recoTrackHelper_generalTracksReduced.trackRelIso03", Track_trackRelIso03)
stream.select("recoTrackHelper_generalTracksReduced.trackerExpectedHitsInner_numberOfLostHits", Track_trackerExpectedHitsInner_numberOfLostHits)
stream.select("recoTrackHelper_generalTracksReduced.trackerExpectedHitsOuter_numberOfHits", Track_trackerExpectedHitsOuter_numberOfHits)
stream.select("recoTrackHelper_generalTracksReduced.vx", Track_vx)
stream.select("recoTrackHelper_generalTracksReduced.vy", Track_vy)
stream.select("recoTrackHelper_generalTracksReduced.vz", Track_vz)
stream.select("recoVertex_offlinePrimaryVertices.ndof", Vertex_ndof)
stream.select("recoVertex_offlinePrimaryVertices.position_rho", Vertex_position_rho)
stream.select("recoVertex_offlinePrimaryVertices.x", Vertex_x)
stream.select("recoVertex_offlinePrimaryVertices.y", Vertex_y)
stream.select("recoVertex_offlinePrimaryVertices.z", Vertex_z)
stream.select("edmEventHelper_info.bunchCrossing", edmEventHelper_bunchCrossing)
stream.select("edmEventHelper_info.event", edmEventHelper_event)
stream.select("edmEventHelper_info.isRealData", edmEventHelper_isRealData)
stream.select("edmEventHelper_info.luminosityBlock", edmEventHelper_luminosityBlock)
stream.select("edmEventHelper_info.orbitNumber", edmEventHelper_orbitNumber)
stream.select("edmEventHelper_info.run", edmEventHelper_run)
stream.select("nrecoTrackHelper_globalMuonsAllHitsReduced", nMuonTracksAllHits)
stream.select("nrecoTrackHelper_generalTracksReduced", nTrack)
stream.select("nrecoTrackHelper_generalTracksAllHitsReduced", nTrackAllHits)
stream.select("nrecoVertex_offlinePrimaryVertices", nVertex)

