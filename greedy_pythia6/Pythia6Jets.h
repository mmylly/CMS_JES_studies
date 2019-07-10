///////////////////////////////////////////////////////////////////
//                                                               //
// Class structure for storing Pythia6 particle data into trees. //
//                                                               //
// Modes of operation:                                           //
//                                                               //
//    0: Generic events                                          //
//       -Normal QCD production for general studies              //
//                                                               //
//    1: Dijet events                                            //
//       -Normal QCD production with dijet-specific settings     //
//                                                               //
//    2: Photon+Jet events                                       //
//       - Photon+Jet production                                 //
//                                                               //
//    3: Zmumu+Jet events                                        //
//       - Z+Jet production with a Z -> mumu setting             //
//                                                               //
//    4: Ttbarlepton+jet events                                  //
//       - Ttbar production with WW -> qqbarllbar                //
//                                                               //
// Authors: Hannu Siikonen (errai- @GitHub)                      //
//          Toni Mäkelä (D0 edits)                               //
//                                                               //
///////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Hints:                                                                     //
// mPythia->GetK(prt,n) reads prt:th particle's n:th column in Pythia6 output //
// n=1: status, n=2: PDG ID, n=3: mother's index, n=4,5: daughter's index     //
// abbreviations: FS (Final State), HP (Hard Process)                         //
////////////////////////////////////////////////////////////////////////////////

#ifndef PYTHIA6TREE
#define PYTHIA6TREE

#include "../greedy_settings.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// An explanation of the particle indices found in Pythia6 (this has a rather rigid structure:    //
//                                                                                                //
// General:                                                                                       //
//      1-2: incoming particles (e.g. protons)                                                    //
//      3-4: incoming partons of the hard interaction                                             //
//      5-6: a copy of the incoming partons of the hard interaction                               //
//      7-8: outgoing partons/particles of the hard interaction                                   //
// QCD events (mode 0, 1):                                                                        //
//      No need for further actions: the partons 7-8 are the ones we are interested in            //
// Gamma+jets events (mode 2):                                                                    //
//      7 or 8: the hard interaction photon                                                       //
//      9: a showering phase copy of the hard interaction photon                                  //
// Zmumu+jets events (mode 3):                                                                    //
//      7 or 8: the hard interaction Z                                                            //
//      9-10: muons produced in the Z decay                                                       //
//      11: a showering phase copy of the Z                                                       //
//      12-13: showering phase copies of the Z decay muons                                        //
// TTbar events (mode 4):                                                                         //
//      7-8: the tops, these won't be useful                                                      //
//      9,11: the b partons from top decay                                                        //
//      10,12: the W's from top decay                                                             //
//      13-16: the W decay products                                                               //
//      17-18: showering phase copies of the W's                                                  //
//      19-20 (or further down): showering phase copies of a leptonic W decay                     //
//                                                                                                //
//  Things to consider:                                                                           //
//      - The particles/partons 1-8 hold pre-showering info and do not include "daughter" info    //
//        linking to the showering phase particles                                                //
//      - Hence, if we want to look e.g. for the final-state photon in gamma+jets events, we need //
//        to find the photon which has the idx 7/8 photon listed as its "mother"                  //
//      - We can only produce generic ttbar events: selection of lepton+jets events requires us to//
//        reject events with two hadronic or leptonics W's                                        //
//      - In the lepton+jets events the lepton is sometimes actually a tau, which decays before   //
//        reaching the detector                                                                   //
//      - The first stage of the decay produces a tau neutrino and a W: the W can decay           //
//        hadronically or leptonically                                                            //
//      - We exclude events with a hadronic tau decay, as this decay mode fails to produce a      //
//        lepton (e/mu) to the final state                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////

// ROOT
#include "TROOT.h"
#include "TFile.h"
#include "TBranch.h"
#include "TLorentzVector.h"
#include "TPythia6.h"
#include "TError.h"
#include "TTree.h"
#include "TClonesArray.h"
#include <TChain.h>
#include <TMatrix.h>
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>
#include <TVector2.h>

// FastJet interface
#include "fastjet/config.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/GridMedianBackgroundEstimator.hh"
#include "fastjet/D0RunIIConePlugin.hh"
#include "Pythia8Plugins/FastJet3.h"

// Scripts
#include "../generic/help_functions.h"
#include "../events/PrtclEvent.h"

