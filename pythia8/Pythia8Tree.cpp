#include "Pythia8Tree.h"

bool TTBarSelector::doVetoProcessLevel(Event& process)
{
    unsigned leptons = 0;
    for (unsigned prt = 0; prt!=process.size(); ++prt)
        if (process[prt].statusAbs() == 23 && process[prt].isLepton())
            ++leptons;

    if (leptons != 2)
        return true;
    return false;
}


Pythia8Tree::Pythia8Tree(string settings, string fileName, int mode) :
    mEvent(mPythia.event),
    mInitialized(true),
    mUseStrange(false)
{
    mMode = mode;

    /* Initialization of the Pythia8 run */
    if (mMode == 4) mPythia.setUserHooksPtr(&mTTBarSelector);
    if (!mPythia.readFile(settings.c_str())) throw std::invalid_argument("Error while reading settings");
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
    mTree = new TTree("Pythia8Tree","Pythia8 particle data.");
    if (!mTree) throw runtime_error("Creating a tree failed");
    mTree->SetAutoSave(100000000); /* 0.1 GBytes */
    mTree->SetCacheSize(10000000); /* 100 MBytes */
    TTree::SetBranchStyle(1); /* New branch style */

    /* Connect an event to the tree */
    mPrtclEvent = new PrtclEvent();
    if (!mPrtclEvent) throw runtime_error("Creating an event handle failed");
    mPrtclEvent->SetBit(kCanDelete);
    mPrtclEvent->SetBit(kMustCleanup);
    mBranch = mTree->Branch("event", &mPrtclEvent, 32000,4);
    if (!mBranch) throw runtime_error("Associating the event handle with the tree failed");
    mBranch->SetAutoDelete(kFALSE);
    mTree->BranchRef();

    /* Setup a custom event timer */
    mTimerStep = 1000;
    mTimer.setParams(mNumEvents,mTimerStep);
    mTimer.startTiming();
} // Pythia8Tree initializer


// Nice info: PrintEvent();
void Pythia8Tree::EventLoop()
{
    if (!mInitialized) {
        cerr << "Event loop can be performed only after proper initialization" << endl;
        return;
    }

    /* The event loop */
    unsigned evtNo = 0;
    while (evtNo != mNumEvents) {
        if (!mPythia.next()) continue;
        if (!ParticleLoop()) continue;
        mTree->Fill();

        ++evtNo;
        if (evtNo%mTimerStep==0) mTimer.printTime();
    }

    /* Cleaning up: */
    mFile = mTree->GetCurrentFile();
    mTree->AutoSave("Overwrite");
    mFile->Close();

    cerr << "Cross-section: " << mPythia.info.sigmaGen() << endl;

    if (mCounter != 0)
        cerr << "Non-zero counter value: " << mCounter << endl;

    delete mPrtclEvent; mPrtclEvent = 0;
    mInitialized = false;
} // EventLoop


bool Pythia8Tree::ParticleLoop()
{
    mPrtclEvent->Clear();
    /* Special particle indices are saved to eliminate saving overlap. */
    mSpecialIndices.clear();

    mPrtclEvent->fWeight = mPythia.info.weight();

    /* Particle loop */
    mHardProcCount = 0;
    for (unsigned prt = 0; prt!=mEvent.size(); ++prt)
        if (!ProcessParticle(prt))
            return false;

    /* Sanity checks (ROOT produces a segfault when the error is launched) */
    if (   (mMode<4 && mHardProcCount !=2)
        || (mMode==2 && mSpecialIndices.size()!=1)
        || (mMode==3 && mSpecialIndices.size()!=2)
        || (mMode==4 && mHardProcCount != 4) )
    {
        mEvent.list();
        cout << "Hard proc count: " << mHardProcCount << " Special indices size: " << mSpecialIndices.size() << endl;
        throw std::runtime_error("Unexpected hard process structure");
    }

    return true;
} // ParticleLoop


void Pythia8Tree::ParticleAdd(unsigned prt, int saveStatus)
{
    mPrtclEvent->AddPrtcl(mEvent[prt].px(),
                          mEvent[prt].py(),
                          mEvent[prt].pz(),
                          mEvent[prt].e(),
                          mEvent[prt].id(),
                          saveStatus);
} // ParticleAdd


void Pythia8Tree::ParticleAdd(TLorentzVector prt, int id, int status)
{
    mPrtclEvent->AddPrtcl(prt.Px(),
                          prt.Py(),
                          prt.Pz(),
                          prt.E(),
                          id,
                          status);
}


