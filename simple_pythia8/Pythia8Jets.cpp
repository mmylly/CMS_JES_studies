#include "Pythia8Jets.h"
#include "Pythia8Plugins/CombineMatchingInput.h"
//#include "Pythia8Plugins/aMCatNLOHooks.h"

bool TTBarSelector::doVetoProcessLevel(Event& process)
{
    unsigned leptons = 0;
    for (unsigned prt = 0; prt!=process.size(); ++prt) {
        if (process[prt].statusAbs() == 23 && process[prt].isLepton())
            ++leptons;
    }

    if (leptons != 2)
        return true;
    return false;
}




Pythia8Jets::Pythia8Jets(string settings, string fileName, int mode) :
    mEvent(mPythia.event),
    mInitialized(true),
    mGhostCoeff(pow(10,-18)),
    mTotWgt(0.0),
    mSelWgt(0.0)
{
    mMode = mode;

    bool tmode = mMode == 4 || mMode ==5 || mMode == 6;
    if (tmode)
        mPythia.settings.addFlag("tt:picking",false);
    if (!mPythia.readFile(settings.c_str()))
        throw std::invalid_argument("Error while reading settings");

    /* Initialization of the Pythia8 run */
    if (tmode and (mPythia.settings.flag("tt:picking") or mMode==4)) {
        mPythia.setUserHooksPtr(&mTTBarSelector);
    }

    if (!mPythia.init()) throw runtime_error("Pythia8 initialization failed");
    mPythia.settings.listChanged();

    mNumEvents = mPythia.mode("Main:numberOfEvents");
    mCounter = 0;
    mNumErrs = 0;

    /* Try to create a file to write */
    mFile = new TFile(fileName.c_str(), "RECREATE");
    if(!mFile->IsOpen()) throw runtime_error("Creating an output file failed");
    mFile->SetCompressionLevel(1);

    /* Create a tree. Autosave every 100 Mb, cache of 10 Mb */
    mTree = new TTree("Pythia8Jets","Pythia8 particle data.");
    if (!mTree) throw runtime_error("Creating a tree failed");
    mTree->SetAutoSave(100000000); /* 0.1 GBytes */
    mTree->SetCacheSize(10000000); /* 100 MBytes */
    TTree::SetBranchStyle(1); /* New branch style */

    /** Connect an event to the tree **/
    mTree->Branch("info",&mInfo)->SetAutoDelete(kFALSE);
    mTree->Branch("bnucount",&mBNuCount)->SetAutoDelete(kFALSE);
    mTree->Branch("onucount",&mONuCount)->SetAutoDelete(kFALSE);
    mTree->Branch("nuob",&mNuOB)->SetAutoDelete(kFALSE);
    mTree->Branch("nuoc",&mNuOC)->SetAutoDelete(kFALSE);
    mTree->Branch("nuolept",&mNuOLept)->SetAutoDelete(kFALSE);
    mTree->Branch("nuoother",&mNuOOther)->SetAutoDelete(kFALSE);
    mTree->Branch("nub",&mNuB)->SetAutoDelete(kFALSE);
    mTree->Branch("nuc",&mNuC)->SetAutoDelete(kFALSE);
    mTree->Branch("nulept",&mNuLept)->SetAutoDelete(kFALSE);
    mTree->Branch("nuother",&mNuOther)->SetAutoDelete(kFALSE);
    mTree->Branch("weight",&mWeight)->SetAutoDelete(kFALSE);
    mTree->Branch("pthat",&mPtHat)->SetAutoDelete(kFALSE);
    /* Particle lvl */
    mTree->Branch("isolation",&mIsolation)->SetAutoDelete(kFALSE);
    mTree->Branch("prtcl_jet",&mJetId)->SetAutoDelete(kFALSE);
    mTree->Branch("prtcl_pdgid",&mPDGID)->SetAutoDelete(kFALSE);
    mTree->Branch("prtcl_pt",&mPt)->SetAutoDelete(kFALSE);
    mTree->Branch("prtcl_eta",&mEta)->SetAutoDelete(kFALSE);
    mTree->Branch("prtcl_phi",&mPhi)->SetAutoDelete(kFALSE);
    mTree->Branch("prtcl_e",&mE)->SetAutoDelete(kFALSE);
    /* Parton lvl */
    mTree->Branch("prtn_jet",&mPJetId)->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_ptn",&mPPtnId)->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_own",&mPOwnId)->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_pdgid",&mPPDGID)->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_tag",&mPTag)->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_pt",&mPPt)->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_eta",&mPEta)->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_phi",&mPPhi)->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_e",&mPE)->SetAutoDelete(kFALSE);
    mTree->Branch("prtn_dr",&mDR)->SetAutoDelete(kFALSE);
    /* Jet lvl */
    mTree->Branch("jet_pt",&mJPt)->SetAutoDelete(kFALSE);
    mTree->Branch("jet_eta",&mJEta)->SetAutoDelete(kFALSE);
    mTree->Branch("jet_phi",&mJPhi)->SetAutoDelete(kFALSE);
    mTree->Branch("jet_e",&mJE)->SetAutoDelete(kFALSE);
    mTree->Branch("jet_ptn",&mJPtnId)->SetAutoDelete(kFALSE);
    mTree->Branch("jet_constituents",&mConst)->SetAutoDelete(kFALSE);
    mTree->Branch("jet_ptd",&mPTD)->SetAutoDelete(kFALSE);
    mTree->Branch("jet_sigma2",&mSigma2)->SetAutoDelete(kFALSE);

    mTree->Branch("met",&mMet)->SetAutoDelete(kFALSE);

    mTree->BranchRef();

    mJetDef = fastjet::JetDefinition(fastjet::antikt_algorithm, 0.4, fastjet::E_scheme, fastjet::Best);
    /* Setup a custom event timer */
    mTimerStep = 1000;
    mTimer.setParams(mNumEvents,mTimerStep);
    mTimer.startTiming();
} // Pythia8Jets initializer




