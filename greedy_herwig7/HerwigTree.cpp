#include "HerwigTree.h"
using namespace Herwig;
using namespace ThePEG;

/** STRUCTURES FOR STORING NECESSARY PARTICLE DATA ACCORDING TO THE EVENT TYPE: */

#ifdef STORE_PRTCLS
/* The danish number system is very complicated, so better just divide by GeV */
void HerwigTree::ParticleAdd(const tPPtr& part, char jetid)
{
    auto pdgid = part->id();
    if (pdgid==22 and GammaChecker(part)) pdgid = 20;

    if (jetid >= 0) { // Associated with jets
        mJetId.push_back(jetid);
        mPDGID.push_back(pdgid);
        mPt.push_back(part->momentum().perp()/GeV);
        mEta.push_back(part->momentum().eta());
        mPhi.push_back(part->momentum().phi());
        mE.push_back(part->momentum().e()/GeV);
    } else { // No jet association
        mNIJPDGID.push_back(pdgid);
        mNIJPt.push_back(part->momentum().perp()/GeV);
        mNIJEta.push_back(part->momentum().eta());
        mNIJPhi.push_back(part->momentum().phi());
        mNIJE.push_back(part->momentum().e()/GeV);
    }
} // ParticleAdd
#endif

/* The danish number system is very complicated, so better just divide by GeV */
void HerwigTree::PartonAdd(const tPPtr& part, char jetid, char tag, int ptnid, int ownid)
{
    mPJetId.push_back(jetid);
    mPPtnId.push_back(ptnid);
    mPOwnId.push_back(ownid);
    mPTag.push_back(tag);
    mPPDGID.push_back(part->id());
    mPPt.push_back(part->momentum().perp()/GeV);
    mPEta.push_back(part->momentum().eta());
    mPPhi.push_back(part->momentum().phi());
    mPE.push_back(part->momentum().e()/GeV);
    mDR.push_back(-1);
} // PartonAdd

void HerwigTree::PartonAdd(unsigned num, char jetid)
{
    assert(num < mPartonInfo.size() and ((jetid < 0) or (unsigned) jetid < mSortedJets.size()));
    mPJetId.push_back(jetid);
    mPPtnId.push_back(mPartonInfo[num].ptnid);
    mPOwnId.push_back(mPartonInfo[num].ownid);
    mPPDGID.push_back(mPartonInfo[num].id);
    mPTag.push_back(mPartonInfo[num].tag);
    mPPt.push_back(mPartonInfo[num].p4.pt());
    mPEta.push_back(mPartonInfo[num].p4.eta());
    // Uniformize phi values between mEvent and PseudoJet
    mPPhi.push_back(mPartonInfo[num].p4.phi() - (mPartonInfo[num].p4.phi()>fastjet::pi ? 2*fastjet::pi : 0));
    mPE.push_back(mPartonInfo[num].p4.e());
    mDR.push_back(jetid<0 ? -1.0 : mPartonInfo[num].p4.delta_R(mSortedJets[jetid]));
} // PartonAdd

void HerwigTree::JetAdd(unsigned jet, int spoil)
{
    mJPt.push_back(mSortedJets[jet].pt());
    mJEta.push_back(mSortedJets[jet].eta());
    mJPhi.push_back(mSortedJets[jet].phi());
    mJE.push_back(mSortedJets[jet].e());
    if (spoil==0) {
        /* Start messing around with jet constituents */
        mJetParts = sorted_by_pt(mSortedJets[jet].constituents());
        Cuts();
        mConst.push_back(mCutJetParts.size());
        mPTD.push_back(PTD());
        mSigma2.push_back(Sigma2());
    }
} // JetAdd

void HerwigTree::PartonAppend(fastjet::PseudoJet p4, tPPtr part, char tag, int ptnid)
{
    mPartonInfo.push_back(PartonHolder{p4, (int) part->id(), tag, ptnid, part->number(), false});
    p4 *= cfg::GhostCoeff;
    p4.set_user_index(-mPartonInfo.size());
    mJetInputs.push_back(p4);
    //mPJetInputs.push_back(p4);
}


