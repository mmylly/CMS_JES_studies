#include "Pythia6Jets.h"

Pythia6Jets::Pythia6Jets(Int_t nEvent, string fileName, Int_t nameId, const int mode) :
    mMode(mode),
    mNumEvents(nEvent),
    mInitialized(true),
    mCounter(0)
{
    /* Commenting the settings! */
    if (cfg::DOD0 and cfg::DOD0RIIC) cerr << "  Using D0 RunII cone algorithm plugin";
    else cerr << "  Using anti-kT jet algorithm";
    cerr << " -> Cone radius = " << cfg::RCone << endl;
    if (cfg::BEnriched) cerr << "Making a b-enriched sample" << endl;
    #ifdef STORE_PRTCLS
    cerr << "  Storing Final-State particles found in jets." << endl;
    if (cfg::StoreNIJ) cerr << "  Storing also particles not in jets." << endl;
    #endif

    /* Create an instance of the Pythia event generator: */
    mPythia = new TPythia6;
    /* Set a seed value according to the run index and make sure it is used: */
    if (nameId < 1 or nameId > 10) throw std::runtime_error("Incompatible seed ID");

    mPythia->SetMRPY(1,cfg::P6Seeds[nameId-1]);
    mPythia->SetMRPY(2,0);

    /* Event type: */
    ModeSettings();
    /* Other settings: */
    GeneralSettings();

    /* Try to create a file to write */
    mFile = TFile::Open(fileName.c_str(), "RECREATE");
    if (!mFile->IsOpen()) throw runtime_error("Creating an output file failed");
    mFile->SetCompressionLevel(1);

    /* Output tree: */
    mTree = new TTree("Pythia6Jets", "Pythia6 particle data.");
    if(!mTree) throw runtime_error("Creating a tree failed");
    mTree->SetAutoSave(100000000); /* 0.1 GBytes */
    mTree->SetCacheSize(10000000); /* 100 MBytes */
    TTree::SetBranchStyle(1); /* New branch style */

    /** Connect an event to the tree **/
    mTree->Branch("info",          &mInfo     )->SetAutoDelete(kFALSE);
    mTree->Branch("bnucount",      &mBNuCount )->SetAutoDelete(kFALSE);
    mTree->Branch("onucount",      &mONuCount )->SetAutoDelete(kFALSE);
    mTree->Branch("nuob",          &mNuOB     )->SetAutoDelete(kFALSE);
    mTree->Branch("nuoc",          &mNuOC     )->SetAutoDelete(kFALSE);
    mTree->Branch("nuolept",       &mNuOLept  )->SetAutoDelete(kFALSE);
    mTree->Branch("nuoother",      &mNuOOther )->SetAutoDelete(kFALSE);
    mTree->Branch("nub",           &mNuB      )->SetAutoDelete(kFALSE);
    mTree->Branch("nuc",           &mNuC      )->SetAutoDelete(kFALSE);
    mTree->Branch("nulept",        &mNuLept   )->SetAutoDelete(kFALSE);
    mTree->Branch("nuother",       &mNuOther  )->SetAutoDelete(kFALSE);
    mTree->Branch("weight",        &mWeight   )->SetAutoDelete(kFALSE);
    mTree->Branch("pthat",         &mPtHat    )->SetAutoDelete(kFALSE);
    #ifdef STORE_PRTCLS
    /* Particle lvl (jets) */
    mTree->Branch("prtcl_jet",       &mJetId    )->SetAutoDelete(kFALSE);
    mTree->Branch("prtcl_pdgid",     &mPDGID    )->SetAutoDelete(kFALSE);
    mTree->Branch("prtcl_pt",        &mPt       )->SetAutoDelete(kFALSE);
    mTree->Branch("prtcl_eta",       &mEta      )->SetAutoDelete(kFALSE);
    mTree->Branch("prtcl_phi",       &mPhi      )->SetAutoDelete(kFALSE);
    mTree->Branch("prtcl_e",         &mE        )->SetAutoDelete(kFALSE);
    /* Particle lvl for particles not in jets (nij) */
    mTree->Branch("prtclnij_pdgid",  &mNIJPDGID )->SetAutoDelete(kFALSE);
    mTree->Branch("prtclnij_pt",     &mNIJPt    )->SetAutoDelete(kFALSE);
    mTree->Branch("prtclnij_eta",    &mNIJEta   )->SetAutoDelete(kFALSE);
    mTree->Branch("prtclnij_phi",    &mNIJPhi   )->SetAutoDelete(kFALSE);
    mTree->Branch("prtclnij_e",      &mNIJE     )->SetAutoDelete(kFALSE);
    #endif
    /* Isolation info is saved as partons, even though these are final-state particles. */
    mTree->Branch("isolation",       &mIsolation)->SetAutoDelete(kFALSE);
    /* Parton lvl */
    mTree->Branch("prtn_jet",        &mPJetId   )->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_ptn",        &mPPtnId   )->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_own",        &mPOwnId   )->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_pdgid",      &mPPDGID   )->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_tag",        &mPTag     )->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_pt",         &mPPt      )->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_eta",        &mPEta     )->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_phi",        &mPPhi     )->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_e",          &mPE       )->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_dr",         &mDR       )->SetAutoDelete(kFALSE);
    /* Jet lvl */
    mTree->Branch("jet_pt",          &mJPt      )->SetAutoDelete(kFALSE);
    mTree->Branch("jet_eta",         &mJEta     )->SetAutoDelete(kFALSE);
    mTree->Branch("jet_phi",         &mJPhi     )->SetAutoDelete(kFALSE);
    mTree->Branch("jet_e",           &mJE       )->SetAutoDelete(kFALSE);
    mTree->Branch("jet_ptn",         &mJPtnId   )->SetAutoDelete(kFALSE);
    mTree->Branch("jet_constituents",&mConst    )->SetAutoDelete(kFALSE);
    mTree->Branch("jet_ptd",         &mPTD      )->SetAutoDelete(kFALSE);
    mTree->Branch("jet_sigma2",      &mSigma2   )->SetAutoDelete(kFALSE);

    mTree->Branch("met",&mMet)->SetAutoDelete(kFALSE);

    mTree->BranchRef();

    if (cfg::DOD0 and cfg::DOD0RIIC) { // D0RunIIConePlugin params: R_cone, min_jet_Et, split_ratio
        mPlugin = new fastjet::D0RunIIConePlugin(cfg::RCone,6,0.5);
        mJetDef = fastjet::JetDefinition(mPlugin);
    } else { // Normal anti-kt, JetDefinition params: algorithm,radius,recombination scheme,strategy
        mJetDef = fastjet::JetDefinition(fastjet::antikt_algorithm, cfg::RCone, fastjet::E_scheme, fastjet::Best);
    }

    /* Setup a custom event timer */
    mTimerStep = 1000;
    mTimer.setParams(mNumEvents,mTimerStep);
    mTimer.startTiming();
} // Pythia6Jets


