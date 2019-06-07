
// The general class for turning particle data
// into analyzed jet data.
// Hannu Siikonen 14.6.2016
// Usage: ./jetanalysis.exe [Standard form input file name] [path - e.g. './'] [Flavour def.]
// Flavour options:
//  1: Physics definition
//  2: LO (Physics definition with ghost partons)
//  3: CLO (Physics definition with momentum correction)
//  4: LO+CLO (Combination of 2 and 3)
//  5: LO+FS (Combination of 2 and 10)
//  6: Historic parton definition (not implemented)
//  7: Historic hadron definition
//  8: Hadronic definition
//  9: Algorithmic definition
//  10: LO (Algorithmic definition with ghost partons)

/////////////////////////////////////////////////

#ifndef JETBASE_H
#define JETBASE_H

// General
#include <iostream>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <vector>
#include <map>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <set>

// ROOT
#include <TROOT.h>
#include <TChain.h>
#include <TMatrix.h>
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <TVector2.h>

// FastJet interface
#include "fastjet/config.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/GridMedianBackgroundEstimator.hh"
#include "Pythia8Plugins/FastJet3.h"

// Header file for the classes stored in the TTree if any.
#include "../events/PrtclEvent.h"
#include "../events/JetEvent.h"

// Fixed size dimensions of array or collections stored in the TTree if any.
#include "../generic/help_functions.h"

#include "QCDAwarePlugin.hh"

using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::pair;
using std::set;
using std::cerr;
using fastjet::PseudoJet;

using namespace fastjet::contrib::QCDAwarePlugin;

class JetBase
{
public :
    JetBase() : fInitialized(false) {
        cerr << "The default constructor is not intended to be used" << endl;
    }
    JetBase(TTree *, const char*, const char*, Int_t, Int_t);

    /* Initializes the tree that is read */
    virtual void        Init(TTree*);
    virtual void        Finalize();

    virtual Int_t       GetEntry(Long64_t);
    virtual Long64_t    LoadTree(Long64_t);
    virtual void        Show(Long64_t = -1);

    virtual void        EventLoop();
    /* Calculate variables for the newly clustered jets */
    virtual bool        JetLoop();
    virtual inline void InitLoop() { return; }
    virtual inline void PostLoop() { return; }
    /* Study particle types in the clustered jets */
    virtual void        ParticleLoop();
    virtual inline void EventProcessing();
    virtual inline void PostProcessing(unsigned = 0) { return; }

    virtual void        ParticlesToJetsorterInput();
    /* Event type specific cuts */
    virtual Bool_t      SelectionParams();

    virtual Bool_t      Isolation(PseudoJet prt, double R = 0.3);

    /* The classically best jet flavour definition in terms of robustness. */
    virtual void        PhysFlavor(unsigned jet);
    /* Ghost parton jet clustering for the physics definition.
     * Used by the ghost parton physics definition and the
     * experimental final (ghost) parton physics definition.
     * The latter uses a sum of the momenta of the hard process descendants
     * instead of the hard process momentum values. */
    virtual void        GhostFlavor(unsigned jet);
    /* Adapts to many ghosts found within a jet */
    virtual void        GhostComboFlavor();
    /* For an equal combination of the definitions (e.g. LO & CLO) */
    virtual void        EqualFlavor();
    /* A combination of the physics definition and algorithmic definition */
    virtual void        LOFSFlavor();
    /* Particle identification the modern "Hadronic definition".
     * Determine whether a jet is dominated by quarks or by gluons.
     * Looping stops when a corresponding jet is found.
     * Hadron flavour is used as a dominating feature.
     * See https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagMCTools
     * for further information. */
    virtual void        HadrFlavor();
    /* Algorithmic flavor tagging is somewhat similar to hadronic tagging.
     * See https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagMCTools
     * for further information. */
    virtual void        AlgoFlavor(unsigned jet);
    /* The hadronic definition without hadrons (or modernized algorithmic deifinition) */
    virtual void        FSFlavor();

protected:
///////////
// Fastjet:
///////////
    int suu1, suu2, suu3, suu4, suu5;
    bool truth1, truth2, truth3;

