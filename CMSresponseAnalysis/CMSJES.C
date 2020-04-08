#define CMSJES_cxx
#include "CMSJES.h"

//Created using ROOT TMakeClass
void CMSJES::Loop()
{

  clock_t t_begin = clock();

  //Variant flags
  bool varCp3, varCm3, varTrkEff, varPhoton;
  varCp3    = 0;
  varCm3    = 0;
  varTrkEff = 0;
  varPhoton = 0;

  if (varCp3 && varCm3) {cout << "Both C-variants enabled!" << endl; return;}

  //Initialize rng's
  srand(1); //srand(time(0));
  TRandom3* gRandom = new TRandom3();
  gRandom->SetSeed(1); //gRandom->SetSeed(0);

  //Initialize cells with ECAL holes
  TFile* coldCellsFile = new TFile("./data_and_MC_input/coldCells/coldjetsmc-18runA.root");
  TH2D*  coldCells     = (TH2D*)coldCellsFile->Get("h2hole");

  int studyMode = -1;
  if (ReadName.find("Zjet")!=string::npos) {
    studyMode = 3;
    cout << "File is for Z+jet sample, with stydy mode: "<< studyMode << endl;
  } else {
    cout << "Error: File not for Z+jet sample! Exiting." << endl;
    return;
  }

  if (fChain == 0) return;
  Long64_t nentries       = fChain->GetEntriesFast();
  string outname          = "./output_ROOT_files/CMSJES_" + ReadName; //Output file
  if (varCp3)    outname += "_varCp3";
  if (varCm3)    outname += "_varCm3";
  if (varTrkEff) outname += "_varTrkEff";
  if (varPhoton) outname += "_varPhoton";
  outname += ".root";
  TFile *fout = new TFile(outname.c_str(),"RECREATE");

  int const nbinsMPF = 15;
  const double binsxMPF[nbinsMPF] = {31.75, 41.0, 50.5, 63.5, 83.0, 105.25, 132.5, 173.25,
                                    228.25, 300.0, 391.25, 503.75, 681.75, 951.5, 1258.25};

  //Jet flavour dependent MPF responses *b = b-jets, *g = g-jets, *lq=(u,d,s,c)-jets
  string MPFTitle = ";p_{T,tag}^{MC} [GeV];R_{MPF}";
  TProfile* prMPF   = new TProfile("prMPF"  , MPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prMPFb  = new TProfile("prMPFb" , MPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prMPFg  = new TProfile("prMPFg" , MPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prMPFlq = new TProfile("prMPFlq", MPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prMPFud = new TProfile("prMPFud", MPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prMPFs  = new TProfile("prMPFs" , MPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prMPFc  = new TProfile("prMPFc" , MPFTitle.c_str(), nbinsMPF-1, binsxMPF);

  //Jet response
  string RjetTitle    = ";p_{T,gen}^{jet} [GeV]";
         RjetTitle   += ";p_{T,reco}^{jet} / p_{T,gen}^{jet}";
  string RjetbTitle   = ";p_{T,gen}^{b-jet} [GeV]";
         RjetbTitle  += ";p_{T,reco}^{b-jet} / p_{T,gen}^{b-jet}";
  string RjetgTitle   = ";p_{T,gen}^{g-jet} [GeV]";
         RjetgTitle  += ";p_{T,reco}^{g-jet} / p_{T,gen}^{g-jet}";
  string RjetlqTitle  = ";p_{T,gen}^{lq-jet} [GeV]";
         RjetlqTitle += ";p_{T,reco}^{lq-jet} / p_{T,gen}^{lq-jet}";
  string RjetudTitle  = ";p_{T,gen}^{ud-jet} [GeV]";
         RjetudTitle += ";p_{T,reco}^{ud-jet} / p_{T,gen}^{ud-jet}";
  string RjetsTitle   = ";p_{T,gen}^{s-jet} [GeV]";
         RjetsTitle  += ";p_{T,reco}^{s-jet} / p_{T,gen}^{s-jet}";
  string RjetcTitle   = ";p_{T,gen}^{c-jet} [GeV]";
         RjetcTitle  += ";p_{T,reco}^{c-jet} / p_{T,gen}^{c-jet}";
  TProfile* prRjet   = new TProfile("prRjet",   RjetTitle.c_str(),   nbinsMPF-1, binsxMPF);
  TProfile* prRjetb  = new TProfile("prRjetb",  RjetbTitle.c_str(),  nbinsMPF-1, binsxMPF);
  TProfile* prRjetg  = new TProfile("prRjetg",  RjetgTitle.c_str(),  nbinsMPF-1, binsxMPF);
  TProfile* prRjetlq = new TProfile("prRjetlq", RjetlqTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prRjetud = new TProfile("prRjetud", RjetudTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prRjets  = new TProfile("prRjets",  RjetsTitle.c_str(),  nbinsMPF-1, binsxMPF);
  TProfile* prRjetc  = new TProfile("prRjetc",  RjetcTitle.c_str(),  nbinsMPF-1, binsxMPF);

  TProfile* prRjet_calo  = new TProfile("prRjet_calo",  RjetcTitle.c_str(),  nbinsMPF-1, binsxMPF);

  //Jet flavour fraction histos: FFb = b-jets, FFg = g-jets, FFlq=(u,d,s,c)-jets
  TH1D* FFb = new TH1D("FFb",  "",nbinsMPF-1,binsxMPF);
  TH1D* FFg = new TH1D("FFg",  "",nbinsMPF-1,binsxMPF);
  TH1D* FFud= new TH1D("FFud", "",nbinsMPF-1,binsxMPF);
  TH1D* FFs = new TH1D("FFs",  "",nbinsMPF-1,binsxMPF);
  TH1D* FFc = new TH1D("FFc",  "",nbinsMPF-1,binsxMPF);
  TH1D* FFlq= new TH1D("FFlq", "",nbinsMPF-1,binsxMPF);
  TH1D* FFa = new TH1D("FFa",  "",nbinsMPF-1,binsxMPF); //All

  //Response function R_h (h for hadron)
  TF1 *fr_h = new TF1("frh","[0]*(1-[1]*pow(x,[2]-1))",0,4000);

  // C-parameter variation +- 3%
  //if (varCp3) { fr_h = new TF1("frh","1.03*[0]*(1-[1]*pow(x,[2]-1))",0,4000);}
  //if (varCm3) { fr_h = new TF1("frh","0.97*[0]*(1-[1]*pow(x,[2]-1))",0,4000);}
  fr_h->Print();

  //Used in HCAL calibration, pion response parameters now in |eta|<1.3
  TF1 *fr_all  = new TF1("fr_all", "x*1.044*(1-1.293*pow(x,0.543-1))",0,4000);
  TF1 *fr_hcal = new TF1("fr_hcal","x*1.123*(1-1.345*pow(x,0.421-1))",0,4000);
  TF1 *fr_ecal = new TF1("fr_ecal","x*1.076*(1-1.403*pow(x,0.646-1))",0,4000);

  //Higher resolution
  int reso = 10000; fr_h->SetNpx(reso); fr_hcal->SetNpx(reso); fr_ecal->SetNpx(reso);

  // INITIALIZATIONS
  Long64_t nbytes = 0, nb = 0;
  TLorentzVector p4;      //4-vector temp.
  TLorentzVector p4_calo; //calorimeter cell 4-vector

  //Calorimeter and track 4-vectors
  TLorentzVector p4_chc;
  TLorentzVector p4_cht;
  TLorentzVector p4_rescale; //rescaled 4-vector 

  TLorentzVector tag;        //Parton level tag object 4-vector
  TLorentzVector tag_r;	     //With muon smearing

  TLorentzVector probe_g;    //Generator level without neutrinos  
  TLorentzVector probe_r;    //PF reconstructed probe 4-vector
  TLorentzVector probe_calo; //Calo reconstructed probe 4-vector

  TLorentzVector jet2_r;     //Second leading pT jet

  //Cell energies of the probe object
  double probe_ch;
  double probe_nh;
  double probe_gamma;
  double probe_e;

  //Partial derivative values for a hadron
  unsigned int i_tag   = 0;   //Stepper to find tag object index
  unsigned int i_probe = 0;   //       -||-     probe jet index
  unsigned int i_jet2  = 1;   //       -||-     2nd jet index
  double eta_muon      = 2.3; //Max |eta| for a single muon in Z+jet      
  double eta_tag_z     = 2.5; //Max |eta| for mumu tag object
  double eta_probe     = 1.3; //Max |eta| for probe jets
  int PDG              = 1;   //Shorthand, to store a particle's PDGID
  int JI               = 0;   //Shorthand, particle's jet index
  double phiMin        = 2.8; //Minimum azimuth angle between tag and probe
  double pTmin_probe   = 15;  //Minimum probe jet p_T (GeV)
  double pTmin_muon    = 15;  //Minimum single tag muon pT (GeV)
  double pTmin_tag_z   = 15;  //Minimum tag muon pair pT (GeV)   
  double resp          = 1.0; //SPR value                (dummy init)
  double respA         = 0.0; //Calorimeter response for All-hadrons
  double respEHE       = 0.0; //Calorimeter response for EHE-hadrons
  double respHHe       = 0.0; //Calorimeter response for HHe-hadrons
  double R_MPF_r       = 0;   //MC-reco'd MPF response
  double Rjet          = 0.0; //Jet pT-response
  double Rjet_calo     = 0.0; //Calo jet pT-response

  double RCone         = 0.4; //Jet radius

  unsigned long njets;          //#jets in the event, for resizing vectors
  TLorentzVector MET_r;         //Reconstructed MET four vector
  vector<TLorentzVector> jets_g;//Gen lvl jet 4-vectors

  //HHe / (HHe + EHE)
  double HHeFrac = 0.45;

  //CMS detector related parameters
  double Bfield = 3.8; //Tesla
  double Rt     = 1.2; //Tracker radius

  //MET calculation related variables
  double cellPhi; double cellEta; double cellE;  double cellPt;  double delta; double cellSigma;
  double eff;     double newPhi;  double calReso; double trkReso; bool trkFail; 
  double cellE_calo; int probeFlav;

  //Histograms to contain the gen level particle content in probe
  TH1F* h_e;      TH1F* h_mu;     TH1F* h_gamma;  TH1F* h_gpi0;
  TH1F* h_K;      TH1F* h_K0S;    TH1F* h_K0L;    TH1F* h_L;
  TH1F* h_p;      TH1F* h_n;      TH1F* h_nu;     TH1F* h_pi;
  TH1F* h_Xi0;    TH1F* h_Xim;    TH1F* h_Om;     TH1F* h_Sig0;
  TH1F* h_Sigm;   TH1F* h_Sigp;   
  TH1F* h_other;  TH1F* h_all;	//For normalizing histo stacks
  TH1F* h_ptr;	//To contain one of the above pointer sets at a time
  vector<int> otherIDs;	//To contain PDGIDs of unclassified particles


  //Machinery for histogram stacking
  vector<string> hTitles;
  THStack* hstack;
  THStack* hstackOther;
  //hTitles.push_back("#font[132]{Z+jet probe particle content (gen)}");
  hTitles.push_back("");

  //Instantiate and setup composition histos if needed
  int const nbins_h = 30; int const nbins_chf = 23;
  const double bins_h_lo = 0.0;
  const double bins_h_hi = 3000;
  //From https://github.com/miquork/jetphys/blob/master/settings.h_template
  const double bins_chf[24] = {15, 21, 28, 37, 49, 64, 84, 114, 153, 196, 272, 330, 395, 468,
                               548, 686, 846, 1032, 1248, 1588, 2000, 2500, 3103, 3832};

  //Jet energy fraction profiles
  TProfile* prchf    = new TProfile("prchf"    , "", nbins_chf, bins_chf);
  TProfile* prnhf    = new TProfile("prnhf"    , "", nbins_chf, bins_chf);
  TProfile* prgammaf = new TProfile("prgammaf" , "", nbins_chf, bins_chf);  
  TProfile* pref     = new TProfile("pref"     , "", nbins_chf, bins_chf);  

  if (GetcontHistos()) {
    h_e     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //e^+-
    h_gamma = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Photons
    h_gpi0  = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //pi^0 phot.
    h_K     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Kaons K^+-
    h_K0S   = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //K^0_S
    h_K0L   = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //K^0_L
    h_L     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Lambda
    h_mu    = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //mu
    h_n     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Neutrons
    h_nu    = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Neutrinos
    h_pi    = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //pi^+-
    h_p     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Protons
    h_other = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi);
    h_Xi0   = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Xi^0
    h_Xim   = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Xi^-
    h_Sig0  = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Sigma^0
    h_Sigm  = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Sigma^-
    h_Sigp  = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Sigma^+
    h_Om    = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Omega^-
    //All particle types
    h_all   = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi);
    //For stacking the above histograms to plot them simultaneously
    hstack      = new THStack("", hTitles[0].c_str() );
    hstackOther = new THStack("","");
    //Fill colours
    h_e->SetFillColor(    kAzure+1);  h_mu->SetFillColor(  kViolet-3);
    h_gamma->SetFillColor(41);        h_gpi0->SetFillColor(42);
    h_pi->SetFillColor(   kAzure-9);  h_K->SetFillColor(    kTeal-9);
    h_K0S->SetFillColor(  kTeal-8);   h_K0L->SetFillColor( kTeal-7);
    h_L->SetFillColor(    kCyan-1);
    h_p->SetFillColor(    kGray+2);   h_n->SetFillColor(   kGray);
    h_other->SetFillColor(kOrange-9); h_nu->SetFillColor(  kWhite);
    h_Xi0->SetFillColor(  kOrange-8); h_Xim->SetFillColor( kOrange-4);
    h_Sig0->SetFillColor( kOrange-7); h_Sigm->SetFillColor(kOrange-5);
    h_Sigp->SetFillColor( kOrange-9); h_Om->SetFillColor(  kOrange-3);
    //More narrow lines for better readability
    h_e->SetLineWidth(    1);  h_mu->SetLineWidth(   1);
    h_gamma->SetLineWidth(1);  h_gpi0->SetLineWidth( 1);
    h_pi->SetLineWidth(   1);  h_K->SetLineWidth(    1);
    h_K0S->SetLineWidth(  1);  h_K0L->SetLineWidth(  1);
    h_p->SetLineWidth(    1);  h_n->SetLineWidth(    1);
    h_L->SetLineWidth(    1);
    h_other->SetLineWidth(1);  h_nu->SetLineWidth(   1);
    h_Xi0->SetLineWidth(  1);  h_Xim->SetLineWidth(  1);
    h_Sig0->SetLineWidth( 1);  h_Sigm->SetLineWidth( 1);
    h_Sigp->SetLineWidth( 1);  h_Om->SetLineWidth(   1);
    //Line colours
    h_e->SetLineColor(    kBlack);  h_mu->SetLineColor(   kBlack);
    h_gamma->SetLineColor(kBlack);  h_gpi0->SetLineColor( kBlack);
    h_pi->SetLineColor(   kBlack);  h_K->SetLineColor(    kBlack);
    h_K0S->SetLineColor(  kBlack);  h_K0L->SetLineColor(  kBlack);
    h_p->SetLineColor(    kBlack);  h_n->SetLineColor(    kBlack);
    h_L->SetLineColor(    kBlack);
    h_other->SetLineColor(kBlack);  h_nu->SetLineColor(   kBlack);
    h_Xi0->SetLineColor(  kBlack);  h_Xim->SetLineColor(  kBlack);
    h_Sig0->SetLineColor( kBlack);  h_Sigm->SetLineColor( kBlack);
    h_Sigp->SetLineColor( kBlack);  h_Om->SetLineColor(   kBlack);
  }

  /* LOOP EVENTS */
  cout << "Entering loop";
  if (!printProg) cout << ", progress printing disabled"; cout << endl;

  //Machinery for skipping events that didn't pass cuts the last time
  fstream cutflag_stream;
  string cutflag_file = "./cutflag_files/" + ReadName;
  cutflag_file += ".bin";
  if (GetuseEarlierCuts()) cout << "Checking for "; 
  else                     cout << "Writing new ";
  cout << cutflag_file << endl;
  char readTemp;
  Long64_t rejected=0, passed=0; //#events rejected and passed in sample

  //Read usable events from file, else write to file at the end of each looping
  if (GetuseEarlierCuts()) {
    cutflag_stream.open(cutflag_file, std::ios::in);		//Read mode
    for (Long64_t l=1; cutflag_stream.read(&readTemp,1); ++l) {
      cutflag_stream.seekg(l);		//Proceed to next byte in file
      if (readTemp=='1') {passedCuts.push_back(true);  ++passed;  }
      else               {passedCuts.push_back(false); ++rejected;}
    }
    cutflag_stream.close();
    if (passedCuts.size()==0) {
      cout << "The file contains no events, writing a new file" << endl;
      SetuseEarlierCuts(false);
      for (long l=0; l != nentries; ++l) passedCuts.push_back(false);
    } else if (GetprintProg()) {
      cout << "Read " << rejected + passed << " events:" << endl;
      cout << "  Passed " << passed << ", rejected " << rejected << endl;
      cout << "  passedCuts.size()=" << passedCuts.size() << " events" << endl;
    }
  } else for (long l=0; l != nentries; ++l) passedCuts.push_back(false);

  //For keeping track of how many events pass each event cut
  int mumuCut = 0;
  int invM = 0;
  int tagCut = 0;
  int alpha = 0;
  int genprobeCut = 0;
  int b2b = 0;
  int probeCut = 0;
  int METtag = 0;
  int METprobe = 0;

//***********************************************************************************************
  //Granularity of cells following the granularity of the HCAL 
  int nPhi = 72; int nEta = 119;
  TH2D* cht          = new TH2D("cht"          ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* chtPt        = new TH2D("chtPt"        ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* chtP_curv    = new TH2D("chtP_curv"    ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* chc          = new TH2D("chc"          ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* chECAL_curv  = new TH2D("chECAL_curv"  ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* chHCAL_curv  = new TH2D("chHCAL_curv"  ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* sigmaTrk     = new TH2D("sigmaTrk"     ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* sigmaTrk_curv= new TH2D("sigmaTrk_curv","",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* nhECAL       = new TH2D("nhECAL"       ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* nhHCAL       = new TH2D("nhHCAL"       ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* ne           = new TH2D("ne"           ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* eCalo        = new TH2D("eCalo"        ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);

  const double bins_chhEff[41] = {0.1, 0.122, 0.149, 0.182, 0.223, 0.272, 0.332, 0.406, 0.496, 
                                  0.605, 0.740, 0.904, 1.104, 1.349, 1.648, 2.013, 2.460, 3.004,
                                  3.670, 4.483, 5.477, 6.690, 8.174, 9.983, 12.198, 14.898,
                                  18.202, 22.232, 27.163, 33.176, 40.536, 49.508, 60.492, 
                                  73.880, 90.272, 110.249, 134.713, 164.522, 201.031, 245.513, 
                                  300.0};

  TProfile* chhEff   = new TProfile("chhEff", "", 40, bins_chhEff);

  //Charged hadron energy fraction from Hannu's dijet studies
  TF1* pchf = new TF1("pchf","[0]*pow(x,2) + [1]*x + [2]", 0, 5000);
  pchf->SetParameters(6.862211e-09, -0.0001268141, 0.6283211);

  //Pion track resolution from our sample:
  TF1* piTrkReso = new TF1("piTrkReso", "0.01336 + 8.548e-5*x", 0, 5000);

  //Relative resolutions from Trackin paper:
  TF1* muTrkReso = new TF1("muTrkReso","4.66042e-07*pow(x,2)+0.00010326*x+0.0080762", 0, 5000); 

  //Full muon system resolution q/p in function of pT -> dp = qoverperror()*P*P
  // DPovP = dp/P
  TF1* muFSReso = new TF1("muFSReso","0.002579707*pow(x,0.4552789)", 0, 5000);

  // 68% parameters
  //TF1* allTrkReso = new TF1("allTrkReso","2.77564e-06*pow(x,2)+0.000124*x+0.010373" , 0, 5000);

  // 90% parametes
  TF1* allTrkReso = new TF1("allTrkReso","2.006093e-05*pow(x,2)+0.00103068*x+0.0190268",0,5000);


//***********************************************************************************************

  //Loop Tree entries = events
  for (Long64_t jentry=0; jentry != nentries; ++jentry) {
    //Print progress for long runs
    if ((GetprintProg() && jentry % 1000==0)){
      cout << "Looping event " << jentry; cout << " in Z+jet" << endl;
    }

    //Skip events that didn't pass cuts earlier. Useful in e.g. repeating Loop
    if (GetuseEarlierCuts() && passedCuts.size()>jentry && !passedCuts[jentry]) continue;
 


    Long64_t ientry = LoadTree(jentry);	//Load new event
    if (ientry < 0) break;		//When no more events
    nb = fChain->GetEntry(jentry); nbytes += nb;

    //Reinit
    i_tag = 0; i_probe = 0; i_jet2 = 1;
    p4.SetPtEtaPhiE(0,0,0,0);      p4_calo.SetPtEtaPhiE(0,0,0,0); 
    MET_r.SetPtEtaPhiE(0,0,0,0);

    tag.SetPtEtaPhiE(0,0,0,0);     tag_r.SetPtEtaPhiE(0,0,0,0);
    
    probe_g.SetPtEtaPhiE(0,0,0,0);
    probe_r.SetPtEtaPhiE(0,0,0,0); 
    probe_calo.SetPtEtaPhiE(0,0,0,0);

    jet2_r.SetPtEtaPhiE(0,0,0,0);

    probe_ch    = 0.0; probe_nh    = 0.0;
    probe_gamma = 0.0; probe_e     = 0.0;

    jets_g.clear();
    njets = (unsigned long)jet_pt->size();
    jets_g.resize(njets);
    for (int i=0; i!=jets_g.size(); ++i) { //All objects have njets size
      jets_g[i].SetPtEtaPhiE(0,0,0,0);
    }

    //**************** Z+JET: FIND AND RECONSTRUCT TAG MUONS ****************//
    int muPDG=13; int muTAG=3; //mu PDGID and parton tag
    if (ReadName.find("H7")!=string::npos) muTAG=2; 

    int i_tag1 = -137;	       // These values won't change if...
    int i_tag2 = -731;	       // ...muons not found.

    for (int a=0; a!= prtn_tag->size(); ++a) {

      // Find muons with tag == 3 and store those in i_tag1 and i_tag2
      if ((*prtn_tag)[a] == muTAG && abs((*prtn_pdgid)[a]) == muPDG) {
        if      (i_tag1 == -137) i_tag1 = a;
        else if (i_tag2 == -731) i_tag2 = a;
        else {cout << "More than two muons with tag 3 in the event." << endl; continue;}
      }
    }

    if (i_tag1 == -137 && i_tag2 == -731) continue; //No two muons found


    //***** 1st muon *****
    p4.SetPtEtaPhiE((*prtn_pt )[i_tag1], (*prtn_eta)[i_tag1],
                    (*prtn_phi)[i_tag1], (*prtn_e  )[i_tag1]);

    tag = p4;//gen lvl

    p4 *= gRandom->Gaus(1,muFSReso->Eval(p4.Pt()));

    if (fabs(p4.Eta()) > eta_muon || p4.Pt() < pTmin_muon) continue;

    tag_r = p4;

    //***** 2nd muon *****
    p4.SetPtEtaPhiE((*prtn_pt )[i_tag2], (*prtn_eta)[i_tag2],
                    (*prtn_phi)[i_tag2], (*prtn_e  )[i_tag2]);

    tag += p4;//gen lvl

    p4 *= gRandom->Gaus(1,muFSReso->Eval(p4.Pt()));

    if (fabs(p4.Eta()) > eta_muon || p4.Pt() < pTmin_muon) continue; mumuCut++;

    tag_r += p4;      

    //Invariant mass in range 70-110 GeV since Z boson mass is 91 GeV
    if (tag.M()<70.0 || tag.M()>110.0) continue; invM ++;

    //Tag eta and pT cuts:
    if (fabs(tag_r.Eta()) > eta_tag_z || tag_r.Pt() < pTmin_tag_z) continue; tagCut++;

    /********************* SELECT THE PROBE JET AND 2nd JET ********************/
    //Probe
    i_probe = 0;
    //2nd jet
    i_jet2 = 1;

    // If muon is found from the jet it is not acceptable
    // Confirm implementation and optimize
    for (int i=0; i < int(jet_e->size()); ++i) { // Loop over jets
      for (int j=0; j != prtcl_pt->size(); ++j) { // Loop over all particles
        if ((*prtcl_jet)[j] == i) {
          if ((*prtn_pt )[i_tag1]==(*prtcl_pt)[j] || (*prtn_pt )[i_tag2]==(*prtcl_pt)[j]) {
            if (i_probe == i) {i_probe++; i_jet2++;}
            if (i_jet2 == i)   i_jet2++;
          }
        }
      }
    }

    /******** RECONSTRUCT GEN-LEVEL JETS AND ADD LEPTONS TO THE PROBE *********/

    for (int i=0; i != prtcl_pt->size(); ++i) {

      JI = (*prtcl_jet)[i]; 
      PDG = abs((*prtcl_pdgid)[i]);
      p4.SetPtEtaPhiE((*prtcl_pt )[i], (*prtcl_eta)[i],	(*prtcl_phi)[i], (*prtcl_e  )[i]);

      Response(PDG, p4.Eta(), p4.E(), p4.Pt(), Rt, Bfield, HHeFrac, 
               fr_h, resp, respA, respEHE, respHHe);
 
      //Reconstruct gen-level jets, at the moment has no neutrinos
      jets_g[JI] += (isNeutrino(PDG) ? 0:1)*p4; //Gen lvl

      //Add muons and electrons to the reconstructed probe jet and second jet
      if (PDG==11 || PDG==13) {
        if ((*prtcl_jet)[i]==i_probe) {
          probe_r += resp*p4;
          if (PDG == 11) {
            probe_e += resp*p4.E(); //Probe electron energy for the fraction calculation
          }
        } else if ((*prtcl_jet)[i] == i_jet2) { //2nd jet
          jet2_r  += resp*p4;
        }
      } 
    } //Loop particles in jets
    
    /********************* RECONSTRUCT GEN LEVEL PROBES *********************/

    //Gen lvl as output by FastJet
    probe_g.SetPtEtaPhiE((*jet_pt)[i_probe],  (*jet_eta)[i_probe],
                       (*jet_phi)[i_probe], (*jet_e)[i_probe]  );

    /****************************** COMMON CUTS FOR Z+JET ******************************/

    //PF probe-tag cuts
    if (fabs(probe_g.Eta()) > eta_probe || probe_g.Pt() < pTmin_probe) continue; genprobeCut ++;

    //Back-to-back leikkaus
    if (fabs(tag.DeltaPhi(probe_g)) < phiMin) continue; b2b ++;


    /********************** PROBE GEN PARTICLE CONTENT **********************/
    
    if (GetcontHistos()) {
      for (unsigned int i=0; i!=prtcl_pdgid->size(); ++i) {

        JI  = (*prtcl_jet)[i];
        PDG = abs((*prtcl_pdgid)[i]); 

        //See if ptcl belongs to the probe jet
        if (JI==i_probe) {
          switch (PDG) {
            case 11   : h_e    ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 22   : h_gamma->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 20   : h_gpi0 ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 321  : h_K    ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 310  : h_K0S  ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 130  : h_K0L  ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 3122 : h_L    ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 13   : h_mu   ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 2112 : h_n    ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 211  : h_pi   ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 2212 : h_p    ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            default :
              if (isNeutrino(PDG)) {
                h_nu->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
              }
              else if (isStrangeB(PDG)) {
                switch (PDG) {
                  case 3112 : h_Sigm->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
                  case 3212 : h_Sig0->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
                  case 3222 : h_Sigp->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
                  case 3312 : h_Xim->Fill( (*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
                  case 3322 : h_Xi0->Fill( (*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
                  case 3334 : h_Om->Fill(  (*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
                  default : cout<<"Error: isStrangeB passed ID "<<PDG<<endl;
                }
                //Fill the general "other particles" histogram already here
                h_other->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]);
              } //Xi, Sigma, Omega^-
              //List further unknown particle PDGIDs
              if (find(otherIDs.begin(),otherIDs.end(),PDG)==otherIDs.end()&& 
                  !isNeutrino(PDG)  ) otherIDs.push_back(PDG);
          } //Switch PDG (general)
          h_all->Fill((*jet_e)[(*prtcl_jet)[i_probe]], (*prtcl_e)[i]);
        }
      }
    }
    
    /******************* RECONSTRUCT PARTICLES NOT IN JETS *******************/
    //Reset histograms
    cht          ->Reset();	//Original directions of
    chtPt        ->Reset();	//Charged hadrons
    chtP_curv    ->Reset();	//Linking track momentum to the cell
    chc          ->Reset();	//For calo-trk weighting
    chECAL_curv  ->Reset();	//Calo jet calculation
    chHCAL_curv  ->Reset();	//       -||-
    nhECAL       ->Reset();	//Neutral hadrons depositing in ECAL (also ch when trk fails)
    nhHCAL       ->Reset();	//           -||-               HCAL         -||-
    ne           ->Reset();	//Photon cells
    eCalo        ->Reset();	//Calo jet calculation
    sigmaTrk     ->Reset();	//For weighting
    sigmaTrk_curv->Reset();	//For shadowing effect


    //Probe flavor check, prtn_tag=0 for outgoing hard process partons
    //for (unsigned int j = 0; j != prtn_tag->size(); ++j) {
    //  if ((*prtn_jet)[j]==i_probe && (*prtn_tag)[j]==0) {
    //    probeFlav = abs((*prtn_pdgid)[j]);
    //    continue;
    //  }
    //}
    //if(probeFlav != 5) continue; //Uncomment to study only specific jets

    //****************** MET calculation *******************//
    //Add tag object to the MET
    MET_r -= tag_r;

    //Loop over all particles
    for (int i=0; i!=prtclnij_pt->size(); ++i) {

      PDG = fabs((*prtclnij_pdgid)[i]);
      p4.SetPtEtaPhiE((*prtclnij_pt )[i],(*prtclnij_eta)[i],
                      (*prtclnij_phi)[i],(*prtclnij_e  )[i]);

      Response((*prtclnij_pdgid)[i], p4.Eta(), p4.E(), p4.Pt(), Rt, Bfield, HHeFrac, 
               fr_h, resp, respA, respEHE, respHHe);

      if (resp == 0.0) continue;

      p4 *= resp;

      switch (PDG) {
        //PHOTON
        case 20 : case 22 : 
          ne->Fill(p4.Phi(), p4.Eta(), p4.E());
          break;

        //LEPTONS
        case 13 : case 11 :
          // Not one of the tag muons
          if ((*prtclnij_pt)[i]!=(*prtn_pt)[i_tag1] && (*prtclnij_pt )[i]!=(*prtn_pt)[i_tag2]){ 
            MET_r -= p4; //Muons and electrons to the MET
          }

          //Electron ECAL deposit taking into account track curvature with response = 1
          if (PDG == 11) {
            newPhi = trackDeltaPhi((*prtclnij_pdgid)[i], (*prtclnij_phi)[i], 
                                   (*prtclnij_pt )[i], Rt, Bfield);

            p4.SetPtEtaPhiE((*prtclnij_pt )[i], (*prtclnij_eta)[i], newPhi, (*prtclnij_e)[i]);
            eCalo->Fill(p4.Phi(), p4.Eta(), p4.E()); //Fill electron calo grid
          }

          break;

        //CHARGED HADRONS
        case 211 : case 321 : case 2212 : case 3112 : case 3222 : case 3312 : case 3334 :
          trkFail = 0;
          eff = 1.0; //No particle level track failing

          //Efficiency from chs
          if (fabs(p4.Eta() < 2.5)) {
            for (int ijet=0; ijet!=jets_g.size(); ++ijet) {
              if (p4.DeltaR(jets_g[ijet]) < RCone) {
                eff = pchf->Eval(jets_g[ijet].Pt()) / 0.607;
                eff -= 0.1381969 - 0.1606539/pow(2,((*prtclnij_pt)[i]/45.31742));
              }
            }
          }

          eff = min(eff,0.96);

          if ((*prtclnij_pt)[i] < 0.6) {
            eff = 2021.944 - 2022.2565603/(1 + pow(((*prtclnij_pt)[i]/36533650), 0.4126626));
          } else if ((*prtclnij_pt)[i] > 0.6 && (*prtclnij_pt)[i] < 10.0) {
            eff -= 0.03836*pow((*prtclnij_pt)[i],-0.70570) - 0.003;
          }

          eff = max(eff,0.0);

          if (varTrkEff) eff *= 0.99; //-1% variation to track efficiency

          //Tracking efficiency plot
          if (fabs((*prtclnij_eta)[i]) < 2.5) {
            chhEff->Fill((*prtclnij_pt)[i], eff);
          }

          if (resp != 1) eff = 0.0; //If not in tracker

          if ( ((double)rand() / (double)RAND_MAX) > eff ) trkFail = 1;

          //Track resolutions
          trkReso = max(0.0, allTrkReso->Eval((*prtclnij_pt)[i]) * p4.P());

          //Phi where particle exits the tracker / enters ECAL
          newPhi = trackDeltaPhi((*prtclnij_pdgid)[i], (*prtclnij_phi)[i], 
                                 (*prtclnij_pt )[i], Rt, Bfield);

          if (!trkFail) { //resp == 1.0
            cht  ->Fill(p4.Phi(), p4.Eta(), p4.E() );
            chtPt->Fill(p4.Phi(), p4.Eta(), p4.Pt());

            if (doesReachEcal((*prtclnij_pdgid)[i], (*prtclnij_pt )[i], Bfield, Rt)) {
              chtP_curv->Fill(newPhi, p4.Eta(), p4.P());
              sigmaTrk->Fill(p4.Phi(), p4.Eta(), pow(trkReso,2));
              sigmaTrk_curv->Fill(newPhi, p4.Eta(), pow(trkReso,2));
            } 
          }
          
          //Re-initialize
          p4.SetPtEtaPhiE((*prtclnij_pt )[i], (*prtclnij_eta)[i], newPhi, (*prtclnij_e)[i]);

          //Calorimeter deposit for the weighting
          if (!trkFail) {
            chc->Fill((*prtclnij_phi)[i], p4.Eta(), respA*p4.E());
          }

          //ECAL and HCAL deposits
          if ( ((double)rand()/(double)RAND_MAX) > HHeFrac ) { //EHE path
            p4 *= respEHE;

            if (trkFail) {
              nhECAL->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E());
              nhHCAL->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E());
            } else {
              chECAL_curv->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E());
              chHCAL_curv->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E());
            }
          } else { //HHe path 
            p4 *= respHHe;

            if (trkFail) {
              nhHCAL->Fill(p4.Phi(), p4.Eta(), p4.E());
            } else {
              chHCAL_curv->Fill(p4.Phi(), p4.Eta(), p4.E());
            }
          }
          break;

        //NEUTRAL HADRONS
        case 130 : case 310 : case 3122 : case 2112 : case 3212 : case 3322 :
          p4.SetPtEtaPhiE((*prtclnij_pt )[i],(*prtclnij_eta)[i],
                          (*prtclnij_phi)[i],(*prtclnij_e  )[i]);

          if ( ((double)rand()/(double)RAND_MAX) > HHeFrac ) { //EHE path
            p4 *= respEHE;
            nhECAL->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E()); //ECAL
            nhHCAL->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E()); //HCAL
          } else { //HHe path 
            p4 *= respHHe;
            nhHCAL->Fill(p4.Phi(), p4.Eta(), p4.E()); //HCAL
          }
          break;

        default : 
          if (fabs(PDG) != 12 && fabs(PDG) != 14 && fabs(PDG) != 16) {
            cout << "PDGID " << PDG << endl;
          }
      }
    } //Particle loop

    // ***************** Loop over cells ******************
    for (int i=1; i!=cht->GetNbinsX()+1; ++i) {
      for (int j=1; j!=cht->GetNbinsY()+1; ++j) {

        
        //Skip if no energy deposit in the cell
        if (cht->GetBinContent(i,j)         == 0.0 && chtPt->GetBinContent(i,j)       == 0.0 &&
            chtP_curv->GetBinContent(i,j)   == 0.0 && chc->GetBinContent(i,j)         == 0.0 &&
            chECAL_curv->GetBinContent(i,j) == 0.0 && chHCAL_curv->GetBinContent(i,j) == 0.0 &&
            nhECAL->GetBinContent(i,j)      == 0.0 && nhHCAL->GetBinContent(i,j)      == 0.0 &&
            ne->GetBinContent(i,j)          == 0.0 && eCalo->GetBinContent(i,j)       == 0.0)
            continue;
        

        double w = 1.0; int iEtaCold; double totalChargedMomentum; 
        double nhHCAL_calib = 0.0; double chc_calib = 0.0; double caloResolution = 0.0;
        p4_chc.SetPtEtaPhiE(0,0,0,0); p4_cht.SetPtEtaPhiE(0,0,0,0);
        p4.SetPtEtaPhiE(0,0,0,0);     p4_rescale.SetPtEtaPhiE(0,0,0,0);
        delta = 0.0; cellSigma = 0.0;        

        cellPhi = cht->GetXaxis()->GetBinCenter(i);
        cellEta = cht->GetYaxis()->GetBinCenter(j);
        
        
        if (varCp3) {
          nhECAL     ->SetBinContent(i,j, 1.03*nhECAL->GetBinContent(i,j));
          nhHCAL     ->SetBinContent(i,j, 1.03*nhHCAL->GetBinContent(i,j));
          chc        ->SetBinContent(i,j, 1.03*chc->GetBinContent(i,j));
          chECAL_curv->SetBinContent(i,j, 1.03*chECAL_curv->GetBinContent(i,j));
          chHCAL_curv->SetBinContent(i,j, 1.03*chHCAL_curv->GetBinContent(i,j));
        } else if (varCm3) {
          nhECAL     ->SetBinContent(i,j, 0.97*nhECAL->GetBinContent(i,j));
          nhHCAL     ->SetBinContent(i,j, 0.97*nhHCAL->GetBinContent(i,j));
          chc        ->SetBinContent(i,j, 0.97*chc->GetBinContent(i,j));
          chECAL_curv->SetBinContent(i,j, 0.97*chECAL_curv->GetBinContent(i,j));
          chHCAL_curv->SetBinContent(i,j, 0.97*chHCAL_curv->GetBinContent(i,j));
        } else if (varPhoton) {
          ne         ->SetBinContent(i,j, 0.99*ne->GetBinContent(i,j)); // Photon scale
        }


        //**** Cold cells
        iEtaCold = floor((cellEta+fabs(coldCells->GetXaxis()->GetBinLowEdge(1)))/
                          coldCells->GetXaxis()->GetBinWidth(1) + 1);

        //In phi-direction, one cell covers two in the cold cells histogram.
        //If either one of those is cold, 150 GeV cut is applied to the ECAL.
	//Effect nevertheless minor, might remove it in the future?
        if (coldCells->GetBinContent(iEtaCold,i*2  ) > 1.0 || 
            coldCells->GetBinContent(iEtaCold,i*2-1) > 1.0) {
          
          if (nhECAL->GetBinContent(i,j) + ne->GetBinContent(i,j) > 150.0) {
            nhECAL->SetBinContent(i,j,0);
            ne->SetBinContent(i,j,150);
          }
        }
        //****

        //Calo cell four vector
        cellE_calo = ne->GetBinContent(i,j)          + eCalo->GetBinContent(i,j)
                   + nhECAL->GetBinContent(i,j)      + nhHCAL->GetBinContent(i,j) 
                   + chECAL_curv->GetBinContent(i,j) + chHCAL_curv->GetBinContent(i,j);
        p4_calo.SetPtEtaPhiE(cellE_calo/cosh(cellEta), cellEta, cellPhi, cellE_calo);

        if (p4_calo.DeltaR(probe_g) < RCone) {
          probe_calo += p4_calo;
        }

        //HCAL calibration
        if (nhHCAL->GetBinContent(i,j) > 0.0) {
          nhHCAL_calib = fr_hcal->GetX(nhHCAL->GetBinContent(i,j), 0.1, 7000.0);
        }

        //***** PF-code calorimeter resolution *****
        totalChargedMomentum = chtP_curv->GetBinContent(i,j);

        caloResolution = fabs(cellEta) < 1.48 
                         ? sqrt(1.02*1.02/max(totalChargedMomentum,1.0) + 0.065*0.065)
                         : sqrt(1.20*1.20/max(totalChargedMomentum,1.0) + 0.028*0.028);

        caloResolution *= totalChargedMomentum;


        //TotalError
        cellSigma  = sqrt(pow(caloResolution,2) + sigmaTrk_curv->GetBinContent(i,j));
        cellSigma *= (1.0 + exp(-totalChargedMomentum/100.0));
        //******************************************


        //Calibrated calorimetric energy for neutral hadrons and photons
        delta = nhECAL->GetBinContent(i,j) + ne->GetBinContent(i,j) + nhHCAL_calib;

        //Calibrated calorimetric energy deposit from charged hadrons
        if (chc->GetBinContent(i,j) > 0.0) {
          chc_calib = fr_all->GetX(chc->GetBinContent(i,j), 0.1, 7000.0);
        }

        if (delta < cellSigma) { //Shadowing
          nhHCAL_calib = 0.0;
          nhECAL->SetBinContent(i,j,0.0); 
          ne    ->SetBinContent(i,j,0.0);
          chc_calib += delta; // Move shadowed energy to ch energy deposit
        }

        //Calculate caloResolution for not curved tracks
        p4.SetPtEtaPhiE(chtPt->GetBinContent(i,j), cellEta, cellPhi, 
                        cht->GetBinContent(i,j));
        
        caloResolution = fabs(cellEta) < 1.48 
                         ? sqrt(1.02*1.02/max(p4.P(),1.0) + 0.065*0.065)
                         : sqrt(1.20*1.20/max(p4.P(),1.0) + 0.028*0.028);
        caloResolution *= p4.P();
        
        //****************************** WEIGHTING ********************************
        w = pow(caloResolution,2) / 
            (sigmaTrk->GetBinContent(i,j) + pow(caloResolution,2));

        if (sqrt(sigmaTrk->GetBinContent(i,j))/p4.P() > 0.1) {
          p4_chc.SetPtEtaPhiE(chc_calib/cosh(cellEta), cellEta, cellPhi, chc_calib);
          p4_cht.SetPtEtaPhiE(chtPt->GetBinContent(i,j), cellEta, cellPhi, 
                              cht->GetBinContent(i,j));

          p4_rescale = w*p4_cht + (1-w)*p4_chc;

        } else { //Normal case
          p4_rescale.SetPtEtaPhiE(chtPt->GetBinContent(i,j), cellEta, cellPhi, 
                                  cht->GetBinContent(i,j));
        }
        //***********************************************************************//

        //Cell 4-vector
        cellE = ne->GetBinContent(i,j) + nhECAL->GetBinContent(i,j) + nhHCAL_calib;
        cellPt = cellE/cosh(cellEta);
        p4.SetPtEtaPhiE(cellPt, cellEta, cellPhi, cellE); //Calibrated calorimeter
        p4 += p4_rescale;                                 //Charged hadron deposit

        MET_r -= p4;

        //Probe reconstruction
        if (p4.DeltaR(probe_g) < RCone) {
          probe_r     += p4;
          probe_ch    += p4_rescale.E();
          probe_nh    += nhHCAL_calib;
          probe_gamma += nhECAL->GetBinContent(i,j) + ne->GetBinContent(i,j);
        }
        //Second leading jet reconstruction for Alpha cut
        if (p4.DeltaR(jets_g[i_jet2]) < RCone) {
          jet2_r += p4;
        }
      }
    }

    /**************************** FILL HISTOGRAMS ****************************/

    //reco Alpha cut?
    if (jet2_r.Pt() > 0.3*tag_r.Pt()) continue; alpha ++;

    //reco probe cuts
    if (fabs(probe_r.Eta()) > eta_probe || probe_r.Pt() < pTmin_probe) continue; probeCut ++;

    //MET cuts
    if (MET_r.Pt() > 0.9*tag_r.Pt())   continue; METtag ++;
    //if (MET_r.Pt() > 0.7*probe_r.Pt()) continue; METprobe ++;

    double totalE;
    totalE = probe_ch + probe_nh + probe_gamma + probe_e;

    prchf   ->Fill(tag_r.Pt(), probe_ch/totalE,    weight);
    prnhf   ->Fill(tag_r.Pt(), probe_nh/totalE,    weight);
    prgammaf->Fill(tag_r.Pt(), probe_gamma/totalE, weight);
    pref    ->Fill(tag_r.Pt(), probe_e/totalE,     weight); 

    //MPF response
    R_MPF_r = 1.0 + (MET_r.Px()*tag_r.Px() + MET_r.Py()*tag_r.Py()) / pow((tag_r.Pt()),2);
    //R_MPF_r = 1.0 + MET_r.Pt()*cos(tag_r.DeltaPhi(MET_r))/tag_r.Pt();

    //Fill MPF profile
    prMPF->Fill(tag_r.Pt(), R_MPF_r, weight);

    Rjet      = probe_r.Pt()/probe_g.Pt();
    Rjet_calo = probe_calo.Pt()/probe_g.Pt(); 

    //All jets
    prRjet->Fill(probe_g.Pt(), Rjet, weight);
    prRjet_calo->Fill(probe_g.Pt(), Rjet_calo, weight);


    //CHECK JET FLAVOUR: FIND FLAVOUR-DEPENDENT QUANTITIES
    //Loop partons to find where jets originated from
    for (unsigned int j = 0; j != prtn_tag->size(); ++j) {
      //prtn_tag=0 for outgoing hard process partons
      //Suffix "a" for "all jet flavours", needed for averaging etc.
      if ((*prtn_jet)[j]==i_probe && (*prtn_tag)[j]==0) {

        //Probe flavour
        probeFlav = abs((*prtn_pdgid)[j]);

        if (probeFlav == 5) {                           //b-jets
          FFb->Fill(tag_r.Pt(), weight);
          prMPFb->Fill(tag_r.Pt(), R_MPF_r, weight);
          prRjetb->Fill(probe_g.Pt(), Rjet, weight);

        } else if (probeFlav < 5) {                     //Light quark (u,d,s,c) jets
          FFlq->Fill(tag_r.Pt(), weight);
          prMPFlq->Fill(tag_r.Pt(), R_MPF_r, weight);
          prRjetlq->Fill(probe_g.Pt(), Rjet, weight);

          if (probeFlav == 4) {                         //c-jets
            FFc->Fill(tag_r.Pt(), weight);
            prMPFc->Fill(tag_r.Pt(), R_MPF_r, weight);
            prRjetc->Fill(probe_g.Pt(), Rjet, weight);            
          } else if (probeFlav == 3) {                  //s-jets
            FFs->Fill(tag_r.Pt(), weight);
            prMPFs->Fill(tag_r.Pt(), R_MPF_r, weight);
            prRjets->Fill(probe_g.Pt(), Rjet, weight);         
          } else if (probeFlav < 3) {                   //(u,d)
            FFud->Fill(tag_r.Pt(), weight);
            prMPFud->Fill(tag_r.Pt(), R_MPF_r, weight);
            prRjetud->Fill(probe_g.Pt(), Rjet, weight);  
          }
        } else if (probeFlav == 21) {                   //Gluon jets
          FFg->Fill(tag_r.Pt(), weight);
          prMPFg->Fill(tag_r.Pt(), R_MPF_r, weight);
          prRjetg->Fill(probe_g.Pt(), Rjet, weight);

        } else continue; //Undetermined flavour
        FFa->Fill(tag_r.Pt(), weight); continue;
      }
    } //Loop partons

    //If the old list of cut events is not read, a new one is written
    if (!GetuseEarlierCuts()) passedCuts[jentry]=true;
  } //Loop Tree entries

  if (!GetuseEarlierCuts()) {
    //Write the accepted event information into a binary file
    cutflag_stream.open(cutflag_file, std::ios::out); //Write mode
    for (long l=0; l!=passedCuts.size(); ++l) {
      if (passedCuts[l]) cutflag_stream << true;
      else               cutflag_stream << false;
    }
    cutflag_stream.close();	//Close the output stream
  }

  
  if (GetcontHistos()) { //Particle composition and flavour fraction histograms

    //Canvas and related setup
    string canvName = ReadName + "_prtclCont";
    TCanvas* canv = new TCanvas(canvName.c_str(),"",1500,750);
    canv->Divide(2,1);

    TLegend* lg      = new TLegend(0.865, 0.1, 1.0,  0.9);
    TLegend* lgOther = new TLegend(0.865,0.55, 1.0, 0.90);
    lg->SetBorderSize(0);  lgOther->SetBorderSize(0);   //No frame
    lg->SetFillStyle(0);   lgOther->SetFillStyle(0);    //No background fill

    //Normalize histograms by the sum of all ptcls and add to stack
    h_gamma->Divide(h_all);	hstack->Add(h_gamma);
    h_gpi0->Divide( h_all);	hstack->Add(h_gpi0 );
    h_pi->Divide(   h_all);	hstack->Add(h_pi   );
    h_K->Divide(    h_all);	hstack->Add(h_K    );
    h_K0S->Divide(  h_all);	hstack->Add(h_K0S  );
    h_K0L->Divide(  h_all);	hstack->Add(h_K0L  );
    h_L->Divide(    h_all);	hstack->Add(h_L    );
    h_n->Divide(    h_all);	hstack->Add(h_n    );
    h_e->Divide(    h_all);	hstack->Add(h_e    );
    h_mu->Divide(   h_all);	hstack->Add(h_mu   );
    h_nu->Divide(   h_all);	hstack->Add(h_nu   );
    h_p->Divide(    h_all);	hstack->Add(h_p    );
    //Normalize the Xi, Sigma & Omega histos by the "other particles" histo
    h_Xi0->Divide( h_other);	hstackOther->Add(h_Xi0);
    h_Xim->Divide( h_other);	hstackOther->Add(h_Xim);
    h_Sig0->Divide(h_other);	hstackOther->Add(h_Sig0);
    h_Sigm->Divide(h_other);	hstackOther->Add(h_Sigm);
    h_Sigp->Divide(h_other);	hstackOther->Add(h_Sigp);
    h_Om->Divide( h_other);	hstackOther->Add(h_Om);
    //Finally divide the "other particles" histo
    h_other->Divide(h_all);	hstack->Add(h_other);
    //Legend setup
    lg->AddEntry(h_other, " #font[12]{Other}",     "f");
    lg->AddEntry(h_p,     " #font[12]{p}",         "f");
    lg->AddEntry(h_nu,    " #nu",                  "f");
    lg->AddEntry(h_mu,    " #mu",                  "f");
    lg->AddEntry(h_e,     " #font[12]{e^{#pm}}",   "f");
    lg->AddEntry(h_n,     " #font[12]{n}",         "f");
    lg->AddEntry(h_L,     " #Lambda",              "f");
    lg->AddEntry(h_K0L,   " #font[12]{K^{0}_{L}}", "f");
    lg->AddEntry(h_K0S,   " #font[12]{K^{0}_{S}}", "f");
    lg->AddEntry(h_K,     " #font[12]{K^{#pm}}",   "f");
    lg->AddEntry(h_pi,    " #pi^{#pm}",            "f");
    lg->AddEntry(h_gpi0,  " #gamma(#pi^{0}) ",     "f");
    lg->AddEntry(h_gamma, " #gamma",               "f");
    //The legend for separate "other particles" plot
    lgOther->AddEntry(h_Om,   " #Omega^{-}", "f");
    lgOther->AddEntry(h_Sigp, " #Sigma^{+}", "f");
    lgOther->AddEntry(h_Sigm, " #Sigma^{-}", "f");
    lgOther->AddEntry(h_Sig0, " #Sigma^{0}", "f");
    lgOther->AddEntry(h_Xim,  " #Xi^{-}",    "f");
    lgOther->AddEntry(h_Xi0,  " #Xi^{0}",    "f");

      //General particle composition histograms
    canv->cd(1);
    hstack->Draw("HISTO");
    hstack->GetYaxis()->SetRange(0.0,0.5);
    hstack->GetXaxis()->SetTitle("#font[12]{E}_{jet}^{gen} [GeV]");
    hstack->GetYaxis()->SetTitle("Fraction of jet #font[12]{E}");
    hstack->GetYaxis()->SetTitleFont(133);
    hstack->GetYaxis()->SetTitleSize(20);
    hstack->GetXaxis()->SetTitleFont(133);
    hstack->GetXaxis()->SetTitleSize(20);
    hstack->GetXaxis()->SetTitleOffset(1.8);
    hstack->GetYaxis()->SetTitleOffset(2.0);
    lg->Draw();

    //More specific "other particles" histograms
    canv->cd(2);

    hstackOther->Draw("HISTO");
    hstackOther->GetYaxis()->SetRange(0.0,1.0);
    hstackOther->GetXaxis()->SetTitle("#font[12]{E}_{jet}^{gen} [GeV]");
    hstackOther->GetYaxis()->SetTitle("Fraction of #font[12]{other}");
    hstackOther->GetYaxis()->SetTitleFont(133);
    hstackOther->GetYaxis()->SetTitleSize(20);
    hstackOther->GetXaxis()->SetTitleFont(133);
    hstackOther->GetXaxis()->SetTitleSize(20);
    hstackOther->GetXaxis()->SetTitleOffset(1.8);
    hstackOther->GetYaxis()->SetTitleOffset(2.0);
    lgOther->Draw();

    //Save particle content histogram plot
    string plotName = "./plots/particleComposition/PC_" + ReadName;
    plotName += ".eps";	//Filetype suffix
    canv->Print(plotName.c_str());

    //Print unclassified particles
    if (printProg && otherIDs.size()!=0) {
      cout << "Sample includes also these "<<otherIDs.size()<<" PDGIDs:"<<endl;
      for (int i=0; i<otherIDs.size(); ++i) cout << otherIDs[i] << endl;
    }

    //Free memory allocated only if GetcontHistos()==true
    delete lg;     delete lgOther;
    delete h_e;    delete h_mu;    delete h_gamma; delete h_gpi0;
    delete h_K;    delete h_K0S;   delete h_K0L;   delete h_L;
    delete h_n;    delete h_nu;    delete h_pi;
    delete h_p;    delete h_other; delete h_Xi0;   delete h_Xim;
    delete h_Sig0; delete h_Sigm;  delete h_Sigp;
    delete h_Om;   delete h_all;
    delete hstack; delete hstackOther;
    delete canv;
  } //Particle composition histograms

  cout << endl << "Event cuts:" << endl;
  cout << "Total number of events: " << nentries    << endl;
  cout << "Tag muon cuts:          " << mumuCut     << endl;
  cout << "Z invariant mass:       " << invM        << endl;
  cout << "Tag eta pT:             " << tagCut      << endl;
  cout << "Gen level Probe:        " << genprobeCut << endl;
  cout << "Btb tag-probe:          " << b2b         << endl;
  cout << "Alpha:                  " << alpha       << endl;
  cout << "Reco level Probe:       " << probeCut    << endl;
  cout << "MET-tag cut:            " << METtag      << endl;
  cout << "MET-probe cut:          " << METprobe    << endl << endl;
  
  //Charged hadron efficiency Profile
  TCanvas *c4     = new TCanvas("","c4",500,500);
  TGraph *PFeff   = new TGraph("data_and_MC_input/Efficiency/hadron_efficiency_PF.txt");
  TGraph *PFeffFr = new TGraph("data_and_MC_input/Efficiency/hadron_efficiencyPlusFakerate_PF.txt");
  TGraph *TRKeff  = new TGraph("data_and_MC_input/Efficiency/hadron_efficiency_TRK.txt");
  PFeff->SetMarkerStyle(kFullCircle ); TRKeff->SetMarkerStyle(kOpenCircle );
  PFeff->SetMarkerColor(kRed);         TRKeff->SetMarkerColor(kBlue+1);
  PFeffFr->SetMarkerStyle(kFullDiamond );
  PFeffFr->SetMarkerColor(kGreen+2);

  TLegend* lgEff = new TLegend(0.5,0.2,0.8,0.40 );
  lgEff->AddEntry(chhEff, "#font[132]{Our eff}", "l");
  lgEff->AddEntry(PFeff,  "#font[132]{PF eff}", "p");
  lgEff->AddEntry(PFeffFr,"#font[132]{PF eff+fakerate}", "p");
  lgEff->AddEntry(TRKeff, "#font[132]{TRK eff}", "p");
  lgEff->SetBorderSize(0); lgEff->SetFillStyle(0);

  PFeff->GetXaxis()->SetTitle("pT");
  PFeff->GetYaxis()->SetTitle("Efficiency");
   
  TAxis *PFaxis = PFeff->GetXaxis();
  PFaxis->SetLimits(0.1,300);
  PFeff->GetHistogram()->SetMaximum(1.1);
  PFeff->GetHistogram()->SetMinimum(0.0);
  PFeff->Draw("ap");      lgEff->Draw("same");
  PFeffFr->Draw("samep"); TRKeff->Draw("samep");

  gStyle->SetOptStat();
  c4->SetLogx();
  chhEff->Draw("same");
  chhEff->SetLineColor(kBlack);
  chhEff->SetLineWidth(3);
  string savename = "./plots/Efficiency/trkEff.eps";
  c4->Print(savename.c_str());

  //Save CMSJES TTree
  fout->Write();
  fout->Close();

  //Free single-particle response functions from memory
  delete fr_h; delete fr_hcal;

  clock_t t_end = clock();
  cout << "Execution time: " << (double)(t_end - t_begin) / CLOCKS_PER_SEC << endl;

} //CMSJES::Loop

void CMSJES::Response(int pdgid, double pseudorap, double energy, double pT, double Rt, 
                      double Bfield, double HHeFrac, TF1* frH, double& retMC, double& retA, 
                      double& retEHE, double& retHHe)
{
  retMC = 0.0; retA = 0.0; retEHE = 0.0; retHHe = 0.0;
  bool zero = false; //If true, return zero responses

  double sfCh = 0.0; //Charged particle step function
  double sfN  = 0.0; //Neutral hadron step function

  //Responses for different groups
  double cat1     = 0.0;    double cat2     = 0.0;     double cat3     = 0.0;
  double cat1_EHE = 0.0;    double cat2_EHE = 0.0;     double cat3_EHE = 0.0;
  double cat1_HHe = 0.0;    double cat2_HHe = 0.0;     double cat3_HHe = 0.0;

  double resp_p      = 0.0; double resp_ap      = 0.0; double resp_n      = 0.0;
  double resp_an     = 0.0; double resp_Kpm     = 0.0; double resp_K0     = 0.0;
  double resp_p_EHE  = 0.0; double resp_ap_EHE  = 0.0; double resp_n_EHE  = 0.0;
  double resp_an_EHE = 0.0; double resp_Kpm_EHE = 0.0; double resp_K0_EHE = 0.0;
  double resp_p_HHe  = 0.0; double resp_ap_HHe  = 0.0; double resp_n_HHe  = 0.0;
  double resp_an_HHe = 0.0; double resp_Kpm_HHe = 0.0; double resp_K0_HHe = 0.0;

  //Pion responses for ECAL and HCAL
  double resp_pi     = 0.0; double resp_pi_EHE = 0.0; double resp_pi_HHe = 0.0; 

  if (    pT > 0.3) sfCh = 1.0; // Step function for charged particles
  if (energy > 2.0) sfN  = 1.0; // Step function for calo deposits 


  if (isChHadron(pdgid)) {
    if (fabs(pseudorap) > 1.3 && pT < 1.0) sfN = 0.0;
  }

  if (!doesReachEcal(pdgid, pT, Bfield, Rt)) sfN = 0.0;

  //Check if particle outside good eta region
  if (fabs(pseudorap) > 5.2) zero = true;

  unsigned int row = int(fabs(pseudorap)*10); //Param matrix row from |eta|

  //Assert there's no pi^0 (PDGID 111) or eta (221) after parton shower
  if (abs(pdgid)==111 || abs(pdgid)==221) { 
    cout << "WARNING: pi^0 (111) or eta (221) found! PDGID: " << pdgid
         << "Returning zero response" << endl; zero = true;
  }

  //Neutrino responses are zero
  if (isNeutrino(abs(pdgid))) zero = true;

  //CALCULATE RESPONSES
  for (int i_r=0; i_r<(zero?0:1); ++i_r) {
    
    //Pion EHE and HHe responses from |eta|<1.3
    frH->SetParameters(params_pi_EHE[row][0], params_pi_EHE[row][1], //EHE
                       params_pi_EHE[row][2], 1, 0, 1); 
    resp_pi_EHE = frH->Eval(energy);

    frH->SetParameters(params_pi_HHe[row][0], params_pi_HHe[row][1], //HHe
                       params_pi_HHe[row][2], 1, 0, 1); 
    resp_pi_HHe = frH->Eval(energy);
    //....................................................................

    //cat1 cat2 cat3 responsens
    frH->SetParameters(params_cat1[row][0], params_cat1[row][1],
                       params_cat1[row][2], 1, 0, 1);
    cat1 = frH->Eval(energy);

    frH->SetParameters(params_cat2[row][0], params_cat2[row][1],
                       params_cat2[row][2], 1, 0, 1);
    cat2 = frH->Eval(energy);

    frH->SetParameters(params_cat3[row][0], params_cat3[row][1],
                       params_cat3[row][2], 1, 0, 1);
    cat3 = frH->Eval(energy);

    //p,ap,n,an,Kpm,K0
    frH->SetParameters(params_pi[row][0], params_pi[row][1],
                       params_pi[row][2], 1, 0, 1);
    resp_pi = frH->Eval(energy);

    frH->SetParameters(params_p[row][0], params_p[row][1],
                       params_p[row][2], 1, 0, 1);
    resp_p = frH->Eval(energy);

    frH->SetParameters(params_ap[row][0], params_ap[row][1],
                       params_ap[row][2], 1, 0, 1);
    resp_ap = frH->Eval(energy);

    frH->SetParameters(params_n[row][0], params_n[row][1],
                       params_n[row][2], 1, 0, 1);
    resp_n = frH->Eval(energy);

    frH->SetParameters(params_an[row][0], params_an[row][1],
                       params_an[row][2], 1, 0, 1);
    resp_an = frH->Eval(energy);

    frH->SetParameters(params_Kpm[row][0], params_Kpm[row][1],
                       params_Kpm[row][2], 1, 0, 1);
    resp_Kpm = frH->Eval(energy);

    frH->SetParameters(params_K0[row][0], params_K0[row][1],
                       params_K0[row][2], 1, 0, 1);
    resp_K0 = frH->Eval(energy);

    //EHE responses
    cat1_EHE = cat1*(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    cat2_EHE = cat2*(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    cat3_EHE = cat3*(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));

    resp_p_EHE   = resp_p  *(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_ap_EHE  = resp_ap *(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_n_EHE   = resp_n  *(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_an_EHE  = resp_an *(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_Kpm_EHE = resp_Kpm*(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_K0_EHE  = resp_K0 *(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));

    //HHe responses
    cat1_HHe = cat1*(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    cat2_HHe = cat2*(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    cat3_HHe = cat3*(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));

    resp_p_HHe   = resp_p  *(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_ap_HHe  = resp_ap *(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_n_HHe   = resp_n  *(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_an_HHe  = resp_an *(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_Kpm_HHe = resp_Kpm*(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_K0_HHe  = resp_K0 *(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));

    switch (pdgid) {
      //PHOTON
      case 20 :
      case 22 :
        retMC = sfCh;
        break;

      //LEPTONS
      case  11 : //e
      case -11 : //e
        retMC = sfCh; 
        break;
      case  13 : //mu
      case -13 : //mu
        retMC = sfCh; 
        break;
      
      //cat1: antineutron, K0S, K0L, pi+, pi-
      case   211 : //pi+
      case  -211 : //pi-
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = resp_pi*sfN; 
        retA   = resp_pi*sfN;
        retEHE = resp_pi_EHE*sfN;
        retHHe = resp_pi_HHe*sfN;

        break;

      //cat2: neutron, antiproton, K+, K-
      case -3122 : //anti-Lambda
      case -3212 : //anti-Sigma^0
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = cat2*sfN; 
        retA   = cat2*sfN;
        retEHE = cat2_EHE*sfN;
        retHHe = cat2_HHe*sfN;
        break;

      //cat3: proton
      case  3122 : //Lambda
      case  3212 : //Sigma^0
      case  3322 : //Xi^0         cat=4
      case -3322 : //anti-Xi^0
      case  3312 : //Xi^-         cat=5
      case -3312 : //Xi^-         cat=4
      case  3112 : //Sigma^-      cat=4
      case -3112 : //anti-Sigma^-
      case  3222 : //Sigma^+      cat=4
      case -3222 : //anti-Sigma^+
      case  3334 : //Omega^-      cat=6
      case -3334 : //anti-Omega^- cat=5
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = cat3*sfN; 
        retA   = cat3*sfN;
        retEHE = cat3_EHE*sfN;
        retHHe = cat3_HHe*sfN;
        break;

      case  2212 : //p
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = resp_p*sfN; 
        retA   = resp_p*sfN;
        retEHE = resp_p_EHE*sfN;
        retHHe = resp_p_HHe*sfN;
        break;

      case -2212 : //ap
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = resp_ap*sfN; 
        retA   = resp_ap*sfN;
        retEHE = resp_ap_EHE*sfN;
        retHHe = resp_ap_HHe*sfN;
        break;

      case  2112 : //n
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = resp_n*sfN; 
        retA   = resp_n*sfN;
        retEHE = resp_n_EHE*sfN;
        retHHe = resp_n_HHe*sfN;
        break;

      case -2112 : //an
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = resp_an*sfN; 
        retA   = resp_an*sfN;
        retEHE = resp_an_EHE*sfN;
        retHHe = resp_an_HHe*sfN;
        break;

      case   321 : //K+
      case  -321 : //K-
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = resp_Kpm*sfN; 
        retA   = resp_Kpm*sfN;
        retEHE = resp_Kpm_EHE*sfN;
        retHHe = resp_Kpm_HHe*sfN;
        break;

      case   310 : //K^0_S
      case   130 : //K^0_L
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = resp_K0*sfN; 
        retA   = resp_K0*sfN;
        retEHE = resp_K0_EHE*sfN;
        retHHe = resp_K0_HHe*sfN;
        break;

      default : 
        zero=true;
        cout << "Unknown particle PDG: " << pdgid << endl;
        continue;	 
    }
  }

  //Set results. Check for NaN and negative results if positive demanded
  if (zero || isnan(retMC)  || retMC  <=0) retMC  = 0.0;
  if (zero || isnan(retA)   || retA   <=0) retA   = 0.0;
  if (zero || isnan(retEHE) || retEHE <=0) retEHE = 0.0;
  if (zero || isnan(retHHe) || retHHe <=0) retHHe = 0.0;

} //Response

//-----------------------------------------------------------------------------
//Check if this particle is a neutrino
//Param:	id	The particle's PDGDID
//Returns true if the particle is a nu_e, nu_mu, nu_tau
bool CMSJES::isNeutrino(int id) {
  int PDG = abs(id);
  switch (PDG) {
    case 12 : return true;
    case 14 : return true;
    case 16 : return true;
    default : return false;
  }
}
//-----------------------------------------------------------------------------
//Check if this particle is one of those for which D0 had no response
//Param:	id	The particle's PDGDID
//Returns true if the particle is a Xi, Sigma or Omega^-
bool CMSJES::isStrangeB(int id) {
  int PDG = abs(id);
  switch (PDG) {
    case 3112 : return true;
    case 3212 : return true;
    case 3222 : return true;
    case 3312 : return true;
    case 3322 : return true;
    case 3334 : return true;
    default   : return false;
  }
}

//-----------------------------------------------------------------------------
//Construct input filenames for functions using multiple CMSJES 
//objects.
//Properties are chosen based on the object doing this analysis, since the
//CMSJES objects instantiated in this function will have the same properties.
// => The user cannot e.g. accidentally enter a file savename that would
//    imply properties that are not really there.
//Saves the filenames into the zjFile string 
//belonging to this CMSJES object.

void CMSJES::InputNameConstructor() {

  //Prevent overwriting existing filenames
  if (zjFile!="") return;

  //Init strings to contain parts of the resulting filenames
  string num = "";	//#events in the sample to study

  //Generator and sample event content indicators
  if (ReadName.find("P8")!=string::npos) {zjFile="P8_";}
  else if (ReadName.find("H7")!=string::npos) {zjFile="H7_";}
  zjFile += "Zjet_";

  //#events
  if      (ReadName.find("10000000") !=string::npos) num = "10000000";
  else if (ReadName.find("5000000")  !=string::npos) num = "5000000";
  else if (ReadName.find("1000000")  !=string::npos) num = "1000000";
  else if (ReadName.find("600000")   !=string::npos) num = "600000";
  else if (ReadName.find("300000")   !=string::npos) num = "300000";
  else if (ReadName.find("100000")   !=string::npos) num = "100000";
  else if (ReadName.find("30000")    !=string::npos) num = "30000";
  else if (ReadName.find("10000")    !=string::npos) num = "10000";
  else if (ReadName.find("3000")     !=string::npos) num = "3000";
  else {
    cout << "ERROR: fitting not supported for this file" << endl;
    return;
  }
  //The resulting filenames. Suffix ".root" to be added in CMSJES constructor
  zjFile += num;
}


void CMSJES::plotEff(int gen, int Nevt) {
  //Choose filenames to open
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);
  TFile* fzj = TFile::Open(zjetFile.c_str());

  TProfile *chhEff=0;
  fzj->GetObject("chhEff",chhEff);

  //Charged hadron efficiency Profile
  TCanvas *c4     = new TCanvas("","c4",500,500);
  c4->SetLeftMargin(0.12);
  TGraph *PFeff   = new TGraph("data_and_MC_input/Efficiency/hadron_efficiency_PF.txt");
  TGraph *PFeffFr = new TGraph("data_and_MC_input/Efficiency/hadron_efficiencyPlusFakerate_PF.txt");
  PFeff->SetMarkerStyle(kFullDiamond );
  PFeff->SetMarkerColor(kBlue);
  PFeffFr->SetMarkerStyle(kFullCircle );
  PFeffFr->SetMarkerColor(kRed);

  TLegend* lgEff = new TLegend(0.3,0.2,0.75,0.40 );
  lgEff->AddEntry(chhEff, "Our efficiency", "l");
  lgEff->AddEntry(PFeffFr,"PF efficiency + fakerate", "p");
  lgEff->AddEntry(PFeff,  "PF efficiency", "p");
  lgEff->SetBorderSize(0); lgEff->SetFillStyle(0);

  PFeff->GetXaxis()->SetTitleOffset(1.0);
  PFeff->GetYaxis()->SetTitleOffset(1.3);
  PFeff->GetXaxis()->SetTitle("p_{T} (GeV)");
  PFeff->GetXaxis()->SetTitleSize(0.045);
  PFeff->GetYaxis()->SetTitle("Tracking efficiency");
  PFeff->GetYaxis()->SetTitleSize(0.045);

  TLatex *tex = new TLatex();
  tex->SetNDC();
  tex->SetTextSize(0.04);
  tex->SetTextColor(kBlack);

  TAxis *PFaxis = PFeff->GetXaxis();
  PFaxis->SetLimits(0.2,300);
  PFeff->SetTitle("");
  PFeff->GetHistogram()->SetMaximum(1.0);
  PFeff->GetHistogram()->SetMinimum(0.5);
  PFeff->GetXaxis()->SetMoreLogLabels();
  PFeff->GetXaxis()->SetNoExponent();
  PFeff->Draw("ap"); 
  lgEff->Draw("same");
  PFeffFr->Draw("samep");

  PFeff->SetMarkerSize(1.5);
  PFeffFr->SetMarkerSize(1.2);

  gStyle->SetOptStat();
  c4->SetLogx();
  chhEff->Draw("same");
  chhEff->SetLineColor(kBlack);
  chhEff->SetLineWidth(5);

  tex->DrawLatex(0.75,0.82,"|#eta|<2.5");

  string plotName = "./plots/Efficiency/trkEff_" + ReadName + ".eps";
  c4->Print(plotName.c_str());
  delete c4;
}

void CMSJES::plotJEF(int gen, int Nevt) {
  //Choose filenames to open
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);

  TFile* fzj = TFile::Open(zjetFile.c_str());

  TGraph *chf   = new TGraph("data_and_MC_input/Chf/chf.txt");
  TGraph *chnhf = new TGraph("data_and_MC_input/Chf/chnhf.txt");
  chf->SetMarkerStyle(kFullDiamond); chnhf->SetMarkerStyle(kOpenSquare);
  chf->SetMarkerColor(kBlack);       chnhf->SetMarkerColor(kBlack);
  chf->SetMarkerSize(1);             chnhf->SetMarkerSize(0.7);

  TProfile *prchf=0; TProfile *prnhf=0; TProfile *prgammaf=0; TProfile *pref=0;

  THStack* hstack_JEF;
  hstack_JEF = new THStack("hstackJEF","");


  fzj->GetObject("prchf"    ,prchf   );
  fzj->GetObject("prnhf"    ,prnhf   );
  fzj->GetObject("prgammaf" ,prgammaf);
  fzj->GetObject("pref"     ,pref    );

  TH1D* hchf    = prchf   ->ProjectionX();
  TH1D* hnhf    = prnhf   ->ProjectionX();
  TH1D* hgammaf = prgammaf->ProjectionX();
  TH1D* hef     = pref    ->ProjectionX();

  hchf   ->SetFillColor(kRed-7);   hchf->SetLineWidth(1);    hchf->SetLineColor(kBlack);
  hnhf   ->SetFillColor(kGreen-6); hnhf->SetLineWidth(1);    hnhf->SetLineColor(kBlack);
  hef    ->SetFillColor(kCyan+1);  hef->SetLineWidth(1);     hef->SetLineColor(kBlack);
  hgammaf->SetFillColor(kBlue-7);  hgammaf->SetLineWidth(1); hgammaf->SetLineColor(kBlack);


  //Canvas and related setup
  TCanvas* canv_c = new TCanvas("canv_JEF","",500,400);
  TLegend* lg     = new TLegend(0.9, 0.1, 1.0, 0.9);
  lg->SetBorderSize(0); //No frame
  lg->SetTextSize(0.028);
  lg->SetFillStyle(0);
  canv_c->SetLogx();
  canv_c->SetBottomMargin(0.13);

  hstack_JEF->Add(hchf);
  hstack_JEF->Add(hnhf);
  hstack_JEF->Add(hgammaf);
  hstack_JEF->Add(hef);

  //Legend setup
  lg->AddEntry(hef,     "e",     "f");
  lg->AddEntry(hgammaf, "#gamma","f");
  lg->AddEntry(hnhf,    "NH",    "f");
  lg->AddEntry(hchf,    "CH",    "f");

  TLegend* lgEff = new TLegend(0.15,0.3,0.4,0.4);
  lgEff->AddEntry(chnhf,"#bf{NH Run2016GH}", "p");
  lgEff->AddEntry(chf,  "#bf{CH Run2016GH}", "p");
  lgEff->SetBorderSize(0); lgEff->SetFillStyle(0);
  lgEff->SetTextSize(0.035);

  hstack_JEF->Draw("HISTO");
  hstack_JEF->GetYaxis()->SetRange(0.0,1.0);
  hstack_JEF->SetTitle("");
  hstack_JEF->GetXaxis()->SetMoreLogLabels();
  hstack_JEF->GetXaxis()->SetNoExponent();

  hstack_JEF->GetXaxis()->SetTitleOffset(1.1);
  hstack_JEF->GetYaxis()->SetTitleOffset(0.9);

  hstack_JEF->GetYaxis()->SetTitle("Jet energy fraction");
  hstack_JEF->GetYaxis()->SetTitleSize(0.045);
  hstack_JEF->GetXaxis()->SetTitle("p_{T,tag}^{reco} (GeV)");
  hstack_JEF->GetXaxis()->SetTitleSize(0.05);

  lg->Draw();
  lgEff->Draw();
  chf->Draw("samep");
  chnhf->Draw("samep");

  TLatex *tex = new TLatex();
  tex->SetNDC();
  tex->SetTextSize(0.04);
  tex->SetTextColor(kBlack);
  tex->DrawLatex(0.21,0.42,"|#eta^{probe}|<1.3");

  string plotName = "./plots/particleComposition/JEF_probe_" + ReadName + ".eps";
  canv_c->Print(plotName.c_str());
  delete canv_c;
}

//Flavour fraction
void CMSJES::plotFF(int gen, int Nevt) {
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);

  TFile* fzj = TFile::Open(zjetFile.c_str());


  TH1D* FFa  = 0;
  TH1D* FFb  = 0;
  TH1D* FFg  = 0;
  TH1D* FFud = 0;
  TH1D* FFs  = 0;
  TH1D* FFc  = 0;
  TH1D* FFlq = 0;

  fzj->GetObject("FFa" ,FFa );
  fzj->GetObject("FFb" ,FFb );
  fzj->GetObject("FFg" ,FFg );
  fzj->GetObject("FFud",FFud);
  fzj->GetObject("FFs" ,FFs);
  fzj->GetObject("FFc" ,FFc);
  fzj->GetObject("FFlq",FFlq);


  string FFstackTitle;
  FFstackTitle  = "";
  FFstackTitle += ";p_{T,tag}^{reco} (GeV)";
  FFstackTitle += ";Jet flavour fraction";

  THStack* FFstack = new THStack("", FFstackTitle.c_str());

  FFb->Divide(FFa);   FFg->Divide(FFa);    FFlq->Divide(FFa);
  FFud->Divide(FFa);  FFs->Divide(FFa);    FFc->Divide(FFa);
  FFstack->Add(FFg);  FFstack->Add(FFud);  FFstack->Add(FFs); 
  FFstack->Add(FFc);   FFstack->Add(FFb);


  string FFcanvName = ReadName + "_FF";
  TCanvas* FFcanv = new TCanvas(FFcanvName.c_str(),"",500,400);
  TLegend* FFlg = new TLegend(0.91, 0.67, 1.00, 0.90);
  FFcanv->SetLogx();
  FFlg->SetBorderSize(0); FFlg->SetFillStyle(0);
  FFcanv->SetBottomMargin(0.13);
  FFlg->AddEntry(FFb,  "b",  "f");
  FFlg->AddEntry(FFc,  "c",  "f");
  FFlg->AddEntry(FFs,  "s",  "f");
  FFlg->AddEntry(FFud, "ud", "f");
  FFlg->AddEntry(FFg,  "g",  "f");
  FFb->SetFillColor(46);  
  FFc->SetFillColor(kOrange);
  FFs->SetFillColor(kSpring+8);
  FFud->SetFillColor(33);
  FFg->SetFillColor(kGray+1);

  FFstack->Draw("HISTO");

  FFstack->GetXaxis()->SetMoreLogLabels();
  FFstack->GetXaxis()->SetNoExponent();

  FFstack->GetXaxis()->SetTitleOffset(1.1);
  FFstack->GetYaxis()->SetTitleOffset(0.9);

  FFstack->GetYaxis()->SetTitle("Jet flavour fraction");
  FFstack->GetYaxis()->SetTitleSize(0.045);
  FFstack->GetXaxis()->SetTitle("p_{T,tag}^{reco} (GeV)");
  FFstack->GetXaxis()->SetTitleSize(0.05);

  gPad->Modified();

  FFlg->Draw();
  FFcanvName = "./plots/FlavourFraction/" + FFcanvName + ".eps";
  FFcanv->Print(FFcanvName.c_str());
  delete FFcanv;
}

//-------------------------------------------------------------------------------------------
void CMSJES::plotMPF(int gen, int Nevt)
{
  //Choose filenames to open
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);

  //Initialize histograms and open ROOT files and fetch the stored objects
  //  TH1 params: name, title, #bins, #lowlimit, #highlimit
  TFile* fzj = TFile::Open(zjetFile.c_str());
  TProfile *przj_MPF=0;  TProfile *przj_MPFb=0;  TProfile *przj_MPFc=0;
  TProfile *przj_MPFs=0; TProfile *przj_MPFud=0; TProfile *przj_MPFg=0;

  fzj->GetObject("prMPF",   przj_MPF);
  fzj->GetObject("prMPFb",  przj_MPFb);
  fzj->GetObject("prMPFc",  przj_MPFc);
  fzj->GetObject("prMPFs",  przj_MPFs);
  fzj->GetObject("prMPFud", przj_MPFud);
  fzj->GetObject("prMPFg",  przj_MPFg);

  TH1D* hzj_MPF   = przj_MPF  ->ProjectionX();
  TH1D* hzj_MPFb  = przj_MPFb ->ProjectionX();
  TH1D* hzj_MPFc  = przj_MPFc ->ProjectionX();
  TH1D* hzj_MPFs  = przj_MPFs ->ProjectionX();
  TH1D* hzj_MPFud = przj_MPFud->ProjectionX();
  TH1D* hzj_MPFg  = przj_MPFg ->ProjectionX();

  //CMS MC points
  TGraphErrors* mc_zj_MPFntI_2018 = new TGraphErrors(nMC_MPFntI, zj_MC_pTp_MPFntI_2018,
                                                     zj_MC_MPFntI_2018, 0, zj_MC_MPFntI_ER_2018);
  //Canvas
  TCanvas* canv_MPF = new TCanvas("MPF","",500,400);
  canv_MPF->SetLeftMargin(0.13);
  canv_MPF->SetBottomMargin(0.13);
  canv_MPF->SetLogx();

  hzj_MPF->SetLineColor( kBlack);
  hzj_MPF->SetMarkerStyle(kFullCircle);       
  hzj_MPF->SetMarkerColor(kBlack);

  mc_zj_MPFntI_2018->SetMarkerStyle( kOpenSquare); mc_zj_MPFntI_2018->SetMarkerColor( kBlack  );


  //Legend
  TLegend* lz_MPF = new TLegend(0.62,0.70,0.89,0.89);
  lz_MPF->SetBorderSize(0);
  lz_MPF->AddEntry(hzj_MPF, "All jets"  , "p");
  lz_MPF->AddEntry(mc_zj_MPFntI_2018, "FullSim 2018ABCD", "p");

  //Title and axis setup
  hzj_MPF->SetStats(0); //Suppress stat box
  hzj_MPF->SetTitle("");
  hzj_MPF->SetAxisRange(0.86,1.0,"Y"); //Vertical axis limits

  hzj_MPF->GetXaxis()->SetMoreLogLabels();
  hzj_MPF->GetXaxis()->SetNoExponent();
  hzj_MPF->GetYaxis()->SetTitleOffset(1.1);
  hzj_MPF->GetXaxis()->SetTitleOffset(1.1);
  hzj_MPF->GetXaxis()->SetTitle("p_{T,tag}^{reco} (GeV)");
  hzj_MPF->GetYaxis()->SetTitle("R_{MPF}");
  hzj_MPF->GetYaxis()->SetTitleSize(0.05);
  hzj_MPF->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx();   gPad->SetTicky();

  //Plot
  hzj_MPF->Draw("p");
  mc_zj_MPFntI_2018->Draw("P SAME");
  lz_MPF->Draw();

  string plotName = "./plots/mpf/MPF_zmmjet_" + ReadName + ".pdf";
  canv_MPF->Print(plotName.c_str()); delete canv_MPF;

  //All flavours
  TCanvas* canv_all = new TCanvas("canv_all","",500,400);
  canv_all->SetLeftMargin(0.13); canv_all->SetBottomMargin(0.13);
  canv_all->SetLogx();

  hzj_MPF->SetMarkerStyle(5); hzj_MPF->SetLineColor(kBlack); 
  hzj_MPF->SetMarkerColor(kBlack);
  hzj_MPFb->SetMarkerStyle(kFullCircle);       hzj_MPFb->SetMarkerColor(kRed+1);
  hzj_MPFb->SetLineColor(kRed+1);
  hzj_MPFc->SetMarkerStyle(kFullTriangleDown); hzj_MPFc->SetMarkerColor(kGreen+3);
  hzj_MPFc->SetLineColor(kGreen+3);
  hzj_MPFs->SetMarkerStyle(kFullTriangleUp); hzj_MPFs->SetMarkerColor(kOrange+7);
  hzj_MPFs->SetLineColor(kOrange+7);
  hzj_MPFud->SetMarkerStyle(kFullDiamond);    hzj_MPFud->SetMarkerColor(kMagenta+2);
  hzj_MPFud->SetLineColor(kMagenta+2);
  hzj_MPFg ->SetMarkerStyle(kFullSquare);   hzj_MPFg ->SetMarkerColor(kBlue+1);
  hzj_MPFg->SetLineColor(kBlue+1);

  TLegend* lz_all = new TLegend(0.68,0.7,0.86,0.87);
  lz_all->SetBorderSize(0);
  lz_all->AddEntry(hzj_MPF,   "All", "p");
  lz_all->AddEntry(hzj_MPFb,  "b",   "p");
  lz_all->AddEntry(hzj_MPFc,  "c",   "p");
  lz_all->AddEntry(hzj_MPFs,  "s",   "p");
  lz_all->AddEntry(hzj_MPFud, "ud",  "p");
  lz_all->AddEntry(hzj_MPFg,  "g",   "p");
  lz_all->SetNColumns(2);

  TH1D* setup = new TH1D("setup","", hzj_MPF->GetXaxis()->GetNbins(),
			 hzj_MPF->GetXaxis()->GetXmin(), hzj_MPF->GetXaxis()->GetXmax());

  setup->SetStats(0); //Suppress stat box
  setup->SetTitle("");
  setup->SetAxisRange(0.84,0.98,"Y"); //Vertical axis limits
  setup->GetXaxis()->SetMoreLogLabels();
  setup->GetXaxis()->SetNoExponent();
  setup->GetYaxis()->SetTitleOffset(1.1);
  setup->GetXaxis()->SetTitleOffset(1.1);
  setup->GetXaxis()->SetTitle("p_{T,tag}^{reco} (GeV)");
  setup->GetYaxis()->SetTitle("R_{MPF}");
  setup->GetYaxis()->SetTitleSize(0.05);
  setup->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup->Draw();
  hzj_MPF->Draw("SAMEP");
  hzj_MPFb->Draw("SAMEP");
  hzj_MPFc->Draw("SAMEP");
  hzj_MPFs->Draw("SAMEP");
  hzj_MPFud->Draw("SAMEP");
  hzj_MPFg->Draw("SAMEP");
  lz_all->Draw("SAMEP");

  plotName = "./plots/mpf/MPF_zmmjet_all_" + ReadName + ".pdf";
  canv_all->Print(plotName.c_str());
  delete canv_all;
}

//-------------------------------------------------------------------------------------------
void CMSJES::plotRjet(int gen, int Nevt)
{
  //Choose filenames to open
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);

  //Initialize histograms and open ROOT files and fetch the stored objects
  TFile* fzj = TFile::Open(zjetFile.c_str());
  TProfile *przj_Rjet=0; TProfile *przj_Rjetb=0; TProfile *przj_Rjetg=0; TProfile *przj_Rjetlq=0;
  TProfile *przj_Rjetud=0; TProfile *przj_Rjets=0; TProfile *przj_Rjetc=0;

  //Create Histograms
  fzj->GetObject("prRjet"   ,przj_Rjet  );
  fzj->GetObject("prRjetb"  ,przj_Rjetb );
  fzj->GetObject("prRjetg"  ,przj_Rjetg );
  fzj->GetObject("prRjetlq" ,przj_Rjetlq);
  fzj->GetObject("prRjetud" ,przj_Rjetud);
  fzj->GetObject("prRjets"  ,przj_Rjets);
  fzj->GetObject("prRjetc"  ,przj_Rjetc);
  TH1D* hzj_Rjet   = przj_Rjet  ->ProjectionX();
  TH1D* hzj_Rjetb  = przj_Rjetb ->ProjectionX();
  TH1D* hzj_Rjetg  = przj_Rjetg ->ProjectionX();
  TH1D* hzj_Rjetlq = przj_Rjetlq->ProjectionX();
  TH1D* hzj_Rjetud = przj_Rjetud->ProjectionX();
  TH1D* hzj_Rjets  = przj_Rjets ->ProjectionX();
  TH1D* hzj_Rjetc  = przj_Rjetc ->ProjectionX();

  //////////////////////////// GLUON DIFFERENCE ///////////////////////////////
  TH1D* h_diffbg  = (TH1D*)hzj_Rjetb->Clone("h_diffbg");
  TH1D* h_diffudg = (TH1D*)hzj_Rjetud->Clone("h_diffudg");
  TH1D* h_diffsg  = (TH1D*)hzj_Rjets->Clone("h_diffsg");
  TH1D* h_diffcg  = (TH1D*)hzj_Rjetc->Clone("h_diffcg");

  h_diffbg->Add(hzj_Rjetg,-1);  //b-response  - g-response
  h_diffudg->Add(hzj_Rjetg,-1); //ud-response - g-response
  h_diffsg->Add(hzj_Rjetg,-1);  //s-response  - g-response
  h_diffcg->Add(hzj_Rjetg,-1);  //c-response  - g-response

  TGraph *diff_b  = new TGraph("data_and_MC_input/Response/diffToGluon_b.txt" );
  TGraph *diff_s  = new TGraph("data_and_MC_input/Response/diffToGluon_s.txt" );
  TGraph *diff_c  = new TGraph("data_and_MC_input/Response/diffToGluon_c.txt" );
  TGraph *diff_ud = new TGraph("data_and_MC_input/Response/diffToGluon_ud.txt");
  diff_b ->SetMarkerStyle(kOpenCircle);       diff_b ->SetMarkerColor(kRed+1);
  diff_ud->SetMarkerStyle(kOpenDiamond);      diff_ud->SetMarkerColor(kMagenta+2);
  diff_s ->SetMarkerStyle(kOpenTriangleUp);   diff_s ->SetMarkerColor(kOrange+1);
  diff_c ->SetMarkerStyle(kOpenTriangleDown); diff_c ->SetMarkerColor(kGreen+1);


  h_diffbg ->SetMarkerStyle(kFullCircle);       h_diffbg ->SetMarkerColor(kRed+1);
  h_diffudg->SetMarkerStyle(kFullDiamond);      h_diffudg->SetMarkerColor(kMagenta+2);
  h_diffsg ->SetMarkerStyle(kFullTriangleUp);   h_diffsg ->SetMarkerColor(kOrange+1);
  h_diffcg ->SetMarkerStyle(kFullTriangleDown); h_diffcg ->SetMarkerColor(kGreen+1 );
  h_diffbg ->SetLineColor(kRed+1);        
  h_diffudg->SetLineColor(kMagenta+2);
  h_diffsg ->SetLineColor(kOrange+1);     
  h_diffcg ->SetLineColor(kGreen+1 );


  TCanvas* canv_diffg = new TCanvas("canv_diffg","",500,400);
  canv_diffg->SetLeftMargin(0.15);
  canv_diffg->SetBottomMargin(0.13);

  //Legend
  TLegend* lz_diffg = new TLegend(0.62,0.60,0.89,0.89);
  lz_diffg->SetBorderSize(0);
  lz_diffg->AddEntry(h_diffbg,           "b");
  lz_diffg->AddEntry(h_diffcg,           "c");
  lz_diffg->AddEntry(h_diffsg,           "s");
  lz_diffg->AddEntry(h_diffudg,         "ud");
  lz_diffg->AddEntry(diff_b,  "b (PF)",  "p");
  lz_diffg->AddEntry(diff_c,  "c (PF)",  "p");
  lz_diffg->AddEntry(diff_s,  "s (PF)",  "p");
  lz_diffg->AddEntry(diff_ud, "ud (PF)", "p");



  h_diffbg->GetYaxis()->SetTitle("Difference to gluon response");
  h_diffbg->GetYaxis()->SetTitleSize(0.042);
  h_diffbg->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  h_diffbg->GetXaxis()->SetTitleSize(0.05);

  h_diffbg->GetYaxis()->SetTitleOffset(1.5);
  h_diffbg->GetXaxis()->SetTitleOffset(1.1);
  h_diffbg->GetXaxis()->SetMoreLogLabels();
  h_diffbg->GetXaxis()->SetNoExponent();




  h_diffbg->SetStats(0); h_diffbg->SetTitle("");
  h_diffbg->SetAxisRange(0.0,0.042,"Y"); //Vertical axis limits

  h_diffbg->Draw("P");
  diff_ud->Draw("SAMEP");
  diff_b->Draw("SAMEP");
  diff_s->Draw("SAMEP");
  diff_c->Draw("SAMEP");
  h_diffudg->Draw("SAMEP");
  h_diffsg->Draw("SAMEP");
  h_diffcg->Draw("SAMEP");
  lz_diffg->Draw("SAMEP");
  canv_diffg->SetLogx();

  string plotName = "./plots/Rjet/gluonDiff_" + ReadName + ".eps";
  canv_diffg->Print(plotName.c_str());
  delete canv_diffg;


  //Canvas
  TCanvas* canv_Rjet = new TCanvas("canv_Rjet","",500,400);
  canv_Rjet->SetLeftMargin(0.15);
  canv_Rjet->SetBottomMargin(0.13);

  hzj_Rjet  ->SetMarkerStyle(kOpenCircle);      hzj_Rjet  ->SetMarkerColor(kBlack);
  hzj_Rjetb ->SetMarkerStyle(kFullCircle);      hzj_Rjetb ->SetMarkerColor(kRed+1);
  hzj_Rjetg ->SetMarkerStyle(kFullSquare);      hzj_Rjetg ->SetMarkerColor(kBlue+1);
  hzj_Rjetud->SetMarkerStyle(kFullDiamond);     hzj_Rjetud->SetMarkerColor(kMagenta+2);
  hzj_Rjets->SetMarkerStyle(kFullTriangleUp);   hzj_Rjets->SetMarkerColor(kOrange+1);
  hzj_Rjetc->SetMarkerStyle(kFullTriangleDown); hzj_Rjetc->SetMarkerColor(kGreen+2);

  hzj_Rjet  ->SetLineColor(kBlack);              
  hzj_Rjetb ->SetLineColor(kRed+1);
  hzj_Rjetg ->SetLineColor(kBlue+1);             
  hzj_Rjetud->SetLineColor(kMagenta+2);
  hzj_Rjets ->SetLineColor(kOrange+1);            
  hzj_Rjetc ->SetLineColor(kGreen+2);

  //Legend
  TLegend* lz_Rjet = new TLegend(0.55,0.17,0.75,0.42);
  lz_Rjet->SetBorderSize(0);
  lz_Rjet->AddEntry(hzj_Rjet,   "All",   "p");
  lz_Rjet->AddEntry(hzj_Rjetb,  "b",     "p");
  lz_Rjet->AddEntry(hzj_Rjetc,  "c",     "p");
  lz_Rjet->AddEntry(hzj_Rjets,  "s",     "p");
  lz_Rjet->AddEntry(hzj_Rjetud, "ud",    "p");
  lz_Rjet->AddEntry(hzj_Rjetg,  "gluon", "p");

  //Title and axis setup
  hzj_Rjet->SetStats(0); //Suppress stat box
  hzj_Rjet->SetTitle("");
  hzj_Rjet->SetAxisRange(0.90,0.95,"Y");
  hzj_Rjet->SetAxisRange(31.75,1258,"X"); 

  hzj_Rjet->GetXaxis()->SetMoreLogLabels();
  hzj_Rjet->GetXaxis()->SetNoExponent();
  canv_Rjet->SetLogx();
  hzj_Rjet->GetYaxis()->SetTitleOffset(1.1);
  hzj_Rjet->GetXaxis()->SetTitleOffset(1.1);

  hzj_Rjet->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  hzj_Rjet->GetXaxis()->SetTitleSize(0.05);
  hzj_Rjet->GetYaxis()->SetTitle("R_{jet}");
  hzj_Rjet->GetYaxis()->SetTitleSize(0.05);


  gPad->SetTickx(); gPad->SetTicky();

  //Plot
  hzj_Rjet->Draw("P");
  hzj_Rjetb->Draw("SAMEP");
  hzj_Rjetg->Draw("SAMEP");
  hzj_Rjetud->Draw("SAMEP");
  hzj_Rjets->Draw("SAMEP");
  hzj_Rjetc->Draw("SAMEP");
  lz_Rjet->Draw("SAMEP");

  plotName = "./plots/Rjet/Rjet_" + ReadName + ".eps";
  canv_Rjet->Print(plotName.c_str()); delete canv_Rjet;
}

void CMSJES::plotF(int gen, int Nevt)
{
  //Choose filenames to open
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);

  //Initialize histograms and open ROOT files and fetch the stored objects
  TFile* fzj = TFile::Open(zjetFile.c_str());
  TProfile *przj_F=0; 
  TProfile *przj_Fb=0; 
  TProfile *przj_Fg=0; 
  TProfile *przj_Fud=0;
  TProfile *przj_Fs=0;
  TProfile *przj_Fc=0;

  //Create F Histograms
  fzj->GetObject("prRjet"   ,przj_F  );
  fzj->GetObject("prRjetb"  ,przj_Fb );
  fzj->GetObject("prRjetg"  ,przj_Fg );
  fzj->GetObject("prRjetud" ,przj_Fud);
  fzj->GetObject("prRjets"  ,przj_Fs);
  fzj->GetObject("prRjetc"  ,przj_Fc);
  TH1D* hzj_F   = przj_F->ProjectionX();
  TH1D* hzj_Fb  = przj_Fb->ProjectionX();
  TH1D* hzj_Fg  = przj_Fg->ProjectionX();
  TH1D* hzj_Fud = przj_Fud->ProjectionX();
  TH1D* hzj_Fs  = przj_Fs->ProjectionX();
  TH1D* hzj_Fc  = przj_Fc->ProjectionX();

  hzj_Fb->Divide(hzj_F);
  hzj_Fg->Divide(hzj_F);
  hzj_Fud->Divide(hzj_F);
  hzj_Fs->Divide(hzj_F);
  hzj_Fc->Divide(hzj_F);

  //Canvas
  TCanvas* canv_F = new TCanvas("canv_F","",500,400);
  canv_F->SetLeftMargin(0.15);
  canv_F->SetBottomMargin(0.13);

  hzj_Fb ->SetMarkerStyle(kFullCircle);      hzj_Fb ->SetMarkerColor(kRed+1);
  hzj_Fg ->SetMarkerStyle(kFullSquare);      hzj_Fg ->SetMarkerColor(kBlue+1);
  hzj_Fud->SetMarkerStyle(kFullDiamond);     hzj_Fud->SetMarkerColor(kMagenta+2);
  hzj_Fs->SetMarkerStyle(kFullTriangleUp);   hzj_Fs->SetMarkerColor(kOrange+1);
  hzj_Fc->SetMarkerStyle(kFullTriangleDown); hzj_Fc->SetMarkerColor(kGreen+2);

  hzj_Fb ->SetLineColor(kRed+1);              
  hzj_Fg ->SetLineColor(kBlue+1);
  hzj_Fud->SetLineColor(kMagenta+2);
  hzj_Fs->SetLineColor(kOrange+1);
  hzj_Fc->SetLineColor(kGreen+2);

  double markersize = 0.8;
  hzj_Fb ->SetMarkerSize(markersize);              
  hzj_Fg ->SetMarkerSize(markersize);
  hzj_Fud->SetMarkerSize(markersize);
  hzj_Fs->SetMarkerSize(markersize);
  hzj_Fc->SetMarkerSize(markersize);

  //Legend
  TLegend* lz_F = new TLegend(0.62,0.65,0.89,0.88);
  lz_F->SetBorderSize(0);
  lz_F->AddEntry(hzj_Fb,  "b",     "p");
  lz_F->AddEntry(hzj_Fc,  "c",     "p");
  lz_F->AddEntry(hzj_Fs,  "s",     "p");
  lz_F->AddEntry(hzj_Fud, "ud",    "p");
  lz_F->AddEntry(hzj_Fg,  "gluon", "p");

  //Title and axis setup
  hzj_Fb->SetStats(0); //Suppress stat box
  hzj_Fb->SetTitle("");
  hzj_Fb->SetAxisRange(0.98,1.02,"Y"); //Vertical axis limits

  hzj_Fb->GetYaxis()->SetTitleOffset(1.1);
  hzj_Fb->GetXaxis()->SetTitleOffset(1.1);
  hzj_Fb->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  hzj_Fb->GetXaxis()->SetTitleSize(0.05);
  hzj_Fb->GetYaxis()->SetTitle("F");
  hzj_Fb->GetYaxis()->SetTitleSize(0.05);

  hzj_Fb->GetXaxis()->SetMoreLogLabels();
  hzj_Fb->GetXaxis()->SetNoExponent();
  canv_F->SetLogx();
  gPad->SetTickx(); gPad->SetTicky();

  TLine *line = new TLine(31.75,1,1258.25,1); 

  //Plot
  hzj_Fb->Draw("P");
  line->Draw("SAME");
  hzj_Fg->Draw("SAMEP");
  hzj_Fud->Draw("SAMEP");
  hzj_Fs->Draw("SAMEP");
  hzj_Fc->Draw("SAMEP");
  lz_F->Draw("SAMEP");

  //Save plot
  string plotName = "./plots/F/F_" + ReadName + ".eps";
  canv_F->Print(plotName.c_str()); delete canv_F;
}

void CMSJES::plotVariants(int gen, int Nevt)
{

  string outname = "./output_ROOT_files/varPlots_5000000"; //Output file
  outname += ".root";
  TFile *foutvar = new TFile(outname.c_str(),"RECREATE");

  // SPR +-3% JEC paper
  TGraph *JEC_SPRp3      = new TGraph("data_and_MC_input/Response/JEC_PFJet_SPRplus3.txt" );
  TGraph *JEC_SPRm3      = new TGraph("data_and_MC_input/Response/JEC_PFJet_SPRminus3.txt" );
  TGraph *JEC_calo_SPRp3 = new TGraph("data_and_MC_input/Response/JEC_Calo_SPRplus3.txt" );
  TGraph *JEC_calo_SPRm3 = new TGraph("data_and_MC_input/Response/JEC_Calo_SPRminus3.txt" );

  JEC_SPRp3->SetMarkerStyle(kOpenCircle);            JEC_SPRp3->SetMarkerColor(kRed);
  JEC_SPRm3->SetMarkerStyle(kOpenSquare);            JEC_SPRm3->SetMarkerColor(kBlue+1);
  JEC_calo_SPRp3->SetMarkerStyle(kOpenTriangleUp);   JEC_calo_SPRp3->SetMarkerColor(kOrange+1);
  JEC_calo_SPRm3->SetMarkerStyle(kOpenTriangleDown); JEC_calo_SPRm3->SetMarkerColor(kGreen+2);

  double markersize = 0.8;
  JEC_SPRp3->SetMarkerSize(markersize);
  JEC_SPRm3->SetMarkerSize(markersize);
  JEC_calo_SPRp3->SetMarkerSize(markersize);
  JEC_calo_SPRm3->SetMarkerSize(markersize);

  TFile* fzj     = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_3000.root");
  TFile* fzj_Cp3 = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_3000_varCp3.root");
  TFile* fzj_Cm3 = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_3000_varCm3.root");

  TProfile *pr_Rjet = 0;
  TProfile *pr_Rjet_b = 0; 
  TProfile *pr_Rjet_Cp3 = 0;
  TProfile *pr_Rjet_Cp3_b = 0;
  TProfile *pr_Rjet_Cm3 = 0;
  TProfile *pr_Rjet_Cm3_b = 0;
  TProfile *pr_Rjet_calo = 0;
  TProfile *pr_Rjet_calo_Cp3 = 0;
  TProfile *pr_Rjet_calo_Cm3 = 0;

  //Create Histograms
  fzj    ->GetObject("prRjet", pr_Rjet);
  fzj    ->GetObject("prRjetb",pr_Rjet_b);
  fzj_Cp3->GetObject("prRjet", pr_Rjet_Cp3);
  fzj_Cp3->GetObject("prRjetb", pr_Rjet_Cp3_b);
  fzj_Cm3->GetObject("prRjet", pr_Rjet_Cm3);
  fzj_Cm3->GetObject("prRjetb", pr_Rjet_Cm3_b);

  fzj    ->GetObject("prRjet_calo",pr_Rjet_calo);
  fzj_Cp3->GetObject("prRjet_calo",pr_Rjet_calo_Cp3);
  fzj_Cm3->GetObject("prRjet_calo",pr_Rjet_calo_Cm3);

  TH1D* h_Rjet       = pr_Rjet->ProjectionX();
  TH1D* h_Rjet_b     = pr_Rjet_b->ProjectionX();
  TH1D* h_Rjet_Cp3   = pr_Rjet_Cp3->ProjectionX();
  TH1D* h_Rjet_Cp3_b = pr_Rjet_Cp3_b->ProjectionX();
  TH1D* h_Rjet_Cm3   = pr_Rjet_Cm3->ProjectionX();
  TH1D* h_Rjet_Cm3_b = pr_Rjet_Cm3_b->ProjectionX();

  TH1D* h_Rjet_calo     = pr_Rjet_calo->ProjectionX();
  TH1D* h_Rjet_calo_Cp3 = pr_Rjet_calo_Cp3->ProjectionX();
  TH1D* h_Rjet_calo_Cm3 = pr_Rjet_calo_Cm3->ProjectionX();

  h_Rjet_Cp3->Divide(h_Rjet);
  h_Rjet_Cp3_b->Divide(h_Rjet_b);
  h_Rjet_Cm3->Divide(h_Rjet);
  h_Rjet_Cm3_b->Divide(h_Rjet_b);
  h_Rjet_calo_Cp3->Divide(h_Rjet_calo);
  h_Rjet_calo_Cm3->Divide(h_Rjet_calo);

  //Canvas
  TCanvas* canv_var = new TCanvas("canv_var","",500,400);
  canv_var->SetLeftMargin(0.15);
  canv_var->SetBottomMargin(0.13);

  h_Rjet_Cp3->SetMarkerStyle(kFullCircle); h_Rjet_Cp3->SetMarkerColor(kRed);
  h_Rjet_Cm3->SetMarkerStyle(kFullSquare); h_Rjet_Cm3->SetMarkerColor(kBlue+1);
  h_Rjet_Cp3->SetLineColor(kRed);          h_Rjet_Cm3->SetLineColor(kBlue+1); 

  h_Rjet_calo_Cp3->SetMarkerStyle(kFullTriangleUp);   h_Rjet_calo_Cp3->SetMarkerColor(kOrange+1);
  h_Rjet_calo_Cm3->SetMarkerStyle(kFullTriangleDown); h_Rjet_calo_Cm3->SetMarkerColor(kGreen+2);
  h_Rjet_calo_Cp3->SetLineColor(kOrange+1);           h_Rjet_calo_Cm3->SetLineColor(kGreen+2); 

  h_Rjet_Cp3->SetMarkerSize(markersize);        h_Rjet_Cm3->SetMarkerSize(markersize);
  h_Rjet_calo_Cp3->SetMarkerSize(markersize);   h_Rjet_calo_Cm3->SetMarkerSize(markersize);

  //Legend
  TLegend* lz_Rjet = new TLegend(0.18,0.67,0.88,0.89);
  lz_Rjet->SetNColumns(2);
  lz_Rjet->SetBorderSize(0);
  lz_Rjet->AddEntry(h_Rjet_Cp3,      "Reco jet C+3% (Our)",   "p");
  lz_Rjet->AddEntry(h_Rjet_calo_Cp3, "Calo jet C+3% (Our)",   "p");
  lz_Rjet->AddEntry(h_Rjet_Cm3,      "Reco jet C-3% (Our)",   "p");
  lz_Rjet->AddEntry(h_Rjet_calo_Cm3, "Calo jet C-3% (Our)",   "p");
  lz_Rjet->AddEntry(JEC_SPRp3,       "Reco jet C+3% (FastSim)", "p");
  lz_Rjet->AddEntry(JEC_calo_SPRp3,  "Calo jet C+3% (FastSim)", "p");
  lz_Rjet->AddEntry(JEC_SPRm3,       "Reco jet C-3% (FastSim)", "p");  
  lz_Rjet->AddEntry(JEC_calo_SPRm3,  "Calo jet C-3% (FastSim)", "p"); 


  TH1D* setup = new TH1D("setup","", h_Rjet_Cp3->GetXaxis()->GetNbins(),
			 h_Rjet_Cp3->GetXaxis()->GetXmin(), h_Rjet_Cp3->GetXaxis()->GetXmax());

  //Title and axis setup
  setup->SetStats(0); //Suppress stat box
  setup->SetTitle("");
  setup->SetAxisRange(0.97,1.05,"Y"); //Vertical axis limits
  setup->GetXaxis()->SetMoreLogLabels();
  setup->GetXaxis()->SetNoExponent();
  canv_var->SetLogx();
  setup->GetYaxis()->SetTitleOffset(1.1);
  setup->GetXaxis()->SetTitleOffset(1.1);
  setup->GetYaxis()->SetTitle("F^{var}");
  setup->GetYaxis()->SetTitleSize(0.05);
  setup->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  //Savefile name setup
  string savename = "./plots/varPlots/varSPR";
  savename+=".eps";

  //Plot
  setup->Draw();
  JEC_calo_SPRp3->Draw("SAMEP");
  JEC_SPRp3->Draw("SAMEP");
  JEC_SPRm3->Draw("SAMEP");
  JEC_calo_SPRm3->Draw("SAMEP");
  h_Rjet_calo_Cp3->Draw("SAMEP");
  h_Rjet_calo_Cm3->Draw("SAMEP");
  h_Rjet_Cm3->Draw("SAMEP");
  h_Rjet_Cp3->Draw("SAMEP");
  lz_Rjet->Draw("SAMEP");

  //Save plot
  canv_var->Print(savename.c_str());

  delete canv_var;

  foutvar->Write();
  foutvar->Close();

}

//-----------------------------------------------------------------------------
//User interface to choose which files to open in plotting functions
//Params:	nameAdd		Additions to the standard CMSJES output names
//		djstr, gjstr	Set dijet & gammajet all flavor filenames here
//		gen,alg,rad,...
//		ct,XS	Preset values if user interface omitted
//N.B. only the interesting cases are enabled in this function ATM
void CMSJES::plotQuery(string& nameAdd, string& zjstr, int& gen, int& Nevt)
{
  //Init
  string respStr = "./output_ROOT_files/CMSJES_";
  string root = ".root";

  /* User interface */
  //Choose generator
  cout << "(1) H7 (2) P8" << endl;
  while (gen<1 || gen>2) cin >> gen;
  respStr += (gen==2 ? "P8_": (gen==1 ? "H7_": ""));

  //Set #events
  string num;
  cout << "#Events (0) 3k (1) 10k (2) 30k (3) 100k (4) 600k (5) 1M (6) 5M (7) 10M" << endl;
  while (Nevt<0 || Nevt>7) cin >> Nevt;
  if      (Nevt==0) num = "3000";
  else if (Nevt==1) num = "10000";
  else if (Nevt==2) num = "30000";
  else if (Nevt==3) num = "100000";
  else if (Nevt==4) num = "600000";
  else if (Nevt==5) num = "1000000";
  else if (Nevt==6) num = "5000000";
  else if (Nevt==7) num = "10000000";

  //Construct all-flavor filenames
  zjstr = respStr + "Zjet_" + num + root;

  //Additions in filename
  nameAdd = num;
} //plotQuery


//Phi value where particle hits the calorimeter
double CMSJES::trackDeltaPhi(int pdgid, double phi, double pT, double Rt, double Bfield) {
  
  double Rp = pT/(0.3*Bfield);
  double dPhi;
  double newPhi;

  //What is the correct sign here?
  dPhi = Charge(pdgid)*(TMath::Pi()/2 - TMath::ACos(Rt/(2*Rp)));

  newPhi = phi + dPhi;

  if ( newPhi > TMath::Pi() )       newPhi -= TMath::Pi();
  else if ( newPhi < -TMath::Pi() ) newPhi += TMath::Pi();

  return newPhi;
}


int CMSJES::Charge(int pdgid) {
  int charge;

  switch (pdgid) {
    //Neutral
    case    12 ://e-neutrino
    case   -12 :
    case    14 ://mu-neutrino
    case   -14 :
    case    16 ://tau-neutrino
    case   -16 :
    case    20 ://photon
    case    22 ://photon from pi0
    case   130 ://K_L0
    case   310 ://K_S0
    case  3122 ://Lambda0
    case -3122 ://anti-Lambda0
    case  2112 ://n
    case -2112 ://anti-n
    case  3212 ://Sigma0
    case -3212 ://anti-Sigma0
    case  3322 ://Xi0
    case -3322 ://anti-Xi0
      charge = 0;
      break;

    //Positive charge
    case   -11 ://e+
    case   -13 ://mu+
    case   211 ://pi+
    case   321 ://K+
    case  2212 ://p+
    case -3112 ://anti-Sigma+
    case  3222 ://Sigma+
    case -3312 ://anti-Xi+
    case -3334 ://anti-Omega+
      charge = 1;
      break;  
  
    //Negative charge
    case    11 ://e-
    case    13 ://mu-
    case  -211 ://pi-
    case  -321 ://K-
    case -2212 ://anti-p
    case  3112 ://Sigma-
    case -3222 ://anti-Sigma-
    case  3312 ://Xi-
    case  3334 ://Omega-
      charge = -1;
      break;
    default : 
         cout << "Unknown particle: " << pdgid << endl;
  }
  return charge;
}

bool CMSJES::isChHadron(int pdgid) {
  bool isChH;
  int PDG = fabs(pdgid);

  switch (PDG) {
    case  211 ://pi+-
    case  321 ://K+-
    case  2212 ://p
    case  3112 ://Sigma-
    case  3222 ://Sigma+
    case  3312 ://Xi+-
    case  3334 ://Omega+-
      isChH = 1;
      break;
    default : 
      isChH = 0;
  }
  return isChH;
}

bool CMSJES::doesReachEcal(int pdgid, double pT, double B, double Rt) {
  double Rp = pT/(0.3*B);

  if (!fabs(Charge(pdgid))) return true;

  if (Rp > 0.5*Rt) return true;
  else return false;
}