void Pythia6Jets::ModeSettings() {
    cerr << "  Initializing Pythia6 ..." << endl;
    if (mMode == 1 or mMode == 0) {
        cerr << "  Generating standard QCD events." << endl;
        mPythia->SetMSEL(1);
        // Min and max pthat
        if (cfg::DOD0) {
            mPythia->SetCKIN(3,5);
            mPythia->SetCKIN(4,980);
        } else {
            mPythia->SetCKIN(3,20);
            mPythia->SetCKIN(4,3000);
        }
        // Cut phase-space to produce mostly |eta_jet|<0.4
        if (cfg::PSCut) {
            mPythia->SetCKIN(13,-3.2);  //Min eta for 2->2 prod. w/ highest eta
            mPythia->SetCKIN(14, 3.2);  //Max -||-^
            mPythia->SetCKIN(15,-3.2);  //Min eta for 2->2 prod. w/ lowest eta
            mPythia->SetCKIN(16, 3.2);  //Max -||-^
        }
    } else if (mMode == 2) {
        cerr << "  Generating photon+jet events." << endl;
        // One could approach with pythia->SetMSEL(10), but this includes gamma-gamma bkg
        mPythia->SetMSEL(0);                     // Everything off
        mPythia->SetMSUB(14,1);                  // qq -> ggamma
        mPythia->SetMSUB(29,1);                  // qg -> qgamma
        if (!cfg::DOD0) mPythia->SetMSUB(115,1); // gg -> ggamma (NOT IN D0 JES)
        // Min and max pthat
        if (cfg::DOD0) {
            mPythia->SetCKIN(3,5);
            mPythia->SetCKIN(4,980);
        } else {
            mPythia->SetCKIN(3,20);
            mPythia->SetCKIN(4,3000);
        }
        // Phase-space parton level eta cut. Bad idea, left here for reference
        // Larger phase-space region due to |eta_gamma|<1.0
        if (cfg::PSCut) {
            mPythia->SetCKIN(13,-3.2);  //Min eta for 2->2 prod. w/ highest eta
            mPythia->SetCKIN(14, 3.2);  //Max -||-^
            mPythia->SetCKIN(15,-3.2);  //Min eta for 2->2 prod. w/ lowest eta
            mPythia->SetCKIN(16, 3.2);  //Max -||-^
        }
    } else if (mMode == 3) {
        cerr << "  Generating Zmumu+jet events." << endl;
        mPythia->SetMSEL(13);      // 11 would be the vanilla y*/Z, not considering next steps
        // Leave only decay to muons on
        mPythia->SetMDME(174,1,0); // Z decay to d dbar
        mPythia->SetMDME(175,1,0); // Z decay to u ubar
        mPythia->SetMDME(176,1,0); // Z decay to s sbar
        mPythia->SetMDME(177,1,0); // Z decay to c cbar
        mPythia->SetMDME(178,1,0); // Z decay to b bbar
        mPythia->SetMDME(179,1,0); // Z decay to t tbar
        mPythia->SetMDME(182,1,0); // Zee
        mPythia->SetMDME(183,1,0); // Znuenue
        mPythia->SetMDME(184,1,1); // Zmumu
        mPythia->SetMDME(185,1,0); // Znumunumu
        mPythia->SetMDME(186,1,0); // Ztautau
        mPythia->SetMDME(187,1,0); // Znutaunutau
        // Min and (no) max mhat to keep virtual photons and off-shell cases in check
        mPythia->SetCKIN(1,40);
        mPythia->SetCKIN(2,-1);
        // Min and max pthat
        if (cfg::DOD0) {
            mPythia->SetCKIN(3,5);
            mPythia->SetCKIN(4,980);
        } else {
            mPythia->SetCKIN(3,20);
            mPythia->SetCKIN(4,3000);
        }
    } else if (mMode == 4) {
        cerr << "  Generating ttbar lepton+jets events." << endl;
        mPythia->SetMSEL(6);    // choose top quark
        mPythia->SetMSUB(81,1); // qqbar -> qqbar
        mPythia->SetMSUB(82,1); // gg->qqbar
        mPythia->SetPMAS(6,1,cfg::P6TMass);
        // Ambitious from the pthat perspective, but ttbar generation has no low-pthat divergence
        mPythia->SetCKIN(3,0);
        mPythia->SetCKIN(4,7000);
    } else {
        throw std::runtime_error("The selected mode is nonsense");
    }
} // ModeSettings


void Pythia6Jets::GeneralSettings() {
    mPythia->SetMSTU(21,2);          // Check for errors (the hard way! - default)
    mPythia->SetMSTJ(22,2);          // Unstable particle decay: (according to lifetime)
    mPythia->SetPARJ(71,cfg::P6CTau);// Length-scale for distinguishing "stable" particles (c*tau)
    mPythia->SetMSTP(33,0);          // no K factors in hard cross sections (this is the default value)
    mPythia->SetMSTP(2,1);           // which order running alphaS (this is also the default value)
    mPythia->SetMSTP(51,10042);      // Structure function (PDF CTEQ6L1)
    mPythia->SetMSTP(52,2);          // LHAPDF

    // Turn on Pt reweighting: note that ttbar events have such a nice profile, that we exclude them
    if (cfg::P6PthatW and mMode!=4) {
        cerr << "  Using pthat weighting!" << endl;
        mPythia->SetMSTP(142,2);
    }

    if (cfg::P6Tune == 0) {
        /* Z2*, a classic CMS tune */
        cerr << "  Z2* tune has been selected." << endl;
        mPythia->SetPARP(82,1.921); // pt cutoff, multiparton interactions
        mPythia->SetPARP(89,1800.); // sqrts for which parp82 is set
        mPythia->SetPARP(90,0.227); // MPI: rescaling power

        mPythia->SetMSTP(95,6);     // Color reconnection setParams
        mPythia->SetPARP(77,1.016); // CR
        mPythia->SetPARP(78,0.538); // CR

        mPythia->SetPARP(80,0.1);   // Prob. colored parton from BBR

        mPythia->SetPARP(83,0.356); // MPI matter distribution
        mPythia->SetPARP(84,0.651); // MPI matter distribution

        mPythia->SetPARP(62,1.025); // ISR cutoff

        mPythia->SetMSTP(91,1);     // Gaussian primordial KT
        mPythia->SetMSTP(93,10.0);  // Primordial KT-max

        mPythia->SetMSTP(81,21);    // MPI
        mPythia->SetMSTP(82,4);     // MPI model
    } else if (cfg::P6Tune == 1) {
        /* cuep6s1 tune by CMS, the newest but not used that much */
        cerr << "  CUEP6S1 tune has been selected." << endl;
        mPythia->SetPARP(82,1.9096); // pt cutoff, multiparton interactions
        mPythia->SetPARP(89,1800.);  // sqrts for which parp82 is set
        mPythia->SetPARP(90,0.2479); // MPI: rescaling power

        mPythia->SetMSTP(95,6);      // Color reconnection setParams
        mPythia->SetPARP(77,0.6646); // CR
        mPythia->SetPARP(78,0.5454); // CR

        mPythia->SetPARP(80,0.1);    // Prob. colored parton from BBR

        mPythia->SetPARP(83,0.8217); // MPI matter distribution
        mPythia->SetPARP(84,0.651);  // MPI matter distribution

        mPythia->SetPARP(62,1.025);  // ISR cutoff

        mPythia->SetMSTP(91,1);      // Gaussian primordial KT
        mPythia->SetMSTP(93,10.0);   // Primordial KT-max

        mPythia->SetMSTP(81,21);     // MPI
        mPythia->SetMSTP(82,4);      // MPI model

        mPythia->SetPARJ(1,0.08);    // HAD diquark suppression
        mPythia->SetPARJ(2,0.21);    // HAD strangeness suppression
        mPythia->SetPARJ(3,0.94);    // HAD strange diquark suppression
        mPythia->SetPARJ(4,0.04);    // HAD vectior diquark suppression
        mPythia->SetPARJ(11,0.35);   // HAD P(vector meson), u and d only
        mPythia->SetPARJ(12,0.35);   // HAD P(vector meson) contains
        mPythia->SetPARJ(13,0.54);   // HAD P(vector meson), heavy quarks
        mPythia->SetPARJ(21,0.34);   // HAD fragmentation pt
        mPythia->SetPARJ(25,0.63);   // HAD eta0 suppression
        mPythia->SetPARJ(26,0.12);   // HAD eta0 suppression
    } else if (cfg::P6Tune == 2) {
        cerr << "  TuneA (Tevatron) has been selected." << endl;
        mPythia->SetMSTP(5,100);     // Tune A, heaviliy used at the tevatron
    } else {
        throw std::logic_error(Form("Unsupported value cfg::P6Tune = %d",cfg::P6Tune));
    }

    if (cfg::P6NoISR) {
        mPythia->SetMSTP(61,0);
        cerr << "  WARNING: ISR has been turned off!" << endl;
    }
    if (cfg::P6NoFSR) {
        mPythia->SetMSTP(71,0); // FSR off
        cerr << "  WARNING: FSR has been turned off!" << endl;
    }
    if (cfg::P6NoMPI) {
        mPythia->SetMSTP(81,0); // MPI off
        cerr << "  WARNING: MPI has been turned off!" << endl;
    }

    mPythia->Initialize("cms", "p", (cfg::DOD0) ? "pbar" : "p", cfg::P6Energy);
} // GeneralSettings


void Pythia6Jets::EventLoop()
{
    if (!mInitialized) {
        cerr << "Event loop can be performed only after proper initialization" << endl;
        return;
    }

    /* The event loop */
    unsigned evtNo = 0;
    unsigned attempts = 0;
    while (evtNo != mNumEvents) {
        mPythia->GenerateEvent();
        ++attempts;
        if (!ParticleLoop()) continue;
        if (!IsolationProc()) continue;
        if (!JetLoop()) continue;
        if (!IsolationFromJets()) continue;

        mTree->Fill();

        ++evtNo;
        if (cfg::Debug) cout << endl << "Event : " << evtNo << " passed!" << endl << endl;
        if (cfg::DebugList) mPythia->Pylist(2);
        if (evtNo%mTimerStep==0) mTimer.printTime();
    }
    if (cfg::DebugStat) mPythia->Pystat(1);

    mFile = mTree->GetCurrentFile();
    mTree->AutoSave("Overwrite");
    mFile->Close();

    if (mCounter != 0) cerr << "Non-zero counter value: " << mCounter << endl;
    if (attempts > 0) cerr << "Chosen " << mNumEvents << " events of " << attempts << " with a rate of " << double(mNumEvents)/double(attempts) << endl;

    delete mPythia;
    mPythia = 0;
    mInitialized = false;
} // EventLoop


