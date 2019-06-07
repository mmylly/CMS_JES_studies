//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb  9 17:01:55 2017 by ROOT version 6.07/07
// from TTree Pythia8Jets/Pythia8 particle data.
// found on file: pjets.root
//////////////////////////////////////////////////////////

#ifndef Lookup_h
#define Lookup_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "string"

using std::string;

class Lookup {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         weight;
   vector<unsigned char> *prtcl_jet;
   vector<int>     *prtcl_pdgid;
   vector<float>   *prtcl_pt;
   vector<float>   *prtcl_eta;
   vector<float>   *prtcl_phi;
   vector<float>   *prtcl_e;
   vector<char>    *prtn_jet;
   vector<int>     *prtn_pdgid;
   vector<char>    *prtn_tag;
   vector<float>   *prtn_pt;
   vector<float>   *prtn_eta;
   vector<float>   *prtn_phi;
   vector<float>   *prtn_e;
   vector<float>   *jet_pt;
   vector<float>   *jet_eta;
   vector<float>   *jet_phi;
   vector<float>   *jet_e;

   // List of branches
   TBranch        *b_weight;   //!
   TBranch        *b_prtcl_jet;   //!
   TBranch        *b_prtcl_pdgid;   //!
   TBranch        *b_prtcl_pt;   //!
   TBranch        *b_prtcl_eta;   //!
   TBranch        *b_prtcl_phi;   //!
   TBranch        *b_prtcl_e;   //!
   TBranch        *b_prtn_jet;   //!
   TBranch        *b_prtn_pdgid;   //!
   TBranch        *b_prtn_tag;   //!
   TBranch        *b_prtn_pt;   //!
   TBranch        *b_prtn_eta;   //!
   TBranch        *b_prtn_phi;   //!
   TBranch        *b_prtn_e;   //!
   TBranch        *b_jet_pt;   //!
   TBranch        *b_jet_eta;   //!
   TBranch        *b_jet_phi;   //!
   TBranch        *b_jet_e;   //!

   Lookup(std::string filename="");
   virtual ~Lookup();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Lookup_cxx
Lookup::Lookup(std::string filename) : fChain(0) 
{
   TTree *tree;
   TFile *f = new TFile(filename.c_str());
   if (!f || !f->IsOpen()) {
      f = new TFile("pjets_pythia8_dijet_1000.root");
   }
   f->GetObject("Pythia8Jets",tree);

   Init(tree);
}

Lookup::~Lookup()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Lookup::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Lookup::LoadTree(Long64_t entry)
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

void Lookup::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   prtcl_jet = 0;
   prtcl_pdgid = 0;
   prtcl_pt = 0;
   prtcl_eta = 0;
   prtcl_phi = 0;
   prtcl_e = 0;
   prtn_jet = 0;
   prtn_pdgid = 0;
   prtn_tag = 0;
   prtn_pt = 0;
   prtn_eta = 0;
   prtn_phi = 0;
   prtn_e = 0;
   jet_pt = 0;
   jet_eta = 0;
   jet_phi = 0;
   jet_e = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("prtcl_jet", &prtcl_jet, &b_prtcl_jet);
   fChain->SetBranchAddress("prtcl_pdgid", &prtcl_pdgid, &b_prtcl_pdgid);
   fChain->SetBranchAddress("prtcl_pt", &prtcl_pt, &b_prtcl_pt);
   fChain->SetBranchAddress("prtcl_eta", &prtcl_eta, &b_prtcl_eta);
   fChain->SetBranchAddress("prtcl_phi", &prtcl_phi, &b_prtcl_phi);
   fChain->SetBranchAddress("prtcl_e", &prtcl_e, &b_prtcl_e);
   fChain->SetBranchAddress("prtn_jet", &prtn_jet, &b_prtn_jet);
   fChain->SetBranchAddress("prtn_pdgid", &prtn_pdgid, &b_prtn_pdgid);
   fChain->SetBranchAddress("prtn_tag", &prtn_tag, &b_prtn_tag);
   fChain->SetBranchAddress("prtn_pt", &prtn_pt, &b_prtn_pt);
   fChain->SetBranchAddress("prtn_eta", &prtn_eta, &b_prtn_eta);
   fChain->SetBranchAddress("prtn_phi", &prtn_phi, &b_prtn_phi);
   fChain->SetBranchAddress("prtn_e", &prtn_e, &b_prtn_e);
   fChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
   fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
   fChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
   fChain->SetBranchAddress("jet_e", &jet_e, &b_jet_e);
   Notify();
}

Bool_t Lookup::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Lookup::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Lookup::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Lookup_cxx
