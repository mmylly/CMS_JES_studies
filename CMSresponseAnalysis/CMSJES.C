#define CMSJES_cxx
#include "CMSJES.h"

//Created using ROOT TMakeClass

void CMSJES::Loop()
{

  //greedy_pythia modes: 0=generic, 1=dijet, 2=gammajet, 3=Zjet, 4=ttbarjet
  //For now, check if dijet or gamma+jet sample file
  int studyMode = -1;
  if (ReadName.find("dijet")!=string::npos) {
    cout << "File is for dijet sample" << endl;
    studyMode = 1;
  } else if (ReadName.find("gammajet")!=string::npos) {
    cout << "File is for gamma+jet sample" << endl;
    studyMode = 2;
  } else if (ReadName.find("Zjet")!=string::npos) {
    studyMode = 3;
    cout << "File is for Z+jet sample, with stydy mode: "<< studyMode << endl;
  } else {
    cout << "Error: File not for dijet, gamma+jet, or Z+jet sample! Exiting." << endl;
    return;
  }

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  //Output file
  string outname = "./output_ROOT_files/CMSJES_" + ReadName;
  outname += ".root";	//Filetype suffix
  TFile *fout = new TFile(outname.c_str(),"RECREATE");

  //TProfile constructors /w custom binning
  //  "name", "header;x_axis;y_axis", nbinsx, binsx
  //  here binsx = array containing bin limits.
  //int const nbins = 13;
  int const nbins = 7;
  int const nbinsMPF = 15;
  //const double binsx[nbins] = {31.0,  36.5,  41.5,  47.0,  52.0,  62.0, 72.5,
  //                             87.0, 107.0, 130.0, 180.0, 350.0, 550.0      };

  //Check values
  const double binsx[nbins] = {82.75, 105.25, 132.75, 173.25, 228.0, 299.0, 380.0}; 


  const double binsxMPF[nbinsMPF] = {30.0,   39.0,  47.0,  55.0,  67.0,  78.0,
                                     94.0,  107.0, 130.0, 160.0, 205.0, 310.0,
                                     385.0, 470.0, 550.0                     };

  // CMS, should have R_cone = 0.4
  string Rcone = "R_{cone}=0.4";	//R_cone info in string form
  R_cone = 0.4;

  string EpTitle = Rcone;     EpTitle    += "RunCMS"; 
  string ETitle = Rcone;      ETitle     += "RunCMS"; 
  string MPFTitle = Rcone;    MPFTitle   += "RunCMS"; 
  string EpMPFTitle = Rcone;  EpMPFTitle += "RunCMS"; 

  //Time scale: // C_tau -> 10mm
  string ctauStr;
  ctauStr = ", c#tau=1 cm"; // Might have to change this
  EpTitle+=ctauStr;  ETitle+=ctauStr;  MPFTitle+=ctauStr;  EpMPFTitle+=ctauStr;

  EpTitle += ";E' [GeV];p_{T}^{probe}/p_{T}^{tag}";
  TProfile* prEp    = new TProfile("prEp", EpTitle.c_str(),      nbins-1,binsx);
  TProfile* prEpFit = new TProfile("prEpFit0", EpTitle.c_str(),  nbins-1,binsx);

  ETitle += ";E_{probe} [GeV];p_{T}^{probe}/p_{T}^{tag}";
  TProfile* prE   = new TProfile("prE",ETitle.c_str(),nbins-1,binsx);
  MPFTitle += ";E_{probe} [GeV];p_{T}^{reco}/p_{T}^{gen}";
  TProfile* prMPF_D0 = new TProfile("prMPF_D0",
                                    MPFTitle.c_str(),nbinsMPF-1,binsxMPF);
  EpMPFTitle += ";E' [GeV];R_{MPF}";
  TProfile* prMPF_Ep = new TProfile("prMPF_Ep", EpMPFTitle.c_str(),
			            nbinsMPF-1, binsxMPF             );
  TProfile* prMPF_EpFit = new TProfile("prMPF_EpFit", EpMPFTitle.c_str(),
			               nbinsMPF-1, binsxMPF                );

  //Jet flavour fraction histos: FFb = b-jets, FFg = g-jets, FFlq=(u,d,s,c)-jets
  TH1D* FFb = new TH1D("FFb",
          "Jet flavour fraction;#font[12]{p}_{T} [GeV];",nbins-1,binsx);
  TH1D* FFg = new TH1D("FFg",
          "Jet flavour fraction;#font[12]{p}_{T} [GeV];",nbins-1,binsx);
  TH1D* FFlq= new TH1D("FFlq",
          "Jet flavour fraction;#font[12]{p}_{T} [GeV];",nbins-1,binsx);
  TH1D* FFa = new TH1D("FFa", ";#font[12]{p}_{T} [GeV];",nbins-1,binsx); //All
  string FFstackTitle;
  FFstackTitle = "#font[132]{Run CMS}";
  FFstackTitle += ";#font[12]{E'} #font[132]{[GeV]}";
  FFstackTitle += ";#font[132]{Jet flavour fraction}";
  THStack* FFstack = new THStack("", FFstackTitle.c_str());

  //Tprofiles for quark and gluon jets
  TProfile* prqjet = new TProfile("prq",ETitle.c_str(),nbins-1,binsx);
  TProfile* prgjet = new TProfile("prg",ETitle.c_str(),nbins-1,binsx);

  //TProfiles for derivatives of pTprobe/pTtag
  TProfile* dAf = new TProfile("dAf",
                  "#partial_{A}p_{T}^{probe}/p_{T}^{tag}",nbins-1,binsx);
  TProfile* dBf = new TProfile("dBf",
                  "#partial_{B}p_{T}^{probe}/p_{T}^{tag}",nbins-1,binsx);
  TProfile* dCf = new TProfile("dCf",
                  "#partial_{C}p_{T}^{probe}/p_{T}^{tag}",nbins-1,binsx);

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
  int nF=13;  int nFr=25;  double FL=15;  double FH=145;  double FHr=140;
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
  TProfile* tEMpG= new TProfile("tEMpG",
            ";p_{T,probe}^{MC} [GeV];p_{T,tag}^{EM(default)}/p_{T,probe}^{gen}",
                                                                    nF,FL,FH);
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
  //  R_h^{MC} = p_h^(0)*( 1 - p_h^(1)*(E/0.75)^(p_h^(2)-1) )
  //Fit params. A=[3], B=[4], C=[5] only used for fitting to D0 data
  //  R_h^{data} = C*p_h^(0)*( 1 - A*p_h^(1)*(E/0.75)^(p_h^(2)+B-1) )
  TF1 *fr_h = new TF1("frh","[5]*[0]*(1-[3]*[1]*pow(x/0.75,[2]+[4]-1))",0,250);
  //gamma, e and mu response ~same in data and MC, no A B C fit params.
  //Photon response
  string Rgam = "0.25*[0]*( 1 + erf( (x + [1])/sqrt(2*[2]*[2]) ) )";
  Rgam +=               "*( 1 + erf( (x + [3])/sqrt(2*[4]*[4]) ) ) + [5]";
  TF1* fr_gam = new TF1("frg",Rgam.c_str(),0.1,251);
  //Electron response
  string Re = "0.25*[0]*( 1 + erf( (x + [1])/sqrt(2*[2]*[2]) ) )";
  Re +=               "*( 1 + erf( (x + [3])/sqrt(2*[4]*[4]) ) )";
  TF1* fr_e = new TF1("fre",Re.c_str(),0.1,251);
  //Muon response
  TF1* fr_mu = new TF1("frmu","([0]+[1]*x)*TMath::Landau(x,[2],[3])",0,251);
  //Pointer to one of the response functions at a time
  TF1* fr;
  //1st derivatives of hadron data response
  TF1* dAR = new TF1("dAR","-[5]*[0]*[1]*pow(x/0.75,[2]+[4]-1)",     0,251);
  TF1* dBR = new TF1("dBR",
             "-[3]*[5]*[0]*[1]*pow(x/0.75,[2]+[4]-1)*log(x/0.75)",   0,251);
  TF1* dCR = new TF1("dCR","[0]*(1 - [3]*[1]*pow(x/0.75,[2]+[4]-1))",0,251);

  //Higher resolution
  int reso = 10000;    fr_e->SetNpx(reso);
  fr_h->SetNpx(reso);  fr_gam->SetNpx(reso);  fr_mu->SetNpx(reso);
  dAR->SetNpx(reso);   dBR->SetNpx(reso);     dCR->SetNpx(reso);

  /* INITIALIZATIONS */

  Long64_t nbytes = 0, nb = 0;
  TLorentzVector tag;		//To contain leading photon or tag jet
  TLorentzVector probe;		//To contain the probe jet
  TLorentzVector p4;		//Particle 4-momentum temp.
  TLorentzVector p4j;		//Jet 4-momentum temp.
  TLorentzVector p4g_probe;	//Probe jet 4-mom., sum of generated particles  
  TLorentzVector p4g_tag;	//Tag    -||-    for dijet case
  TLorentzVector p4r_probe;	//Reconstr. probe 4-momentum, resp. included
  TLorentzVector p4r_tag;	//       -||-    for tag in dijet case
  TLorentzVector p4EM_tag;	//Tag    -||-
  TLorentzVector p4f_probe;	//w/ fit params to CMS data
  TLorentzVector p4f_tag;

  //Partial derivative values for a hadron
  double dARtemp = 0;  
  double dBRtemp = 0;
  double dCRtemp = 0;
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
  double pTp  = 0;		//pT' = E'/cosh(eta_det)
  double eta_gamma = 1.0;	//Max |eta| for photon in gamma+jet (ALT 1.0)
  double eta_muon  = 1.0;
  double eta_tag   = 0.4;	//Max |eta| for tag jet in dijet
  double eta_probe = 0.4;	//Max |eta| for probe jets
  vector<double> f_05;		//Fraction of jets' E w/in R<0.5 from jet axis
  vector<double> f_CH;		//Fraction of jets' E left in coarse HCAL
  vector<double> f_EM;		//Fraction of jets' E left in EMCAL
  double f_05jetMin = 0.5;	//Min. probe f_05 value  w/in [0,1], 0=inactive
  double f_05jetMinS= 0.1;	//^soft pT<15 GeV jets,  w/in [0,1], 0=inactive
  double f_EMjetMin = 0.05;	//Min. probe f_EM value  w/in [0,1], 0=inactive
  double f_EMjetMax = 0.95;	//Max. probe f_EM value  w/in [0,1], 1=inactive
  double f_CHjetMax = 1;//0.44;	//Max. probe f_CH value  w/in [0,1], 1=inactive
  double softPt = 15;		//Jets below this [GeV] are considered soft
  vector<TLorentzVector> jets_g;//Gen lvl jet 4-vectors
  vector<TLorentzVector> jets_r;//MC reco'd jet 4-vectors
  vector<TLorentzVector> jets_f;//Fit reco'd jet 4-vectors
  vector<double> Fden;		//F denominator:   sum_j R_j^MC   E_j^g
  vector<double> Fnum;		//F numerator:     sum_i R_i^data E_i^g
  double F;			//Temps to store F values for the probe jet
  int PDG = 1;			//Shorthand, to store a particle's PDGID
  int JI = 0;			//Shorthand, particle's jet index
  double phiMin = 3.0;		//Minimum azimuth angle between tag and probe
  double prtnPt = 0;		//Temp, find prtn lvl gamma w/ highest pT
  double pTmin_probe = 6;	//Minimum probe jet p_T (GeV)
  double pTmin_tag   = 6; 	//Minimum tag p_T (GeV) in dijet
  double pTmin_gamma = 7;	//Minimum tag gamma p_T (GeV) (Meas. 7 GeV)
  double pTmin_muon = 0;        //Minimum single tag muon pT (GeV)
  double pTmin_muon_tag = 0;    //Minimum tag muon pair pT (GeV)   
  bool   tagIsJet = false;	//Tag is among jets in gamma+jet mode
  double resp   = 1.0;	        //SPR value                (dummy init)
  double resp_f = 1.0;	        // -||- w/ fit params      (dummy init)
  double respEM = 1.0;	        // -||- for EM-object reco (dummy init)
  double R_MPF_D0= 0;		//D0 style MPF response
  double R_MPF_r = 0;		//MC-reco'd MPF response
  double R_MPF_f = 0;		//Fit reco'd MPF response
  double R_EMc = 0.3;		//EM-cluster radius, contributes to tag
  double FSU = 0.0;		//Temp for F systematic uncertainty calculations
  unsigned long njets;		//#jets in the event, for resizing vectors
  TLorentzVector NIJ_g;		//Gen lvl 4-vec sum of prtcls Not In Jets
  TLorentzVector NIJ_r;		//MC-reco lvl  -||-
  TLorentzVector NIJ_f;		//Fit reco lvl -||-
  TLorentzVector MET_r;		//MET for MPF response (MC reco)
  TLorentzVector MET_f;		//         -||-        (fitted param reco)
  
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
  //Machinery for histogram stacking
  vector<string> hTitles;
  THStack* hstack[3];
  THStack* hstackOther[3];
  if (studyMode==1) {
    hTitles.push_back("#font[132]{EM+jet probe particle content (gen)}");
    hTitles.push_back("#font[132]{EM+jet probe particle content (MC SPR)}");
    hTitles.push_back("#font[132]{EM+jet probe particle content (Fit)}");
  } else if (studyMode==2) {
    hTitles.push_back("#font[132]{#gamma+jet probe particle content (gen)}");
    hTitles.push_back("#font[132]{#gamma+jet probe particle content (MC SPR)}");
    hTitles.push_back("#font[132]{#gamma+jet probe particle content (Fit)}");
  } else {
    hTitles.push_back("#font[132]{Z+jet probe particle content (gen)}");
    hTitles.push_back("#font[132]{Z+jet probe particle content (MC SPR)}");
    hTitles.push_back("#font[132]{Z+jet probe particle content (Fit)}");
  }

  //Instantiate and setup composition histos if needed
  if (GetcontHistos()) {
    int const nbins_h = 27;	//#Bins in particle composition histograms
    const double bins_h_lo = 0.0;
    const double bins_h_hi = 540.0;
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
      h_pi[n]    = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //pi* +-
      h_p[n]     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Protons
      //Other particles (=strange particles for which D0 gave no response)
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

  //Loop Tree entries = events
  for (Long64_t jentry=0; jentry != nentries; ++jentry) {

    //Print progress for long runs
    if ((GetprintProg() && jentry % 1000==0) || Getverbose()) {
      cout << "Looping event " << jentry;
      if      (studyMode==1) cout << " in EM+jet";
      else if (studyMode==2) cout << " in gamma+jet";
      else if (studyMode==3) cout << " in Z+jet";
      cout << endl;
    }

    //Skip events that didn't pass cuts earlier. Useful in e.g. repeating Loop
    if (GetuseEarlierCuts() && passedCuts.size()>jentry && !passedCuts[jentry]) continue;
 
    Long64_t ientry = LoadTree(jentry);	//Load new event
    if (ientry < 0) break;		//When no more events
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;	//Cut is currently dummy in header

    //Reinit
    i_tag = 0;		i_probe = 0;	  p4.SetPtEtaPhiE(0,0,0,0);
    tag.SetPtEtaPhiE(0,0,0,0);		  probe.SetPtEtaPhiE(0,0,0,0);
    p4g_probe.SetPtEtaPhiE(0,0,0,0);	  p4g_tag.SetPtEtaPhiE(0,0,0,0);
    p4r_probe.SetPtEtaPhiE(0,0,0,0);      p4r_tag.SetPtEtaPhiE(0,0,0,0);
    p4f_probe.SetPtEtaPhiE(0,0,0,0);      p4f_tag.SetPtEtaPhiE(0,0,0,0);
    p4EM_tag.SetPtEtaPhiE(0,0,0,0);
    NIJ_g.SetPtEtaPhiE(0,0,0,0);          NIJ_r.SetPtEtaPhiE(0,0,0,0);
    NIJ_f.SetPtEtaPhiE(0,0,0,0);
    MET_r.SetPtEtaPhiE(0,0,0,0);          MET_f.SetPtEtaPhiE(0,0,0,0);
    jets_g.clear();     jets_r.clear();   jets_f.clear();
    Fden.clear();       Fnum.clear();  
    njets = (unsigned long)jet_pt->size();
    jets_g.resize(njets);   jets_r.resize(njets);
    jets_f.resize(njets);
    Fden.resize(njets);     Fnum.resize(njets);
    f_05.resize(njets);     f_CH.resize(njets);     f_EM.resize(njets);
    dAjetsE.resize(njets);  dAjetsX.resize(njets);  dAjetsY.resize(njets);
    dBjetsE.resize(njets);  dBjetsX.resize(njets);  dBjetsY.resize(njets);
    dCjetsE.resize(njets);  dCjetsX.resize(njets);  dCjetsY.resize(njets);
    for (int i=0; i!=jets_g.size(); ++i) {	//All objects have njets size
      jets_g[i].SetPtEtaPhiE(0,0,0,0);  jets_r[i].SetPtEtaPhiE(0,0,0,0);
      jets_f[i].SetPtEtaPhiE(0,0,0,0);
      f_05[i] = 0;     f_CH[i] = 0;     f_EM[i] = 0;
      Fden[i] = 0;     Fnum[i] = 0;
      dAjetsE[i] = 0;  dAjetsX[i] = 0;  dAjetsY[i] = 0;
      dBjetsE[i] = 0;  dBjetsX[i] = 0;  dBjetsY[i] = 0;
      dCjetsE[i] = 0;  dCjetsX[i] = 0;  dCjetsY[i] = 0;
    }
    tagIsJet=false;

    dA_E=0; dA_X=0; dA_Y=0; dB_E=0; dB_X=0; dB_Y=0; dC_E=0; dC_X=0; dC_Y=0;
    prtnPt=0;

    /**************** GAMMA+JET: FIND TAG PHOTON / EM-CLUSTER ****************/

    int gPDG=-22;  int gTAG=-2; //Gamma ID and tag, init to EM-cluster values 
    if (inSubDir=="can_repr_pprplots_photon_tag_2/") {gPDG=22;  gTAG=2;}
    if (studyMode == 2) {

      //It has prtn_tag==2 in gamma+jet case.
      i_tag = -137;	//This value wont be changed if gamma not found
      for (int a=0; a!= prtn_tag->size(); ++a) {
        // Find highest parton level pT *prtn_pt)[a] and store it to prtnPt
        // mark that index as i_tag = a.
        if ((*prtn_tag)[a]   == gTAG &&
            (*prtn_pdgid)[a] == gPDG &&
            (*prtn_pt)[a]    >  prtnPt ) {prtnPt=(*prtn_pt)[a];  i_tag=a;}
      }

      if (i_tag == -137) continue;	//No leading photon found
      //Parton level single photon tag:
      tag.SetPtEtaPhiE((*prtn_pt)[i_tag], (*prtn_eta)[i_tag],
                       (*prtn_phi)[i_tag],(*prtn_e)[i_tag]  );

      //Fast gen lvl cuts, more strict reco lvl cuts below
      if (fabs(tag.Eta())>eta_gamma || tag.Pt()<pTmin_gamma) continue;
      // PDG index not gamma? Probably since EM reconstruction only activated
      Response(PDG,tag.Eta(),tag.E(),tag.Pt(),fr_e,fr_mu,fr_gam,fr_h,true,
               1, 0, 1,  false, false, true,  resp, resp_f, respEM       );
      p4g_tag  = tag;
      p4EM_tag = tag*respEM;
      p4r_tag  = tag*respEM;
      p4f_tag  = tag*respEM;
    }

    /**************** Z+JET: FIND AND RECONSTRUCT TAG MUONS ****************/
    // Find how many muons in the events
    
    int muPDG=13;  int muTAG=3; //mu PDGID and tag 
    if (studyMode == 3) {
      if (Getverbose()) cout << "Entering Z+JET: FIND TAG" << endl;

      int i_tag1 = -137;	// These values won't change if...
      int i_tag2 = -731;	// ...muons not found.
      for (int a=0; a!= prtn_tag->size(); ++a) {
        // Find muons with tag == 3 and store those in i_tag1 and i_tag2
        if ((*prtn_tag)[a] == muTAG && abs((*prtn_pdgid)[a]) == muPDG) {
          if (i_tag1 == -137) i_tag1 = a;
          else if (i_tag2 == -731) i_tag2 = a;
          else {cout << "More than two muons with tag 3 in an event." << endl;
                continue;}
        }
      }
      
      if (i_tag1 == -137 && i_tag2 == -731) continue; //No two muons found

      //***** 1st muon *****
      //Parton level first tag muon:
      tag.SetPtEtaPhiE((*prtn_pt)[i_tag1], (*prtn_eta)[i_tag1],
                       (*prtn_phi)[i_tag1],(*prtn_e)[i_tag1]);

      //Fast gen lvl cuts, more strict reco lvl cuts below

      if (fabs(tag.Eta())>eta_gamma || tag.Pt()<pTmin_muon) continue;

      Response((*prtn_pdgid)[i_tag1],tag.Eta(),tag.E(),tag.Pt(),fr_e,fr_mu,fr_gam,fr_h,true,
               1, 0, 1, true, false, false, resp, resp_f, respEM       );

      p4g_tag  = tag;		//gen lvl
      p4r_tag  = tag*resp;	//MC lvl


      //***** 2nd muon ******
      tag.SetPtEtaPhiE((*prtn_pt)[i_tag2], (*prtn_eta)[i_tag2],
                       (*prtn_phi)[i_tag2],(*prtn_e)[i_tag2]);

      if (fabs(tag.Eta())>eta_gamma || tag.Pt()<pTmin_muon) continue;

      Response((*prtn_pdgid)[i_tag2],tag.Eta(),tag.E(),tag.Pt(),fr_e,fr_mu,fr_gam,fr_h,true,
               1, 0, 1, true, false, false,  resp, resp_f, respEM       );

      p4g_tag  += tag;		//gen lvl
      p4r_tag  += tag*resp;	//MC lvl
    }

    /***************** RECONSTRUCT JETS AND PARTICLES IN JETS *****************/

    if (Getverbose()) cout<<"Reconstructing jets and particles in jets"<<endl;

    //Note that only the 2 leading jets are likely to be probe or tag candidates
    for (int i=0; i != prtcl_pt->size(); ++i) {

      JI = (*prtcl_jet)[i];	                        //Which jet this particle belongs to
      PDG = abs((*prtcl_pdgid)[i]);
      p4.SetPtEtaPhiE((*prtcl_pt)[i], (*prtcl_eta)[i],	//Current prtcl 4-vec
                      (*prtcl_phi)[i],(*prtcl_e)[i]  );

      //Calculate responses. Store results to [resp, resp_f, respEM] tuples
      Response(PDG, p4.Eta(), p4.E(), p4.Pt(), fr_e, fr_mu, fr_gam, fr_h, true,
               GetA(), GetB(), GetC(), true, true, true, resp, resp_f, respEM );

      //Reconstruct jets
      //jets_g[ JI] += (PDG==13||!fidCuts(PDG,(*prtcl_pt)[i])?0:1)*p4;//Gen lvl
      jets_g[ JI] += (PDG==13 || isNeutrino(PDG) ? 0:1)*p4;           //Gen lvl
      jets_r[ JI] += p4*resp;   //MC reco
      jets_f[ JI] += p4*resp_f;                  //Fit reco always w/ MC'


      Fnum[JI]   += resp_f*p4.E();
      Fden[JI]   += resp*p4.E(); //F denominator reco'd w/ default params

      //Hadrons in jets: check what is left in HCAL
      if (PDG>99 && (GetStrangeB() || !isStrangeB(PDG))) {	// Hadrons
        //1/4 of E_jet to coarse HCAL.
        f_CH[JI] += resp*0.25*p4.E();
        //1/2 of E_jet to EMCALroot 
        f_EM[JI] += resp*0.5*p4.E();
      } else if (PDG==20 || PDG==22 || PDG==11 || PDG==13) {	// EM-interactive
        f_EM[JI] += resp*p4.E();
      }

      //Fraction of jet E within R=0.5 from jet axis (n.b. D0 cone algorithm
      // may produce monstrous jets even with R=0.5)
      p4j.SetPtEtaPhiE((*jet_pt)[JI], (*jet_eta)[JI],	//Gen jet 4-vec
                       (*jet_phi)[JI],(*jet_e)[JI]  );
      if (p4.DeltaR(p4j)<0.5) { // Should this be 0.4??
        f_05[JI] += resp*p4.E();
      }
      
      //Find derivatives of hadron (PDG>100) responses for 2 leading jets
      //Most likely probe candidates are the jets w/ indices 0 or 1. Thus to
      //save time, we only calculate the first 2 jets' derivatives.
      //Rare cases with i_probe>1 are calculated separately at a later stage. 
      if (PDG>99 && JI<2) {
        //Notice the fr_h are replaced by partial derivative functions
        //Other responses are not calculated
        Response(PDG, p4.Eta(), p4.E(), p4.Pt(),fr_e,fr_mu,fr_gam,dAR, false,
                 GetA(),GetB(),GetC(),false,true,false, resp,dARtemp,respEM );
        Response(PDG, p4.Eta(), p4.E(), p4.Pt(),fr_e,fr_mu,fr_gam,dBR, false,
                 GetA(),GetB(),GetC(),false,true,false, resp,dBRtemp,respEM );
        Response(PDG, p4.Eta(), p4.E(), p4.Pt(),fr_e,fr_mu,fr_gam,dCR, false,
                 GetA(),GetB(),GetC(),false,true,false, resp,dCRtemp,respEM );
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

    //Turn f_CH and f_EM values into ratios
    for (int i=0; i!=f_CH.size(); ++i) {
      f_05[i]*=1.0/(jets_r[i].E());
      f_CH[i]*=1.0/(jets_r[i].E());
      f_EM[i]*=1.0/(jets_r[i].E());
    }

    /******************* RECONSTRUCT PARTICLES NOT IN JETS *******************/
    #ifdef NIJ

    if (Getverbose()) cout<<"Reconstructing particles not in jets"<<endl;

    //Reconstruct prtcls in nij vecs if any saved in tuple and flag true 
    if (GetrecoMissing() && prtclnij_pt->size()!=0) {

      for (int i=0; i!=prtclnij_pt->size(); ++i) {

        PDG = abs((*prtclnij_pdgid)[i]);
        p4.SetPtEtaPhiE((*prtclnij_pt)[i], (*prtclnij_eta)[i],
                        (*prtclnij_phi)[i],(*prtclnij_e)[i]  );

        //Reconstruction
        Response(PDG,p4.Eta(),p4.E(),p4.Pt(),fr_e,fr_mu,fr_gam,fr_h,true,
                 GetA(),GetB(),GetC(),true,true,true,resp,resp_f,respEM );

        //Check for contributions to gamma+jet tag EM-cluster
        if (studyMode==2 && p4.DeltaR(tag)<R_EMc) {
          NIJ_r+=p4*respEM; //Reco as EM
          NIJ_f+=p4*respEM;
        } else {
          NIJ_r+=p4*resp;
          NIJ_f+=p4*resp_f;
        }

      } //Loop particles not in jets

    } else {	//Reco from jet inbalance 

    #endif

	//WIP, to be taken into account the EM tag & moved below for simplicity
      for (int i=0; i!=jets_r.size(); ++i) {
        NIJ_g += jets_g[i];  NIJ_r += jets_r[i];  NIJ_f += jets_f[i];
      }
      NIJ_r *= -1.0;         NIJ_f *= -1.0;

    #ifdef NIJ
    } //Reco from jet inbalance
    #endif

    /************************* GAMMA+JET: FIND PROBE *************************/

    if (studyMode == 2) {

      if (Getverbose()) cout << "Entering GAMMA+JET: FIND PROBE" << endl;

      switch (jets_r.size()) {
        case 0 : continue;      //No jets
        case 1 :		//Take the only jet as probe if not tag
          if (tag.DeltaR(jets_r[0]) < 0.2) continue;
          i_probe = 0;
          break;
        default :		//One excess high-pT jet OK if confused w/ tag
          if (jets_r.size()>2 && jets_r[2].Pt()>pTmin_probe) continue;
          if (tag.DeltaR(jets_r[0]) < 0.2) {
            i_probe = 1;
            i_tag   = 0;	//In MPF, EM-cluster subtracted from this jet
            tagIsJet = true;
          } else if (tag.DeltaR(jets_r[1]) < 0.2) {
            i_probe = 0;
            i_tag   = 1;	//In MPF, EM-cluster subtracted from this jet
            tagIsJet = true;
          } else if (jets_r[1].Pt() < pTmin_probe) i_probe = 0;
          else continue;
      }

      p4.SetPtEtaPhiE(0,0,0,0);	//Reinit
      //Set probe 4-vectors. EM reco'd probe is useless in gamma+jet
      //Gen lvl as output by FastJet
      probe.SetPtEtaPhiE((*jet_pt)[i_probe],  (*jet_eta)[i_probe],
			 (*jet_phi)[i_probe], (*jet_e)[i_probe]  );
      //Gen lvl for particles that reach D0 detector volume
      p4g_probe.SetPtEtaPhiE(jets_g[i_probe].Pt(),  jets_g[i_probe].Eta(),
			     jets_g[i_probe].Phi(), jets_g[i_probe].E()  );
      //MC SPR reco
      p4r_probe.SetPtEtaPhiE(jets_r[i_probe].Pt(),  jets_r[i_probe].Eta(),
			     jets_r[i_probe].Phi(), jets_r[i_probe].E()  );
      //Data reco
      p4f_probe.SetPtEtaPhiE(jets_f[i_probe].Pt(),  jets_f[i_probe].Eta(),
			     jets_f[i_probe].Phi(), jets_f[i_probe].E()  );

      //Assertions:
      //-sufficiently little jet E left in coarse HCAL
      if (f_CH[i_probe] > f_CHjetMax) continue;
      //-probe E left in EMCAL is in the correct energy window
      if (f_EM[i_probe] < f_EMjetMin || f_EM[i_probe] > f_EMjetMax) continue;
      //-half (10%) of probe E within R<0.5 of the jet axis for hard (soft) jets
      if ((p4r_probe.Pt()<softPt && f_05[i_probe]<f_05jetMinS) ||
          f_05[i_probe] < f_05jetMin                             ) continue;
      //-tag and probe in the right |eta| region with enough p_T
      if (fabs(p4EM_tag.Eta())  > eta_gamma   ||
          p4EM_tag.Pt()         < pTmin_gamma ||
          fabs(p4r_probe.Eta()) > eta_probe   ||
          p4r_probe.Pt()        < pTmin_probe   ) continue;

    }

    /************************* Z+JET: FIND PROBE *************************/

    if (studyMode == 3) {

      if (Getverbose()) cout << "Entering Z+JET: FIND PROBE" << endl;

      switch (jets_r.size()) {
        case 0 : continue;      //No jets
        case 1 :		//Take the only jet as a probe 
          i_probe = 0;
          break;
        default :		//One excess high-pT jet OK if confused w/ tag

          //if more than two high energy jets
          //if (jets_r.size()>2 && jets_r[2].Pt()>pTmin_probe) continue;
          i_probe = 0;
      }

      p4.SetPtEtaPhiE(0,0,0,0);	//Reinit

      //Set probe 4-vectors.
      //Gen lvl as output by FastJet
      probe.SetPtEtaPhiE((*jet_pt)[i_probe],  (*jet_eta)[i_probe],
			 (*jet_phi)[i_probe], (*jet_e)[i_probe]  );
      //Gen lvl for particles that reach D0 detector volume
      p4g_probe.SetPtEtaPhiE(jets_g[i_probe].Pt(),  jets_g[i_probe].Eta(),
			     jets_g[i_probe].Phi(), jets_g[i_probe].E()  );
      //MC SPR reco
      p4r_probe.SetPtEtaPhiE(jets_r[i_probe].Pt(),  jets_r[i_probe].Eta(),
			     jets_r[i_probe].Phi(), jets_r[i_probe].E()  );
      //Data reco
      p4f_probe.SetPtEtaPhiE(jets_f[i_probe].Pt(),  jets_f[i_probe].Eta(),
			     jets_f[i_probe].Phi(), jets_f[i_probe].E()  );

      //Assertions:
      //-sufficiently little jet E left in coarse HCAL
      if (f_CH[i_probe] > f_CHjetMax) continue;
      //-probe E left in EMCAL is in the correct energy window
      if (f_EM[i_probe] < f_EMjetMin || f_EM[i_probe] > f_EMjetMax) continue;
      //-half (10%) of probe E within R<0.5 of the jet axis for hard (soft) jets
      if ((p4r_probe.Pt()<softPt && f_05[i_probe]<f_05jetMinS) ||
          f_05[i_probe] < f_05jetMin                             ) continue;

      //-tag and probe in the right |eta| region with enough p_T
      if (fabs(p4r_tag.Eta())  > eta_muon       ||
          p4r_tag.Pt()         < pTmin_muon_tag ||
          fabs(p4r_probe.Eta()) > eta_probe     ||
          p4r_probe.Pt()        < pTmin_probe   ) continue;

    }// Z+JET: FIND PROBE


    /****************************** COMMON CUTS FOR Z+JET ******************************/
    if (Getverbose()) cout << "Entering COMMON CUTS" << endl;
    
    //Tag object and probe jet back-to-back. Note ROOT DeltaPhi is in [-pi,pi]

    if (fabs(tag.DeltaPhi(probe)) < phiMin) continue;

    //Assert probe is not confused with tag

    // How can this be true if already required back to back
    // if (fabs(tag.DeltaR(probe)) < 0.2) continue;

    //Assert sufficiently low MET w.r.t. tag pT and leading jet (we use probe)
    if      (p4r_tag.Pt() > 50 && met > 0.9*p4r_tag.Pt()) continue;
    else if (p4r_tag.Pt() > 25 && met > 1.1*p4r_tag.Pt()) continue;
    else if (p4r_tag.Pt() > 15 && met > 1.2*p4r_tag.Pt()) continue;
    else if (                     met > 2.0*p4r_tag.Pt()) continue;
    if      (met/p4r_probe.Pt()       > 0.7             ) continue;

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
          Response(PDG, p4.Eta(), p4.E(), p4.Pt(),fr_e,fr_mu,fr_gam,dAR, false,
                   GetA(),GetB(),GetC(),false,true,false,resp,dARtemp,respEM  );
          Response(PDG, p4.Eta(), p4.E(), p4.Pt(),fr_e,fr_mu,fr_gam,dBR, false,
                   GetA(),GetB(),GetC(),false,true,false,resp,dBRtemp,respEM  );
          Response(PDG, p4.Eta(), p4.E(), p4.Pt(),fr_e,fr_mu,fr_gam,dCR, false,
                   GetA(),GetB(),GetC(),false,true,false,resp,dCRtemp,respEM  );
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

        Response(PDG,p4.Eta(),p4.E(),p4.Pt(),fr_e,fr_mu,fr_gam,fr_h,true,
                 GetA(), GetB(), GetC(),true,true,true,resp,resp_f,respEM);

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
                h_other[0]->Fill((*jet_e)[(*prtcl_jet)[i_probe]],
                                 (*prtcl_e)[i]                  );
                h_other[1]->Fill((*jet_e)[(*prtcl_jet)[i_probe]],
                                 (resp*p4).E()               );
                h_other[2]->Fill((*jet_e)[(*prtcl_jet)[i_probe]],
                                 (resp_f*p4).E());
              } //Xi, Sigma, Omega^-
              //List further unknown particle PDGIDs
              if (find(otherIDs.begin(),otherIDs.end(),PDG)==otherIDs.end() &&
                  !GetStrangeB() && !isNeutrino(PDG)  ) otherIDs.push_back(PDG);
          } //Switch PDG (general)
          h_ptr[0]->Fill((*jet_e)[(*prtcl_jet)[i_probe]],   (*prtcl_e)[i] );
          h_ptr[1]->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(resp*p4).E() );
          h_ptr[2]->Fill((*jet_e)[(*prtcl_jet)[i_probe]],
                         (resp_f*p4).E() );
          h_all[0]->Fill((*jet_e)[(*prtcl_jet)[i_probe]],   (*prtcl_e)[i] );
          h_all[1]->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(resp*p4).E() );
          h_all[2]->Fill((*jet_e)[(*prtcl_jet)[i_probe]],
                         (resp_f*p4).E() );
        } //Particle content histograms
	
      } //If ptcl in the studied probe or tag

    } //Loop over particles


    /**************************** FILL HISTOGRAMS ****************************/
    //Jet energy estimator: E' = p_T_tag cosh(eta_probe) and pT'.
    //- Jet direction change in reconstruction is usually negligible, though.

    // Modified for Z+jet usage, most of the p4EM_tag changed to p4r_tag
    Ep   = p4r_tag.Pt()*cosh(probe.Eta());
    pTp  = p4r_tag.Pt();

    //Data<->MC & MC'<->MC factors
    F    = Fnum[i_probe]/Fden[i_probe];


    //Add the new values to TProfile histograms:
    //p_T balance method
    prEp->Fill(    Ep,            p4r_probe.Pt()/p4r_tag.Pt(), weight);
    prE->Fill(     p4g_probe.E(), p4r_probe.Pt()/p4r_tag.Pt(), weight);
    prEpFit->Fill( Ep,            p4f_probe.Pt()/p4r_tag.Pt(), weight);

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
    MET_r = -1.0*NIJ_r;
    MET_f = -1.0*NIJ_f;
    for (int i=0; i!=jets_r.size(); ++i) {
      if (studyMode==1 && i!=i_tag) {
        MET_r -= jets_r[i];
        MET_f -= jets_f[i];
      }
    }
    R_MPF_r = 1.0 + MET_r.Pt()*cos(p4r_tag.DeltaPhi(MET_r))/p4r_tag.Pt();
    R_MPF_f = 1.0 + MET_f.Pt()*cos(p4r_tag.DeltaPhi(MET_f))/p4r_tag.Pt();

    //Fill MPF histograms
    R_MPF_D0= -p4r_probe.Pt()*cos(p4r_tag.DeltaPhi(p4r_probe))/p4r_tag.Pt();
    prMPF_D0->Fill(   Ep, (!isnan(R_MPF_D0) ? R_MPF_D0 : 0), weight);
    prMPF_Ep->Fill(   Ep, (!isnan(R_MPF_r)  ? R_MPF_r  : 0), weight);
    prMPF_EpFit->Fill(Ep, (!isnan(R_MPF_f)  ? R_MPF_f  : 0), weight);

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
        EpP->Fill(     Ep,             p4r_probe.Pt(),                weight);
        EpPptr->Fill(  Ep,             p4r_probe.Pt(),                weight);
        EpE->Fill(     p4r_probe.E(),  Ep/p4r_probe.E(),              weight);
        EpEptr->Fill(  p4r_probe.E(),  Ep/p4r_probe.E(),              weight);
        Fptr_p->Fill(  pTp,            F,                             weight);
        Fjet_p->Fill(  pTp,            F,                             weight);
        Fptr_r->Fill(  p4r_probe.Pt(), F,                             weight);
        Fjet_r->Fill(  p4r_probe.Pt(), F,                             weight);
        Fptr_g->Fill(  p4g_probe.Pt(), F,                             weight);
        Fjet_g->Fill(  p4g_probe.Pt(), F,                             weight);
        Fptr->Fill(    Ep,             F,                             weight);
        Fjet->Fill(    Ep,             F,                             weight);
        pTEp->Fill(    pTp,            p4g_probe.Pt()/pTp,            weight);
        FFa->Fill(     Ep,                                            weight);
        pRpGptr->Fill( p4g_probe.Pt(), p4r_probe.Pt(),                weight);
        pRpGa->Fill(   p4g_probe.Pt(), p4r_probe.Pt(),                weight);
        pTppRptr->Fill(  pTp,          p4r_probe.Pt(),                weight);
        pTppRa->Fill(  pTp,            p4r_probe.Pt(),                weight);
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
          FptrSU->Fill(  Ep,                                     FSU, weight);
          FjetSU->Fill(  Ep,                                     FSU, weight);
          FptrSU_p->Fill(pTp,                                    FSU, weight);
          FjetSU_p->Fill(pTp,                                    FSU, weight);
          FptrSU_r->Fill(p4r_probe.Pt(),                         FSU, weight);
          FjetSU_r->Fill(p4r_probe.Pt(),                         FSU, weight);
          FptrSU_g->Fill(p4g_probe.Pt(),                         FSU, weight);
          FjetSU_g->Fill(p4g_probe.Pt(),                         FSU, weight);
        }
        continue;	//Only one flavour may be associated with a jet
      }
    } //Loop partons

    //If the old list of cut events is not read, a new one is written
    if (!GetuseEarlierCuts()) passedCuts[jentry]=true;
  } //Loop Tree entries

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
//      canv->Modified();
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
//      canv->Modified();
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
    delete lg;  delete lgOther;
    for (int i=0; i!=3; ++i) {
      delete h_e[i];    delete h_mu[i];    delete h_gamma[i]; delete h_gpi0[i];
      delete h_K[i];    delete h_K0S[i];   delete h_K0L[i];   delete h_L[i];
      delete h_n[i];    delete h_nu[i];    delete h_pi[i];
      delete h_p[i];    delete h_other[i]; delete h_Xi0[i];   delete h_Xim[i];
      delete h_Sig0[i]; delete h_Sigm[i];  delete h_Sigp[i];
      delete h_Om[i];   delete h_all[i];
      delete hstack[i]; delete hstackOther[i];
    }
    delete canv;

  } //Particle composition histograms

  //Save CMSJES TTree
  fout->Write();
  fout->Close();

  //Free single-particle response functions from memory
  delete fr_mu;      delete fr_e;       delete fr_gam;     delete fr_h;

} //CMSJES::Loop


