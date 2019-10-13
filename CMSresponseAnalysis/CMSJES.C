#define CMSJES_cxx
#include "CMSJES.h"

//Created using ROOT TMakeClass

void CMSJES::Loop()
{
  srand(time(0)); //Set random seed

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
  string outname = "./output_ROOT_files/CMSJES_" + ReadName + ".root"; //Output file
  TFile *fout = new TFile(outname.c_str(),"RECREATE");

  int const nbins = 11;
  int const nbinsMPF = 15;

  //Check values
  const double binsx[nbins] = {31.75, 41.0, 50.5, 63.5, 82.75, 105.25, 132.5, 173.5, 
                               228.5, 299.5, 380.75}; 
  const double binsxMPF[nbinsMPF] = {31.75, 41.0, 50.5, 63.5, 83.0, 105.25, 132.5, 173.25,
                                    228.25, 300.0, 391.25, 503.75, 681.75, 951.5, 1258.25};

  string Rcone = "R_{cone}=0.4"; //R_cone info in string form
  R_cone = 0.4;

  string pTpTitle    = Rcone;
  string ETitle      = Rcone;
  string MPFTitle    = Rcone;

  string ctauStr; ctauStr = ", c#tau=1 cm";   //Time scale: C_tau -> 10mm
  ETitle+=ctauStr; pTpTitle+=ctauStr; MPFTitle+=ctauStr;

  pTpTitle += ";#font[132]{#font[12]{p}_{T,tag}^{MC} [GeV]}; p_{T}^{probe}/p_{T}^{tag}";
  TProfile* prpTbal    = new TProfile("prpTbal",     pTpTitle.c_str(), nbins-1, binsx);
  TProfile* prpTbalFit = new TProfile("prpTbalFit0", pTpTitle.c_str(), nbins-1, binsx);

  ETitle += ";E_{probe} [GeV];p_{T}^{probe}/p_{T}^{tag}";
  TProfile* prE   = new TProfile("prE",ETitle.c_str(),nbins-1,binsx);

  MPFTitle += ";#font[132]{#font[12]{p}_{T,tag}^{MC} [GeV]};R_{MPF}";
  TProfile* prMPF = new TProfile("prMPF", MPFTitle.c_str(), nbinsMPF-1, binsxMPF);

  //Jet flavour fraction histos: FFb = b-jets, FFg = g-jets, FFlq=(u,d,s,c)-jets
  TH1D* FFb = new TH1D("FFb",  "Jet flavour fraction;#font[12]{p}_{T} [GeV];",nbins-1,binsx);
  TH1D* FFg = new TH1D("FFg",  "Jet flavour fraction;#font[12]{p}_{T} [GeV];",nbins-1,binsx);
  TH1D* FFlq= new TH1D("FFlq", "Jet flavour fraction;#font[12]{p}_{T} [GeV];",nbins-1,binsx);
  TH1D* FFa = new TH1D("FFa", ";#font[12]{p}_{T} [GeV];",nbins-1,binsx); //All
  string FFstackTitle;
  FFstackTitle  = "#font[132]{Run CMS}";
  FFstackTitle += ";#font[12]{E'} #font[132]{[GeV]}";
  FFstackTitle += ";#font[132]{Jet flavour fraction}";
  THStack* FFstack = new THStack("", FFstackTitle.c_str());

  //Tprofiles for quark and gluon jets
  TProfile* prqjet = new TProfile("prq",ETitle.c_str(),nbins-1,binsx);
  TProfile* prgjet = new TProfile("prg",ETitle.c_str(),nbins-1,binsx);

  //TProfiles for derivatives of pTprobe/pTtag
  TProfile* dAf = new TProfile("dAf", "#partial_{A}p_{T}^{probe}/p_{T}^{tag}",nbins-1,binsx);
  TProfile* dBf = new TProfile("dBf", "#partial_{B}p_{T}^{probe}/p_{T}^{tag}",nbins-1,binsx);
  TProfile* dCf = new TProfile("dCf", "#partial_{C}p_{T}^{probe}/p_{T}^{tag}",nbins-1,binsx);

  //Flavour-dependent jet energy correction factors as a function of E'
  //1st index: F as a function of...
  //  [0] E' or pT', [1] MC reco jet pT, [2] gen jet pT w/o/ nu and mu
  //2nd index:
  //  [0] all jets for avg.ing, [1] b-jets, [2] g-jets, [3] lq-jets
  string Fstr[4][4];
  for (int i=0; i!=3; ++i) {
    for (int j=0; j!=4; ++j) {
      if      (i==0) Fstr[i][j]=";#font[132]{#font[12]{E'} [GeV]};";
      else if (i==1) Fstr[i][j]=";#font[132]{#font[12]{p}_{T}^{MC} [GeV]};";
      else if (i==2) Fstr[i][j]=";#font[132]{#font[12]{p}_{T}^{gen} [GeV]};";
      else if (i==3) Fstr[i][j]=";#font[132]{#font[12]{p'}_{T} [GeV]};";
    }
    Fstr[i][0] += "#font[12]{F}_{#font[132]{jet}}";
    Fstr[i][1] += "#font[12]{F}_{#font[132]{#font[12]{b}}}";
    Fstr[i][2] += "#font[12]{F}_{#font[132]{#font[12]{g}}}";
    Fstr[i][3] += "#font[12]{F}_{#font[132]{#font[12]{(u,d,s,c)}}}";
  }
  int nF=15; double FL=100; double FH=3100; //Point from 30 to 3050 every 100
  int nFr=10; double FHr=3000;

  //As a function of E'
  TProfile* Fjet     = new TProfile("Fjet",    Fstr[0][0].c_str(), nF, FL,FH );
  TProfile* Fb       = new TProfile("Fb",      Fstr[0][1].c_str(), nF, FL,FH );
  TProfile* Fg       = new TProfile("Fg",      Fstr[0][2].c_str(), nF, FL,FH );
  TProfile* Flq      = new TProfile("Flq",     Fstr[0][3].c_str(), nF, FL,FH );
  TProfile* FjetSU   = new TProfile("FjetSU",  Fstr[0][0].c_str(), nF, FL,FH );
  TProfile* FbSU     = new TProfile("FbSU",    Fstr[0][1].c_str(), nF, FL,FH );
  TProfile* FgSU     = new TProfile("FgSU",    Fstr[0][2].c_str(), nF, FL,FH );
  TProfile* FlqSU    = new TProfile("FlqSU",   Fstr[0][3].c_str(), nF, FL,FH );
  //As a function of pT'
  TProfile* Fjet_p   = new TProfile("Fjet_p",  Fstr[3][0].c_str(), nF, FL,FH );
  TProfile* Fb_p     = new TProfile("Fb_p",    Fstr[3][1].c_str(), nF, FL,FH );
  TProfile* Fg_p     = new TProfile("Fg_p",    Fstr[3][2].c_str(), nF, FL,FH );
  TProfile* Flq_p    = new TProfile("Flq_p",   Fstr[3][3].c_str(), nF, FL,FH );
  TProfile* FjetSU_p = new TProfile("FjetSU_p",Fstr[3][0].c_str(), nF, FL,FH );
  TProfile* FbSU_p   = new TProfile("FbSU_p",  Fstr[3][1].c_str(), nF, FL,FH );
  TProfile* FgSU_p   = new TProfile("FgSU_p",  Fstr[3][2].c_str(), nF, FL,FH );
  TProfile* FlqSU_p  = new TProfile("FlqSU_p", Fstr[3][3].c_str(), nF, FL,FH );
  //As a function of reconstructed jet pT
  TProfile* Fjet_r   = new TProfile("Fjet_r",  Fstr[1][0].c_str(), nFr,FL,FHr);
  TProfile* Fb_r     = new TProfile("Fb_r",    Fstr[1][1].c_str(), nFr,FL,FHr);
  TProfile* Fg_r     = new TProfile("Fg_r",    Fstr[1][2].c_str(), nFr,FL,FHr);
  TProfile* Flq_r    = new TProfile("Flq_r",   Fstr[1][3].c_str(), nFr,FL,FHr);
  TProfile* FjetSU_r = new TProfile("FjetSU_r",Fstr[1][0].c_str(), nFr,FL,FHr);
  TProfile* FbSU_r   = new TProfile("FbSU_r",  Fstr[1][1].c_str(), nFr,FL,FHr);
  TProfile* FgSU_r   = new TProfile("FgSU_r",  Fstr[1][2].c_str(), nFr,FL,FHr);
  TProfile* FlqSU_r  = new TProfile("FlqSU_r", Fstr[1][3].c_str(), nFr,FL,FHr);
  //Gen lvl jet pT w/o/ muons and neutrinos
  TProfile* Fjet_g   = new TProfile("Fjet_g",  Fstr[2][0].c_str(), nF, FL,FH );
  TProfile* Fb_g     = new TProfile("Fb_g",    Fstr[2][1].c_str(), nF, FL,FH );
  TProfile* Fg_g     = new TProfile("Fg_g",    Fstr[2][2].c_str(), nF, FL,FH );
  TProfile* Flq_g    = new TProfile("Flq_g",   Fstr[2][3].c_str(), nF, FL,FH );
  TProfile* FjetSU_g = new TProfile("FjetSU_g",Fstr[2][0].c_str(), nF, FL,FH );
  TProfile* FbSU_g   = new TProfile("FbSU_g",  Fstr[2][1].c_str(), nF, FL,FH );
  TProfile* FgSU_g   = new TProfile("FgSU_g",  Fstr[2][2].c_str(), nF, FL,FH );
  TProfile* FlqSU_g  = new TProfile("FlqSU_g", Fstr[2][3].c_str(), nF, FL,FH );
  TProfile *Fptr,   *FptrSU   = 0; //Ptrs for handling the TProfiles
  TProfile *Fptr_p, *FptrSU_p, *Fptr_r, *FptrSU_r, *Fptr_g, *FptrSU_g = 0;

  string pTEpStr = ";#font[132]{#font[12]{E'} [GeV]};";
  pTEpStr       += "#font[132]{#font[12]{p}_{T,probe}^{gen}/#font[12]{E'}}"; 
  TProfile* pTEp = new TProfile("pTEp", pTEpStr.c_str(), 17, 30, 200);
  string EpPstr=";#font[132]{#font[12]{p}_{T,probe}^{MC} [GeV]};#font[12]{E'}";
  string EpEstr=";#font[132]{#font[12]{E}_{probe} [GeV]};";
  EpEstr += "#font[132]{#font[12]{E'}/#font[12]{E}_{probe}^{gen}}";
  TProfile* EpP    = new TProfile("EpP",   EpPstr.c_str(), 17, 30, 200);
  TProfile* EpE    = new TProfile("EpE",   EpEstr.c_str(), 17, 30, 200);
  TProfile* EpP_b  = new TProfile("EpP_b", EpPstr.c_str(), 17, 30, 200);
  TProfile* EpE_b  = new TProfile("EpE_b", EpEstr.c_str(), 17, 30, 200);
  TProfile* EpP_g  = new TProfile("EpP_g", EpPstr.c_str(), 17, 30, 200);
  TProfile* EpE_g  = new TProfile("EpE_g", EpEstr.c_str(), 17, 30, 200);
  TProfile* EpP_lq = new TProfile("EpP_lq",EpPstr.c_str(), 17, 30, 200);
  TProfile* EpE_lq = new TProfile("EpE_lq",EpEstr.c_str(), 17, 30, 200);
  TProfile* EpEptr;  TProfile* EpPptr;

  //MC-reco probe pT vs. gen probe pT (TRUE RESPONSE). a=all flavours
  string RtrueStr = ";#font[132]{#font[12]{p}_{T,probe}^{gen} [GeV]};";
  RtrueStr += "#font[132]{#font[12]{p}_{T,probe}^{MC} [GeV]}";
  TProfile* pRpGa  = new TProfile("pRpGa",  RtrueStr.c_str(), 17, 30, 200);
  TProfile* pRpGb  = new TProfile("pRpGb",  RtrueStr.c_str(), 17, 30, 200);
  TProfile* pRpGg  = new TProfile("pRpGg",  RtrueStr.c_str(), 17, 30, 200);
  TProfile* pRpGlq = new TProfile("pRpGlq", RtrueStr.c_str(), 17, 30, 200);
  //Pointers for handling the above TProfiles
  TProfile *pGtGptr, *pRpGptr = 0;

  //MC-reco probe pT vs. pT' (approx. pTtagReco)
  string pTppRstr = ";#font[132]{#font[12]{p'}_{T} [GeV]};";
  pTppRstr += "#font[132]{#font[12]{p}_{T,probe}^{MC} [GeV]}";
  TProfile* pTppRa  = new TProfile("pTppRa", pTppRstr.c_str(),17, 30, 200);
  TProfile* pTppRb  = new TProfile("pTppRb", pTppRstr.c_str(),17, 30, 200);
  TProfile* pTppRg  = new TProfile("pTppRg", pTppRstr.c_str(),17, 30, 200);
  TProfile* pTppRlq = new TProfile("pTppRlq",pTppRstr.c_str(),17, 30, 200);
  TProfile *pTppRptr= 0;	//Pointer for handling the above TProfiles

  //Response function R_h (h for hadron)
  TF1 *fr_h = new TF1("frh","[5]*[0]*(1-[3]*[1]*pow(x,[2]+[4]-1))",0,4000);

  //Used in HCAL calibration
  TF1 *fr_hcal = new TF1("fr_hcal","x*1.10286*(1-1.25613*pow(x,0.397034-1))",0,4000);
  //TF1 *fr_ecal = new TF1("fr_ecal","x*1.02900*(1-1.67580*pow(x,0.553456-1))",0,4000);

  //Pointer to one of the response functions at a time
  TF1* fr;
  //1st derivatives of hadron data response
  TF1* dAR = new TF1("dAR","-[5]*[0]*[1]*pow(x,[2]+[4]-1)",0,251);
  TF1* dBR = new TF1("dBR","-[3]*[5]*[0]*[1]*pow(x,[2]+[4]-1)*log(x)",0,251);
  TF1* dCR = new TF1("dCR","[0]*(1 - [3]*[1]*pow(x,[2]+[4]-1))",0,251);

  //Higher resolution
  int reso = 10000; 
  fr_h->SetNpx(reso); fr_hcal->SetNpx(reso);
  dAR->SetNpx(reso); dBR->SetNpx(reso); dCR->SetNpx(reso);

  /* INITIALIZATIONS */
  Long64_t nbytes = 0, nb = 0;
  TLorentzVector tag;		//To contain leading photon or tag jet
  TLorentzVector probe;		//To contain the probe jet
  TLorentzVector p4;		//Particle 4-momentum temp.
  TLorentzVector p4_2;		//Particle 4-momentum temp.  
  TLorentzVector p4j;		//Jet 4-momentum temp.
  TLorentzVector p4g_probe;	//Probe jet 4-mom., sum of generated particles  
  TLorentzVector p4g_tag;	//Tag    -||-    for dijet case
  TLorentzVector p4r_probe;	//Reconstr. probe 4-momentum, resp. included
  TLorentzVector p4r_tag;	//       -||-    for tag in dijet case
  TLorentzVector p4f_probe;	//w/ fit params to CMS data
  TLorentzVector p4f_tag;

  //Partial derivative values for a hadron
  double dARtemp = 0; double dBRtemp = 0; double dCRtemp = 0;
  double dA_E, dA_X, dA_Y;	//Temps for sum terms...
  double dB_E, dB_X, dB_Y;	//... in the derivatives...  
  double dC_E, dC_X, dC_Y;	//... when fitting
  vector<double> dAjetsE, dAjetsX, dAjetsY;  //Derivative values of 2...	
  vector<double> dBjetsE, dBjetsX, dBjetsY;  //...leading jets for finding...
  vector<double> dCjetsE, dCjetsX, dCjetsY;  //...the d*_* variables above
  double dAfTMP, dBfTMP, dCfTMP;//Temps to store derivatives of pTprobe/pTtag
  unsigned int i_tag = 0;	//Stepper to find tag object index
  unsigned int i_probe = 0;	//       -||-     probe jet index
  double Ep   = 0;		//Jet energy estimator E' = pTtag*cosh(eta_gen)
  double pTp  = 0;		//tag object pT
  double eta_muon  = 2.3;	//Max |eta| for a single muon in Z+jet      
  double eta_tag_z = 2.5;	//Max |eta| for mumu tag object
  double eta_probe = 1.3;       //Max |eta| for probe jets
  vector<TLorentzVector> jets_g;//Gen lvl jet 4-vectors
  vector<TLorentzVector> jets_r;//MC reco'd jet 4-vectors
  vector<TLorentzVector> jets_f;//Fit reco'd jet 4-vectors
  vector<double> Fden;		//F denominator:   sum_j R_j^MC   E_j^g
  vector<double> Fnum;		//F numerator:     sum_i R_i^data E_i^g
  double F;			//Temps to store F values for the probe jet
  int PDG = 1;			//Shorthand, to store a particle's PDGID
  int JI = 0;			//Shorthand, particle's jet index
  double phiMin = 2.8;		//Minimum azimuth angle between tag and probe
  double pTmin_probe = 15;	//Minimum probe jet p_T (GeV)
  double pTmin_muon  = 15;      //Minimum single tag muon pT (GeV)
  double pTmin_tag_z = 15;      //Minimum tag muon pair pT (GeV)   
  double resp   = 1.0;	        //SPR value                (dummy init)
  double resp_f = 1.0;	        // -||- w/ fit params      (dummy init)
  double respH  = 1.0;	        //Calorimeter response for hadrons
  double R_MPF_r = 0;		//MC-reco'd MPF response
  double R_MPF_f = 0;		//Fit reco'd MPF response
  double FSU = 0.0;		//Temp for F systematic uncertainty calculations
  unsigned long njets;		//#jets in the event, for resizing vectors
  TLorentzVector NIJ_g;		//Gen lvl 4-vec sum of prtcls Not In Jets
  TLorentzVector NIJ_r;		//MC-reco lvl  -||-
  TLorentzVector NIJ_f;		//Fit reco lvl -||-
  TLorentzVector MET_g;         //Gen lvl event MET
  TLorentzVector MET_r;		//MC    -||-
  TLorentzVector MET_f;		//FIT   -||-

  //CMS detector related parameters
  double Bfield = 4.0; //Tesla
  double Rt     = 1.2; //Tracker radius
  //double Rt     = 0.0; //HCAL radius
  
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

/*
  SOME TEMPORARY VARIABLES TO TRACK CUTTING ETC.
*/

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
  int tagProbeCut = 0;
  int b2b = 0;
  int alpha = 0;
  int lowMet = 0;
  

//***********************************************************************************************

  TCanvas *c2         = new TCanvas("c2","c2",600,400);
  TH2D* cutHist       = new TH2D("cutHist", "Cut Histogram", 10, 0, 500, 7, 0, 7);
  const char *cuts[7] = {"All", "Muon cut", "Invariant mass", "Tag probe cuts", "b2b", 
                         "Alpha cut", "Low met"};


  //Granularity of cells
  //quarter
  //int nPhi = 18;
  //int nEta = 30;  

  //half
  //int nPhi = 36;
  //int nEta = 60;

  //normal
  int nPhi = 72;
  int nEta = 119;

  //twice
  //int nPhi = 2*72;
  //int nEta = 2*119;

  //quad
  //int nPhi = 4*72;
  //int nEta = 4*119;

  TH2D* cht         = new TH2D("cht"   , "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* chtPt       = new TH2D("chtPt" , "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* chc         = new TH2D("chc"   , "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* sigma       = new TH2D("sigma" , "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* nhECAL      = new TH2D("nhECAL", "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* nhHCAL      = new TH2D("nhHCAL", "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);
  TH2D* ne          = new TH2D("ne"    , "", nPhi, -TMath::Pi(), TMath::Pi(), nEta, -5.2, 5.2);

  const double bins_chhEff[21] = {0.1, 0.149, 0.223, 0.332, 0.496, 0.740, 1.104, 1.648, 2.460,
                                 3.670, 5.477, 8.174, 12.198, 18.202, 27.163, 40.536, 60.492,
                                 90.272, 134.713, 201.031, 300.0};

  TProfile* chhEff = new TProfile("chhEff", "", 20, bins_chhEff);

  TF1* pchf = new TF1("pchf","[0]*pow(x,2) + [1]*x + [2]", 0, 5000);
  pchf->SetParameters(6.86221185e-09, -0.0001268141794, 0.6283211821);

  //Single pion resolution
  TF1* jerg_A = new TF1("jerg_A", "sqrt([0]*[0]/(x*x)+ [1]*[1]/x + [2]*[2]) * (0.55*1.02900*(1-1.6758*pow(x/0.75,0.553456-1)) + 0.45*1.10286*(1-1.25613*pow(x/0.75,0.397034-1)))", 0, 1000);
  jerg_A->SetParameters(9.59431e-05, 1.49712, 8.92104e-02);

  TF1* eff_fit = new TF1("eff_fit","1+[0]*x+[1]*x*x+[2]*exp([3]*x)", 0, 5000);
  eff_fit->SetParameters(-0.0003897, 3.589e-07, -0.02651, -0.2829);


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
    // if (Cut(ientry) < 0) continue;	//Cut is currently dummy in header

    //Reinit
    i_tag = 0; i_probe = 0; p4.SetPtEtaPhiE(0,0,0,0); p4_2.SetPtEtaPhiE(0,0,0,0);
    tag.SetPtEtaPhiE(0,0,0,0); probe.SetPtEtaPhiE(0,0,0,0);
    p4g_probe.SetPtEtaPhiE(0,0,0,0); p4g_tag.SetPtEtaPhiE(0,0,0,0);
    p4r_probe.SetPtEtaPhiE(0,0,0,0); p4r_tag.SetPtEtaPhiE(0,0,0,0);
    p4f_probe.SetPtEtaPhiE(0,0,0,0); p4f_tag.SetPtEtaPhiE(0,0,0,0);
    NIJ_g.SetPtEtaPhiE(0,0,0,0); MET_g.SetPtEtaPhiE(0,0,0,0);
    NIJ_r.SetPtEtaPhiE(0,0,0,0); MET_r.SetPtEtaPhiE(0,0,0,0);
    NIJ_f.SetPtEtaPhiE(0,0,0,0); MET_f.SetPtEtaPhiE(0,0,0,0);
    jets_g.clear(); jets_r.clear(); jets_f.clear();
    Fden.clear(); Fnum.clear();  
    njets = (unsigned long)jet_pt->size();
    jets_g.resize(njets); jets_r.resize(njets); jets_f.resize(njets);
    Fden.resize(njets); Fnum.resize(njets);
    dAjetsE.resize(njets); dAjetsX.resize(njets); dAjetsY.resize(njets);
    dBjetsE.resize(njets); dBjetsX.resize(njets); dBjetsY.resize(njets);
    dCjetsE.resize(njets); dCjetsX.resize(njets); dCjetsY.resize(njets);
    for (int i=0; i!=jets_g.size(); ++i) { //All objects have njets size
      jets_g[i].SetPtEtaPhiE(0,0,0,0);  jets_r[i].SetPtEtaPhiE(0,0,0,0);
      jets_f[i].SetPtEtaPhiE(0,0,0,0);
      Fden[i] = 0; Fnum[i] = 0;
      dAjetsE[i] = 0;  dAjetsX[i] = 0;  dAjetsY[i] = 0;
      dBjetsE[i] = 0;  dBjetsX[i] = 0;  dBjetsY[i] = 0;
      dCjetsE[i] = 0;  dCjetsX[i] = 0;  dCjetsY[i] = 0;
    }
    dA_E=0; dA_X=0; dA_Y=0; dB_E=0; dB_X=0; dB_Y=0; dC_E=0; dC_X=0; dC_Y=0;

    /**************** Z+JET: FIND AND RECONSTRUCT TAG MUONS ****************/
    int muPDG=13;  int muTAG=3; //mu PDGID and parton tag
    int i_tag1 = -137;	// These values won't change if...
    int i_tag2 = -731;	// ...muons not found.
    bool muonCut = 0; //Necessary for the cut histogram

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

    if (fabs(p4.Eta()) > eta_muon || p4.Pt() < pTmin_muon) muonCut = 1;

    Response((*prtn_pdgid)[i_tag1],p4.Eta(),p4.E(),p4.Pt(),Rt,Bfield,
             fr_h,true,1, 0, 1, true, true, false, resp, resp_f, respH);

    tag     = p4;        //gen lvl
    p4r_tag = p4*resp;   //MC lvl
    p4f_tag = p4*resp_f; //FIT lvl

    //***** 2nd muon *****
    p4.SetPtEtaPhiE((*prtn_pt )[i_tag2], (*prtn_eta)[i_tag2],
                    (*prtn_phi)[i_tag2], (*prtn_e  )[i_tag2]);

    if (fabs(p4.Eta()) > eta_muon || p4.Pt() < pTmin_muon) muonCut = 1;

    Response((*prtn_pdgid)[i_tag2],p4.Eta(),p4.E(),p4.Pt(),Rt,Bfield,
             fr_h,true,1, 0, 1, true, true, false,  resp, resp_f, respH);

    tag      += p4;	   //gen lvl
    p4r_tag  += p4*resp;   //MC lvl
    p4f_tag  += p4*resp_f; //FIT lvl

    cutHist->Fill(tag.Pt(), cuts[0], 1);      
    if (muonCut) continue; mumuCut++;
    cutHist->Fill(tag.Pt(), cuts[1], 1);     

    //Invariant mass in range 70-110 GeV since Z boson mass is 91 GeV
    double M = tag.M();
    if ( M<70.0 || M>110.0) continue; invM ++;
    cutHist->Fill(tag.Pt(), cuts[2], 1);

    /***************** RECONSTRUCT JETS AND PARTICLES IN JETS *****************/
    //Note that only the 2 leading jets are likely to be probe or tag candidates
    for (int i=0; i != prtcl_pt->size(); ++i) {

      JI = (*prtcl_jet)[i];	                        //Which jet this particle belongs to
      PDG = abs((*prtcl_pdgid)[i]);
      p4.SetPtEtaPhiE((*prtcl_pt )[i], (*prtcl_eta)[i],	//Current prtcl 4-vec
                      (*prtcl_phi)[i], (*prtcl_e  )[i]);

      //Calculate responses. Store results to [resp, resp_f, respH] tuples
      Response(PDG, p4.Eta(), p4.E(), p4.Pt(),Rt,Bfield, fr_h, true,
               GetA(), GetB(), GetC(), true, true, true, resp, resp_f, respH );

      //Reconstruct jets
      jets_g[JI] += (isNeutrino(PDG) ? 0:1)*p4; //Gen lvl
      jets_r[JI] += p4*resp;                    //MC reco
      jets_f[JI] += p4*resp_f;                  //Fit reco
      Fnum[JI]   += resp_f*p4.E();
      Fden[JI]   += resp*p4.E(); //F denominator reco'd w/ default params
      
      //Find derivatives of hadron (PDG>100) responses for 2 leading jets
      //Most likely probe candidates are the jets w/ indices 0 or 1. Thus to
      //save time, we only calculate the first 2 jets' derivatives.
      //Rare cases with i_probe>1 are calculated separately at a later stage. 
      if (PDG>99 && JI<2) {
        //Notice the fr_h are replaced by partial derivative functions
        //Other responses are not calculated
        Response(PDG, p4.Eta(), p4.E(), p4.Pt(),Rt,Bfield,dAR, false,
                 GetA(),GetB(),GetC(),false,true,false, resp,dARtemp,respH );
        Response(PDG, p4.Eta(), p4.E(), p4.Pt(),Rt,Bfield,dBR, false,
                 GetA(),GetB(),GetC(),false,true,false, resp,dBRtemp,respH );
        Response(PDG, p4.Eta(), p4.E(), p4.Pt(),Rt,Bfield,dCR, false,
                 GetA(),GetB(),GetC(),false,true,false, resp,dCRtemp,respH );
        dAjetsE[JI] += p4.E()*dARtemp;
        dAjetsX[JI] += p4.X()*dARtemp;
        dAjetsY[JI] += p4.Y()*dARtemp;
        dBjetsE[JI] += p4.E()*dBRtemp;
        dBjetsX[JI] += p4.X()*dBRtemp;
        dBjetsY[JI] += p4.Y()*dBRtemp;
        dCjetsE[JI] += p4.E()*dCRtemp;
        dCjetsX[JI] += p4.X()*dCRtemp;
        dCjetsY[JI] += p4.Y()*dCRtemp;
      } //Find derivatives
    } //Loop particles in jets

    /************************* Z+JET: FIND PROBE *************************/
    switch (jets_r.size()) {
      case 0  : continue;           //No jets
      case 1  : i_probe = 0; break; //Take the only jet as a probe 
      default : i_probe = 0;        //More than two jets
    }

    p4.SetPtEtaPhiE(0,0,0,0);	//Reinit

    //Set probe 4-vectors.
    //Gen lvl as output by FastJet
    probe.SetPtEtaPhiE((*jet_pt)[i_probe],  (*jet_eta)[i_probe],
                       (*jet_phi)[i_probe], (*jet_e)[i_probe]  );
    //Gen lvl for particles that reach CMC detector volume
    p4g_probe.SetPtEtaPhiE(jets_g[i_probe].Pt(),  jets_g[i_probe].Eta(),
                           jets_g[i_probe].Phi(), jets_g[i_probe].E() );
    //MC SPR reco
    p4r_probe.SetPtEtaPhiE(jets_r[i_probe].Pt(),  jets_r[i_probe].Eta(),
                           jets_r[i_probe].Phi(), jets_r[i_probe].E() );
    //FIT reco
    p4f_probe.SetPtEtaPhiE(jets_f[i_probe].Pt(),  jets_f[i_probe].Eta(),
                           jets_f[i_probe].Phi(), jets_f[i_probe].E() );

    //tag and probe in the right |eta| region with enough pT
    if (fabs(p4r_tag.Eta())   > eta_tag_z || p4r_tag.Pt()   < pTmin_tag_z ||
        fabs(p4r_probe.Eta()) > eta_probe || p4r_probe.Pt() < pTmin_probe  ) continue; 
    tagProbeCut ++;

    cutHist->Fill(tag.Pt(), cuts[3], 1);

    /****************************** COMMON CUTS FOR Z+JET ******************************/
    //Tag object and probe jet back-to-back. Note ROOT DeltaPhi is in [-pi,pi]
    if (fabs(tag.DeltaPhi(probe)) < phiMin) continue; b2b ++;
    cutHist->Fill(tag.Pt(), cuts[4], 1);

    //Alpha cut
    if (jets_r[1].Pt() > 0.3*p4r_tag.Pt()) continue; alpha ++;
    cutHist->Fill(tag.Pt(), cuts[5], 1);

    //Assert sufficiently low MET w.r.t. tag pT and leading jet (we use probe)
    if      (p4r_tag.Pt() > 50 && met > 0.9*p4r_tag.Pt()) continue;
    else if (p4r_tag.Pt() > 25 && met > 1.1*p4r_tag.Pt()) continue;
    else if (p4r_tag.Pt() > 15 && met > 1.2*p4r_tag.Pt()) continue;
    else if (                     met > 2.0*p4r_tag.Pt()) continue;
    if      (met/p4r_probe.Pt()       > 0.7             ) continue;
    lowMet ++;

    cutHist->Fill(tag.Pt(), cuts[6], 1);


    /******************* RECONSTRUCT PARTICLES NOT IN JETS *******************/
    #ifdef NIJ

    //Reset histograms
    cht    ->Reset();
    chtPt  ->Reset();
    chc    ->Reset();
    sigma  ->Reset();
    nhECAL ->Reset();
    nhHCAL ->Reset();
    ne     ->Reset();

    // ***************** Loop over all particles ******************
    for (int i=0; i!=prtclnij_pt->size(); ++i) {
      double resolution = 0.0;

      PDG = fabs((*prtclnij_pdgid)[i]);
      p4.SetPtEtaPhiE((*prtclnij_pt )[i],(*prtclnij_eta)[i],
                      (*prtclnij_phi)[i],(*prtclnij_e)[i]);

      Response((*prtclnij_pdgid)[i],p4.Eta(),p4.E(),p4.Pt(),Rt,Bfield,fr_h,true,
               GetA(),GetB(),GetC(),true,true,true,resp,resp_f,respH);
      p4 *= resp;

      switch (PDG) {
        //PHOTON
        case 20 : case 22 : 
          ne->Fill(p4.Phi(), p4.Eta(), p4.E());
          break;

        //LEPTONS
        case 13 : case 11 :
          NIJ_r += p4;
          break;

        //CHARGED HADRONS
        case 211 : case 321 : case 2212 : case 3112 : case 3222 : case 3312 : case 3334 :
          double eff; bool trkFail; double effConst; double newPhi;

          trkFail = 0;
          eff = 1.0; //No particle level track failing
          eff = 0.5284 + 0.3986/(1+pow(((*prtclnij_pt )[i]/88.76),1.22));
          if ((*prtclnij_pt )[i] < 0.9) eff = 0.2514 + 0.7429*(*prtclnij_pt )[i];
          
          /*
          //Efficiency from chf
          if (fabs(p4.Eta() < 5.2)) {
            for (int ijet=0; ijet!=jets_r.size(); ++ijet) {
              if (p4.DeltaR(jets_r[ijet]) < 0.4) { //Distance from the jet
                eff = pchf->Eval(jets_r[ijet].Pt()) / 0.607;

                //eff = 0.5284 + 0.3986/(1+pow(((*prtclnij_pt )[i]/88.76),1.22));
                //if ((*prtclnij_pt )[i] < 0.9) eff = 0.2514 + 0.7429*(*prtclnij_pt )[i];
                if (p4.DeltaR(jets_r[ijet]) < 0.02 && jets_r[ijet].Pt() > 1000) { 
                  eff = 0.5; //eff + 0.0005*(200-jets_r[ijet].Pt());
                }
              }
            }
          }*/

          //Fix chf to the single particle efficiency
          //eff -= 0.263243+(0.04199621-0.263243)/(1+pow(((*prtclnij_pt)[i]/74.1766),1.737401));
          
          //eff = min(eff,0.92);
          
          /*
          //Don't use if cellFail
          if (eff < 0.0) eff = 0.0;
          if (eff > 1.0) eff = 1.0;
          if ( (fabs((*prtclnij_eta)[i]) < 2.5)) {
            chhEff->Fill((*prtclnij_pt)[i], eff);
          }*/

          if ( ((double)rand() / (double)RAND_MAX) > eff ) trkFail =  1;


          if (!trkFail) {
            //if (((double)rand() / (double)RAND_MAX) > 0.92 ) {
            //  p4 *= 0.8;
            //  cht  ->Fill(p4.Phi(), p4.Eta(), p4.E() );
            //  chtPt->Fill(p4.Phi(), p4.Eta(), p4.Pt());
            //}else {
              cht  ->Fill(p4.Phi(), p4.Eta(), p4.E() );
              chtPt->Fill(p4.Phi(), p4.Eta(), p4.Pt());
            //}
          }

          //Track curvature in the calorimeter energy deposit
          newPhi = trackDeltaPhi((*prtclnij_pdgid)[i], (*prtclnij_phi)[i], 
                                 (*prtclnij_pt )[i], Rt, Bfield);


          p4.SetPtEtaPhiE((*prtclnij_pt )[i],(*prtclnij_eta)[i],newPhi,(*prtclnij_e)[i]);
          p4 *= respH;

          chc->Fill(p4.Phi(), p4.Eta(), p4.E());
          
          if (respH != 0.0) {
            resolution = jerg_A->Eval((*prtclnij_pt)[i]) * (*prtclnij_pt )[i];
            if (resolution < 0.0) resolution = 0.0;
          }

          //***************** Track resolution
          double trkReso;
          trkReso = (*prtclnij_pt )[i]*(0.01336 + 8.548e-5*(*prtclnij_pt )[i]);
          sigma->Fill(p4.Phi(), p4.Eta(), pow(trkReso,2));
          //*****************          

          sigma->Fill(p4.Phi(), p4.Eta(), pow(resolution,2));
          
          if (trkFail) {
            p4.SetPtEtaPhiE((*prtclnij_pt )[i], (*prtclnij_eta)[i], newPhi, (*prtclnij_e)[i]);

            if ( ((double)rand() / (double)RAND_MAX) > 0.45 ) { //EHE hadron path
              fr_h->SetParameters(params_pi_EHE[0][0], params_pi_EHE[0][1],
                                  params_pi_EHE[0][2], 1, 0, 1);

              if (fr_h->Eval(p4.E()) > 0.0) { 
                nhHCAL->Fill(p4.Phi(), p4.Eta(), 0.5*fr_h->Eval(p4.E())*p4.E()); //HCAL
                nhECAL->Fill(p4.Phi(), p4.Eta(), 0.5*fr_h->Eval(p4.E())*p4.E()); //ECAL
              }
            } else { //HHe path 
              fr_h->SetParameters(params_pi_HHe[0][0], params_pi_HHe[0][1],
                                  params_pi_HHe[0][2], 1, 0, 1);

              if (fr_h->Eval(p4.E()) > 0.0) {
                nhHCAL->Fill(p4.Phi(), p4.Eta(), fr_h->Eval(p4.E())*p4.E());
              }
            }
          }
          break;

        //NEUTRAL HADRONS
        case 130 : case 310 : case 3122 : case 2112 : case 3212 : case 3322 :
       
          p4.SetPtEtaPhiE((*prtclnij_pt )[i],(*prtclnij_eta)[i],
                          (*prtclnij_phi)[i],(*prtclnij_e  )[i]);


          if ( ((double)rand() / (double)RAND_MAX) > 0.45 ) { //EHE hadron path
            fr_h->SetParameters(params_pi_EHE[0][0], params_pi_EHE[0][1],
                                params_pi_EHE[0][2], 1, 0, 1);

            if (fr_h->Eval(p4.E()) > 0.0) { 
              nhHCAL->Fill(p4.Phi(), p4.Eta(), 0.5*fr_h->Eval(p4.E())*p4.E()); //HCAL
              nhECAL->Fill(p4.Phi(), p4.Eta(), 0.5*fr_h->Eval(p4.E())*p4.E()); //ECAL
            }
          } else { //HHe path 
            fr_h->SetParameters(params_pi_HHe[0][0], params_pi_HHe[0][1],
                                params_pi_HHe[0][2], 1, 0, 1);

            if (fr_h->Eval(p4.E()) > 0.0) {
              nhHCAL->Fill(p4.Phi(), p4.Eta(), fr_h->Eval(p4.E())*p4.E()); //HCAL
            }
          }
          break;
          
        default : 
          if (fabs(PDG) != 12 && fabs(PDG) != 14 && fabs(PDG) != 16) {
            cout << "PDGID " << PDG << endl;
          }
      }
    } //Particle loop

    double cellPhi; double cellEta; double cellE; double cellPt;
    double delta = 0.0; double cellSigma = 0.0;

    // ***************** Loop over cells ******************
    for (int i=1; i!=cht->GetNbinsX()+1; ++i) {
      for (int j=1; j!=cht->GetNbinsY()+1; ++j) {
        double nhHCAL_calib = 0.0; double eff_c;

        cellPhi = cht->GetXaxis()->GetBinCenter(i);
        cellEta = cht->GetYaxis()->GetBinCenter(j);

        eff_c = 1.0; //No tracking fail
        //eff_c = 1.0 - 0.0002935 * chtPt->GetBinContent(i,j);

        //Cell four vector
        cellE  = nhECAL->GetBinContent(i,j) + 
                 nhHCAL->GetBinContent(i,j) + 
                     ne->GetBinContent(i,j) + 
                    cht->GetBinContent(i,j);
        cellPt =(nhECAL->GetBinContent(i,j) + 
                 nhHCAL->GetBinContent(i,j) + 
                     ne->GetBinContent(i,j))/cosh(cellEta) + 
                  chtPt->GetBinContent(i,j);
        p4.SetPtEtaPhiE(cellPt, cellEta, cellPhi, cellPt);

        /*
        //Track fail from chf
        for (int ijet=0; ijet!=jets_r.size(); ++ijet) {
          if (p4.DeltaR(jets_r[ijet]) < 0.4) { //Cells in 0.4 from the jet

            eff_c = 1.0 - 0.0009 * chtPt->GetBinContent(i,j);

            //if (cht->GetBinContent(i,j) > 0.0) {
            //  eff_c = pchf->Eval(jets_r[ijet].Pt()) / 0.607;
            //}
          }
        }*/

        //eff_c = 1.0 - 0.0009 * chtPt->GetBinContent(i,j);
        //eff_c = 1.0 - 0.0016 * chtPt->GetBinContent(i,j);

        //eff_c = min(0.92, eff_c);         

        if(eff_c < 0.0) eff_c = 0.0;
        if(eff_c > 1.0) eff_c = 1.0;

        //Track fail
        if ( ((double)rand() / (double)RAND_MAX) > eff_c ) {
          nhECAL->Fill(cellPhi,cellEta,      0.55*0.5 *chc->GetBinContent(i,j));//ECAL
          nhHCAL->Fill(cellPhi,cellEta,(0.45+0.55*0.5)*chc->GetBinContent(i,j));//HCAL
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
            if ( isChHadron((*prtclnij_pdgid)[k])) {

              phiIdx = floor(((*prtclnij_phi)[k]+TMath::Pi())/phiStep) + 1;
              etaIdx = floor(((*prtclnij_eta)[k]+5.2)/etaStep) + 1;
              
              if ((phiIdx == i) && (etaIdx == j)) {
                chhEff->Fill((*prtclnij_pt)[k], eff_c);
              }
            }
          }
        }*/

        //HCAL calibration
        if (nhHCAL->GetBinContent(i,j) > 0.0) {
          nhHCAL_calib = fr_hcal->GetX(nhHCAL->GetBinContent(i,j), 0.1, 7000.0);
        }

        delta = nhECAL->GetBinContent(i,j) + ne->GetBinContent(i,j) + nhHCAL_calib;
        cellSigma = sqrt(sigma->GetBinContent(i,j));

        //Changing HCAL cluster sigma threshold with uncalibrated cell energy
        //cellSigma *= (1 + exp(-nhHCAL->GetBinContent(i,j)/100)); 
        cellSigma *= (1 + exp(-nhHCAL_calib/100)); 
 
        //Energy fraction of jets |eta|<1.3
        for (int ijet=0; ijet!=jets_r.size(); ++ijet) {
          if (fabs(jets_r[ijet].Eta()) < 1.3) {
            if (p4.DeltaR(jets_r[ijet]) < 0.4) {
              if (delta < cellSigma) {
                h_ch_c   ->Fill(jets_r[ijet].Pt(), cht->GetBinContent(i,j));
                h_all_c  ->Fill(jets_r[ijet].Pt(), cht->GetBinContent(i,j));
              } else {
                h_ch_c   ->Fill(jets_r[ijet].Pt(), cht->GetBinContent(i,j));
                h_nh_c   ->Fill(jets_r[ijet].Pt(), nhHCAL_calib);
                h_gamma_c->Fill(jets_r[ijet].Pt(), nhECAL->GetBinContent(i,j)+
                                                       ne->GetBinContent(i,j));
                h_all_c  ->Fill(jets_r[ijet].Pt(), delta+cht->GetBinContent(i,j));
              }
            }
          }
        }

        if (delta < cellSigma) { //Shadowing
          //Tracker
          cellE  = cht->GetBinContent(i,j);
          cellPt = chtPt->GetBinContent(i,j);
          p4.SetPtEtaPhiE(cellPt, cellEta, cellPhi, cellE);
          NIJ_r += p4;
        } else { //Normal case
          // Neutral hadrons and photons
          cellE  = delta;
          cellPt = delta/cosh(cellEta);
          p4.SetPtEtaPhiE(cellPt, cellEta, cellPhi, cellE);
          NIJ_r += p4;
          //Tracker
          cellE  = cht->GetBinContent(i,j);
          cellPt = chtPt->GetBinContent(i,j);
          p4.SetPtEtaPhiE(cellPt, cellEta, cellPhi, cellE);
          NIJ_r += p4;
        }
      }
    }
    #endif


    /************** FIND DERIVATIVES IN CASE i_probe > 1 (RARE) **************/
    if (i_probe>1) {
      for (int i=0; i!=prtcl_pdgid->size(); ++i) {
        PDG = abs((*prtcl_pdgid)[i]);
        JI = (*prtcl_jet)[i];
        if (PDG>100 && JI==i_probe) {	//Hadrons in probe
          p4.SetPtEtaPhiE((*prtcl_pt)[i], (*prtcl_eta)[i],
                          (*prtcl_phi)[i],(*prtcl_e)[i]  );
          //Notice the fr_h are replaced by partial derivative functions
          //Other responses are not calculated
          Response(PDG, p4.Eta(), p4.E(), p4.Pt(),Rt,Bfield,dAR, false,
                   GetA(),GetB(),GetC(),false,true,false,resp,dARtemp,respH  );
          Response(PDG, p4.Eta(), p4.E(), p4.Pt(),Rt,Bfield,dBR, false,
                   GetA(),GetB(),GetC(),false,true,false,resp,dBRtemp,respH  );
          Response(PDG, p4.Eta(), p4.E(), p4.Pt(),Rt,Bfield,dCR, false,
                   GetA(),GetB(),GetC(),false,true,false,resp,dCRtemp,respH  );
          dAjetsE[JI] += p4.E()*dARtemp;
          dAjetsX[JI] += p4.X()*dARtemp;
          dAjetsY[JI] += p4.Y()*dARtemp;
          dBjetsE[JI] += p4.E()*dBRtemp;
          dBjetsX[JI] += p4.X()*dBRtemp;
          dBjetsY[JI] += p4.Y()*dBRtemp;
          dCjetsE[JI] += p4.E()*dCRtemp;
          dCjetsX[JI] += p4.X()*dCRtemp;
          dCjetsY[JI] += p4.Y()*dCRtemp;
        } //Find derivatives
      } //Loop particles in jets
    }

    /************************** PARTICLE COMPOSITION **************************/
    //Loop all particles associated with jets
    for (unsigned int i=0; i!=(GetcontHistos() ? prtcl_pdgid->size():0); ++i) {
      JI = (*prtcl_jet)[i];

      //See if ptcl belongs to the studied jet(s)
      if (JI==i_probe) {

        PDG = abs((*prtcl_pdgid)[i]); 
        p4.SetPtEtaPhiE((*prtcl_pt)[i], (*prtcl_eta)[i], //Prtcl gen lvl values
		        (*prtcl_phi)[i],(*prtcl_e)[i]  );

        Response(PDG,p4.Eta(),p4.E(),p4.Pt(),Rt,Bfield,fr_h,true,
                 GetA(), GetB(), GetC(),true,true,true,resp,resp_f,respH);

        /* Fill (probe) particle content to the right histogram */
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
                h_other[2]->Fill((*jet_e)[(*prtcl_jet)[i_probe]], (resp_f*p4).E());
              } //Xi, Sigma, Omega^-
          } //Switch PDG (general)
          h_ptr[0]->Fill((*jet_e)[(*prtcl_jet)[i_probe]], (*prtcl_e)[i]  );
          h_ptr[1]->Fill((*jet_e)[(*prtcl_jet)[i_probe]], (resp*p4).E()  );
          h_ptr[2]->Fill((*jet_e)[(*prtcl_jet)[i_probe]], (resp_f*p4).E());
          h_all[0]->Fill((*jet_e)[(*prtcl_jet)[i_probe]], (*prtcl_e)[i]  );
          h_all[1]->Fill((*jet_e)[(*prtcl_jet)[i_probe]], (resp*p4).E()  );
          h_all[2]->Fill((*jet_e)[(*prtcl_jet)[i_probe]], (resp_f*p4).E());
        } //Particle content histograms
      } //If ptcl in the studied probe or tag
    } //Loop over particles

    /**************************** FILL HISTOGRAMS ****************************/
    //Jet energy estimator: E' = p_T_tag cosh(eta_probe) and pT'.
    //- Jet direction change in reconstruction is usually negligible, though.

    // Modified for Z+jet usage
    Ep  = p4r_tag.Pt()*cosh(probe.Eta());
    pTp = p4r_tag.Pt();

    //Data<->MC & MC'<->MC factors
    F = Fnum[i_probe]/Fden[i_probe];

    //Add the new values to TProfile histograms:
    //p_T balance method
    prpTbal   ->Fill(pTp, p4r_probe.Pt()/p4r_tag.Pt(), weight);
    prpTbalFit->Fill(pTp, p4f_probe.Pt()/p4r_tag.Pt(), weight);
    prE->Fill(p4g_probe.E(), p4r_probe.Pt()/p4r_tag.Pt(), weight);

    //Derivatives of pTprobe/pTtag
    //The x- and y-term sums in the derivatives of pTprobe/pTtag
    dA_X=dAjetsX[i_probe];  dB_X=dBjetsX[i_probe];  dC_X=dCjetsX[i_probe];
    dA_Y=dAjetsY[i_probe];  dB_Y=dBjetsY[i_probe];  dC_Y=dCjetsY[i_probe];

    //Derivatives of the nominator in F factors
    dAfTMP = (jets_f[i_probe].X()*dA_X + jets_f[i_probe].Y()*dA_Y)/
               (p4r_tag.Pt()*jets_f[i_probe].Pt());
    dBfTMP = (jets_f[i_probe].X()*dB_X + jets_f[i_probe].Y()*dB_Y)/
               (p4r_tag.Pt()*jets_f[i_probe].Pt());
    dCfTMP = (jets_f[i_probe].X()*dC_X + jets_f[i_probe].Y()*dC_Y)/
               (p4r_tag.Pt()*jets_f[i_probe].Pt());
    if (!isnan(dAfTMP)) dAf->Fill(Ep, dAfTMP, weight);
    if (!isnan(dBfTMP)) dBf->Fill(Ep, dBfTMP, weight);
    if (!isnan(dCfTMP)) dCf->Fill(Ep, dCfTMP, weight);

    //MPF method
    MET_g = -1*NIJ_g;
    MET_r = -1*NIJ_r;
    MET_f = -1*NIJ_f;

    
    // From Mikko's slides
    R_MPF_r = 1.0 + (MET_r.Px()*p4r_tag.Px() + MET_r.Py()*p4r_tag.Py()) / pow((p4r_tag.Pt()),2);
    R_MPF_f = 1.0 + (MET_f.Px()*p4f_tag.Px() + MET_f.Py()*p4f_tag.Py()) / pow((p4f_tag.Pt()),2);
    
    //R_MPF_r = 1.0 + MET_r.Pt()*cos(p4r_tag.DeltaPhi(MET_r))/p4r_tag.Pt();
    //R_MPF_f = 1.0 + MET_f.Pt()*cos(p4r_tag.DeltaPhi(MET_f))/p4r_tag.Pt();

    //Fill MPF histograms
    prMPF->Fill(pTp,  (!isnan(R_MPF_r)  ? R_MPF_r  : 0), weight);


    //CHECK JET FLAVOUR: FIND FLAVOUR-DEPENDENT QUANTITIES
    //                   SUCH AS MC-DATA CORRECTION FACTORS F
    //  N.B. Results are meaningful only if A,B,C params already optimized.
    //Loop partons to find where jets originated from
    for (unsigned int j = 0; j != prtn_tag->size(); ++j) {
      //prtn_tag=0 for outgoing hard process partons
      //Suffix "a" for "all jet flavours", needed for averaging etc.
      if ((*prtn_jet)[j]==i_probe && (*prtn_tag)[j]==0) {
        if (abs((*prtn_pdgid)[j])==5) {	//b-jets
          prqjet->Fill(Ep, p4r_probe.Pt()/p4r_tag.Pt(), weight); //q-jet resp
          pRpGptr  = pRpGb;
          pTppRptr = pTppRb;
          Fptr     = Fb;      FptrSU   = FbSU;
          Fptr_p   = Fb_p;    FptrSU_p = FbSU_p;
          Fptr_r   = Fb_r;    FptrSU_r = FbSU_r;
          Fptr_g   = Fb_g;    FptrSU_g = FbSU_g;
          EpEptr   = EpE_b;   EpPptr= EpP_b;
          FFb->Fill(Ep, weight);
        } else if (abs((*prtn_pdgid)[j])<5) { //Light quark (u,d,s,c) jets
          prqjet->Fill(Ep, p4r_probe.Pt()/p4r_tag.Pt(), weight); //q-jet resp
          pRpGptr  = pRpGlq;
          pTppRptr = pTppRlq;
          Fptr     = Flq;      FptrSU   = FlqSU;
          Fptr_p   = Flq_p;    FptrSU_p = FlqSU_p;
          Fptr_r   = Flq_r;    FptrSU_r = FlqSU_r;
          Fptr_g   = Flq_g;    FptrSU_g = FlqSU_g;
          EpEptr   = EpE_lq;   EpPptr   = EpP_lq;
          FFlq->Fill(Ep, weight);
        } else if ((*prtn_pdgid)[j]==21) {	//Gluon jets
          prgjet->Fill(Ep, p4r_probe.Pt()/p4r_tag.Pt(), weight); //g-jet resp
          pRpGptr  = pRpGg;
          pTppRptr = pTppRg;
          Fptr     = Fg;      FptrSU   = FgSU;
          Fptr_p   = Fg_p;    FptrSU_p = FgSU_p;
          Fptr_r   = Fg_r;    FptrSU_r = FgSU_r;
          Fptr_g   = Fg_g;    FptrSU_g = FgSU_g;
          EpEptr   = EpE_g;   EpPptr   = EpP_g;
          FFg->Fill(Ep, weight);
        } else continue;			//Undetermined flavour
        EpP->Fill(     Ep,             p4r_probe.Pt(),     weight);
        EpPptr->Fill(  Ep,             p4r_probe.Pt(),     weight);
        EpE->Fill(     p4r_probe.E(),  Ep/p4r_probe.E(),   weight);
        EpEptr->Fill(  p4r_probe.E(),  Ep/p4r_probe.E(),   weight);
        Fptr_p->Fill(  pTp,            F,                  weight);
        Fjet_p->Fill(  pTp,            F,                  weight);
        Fptr_r->Fill(  p4r_probe.Pt(), F,                  weight);
        Fjet_r->Fill(  p4r_probe.Pt(), F,                  weight);
        Fptr_g->Fill(  p4g_probe.Pt(), F,                  weight);
        Fjet_g->Fill(  p4g_probe.Pt(), F,                  weight);
        Fptr->Fill(    Ep,             F,                  weight);
        Fjet->Fill(    Ep,             F,                  weight);
        pTEp->Fill(    pTp,            p4g_probe.Pt()/pTp, weight);
        FFa->Fill(     Ep,                                 weight);
        pRpGptr->Fill( p4g_probe.Pt(), p4r_probe.Pt(),     weight);
        pRpGa->Fill(   p4g_probe.Pt(), p4r_probe.Pt(),     weight);
        pTppRptr->Fill(  pTp,          p4r_probe.Pt(),     weight);
        pTppRa->Fill(  pTp,            p4r_probe.Pt(),     weight);
        //Systematic uncertainties
        dA_E=dAjetsE[i_probe];  dB_E=dBjetsE[i_probe];  dC_E=dCjetsE[i_probe];
          if (!isnan(dA_E) && !isnan(dB_E) && !isnan(dC_E)) {
          //sigma = sqrt( (H^{-1})^{ij} (\partial_i F) (\partial_j F))
          FSU = sqrt( pow(GetAer()*dA_E,2)
                     +pow(GetBer()*dB_E,2)
                     +pow(GetCer()*dC_E,2)
                     +2*GetABer()*dA_E*dB_E //Covar. matrix is symmetric => 2*
                     +2*GetACer()*dA_E*dC_E
                     +2*GetBCer()*dB_E*dC_E )/p4r_probe.E();
          FptrSU->Fill(  Ep,             FSU, weight);
          FjetSU->Fill(  Ep,             FSU, weight);
          FptrSU_p->Fill(pTp,            FSU, weight);
          FjetSU_p->Fill(pTp,            FSU, weight);
          FptrSU_r->Fill(p4r_probe.Pt(), FSU, weight);
          FjetSU_r->Fill(p4r_probe.Pt(), FSU, weight);
          FptrSU_g->Fill(p4g_probe.Pt(), FSU, weight);
          FjetSU_g->Fill(p4g_probe.Pt(), FSU, weight);
        }
        continue;	//Only one flavour may be associated with a jet
      }
    } //Loop partons

    //If the old list of cut events is not read, a new one is written
    if (!GetuseEarlierCuts()) passedCuts[jentry]=true;
  } //Loop Tree entries

  TGraph *chf   = new TGraph("data_and_MC_input/chf.txt");
  TGraph *chnhf = new TGraph("data_and_MC_input/chnhf.txt");
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

  cout << "all events:        " << nentries    << endl;
  cout << "tag muon cuts:     " << mumuCut     << endl;
  cout << "Invariant mass:    " << invM        << endl;
  cout << "Tag probe cuts:    " << tagProbeCut << endl;
  cout << "btb tag and probe: " << b2b         << endl;
  cout << "alpha cut:         " << alpha       << endl;
  cout << "Low met:           " << lowMet      << endl;


  //Charged hadron efficiency Profile
  TCanvas *c4   = new TCanvas("c4","c4",500,500);
  TGraph *PFeff = new TGraph("data_and_MC_input/hadron_efficiency_PF.txt");
  TGraph *TRKeff = new TGraph("data_and_MC_input/hadron_efficiency_TRK.txt");
  PFeff->SetMarkerStyle(kFullCircle ); TRKeff->SetMarkerStyle(kOpenCircle );
  PFeff->SetMarkerColor(kRed);         TRKeff->SetMarkerColor(kBlue+1);
  TAxis *PFaxis = PFeff->GetXaxis();
  PFaxis->SetLimits(0.1,300);             // along X
  PFeff->GetHistogram()->SetMaximum(1.1);   // along          
  PFeff->GetHistogram()->SetMinimum(0.4);  //   Y     
  PFeff->Draw("ap");
  TRKeff->Draw("samep");

  gStyle->SetOptStat();
  c4->SetLogx();
  chhEff->Draw("same");
  chhEff->SetLineColor(kBlack);
  chhEff->SetLineWidth(2);
  string savename = "./plots/trackingEff.eps";
  c4->Print(savename.c_str());


  //Save CMSJES TTree
  fout->Write();
  fout->Close();

  //Free single-particle response functions from memory
  delete fr_h; delete fr_hcal;
} //CMSJES::Loop


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

