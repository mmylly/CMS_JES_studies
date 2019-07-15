////////////////////////////////////////////////////////////////
// This class has been automatically generated on             //
// Thu Feb  1 12:21:30 2018 by ROOT version 6.13/01           //
// from TTree Pythia8Jets/Pythia8 particle data               //
// found on files: P6_dijet_*.root, H7_dijet_*.root, etc.     //
//                                                            //
// The code has been modified further along the way by        //
// toni.makela@cern.ch of Helsinki Institute of Physics, 2018 //
////////////////////////////////////////////////////////////////

#ifndef CMSJES_h
#define CMSJES_h

// C/C++
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ios>
#include <pthread.h>
#include <time.h>
#include <sstream>

// Header file for the classes stored in the TTree if any.
#include <vector>

// ROOT
#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TMatrix.h"
#include "TMatrixD.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TThread.h"
#include "TVector.h"
#include "TVector3.h"
#include "TVectorD.h"
#include "Riostream.h"

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::fstream;
using std::ifstream;
using std::max;
using std::min;
using std::ofstream;
using std::stringstream;

#define NIJ //If uncommented, sample must have prtclsnij_ -branches

class CMSJES {
public :
  string ReadName;	//ROOT filename to read
  string OpenName;	// -||- with ".root" -suffix

  /* Uncomment the subdirectory of input_ROOT_files to fetch input from */
  string inSubDir = "";

  TTree* fChain;	//Pointer to the analyzed TTree or TChain
  Int_t  fCurrent;	//Current Tree number in a TChain

  string zjFile="";	//Z+jet -||-

  // Fixed size dimensions of array or collections stored in the TTree if any.
  // Declaration of leaf types
  Float_t         weight;		// Event weight
  //Particle lvl
  vector<unsigned char> *prtcl_jet;	// Jet which particle belongs to 0 - highest pT
  vector<int>     *prtcl_pdgid;		// Particle ID - anti
  vector<float>   *prtcl_pt;		// Particle pT
  vector<float>   *prtcl_eta;		// Pseudorapidity
  vector<float>   *prtcl_phi;		// Phi
  vector<float>   *prtcl_e;		// energy
  //Particles not associated with jets
  #ifdef NIJ
  vector<int>     *prtclnij_pdgid;
  vector<float>   *prtclnij_pt;
  vector<float>   *prtclnij_eta;
  vector<float>   *prtclnij_phi;
  vector<float>   *prtclnij_e;
  #endif
  //Parton lvl - used for jet tagging and 
  vector<char>    *prtn_jet;		// Jet originated parton?
  vector<int>     *prtn_pdgid;		// Parton ID
  vector<char>    *prtn_tag;		// 
  vector<float>   *prtn_pt;		// pT of the parton
  vector<float>   *prtn_eta;		// -- 
  vector<float>   *prtn_phi;		// --
  vector<float>   *prtn_e;		//
  vector<float>   *jet_pt;		// Gen jets
  vector<float>   *jet_eta;		//
  vector<float>   *jet_phi;		//
  vector<float>   *jet_e;		//
  Float_t         met;			// Neutrino E_T sum not used

  // List of branches
  //Particle lvl
  TBranch        *b_weight;
  TBranch        *b_prtcl_jet;
  TBranch        *b_prtcl_pdgid;
  TBranch        *b_prtcl_pt;
  TBranch        *b_prtcl_eta;
  TBranch        *b_prtcl_phi;
  TBranch        *b_prtcl_e;
  //Particles not in jets
  #ifdef NIJ
  TBranch        *b_prtclnij_pdgid;
  TBranch        *b_prtclnij_pt;
  TBranch        *b_prtclnij_eta;
  TBranch        *b_prtclnij_phi;
  TBranch        *b_prtclnij_e;
  #endif
  //Parton lvl
  TBranch        *b_prtn_jet;
  TBranch        *b_prtn_pdgid;
  TBranch        *b_prtn_tag;
  TBranch        *b_prtn_pt;
  TBranch        *b_prtn_eta;
  TBranch        *b_prtn_phi;
  TBranch        *b_prtn_e;
  //Jet lvl
  TBranch        *b_jet_pt;
  TBranch        *b_jet_eta;
  TBranch        *b_jet_phi;
  TBranch        *b_jet_e;
  TBranch        *b_met;

