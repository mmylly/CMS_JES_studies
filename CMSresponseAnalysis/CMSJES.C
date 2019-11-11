#define CMSJES_cxx
#include "CMSJES.h"

//Created using ROOT TMakeClass
void CMSJES::Loop()
{
  //Variant flags
  bool varCp3, varCm3;
  varCp3 = 0;
  varCm3 = 0;

  if (varCp3 && varCm3) {cout << "Both C-variants enabled!" << endl; return;}

  //Initialize rng's
  //srand(time(0));
  srand(1);
  TRandom3* gRandom = new TRandom3();
  //gRandom->SetSeed(0);
  gRandom->SetSeed(1);

  TFile* coldCellsFile = new TFile("./data_and_MC_input/coldCells/coldjetsmc-18runA.root");
  TH2D*  coldCells = (TH2D*)coldCellsFile->Get("h2hole");

  //greedy_pythia modes: 0=generic, 1=dijet, 2=gammajet, 3=Zjet, 4=ttbarjet
  int studyMode = -1;
  if (ReadName.find("Zjet")!=string::npos) {
    studyMode = 3;
    cout << "File is for Z+jet sample, with stydy mode: "<< studyMode << endl;
  } else {
    cout << "Error: File not for Z+jet sample! Exiting." << endl;
    return;
  }

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  string outname = "./output_ROOT_files/CMSJES_" + ReadName; //Output file
  if (varCp3) outname += "_varCp3";
  if (varCm3) outname += "_varCm3";
  outname += ".root";

  TFile *fout = new TFile(outname.c_str(),"RECREATE");

  int const nbins = 11;
  int const nbinsMPF = 15;

  //Check values
  const double binsx[nbins] = {31.75, 41.0, 50.5, 63.5, 82.75, 105.25, 132.5, 173.5, 228.5,
                               299.5, 380.75}; 
  const double binsxMPF[nbinsMPF] = {31.75, 41.0, 50.5, 63.5, 83.0, 105.25, 132.5, 173.25,
                                    228.25, 300.0, 391.25, 503.75, 681.75, 951.5, 1258.25};

  string pTpTitle="c#tau=1 cm"; string MPFTitle="c#tau=1 cm";
  
  pTpTitle += ";#font[132]{#font[12]{p}_{T,tag}^{MC} [GeV]}; p_{T}^{probe}/p_{T}^{tag}";
  TProfile* prpTbal = new TProfile("prpTbal", pTpTitle.c_str(), nbinsMPF-1, binsxMPF);

  //Jet flavour dependent MPF responses *b = b-jets, *g = g-jets, *lq=(u,d,s,c)-jets
  MPFTitle += ";p_{T,tag}^{MC} [GeV];R_{MPF}";
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
  TH1D* FFb = new TH1D("FFb",  "",nbins-1,binsx);
  TH1D* FFg = new TH1D("FFg",  "",nbins-1,binsx);
  TH1D* FFlq= new TH1D("FFlq", "",nbins-1,binsx);
  TH1D* FFa = new TH1D("FFa",  "",nbins-1,binsx); //All
  string FFstackTitle;
  FFstackTitle  = "#font[132]{Run CMS}";
  FFstackTitle += ";#font[12]{p}_{T,tag}^{MC}#font[132]{[GeV]}";
  FFstackTitle += ";#font[132]{Jet flavour fraction}";
  THStack* FFstack = new THStack("", FFstackTitle.c_str());

  //Response function R_h (h for hadron)
  TF1 *fr_h = new TF1("frh","[0]*(1-[1]*pow(x,[2]-1))",0,4000);

  // C-parameter variation +- 3%
  if (varCp3) { fr_h = new TF1("frh","1.03*[0]*(1-[1]*pow(x,[2]-1))",0,4000);}
  if (varCm3) { fr_h = new TF1("frh","0.97*[0]*(1-[1]*pow(x,[2]-1))",0,4000);}

  fr_h->Print();

  //Used in HCAL calibration, pion response parameters now in |eta|<1.3
  TF1 *fr_hcal = new TF1("fr_hcal","x*1.123*(1-1.345*pow(x,0.421-1))",0,4000);
  TF1 *fr_ecal = new TF1("fr_ecal","x*1.076*(1-1.403*pow(x,0.646-1))",0,4000);

  //Higher resolution
  int reso = 10000; 
  fr_h->SetNpx(reso); fr_hcal->SetNpx(reso); fr_ecal->SetNpx(reso);

  // INITIALIZATIONS
  Long64_t nbytes = 0, nb = 0;
  TLorentzVector p4;            //4-vector temp.
  TLorentzVector p4_2;
  TLorentzVector p4_calo;       //4-vector temp.

  //Calorimeter and track 4-vectors
  TLorentzVector p4_chc;
  TLorentzVector p4_cht;

  TLorentzVector tag;           //Parton level tag object 4-vector
  TLorentzVector tag_r;	        //With muon smearing

  TLorentzVector probe;         //Generator level probe 4-vector
  TLorentzVector probe_g;       //Generator level without neutrinos  
  TLorentzVector probe_spr;     //SPR reconstructed probe 4-vector
  TLorentzVector probe_pf;      //PF reconstructed probe 4-vector
  TLorentzVector probe_calo;    //Calo reconstructed probe 4-vector


  //Partial derivative values for a hadron
  unsigned int i_tag = 0;       //Stepper to find tag object index
  unsigned int i_probe = 0;     //       -||-     probe jet index
  double pTp  = 0;              //tag object pT
  double eta_muon  = 2.3;       //Max |eta| for a single muon in Z+jet      
  double eta_tag_z = 2.5;       //Max |eta| for mumu tag object
  double eta_probe = 1.3;       //Max |eta| for probe jets
  vector<TLorentzVector> jets_g;//Gen lvl jet 4-vectors
  vector<TLorentzVector> jets_r;//MC reco'd jet 4-vectors
  int PDG = 1;                  //Shorthand, to store a particle's PDGID
  int JI = 0;                   //Shorthand, particle's jet index
  double phiMin = 2.8;          //Minimum azimuth angle between tag and probe
  double pTmin_probe = 15;      //Minimum probe jet p_T (GeV)
  double pTmin_muon  = 15;      //Minimum single tag muon pT (GeV)
  double pTmin_tag_z = 15;      //Minimum tag muon pair pT (GeV)   
  double resp   = 1.0;	        //SPR value                (dummy init)
  double respH  = 0.0;	        //Calorimeter response for hadrons
  double respEHE  = 0.0;        //Calorimeter response for hadrons
  double respHHe  = 0.0;        //Calorimeter response for hadrons
  double R_MPF_r = 0;           //MC-reco'd MPF response
  unsigned long njets;          //#jets in the event, for resizing vectors
  TLorentzVector MET_r;         //Reconstructed MET four vector

  //CMS detector related parameters
  double Bfield = 4.0; //Tesla
  double Rt     = 1.2; //Tracker radius

  //MET calculation related variables
  double cellPhi; double cellEta; double cellE; double cellPt; double delta; double cellSigma;
  double eff;  double effConst; double newPhi; double calReso; double trkReso; bool trkFail; 
  double cellE_calo; double cellPt_calo;
  int probeFlav;
  
  //Histograms to contain the particles in probe jet
  //[0]=gen lvl, [1]=reco lvl using r^MC, [2]=reco lvl using R^data 
  TH1F* h_e[3];      TH1F* h_mu[3];     TH1F* h_gamma[3];  TH1F* h_gpi0[3];
  TH1F* h_K[3];      TH1F* h_K0S[3];    TH1F* h_K0L[3];    TH1F* h_L[3];
  TH1F* h_p[3];      TH1F* h_n[3];      TH1F* h_nu[3];     TH1F* h_pi[3];
  TH1F* h_Xi0[3];    TH1F* h_Xim[3];    TH1F* h_Om[3];     TH1F* h_Sig0[3];
  TH1F* h_Sigm[3];   TH1F* h_Sigp[3];   
  TH1F* h_other[3];  TH1F* h_all[3];	//For normalizing histo stacks
  TH1F* h_ptr[3];	//To contain one of the above pointer sets at a time
  vector<int> otherIDs;	//To contain PDGIDs of unclassified particles

  //Histograms for particle content from cells
  TH1F* h_all_c; TH1F* h_ch_c; TH1F* h_nh_c; TH1F* h_gamma_c;

  //Machinery for histogram stacking
  vector<string> hTitles;
  THStack* hstack[3];
  THStack* hstack_c;
  THStack* hstackOther[3];
  hTitles.push_back("#font[132]{Z+jet probe particle content (gen)}");
  hTitles.push_back("#font[132]{Z+jet probe particle content (MC SPR)}");
  hTitles.push_back("#font[132]{Z+jet probe particle content (Fit)}");

  //Instantiate and setup composition histos if needed
  if (GetcontHistos()) {
    int const nbins_h = 30; int const nbins_chf = 23;
    const double bins_h_lo = 0.0;
    const double bins_h_hi = 3000;
    const double bins_chf[24] = {14.970, 20.905, 27.804, 36.981, 48.845, 64.069, 84.037, 114.131,
                                 152.859, 196.359, 272.299, 330.855, 396.449, 468.484, 549.770,
                                 691.638, 852.145, 1035.394, 1258.050, 1604.863, 2018.998,
                                 2522.393, 3129.452, 3882.611};

    //Particle content from cells in function of jet energy
    h_all_c   = new TH1F("", "", nbins_chf, bins_chf);
    h_ch_c    = new TH1F("", "", nbins_chf, bins_chf);
    h_nh_c    = new TH1F("", "", nbins_chf, bins_chf);
    h_gamma_c = new TH1F("", "", nbins_chf, bins_chf);

    h_ch_c   ->SetFillColor(kRed-7);   h_ch_c->SetLineWidth(1);    h_ch_c->SetLineColor(kBlack);
    h_nh_c   ->SetFillColor(kGreen-6); h_nh_c->SetLineWidth(1);    h_nh_c->SetLineColor(kBlack);
    h_gamma_c->SetFillColor(kBlue-7);  h_gamma_c->SetLineWidth(1); 
    h_gamma_c->SetLineColor(kBlack);
    hstack_c = new THStack("", "#font[132]{Z#mu#mu jet particle content in |#eta| < 1.3}");

    for (int n=0; n!=3; ++n) {	//Indices: 0=gen lvl, 1=MC reco, 2=fit reco
      h_e[n]     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //e^+-
      h_gamma[n] = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Photons
      h_gpi0[n]  = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //pi^0 phot.
      h_K[n]     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Kaons K^+-
      h_K0S[n]   = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //K^0_S
      h_K0L[n]   = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //K^0_L
      h_L[n]     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Lambda
      h_mu[n]    = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //mu
      h_n[n]     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Neutrons
      h_nu[n]    = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Neutrinos
      h_pi[n]    = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //pi^+-
      h_p[n]     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Protons
      h_other[n] = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi);
      h_Xi0[n]   = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Xi^0
      h_Xim[n]   = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Xi^-
      h_Sig0[n]  = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Sigma^0
      h_Sigm[n]  = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Sigma^-
      h_Sigp[n]  = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Sigma^+
      h_Om[n]    = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Omega^-
      //All particle types
      h_all[n]   = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi);
      //For stacking the above histograms to plot them simultaneously
      hstack[n] = new THStack("", hTitles[n].c_str() );
      hstackOther[n] = new THStack("","");
      //Fill colours
      h_e[n]->SetFillColor(    kAzure+1);    h_mu[n]->SetFillColor(  kViolet-3);
      h_gamma[n]->SetFillColor(41);          h_gpi0[n]->SetFillColor(42);
      h_pi[n]->SetFillColor(   kAzure-9);    h_K[n]->SetFillColor(    kTeal-9);
      h_K0S[n]->SetFillColor(  kTeal-8);     h_K0L[n]->SetFillColor( kTeal-7);
      h_L[n]->SetFillColor(    kCyan-1);
      h_p[n]->SetFillColor(    kGray+2);     h_n[n]->SetFillColor(   kGray);
      h_other[n]->SetFillColor(kOrange-9);   h_nu[n]->SetFillColor(  kWhite);
      h_Xi0[n]->SetFillColor(  kOrange-8);   h_Xim[n]->SetFillColor( kOrange-4);
      h_Sig0[n]->SetFillColor( kOrange-7);   h_Sigm[n]->SetFillColor(kOrange-5);
      h_Sigp[n]->SetFillColor( kOrange-9);   h_Om[n]->SetFillColor(  kOrange-3);
      //More narrow lines for better readability
      h_e[n]->SetLineWidth(    1);  h_mu[n]->SetLineWidth(   1);
      h_gamma[n]->SetLineWidth(1);  h_gpi0[n]->SetLineWidth( 1);
      h_pi[n]->SetLineWidth(   1);  h_K[n]->SetLineWidth(    1);
      h_K0S[n]->SetLineWidth(  1);  h_K0L[n]->SetLineWidth(  1);
      h_p[n]->SetLineWidth(    1);  h_n[n]->SetLineWidth(    1);
      h_L[n]->SetLineWidth(    1);
      h_other[n]->SetLineWidth(1);  h_nu[n]->SetLineWidth(   1);
      h_Xi0[n]->SetLineWidth(  1);  h_Xim[n]->SetLineWidth(  1);
      h_Sig0[n]->SetLineWidth( 1);  h_Sigm[n]->SetLineWidth( 1);
      h_Sigp[n]->SetLineWidth( 1);  h_Om[n]->SetLineWidth(   1);
      //Line colours
      h_e[n]->SetLineColor(    kBlack);  h_mu[n]->SetLineColor(   kBlack);
      h_gamma[n]->SetLineColor(kBlack);  h_gpi0[n]->SetLineColor( kBlack);
      h_pi[n]->SetLineColor(   kBlack);  h_K[n]->SetLineColor(    kBlack);
      h_K0S[n]->SetLineColor(  kBlack);  h_K0L[n]->SetLineColor(  kBlack);
      h_p[n]->SetLineColor(    kBlack);  h_n[n]->SetLineColor(    kBlack);
      h_L[n]->SetLineColor(    kBlack);
      h_other[n]->SetLineColor(kBlack);  h_nu[n]->SetLineColor(   kBlack);
      h_Xi0[n]->SetLineColor(  kBlack);  h_Xim[n]->SetLineColor(  kBlack);
      h_Sig0[n]->SetLineColor( kBlack);  h_Sigm[n]->SetLineColor( kBlack);
      h_Sigp[n]->SetLineColor( kBlack);  h_Om[n]->SetLineColor(   kBlack);
    }
  }

  /* LOOP EVENTS */
  //SOME TEMPORARY VARIABLES TO TRACK CUTTING ETC.
  cout << "Entering loop";
  if (!printProg) cout << ", progress printing disabled";
  cout << endl;

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

  //Keep track of how many events pass each cut
  int mumuCut = 0;
  int invM = 0;
  int tagCut = 0;
  int alpha = 0;
  int probeCut = 0;
  int b2b = 0;