//MULTIPLE SAMPLE LOOPING FUNCTIONALITY
//  For looping through multiple samples in parallel

//A handle to call the Loop function of a given CMSJES object within a thread, 
//takes care of TThreads' "return value and arguments as (void*)" formalism
void* loopHandle(void* CMSJESToLoop) {
  CMSJES* respPtr;
  respPtr = (CMSJES*) CMSJESToLoop;
  respPtr->Loop();
  return 0;
}

//Call CMSJES::Loop() for multiple samples in parallel
//Params:	dj_in, gj_in	Ready-given CMSJES objects for dijet...
//				...and gamma+jet whose Loop to call
//		fitPars		If true, take A,B,C from this CMSJES object
void CMSJES::MultiLoop(CMSJES* zj_in, bool fitPars) {

  //Input filenames to read
  InputNameConstructor();

  //Instantiate new CMSJES objects to be run in parallel
  CMSJES* resp_zj;

  bool noInputGiven = true;

  //Instantiate new CMSJES objects where needed
   if (zj_in) {	//Use given Z+jet CMSJES object
    //MultiLoop: using input CMSJES objects
    resp_zj = zj_in;
    noInputGiven = false;
  } else {	//No Z+jet object given
    //MultiLoop: Instantiating new CMSJES object
    resp_zj = new CMSJES(0,zjFile);
  }

  //MultiLoop may be called from FitHandle, ensure fit params. are taken from 
  //this CMSJES object and not read from the header "as is"
  if (fitPars) {
    resp_zj->SetABC(GetA(),GetB(),GetC());
  }

  //Instantiate the threads
  TThread* t_zj;
  t_zj = new TThread("t_zj", loopHandle, (void*) resp_zj);

  //Start running the threads
  t_zj->Run();

  //Gather the threads together after execution
  t_zj->Join();

  //Free memory
  delete t_zj; //Threads always instantiated here

  if (noInputGiven) {delete resp_zj;}

} //MultiLoop