bool Pythia6Jets::SanityChecks()
{
    /* This is a store for the original hard process parton locations.
       Comes in handy, since the daughters of the Hard Process are unknown, but the mothers
       from a hard process can be traced back. */
    mHPPartons.clear();
    /* This is a store for the original b partons for checking ancestry. */
    mBs.clear();

    if (mMode==0 or mMode==1) { /* QCD index check */
        if (!IsPartonIdx(7) or !IsPartonIdx(8)) throw std::runtime_error("Did not find two outgoing hard process partons! :(");
        mHPPartons.push_back(7);
        mHPPartons.push_back(8);
    } else if (mMode==2) {      /* Gamma+jets index check */
        bool part7 = IsPartonIdx(7);
        bool part8 = IsPartonIdx(8);
        bool gamm7 = mPythia->GetK(7,2)==22;
        bool gamm8 = mPythia->GetK(8,2)==22;
        if      (!part7 and !part8) throw std::runtime_error("No outgoing hard process parton! :(");
        else if (!gamm7 and !gamm8) throw std::runtime_error("No outgoing hard process photon! :(");
        if      (part7) mHPPartons.push_back(7);
        else if (part8) mHPPartons.push_back(8);
    } else if (mMode==3) {      /* Zmumu+jets index check */
        bool part7 = IsPartonIdx(7);
        bool part8 = IsPartonIdx(8);
        bool    Z7 = mPythia->GetK(7,2)==23;
        bool    Z8 = mPythia->GetK(8,2)==23;
        if      (!part7 and !part8) throw std::runtime_error("No outgoing hard process parton! :(");
        else if (!Z7    and !Z8   ) throw std::runtime_error("No outgoing hard process Zboson! :(");
        if      (part7) mHPPartons.push_back(7);
        else if (part8) mHPPartons.push_back(8);
    } else if (mMode==4) {      /* TTbar lepton+jets index check */
        /* We look cleverly at the indices 13-16, which should be the W descendants */
        unsigned partno = 0, leptno = 0;
        for (unsigned prt = 13; prt <= 16; ++prt) {
            if (IsPartonIdx(prt)) {
                ++partno;
                mHPPartons.push_back(prt);
            } else {
                int absid = abs(mPythia->GetK(prt,2));
                if (absid < 20) {
                    ++leptno;
                    if (absid==15) { // Tau case
                    }
                } else throw std::runtime_error("Unexpected W descendants");
            }
        }
        /* Ensure that there are exactly two leptons and exactly two partons: lepton+jets events. */
        if (partno!=2 or leptno!=2) return false;

        /* Check that W's are in the right place */
        if (abs(mPythia->GetK(9,2))!=24 or abs(mPythia->GetK(11,2))!=24)
            throw std::runtime_error("Incorrect W locations");
        /* Check if we have a b corresponding to W1 */
        if (abs(mPythia->GetK(10,2))!=5) {
            if (IsPartonIdx(10)) return false; /* Sometimes there is another parton instead of b: this is ok */
            else                 throw std::runtime_error("Incorrect b1 location");
        } else {
            mHPPartons.push_back(10);
            mBs.push_back(10);
        }

        /* Check if we have a b corresponding to W2 */
        if (abs(mPythia->GetK(12,2))!=5) {
            if (IsPartonIdx(12)) return false; /* Sometimes there is another parton instead of b: this is ok */
            else                 throw std::runtime_error("Incorrect b2 location");
        } else {
            mHPPartons.push_back(12);
            mBs.push_back(12);
        }
        if (abs(mPythia->GetK(13,3))!=9  or abs(mPythia->GetK(14,3))!=9 )
            throw std::runtime_error("Incorrect W1 descendants");
        if (abs(mPythia->GetK(15,3))!=11 or abs(mPythia->GetK(16,3))!=11)
            throw std::runtime_error("Incorrect W2 descendants");
    }

    return true;
}


bool Pythia6Jets::InitEvent()
{
    /* Special particle indices are saved to eliminate saving overlap. */
    mSpecialIndices.clear();
    if (mMode==2) {
        /* All photons to be considered as potential signal photons. */
        mGammas.clear();
    } else if (mMode==3) {
        /* All muons to be considered as potential signal muons. */
        mMuons.clear();
    } else if (mMode==4) {
        /* All charged leptons to be considered as potential signal leptons. */
        mChargeds.clear();
        /* counters etc. */
        mBNuCount = 0;
        mONuCount = 0;
        mNuOB     = 0;
        mNuOC     = 0;
        mNuOLept  = 0;
        mNuOOther = 0;
        mNuB      = 0;
        mNuC      = 0;
        mNuLept   = 0;
        mNuOther  = 0;

        mHadronFriend = 0;
        mWs.clear();
    }
    /* Interpreted as a group of boolean flags for each event. Default: no additional info (0). */
    mInfo = 0;

    /* Parton lvl containers refreshed */
    mPJetId.clear();
    mPPtnId.clear();
    mPOwnId.clear();
    mPPDGID.clear();
    mPTag.  clear();
    mPPt.   clear();
    mPEta.  clear();
    mPPhi.  clear();
    mPE.    clear();
    mDR.    clear();

    /* These can be utilized in the rejection routines. */
    mPartonInfo.clear();

    /* Event-type specific rejections: */
    if      (mMode==2) { if (!GammaAdd())  return false; } // Gamma+jets
    else if (mMode==3) { if (!MuonAdd())   return false; } // Zmumu+jets
    else if (mMode==4) { if (!LeptonAdd()) return false; } // TTbarlepton+jets

    /* All the special steps & rejections have been performed (and survived): now we continue to refresh the bulk of containers. */
    mIsolation.clear();

    /* Particle lvl containers refreshed */
    #ifdef STORE_PRTCLS
    /* Particle lvl (jets) */
    mJetId.clear();
    mPDGID.clear();
    mPt.   clear();
    mEta.  clear();
    mPhi.  clear();
    mE.    clear();
    /* Particle lvl for particles not in jets (nij) */
    if (cfg::StoreNIJ) {
        mNIJPDGID.clear();
        mNIJPt   .clear();
        mNIJEta  .clear();
        mNIJPhi  .clear();
        mNIJE    .clear();
    }
    #endif

    /* Jet lvl containers refreshed */
    mJPt.clear();
    mJEta.clear();
    mJPhi.clear();
    mJE.clear();
    mConst.clear();
    mPTD.clear();
    mSigma2.clear();

    mJetInputs.clear();
    mSortedJets.clear();

    if (cfg::DOCorrParton) mPartonHistory.clear();

    mPtHat  = mPythia->GetPARI(17);
    mWeight = 1./mPythia->GetVINT(99);

    mMetVect = fastjet::PseudoJet();
    mNumErrs = 0;

    return true;
}


bool Pythia6Jets::ParticleLoop()
{
    /* Pythia 6 speciality: do the gamma/muon/top analysis before the complete particle loop. */

    if (!SanityChecks()) return false;

    if (cfg::DebugPartons) {
        for (unsigned prt = 1; prt <= 30; ++prt) {
            cout << prt << " ";
            for (unsigned i = 1; i <= 5; ++i)
                cout << mPythia->GetK(prt,i) << " ";
            cout << endl;
        }
        cout << endl;
    }

    if (!InitEvent()) return false;

    /* Particle loop: all particles */
    for (unsigned prt = 1, N = mPythia->GetN(); prt <= N; ++prt)
        if (!ProcessParticle(prt)) return false;

    /* The ideal MET taken from neutrinos. */
    mMet = mMetVect.perp();

    /* Adding corrected parton momenta */
    if (cfg::DOCorrParton) {
        for (auto &prt : mPartonHistory) PartonAppend(PseudoJettify(prt.second),prt.first,1);
    }

    return true;
} // ParticleLoop


bool Pythia6Jets::IsExcitedHadronState(unsigned int prt, int quarkId)
{
    for (unsigned dtr = mPythia->GetK(prt,4), N = mPythia->GetK(prt,5); dtr <= N; ++dtr)
        if (HadrFuncs::StatusCheck(quarkId, abs(mPythia->GetK(dtr,2)))) return true;
    return false;
}

bool Pythia6Jets::IsAncestor(unsigned int ancestor, unsigned int descendant)
{
    int moth = mPythia->GetK(descendant,3);
    while (moth != 0) {
        if (moth == ancestor) return true;
        descendant = moth;
        moth = mPythia->GetK(descendant,3);
    }
    return false;
}

bool Pythia6Jets::IsHPPartonIdx(unsigned int prt)
{
    return std::find(mHPPartons.begin(),mHPPartons.end(),prt)!=mHPPartons.end();
}

/* Returns true, if particle prt has already been recorded (in mSpecialIndices). */
inline bool Pythia6Jets::Absent(unsigned int prt)
{
    return std::find(mSpecialIndices.begin(),mSpecialIndices.end(),prt)==mSpecialIndices.end();
}


///////////////////////////////////////////////////
// Event type specific ProcessParticle functions //
///////////////////////////////////////////////////

