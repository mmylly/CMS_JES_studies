/////////////////////////////////////////////////////////////////////
/// @file Pythia8Jets.h
///
/// @author Hannu Siikonen (errai- \@GitHub, hsiikone \@GitLab)
/// @date 1.11.2017 (Last modification; fun times)
/////////////////////////////////////////////////////////////////////

#ifndef PYTHIA8JETS
#define PYTHIA8JETS

/* Stdlib header file for input and output. */
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <iterator>

/* Header file to access Pythia 8 program elements. */
#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/FastJet3.h"
//#include "Pythia8Plugins/CombineMatchingInput.h"
#include "Pythia8Plugins/aMCatNLOHooks.h"
#include "Pythia8Plugins/PowhegHooks.h"

// ROOT
#include <TROOT.h>
#include <TChain.h>
#include <TMatrix.h>
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>
#include <TMath.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <TVector2.h>

// FastJet interface
#include "fastjet/config.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/GridMedianBackgroundEstimator.hh"
#include "fastjet/internal/numconsts.hh"

/* scripts */
#include "../generic/help_functions.h"
#include "../events/PrtclEvent.h"

using namespace Pythia8;
using std::string;
using std::vector;
using std::pair;
using std::map;
using std::cout;
using std::endl;
using std::cerr;
using std::runtime_error;


struct PartonHolder {
    fastjet::PseudoJet p4;
    int id;
    char tag;
    int ptnid;
    int ownid;
    bool used;
};


/* A selector class to speed up lepton+ selection in ttbar */
class TTBarSelector : public UserHooks
{
public:
    TTBarSelector() {}
    virtual ~TTBarSelector() {}

    /* In UserHooks this returns false */
    virtual bool canVetoProcessLevel() { return true; }

    /* Returns true, if the event is vetoed (i.e. amount of hard proc leptons != 2) */
    virtual bool doVetoProcessLevel(Event& process);
};

/////////////////////////////////////////////////////////////////////
/// Class structure for storing Pythia8 particle data into trees.
///
/// Modes of operation:
///
///     0: Generic events
///         - Normal QCD production for general studies
///
///     1: Dijet events
///         - Normal QCD production with dijet-specific settings
///
///     2: Photon+Jet events
///         - Photon+Jet production
///
///     3: Zmumu+Jet events
///         - Z+Jet production with a Z -> mumu setting
///
///     4: Ttbarlepton+jet events
///         - Ttbar production with WW -> qqbarllbar
///
///     5: Ttbarlepton+jet events \@POWHEG
///         - Ttbar production with WW -> qqbarllbar
///
///     6: Ttbarlepton+jet events \@MADGRAPH5aMC\@NLO
///         - Ttbar production with WW -> qqbarllbar
/////////////////////////////////////////////////////////////////////
class Pythia8Jets
{
public:
    /* The event loop should be called once after a proper initialization */
    void EventLoop();

    /* No copying or constructing with another instance */
    Pythia8Jets(const Pythia8Jets& other) = delete;
    Pythia8Jets& operator=(const Pythia8Jets&) = delete;

    /* Run settings are provided through the initializer */
    Pythia8Jets(string settings, string fileName, int mode);
    Pythia8Jets() :
                    mEvent(mPythia.event),
                    mInitialized(false)
    {
        cerr << "Pythia8Jets is intended to be used only with the non-default initializer" << endl;
    }
    /* ROOT has an awful behaviour with pointers. It should do the cleaning-up
       and such stuff well, but there is a ton of memory leaks that result
       already from ROOT 'being there'. At least the software runs - but the
       memory leaks and ROOT style pointer handling are regrettable. */
    ~Pythia8Jets() {
        //if (mMode==6) {
            //delete mMatching;
            //delete mMerging;
        //}
    }

protected:

    /* A handle for adding particle information */
    void                ParticleAdd(unsigned prt, char jetid);
    void                PartonAdd(unsigned prt, char jetid, char tag, int ptnid = -1, int ownid = -1);
    void                PartonAdd(unsigned prt, char jetid);
    void                JetAdd(unsigned jet, int spoil = 0);