void Pythia8Tree::GhostHadronAdd(unsigned prt)
{
    int id = mEvent[prt].idAbs();

    if (HadrFuncs::HasBottom(id) && !IsExcitedHadronState(prt,5))
        ParticleAdd(prt,5);
    else if (HadrFuncs::HasCharm(id) && !IsExcitedHadronState(prt,4))
        ParticleAdd(prt,4);
    else if (mUseStrange && (HadrFuncs::HasStrange(id) && !IsExcitedHadronState(prt,3)))
        ParticleAdd(prt,3);
} // GhostHadronAdd


bool Pythia8Tree::IsExcitedHadronState(unsigned prt, int quarkId)
{
    vector<int> daughters = mEvent[prt].daughterList();
    for (int& dtr : daughters)
        if (HadrFuncs::StatusCheck(quarkId, mEvent[dtr].idAbs()))
            return true;
    return false;
} // IsExcitedHadronState


inline bool Pythia8Tree::Absent(unsigned int prt)
{
    return std::find(mSpecialIndices.begin(),mSpecialIndices.end(),prt)==mSpecialIndices.end();
}


///////////////////////////////////////////////////
// Event type specific ProcessParticle functions //
///////////////////////////////////////////////////


bool Pythia8Tree::ProcessParticle(unsigned prt)
{
    int id = mEvent[prt].idAbs();
    int stat = mEvent[prt].statusAbs();

//     if (stat>19&&stat<30) {
//         cout << stat << " " << id << " " << mEvent[mEvent[prt].mother1()].id() << " " << mEvent[mEvent[prt].mother2()].id() << endl;
//     }
    if (stat==71 || stat==72) { /* Alt. 61-63 */
        /* Save the combined final parton level */
        ParticleAdd(prt, 6);
        return true;
    } else if (mEvent[prt].isParton()) {
        if (stat==23) { /* Hard process activities. */
            ++mHardProcCount;
            /* Save hard process outgoing partons with and without a corrected momentum. */
            ParticleAdd(LastParton(prt), mEvent[prt].id(), 7);
            ParticleAdd(prt, 8);
        }
        return true;
    } else if (mEvent[prt].isHadron()) {
        /* Ghost hadrons can be final state particles */
        GhostHadronAdd(prt);
    }

    /* A process type -dependent custom process */
    int cp = CustomProcess(prt);
    if (cp==0)
        return false;
    else if (cp==1)
        return true;

    /* Final-state particles */
    if (mEvent[prt].isFinal() && Absent(prt)) {
        int saveStatus = 1;
        if (mEvent[prt].id()==22 && GammaChecker(prt)) saveStatus = 2;
        ParticleAdd(prt, saveStatus);
    }

    return true;
} // ProcessParticle : Base


inline int P8GenericTree::CustomProcess(unsigned prt)
{
    return 2;
} // CustomProcess : Generic


inline int P8DijetTree::CustomProcess(unsigned prt)
{
    return 2;
} // CustomProcess : Dijet


inline int P8GammajetTree::CustomProcess(unsigned prt)
{
    if (mSpecialIndices.size()==0 && mEvent[prt].statusAbs()==23 && mEvent[prt].idAbs()==22)
        return (GammaAdd(prt) ? 1 : 0);

    return 2;
} // CustomProcess : Gammajet


bool P8GammajetTree::GammaAdd(unsigned prt)
{
    /* Save a raw photon and the final photon distorted by ISR */
    ParticleAdd(prt, 8);
    double ptmp = mEvent[prt].pT();

    while (!mEvent[prt].isFinal()) {
        if (mEvent[prt].daughterList().size()>1)
            return false; // Detect pair production
        prt = mEvent[prt].daughter1();
    }

    if (mEvent[prt].idAbs()==22) {
        mSpecialIndices.push_back(prt);
        ParticleAdd(prt, 7);
        ++mHardProcCount;
        return true;
    }

    cerr << "Failed to locate final-state photon! " << ++mNumErrs << endl;
    return false;
} // GammaAdd


/* This is based on an assumption made about status codes - there are safeguards in muonadd. */
inline int P8ZmumujetTree::CustomProcess(unsigned prt)
{
    if (mSpecialIndices.size()==0 && mEvent[prt].statusAbs()==22 && mEvent[prt].idAbs()==23)
        return MuonAdd(prt);

    return 2;
} // CustomProcess : Zmumujet