/** Loops **/

// Nice info: PrintEvent();
void Pythia8Jets::EventLoop()
{
    if (!mInitialized) {
        cerr << "Event loop can be performed only after proper initialization" << endl;
        return;
    }

    /* The event loop */
    mEvtNo = 0;
    while (mEvtNo != mNumEvents) {
        if (!mPythia.next()) {
            if (mPythia.info.atEndOfFile()) {
                cerr << "Les Houches File ended at " << mEvtNo << " of " << mNumEvents << endl;
                break;
            }
            continue;
        }
        mWeight = mPythia.info.weight();
        if (mMode==6 || mMode==5) {
            //cout << mWeight << " " << mPythia.info.mergingWeightNLO() << endl; // << " " << mMerging->getNormFactor() << endl;
        }
        mTotWgt += mWeight;
        if (mWeight == 0) continue;
        PreProcess();
        if (!ParticleLoop()) continue;
        if (!IsolationProc()) continue;
        if (!JetLoop()) continue;
        mSelWgt += mWeight;
        //cout << mEvtNo << endl;

        //mPythia.event.list();
        //PrintEvent();
        mTree->Fill();

        ++mEvtNo;
        if (mEvtNo%mTimerStep==0) mTimer.printTime();
    }

    /* Cleaning up: */
    mFile = mTree->GetCurrentFile();
    mTree->AutoSave("Overwrite");
    mFile->Close();

    mPythia.stat();

    cerr << "Total cross-section: " << mPythia.info.sigmaGen()*1000000000 << " pm " << mPythia.info.sigmaErr()*1000000000 << " pb." << endl;
    cerr << "Passed cross-section: " << mPythia.info.sigmaGen()*1000000000*(mSelWgt/mTotWgt) << " pm " << mPythia.info.sigmaErr()*1000000000*(mSelWgt/mTotWgt) << " pb." << endl;
    cerr << mPythia.info.nSelected() << endl;
    if (mMode>=5) {
        cerr << "Passed cross-section: " << mPythia.info.sigmaLHEF(0)*(mSelWgt/mTotWgt) << endl;
        cerr << "Passed cross-section: " << (mSelWgt/mPythia.info.nSelected()) << endl;
        cerr << "Testing " << mPythia.info.sigmaLHEF(0) << endl;
    }

    cerr << "Possible errors:" << endl;
    for (auto &err : mErrorList)
        cerr << "Error: " << err.first << " weight of total cross section: " << err.second/mTotWgt << endl;

    if (mCounter != 0)
        cerr << "Non-zero counter value: " << mCounter << endl;

    mInitialized = false;
} // EventLoop


bool Pythia8Jets::ParticleLoop()
{
    /* Particle lvl */
    mIsolation.clear();

    mJetId.clear();
    mPDGID.clear();
    mPt.clear();
    mEta.clear();
    mPhi.clear();
    mE.clear();
    /* Parton lvl */
    mPJetId.clear();
    mPPtnId.clear();
    mPOwnId.clear();
    mPPDGID.clear();
    mPTag.clear();
    mPPt.clear();
    mPEta.clear();
    mPPhi.clear();
    mPE.clear();
    mDR.clear();
    /* Jet lvl */
    mJPt.clear();
    mJEta.clear();
    mJPhi.clear();
    mJE.clear();
    mJPtnId.clear();
    mConst.clear();
    mPTD.clear();
    mSigma2.clear();

    mJetInputs.clear();
    mSortedJets.clear();
    mJetParts.clear();
    mCutJetParts.clear();

    mPartonInfo.clear();
    mAncestry.clear();
    mNAncestry.clear();
    mWs.clear();

    /* Special particle indices are saved to eliminate saving overlap. */
    mSpecialIndices.clear();

    mInfo = 0;
    mPtHat = mPythia.info.pTHat();

    mMetVect = fastjet::PseudoJet();

    /* Particle loop */
    mHardProcCount = 0;
    for (unsigned prt = 0; prt!=mEvent.size(); ++prt) {
        if (!ProcessParticle(prt))
            return false;
    }
    mMet = mMetVect.perp();

    /* Sanity checks (ROOT produces a segfault when the error is launched) */
    if (   (mMode<4  and mHardProcCount!=2)
        or (mMode==2 and mSpecialIndices.size()!=1)
        or (mMode==3 and mSpecialIndices.size()!=2)
        or (mMode==4 and mHardProcCount!=4) )
    {
        mEvent.list();
        cout << "Hard proc count: " << mHardProcCount << " Special indices size: " << mSpecialIndices.size() << endl;
        throw std::runtime_error("Unexpected hard process structure");
    }

    return true;
} // ParticleLoop