    void                PartonAppend(fastjet::PseudoJet p4, unsigned prt, char tag, int ptnid=-1, int ownid = -1);
    //void                PartonAppend(fastjet::PseudoJet p4, unsigned prt, int tag);

    /* A handy handle */
    TLorentzVector      TLorentzify(unsigned prt);
    /* Another handy handle */
    fastjet::PseudoJet  PseudoJettify(unsigned prt);
    fastjet::PseudoJet  PseudoJettify(TLorentzVector p4);


    /* Add a possibility for per event pre-processing */
    virtual void        PreProcess() {}
    /* Loop over particles within an event: returns true if event is to be saved */
    bool                ParticleLoop();
    /* The logic within particleloop. */
    bool                ProcessParticle(unsigned prt);
    /* A custom phase within ProcessParticle */
    virtual int         CustomProcess(unsigned prt) { return 2; }
    /* Allow an event type based veto */
    virtual bool        IsolationProc() { return true; }
    /* Allow an event type based parton processing */
    virtual int         PartonProc(unsigned prt) { return 0; }
    /* Jet clustering routines */
    bool                JetLoop();
    /* Check nlo and so forth */
    void                PartonDescend(unsigned prt);

    /* See: HadronAndPartonSelector.cc in CMSSW. Indicates whether a ghost hadron
     * is in an excited state or not. Checks whether a hadron has a daughter of
     * the same flavour. Parameter quarkId is a PDG quark flavour. */
    int                 IsExcitedHadronState(unsigned prt, int quarkID);
    /* A function that checks whether a photon is originated from a pi0 and that
     * the energy of the photon-pair corresponds to the pion. returns 0 if
     * the origin is not a pion with good energy and 1 if it is */
    bool                GammaChecker(unsigned prt);
    /* Has the particle already been appended */
    bool                Absent(unsigned prt);

    /* Print the decay tree */
    void                PrintParticle(unsigned prt);
    void                PrintEvent();

    /* Corrected parton momentum */
    unsigned            OptimalParton(unsigned prt);
    TLorentzVector      LastParton(unsigned prt, bool &stop);
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

    void                AddMessage(string msg, double wgt);

    int                 JetAnc(float pt);

protected:

    /* Indicator that the event loop can be run */
    bool                mInitialized;
    /* A general-purpose counters for physics debugging */
    unsigned            mCounter;
    unsigned            mIterCount;

    unsigned            mHardProcCount;
    char                mPartonCount;

    Pythia              mPythia;
    Event&              mEvent;

    TFile              *mFile;
    TTree              *mTree;

    TTBarSelector       mTTBarSelector;


    int                 mNumErrs;
    int                 mNumEvents;
    int                 mEvtNo;
    int                 mMode;
    int                 mTimerStep;
    Timer               mTimer;

    vector<unsigned>    mSpecialIndices;
    vector<int>         mBs;

    char                         mBNuCount;
    char                         mONuCount;
    char                         mNuOB;
    char                         mNuOC;
    char                         mNuOLept;
    char                         mNuOOther;
    char                         mNuB;
    char                         mNuC;
    char                         mNuLept;
    char                         mNuOther;

    char                         mInfo;
    float                        mWeight;
    float                        mPtHat;
    /* Particle level. */
    vector<float>         mIsolation;
    vector<unsigned char> mJetId;
    vector<int>           mPDGID;
    vector<float>         mPt;
    vector<float>         mEta;
    vector<float>         mPhi;
    vector<float>         mE;
    /* Parton level. */
    vector<char>          mPJetId;
    vector<int>           mPPtnId;
    vector<int>           mPOwnId;
    vector<int>           mPPDGID;
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

    double                     mGhostCoeff;
    vector<PartonHolder>       mPartonInfo;