  double R_cone;//Jet algorithm cone radius

  //Tensors to contain response function parameters
  //  1st index: |eta| region. 32 entries for this index
  //  2nd index: the parameters p^(i)_{particle type} in the SPR functions
  //             3rd index has 3 entries unless otherwise stated
  vector<vector<double>> params_e;	//electron //3rd index 5 entries
  vector<vector<double>> params_K;	//Kaon
  vector<vector<double>> params_KL;	//Kaon_long
  vector<vector<double>> params_KS;	//Kaon_short
  vector<vector<double>> params_L;	//Lambda
  vector<vector<double>> params_mu;	//muon //3rd index 4 entries
  vector<vector<double>> params_n;	//neutron
  vector<vector<double>> params_gam;	//photon //3rd index 6 entries
  vector<vector<double>> params_pi;	//pion
  vector<vector<double>> params_p;	//proton


  //CMS jecsys pT-balance data points and errors zj for Z+jet
  static int const nCMSdata = 6; //#Data points available from CMS
  double zjEp[nCMSdata];
  double zjCMS[nCMSdata];
  double zjER[nCMSdata];

  //CMS jecsys MPF data points and errors for Z+jet
  static int const nCMSdata_MPF = 11;
  double zjEp_MPF[nCMSdata_MPF];
  double zjCMS_MPF[nCMSdata_MPF];
  double zjER_MPF[nCMSdata_MPF];

  //CMS MC Z+jet with invented values
  static int const nCMSMC = 6;	
  double zjMCEp[nCMSMC];	
  double zjCMSMC[nCMSMC];
  double zjCMSMCER[nCMSMC];

  //CMS jecsys MPF MC points and errors for Z+jet
  static int const nCMSMC_MPF = 11;
  double zjMCEp_MPF[nCMSMC_MPF];
  double zjCMSMC_MPF[nCMSMC_MPF];
  double zjCMSMCER_MPF[nCMSMC_MPF];



  //Flags etc. for changing calculation properties
  vector<bool> passedCuts;	//Flags for all evts if they passed cuts before
  unsigned int maxIter=500;	//Maximum #iterations in fitting
  bool verbose = false;		//Print additional information in Loop
  bool contHistos = true;	//Produce probe particle content histograms
  bool MPFmode = false;		//Fit to MPF data? If false, use pT-bal.
  bool runCMS = true;		//Use CMS parameters

  bool StrangeB = true;		//Use Ansätze for strange baryon response
  //string Ansatz = "pn";   	//If StrangeB=true, use p & n  based Ansätze
  //string Ansatz = "L";  	//        -||-         Lambda     -||-
  string Ansatz = "pi";  	//        -||-         pion       -||-

  bool printProg = true;	//Print info on Loop() progress?
  bool useEarlierCuts  = false;	//True if events chosen based on readEvt
  bool useInitGuessABC = false;	//Use initial guess A,B,C; for starting fits
  bool zjFitting = true;	//Use Z+jet data points for fitting
  double epsilon = 0.01;	//Small step to take along gradient
  double epsilonMin = 1e-6;	//Fit converged when step smaller than this
  double A=1,B=0,C=1;		//Fit reco hadron response fit parameters
  double Aer=0,Ber=0,Cer=0;	//Fit parameter uncertainties
  double ABer=0,ACer=0,BCer=0;	//Off-diag. elem.s of covariance matrix

  #ifdef NIJ
  bool recoMissing = true;	//Fully reconstruct also particles not in jets
  #endif