//End MULTIPLE SAMPLE LOOPING FUNCTIONALITY

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
//Find the params A,B,C to fit to D0 data. Minimize LSQ-sum using Gauss-Newton.
//In the D0 studies, the parameter C was constrained close to 1 by a penalty 
//term in order to get physically meaningful fit results. Also other ways to
//constrain the fit more (or in other ways) have been provided for reference
void CMSJES::FitGN()
{
  //Fix some fit parameters to ineffective values. Set to false in D0 studies,
  //but implemented here for reference
  bool fixA = false;  bool fixB = false;  bool fixC = false;

  //Ensure first that at least some fitting functionality is enabled
  if (!GetzjFitting()) {
    cout << "ERROR: Z+jet fitting enabled!" << endl;
    return;
  }

  //To avoid boilerplate, store the following combinations in sampleIndices:
  //  0		if fit to Z+jet sample only

  //The vector is to be used via:
  //  for (int s: sampleIndices) {index s picks components for studied samples}
  vector<int> sampleIndices;
  if (GetzjFitting()) sampleIndices.push_back(0);

  //Construct input and output files
  InputNameConstructor(); //Input filenames to read

  string zjOut = "./output_ROOT_files/CMSJES_"+zjFile+".root";
  vector<string> outs; outs.push_back(zjOut);//Handle

  /* Fitting functionality starts here */
  
  //Init
  unsigned int nIter = 0;	//Stepper to count #iterations
  int const dim = (fixA?0:1) + (fixB?0:1) + (fixC?0:1);

  if (dim==0) {cout << "ERROR: all fit parameters fixed" << endl;  return;}

  //Fit constraint scale factors: 1/pow(desiredStdDev,2). 0=ineffective
  //double LA = 0;  double LB = 0;  double LC = 1.0/pow(0.01,2);
  double LA = 0;  double LB = 0;  double LC = 1.0/pow(0.015,2);

  //iA,iB,iC act as column indices: not 0,1,2 if some parameters are fixed
  int const iA = 0;
  int const iB = (fixA?0:1);
  int const iC = (fixA?0:1) + (fixB?0:1);
  double H[dim*dim];		//3x3 Hessian as a C-style array (for TMatrixD)
  for (int i=0; i!=dim*dim; ++i) H[i]=0;
  double vec_arr[dim];		//Dummy array for ROOT vector usage
  for (int i=0; i!=dim; ++i) vec_arr[i]=0;
  TVectorD ABC( dim,vec_arr);	//Vector of current fit parameter values
  TVectorD grad(dim,vec_arr);	//Gradient
  TVectorD step(dim,vec_arr);	//Iteration step to change A,B,C values by
  TMatrixD HES( dim,dim,H,"");	//Hessian
  TMatrixD Hinv(dim,dim,H,"");	//Inverse Hessian
  TMatrixD Cov( dim,dim,H,"");	//An estimate for the covariance matrix
  double chi2 = 0.0;		//To store the least square (LSQ) sum (chi^2)
  double dAchi2=0, dBchi2=0, dCchi2=0;	//Partial derivatives of LSQ sum

  //Vector of initial params, to be compared to gradient
  if (!fixA) vec_arr[iA] = GetA();
  if (!fixB) vec_arr[iB] = GetB();
  if (!fixC) vec_arr[iC] = GetC();
  ABC.SetElements(vec_arr);

  //Objects to read CMSJES output
  //[0]=dijet, [1]=gammajet, [2]=zjet, xjdY=derivative of pTprobe/pTtag w.r.t. Y (x=d,g)
  TFile*    file[3];  TProfile* prof[3];  //Files and prpTbalFit profiles
  TProfile* dA[3];    TProfile* dB[3];    TProfile* dC[3];
  double fit[3];      double fitdA[3];    double fitdB[3];  double fitdC[3];

  //Get data points to vector handles
  vector<double> zjCMSv, zjERv2; //ERv2: errors squared
  
  //Data points for CMS pT balance
  for (int i=0; i!=ndata_pTbal; ++i) { //Allocated here
    if (GetzjFitting()) {
      zjCMSv.push_back(zj_pTbal[i]);
      zjERv2.push_back(pow(zj_pTbal_ER[i],2)); 
    }
  }  

  //Handles to the above data and uncertainty vectors to enable sample indexing
  vector<vector<double>> CMSv; CMSv.push_back( zjCMSv ); //Change the name later
  vector<vector<double>> ERv2; ERv2.push_back(zjERv2); //Change the name later

  //#D.O.F. for chi^2 normalization -- non-rigorous, but D0 apparently did this
  //#D.O.F.="Number of data points to fit to minus number of free fit parameters"
  double n_dof = (GetzjFitting() ? (double) ndata_pTbal : 0) - (double) dim
                +(fixA || fabs(LA)>0.01 ? 1:0)  //Constrained param.s are not...
                +(fixB || fabs(LB)>0.01 ? 1:0)  //...free, hence they do not...
                +(fixC || fabs(LC)>0.01 ? 1:0); //...contribute to "-dim"

  if (n_dof<1) {cout << "ERROR: #D.O.F. < 1 in FitGN" << endl;  return;}

  cout<<"Inverses of CMS data unertainties squared, weigh LSQ sum terms:"<<endl;
  for (int i=0; i!=ndata_pTbal; ++i) {  //Don't use sampleIndices for print format
    if (GetzjFitting()) printf("zj: 1/sigma^2[%d]=%-9.3f",  i,1/ERv2[0][i]);
    printf("\n");
  }
  cout<<"^The above factors may make |gradient| large, don't panic."<<endl;

  //Instantiate and setup CMSJES objects to read in Z+jet TTree
  CMSJES* zjet = new CMSJES(0,zjFile);
  zjet->SetprintProg(     false);
  zjet->SetcontHistos(    false);
  zjet->SetuseEarlierCuts(GetuseEarlierCuts());

  do { //The fit calculation loop

    cout << "\n* FitGN: Iteration " << nIter << endl;

    //Reinit., update fit param. values and introduce constraints' contributions
    //         N.B. all constraints may not be active, this depends on LA,LB,LC
    chi2   = LA*pow(ABC.GetMatrixArray()[iB]-1, 2)/n_dof  //Constrain near...  
            +LB*pow(ABC.GetMatrixArray()[iB],   2)/n_dof  //...default values:
            +LC*pow(ABC.GetMatrixArray()[iC]-1, 2)/n_dof; //  (A,B,C)=(1,0,1)
    cout << "Constraint contributions to chi2: " << chi2 << endl;

    for (int i=0; i!=dim*dim; ++i) H[i]=0;
    if (!fixA) {zjet->SetA(ABC.GetMatrixArray()[iA]);
                dAchi2 = 2*LA*(ABC.GetMatrixArray()[iA]-1);
                H[dim*iA+iA]+=2*LA;                        }
    if (!fixB) {zjet->SetB(ABC.GetMatrixArray()[iB]);
                dBchi2 =  2*LB*ABC.GetMatrixArray()[iB];
                H[dim*iB+iB]+=2*LB;                        }
    if (!fixC) {zjet->SetC(ABC.GetMatrixArray()[iC]);
                dCchi2 = 2*LC*(ABC.GetMatrixArray()[iC]-1);
                H[dim*iC+iC]+=2*LC;                        }

    //Calculate new values
    if (GetzjFitting()) zjet->Loop();
  
    //Retrieve CMSJES output
    for (int s : sampleIndices) {
      file[s] = TFile::Open(outs[s].c_str());
      file[s]->GetObject("prpTbalFit0",prof[s]);  file[s]->GetObject("dAf",dA[s]);
      file[s]->GetObject("dBf",dB[s]);         file[s]->GetObject("dCf",dC[s]);
    }

    for (int s : sampleIndices) {
      for (int i=0; i!=ndata_pTbal; ++i) { //GetBinContent indices start from 1
        fit[s]=prof[s]->GetBinContent(i+1);  
        fitdA[s]=dA[s]->GetBinContent(i+1);
        fitdB[s]=dB[s]->GetBinContent(i+1);  
        fitdC[s]=dC[s]->GetBinContent(i+1);
        chi2 += pow(fit[s]-CMSv[s][i],2)/ERv2[s][i]/n_dof; //LSQ sum
        //NaN check
        if (isnan(fitdA[s]) || isnan(fitdB[s]) || isnan(fitdC[s])) {
          cout << "Nonsensical gradient at dijet data point " << i << ": "
               << fitdA[s]<<", "<<fitdB[s]<<", "<<fitdC[s]<<", skip" << endl;  
          continue;
        }
        //Gradient components
        if (!fixA) dAchi2 += 2*(fit[s]-CMSv[s][i])*fitdA[s]/ERv2[s][i];
        if (!fixB) dBchi2 += 2*(fit[s]-CMSv[s][i])*fitdB[s]/ERv2[s][i];
        if (!fixC) dCchi2 += 2*(fit[s]-CMSv[s][i])*fitdC[s]/ERv2[s][i];
        //The relevant components of Gauss-Newton's Hessian  estimate
        if (!fixA         ) H[iA       ]+=2*fitdA[s]*fitdA[s]/ERv2[s][i];
        if (!fixA && !fixB) H[iB       ]+=2*fitdA[s]*fitdB[s]/ERv2[s][i];
        if (!fixA && !fixC) H[iC       ]+=2*fitdA[s]*fitdC[s]/ERv2[s][i];
        if (!fixB         ) H[dim*iB+iB]+=2*fitdB[s]*fitdB[s]/ERv2[s][i];
        if (!fixB && !fixC) H[dim*iB+iC]+=2*fitdB[s]*fitdC[s]/ERv2[s][i];
        if (!fixC         ) H[dim*iC+iC]+=2*fitdC[s]*fitdC[s]/ERv2[s][i];
      }
    }

    //Fill the rest of the Hessian by symmetry
    if (!fixA && !fixB) H[dim*iB+iA] = H[iB];	//iA=0, but written...
    if (!fixA && !fixC) H[dim*iC+iA] = H[iC]; //...here for clarity
    if (!fixB && !fixC) H[dim*iC+iB] = H[dim*iB+iC];

    for (int s : sampleIndices) file[s]->Close();  //Close the files

    //Assign gradient elements
    if (!fixA) vec_arr[iA] = dAchi2;
    if (!fixB) vec_arr[iB] = dBchi2;
    if (!fixC) vec_arr[iC] = dCchi2;
    grad.SetElements(vec_arr);
    cout << "chi^2/n_dof:\t" << chi2 << endl;
    cout << "Gradient: ";
    if (!fixA) cout << grad.GetMatrixArray()[iA] << ", ";
    if (!fixB) cout << grad.GetMatrixArray()[iB] << ", ";
    if (!fixC) cout << grad.GetMatrixArray()[iC] << ", ";
    cout << "\tlength: " << sqrt(grad.Norm2Sqr())     << endl;

    //Invert the Hessian and find the covariance matrix from the inverse
    HES.SetMatrixArray(H);	//Reset the values
    Hinv = HES.Invert();

    //Update parameter values for new iteration
    step = Hinv*grad;

    ABC -= step;

    cout << "Step length: " << sqrt(step.Norm2Sqr())
         << " (" << sqrt((Hinv*grad).Norm2Sqr()) << ")" << endl;

    Cov = Hinv;	//Fit param. errors from covariance matrix i.e. inverse Hessian

    //Print current parameter values and uncertainties
    if (!fixA) {cout << "A: "  << ABC.GetMatrixArray()[iA]   << "\t+-"
                     << sqrt(fabs(Cov.GetMatrixArray()[iA])) << endl;     }
    if (!fixB) {cout << "B: "  << ABC.GetMatrixArray()[iB]   << "\t+-"
                     << sqrt(fabs(Cov.GetMatrixArray()[dim*iB+iB]))<<endl;}
    if (!fixC) {cout << "C: "  << ABC.GetMatrixArray()[iC]   << "\t+-"
                     <<sqrt(fabs(Cov.GetMatrixArray()[dim*iC+iC]))<<"\n"<<endl;}

    //Check if fit converged
    if (sqrt((Hinv*grad).Norm2Sqr()) < GetepsilonMin()) {	//Step length
      cout << "Minimum found, step size reduced enough\n" << endl; 
      break;
    } else if (chi2<1) {cout<<"Minimum found, chi2/n_d0f < 1\n"<<endl; break;}

    ++nIter;			//Advance stepper

    //The next iteration should use the same set of events as earlier
    zjet->SetuseEarlierCuts(true);

  } while (nIter!=GetmaxIter()); //The fit calculation loop

  //Fit parameter errors are the diagonal elements of the covariance matrix,
  //e.g. Cov_AA = sigma_A^2. Param. correlations from off-diagonal elments
  double Aer_temp  = (fixA      ?0:sqrt(fabs(Cov.GetMatrixArray()[iA       ])));
  double Ber_temp  = (fixB      ?0:sqrt(fabs(Cov.GetMatrixArray()[dim*iB+iB])));
  double Cer_temp  = (fixC      ?0:sqrt(fabs(Cov.GetMatrixArray()[dim*iC+iC])));
  double ABer_temp = (fixA||fixB?0:          Cov.GetMatrixArray()[iA    +iB]  );
  double ACer_temp = (fixA||fixC?0:          Cov.GetMatrixArray()[iA    +iC]  );
  double BCer_temp = (fixB||fixC?0:          Cov.GetMatrixArray()[dim*iB+iC]  );

  //Free memory
  delete zjet;

  //Obtain uncertainty histograms using this CMSJES object's MultiLoop
  cout<<"* Finding uncert. histos *"<<endl;
  //Set results to this CMSJES
  if (!fixA) {
    SetA(ABC.GetMatrixArray()[iA]);
    SetAer(Aer_temp);
    if (!fixB) SetABer(ABer_temp);
    if (!fixC) SetACer(ACer_temp);
  }
  if (!fixB) {
    SetB(ABC.GetMatrixArray()[iB]);
    SetBer(Ber_temp);
    if (!fixC) SetBCer(BCer_temp);
  }
  if (!fixC) {SetC(ABC.GetMatrixArray()[iC]);  SetCer(Cer_temp);}

  SetuseEarlierCuts(false);

  MultiLoop(); // Has to be fixed for Z+jet probably

  //Write results to terminal and file
  cout << "*** FitGN result ***" << endl;
  cout << "Parameters\t Errors\t Correlations" << endl;
  cout << "A: "<<GetA()<<"\t+-"<<GetAer()<<"\t AB: "<<GetABer()<<"\n"
       << "B: "<<GetB()<<"\t+-"<<GetBer()<<"\t AC: "<<GetACer()<<"\n"
       << "C: "<<GetC()<<"\t+-"<<GetCer()<<"\t BC: "<<GetBCer()<<endl;
  ofstream output;
  output.open("fitOutput.txt", std::ios::app);	//Append file, do not overwrite
  output << "    //";
  if      (zjFile.find("P6")!=string::npos) output << "P6";
  else if (zjFile.find("P8")!=string::npos) output << "P8";
  else if (zjFile.find("H7")!=string::npos) output << "H7";
  output << " G-N: " << "chi2/n_d0f=" << chi2; //Divided by n_dof before
  output << "\n" << "    A    = " << GetA()
                 <<";\t\tB    = " << GetB()
                 <<";\t\tC    = " << GetC() << ";\n"
                 << "    Aer  = " << GetAer()
                 <<  ";\tBer  = " << GetBer()
                 <<  ";\tCer  = " << GetCer() << ";\n"
                 << "    ABer = " << GetABer()
                 <<  ";\tACer = " << GetACer()
                 <<  ";\tBCer = " << GetBCer() << ";\n\n";
  output.close();
  cout << "Parameters and errors written to fitOutput.txt" << endl;

} //FitGN