/* TRIVIAL STUFF FOR EVENT ANALYSIS */

void HerwigTree::Initialize()
{
    /* Particle lvl */
    mIsolation.clear();

    mJetId.clear();
    mPDGID.clear();
    mPt.clear();
    mEta.clear();
    mPhi.clear();
    mE.clear();
    mNIJPDGID.clear();
    mNIJPt.clear();
    mNIJEta.clear();
    mNIJPhi.clear();
    mNIJE.clear();
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
    //mNJetInputs.clear();
    //mPJetInputs.clear();
    mSortedJets.clear();
    mJetParts.clear();
    mCutJetParts.clear();

    mPartonInfo.clear();
    mAncestry.clear();
    mNAncestry.clear();
    mWs.clear();

    mFinals.clear();
    mGammas.clear();
    mMuons.clear();
    mChargeds.clear();

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

    mLeptonFriend = 0;

    /* Special particle indices are saved to eliminate saving overlap. */
    mSpecialIndices.clear();

    mInfo = 0;

    mMetVect = fastjet::PseudoJet();
    mHardProcCount = 0;
    mWeight = mEvent->weight();
    mHardHandler = mEvent->primaryCollision()->handler();
} // Initialize


void HerwigTree::analyze(tEventPtr event, long ieve, int loop, int status)
{
    try {
        if (loop > 0 or status != 0 or !event) return;

        if (cfg::DebugGViz) event->printGraphviz();

        mEvent = event;
        Initialize();
        mTotWgt += mWeight;
        if (!HardProc())          { Repeat(); return; }
        if (!FinalState())        { Repeat(); return; }
        if (!IsolationProc())     { Repeat(); return; }
        if (!JetLoop())           { Repeat(); return; }
        if (!IsolationFromJets()) { Repeat(); return; }

        mSelWgt += mWeight;
        mTree->Fill();

        if (ieve%mTimerStep==0 and ieve>0) mTimer.printTime();
    } catch (std::exception& e) {
        cout << "An error occurred: " << e.what() << endl;
    }
} // analyze


bool HerwigTree::IsolationProc()
{
    if (mMode==2) {
        for (auto &gm : mGammas) {
            mIsolation.push_back(IsolationPhotons(gm.second,gm.first));
            PartonAdd(mFinals[gm.second],-1,6);
        }
    } else if (mMode==3) {
        for (auto &mu : mMuons) {
            mIsolation.push_back(IsolationMuons(mu.second,mu.first));
            PartonAdd(mFinals[mu.second],-1,6);
        }
    } else if (mMode>=4) {
        /* Adding Isolation monitoring for all present charged particles */
        for (auto &ch : mChargeds) {
            mIsolation.push_back(IsolationLeptons(ch.second,ch.first));
            PartonAdd(mFinals[ch.second],-1,6);
        }
    }

    return true;
} // Veto

/* Notation: (i)prt the index of a particle, (i)part the corresponding PseudoJet.
 * The prefix 'i' stands for isolation (i.e. the particle which we study). */