    fastjet::JetDefinition     mJetDef;
    vector<fastjet::PseudoJet> mJetInputs;
    vector<fastjet::PseudoJet> mNJetInputs;
    vector<fastjet::PseudoJet> mPJetInputs;
    vector<fastjet::PseudoJet> mSortedJets;
    vector<fastjet::PseudoJet> mJetParts;
    vector<fastjet::PseudoJet> mCutJetParts;

    vector<int>                mAncestry;
    vector<int>                mNAncestry;
    vector<int>                mWs;

    double                     mTotWgt;
    double                     mSelWgt;
    map<string,double>         mErrorList;

    const bool                 mCutMode = true;
};


class P8GenericTree : public Pythia8Jets
{
public:
    P8GenericTree(string settings, string fileName, int mode) :
        Pythia8Jets(settings, fileName, mode) {}
    ~P8GenericTree() {}

protected:
    virtual int CustomProcess(unsigned prt);
};


class P8DijetTree : public Pythia8Jets
{
public:
    P8DijetTree(string settings, string fileName, int mode) :
        Pythia8Jets(settings, fileName, mode) {}
    ~P8DijetTree() {}

protected:
    /* Dijet specific particle logic */
    virtual int CustomProcess(unsigned prt);
};


class P8GammajetTree : public Pythia8Jets
{
public:
    P8GammajetTree(string settings, string fileName, int mode) :
        Pythia8Jets(settings, fileName, mode) {}
    ~P8GammajetTree() {}

protected:
    vector<pair<fastjet::PseudoJet,int>> mGammas;

    /* Photon isolation parameters */
    const double mGammaDR = 0.1;
    const double mGammaPt = 30;
    const double mGammaEta = 2.1;

    /* Gammajet specific preprocessing */
    virtual void PreProcess();
    /* A handle for adding a hard process photon descended from prt */
    bool GammaAdd(unsigned prt);
    /* Gammajet specific particle logic */
    virtual int CustomProcess(unsigned prt);
    /* Gammajet veto for isolation */
    virtual bool IsolationProc();
};


class P8ZmumujetTree : public Pythia8Jets
{
public:
    P8ZmumujetTree(string settings, string fileName, int mode) :
        Pythia8Jets(settings, fileName, mode) {}
    ~P8ZmumujetTree() {}

protected:
    vector<pair<fastjet::PseudoJet,int>> mMuons;

    /* Muon isolation parameters */
    const double mMuonDR = 0.1; // Limit 0.1
    const double mMuonPt = 30;
    const double mMuonEta = 2.1;

    /* Z+jet specific preprocessing */
    virtual void PreProcess();
    /* A handle for adding the two muons originating from a hard process Z prt */
    bool MuonAdd(unsigned prt);
    /* Zmumujet specific particle logic */
    virtual int CustomProcess(unsigned prt);
    /* Z+jet veto for isolation */
    virtual bool IsolationProc();
};


class P8ttbarjetTree : public Pythia8Jets
{
public:
    P8ttbarjetTree(string settings, string fileName, int mode) :
        Pythia8Jets(settings, fileName, mode) {}
    ~P8ttbarjetTree() {}

protected:
    vector<pair<fastjet::PseudoJet,int>> mChargeds;
    int mLeptonFriend;

    /* Lepton isolation parameters */
    const double mElDR = 0.3; // Limit 0.12 or 0.20
    const double mMuDR = 0.4; // Limit 0.15 or 0.25
    /* Worst case kind of cuts for bonus leptons */
    const double mLeptPt = 5;
    const double mLeptEta = 2.4;

    /* ttbarjet specific preprocessing */
    virtual void PreProcess();
    /* A handle for adding the produced leptons in ttbar events */
    bool LeptonAdd(unsigned prt, int parent=-1);
    /* ttbarjet specific particle logic */
    virtual int CustomProcess(unsigned prt);
    /* ttbarjet veto for isolation */
    virtual bool IsolationProc();
    /* W parton extract */
    virtual int PartonProc(unsigned prt);
};


#endif // PYTHIA8JETS