//-----------------------------------------------------------------------------
//A function to combine several response function plots stored as TProfiles
//in "CMSJES_X.root" files.
//MConly and fitOnly are Master flags, overriding all others if true
void CMSJES::plotPT(int gen, int Nevt, bool MConly, bool fitOnly)
{
  bool plotOurMC  = true;
  bool plotCMSMC   = true;
  bool plotCMSdata = true;
  bool plotFit    = false;

  //Choose filenames to open
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);

  //Initialize histograms and open ROOT files and fetch the stored objects
  TFile* fzj = TFile::Open(zjetFile.c_str()); // Z+jet file

  //TProfile* przj=0; 
  TProfile* przj_f=0; TProfile* przj_pTp=0;

  /* Z+jet */
  fzj->GetObject("prpTbal", przj_pTp);	//Z+jet response pTp
  fzj->GetObject("prpTbalFit0",przj_f);    //Z+jet fit to data
  //TH1D* hzj     = przj->ProjectionX();
  TH1D* hzj_pTp = przj_pTp->ProjectionX();
  TH1D* hzj_f   = przj_f->ProjectionX();

  //CMS data points for Z+jet
  TGraphErrors* dzj = new TGraphErrors(); //Z+jet data
  TGraph* mc_zj = new TGraph();	          //Z+jet MC

  //Filled circle (4 hollow circle); 1 black, 2 red, 4 blue, 3/8 green
  dzj->SetMarkerStyle(8);    dzj->SetMarkerColor(  kGreen+2);
  mc_zj->SetMarkerStyle(4);  mc_zj->SetMarkerColor(kGreen+2);

  //hzj->SetLineColor(kGreen+2);
  hzj_pTp->SetLineColor(kBlack);
  hzj_f->SetMarkerStyle(kFullDiamond);  
  hzj_f->SetMarkerColor(kGreen-6);

  //CMS pT-balance data points and errors
  for (int i=0; i!=ndata_pTbal; ++i) {	//CMS pT-bal data
    dzj->SetPoint(i, zj_pTp[i], zj_pTbal[i]);
    dzj->SetPointError(i, 0, zj_pTbal_ER[i]);
  }
  for (int i=0; i!=nMC_pTbal; ++i) {	//CMS pT-bal. MC
    mc_zj->SetPoint(i, zj_MC_pTp[i], zj_MC_pTbal[i]); //Can add errors since those exist
  }

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
  //double ld_horiz[2] = {       0.43, 0.67};
  double lz_horiz[2] = {0.67, 0.89};
  double vertical[2] = {0.13, 0.37};
  if (MConly || fitOnly) vertical[1] -= 0.12;
  else {
    if (!plotCMSMC  ) vertical[1] -= 0.06;
    if (!plotOurMC ) vertical[1] -= 0.06;  //...they appear properly...
    if (!plotCMSdata) vertical[0] += 0.06;
    if (!plotFit   ) vertical[0] += 0.06;  //...of plot flags
  }
  TLegend* lz = new TLegend(lz_horiz[0],vertical[0],lz_horiz[1],vertical[1]);
  lz->SetBorderSize(0);	//No box around legend
  lz->SetFillStyle( 0);	//No background fill

  if (plotCMSMC  ) lz->AddEntry(mc_zj,"#font[132]{CMS Z+jet MC}", "p");
  //if (plotOurMC )  lz->AddEntry(hzj,  "#font[132]{Our Z+jet MC}", "l");
  if (plotOurMC )  lz->AddEntry(hzj_pTp,  "#font[132]{Our Z+jet MC pTp-bins}", "l");
  if (plotCMSdata) lz->AddEntry(dzj,  "#font[132]{CMS Z+jet data}","p");
  if (plotFit   )  lz->AddEntry(hzj_f,"#font[132]{Our Z+jet data fit}","p");


  //Main plot
  setup->Draw();
  //if (plotOurMC )  {hzj->Draw(  "SAME"       );}
  if (plotOurMC )  {hzj_pTp->Draw(  "SAME"       );}
  if (plotCMSdata) {dzj->Draw(  "P SAME"     );}
  if (plotFit   )  {hzj_f->Draw("HIST P SAME");}
  if (plotCMSMC  ) {mc_zj->Draw("P SAME"     );}
  lz->Draw();	//Legends

  //Save plot
  string saveTo = "./plots/pT-bal/";
  saveTo = saveTo + (MConly?"MC_":"") + (fitOnly?"data_fit_":"") + savename;

  canv->Print(saveTo.c_str());

  //Free memory
  delete setup;  delete lz;   delete pad1;   delete canv;
  delete dzj;  delete mc_zj;

} //plotPT