/*struct PartonHolder {
    fastjet::PseudoJet p4;
    int id;
    char tag;
    int ptnid;
    int ownid;
    bool used;
};*/

class Pythia6Jets
{
public:
    /* The event loop should be called once after a proper initialization */
    void EventLoop();

    /* No copying or constructing with another instance */
    Pythia6Jets( const Pythia6Jets& other ) = delete;
    Pythia6Jets& operator=( const Pythia6Jets& ) = delete;

    /* Run settings are provided via the initializer */
    Pythia6Jets(Int_t nEvent, string fileName, Int_t nameId, const int mode);
    Pythia6Jets() : mInitialized(false)
    {
        cerr << "Pythia6Tree is intended to be used only with the non-default initializer" << endl;
    }
    /* ROOT has an awful behaviour with pointers. It should do the cleaning-up
       and such stuff well, but there is a ton of memory leaks that result
       already from ROOT 'being there'. At least the software runs - but the
       memory leaks and ROOT style pointer handling are regrettable. */
    ~Pythia6Jets() {}

protected:

    /* A handle for adding particle information */
    #ifdef STORE_PRTCLS
    void                ParticleAdd(unsigned prt, char jetid=-1);
    #endif
    void                PartonAdd(unsigned prt, char jetid = -1);
    void                PartonAdd(unsigned prt, char jetid, char tag, int ptnid = -1, int ownid = -1);
    void                JetAdd(unsigned jet);

    void                PartonAppend(fastjet::PseudoJet p4, unsigned prt, int tag, int ptnid = -1, int ownid = -1);
    /* Particles needed by the hadronic flavor definition */
    void                GhostHadronAdd(unsigned prt, bool useStrange = false);
    /* Add the descendants of this particle. */
    void                PartonDescend(unsigned prt);

    /* Event type specific checks for the event display structure. */
    bool                SanityChecks();
    /* Clear containers and do some additional event-type specific procedures. */
    bool                InitEvent();
    /* Loop over particles within an event: return true if event is to be saved */
    bool                ParticleLoop();
    /* The logic within particleloop */
    bool                ProcessParticle(unsigned prt);
    /* Check the ancestor of a hard process parton. */
    int                 PartonProc(unsigned prt);
    /* Jet clustering routines */
    bool                JetLoop();
    /* Process isolation conditions */
    bool                IsolationProc();
    /* Isolation for e.g. photons */
    bool                IsolationPhotons(int iprt, fastjet::PseudoJet &ipart);
    bool                IsolationMuons  (int iprt, fastjet::PseudoJet &ipart);
    bool                IsolationLeptons(int iprt, fastjet::PseudoJet &ipart);
    /* Check the eta-phi distance of isolated objects to jets. */
    bool                IsolationFromJets();

    /* Settings that depend on the selected mode */
    void                ModeSettings();
    /* General settings that are always used */
    void                GeneralSettings();

    /* A handle for adding a hard process photon descended from the signal event photon */
    bool                GammaAdd();
    /* A handle for adding the two muons originating from a hard process Z prt */
    bool                MuonAdd();
    /* A handle for adding the produced leptons in ttbar events */
    bool                LeptonAdd();

    /* Checks if ancestor is in the direct mother line of descendant. */
    bool                IsAncestor(unsigned ancestor, unsigned descendant);
    /* See: HadronAndPartonSelector.cc in CMSSW. Indicates whether a ghost hadron
     * is in an excited state or not. Checks whether a hadron has a daughter of
     * the same flavour. Parameter quarkId is a PDG quark flavour. */
    bool                IsExcitedHadronState(unsigned prt, int quarkId);
    /* A function that checks whether a photon is originated from a pi0 and that
     * the energy of the photon-pair corresponds to the pion. returns 0 if
     * the origin is not a pion with good energy and 1 if it is */
    bool                GammaChecker(unsigned);
    /* Has the particle already been appended */
    bool                Absent(unsigned prt);

    /* Print the decay tree */
    void                PrintParticle(unsigned prt);
    void                PrintEvent();

    /* A handy handle */
    TLorentzVector      TLorentzify(unsigned prt);
    /* Another handy handle */
    fastjet::PseudoJet  PseudoJettify(unsigned prt);
    fastjet::PseudoJet  PseudoJettify(TLorentzVector p4);