//***********************************************************************************************
  //Granularity of cells
  int nPhi = 72; int nEta = 119;

  TH2D* cht       = new TH2D("cht"      , "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* chtPt     = new TH2D("chtPt"    , "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* chECAL    = new TH2D("chECAL"   , "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* chHCAL    = new TH2D("chHCAL"   , "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* sigma     = new TH2D("sigma"    , "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* sigmaTrk  = new TH2D("sigmaTrk" , "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* sigmaCalo = new TH2D("sigmaCalo", "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* nhECAL    = new TH2D("nhECAL"   , "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* nhHCAL    = new TH2D("nhHCAL"   , "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* ne        = new TH2D("ne"       , "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* eCalo     = new TH2D("eCalo"    , "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);

  const double bins_chhEff[21] = {0.1, 0.149, 0.223, 0.332, 0.496, 0.740, 1.104, 1.648, 2.460,
                                  3.670, 5.477, 8.174, 12.198, 18.202, 27.163, 40.536, 60.492,
                                  90.272, 134.713, 201.031, 300.0};
  const double bins_zsmear[21] = {10, 12.59, 15.85, 19.95, 25.12, 31.62, 39.81, 50.12, 63.10,
                                  79.43, 100, 125.89, 158.49, 199.53, 251.19, 316.23, 398.12,
                                  501.19, 630.96, 794.33, 1000};

  TProfile* chhEff   = new TProfile("chhEff", "", 20, bins_chhEff);
  TProfile* ZSmear   = new TProfile("ZSmear", "", 20, bins_zsmear);

  TF1* pchf = new TF1("pchf","[0]*pow(x,2) + [1]*x + [2]", 0, 5000);
  pchf->SetParameters(6.86221185e-09, -0.0001268141794, 0.6283211821);

  //Single pion resolution
  TF1* jerg_A = new TF1("jerg_A", "sqrt([0]*[0]/(x*x)+[1]*[1]/x+[2]*[2]) * (0.55*1.02900*(1-1.6758*pow(x/0.75,0.553456-1)) + 0.45*1.10286*(1-1.25613*pow(x/0.75,0.397034-1)))", 0, 1000);
  jerg_A->SetParameters(9.59431e-05, 1.49712, 8.92104e-02);
  //jerg_A->SetParameters(0, 1.02, 0.065); //PF-code

  //Track resolution in a function of gen pT
  //TF1* pionTrkReso = new TF1("pionTrkReso", "0.01336 + 8.548e-5*x", 0, 5000);

  TF1* pionTrkReso = new TF1("pionTrkReso", "2*(0.01336 + 8.548e-5*x)", 0, 5000);


  TF1* eff_fit = new TF1("eff_fit","1+[0]*x+[1]*x*x+[2]*exp([3]*x)", 0, 5000);
  eff_fit->SetParameters(-0.0003897, 3.589e-07, -0.02651, -0.2829);

  //Fakerate from PF paper
  TF1* fakeRate = new TF1("fakeRate", "0.00046178*x + 0.02", 0, 5000);

//***********************************************************************************************

  //Loop Tree entries = events
  for (Long64_t jentry=0; jentry != nentries; ++jentry) {
    //Print progress for long runs
    if ((GetprintProg() && jentry % 1000==0)) {
      cout << "Looping event " << jentry; cout << " in Z+jet" << endl;
    }

    //Skip events that didn't pass cuts earlier. Useful in e.g. repeating Loop
    if (GetuseEarlierCuts() && passedCuts.size()>jentry && !passedCuts[jentry]) continue;
 
    Long64_t ientry = LoadTree(jentry);	//Load new event
    if (ientry < 0) break;		//When no more events
    nb = fChain->GetEntry(jentry); nbytes += nb;

    //Reinit
    i_tag = 0; i_probe = 0; 
    p4.SetPtEtaPhiE(0,0,0,0);        MET_r.SetPtEtaPhiE(0,0,0,0);
    p4_calo.SetPtEtaPhiE(0,0,0,0);
    tag.SetPtEtaPhiE(0,0,0,0);       tag_r.SetPtEtaPhiE(0,0,0,0);
    probe.SetPtEtaPhiE(0,0,0,0);     probe_g.SetPtEtaPhiE(0,0,0,0);
    probe_spr.SetPtEtaPhiE(0,0,0,0); probe_pf.SetPtEtaPhiE(0,0,0,0);
    probe_calo.SetPtEtaPhiE(0,0,0,0);
    jets_g.clear(); jets_r.clear(); 
    njets = (unsigned long)jet_pt->size();
    jets_g.resize(njets); jets_r.resize(njets);
    for (int i=0; i!=jets_g.size(); ++i) { //All objects have njets size
      jets_g[i].SetPtEtaPhiE(0,0,0,0);  jets_r[i].SetPtEtaPhiE(0,0,0,0);
    }

    //**************** Z+JET: FIND AND RECONSTRUCT TAG MUONS ****************//
    int muPDG=13; int muTAG=3; //mu PDGID and parton tag
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

    Response((*prtn_pdgid)[i_tag1],p4.Eta(),p4.E(),p4.Pt(),Rt,Bfield,
             fr_h, resp, respH, respEHE, respHHe);

    p4 *= resp*gRandom->Gaus(1,pionTrkReso->Eval(p4.Pt())); //MC lvl

    if (fabs(p4.Eta()) > eta_muon || p4.Pt() < pTmin_muon) continue;

    tag_r = p4;

    //***** 2nd muon *****
    p4.SetPtEtaPhiE((*prtn_pt )[i_tag2], (*prtn_eta)[i_tag2],
                    (*prtn_phi)[i_tag2], (*prtn_e  )[i_tag2]);

    tag += p4;//gen lvl

    Response((*prtn_pdgid)[i_tag2],p4.Eta(),p4.E(),p4.Pt(),Rt,Bfield,
             fr_h, resp, respH, respEHE, respHHe);

    p4 *= resp*gRandom->Gaus(1,pionTrkReso->Eval(p4.Pt())); //MC lvl

    if (fabs(p4.Eta()) > eta_muon || p4.Pt() < pTmin_muon) continue;

    tag_r += p4;

    mumuCut++;    

    //Invariant mass in range 70-110 GeV since Z boson mass is 91 GeV
    if (tag.M()<70.0 || tag.M()>110.0) continue; invM ++;
    //ZSmear->Fill(   tag_r.Pt(), (tag.Pt()/tag_r.Pt()), weight);

    //Tag eta and pT cuts:
    if (fabs(tag_r.Eta()) > eta_tag_z || tag_r.Pt() < pTmin_tag_z) continue; tagCut++;
     

    /***************** RECONSTRUCT JETS AND PARTICLES IN JETS *****************/
    for (int i=0; i != prtcl_pt->size(); ++i) {
      JI = (*prtcl_jet)[i]; 
      PDG = abs((*prtcl_pdgid)[i]);
      p4.SetPtEtaPhiE((*prtcl_pt )[i], (*prtcl_eta)[i],	(*prtcl_phi)[i], (*prtcl_e  )[i]);

      //Calculate responses. Store results to [resp, respH] tuples
      Response(PDG, p4.Eta(), p4.E(), p4.Pt(), Rt, Bfield, fr_h, resp, respH, respEHE, respHHe);

      //Reconstruct jets
      jets_g[JI] += (isNeutrino(PDG) ? 0:1)*p4; //Gen lvl
      jets_r[JI] += p4*resp;                    //MC reco
      
      //Leptons to PF_probe
      if ((*prtcl_jet)[i]==i_probe && (PDG==11 || PDG==13)) {
        probe_pf += p4*resp; //No tracking fail
      }
    } //Loop particles in jets

    /************************* Z+JET: FIND PROBE *************************/

    i_probe = 0; //Always the leading pT jet
    //Gen lvl as output by FastJet
    probe.SetPtEtaPhiE((*jet_pt)[i_probe],  (*jet_eta)[i_probe],
                       (*jet_phi)[i_probe], (*jet_e)[i_probe]  );
    //Gen lvl minus neutrinos
    probe_g.SetPtEtaPhiE(jets_g[i_probe].Pt(),  jets_g[i_probe].Eta(),
                         jets_g[i_probe].Phi(), jets_g[i_probe].E() );
    //MC SPR reco
    probe_spr.SetPtEtaPhiE(jets_r[i_probe].Pt(),  jets_r[i_probe].Eta(),
                           jets_r[i_probe].Phi(), jets_r[i_probe].E() );


    /****************************** COMMON CUTS FOR Z+JET ******************************/

    //Alpha cut
    if (jets_r[1].Pt() > 0.3*tag_r.Pt()) continue; alpha ++;

    /******************* RECONSTRUCT PARTICLES NOT IN JETS *******************/
    //Reset histograms
    cht      ->Reset();
    chtPt    ->Reset();
    chECAL   ->Reset();
    chHCAL   ->Reset();
    sigma    ->Reset();
    sigmaTrk ->Reset();
    sigmaCalo->Reset();
    nhECAL   ->Reset();
    nhHCAL   ->Reset();
    ne       ->Reset();
    eCalo    ->Reset();

    //Probe flavor check
    for (unsigned int j = 0; j != prtn_tag->size(); ++j) {
      //prtn_tag=0 for outgoing hard process partons
      if ((*prtn_jet)[j]==i_probe && (*prtn_tag)[j]==0) {
        probeFlav = abs((*prtn_pdgid)[j]);
        //5; b-jets < 5; light quark 21; Gluon jets else "undetermined"
        continue;
      }
    } //Loop partons
    //if(probeFlav != 5) continue; //b-jet events

    //****************** MET calculation *******************//
    //Add tag object to the MET
    MET_r -= tag_r;

    //Loop over all particles
    for (int i=0; i!=prtclnij_pt->size(); ++i) {
      PDG = fabs((*prtclnij_pdgid)[i]);
      p4.SetPtEtaPhiE((*prtclnij_pt )[i],(*prtclnij_eta)[i],
                      (*prtclnij_phi)[i],(*prtclnij_e  )[i]);

      Response((*prtclnij_pdgid)[i], p4.Eta(), p4.E(), p4.Pt(), Rt, Bfield, fr_h, 
               resp, respH, respEHE, respHHe);
 
      if (resp == 0.0) continue;

      p4 *= resp;

      switch (PDG) {
        //PHOTON
        case 20 : case 22 : 
          ne->Fill(p4.Phi(), p4.Eta(), p4.E());
          break;

        //LEPTONS
        case 13 : case 11 :
          // Not a tag muon
          if ((*prtclnij_pt)[i]!=(*prtn_pt)[i_tag1] && (*prtclnij_pt )[i]!=(*prtn_pt)[i_tag2]){ 
            MET_r -= p4;
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

          //Particle flow paper efficiency
          //eff = 0.5284 + 0.3986/(1+pow(((*prtclnij_pt )[i]/88.76),1.22));
          //if ((*prtclnij_pt )[i] < 0.9) eff = 0.2514 + 0.7429*(*prtclnij_pt )[i];

          
          //Efficiency from chs
          if (fabs(p4.Eta() < 5.2)) {
            for (int ijet=0; ijet!=jets_r.size(); ++ijet) {
              if (p4.DeltaR(jets_r[ijet]) < 0.4) {
                eff = pchf->Eval(jets_r[ijet].Pt()) / 0.607;
                eff -= 0.263243+(0.04199621-0.263243)/(1+pow(((*prtclnij_pt)[i]/74.1766),1.737401));
              }
            }
          }
          
          eff = max(eff,0.0);
          eff = min(eff,0.94);
          //eff = min(eff,1.0);
          
          //Tracking efficiency plot
          if ( (fabs((*prtclnij_eta)[i]) < 2.5)) {
            chhEff->Fill((*prtclnij_pt)[i], eff);
          }

          if ( ((double)rand() / (double)RAND_MAX) > eff ) trkFail =  1;

          if (!trkFail) {
              cht  ->Fill(p4.Phi(), p4.Eta(), p4.E() );
              chtPt->Fill(p4.Phi(), p4.Eta(), p4.Pt());
          }

          //Phi where particle exits tracker
          newPhi = trackDeltaPhi((*prtclnij_pdgid)[i], (*prtclnij_phi)[i], 
                                 (*prtclnij_pt )[i], Rt, Bfield);

          p4.SetPtEtaPhiE((*prtclnij_pt )[i], (*prtclnij_eta)[i], newPhi, (*prtclnij_e)[i]);
          p4 *= respH; //Calorimeter response

          //*** Shadowing resolution ***//
          if (respH > 0.0) {
            calReso = max(0.0,      jerg_A->Eval((*prtclnij_pt)[i]) * (*prtclnij_pt)[i]);
            trkReso = max(0.0, pionTrkReso->Eval((*prtclnij_pt)[i]) * (*prtclnij_pt)[i]);

            sigma    ->Fill(p4.Phi(), p4.Eta(), pow(calReso,2));
            sigma    ->Fill(p4.Phi(), p4.Eta(), pow(trkReso,2));
            sigmaTrk ->Fill(p4.Phi(), p4.Eta(), pow(trkReso,2));
            sigmaCalo->Fill(p4.Phi(), p4.Eta(), pow(calReso,2));
          }

          
          p4.SetPtEtaPhiE((*prtclnij_pt )[i], (*prtclnij_eta)[i], newPhi, (*prtclnij_e)[i]);


          //ECAL and HCAL deposits
          if ( ((double)rand()/(double)RAND_MAX) > 0.45 ) { //EHE path
            p4 *= respEHE;
            chECAL->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E()); //ECAL
            chHCAL->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E()); //HCAL

            if (trkFail) {
              nhECAL->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E()); //ECAL
              nhHCAL->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E()); //HCAL
            }
          } else { //HHe path 
            p4 *= respHHe;
            chHCAL->Fill(p4.Phi(), p4.Eta(), p4.E()); //HCAL

            if (trkFail) {
              nhHCAL->Fill(p4.Phi(), p4.Eta(), p4.E()); //HCAL
            }
          }
          break;

        //NEUTRAL HADRONS
        case 130 : case 310 : case 3122 : case 2112 : case 3212 : case 3322 :
          p4.SetPtEtaPhiE((*prtclnij_pt )[i],(*prtclnij_eta)[i],
                          (*prtclnij_phi)[i],(*prtclnij_e  )[i]);

          if ( ((double)rand()/(double)RAND_MAX) > 0.45 ) { //EHE path
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

    delta = 0.0; cellSigma = 0.0;
    // ***************** Loop over cells ******************
    for (int i=1; i!=cht->GetNbinsX()+1; ++i) {
      for (int j=1; j!=cht->GetNbinsY()+1; ++j) {
        double eff_c;
        double nhHCAL_calib = 0.0; double chc_calib = 0.0;
        p4_chc.SetPtEtaPhiE(0,0,0,0); p4_cht.SetPtEtaPhiE(0,0,0,0);
        p4.SetPtEtaPhiE(0,0,0,0);     p4_2.SetPtEtaPhiE(0,0,0,0);

        
        eff_c = 1.0; //No tracking fail

        cellPhi = cht->GetXaxis()->GetBinCenter(i);
        cellEta = cht->GetYaxis()->GetBinCenter(j);

        //Cell four vector
        cellE  = nhECAL->GetBinContent(i,j)                + nhHCAL->GetBinContent(i,j) + 
                     ne->GetBinContent(i,j)                +    cht->GetBinContent(i,j);
        cellPt =(nhECAL->GetBinContent(i,j)                + nhHCAL->GetBinContent(i,j) + 
                     ne->GetBinContent(i,j))/cosh(cellEta) +  chtPt->GetBinContent(i,j);
        p4.SetPtEtaPhiE(cellPt, cellEta, cellPhi, cellPt);

        //eff_c = 1.0 - 0.0006 * chtPt->GetBinContent(i,j);

        eff_c = max(eff_c,0.0);
        eff_c = min(eff_c,1.0);
        
        //Cell fail
        if ( ((double)rand() / (double)RAND_MAX) > eff_c ) {
          nhECAL->Fill(cellPhi, cellEta, chECAL->GetBinContent(i,j));//ECAL
          nhHCAL->Fill(cellPhi, cellEta, chHCAL->GetBinContent(i,j));//HCAL
          cht  ->SetBinContent(i,j,0.0);
          chtPt->SetBinContent(i,j,0.0);
        }

        /*
        //Single particle efficiency plot
        int etaIdx; int phiIdx;
        double phiStep; phiStep = 2*TMath::Pi() / nPhi;
        double etaStep; etaStep = 10.4 / nEta;

        if (fabs(cellEta) < 2.5) {
          for (int k=0; k!=prtclnij_pt->size(); ++k) { //Loop over particles
            if (isChHadron((*prtclnij_pdgid)[k])) {
              double newPhi;
              newPhi = trackDeltaPhi((*prtclnij_pdgid)[k], (*prtclnij_phi)[k], 
                                     (*prtclnij_pt )[k], Rt, Bfield);
              phiIdx = floor(((*prtclnij_phi)[k]+TMath::Pi())/phiStep) + 1;
              etaIdx = floor(((*prtclnij_eta)[k]+5.2)/etaStep) + 1;
              
              if ((phiIdx == i) && (etaIdx == j)) {
                chhEff->Fill((*prtclnij_pt)[k], eff_c);
              }
            }
          }
        }*/

        int iEtaCold;  
        iEtaCold = floor((cellEta+fabs(coldCells->GetXaxis()->GetBinLowEdge(1)))/coldCells->GetXaxis()->GetBinWidth(1) + 1);
        
        if (   coldCells->GetBinContent(iEtaCold,i*2  ) > 1.0
            || coldCells->GetBinContent(iEtaCold,i*2-1) > 1.0) {

          if (nhECAL->GetBinContent(i,j) + ne->GetBinContent(i,j) > 150.0) {
            nhECAL->SetBinContent(i,j,75);
            ne->SetBinContent(i,j,75);
          }
        }

        /*
        if (varCp3) {
          nhECAL->SetBinContent(i,j, 1.03*nhECAL->GetBinContent(i,j));
          nhHCAL->SetBinContent(i,j, 1.03*nhHCAL->GetBinContent(i,j));
          chECAL->SetBinContent(i,j, 1.03*chECAL->GetBinContent(i,j));
          chHCAL->SetBinContent(i,j, 1.03*chHCAL->GetBinContent(i,j));
        } else if (varCm3) {
          nhECAL->SetBinContent(i,j, 0.97*nhECAL->GetBinContent(i,j));
          nhHCAL->SetBinContent(i,j, 0.97*nhHCAL->GetBinContent(i,j));
          chECAL->SetBinContent(i,j, 0.97*chECAL->GetBinContent(i,j));
          chHCAL->SetBinContent(i,j, 0.97*chHCAL->GetBinContent(i,j));
        }*/

        delta = nhECAL->GetBinContent(i,j) + ne->GetBinContent(i,j) + nhHCAL->GetBinContent(i,j);
        
        //Changing HCAL cluster sigma threshold with uncalibrated cell energy
        cellSigma = sqrt(sigma->GetBinContent(i,j));
        cellSigma *= (1 + exp(-cht->GetBinContent(i,j)/100));

        

        //HCAL calibration
        if (nhHCAL->GetBinContent(i,j) > 0.0) {
          nhHCAL_calib = fr_hcal->GetX(nhHCAL->GetBinContent(i,j), 0.1, 7000.0);
        }


        double chc = 0.0;
        chc = chHCAL->GetBinContent(i,j) + chECAL->GetBinContent(i,j);

        //Calibrate ch calorimeter energy deposit chc
        if (chc > 0.0) {
          chc_calib = fr_hcal->GetX(chc, 0.1, 7000.0);
        }

        //Calo cell four vector
        cellE_calo = ne->GetBinContent(i,j)     + eCalo->GetBinContent(i,j)
                   + nhECAL->GetBinContent(i,j) + nhHCAL->GetBinContent(i,j) 
                   + chECAL->GetBinContent(i,j) + chHCAL->GetBinContent(i,j);

        p4_calo.SetPtEtaPhiE(cellE_calo/cosh(cellEta), cellEta, cellPhi, cellE_calo);


        if (delta < cellSigma) { //Shadowing
          nhHCAL_calib = 0.0;
          nhECAL->SetBinContent(i,j,0.0); 
          ne    ->SetBinContent(i,j,0.0);
        }


        //Weighting of track momentum trkP and calorimeter energy caloE
        double w;
        w = sigmaCalo->GetBinContent(i,j) / 
           (sigmaTrk->GetBinContent(i,j) + sigmaCalo->GetBinContent(i,j));

        
        //Charged hadron deposit
        if (sqrt(sigmaTrk->GetBinContent(i,j)) > 0.1) {
          p4_chc.SetPtEtaPhiE(chc_calib/cosh(cellEta), cellEta, cellPhi, chc_calib);
          p4_cht.SetPtEtaPhiE(chtPt->GetBinContent(i,j), cellEta, cellPhi, 
                              cht->GetBinContent(i,j));

          p4_2 = w*p4_cht + (1-w)*p4_chc;

        } else {
          p4_2.SetPtEtaPhiE(chtPt->GetBinContent(i,j), cellEta, cellPhi, 
                            cht->GetBinContent(i,j)); // Normal hadron deposit
        }
        
        cellE = ne->GetBinContent(i,j) + nhECAL->GetBinContent(i,j) + nhHCAL_calib;
        cellPt = cellE/cosh(cellEta);
        p4.SetPtEtaPhiE(cellPt, cellEta, cellPhi, cellE);

        p4 += p4_2; //Charged hadron deposit

        MET_r -= p4;

        if (p4_calo.DeltaR(jets_r[0]) < 0.4) {
          probe_calo += p4_calo;
        }

        //Probe reconstruction
        if (p4.DeltaR(jets_r[0]) < 0.4) {
          probe_pf   += p4;
          h_ch_c   ->Fill(jets_r[0].Pt(), cht->GetBinContent(i,j));
          h_nh_c   ->Fill(jets_r[0].Pt(), nhHCAL_calib);
          h_gamma_c->Fill(jets_r[0].Pt(), nhECAL->GetBinContent(i,j) + ne->GetBinContent(i,j));
          h_all_c  ->Fill(jets_r[0].Pt(), cht->GetBinContent(i,j) + nhHCAL_calib +
                                          nhECAL->GetBinContent(i,j) + ne->GetBinContent(i,j));
        }
      }
    }

    /************************** PARTICLE COMPOSITION **************************/
    /*
    //Loop all particles associated with jets
    for (unsigned int i=0; i!=(GetcontHistos() ? prtcl_pdgid->size():0); ++i) {
      JI = (*prtcl_jet)[i];

      //See if ptcl belongs to the studied jet(s)
      if (JI==i_probe) {

        PDG = abs((*prtcl_pdgid)[i]); 
        p4.SetPtEtaPhiE((*prtcl_pt)[i], (*prtcl_eta)[i], //Prtcl gen lvl values
		        (*prtcl_phi)[i],(*prtcl_e)[i]  );

        Response(PDG,p4.Eta(),p4.E(),p4.Pt(),Rt,Bfield,fr_h, resp,respH, respEHE, respHHe);

        // Fill (probe) particle content to the right histogram
        if (GetcontHistos()) {
          switch (PDG) {
            case 11   : for (int l=0; l!=3; ++l) h_ptr[l]=h_e[l];     break;
            case 22   : for (int l=0; l!=3; ++l) h_ptr[l]=h_gamma[l]; break;
            case 20   : for (int l=0; l!=3; ++l) h_ptr[l]=h_gpi0[l];  break;
            case 321  : for (int l=0; l!=3; ++l) h_ptr[l]=h_K[l];     break;
            case 310  : for (int l=0; l!=3; ++l) h_ptr[l]=h_K0S[l];   break;
            case 130  : for (int l=0; l!=3; ++l) h_ptr[l]=h_K0L[l];   break;
            case 3122 : for (int l=0; l!=3; ++l) h_ptr[l]=h_L[l];     break;
            case 13   : for (int l=0; l!=3; ++l) h_ptr[l]=h_mu[l];    break;
            case 2112 : for (int l=0; l!=3; ++l) h_ptr[l]=h_n[l];     break;
            case 211  : for (int l=0; l!=3; ++l) h_ptr[l]=h_pi[l];    break;
            case 2212 : for (int l=0; l!=3; ++l) h_ptr[l]=h_p[l];     break;
            default :
              if (isNeutrino(PDG)) {for (int l=0; l!=3; ++l) h_ptr[l]=h_nu[l];}
              else if (isStrangeB(PDG)) {
                switch (PDG) {
                  case 3112 : for (int l=0; l!=3; ++l) h_ptr[l]=h_Sigm[l];break;
                  case 3212 : for (int l=0; l!=3; ++l) h_ptr[l]=h_Sig0[l];break;
                  case 3222 : for (int l=0; l!=3; ++l) h_ptr[l]=h_Sigp[l];break;
                  case 3312 : for (int l=0; l!=3; ++l) h_ptr[l]=h_Xim[l]; break;
                  case 3322 : for (int l=0; l!=3; ++l) h_ptr[l]=h_Xi0[l]; break;
                  case 3334 : for (int l=0; l!=3; ++l) h_ptr[l]=h_Om[l];  break;
                  default : cout<<"Error: isStrangeB passed ID "<<PDG<<endl;
                }
                //Fill the general "other particles" histogram already here
                h_other[0]->Fill((*jet_e)[(*prtcl_jet)[i_probe]], (*prtcl_e)[i]  );
                h_other[1]->Fill((*jet_e)[(*prtcl_jet)[i_probe]], (resp*p4).E()  );
              } //Xi, Sigma, Omega^-
          } //Switch PDG (general)
          h_ptr[0]->Fill((*jet_e)[(*prtcl_jet)[i_probe]], (*prtcl_e)[i]  );
          h_ptr[1]->Fill((*jet_e)[(*prtcl_jet)[i_probe]], (resp*p4).E()  );
          h_all[0]->Fill((*jet_e)[(*prtcl_jet)[i_probe]], (*prtcl_e)[i]  );
          h_all[1]->Fill((*jet_e)[(*prtcl_jet)[i_probe]], (resp*p4).E()  );
        } //Particle content histograms
      } //If ptcl in the studied probe or tag
    } //Loop over particles*/

    /**************************** FILL HISTOGRAMS ****************************/

    pTp = tag_r.Pt();

    //PF probe-tag cuts
    if (fabs(probe_pf.Eta()) > eta_probe || probe_pf.Pt() < pTmin_probe) continue; probeCut ++;

    //Back-to-back leikkaus
    if (fabs(tag_r.DeltaPhi(probe_pf)) < phiMin) continue; b2b ++;

    //pT balance
    prpTbal->Fill(pTp, probe_pf.Pt()/tag_r.Pt(), weight);

    //MPF response
    R_MPF_r = 1.0 + (MET_r.Px()*tag_r.Px() + MET_r.Py()*tag_r.Py()) / pow((tag_r.Pt()),2);
    //R_MPF_r = 1.0 + MET_r.Pt()*cos(tag_r.DeltaPhi(MET_r))/tag_r.Pt();
    if(isnan(R_MPF_r)) R_MPF_r = 0.0;

    //Fill MPF histograms
    prMPF->Fill(pTp, R_MPF_r, weight);

    double Rjet, Rjet_calo;
    Rjet      = probe_pf.Pt()/probe_g.Pt();
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
        if (abs((*prtn_pdgid)[j])==5) {	//b-jets
          FFb->Fill(pTp, weight);
          prMPFb->Fill(pTp, R_MPF_r, weight);
          prRjetb->Fill(probe_g.Pt(), Rjet, weight);

        } else if (abs((*prtn_pdgid)[j])<5) { //Light quark (u,d,s,c) jets
          FFlq->Fill(pTp, weight);
          prMPFlq->Fill(pTp, R_MPF_r, weight);
          prRjetlq->Fill(probe_g.Pt(), Rjet, weight);

          if (abs((*prtn_pdgid)[j])==4) { //c-jets
            prMPFc->Fill(pTp, R_MPF_r, weight);
            prRjetc->Fill(probe_g.Pt(), Rjet, weight);            
          } else if (abs((*prtn_pdgid)[j])==3) { //s-jets
            prMPFs->Fill(pTp, R_MPF_r, weight);
            prRjets->Fill(probe_g.Pt(), Rjet, weight);         
          } else if (abs((*prtn_pdgid)[j])<3) { //(u,d)
            prMPFud->Fill(pTp, R_MPF_r, weight);
            prRjetud->Fill(probe_g.Pt(), Rjet, weight);  
          }

        } else if ((*prtn_pdgid)[j]==21) { //Gluon jets
          FFg->Fill(pTp, weight);
          prMPFg->Fill(pTp, R_MPF_r, weight);
          prRjetg->Fill(probe_g.Pt(), Rjet, weight);

        } else continue; //Undetermined flavour
        FFa->Fill(pTp, weight);
        continue;	//Only one flavour may be associated with a jet
      }
    } //Loop partons

    //If the old list of cut events is not read, a new one is written
    if (!GetuseEarlierCuts()) passedCuts[jentry]=true;
  } //Loop Tree entries

  TGraph *chf   = new TGraph("data_and_MC_input/Chf/chf.txt");
  TGraph *chnhf = new TGraph("data_and_MC_input/Chf/chnhf.txt");
  chf->SetMarkerStyle(kFullDiamond); chnhf->SetMarkerStyle(kFullSquare);
  chf->SetMarkerColor(kBlack);       chnhf->SetMarkerColor(kBlack);
  chf->SetMarkerSize(2);             chnhf->SetMarkerSize(1);

  //Canvas and related setup
  string canvName = ReadName + "_prtclCont_cells";
  TCanvas* canv_c = new TCanvas(canvName.c_str(),"",1200,1200);
  TLegend* lg_c = new TLegend(0.9, 0.1, 1.0, 0.9);
  lg_c->SetBorderSize(0); //No frame
  lg_c->SetTextSize(0.028);
  lg_c->SetFillStyle(0);  //No background fill
  canv_c->SetLogx();

  //Normalize histograms by the sum of all ptcls and add to stack
  h_ch_c   ->Divide(h_all_c);  hstack_c->Add(h_ch_c);
  h_nh_c   ->Divide(h_all_c);  hstack_c->Add(h_nh_c);
  h_gamma_c ->Divide(h_all_c); hstack_c->Add(h_gamma_c);

  //Legend setup
  lg_c->AddEntry(h_gamma_c, "#gamma",         "f");
  lg_c->AddEntry(h_nh_c,    "#font[132]{NH}", "f");
  lg_c->AddEntry(h_ch_c,    "#font[132]{CH}", "f");

  hstack_c->Draw("HISTO");
  hstack_c->GetYaxis()->SetRange(0.0,1.0);
  hstack_c->GetXaxis()->SetTitle("#font[12]{p}_{T}^{jet} [GeV]");
  hstack_c->GetYaxis()->SetTitle("Jet energy fraction");
  hstack_c->GetYaxis()->SetTitleFont(133);
  hstack_c->GetYaxis()->SetTitleSize(35);
  hstack_c->GetXaxis()->SetTitleFont(133);
  hstack_c->GetXaxis()->SetTitleSize(35);
  hstack_c->GetXaxis()->SetTitleOffset(1.5);
  hstack_c->GetYaxis()->SetTitleOffset(1.5);
  hstack_c->GetYaxis()->SetNdivisions(20);
  lg_c->Draw();
  chf->Draw("samep");
  chnhf->Draw("samep");

  //Save particle content histogram plot
  string plotName = "./plots/particleComposition/PC_jets.eps";
  canv_c->Print(plotName.c_str());

  //Normalize flavour fraction histograms, add to stack and plot
  FFb->Divide(FFa);   FFg->Divide(FFa);    FFlq->Divide(FFa);
  FFstack->Add(FFg);  FFstack->Add(FFlq);  FFstack->Add(FFb);

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
    //Flavour fraction
    string FFcanvName = ReadName + "_FF";
    TCanvas* FFcanv = new TCanvas(FFcanvName.c_str(),"",400,400);
    TLegend* FFlg = new TLegend(0.91, 0.67, 1.00, 0.90);
    FFcanv->SetLogx();
    FFlg->SetBorderSize(0);  FFlg->SetFillStyle(0);
    FFlg->AddEntry(FFb,  " #font[12]{b}",  "f");
    FFlg->AddEntry(FFlq, " #font[12]{lq}", "f");
    FFlg->AddEntry(FFg,  " #font[12]{g}",  "f");
    FFb->SetFillColor(46);  FFg->SetFillColor(kGray+1);  FFlq->SetFillColor(33);
    FFstack->Draw("HISTO");
    FFlg->Draw();
    FFcanvName = "./plots/FlavourFraction/" + FFcanvName + ".eps";
    FFcanv->Print(FFcanvName.c_str());
    delete FFcanv;
    delete FFlg;

    //Canvas and related setup
    string canvName = ReadName + "_prtclCont";
    TCanvas* canv = new TCanvas(canvName.c_str(),"",1500,1000);
    canv->Divide(3,2);

    TLegend* lg      = new TLegend(0.865, 0.1, 1.0,  0.9);
    TLegend* lgOther = new TLegend(0.865,0.55, 1.0, 0.90);
    lg->SetBorderSize(0);  lgOther->SetBorderSize(0);   //No frame
    lg->SetFillStyle(0);   lgOther->SetFillStyle(0);    //No background fill

    for (int n=0; n!=3; ++n) {
      //Normalize histograms by the sum of all ptcls and add to stack
      h_gamma[n]->Divide(h_all[n]);	hstack[n]->Add(h_gamma[n]);
      h_gpi0[n]->Divide( h_all[n]);	hstack[n]->Add(h_gpi0[n] );
      h_pi[n]->Divide(   h_all[n]);	hstack[n]->Add(h_pi[n]   );
      h_K[n]->Divide(    h_all[n]);	hstack[n]->Add(h_K[n]    );
      h_K0S[n]->Divide(  h_all[n]);	hstack[n]->Add(h_K0S[n]  );
      h_K0L[n]->Divide(  h_all[n]);	hstack[n]->Add(h_K0L[n]  );
      h_L[n]->Divide(    h_all[n]);	hstack[n]->Add(h_L[n]    );
      h_n[n]->Divide(    h_all[n]);	hstack[n]->Add(h_n[n]    );
      h_e[n]->Divide(    h_all[n]);	hstack[n]->Add(h_e[n]    );
      h_mu[n]->Divide(   h_all[n]);	hstack[n]->Add(h_mu[n]   );
      h_nu[n]->Divide(   h_all[n]);	hstack[n]->Add(h_nu[n]   );
      h_p[n]->Divide(    h_all[n]);	hstack[n]->Add(h_p[n]    );
      //Normalize the Xi, Sigma & Omega histos by the "other particles" histo
      h_Xi0[n]->Divide( h_other[n]);	hstackOther[n]->Add(h_Xi0[n]);
      h_Xim[n]->Divide( h_other[n]);	hstackOther[n]->Add(h_Xim[n]);
      h_Sig0[n]->Divide(h_other[n]);	hstackOther[n]->Add(h_Sig0[n]);
      h_Sigm[n]->Divide(h_other[n]);	hstackOther[n]->Add(h_Sigm[n]);
      h_Sigp[n]->Divide(h_other[n]);	hstackOther[n]->Add(h_Sigp[n]);
      h_Om[n]->Divide( h_other[n]);	hstackOther[n]->Add(h_Om[n]);
      //Finally divide the "other particles" histo
      h_other[n]->Divide(h_all[n]);	hstack[n]->Add(h_other[n]);
    }
    //Legend setup
    lg->AddEntry(h_other[0], " #font[12]{Other}",     "f");
    lg->AddEntry(h_p[0],     " #font[12]{p}",         "f");
    lg->AddEntry(h_nu[0],    " #nu",                  "f");
    lg->AddEntry(h_mu[0],    " #mu",                  "f");
    lg->AddEntry(h_e[0],     " #font[12]{e^{#pm}}",   "f");
    lg->AddEntry(h_n[0],     " #font[12]{n}",         "f");
    lg->AddEntry(h_L[0],     " #Lambda",              "f");
    lg->AddEntry(h_K0L[0],   " #font[12]{K^{0}_{L}}", "f");
    lg->AddEntry(h_K0S[0],   " #font[12]{K^{0}_{S}}", "f");
    lg->AddEntry(h_K[0],     " #font[12]{K^{#pm}}",   "f");
    lg->AddEntry(h_pi[0],    " #pi^{#pm}",            "f");
    lg->AddEntry(h_gpi0[0],  " #gamma(#pi^{0}) ",     "f");
    lg->AddEntry(h_gamma[0], " #gamma",               "f");
    //The legend for separate "other particles" plot
    lgOther->AddEntry(h_Om[0],   " #Omega^{-}", "f");
    lgOther->AddEntry(h_Sigp[0], " #Sigma^{+}", "f");
    lgOther->AddEntry(h_Sigm[0], " #Sigma^{-}", "f");
    lgOther->AddEntry(h_Sig0[0], " #Sigma^{0}", "f");
    lgOther->AddEntry(h_Xim[0],  " #Xi^{-}",    "f");
    lgOther->AddEntry(h_Xi0[0],  " #Xi^{0}",    "f");

    //Plots
    for (int n=0; n!=3; ++n) {
      //General particle composition histograms
      canv->cd(n+1);
      hstack[n]->Draw("HISTO");
      hstack[n]->GetYaxis()->SetRange(0.0,1.0);
      hstack[n]->GetXaxis()->SetTitle("#font[12]{E}_{jet}^{gen} [GeV]");
      hstack[n]->GetYaxis()->SetTitle("Fraction of jet #font[12]{E}");
      hstack[n]->GetYaxis()->SetTitleFont(133);
      hstack[n]->GetYaxis()->SetTitleSize(20);
      hstack[n]->GetXaxis()->SetTitleFont(133);
      hstack[n]->GetXaxis()->SetTitleSize(20);
      hstack[n]->GetXaxis()->SetTitleOffset(1.8);
      hstack[n]->GetYaxis()->SetTitleOffset(2.0);
      lg->Draw();
      //More specific "other particles" histograms
      canv->cd(n+1+3);
      hstackOther[n]->Draw("HISTO");
      hstackOther[n]->GetYaxis()->SetRange(0.0,1.0);
      hstackOther[n]->GetXaxis()->SetTitle("#font[12]{E}_{jet}^{gen} [GeV]");
      hstackOther[n]->GetYaxis()->SetTitle("Fraction of #font[12]{other}");
      hstackOther[n]->GetYaxis()->SetTitleFont(133);
      hstackOther[n]->GetYaxis()->SetTitleSize(20);
      hstackOther[n]->GetXaxis()->SetTitleFont(133);
      hstackOther[n]->GetXaxis()->SetTitleSize(20);
      hstackOther[n]->GetXaxis()->SetTitleOffset(1.8);
      hstackOther[n]->GetYaxis()->SetTitleOffset(2.0);
      lgOther->Draw();
    }

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
    delete lg;  delete lgOther; delete lg_c;
    for (int i=0; i!=3; ++i) {
      delete h_e[i];    delete h_mu[i];    delete h_gamma[i]; delete h_gpi0[i];
      delete h_K[i];    delete h_K0S[i];   delete h_K0L[i];   delete h_L[i];
      delete h_n[i];    delete h_nu[i];    delete h_pi[i];
      delete h_p[i];    delete h_other[i]; delete h_Xi0[i];   delete h_Xim[i];
      delete h_Sig0[i]; delete h_Sigm[i];  delete h_Sigp[i];
      delete h_Om[i];   delete h_all[i];
      delete hstack[i]; delete hstackOther[i];
    }
    delete canv; delete canv_c;
    delete h_all_c; delete h_ch_c; delete h_nh_c; delete h_gamma_c;
    delete hstack_c;
  } //Particle composition histograms

  cout << endl << "Event cuts:" << endl;
  cout << "Total number of events: " << nentries << endl;
  cout << "Tag muon cuts:          " << mumuCut  << endl;
  cout << "Z invariant mass:       " << invM     << endl;
  cout << "Tag eta pT:             " << tagCut   << endl;
  cout << "Alpha:                  " << alpha    << endl;
  cout << "Probe eta pT:           " << probeCut << endl;
  cout << "Btb tag-probe:          " << b2b      << endl << endl;

  /*
  //ZSmear plot
  TCanvas *c5   = new TCanvas("c5","c5",500,500);
  ZSmear->SetAxisRange(0.99,1.01,"Y");
  c5->SetLogx();
  ZSmear->Draw();
  string savename2 = "./plots/ZSmear.eps";
  c5->Print(savename2.c_str());
  */

  //Charged hadron efficiency Profile
  TCanvas *c4     = new TCanvas("c4","c4",500,500);
  TGraph *PFeff   = new TGraph("data_and_MC_input/Efficiency/hadron_efficiency_PF.txt");
  TGraph *PFeffFr = new TGraph("data_and_MC_input/Efficiency/hadron_efficiencyPlusFakerate_PF.txt");
  TGraph *TRKeff  = new TGraph("data_and_MC_input/Efficiency/hadron_efficiency_TRK.txt");
  PFeff->SetMarkerStyle(kFullCircle ); TRKeff->SetMarkerStyle(kOpenCircle );
  PFeff->SetMarkerColor(kRed);         TRKeff->SetMarkerColor(kBlue+1);
  PFeffFr->SetMarkerStyle(kFullDiamond );
  PFeffFr->SetMarkerColor(kGreen+2);

  TLegend* lgEff = new TLegend(0.5,0.2,0.8,0.40 );
  lgEff->AddEntry(PFeff,  "#font[132]{PF eff}", "p");
  lgEff->AddEntry(PFeffFr,"#font[132]{PF eff+fakerate}", "p");
  lgEff->AddEntry(TRKeff, "#font[132]{TRK eff}", "p");
  lgEff->SetBorderSize(0); lgEff->SetFillStyle(0);
   
  TAxis *PFaxis = PFeff->GetXaxis();
  PFaxis->SetLimits(0.1,300);
  PFeff->GetHistogram()->SetMaximum(1.1);
  PFeff->GetHistogram()->SetMinimum(0.4);
  PFeff->Draw("ap"); lgEff->Draw("same");
  PFeffFr->Draw("samep"); TRKeff->Draw("samep");

  gStyle->SetOptStat();
  c4->SetLogx();
  chhEff->Draw("same");
  chhEff->SetLineColor(kBlack);
  chhEff->SetLineWidth(2);
  string savename = "./plots/Efficiency/trkEff.eps";
  c4->Print(savename.c_str());

  //Save CMSJES TTree
  fout->Write();
  fout->Close();

  //Free single-particle response functions from memory
  delete fr_h; delete fr_hcal;
} //CMSJES::Loop

void CMSJES::Response(int pdgid, double pseudorap, double energy, double pT, double Rt, 
                      double Bfield, TF1* frH, double& retMC, double& retH, double& retEHE,
                      double& retHHe)
{
  retMC = 0.0; retH = 0.0; retEHE = 0.0; retHHe = 0.0;
  bool zero = false; //If true, return zero responses

  double sfCh  = 0.0; //Charged particle step function
  double sfN   = 0.0; //Neutral hadron step function

  //Responses for different groups
  double cat1 = 0.0;     double cat2 = 0.0;     double cat3 = 0.0;
  double cat1_EHE = 0.0; double cat2_EHE = 0.0; double cat3_EHE = 0.0;
  double cat1_HHe = 0.0; double cat2_HHe = 0.0; double cat3_HHe = 0.0;

  double respPi_EHE = 0.0; double respPi_HHe = 0.0; //Pion responses for ECAL and HCAL

  if (pT > 0.3) sfCh = 1.0; // Step function for charged particles // 0.2 ?
  if (pT > 3.0) sfN  = 1.0; // Step function for neutral particles  

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
    respPi_EHE = frH->Eval(energy);

    frH->SetParameters(params_pi_HHe[row][0], params_pi_HHe[row][1], //HHe
                       params_pi_HHe[row][2], 1, 0, 1); 
    respPi_HHe = frH->Eval(energy);
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

    //EHE responses
    cat1_EHE = cat1*(respPi_EHE/(0.45*respPi_HHe + 0.55*respPi_EHE));
    cat2_EHE = cat2*(respPi_EHE/(0.45*respPi_HHe + 0.55*respPi_EHE));
    cat3_EHE = cat3*(respPi_EHE/(0.45*respPi_HHe + 0.55*respPi_EHE));

    //HHe responses
    cat1_HHe = cat1*(respPi_HHe/(0.45*respPi_HHe + 0.55*respPi_EHE));
    cat2_HHe = cat2*(respPi_HHe/(0.45*respPi_HHe + 0.55*respPi_EHE));
    cat3_HHe = cat3*(respPi_HHe/(0.45*respPi_HHe + 0.55*respPi_EHE));

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
      case -2112 : //nbar
      case   310 : //K^0_S
      case   130 : //K^0_L
      case   211 : //pi+
      case  -211 : //pi-
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = cat1*sfN; 
        retH   = cat1*sfN;
        retEHE = cat1_EHE*sfN;
        retHHe = cat1_HHe*sfN;
        break;

      //cat2: neutron, antiproton, K+, K-
      case  2112 : //n
      case -2212 : //pbar
      case   321 : //K+
      case  -321 : //K-
      case -3122 : //anti-Lambda
      case -3212 : //anti-Sigma^0
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = cat2*sfN; 
        retH   = cat2*sfN;
        retEHE = cat2_EHE*sfN;
        retHHe = cat2_HHe*sfN;
        break;

      //cat3: proton
      case  2212 : //p
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
        retH   = cat3*sfN;
        retEHE = cat3_EHE*sfN;
        retHHe = cat3_HHe*sfN;
        break;

      default : 
        zero=true;
        cout << "Unknown particle PDG: " << pdgid << endl;
        continue;	 
    }
  } 

  //Set results. Check for NaN and negative results if positive demanded
  if (zero || isnan(retMC) || retMC  <=0) retMC  =0;
  if (zero || isnan(retH)  || retH   <=0) retH   =0;
  if (zero || isnan(retH)  || retEHE <=0) retEHE =0;
  if (zero || isnan(retH)  || retHHe <=0) retHHe =0;

} //Response