/* Inserting ghost partons into jet inputs */
void Pythia6Jets::PartonAppend(fastjet::PseudoJet p4, unsigned prt, int tag, int ptnid, int ownid)
{
    mPartonInfo.push_back(PartonHolder{p4, mPythia->GetK(prt,2), tag, ptnid, ownid, false});
    p4 *= cfg::GhostCoeff; // Make ghost four-momentum negligible, but conserve direction
    p4.set_user_index(-mPartonInfo.size());
    mJetInputs.push_back(p4);
}

/* In ttbar events we want to indicate, if the actual parent is a W boson. */
inline int Pythia6Jets::PartonProc(unsigned int prt)
{
    if (mMode==4 and abs(mPythia->GetK(prt,2))!=5 and mHadronFriend>0) return mHadronFriend;
    return -1;
}

void Pythia6Jets::PartonDescend(unsigned int prt)
{
    if (IsPreFSPartonIdx(prt)) {
        /* NLO-level partons (tree). */
        for (unsigned dtr = mPythia->GetK(prt,4), N = mPythia->GetK(prt,5); dtr <= N; ++dtr) {
            auto part = PseudoJettify(dtr);
            if (part.pt()<4) continue;
            PartonAppend(PseudoJettify(dtr),dtr,4,prt,dtr);
            PartonDescend(dtr);
        }
    }
}


/* Generic routine for inspecting single particles within an event. */
bool Pythia6Jets::ProcessParticle(unsigned prt)
{
    int status =     mPythia->GetK(prt,1);
    int absid  = abs(mPythia->GetK(prt,2));
    bool isparton = IsPartonIdx(prt);
    bool ishadron = !isparton and IsHadron(absid);
    auto parent = mPythia->GetK(prt,3);

    if (isparton) {
        /* Look at the partons */
        if (IsHPPartonIdx(prt)) { /* Add the Hard Process partons. */
            if (cfg::DOCorrParton) mPartonHistory.emplace(prt,TLorentzVector());

            int ptnid = PartonProc(prt); // Find out if W is the father
            PartonAppend(PseudoJettify(prt),prt,0,ptnid,prt);
            if (cfg::Debug) {
                TLorentzVector p4 = TLorentzify(prt);
                cout << endl << "Parton HP: " << "E=" << p4.E() <<  ", pT=" << p4.Pt()
                     << ", eta=" << p4.Eta() << ", phi=" << p4.Phi()
                     << "idx " << prt << " id " << mPythia->GetK(prt,2) << endl;
            }
        } else if (IsHPPartonIdx(parent)) { /* The descendants of the HP partons. */
            if (cfg::DOCorrParton) {
                /* An attempt to correct the Hard Process parton momenta. */
                bool dummy;
                mPartonHistory[parent] += LastParton(prt,dummy);
            }
            PartonAppend(PseudoJettify(prt),prt,4,parent,prt);
            PartonDescend(prt);
            if (cfg::Debug) {
                TLorentzVector p4 = TLorentzify(prt);
                cout << endl << "Parton HP: " << "E=" << p4.E() <<  ", pT=" << p4.Pt()
                     << ", eta=" << p4.Eta() << ", phi=" << p4.Phi()
                     << "idx " << prt << " id " << mPythia->GetK(prt,2) << endl;
            }
        }
        return true;
    } else if (mMode==4 and ishadron and HadrFuncs::StatusCheck(5, absid) and !IsExcitedHadronState(prt,5)) {
        /* Ghost b hadrons, not necessarily the final state. */
        PartonAppend(PseudoJettify(prt),prt,9);
    }

    if (status <= 10) {
        /* Stable particles. */
        auto part = PseudoJettify(prt);
        /* Set the user index to pdgid, except let this be 20 for pi0 photons */
        part.set_user_index(prt);
        if (absid==12 or absid==14 or absid==16) {
            /* Ideal MET: only neutrinos */
            mMetVect += part;
            /* Special ttbar event measures. */
            if (mMode==4 and Absent(prt)) {
                /* Neutrino catcher */
                bool bjetnu = false;
                for (auto &b : mBs) {
                    if (IsAncestor(b,prt)) {
                        if (absid==16) {
                            if (mInfo & 2) mInfo |= 4;
                            else           mInfo |= 2;
                        }
                        bjetnu = true;
                        break;
                    }
                }
                /* What kind of a monster is the origin of this neutrino? */
                int parentid = abs(mPythia->GetK(parent,2));
                int parenttype = parentid/100;
                while (parenttype>9) parenttype /= 10;
                if (bjetnu) { /* b jet neutrinos */
                    if (parenttype==5)
                        ++mNuB;
                    else if (parenttype==4)
                        ++mNuC;
                    else if (IsLepton(parentid))
                        ++mNuLept;
                    else
                        ++mNuOther;
                    ++mBNuCount;
                    PartonAdd(prt,-1,7);
                } else { /* neutrinos from other sources */
                    if (parenttype==5)
                        ++mNuOB;
                    else if (parenttype==4)
                        ++mNuOC;
                    else if (IsLepton(parentid))
                        ++mNuOLept;
                    else
                        ++mNuOOther;
                    PartonAdd(prt,-1,8);
                    ++mONuCount;
                }
            }
        } else {
            /* Special particles ignored, unless we are using Toni's special processing. */
            /* Neutrinos ignored always. This is supposed to mimick the particles observed
             * by a particle detector, such as the CMS or D0. */

            if (cfg::StoreNIJ or Absent(prt)) {
              if (mMode==4 and cfg::DOMuons and absid==13) PartonAdd(prt,-1,10);
              else if (!cfg::StoreNIJ)                     mJetInputs.push_back(part);
            }
            /* Add particles for isolation studies! */
            if (part.pt() > cfg::NuisancePt and fabs(part.eta()) > cfg::NuisanceEta) {
                if (mMode==2) {        /* All nice photons. */
                    if (absid==22)       mGammas.push_back  (std::make_pair(part,prt));
                } else if (mMode==3) { /* All nice muons. */
                    if (absid==13)       mMuons.push_back   (std::make_pair(part,prt));
                } else if (mMode==4) { /* All nice charged leptons (neutrinos are excluded). */
                    if (IsLepton(absid)) mChargeds.push_back(std::make_pair(part,prt));
                }
            }
        }
    }

    return true;
} // ProcessParticle


bool Pythia6Jets::JetLoop()
{
    fastjet::ClusterSequence clustSeq(sorted_by_pt(mJetInputs), mJetDef);
    vector<fastjet::PseudoJet> unsorted = clustSeq.inclusive_jets(cfg::MinJetClustPt);
    /* Check that enough jets are found */
    if      (mMode==1) { if (unsorted.size() <2) return false; }
    else if (mMode==2) { if (unsorted.size()==0) return false; }
    else if (mMode==3) { if (unsorted.size()==0) return false; }
    else if (mMode==4) { if (unsorted.size() <4) return false; }
    mSortedJets = sorted_by_pt(unsorted);

    int bJets = 0;     /* #B jets */
    int eta04Jets = 0; /* Jets in |eta|<0.4 */

    for (unsigned jet = 0; jet<mSortedJets.size(); ++jet) {
        JetAdd(jet);
        if (cfg::EtaCut and fabs(mSortedJets[jet].eta())<0.4) ++eta04Jets;
        for (auto &part : mJetParts) {
            int prt = part.user_index();
            if (prt<0) {
                /* Adding uncorrected and corrected parton momentum */
                prt *= -1;
                --prt;
                PartonAdd(prt,jet);
                mPartonInfo[prt].used = true;
                if (cfg::BEnriched and abs(mPartonInfo[prt].id)==5) ++bJets;
            } else {
                #ifdef STORE_PRTCLS
                ParticleAdd(prt,jet);
                #endif
            }
        }
    }

    /* Require a min amount of jets in |eta|<0.4 for D0 JES studies
     * The minimum is at least 2 for dijet and 1 for gamma+jet */
    if (cfg::EtaCut) {
        if      (mMode==1) { if (eta04Jets<2) return false; }
        else if (mMode==2) { if (eta04Jets<1) return false; }
        else if (mMode==3) { if (eta04Jets<1) return false; }
        else if (mMode==4) { if (eta04Jets<4) return false; }
    }
    /* Cut events with too high-pT excess jets:
     * 11 GeV cut based on checking the gen lvl pT of jets cut in respAna
     * (cutting reco'd jets, then asking the corresponding gen pT) */
    if (cfg::PtCut) {
        if      (mMode==1) { if (mSortedJets.size()>2 and mSortedJets[2].pt() > 11) return false; }
        else if (mMode==2) { if (mSortedJets.size()>1 and mSortedJets[1].pt() > 11) return false; }
        else if (mMode==3) { if (mSortedJets.size()>1 and mSortedJets[1].pt() > 11) return false; }
        else if (mMode==4) { if (mSortedJets.size()>4 and mSortedJets[4].pt() > 11) return false; }
    }
    if (cfg::BEnriched and bJets==0) return false;

    #ifdef STORE_PRTCLS
    /* Process the particles that do not belong to a jet:
     * - They have "jet index" -1 in particle_jet_indices
     * - Add the particles to a "particles not in jets (nij)" -level,
     *   separately from the particles associated with jets. */
    if (cfg::StoreNIJ) {
        vector<int> jetIndices = clustSeq.particle_jet_indices(mSortedJets);
        for (int a=0; a!=jetIndices.size(); ++a) {
            int prt = mJetInputs[a].user_index();
            if (prt > 0 and jetIndices[a] == -1) ParticleAdd(prt);
        }
    }
    #endif

    /* Adding the partons lacking a jet association */
    for (unsigned prt = 0; prt < mPartonInfo.size(); ++prt) {
        if (mPartonInfo[prt].used) continue;
        PartonAdd(prt,-1);
    }

    return true;
} // JetLoop


