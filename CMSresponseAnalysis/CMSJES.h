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
  vector<int>     *prtclnij_pdgid;
  vector<float>   *prtclnij_pt;
  vector<float>   *prtclnij_eta;
  vector<float>   *prtclnij_phi;
  vector<float>   *prtclnij_e;
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
  TBranch        *b_prtclnij_pdgid;
  TBranch        *b_prtclnij_pt;
  TBranch        *b_prtclnij_eta;
  TBranch        *b_prtclnij_phi;
  TBranch        *b_prtclnij_e;
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


  //EHE and HHe responses for certain particles
  vector<vector<double>> params_pi_EHE; //pion
  vector<vector<double>> params_pi_HHe;
  vector<vector<double>> params_an_EHE; //antineutron
  vector<vector<double>> params_an_HHe;
  vector<vector<double>> params_ap_EHE; //antiproton
  vector<vector<double>> params_ap_HHe;
  vector<vector<double>> params_K0L_EHE; //K0L
  vector<vector<double>> params_K0L_HHe;
  vector<vector<double>> params_K0S_EHE; //K0S
  vector<vector<double>> params_K0S_HHe;
  vector<vector<double>> params_Km_EHE; //Km
  vector<vector<double>> params_Km_HHe;
  vector<vector<double>> params_Kp_EHE; //Kp
  vector<vector<double>> params_Kp_HHe;
  vector<vector<double>> params_n_EHE; //neutron
  vector<vector<double>> params_n_HHe;
  vector<vector<double>> params_p_EHE; //proton
  vector<vector<double>> params_p_HHe;


  //Params for hadron groups and combined H + EH response
  vector<vector<double>> params_cat1;
  vector<vector<double>> params_cat2;
  vector<vector<double>> params_cat3;

  //CMS jecsys pT-balance data points and errors zj for Z+jet
  static int const ndata_pTbal = 10; //#Data points available from CMS
  double zj_pTp[ndata_pTbal];
  double zj_pTbal[ndata_pTbal];
  double zj_pTbal_ER[ndata_pTbal];

  //CMS jecsys pT-balance MC points
  static int const nMC_pTbal = 10;	
  double zj_MC_pTp[nMC_pTbal];	
  double zj_MC_pTbal[nMC_pTbal];
  double zj_MC_pTbal_ER[nMC_pTbal];

  //CMS jecsys MPF data points and errors for Z+jet
  static int const ndata_MPF = 11;
  double zj_pTp_MPF[ndata_MPF];
  double zj_MPF[ndata_MPF];
  double zj_MPF_ER[ndata_MPF];

  //CMS jecsys MPF MC points and errors for Z+jet
  static int const nMC_MPF = 11;
  double zj_MC_pTp_MPF[nMC_MPF];
  double zj_MC_MPF[nMC_MPF];
  double zj_MC_MPF_ER[nMC_MPF];

  //CMS jecsys MPF-notypeI data points and errors for Z+jet
  static int const ndata_MPFntI = 14; //14 points but ignore last ones maybe since large err.?
  double zj_pTp_MPFntI[ndata_MPFntI];
  double zj_MPFntI[ndata_MPFntI];
  double zj_MPFntI_ER[ndata_MPFntI];

  //CMS jecsys MPF-notypeI MC points and errors for Z+jet
  static int const nMC_MPFntI = 14;
  double zj_MC_pTp_MPFntI[nMC_MPFntI];
  double zj_MC_MPFntI[nMC_MPFntI];
  double zj_MC_MPFntI_ER[nMC_MPFntI];

  //CMS 2018 MPF-notypeI MC points and errors for Z+jet
  double zj_MC_pTp_MPFntI_2018[nMC_MPFntI];
  double zj_MC_MPFntI_2018[nMC_MPFntI];
  double zj_MC_MPFntI_ER_2018[nMC_MPFntI];


  //Flags etc. for changing calculation properties
  vector<bool> passedCuts;	//Flags for all evts if they passed cuts before
  bool contHistos = true;	//Produce probe particle content histograms
  bool MPFmode = false;		//Fit to MPF data? If false, use pT-bal.
  bool runCMS = true;		//Use CMS parameters
  bool printProg = true;	//Print info on Loop() progress?
  bool useEarlierCuts  = false;	//True if events chosen based on readEvt
  bool zjFitting = true;	//Use Z+jet data points for fitting

  bool recoMissing = true;	//Fully reconstruct also particles not in jets

  void SetuseEarlierCuts(bool flag) {useEarlierCuts = flag;}
  void SetzjFitting( bool flag) {zjFitting = flag;}
  void SetrunCMS( bool flag)    {runCMS = flag;}
  void SetprintProg( bool flag) {printProg = flag;}
  void SetcontHistos(bool flag) {contHistos=flag;}
  void SetMPFmode( bool flag ) {MPFmode = flag;}
  void SetrecoMissing(bool flag) {recoMissing = flag;}

  bool GetuseEarlierCuts() {return useEarlierCuts;}
  bool GetzjFitting() {return zjFitting;}
  bool GetrunCMS() {return runCMS;}
  bool GetprintProg() {return printProg;}
  bool GetcontHistos() {return contHistos;}
  bool GetMPFmode()  {return MPFmode;}
  bool GetrecoMissing() {return recoMissing;}

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
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
  void   plotPT(int gen=0,int Nevt=-1, bool MConly=false);
  void   plotMPF(int gen=0, int Nevt=-1);
  void   Response(int id, double pseudorap, double energy, double pT,double Rt, double Bfield,
	          TF1* frH, double& retMC, double& retH, double& retEHE, double& retHHe);
  void   ParamReader(string file, int n1, int n2, vector<vector<double>> &params);
  void   plotQuery(string& respStr, string& zjstr, int& gen, int& Nevt);
  bool   fidCuts(int id, double pT);
  bool   isNeutrino(int id);  //PDGID is for Neutrino
  bool   isStrangeB(int id);  //Check if PDGID is for Xi, Sigma or Omega^-
  double trackDeltaPhi(int pdgid, double phi, double pT, double Rt, double B); //New phi where particle hits the calo
  int    Charge(int pdgid); //Charge of the particle
  bool   isChHadron(int pdgid); //Returns true if particle is charged hadron
};