/*
//-----------------------------------------------------------------------------
//A function to calculate the MC hadron response R 
//Params:	id		Particle PDGID
//		pseudorap	Particle pseudorapidity eta
//		energy		Particle energy
//		pT		Particle transverse momentum
//		frH		Pointer to hadron response
//              ret**		References where to put the resulting response
void CMSJES::Response(int pdgid, double pseudorap, double energy, double pT, double Rt, 
                      double Bfield, TF1* frH, double& retMC, double& retH, double& retEHE,
                      double& retHHe)
{
  retMC = 0.0; retH = 0.0; retEHE = 0.0; retHHe = 0.0;
  bool zero = false; //If true, return zero responses

  double sfCh  = 0.0; //Charged particle step function
  double sfN   = 0.0; //Neutral hadron step function
  double cat1 = 0.0; double cat2 = 0.0; double cat3 = 0.0; //Responses for different groups
  double respPiEH = 0.0; double respPiH = 0.0; //Pion responses for ECAL and HCAL

  if (pT > 0.2) sfCh = 1.0; // Step function for charged particles
  if (pT > 3.0) sfN  = 1.0; // Step function for neutral particles 

  //Check if particle outside good eta region
  if (fabs(pseudorap) > 5.2) zero = true;

  unsigned int row = int(fabs(pseudorap)*10); //Param matrix row from |eta|

  //Assert there's no pi^0 (PDGID 111) or eta (221) after parton shower
  if (abs(pdgid)==111 || abs(pdgid)==221) { 
    cout << "WARNING: pi^0 (111) or eta (221) found! PDGID: " << pdgid
         << "Returning zero response" << endl; zero = true;
  }

  //Charged particle not reaching calorimeters
  //if ( fabs(Charge(pdgid)) && (pT < 0.3*Bfield*0.5*Rt) ) zero = true;

  //Neutrino responses are zero
  if (isNeutrino(abs(pdgid))) zero = true;



  //CALCULATE RESPONSES
  for (int i_r=0; i_r<(zero?0:1); ++i_r) {



    //cat1: antineutron, K0S, K0L, pi+, pi-
    frH->SetParameters(params_cat1[row][0], params_cat1[row][1], params_cat1[row][2]);
    cat1 = frH->Eval(energy);


    //cat2: neutron, antiproton, K+, K-
    frH->SetParameters(params_cat2[row][0], params_cat2[row][1], params_cat2[row][2]);
    cat2 = frH->Eval(energy);

    //cat3: proton
    frH->SetParameters(params_cat3[row][0], params_cat3[row][1], params_cat3[row][2]);
    cat3 = frH->Eval(energy);

    frH->SetParameters(params_pi_EHE[row][0], params_pi_EHE[row][1], params_pi_EHE[row][2]);
    respPiEH = frH->Eval(energy);

    frH->SetParameters(params_pi_HHe[row][0], params_pi_HHe[row][1], params_pi_HHe[row][2]);
    respPiH = frH->Eval(energy);

    //step function cut
    retEHE = respPiEH*sfN;
    retHHe  = respPiH*sfN; 

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
      case -2112 : //an
        retMC = cat1*sfN;
        retH  = cat1*sfN;
        frH->SetParameters(params_an_EHE[row][0], params_an_EHE[row][1], params_an_EHE[row][2]);
        retEHE = frH->Eval(energy)*sfN;
        frH->SetParameters(params_an_HHe[row][0], params_an_HHe[row][1], params_an_HHe[row][2]);
        retHHe = frH->Eval(energy)*sfN;
        break;

      case   310 : //K^0_S
        retMC = cat1*sfN; 
        retH  = cat1*sfN;
        frH->SetParameters(params_K0S_EHE[row][0],params_K0S_EHE[row][1],params_K0S_EHE[row][2]);
        retEHE = frH->Eval(energy)*sfN;
        frH->SetParameters(params_K0S_HHe[row][0],params_K0S_HHe[row][1],params_K0S_HHe[row][2]);
        retHHe = frH->Eval(energy)*sfN;
        break;

      case   130 : //K^0_L
        retMC = cat1*sfN; 
        retH  = cat1*sfN;
        frH->SetParameters(params_K0L_EHE[row][0],params_K0L_EHE[row][1],params_K0L_EHE[row][2]);
        retEHE = frH->Eval(energy)*sfN;
        frH->SetParameters(params_K0L_HHe[row][0],params_K0L_HHe[row][1],params_K0L_HHe[row][2]);
        retHHe = frH->Eval(energy)*sfN;
        break;

      case   211 : //pi+
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = cat1*sfN; 
        retH = cat1*sfN;
        break;

      case  -211 : //pi-
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = cat1*sfN; 
        retH = cat1*sfN;
        break;

      //cat2: neutron, antiproton, K+, K-
      case  2112 : //n
        retMC = cat2*sfN; 
        retH  = cat2*sfN;
        frH->SetParameters(params_n_EHE[row][0], params_n_EHE[row][1], params_n_EHE[row][2]);
        retEHE = frH->Eval(energy)*sfN;
        frH->SetParameters(params_n_HHe[row][0], params_n_HHe[row][1], params_n_HHe[row][2]);
        retHHe = frH->Eval(energy)*sfN;
        break;

      case -2212 : //ap
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = cat2*sfN; 
        retH = cat2*sfN;
        frH->SetParameters(params_ap_EHE[row][0], params_ap_EHE[row][1], params_ap_EHE[row][2]);
        retEHE = frH->Eval(energy)*sfN;
        frH->SetParameters(params_ap_HHe[row][0], params_ap_HHe[row][1], params_ap_HHe[row][2]);
        retHHe = frH->Eval(energy)*sfN;
        break;

      case   321 : //K+
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = cat2*sfN; 
        retH = cat2*sfN;
        frH->SetParameters(params_Kp_EHE[row][0], params_Kp_EHE[row][1], params_Kp_EHE[row][2]);
        retEHE = frH->Eval(energy)*sfN;
        frH->SetParameters(params_Kp_HHe[row][0], params_Kp_HHe[row][1], params_Kp_HHe[row][2]);
        retHHe = frH->Eval(energy)*sfN;
        break;

      case  -321 : //K-
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = cat2*sfN; 
        retH = cat2*sfN;
        frH->SetParameters(params_Km_EHE[row][0], params_Km_EHE[row][1], params_Km_EHE[row][2]);
        retEHE = frH->Eval(energy)*sfN;
        frH->SetParameters(params_Km_HHe[row][0], params_Km_HHe[row][1], params_Km_HHe[row][2]);
        retHHe = frH->Eval(energy)*sfN;
        break;

      case -3122 : //anti-Lambda
      case -3212 : //anti-Sigma^0
        retMC = cat2*sfN; 
        retH  = cat2*sfN;
        break;

      //cat3: proton
      case  2212 : //p
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = cat3*sfN; 
        retH = cat3*sfN;
        frH->SetParameters(params_p_EHE[row][0], params_p_EHE[row][1], params_p_EHE[row][2]);
        retEHE = frH->Eval(energy)*sfN;
        frH->SetParameters(params_p_HHe[row][0], params_p_HHe[row][1], params_p_HHe[row][2]);
        retHHe = frH->Eval(energy)*sfN;
        break;

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
        retH = cat3*sfN;
        break;

      default : 
        zero=true;
        cout << "Unknown particle PDG: " << pdgid << endl;
        continue;	 
    }
  } 

  //Set results. Check for NaN and negative results if positive demanded
  if (zero || isnan(retMC)  || retMC <=0 ) retMC  =0;
  if (zero || isnan(retH)   || retH  <=0 ) retH   =0;
  if (zero || isnan(retH)   || retH  <=0 ) retEHE =0;
  if (zero || isnan(retH)   || retH  <=0 ) retHHe =0;

} //Response
*/





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
  if      (ReadName.find("P6")!=string::npos) {zjFile="P6_";}
  else if (ReadName.find("P8")!=string::npos) {zjFile="P8_";}
  else if (ReadName.find("H7")!=string::npos) {zjFile="H7_";}
  zjFile += "Zjet_";

  //#events
  if      (ReadName.find("600000") !=string::npos) num = "600000";
  else if (ReadName.find("300000") !=string::npos) num = "300000";
  else if (ReadName.find("100000") !=string::npos) num = "100000";
  else if (ReadName.find("30000")  !=string::npos) num = "30000";
  else if (ReadName.find("10000")  !=string::npos) num = "10000";
  else if (ReadName.find("3000")   !=string::npos) num = "3000";
  else {
    cout << "ERROR: fitting not supported for this file" << endl;
    return;
  }
  //The resulting filenames. Suffix ".root" to be added in CMSJES constructor
  zjFile += num;
}