bool Pythia6Jets::IsolationFromJets()
{
    if (mMode==2) {
        if (mGammas.size() != mIsolation.size()) throw std::logic_error("Isolation an gammas dimension mismatch");

        for (unsigned gidx = 0; gidx < mGammas.size(); ++gidx) {
            if (!mIsolation[gidx]) continue; /* If this is non-isolated, don't bother. */

            auto &gm = mGammas[gidx].first;
            for (auto &jet : mSortedJets) {
                if (jet.pt() < cfg::MinJetVisiblePt) break;
                if (gm.delta_R(jet) < 0.5) {
                    mIsolation[gidx] = false;
                    break;
                }
            }
        }
    } else if (mMode==3) {
        if (mMuons.size() != mIsolation.size()) throw std::logic_error("Isolation an gammas dimension mismatch");

        for (unsigned midx = 0; midx < mMuons.size(); ++midx) {
            if (!mIsolation[midx]) continue; /* If this is non-isolated, don't bother. */

            auto &mu = mMuons[midx].first;
            for (auto &jet : mSortedJets) {
                if (jet.pt() < cfg::MinJetVisiblePt) break;
                if (mu.delta_R(jet) < 0.5) {
                    mIsolation[midx] = false;
                    break;
                }
            }
        }
    } else if (mMode==4) {
        if (mChargeds.size() != mIsolation.size()) throw std::logic_error("Isolation an gammas dimension mismatch");

        for (unsigned cidx = 0; cidx < mChargeds.size(); ++cidx) {
            if (!mIsolation[cidx]) continue; /* If this is non-isolated, don't bother. */

            auto &ch = mChargeds[cidx].first;
            for (auto &jet : mSortedJets) {
                if (jet.pt() < cfg::MinJetVisiblePt) break;
                if (ch.delta_R(jet) < 0.5) {
                    mIsolation[cidx] = false;
                    break;
                }
            }
        }
    }

    return true;
} // IsolationFromJets


/* for relevant D0 isolation settings, see:
 *            - muons:             https://arxiv.org/pdf/1307.5202.pdf
 *            - electrons/photons: https://arxiv.org/pdf/1401.0029.pdf */
inline bool Pythia6Jets::IsolationProc()
{
    if (mMode==2) {
        for (auto &gm : mGammas) {
            mIsolation.push_back(IsolationPhotons(gm.second,gm.first));
            PartonAdd(gm.second,-1,6);
        }
    } else if (mMode==3) {
        for (auto &mu : mMuons) {
            mIsolation.push_back(IsolationMuons(mu.second,mu.first));
            PartonAdd(mu.second,-1,6);
        }
    } else if (mMode==4) {
        /* Adding Isolation monitoring for all present charged particles */
        for (auto &ch : mChargeds) {
            mIsolation.push_back(IsolationLeptons(ch.second,ch.first));
            PartonAdd(ch.second,-1,6);
        }
    }

    return true;
}

/* Notation: (i)prt the index of a particle, (i)part the corresponding PseudoJet.
 * The prefix 'i' stands for isolation (i.e. the particle which we study). */
bool Pythia6Jets::IsolationPhotons(int iprt, fastjet::PseudoJet& ipart)
{
    if (cfg::DOD0) { /* D0 JES style algorithm (implemented on gen lvl) */
        /* Kinematic basic cuts for the particle to-be-isolated: */
        if (abs(ipart.eta()) > 1.0 or ipart.pt() < 7.0) return false;

        double E_tot03             = 0.0; /* Stepper to sum all ptcl E within     R<0.3 cone */
        double E_tot04             = 0.0; /*              -||-                    R<0.4 cone */
        double E_EM02              = 0.0; /* Stepper to sum EM  ptcl E within     R<0.2 cone */
        double E_EM03              = 0.0; /*              -||-                    R<0.3 EM clus. */
        double pTsum               = 0.0; /* Stepper to sum track p_T within 0.05<R<0.4. */

        /* Adding the photon to-be-studied. */
        E_tot03 += ipart.e();
        E_tot04 += ipart.e();
        E_EM02  += ipart.e();
        E_EM03  += ipart.e();

        for (auto &part : mJetInputs) {
            int prt = part.user_index();
            if (prt<0 or prt==iprt) continue; /* Skip ghosts and the current particle */

            double dR = ipart.delta_R(part);
            if (dR < 0.4) {
                int absid = abs(mPythia->GetK(prt,2));
                bool ischarged = IsCharged(absid);

                if (ischarged and part.pt() > 0.5 and dR > 0.05) pTsum += part.pt();

                E_tot04 += part.e();
                if (dR<0.3) {
                    /* For charged particles & photons the coefficient is one and for other neutrals 0.5. */
                    double EMcoeff = (ischarged or absid == 22) ? 1.0 : 0.5;

                    E_tot03            +=         part.e();
                    E_EM03             += EMcoeff*part.e();
                    if (dR<0.2) E_EM02 += EMcoeff*part.e();
                }
            }
        }

        /* Pseudorapidity, p_T, etc. limits for isolated photon. Note: these are quite harsh limits. */
        return (E_tot04-E_EM02)/E_EM02 < 0.07 /* fiso      in the D0 language */
           and E_EM03/E_tot03          > 0.96 /* fEM       in the D0 language */
           and pTsum                   < 2.5; /* sum pttrk in the D0 language */
    } else { /* CMS STYLE ALGORITHM */
        double EdR = 0;

        for (auto part : mJetInputs) {
            int prt = part.user_index();
            if (prt<0 or prt==iprt) continue; /* Skip ghosts and the current particle */

            double dR = ipart.delta_R(part);
            if (dR < cfg::GammaDR) EdR += part.e();
        }

        return EdR/ipart.e() < cfg::GammaLim;
    }
} // IsolationPhotons

/* Notation: (i)prt the index of a particle, (i)part the corresponding PseudoJet.
 * The prefix 'i' stands for isolation (i.e. the particle which we study). */
bool Pythia6Jets::IsolationMuons(int iprt, fastjet::PseudoJet& ipart)
{
    if (cfg::DOD0) { /* D0 JES style algorithm (implemented on gen lvl) */
        double E_EM01              = 0.0; /* Stepper to sum EM  ptcl E within R<0.1 EM clus. */
        double E_EM04              = 0.0; /* Stepper to sum EM  ptcl E within R<0.4 EM clus. */
        double pTsumTrk            = 0.0; /* Stepper to sum track p_T within  R<0.5. */

        /* Adding the muon to-be-studied. */
        E_EM01 += ipart.e();
        E_EM04 += ipart.e();

        for (auto &part : mJetInputs) {
            int prt = part.user_index();
            if (prt<0 or prt==iprt) continue; /* Skip ghosts and the current particle */

                double dR = ipart.delta_R(part);
            if (dR < 0.5) {
                int absid = abs(mPythia->GetK(prt,2));
                bool ischarged = IsCharged(absid);
                if (ischarged and part.pt() > 0.4 and dR > 0.05) pTsumTrk += part.pt();

                if (dR < 0.4) {
                    /* For charged particles & photons the coefficient is one and for other neutrals 0.5. */
                    double EMcoeff = (ischarged or absid == 22) ? 1.0 : 0.5;

                    E_EM04             += EMcoeff*part.e();
                    if (dR<0.1) E_EM01 += EMcoeff*part.e();
                }
            }
        }

        /* Pseudorapidity, p_T, etc. limits for isolated electrons. */
        return pTsumTrk/ipart.pt()        < 0.1  /* scaled tight for track pt */
           and (E_EM04-E_EM01)/ipart.pt() < 0.1  /* scaled tight for EM clusters */
           and (E_EM04-E_EM01)            < 2.5  /* tight for track pt */
           and pTsumTrk                   < 2.5; /* tight for EM clusters */
        return true;
    } else { /* CMS STYLE ALGORITHM */
        double EdR = 0;

        for (auto &part : mJetInputs) {
            if (part.user_index()<0 or part.user_index()==iprt) continue;

            double dR = ipart.delta_R(part);
            if (dR < cfg::MuonDR) EdR += part.e();
        }

        return EdR/ipart.e() < cfg::MuonLim;
    }
} // IsolationMuons