void Pythia8Jets::PartonDescend(unsigned prt)
{
    /* NLO-level partons (tree). */
    for (auto &dtr : mEvent[prt].daughterList()) {
        unsigned truth = OptimalParton(dtr);
        if (mEvent[truth].pT()<4) continue;
        mNAncestry.push_back(truth);
        /* Slightly corrected: */
        if (!mEvent[truth].isHadron()) PartonAppend(PseudoJettify(truth),truth,4,prt,truth);
        /* Corrected: */
        //bool dimmy;
        //PartonAppend(PseudoJettify(LastParton(truth,dimmy)),truth,5,prt,truth);
        if (mEvent[truth].isParton()) PartonDescend(truth);
    }
}


bool Pythia8Jets::ProcessParticle(unsigned prt)
{
    int stat = mEvent[prt].statusAbs();

    if (mEvent[prt].isParton()) {
        if (stat==23) { /* Hard process activities. */
            ++mHardProcCount;
            /** Save hard process outgoing partons with and without a corrected momentum. **/
            /* Uncorrected */
            //int ptnid = PartonProc(prt); // Find out if W is the father
            //PartonAppend(PseudoJettify(prt),prt,0,ptnid,prt);
            /* Slightly corrected: */
            prt = OptimalParton(prt);
            int ptnid = PartonProc(prt); // Find out if W is the father
            PartonAppend(PseudoJettify(prt),prt,0,ptnid,prt);
            mAncestry.push_back(prt);
            /* Corrected: */
            //int ptnid = PartonProc(prt); // Find out if W is the father
            //bool dummy;
            //PartonAppend(PseudoJettify(LastParton(prt,dummy)),prt,0,ptnid,prt);

            PartonDescend(prt);
        }

        //if (stat==71 || stat==72) {
        //    fastjet::PseudoJet part = PseudoJettify(prt);
        //    part.set_user_index(prt);
        //}

        if (mEvent[prt].idAbs()!=5) return true; /* Let b-partons go */
    }

    /* A process type -dependent custom process */
    int cp = CustomProcess(prt);
    if (cp==0)
        return false;
    else if (cp==1)
        return true;

    // Ghost b hadrons
    if (mEvent[prt].isHadron() and IsExcitedHadronState(prt,5)==0)
        PartonAppend(PseudoJettify(prt),prt,9);

    /* Final-state particles */
    if (mEvent[prt].isFinal()) {
        /* Set the user index to pdgid, except let this be 20 for pi0 photons */
        int id = mEvent[prt].idAbs();
        fastjet::PseudoJet part = PseudoJettify(prt);
        part.set_user_index(prt);

        //ParticleAdd(prt,-1);
        if (id==12 or id==14 or id==16) {
            mMetVect += part;
        } else if (Absent(prt)) {
            mJetInputs.push_back(part);
        }
    }

    return true;
} // ProcessParticle : Base


bool Pythia8Jets::JetLoop()
{
    fastjet::ClusterSequence clustSeq(mJetInputs, mJetDef);
    vector<fastjet::PseudoJet> unsorted = clustSeq.inclusive_jets(4.0);
    mSortedJets = sorted_by_pt(unsorted);
    for (unsigned jet = 0; jet<mSortedJets.size(); ++jet) {
        JetAdd(jet);

        for (auto &part : mJetParts) {
            int prt = part.user_index();
            if (prt<0) {
                /* Adding uncorrected and corrected parton momentum */
                prt *= -1;
                --prt;
                PartonAdd(prt,jet);
                mPartonInfo[prt].used = true;
            } else {
                //ParticleAdd(prt,jet);
                continue;
            }
        }
    }

    for (unsigned prt = 0; prt < mPartonInfo.size(); ++prt) {
        if (mPartonInfo[prt].used)
            continue;
        PartonAdd(prt,-1);
    }

    return true;
} // JetLoop




/** Infrastructure - particle adding **/

/* Adding final-state particles associated with jets */
void Pythia8Jets::ParticleAdd(unsigned prt, char jetid)
{
    int pdgid = mEvent[prt].id();
    if (pdgid==22 and GammaChecker(prt)) pdgid = 20;

    mJetId.push_back(jetid);
    mPDGID.push_back(pdgid);
    mPt.push_back(mEvent[prt].pT());
    mEta.push_back(mEvent[prt].eta());
    mPhi.push_back(mEvent[prt].phi());
    mE.push_back(mEvent[prt].e());
} // ParticleAdd

void Pythia8Jets::PartonAdd(unsigned prt, char jetid, char tag, int ptnid, int ownid)
{
    mPJetId.push_back(jetid);
    mPPtnId.push_back(ptnid);
    mPOwnId.push_back(ownid);
    mPTag.push_back(tag);
    mPPDGID.push_back(mEvent[prt].id());
    mPPt.push_back(mEvent[prt].pT());
    mPEta.push_back(mEvent[prt].eta());
    mPPhi.push_back(mEvent[prt].phi());
    mPE.push_back(mEvent[prt].e());
    mDR.push_back(-1);
} // PartonAdd