    /* IMPORTANT: jet sorting parameters */
    double fR;
    double fMinPT;
    /* end of jet sorting parameters */

    int fJetsPerEvent;   /* How many leading jets are stored in a run */

    fastjet::JetDefinition   fJetDef;

    /* PseudoJet storages */
    vector<PseudoJet> fJetInputs;   /* Raw final-state particle data */
    vector<PseudoJet> fSortedJets;  /* Post jet clustering */
    vector<PseudoJet> fJetParts;    /* Jet constituents */
    vector<PseudoJet> fHardPartons; /* Outgoing hard process partons */
    vector<PseudoJet> fCorrPartons; /* Outgoing hard process partons with corrected four-momentum */
    vector<PseudoJet> fPartons;     /* Other partons */
    vector<PseudoJet> fLeptons;     /* Z+jets, ttbarlepton+jets */
    vector<PseudoJet> fAux;         /* Auxiliary storage */

    PseudoJet         fTheGamma;       /* gamma+jets */
    PseudoJet         fTheLepton;   /* ttbar */

/////////
// Input:
/////////
    TTree          *fChain;   //!
    Int_t           fCurrent; //! current Tree number in a fChain

    /* Fixed size dimensions of array or collections stored in the TTree if any.
     * If this is too small, Segfaults may follow. */
    static const Int_t  kMaxfPrtcls = 5000;

    //PrtclEvent    *event;
    Double_t        fWeight;
    Int_t           fPrtcls_;
    Double_t        fX[kMaxfPrtcls];   //[fPrtcls_]
    Double_t        fY[kMaxfPrtcls];   //[fPrtcls_]
    Double_t        fZ[kMaxfPrtcls];   //[fPrtcls_]
    Double_t        fT[kMaxfPrtcls];   //[fPrtcls_]

    Int_t           fPDGCode[kMaxfPrtcls];   //[fPrtcls_]
    Int_t           fAnalysisStatus[kMaxfPrtcls];   //[fPrtcls_]

//////////
// Output:
//////////
    TFile          *fOutFile;
    TFile          *fOutFile2;
    string          fOutFileName;
    TTree          *fOutTree;
    TBranch        *fJetBranch;
    JetEvent       *fJetEvent;

//////////
// Others:
//////////
    /* Transverse energy counters: */
    PseudoJet       fPiPlus;
    PseudoJet       fPiMinus;
    PseudoJet       fPi0Gamma;
    PseudoJet       fGamma;
    PseudoJet       fKaPlus;
    PseudoJet       fKaMinus;
    PseudoJet       fKSZero;
    PseudoJet       fKLZero;
    PseudoJet       fXiZero;
    PseudoJet       fXiMinus;
    PseudoJet       fOmMinus;
    PseudoJet       fProton;
    PseudoJet       fAproton;
    PseudoJet       fNeutron;
    PseudoJet       fAneutron;
    PseudoJet       fLambda0;
    PseudoJet       fSigma;
    PseudoJet       fElec;
    PseudoJet       fMuon;
    PseudoJet       fOthers;
    PseudoJet       fEtSum;

    PseudoJet       fMET;

    Timer           fTimer;

    /* The user may choose to make cuts manually in the final analysis by setting this to false */
    bool            fJetCuts;
    bool            fParamCuts;
    bool            fParticleStudy;
    bool            fInitialized;
    bool            fAddNonJet;
    bool            fSkipSelection;

    Int_t           fFlavour;
    Int_t           fMode;       /* Event type */
    Int_t           fDefinition; /* Flavour definition */
    Int_t           fBookedParton; /* Monitor partons that are paired with jets */
    Int_t           fSuccessCount;
    Int_t           fStatus; /* Numeric indicator for various issues in the event */

    JetVariables    fJetVars;
};

#endif // JETBASE_H