/* Notation: (i)prt the index of a particle, (i)part the corresponding PseudoJet.
 * The prefix 'i' stands for isolation (i.e. the particle which we study). */
bool Pythia6Jets::IsolationLeptons(int iprt, fastjet::PseudoJet& ipart)
{
    if (cfg::DOD0) { /* D0 JES style algorithm (implemented on gen lvl) */
        int iabsid = abs(mPythia->GetK(iprt,2));

        if (iabsid==11) {
            /* Note: in principle the isolation of photons and electrons is similar.
             * However, we do not simply call IsolationPhotons here, since this function
             * embeds some special parametrizations used by Toni. */
            double E_tot04             = 0.0; /*              -||-                    R<0.4 cone */
            double E_EM02              = 0.0; /* Stepper to sum EM  ptcl E within     R<0.2 cone */
            double E_EM04              = 0.0; /*              -||-                    R<0.4 EM clus. */
            double pTsumTrk            = 0.0; /* Stepper to sum track p_T within 0.05<R<0.4. */

            /* Adding the charged lepton to-be-studied. */
            E_tot04 += ipart.e();
            E_EM02  += ipart.e();
            E_EM04  += ipart.e();

            for (auto &part : mJetInputs) {
                int prt = part.user_index();
                if (prt<0 or prt==iprt) continue; /* Skip ghosts and the current particle */

                double dR = ipart.delta_R(part);
                if (dR < 0.4) {
                    int absid = abs(mPythia->GetK(prt,2));
                    bool ischarged = IsCharged(absid);
                    if (ischarged and part.pt() > 0.5 and dR > 0.05) pTsumTrk += part.pt();

                    /* For charged particles & photons the coefficient is one and for other neutrals 0.5. */
                    double EMcoeff = (ischarged or absid == 22) ? 1.0 : 0.5;

                    E_tot04            +=         part.e();
                    E_EM04             += EMcoeff*part.e();
                    if (dR<0.2) E_EM02 += EMcoeff*part.e();
                }
            }

            /* Pseudorapidity, p_T, etc. limits for isolated electrons. */
            return (E_tot04-E_EM02)/E_EM02 < 0.07 /* fiso      in the D0 language */
               and E_EM04/E_tot04          > 0.9  /* fEM       in the D0 language */
               and pTsumTrk                < 2.5; /* sum pttrk in the D0 language */
        } else if (iabsid==13) {
            return IsolationMuons(iprt, ipart);
        } else {
            throw std::logic_error("Unknown form of a charged lepton observed.");
        }

        return true;
    } else { /* CMS STYLE ALGORITHM */
        double EdR = 0;
        bool isEl = abs(mPythia->GetK(iprt,2))==11;
        double dRlim = isEl ? cfg::ElDR : cfg::MuDR;
        double fraclim = isEl ? cfg::ElLim : cfg::MuLim;

        for (auto &part : mJetInputs) {
            if (part.user_index()<0 or part.user_index()==iprt) continue;

            double dR = ipart.delta_R(part);
            if (dR < dRlim) EdR += part.e();
        }

        return EdR/ipart.e() < fraclim;
    }
} // IsolationLeptons


bool Pythia6Jets::GammaAdd()
{
    // The ninth particle should be a photon (error, if not)
    mSpecialIndices.push_back(9);
    auto &prt = mSpecialIndices[0];
    while (mPythia->GetK(prt,2)!=22 and (mPythia->GetK(prt,3)==7 or mPythia->GetK(prt,3)==8)) ++prt;

    if (cfg::Debug) {
        TLorentzVector p4 = TLorentzify(prt);
        cout << "Photon (HP): E=" << p4.E() << ", pT=" << p4.Pt()
        << ", eta=" << p4.Eta() << ", phi=" << p4.Phi() << endl;
    }
    if (cfg::DOUnCorrLepton) PartonAdd(prt, (char)-1, 2);

    while (mPythia->GetK(prt,1)>10) // Go looking for the final-state photon
        prt = mPythia->GetK(mSpecialIndices[0],4);

    if (mPythia->GetK(mSpecialIndices[0],2)!=22)
        throw std::logic_error("The photon finder did not find a photon.");

    if (cfg::Debug) {
        cout << "Photon (FS): E=" << mGamma.e() << ", pt=" << mGamma.pt()
        << ", eta=" << mGamma.eta() << ", phi=" << mGamma.phi() << endl;
    }

    auto part = PseudoJettify(prt);
    mGammas.push_back(std::make_pair(part,prt));
    PartonAdd(mSpecialIndices[0], (char)-1, 3);
    mGamma = PseudoJettify(mSpecialIndices[0]);

    return true;
} // GammaAdd

bool Pythia6Jets::MuonAdd()
{
    // The twelwth and thirteenth particles (or later) are muons
    mSpecialIndices.push_back(12);
    // If the first particle is not a muon, progress first index
    int steps = 0;
    while (abs(mPythia->GetK(mSpecialIndices[0],2))!=13 and steps++<10) ++mSpecialIndices[0];
    mSpecialIndices.push_back(mSpecialIndices[0]+1);
    // If the second particle is not a muon, progress only its index
    while (abs(mPythia->GetK(mSpecialIndices[1],2))!=13 and steps++<10) ++mSpecialIndices[1];

    if (steps>10 or mSpecialIndices.size()!=2) throw std::logic_error("The muon finder did not find two muons.");

    // Go looking for the two final-state muons
    for (auto &prt : mSpecialIndices) {
        if (cfg::Debug) {
            TLorentzVector p4 = TLorentzify(prt);
            cout << "Muon" << prt << " (HP): E=" << p4.E() << ", pT=" << p4.Pt()
                 << ", eta=" << p4.Eta() << ", phi=" << p4.Phi() << endl;
        }
        if (cfg::DOUnCorrLepton) PartonAdd(prt, (char)-1, 2);

        while (mPythia->GetK(prt,1)>10) {
            bool muonfinding = false;
            for (auto probe = mPythia->GetK(prt,4); probe <= mPythia->GetK(prt,5); ++probe) {
                if (abs(mPythia->GetK(probe,2))==13) {
                    prt = probe;
                    muonfinding = true;
                    break;
                }
            }
            if (!muonfinding)
                throw std::logic_error("A muon is decaying into something not muonic.");
        }
        if (cfg::Debug) {
            TLorentzVector p4 = TLorentzify(prt);
            cout << "Muon" << prt << " (FS): E=" << p4.E() << ", pT=" << p4.Pt()
                 << ", eta=" << p4.Eta() << ", phi=" << p4.Phi() << endl;
        }

        auto part = PseudoJettify(prt);
        mMuons.push_back(std::make_pair(part,prt));
        PartonAdd(prt, (char)-1,3);
    }

    return true;
} // MuonAdd