//-----------------------------------------------------------------------------
void CMSJES::plotPT(int gen, int Nevt)
{

  //Choose filenames to open
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);

  //Initialize histograms and open ROOT files and fetch the stored objects
  TFile* fzj = TFile::Open(zjetFile.c_str()); // Z+jet file
  TProfile* przj_pTp=0;

  /* Z+jet */
  fzj->GetObject("prpTbal", przj_pTp);	//Z+jet response pTp
  TH1D* hzj_pTp = przj_pTp->ProjectionX();

  hzj_pTp->SetLineColor(kBlack);

  //CMS MC points
  TGraphErrors* mc_zj_pTbal_2018 = new TGraphErrors(nMC_pTbal2018,zj_MC_pTp_pTbal_2018,
                                                    zj_MC_pTbal_2018,0,zj_MC_pTbal_ER_2018);


  mc_zj_pTbal_2018->SetMarkerStyle(kOpenCircle);  mc_zj_pTbal_2018->SetMarkerColor(kGreen+2);

  //Savefile name setup
  string savename = "P8_Zjet";
  savename+=".eps";

  //Canvas and dividing it into pads.
  //TPad: name,title,xlow,ylow,xup,yup,color
  TCanvas* canv = new TCanvas("canvas","canvas",500,350/*500*/);
  TPad *pad1 = new TPad("pad1","",0,0/*0.3*/,1,1,0);	//Modify for 1 or 2 pads
  pad1->Draw();

  //General plot setup
  pad1->SetLeftMargin(0.12);	//Room for axis label
  pad1->SetBottomMargin(0.115);
  pad1->cd();			//Go to pad1
  pad1->SetLogx();		//Logarithmic horizontal axis
  hzj_pTp->GetXaxis()->SetRangeUser(10,1000);

  //Suppress stat boxes
  hzj_pTp->SetStats(0);

  //Axis setup. New dummy TH1 for easy usage in multiple plots
  TH1D* setup = new TH1D("setup",""/*hdj->GetTitle()*/, hzj_pTp->GetXaxis()->GetNbins(),
			 hzj_pTp->GetXaxis()->GetXmin(),    hzj_pTp->GetXaxis()->GetXmax());
  setup->SetStats(0);				//Suppress stat box
  setup->GetXaxis()->SetTitle(hzj_pTp->GetXaxis()->GetTitle());
  setup->GetYaxis()->SetTitle(hzj_pTp->GetYaxis()->GetTitle());
  setup->SetAxisRange(0.6, 1.0,"Y");		//Vertical axis limits
  setup->GetYaxis()->SetTitleFont(133);
  int titleSize = 18;				//Common title size everywhere
  setup->GetYaxis()->SetTitleSize(titleSize);
  setup->GetXaxis()->SetMoreLogLabels();
  setup->GetXaxis()->SetNoExponent();
  setup->GetXaxis()->SetTitleFont(133);
  setup->GetXaxis()->SetTitleSize(titleSize);
  setup->GetXaxis()->SetTitleOffset(0.9);
  setup->GetYaxis()->SetTitleOffset(0.9);
  
  //Separate legends for dijet and gamma+jet
  double lz_horiz[2] = {0.67, 0.89};
  double vertical[2] = {0.13, 0.37};
  vertical[1] -= 0.12;
  vertical[0] += 0.06;

  TLegend* lz = new TLegend(lz_horiz[0],vertical[0],lz_horiz[1],vertical[1]);
  lz->SetBorderSize(0);	//No box around legend
  lz->SetFillStyle( 0);	//No background fill

  lz->AddEntry(mc_zj_pTbal_2018,"#font[132]{CMS Z+jet MC}", "p");
  lz->AddEntry(hzj_pTp,  "#font[132]{Our Z+jet MC pTp-bins}", "l");

  //Main plot
  setup->Draw();
  hzj_pTp->Draw("SAME");
  mc_zj_pTbal_2018->Draw("P SAME");
  lz->Draw();	//Legends

  //Save plot
  string saveTo = "./plots/pT-bal/";
  saveTo = saveTo + "MC_" + savename;

  canv->Print(saveTo.c_str());

  //Free memory
  delete setup;  delete lz;   delete pad1;   delete canv;
} //plotPT