void Pythia8Jets::PartonAdd(unsigned prt, char jetid)
{
    assert(prt < mPartonInfo.size() and jetid < (char) mSortedJets.size());
    mPJetId.push_back(jetid);
    mPPtnId.push_back(mPartonInfo[prt].ptnid);
    mPOwnId.push_back(mPartonInfo[prt].ownid);
    mPPDGID.push_back(mPartonInfo[prt].id);
    mPTag.push_back(mPartonInfo[prt].tag);
    mPPt.push_back(mPartonInfo[prt].p4.pt());
    mPEta.push_back(mPartonInfo[prt].p4.eta());
    // Uniformize phi values between TLorentzVector and PseudoJet
    mPPhi.push_back(mPartonInfo[prt].p4.phi() - (mPartonInfo[prt].p4.phi()>fastjet::pi ? 2*fastjet::pi : 0));
    mPE.push_back(mPartonInfo[prt].p4.e());
    mDR.push_back(jetid<0 ? -1.0 : mPartonInfo[prt].p4.delta_R(mSortedJets[jetid]));
} // PartonAdd

void Pythia8Jets::JetAdd(unsigned jet, int spoil)
{
    mJPt.push_back(mSortedJets[jet].pt());
    mJEta.push_back(mSortedJets[jet].eta());
    // Uniformize phi values between TLorentzVector and PseudoJet
    mJPhi.push_back(mSortedJets[jet].phi() - (mSortedJets[jet].phi()>fastjet::pi ? 2*fastjet::pi : 0));
    mJE.push_back(mSortedJets[jet].e());
    /* Start messing around with jet constituents */
    mJetParts = sorted_by_pt(mSortedJets[jet].constituents());
    if (spoil==0) {
        Cuts();
        mConst.push_back(mCutJetParts.size());
        mPTD.push_back(PTD());
        mSigma2.push_back(Sigma2());
    } else {
        mJPtnId.push_back(JetAnc(mSortedJets[jet].pt()));
    }
} // JetAdd

void Pythia8Jets::PartonAppend(fastjet::PseudoJet p4, unsigned prt, char tag, int ptnid, int ownid)
{
    mPartonInfo.push_back(PartonHolder{p4, mEvent[prt].id(), tag, ptnid, ownid, false});
    p4 *= mGhostCoeff;
    p4.set_user_index(-mPartonInfo.size());
    mJetInputs.push_back(p4);
}


int Pythia8Jets::JetAnc(float pt) {
    map<int,float> compareA, compareNA, compareW;
    for (auto &ptn : mJetParts) {
        if (ptn.user_index() < 0 || ptn.pt()<0.001)
            continue;
        int prt = ptn.user_index();
        for (auto &anc : mAncestry) {
            if (mEvent[prt].isAncestor(anc)) {
                if (compareA.count(anc)==0)
                    compareA[anc] = mEvent[prt].pT();
                else
                    compareA[anc] += mEvent[prt].pT();
            }
        }
        for (auto &anc : mNAncestry) {
            if (mEvent[prt].isAncestor(anc)) {
                if (compareNA.count(anc)==0)
                    compareNA[anc] = mEvent[prt].pT();
                else
                    compareNA[anc] += mEvent[prt].pT();
            }
        }
        for (auto &anc : mWs) {
            if (mEvent[prt].isAncestor(anc)) {
                if (compareW.count(anc)==0)
                    compareW[anc] = mEvent[prt].pT();
                else
                    compareW[anc] += mEvent[prt].pT();
            }
        }
    }
    int trialInd = compareNA.begin()->first;
    for (auto &a : compareNA) {
        if (a.second>compareNA[trialInd])
            trialInd = a.first;
    }
    float best = compareNA[trialInd];
    int maxInd = trialInd;
    trialInd = compareA.begin()->first;
    for (auto &a : compareA) {
        if (a.second>compareA[trialInd])
            trialInd = a.first;
    }
    if (compareA[trialInd]>1.1*best) {
        best = compareA[trialInd];
        maxInd = trialInd;
    }
    trialInd = compareW.begin()->first;
    for (auto &a : compareW) {
        if (a.second>compareW[trialInd])
            trialInd = a.first;;
    }
    if (compareW[trialInd]>1.1*best) {
        best = compareW[trialInd];
        maxInd = trialInd;
    }

    if (best/pt < 0.3)
        return 0;

    return maxInd;
}



/** Misc **/

inline int Pythia8Jets::IsExcitedHadronState(unsigned prt, int quarkId)
{
    assert(quarkId>=0 && quarkId<=5);
    if (!HadrFuncs::StatusCheck(quarkId, mEvent[prt].idAbs()))
        return -1;
    vector<int> daughters = mEvent[prt].daughterList();
    for (int& dtr : daughters)
        if (HadrFuncs::StatusCheck(quarkId, mEvent[dtr].idAbs()))
            return 1;
    return 0;
} // IsExcitedHadronState


inline bool Pythia8Jets::Absent(unsigned int prt)
{
    return std::find(mSpecialIndices.begin(),mSpecialIndices.end(),prt)==mSpecialIndices.end();
}


inline int P8GenericTree::CustomProcess(unsigned prt)
{
    return 2;
} // CustomProcess : Generic


inline int P8DijetTree::CustomProcess(unsigned prt)
{
    return 2;
} // CustomProcess : Dijet

inline void Pythia8Jets::AddMessage(string msg, double wgt) {
    if (mErrorList.count(msg)==0)
        mErrorList[msg] = wgt;
    else
        mErrorList[msg] += wgt;
}





/** Photon stuff **/
inline void P8GammajetTree::PreProcess()
{
    mGammas.clear();
}