bool Pythia6Jets::LeptonAdd()
{
    int hadrW = 0;
    int leptW = 0;

    /* Search for the W leptons in the HP */
    vector<int> lepts;
    for (unsigned lept = 13; lept <= 16; ++lept) {
        int id = mPythia->GetK(lept,2);
        if (!IsLepton(id)) continue;

        lepts.push_back(lept);
        if (cfg::Debug) {
            TLorentzVector p4 = TLorentzify(lept);
            cout << "Lepton " << lept << " (" << mPythia->GetK(lept,2) << ")" << " (HP): E=" << p4.E()
            << ", pT=" << p4.Pt() << ", eta=" << p4.Eta() << ", phi=" << p4.Phi() << endl;
        }
    }

    // Search for W's
    for (unsigned prt = 17; prt < 30 and (hadrW==0 or leptW==0); ++prt) {
        int parent = mPythia->GetK(prt,3);
        if (parent!=9 and parent!=11) continue;

        if (mPythia->GetK(lepts[0],3)==parent) {
            if (leptW != 0) throw std::logic_error("Double leptonic W");
            leptW = prt;
            PartonAdd(prt,(char)-1,(char)2,-1,prt);
        } else {
            if (hadrW != 0) throw std::logic_error("Double hadronic W");
            hadrW = prt;
            mHadronFriend = hadrW;
            PartonAppend(PseudoJettify(prt),prt,(char)2,-1,prt);
        }
        mWs.push_back(prt);
    }
    if (hadrW==0 or leptW==0 or lepts.size()!=2) throw std::logic_error("Somehow, we lost a W!");
    /* Save a raw charged lepton*/
    if (cfg::DOUnCorrLepton) {
        for (auto &lept : lepts) {
            if (abs(mPythia->GetK(lept,2)%2==1)) {
                PartonAdd(lept, (char)-1, 2, leptW);
            }
        }
    }

    /* Add gammas, put leptons in their place. */
    for (unsigned lept = mPythia->GetK(leptW,4); lept <= mPythia->GetK(leptW,5); ++lept) {
        int absid = abs(mPythia->GetK(lept,2));
        if (absid==22) {
            auto gamma = PseudoJettify(lept);
            if (gamma.pt()>0.1) PartonAdd(lept,(char)-1,(char)2,leptW,lept);
        } else if (IsLepton(absid)) {
            mSpecialIndices.push_back(lept);
        } else {
            throw std::logic_error("A bug in the lepton+jets logic.");
        }
    }

    /* Go looking for the two final-state leptons (important e.g. for tau decays) */
    for (unsigned prtidx = 0; prtidx < mSpecialIndices.size(); ++prtidx) {
        auto prt = mSpecialIndices[prtidx];

        int type = abs(mPythia->GetK(prt,2))%2;
        if (type) { /* Charged lepton input: find a final-state charged lepton. */
            if (abs(mPythia->GetK(prt,2))==15) { /* Is this a tau */
                mInfo |= 1;

                while (mPythia->GetK(prt,1)>9) {
                    auto prevprt = prt;
                    for (unsigned dtr = mPythia->GetK(prevprt,4); dtr <= mPythia->GetK(prevprt,5); ++dtr) {
                        unsigned dId = abs(mPythia->GetK(dtr,2));
                        unsigned dType = dId%2;
                        if (IsHadron(dId)) { /* Hadronic tau decay (25%-30% chance) */
                            if (cfg::Debug) cerr << "Hadronic tau decay!" << endl;
                            return false;
                        } else if (IsLepton(dId)) {
                            if (dType==1) prt = dtr;                      /* Charged lept: follow */
                                else          mSpecialIndices.push_back(dtr); /* Neutrino: push back */
                        } else {
                            throw std::logic_error("A bug in the lepton+jets logic.");
                        }
                    }

                    if (prt == prevprt) throw std::logic_error("Lepton loop stuck!");
                }
            } else if (mPythia->GetK(prt,1)>9) {
                throw std::logic_error("Non-tau leptons not supposed to decay!");
            }

            auto part = PseudoJettify(prt);
            mChargeds.push_back(std::make_pair(part,prt));
        } else {
            /* Neutrinos */
            if (mPythia->GetK(prt,1)>9) throw std::logic_error("Neutrinos not supposed to decay!");
        }

        if (cfg::Debug) {
            TLorentzVector p4 = TLorentzify(prt);
            cout << "Lepton" << prt << " (" << mPythia->GetK(prt,2) << ")" << " (FS): E=" << p4.E()
                 << ", pT=" << p4.Pt() << ", eta=" << p4.Eta() << ", phi=" << p4.Phi() << endl;
        }
        mSpecialIndices[prtidx] = prt;
        PartonAdd(prt, (char)-1, 3, leptW, prt);
    }

    return true;
} // LeptonAdd

#ifdef STORE_PRTCLS
/* Adding final-state particles */
void Pythia6Jets::ParticleAdd(unsigned prt, char jetid)
{
    int pdgid = mPythia->GetK(prt,2);
    if (pdgid==22 and GammaChecker(prt)) pdgid = 20;
    TLorentzVector handle = TLorentzify(prt);

    if (jetid >= 0) { // Associated with jets
        mJetId.push_back(jetid);
        mPDGID.push_back(pdgid);
        mPt.push_back(handle.Pt());
        mEta.push_back(handle.Eta());
        mPhi.push_back(handle.Phi());
        mE.push_back(handle.E());
    } else { // No jet association
        mNIJPDGID.push_back(pdgid);
        mNIJPt.push_back(handle.Pt());
        mNIJEta.push_back(handle.Eta());
        mNIJPhi.push_back(handle.Phi());
        mNIJE.push_back(handle.E());
    }
} // ParticleAdd
#endif

void Pythia6Jets::PartonAdd(unsigned prt, char jetid, char tag, int ptnid, int ownid)
{
    TLorentzVector handle = TLorentzify(prt);

    mPJetId.push_back(jetid);
    mPPDGID.push_back(mPythia->GetK(prt,2));
    mPPtnId.push_back(ptnid);
    mPOwnId.push_back(ownid);
    mPTag.push_back(tag);
    mPPt.push_back(handle.Pt());
    mPEta.push_back(handle.Eta());
    mPPhi.push_back(handle.Phi());
    mPE.push_back(handle.E());
    mDR.push_back(-1.0);
} // PartonAdd

void Pythia6Jets::PartonAdd(unsigned int prt, char jetid)
{
    assert(prt < mPartonInfo.size() and jetid < (char) mSortedJets.size());

    mPJetId.push_back(jetid);
    mPPDGID.push_back(mPartonInfo[prt].id);
    mPPtnId.push_back(mPartonInfo[prt].ptnid);
    mPOwnId.push_back(mPartonInfo[prt].ownid);
    mPTag.push_back(mPartonInfo[prt].tag);
    mPPt.push_back(mPartonInfo[prt].p4.pt());
    mPEta.push_back(mPartonInfo[prt].p4.eta());
    // Uniformize phi values between TLorentzVector and PseudoJet
    mPPhi.push_back(mPartonInfo[prt].p4.phi() - (mPartonInfo[prt].p4.phi()>fastjet::pi ? 2*fastjet::pi : 0));
    mPE.push_back(mPartonInfo[prt].p4.e());
    mDR.push_back(jetid<0 ? -1.0 : mPartonInfo[prt].p4.delta_R(mSortedJets[jetid]));
} // PartonAdd

void Pythia6Jets::JetAdd(unsigned jet)
{
    mJPt.push_back(mSortedJets[jet].pt());
    mJEta.push_back(mSortedJets[jet].eta());
    // Uniformize phi values between TLorentzVector and PseudoJet
    mJPhi.push_back(mSortedJets[jet].phi() - (mSortedJets[jet].phi()>fastjet::pi ? 2*fastjet::pi : 0));
    mJE.push_back(mSortedJets[jet].e());
    /* Start messing around with jet constituents */
    mJetParts.clear();
    mJetParts = mSortedJets[jet].constituents();
    Cuts();
    mConst.push_back(mCutJetParts.size());
    mPTD.push_back(PTD());
    mSigma2.push_back(Sigma2());

    if (cfg::Debug) {
        cout << "Added jet: E = " << mSortedJets[jet].e() << ", pT = " << mSortedJets[jet].pt()
        << ", eta = " << mSortedJets[jet].eta() << ", phi = " << mSortedJets[jet].phi() << endl;
    }
} // JetAdd


/* The most recent parton with more than one daughter. Typically, this does nothing. */
unsigned int Pythia6Jets::OptimalParton(unsigned prt)
{
    if (IsPreFSPartonIdx(prt)) { /* Do we have something repeating? */
        while (mPythia->GetK(prt,4) == mPythia->GetK(prt,5) and mPythia->GetK(prt,4)!=0)
            prt = mPythia->GetK(prt,4);
    }

    return prt;
}

/* Corrected parton momentum */
TLorentzVector Pythia6Jets::LastParton(unsigned prt, bool &prev_stop)
{
    if (IsPreFSPartonIdx(prt)) {
        /* stop: detect if even one of the daughters is beyond final state => return current prt. */
        bool stop = false;
        TLorentzVector cumulator;
        for (unsigned i = mPythia->GetK(prt,4); i <= mPythia->GetK(prt,5); ++i) {
            cumulator += LastParton(i,stop);
            if (stop) return TLorentzify(prt);
        }

        return cumulator;
    }

    prev_stop = true;
    return TLorentzify(prt);
}

/* Corrected parton momentum: same as LastParton, but only proceed a given amount of levels. */
TLorentzVector Pythia6Jets::NextParton(unsigned prt, unsigned lvl)
{
    auto tmpPrt = OptimalParton(prt);
    if (prt != tmpPrt) {
        prt = tmpPrt;

        if (lvl==0) return TLorentzify(prt);

        TLorentzVector cumulator;
        for (unsigned dtr = mPythia->GetK(prt,4); dtr <= mPythia->GetK(prt,5); ++dtr) {
            if (IsPreFSPartonIdx(prt)) cumulator += NextParton(dtr,lvl-1);
            else                    return TLorentzify(prt);
        }
        return cumulator;
    }

    return TLorentzify(prt);
}


/* Does a photon originate from pions? */
bool Pythia6Jets::GammaChecker(unsigned prt)
{
    /* One mother, which is pi0? */
    int mother = mPythia->GetK(prt,3);
    if (abs(mPythia->GetK(mother,2))!=111) return false;

    /* Check that the photons correspond to the pion */
    double eDifference = mPythia->GetP(mother,4);
    for (unsigned dtr = mPythia->GetK(mother,4), N = mPythia->GetK(mother,5); dtr <= N; ++dtr)
        eDifference -= mPythia->GetP(dtr,4);

    if (fabs(eDifference) > 0.001)
        return false;

    return true;
}