    /* Corrected parton momentum */
    unsigned            OptimalParton(unsigned prt);
    TLorentzVector      LastParton(unsigned prt,bool &stop);
    TLorentzVector      NextParton(unsigned prt,unsigned lvl);

    /* Cuts for jet property calculations */
    void                Cuts();
    /* Fragmentation function calculation */
    double              PTD();
    /* Calculation of jet inner radius */
    double              Sigma2();
    /* Helper functions for Cuts */
    bool                IsCharged(int pdgid);
    bool                IsHadron(int pdgid);
    bool                IsLepton(int pdgid);
    /* A shortcut for finding out what stuff is partons (no beam remnants) */
    bool                IsPartonIdx(unsigned prt);
    /* Check whether this is an FS parton */
    bool                IsFSPartonIdx(unsigned prt);
    /* Check whether this is an FS parton */
    bool                IsPreFSPartonIdx(unsigned prt);
    /* Check whether this belongs to the Hard Process partons. */
    bool                IsHPPartonIdx(unsigned prt);
    /* Check if this is a cluster or string. */
    bool                IsPreHadronization(unsigned prt);

protected:
    vector<pair<fastjet::PseudoJet,int>> mGammas;
    vector<pair<fastjet::PseudoJet,int>> mMuons;
    vector<pair<fastjet::PseudoJet,int>> mChargeds;
    int                                  mHadronFriend;

    /* Indicator that the event loop can be run */
    bool                            mInitialized;
    /* A general-purpose counter for physics debugging */
    unsigned                        mCounter;
    unsigned                        mNumErrs;
    /* Counters for neutrinos */
    char                            mBNuCount;
    char                            mONuCount;
    char                            mNuOB;
    char                            mNuOC;
    char                            mNuOLept;
    char                            mNuOOther;
    char                            mNuB;
    char                            mNuC;
    char                            mNuLept;
    char                            mNuOther;

    TPythia6                       *mPythia;

    TFile                          *mFile;
    TTree                          *mTree;
    TBranch                        *mBranch;

    int                             mNumEvents;
    int                             mMode;
    int                             mTimerStep;
    Timer                           mTimer;

    vector<unsigned>                mSpecialIndices;
    map<unsigned,TLorentzVector>    mPartonHistory;

    char                  mInfo;
    float                 mWeight;
    float                 mPtHat;
    vector<bool>          mIsolation;
    /* Particle level for the particles within jets. */
    vector<unsigned char> mJetId;
    vector<int>           mPDGID;
    vector<float>         mPt;
    vector<float>         mEta;
    vector<float>         mPhi;
    vector<float>         mE;
    /* Particle level for the particles not in jets (nij). */
    vector<int>           mNIJPDGID;
    vector<float>         mNIJPt;
    vector<float>         mNIJEta;
    vector<float>         mNIJPhi;
    vector<float>         mNIJE;
    /* Parton level. */
    vector<char>          mPJetId;
    vector<int>           mPPDGID;
    vector<int>           mPPtnId;
    vector<int>           mPOwnId;
    vector<char>          mPTag;
    vector<float>         mPPt;
    vector<float>         mPEta;
    vector<float>         mPPhi;
    vector<float>         mPE;
    vector<float>         mDR;
    /* Jet level. */
    vector<float>         mJPt;
    vector<float>         mJEta;
    vector<float>         mJPhi;
    vector<float>         mJE;
    vector<int>           mJPtnId;
    vector<int>           mConst;
    vector<float>         mPTD;
    vector<float>         mSigma2;
    /* MET level. */
    float                 mMet;
    fastjet::PseudoJet    mMetVect;
    fastjet::PseudoJet    mGamma;

    vector<PartonHolder>  mPartonInfo;

    vector<int>           mWs;
    vector<int>           mBs;
    vector<int>           mHPPartons;

    fastjet::JetDefinition::Plugin *mPlugin;
    fastjet::JetDefinition          mJetDef;
    vector<fastjet::PseudoJet>      mJetInputs;
    vector<fastjet::PseudoJet>      mSortedJets;
    vector<fastjet::PseudoJet>      mJetParts;
    vector<fastjet::PseudoJet>      mCutJetParts;
    vector<TLorentzVector>          mPartons;
};

#endif // PYTHIA6TREE