inline bool P8GammajetTree::IsolationProc()
{
    for (auto &gm : mGammas) {
        double EdR = 0;

        for (auto &part : mJetInputs) {
            if (part.user_index()<0) continue;

            double dR = gm.first.delta_R(part);
            if (dR < mGammaDR)
                EdR += part.e();
        }

        float isolation = EdR/gm.first.e();
        mIsolation.push_back(isolation);
        PartonAdd(gm.second,-1,6);
    }

    return true;
}


inline int P8GammajetTree::CustomProcess(unsigned prt)
{
    if (mEvent[prt].idAbs()==22) {
        if (mSpecialIndices.size()==0 and mEvent[prt].statusAbs()==23)
            return (GammaAdd(prt) ? 1 : 0);

        /* Save all nice gammas for isolation (incl. signal photon) */
        if (mEvent[prt].isFinal()) {
            auto part = PseudoJettify(prt);
            if (mEvent[prt].pT()>mGammaPt)
                mGammas.push_back(std::make_pair(part,prt));
        }
    }

    return 2;
} // CustomProcess : Gammajet


bool P8GammajetTree::GammaAdd(unsigned prt)
{
    /* Save a raw photon and the final photon distorted by ISR */
    PartonAdd(prt, -1, 2);
    double ptmp = mEvent[prt].pT();

    while (!mEvent[prt].isFinal()) {
        if (mEvent[prt].daughterList().size()>1) {
            AddMessage("Signal photon pair production.",mWeight);
            return false; // Detect pair production
        }
        prt = mEvent[prt].daughter1();
    }

    if (mEvent[prt].idAbs()==22) {
        //if (mEvent[prt].pT()<mGammaPt || fabs(mEvent[prt].eta())>mGammaEta) {
        //    AddMessage("Signal photon off eta/pT range!",mWeight);
        //    return false;
        //}
        mSpecialIndices.push_back(prt);
        PartonAdd(prt, -1, 3);
        ++mHardProcCount;
        return true;
    }

    AddMessage("Failed to locate final-state photon!",mWeight);
    return false;
} // GammaAdd




/** Zmumujet stuff **/
inline void P8ZmumujetTree::PreProcess()
{
    mMuons.clear();
}

inline bool P8ZmumujetTree::IsolationProc()
{
    for (auto &mu : mMuons) {
        double EdR = 0;

        for (auto &part : mJetInputs) {
            if (part.user_index()<0) continue;

            double dR = mu.first.delta_R(part);
            if (dR < mMuonDR)
                EdR += part.e();
        }

        float isolation = EdR/mu.first.e();
        mIsolation.push_back(isolation);
        PartonAdd(mu.second,-1,6);
    }

    return true;
}


/* This is based on an assumption made about status codes - there are safeguards in muonadd. */
inline int P8ZmumujetTree::CustomProcess(unsigned prt)
{
    if (mSpecialIndices.size()==0 and mEvent[prt].statusAbs()==22 && mEvent[prt].idAbs()==23)
        return MuonAdd(prt);

    if (mEvent[prt].idAbs()==13 and mEvent[prt].isFinal()) {
        auto part = PseudoJettify(prt);
        if (mEvent[prt].pT()>mMuonPt)
            mMuons.push_back(std::make_pair(part,prt));
    }

    return 2;
} // CustomProcess : Zmumujet


bool P8ZmumujetTree::MuonAdd(unsigned prt)
{
    if (mSpecialIndices.size()!=0)
        throw std::logic_error("Only muons are monitored in Z+jets events.");

    // Catch the latest version of the Z boson
    while (mEvent[mEvent[prt].daughter1()].idAbs()==23)
            prt = mEvent[prt].daughter1();

    for (int daughter : mEvent[prt].daughterList()) {
        if (mEvent[daughter].idAbs()==13) {
            mSpecialIndices.push_back(daughter);
            PartonAdd(daughter, -1, 2);
        }
    }

    /* Descend to the final muon forms; counter checks for absurd cases. */
    for (unsigned i = 0; i < mSpecialIndices.size(); ++i) {
        unsigned counter = 0;
        while (!mEvent[mSpecialIndices[i]].isFinal() && counter++ < 100) {
            for (auto daughter : mEvent[mSpecialIndices[i]].daughterList()) {
                if (mEvent[daughter].idAbs()==13) {
                    mSpecialIndices[i] = daughter;
                    break;
                }
            }
        }
        PartonAdd(mSpecialIndices[i], -1, 3);
    }

    if (mSpecialIndices.size()==2) {
        ++mHardProcCount;
        return true;
    }

    AddMessage("Failed to locate muon pair!",mWeight);
    return false;
} // MuonAdd




/** Ttbar stuff **/
inline void P8ttbarjetTree::PreProcess()
{
    mChargeds.clear();
    mBs.clear();
    mBNuCount = 0;
    mONuCount = 0;
    mNuOB = 0;
    mNuOC = 0;
    mNuOLept = 0;
    mNuOOther = 0;
    mNuB = 0;
    mNuC = 0;
    mNuLept = 0;
    mNuOther = 0;
}

inline int P8ttbarjetTree::PartonProc(unsigned prt) {
    for (auto &w : mWs) {
        if (mEvent[prt].isAncestor(w))
            return w;
    }
    return -1;
}