bool HerwigTree::IsolationPhotons(int iprt, fastjet::PseudoJet& ipart)
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
                int absId = abs(mFinals[prt]->id());
                bool ischarged = IsCharged(absId);

                if (ischarged and part.pt() > 0.5 and dR > 0.05) pTsum += part.pt();

                E_tot04 += part.e();
                if (dR<0.3) {
                    /* For charged particles & photons the coefficient is one and for other neutrals 0.5. */
                    double EMcoeff = (ischarged or absId == 22) ? 1.0 : 0.5;

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
bool HerwigTree::IsolationMuons(int iprt, fastjet::PseudoJet& ipart)
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
                int absId = abs(mFinals[prt]->id());
                bool ischarged = IsCharged(absId);
                if (ischarged and part.pt() > 0.4 and dR > 0.05) pTsumTrk += part.pt();

                if (dR < 0.4) {
                    /* For charged particles & photons the coefficient is one and for other neutrals 0.5. */
                    double EMcoeff = (ischarged or absId == 22) ? 1.0 : 0.5;

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
bool HerwigTree::IsolationLeptons(int iprt, fastjet::PseudoJet& ipart)
{
    int iabsId = abs(mFinals[iprt]->id());
    if (cfg::DOD0) { /* D0 JES style algorithm (implemented on gen lvl) */
        if (iabsId==11) {
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
                    int absId = abs(mFinals[prt]->id());
                    bool ischarged = IsCharged(absId);
                    if (ischarged and part.pt() > 0.5 and dR > 0.05) pTsumTrk += part.pt();

                    /* For charged particles & photons the coefficient is one and for other neutrals 0.5. */
                    double EMcoeff = (ischarged or absId == 22) ? 1.0 : 0.5;

                    E_tot04            +=         part.e();
                    E_EM04             += EMcoeff*part.e();
                    if (dR<0.2) E_EM02 += EMcoeff*part.e();
                }
            }

            /* Pseudorapidity, p_T, etc. limits for isolated electrons. */
            return (E_tot04-E_EM02)/E_EM02 < 0.07 /* fiso      in the D0 language */
            and E_EM04/E_tot04          > 0.9  /* fEM       in the D0 language */
            and pTsumTrk                < 2.5; /* sum pttrk in the D0 language */
        } else if (iabsId==13) {
            return IsolationMuons(iprt, ipart);
        } else {
            cout << iabsId << endl;
            throw std::logic_error("Unknown form of a charged lepton observed.");
        }

        return true;
    } else { /* CMS STYLE ALGORITHM */
        double EdR = 0;
        bool isEl = iabsId==11;
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


bool HerwigTree::FinalState()
{
    /* In ttbar events, we store the b hadrons (also intermediate) */
    if (mMode>=4) {
        tPVector all;
        mEvent->select(std::back_inserter(all),SelectAll());
        for (auto &part : all) {
            auto id = abs(part->id());
            if (IsHadron(id) and IsExcitedHadronState(part,5)==0)
                PartonAppend(PseudoJettify(part),part,(char)9,-1);
        }
    }

    /* Final state particles */
    for (auto &part : mEvent->getFinalState()) {
        auto finalIdx = part->number();
        auto prt = PseudoJettify(part);
        int currIdx = mFinals.size();
        prt.set_user_index(currIdx);
        mFinals.push_back(part);

        int idAbs = abs(part->id());
        /* Adding all interesting muons, photons and charged leptons for isolation monitoring. */
        if (prt.perp() > cfg::NuisancePt and fabs(prt.eta()) < cfg::NuisanceEta) {
            if (mMode==2 and idAbs==22) {
                mGammas.push_back(std::make_pair(prt,currIdx));
            } else if (mMode==3 and idAbs==13) {
                mMuons.push_back (std::make_pair(prt,currIdx));
            } else if (mMode>=4 and (idAbs==11 or idAbs==13 or idAbs==15)) {
                mChargeds.push_back(std::make_pair(prt,currIdx));
            }
        }

        /* Do not add neutrinos to "observed" particles. */
        if (idAbs==12 or idAbs==14 or idAbs==16) {
            mMetVect += prt;
            if (mMode>=4 and Absent(finalIdx)) {
                auto parent = part->parents()[0];
                while (abs(parent->id())==24)
                    parent = parent->parents()[0];
                auto feeling = abs(parent->id())/100;
                if (feeling>9)
                    feeling /= 10;
                if (feeling==5) ++mNuOB;
                else if (feeling==4) ++mNuOC;
                else if (IsLepton(abs(parent->id()))) ++mNuOLept;
                else ++mNuOOther;
                PartonAdd(part,-1,8);
            }
        } else if (cfg::StoreNIJ or Absent(finalIdx)) {
            /* Special particles ignored, unless we are using Toni's special processing. */
            /* Neutrinos ignored always. This is supposed to mimick the particles observed
             * by a particle detector, such as the CMS or D0. */
            mJetInputs.push_back(prt);
        }
    }

    return true;
} // FinalState


bool HerwigTree::JetLoop()
{
    fastjet::ClusterSequence clustSeq(mJetInputs, mJetDef);
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
                ParticleAdd(mFinals[prt],jet);
                ParticleAdd(mFinals[prt],-1);
                #endif
                continue;
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
        for (unsigned int a=0; a!=jetIndices.size(); ++a) {
            int prt = mJetInputs[a].user_index();
            if (prt > 0 and jetIndices[a] == -1) ParticleAdd(mFinals[prt],-1);
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


bool HerwigTree::IsolationFromJets()
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

/* HARD PROCESS ANALYSIS (NONTRIVIAL) */

bool HerwigTree::HardProc()
{
    /* We need this to extract the pthat */
    float minPt = 100000;
    /* Counters and a storage for ttbar events */
    int bCount = 0, qCount = 0;
    vector<tPPtr> topLepts;

    /* The hardest subprocess
        Hard process (method 1 - brings also the id 82 collimations)
    const ParticleVector hardParts = event->primarySubProcess()->outgoing(); (const_iterator)
        Hard process (method 2 - preferred, and used in this code) */
    tPVector hardParts = mEvent->primaryCollision()->step(0)->getFinalState();

    for (auto &hard : hardParts) {
        int absId = abs(hard->id());

        if (absId==82) continue;
        auto part = OptimalParton(hard);

        #ifdef DEBUG
        cerr << "id: " << absId << ": " << hard->number() << ": " << hard->momentum().perp()/GeV << endl;
        cerr << absId << ": " << hard->momentum().perp()/GeV << " " << first.perp()/GeV << " " << second.perp()/GeV << endl;
        #endif
        minPt = min(minPt,part->momentum().perp()/GeV);

        if (mMode==2 and absId==22) {
            if (!GammaAdd(part)) return false;
        } else if (mMode==3 and absId==13) {
            if (!MuonAdd(part)) return false;
        } else if (mMode>=4 and absId==6) {
            auto topc = Probe(part);
            int wInd = -1;
            for (auto &child : topc) {
                auto childId = abs(child->id());
                if (childId > 10 and childId < 20) {
                    topLepts.push_back(child);
                } else if (childId==24) {
                    PartonAppend(PseudoJettify(child),child,(char) 2,-1);
                    wInd = child->number();
                    if (part->children().size()>0 and IsLepton(part->children()[0]->id()))
                        mLeptonFriend = part->number();
                } else if (childId==5) {
                    ++bCount;
                    PartonAppend(PseudoJettify(child),child,(char) 0,-1);
                    PartonDescend(child);
                    BNeutrinos(child);
                } else if (childId<5) {
                    ++qCount;
                    if (wInd < 0)
                        wInd = part->number();
                    PartonAppend(PseudoJettify(child),child,(char) 0,wInd);
                    PartonDescend(child);
                } else if (childId==22) {
                    throw runtime_error("Illegal top photon activity!");
                } else if (childId==21) { // This is odd
                    PartonAppend(PseudoJettify(child),child,(char) 0,-1);
                    PartonDescend(child);
                } else {
                    throw runtime_error(Form("Unexpected t consequences! %ld",childId));
                }
            }

            if (wInd==part->number()) throw runtime_error("W lost. :(");
        } else if (absId<10 or absId==21) {
            PartonAppend(PseudoJettify(part),part,(char) 0,-1);
            PartonDescend(part);
        } else {
            cerr << "Unidentified ME particle." << endl;
            continue;
        }
        ++mHardProcCount;
    }
    mPtHat = minPt;

    /* ttbar events: seek lepton+jets (one w to leptons, one to quarks) */
    if (mMode>=4) {
        if ((bCount!=2 or qCount!=2 or topLepts.size()!=2)) {
            AddMessage("Non-semileptonic ttbar",mWeight);
            return false;
        }
        for (auto &tl : topLepts) if (!LeptonAdd(tl,mLeptonFriend)) return false;
    }

    if (mMode==3) --mHardProcCount;

    /* Sanity checks */
    if (   (mMode<4  and mHardProcCount !=2)
        or (mMode==2 and mSpecialIndices.size()!=1)
        or (mMode==3 and mSpecialIndices.size()!=2)
        or (mMode>=4 and mHardProcCount != 2) )
    {
        throw std::logic_error("Unexpected hard process structure");
    }

    return true;
}


bool HerwigTree::GammaAdd(tPPtr gamma)
{
    gamma = OptimalParton(gamma);
    /* No pair production */
    if (gamma->children().size()>0 or gamma->decayed()) {
        AddMessage("Signal photon pair production.",mWeight);
        return false;
    }
    mSpecialIndices.push_back(gamma->number());
    PartonAdd(gamma,-1,(char) 2);
    return true;
}


bool HerwigTree::MuonAdd(tPPtr muon)
{
    muon = OptimalParton(muon);
    while (muon->decayed() > 0 or muon->children().size()>0) {
        for (auto &mu : muon->children()) {
            if (abs(mu->id())==ParticleID::muminus) {
                muon = mu;
                continue;
            }
        }
    }
    mSpecialIndices.push_back(muon->number());
    PartonAdd(muon,-1,(char) 2);
    return true;
}


bool HerwigTree::LeptonAdd(tPPtr lepton, int parent)
{
    int type = abs(lepton->id())%2;
    if (type) {
        /* Charged leptons */

        /* Save a raw charged lepton */
        PartonAdd(lepton, (char)-1, '\2',parent, lepton->number());

        /* Check for tau */
        if (abs(lepton->id())==15)
            mInfo |= 1;

        while (lepton->decayed() or lepton->children().size()>0) {
            bool hadrons = false;
            auto previdx = lepton->number();
            for (auto &child : lepton->children()) {
                int absId = abs(child->id());
                if (IsHadron(absId)) {
                    hadrons = true;
                    break;
                } else if (absId==22) {
                    auto optchild = OptimalParton(child);
                    if (optchild->momentum().perp()/GeV>0.1)
                        PartonAdd(OptimalParton(child),-1,'\2', parent, optchild->number());
                } else if (IsLepton(absId)) {
                    if (absId%2==1) {
                        lepton = child;
                        hadrons = false;
                    } else {
                        /* Neutrino sent into the neutrino loop */
                        LeptonAdd(child, mLeptonFriend);
                    }
                } else if (absId==24) {
                    lepton = child;
                } else {
                    cerr << "Unexpected lepton-loop result: " << absId << " " << lepton->id() << endl;
                }
            }

            if (hadrons) {
                /* This occurs around 25-30% of the time originating from tau decay */
                AddMessage("Partonic tau decay!",mWeight);
                return false;
            } else if (previdx == lepton->number()) {
                /* Check if stuck in a loop*/
                AddMessage("Lepton loop stuck!",mWeight);
                return false;
            }
        }
    } else {
        /* Neutrinos */
        lepton = OptimalParton(lepton);
        if (lepton->decayed() or lepton->children().size()>0)
            throw std::logic_error("Neutrino decay spotted.");
    }

    mSpecialIndices.push_back(lepton->number());
    PartonAdd(lepton,-1,(char) 3, parent,lepton->number());
    return true;
}


vector<tPPtr> HerwigTree::Probe(tPPtr part, int lvl) const {
    vector<tPPtr> hards;
    for (auto &child : part->children()) {
        auto absId = abs(child->id());
        auto oChild = OptimalParton(child);
//         if (absId==22) {
//             cout << part->id() << endl;
//             for (auto &port : part->children())
//                 cout << "  " << port->id() << endl;
//         }
        if (absId==6 or absId==24) {
            auto results = Probe(oChild,lvl+1);
            hards.reserve(hards.size() + results.size());
            if (absId==24)
                hards.push_back(oChild);
            hards.insert(hards.end(), results.begin(), results.end());
        } else if (absId==22 and abs(part->id())==6) {
            continue;
        } else
            hards.push_back(oChild);
    }
    return hards;
} // Probe


void Herwig::HerwigTree::BNeutrinos(const tPPtr& part)
{
    if(part->children().empty() and !part->decayed()) {
        auto absId = abs(part->id());
        if (absId!=12 and absId!=14 and absId!=16) return;
        if (part->parents().size()==0)
            throw runtime_error("No parents!!");
        auto parent = part->parents()[0];
        while (abs(parent->id())==24)
            parent = parent->parents()[0];
        auto feeling = abs(parent->id())/100;
        while (feeling>9)
            feeling /= 10;
        if (feeling==5) ++mNuB;
        else if (feeling==4) ++mNuC;
        else if (IsLepton(abs(parent->id()))) ++mNuLept;
        else ++mNuOther;
        ++mBNuCount;
        PartonAdd(part,-1,7);
        mSpecialIndices.push_back(part->number());
    } else {
        for (auto &child : part->children())
            BNeutrinos(child);
    }
} // BNeutrinos


void HerwigTree::PartonDescend(tPPtr part)
{
    for (auto &dtr : part->children()) {
        auto truth = OptimalParton(dtr);
        if (part->momentum().perp()/GeV<4)
            continue;
        auto absId = abs(truth->id());
        if (IsParton(absId)) {
            PartonAppend(PseudoJettify(truth),truth,4,part->number());
            PartonDescend(truth);
        }
    }
}


tPPtr HerwigTree::OptimalParton(tPPtr part) const
{
    while (part->children().size()==1) {
        auto trial = (part->children())[0];
        auto absId = abs(trial->id());
        if (trial->parents().size()>1 or absId>100 or absId==81)
            return part;
        part = trial;
    }

    if (part->children().size()==0 and !IsLepton(abs(part->id())) and part->id()!=22)
        cerr << "No partonic children?" << endl;

    return part;
}

Lorentz5Momentum HerwigTree::FinalMomenta(tPPtr parent) const {
    Lorentz5Momentum dummy;
    if(parent->children().empty()) {
        return parent->momentum();
    } else {
        for (auto &child : parent->children())
            dummy += FinalMomenta(child);
    }
    return dummy;
}

Lorentz5Momentum HerwigTree::FinalPMomenta(tPPtr parent) const {
    if(parent->children().empty()) {
        return parent->momentum();
    }
    for (auto& child : parent->children()) {
        auto absId = abs(child->id());
        if (absId>100 || absId==81)
            return parent->momentum();
    }

    Lorentz5Momentum dummy;
    for (auto &child : parent->children())
        dummy += FinalPMomenta(child);
    return dummy;
}




/* JET PROPERTIES */

double HerwigTree::PTD()
{
    double square = 0, linear = 0;
    for(size_t q = 0; q != mCutJetParts.size(); ++q) {
        square += pow(mCutJetParts[q].pt(),2);
        linear += mCutJetParts[q].pt();
    }
    return sqrt(square)/linear;
}

double HerwigTree::Sigma2()
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

void HerwigTree::Cuts()
{
    mCutJetParts.clear();

    if (cfg::CutMode) {
        /* Explicit cuts (pt cut for photons and neutral hadrons) */
        vector<fastjet::PseudoJet> tmpParts;
        for (auto &q : mJetParts) {
            if (q.user_index() < 0) continue;
            int id = abs(mFinals[q.user_index()]->id());
            if (q.pt()>1 or (id != 22 and (!IsHadron(id) or IsCharged(id))))
                tmpParts.push_back(q);
        }

        /* Implicit cuts (pt cut for hadrons - harsher for neutral hadrons) */
        for (auto &q : tmpParts) {
            int id = abs(mFinals[q.user_index()]->id());
            if (!IsHadron(id) or ((IsCharged(id) and q.pt()>0.3) || (!IsCharged(id) and q.pt()>3)))
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

bool HerwigTree::IsHadron(int pdg) const
{
    if(pdg>99) return true;
    return false;
}

bool HerwigTree::IsParton(int pdg) const
{
    if(pdg<=6 or pdg==21) return true;
    return false;
}

bool HerwigTree::IsLepton(int pdg) const
{
    if (pdg>10 and pdg<20) return true;
    return false;
}

bool HerwigTree::IsCharged(int pdg) const
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





/* VARIOUS HELPER FUNCTIONALITIES */

bool HerwigTree::GammaChecker(const tPPtr& photon) const {
    /* One mother, which is pi0 */
    auto parents = photon->parents();
    if (parents.size()!=1 || parents[0]->id()!=ParticleID::pi0) return false;

    auto children = parents[0]->children();
    if (children.size()!=2) return false;

    double eDifference = fabs((parents[0]->momentum().t()-children[0]->momentum().t()-children[1]->momentum().t() )/GeV);
    if (eDifference > 0.001) return false;

    return true;
}

int HerwigTree::IsExcitedHadronState(const tPPtr& part, int quarkId) const
{
    assert(quarkId>=0 && quarkId<=5);
    if (!HadrFuncs::StatusCheck(quarkId, part->id()))
        return -1;

    ParticleVector children = part->children();
    for (auto &child : part->children()) if (HadrFuncs::StatusCheck(quarkId, child->id())) return 1;

    return 0;
}

int HerwigTree::GetStatusCode(const tPPtr& part) const
{
    int status = 1;
    if (!part->children().empty() || part->next()) {
        tStepPtr step = part->birthStep();
        if ((!step || (step && (!step->handler() || step->handler() == mHardHandler))) && part->id() != 82)
            status = 3;
        else
            status = 2;
    }
    return status;
}

void HerwigTree::Print_parents(const tPPtr& part) const {
    cout << " " << part->number() << " " << part->id() << "  ";
    for (auto &parent : part->parents())
        cout << parent->number() << "-" << GetStatusCode(parent) << " ";
    cout << endl;
    for (auto &parent : part->parents())
        Print_parents(parent);
}

void HerwigTree::Iter_print(tPPtr prt, unsigned gen) const {
    cout << gen++ << " " << prt->id() << endl;
    const ParticleVector childs = prt->children();
    for (ParticleVector::const_iterator part = childs.begin(); part != childs.end(); ++part)
        Iter_print(*part, gen);
}

inline bool HerwigTree::Absent(unsigned int num) const
{
    return std::find(mSpecialIndices.begin(),mSpecialIndices.end(),num)==mSpecialIndices.end();
}

inline void HerwigTree::AddMessage(string msg, double wgt) {
    if (mErrorList.count(msg)==0)
        mErrorList[msg] = wgt;
    else
        mErrorList[msg] += wgt;
}

/* A very handy handle */
inline TLorentzVector HerwigTree::TLorentzify(const tPPtr& part) const
{
    return TLorentzVector(part->momentum().x()/GeV,part->momentum().y()/GeV,part->momentum().z()/GeV,part->momentum().t()/GeV);
}

/* Another handy handle */
inline fastjet::PseudoJet HerwigTree::PseudoJettify(const tPPtr& part) const
{
    return fastjet::PseudoJet(part->momentum().x()/GeV,part->momentum().y()/GeV,part->momentum().z()/GeV,part->momentum().t()/GeV);
}

inline fastjet::PseudoJet HerwigTree::PseudoJettify(TLorentzVector p4) const {
    return fastjet::PseudoJet(p4.Px(),p4.Py(),p4.Pz(),p4.E());
}