//PlotMPF
void CMSJES::plotMPF(int gen, int Nevt)
{
  //Choose filenames to open
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);

  //Initialize histograms and open ROOT files and fetch the stored objects
  //  TH1 params: name, title, #bins, #lowlimit, #highlimit
  TFile* fzj = TFile::Open(zjetFile.c_str());
  TProfile *przj_MPF=0;

  /* 1: Z+jet */
  fzj->GetObject("prMPF" ,przj_MPF);

  TH1D* hzj_MPF = przj_MPF->ProjectionX();

  //CMS data and MC points
  TGraph* mc_zj_MPF    = new TGraph(); TGraph* d_zj_MPF    = new TGraph(); //Z+jet MPF

  TGraphErrors* mc_zj_MPFntI = new TGraphErrors(nMC_MPFntI,zj_MC_pTp_MPFntI,
                                                    zj_MC_MPFntI,0,zj_MC_MPFntI_ER);

  //CMS Z+jet MPF data points
  for (int i=0; i!=ndata_MPF; ++i) {
    d_zj_MPF->SetPoint(i,zj_pTp_MPF[i],zj_MPF[i]);
  }

  //CMS Z+jet MPF MC simulation points
  for (int i=0; i!=nMC_MPF; ++i) {
    mc_zj_MPF->SetPoint(i, zj_MC_pTp_MPF[i], zj_MC_MPF[i]);  
  }

  //Canvas
  TCanvas* canv_MPF = new TCanvas("MPF","",600,600);
  canv_MPF->SetLeftMargin(0.13);	//To fit vertical axis labels
  canv_MPF->SetBottomMargin(0.13);
  //Style setup
  hzj_MPF ->SetLineColor( kBlack);
  mc_zj_MPF->SetMarkerStyle( kOpenCircle);    mc_zj_MPF->SetMarkerColor( kBlack  );
  mc_zj_MPFntI->SetMarkerStyle( kOpenCircle); mc_zj_MPFntI->SetMarkerColor( kBlack  );


  d_zj_MPF->SetMarkerStyle(kFullCircle);    d_zj_MPF->SetMarkerColor(kBlack);

  //Legend
  TLegend* lz_MPF = new TLegend(0.62,0.70,0.89,0.89);
  lz_MPF->SetBorderSize(0);
  //lz_MPF->AddEntry(hzj_MPF, "#font[132]{Our Z+jet MPF MC}", "l");
  lz_MPF->AddEntry(hzj_MPF, "#font[132]{Our toy MC      }", "l");

  //lz_MPF->AddEntry(d_zj_MPF,  "#font[132]{CMS jecsys Z+jet MPF data}", "p");
  //lz_MPF->AddEntry(mc_zj_MPF, "#font[132]{CMS jecsys Z+jet MPF MC}", "p");

  lz_MPF->AddEntry(mc_zj_MPFntI, "#font[132]{FullSim}", "p");

  //Title and axis setup
  hzj_MPF->SetStats(0); //Suppress stat box
  hzj_MPF->SetTitle("");
  //hzj_MPF->SetLineWidth(2);
  hzj_MPF->SetAxisRange(0.82,1.02,"Y"); //Vertical axis limits
  //hzj_MPF->SetAxisRange(0.7,1.05,"Y"); //Vertical axis limits

  //hzj_MPF->SetAxisRange(0.55,1.1,"Y"); //Vertical axis limits
  hzj_MPF->GetYaxis()->SetTitleFont(133);
  int titleSize = 24; //Common title size everywhere
  hzj_MPF->GetYaxis()->SetTitleSize(titleSize);
  hzj_MPF->GetXaxis()->SetMoreLogLabels();
  hzj_MPF->GetXaxis()->SetNoExponent();
  hzj_MPF->GetXaxis()->SetTitleFont(133);
  hzj_MPF->GetXaxis()->SetTitleSize(titleSize);
  canv_MPF->SetLogx();
  hzj_MPF->GetYaxis()->SetTitle("#font[12]{R}_{MPF}^{Z+jet}");
  hzj_MPF->GetYaxis()->SetTitleOffset(1.5);
  hzj_MPF->GetXaxis()->SetTitleOffset(1);

  //Savefile name setup
  string savename = "./plots/mpf/MPF_zmmjet";
  string MPFtitle = hzj_MPF->GetTitle();
  cout << MPFtitle << endl;
  savename+=".eps";

  //Plot
  hzj_MPF->Draw();
  //hzj_MPF_f->Draw("HISTO P SAME"); // Fit now disabled from the plot
  //mc_zj_MPF->Draw("P SAME");
  mc_zj_MPFntI->Draw("P SAME");
  //d_zj_MPF->Draw("P SAME");
  lz_MPF->Draw();

  //Save plot
  canv_MPF->Print(savename.c_str());
} //plotMPF