//-------------------------------------------------------------------------------------------
void CMSJES::plotMPF(int gen, int Nevt)
{
  //Choose filenames to open
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);

  //Initialize histograms and open ROOT files and fetch the stored objects
  //  TH1 params: name, title, #bins, #lowlimit, #highlimit
  TFile* fzj = TFile::Open(zjetFile.c_str());
  TProfile *przj_MPF=0; TProfile *przj_MPFb=0; TProfile *przj_MPFg=0; TProfile *przj_MPFlq=0;

  /* 1: Z+jet */
  fzj->GetObject("prMPF" ,przj_MPF);
  fzj->GetObject("prMPFb" ,przj_MPFb);
  fzj->GetObject("prMPFg" ,przj_MPFg);
  fzj->GetObject("prMPFlq" ,przj_MPFlq);
  TH1D* hzj_MPF   = przj_MPF  ->ProjectionX();
  TH1D* hzj_MPFb  = przj_MPFb ->ProjectionX();
  TH1D* hzj_MPFg  = przj_MPFg ->ProjectionX();
  TH1D* hzj_MPFlq = przj_MPFlq->ProjectionX();

  //CMS MC points
  TGraphErrors* mc_zj_MPFntI_2018 = new TGraphErrors(nMC_MPFntI,zj_MC_pTp_MPFntI_2018,
                                                    zj_MC_MPFntI_2018,0,zj_MC_MPFntI_ER_2018);

  //Canvas
  TCanvas* canv_MPF = new TCanvas("MPF","",600,600);
  canv_MPF->SetLeftMargin(0.13);
  canv_MPF->SetBottomMargin(0.13);

  hzj_MPF ->SetLineColor( kBlack);
  hzj_MPF  ->SetMarkerStyle(kFullCircle);       hzj_MPF  ->SetMarkerColor(kBlack);
  hzj_MPFb ->SetMarkerStyle(kFullSquare);       hzj_MPFb ->SetMarkerColor(kRed  );
  hzj_MPFg ->SetMarkerStyle(kFullTriangleUp);   hzj_MPFg ->SetMarkerColor(kBlue+1);
  hzj_MPFlq->SetMarkerStyle(kFullTriangleDown); hzj_MPFlq->SetMarkerColor(kGreen+2);
  hzj_MPFb->SetLineColor(kRed+1);               hzj_MPFg->SetLineColor(kBlue+1);
  hzj_MPFlq->SetLineColor(kGreen+2);


  mc_zj_MPFntI_2018->SetMarkerStyle( kOpenSquare); mc_zj_MPFntI_2018->SetMarkerColor( kBlack  );


  //Legend
  TLegend* lz_MPF = new TLegend(0.62,0.70,0.89,0.89);
  lz_MPF->SetBorderSize(0);
  lz_MPF->AddEntry(hzj_MPF, "All jets"  , "p");
  //lz_MPF->AddEntry(hzj_MPFb, "b-jets"    , "p");
  //lz_MPF->AddEntry(hzj_MPFg, "gluon jets", "p");
  //lz_MPF->AddEntry(hzj_MPFlq, "lq-jets"   , "p");
  lz_MPF->AddEntry(mc_zj_MPFntI_2018, "#font[132]{FullSim 2018ABCD}", "p");

  //Title and axis setup
  hzj_MPF->SetStats(0); //Suppress stat box
  hzj_MPF->SetTitle("");
  //hzj_MPF->SetLineWidth(2);
  hzj_MPF->SetAxisRange(0.82,1.02,"Y"); //Vertical axis limits

  hzj_MPF->GetYaxis()->SetTitleFont(133);
  int titleSize = 24; //Common title size everywhere
  hzj_MPF->GetYaxis()->SetTitleSize(titleSize);
  hzj_MPF->GetXaxis()->SetMoreLogLabels();
  hzj_MPF->GetXaxis()->SetNoExponent();
  hzj_MPF->GetXaxis()->SetTitleFont(133);
  hzj_MPF->GetXaxis()->SetTitleSize(titleSize);
  canv_MPF->SetLogx();
  hzj_MPF->GetYaxis()->SetTitleOffset(1.5);
  hzj_MPF->GetXaxis()->SetTitleOffset(1);
  gPad->SetTickx();   gPad->SetTicky();

  //Savefile name setup
  string savename = "./plots/mpf/MPF_zmmjet";
  string MPFtitle = hzj_MPF->GetTitle();
  savename+=".eps";

  //Plot
  hzj_MPF->Draw("p");
  mc_zj_MPFntI_2018->Draw("P SAME");
  //hzj_MPFb->Draw("SAMEP");
  //hzj_MPFg->Draw("SAMEP");
  //hzj_MPFlq->Draw("SAMEP");
  lz_MPF->Draw();

  //Save plot
  canv_MPF->Print(savename.c_str());
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

  //For computing the difference between b and g responses:
  //-------------------------------------------------------------------------------
  TH1D* h_diffbg  = (TH1D*)hzj_Rjetb->Clone("h_diffbg");
  TH1D* h_diffudg = (TH1D*)hzj_Rjetud->Clone("h_diffudg");
  TH1D* h_diffsg  = (TH1D*)hzj_Rjets->Clone("h_diffsg");
  TH1D* h_diffcg  = (TH1D*)hzj_Rjetc->Clone("h_diffcg");

  h_diffbg->Add(hzj_Rjetg,-1);  //b-response - g-response
  h_diffudg->Add(hzj_Rjetg,-1); //ud-response - g-response
  h_diffsg->Add(hzj_Rjetg,-1);  //s-response - g-response
  h_diffcg->Add(hzj_Rjetg,-1);  //c-response - g-response

  TGraph *diff_b  = new TGraph("data_and_MC_input/Response/diffToGluon_b.txt" );
  TGraph *diff_s  = new TGraph("data_and_MC_input/Response/diffToGluon_s.txt" );
  TGraph *diff_c  = new TGraph("data_and_MC_input/Response/diffToGluon_c.txt" );
  TGraph *diff_ud = new TGraph("data_and_MC_input/Response/diffToGluon_ud.txt");
  diff_b ->SetMarkerStyle(kFullCircle);  diff_b ->SetMarkerColor(kBlue+1);
  diff_s ->SetMarkerStyle(kFullCircle); diff_s ->SetMarkerColor(kOrange+1);
  diff_c ->SetMarkerStyle(kFullCircle); diff_c ->SetMarkerColor(kGreen+1);
  diff_ud->SetMarkerStyle(kFullCircle); diff_ud->SetMarkerColor(kRed+1);

  h_diffbg ->SetMarkerStyle(kOpenSquare); h_diffbg ->SetMarkerColor(kBlue+1);
  h_diffudg->SetMarkerStyle(kOpenSquare); h_diffudg->SetMarkerColor(kRed+1 );
  h_diffsg->SetMarkerStyle(kOpenSquare);  h_diffsg->SetMarkerColor(kOrange+1);
  h_diffcg->SetMarkerStyle(kOpenSquare);  h_diffcg->SetMarkerColor(kGreen+1 );
  h_diffbg ->SetLineColor(kBlue+1);       h_diffudg->SetLineColor(kRed+1 );
  h_diffsg ->SetLineColor(kOrange+1);       h_diffcg->SetLineColor(kGreen+1 );

  TCanvas* canv_diffg = new TCanvas("canv_diffg","",600,600);
  canv_diffg->SetLeftMargin(0.15);
  canv_diffg->SetBottomMargin(0.13);

  //Legend
  TLegend* lz_diffg = new TLegend(0.62,0.60,0.89,0.89);
  lz_diffg->SetBorderSize(0);
  lz_diffg->AddEntry(h_diffbg,  "b");
  lz_diffg->AddEntry(h_diffudg, "ud");
  lz_diffg->AddEntry(h_diffsg,  "s");
  lz_diffg->AddEntry(h_diffcg,  "c");
  lz_diffg->AddEntry(diff_b,  "b (PF)", "p");
  lz_diffg->AddEntry(diff_ud, "ud (PF)", "p");
  lz_diffg->AddEntry(diff_s,  "s (PF)", "p");
  lz_diffg->AddEntry(diff_c,  "c (PF)", "p");

  h_diffbg->GetYaxis()->SetTitle("Difference to gluon response");
  h_diffbg->GetXaxis()->SetTitle("p_{T}^{gen} [GeV]");
  h_diffbg->GetYaxis()->SetTitleOffset(1.9);
  h_diffbg->GetXaxis()->SetTitleOffset(1.2);

  h_diffbg->SetStats(0); //Suppress stat box
  h_diffbg->SetTitle("");
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
  canv_diffg->Print("./plots/Rjet/gluonDiff.eps");

  //-------------------------------------------------------------------------------

  //Canvas
  TCanvas* canv_Rjet = new TCanvas("canv_Rjet","",600,600);
  canv_Rjet->SetLeftMargin(0.15);
  canv_Rjet->SetBottomMargin(0.13);

  hzj_Rjet  ->SetMarkerStyle(kFullCircle);       hzj_Rjet  ->SetMarkerColor(kBlack);
  hzj_Rjetb ->SetMarkerStyle(kFullSquare);       hzj_Rjetb ->SetMarkerColor(kRed  );
  hzj_Rjetg ->SetMarkerStyle(kFullTriangleUp);   hzj_Rjetg ->SetMarkerColor(kBlue+1);
  hzj_Rjetlq->SetMarkerStyle(kFullTriangleDown); hzj_Rjetlq->SetMarkerColor(kGreen+2);
  hzj_Rjet  ->SetLineColor(kBlack);              hzj_Rjetb ->SetLineColor(kRed+1);
  hzj_Rjetg ->SetLineColor(kBlue+1);             hzj_Rjetlq->SetLineColor(kGreen+2);

  //Legend
  TLegend* lz_Rjet = new TLegend(0.58,0.2,0.89,0.40);
  lz_Rjet->SetBorderSize(0);
  lz_Rjet->AddEntry(hzj_Rjet,   "#font[132]{All jets}",   "p");
  lz_Rjet->AddEntry(hzj_Rjetb,  "#font[132]{b-jets}",     "p");
  lz_Rjet->AddEntry(hzj_Rjetg,  "#font[132]{gluon jets}", "p");
  lz_Rjet->AddEntry(hzj_Rjetlq, "#font[132]{lq-jets}",    "p");

  //Title and axis setup
  hzj_Rjet->SetStats(0); //Suppress stat box
  hzj_Rjet->SetTitle("");
  hzj_Rjet->SetAxisRange(0.8,1.0,"Y"); //Vertical axis limits

  //hzj_Rjet->GetYaxis()->SetTitleFont(133);
  //int titleSize = 20; //Common title size everywhere
  //hzj_Rjet->GetYaxis()->SetTitleSize(titleSize);
  hzj_Rjet->GetXaxis()->SetMoreLogLabels();
  hzj_Rjet->GetXaxis()->SetNoExponent();
  canv_Rjet->SetLogx();
  hzj_Rjet->GetYaxis()->SetTitleOffset(1.8);
  hzj_Rjet->GetXaxis()->SetTitleOffset(1.2);

  gPad->SetTickx(); gPad->SetTicky();

  //Savefile name setup
  string savename = "./plots/Rjet/Rjet";
  savename+=".eps";

  //Plot
  hzj_Rjet->Draw("P");
  hzj_Rjetb->Draw("SAMEP");
  hzj_Rjetg->Draw("SAMEP");
  hzj_Rjetlq->Draw("SAMEP");
  lz_Rjet->Draw("SAMEP");

  //Save plot
  canv_Rjet->Print(savename.c_str());
}