  //Setters
  void SetA(double val) {A = val;}
  void SetB(double val) {B = val;}
  void SetC(double val) {C = val;}
  void SetABC(double Ain,double Bin,double Cin) {A=Ain;  B=Bin;  C=Cin;}
  void SetAer( double val) {Aer  = val;}
  void SetBer( double val) {Ber  = val;}
  void SetCer( double val) {Cer  = val;}
  void SetABer(double val) {ABer = val;}
  void SetACer(double val) {ACer = val;}
  void SetBCer(double val) {BCer = val;}
  void SetABCer(double Ain,double Bin,double Cin) {Aer=Ain;  Ber=Bin;  Cer=Cin;}

  void Setepsilon(   double val) {epsilon = val;}
  void SetepsilonMin(double val) {epsilonMin = val;}
  void SetuseEarlierCuts(bool flag) {useEarlierCuts = flag;}
  void SetzjFitting( bool flag) {zjFitting = flag;}
  void SetrunCMS( bool flag)    {runCMS = flag;}
  void SetprintProg( bool flag) {printProg = flag;}
  void SetcontHistos(bool flag) {contHistos=flag;}
  void SetMPFmode( bool flag ) {MPFmode = flag;}
  void SetStrangeB(bool flag ) {StrangeB = flag;}
  void SetAnsatz(  string val) {
   if (val=="pn" || val=="L" || val=="pi") Ansatz = val;
   else cout << "SetAnsatz: value "<< val << " unknown, wont apply" << endl;
  }
  #ifdef NIJ
  void SetrecoMissing(bool flag) {recoMissing = flag;}
  #endif
  void SetmaxIter(unsigned int val) {maxIter = val;}
  void Setverbose(bool flag) {verbose = flag;}

  //Getters
  double GetA() {return A;}
  double GetB() {return B;}
  double GetC() {return C;}
  double GetAer() {return Aer;}
  double GetBer() {return Ber;}
  double GetCer() {return Cer;}
  double GetABer() {return ABer;}
  double GetACer() {return ACer;}
  double GetBCer() {return BCer;}
  double Getepsilon()    {return epsilon;}
  double GetepsilonMin() {return epsilonMin;}
  bool GetuseEarlierCuts() {return useEarlierCuts;}
  bool GetzjFitting() {return zjFitting;}
  bool GetrunCMS() {return runCMS;}
  bool GetprintProg() {return printProg;}
  bool GetcontHistos() {return contHistos;}
  bool GetMPFmode()  {return MPFmode;}
  bool GetStrangeB() {return StrangeB;}
  string GetAnsatz() {return Ansatz;}
  #ifdef NIJ
  bool GetrecoMissing() {return recoMissing;}
  #endif
  unsigned int GetmaxIter() {return maxIter;}
  bool Getverbose() {return verbose;}

  //Constructor and destructor
  CMSJES(TTree *tree=0, string="");
  virtual ~CMSJES();

  //ROOT TTree specific functions
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);

  //Other memeber functions
  virtual void     InputNameConstructor();
  virtual void     Loop();
  virtual void     Plot2D();
  virtual void     StudyTree();
  virtual void     PrintEvt();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
  void   FitGN();		//Gauss-Newton fit function
  void   MultiLoop(CMSJES* zj_in=NULL, bool fitPars=true); //Only for Z+jet at the moment
  void   plotPT(int gen=0,int Nevt=-1, bool MConly=false, bool fitOnly=false);
  void   plotSepPT();
  void   plotMPF(int gen=0, int Nevt=-1);
  void   Response(int id, double pseudorap, double energy,   double pT,
	          TF1* frE, TF1* frMU, TF1* frG, TF1* frH, bool pos,
                  double fA, double fB, double fC, bool MC, bool FIT, bool EM,
                  double& retMC, double& retFIT, double& retEM);
  void   ParamReader(string file, int n1, int n2, vector<vector<double>> &params);
  void   flavCorr(bool plot=true, int gen=0, int Nevt=-1);
  void   plotQuery(string& respStr, string& zjstr, int& gen, int& Nevt);
  bool   fidCuts(int id, double pT);
  bool   isNeutrino(int id);  //PDGID is for Neutrino
  bool   isStrangeB(int id);  //Check if PDGID is for Xi, Sigma or Omega^-
};

