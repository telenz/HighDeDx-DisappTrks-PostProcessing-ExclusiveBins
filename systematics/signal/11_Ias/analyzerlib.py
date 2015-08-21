# -----------------------------------------------------------------------------
#  File:        analyzerlib.py
#  Description: Analyzer for ntuples created by TheNtupleMaker
#  Created:     Tue Jul 21 14:55:47 2015 by mkanalyzer.py
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
PileupSummaryInfo_getBunchCrossing	= vector("int")(10)
PileupSummaryInfo_getPU_NumInteractions	= vector("int")(10)
PileupSummaryInfo_getTrueNumInteractions	= vector("float")(10)
Track_charge	= vector("float")(2000)
Track_chi2	= vector("float")(2000)
Track_dEdxASmi	= vector("float")(2000)
Track_dEdxNPASmi	= vector("float")(2000)
Track_dEdxNPNoM	= vector("unsigned int")(2000)
Track_dEdxNoM	= vector("unsigned int")(2000)
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
Track_trackerExpectedHitsInner_numberOfLostHits	= vector("unsigned short")(2000)
Track_trackerExpectedHitsOuter_numberOfHits	= vector("unsigned short")(2000)
Track_vx	= vector("float")(2000)
Track_vy	= vector("float")(2000)
Track_vz	= vector("float")(2000)
edmEventHelper_bunchCrossing	= Long()
edmEventHelper_event	= Long()
edmEventHelper_isRealData	= Long()
edmEventHelper_luminosityBlock	= Long()
edmEventHelper_orbitNumber	= Long()
edmEventHelper_run	= Long()
nPileupSummaryInfo	= Long()
nTrack	= Long()

stream.select("PileupSummaryInfo_addPileupInfo.getBunchCrossing", PileupSummaryInfo_getBunchCrossing)
stream.select("PileupSummaryInfo_addPileupInfo.getPU_NumInteractions", PileupSummaryInfo_getPU_NumInteractions)
stream.select("PileupSummaryInfo_addPileupInfo.getTrueNumInteractions", PileupSummaryInfo_getTrueNumInteractions)
stream.select("recoTrackHelper_TrackRefitter.charge", Track_charge)
stream.select("recoTrackHelper_TrackRefitter.chi2", Track_chi2)
stream.select("recoTrackHelper_TrackRefitter.dEdxASmi", Track_dEdxASmi)
stream.select("recoTrackHelper_TrackRefitter.dEdxNPASmi", Track_dEdxNPASmi)
stream.select("recoTrackHelper_TrackRefitter.dEdxNPNoM", Track_dEdxNPNoM)
stream.select("recoTrackHelper_TrackRefitter.dEdxNoM", Track_dEdxNoM)
stream.select("recoTrackHelper_TrackRefitter.eta", Track_eta)
stream.select("recoTrackHelper_TrackRefitter.hitPattern_trackerLayersWithoutMeasurement", Track_hitPattern_trackerLayersWithoutMeasurement)
stream.select("recoTrackHelper_TrackRefitter.ndof", Track_ndof)
stream.select("recoTrackHelper_TrackRefitter.numberOfValidHits", Track_numberOfValidHits)
stream.select("recoTrackHelper_TrackRefitter.phi", Track_phi)
stream.select("recoTrackHelper_TrackRefitter.pt", Track_pt)
stream.select("recoTrackHelper_TrackRefitter.ptError", Track_ptError)
stream.select("recoTrackHelper_TrackRefitter.px", Track_px)
stream.select("recoTrackHelper_TrackRefitter.py", Track_py)
stream.select("recoTrackHelper_TrackRefitter.pz", Track_pz)
stream.select("recoTrackHelper_TrackRefitter.trackHighPurity", Track_trackHighPurity)
stream.select("recoTrackHelper_TrackRefitter.trackerExpectedHitsInner_numberOfLostHits", Track_trackerExpectedHitsInner_numberOfLostHits)
stream.select("recoTrackHelper_TrackRefitter.trackerExpectedHitsOuter_numberOfHits", Track_trackerExpectedHitsOuter_numberOfHits)
stream.select("recoTrackHelper_TrackRefitter.vx", Track_vx)
stream.select("recoTrackHelper_TrackRefitter.vy", Track_vy)
stream.select("recoTrackHelper_TrackRefitter.vz", Track_vz)
stream.select("edmEventHelper_info.bunchCrossing", edmEventHelper_bunchCrossing)
stream.select("edmEventHelper_info.event", edmEventHelper_event)
stream.select("edmEventHelper_info.isRealData", edmEventHelper_isRealData)
stream.select("edmEventHelper_info.luminosityBlock", edmEventHelper_luminosityBlock)
stream.select("edmEventHelper_info.orbitNumber", edmEventHelper_orbitNumber)
stream.select("edmEventHelper_info.run", edmEventHelper_run)
stream.select("nPileupSummaryInfo_addPileupInfo", nPileupSummaryInfo)
stream.select("nrecoTrackHelper_TrackRefitter", nTrack)