void CMSJES::plotF(int gen, int Nevt)
{
  //Choose filenames to open
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);

  //Initialize histograms and open ROOT files and fetch the stored objects
  TFile* fzj = TFile::Open(zjetFile.c_str());
  TProfile *przj_F=0; TProfile *przj_Fb=0; TProfile *przj_Fg=0; TProfile *przj_Flq=0;

  //Create F Histograms
  fzj->GetObject("prRjet"   ,przj_F  );
  fzj->GetObject("prRjetb"  ,przj_Fb );
  fzj->GetObject("prRjetg"  ,przj_Fg );
  fzj->GetObject("prRjetlq" ,przj_Flq);
  TH1D* hzj_F   = przj_F->ProjectionX();
  TH1D* hzj_Fb  = przj_Fb->ProjectionX();
  TH1D* hzj_Fg  = przj_Fg->ProjectionX();
  TH1D* hzj_Flq = przj_Flq->ProjectionX();

  hzj_Fb->Divide(hzj_F);
  hzj_Fg->Divide(hzj_F);
  hzj_Flq->Divide(hzj_F);

  //Canvas
  TCanvas* canv_F = new TCanvas("canv_F","",600,600);
  canv_F->SetLeftMargin(0.15);
  canv_F->SetBottomMargin(0.13);

  hzj_Fb ->SetMarkerStyle(kFullSquare);       hzj_Fb ->SetMarkerColor(kRed  );
  hzj_Fg ->SetMarkerStyle(kFullTriangleUp);   hzj_Fg ->SetMarkerColor(kBlue+1);
  hzj_Flq->SetMarkerStyle(kFullTriangleDown); hzj_Flq->SetMarkerColor(kGreen+2);
  hzj_Fb ->SetLineColor(kRed+1);              hzj_Fg ->SetLineColor(kBlue+1);
  hzj_Flq->SetLineColor(kGreen+2);

  //Legend
  TLegend* lz_F = new TLegend(0.58,0.2,0.89,0.40);
  lz_F->SetBorderSize(0);
  lz_F->AddEntry(hzj_Fb,  "#font[132]{b-jets}",     "p");
  lz_F->AddEntry(hzj_Fg,  "#font[132]{gluon jets}", "p");
  lz_F->AddEntry(hzj_Flq, "#font[132]{lq-jets}",    "p");

  //Title and axis setup
  hzj_Fb->SetStats(0); //Suppress stat box
  hzj_Fb->SetTitle("");
  hzj_Fb->SetAxisRange(0.9,1.1,"Y"); //Vertical axis limits

  //hzj_F->GetYaxis()->SetTitleFont(133);
  //int titleSize = 20; //Common title size everywhere
  //hzj_F->GetYaxis()->SetTitleSize(titleSize);
  hzj_Fb->GetXaxis()->SetTitle("p_{T,gen}^{jet}");
  hzj_Fb->GetYaxis()->SetTitle("F");
  hzj_Fb->GetXaxis()->SetMoreLogLabels();
  hzj_Fb->GetXaxis()->SetNoExponent();
  canv_F->SetLogx();
  hzj_Fb->GetYaxis()->SetTitleOffset(1.8);
  hzj_Fb->GetXaxis()->SetTitleOffset(1.2);

  gPad->SetTickx(); gPad->SetTicky();

  //Savefile name setup
  string savename = "./plots/F/F";
  savename+=".eps";

  //Plot
  hzj_Fb->Draw("P");
  hzj_Fg->Draw("SAMEP");
  hzj_Flq->Draw("SAMEP");
  lz_F->Draw("SAMEP");

  //Save plot
  canv_F->Print(savename.c_str());
}