inline bool P8ttbarjetTree::IsolationProc()
{
    /* Adding Isolation monitoring for all present charged particles */
    for (auto &ch : mChargeds) {
        double EdR = 0;

        double dRlim = mEvent[ch.second].idAbs()==11 ? mElDR : mMuDR;
        for (auto &part : mJetInputs) {
            if (part.user_index()<0)
                continue;

            double dR = ch.first.delta_R(part);
            if (dR < dRlim) EdR += part.e();
        }

        // Appending the isolation info & charged particle info
        float isolation = EdR/ch.first.e();
        mIsolation.push_back(isolation);
        PartonAdd(ch.second,-1,6);
    }

    return true;
}


inline int P8ttbarjetTree::CustomProcess(unsigned prt)
{
    /* Add the W's */
    if (mEvent[prt].statusAbs()==22 and mEvent[prt].idAbs()==24) {
        prt = OptimalParton(prt);
        int nextW = prt;
        bool leptonic = false, hadronic = false;
        vector<int> photons;
        while (nextW!=-1) {
            prt = nextW;
            nextW = -1;
            for (auto &dtr : mEvent[prt].daughterList()) {
                int truth = OptimalParton(dtr);
                if (mEvent[truth].idAbs()==24)
                    nextW = truth;
                else if (mEvent[truth].idAbs()==22) {
                    if (mEvent[truth].pT()>0.1) photons.push_back(truth);
                } else if (mEvent[truth].isLepton()) {
                    leptonic = true;
                } else if (mEvent[truth].idAbs()<6) {
                    hadronic = true;
                } else
                    throw runtime_error(Form("W decays into unkown stuff! %d",mEvent[truth].id()));
            }
        }
        for (auto &gamma : photons) PartonAdd(gamma,(char)-1,(char)2,prt,gamma);
        mWs.push_back(prt);
        if (leptonic and hadronic)
            throw runtime_error("W has both leptons and hadrons!");
        else if (leptonic) {
            mLeptonFriend = prt;
            PartonAdd(prt,(char)-1,(char)2,-1,prt);
        } else if (hadronic) // Cluster the hadronic W
            PartonAppend(PseudoJettify(prt),prt,(char)2,-1,prt);
        else
            throw runtime_error("W has no hadrons nor leptons!");

        return 1;
    }

    /* Add the outgoing b indices */
    if (mEvent[prt].statusAbs()==23 and mEvent[prt].idAbs()==5) {
        mBs.push_back(prt);
        return 1;
    }

    if (mEvent[prt].isLepton()) {
        /* Add the outgoing hard process lepton (stops particle handling) */
        if (mEvent[prt].statusAbs()==23) {
            int state = (LeptonAdd(prt,mLeptonFriend) ? 1 : 0);
            if (state==1 && mEvent[mSpecialIndices.back()].status()==23)
                state = 2;
            return state;
        }

        if (mEvent[prt].isFinal() and Absent(prt)) {
            /* Neutrinos vs. charged leptons. */
            int id = mEvent[prt].idAbs();
            if (id==12 or id==14 or id==16) {
                // Neutrino catcher
                bool trial = false;
                for (auto &b : mBs) {
                    if (mEvent[prt].isAncestor(b)) {
                        if (id==16) {
                            if (mInfo & 2)
                                mInfo |= 4;
                            else
                                mInfo |= 2;
                        }

                        trial = true;
                        break;
                    }
                }
                int moth = mEvent[prt].mother1();
                int feeling = mEvent[moth].idAbs()/100;
                while (feeling>9)
                    feeling /= 10;
                if (trial) {
                    // b leptons
                    if (feeling==5)
                        ++mNuB;
                    else if (feeling==4)
                        ++mNuC;
                    else if (mEvent[moth].isLepton())
                        ++mNuLept;
                    else
                        ++mNuOther;
                    ++mBNuCount;
                    PartonAdd(prt,-1,7);
                } else {
                    // mystical leptons
                    if (feeling==5)
                        ++mNuOB;
                    else if (feeling==4)
                        ++mNuOC;
                    else if (mEvent[moth].isLepton())
                        ++mNuOLept;
                    else
                        ++mNuOOther;
                    PartonAdd(prt,-1,8);
                    ++mONuCount;
                }
            } else {
                /* Save all nice charged leptons for isolation */
                if (mEvent[prt].pT()>mLeptPt) {
                    auto part = PseudoJettify(prt);
                    mChargeds.push_back(std::make_pair(part,prt));
                }
            }
        }
    }

    return 2;
} // ProcessParticle : ttbarjet


bool P8ttbarjetTree::LeptonAdd(unsigned prt, int parent)
{
    int type = mEvent[prt].idAbs()%2;
    if (type) {
        /* Charged lepton input: find a final-state charged lepton. */

        /* Save a raw charged lepton*/
        PartonAdd(prt, (char)-1, 2,parent, prt);

        /* Check for tau */
        if (mEvent[prt].idAbs()==15)
            mInfo |= 1;

        unsigned origin = prt;
        while (!mEvent[prt].isFinal()) {
            auto prevprt = prt;
            for (int &daughter : mEvent[prevprt].daughterList()) {
                if (mEvent[daughter].isHadron()) {
                    prt = 0;
                    break;
                } else if (mEvent[daughter].id()==22) {
                    auto didx = OptimalParton(daughter);
                    if (mEvent[didx].pT()>0.1)
                        PartonAdd(didx,-1,'\2',parent,didx);
                } else if (mEvent[daughter].isLepton()) {
                    int dType = mEvent[daughter].idAbs()%2;
                    if (dType==1) {
                        /* Charged lepton kept */
                        prt = daughter;
                    } else {
                        /* Neutrino sent into neutrino loop */
                        LeptonAdd(daughter, mLeptonFriend);
                    }
                }
            }

            if (prt == 0) {
                /* This occurs around 25-30% of the time originating from tau decay */
                AddMessage("Partonic tau decay!",mWeight);
                return false;
            } else if (prt == prevprt) {
                /* Check if stuck in a loop*/
                AddMessage("Lepton loop stuck!",mWeight);
                //throw std::logic_error("Lepton loop stuck!");
                return false;
            }
        }

        mChargeds.push_back(std::make_pair(PseudoJettify(prt),prt));
    } else {
        /* Neutrinos */
        prt = OptimalParton(prt);
        if (mEvent[prt].daughterList().size()>0)
            throw std::logic_error("Neutrino decay spotted.");
    }

    mSpecialIndices.push_back(prt);
    PartonAdd(prt, (char)-1, 3, parent);

    return true;
} // LeptonAdd




