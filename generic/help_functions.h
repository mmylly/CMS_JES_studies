#ifndef HELP_FUNCTIONS_H
#define HELP_FUNCTIONS_H

// Stdlib header file for input and output.
#include <iostream>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <chrono>
// Nice libraries from C
#include <cassert>
#include <cmath>
#include <cstdint>
// ROOT, histogramming
#include "TROOT.h"
#include "TMath.h"
#include "TProfile.h"

using std::cout;
using std::endl;
using std::string;
using namespace std::chrono;

////////////////////////////////////////////////////////////////////
/// @brief Timing class for showing the progress of a simulation.
/////////////////////////////////////////////////////////////////
class Timer {
private:
    /** Starting time */
    high_resolution_clock::time_point mStart;
    /** Total number of events */
    int mTotal;
    /** Spacing of time/event lattice */
    int mDelta;
    /** Current event */
    int mCurr;
    /** Hours left */
    int mHours;
    /** Minutes left */
    int mMinutes;
    /** Seconds left */
    int mSeconds;

public:
    Timer(){}

    /** The user must set the number of total events and the amount of events in-between steps
    * @param eventNo Number of events to be processed.
    * @param dEvent Number of events in-between steps. */
    Timer(int eventNo, int dEvent): mTotal(eventNo), mDelta(dEvent) {}

    /** Destructor prints the total elapsed time. */
    ~Timer()
    {
        cout << "Time elapsed: ";
        cout << duration_cast<seconds>(high_resolution_clock::now()-mStart).count();
        cout << " s." << endl;
    }

    /** The user can also set afterwards the number of total events and the amount of events in-between steps
     * @param eventNo Number of events to be processed.
     * @param dEvent Number of events in-between steps. */
    void setParams(int eventNo, int dEvent)
    {
        mTotal = eventNo;
        mDelta = dEvent;
    }

    /** Tells the timer to start. */
    void startTiming()
    {
        mCurr = 0;
        mStart = high_resolution_clock::now();
    }

    /** Counts the current time in readable units. */
    void countTime()
    {
        mCurr += mDelta;
        high_resolution_clock::time_point now = high_resolution_clock::now();
        double timeProcessor = duration_cast<seconds>(now-mStart).count();
        timeProcessor = timeProcessor*( ((double) mTotal)/mCurr-1);
        mMinutes =  timeProcessor/60;
        mHours = mMinutes/60;
        mSeconds = timeProcessor-60*mMinutes;
        mMinutes = mMinutes - mHours*60;
    }

    /** Prints current status. */
    void printTime() {
        countTime();
        cout << mCurr << " events processed, ETA : " << mHours << "h" <<
            mMinutes << "m" << mSeconds << "s." << endl;
    }
};


//////////////////////////////////////////////////////
/// @brief A struct for storing parton information.
///////////////////////////////////////////////////
struct PartonHolder {
    /** Parton four momentum. */
    fastjet::PseudoJet p4;
    /** Parton pdg id. */
    int id;
    /** The reason for which the parton was saved.
        - 0: ME Parton
        - 1: ME Parton with final-state momentum
        - 2: ME Lepton or photon
        - 3: ME Lepton or photon with final-state momentum
        - 4: Descendant of ME parton
        - 5: Descendant of ME parton with final-state momentum
        - 6: Subjects for isolation (photons or leptons)
        - 7: bottom neutrinos
        - 8: Other neutrinos
        - 9: Ghost bottom hadrons */
    char tag;
    /** The index of the parent parton in the program flow (0 if no parent). */
    int ptnid;
    /** The index of the current parton in the program flow. */
    int ownid;
    /** Indicate whether the parton is already in the use of a jet. */
    bool used;
};

///////////////////////////////////////////////////////////////////////////
/// @brief Some functions for PDG particle code recognition for hadrons.
///
/// These are basically implementations taken from CMSSW.
///////////////////////////////////////////////////
namespace HadrFuncs
{
    /** Is there a top within the hadron (should not occur)?
     @param id pdg id of the hadron.
     @return True, if there is a top. */
    inline static bool HasTop(int id)
    {
        int code1 = (abs(id)/ 100)%10;
        int code2 = (abs(id)/1000)%10;
        if (code1 == 6 or code2 == 6) return true;
        return false;
    }

    /** Is there a bottom within the hadron?
     @param id pdg id of the hadron.    *
     @return True, if there is a b quark. */
    inline static bool HasBottom(int id)
    {
        int code1 = (abs(id)/ 100)%10;
        int code2 = (abs(id)/1000)%10;
        if (code1 == 5 or code2 == 5) return true;
        return false;
    }

    /** Is there a charm within the hadron?
     @param id pdg id of the hadron.    *  *
     @return True, if there is a c quark. */
    inline static bool HasCharm(int id)
    {
        int code1 = (abs(id)/ 100)%10;
        int code2 = (abs(id)/1000)%10;
        if (code1 == 4 or code2 == 4) return true;
        return false;
    }

    /** Is there a strange within the hadron?
     @param id pdg id of the hadron.    *  *
     @return True, if there is a s quark. */
    inline static bool HasStrange(int id)
    {
        int code1 = (abs(id)/ 100)%10;
        int code2 = (abs(id)/1000)%10;
        if (code1 == 3 or code2 == 3) return true;
        return false;
    }

    /** Is there a down within the hadron?
     @param id pdg id of the hadron.    *  *
     @return True, if there is a d quark. */
    inline static bool HasDown( int id )
    {
        int code1 = (abs(id)/ 100)%10;
        int code2 = (abs(id)/1000)%10;
        if (code1 == 2 or code2 == 2) return true;
        return false;
    }

    /** Is there an up within the hadron?
     @param id pdg id of the hadron.    *  *
     @return True, if there is a u quark. */
    inline static bool HasUp( int id )
    {
        int code1 = (abs(id)/ 100)%10;
        int code2 = (abs(id)/1000)%10;
        if (code1 == 1 or code2 == 1) return true;
        return false;
    }

    /** Is the given particle a hadron?
     @param id pdg id of the particle.    *  *
     @return True, if the given particle is a hadron. */
    inline static bool IsHadron(int id)
    {
        auto aid = abs(id);
        if (aid > 99) return true;
        return false;
    }


    /** Checks whether a hadron has the given quark.
     @param quarkId The pdg id of the given quark flavor.
     @param hadronId the pdg id of the given hadron.
     @return True, if there is a quark of the given flavor within the hadron. */
    static bool StatusCheck( int quarkId, int hadronId )
    {
        switch (quarkId) {
            case 6: return HasTop(hadronId); break;
            case 5: return HasBottom(hadronId); break;
            case 4: return HasCharm(hadronId); break;
            case 3: return HasStrange(hadronId); break;
            case 2: return HasDown(hadronId); break;
            case 1: return HasUp(hadronId); break;
            default: return false; break;
        }
    }
}

#endif // HELP_FUNCTIONS_H