//-----------------------------------------------------------------------------
/* Avoid boiler plate and do axis setup in flavCorr() here */
void axisSetup(TAxis* xAxis, TAxis* yAxis, string Xtitle, string Ytitle) {
  xAxis->SetTitle(Xtitle.c_str());
  yAxis->SetTitle(Ytitle.c_str());
  yAxis->SetTitleFont(133);
  yAxis->SetTitleOffset(1.0);
  yAxis->SetTitleSize(20);
  xAxis->SetTitleFont(133);
  xAxis->SetTitleSize(18);
  xAxis->SetTitleOffset(0.9);
}

//-----------------------------------------------------------------------------
/* A handle to axisSetup for TH1D objects */
void histoSetup(TH1D* h, string Xtitle, string Ytitle) {
  axisSetup(h->GetXaxis(), h->GetYaxis(), Xtitle, Ytitle);
}
//-----------------------------------------------------------------------------
/* A handle to axisSetup for TGraphErrors objects */
void graphSetup(TGraphErrors* g, string Xtitle, string Ytitle) {
  axisSetup(g->GetXaxis(), g->GetYaxis(), Xtitle, Ytitle);
}

//-----------------------------------------------------------------------------
//A function to calculate the MC hadron response R 
//Params:	id		Particle PDGID
//		pseudorap	Particle pseudorapidity eta
//		energy		Particle energy
//		pT		Particle transverse momentum
//		pos		Flag to demand the result must be positive
//                              - this same function is also handy for finding
//                                derivatives, which may be negative.
//		frE		TF1* to use for calculating electron response
//		frMU		            -||-            muon     response
//		frG		            -||-            gamma    response
//		frH		            -||-            hadron   response
//		fA,fB,fC	Param.s for fitting to D0 data
//              MC, FIT, HR	Flags to find MC/fit/hadron response
//              ret**		References where to put the resulting response
void CMSJES::Response(int pdgid, double pseudorap, double energy, double pT, double Rt, 
                      double Bfield, TF1* frH, bool pos, 
                      double fA, double fB, double fC, bool MC, bool FIT, bool HR, 
                      double& retMC, double& retFIT, double& retH)
{
  retFIT = 0.0; retH = 0.0;
  bool zero = false; //If true, return zero responses

  double sfCh  = 0.0; //Charged particle step function
  double sfN   = 0.0; //Neutral hadron step function
  double cat1 = 0.0; double cat2 = 0.0; double cat3 = 0.0; //Responses for different groups

  if (pT > 0.3) sfCh = 1.0; // Step function for charged particles 
  if (pT > 3.0) sfN  = 1.0; // Step function for neutral particles  

  //if (energy > 0.3) sfCh = 1.0; // Step function for charged particles 
  //if (energy > 3.0) sfN  = 1.0; // Step function for neutral particles  

  //Check if particle outside good eta region
  if (fabs(pseudorap) > 5.2) zero = true;

  unsigned int row = int(fabs(pseudorap)*10); //Param matrix row from |eta|

  //Assert there's no pi^0 (PDGID 111) or eta (221) after parton shower
  if (abs(pdgid)==111 || abs(pdgid)==221) { 
    cout << "WARNING: pi^0 (111) or eta (221) found! PDGID: " << pdgid
         << "Returning zero response" << endl; zero = true;
  }

  //Charged particle not reaching calorimeters
  if ( fabs(Charge(pdgid)) && (pT < 0.3*Bfield*0.5*Rt) ) zero = true;

  //Neutrino responses are zero
  if (isNeutrino(abs(pdgid))) zero = true;

  //CALCULATE RESPONSES
  for (int i_r=0; i_r<(zero?0:1); ++i_r) {
    
    //Hadron response from pi fit
    //frH->SetParameters(params_pi_EHE[row][0], params_pi_EHE[row][1], //EHE
    //                   params_pi_EHE[row][2], 1, 0, 1); 
    //respH  = 0.55*frH->Eval(energy);
    //frH->SetParameters(params_pi_HHe[row][0], params_pi_HHe[row][1], //HHe
    //                   params_pi_HHe[row][2], 1, 0, 1); 
    //respH += 0.45*frH->Eval(energy);

    frH->SetParameters(params_cat1[row][0], params_cat1[row][1],
                       params_cat1[row][2], 1, 0, 1);
    cat1 = frH->Eval(energy);

    frH->SetParameters(params_cat2[row][0], params_cat2[row][1],
                       params_cat2[row][2], 1, 0, 1);
    cat2 = frH->Eval(energy);

    frH->SetParameters(params_cat3[row][0], params_cat3[row][1],
                       params_cat3[row][2], 1, 0, 1);
    cat3 = frH->Eval(energy);

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
        retH = cat1*sfN;
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
        retH = cat2*sfN;
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
        retH = cat3*sfN;
        break;
      
      /*
      //CHARGED HADRONS
      case 211 :  //pi^+-
      case 321 :  //K^+-
      case 2212 : //p
      case 3112 : //Sigma^-
      case 3222 : //Sigma^+
      case 3312 : //Xi^-
      case 3334 : //Omega^-
        if (fabs(pseudorap) > 2.5) retMC = cat2*sfN; //Check if out of tracker
        else retMC = sfCh; 
        break;

      //NEUTRAL HADRONS
      case 130 :  //K^0_L
      case 310 :  //K^0_S
      case 3122 : //Lambda
      case 2112 : //n
      case 3212 : //Sigma^0
      case 3322 : //Xi^0
        retMC = cat2*sfN;
        break;
      */
      default : 
        zero=true;
        cout << "Unknown particle PDG: " << pdgid << endl;
        continue;	 
    }
  } 

  //Set results. Check for NaN and negative results if positive demanded
  if (!MC || zero || isnan(retMC)  || (pos && retMC <=0)) retMC =0;
  if (!FIT|| zero || isnan(retFIT) || (pos && retFIT<=0)) retFIT=0;
  if (!HR || zero || isnan(retH)   || (pos && retH  <=0)) retH =0;

} //Response

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