//-----------------------------------------------------------------------------
//A function to find an average correction factor from gen jet (as output by
//FastJet) level to MC-recontructed level.
//  N.B. This function is intended to be used on dijet samples w/o/ eta cuts
void CMSJES::FindFJtoMC() {

  Long64_t nbytes = 0, nb = 0;
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();

  //Output file
  string outname = "./output_ROOT_files/FJtoMC/";
  string printname = "FJtoMC_";
  outname = outname + printname + ".root";	//Filetype suffix
  TFile *fout = new TFile(outname.c_str(),"RECREATE");

  //Init.
  TLorentzVector p4;			//Prtcl 4-momentum temp
  TLorentzVector jet_r; 		//MC-reco'd jet
  TLorentzVector jet_g; 		//Gen jet as output by FastJet
  TLorentzVector jet_g_no_nu; 		//Gen jet w/o/ neutrinos
  TLorentzVector tag; 			//Store prompt photon in gamma+jet
  double R_MC =1.0;	//For storing responses...
  double R_FIT=1.0;	//...at different...
  double R_EM =1.0;	//...reconstruction levels
  int PDG;				//Particle PDGID temp
  double prtnPt=0;			//For finding photon w/ highest pT (g+j)
  int i_tag = -1;			//Temp for tag photon prtn index (g+j)
  double Fden=0;			//F denominator: sum_j R_i^MC   E_i^g
  double Fnum=0;			//F numerator:   sum_i R_i^data E_i^g
  
  //Response functions, see CMSJES::Loop() for more info
  TF1 *fr_h = new TF1("frh","[5]*[0]*(1-[3]*[1]*pow(x/0.75,[2]+[4]-1))",0,250);
  string Rgam = "0.25*[0]*( 1 + erf( (x + [1])/sqrt(2*[2]*[2]) ) )";
  Rgam +=               "*( 1 + erf( (x + [3])/sqrt(2*[4]*[4]) ) ) + [5]";
  TF1* fr_gam = new TF1("frg",Rgam.c_str(),0.1,251);
  string Re = "0.25*[0]*( 1 + erf( (x + [1])/sqrt(2*[2]*[2]) ) )";
  Re +=               "*( 1 + erf( (x + [3])/sqrt(2*[4]*[4]) ) )";
  TF1* fr_e  = new TF1("fre", Re.c_str(),0.1,251);
  TF1* fr_mu = new TF1("frmu","([0]+[1]*x)*TMath::Landau(x,[2],[3])",0,251);
  TF1* fr;

  //Correction factors from FastJet gen lvl to MC reco level
  //TProfile2D params: name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, opts.
  TProfile2D* FJtoMC_b  = new TProfile2D("b", "", 12,20,200, 8, 0,2.5, "");
  TProfile2D* FJtoMC_lq = new TProfile2D("lq","", 18,20,200, 25, 0,2.5, "");
  TProfile2D* FJtoMC_b_no_nu  = new TProfile2D("b_no_nu","",
                                                  12,20,200, 8, 0,2.5, "");
  TProfile2D* FJtoMC_lq_no_nu = new TProfile2D("lq_no_nu","",
                                                  18,20,200, 25, 0,2.5, "");
  FJtoMC_b->SetStats( 0);      FJtoMC_b_no_nu->SetStats( 0);
  FJtoMC_lq->SetStats(0);      FJtoMC_lq_no_nu->SetStats(0);
  axisSetupFJtoMC(FJtoMC_b,       "#font[12]{b}#font[132]{-jets, }");
  axisSetupFJtoMC(FJtoMC_b_no_nu ,"#font[12]{b}#font[132]{-jets, no} #nu,");
  axisSetupFJtoMC(FJtoMC_lq,      "#font[12]{lq}#font[132]{-jets, }");
  axisSetupFJtoMC(FJtoMC_lq_no_nu,"#font[12]{lq}#font[132]{-jets,no} #nu, ");

  //Loop Tree entries = events
  for (Long64_t jentry=0; jentry<nentries; jentry++) {

    //Print progress for long runs
    if (jentry%10000==0) cout<<"Looping event "<<jentry<<" (FJtoMC)"<<endl;

    Long64_t ientry = LoadTree(jentry);	//Load event
    if (ientry < 0) break;		//When no more events
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    //Study the leading jet (in a loop for ease of extending to more if need be)
    for (int ijet = 0; ijet != min(1,int(jet_e->size())); ++ijet) {
      //Reinit
      jet_r.SetPtEtaPhiE(0,0,0,0);        jet_g.SetPtEtaPhiE(0,0,0,0);
      jet_g_no_nu.SetPtEtaPhiE(0,0,0,0);  Fnum=0;    Fden=0;

      //Loop over particles associated with jets in the event
      for (unsigned int i = 0; i != prtcl_pdgid->size(); ++i) {
        //See if the particle belongs to this jet
        if ((*prtcl_jet)[i] == ijet) {
          PDG=abs((*prtcl_pdgid)[i]);
          p4.SetPtEtaPhiE( (*prtcl_pt)[i],  (*prtcl_eta)[i],
                           (*prtcl_phi)[i], (*prtcl_e)[i]  );
          Response(PDG,p4.Eta(),p4.E(),p4.Pt(),fr_e,fr_mu,fr_gam,fr_h,
                   true,1,0,1,true,false,false,R_MC,R_FIT,R_EM       );
          jet_r += R_MC*p4;
          if (isNeutrino(PDG)) jet_g_no_nu += p4; //gen lvl 4-mom for neutrinos
          Fnum += R_MC*p4.E(); // poista
          Fden += R_MC*p4.E();
	}
      } //Loop particles

      //Find correction factor from FastJet jets to avg. MC-reco jets.
      jet_g.SetPtEtaPhiE((*jet_pt)[0], (*jet_eta)[0],  //FJ-lvl gen jet
                         (*jet_phi)[0],(*jet_e)[0]  );
      jet_g_no_nu = jet_g - jet_g_no_nu; //gen lvl 4-mom w/o/ neutrinos

      //Loop partons to find jet flavour
      for (unsigned int j = 0; j != prtn_tag->size(); ++j) {
        //prtn_tag=0 for outgoing hard process partons
        if ((*prtn_jet)[j]==0 && (*prtn_tag)[j]==0) {
          if (abs((*prtn_pdgid)[j])<5) {		//lq-jets
            FJtoMC_lq->Fill(jet_g.Pt(),jet_g.Eta(),
                       Fnum*jet_r.Pt()/(Fden*jet_g.Pt()),             weight);
            FJtoMC_lq_no_nu->Fill(jet_g_no_nu.Pt(),jet_g_no_nu.Eta(),
                             Fnum*jet_r.Pt()/(Fden*jet_g_no_nu.Pt()), weight);
          } else if (abs((*prtn_pdgid)[j])==5) {	//b-jets
            FJtoMC_b->Fill(jet_g.Pt(),jet_g.Eta(),
                      Fnum*jet_r.Pt()/(Fden*jet_g.Pt()),              weight);
            FJtoMC_b_no_nu->Fill(jet_g_no_nu.Pt(),jet_g_no_nu.Eta(),
                            Fnum*jet_r.Pt()/(Fden*jet_g_no_nu.Pt()),  weight);
          }
        } //Find hard process partons
      } //Loop partons
    } //Loop jets
  } //Loop events

  //Plots
  TCanvas canv("","",400,400);

  FJtoMC_b->Draw("SURF 3");
  string printname_b  = "./plots/" + printname + "_b.eps";
  canv.Print(printname_b.c_str());

  FJtoMC_lq->Draw("SURF 3");
  string printname_lq = "./plots/" + printname + "_lq.eps";
  canv.Print(printname_lq.c_str());

  FJtoMC_b_no_nu->Draw("SURF 3");
  string printname_b_no_nu  = "./plots/" + printname + "_b_no_nu.eps";
  canv.Print(printname_b_no_nu.c_str());

  FJtoMC_lq_no_nu->Draw("SURF 3");
  string printname_lq_no_nu = "./plots/" + printname + "_lq_no_nu.eps";
  canv.Print(printname_lq_no_nu.c_str());

  //Free memory
  delete fr_e;  delete fr_mu;  delete fr_gam;  delete fr_h;

  //Save output ROOT file
  fout->Write();
  fout->Close();

} //FindFJtoMC
//-----------------------------------------------------------------------------
//A function to check if gen-lvl particles are good for D0 particle level,
//i.e. no neutrinos, muons or strange hadrons.
//Params:	id	Particle PDGID
//		pT	
//Returns: true if prtcl should be included in D0 prtcl lvl, false if not
bool CMSJES::fidCuts(int id, double pT) {
  int PDG = abs(id);
  if (isNeutrino(PDG)) return false;
  if (isStrangeB(PDG) && !GetStrangeB()) return false;
  //else if (GetrunCMS()) {	//Same pT threshold for all hadrons & leptons
  if (PDG==20 || PDG==22 || pT>0.3) return true;
  else return false;

  /*
  switch (PDG) {	//Run IIb: Hadron threshold pT > m_h
    case 22   :                            //Photons have no pT...
    case 20   :               return true; //...threshold
    case 130  : if (pT>0.498) return true; else return false; //K^0_L
    case 211  : if (pT>0.140) return true; else return false; //pi^+-
    case 310  : if (pT>0.498) return true; else return false; //K^0_S
    case 321  : if (pT>0.498) return true; else return false; //K^+-
    case 2112 : if (pT>0.940) return true; else return false; //n
    case 2212 : if (pT>0.938) return true; else return false; //p
    case 3112 : if (pT>1.197) return true; else return false; //Sigma^-
    case 3122 : if (pT>1.116) return true; else return false; //Lambda
    case 3212 : if (pT>1.192) return true; else return false; //Sigma^0
    case 3222 : if (pT>1.189) return true; else return false; //Sigma^+
    case 3312 : if (pT>1.322) return true; else return false; //Xi^-
    case 3322 : if (pT>1.315) return true; else return false; //Xi^0
    case 3334 : if (pT>1.672) return true; else return false; //Omega^-
    default   : if (pT>0.3  ) return true; else return false;
  }
  */
} //fidCuts
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
void CMSJES::MultiLoop(CMSJES* dj_in, CMSJES* gj_in, bool fitPars) {

  //Input filenames to read
  InputNameConstructor();

  //Instantiate new CMSJES objects to be run in parallel
  CMSJES* resp_dj;  CMSJES* resp_gj;

  bool noInputGiven = true;

  //Instantiate new CMSJES objects where needed
   if (dj_in && gj_in) {	//Use given dijet & gamma+jet CMSJES objects
    if (Getverbose()) cout<<"MultiLoop: using input CMSJES objects"<<endl;
    resp_dj = dj_in;
    resp_gj = gj_in;
    noInputGiven = false;
  } else {	//No dijet & gamma+jet objects given
    if (Getverbose()) cout<<"MultiLoop: Instantiating new CMSJES objects"<<endl;
    resp_dj = new CMSJES(0,djFile);
    resp_gj = new CMSJES(0,gjFile);
  }

  //MultiLoop may be called from FitHandle, ensure fit params. are taken from 
  //this CMSJES object and not read from the header "as is"
  if (fitPars) {
    resp_dj->SetABC(GetA(),GetB(),GetC());
    resp_gj->SetABC(GetA(),GetB(),GetC());
  }

  //Instantiate the threads
  if (Getverbose()) cout<<"Instantiating threads"<<endl; 
  TThread* t_dj;  TThread* t_gj;
  t_dj = new TThread("t_dj", loopHandle, (void*) resp_dj);
  t_gj = new TThread("t_gj", loopHandle, (void*) resp_gj);

  //Start running the threads
  t_dj->Run();	t_gj->Run();

  //Gather the threads together after execution
  t_dj->Join();	t_gj->Join();

  //Free memory
  delete t_dj;  delete t_gj;	//Threads always instantiated here

  if (noInputGiven) {delete resp_dj;  delete resp_gj;}

} //MultiLoop

