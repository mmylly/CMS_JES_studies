#define Lookup_cxx
#include "Lookup.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void Lookup::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Lookup.C
//      root> Lookup t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   double ctot = 0, c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0, c6 = 0, c7 = 0, c8 = 0, c9 = 0, c10 = 0, c11 = 0, c12 = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      ctot += weight;
      int first = 0, second = 0, fifth = 0, sixth = 0, seventh = 0, eighth = 0;
      for (unsigned i = 0; i < (*prtn_jet).size(); ++i) {
         if ( (*prtn_jet)[i]==-1 )
           continue;
         if ( (*prtn_tag)[i]==0 )
            ++first;
         else if ( (*prtn_tag)[i]==1 )
            ++second;
         else if ( (*prtn_tag)[i]==4 )
            ++fifth;
         else if ( (*prtn_tag)[i]==5 )
            ++sixth;
         else if ( (*prtn_tag)[i]==6 )
            ++seventh;
         else if ( (*prtn_tag)[i]==7 )
            ++eighth;
      }
      if (second>first)
        c1 += weight;
      else if (first<2)
        c2 += weight;
      if (second>first || first<2) {
         if (fifth==2 && seventh==2)
            c5 += weight;
         else if (fifth==2 || seventh==2)
            c6 += weight;
         else if (fifth==1 && seventh==1)
            c8 += weight;
         else if (fifth==1 || seventh==1)
            c8 += weight;
         if (sixth==2 && eighth == 2)
            c9 += weight;
         else if (sixth==2 || eighth == 2)
            c10 += weight;
         else if (sixth==1 && eighth == 1)
            c11 += weight;
         else if (sixth==1 || eighth == 1)
            c12 += weight;
      }
      if (first==2)
        c3 += weight;
      if (second==2)
        c4 += weight;
   }
   cout << "FS better than cME: " << c1 << ", both are bad: " << c2 << endl;
   cout << "cME tot. hits: " << c3 << " FS tot. hits: " << c4 << endl;
   cout << "Two hits cME: " << c5 << " " << c6 << endl;
   cout << "One hit cME: " << c7 << " " << c8 << endl;
   cout << "Sum cME: " << c5 + c6 + c7 + c8 << endl;
   cout << "Two hits FS: " << c9 << " " << c10 << endl;
   cout << "One hit FS: " << c11 << " " << c12 << endl;
   cout << "Sum FS: " << c9 + c10 + c11 + c12 << endl;
   cout << "Total weights: " << ctot << endl;
}