//A function to find and plot flavor dependent correction factors.
//Param		plot	if false, only reads in the F_corr histograms
//		gen	Preset value for which generator to use
void CMSJES::flavCorr(bool plot, int gen, int Nevt)
{
  bool fitMode = true;	//Draw fitted TF1s on top of the histos
  bool Eprm    = true;	//True: F(E'), false: F(pT') at intpretation 0
  bool pTgen   = true;	//True: F(pT^gen_jet), false: F(pT^MC_jet) at int.pre. 1
  bool uncertF     = false;	//Draw F uncertainties?
  bool uncertFcorr = true;	//Draw Fcorr uncertainties?

  //Set color codes
  int black = kGray+3;  int red  = 2;   int blue  = 9;	//Dark shades
  int gray  = kGray+1;  int lred = 46;  int lblue = 33;	//Light shades

  //Choose filenames to open
  string nameAdd, in_z;
  plotQuery(nameAdd, in_z, gen,Nevt);

  //Check which generator was used for producing the files asked for
  string genStr="";
  if      (in_z.find("P6")!=string::npos) genStr="P6_";
  else if (in_z.find("P8")!=string::npos) genStr="P8_";
  else if (in_z.find("H7")!=string::npos) genStr="H7_";

  int const Nf=3;	//#Jet flavour classes: b, g, lq
  int const Ns=1;	//#Sample types: Z+jet
  int const NI=2;	//#Plots w/ diff. interpretation for hor. axis variable

  //Retrieve flavor-dependent factor F profiles to these pointers
  //  Naming: F[flavour]_[gen/reco/fit]_[dijet/gammajet]
  TProfile* F[Nf][Ns][NI]; TProfile* Fjet[NI]; TProfile* Fbstd[NI]; 
  //Standard gamma+jet Fb for obtaining weights
  //...and their upper and lower limits (for syst. uncert. SU) here
  TProfile* FSU[Nf][Ns][NI]; TProfile* FjetSU[NI];
  //For storing TH1D projections of the above TProfiles
  TH1D* h_F[Nf][Ns][NI]; TH1D* h_FSU[Nf][Ns][NI]; TH1D* h_Fjet[NI]; TH1D* h_FjetSU[NI];
  //The TFiles to open
  TFile* fz;

  /* z+jet */
  fz   = TFile::Open(in_z.c_str());
  if (!fz) {
    cout << "Error opening files! Exiting" << endl;
    return;
  }
  //b-jets
  if (Eprm)  {fz->GetObject( "Fb",      F[0][0][0]);
              fz->GetObject( "FbSU",  FSU[0][0][0]);}
  else       {fz->GetObject( "Fb_p",    F[0][0][0]);
              fz->GetObject( "FbSU_p",FSU[0][0][0]);}
  if (pTgen) {fz->GetObject( "FbSU_g",FSU[0][0][1]);
              fz->GetObject( "Fb_g",    F[0][0][1]);}
  else       {fz->GetObject( "FbSU_r",FSU[0][0][1]);
              fz->GetObject( "Fb_r",    F[0][0][1]);}

  //Always retrieve standard sample's Fb, needed for Fcorr calculation (new avg)
  if (Eprm ) fz->GetObject("Fb",   Fbstd[0]);
  else       fz->GetObject("Fb_p", Fbstd[0]);
  if (pTgen) fz->GetObject("Fb_g", Fbstd[1]);
  else       fz->GetObject("Fb_r", Fbstd[1]);

  //Gluon jets, light quark jets and all jets (Z+jet specific, for avg.)
  if (Eprm) {
    fz->GetObject("Fg",    F[1][0][0]);  fz->GetObject("FgSU",    FSU[1][0][0]);
    fz->GetObject("Flq",   F[2][0][0]);  fz->GetObject("FlqSU",   FSU[2][0][0]);
    fz->GetObject("Fjet",     Fjet[0]);  fz->GetObject("FjetSU",     FjetSU[0]);
  } else {
    fz->GetObject("Fg_p",  F[1][0][0]);  fz->GetObject("FgSU_p",  FSU[1][0][0]);
    fz->GetObject("Flq_p", F[2][0][0]);  fz->GetObject("FlqSU_p", FSU[2][0][0]);
    fz->GetObject("Fjet_p",   Fjet[0]);  fz->GetObject("FjetSU_p",   FjetSU[0]);
  }
  if (pTgen) {
    fz->GetObject("Fg_g",  F[1][0][1]);  fz->GetObject("FgSU_g", FSU[1][0][1]);
    fz->GetObject("Flq_g", F[2][0][1]);  fz->GetObject("FlqSU_g",FSU[2][0][1]);
    fz->GetObject("Fjet_g",   Fjet[1]);  fz->GetObject("FjetSU_g",  FjetSU[1]);
  } else {
    fz->GetObject("Fg_r",  F[1][0][1]);  fz->GetObject("FgSU_r", FSU[1][0][1]);
    fz->GetObject("Flq_r", F[2][0][1]);  fz->GetObject("FlqSU_r",FSU[2][0][1]);
    fz->GetObject("Fjet_r",   Fjet[1]);  fz->GetObject("FjetSU_r",  FjetSU[1]);
  }

  //Project TProfiles to TH1Ds
  for (int a=0; a!=NI; ++a) {
    for (int s=0; s!=Ns; ++s) {
      for (int f=0; f!=Nf; ++f) {
        h_F[f][s][a]   = F[f][s][a]->ProjectionX();
        h_FSU[f][s][a] = FSU[f][s][a]->ProjectionX();	//Syst. uncert.
      }
    }
    h_Fjet[a]   = Fjet[a]->ProjectionX();
    h_FjetSU[a] = FjetSU[a]->ProjectionX();
  }

  /* Conversion to prtcl lvl jet pT from Z+jet sample */
  TCanvas* canvConv = new TCanvas("canvConv","",400,400);
  canvConv->SetLeftMargin(0.12);
  TProfile *prEpP,    *prEpP_b,   *prEpP_g,   *prEpP_lq   = 0; 
  TProfile *prRtrue,  *prRtrue_b, *prRtrue_g, *prRtrue_lq = 0;
  TProfile *prpTppRb, *prpTppRg,  *prpTppRlq = 0;
  fz->GetObject( "EpP",      prEpP     );  prEpP->SetStats(     0);
  fz->GetObject( "EpP_g",    prEpP_g   );  prEpP_g->SetStats(   0);
  fz->GetObject( "EpP_lq",   prEpP_lq  );  prEpP_lq->SetStats(  0);
  fz->GetObject( "pRpGa",    prRtrue   );  prRtrue->SetStats(   0);
  fz->GetObject( "pTppRg",   prpTppRg  );  prpTppRg->SetStats(  0);
  fz->GetObject( "pTppRlq",  prpTppRlq );  prpTppRlq->SetStats( 0);
  fz->GetObject( "pTppRb",   prpTppRb  );  prpTppRb->SetStats(  0);
  fz->GetObject( "EpP_b",    prEpP_b   );  prEpP_b->SetStats(   0);
  fz->GetObject( "pRpGb",    prRtrue_b );  prRtrue_b->SetStats( 0);
  fz->GetObject( "pRpGg",    prRtrue_g );  prRtrue_g->SetStats( 0);
  fz->GetObject( "pRpGlq",   prRtrue_lq);  prRtrue_lq->SetStats(0);

  string pwrlaw   = "[0]*(1-[1]*pow(x,[2]-1))";	//D. Groom parametrization

  TF1* fEpP       = new TF1("fEpP",     "pol1",          10, 145);
  TF1* fEpP_b     = new TF1("fEpP_b",   "pol1",          10, 145);
  TF1* fEpP_g     = new TF1("fEpP_g",   "pol1",          10, 145);
  TF1* fEpP_lq    = new TF1("fEpP_lq",  "pol1",          10, 145);

  TF1* fRtrue;   TF1* fRtrue_b;  TF1* fRtrue_g;  TF1* fRtrue_lq;
  TF1* fpTppRb;  TF1* fpTppRg;   TF1* fpTppRlq;

  fRtrue        = new TF1("fRtrue",    "pol1",         0,  200);
  fRtrue_b      = new TF1("fRtrue_b",  "pol1",         0,  200);
  fRtrue_g      = new TF1("fRtrue_g",  "pol1",         0,  200);
  fRtrue_lq     = new TF1("fRtrue_lq", "pol1",         0,  200);
  fpTppRb       = new TF1("fpTppRb",   "pol1",         0,  200);
  fpTppRg       = new TF1("fpTppRg",   "pol1",         0,  200);
  fpTppRlq      = new TF1("fpTppRlq",  "pol1",         0,  200);

  prEpP->Fit(  fEpP,  "Q");    prEpP_b->Fit( fEpP_b, "Q");
  prEpP_g->Fit(fEpP_g,"Q");    prEpP_lq->Fit(fEpP_lq,"Q");


  TF1* fF[Nf][Ns][NI];
  stringstream fFname;
  for (int f=0; f!=Nf; ++f) {
    for (int s=0; s!=Ns; ++s) {
      for (int a=0; a!=NI; ++a) {
        fFname.str("");
        fFname << "fF_" << (f==0?"b_":(f==1?"g_":"lq_"))
                        << "z_" << (a==0?"Ep":"pT");
        fF[f][s][a] = new TF1(fFname.str().c_str(),pwrlaw.c_str(),30,145);
        fF[f][s][a]->SetParameters(1.15,0.7,0.85);
        F[ f][s][a]->Fit(fF[f][s][a], "Q");
      }
    }
  }

  fRtrue->SetParameters(   1.15,0.7,0.85);
  fRtrue_b->SetParameters( 1.15,0.7,0.85);
  fRtrue_g->SetParameters( 1.15,0.7,0.85);
  fRtrue_lq->SetParameters(1.15,0.7,0.85);

  TLegend* RtrueLegend = new TLegend(0.7,0.13,0.9,0.37);
  RtrueLegend->SetFillStyle(0); RtrueLegend->SetBorderSize(0);
  RtrueLegend->AddEntry(fRtrue_lq,"#font[12]{lq}#font[132]{-jets}", "l");
  RtrueLegend->AddEntry(fRtrue,   "#font[132]{All jets}",           "l");
  RtrueLegend->AddEntry(fRtrue_b, "#font[12]{b}#font[132]{-jets}",  "l");
  RtrueLegend->AddEntry(fRtrue_g, "#font[12]{g}#font[132]{-jets}",  "l");
  cout <<"\n\n*** Fitting pol1 to (p_T^gen, p_T^MC) ***" << endl;
  cout <<        "All jets: "<< endl;  prRtrue->Fit(   fRtrue   );
  cout <<        "\nb-jets: "<< endl;  prRtrue_b->Fit( fRtrue_b );
  cout <<        "\ng-jets: "<< endl;  prRtrue_g->Fit( fRtrue_g );
  cout <<"\n(u,d,s,c)-jets: "<< endl;  prRtrue_lq->Fit(fRtrue_lq);
  cout <<"\n\n*** Fitting pol1 to (p_T', p_T^MC) ***" << endl;
  cout <<        "\nb-jets: "<< endl;  prpTppRb->Fit( fpTppRb );
  cout <<        "\ng-jets: "<< endl;  prpTppRg->Fit( fpTppRg );
  cout <<"\n(u,d,s,c)-jets: "<< endl;  prpTppRlq->Fit(fpTppRlq);
  //Style setup
  fEpP->SetLineColor(     kOrange+2);  fEpP->SetTitle(          ""  );
  fEpP_b->SetLineColor(   red      );  fEpP_b->SetTitle(        ""  );
  fEpP_g->SetLineColor(   black    );  fEpP_g->SetTitle(        ""  );
  fEpP_lq->SetLineColor(  blue     );  fEpP_lq->SetTitle(       ""  );
  fpTppRb->SetLineColor(  red      );  fpTppRb->SetTitle(       ""  );
  fpTppRg->SetLineColor(  black    );  fpTppRg->SetTitle(       ""  );
  fpTppRlq->SetLineColor( blue     );  fpTppRlq->SetTitle(      ""  );
  fRtrue->SetLineColor(   kOrange+2);  fRtrue->SetTitle(        ""  );
  fRtrue_b->SetLineColor( red      );  fRtrue_b->SetTitle(      ""  );
  fRtrue_g->SetLineColor( black    );  fRtrue_g->SetTitle(      ""  );
  fRtrue_lq->SetLineColor(blue     );  fRtrue_lq->SetTitle(     ""  );
  prRtrue->SetLineColor(  kOrange+2);  prRtrue_b->SetLineColor( red );
  prRtrue_g->SetLineColor(black    );  prRtrue_lq->SetLineColor(blue);

  //Draw and save conversion factor plots
  //E'
  string convsavename = "./plots/pTconversion/Ep_";
  fEpP->Draw();
  fEpP_b->Draw("SAME"); fEpP_g->Draw("SAME"); fEpP_lq->Draw("SAME");
  convsavename = convsavename + genStr + nameAdd + ".eps";
  canvConv->Print(convsavename.c_str());
  //pT'
  convsavename = "./plots/pTconversion/pTp_";
  prpTppRlq->Draw(); fpTppRlq->Draw("SAME"); prpTppRb->Draw("SAME");
  prpTppRg->Draw("SAME"); fpTppRb->Draw( "SAME"); fpTppRg->Draw( "SAME");
  RtrueLegend->Draw();
  convsavename = convsavename + genStr + nameAdd + ".eps";
  canvConv->Print(convsavename.c_str());
  //pTgen
  convsavename = "./plots/pTconversion/pT_gen_";
  prRtrue->Draw();          fRtrue->Draw(   "SAME");
  prRtrue_b->Draw("SAME");  prRtrue_g->Draw("SAME");  prRtrue_lq->Draw("SAME");
  fRtrue_b->Draw( "SAME");  fRtrue_g->Draw( "SAME");  fRtrue_lq->Draw( "SAME");
  RtrueLegend->Draw();
  convsavename = convsavename + genStr + nameAdd + ".eps";
  canvConv->Print(convsavename.c_str());

  //Find a new shifted Fjet TProfile proper avg.ing. The contribution of b-jets
  //must no be taken from the b-enriched sample but the unbiased one. However,
  //there are so few of them that the b-jet contribution becomes negligible and
  //we average here only by the g-jet and lq-jet weights. These are returned by 
  //The function TProfile->GetBinEntries(i), i>=1.
  TF1* FjetS = new TF1("FjetS",pwrlaw.c_str(),20,145);
  TH1D* h_FjetS[NI];	//Shifted <F>_{Z+jet}
  TF1* fFjetS[NI];
  double nEtot=0;  int nE_g=0;  int nE_lq=0;
  stringstream FjetsName;
  double init, shift_b, shift_g, shift_lq = 0;
  int Nb, Ng, Nlq, Ntot = 0;
  double FbS, FgS, FlqS, FbSstd = 0;
  for (int a=0; a!=NI; ++a) {	//Horizontal axis interpretations
    for (int f=0; f!=Nf; ++f) {
      for (int s=0; s!=Ns; ++s) {
        FjetsName.str("");
        FjetsName << "h_FS_" << (f==0?"b_":(f==1?"g_":"lq_"))
                             << "z_" << (a==0?"Ep":"pT");
      }
    }
    FjetsName.str("");
    FjetsName << "h_FjetS_" << a;
    h_FjetS[a] = new TH1D(FjetsName.str().c_str(), "",
                          F[1][0][a]->GetXaxis()->GetNbins(),
                          F[1][0][a]->GetXaxis()->GetXmin(),
                          F[1][0][a]->GetXaxis()->GetXmax()  );
    for (int s=0; s!=Ns; ++s) {	//Sample types
      for (int i=1; i!=F[1][s][a]->GetNbinsX()+1; ++i) {
        Nb  = 1e6*Fbstd[  a]->GetBinEntries(i); //The factors 1e6 guard...
        Ng  = 1e6*F[1][s][a]->GetBinEntries(i); //...for NaN, but will...
        Nlq = 1e6*F[2][s][a]->GetBinEntries(i); //...eventually cancel

        Ntot = Nb + Ng + Nlq;
        
        init = F[1][s][a]->GetBinCenter(i); //g, lq have same initial binning
        if (a==0 && Eprm) {shift_b  = init -     fpTppRb->Eval(   init);
                           shift_g  = init -     fpTppRg->Eval(   init);
                           shift_lq = init -    fpTppRlq->Eval(   init);}
        else if (a==0)    {shift_b  = init -      fEpP_b->Eval(   init);
                           shift_g  = init -      fEpP_g->Eval(   init);
                           shift_lq = init -     fEpP_lq->Eval(   init);}
        else if (!pTgen)  {cout << "Define ftEMpG since pTgen false";}
			 /*{shift_b  = init - init*ftEMpG->Eval(   init);
                           shift_g  = init - init*ftEMpG->Eval(   init);
                           shift_lq = init - init*ftEMpG->Eval(   init);}*/
        else              {shift_b  = init -      fRtrue_b->Eval( init);
                           shift_g  = init -      fRtrue_g->Eval( init);
                           shift_lq = init -      fRtrue_lq->Eval(init);}
        FbS =F[0][s][a]->GetBinContent(i)+( fF[0][s][a]->Eval(init+shift_b)
                                           -fF[0][s][a]->Eval(init        ));
        FgS =F[1][s][a]->GetBinContent(i)+( fF[1][s][a]->Eval(init+shift_g)
                                           -fF[1][s][a]->Eval(init        ));
        FlqS=F[2][s][a]->GetBinContent(i)+( fF[2][s][a]->Eval(init+shift_lq)
                                           -fF[2][s][a]->Eval(init         ));
        FbSstd=Fbstd[a]->GetBinContent(i)+( fF[0][s][a]->Eval(init+shift_b)
                                           -fF[0][s][a]->Eval(init        ));
        h_F[0][s][a]->SetBinContent(i,FbS );
        h_F[1][s][a]->SetBinContent(i,FgS );
        h_F[2][s][a]->SetBinContent(i,FlqS);

        if (s==0) { //Does this make sense for Z+jet sample?
          h_FjetS[a]->SetBinContent(i,(FbSstd*Nb + FgS*Ng + FlqS*Nlq) / ((double) Ntot));
          h_FjetS[a]->SetBinError(i,sqrt(pow(h_Fjet[  a]->GetBinError(  i),2)
                                        +pow(h_FjetSU[a]->GetBinContent(i),2)));

        }
      } //TProfile bins
    } //Sample types
    FjetsName.str("");
    FjetsName << "fFjets_" << (a==0?"Ep":"pT");
    fFjetS[a] = new TF1(FjetsName.str().c_str(),pwrlaw.c_str(),20,145);
    fFjetS[a]->SetParameters(1.0,1.0,0.05);
    h_FjetS[a]->Fit(fFjetS[a],"Q");
  } //Horizontal axis interpretations

  delete canvConv;  delete RtrueLegend;  //Free memory

  //Fcorr = F normalized by Z+jet sample average (given by Fjet)
  TH1D* h_Fcorr[Nf][Ns][NI];
  for (int f=0; f!=Nf; ++f) {
    for (int s=0; s!=Ns; ++s) {
      for (int a=0; a!=NI; ++a) {
        h_Fcorr[f][s][a] = new TH1D(*(h_F[f][s][a]));
        h_Fcorr[f][s][a]->Divide(h_FjetS[a]);
      }
    }
  }

  cout << "Flavour dependent correction factors read" << endl;
  if (!plot) return;

  //Add syst. uncert. to the stat. uncert. already present in the histos:
  //Divide each F bin's highest and lowest values (as given by the uncertainty
  //bounds) by the highest or lowest boundary for the corresponding Fjet bin,
  //then take half of these "upper and lower bd." histo points' distance as the
  //uncertainty. Then add this in quadrature w/ stat. uncertainty
  for (int a=0; a!=NI; ++a) {		//Horizontal axis interpretation
    for (int i=1; i!=1+h_Fjet[a]->GetXaxis()->GetNbins(); ++i) {
      h_Fjet[a]->SetBinError(i,sqrt( pow(h_Fjet[a]->GetBinError(i),2)
                                    +pow(h_FjetSU[a]->GetBinContent(i),2)));
      for (int f=0; f!=Nf; ++f) {	//Jet flavour
        for (int s=0; s!=Ns; ++s) {	//Sample type Z+jet
          h_F[f][s][a]->SetBinError(i,sqrt(pow(h_F[f][s][a]->GetBinError(i),2)
                                     +pow(h_FSU[f][s][a]->GetBinContent(i),2)));
          h_Fcorr[f][s][a]->SetBinError(i,
            0.5*fabs(( h_F[f][s][a]->GetBinContent(i)
                      +h_F[f][s][a]->GetBinError(i))
                     /h_FjetS[a]->GetBinContent(i)
                    -( h_F[f][s][a]->GetBinContent(i)
                      -h_F[f][s][a]->GetBinError(i))
                     /h_FjetS[a]->GetBinContent(i)));
        }
      }
    }
  }

  //Axis setup
  string pTtitle = "#font[132]{#font[12]{p}_{T,jet}^{<MC>} (#font[12]{p}";
  if      (pTgen                         ) pTtitle += "_{T,jet}^{gen}";
  else                                     pTtitle += "_{T,jet}^{MC}";
  pTtitle += "-bins) [GeV]}";
  string pTtitleEp = "#font[132]{#font[12]{p}_{T,jet}^{<MC>} ";
  if (Eprm) pTtitleEp       += "(#font[12]{E'}-bins) [GeV]}";
  else      pTtitleEp       += "(#font[12]{p'}_{T}-bins) [GeV]}";
  string FcorrTitle = "#font[12]{F}_{corr}";
  vector<string> Fstrs;
  Fstrs.push_back("#font[12]{F_{b}}");
  Fstrs.push_back("#font[12]{F_{g}}");
  Fstrs.push_back("#font[12]{F_{(u,d,s,c)}}");
  int diaO = kOpenDiamond;  int sqrO = kOpenSquare;	  //Marker styles
  int cirO = kOpenCircle;   int diaF = kFullDiamond;
  int sqrF = kFullSquare;   int cirF = kFullCircle;
  for (int a=0; a!=NI; ++a) {
    for (int s=0; s!=Ns; ++s) {
      for (int f=0; f!=Nf; ++f) {
        if (a==0) {	//E'-bins
          h_F[f][s][a]->GetXaxis()->SetTitle(pTtitleEp.c_str());
          histoSetup(h_F[f][s][a],     pTtitleEp, Fstrs[f]    );
          histoSetup(h_Fcorr[f][s][a], pTtitleEp, FcorrTitle  );
        } else {	//pT-bins
          h_F[f][s][a]->GetXaxis()->SetTitle(pTtitle.c_str()  );
          histoSetup(h_F[f][s][a],     pTtitle,   Fstrs[f]    );
          histoSetup(h_Fcorr[f][s][a], pTtitle,   FcorrTitle  );
        }
        h_F[f][s][a]->SetStats(0);	  //Suppress stat boxes
        h_Fcorr[f][s][a]->SetStats(0);
      }
    }
    h_F[0][0][a]->SetMarkerStyle(cirF);
    h_F[1][0][a]->SetMarkerStyle(sqrF);
    h_F[2][0][a]->SetMarkerStyle(diaF);
    h_Fcorr[0][0][a]->SetMarkerStyle(cirF);
    h_Fcorr[1][0][a]->SetMarkerStyle(sqrF);
    h_Fcorr[2][0][a]->SetMarkerStyle(diaF);

    //Marker colors
    h_F[0][0][a]->SetMarkerColor(lred );
    h_F[1][0][a]->SetMarkerColor(black);
    h_F[2][0][a]->SetMarkerColor(blue );
    h_Fcorr[0][0][a]->SetMarkerColor(lred );
    h_Fcorr[1][0][a]->SetMarkerColor(black);
    h_Fcorr[2][0][a]->SetMarkerColor(blue );
  }
  
  /* F PLOTS */

  //To store savefile name
  string savename;

  //Indicate if the plots use D0 A,B,C in our MC
  string OMC = "#font[132]{Our MC}";

  //Where to place some additional LaTeX text in the plots
  double latexBD[3]={70.0, 85.0, 0.955};

  //Canvas and related setup
  TCanvas* canv = new TCanvas("canv","",400,400);
  canv->SetTicks(1,1);		//Axis ticks on both sides
  canv->SetLeftMargin(0.12);	//Room for axis label
  canv->SetBottomMargin(0.115);

  //Convert jet pT to particle level to be comparable w/ D0
  //Indices, growing order: [flavour: b,g,lq][sample: gj,dj][Hor.axis:E',pT]
  TGraphErrors* g_F[Nf][Ns][NI];  TGraphErrors* g_Fcorr[Nf][Ns][NI];
  TGraphErrors* g_Fjet[NI];       //Taken only from gamma+jet sample
  for (int a=0; a!=NI; ++a) {     //Axis interpretations
    for (int s=0; s!=Ns; ++s) {	  //Sample type
      for (int f=0; f!=Nf; ++f) { //Flavour
        g_F[f][s][a]=new TGraphErrors();  g_Fcorr[f][s][a]=new TGraphErrors();
      }
    }
    g_Fjet[a] = new TGraphErrors();
  }

  /* F level: copy points and uncertainties from histos */

  for (int a=0; a!=NI; ++a) { //Horizontal axis interpretations

    //All-flavour avg.
    for (int i=1; i!=1+h_Fjet[a]->GetXaxis()->GetNbins(); ++i) { //F bins
      g_Fjet[a]->SetPoint(i-1, h_FjetS[a]->GetBinCenter(i),
                               h_FjetS[a]->GetBinContent(i));
      g_Fjet[a]->SetPointError(i-1,  0, h_FjetS[a]->GetBinError(i)  );      
    }

    //Flavour-specific histos
    for (int s=0; s!=Ns; ++s) {		//Sample type
      for (int f=0; f!=Nf; ++f) {	//Jet flavour
        //(Re)init
        //Copy TH1D points to TGraphs for horizontal axis shifting
        for (int i=1; i!=1+h_F[f][s][a]->GetXaxis()->GetNbins(); ++i) {	//F bins
          //Copy the F histogram points to TGraphs, shift by pT conv. factor
          g_F[f][s][a]->SetPoint(i-1, h_F[f][s][a]->GetBinCenter(i),
                                      h_F[f][s][a]->GetBinContent(i));
          //Copy the uncertainties
          if (uncertF) g_F[f][s][a]->SetPointError(i-1, 0,
                                               h_F[f][s][a]->GetBinError(i)  );
          g_Fcorr[f][s][a]->SetPoint(i-1,h_Fcorr[f][s][a]->GetBinCenter(i),
                                         h_Fcorr[f][s][a]->GetBinContent(i));
          if (uncertFcorr) g_Fcorr[f][s][a]->SetPointError(i-1, 0,
                                             h_Fcorr[f][s][a]->GetBinError(i));
        } //Loop F bins
      } //Jet flavour
    } //Sample type

  } //Horizontal axis interpretations

  /* Fcorr level (if not taken care of above) */

  //Init legend etc.
  TLatex latexF;
  TLegend* lgF = new TLegend(0.50,0.15,0.92,0.50 );
  lgF->AddEntry(g_F[2][0][0],"#font[132]{Z+jet (#font[12]{u,d,s,c})-jets}", "p");
  lgF->AddEntry(g_F[1][0][0],"#font[132]{Z+jet gluon jets}", "p");
  lgF->AddEntry(g_F[0][0][0],"#font[132]{Z+jet #font[12]{b}-jets}", "p");

  //Setup the plots and draw
  for (int a=0; a!=NI; ++a) {	//Horizontal axis interpretations
    g_F[2][0][a]->SetLineColor(blue );
    g_F[1][0][a]->SetLineColor(black);
    g_F[0][0][a]->SetLineColor(lred );
    g_Fcorr[2][0][a]->SetFillColor(blue );  g_Fcorr[2][0][a]->SetLineWidth(0);
    g_Fcorr[1][0][a]->SetFillColor(black);  g_Fcorr[1][0][a]->SetLineWidth(0);
    g_Fcorr[0][0][a]->SetFillColor(lred );  g_Fcorr[0][0][a]->SetLineWidth(0);
    g_F[2][0][a]->SetMarkerStyle(  diaF );  g_F[2][0][a]->SetMarkerColor(blue );
    g_F[1][0][a]->SetMarkerStyle(  sqrF );  g_F[1][0][a]->SetMarkerColor(black);
    g_F[0][0][a]->SetMarkerStyle(  cirF );  g_F[0][0][a]->SetMarkerColor(lred );
    g_Fcorr[2][0][a]->SetMarkerStyle(diaF );
    g_Fcorr[2][0][a]->SetMarkerColor(blue );
    g_Fcorr[1][0][a]->SetMarkerStyle(sqrF );
    g_Fcorr[1][0][a]->SetMarkerColor(black);
    g_Fcorr[0][0][a]->SetMarkerStyle(cirF );
    g_Fcorr[0][0][a]->SetMarkerColor(lred );
    g_Fcorr[2][0][a]->SetFillStyle(3395);
    g_Fcorr[1][0][a]->SetFillStyle(3354);
    g_Fcorr[0][0][a]->SetFillStyle(3345);
    gStyle->SetHatchesSpacing(1.3);	//Fill pattern sparsity

    //F plots
    h_F[0][0][a]->SetAxisRange(-0.1,2.0,"Y");
    h_F[0][0][a]->GetYaxis()->SetTitle("#font[12]{F}");
    //h_F[0][0][a]->SetAxisRange(0,3000,"X");
    h_F[0][0][a]->Draw("P HISTO"     ); //Plots also lowest data point corresponding to the 
					//first Drawn object.
    h_F[1][0][a]->Draw("P HISTO SAME");
    h_F[2][0][a]->Draw("P HISTO SAME");

    lgF->SetBorderSize(0);  
    lgF->SetFillStyle(0);
    lgF->Draw();
    savename = "./plots/F/";
    savename = savename+"F_"+(a==0?"Ep_":"pTjet_")+genStr+nameAdd+".eps";
    canv->Print(savename.c_str());	//Save plot
  } //Horizontal axis interpretations

  /* F_CORR PLOTS */

  //Legends etc.
  TLegend* lgc;
  //Only gamma+jet drawn as comparison
  lgc = new TLegend(0.50, 0.15, 0.92, 0.50);
  lgc->SetBorderSize(0);       
  lgc->SetFillStyle(0);
  lgc->AddEntry(g_Fcorr[2][0][0], "#font[132]{Z+jet (#font[12]{u,d,s,c})-jets}", "p");
  lgc->AddEntry(g_Fcorr[0][0][0], "#font[132]{Z+jet #font[12]{b}-jets}","p");
  lgc->AddEntry(g_Fcorr[1][0][0], "#font[132]{Z+jet gluon jets}",    "p");

  TLatex latex;	latex.SetTextFont(132);	//For writing on the plot

  //Fcorr
  for (int a=0; a!=NI; ++a) {	//Horizontal axis interpretations
    h_Fcorr[1][0][a]->GetXaxis()->SetRangeUser(100,1300);//(15,125);
    //h_Fcorr[1][0][a]->SetAxisRange(15,1000,"X");
    h_Fcorr[1][0][a]->SetAxisRange(-1.0,3.0,"Y");
    h_Fcorr[1][0][a]->GetYaxis()->SetTitle("#font[132]{#font[12]{F}_{corr}}");
    h_Fcorr[1][0][a]->SetMarkerSize(0);
    h_Fcorr[1][0][a]->Draw("P HISTO");	//Ordering s.t. this one gives title
    if (uncertFcorr) {  //Draw uncertainty bands at the bottom
      h_Fcorr[1][0][a]->Draw("E3 SAME");
      h_Fcorr[0][0][a]->Draw("E3 SAME");
      h_Fcorr[2][0][a]->Draw("E3 SAME");
    }
    g_Fcorr[1][0][a]->Draw("P SAME"); //Draw horizontally shifted points 
    g_Fcorr[0][0][a]->Draw("P SAME");
    g_Fcorr[2][0][a]->Draw("P SAME");
    lgc->Draw();
    savename = "./plots/Fcorr/";
    savename = savename+"Fcorr_"+(a==0?"Ep_":"pTjet_")+genStr+nameAdd+".eps";
    canv->Print(savename.c_str()); //Save plot
  } //Horizontal axis interpretations




  delete canv;  
  TCanvas* canvFit = new TCanvas("","",400,400);
  canvFit->SetBottomMargin(0.11);
  canvFit->SetLeftMargin(0.12);
  canvFit->SetTicks(1,1);
  //Fit to F_corr (gamma+jet & EM+jet avg.) & draw fits if fitMode=true
  string funcForm;	//To store the function to be fitted (as a string)

  if (fitMode) {

    /* Uncomment the function to be used */
    //funcForm = "pol1";  			//Linear fit
    //funcForm = "pol2";  			//2nd deg. polynomial
    //funcForm = "pol3";  			//3rd deg. polynomial
    funcForm = "[0]*(1-[1]*pow(x,[2]-1))";	//Power law (D. Groom's param.)

    TF1* fit_b  = new TF1("fit_b",  funcForm.c_str(), 0, 140);
    TF1* fit_lq = new TF1("fit_lq", funcForm.c_str(), 0, 140);
    TF1* fit_g  = new TF1("fit_g",  funcForm.c_str(), 0, 140);
    fit_b->SetNpx(1e4);  fit_lq->SetNpx(1e4);  fit_g->SetNpx(1e4); //Resolution
    fit_b->SetLineColor( kRed-5);
    fit_g->SetLineColor( gray  );
    fit_lq->SetLineColor(lblue );

    double FcorrLO, FcorrHI;
    TH1D* h_Fcorr_avg[Nf][NI];
    for (int f=0; f!=Nf; ++f) {
      for (int a=0; a!=NI; ++a) {
        FcorrLO = h_Fcorr[f][0][a]->GetXaxis()->GetXmin();
        FcorrHI = h_Fcorr[f][0][a]->GetXaxis()->GetXmax();
        h_Fcorr_avg[f][a] = new TH1D("", "", h_Fcorr[f][0][a]->GetNbinsX(),
                                             FcorrLO,  FcorrHI             );
        h_Fcorr_avg[f][a]->SetStats(0);
      }
    }

    //Find an average of the Z+jet Fcorr histograms
    double Favg=0;  double Navg[Ns]; //Temps for avg
    for (int a=0; a!=NI; ++a) {	//Horizontal axis interpretations
      for (int f=0; f!=Nf; ++f) {	//Jet flavours
        for (int i=1; i!=1+h_Fcorr[f][0][a]->GetXaxis()->GetNbins(); ++i) {
          Navg[0] = 1e6*F[f][0][a]->GetBinEntries(i); //Factors 1e6 guard for NaN, 
						      //but will cancel.
          Favg =( Navg[0]*h_Fcorr[f][0][a]->GetBinContent(i)) / (Navg[0]);
          h_Fcorr_avg[f][a]->SetBinContent(i, Favg);
          h_Fcorr_avg[f][a]->SetBinError(i,h_Fcorr[f][0][a]->GetBinError(i));
        }
      }
    }

    //Turn the average histogram into TGraphErrors
    TGraphErrors* g_Fcorr_avg[Nf][NI];    TGraph* g_Fcorr_avg_p[Nf][NI];
    for (int a=0; a!=NI; ++a) {	//Horizontal axis interpretations
      for (int f=0; f!=Nf; ++f) {	//Jet flavours
        g_Fcorr_avg[f][a]   = new TGraphErrors();
        g_Fcorr_avg_p[f][a] = new TGraph();	//Just the points
        for (int i=1; i!=1+h_F[f][0][a]->GetXaxis()->GetNbins(); ++i) {
          //Copy the F histogram points to TGraphs, shift by pT conv. factors
          g_Fcorr_avg[f][a]->SetPoint(i-1, h_Fcorr_avg[f][a]->GetBinCenter(i),
                                      h_Fcorr_avg[f][a]->GetBinContent(i));
          g_Fcorr_avg_p[f][a]->SetPoint(i-1, h_Fcorr_avg[f][a]->GetBinCenter(i),
                                        h_Fcorr_avg[f][a]->GetBinContent(i));
          //Copy uncertainties
          g_Fcorr_avg[f][a]->SetPointError(i-1, 0, h_Fcorr_avg[f][a]->GetBinError(i));
        }
        graphSetup(g_Fcorr_avg[f][a], pTtitle, FcorrTitle);
        histoSetup(h_Fcorr_avg[f][a], pTtitle, FcorrTitle);
        h_Fcorr_avg[f][a]->SetAxisRange(0.9,1.02,"Y");
        h_Fcorr_avg[f][a]->SetAxisRange(20, 120, "X");
      } //Jet flavours

      //Colour and style setup
      g_Fcorr_avg[0][a]->SetLineColor(lred );
      g_Fcorr_avg[1][a]->SetLineColor(black);
      g_Fcorr_avg[2][a]->SetLineColor(blue );
      g_Fcorr_avg[0][a]->SetMarkerStyle(kFullCircle );
      g_Fcorr_avg[1][a]->SetMarkerStyle(kFullSquare );
      g_Fcorr_avg[2][a]->SetMarkerStyle(kFullDiamond);
      g_Fcorr_avg[0][a]->SetMarkerColor(lred );
      g_Fcorr_avg[1][a]->SetMarkerColor(black);
      g_Fcorr_avg[2][a]->SetMarkerColor(blue );
      g_Fcorr_avg[0][a]->SetFillColor(  lred );
      g_Fcorr_avg[1][a]->SetFillColor(  black);
      g_Fcorr_avg[2][a]->SetFillColor(  blue );
      g_Fcorr_avg[0][a]->SetFillStyle(3345);
      g_Fcorr_avg[1][a]->SetFillStyle(3354);
      g_Fcorr_avg[2][a]->SetFillStyle(3395);
      g_Fcorr_avg_p[0][a]->SetMarkerStyle(kFullCircle );
      g_Fcorr_avg_p[1][a]->SetMarkerStyle(kFullSquare );
      g_Fcorr_avg_p[2][a]->SetMarkerStyle(kFullDiamond);
      g_Fcorr_avg_p[0][a]->SetMarkerColor(lred );
      g_Fcorr_avg_p[1][a]->SetMarkerColor(black);
      g_Fcorr_avg_p[2][a]->SetMarkerColor(blue );
    } //Axis interpretations

    //Initial guesses for params, ROOT can't fit power law without them
    fit_b->SetParameters( 1.0, 1.0, 0.05);
    fit_lq->SetParameters(1.0, 1.0, 0.05);
    fit_g->SetParameters( 1.0, 1.0, 0.05);

    //Legend for the plots
    TLegend* lgfit  = new TLegend(0.55,  0.15, 0.9,   0.475);
    TLegend* lgfitU = new TLegend(0.565, 0.15, 0.795, 0.475); //Undelay main lg
    lgfit->SetBorderSize( 0);  lgfit->SetFillStyle( 0);
    lgfitU->SetBorderSize(0);  lgfitU->SetFillStyle(0);
    lgfit->AddEntry(g_Fcorr_avg[1][0], "#font[12]{g}#font[132]{-jets}",    "p");
    lgfit->AddEntry(fit_g,             "#font[12]{g}#font[132]{-jet fit}", "l");
    lgfit->AddEntry(g_Fcorr_avg[0][0], "#font[12]{b}#font[132]{-jets}",    "p");
    lgfit->AddEntry(fit_b,             "#font[12]{b}#font[132]{-jet fit}", "l");
    lgfit->AddEntry(g_Fcorr_avg[2][0],
                                "#font[12]{(u,d,s,c)}#font[132]{-jets}",   "p");
    lgfit->AddEntry(fit_lq,     "#font[12]{(u,d,s,c)}#font[132]{-jet fit}","l");
    lgfitU->AddEntry(g_Fcorr_avg[1][0], " ", "f");
    lgfitU->AddEntry(g_Fcorr_avg[1][0], " ", "l");
    lgfitU->AddEntry(g_Fcorr_avg[0][0], " ", "f");
    lgfitU->AddEntry(g_Fcorr_avg[0][0], " ", "l");
    lgfitU->AddEntry(g_Fcorr_avg[2][0], " ", "f");
    lgfitU->AddEntry(g_Fcorr_avg[2][0], " ", "l");

    TH1D* h_dum = new TH1D("","",10,20,120); //For setting up horizontal axis
    histoSetup(h_dum, pTtitle, FcorrTitle);  h_dum->SetAxisRange(0.9,1.02,"Y");
    h_dum->SetMarkerSize(0);                 h_dum->SetStats(0);

    string fitName;		//Filename for saving the fit plot

    for (int a=0; a!=NI; ++a) {	//Horizontal axis interpretations

      if (a==0) h_dum->GetXaxis()->SetTitle(pTtitleEp.c_str());
      else      h_dum->GetXaxis()->SetTitle(pTtitle.c_str()  );

      //Perform the fits
      cout << "*** Fitting to Fcorr in bins of "
           << (a==0 ? (Eprm?"E'":"pT'") : "pTgen") << " ***" << endl;
      printf("\nFitting Fbcorr\n" );  g_Fcorr_avg[0][a]->Fit(fit_b );
      printf("\nFitting Fgcorr\n" );  g_Fcorr_avg[1][a]->Fit(fit_g );
      printf("\nFitting Flqcorr\n");  g_Fcorr_avg[2][a]->Fit(fit_lq);

      //Plot
      h_Fcorr_avg[1][a]->SetMarkerSize(0);
      h_dum->Draw(              "P HISTO");  g_Fcorr_avg[1][a]->Draw("E3 SAME");
      g_Fcorr_avg[0][a]->Draw(  "E3 SAME");  g_Fcorr_avg[2][a]->Draw("E3 SAME");
      g_Fcorr_avg_p[1][a]->Draw("P SAME" );  fit_g->Draw(            "SAME"   );
      g_Fcorr_avg_p[0][a]->Draw("P SAME" );  fit_b->Draw(            "SAME"   );
      g_Fcorr_avg_p[2][a]->Draw("P SAME" );  fit_lq->Draw(           "SAME"   );
      TLatex latex_fit;              latex_fit.SetTextSize(0.037);
      latex_fit.DrawLatex(79,0.959,"#font[132]{Z+jet avg.}");
      lgfitU->Draw();  lgfit->Draw();

      //Save the plot
      fitName = "./plots/Fcorr/fits/";
      fitName = fitName+"Fcorr_"+(a==0?"Ep_":"pTjet_")+genStr+nameAdd+".eps";
      canvFit->Print(fitName.c_str());

    } //Horizontal axis interpretations

    delete lgfitU;  delete lgfit;  delete h_dum;
  } //if fitMode

  //Free memory
  delete lgc;  delete canvFit;
  for (int f=0; f!=NI; ++f) {
    for (int s=0; s!=Ns; ++s) {
      for (int a=0; a!=NI; ++a) {
        delete g_F[f][s][a];  delete g_Fcorr[f][s][a];  delete fF[f][s][a];
      }
    }
  }
} //flavCorr

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

