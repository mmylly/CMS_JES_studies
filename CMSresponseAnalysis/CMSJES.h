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

//#define NIJ //If uncommented, sample must have prtclsnij_ -branches

class CMSJES {
public :
  string ReadName;	//ROOT filename to read
  string OpenName;	// -||- with ".root" -suffix

  /* Uncomment the subdirectory of input_ROOT_files to fetch input from */
  string inSubDir = "";

  TTree* fChain;	//Pointer to the analyzed TTree or TChain
  Int_t  fCurrent;	//Current Tree number in a TChain

  string gjFile="";	//gamma+jet filename compatible with this object
  string djFile="";	//EM+jet -||-
  string zjFile="";	//Z+jet -||-

  string gjFile_b="";	//Same as the two above but...
  string djFile_b="";	//...for b-enriched samples

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
  vector<vector<vector<double>>> params_e;	//electron //3rd index 5 entries
  vector<vector<vector<double>>> params_K;	//Kaon
  vector<vector<vector<double>>> params_KL;	//Kaon_long
  vector<vector<vector<double>>> params_KS;	//Kaon_short
  vector<vector<vector<double>>> params_L;	//Lambda
  vector<vector<vector<double>>> params_mu;	//muon //3rd index 4 entries
  vector<vector<vector<double>>> params_n;	//neutron
  vector<vector<vector<double>>> params_gam;	//photon //3rd index 6 entries
  vector<vector<vector<double>>> params_pi;	//pion
  vector<vector<vector<double>>> params_p;	//proton

  //For storing D0 dijet / EM+jet data points and errors
  static int const nD0data = 10;	//#Data points available from D0
  static int const nCMSdata = 10;	//#Data points available from CMS
  static int const nEpochs = 0;		//#Epochs in run IIa and IIb altogether
  static int const nCMSEpochs = 1;	//#Epochs in CMS data

  //D0 pT-balance data points and errors. dj for dijet, gj for gamma+jet
  double djEpII[nEpochs][nD0data];
  double djD0II[nEpochs][nD0data];
  double djERII[nEpochs][nD0data];
  double gjEpII[nEpochs][nD0data];
  double gjD0II[nEpochs][nD0data];
  double gjERII[nEpochs][nD0data];

  //CMS pT-balance data points and errors zj for Z+jet
  double zjEp[nCMSdata];
  double zjCMS[nCMSdata];
  double zjER[nCMSdata];

  //RunIIa D0 PTB MC
  static int const nD0MC = 13;	//#MC sim. points available from D0
  double djMCEpII[nEpochs][nD0MC];	//Dijet
  double djD0MCII[nEpochs][nD0MC];
  double gjMCEpII[nEpochs][nD0MC];	//Gamma+jet
  double gjD0MCII[nEpochs][nD0MC];
  //D0 gamma+jet MPF MC (run unknown) R_cone=0.7
  static int const nD0MC_MPF_R07=16;	//#MC MPF points R=0.7 from D0
  double gjMCEp_MPF_R07[nD0MC_MPF_R07];
  double gjD0MC_MPF_R07[nD0MC_MPF_R07];
  //D0 gamma+jet MPF data
  static int const nD0_MPF_R07 = 11;  //#MC MPF data points available from D0
  double gjEp_MPF_R07[nD0_MPF_R07];
  double gjD0_MPF_R07[nD0_MPF_R07];
  //D0 gamma+jet MPF ALT MC (run and R_cone unknown)
  static int const nD0MC_MPF=11;	//#MC MPF points from D0
  double gjMCEp_MPF[nD0MC_MPF];
  double gjD0MC_MPF[nD0MC_MPF];
  double djMCEp_MPF[nD0MC_MPF];
  double djD0MC_MPF[nD0MC_MPF];


  // Similar for CMS Z+jet with invented values
  static int const nCMSMC = 13;	
  double zjMCEpII[nCMSMC];	
  double zjCMSMCII[nCMSMC];

  static int const nCMSMC_MPF_R07=16;	//#MC MPF points R=0.7 from CMS
  double zjCMSEp_MPF_R07[nCMSMC_MPF_R07];
  double zjCMSMC_MPF_R07[nCMSMC_MPF_R07];

  static int const nCMS_MPF_R07 = 11;  //#MC MPF data points available from CMS
  double zjEp_MPF_R07[nCMS_MPF_R07];
  double zjCMS_MPF_R07[nCMS_MPF_R07];

  static int const nCMSMC_MPF=11;	//#MC MPF points from D0
  double zjMCEp_MPF[nCMSMC_MPF];
  double zjCMSMC_MPF[nCMSMC_MPF];


