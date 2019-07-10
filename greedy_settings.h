#ifndef GREEDY_SETTINGS
#define GREEDY_SETTINGS

// Generic c++, so that these do not need to be listed each time
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
#include <typeinfo>

/* Adding the things that we need. */
using std::string;
using std::vector;
using std::pair;
using std::cout;
using std::endl;
using std::cerr;
using std::runtime_error;
using std::stoi;
using std::map;

/** @brief Configuration settings.
 *
 * The 'greedy' analysis programs are simple and short enough to define the conf. within code. */
namespace cfg {
    /** @name Debugging tools */
    //@{
    /** Mostly print some specific info. */
    constexpr bool Debug        = false;
    /** Print parton info. */
    constexpr bool DebugPartons = false;
    /** Print graphviz event info. */
    constexpr bool DebugGViz    = false;
    /** Print whole event. */
    constexpr bool DebugList    = false;
    /** Print event statistics. */
    constexpr bool DebugStat    = false;
    //@}

    /** @name Generic flags and settings */
    //@{
    /** While executing Herwig::HerwigTree::PTD and Herwig::HerwigTree::Sigma2: do we apply detector-like cuts. */
    constexpr bool CutMode = true;
    /** Do we add corrected momentum values for partons (idx 1)? */
    constexpr bool DOCorrParton   = false;
    /** Do we add uncorrected momentum values for signal leptons (idx 3)? */
    constexpr bool DOUnCorrLepton = false;
    constexpr bool DOMuons = false;
    //@}

    /** @name D0 settings (generic) */
    //@{
    /** General D0 settings (energy & so on). */
    constexpr bool DOD0      = false;
    /** Usage of D0 RunII Cone. */
    constexpr bool DOD0RIIC  = false;
    //@}

    /** @name D0 settings (specific for Toni's studies */
    //@{
    /** Tag photon close to central region for JES studies. */
    constexpr double GammaEtaD0Max = 1.0;
    /** Produce a b-jet enriched sample? */
    constexpr bool BEnriched = false;
    /** Demand min amount of |eta|<0.4 jets. */
    constexpr bool EtaCut    = false;
    /** Reject events w/ high-pT extra jets. */
    constexpr bool PtCut     = false;
    /** Use prtn lvl phase-space eta cut. */
    constexpr bool PSCut     = false;
    //@}

    /** @name Usage of final-state particles */
    //@{
    /** Do we store the final state particles? */
    #define STORE_PRTCLS
    /** Store particles not in jets (NIJ), REQUIRES STORE_PRTCLS */
    constexpr bool StoreNIJ = true;
    //@}

    /** @name Photon isolation parameters (gamma+jets) */
    //@{
    /** Delta R cone in which we look for other material than the photon. */
    constexpr double GammaDR  = 0.1;
    /** Limit fraction for other content than the photon, within the given GammaDR. */
    constexpr double GammaLim = 0.1;
    /** Minimum Pt for signal photon. */
    constexpr double GammaPt  = 30;
    /** Maximum Eta for signal photon. */
    constexpr double GammaEta = 2.1;
    //@}

    /** @name Muon isolation parameters (Zmumu+jets) */
    //@{
    /** Delta R cone in which we look for other material than the muon. */
    constexpr double MuonDR  = 0.1;
    /** Limit fraction for other content than the muon, with the given MuonDR. */
    constexpr double MuonLim = 0.1;
    /** Minimum Pt for signal muon. */
    constexpr double MuonPt  = 30;
    /** Maximum Eta for signal muon. */
    constexpr double MuonEta = 2.1;
    //@}

    /** @name Lepton isolation parameters (ttbarlepton+jets) */
    //@{
    /** Delta R cone in which we look for other material than the electron. */
    constexpr double ElDR = 0.3;
    /** Delta R cone in which we look for other material than the muon. */
    constexpr double MuDR = 0.4;
    /** Limit fraction for other content than the electron, with the given ElDR (tight: 0.12, loose, 0.20). */
    constexpr double ElLim = 0.12;
    /** Limit fraction for other content than the muon, with the given MuDR (tight: 0.15, loose: 0.25). */
    constexpr double MuLim = 0.15;
    //@}

    /** @name Some kinematic selections. */
    //@{
    /* Worst case kind of cuts for bonus leptons */
    constexpr double LeptPt = 5;
    constexpr double LeptEta = 2.4;

    /* General limits for "nuisance leptons/photons" that are taken into account for isolation. */
    constexpr double NuisancePt  = 7.0;
    constexpr double NuisanceEta = 5.0;
    //@}

    /** @name Jet parameters */
    //@{
    /** Pt limit to be used while checking the isolation vs. nearby jets. */
    constexpr double MinJetVisiblePt = 15.0;
    /** Pt limit to be used for fastjet clustering. */
    constexpr double MinJetClustPt   = 4.0;
    //@}

    /** @name Generic run parameters */
    //@{
    /* Coefficient for scaling the momentum of "ghosts" */
    constexpr double GhostCoeff = pow(10,-18);
    constexpr double RCone = DOD0 ? 0.5 : 0.4;
    //@}

    /** @name Pythia6 exclusive parameters. */
    //@{
    /* Length/Timescale deciding which particles decay */
    constexpr double P6CTau = 10.0;
    constexpr double P6Energy = DOD0 ? 1960 : 13000;

    constexpr double P6TMass = 173.7; // Top mass for ttbar events

    constexpr int P6Tune = 2;     // 0: Z2*, 1: CMS CUEP6S1, 2: Tevatron TuneA
    constexpr bool P6PthatW  = true;  // PtHat reweighting in other that ttbar events

    /* Switches for turning off things (normally these are not used) */
    constexpr bool P6NoMPI = false;
    constexpr bool P6NoISR = false;
    constexpr bool P6NoFSR = false;

    constexpr unsigned P6Seeds[] = {
        840744607,431166825, 11489507,859341684,719632152,384411333, 90405435,297596781,
        620424940,829585206,350220548,862060943,865146589, 11119376,706126850,761335296,
        286390445,408256820,447625541,368022699,281922559,852542479,509348179,175162098,
        688006297,512118632,676751467,212155085,158795947, 68988051,258456879,625579469,
        146828216,582720998,226158642,439232438,366169042,745702146,412672564,177882235
    };
    //@}
}

#endif // GREEDY_SETTINGS