/* Does a photon originate from pions? */
bool Pythia8Jets::GammaChecker(unsigned prt)
{
    assert(mEvent.size() > prt);

    /* One mother, which is pi0 */
    vector<int> mothers = mEvent[prt].motherList();
    if (mothers.size()!=1 || abs(mEvent[mothers[0]].id())!=111) return false;

    vector<int> daughters = mEvent[mothers[0]].daughterList();

    double eDifference = mEvent[mothers[0]].e();
    for (auto &daughter : daughters)
        eDifference -= mEvent[daughter].e();

    if (fabs(eDifference) > 0.001)
        return false;

    return true;
}




/** Infrastructure - partons **/

/* The most recent parton with more than one daughter */
unsigned int Pythia8Jets::OptimalParton(unsigned int prt)
{
    if (mEvent[prt].statusAbs() == 62 || mEvent[prt].statusAbs() == 63 || mEvent[prt].isFinal() || mEvent[prt].idAbs()>69)
        return prt;

    while (mEvent[prt].daughterList().size()==1) {
        auto tmp = mEvent[prt].daughterList()[0];
        if (mEvent[tmp].motherList().size()==1)
            prt = tmp;
        else
            break;
    }

    return prt;
}

/* Corrected parton momentum (fine 62 & 63 have a contrast to FS partons 71 and 72) */
TLorentzVector Pythia8Jets::LastParton(unsigned prt, bool &prev_stop)
{
    if (mEvent[prt].statusAbs() == 62 or mEvent[prt].statusAbs() == 63 or (mEvent[prt].isFinal() and mEvent[prt].idAbs()<69)) {
        if (mEvent[prt].motherList().size()!=1) {
            cout << "HOX! Rogue parton " << mEvent[prt].status() << " " << mEvent[prt].id()
                 << " with " << mEvent[prt].motherList().size() << " mothers!" << endl;
            for (auto &moth : mEvent[prt].motherList()) cout << " " << mEvent[moth].status();
            cout << endl;
        }
        // This is the optimal stage to stop momentum calculation
        return TLorentzify(prt);
    } else if (mEvent[prt].statusAbs() >= 70 or mEvent[prt].idAbs() > 69 or mEvent[prt].isFinal()) {
        // Detect when a daughter is overstepping boundaries and stop summation
        prev_stop = true;
        return TLorentzVector();
    }
    if (mEvent[prt].motherList().size()!=1) {
        cout << "HOX! Rogue parton " << mEvent[prt].status() << " " << mEvent[prt].id()
             << " with " << mEvent[prt].motherList().size() << " mothers!" << endl;
        for (auto &moth : mEvent[prt].motherList()) cout << " " << mEvent[moth].status();
        cout << endl;
    }
    // stop: detect if even one of the daughters is beyond final state
    bool stop = false;
    TLorentzVector cumulator;
    for (auto &daughter : mEvent.daughterList(prt)) {
        cumulator += LastParton(daughter,stop);
        if (stop) return TLorentzify(prt);
    }

    return cumulator;
}

/* Corrected parton momentum: same as LastParton, but only proceed a given amount of levels. */
TLorentzVector Pythia8Jets::NextParton(unsigned prt, unsigned lvl)
{
    if (mEvent[prt].statusAbs() == 62 or mEvent[prt].statusAbs() == 63 or mEvent[prt].isFinal())
        return TLorentzify(prt);

    while (mEvent.daughterList(prt).size()==1)
        prt = mEvent[prt].daughter1();

    if (lvl==0) return TLorentzify(prt);

    TLorentzVector cumulator;
    for (auto &daughter : mEvent.daughterList(prt)) {
        //if (mEvent[daughter].idAbs() < 30) {
        if (mEvent[daughter].statusAbs() < 70) {
            cumulator += NextParton(daughter,lvl-1);
        } else {
            /* If there is even one hadron in the resulting particles, hadronization has started. */
            return TLorentzify(prt);
        }
    }

    return cumulator;
}

/* A very handy handle */
inline TLorentzVector Pythia8Jets::TLorentzify(unsigned int prt)
{
    return TLorentzVector(mEvent[prt].px(),mEvent[prt].py(),mEvent[prt].pz(),mEvent[prt].e());
}

/* Another handy handle */
inline fastjet::PseudoJet Pythia8Jets::PseudoJettify(unsigned int prt)
{
    return fastjet::PseudoJet(mEvent[prt].px(),mEvent[prt].py(),mEvent[prt].pz(),mEvent[prt].e());
}