/* A very handy handle */
inline TLorentzVector Pythia6Jets::TLorentzify(unsigned int prt)
{
    return TLorentzVector(mPythia->GetP(prt,1),mPythia->GetP(prt,2),mPythia->GetP(prt,3),mPythia->GetP(prt,4));
}

/* Another handy handle */
inline fastjet::PseudoJet Pythia6Jets::PseudoJettify(unsigned int prt)
{
    return fastjet::PseudoJet(mPythia->GetP(prt,1),mPythia->GetP(prt,2),mPythia->GetP(prt,3),mPythia->GetP(prt,4));
}

inline fastjet::PseudoJet Pythia6Jets::PseudoJettify(TLorentzVector p4) {
    return fastjet::PseudoJet(p4.Px(),p4.Py(),p4.Pz(),p4.E());
}


double Pythia6Jets::PTD()
{
    double square = 0, linear = 0;
    for(size_t q = 0; q != mCutJetParts.size(); ++q) {
        square += pow(mCutJetParts[q].pt(),2);
        linear += mCutJetParts[q].pt();
    }
    return sqrt(square)/linear;
}

double Pythia6Jets::Sigma2()
{
    double weightedDiffs[4] = {0,0,0,0};
    double phi = 0, eta = 0, pT2 = 0;

    for (unsigned q = 0; q != mCutJetParts.size(); ++q) {
        pT2 += pow(mCutJetParts[q].pt(),2);
        eta += pow(mCutJetParts[q].pt(),2)*mCutJetParts[q].eta();
        phi += pow(mCutJetParts[q].pt(),2)*mCutJetParts[q].phi();
    }
    eta /= pT2; phi /= pT2;

    for(unsigned int q = 0; q != mCutJetParts.size(); ++q)
    {
        double deltaEta = eta-mCutJetParts[q].eta();
        double deltaPhi = TVector2::Phi_mpi_pi(phi-mCutJetParts[q].phi());
        double pT2Tmp = pow(mCutJetParts[q].pt(),2);
        weightedDiffs[0] += pT2Tmp*deltaEta*deltaEta;
        weightedDiffs[3] += pT2Tmp*deltaPhi*deltaPhi;
        weightedDiffs[1] -= pT2Tmp*deltaEta*deltaPhi;
    }
    weightedDiffs[2] = weightedDiffs[1];

    TMatrixDSymEigen me( TMatrixDSym(2,weightedDiffs) );
    TVectorD eigenvals = me.GetEigenValues();

    return sqrt(eigenvals[1]/pT2);
}

void Pythia6Jets::Cuts()
{
    mCutJetParts.clear();
    bool cutMode = true;

    if (cfg::CutMode) {
        /* Explicit cuts (pt cut for photons and neutral hadrons) */
        vector<fastjet::PseudoJet> tmpParts;
        for (auto &q : mJetParts) {
            if (q.user_index() < 0) continue; // Ghost partons
            int absid = abs(mPythia->GetK(q.user_index(),2));
            if (!( q.pt()<1 and (absid == 22 or (IsHadron(absid) and !IsCharged(absid)))) )
                tmpParts.push_back(q);
        }

        /* Implicit cuts (pt cut for hadrons) */
        for (auto &q : tmpParts) {
            int absid = abs(mPythia->GetK(q.user_index(),2));
            if ( !IsHadron(absid) or ( (IsCharged(absid) and q.pt()>0.3) or (!IsCharged(absid) and q.pt()>3) ) )
                mCutJetParts.push_back( q );
        }
    } else {
        /* Just a dry copy of the mJetParts to mCutJetParts (without ghosts) */
        for (auto &q : mJetParts) {
            if (q.user_index() < 0) continue; // Ghost partons
            mCutJetParts.push_back(q);
        }
    }
}


bool Pythia6Jets::IsHadron(int pdg)
{
    return abs(pdg)>99;
}

bool Pythia6Jets::IsLepton(int pdg)
{
    pdg = abs(pdg);
    return (pdg>10 and pdg<=16);
}

bool Pythia6Jets::IsCharged(int pdg)
{
    pdg = abs(pdg);

    int charge = 0;
    /* Photons and neutrinos */
    if (pdg==22 or pdg==12 or pdg==14 or pdg==16) return false;
    /* Charged leptons */
    if (pdg==11 or pdg==13 or pdg==15) return true;

    pdg = (pdg/10)%1000;
    if (pdg < 100) { /* Mesons */
        if ((pdg%10)%2 == 0) charge += 2;
        else                 charge -= 1;

        if ((pdg/10)%2 == 0) charge -= 2;
        else                 charge += 1;

    } else if (pdg>=100) { /* Baryons */
        while (pdg != 0) {
            int digit = pdg%10;
            pdg = pdg/10;
            if (digit%2 == 0) charge += 2;
            else              charge -= 1;
        }
    }

    return charge != 0;
}

inline bool Pythia6Jets::IsPartonIdx(unsigned int prt)
{
    unsigned pdg = abs(mPythia->GetK(prt,2));
    return (pdg <= 6 or pdg == 21);
}

inline bool Pythia6Jets::IsPreHadronization(unsigned int prt)
{
    unsigned absid = abs(mPythia->GetK(prt,2));
    /* Clusters, strings and such indicate that this is not the partonic final state. */
    if (absid > 90 and absid < 94) return true;
}


/* This assumes implicitly that the particle prt is a parton.
 * Checks, if this is a parton before the final state. */
inline bool Pythia6Jets::IsFSPartonIdx(unsigned int prt)
{
    /* No decays in sight (e.g. the Hard Process stage) */
    if (mPythia->GetK(prt,4)>mPythia->GetK(prt,5)) return false;

    /* Checking the daughters of the parton. */
    for (unsigned dtr = mPythia->GetK(prt,4), N = mPythia->GetK(prt,5); dtr <= N; ++dtr)
        if (IsPreHadronization(dtr)) return true;

    /* No final state indicators observed. */
    return false;
}

inline bool Pythia6Jets::IsPreFSPartonIdx(unsigned int prt)
{
    return IsPartonIdx(prt) and !IsFSPartonIdx(prt);
}

/* Graphviz printing for a single particle */
void Pythia6Jets::PrintParticle(unsigned prt)
{
    unsigned stat = mPythia->GetK(prt,2);
    unsigned mother = mPythia->GetK(prt,3);
    if (mother == 0 and prt > 2) mother = prt-2;

    cout << mother << " -> " << prt << " [label=\"" << prt << " " << mPythia->GetK(prt,1) << " " << mPythia->GetK(prt,2) << "\\n";
    cout << mPythia->GetK(prt,3) << " " << mPythia->GetK(prt,4) << " " << mPythia->GetK(prt,5) << "\\n";
    cout << mPythia->GetP(prt,4) << " " << mPythia->GetP(prt,5) << "\\n";
    cout << '"';
    cout << ",penwidth=2,color=\"";
    if (prt>2 && prt < 9) cout << "blue";
    else cout << "violet";
    cout << "\"];" << endl;

    if (prt == 7 or prt == 8) {
        if (prt==7)
            mother += 1;
        if (prt==8)
            mother -= 1;
        cout << mother << " -> " << prt << " [label=\"" << prt << " " << mPythia->GetK(prt,1) << " " << mPythia->GetK(prt,2) << "\\n";
        cout << mPythia->GetK(prt,3) << " " << mPythia->GetK(prt,4) << " " << mPythia->GetK(prt,5) << "\\n";
        cout << mPythia->GetP(prt,4) << " " << mPythia->GetP(prt,5) << "\\n";
        cout << '"';
        cout << ",penwidth=2,color=\"";
        cout << "blue";
        cout << "\"];" << endl;
    }

    mother = prt;
    prt = mPythia->GetK(prt,4);
    if (prt==mPythia->GetK(mother,5) && mPythia->GetK(prt,2)==92) {
        cout << mother << " -> " << prt << " [label=\"" << prt << " " << mPythia->GetK(prt,1) << " " << mPythia->GetK(prt,2) << "\\n";
        cout << mPythia->GetK(prt,3) << " " << mPythia->GetK(prt,4) << " " << mPythia->GetK(prt,5) << "\\n";
        cout << mPythia->GetP(prt,4) << " " << mPythia->GetP(prt,5) << "\\n";
        cout << '"';
        cout << ",penwidth=2,color=\"";
        cout << "yellow";
        cout << "\"];" << endl;
    }
}


/* Graphviz printing for a whole event */
void Pythia6Jets::PrintEvent()
{
    cout << "digraph test {" << endl;
    cout << "randir=LR;" << endl;
    cout << "ranksep=1.5;" << endl;
    cout << "node [width=0.03,height=0.03,shape=point,label=\"\"];" << endl;
    for (unsigned prt = 1; prt <= mPythia->GetN(); ++prt) PrintParticle(prt);
    cout << "}" << endl;
}