bool P8ZmumujetTree::MuonAdd(unsigned prt)
{
    /* Save a raw Z0 and the muons distorted by ISR and brehmsstrahlung */
    ParticleAdd(prt, 8);

    while (mEvent[mEvent[prt].daughter1()].idAbs()==23)
            prt = mEvent[prt].daughter1();

    for (int daughter : mEvent[prt].daughterList()) {
        if (mEvent[daughter].idAbs()==13)
            mSpecialIndices.push_back(daughter);
    }

    /* Descend to the final muon forms; counter checks for absurd cases. */
    for (unsigned i = 0; i < mSpecialIndices.size(); ++i) {
        unsigned counter = 0;
        while (!mEvent[mSpecialIndices[i]].isFinal() && counter++ < 100) {
            for (auto daughter : mEvent[mSpecialIndices[i]].daughterList()) {
                if (mEvent[daughter].idAbs()==13)
                    mSpecialIndices[i] = daughter; break;
            }
        }
        ParticleAdd(mSpecialIndices[i], 7);
    }

    if (mSpecialIndices.size()==2) {
        ++mHardProcCount;
        return true;
    }

    cerr << "Failed to locate muon pair! " << ++mNumErrs << endl;
    return false;
} // MuonAdd


inline int P8ttbarjetTree::CustomProcess(unsigned prt)
{
    /* Add the W's */
    if (mEvent[prt].statusAbs()==22 && mEvent[prt].idAbs()==24) {
        ParticleAdd(LastParton(prt), mEvent[prt].id(), 7);
        ParticleAdd(prt,8);
        return 1;
    }

    /* Add the outgoing hard process lepton */
    if (mEvent[prt].statusAbs()==23 && mEvent[prt].idAbs() < 20)
        return (LeptonAdd(prt) ? 1 : 0);

    return 2;
} // ProcessParticle : ttbarjet


bool P8ttbarjetTree::LeptonAdd(unsigned prt)
{
    /* Save a raw lepton before separation to neutrinos and charged leptons */
    ParticleAdd(prt, 8);

    /* Charged lepton input: find a final-state charged lepton. */
    int type = mEvent[prt].idAbs()%2;
    if (type) {
        /* Charged leptons */
        while (!mEvent[prt].isFinal()) {
            vector<int> leptons = mEvent[prt].daughterList();
            prt = 0;
            for (int daughter : leptons) {
                int dType = mEvent[daughter].idAbs()%2;
                if (mEvent[daughter].isLepton() && dType==1)
                    prt = daughter;
            }

            /* This occurs around 25-30% of the time originating from tau decay */
            if (prt == 0)
                return false; /* Charged lepton decays to partons and a neutrino */
        }

        if (mEvent[prt].idAbs()==15)
            throw std::logic_error("Final-state tau spotted.");
    } else {
        /* Neutrinos - also saved. (Secondary neutrinos ignored.) */
        while (!mEvent[prt].isFinal()) {
            vector<int> leptons = mEvent[prt].daughterList();
            if (leptons.size()>1)
                throw std::logic_error("Neutrino decay spotted.");
            prt = leptons[0];
        }
    }
    mSpecialIndices.push_back(prt);
    ParticleAdd(prt, 7);
    return true;
} // LeptonAdd


/* Does a photon originate from pions? */
bool Pythia8Tree::GammaChecker(unsigned prt)
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


/* Corrected parton momentum (contrast to FS partons 71 and 72) */
TLorentzVector Pythia8Tree::LastParton(unsigned prt)
{
    if (mEvent[prt].statusAbs() == 62 || mEvent[prt].statusAbs() == 63 || mEvent[prt].isFinal())
        return TLorentzVector(mEvent[prt].px(),mEvent[prt].py(),mEvent[prt].pz(),mEvent[prt].e());

    TLorentzVector cumulator;
    for (auto &daughter : mEvent.daughterList(prt)) {
        //if (mEvent[daughter].idAbs() < 30) {
        if (mEvent[daughter].statusAbs() < 70) {
            cumulator += LastParton(daughter);
        } else {
            /* If there is even one hadron in the resulting particles, hadronization has started. */
            return TLorentzVector(mEvent[prt].px(),mEvent[prt].py(),mEvent[prt].pz(),mEvent[prt].e());
        }
    }

    return cumulator;
}


/* Graphviz printing for a single particle */
void Pythia8Tree::PrintParticle(unsigned prt)
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
void Pythia8Tree::PrintEvent()
{
    cout << "digraph test {" << endl;
    cout << "randir=LR;" << endl;
    cout << "ranksep=1.5;" << endl;
    cout << "node [width=0.03,height=0.03,shape=point,label=\"\"];" << endl;
    for (unsigned prt = 0; prt!=mEvent.size(); ++prt)
        PrintParticle(prt);
    cout << "}" << endl;
}