void CMSJES::plotVariants(int gen, int Nevt)
{

  //Choose filenames to open
  //string nameAdd, zjetFile, str, str_varCp3, str_varCm3;
  //plotQuery(nameAdd, zjetFile, gen, Nevt);

  //Initialize histograms and open ROOT files and fetch the stored objects
  TFile* fzj     = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_30000.root");
  TFile* fzj_Cp3 = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_30000_varCp3.root");
  //TFile* fzj_Cm3 = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_30000_varCm3.root");

  //TFile* fzj     = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_10000.root");
  //TFile* fzj_Cp3 = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_10000_varCp3.root");
  //TFile* fzj_Cm3 = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_10000_varCm3.root");

  //TFile* fzj     = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_100000.root");
  //TFile* fzj_Cp3 = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_100000_varCp3.root");
  //TFile* fzj_Cm3 = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_100000_varCm3.root");


  // SPR +-3% JEC paper
  TGraph *JEC_SPRp3       = new TGraph("data_and_MC_input/Response/JEC_PFJet_SPRplus3.txt" );
  TGraph *JEC_SPRm3       = new TGraph("data_and_MC_input/Response/JEC_PFJet_SPRminus3.txt" );
  TGraph *JEC_calo_SPRp3  = new TGraph("data_and_MC_input/Response/JEC_Calo_SPRplus3.txt" );
  TGraph *JEC_calo_SPRm3  = new TGraph("data_and_MC_input/Response/JEC_Calo_SPRminus3.txt" );


  JEC_SPRp3->SetMarkerStyle(kOpenCircle);      JEC_SPRp3->SetMarkerColor(kRed);
  JEC_SPRm3->SetMarkerStyle(kOpenCircle);      JEC_SPRm3->SetMarkerColor(kBlue+1);
  JEC_calo_SPRp3->SetMarkerStyle(kOpenCircle); JEC_calo_SPRp3->SetMarkerColor(kOrange+1);
  JEC_calo_SPRm3->SetMarkerStyle(kOpenCircle); JEC_calo_SPRm3->SetMarkerColor(kGreen+2);


  TProfile *pr_Rjet=0;
  TProfile *pr_Rjet_Cp3=0;
  TProfile *pr_Rjet_Cm3=0;
  TProfile *pr_Rjet_calo=0;
  TProfile *pr_Rjet_calo_Cp3=0;
  TProfile *pr_Rjet_calo_Cm3=0;

  //Create Histograms
  fzj    ->GetObject("prRjet",pr_Rjet);
  fzj_Cp3->GetObject("prRjet",pr_Rjet_Cp3);
  //fzj_Cm3->GetObject("prRjet", pr_Rjet_Cm3);
  fzj    ->GetObject("prRjet_calo",pr_Rjet_calo);
  fzj_Cp3->GetObject("prRjet_calo",pr_Rjet_calo_Cp3);
  //fzj_Cm3->GetObject("prRjet_calo",pr_Rjet_calo_Cm3);

  TH1D* h_Rjet     = pr_Rjet->ProjectionX();
  TH1D* h_Rjet_Cp3 = pr_Rjet_Cp3->ProjectionX();
  //TH1D* h_Rjet_Cm3 = pr_Rjet_Cm3->ProjectionX();

  TH1D* h_Rjet_calo     = pr_Rjet_calo->ProjectionX();
  TH1D* h_Rjet_calo_Cp3 = pr_Rjet_calo_Cp3->ProjectionX();
  //TH1D* h_Rjet_calo_Cm3 = pr_Rjet_calo_Cm3->ProjectionX();

  h_Rjet_Cp3->Divide(h_Rjet);
  //h_Rjet_Cm3->Divide(h_Rjet);
  h_Rjet_calo_Cp3->Divide(h_Rjet_calo);
  //h_Rjet_calo_Cm3->Divide(h_Rjet_calo);

  //Canvas
  TCanvas* canv_var = new TCanvas("canv_var","",600,600);
  canv_var->SetLeftMargin(0.15);
  canv_var->SetBottomMargin(0.13);

  h_Rjet_Cp3->SetMarkerStyle(kFullSquare); h_Rjet_Cp3->SetMarkerColor(kRed);
  //h_Rjet_Cm3->SetMarkerStyle(kFullSquare); h_Rjet_Cm3->SetMarkerColor(kBlue+1);
  h_Rjet_Cp3->SetLineColor(kRed);          //h_Rjet_Cm3->SetLineColor(kBlue+1); 

  h_Rjet_calo_Cp3->SetMarkerStyle(kFullSquare); h_Rjet_calo_Cp3->SetMarkerColor(kOrange+1);
  //h_Rjet_calo_Cm3->SetMarkerStyle(kFullSquare); h_Rjet_calo_Cm3->SetMarkerColor(kGreen+2);
  h_Rjet_calo_Cp3->SetLineColor(kOrange+1);     //h_Rjet_calo_Cm3->SetLineColor(kGreen+2); 

  //Legend
  TLegend* lz_Rjet = new TLegend(0.2,0.7,0.7,0.89);
  lz_Rjet->SetNColumns(2);
  lz_Rjet->SetBorderSize(0);
  lz_Rjet->AddEntry(h_Rjet_Cp3,      "PF C + 3%",   "p");
  lz_Rjet->AddEntry(h_Rjet_calo_Cp3, "Calo C + 3% ",   "p");
  //lz_Rjet->AddEntry(h_Rjet_Cm3,      "PF C - 3%",   "p");
  //lz_Rjet->AddEntry(h_Rjet_calo_Cm3, "Calo C - 3%",   "p");
  lz_Rjet->AddEntry(JEC_SPRp3,       "PF C + 3% (JEC 8TeV)", "p");
  lz_Rjet->AddEntry(JEC_calo_SPRp3,  "Calo C + 3% (JEC 8TeV)", "p");
  lz_Rjet->AddEntry(JEC_SPRm3,       "PF C - 3% (JEC 8TeV)", "p");  
  lz_Rjet->AddEntry(JEC_calo_SPRm3,  "Calo C - 3% (JEC 8TeV)", "p"); 


  TH1D* setup = new TH1D("setup","", h_Rjet_Cp3->GetXaxis()->GetNbins(),
			 h_Rjet_Cp3->GetXaxis()->GetXmin(), h_Rjet_Cp3->GetXaxis()->GetXmax());

  //Title and axis setup
  setup->SetStats(0); //Suppress stat box
  setup->SetTitle("");
  setup->SetAxisRange(0.96,1.06,"Y"); //Vertical axis limits
  setup->GetXaxis()->SetMoreLogLabels();
  setup->GetXaxis()->SetNoExponent();
  canv_var->SetLogx();
  setup->GetYaxis()->SetTitleOffset(1.8);
  setup->GetXaxis()->SetTitleOffset(1.2);
  setup->GetYaxis()->SetTitle("Response ratio");
  setup->GetXaxis()->SetTitle("p_{T}^{gen} [GeV]");

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
  //h_Rjet_calo_Cm3->Draw("SAMEP");
  //h_Rjet_Cm3->Draw("SAMEP");
  h_Rjet_Cp3->Draw("SAMEP");
  lz_Rjet->Draw("SAMEP");

  //Save plot
  canv_var->Print(savename.c_str());
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
  cout << "Which generator to use? (1) P6 (2) H7 (3) P8" << endl;
  while (gen<1 || gen>3) cin >> gen;
  respStr += (gen==3 ? "P8_": (gen==1 ? "P6_": (gen==2 ? "H7_" : "")));

  //Set #events
  string num;
  cout << "#Events (0) 3k (1) 10k (2) 30k (3) 100k (4) 600k" << endl;
  while (Nevt<0 || Nevt>4) cin >> Nevt;
  if      (Nevt==0) num = "3000";
  else if (Nevt==1) num = "10000";
  else if (Nevt==2) num = "30000";
  else if (Nevt==3) num = "100000";
  else if (Nevt==4) num = "600000";

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
  dPhi = Charge(pdgid) * (TMath::Pi()/2 - TMath::ACos(Rt/(2*Rp)));

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