  //Flags etc. for changing calculation properties
  int const nABCvars=25;	//#Neighboring points to calculate respFit at
  vector<bool> passedCuts;	//Flags for all evts if they passed cuts before
  unsigned int maxIter=500;	//Maximum #iterations in fitting
  bool verbose = false;		//Print additional information in Loop
  bool bEnrichedFiles = false;	//Use separate files for getting F^b_corr?
  bool contHistos = true;	//Produce probe particle content histograms
  bool MPFmode = false;		//Fit to MPF data? If false, use pT-bal.
  bool runIIa = false; 		//Use runIIa response parameters
  bool runCMS = true;		//Use CMS parameters
  string run = "RunIIb34";	//... epoch to use

  bool StrangeB = true;		//Use Ansätze for strange baryon response
  //string Ansatz = "pn";   	//If StrangeB=true, use p & n  based Ansätze
  //string Ansatz = "L";  	//        -||-         Lambda     -||-
  string Ansatz = "pi";  	//        -||-         pion       -||-

  bool printProg = true;	//Print info on Loop() progress?
  bool useEarlierCuts  = false;	//True if events chosen based on readEvt
  bool useD0ABC        = false;	//Use fit param A,B,C values given in D0 ANs.
  bool useInitGuessABC = false;	//Use initial guess A,B,C; for starting fits
  bool djFitting = false;	//Use EM+jet ("dijet") data points for fitting
  bool gjFitting = false;	//Use gamma+jet data points for fitting
  bool zjFitting = true;	//Use Z+jet data points for fitting
  double epsilon = 0.01;	//Small step to take along gradient
  double epsilonMin = 1e-6;	//Fit converged when step smaller than this
  double A=1,B=0,C=1;		//Fit reco hadron response fit parameters
  double Aer=0,Ber=0,Cer=0;	//Fit parameter uncertainties
  double ABer=0,ACer=0,BCer=0;	//Off-diag. elem.s of covariance matrix
  double E_O[2] = {0.15,0.45};	//Approx. offset energy for CMS, value from D0, does not make sense
  #ifdef NIJ
  bool recoMissing = false;	//Fully reconstruct also particles not in jets
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
  void SetuseD0ABC(  bool flag) {useD0ABC  = flag;}
  void SetdjFitting( bool flag) {djFitting = flag;}
  void SetgjFitting( bool flag) {gjFitting = flag;}
  void SetzjFitting( bool flag) {zjFitting = flag;}

  void SetprintProg( bool flag) {printProg = flag;}
  void SetbEnrichedFiles(bool flag) {bEnrichedFiles=flag;}
  void SetcontHistos(bool flag) {contHistos=flag;}
  void SetMPFmode( bool flag ) {MPFmode = flag;}
  void Setrun(     string val) {run    = val; }
  void SetrunIIa(  bool flag ) {runIIa = flag;}
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
  string Getrun()    {return run;}
  bool GetrunIIa()   {return runIIa;}
  bool GetuseD0ABC()  {return useD0ABC; }
  double Getepsilon()    {return epsilon;}
  double GetepsilonMin() {return epsilonMin;}
  bool GetuseEarlierCuts() {return useEarlierCuts;}
  bool GetdjFitting() {return djFitting;}
  bool GetgjFitting() {return gjFitting;}
  bool GetzjFitting() {return zjFitting;}
  bool GetprintProg() {return printProg;}
  bool GetbEnrichedFiles() {return bEnrichedFiles;}
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
  virtual void     FindFJtoMC();
  virtual void     Plot2D();
  virtual void     StudyTree();
  virtual void     PrintEvt();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
  void   axisSetupFJtoMC(TProfile2D* FJtoMC, string titleAdd);
  void   FitGN();		//Gauss-Newton fit function
  void   MultiLoop(CMSJES* dj_in=NULL, CMSJES* gj_in=NULL, bool fitPars=true);
  void   plotPT(int gen=0,int alg=0,int rad=0,int ct=-1,int Nevt=0,int run=0,
                int P=0,  int XS=0, bool MConly=false, bool fitOnly=false    );
  void   plotSepPT();
  void   plotMPF(int gen=0,  int alg=0, int rad=0, int ct=-1,
                 int Nevt=0, int run=0, int P=0,   int XS=0  );
  void   Response(int id, double pseudorap, double energy,   double pT,
	          TF1* frE, TF1* frMU, TF1* frG, TF1* frH, bool pos,
                  double fA, double fB, double fC, bool MC, bool FIT, bool EM,
                  vector<double>& retMC, vector<double>& retFIT,
                  vector<double>& retEM                                       );
  void   ParamReader(string file, int n1, int n2, int n3,
                     vector<vector<vector<double>>> &params);
  void   flavCorr(bool plot=true, int gen=0, int alg=0,
                  int rad=0,int ct=-1,int Nevt=0,int run=0,int P=0,int XS=0);
  void   FFplot();
  void   plotQuery(string& respStr, string& djstr,  string& gjstr,
                    string& zjstr, string& djstrb, string& gjstrb,
                   int& gen,  int& alg,  int& rad,  int& ct,
                   int& Nevt, int& run,  int& P,    int& XS        );
  bool   fidCuts(int id, double pT);
  bool   isNeutrino(int id);  //PDGID is for Neutrino
  bool   isStrangeB(int id);  //Check if PDGID is for Xi, Sigma or Omega^-
};