#endif

#ifdef CMSJES_cxx

//A function to read hadron response function parameters from files
//Params:	file		The filename to read as a string
//		n1,n2		Dimensions of the params tensor
//		params		Reference to the tensor to read parameters into
void CMSJES::ParamReader(string file, int n1, int n2,
                        vector<vector<double>> &params)
{
  //Init temps to read into
  double p=0;
  vector<double> v;
  vector<vector<double>> M;		//Temp matrix
  //Read the parameters from files
  ifstream in;
  string paramFile;
  paramFile = "./spr_mc/RunIIa" + file; 

  in.open(paramFile);
  if (!in.is_open()) {
    cout << "Error opening " << paramFile << endl;
    return;
  }
  for (int lines=0; lines!=n1; ++lines) {
    in >> p;		//1st string on line is eta region low. bd., omit it
    for (int i=0; i!=n2; ++i) {
      in >> p;  v.push_back(p);
    }
    M.push_back(v);
    v.clear();
  }

  params = M;
  M.clear();
  in.close();
} //ParamReader


//Constructor
CMSJES::CMSJES(TTree *tree, string toRead) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  if (tree == 0) {	//No pointer to a tree given
    if (toRead != "") ReadName = toRead;
    else {
      //Add here all the files one should be able to study
      vector<string> files;	//To contain studiable filenames
      files.push_back("P8_Zjet_1000"  );
      files.push_back("P8_Zjet_10000" );
      files.push_back("P8_Zjet_100000");
      files.push_back("P8_Zjet_500000");
      files.push_back("P8_Zjet_3000");
      files.push_back("P8_Zjet_30000");
      files.push_back("P8_Zjet_300000");

      //User interface
      printf("No filename given, choose file (y = default file):\n");
      for (int i=0; i!=files.size(); ++i) {
        if (i%4==0) printf("\n");
        printf("%2d:%56s\n", i, files[i].c_str());
      }
      printf("Want something else? Add it to the constructor in header\n");
      //Read input
      int input;	//For cin
      cin >> input;
      if (input < 0 || input >= files.size()) {	//Check for erroneous input
	cout<< "Invalid input! Chose 0" <<endl;
        input = 0;
      }
      ReadName = files[input];			//Pick the filename to study
    }
    OpenName = "./input_ROOT_files/" + inSubDir + ReadName + ".root";
    printf("Opening file: %s\n", OpenName.c_str());
    TFile *f = (TFile*)gROOT->GetListOfFiles()
			    ->FindObject(OpenName.c_str());
    if (!f || !f->IsOpen()) f = new TFile(OpenName.c_str());
    if (ReadName.find("P6")!=string::npos) f->GetObject("Pythia6Jets",tree);
    else if (ReadName.find("P8")!=string::npos) f->GetObject("Pythia8Jets",tree);
    else f->GetObject("HerwigTree",tree);

  }
  Init(tree);	//Setup branch adresses etc.

  /* Read D0 data and MC points */
  ifstream inPTBdata_zj;     ifstream inPTBMC_zj;     //CMS jecdata pT Z+jet
  ifstream inMPFdata_zj;     ifstream inMPFMC_zj;     //CMS jecdata MPF  Z+jet

  //CMS jecdataGH pT-balance
  inPTBdata_zj.open("./data_and_MC_input/pTbal/jecdataGH/zmmjet_data");
  inPTBMC_zj.open(  "./data_and_MC_input/pTbal/jecdataGH/zmmjet_mc");

  //CMS jecdataGH MPF 
  inMPFdata_zj.open("./data_and_MC_input/MPF/jecdataGH/MPF_zmmjet_data");
  inMPFMC_zj.open("./data_and_MC_input/MPF/jecdataGH/MPF_zmmjet_mc");

  if (!inPTBdata_zj.is_open() || !inPTBMC_zj.is_open() ||
      !inMPFdata_zj.is_open() || !inMPFMC_zj.is_open()  )
  {
    cout << "Error opening D0/CMS data/MC point files!" << endl;
    return;
  }
  
  for (int step=0; step != nCMSdata; ++step) {	//CMS pT-balance data
    inPTBdata_zj   >> zjEp[step] >> zjCMS[step] >> zjER[step];
  }
  for (int step=0; step != nCMSMC; ++step) {	//CMS pT-balance MC points
    inPTBMC_zj   >> zjMCEp[step] >> zjCMSMC[step] >> zjCMSMCER[step];
  }
  for (int step=0; step != nCMSdata_MPF; ++step) {	//CMS jecsys MPF data
    inMPFdata_zj   >> zjEp_MPF[step] >> zjCMS_MPF[step] >> zjER_MPF[step];
  }
  for (int step=0; step != nCMSMC_MPF; ++step) {	//CMS jecsys MPF MC points
    inMPFMC_zj   >> zjMCEp_MPF[step] >> zjCMSMC_MPF[step] >> zjCMSMCER_MPF[step];
  }

  //Close the CMS MC / data point files
  inPTBdata_zj.close(); 
  inMPFdata_zj.close();
  inPTBMC_zj.close();
  inMPFMC_zj.close(); 

  /* Read params from files to matrices */

  //Open file and initializations for reading
  int line = 0;	//Stepper

  //Param reading works the same way for all hadrons
  ParamReader("/photon.txt",   32, 6, params_gam);
  ParamReader("/electron.txt", 32, 5, params_e  );
  ParamReader("/muon.txt",     32, 4, params_mu );
  ParamReader("/kaon.txt",     32, 3, params_K  );
  ParamReader("/klong.txt",    32, 3, params_KL );
  ParamReader("/kshort.txt",   32, 3, params_KS );
  ParamReader("/lambda.txt",   32, 3, params_L  );
  ParamReader("/neutron.txt",  32, 3, params_n  );
  ParamReader("/pion.txt",     32, 3, params_pi );
  ParamReader("/proton.txt",   32, 3, params_p  );

  /* Plug fit parameter values here for fit reco */
  if (runCMS) {
    //Initial guess to start fitting from
    if (useInitGuessABC) {A   = 1.4;     B   = 0.0;       C   = 1.0;
                          Aer = 0.0;     Ber = 0.0;       Cer = 0.0;}
    //Default: use our A,B,C depending on generator
    else {
      if (ReadName.find("P8")!=string::npos) {
        // With 10k sample and 500 loops
	//A    = -29.8281;	B    = -1.70343;	C    =  0.959501;
	//Aer  =   0.0529828;	Ber  =  0.0558275;	Cer  =  0.00833123;
	ABer =  -0.00292137;	ACer =  0.00016457;	BCer = -0.000238565;
	// With 100k sample and 20 loops
	//A   = -27.8065;		B    = -0.661071;	C    = 0.868749; 
        //Aer  = 0.00593366;	Ber  =  0.00636191;	Cer  =  0.00918395;
        //ABer = 0.0000134886;	ACer = -0.0000365345;	BCer = -0.0000543319;
        // With 30k sample 269 loops
	A    = 0.0733748;	B    = 0.36875;		C    =  0.968427;
	Aer  = 0.375176;	Ber  = 0.0960518;	Cer  =  0.0104257;
        cout << "\nCMS with Pythia 8 parameters chosen\n" << endl;
      } else cout << "\nWARNING: unknown fit parameters!\n" << endl;
    }
  }
} //Constructor

