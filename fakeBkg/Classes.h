class sample{


public:

  TFile *file;
  TTree *tree;

  TH1D* histo;
  TH1D* histoASmi;
  TH1D* histoECalo;

  int    statistics;
  double purity;

  vector<double> *trackEta;
  vector<double> *trackPt;
  vector<double> *trackPtError;
  vector<double> *trackP;
  vector<double> *trackGenPt;
  vector<double> *trackASmi;
  vector<double> *trackHarm2;
  vector<double> *trackCaloIso;
  vector<double> *trackMass;
  vector<double> *trackIsolation;
  vector<double> *trackGenE;
  vector<double> *trackGenEt;
  vector<double> *trackEndVertexRho ;
  vector<double> *trackChi2;
  vector<double> *trackNdof;
  vector<double> *trackHCALRp5Isolation;
  vector<double> *trackECALRp5Isolation;
  vector<double> *trackHCALRp4Isolation;
  vector<double> *trackECALRp4Isolation;
  vector<double> *trackHCALRp3Isolation;
  vector<double> *trackECALRp3Isolation;
  vector<int>    *trackNLostOuter;
  vector<int>    *trackNLostMiddle;
  vector<int>    *trackNLostInner;
  vector<int>    *trackNValid;
  vector<int>    *trackPdgId;
  vector<int>    *trackStatus;
  double weight;
  double weight_xsec_lumi;
  double met;
  double leadingJetPt;
  int nJets;
  double VertexZ;
  unsigned int event;

  vector<double> *trackDeDx1;
  vector<double> *trackDeDx2;
  vector<double> *trackDeDx3;
  vector<double> *trackDeDx4;
  vector<double> *trackDx1;
  vector<double> *trackDx2;
  vector<double> *trackDx3;
  vector<double> *trackDx4;

  vector<int>    *trackMeasSize;

  std::vector<Double_t>  *muonPt;
  std::vector<Double_t>  *muonEt;
  std::vector<Double_t>  *muonPhi;
  std::vector<Double_t>  *muonEta;
  std::vector<Double_t>  *muonChi2;
  std::vector<Double_t>  *muonndof;
  std::vector<Double_t>  *muondB;
  std::vector<Double_t>  *muond0;
  std::vector<Double_t>  *muonVertex_z;
  std::vector<Double_t>  *muonNOfValidMuonHits;
  std::vector<Double_t>  *muonTrackerLayersWithMeas;
  std::vector<Double_t>  *muonNOfValidPixelHits;
  std::vector<Double_t>  *muonNOfMatchedMuonStations;
  std::vector<Double_t>  *muonChargedHadronIso;
  std::vector<Double_t>  *muonNeutralHadronIso;
  std::vector<Double_t>  *muonPhotonIso;
  std::vector<Double_t>  *muonpuChargedHadronIso;
  std::vector<Int_t>  *muonPdgId;
  std::vector<Int_t>  *muonStatus;

  sample(){

    tree=0;
    file=0;
    histo=0;
    statistics=0;
    purity=0;
    histoASmi=0;
    histoECalo=0;

    trackEta        = 0;
    trackEta        = 0;
    trackPt         = 0;
    trackPtError    = 0;
    trackP          = 0;
    trackGenPt      = 0;
    trackASmi       = 0;
    trackHarm2      = 0;
    trackCaloIso    = 0;
    trackMass       = 0;
    trackIsolation  = 0;
    trackGenE       = 0;
    trackGenEt      = 0;
    trackEndVertexRho = 0;
    trackChi2 = 0;
    trackNdof = 0;
    trackHCALRp5Isolation = 0;
    trackECALRp5Isolation = 0;
    trackHCALRp4Isolation = 0;
    trackECALRp4Isolation = 0;
    trackHCALRp3Isolation = 0;
    trackECALRp3Isolation = 0;
    trackNLostOuter = 0;
    trackNLostMiddle = 0;
    trackNLostInner = 0;
    trackNValid     = 0;
    trackPdgId      = 0;
    trackStatus     = 0;
    trackDeDx1   = 0;
    trackDeDx2   = 0;
    trackDeDx3   = 0;
    trackDeDx4   = 0;
    trackDx1   = 0;
    trackDx2   = 0;
    trackDx3   = 0;
    trackDx4   = 0;
    trackMeasSize = 0;
    muonPt = 0;
    muonEt = 0;
    muonPhi = 0;
    muonEta = 0;
    muonChi2 = 0;
    muonndof = 0;
    muondB = 0;
    muond0 = 0;
    muonVertex_z = 0;
    muonNOfValidMuonHits = 0;
    muonTrackerLayersWithMeas = 0;
    muonNOfValidPixelHits = 0;
    muonNOfMatchedMuonStations = 0;
    muonChargedHadronIso = 0;
    muonNeutralHadronIso = 0;
    muonPhotonIso = 0;
    muonpuChargedHadronIso = 0;
    muonPdgId = 0;
    muonStatus = 0;
  };

  void getTreeVariables(){

    tree->SetBranchAddress("trackEta",&trackEta);
    tree->SetBranchAddress("trackPt",&trackPt);
    tree->SetBranchAddress("trackPtError",&trackPtError);
    tree->SetBranchAddress("trackP",&trackP);
    tree->SetBranchAddress("trackGenPt",&trackGenPt);
    tree->SetBranchAddress("trackCaloIsolation",&trackCaloIso);
    tree->SetBranchAddress("trackIsolation",&trackIsolation);
    tree->SetBranchAddress("trackGenE",&trackGenE);
    tree->SetBranchAddress("trackGenEt",&trackGenEt);
    tree->SetBranchAddress("trackChi2",&trackChi2);
    tree->SetBranchAddress("trackNdof",&trackNdof);
    tree->SetBranchAddress("trackNLostOuter",&trackNLostOuter);
    tree->SetBranchAddress("trackNLostInner",&trackNLostInner);
    tree->SetBranchAddress("trackNLostMiddle",&trackNLostMiddle);
    tree->SetBranchAddress("trackNValid",&trackNValid);
    tree->SetBranchAddress("trackStatus",&trackStatus);
    tree->SetBranchAddress("trackPdgId",&trackPdgId);
    tree->SetBranchAddress("trackDeDxASmi",&trackASmi);
    tree->SetBranchAddress("trackDeDxHarm2",&trackHarm2);
    tree->SetBranchAddress("muonPt",&muonPt);
    tree->SetBranchAddress("muonEt",&muonEt);
    tree->SetBranchAddress("muonPhi",&muonPhi);
    tree->SetBranchAddress("muonEta",&muonEta);
    tree->SetBranchAddress("muonChi2",&muonChi2);
    tree->SetBranchAddress("muonndof",&muonndof);
    tree->SetBranchAddress("muondB",&muondB);
    tree->SetBranchAddress("muond0",&muond0);
    tree->SetBranchAddress("muonVertex_z",&muonVertex_z);
    tree->SetBranchAddress("muonNOfValidMuonHits",&muonNOfValidMuonHits);
    tree->SetBranchAddress("muonTrackerLayersWithMeas",&muonTrackerLayersWithMeas);
    tree->SetBranchAddress("muonNOfValidPixelHits",&muonNOfValidPixelHits);
    tree->SetBranchAddress("muonNOfMatchedMuonStations",&muonNOfMatchedMuonStations);
    tree->SetBranchAddress("muonChargedHadronIso",&muonChargedHadronIso);
    tree->SetBranchAddress("muonNeutralHadronIso",&muonNeutralHadronIso);
    tree->SetBranchAddress("muonPhotonIso",&muonPhotonIso);
    tree->SetBranchAddress("muonpuChargedHadronIso",&muonpuChargedHadronIso);
    tree->SetBranchAddress("muonPdgId",&muonPdgId);
    tree->SetBranchAddress("muonStatus",&muonStatus);
    tree->SetBranchAddress("MET",&met);
    tree->SetBranchAddress("njets",&nJets);
    tree->SetBranchAddress("event",&event);
    tree->SetBranchAddress("VertexZ",&VertexZ);
    tree->SetBranchAddress("LeadingJetPt",&leadingJetPt);
    tree->SetBranchAddress("weight",&weight);
    tree->SetBranchAddress("weight_xsec_lumi",&weight_xsec_lumi);


    tree->SetBranchAddress("trackHCALRp5Isolation",&trackHCALRp5Isolation);
    tree->SetBranchAddress("trackECALRp5Isolation",&trackECALRp5Isolation);
    tree->SetBranchAddress("trackHCALRp4Isolation",&trackHCALRp4Isolation);
    tree->SetBranchAddress("trackECALRp4Isolation",&trackECALRp4Isolation);
    tree->SetBranchAddress("trackHCALRp3Isolation",&trackHCALRp3Isolation);
    tree->SetBranchAddress("trackECALRp3Isolation",&trackECALRp3Isolation);

  };


  void Selection(double metCut, double jetPtCut, bool fakeSelection, double ptCutMin, double ptCutMax, bool ecaloSelection, bool LowECaloRegion, double ecaloCut=100000.){

    if(histo)      histo      -> Sumw2();
    if(histoASmi)  histoASmi  -> Sumw2();
    if(histoECalo) histoECalo -> Sumw2();

    for(int n=0; n<tree->GetEntries(); n++){

      tree->GetEntry(n);

      if(met<metCut)                     continue;
      if(leadingJetPt<jetPtCut)          continue;

      if(fakeSelection){

	//if(trackNLostOuter->at(0)<1)     continue; 
	if(abs(trackEta->at(0))>2.1)     continue;
	if(abs(trackEta->at(0))>1.42 && abs(trackEta->at(0))<1.65 )  continue;
	
	if(trackPt->at(0)<ptCutMin)         continue;
	if(trackPt->at(0)>ptCutMax)         continue;
		
	if(ecaloSelection){      
	  if(LowECaloRegion){
	    if(trackCaloIso->at(0)>ecaloCut)  continue;
	  }
	  else{
	    if(trackCaloIso->at(0)<ecaloCut)  continue;
	  }

	}
	//	if(trackNValid->at(0)<7)         continue;
      }

      if(histo)      histo        -> Fill(0.5,weight_xsec_lumi*weight);
      if(histoASmi)  histoASmi    -> Fill(trackASmi->at(0),weight_xsec_lumi*weight);
      if(histoECalo) histoECalo   -> Fill(trackCaloIso->at(0),weight_xsec_lumi*weight);
        
    }
  };
};