#endif

#ifdef CMSJES_cxx

//A function to read hadron response function parameters from files
//Params:	file		The filename to read as a string
//		n1,n2		Dimensions of the params tensor
//		params		Reference to the tensor to read parameters into
void CMSJES::ParamReader(string file, int n1, int n2, vector<vector<double>> &params)
{
  //Init temps to read into
  double p=0;
  vector<double> v;
  vector<vector<double>> M;		//Temp matrix
  //Read the parameters from files
  ifstream in;
  string paramFile;
  //paramFile = "./spr_mc/RunIIa" + file; 
  paramFile = "./spr_mc/CMS" + file; 

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
      files.push_back("P8_Zjet_3000"  );
      files.push_back("P8_Zjet_10000" );
      files.push_back("P8_Zjet_30000" );
      files.push_back("P8_Zjet_100000");
      files.push_back("P8_Zjet_600000");
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
  ifstream inzj_data_pTbal;   ifstream inzj_MC_pTbal; //CMS jecdata pT Z+jet
  ifstream inzj_data_MPF;     ifstream inzj_MC_MPF;   //CMS jecdata MPF Z+jet
  ifstream inzj_data_MPFntI;  ifstream inzj_MC_MPFntI;//CMS jecdata MPF-notypeI Z+jet
                              ifstream inzj_MC_MPFntI_2018;

  //CMS jecdataGH pT-balance
  inzj_data_pTbal.open("./data_and_MC_input/pTbal/zjet_combination_07Aug2017_Summer16_JECV6_Zmm_GH_2018-03-06/zmmjet_data");
  inzj_MC_pTbal.open("./data_and_MC_input/pTbal/zjet_combination_07Aug2017_Summer16_JECV6_Zmm_GH_2018-03-06/zmmjet_mc");

  //CMS jecdataGH MPF 
  inzj_data_MPF.open("./data_and_MC_input/MPF/jecdataGH/MPF_zmmjet_data");
  inzj_MC_MPF.open("./data_and_MC_input/MPF/jecdataGH/MPF_zmmjet_mc");

  //CMS jecdataGH MPF-notypeI
  inzj_data_MPFntI.open("./data_and_MC_input/MPF/zjet_combination_07Aug2017_Summer16_JECV6_Zmm_GH_2018-03-06/MPF-notypeI_zmmjet_data");
  inzj_MC_MPFntI.open("./data_and_MC_input/MPF/zjet_combination_07Aug2017_Summer16_JECV6_Zmm_GH_2018-03-06/MPF-notypeI_zmmjet_mc");
  inzj_MC_MPFntI_2018.open("./data_and_MC_input/MPF/FullCombination_Zmm_17Sep2018_Autumn18_JECv17/2018MPF-notypeI_zmmjet_mc");

  if (!inzj_data_pTbal.is_open()  || !inzj_MC_pTbal.is_open() ||
      !inzj_data_MPF.is_open()    || !inzj_MC_MPF.is_open()   ||
      !inzj_data_MPFntI.is_open() || !inzj_MC_MPFntI.is_open()||
                                     !inzj_MC_MPFntI_2018.is_open())
  {
    cout << "Error opening CMS data/MC point files!" << endl; return;
  }
  
  for (int step=0; step != ndata_pTbal; ++step) { //CMS pT-balance data
    inzj_data_pTbal   >> zj_pTp[step] >> zj_pTbal[step] >> zj_pTbal_ER[step];
  }
  for (int step=0; step != nMC_pTbal; ++step) {	//CMS pT-balance MC points
    inzj_MC_pTbal   >> zj_MC_pTp[step] >> zj_MC_pTbal[step] >> zj_MC_pTbal_ER[step];
  }
  for (int step=0; step != ndata_MPF; ++step) {	//CMS jecsys MPF data
    inzj_data_MPF   >> zj_pTp_MPF[step] >> zj_MPF[step] >> zj_MPF_ER[step];
  }
  for (int step=0; step != nMC_MPF; ++step) { //CMS jecsys MPF MC points
    inzj_MC_MPF   >> zj_MC_pTp_MPF[step] >> zj_MC_MPF[step] >> zj_MC_MPF_ER[step];
  }
  for (int step=0; step != ndata_MPFntI; ++step) { //CMS jecsys MPF-notypeI data
    inzj_data_MPFntI   >> zj_pTp_MPFntI[step] >> zj_MPFntI[step] >> zj_MPFntI_ER[step];
  }
  for (int step=0; step != nMC_MPFntI; ++step) { //CMS jecsys MPF-notypeI MC points
    inzj_MC_MPFntI   >> zj_MC_pTp_MPFntI[step] >> zj_MC_MPFntI[step] >> zj_MC_MPFntI_ER[step];
  }
  for (int step=0; step != nMC_MPFntI; ++step) { //CMS 2018 MPF-notypeI MC points
    inzj_MC_MPFntI_2018   >> zj_MC_pTp_MPFntI_2018[step] >> zj_MC_MPFntI_2018[step] >> zj_MC_MPFntI_ER_2018[step];
  }

  //Close the CMS MC / data point files
  inzj_data_pTbal.close(); 
  inzj_MC_pTbal.close();
  inzj_data_MPF.close();
  inzj_MC_MPF.close();
  inzj_data_MPFntI.close();
  inzj_MC_MPFntI.close();
  inzj_MC_MPFntI_2018.close();

  /* Read params from files to matrices */

  //Open file and initializations for reading
  int line = 0;	//Stepper

  ParamReader("/pi_EHE.txt",  52, 3, params_pi_EHE);
  ParamReader("/pi_HHe.txt",  52, 3, params_pi_HHe);
  ParamReader("/an_EHE.txt",  52, 3, params_an_EHE);
  ParamReader("/an_HHe.txt",  52, 3, params_an_HHe);
  ParamReader("/ap_EHE.txt",  52, 3, params_ap_EHE);
  ParamReader("/ap_HHe.txt",  52, 3, params_ap_HHe);
  ParamReader("/K0L_EHE.txt", 52, 3, params_K0L_EHE);
  ParamReader("/K0L_HHe.txt", 52, 3, params_K0L_HHe);
  ParamReader("/K0S_EHE.txt", 52, 3, params_K0S_EHE);
  ParamReader("/K0S_HHe.txt", 52, 3, params_K0S_HHe);
  ParamReader("/Km_EHE.txt",  52, 3, params_Km_EHE);
  ParamReader("/Km_HHe.txt",  52, 3, params_Km_HHe);
  ParamReader("/Kp_EHE.txt",  52, 3, params_Kp_EHE);
  ParamReader("/Kp_HHe.txt",  52, 3, params_Kp_HHe);
  ParamReader("/n_EHE.txt",   52, 3, params_n_EHE);
  ParamReader("/n_HHe.txt",   52, 3, params_n_HHe);
  ParamReader("/p_EHE.txt",   52, 3, params_p_EHE);
  ParamReader("/p_HHe.txt",   52, 3, params_p_HHe);

  //Particle groups
  ParamReader("/cat1.txt",   52, 3, params_cat1);
  ParamReader("/cat2.txt",   52, 3, params_cat2);
  ParamReader("/cat3.txt",   52, 3, params_cat3);
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
  prtclnij_pdgid = 0;
  prtclnij_pt = 0;
  prtclnij_eta = 0;
  prtclnij_phi = 0;
  prtclnij_e = 0;
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
  fChain->SetBranchAddress("prtclnij_pdgid",&prtclnij_pdgid, &b_prtclnij_pdgid);
  fChain->SetBranchAddress("prtclnij_pt", &prtclnij_pt, &b_prtclnij_pt);
  fChain->SetBranchAddress("prtclnij_eta", &prtclnij_eta, &b_prtclnij_eta);
  fChain->SetBranchAddress("prtclnij_phi", &prtclnij_phi, &b_prtclnij_phi);
  fChain->SetBranchAddress("prtclnij_e", &prtclnij_e, &b_prtclnij_e);
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