//-----------------------------------------------------------------------------
//Destructor
CMSJES::~CMSJES()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}
//-----------------------------------------------------------------------------
Int_t CMSJES::GetEntry(Long64_t entry)
{
// Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
//-----------------------------------------------------------------------------
Long64_t CMSJES::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
     fCurrent = fChain->GetTreeNumber();
     Notify();
  }
  return centry;
}
//-----------------------------------------------------------------------------
void CMSJES::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set object pointers initially to NULL
  //Particle lvl
  prtcl_jet = 0;
  prtcl_pdgid = 0;
  prtcl_pt = 0;
  prtcl_eta = 0;
  prtcl_phi = 0;
  prtcl_e = 0;
  //Particles not in jets
  #ifdef NIJ
  prtclnij_pdgid = 0;
  prtclnij_pt = 0;
  prtclnij_eta = 0;
  prtclnij_phi = 0;
  prtclnij_e = 0;
  #endif
  //Parton lvl
  prtn_jet = 0;
  prtn_pdgid = 0;
  prtn_tag = 0;
  prtn_pt = 0;
  prtn_eta = 0;
  prtn_phi = 0;
  prtn_e = 0;
  //Jet lvl
  jet_pt = 0;
  jet_eta = 0;
  jet_phi = 0;
  jet_e = 0;
  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("weight", &weight, &b_weight);	//Event weight
  //Particle lvl  
  fChain->SetBranchAddress("prtcl_jet", &prtcl_jet, &b_prtcl_jet);
  fChain->SetBranchAddress("prtcl_pdgid", &prtcl_pdgid, &b_prtcl_pdgid);
  fChain->SetBranchAddress("prtcl_pt", &prtcl_pt, &b_prtcl_pt);
  fChain->SetBranchAddress("prtcl_eta", &prtcl_eta, &b_prtcl_eta);
  fChain->SetBranchAddress("prtcl_phi", &prtcl_phi, &b_prtcl_phi);
  fChain->SetBranchAddress("prtcl_e", &prtcl_e, &b_prtcl_e);
  //Particles not in jets lvl
  #ifdef NIJ
  fChain->SetBranchAddress("prtclnij_pdgid",&prtclnij_pdgid, &b_prtclnij_pdgid);
  fChain->SetBranchAddress("prtclnij_pt", &prtclnij_pt, &b_prtclnij_pt);
  fChain->SetBranchAddress("prtclnij_eta", &prtclnij_eta, &b_prtclnij_eta);
  fChain->SetBranchAddress("prtclnij_phi", &prtclnij_phi, &b_prtclnij_phi);
  fChain->SetBranchAddress("prtclnij_e", &prtclnij_e, &b_prtclnij_e);
  #endif
  //Parton lvl
  fChain->SetBranchAddress("prtn_jet", &prtn_jet, &b_prtn_jet);
  fChain->SetBranchAddress("prtn_pdgid", &prtn_pdgid, &b_prtn_pdgid);
  fChain->SetBranchAddress("prtn_tag", &prtn_tag, &b_prtn_tag);
  fChain->SetBranchAddress("prtn_pt", &prtn_pt, &b_prtn_pt);
  fChain->SetBranchAddress("prtn_eta", &prtn_eta, &b_prtn_eta);
  fChain->SetBranchAddress("prtn_phi", &prtn_phi, &b_prtn_phi);
  fChain->SetBranchAddress("prtn_e", &prtn_e, &b_prtn_e);
  //Jet lvl
  fChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
  fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
  fChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
  fChain->SetBranchAddress("jet_e", &jet_e, &b_jet_e);
  fChain->SetBranchAddress("met", &met, &b_met);
  Notify();
}
//-----------------------------------------------------------------------------
Bool_t CMSJES::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}
//-----------------------------------------------------------------------------
void CMSJES::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
//-----------------------------------------------------------------------------
Int_t CMSJES::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
  return 1;
}
#endif // #ifdef CMSJES_cxx