//End MULTIPLE SAMPLE LOOPING FUNCTIONALITY

//-----------------------------------------------------------------------------
//Construct input filenames for functions using multiple CMSJES 
//objects.
//Properties are chosen based on the object doing this analysis, since the
//CMSJES objects instantiated in this function will have the same properties.
// => The user cannot e.g. accidentally enter a file savename that would
//    imply properties that are not really there.
//Saves the filenames into the djFile, gjFile, zjFile strings 
//belonging to this CMSJES object.

void CMSJES::InputNameConstructor() {

  //Prevent overwriting existing filenames
  if (djFile!="" || gjFile!="" || zjFile!="") return;

  //Init strings to contain parts of the resulting filenames
  string jetAlg = "";	//Jet algorithm and cone radius
  string ctauStr = "";	//Stable particle minimum avg. lifetime as lengthscale
  string num = "";	//#events in the sample to study

  //Generator and sample event content indicators
  if      (ReadName.find("P6")!=string::npos) {gjFile="P6_";  djFile="P6_"; zjFile="P6_";}
  else if (ReadName.find("P8")!=string::npos) {gjFile="P8_";  djFile="P8_"; zjFile="P8_";}
  else if (ReadName.find("H7")!=string::npos) {gjFile="H7_";  djFile="H7_"; zjFile="H7_";}
  gjFile += "gammajet_";  djFile += "dijet_"; zjFile += "Zjet_";
  //Jet algorithm
  if      (ReadName.find("D0rIIc") !=string::npos) jetAlg += "D0rIIc_"; 
  else if (ReadName.find("a-kT")   !=string::npos) jetAlg += "a-kT_";
  //Cone radius
  if      (ReadName.find("R05") !=string::npos) jetAlg += "R05_";
  else if (ReadName.find("R07") !=string::npos) jetAlg += "R07_";
  //Min. avg. lifetime (lengthscale)
  if      (ReadName.find("ct3mm")  !=string::npos) ctauStr = "ct3mm_"; 
  else if (ReadName.find("ct10mm") !=string::npos) ctauStr = "ct10mm_"; 
  else if (ReadName.find("ct25mm") !=string::npos) ctauStr = "ct25mm_";
  else if (ReadName.find("ct50mm") !=string::npos) ctauStr = "ct50mm_";
  else if (ReadName.find("ct100mm")!=string::npos) ctauStr = "ct100mm_";
  gjFile = gjFile + jetAlg + ctauStr;  djFile = djFile + jetAlg + ctauStr;
  //#events
  if      (ReadName.find("1000000")!=string::npos) num = "1000000";
  else if (ReadName.find("2000000")!=string::npos) num = "2000000";
  else if (ReadName.find("200000") !=string::npos) num = "200000";
  else if (ReadName.find("100000") !=string::npos) num = "100000";
  else if (ReadName.find("30000")  !=string::npos) num = "30000";
  else if (ReadName.find("10000")  !=string::npos) num = "10000";
  else if (ReadName.find("1000")   !=string::npos) num = "1000";
  else {
    cout << "ERROR: fitting not supported for this file" << endl;
    return;
  }
  //The resulting filenames. Suffix ".root" to be added in CMSJES constructor
  djFile += num;
  gjFile += num; 
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
  if (!GetdjFitting() && !GetgjFitting() && !GetzjFitting()) {
    cout << "ERROR: neither gamma+jet, dijet nor Z+jet fitting enabled!" << endl;
    return;
  }

  //To avoid boilerplate, store the following combinations in sampleIndices:
  //  0		if fit to EM+jet (dijet) sample only
  //  1		if fit to gamma+jet sample only
  //  2		if fit to Z+jet sample only
  //  0 and 1	if simultaneous fit to both samples
  //The vector is to be used via:
  //  for (int s: sampleIndices) {index s picks components for studied samples}
  vector<int> sampleIndices;
  if (GetdjFitting()) sampleIndices.push_back(0);
  if (GetgjFitting()) sampleIndices.push_back(1);
  if (GetzjFitting()) sampleIndices.push_back(2);

  //Construct input and output files
  InputNameConstructor();			//Input filenames to read

  string gjOut = "./output_ROOT_files/CMSJES_"+gjFile+".root";
  string djOut = "./output_ROOT_files/CMSJES_"+djFile+".root";
  string zjOut = "./output_ROOT_files/CMSJES_"+zjFile+".root";
  vector<string> outs;  outs.push_back(djOut);  outs.push_back(gjOut); 
  outs.push_back(zjOut);//Handle

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
  //[0]=dijet, [1]=gammajet, [2]=gammajet, xjdY=derivative of pTprobe/pTtag w.r.t. Y (x=d,g)
  TFile*    file[3];  TProfile* prof[3];  //Files and prEpFit profiles
  TProfile* dA[3];    TProfile* dB[3];    TProfile* dC[3];
  double fit[3];      double fitdA[3];    double fitdB[3];  double fitdC[3];

  //Get data points to vector handles, choose runIIa or IIb depending on flags
  vector<double> djD0v, gjD0v, zjCMSv, djERv2, gjERv2, zjERv2;	//Last two: errors squared
  for (int i=0; i!=nD0data; ++i) { //N.B. do not use "sampleIndices" here,...
    if (GetdjFitting()) {          //...these vectors must first be allocated...
      djD0v.push_back( djD0II[i]);  //...as such. Only later are...
      djERv2.push_back(pow(djERII[i],2)); //...they stored into...
    }                                                 //...handles allowing...
    if (GetgjFitting()) {                             //...the use of...
      gjD0v.push_back( gjD0II[i]);        //...sampleIndices
      gjERv2.push_back(pow(gjERII[i],2));
    }
  }
  
  //Data points for CMS pT balance
  for (int i=0; i!=nCMSdata; ++i) { //Allocated here
    if (GetzjFitting()) {
      zjCMSv.push_back(zjCMS[i]); //Not read yet
      zjERv2.push_back(pow(zjER[i],2)); 
    }
  }  

  //Handles to the above data and uncertainty vectors to enable sample indexing
  vector<vector<double>> D0v;   D0v.push_back( djD0v );  D0v.push_back( gjD0v );
  D0v.push_back( zjCMSv ); //Change the name later
  vector<vector<double>> ERv2;  ERv2.push_back(djERv2);  ERv2.push_back(gjERv2);
  ERv2.push_back(zjERv2); //Change the name later

  //#D.O.F. for chi^2 normalization -- non-rigorous, but D0 apparently did this
  //  #D.O.F.="Number of data points to fit to minus number of free fit parameters"
  double n_dof = (GetgjFitting() ? (double) nD0data : 0)
                +(GetdjFitting() ? (double) nD0data : 0)
                +(GetzjFitting() ? (double) nCMSdata : 0) - (double) dim
                +(fixA || fabs(LA)>0.01 ? 1:0)  //Constrained param.s are not...
                +(fixB || fabs(LB)>0.01 ? 1:0)  //...free, hence they do not...
                +(fixC || fabs(LC)>0.01 ? 1:0); //...contribute to "-dim"

  if (n_dof<1) {cout << "ERROR: #D.O.F. < 1 in FitGN" << endl;  return;}

  cout<<"Inverses of D0 data unertainties squared, weigh LSQ sum terms:"<<endl;
  for (int i=0; i!=nD0data; ++i) {  //Don't use sampleIndices for print format
    if (GetdjFitting()) printf("dj: 1/sigma^2[%d]=%-9.3f\t",i,1/ERv2[0][i]);
    if (GetgjFitting()) printf("gj: 1/sigma^2[%d]=%-9.3f",  i,1/ERv2[1][i]);
    if (GetzjFitting()) printf("zj: 1/sigma^2[%d]=%-9.3f",  i,1/ERv2[2][i]);
    printf("\n");
  }
  cout<<"^The above factors may make |gradient| large, don't panic."<<endl;

  //Instantiate and setup CMSJES objects to read in dijet, gamma+jet and Z+jet TTrees
  CMSJES* dijet = new CMSJES(0,djFile);  CMSJES* gammajet = new CMSJES(0,gjFile);
  CMSJES* zjet = new CMSJES(0,zjFile);
  dijet->SetprintProg(     false);     gammajet->SetprintProg(     false);
  dijet->SetcontHistos(    false);     gammajet->SetcontHistos(    false);
  zjet->SetprintProg(     false);
  zjet->SetcontHistos(    false);
  dijet->SetuseEarlierCuts(GetuseEarlierCuts());
  gammajet->SetuseEarlierCuts(GetuseEarlierCuts());
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
    if (!fixA) {dijet->SetA(   ABC.GetMatrixArray()[iA]);
                gammajet->SetA(ABC.GetMatrixArray()[iA]);
                zjet->SetA(ABC.GetMatrixArray()[iA]);
                dAchi2 = 2*LA*(ABC.GetMatrixArray()[iA]-1);
                H[dim*iA+iA]+=2*LA;                        }
    if (!fixB) {dijet->SetB(   ABC.GetMatrixArray()[iB]);
                gammajet->SetB(ABC.GetMatrixArray()[iB]);
                zjet->SetB(ABC.GetMatrixArray()[iB]);
                dBchi2 =  2*LB*ABC.GetMatrixArray()[iB];
                H[dim*iB+iB]+=2*LB;                        }
    if (!fixC) {dijet->SetC(   ABC.GetMatrixArray()[iC]);
                gammajet->SetC(ABC.GetMatrixArray()[iC]);
                zjet->SetC(ABC.GetMatrixArray()[iC]);
                dCchi2 = 2*LC*(ABC.GetMatrixArray()[iC]-1);
                H[dim*iC+iC]+=2*LC;                        }

    //Calculate new values
    if (GetdjFitting() && GetgjFitting()) MultiLoop(dijet, gammajet, false);
    else if (GetdjFitting()) dijet->Loop();
    else if (GetgjFitting()) gammajet->Loop();
    else zjet->Loop();
  
    //Retrieve CMSJES output
    for (int s : sampleIndices) {
      file[s] = TFile::Open(outs[s].c_str());
      file[s]->GetObject("prEpFit0",prof[s]);  file[s]->GetObject("dAf",dA[s]);
      file[s]->GetObject("dBf",dB[s]);         file[s]->GetObject("dCf",dC[s]);
    }

    //Read the first 10 bins of CMSJES output histos, compare to D0 data
    /*
    for (int s : sampleIndices) {
      for (int i=0; i!=nD0data; ++i) { //GetBinContent indices start from 1
        fit[s]=prof[s]->GetBinContent(i+1);  fitdA[s]=dA[s]->GetBinContent(i+1);
        fitdB[s]=dB[s]->GetBinContent(i+1);  fitdC[s]=dC[s]->GetBinContent(i+1);
        chi2 += pow(fit[s]-D0v[s][i],2)/ERv2[s][i]/n_dof;	//LSQ sum
        //NaN check
        if (isnan(fitdA[s]) || isnan(fitdB[s]) || isnan(fitdC[s])) {
          cout << "Nonsensical gradient at dijet data point " << i << ": "
               << fitdA[s]<<", "<<fitdB[s]<<", "<<fitdC[s]<<", skip" << endl;  
          continue;
        }
        //Gradient components
        if (!fixA) dAchi2 += 2*(fit[s]-D0v[s][i])*fitdA[s]/ERv2[s][i];
        if (!fixB) dBchi2 += 2*(fit[s]-D0v[s][i])*fitdB[s]/ERv2[s][i];
        if (!fixC) dCchi2 += 2*(fit[s]-D0v[s][i])*fitdC[s]/ERv2[s][i];
        //The relevant components of Gauss-Newton's Hessian  estimate
        if (!fixA         ) H[iA       ]+=2*fitdA[s]*fitdA[s]/ERv2[s][i];
        if (!fixA && !fixB) H[iB       ]+=2*fitdA[s]*fitdB[s]/ERv2[s][i];
        if (!fixA && !fixC) H[iC       ]+=2*fitdA[s]*fitdC[s]/ERv2[s][i];
        if (!fixB         ) H[dim*iB+iB]+=2*fitdB[s]*fitdB[s]/ERv2[s][i];
        if (!fixB && !fixC) H[dim*iB+iC]+=2*fitdB[s]*fitdC[s]/ERv2[s][i];
        if (!fixC         ) H[dim*iC+iC]+=2*fitdC[s]*fitdC[s]/ERv2[s][i];
      }
    }
    */

    for (int s : sampleIndices) {
      for (int i=0; i!=nCMSdata; ++i) { //GetBinContent indices start from 1
        fit[s]=prof[s]->GetBinContent(i+1);  fitdA[s]=dA[s]->GetBinContent(i+1);
        fitdB[s]=dB[s]->GetBinContent(i+1);  fitdC[s]=dC[s]->GetBinContent(i+1);
        chi2 += pow(fit[s]-D0v[s][i],2)/ERv2[s][i]/n_dof;	//LSQ sum
        //NaN check
        if (isnan(fitdA[s]) || isnan(fitdB[s]) || isnan(fitdC[s])) {
          cout << "Nonsensical gradient at dijet data point " << i << ": "
               << fitdA[s]<<", "<<fitdB[s]<<", "<<fitdC[s]<<", skip" << endl;  
          continue;
        }
        //Gradient components
        if (!fixA) dAchi2 += 2*(fit[s]-D0v[s][i])*fitdA[s]/ERv2[s][i];
        if (!fixB) dBchi2 += 2*(fit[s]-D0v[s][i])*fitdB[s]/ERv2[s][i];
        if (!fixC) dCchi2 += 2*(fit[s]-D0v[s][i])*fitdC[s]/ERv2[s][i];
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
    dijet->SetuseEarlierCuts(true);
    gammajet->SetuseEarlierCuts(true);
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
  delete dijet;  delete gammajet; delete zjet;

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
  Setverbose(true);
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
  if      (gjFile.find("P6")!=string::npos) output << "P6";
  else if (gjFile.find("P8")!=string::npos) output << "P8";
  else if (gjFile.find("H7")!=string::npos) output << "H7";
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
void CMSJES::Plot2D()
{
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  //Output file
  string outname = "plot2D_" + ReadName;
  TFile *fout = new TFile(outname.c_str(),"RECREATE");
  
  //2D Histogram
  //"name", "title",nbinsx, xlow, xup, nbinsy, ylow, yup
  TH2* h2 = new TH2D("h2", "", 100, -5.0, 3.0, 100, -TMath::Pi(), TMath::Pi());

  //Initialization
  TLorentzVector p4; //4-momentum
  Long64_t nbytes = 0, nb = 0;
  //Loop Tree entries = events
  //Plot only the first event for now
  for (Long64_t jentry=0; jentry</*nentries*/1;jentry++) {
    Long64_t ientry = LoadTree(jentry);	//Load new event
    if (ientry < 0) break;		//When no more events
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;	//Cut is currently dummy in header
    //Loop jets within this event:
    for (int ijet = 0; ijet != int(jet_e->size()); ++ijet) {
      p4.SetPtEtaPhiE(0,0,0,0);
      //Loop particles in jets, see if they belong to this jet
      for (unsigned int i = 0; i != prtcl_pdgid->size(); ++i) {
        if ((*prtcl_jet)[i]==ijet) {
	  p4.SetPtEtaPhiE((*prtcl_pt)[i],(*prtcl_eta)[i],
		          (*prtcl_phi)[i],(*prtcl_e)[i]);
	}
      }
      //Add the new value to 2D histogram:
      //x value, y value, weight
      h2->Fill(p4.Eta(),p4.Phi(),p4.E());
    }
  }
  h2->GetXaxis()->SetTitle("#font[12]{#eta}");
  h2->GetYaxis()->SetTitle("#font[12]{#phi}");

  fout->Write();	//Save output
  fout->Close();	//Close file
} //Plot2D

//-----------------------------------------------------------------------------
//A function to combine several response function plots stored as TProfiles
//in "CMSJES_X.root" files.
//MConly and fitOnly are Master flags, overriding all others if true
void CMSJES::plotPT(int gen, int Nevt, bool MConly, bool fitOnly)
{
  bool plotOurMC  = true;
  bool plotD0MC   = false; //true;
  bool plotD0data = false; //true;
  bool plotCMSMC   = true;
  bool plotCMSdata = true;
  bool plotFit    = true;

  /*
  plotOurMC =(MConly ?true :plotOurMC );  plotD0MC=(MConly ?true :plotD0MC);
  plotD0data=(MConly ?false:plotD0data);  plotFit =(MConly ?false:plotFit );
  plotCMSdata=(MConly ?false:plotCMSdata);plotCMSMC=(MConly ?true :plotCMSMC);
  plotOurMC =(fitOnly?false:plotOurMC );  plotD0MC=(fitOnly?false:plotD0MC);
  plotD0data=(fitOnly?true :plotD0data);  plotFit =(fitOnly?true :plotFit );
  */

  //Choose filenames to open
  string nameAdd, dijetFile, gammajetFile, zjetFile;
  plotQuery(nameAdd, dijetFile, gammajetFile, zjetFile, gen, Nevt);

  //Initialize histograms and open ROOT files and fetch the stored objects
  TFile* fzj = TFile::Open(zjetFile.c_str()); // Z+jet file

  TProfile* przj=0;      TProfile* przj_f=0;

  /* Z+jet */
  fzj->GetObject("prEp",przj);		//Z+jet response
  fzj->GetObject("prEpFit0",przj_f);	//Z+jet fit to data
  TH1D* hzj   = przj->ProjectionX();
  TH1D* hzj_f = przj_f->ProjectionX();

  //D0 data points for gamma
  TGraphErrors* dgj = new TGraphErrors();	//gamma+jet data
  TGraph* mc_gj = new TGraph();			//gamma+jet MC

  //CMS data points for Z+jet
  TGraphErrors* dzj = new TGraphErrors();	//Z+jet data
  TGraph* mc_zj = new TGraph();			//Z+jet MC

  //Filled circle (4 hollow circle); 1 black, 2 red, 4 blue, 3/8 green
  dgj->SetMarkerStyle(8);    dgj->SetMarkerColor(  kGreen+2);
  mc_gj->SetMarkerStyle(4);  mc_gj->SetMarkerColor(kGreen+2);
  dzj->SetMarkerStyle(8);    dzj->SetMarkerColor(  kGreen+2);
  mc_zj->SetMarkerStyle(4);  mc_zj->SetMarkerColor(kGreen+2);

  hzj->SetLineColor(kGreen+2);
  hzj_f->SetMarkerStyle(kFullDiamond);  
  hzj_f->SetMarkerColor(kGreen-6);



  //D0 pT-balance data points and errors
  for (int i=0; i!=nD0data; ++i) {	//D0 pT-bal data
    dgj->SetPoint(i, gjEpII[i], gjD0II[i]);
    dgj->SetPointError(i, 0, gjERII[i]);
  }
  for (int i=0; i!=nD0MC; ++i) {	//D0 pT-bal. MC
    mc_gj->SetPoint(i, gjMCEpII[i], gjD0MCII[i]);  
  }

  //CMS pT-balance data points and errors
  for (int i=0; i!=nCMSdata; ++i) {	//CMS pT-bal data
    dzj->SetPoint(i, zjEp[i], zjCMS[i]);
    dzj->SetPointError(i, 0, zjER[i]);
  }
  for (int i=0; i!=nCMSMC; ++i) {	//CMS pT-bal. MC
    mc_zj->SetPoint(i, zjMCEp[i], zjCMSMC[i]); //Can add errors since those exist
  }

  //Savefile name setup
  string savename = "P8_Zjet";

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
  hzj->GetXaxis()->SetRangeUser(10,1000);

  //Suppress stat boxes
  hzj->SetStats(0);

  //Axis setup. New dummy TH1 for easy usage in multiple plots
  TH1D* setup = new TH1D("setup",""/*hdj->GetTitle()*/, hzj->GetXaxis()->GetNbins(),
			 hzj->GetXaxis()->GetXmin(),    hzj->GetXaxis()->GetXmax());
  setup->SetStats(0);				//Suppress stat box
  setup->GetXaxis()->SetTitle(hzj->GetXaxis()->GetTitle());
  setup->GetYaxis()->SetTitle(hzj->GetYaxis()->GetTitle());
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
    if (!plotD0MC  ) vertical[1] -= 0.06;  //Resize legends so that...
    if (!plotCMSMC  ) vertical[1] -= 0.06;
    if (!plotOurMC ) vertical[1] -= 0.06;  //...they appear properly...
    if (!plotD0data) vertical[0] += 0.06;  //...with all combinations ...
    if (!plotCMSdata) vertical[0] += 0.06;
    if (!plotFit   ) vertical[0] += 0.06;  //...of plot flags
  }
  TLegend* lz = new TLegend(lz_horiz[0],vertical[0],lz_horiz[1],vertical[1]);
  lz->SetBorderSize(0);	//No box around legend
  lz->SetFillStyle( 0);	//No background fill

  if (plotD0MC  )  lz->AddEntry(mc_gj,"#font[132]{D#oslash #gamma+jet MC}", "p");
  if (plotCMSMC  ) lz->AddEntry(mc_zj,"#font[132]{CMS Z+jet MC}", "p");
  if (plotOurMC )  lz->AddEntry(hzj,  "#font[132]{Our Z+jet MC}", "l");
  if (plotD0data)  lz->AddEntry(dgj,  "#font[132]{D#oslash #gamma+jet data}","p");
  if (plotCMSdata) lz->AddEntry(dzj,  "#font[132]{CMS Z+jet data}","p");
  if (plotFit   )  lz->AddEntry(hzj_f,"#font[132]{Our Z+jet data fit}","p");


  //Main plot
  setup->Draw();
  if (plotOurMC )  {hzj->Draw(  "SAME"       );}
  if (plotD0data)  {dgj->Draw(  "P SAME"     );}
  if (plotCMSdata) {dzj->Draw(  "P SAME"     );}
  if (plotFit   )  {hzj_f->Draw("HIST P SAME");}
  if (plotD0MC  )  {mc_gj->Draw("P SAME"     );}
  if (plotCMSMC  ) {mc_zj->Draw("P SAME"     );}
  lz->Draw();	//Legends



  //Save plot
  string saveTo = "./plots/pT-bal/";
  saveTo = saveTo + (MConly?"MC_":"") + (fitOnly?"data_fit_":"") + savename;

  canv->Print(saveTo.c_str());

  //Free memory
  delete setup;  delete lz;   delete pad1;   delete canv;
  delete dgj;  delete mc_gj;  delete dzj;  delete mc_zj;

} //plotPT
//-----------------------------------------------------------------------------
//A handle for drawing "MC only" and "Fit+data only" pT-bal. plots at once
void CMSJES::plotSepPT() {
  int gen=0, Nevt=0;
  string nameDum, djdummy, gjdummy, zjdummy;
  plotQuery(nameDum, djdummy, gjdummy, zjdummy, gen, Nevt);
  plotPT(gen, Nevt, true,  false);
  plotPT(gen, Nevt, false, true );
}
//-----------------------------------------------------------------------------
//Like plotPT, but for MPF histograms
void CMSJES::plotMPF(int gen,  int alg, int rad, int ct,
                    int Nevt)
{
  //Choose filenames to open
  string nameAdd, dijetFile, gammajetFile, zjetFile;
  plotQuery(nameAdd, dijetFile, gammajetFile, zjetFile, gen, Nevt);

  //Initialize histograms and open ROOT files and fetch the stored objects
  //  TH1 params: name, title, #bins, #lowlimit, #highlimit
  TFile* fdj = TFile::Open(dijetFile.c_str());
  TFile* fgj = TFile::Open(gammajetFile.c_str());
  TProfile *prgj_MPF_D0=0, *prgj_MPF=0, *prgj_MPF_f=0;  //gamma+jet (_f = fit)
  TProfile *prdj_MPF_D0=0, *prdj_MPF=0, *prdj_MPF_f=0;  //EM+jet

  /* 1: dijet */
  fdj->GetObject("prMPF_D0",prdj_MPF_D0);
  fdj->GetObject("prMPF_Ep",prdj_MPF);
  fdj->GetObject("prMPF_EpFit",prdj_MPF_f);
  TH1D* hdj_MPF_D0 = prdj_MPF_D0->ProjectionX();
  TH1D* hdj_MPF    = prdj_MPF->ProjectionX();
  TH1D* hdj_MPF_f  = prdj_MPF_f->ProjectionX();

  /* 2: gamma+jet */
  fgj->GetObject("prMPF_D0",prgj_MPF_D0);
  fgj->GetObject("prMPF_Ep",prgj_MPF);
  fgj->GetObject("prMPF_EpFit",prgj_MPF_f);
  TH1D* hgj_MPF_D0 = prgj_MPF_D0->ProjectionX();
  TH1D* hgj_MPF    = prgj_MPF->ProjectionX();
  TH1D* hgj_MPF_f  = prgj_MPF_f->ProjectionX();

  //D0 data and MC points
  TGraph* mc_dj_MPF=new TGraph(); TGraph* d_dj_MPF=new TGraph(); //EM+jet MPF
  TGraph* mc_gj_MPF=new TGraph(); TGraph* d_gj_MPF=new TGraph(); //gamma+jet MPF
  TGraph* mc_gj_MPF_R07=new TGraph();
  TGraph* d_gj_MPF_R07=new TGraph(); //gamma+jet MPF (R_cone=0.7)

/* Can be uncommented when we have both IIa and IIb MPF points
  ans ="";	//Reinit
  cout << "Choose 1: use runIIa D0 data/MC reference points" << endl;
  cout << "       2: use runIIb1 D0 data/MC reference points"<< endl;
  while (ans!="1" && ans!="2")cin >> ans;
  if (ans != "1") {

  } else {

  }
*/
  //A will-do-for-now alternative to the code commented out immediately above
  //D0 gamma+jet MPF MC simulation points
  for (int i=0; i!=nD0MC_MPF_R07; ++i) {	//R_cone=0.7
    mc_gj_MPF_R07->SetPoint(i, gjMCEp_MPF_R07[i], gjD0MC_MPF_R07[i]);  
  }
  for (int i=0; i!=nD0MC_MPF; ++i) {
    mc_gj_MPF->SetPoint(i, gjMCEp_MPF[i], gjD0MC_MPF[i]);  
    mc_dj_MPF->SetPoint(i, djMCEp_MPF[i], djD0MC_MPF[i]);  
  }
  //D0 gamma+jet MPF data points
  for (int i=0; i!=nD0_MPF_R07; ++i) {		//R_cone=0.7
    d_gj_MPF_R07->SetPoint(i,gjEp_MPF_R07[i],gjD0_MPF_R07[i]);
  }
/*	Can be uncommented if we get D0 MPF detector data points
  for (int i=0; i!=nD0_MPF; ++i) {
    d_gj_MPF->SetPoint(i,gjEp_MPF[i],gjD0_MPF[i]);
    d_dj_MPF->SetPoint(i,djEp_MPF[i],djD0_MPF[i]);
  }
*/

  //Canvas
  TCanvas* canv_MPF = new TCanvas("MPF","",500,500);
  canv_MPF->SetLeftMargin(0.12);	//To fit vertical axis labels

  //Style setup
  hdj_MPF->SetLineColor(     kBlack);       hgj_MPF->SetLineColor(    kGreen+2);
  hdj_MPF_D0->SetLineColor(  kGray);        hgj_MPF_D0->SetLineColor( kAzure+7);
  mc_dj_MPF->SetMarkerStyle( kOpenCircle);  mc_dj_MPF->SetMarkerColor(kBlack  );
  mc_gj_MPF->SetMarkerStyle( kOpenCircle);  mc_gj_MPF->SetMarkerColor(kGreen+2);
  prdj_MPF_f->SetMarkerStyle(kFullDiamond);
  prdj_MPF_f->SetMarkerColor(kGray+1);
  hgj_MPF_f->SetMarkerStyle(kFullDiamond);  hgj_MPF_f->SetMarkerColor(kGreen-6);
  d_dj_MPF->SetMarkerStyle(kFullCircle);    d_dj_MPF->SetMarkerColor(kBlack);
  mc_gj_MPF_R07->SetMarkerStyle(kOpenCircle);
  mc_gj_MPF_R07->SetMarkerColor(kAzure+7);
  d_gj_MPF_R07->SetMarkerStyle(kFullCircle);
  d_gj_MPF_R07->SetMarkerColor(kAzure+7);

  //Legend
  TLegend* ld_MPF = new TLegend(0.15,0.70,0.52,0.89);
  TLegend* lg_MPF = new TLegend(0.52,0.70,0.89,0.89);
  ld_MPF->SetBorderSize(0);   lg_MPF->SetBorderSize(0);
  lg_MPF->AddEntry(hgj_MPF,   "#font[132]{Our #gamma+jet MPF MC}",     "l");
  lg_MPF->AddEntry(hgj_MPF_D0,
                   "#font[132]{Our D#oslash style #gamma+jet MPF MC}", "l");
  lg_MPF->AddEntry(hgj_MPF_f, "#font[132]{Our #gamma+jet MPF MC fit}", "p");
  lg_MPF->AddEntry(d_gj_MPF_R07,
                   "#font[132]{D#oslash #gamma+jet MPF data R=0.7}",   "p");
  lg_MPF->AddEntry(mc_gj_MPF_R07,
                   "#font[132]{D#oslash #gamma+jet MPF MC R=0.7}",     "p");
  ld_MPF->AddEntry(hdj_MPF,   "#font[132]{Our EM+jet MPF MC}",         "l");
  ld_MPF->AddEntry(hdj_MPF_D0,
                   "#font[132]{Our D#oslash style EM+jet MPF MC}",     "l");
  ld_MPF->AddEntry(hdj_MPF_f,"#font[132]{Our EM+jet MPF MC fit}",      "p");
  ld_MPF->AddEntry(d_dj_MPF, "#font[132]{D#oslash EM+jet MPF data}",   "p");
  ld_MPF->AddEntry(mc_dj_MPF,"#font[132]{D#oslash EM+jet MPF MC}",     "p");

  //Title and axis setup
  hgj_MPF->SetStats(0);  hdj_MPF->SetStats(0);  //Suppress stat boxes
  hgj_MPF->SetAxisRange(0.6,1.1,"Y");		//Vertical axis limits
  hgj_MPF->GetYaxis()->SetTitleFont(133);
  int titleSize = 18;				//Common title size everywhere
  hgj_MPF->GetYaxis()->SetTitleSize(titleSize);
  hgj_MPF->GetXaxis()->SetMoreLogLabels();
  hgj_MPF->GetXaxis()->SetNoExponent();
  hgj_MPF->GetXaxis()->SetTitleFont(133);
  hgj_MPF->GetXaxis()->SetTitleSize(titleSize);
  canv_MPF->SetLogx();
  hgj_MPF->GetYaxis()->SetTitle("#font[12]{R}_{MPF}^{#gamma+jet}");
  hgj_MPF->GetXaxis()->SetTitleOffset(1);

  //Savefile name setup
  string savename = "./plots/MPF_R";
  string MPFtitle = hgj_MPF->GetTitle();
  cout << MPFtitle << endl;
  if      (MPFtitle.find("=0.7")!=string::npos) savename+="07";
  else if (MPFtitle.find("=0.5")!=string::npos) savename+="05";
  else savename += "_NA";	//R not available
  if      (MPFtitle.find("D#oslash cone")!=string::npos) savename += "_D0rIIc";
  else if (MPFtitle.find("_a-kT"        )!=string::npos) savename += "_a-kT";
  if      (MPFtitle.find("ZS"      )!=string::npos) savename+="_ZS";
  if      (MPFtitle.find("0.3 cm")!=string::npos) savename+="_ct3mm";
  else if (MPFtitle.find("1 cm")  !=string::npos) savename+="_ct10mm";
  else if (MPFtitle.find("2.5 cm")!=string::npos) savename+="_ct25mm";
  else if (MPFtitle.find("5 cm")  !=string::npos) savename+="_ct50mm";
  else if (MPFtitle.find("10 cm") !=string::npos) savename+="_ct100mm";
  savename+=".eps";

  //Plot
  hgj_MPF->Draw();
  hdj_MPF->Draw("SAME");
  hgj_MPF_D0->Draw("SAME");
  hdj_MPF_D0->Draw("SAME");
  hgj_MPF_f->Draw("HISTO P SAME");
  hdj_MPF_f->Draw("HISTO P SAME");
  mc_gj_MPF_R07->Draw("P SAME");
  d_gj_MPF_R07->Draw("P SAME");
  mc_dj_MPF->Draw("P SAME");
  mc_gj_MPF->Draw("P SAME");
  //d_dj_MPF->Draw("P SAME");	//To be uncommented when we have D0 MPF data
  //d_gj_MPF->Draw("P SAME");	//To be uncommented when we have D0 MPF data
  lg_MPF->Draw();
  ld_MPF->Draw();

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
/* A handle to set up TProfile2D object axis in FindFJtoMC */
void CMSJES::axisSetupFJtoMC(TProfile2D* FJtoMC, string titleAdd) {
  string title = titleAdd + " #font[132]{MC vs gen, ";
  title += "}";
  FJtoMC->SetTitle(title.c_str());
  FJtoMC->GetXaxis()->SetTitle("#font[132]{#font[12]{p}_{T,jet}^{FJ}}");
  FJtoMC->GetYaxis()->SetTitle("#font[132]{|#font[12]{#eta}_{jet}^{FJ}|}");
  FJtoMC->GetZaxis()->SetTitle(
          "#font[132]{#font[12]{p}_{T,MC-jet}/#font[12]{p}_{T,FJ-jet}}");
  FJtoMC->GetXaxis()->SetTitleOffset(1.8);
  FJtoMC->GetYaxis()->SetTitleOffset(2.0);
  FJtoMC->GetZaxis()->SetTitleOffset(1.45);
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
//  D0 single-particle response parameters for different particles & eta 
//  regions extracted from D0 analysis notes 6143 and 6368.
//  Note that IIb parameters are identical for all epochs b1, b2 and b3-4.
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
//              MC, FIT, EM	Flags to find MC/fit/EM-lvl response
//              ret**		References where to put the resulting response
//				N=0 using IIa or default IIb
//				N=1 using IIb*-P20ToP17
//Returns:	The calculated response (>= 0) or -1 if event invalid
void CMSJES::Response(int id, double pseudorap, double energy, double pT,
	             TF1* frE, TF1* frMU, TF1* frG, TF1* frH, bool pos,
                     double fA,double fB,double fC,bool MC,bool FIT,bool EM,
                     double& retMC, double& retFIT,
                     double& retEM                                  )
{
  //Init
  bool zero = false; //Return zero responses (run index-wise)
  bool pTm[2] = {false,true};	     //Hadron pT cut at m_h (true) or 0.3 GeV (false)
  double R_temp;
  int PDG = abs(id);

  //Check if particle outside good eta region
  if (fabs(pseudorap) > 3.2) zero = true;
  unsigned int row = int(fabs(pseudorap)*10);	//Param matrix row from |eta|

  //Assert there's no pi^0 (PDGID 111) or eta (221) after parton shower
  if (PDG==111 || PDG==221) {
    cout << "WARNING: pi^0 (111) or eta (221) found! PDGID: " << PDG
         << "Returning zero response" << endl;
    zero = true;
  }

  //Neutrino responses are zero
  if (isNeutrino(PDG)) zero = true;

  //Particle must have enough pT to reach D0 detector volume
  if (!fidCuts(PDG,pT)) zero = true;

  //CALCULATE RESPONSES
  //  Loop structure to go over IIa/IIb default & IIb-P20ToP17 params
  for (int i_r=0; i_r<(zero?0:1); ++i_r) {
    frE->SetParameters(params_e[row][0], params_e[row][1],
                       params_e[row][2], params_e[row][3],
                       params_e[row][4]                       );
    frG->SetParameters(params_gam[row][0], params_gam[row][1],
                       params_gam[row][2], params_gam[row][3],
                       params_gam[row][4], params_gam[row][5]);
    frMU->SetParameters(params_mu[row][0], params_mu[row][1],
                        params_mu[row][2], params_mu[row][3]);

    //EM-reco always using photon response
    R_temp = frG->Eval(energy);
    retEM = R_temp;
    switch (PDG) {
      case 20 : //Same for both photon IDs. No min pT or fit params A,B,C
      case 22 : retMC=R_temp;  retFIT=R_temp;	
                break;
      //LEPTONS: choose params to use. N.B. leptons have no fit params A,B,C
      case 11 : R_temp = frE->Eval(energy);
                retMC=R_temp;  retFIT=R_temp;
                break;
//***********************************************************************************************
      case 13 : R_temp = frG->Eval(energy);//frMU->Eval(energy); Change this
//***********************************************************************************************
                retMC=R_temp;  retFIT=R_temp;
                break;
      //HADRONS
      case 211 :		//pi^+-
        frH->SetParameters(params_pi[row][0],params_pi[row][1],
                           params_pi[row][2],     1,    0,    1    );
        retMC = frH->Eval(energy);
        break;
      case 321 : 		//K^+-
        frH->SetParameters(params_K[row][0], params_K[row][1],
                           params_K[row][2],       1,     0,    1 );
        retMC = frH->Eval(energy);
        break;
      case 130 :		//K^0_L
        frH->SetParameters(params_KL[row][0],params_KL[row][1],
                           params_KL[row][2],      1,     0,    1  );
        retMC = frH->Eval(energy);
        break;
      case 310 :		//K^0_S
        frH->SetParameters(params_KS[row][0],params_KS[row][1],
                           params_KS[row][2],      1,     0,    1  );
        retMC = frH->Eval(energy);
        break;
      case 3122 :		//Lambda
        frH->SetParameters(params_L[row][0],params_L[row][1],
                           params_L[row][2],     1,     0,     1 );
        retMC = frH->Eval(energy);
        break;
      case 2112 :		//n
        frH->SetParameters(params_n[row][0],params_n[row][1],
                           params_n[row][2],     1,     0,     1 );
        retMC = frH->Eval(energy);
        break;
      case 2212 :		//p
        frH->SetParameters(params_p[row][0],params_p[row][1],
                           params_p[row][2],     1,     0,    1  );
        retMC = frH->Eval(energy);
        break;
      default: // ANSÄTZE FOR XI, SIGMA, OMEGA
        //According to D0 JES, strange baryons may have had responses but such
        //were not presented. As an Ansatz, we modify the responses of known
        //hadrons by  R^MC_h(E=m_h) = 0 <=> p^(1)_h = (m_h/0.75)^(1-p^(2)_h).
        //The different Ansätze below are based on different hadrons' params.
        //To use the Ansätze, set StrangeB=true in CMSJES.h
        if (GetStrangeB() && GetAnsatz()=="pn") { //PROTON & NEUTRON params
          switch (PDG) {
            case 3112 :		//Sigma^-
              frH->SetParameters(params_p[row][0],
                                 pow((1.197/0.75),1-params_p[row][2]),
                                 params_p[row][2],   1,    0,   1    );
              retMC = frH->Eval(energy);
              break;
            case 3212 :		//Sigma^0
              frH->SetParameters(params_n[row][0],
                                 pow((1.192/0.75),1-params_n[row][2]),
                                 params_n[row][2],   1,   0,   1     );
              retMC = frH->Eval(energy);
              break;
            case 3222 :		//Sigma^+
              frH->SetParameters(params_p[row][0],
                                 pow((1.189/0.75),1-params_p[row][2]),
                                 params_p[row][2],   1,   0,   1     );
              retMC = frH->Eval(energy);
              break;
            case 3312 :		//Xi^-
              frH->SetParameters(params_p[row][0],
                                 pow((1.322/0.75),1-params_p[row][2]),
                                 params_p[row][2],   1,   0,   1     );
              retMC = frH->Eval(energy);
              break;
            case 3322 :		//Xi^0
              frH->SetParameters(params_n[row][0],
                                 pow((1.315/0.75),1-params_n[row][2]),
                                 params_n[row][2],   1,   0,   1     );
              retMC = frH->Eval(energy);
              break;
            case 3334 :		//Omega^-
              frH->SetParameters(params_p[row][0],
                                 pow((1.67245/0.75),1-params_p[row][2]),
                                 params_p[row][2],    1,    0,   1     );
              retMC = frH->Eval(energy);
             break;
            default : zero=true; continue;	 //Unknown particle
          } //Switch PDG (proton & neutron Ansatz)
        } else if (GetStrangeB() && GetAnsatz()=="L") { //LAMBDA params
          switch (PDG) {
            case 3112 :		//Sigma^-
              frH->SetParameters(params_L[row][0],
                                 pow((1.197/0.75),1-params_L[row][2]),
                                 params_L[row][2],   1,   0,   1     );
              retMC = frH->Eval(energy);
              break;
            case 3212 :		//Sigma^0
              frH->SetParameters(params_L[row][0],
                                 pow((1.192/0.75),1-params_L[row][2]),
                                 params_L[row][2],   1,   0,   1     );
              retMC = frH->Eval(energy);
              break;
            case 3222 :		//Sigma^+
              frH->SetParameters(params_L[row][0],
                                 pow((1.189/0.75),1-params_L[row][2]),
                                 params_L[row][2],   1,   0,   1     );
              retMC = frH->Eval(energy);
              break;
            case 3312 :		//Xi^-
              frH->SetParameters(params_L[row][0],
                                 pow((1.322/0.75),1-params_L[row][2]),
                                 params_L[row][2],   1,   0,   1     );
              retMC = frH->Eval(energy);
              break;
            case 3322 :		//Xi^0
              frH->SetParameters(params_L[row][0],
                                 pow((1.315/0.75),1-params_L[row][2]),
                                 params_L[row][2],   1,   0,   1     );
              retMC = frH->Eval(energy);
              break;
            case 3334 :		//Omega^-
              frH->SetParameters(params_L[row][0],
                                 pow((1.67245/0.75),1-params_L[row][2]),
                                 params_L[row][2],    1,    0,   1     );
              retMC = frH->Eval(energy);
              break;
            default : zero=true; continue;	 //Unknown particle
          } //Switch PDG (Lambda Ansatz)
        } else if (GetStrangeB() && GetAnsatz()=="pi") {	//PION params
          switch (PDG) {
            case 3112 :		//Sigma^-
              frH->SetParameters(params_pi[row][0],
                                 pow((1.197/0.75),1-params_pi[row][2]),
                                 params_pi[row][2],   1,   0,   1     );
              retMC = frH->Eval(energy);
              break;
            case 3212 :		//Sigma^0
              frH->SetParameters(params_pi[row][0],
                                 pow((1.192/0.75),1-params_pi[row][2]),
                                 params_pi[row][2],   1,   0,   1     );
              retMC = frH->Eval(energy);
              break;
            case 3222 :		//Sigma^+
              frH->SetParameters(params_pi[row][0],
                                 pow((1.189/0.75),1-params_pi[row][2]),
                                 params_pi[row][2],   1,   0,   1     );
              retMC = frH->Eval(energy);
              break;
            case 3312 :		//Xi^-
              frH->SetParameters(params_pi[row][0],
                                 pow((1.322/0.75),1-params_pi[row][2]),
                                 params_pi[row][2],   1,   0,   1     );
              retMC = frH->Eval(energy);
              break;
            case 3322 :		//Xi^0
              frH->SetParameters(params_pi[row][0],
                                 pow((1.315/0.75),1-params_pi[row][2]),
                                 params_pi[row][2],   1,   0,   1     );
              retMC = frH->Eval(energy);
              break;
            case 3334 :		//Omega^-
              frH->SetParameters(params_pi[row][0],
                                 pow((1.67245/0.75),1-params_pi[row][2]),
                                 params_pi[row][2],    1,    0,   1     );
              retMC = (MC ? frH->Eval(energy) : 0);
              break;
            default : zero=true; continue;	 //Unknown particle
          } //Switch PDG (pion Ansatz)
        } else {zero=true;  break;} //Unknown partcle, not observed
    } //Switch PDG (before Ansätze)

    if (PDG>99 && FIT && !zero) { //Had.: reset A,B,C; others same as MC
      frH->SetParameter(3,fA);
      frH->SetParameter(4,fB);
      frH->SetParameter(5,fC);
      retFIT = frH->Eval(energy);
    }

  } 

  //Set results. Check for NaN and negative results if positive demanded
  if (!MC || zero || isnan(retMC)  || (pos && retMC <0)) retMC =0;
  if (!FIT|| zero || isnan(retFIT) || (pos && retFIT<0)) retFIT=0;
  if (!EM || zero || isnan(retEM)  || (pos && retEM <0)) retEM =0;

} //Response
//-----------------------------------------------------------------------------
//For studying the general properties TTree read in by the CMSJES object.
void CMSJES::StudyTree()
{
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();

  //Initialization
  Long64_t nbytes = 0, nb = 0;
  int nptcls = 0;
  double Esum = 0;		//Sum over particles in a jet
  TLorentzVector p4;
  TLorentzVector p4sum;
  TLorentzVector jetTemp, jetSum;
  #ifdef NIJ
  TLorentzVector NIJ_g;	//Particles not in jets (gen lvl)
  #endif
  TLorentzVector allprtcls;	//Sum all particles in the event
  vector<TLorentzVector> jets_r;//MC
  double pTmax_g=0;		//Highest pT of a gamma in the event
  int i_lg=-1;			//Index of highest pT gamma
  double nu_pT = 0;

  //User interface
  cout<<"TTree has "<< nentries <<" entries, how many to print?"<<endl;
  Long64_t nToPrint;
  cin >> nToPrint;
  if (nToPrint > nentries) nToPrint = nentries;

  //Loop Tree entries = events
  for (Long64_t jentry=0; jentry<nToPrint;jentry++) {

    Long64_t ientry = LoadTree(jentry);	//Load event
    if (ientry < 0) break;		//When no more events
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    //Reinit
    jetSum.SetPtEtaPhiE(0,0,0,0);  nu_pT = 0;

    //Check #jets and #particles
    cout<<"*** Entry "<<jentry<<" has "<<int(jet_e->size())<<" jets"
	<<" and "<< prtcl_jet->size() <<" particles. "
        <<"MET:" << met << endl;

    //Check jet energies
    cout << "  Jet: p_T \t eta \t\t phi \t\t E" << endl;
    for (int i=0; i < int(jet_e->size()); ++i) {
      jetTemp.SetPtEtaPhiE((*jet_pt)[i], (*jet_eta)[i],
                           (*jet_phi)[i],(*jet_e)[i]  );
      cout << "       " << (*jet_pt)[i] << "\t" << (*jet_eta)[i]
	   << " \t "    << (*jet_phi)[i]<<" \t "<< (*jet_e)[i] << endl;
      jetSum += jetTemp;
    }
    cout << "jetSum pT: " << jetSum.Pt() << endl;
    cout << "MET (neutrinos): " << met << endl;

    //Reinit
    allprtcls.SetPtEtaPhiE(0,0,0,0);

    //Particles associated with jets
    for (unsigned int i = 0; i != prtcl_pdgid->size(); ++i) {
      p4.SetPtEtaPhiE((*prtcl_pt)[i], (*prtcl_eta)[i],
                      (*prtcl_phi)[i],(*prtcl_e)[i]  );
      allprtcls += p4;
      if (isNeutrino((*prtcl_pdgid)[i])) nu_pT += (*prtcl_pt)[i];
    }

    cout << "Particles in jets: " << allprtcls.Pt() << endl;

    //Particles not in jets
    #ifdef NIJ
    NIJ_g.SetPtEtaPhiE(0,0,0,0);
    bool printAllNIJ = false;
    if (printAllNIJ) cout << "*** START NIJ PRINT ****" << endl;
    for (unsigned int i = 0; i != prtclnij_pdgid->size(); ++i) {
      p4.SetPtEtaPhiE((*prtclnij_pt)[i], (*prtclnij_eta)[i],
                      (*prtclnij_phi)[i],(*prtclnij_e)[i]  );
      if (printAllNIJ) {
        cout << (*prtclnij_pdgid)[i] << ", " << p4.Pt()
                                     << ", " << p4.Eta() 
                                     << ", " << p4.Phi()
                                     << ", " << p4.E() << endl;
      }
      NIJ_g += p4;
      allprtcls += p4;
      if (isNeutrino((*prtclnij_pdgid)[i])) nu_pT += (*prtclnij_pt)[i];
    }
    if (printAllNIJ) cout << "*** END NIJ PRINT ****"           << endl;
    cout << "pT of prtcls not in jets: " << NIJ_g.Pt()          << endl; 
    cout << "(jetSum + NIJ_g) pT: "      << (jetSum+NIJ_g).Pt() << endl; 
    #endif

    cout << "Check nu pT    : "          << nu_pT               << endl;
    cout << "pT of all prtcls: "         << allprtcls.Pt()      << endl; 

    //Loop jets within this event:
    for (int ijet = 0; ijet != int(jet_e->size()); ++ijet) {

      //Reinit
      nptcls = 0;
      Esum = 0;
      p4sum.SetPtEtaPhiE(0,0,0,0);

      //Loop all particles in the event
      for (unsigned int i = 0; i != prtcl_pdgid->size(); ++i) {
        //See if the particle belongs to this jet
        if ((*prtcl_jet)[i] == ijet) {
	  nptcls++;
          p4.SetPtEtaPhiE( (*prtcl_pt)[i],  (*prtcl_eta)[i],
                           (*prtcl_phi)[i], (*prtcl_e)[i]  );
          p4sum += p4;
	  Esum  += (*prtcl_e)[i];
	}
        //if ((*prtcl_e)[i]  > 500) cout << "E > 500 GeV ptcl found!" << endl;
      }
      //Jet particle count
      cout<<"Jet "<<ijet<<": nptcls="<<nptcls
          <<", E_sum="<<Esum<<", pT_sum="<<p4sum.Pt()<<endl;

    }	//Loop jets

    /* Loop partons in the event */
    cout << "    Prtn tag\t ID\t p_T\t eta\t  phi\t\t E\t jetID" << endl;
    for (unsigned int j = 0; j != prtn_tag->size(); ++j) {
      cout << "    \t " << int((*prtn_tag)[j])
           << "\t " << (*prtn_pdgid)[j]
	   << "\t" << (*prtn_pt)[j]
	   << "\t" << (*prtn_eta)[j]
	   << " "  << (*prtn_phi)[j]
	   << "\t" << (*prtn_e)[j]
           << "\t" << (int)(*prtn_jet)[j] << endl;
    }	//Loop partons

  }	//Loop events

} //StudyTree
//-----------------------------------------------------------------------------
//Print a chosen event from the TTree read in by the CMSJES object.
void CMSJES::PrintEvt()
{
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();

  //Initialization
  Long64_t nbytes = 0, nb = 0;
  TLorentzVector p4, p4g;
  p4.SetPtEtaPhiE(0,0,0,0);
  p4g.SetPtEtaPhiE(0,0,0,0);

  //User interface
  cout<<nentries<<" evts. Type \"[i_evt] [i_jet]\" e.g. \"0 0\" to print"<<endl;
  cout<<"Type -1 to print particles not in jets"<<endl;
  Long64_t i_evt;
  int i_jet;
  cin >> i_evt >> i_jet;
  if (i_evt > nentries) i_evt = nentries;

  //Load event
  Long64_t ientry = LoadTree(i_evt);	//Load event
  nb = fChain->GetEntry(i_evt);   nbytes += (i_evt+1)*nb;

  cout << "Jet " << i_jet << " contains:" << endl;
  cout << "PDGID\tE\tp_T\teta" << endl;

  if (i_jet>=0) {	//Particles in jets
    for (int i=0; i < prtcl_pdgid->size(); ++i) {
      if ((*prtcl_jet)[i]==i_jet) {
        p4.SetPtEtaPhiE((*prtcl_pt)[i], (*prtcl_eta)[i],
	                (*prtcl_phi)[i],(*prtcl_e)[i]  );
        p4g += p4;
        cout << (*prtcl_pdgid)[i] << "\t"
             << (*prtcl_e)[i] << "\t"
             << (*prtcl_pt)[i] << "\t"
             << (*prtcl_eta)[i] << endl;
      }
    }
  }

  #ifdef NIJ
  else {		//Particles not in jets
    for (int i=0; i < prtclnij_pdgid->size(); ++i) {
      if ((*prtcl_jet)[i]==i_jet) {
        p4.SetPtEtaPhiE((*prtcl_pt)[i], (*prtcl_eta)[i],
	                (*prtcl_phi)[i],(*prtcl_e)[i]  );
        p4g += p4;
        cout << (*prtcl_pdgid)[i] << "\t"
             << (*prtcl_e)[i] << "\t"
             << (*prtcl_pt)[i] << "\t"
             << (*prtcl_eta)[i] << endl;
      }
    }
  }
  #endif

  cout << "p4g.E() = " << p4g.E()
       << ", p4g.Pt() = " << p4g.Pt()
       << ", p4g.Eta() = " << p4g.Eta() << endl;
} //PrintEvt
//-----------------------------------------------------------------------------
//User interface to choose which files to open in plotting functions
//Params:	nameAdd		Additions to the standard CMSJES output names
//		djstr, gjstr	Set dijet & gammajet all flavor filenames here
//		gen,alg,rad,...
//		ct,XS	Preset values if user interface omitted
//N.B. only the interesting cases are enabled in this function ATM
void CMSJES::plotQuery(string& nameAdd, string& djstr, string& gjstr,
                       string& zjstr, int& gen, int& Nevt)
{
  //Init
  string respStr = "./output_ROOT_files/CMSJES_";
  string root = ".root";	//File suffix

  /* User interface */
  //Choose generator
  cout << "Which generator to use? (1) P6 (2) H7 (3) P8" << endl;
  while (gen<1 || gen>3) cin >> gen;
  respStr += (gen==3 ? "P8_": (gen==1 ? "P6_": (gen==2 ? "H7_" : "")));

  //Set #events
  string num = "100000";
  cout << "#Events (1) 100k (2) 10k (3) 1k" << endl;
  while (Nevt<1 || Nevt>3) cin >> Nevt;
  if      (Nevt==1) num = "100000";
  else if (Nevt==2) num = "10000";
  else if (Nevt==3) num = "1000";

  //Construct all-flavor filenames
  djstr = respStr + "dijet_"    + num + root;
  gjstr = respStr + "gammajet_" + num + root;
  zjstr = respStr + "Zjet_"     + num + root;

  //Additions in filename
  nameAdd = num;
} //plotQuery

//-----------------------------------------------------------------------------
//A function to find and plot flavor dependent correction factors.
//Param		plot	if false, only reads in the F_corr histograms
//		gen	Preset value for which generator to use
//		alg	      -||-       jet algorithm
//		rad	      -||-       cone radius
//              ct	      -||-       ctau time-/lengthscale
//		XS	      -||-       "use strange hadron Ansätze?"
void CMSJES::flavCorr(bool plot, int gen, int alg, int rad, int ct,
                     int Nevt)
{
  bool fitMode = true;	//Draw fitted TF1s on top of the histos
  bool drawD0  = false;	//Draw D0 Fcorr on top of the histos (extracted from AN)
  bool drawD0_h= false;	//^Draw the histos instead of the fits
  bool Eprm    = true;	//True: F(E'), false: F(pT') at intpretation 0
  bool pTgen   = true;	//True: F(pT^gen_jet), false: F(pT^MC_jet) at int.pre. 1
  bool uncertF     = false;	//Draw F uncertainties?
  bool uncertFcorr = true;	//Draw Fcorr uncertainties?

  //Set color codes
  int black = kGray+3;  int red  = 2;   int blue  = 9;	//Dark shades
  int gray  = kGray+1;  int lred = 46;  int lblue = 33;	//Light shades

  //Choose filenames to open
  string nameAdd, in_d, in_g, in_z;
  plotQuery(nameAdd, in_d, in_g, in_z, gen, Nevt);

  //Check which generator was used for producing the files asked for
  string genStr="";
  if      (in_z.find("P6")!=string::npos) genStr="P6_";
  else if (in_z.find("P8")!=string::npos) genStr="P8_";
  else if (in_z.find("H7")!=string::npos) genStr="H7_";

  int const Nf=3;	//#Jet flavour classes: b, g, lq
  //int const Ns=2;	//#Sample types: gamma+jet, EM+jet
  int const Ns=3;	//#Sample types: gamma+jet, EM+jet, and Z+jet
  int const NI=2;	//#Plots w/ diff. interpretation for hor. axis variable

  //Retrieve flavor-dependent factor F profiles to these pointers
  //  Naming: F[flavour]_[gen/reco/fit]_[dijet/gammajet]
  TProfile* F[Nf][Ns][NI];    TProfile* Fjet[NI];
  TProfile* Fbstd[NI];	//Standard gamma+jet Fb for obtaining weights
  //...and their upper and lower limits (for syst. uncert. SU) here
  TProfile* FSU[Nf][Ns][NI];  TProfile* FjetSU[NI];
  //For storing TH1D projections of the above TProfiles
  TH1D* h_F[Nf][Ns][NI];      TH1D* h_FSU[Nf][Ns][NI];
  TH1D* h_Fjet[NI];           TH1D* h_FjetSU[NI];
  //The TFiles to open
  TFile* fd; TFile* fg; TFile* fz;

  if (!GetrunCMS()) {
  /* EM+jet (dijet) */
  fd = TFile::Open(in_d.c_str());
  if (!fd) {
    cout << "Error opening files! Exiting" << endl;
    return;
  }
  //b-jets
  if (Eprm) {fd->GetObject(  "Fb",      F[0][1][0]);
             fd->GetObject(  "FbSU",  FSU[0][1][0]);}
  else      {fd->GetObject(  "Fb_p",    F[0][1][0]);
             fd->GetObject(  "FbSU_p",FSU[0][1][0]);}
  if (pTgen) {fd->GetObject( "Fb_g",    F[0][1][1]);
              fd->GetObject( "FbSU_g",FSU[0][1][1]);}
  else       {fd->GetObject( "Fb_r",    F[0][1][1]);
              fd->GetObject( "FbSU_r",FSU[0][1][1]);}

  //Gluon jets, light quark jets and all jets (gamma+jet specific, for avg.)
  if (Eprm) {
    fd->GetObject("Fg",   F[1][1][0]);  fd->GetObject("FgSU",   FSU[1][1][0]);
    fd->GetObject("Flq",  F[2][1][0]);  fd->GetObject("FlqSU",  FSU[2][1][0]);
  } else {
    fd->GetObject("Fg_p", F[1][1][0]);  fd->GetObject("FgSU_p", FSU[1][1][0]);
    fd->GetObject("Flq_p",F[2][1][0]);  fd->GetObject("FlqSU_p",FSU[2][1][0]);
  }
  if (pTgen) {
    fd->GetObject("Fg_g", F[1][1][1]);  fd->GetObject("FgSU_g", FSU[1][1][1]);
    fd->GetObject("Flq_g",F[2][1][1]);  fd->GetObject("FlqSU_g",FSU[2][1][1]);
  } else {
    fd->GetObject("Fg_r", F[1][1][1]);  fd->GetObject("FgSU_r", FSU[1][1][1]);
    fd->GetObject("Flq_r",F[2][1][1]);  fd->GetObject("FlqSU_r",FSU[2][1][1]);
  }

  /* gamma+jet */
  fg   = TFile::Open(in_g.c_str());
  if (!fg) {
    cout << "Error opening files! Exiting" << endl;
    return;
  }
  //b-jets

  if (Eprm)  {fg->GetObject( "Fb",      F[0][0][0]);
              fg->GetObject( "FbSU",  FSU[0][0][0]);}
  else       {fg->GetObject( "Fb_p",    F[0][0][0]);
              fg->GetObject( "FbSU_p",FSU[0][0][0]);}
  if (pTgen) {fg->GetObject( "FbSU_g",FSU[0][0][1]);
	      fg->GetObject( "Fb_g",    F[0][0][1]);}
  else       {fg->GetObject( "FbSU_r",FSU[0][0][1]);
	      fg->GetObject( "Fb_r",    F[0][0][1]);}
  //Always retrieve standard sample's Fb, needed for Fcorr calculation (new avg)
  if (Eprm ) fg->GetObject("Fb",   Fbstd[0]);
  else       fg->GetObject("Fb_p", Fbstd[0]);
  if (pTgen) fg->GetObject("Fb_g", Fbstd[1]);
  else       fg->GetObject("Fb_r", Fbstd[1]);
  //Gluon jets, light quark jets and all jets (gamma+jet specific, for avg.)
  if (Eprm) {
    fg->GetObject("Fg",    F[1][0][0]);  fg->GetObject("FgSU",    FSU[1][0][0]);
    fg->GetObject("Flq",   F[2][0][0]);  fg->GetObject("FlqSU",   FSU[2][0][0]);
    fg->GetObject("Fjet",     Fjet[0]);  fg->GetObject("FjetSU",     FjetSU[0]);
  } else {
    fg->GetObject("Fg_p",  F[1][0][0]);  fg->GetObject("FgSU_p",  FSU[1][0][0]);
    fg->GetObject("Flq_p", F[2][0][0]);  fg->GetObject("FlqSU_p", FSU[2][0][0]);
    fg->GetObject("Fjet_p",   Fjet[0]);  fg->GetObject("FjetSU_p",   FjetSU[0]);
  }
  if (pTgen) {
    fg->GetObject("Fg_g",  F[1][0][1]);  fg->GetObject("FgSU_g", FSU[1][0][1]);
    fg->GetObject("Flq_g", F[2][0][1]);  fg->GetObject("FlqSU_g",FSU[2][0][1]);
    fg->GetObject("Fjet_g",   Fjet[1]);  fg->GetObject("FjetSU_g",  FjetSU[1]);
  } else {
    fg->GetObject("Fg_r",  F[1][0][1]);  fg->GetObject("FgSU_r", FSU[1][0][1]);
    fg->GetObject("Flq_r", F[2][0][1]);  fg->GetObject("FlqSU_r",FSU[2][0][1]);
    fg->GetObject("Fjet_r",   Fjet[1]);  fg->GetObject("FjetSU_r",  FjetSU[1]);
  }

  } //if !runCMS
  else {
    //Z+jet 
    fz   = TFile::Open(in_z.c_str());
    if (!fz) {
      cout << "Error opening files! Exiting" << endl;
      return;
    }
    //b-jets
    if (Eprm)  {fz->GetObject( "Fb",      F[0][2][0]);
                fz->GetObject( "FbSU",  FSU[0][2][0]);}
    else       {fz->GetObject( "Fb_p",    F[0][2][0]);
                fz->GetObject( "FbSU_p",FSU[0][2][0]);}
    if (pTgen) {fz->GetObject( "FbSU_g",FSU[0][2][1]);
                fz->GetObject( "Fb_g",    F[0][2][1]);}
    else       {fz->GetObject( "FbSU_r",FSU[0][2][1]);
                fz->GetObject( "Fb_r",    F[0][2][1]);}
    //Always retrieve standard sample's Fb, needed for Fcorr calculation (new avg)
    if (Eprm ) fz->GetObject("Fb",   Fbstd[0]);
    else       fz->GetObject("Fb_p", Fbstd[0]);
    if (pTgen) fz->GetObject("Fb_g", Fbstd[1]);
    else       fz->GetObject("Fb_r", Fbstd[1]);
    //Gluon jets, light quark jets and all jets (gamma+jet specific, for avg.)
    if (Eprm) {
      fz->GetObject("Fg",    F[1][2][0]);  fz->GetObject("FgSU",    FSU[1][2][0]);
      fz->GetObject("Flq",   F[2][2][0]);  fz->GetObject("FlqSU",   FSU[2][2][0]);
      fz->GetObject("Fjet",     Fjet[0]);  fz->GetObject("FjetSU",     FjetSU[0]);
    } else {
      fz->GetObject("Fg_p",  F[1][2][0]);  fz->GetObject("FgSU_p",  FSU[1][2][0]);
      fz->GetObject("Flq_p", F[2][2][0]);  fz->GetObject("FlqSU_p", FSU[2][2][0]);
      fz->GetObject("Fjet_p",   Fjet[0]);  fz->GetObject("FjetSU_p",   FjetSU[0]);
    }
    if (pTgen) {
      fz->GetObject("Fg_g",  F[1][2][1]);  fz->GetObject("FgSU_g", FSU[1][2][1]);
      fz->GetObject("Flq_g", F[2][2][1]);  fz->GetObject("FlqSU_g",FSU[2][2][1]);
      fz->GetObject("Fjet_g",   Fjet[1]);  fz->GetObject("FjetSU_g",  FjetSU[1]);
    } else {
      fz->GetObject("Fg_r",  F[1][2][1]);  fz->GetObject("FgSU_r", FSU[1][2][1]);
      fz->GetObject("Flq_r", F[2][2][1]);  fz->GetObject("FlqSU_r",FSU[2][2][1]);
      fz->GetObject("Fjet_r",   Fjet[1]);  fz->GetObject("FjetSU_r",  FjetSU[1]);
    }
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


  /* Conversion to prtcl lvl jet pT from gamma+jet sample */

  TCanvas* canvConv = new TCanvas("canvConv","",400,400);
  canvConv->SetLeftMargin(0.12);
  TProfile *tEMpG = 0;
  TProfile *prEpP,    *prEpP_b,   *prEpP_g,   *prEpP_lq   = 0; 
  TProfile *prRtrue,  *prRtrue_b, *prRtrue_g, *prRtrue_lq = 0;
  TProfile *prpTppRb, *prpTppRg,  *prpTppRlq = 0;
  fg->GetObject(       "EpP",      prEpP     );  prEpP->SetStats(     0);
  fg->GetObject(       "EpP_g",    prEpP_g   );  prEpP_g->SetStats(   0);
  fg->GetObject(       "EpP_lq",   prEpP_lq  );  prEpP_lq->SetStats(  0);
  fg->GetObject(       "pRpGa",    prRtrue   );  prRtrue->SetStats(   0);
  fg->GetObject(       "pTppRg",   prpTppRg  );  prpTppRg->SetStats(  0);
  fg->GetObject(       "pTppRlq",  prpTppRlq );  prpTppRlq->SetStats( 0);
  fg->GetObject(       "pTppRb",   prpTppRb  );  prpTppRb->SetStats(  0);
  fg->GetObject(       "EpP_b",    prEpP_b   );  prEpP_b->SetStats(   0);
  fg->GetObject(       "pRpGb",    prRtrue_b );  prRtrue_b->SetStats( 0);
  fg->GetObject(       "pRpGg",    prRtrue_g );  prRtrue_g->SetStats( 0);
  fg->GetObject(       "pRpGlq",   prRtrue_lq);  prRtrue_lq->SetStats(0);
  fg->GetObject(       "tEMpG",    tEMpG     );  tEMpG->SetStats(     0);
  string pwrlaw   = "[0]*(1-[1]*pow(x,[2]-1))";	//D. Groom parametrization
  TF1* fpTbal     = new TF1("fpTbal",    pwrlaw.c_str(), 30, 500);
  TF1* fEpP       = new TF1("fEpP",     "pol1",          10, 145);
  TF1* fEpP_b     = new TF1("fEpP_b",   "pol1",          10, 145);
  TF1* fEpP_g     = new TF1("fEpP_g",   "pol1",          10, 145);
  TF1* fEpP_lq    = new TF1("fEpP_lq",  "pol1",          10, 145);
  TF1* fRtrue;   TF1* fRtrue_b;  TF1* fRtrue_g;  TF1* fRtrue_lq;
  TF1* fpTppRb;  TF1* fpTppRg;   TF1* fpTppRlq;
  bool fitPwrLaw = false;
  if (fitPwrLaw) {
    fRtrue        = new TF1("fRtrue",    pwrlaw.c_str(), 30, 200);
    fRtrue_b      = new TF1("fRtrue_b",  pwrlaw.c_str(), 30, 200);
    fRtrue_g      = new TF1("fRtrue_g",  pwrlaw.c_str(), 30, 200);
    fRtrue_lq     = new TF1("fRtrue_lq", pwrlaw.c_str(), 30, 200);
    fpTppRb       = new TF1("fpTppRb",   pwrlaw.c_str(), 30, 200);
    fpTppRg       = new TF1("fpTppRg",   pwrlaw.c_str(), 30, 200);
    fpTppRlq      = new TF1("fpTppRlq",  pwrlaw.c_str(), 30, 200);
  } else {
    fRtrue        = new TF1("fRtrue",    "pol1",         0,  200);
    fRtrue_b      = new TF1("fRtrue_b",  "pol1",         0,  200);
    fRtrue_g      = new TF1("fRtrue_g",  "pol1",         0,  200);
    fRtrue_lq     = new TF1("fRtrue_lq", "pol1",         0,  200);
    fpTppRb       = new TF1("fpTppRb",   "pol1",         0,  200);
    fpTppRg       = new TF1("fpTppRg",   "pol1",         0,  200);
    fpTppRlq      = new TF1("fpTppRlq",  "pol1",         0,  200);
  }
  prEpP->Fit(  fEpP,  "Q");    prEpP_b->Fit( fEpP_b, "Q");
  prEpP_g->Fit(fEpP_g,"Q");    prEpP_lq->Fit(fEpP_lq,"Q");

  TF1* ftEMpG  = new TF1("ftEMpG",  pwrlaw.c_str(), 30, 200);
  //Some initial guesses for the parameters. The choice doesn't seem to matter
  //much, bu ROOT needs some init values when fitting a "custom" function
  ftEMpG->SetParameters( 1.15,0.7,0.85);
  cout << "*** tEMpG fit" << endl;              tEMpG->Fit(   ftEMpG     );
  fpTbal->SetParameters(   1.15,0.7,0.85);
  TF1* fF[Nf][Ns][NI];
  stringstream fFname;
  for (int f=0; f!=Nf; ++f) {
    for (int s=0; s!=Ns; ++s) {
      for (int a=0; a!=NI; ++a) {
        fFname.str("");
        fFname << "fF_" << (f==0?"b_":(f==1?"g_":"lq_"))
                        << (s==0?"g_":"d_") << (a==0?"Ep":"pT");
        fF[f][s][a] = new TF1(fFname.str().c_str(),pwrlaw.c_str(),30,145);
        fF[f][s][a]->SetParameters(1.15,0.7,0.85);
        F[ f][s][a]->Fit(fF[f][s][a], "Q");
      }
    }
  }
  if (fitPwrLaw) {
    fRtrue->SetParameters(   1.15,0.7,0.85);
    fRtrue_b->SetParameters( 1.15,0.7,0.85);
    fRtrue_g->SetParameters( 1.15,0.7,0.85);
    fRtrue_lq->SetParameters(1.15,0.7,0.85);
  }
  TLegend* RtrueLegend = new TLegend(0.7,0.13,0.9,0.37);
  RtrueLegend->SetFillStyle(0);		RtrueLegend->SetBorderSize(0);
  RtrueLegend->AddEntry(fRtrue_lq,"#font[12]{lq}#font[132]{-jets}", "l");
  RtrueLegend->AddEntry(fRtrue,   "#font[132]{All jets}",           "l");
  RtrueLegend->AddEntry(fRtrue_b, "#font[12]{b}#font[132]{-jets}",  "l");
  RtrueLegend->AddEntry(fRtrue_g, "#font[12]{g}#font[132]{-jets}",  "l");
  if (fitPwrLaw) {
    cout <<"\n\n*** Fitting power law to (p_T^gen, p_T^MC) ***" << endl;
  } else cout <<"\n\n*** Fitting pol1 to (p_T^gen, p_T^MC) ***" << endl;
  cout <<        "All jets: "<< endl;  prRtrue->Fit(   fRtrue   );
  cout <<        "\nb-jets: "<< endl;  prRtrue_b->Fit( fRtrue_b );
  cout <<        "\ng-jets: "<< endl;  prRtrue_g->Fit( fRtrue_g );
  cout <<"\n(u,d,s,c)-jets: "<< endl;  prRtrue_lq->Fit(fRtrue_lq);
  if (fitPwrLaw) {
    cout <<"\n\n*** Fitting power law to (p_T', p_T^MC) ***" << endl;
  } else cout <<"\n\n*** Fitting pol1 to (p_T', p_T^MC) ***" << endl;
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
  tEMpG->SetLineColor(    kBlack   );  ftEMpG->SetTitle(        ""  );
  ftEMpG->SetLineColor(   kBlack   );  ftEMpG->SetTitle(        ""  );
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
  fEpP_b->Draw("SAME");     fEpP_g->Draw("SAME");     fEpP_lq->Draw("SAME");
  convsavename = convsavename + genStr + nameAdd + ".eps";
  canvConv->Print(convsavename.c_str());
  //pT'
  convsavename = "./plots/pTconversion/pTp_";
  prpTppRlq->Draw();        fpTppRlq->Draw("SAME");   prpTppRb->Draw("SAME");
  prpTppRg->Draw("SAME");   fpTppRb->Draw( "SAME");   fpTppRg->Draw( "SAME");
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
  //pTMC
  convsavename = "./plots/pTconversion/pT_MC_";
  tEMpG->Draw();
  ftEMpG->Draw("SAME");
  convsavename = convsavename + genStr + nameAdd + ".eps";
  canvConv->Print(convsavename.c_str());

  //Find a new shifted Fjet TProfile proper avg.ing. The contribution of b-jets
  //must no be taken from the b-enriched sample but the unbiased one. However,
  //there are so few of them that the b-jet contribution becomes negligible and
  //we average here only by the g-jet and lq-jet weights. These are returned by 
  //The function TProfile->GetBinEntries(i), i>=1.
  TF1* FjetS = new TF1("FjetS",pwrlaw.c_str(),20,145);
  TH1D* h_FjetS[NI];	//Shifted <F>_{gamma+jet}
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
                             << (s==0?"g_":"d_") << (a==0?"Ep":"pT");
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
        else if (!pTgen) {shift_b  = init - init*ftEMpG->Eval(   init);
                          shift_g  = init - init*ftEMpG->Eval(   init);
                          shift_lq = init - init*ftEMpG->Eval(   init);}
        else             {shift_b  = init -      fRtrue_b->Eval( init);
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
        if (s==0) {
          h_FjetS[a]->SetBinContent(i, (FbSstd*Nb + FgS*Ng + FlqS*Nlq)
                                       /((double) Ntot)               );
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

  //Fcorr = F normalized by gamma+jet sample average (given by Fjet)
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
        for (int s=0; s!=Ns; ++s) {	//Sample type (gamma+jet, EM+jet)
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
  else if (in_g.find("IIb")!=string::npos) pTtitle += "_{T,jet}^{MC'}";
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
    h_F[0][0][a]->SetMarkerStyle(cirF);    h_F[0][1][a]->SetMarkerStyle(cirO);
    h_F[1][0][a]->SetMarkerStyle(sqrF);    h_F[1][1][a]->SetMarkerStyle(sqrO);
    h_F[2][0][a]->SetMarkerStyle(diaF);    h_F[2][1][a]->SetMarkerStyle(diaO);
    h_Fcorr[0][0][a]->SetMarkerStyle(cirF);
    h_Fcorr[0][1][a]->SetMarkerStyle(cirO);
    h_Fcorr[1][0][a]->SetMarkerStyle(sqrF);
    h_Fcorr[1][1][a]->SetMarkerStyle(sqrO);
    h_Fcorr[2][0][a]->SetMarkerStyle(diaF);
    h_Fcorr[2][1][a]->SetMarkerStyle(diaO);
    //Marker colors
    h_F[0][0][a]->SetMarkerColor(lred );   h_F[0][1][a]->SetMarkerColor(lred );
    h_F[1][0][a]->SetMarkerColor(black);   h_F[1][1][a]->SetMarkerColor(gray );
    h_F[2][0][a]->SetMarkerColor(blue );   h_F[2][1][a]->SetMarkerColor(lblue);
    h_Fcorr[0][0][a]->SetMarkerColor(lred );
    h_Fcorr[0][1][a]->SetMarkerColor(lred );
    h_Fcorr[1][0][a]->SetMarkerColor(black);
    h_Fcorr[1][1][a]->SetMarkerColor(gray );
    h_Fcorr[2][0][a]->SetMarkerColor(blue );
    h_Fcorr[2][1][a]->SetMarkerColor(lblue);
  }

  //For adding D0 fits as TF1 pol2
  TF1* D0Fbcorr  = new TF1("D0Fbcorr","pol2",0,140); //Extracted from AN...
  TF1* D0Fgcorr  = new TF1("D0Fbcorr","pol2",0,140); //...Fcorr-1 plots
  TF1* D0Flqcorr = new TF1("D0Fbcorr","pol2",0,140);
  TF1* D0FbcorrALT  = new TF1("D0FbcorrALT","pol2",0,140); //Extracted from...
  TF1* D0FgcorrALT  = new TF1("D0FbcorrALT","pol2",0,140); //...AN fits over...
  TF1* D0FlqcorrALT = new TF1("D0FbcorrALT","pol2",0,140); //...histograms
  TGraph* g_D0Fb  = new TGraph();     TGraph* g_D0Fbcorr  = new TGraph();
  TGraph* g_D0Fg  = new TGraph();     TGraph* g_D0Fgcorr  = new TGraph();
  TGraph* g_D0Flq = new TGraph();     TGraph* g_D0Flqcorr = new TGraph();
  D0Fbcorr->SetLineColor( lred );     D0FbcorrALT->SetLineColor( red    );
  D0Fgcorr->SetLineColor( gray );     D0FgcorrALT->SetLineColor( black  );
  D0Flqcorr->SetLineColor(lblue);     D0FlqcorrALT->SetLineColor(blue   );
  D0Fgcorr->SetLineStyle( kDashed);   D0Flqcorr->SetLineStyle(   kDotted);
  D0FgcorrALT->SetLineStyle( kDashDotted);
  D0FlqcorrALT->SetLineStyle(kDotted);
  g_D0Flqcorr->SetMarkerStyle(diaO);  g_D0Fgcorr->SetMarkerStyle( sqrO);
  g_D0Fbcorr->SetMarkerStyle( cirO);  g_D0Flq->SetMarkerStyle(    diaO);
  g_D0Fg->SetMarkerStyle(     sqrO);  g_D0Fb->SetMarkerStyle(     cirO);
  g_D0Fb->SetMarkerColor( red  );     g_D0Fbcorr->SetMarkerColor( red  );
  g_D0Fg->SetMarkerColor( black);     g_D0Fgcorr->SetMarkerColor( black);
  g_D0Flq->SetMarkerColor(blue );     g_D0Flqcorr->SetMarkerColor(blue );

  //For reading the extracted D0 Fcorr histogram points from files
  ifstream D0FbcH;  ifstream D0FgcH;  ifstream D0FlqcH;	//Input streams
  ifstream D0FbH;   ifstream D0FgH;   ifstream D0FlqH;
  double pTtemp = 0, Fctemp = 0;			//Temps for reading
  int l_max=27;						//#Lines to read

  //Set parameters obtained by fitting pol2 above D0 AN plots.
  //  The ALT function parameters are taken directly from the ANs
  if (nameAdd.find("RunIIa")!=string::npos) {
    D0FbcH.open( "./data_and_MC_input/Fcorr/runIIa/eta00-04/Fbcorr_histo" );
    D0FgcH.open( "./data_and_MC_input/Fcorr/runIIa/eta00-04/Fgcorr_histo" );
    D0FlqcH.open("./data_and_MC_input/Fcorr/runIIa/eta00-04/Flqcorr_histo");
    D0FbH.open( "./data_and_MC_input/F/runIIa/eta00-04/Fb" );
    D0FgH.open( "./data_and_MC_input/F/runIIa/eta00-04/Fg" );
    D0FlqH.open("./data_and_MC_input/F/runIIa/eta00-04/Flq");
    D0Fbcorr->SetParameters(    0.970084, 0.000234787, 2.58402e-08);
    D0Fgcorr->SetParameters(    0.983459, 7.64048e-05, 3.50195e-09);
    D0Flqcorr->SetParameters(   0.999494, 7.07541e-05,-1.45269e-07);
    D0FbcorrALT->SetParameters( 0.970,    0.0002394,             0); 
    D0FgcorrALT->SetParameters( 0.9835,   0.00007392, -1.76300e-08);
    D0FlqcorrALT->SetParameters(0.9996,   6.756e-05,  -1.28200e-07);
  } else cout << "Unknown run, could not set D0 parameters" << endl;

  //Read Fcorr histo points into TGraphs
  for (int l=0; l!=l_max; ++l) {
    D0FbcH  >> pTtemp >> Fctemp;  g_D0Fbcorr->SetPoint( l,pTtemp,Fctemp);
    D0FgcH  >> pTtemp >> Fctemp;  g_D0Fgcorr->SetPoint( l,pTtemp,Fctemp);
    D0FlqcH >> pTtemp >> Fctemp;  g_D0Flqcorr->SetPoint(l,pTtemp,Fctemp);
    D0FbH   >> pTtemp >> Fctemp;  g_D0Fb->SetPoint( l,pTtemp,Fctemp);
    D0FgH   >> pTtemp >> Fctemp;  g_D0Fg->SetPoint( l,pTtemp,Fctemp);
    D0FlqH  >> pTtemp >> Fctemp;  g_D0Flq->SetPoint(l,pTtemp,Fctemp);
  } 
  D0FbcH.close();  D0FbcH.close();  D0FbcH.close(); //Close Fcorr&F histo files
  D0FbH.close();   D0FbH.close();   D0FbH.close();

  h_Fcorr[1][0][0]->SetAxisRange(0.9,1.02,"Y");
  string runTitle = "#font[132]{Run ";
  if      (nameAdd.find("RunIIa"  )!=string::npos) runTitle += "IIa}";
  h_Fcorr[1][0][0]->SetTitle(runTitle.c_str());
  
  /* F PLOTS */

  //To store savefile name
  string savename;

  //Indicate if the plots use D0 A,B,C in our MC
  string OMC = "#font[132]{Our MC}";

  //Where to place some additional LaTeX text in the plots
  double latexBD[3]={70.0, 85.0, 0.955};
  if (nameAdd.find("RunIIa")!=string::npos) latexBD[2]=0.855;

  //Canvas and related setup
  TCanvas* canv = new TCanvas("canv","",400,400);
  canv->SetTicks(1,1);		//Axis ticks on both sides
  canv->SetLeftMargin(0.12);	//Room for axis label
  canv->SetBottomMargin(0.115);

  //Convert jet pT to particle level to be comparable w/ D0
  //Indices, growing order: [flavour: b,g,lq][sample: gj,dj][Hor.axis:E',pT]
  TGraphErrors* g_F[Nf][Ns][NI];  TGraphErrors* g_Fcorr[Nf][Ns][NI];
  TGraphErrors* g_Fjet[NI];		//Taken only from gamma+jet sample
  for (int a=0; a!=NI; ++a) {		//Axis interpretations
    for (int s=0; s!=Ns; ++s) {		//Sample type
      for (int f=0; f!=Nf; ++f) {	//Flavour
        g_F[f][s][a]=new TGraphErrors();  g_Fcorr[f][s][a]=new TGraphErrors();
      }
    }
    g_Fjet[a] = new TGraphErrors();
  }

  /* F level: copy points and uncertainties from histos */

  for (int a=0; a!=NI; ++a) {		//Horizontal axis interpretations

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
  TLegend* lgF;
  TLegend* lgFD0  = new TLegend(0.45,0.15,0.55, 0.325);
  if (drawD0) lgF = new TLegend(0.50,0.15,0.92,0.325);
  else        lgF = new TLegend(0.50,0.15,0.92,0.50 );
  if (!drawD0) lgF->AddEntry(g_F[2][1][0],
                    "#font[132]{EM+jet (#font[12]{u,d,s,c})-jets}",     "p");
  lgF->AddEntry(g_F[2][0][0],
                    "#font[132]{#gamma+jet (#font[12]{u,d,s,c})-jets}", "p");
  if (!drawD0) lgF->AddEntry(g_F[1][1][0],
                             "#font[132]{EM+jet gluon jets}",           "p");
  lgF->AddEntry(g_F[1][0][0],"#font[132]{#gamma+jet gluon jets}",       "p");
  if (!drawD0) lgF->AddEntry(g_F[0][1][0],
                  "#font[132]{EM+jet #font[12]{b}-jets}",               "p");
  lgF->AddEntry(g_F[0][0][0],"#font[132]{#gamma+jet #font[12]{b}-jets}","p");
  if (drawD0) {
    lgFD0->AddEntry(g_D0Flq, " ", "p");
    lgFD0->AddEntry(g_D0Fg,  " ", "p");
    lgFD0->AddEntry(g_D0Fb,  " ", "p");
  }

  //Setup the plots and draw
  for (int a=0; a!=NI; ++a) {	//Horizontal axis interpretations
    g_F[2][1][a]->SetLineColor(    lblue);  g_F[2][0][a]->SetLineColor(blue );
    g_F[1][1][a]->SetLineColor(    gray );  g_F[1][0][a]->SetLineColor(black);
    g_F[0][1][a]->SetLineColor(    red  );  g_F[0][0][a]->SetLineColor(lred );
    g_Fcorr[2][1][a]->SetFillColor(lblue);  g_Fcorr[2][1][a]->SetLineWidth(0);
    g_Fcorr[2][0][a]->SetFillColor(blue );  g_Fcorr[2][0][a]->SetLineWidth(0);
    g_Fcorr[1][1][a]->SetFillColor(gray );  g_Fcorr[1][1][a]->SetLineWidth(0);
    g_Fcorr[1][0][a]->SetFillColor(black);  g_Fcorr[1][0][a]->SetLineWidth(0);
    g_Fcorr[0][1][a]->SetFillColor(red  );  g_Fcorr[0][1][a]->SetLineWidth(0);
    g_Fcorr[0][0][a]->SetFillColor(lred );  g_Fcorr[0][0][a]->SetLineWidth(0);
    g_F[2][1][a]->SetMarkerStyle(  diaO );  g_F[2][1][a]->SetMarkerColor(lblue);
    g_F[2][0][a]->SetMarkerStyle(  diaF );  g_F[2][0][a]->SetMarkerColor(blue );
    g_F[1][1][a]->SetMarkerStyle(  sqrO );  g_F[1][1][a]->SetMarkerColor(gray );
    g_F[1][0][a]->SetMarkerStyle(  sqrF );  g_F[1][0][a]->SetMarkerColor(black);
    g_F[0][1][a]->SetMarkerStyle(  cirO );  g_F[0][1][a]->SetMarkerColor(red  );
    g_F[0][0][a]->SetMarkerStyle(  cirF );  g_F[0][0][a]->SetMarkerColor(lred );
    g_Fcorr[2][1][a]->SetMarkerStyle(diaO );
    g_Fcorr[2][1][a]->SetMarkerColor(lblue);
    g_Fcorr[2][0][a]->SetMarkerStyle(diaF );
    g_Fcorr[2][0][a]->SetMarkerColor(blue );
    g_Fcorr[1][1][a]->SetMarkerStyle(sqrO );
    g_Fcorr[1][1][a]->SetMarkerColor(gray );
    g_Fcorr[1][0][a]->SetMarkerStyle(sqrF );
    g_Fcorr[1][0][a]->SetMarkerColor(black);
    g_Fcorr[0][1][a]->SetMarkerStyle(cirO );
    g_Fcorr[0][1][a]->SetMarkerColor(red  );
    g_Fcorr[0][0][a]->SetMarkerStyle(cirF );
    g_Fcorr[0][0][a]->SetMarkerColor(lred );
    g_Fcorr[2][0][a]->SetFillStyle(3395);
    g_Fcorr[1][0][a]->SetFillStyle(3354);
    g_Fcorr[0][0][a]->SetFillStyle(3345);
    gStyle->SetHatchesSpacing(1.3);	//Fill pattern sparsity

    //F plots
    if (nameAdd.find("RunIIa")!=string::npos) {
      h_F[0][0][a]->SetAxisRange(0.8,1.0,"Y");
    } else h_F[0][0][a]->SetAxisRange(0.9,1.1,"Y");
    h_F[0][0][a]->GetYaxis()->SetTitle("#font[12]{F}");
    h_F[0][0][a]->SetAxisRange(15,140,"X");
    h_F[0][0][a]->Draw(             "P HISTO"     );
    if (!drawD0) h_F[0][1][a]->Draw("P HISTO SAME");
    h_F[1][0][a]->Draw(             "P HISTO SAME");
    if (!drawD0) h_F[1][1][a]->Draw("P HISTO SAME");
    if (!drawD0) h_F[2][1][a]->Draw("P HISTO SAME");
    h_F[2][0][a]->Draw(             "P HISTO SAME");

    lgF->SetBorderSize(  0);  lgF->SetFillStyle(  0);
    lgFD0->SetBorderSize(0);  lgFD0->SetFillStyle(0);
    if (drawD0) {
      latexF.SetTextSize(0.035);
      latexF.DrawLatex(latexBD[0], latexBD[2], "#font[132]{D#oslash}");
      latexF.DrawLatex(latexBD[1], latexBD[2], OMC.c_str());
      g_D0Fb->Draw( "P SAME");
      g_D0Fg->Draw( "P SAME");
      g_D0Flq->Draw("P SAME");
      lgFD0->Draw();
    }
    lgF->Draw();
    savename = "./plots/F/";
    if (drawD0 || drawD0_h) savename += "D0comp_";
    savename = savename+"F_"+(a==0?"Ep_":"pTjet_")+genStr+nameAdd+".eps";
    canv->Print(savename.c_str());	//Save plot
  } //Horizontal axis interpretations

  /* F_CORR PLOTS */

  //Legends etc.
  TLegend* lgc;
  //Only gamma+jet drawn as comparison
  if (drawD0) lgc = new TLegend(0.25, 0.15, 0.67, 0.325);
  else        lgc = new TLegend(0.50, 0.15, 0.92, 0.50);
  TLegend* lgcD0    = new TLegend(0.74, 0.15, 0.85, 0.325);
  TLegend* lgcD0ALT = new TLegend(0.66, 0.15, 0.80, 0.325);
  lgcD0->SetBorderSize(0);     lgcD0->SetFillStyle(0);
  lgcD0ALT->SetBorderSize(0);  lgcD0ALT->SetFillStyle(0);
  lgc->SetBorderSize(0);       lgc->SetFillStyle(0);
  if (!drawD0_h && !drawD0) lgc->AddEntry(g_Fcorr[2][1][0],
                   "#font[132]{EM+jet (#font[12]{u,d,s,c})-jets}",        "p");
  lgc->AddEntry(                          g_Fcorr[2][0][0],
                "#font[132]{#gamma+jet (#font[12]{u,d,s,c})-jets}",       "p");
  if (!drawD0_h && !drawD0) lgc->AddEntry(g_Fcorr[0][1][0],
                               "#font[132]{EM+jet #font[12]{b}-jets}",    "p");
  lgc->AddEntry(g_Fcorr[0][0][0],
                               "#font[132]{#gamma+jet #font[12]{b}-jets}","p");
  if (!drawD0_h && !drawD0) lgc->AddEntry(g_Fcorr[1][1][0],
                               "#font[132]{EM+jet gluon jets}",           "p");
  lgc->AddEntry(g_Fcorr[1][0][0], "#font[132]{#gamma+jet gluon jets}",    "p");
  if (drawD0) {
    lgcD0->SetTextFont(132);
    lgcD0->AddEntry(D0Flqcorr," #font[12]{F}^{#font[12]{lq}}_{corr}", "l");
    lgcD0->AddEntry(D0Fbcorr, " #font[12]{F}^{#font[12]{b}}_{corr}",  "l");
    lgcD0->AddEntry(D0Fgcorr, " #font[12]{F}^{#font[12]{g}}_{corr}",  "l");
  }
  TLatex latex;	latex.SetTextFont(132);	//For writing on the plot
  if (drawD0_h) {
    g_D0Fbcorr->Draw( "P SAME");  lgcD0ALT->AddEntry(g_D0Flqcorr, " ", "p");
    g_D0Fgcorr->Draw( "P SAME");  lgcD0ALT->AddEntry(g_D0Fbcorr,  " ", "p");
    g_D0Flqcorr->Draw("P SAME");  lgcD0ALT->AddEntry(g_D0Fgcorr,  " ", "p");
  } else if (drawD0) {	//Continuous functions fitted above D0 histos in the ANs
    D0FbcorrALT->Draw( "SAME");   lgcD0ALT->AddEntry(D0FlqcorrALT," ", "l");
    D0FgcorrALT->Draw( "SAME");   lgcD0ALT->AddEntry(D0FbcorrALT, " ", "l");
    D0FlqcorrALT->Draw("SAME");   lgcD0ALT->AddEntry(D0FgcorrALT, " ", "l");
  }

  //Fcorr
  for (int a=0; a!=NI; ++a) {	//Horizontal axis interpretations
    h_Fcorr[1][0][a]->GetXaxis()->SetRangeUser(15,125);
    h_Fcorr[1][0][a]->SetAxisRange(0.9,1.02,"Y");
    h_Fcorr[1][0][a]->GetYaxis()->SetTitle("#font[132]{#font[12]{F}_{corr}}");
    h_Fcorr[1][0][a]->SetMarkerSize(0);
    h_Fcorr[1][0][a]->Draw("P HISTO");	//Ordering s.t. this one gives title
    if (uncertFcorr) {  //Draw uncertainty bands at the bottom
      h_Fcorr[1][0][a]->Draw("E3 SAME");
      h_Fcorr[0][0][a]->Draw("E3 SAME");
      h_Fcorr[2][0][a]->Draw("E3 SAME");
    }
    g_Fcorr[1][0][a]->Draw("P SAME" );	//Draw horizontally shifted points 
    if (drawD0) {
      D0Fbcorr->Draw("SAME");  D0Fgcorr->Draw("SAME");  D0Flqcorr->Draw("SAME");
      g_Fcorr[1][0][a]->Draw("P SAME");	//Draw this one again to put atop
      if (drawD0_h) {
        g_D0Fbcorr->Draw( "P SAME");
        g_D0Fgcorr->Draw( "P SAME");
        g_D0Flqcorr->Draw("P SAME");
      } else {	//The continuous functions fitted aboe D0 histos in the ANs
        D0FbcorrALT->Draw( "SAME");
        D0FgcorrALT->Draw( "SAME");
        D0FlqcorrALT->Draw("SAME");
      }
      lgcD0->Draw();  lgcD0ALT->Draw();	//Draw D0 legends
      //Write above the legend where the different D0 Fcorr have been extracted
      latex.SetTextSize(0.037);
      latex.DrawLatex(39.0, 0.933, OMC.c_str());
      latex.SetTextSize(0.035);
      latex.DrawLatex(90.0, 0.948, "D#oslash #font[12]{F}_{corr} extracted");
      latex.DrawLatex(90.0, 0.942, "from AN fits to:");
      latex.SetTextSize(0.03);
      latex.DrawLatex(91.5,  0.935,  "Hist."                );
      latex.DrawLatex(102.0, 0.9375, "#font[12]{F}_{corr}-1");
      latex.DrawLatex(102.0, 0.9325, " plots"               );
    }
    if (!drawD0) g_Fcorr[1][1][a]->Draw("P SAME");
    if (!drawD0) g_Fcorr[0][1][a]->Draw("P SAME");
    g_Fcorr[0][0][a]->Draw(             "P SAME");
    if (!drawD0) g_Fcorr[2][1][a]->Draw("P SAME");
    g_Fcorr[2][0][a]->Draw(             "P SAME");
    lgc->Draw();
    savename = "./plots/Fcorr/";
    if (drawD0 || drawD0_h) savename += "D0comp_";
    savename = savename+"Fcorr_"+(a==0?"Ep_":"pTjet_")+genStr+nameAdd+".eps";
    canv->Print(savename.c_str());	//Save plot
  } //Horizontal axis interpretations

  delete canv;  
  TCanvas* canvFit = new TCanvas("","",400,400);
  canvFit->SetBottomMargin(0.11);  canvFit->SetLeftMargin(0.12);
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

    //Find an average of the gamma+jet and EM+jet Fcorr histograms
    double Favg=0;  double Navg[Ns]; //Temps for avg
    for (int a=0; a!=NI; ++a) {	//Horizontal axis interpretations
      for (int f=0; f!=Nf; ++f) {	//Jet flavours
        for (int i=1; i!=1+h_Fcorr[f][0][a]->GetXaxis()->GetNbins(); ++i) {
          Navg[0] = 1e6*F[f][0][a]->GetBinEntries(i); //Factors 1e6 guard for...
          Navg[1] = 1e6*F[f][1][a]->GetBinEntries(i); //...NaN, but will cancel
          Favg =( Navg[0]*h_Fcorr[f][0][a]->GetBinContent(i)
                 +Navg[1]*h_Fcorr[f][1][a]->GetBinContent(i)
                ) / ( (Navg[0]+Navg[1]));
          h_Fcorr_avg[f][a]->SetBinContent(i, Favg);
          h_Fcorr_avg[f][a]->SetBinError(i,max(h_Fcorr[f][0][a]->GetBinError(i),
                                             h_Fcorr[f][1][a]->GetBinError(i)));
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
          g_Fcorr_avg[f][a]->SetPointError(i-1, 0,
                                           h_Fcorr_avg[f][a]->GetBinError(i));
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
    lgfit->AddEntry(g_Fcorr_avg[1][0],"#font[12]{g}#font[132]{-jets}",     "p");
    lgfit->AddEntry(fit_g,      "#font[12]{g}#font[132]{-jet fit}",        "l");
    lgfit->AddEntry(g_Fcorr_avg[0][0], "#font[12]{b}#font[132]{-jets}",    "p");
    lgfit->AddEntry(fit_b,      "#font[12]{b}#font[132]{-jet fit}",        "l");
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
      latex_fit.DrawLatex(79,0.959,"#font[132]{EM+jet and #gamma+jet avg.}");
      lgfitU->Draw();  lgfit->Draw();

      //Save the plot
      fitName = "./plots/Fcorr/fits/";
      fitName = fitName+"Fcorr_"+(a==0?"Ep_":"pTjet_")+genStr+nameAdd+".eps";
      canvFit->Print(fitName.c_str());

    } //Horizontal axis interpretations

    delete lgfitU;  delete lgfit;  delete h_dum;
  } //if fitMode

  //Free memory
  delete lgc;   delete lgcD0;   delete lgcD0ALT;  delete canvFit;
  for (int f=0; f!=NI; ++f) {
    for (int s=0; s!=Ns; ++s) {
      for (int a=0; a!=NI; ++a) {
        delete g_F[f][s][a];  delete g_Fcorr[f][s][a];  delete fF[f][s][a];
      }
    }
  }
  delete D0Fbcorr;      delete D0Fgcorr;      delete D0Flqcorr;
  delete D0FbcorrALT;   delete D0FgcorrALT;   delete D0FlqcorrALT;
  delete g_D0Fb;        delete g_D0Fg;        delete g_D0Flq;
  delete g_D0Fbcorr;    delete g_D0Fgcorr;    delete g_D0Flqcorr;

} //flavCorr
//------------------------------------------------------------------------------
//A function to combine flavour fractions of several generators in a single plot
void   CMSJES::FFplot() {

  bool gjMode = true;	//true = plot gamma+jet, false = plot EM+jet
  bool bEnr   = true;	//Plot b-enriched samples

  //Retrieve flavour fractions from files
  //Indices: 0 for P6, 1 for H7
  int const nGen = bEnr ? 1:2;
  TH1D* FFa[nGen];  TH1D* FFb[nGen];  TH1D* FFg[nGen];  TH1D* FFlq[nGen];
  TFile* file;  //For opening TFiles

  vector<string> ins;
  if (bEnr) {
    if (gjMode) {
      ins.push_back(
          "CMSJES_P6_gammajet_D0rIIc_R05_ct10mm_b-enriched_1000000_RunIIa.root");
    } else {	//Em+jet (dijet)
      ins.push_back(
          "CMSJES_P6_dijet_D0rIIc_R05_ct10mm_b-enriched_1000000_RunIIa.root");
    }
  } else {
    if (gjMode) {
      ins.push_back("CMSJES_P6_gammajet_D0rIIc_R05_ct10mm_1000000_RunIIa.root");
      ins.push_back("CMSJES_H7_gammajet_D0rIIc_R05_ct10mm_2000000_RunIIa.root");
    } else {	//Em+jet (dijet)
      ins.push_back("CMSJES_P6_dijet_D0rIIc_R05_ct10mm_1000000_RunIIa.root");
      ins.push_back("CMSJES_H7_dijet_D0rIIc_R05_ct10mm_2000000_RunIIa.root");
    }
  }
  string temp;
  if (ins.size()!=nGen) {cout<<"ERROR FFplot: ins dim."<<endl;  return;}
  for (int n=0; n!=nGen; ++n) {
    temp = "./output_ROOT_files/" + ins[n];
    file = TFile::Open(temp.c_str());
    if (!file) {cout << "Error opening files! Exiting" << endl;  return;}
    file->GetObject("FFb",  FFb[ n]);
    file->GetObject("FFg",  FFg[ n]);
    file->GetObject("FFlq", FFlq[n]);
  }

  //Canvas
  TCanvas* FFcanv = new TCanvas("","",400,400);
  FFcanv->SetRightMargin(20);
  FFcanv->SetLogx();

  //Legend
  TLegend* FFlg;
  if (nGen==1) FFlg = new TLegend(0.87, 0.575, 1.00, 0.75);
  else         FFlg = new TLegend(0.87, 0.4,   1.00, 0.75);
  FFlg->SetBorderSize(0);  FFlg->SetFillStyle(0);
  vector<string> lgTitles;
  lgTitles.push_back("#font[132]{P6} #font[12]{b}" );
  lgTitles.push_back("#font[132]{P6} #font[12]{lq}");
  lgTitles.push_back("#font[132]{P6} #font[12]{g}" );
  if (nGen>1) {
    lgTitles.push_back("#font[132]{H7} #font[12]{b}" );
    lgTitles.push_back("#font[132]{H7} #font[12]{lq}");
    lgTitles.push_back("#font[132]{H7} #font[12]{g}" );
  }

  //Stack the flavour fractions together
  string FFstackTitle = ";#font[12]{E'} #font[132]{[GeV]}";
  FFstackTitle       += ";#font[132]{Jet flavour fraction}";
  THStack* FFstack[nGen];
  for (int i=0; i!=nGen; ++i) {
    FFstack[i] = new THStack("",FFstackTitle.c_str());

    //Add the histograms to this generator's stack
    FFstack[i]->Add(FFg[ i]);
    FFstack[i]->Add(FFlq[i]);
    FFstack[i]->Add(FFb[ i]);

    //Add to legend
    FFlg->AddEntry(FFb[ i], lgTitles[i*3+0].c_str(), i==0?"f":"p");
    FFlg->AddEntry(FFlq[i], lgTitles[i*3+1].c_str(), i==0?"f":"p");
    FFlg->AddEntry(FFg[ i], lgTitles[i*3+2].c_str(), i==0?"f":"p");

    //Style setup
    FFb[ i]->SetFillColor(kRed-7 );    FFb[ i]->SetMarkerColor(kRed+1 );
    FFg[ i]->SetFillColor(kGray+1);    FFg[ i]->SetMarkerColor(kGray+3);
    FFlq[i]->SetFillColor(33     );    FFlq[i]->SetMarkerColor(kBlue  );
    FFb[ i]->SetMarkerStyle(kFullDiamond);
    FFg[ i]->SetMarkerStyle(kFullSquare);
    FFlq[i]->SetMarkerStyle(kOpenCircle); 

  }

  //Dummy for setting horizontal axis. THStack cannot be manipulated directly.
  TH1* setup = new TH1D("","",FFb[0]->GetXaxis()->GetNbins(),
                              FFb[0]->GetXaxis()->GetXmin(),
                              FFb[0]->GetXaxis()->GetXmax());
  setup->SetStats(0);
  setup->GetXaxis()->SetMoreLogLabels();
  setup->GetXaxis()->SetNoExponent();
  setup->GetXaxis()->SetTitle(FFb[0]->GetXaxis()->GetTitle());
  setup->GetYaxis()->SetTitle(FFb[0]->GetYaxis()->GetTitle());
  setup->GetYaxis()->SetTitleFont(133);  setup->GetYaxis()->SetTitleSize(18);
  setup->GetXaxis()->SetTitleFont(133);  setup->GetXaxis()->SetTitleSize(18);
  setup->GetXaxis()->SetTitleOffset(0.8);
  setup->GetYaxis()->SetTitleOffset(0.85);
  FFstack[0]->SetHistogram(setup);	//Copy setup's properties to FFstack

  //Draw
  FFstack[0]->Draw("HISTO");
  if (nGen>1) FFstack[1]->Draw("P HISTO SAME");
  FFlg->Draw();

  //Save plot
  string FFcanvName = "./plots/FlavourFraction/FF_P6_H7";
  if (gjMode) FFcanvName += "_gammajet";
  else        FFcanvName += "_dijet";
  if (bEnr)   FFcanvName += "_b-enriched";
  FFcanvName += ".eps";
  FFcanv->Print(FFcanvName.c_str());

  //Free memory
  delete FFcanv;  delete FFlg;
  for (int i=0; i!=nGen; ++i) delete FFstack[i];

} //FFplot