#endif

#ifdef CMSJES_cxx

//A function to read hadron response function parameters from files
//Params:	file		The filename to read as a string
//		n1,n2,n3	Dimensions of the params tensor
//		params		Reference to the tensor to read parameters into

void CMSJES::ParamReader(string file, int n1, int n2, int n3,
                        vector<vector<vector<double>>> &params)
{
  //Init temps to read into
  double p=0;
  vector<double> v;
  vector<vector<double>> M;		//Temp matrix
  vector<vector<vector<double>>> T;	//Temp tensor
  //Read the parameters from files
  ifstream in;
  string paramFile;
  paramFile = "./spr_mc/" + (runIIa ? "RunIIa" : run) + file; 

  in.open(paramFile);
  if (!in.is_open()) {
    cout << "Error opening " << paramFile << endl;
    return;
  }
  for (int lines=0; lines!=n2; ++lines) {
    in >> p;		//1st string on line is eta region low. bd., omit it
    for (int i=0; i!=n3; ++i) {
      in >> p;  v.push_back(p);
    }
    M.push_back(v);
    v.clear();
  }
  T.push_back(M);
  M.clear();
  in.close();
  
  params = T;
  T.clear();

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
      files.push_back("P6_gammajet_D0rIIc_R05_ct10mm_30000");

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

  ifstream inPTBdata_dj_a;   ifstream inPTBMC_dj_a;   //RunIIa dijet
  ifstream inPTBdata_gj_a;   ifstream inPTBMC_gj_a;   // -||-  gammajet
  ifstream inMPFdata_gj;     ifstream inMPFMC_gj;     //MPF gammajet, run NA
  ifstream inMPFdata_dj;     ifstream inMPFMC_dj;
  ifstream inMPFdata_gj_R07; ifstream inMPFMC_gj_R07;
  //RunIIa pT-balance
  inPTBdata_dj_a.open("./data_and_MC_input/pTbal/runIIa/dijet_data");
  inPTBdata_gj_a.open("./data_and_MC_input/pTbal/runIIa/gammajet_data");
  inPTBMC_dj_a.open(  "./data_and_MC_input/pTbal/runIIa/dijet_MC");
  inPTBMC_gj_a.open(  "./data_and_MC_input/pTbal/runIIa/gammajet_MC");
  //MPF
  inMPFdata_gj_R07.open("./data_and_MC_input/MPF/MPF_gammajet_data_R07");
  inMPFMC_gj_R07.open("./data_and_MC_input/MPF/MPF_gammajet_MC_R07");
  inMPFMC_dj.open("./data_and_MC_input/MPF/MPF_dijet_MC");
  inMPFMC_gj.open("./data_and_MC_input/MPF/MPF_gammajet_MC");
	//Whe D0 MPF detector data and MC points for different run epochs found,
	//support for them can be added here as above

  if (!inPTBdata_dj_a.is_open()   || !inPTBMC_dj_a.is_open()   ||
      !inPTBdata_gj_a.is_open()   || !inPTBMC_gj_a.is_open()   ||
      !inMPFMC_dj.is_open()       || !inMPFMC_gj.is_open()     ||
      !inMPFdata_gj_R07.is_open() || !inMPFMC_gj_R07.is_open()   )
  {
    cout << "Error opening D0 data/MC point files!" << endl;
    return;
  }
  
  for (int step=0; step != nD0data; ++step) {	//D0 pT-balance data
    inPTBdata_dj_a   >> djEpII[0][step] >> djD0II[0][step] >> djERII[0][step];
    inPTBdata_gj_a   >> gjEpII[0][step] >> gjD0II[0][step] >> gjERII[0][step];
  }
  for (int step=0; step != nD0MC; ++step) {	//D0 pT-balance MC points
    inPTBMC_dj_a   >> djMCEpII[0][step] >> djD0MCII[0][step];
    inPTBMC_gj_a   >> gjMCEpII[0][step] >> gjD0MCII[0][step];
  }
  for (int step=0; step != nD0_MPF_R07; ++step) {	//MPF data points
    inMPFdata_gj_R07 >> gjEp_MPF_R07[step] >> gjD0_MPF_R07[step];
  }
  for (int step=0; step != nD0MC_MPF_R07; ++step) {	//MPF MC points
    inMPFMC_gj_R07 >> gjMCEp_MPF_R07[step] >> gjD0MC_MPF_R07[step];
  }
  for (int step=0; step != nD0MC_MPF; ++step) {	//MPF MC points
    inMPFMC_gj >> gjMCEp_MPF[step] >> gjD0MC_MPF[step];
    inMPFMC_dj >> djMCEp_MPF[step] >> djD0MC_MPF[step];
  }

  //Close the D0 MC / data point files
  inPTBdata_dj_a.close();    
  inPTBdata_gj_a.close();    
  inPTBMC_dj_a.close();      
  inPTBMC_gj_a.close();      
  inMPFdata_gj_R07.close();  inMPFMC_gj_R07.close();
  inMPFMC_dj.close();        inMPFMC_gj.close();

  /* Read params from files to matrices */

  //Open file and initializations for reading
  ifstream in0;	//RunIIa
  int line = 0;	//Stepper

  //Init dummies to read into
  double ap0=0,ap1=0,ap2=0,ap3=0,ap4=0,ap5=0;
  double bp0=0,bp1=0,bp2=0,bp3=0,bp4=0,bp5=0;

  //Param reading works the same way for all hadrons
  ParamReader("/photon.txt",   1, 32, 6, params_gam);
  ParamReader("/electron.txt", 1, 32, 5, params_e  );
  ParamReader("/muon.txt",     1, 32, 4, params_mu );
  ParamReader("/kaon.txt",     1, 32, 3, params_K  );
  ParamReader("/klong.txt",    1, 32, 3, params_KL );
  ParamReader("/kshort.txt",   1, 32, 3, params_KS );
  ParamReader("/lambda.txt",   1, 32, 3, params_L  );
  ParamReader("/neutron.txt",  1, 32, 3, params_n  );
  ParamReader("/pion.txt",     1, 32, 3, params_pi );
  ParamReader("/proton.txt",   1, 32, 3, params_p  );

  /* Plug fit parameter values here for fit reco */
  if (runIIa) {
    //D0 runIIa 0<|eta|0.4 fit params
    if      (useD0ABC       ) {A  =1.409;     B  =0.0017;      C  =0.9973;
                               Aer=0.020468;  Ber=0.00710429;  Cer=0.0929739;}
    //Initial guess to start fitting from
    else if (useInitGuessABC) {A   = 1.4;     B   = 0.0;       C = 1.0;
                               Aer = 0.0;     Ber = 0.0;       Cer = 0.0;    }
    //Default: use our A,B,C depending on generator
    else {
      if (ReadName.find("P6")!=string::npos) {
        A    = 1.45399;	B    = 0.0115405;	C    = 1.00865;
        Aer  = 0.0572361;	Ber  = 0.0287636;	Cer  = 0.0105337;
        ABer = -0.00148885;	ACer = -0.000124997;	BCer = 0.000180898;
      } else if (ReadName.find("H7")!=string::npos) {
        A    = 1.23482;	B    = 0.0333262;	C    = 1.0047;
        Aer  = 0.0559627;	Ber  = 0.0321562;	Cer  = 0.0105726;
        ABer = -0.00160909;	ACer = -7.75965e-05;	BCer = 0.00018794;     
      } else cout << "\nWARNING: unknown fit parameters!\n" << endl;
    }
  } else if (runCMS) {
    if      (useD0ABC       ) {A  =1.409;     B  =0.0017;      C  =0.9973;
                               Aer=0.020468;  Ber=0.00710429;  Cer=0.0929739;}
    //Initial guess to start fitting from
    else if (useInitGuessABC) {A   = 1.4;     B   = 0.0;       C = 1.0;
                               Aer = 0.0;     Ber = 0.0;       Cer = 0.0;    }
    //Default: use our A,B,C depending on generator
    else {
      if (ReadName.find("P8")!=string::npos) {
        A    = 1.45399;	B    = 0.0115405;	C    = 1.00865;
        Aer  = 0.0572361;	Ber  = 0.0287636;	Cer  = 0.0105337;
        ABer = -0.00148885;	ACer = -0.000124997;	BCer = 0.000180898;
        cout << "\nCMS with Pythia 8 parameters chosen\n" << endl;
      } else cout << "\nWARNING: unknown fit parameters!\n" << endl;
    }
  } else cout << "\n\n\tERROR: neither run IIa nor runCMS activated!\n\n" << endl;
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