inline fastjet::PseudoJet Pythia8Jets::PseudoJettify(TLorentzVector p4) {
    return fastjet::PseudoJet(p4.Px(),p4.Py(),p4.Pz(),p4.E());
}




/** QGL variables **/

double Pythia8Jets::PTD()
{
    double square = 0, linear = 0;
    for(size_t q = 0; q != mCutJetParts.size(); ++q) {
        square += pow(mCutJetParts[q].pt(),2);
        linear += mCutJetParts[q].pt();
    }
    return sqrt(square)/linear;
}

double Pythia8Jets::Sigma2()
{
    double weightedDiffs[4] = {0,0,0,0};
    double phi = 0, eta = 0, pT2 = 0;

    for(size_t q = 0; q != mCutJetParts.size(); ++q) {
        pT2 += pow(mCutJetParts[q].pt(),2);
        eta += pow(mCutJetParts[q].pt(),2)*mCutJetParts[q].eta();
        phi += pow(mCutJetParts[q].pt(),2)*mCutJetParts[q].phi();
    }
    eta /= pT2; phi /= pT2;

    for(unsigned int q = 0; q != mCutJetParts.size(); ++q)
    {
        double deltaEta = eta-mCutJetParts[q].eta();
        double deltaPhi = TVector2::Phi_mpi_pi( phi-mCutJetParts[q].phi() );
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

void Pythia8Jets::Cuts()
{
    mCutJetParts.clear();

    if (mCutMode) {
        /* Explicit cuts (pt cut for photons and neutral hadrons) */
        vector<fastjet::PseudoJet> tmpParts;
        for (auto &q : mJetParts) {
            if (q.user_index() < 0) continue;
            int id = abs(mEvent[q.user_index()].id());
            if (!( q.pt()<1 && (id == 22 || (IsHadron(id) && !IsCharged(id)))) )
                tmpParts.push_back(q);
        }

        /* Implicit cuts (pt cut for hadrons) */
        for (auto &q : tmpParts) {
            int id = abs(mEvent[q.user_index()].id());
            if ( !IsHadron(id) || ( (IsCharged(id) && q.pt()>0.3) || (!IsCharged(id) && q.pt()>3) ) )
                mCutJetParts.push_back( q );
        }
    } else {
        for (auto &q : mJetParts) {
            if (q.user_index() < 0)
                continue;
            mCutJetParts.push_back(q);
        }
    }
}




/** Infrasturcture - leftovers */

bool Pythia8Jets::IsHadron(int pdg)
{
    if(pdg>99) return true;
    return false;
}

bool Pythia8Jets::IsCharged(int pdg)
{
    int charge = 0;
    /* photons and neutrinos */
    if (pdg==22 || pdg==12 || pdg==14 ||pdg==16 ) return false;
    /* charged leptons */
    if (pdg==11 || pdg==13 || pdg==15 ) return true;

    pdg = (pdg/10)%1000;
    if (pdg < 100) { /* Mesons */
        if ((pdg%10)%2 == 0) { charge += 2; }
        else { charge -= 1; }

        if ((pdg/10)%2 == 0) { charge -= 2; }
        else { charge += 1; }

    } else { /* Baryons */
        while (pdg != 0) {
            int digit = pdg%10;
            pdg = pdg/10;
            if (digit%2 == 0) { charge += 2; }
            else { charge -= 1; }
        }
    }
    if (charge == 0) return false;
    else return true;
}


/* Graphviz printing for a single particle */
void Pythia8Jets::PrintParticle(unsigned prt)
{
    unsigned stat = mEvent[prt].statusAbs();
    for (auto i : mEvent[prt].motherList()) {
        unsigned output = (i == 0) ? -prt : i;
        cout << output << " -> " << prt << " [label=\"" << prt << " ";
        cout << mEvent[prt].name() << "\\n" << mEvent[prt].m2() << " " << mEvent[prt].m2Calc() << "\\n" << mEvent[prt].pT() << "\\n" << mEvent[prt].e();
        cout << "\\n" << stat << "\"";
        cout << ",penwidth=2,color=\"";
        if (stat < 30)
            cout << "blue";
        else if (stat < 40)
            cout << "yellow";
        else if (stat < 50) {
            if (stat == 44)
                cout << "darkgreen";
            else if (stat == 43)
                cout << "limegreen";
            else if (stat == 42)
                cout << "mediumseagreen";
            else
                cout << "green";
        } else if (stat < 60) {
            if (stat == 51)
                cout << "indianred";
            else if (stat == 52)
                cout << "maroon";
            else
                cout << "red";
        } else if (stat < 70) {
            if (stat == 71)
                cout << "chocolate";
            else if (stat == 72)
                cout << "orangered";
            else
                cout << "orange";
        } else if (stat < 80)
            cout << "cyan";
        else
            cout << "violet";
        cout << "\"];" << endl;
    }
}


/* Graphviz printing for a whole event */
void Pythia8Jets::PrintEvent()
{
    cout << "digraph test {" << endl;
    cout << "randir=LR;" << endl;
    cout << "ranksep=1.5;" << endl;
    cout << "node [width=0.03,height=0.03,shape=point,label=\"\"];" << endl;
    for (unsigned prt = 0; prt!=mEvent.size(); ++prt)
        PrintParticle(prt);
    cout << "}" << endl;
}
