/////////////////////////////////////////////////
// This is kind of an etchy patch for plotting //
// the particles in the y/phi plane.           //
// We use the constituents storage for saving  //
// the particle type (yes, this is not clean). //
// Const=1: final-state particles that belong  //
//          into jet constituents.             //
// Const=2: final-state particles, all. The    //
//          history flavor is also saved.      //
// Const=3/4: LO particles/antiparticles.      //
// Const=5: the corrected LO momentum.         //
// Const=6: FS partons with history flavor.    //
// Const=7/8: FS partons/anti-partons.         //
// Const=9: Status 2 particles.                //
// Hannu Siikonen 13.6.2016                    //
/////////////////////////////////////////////////

#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "JetBase.h"

using std::cout;
using std::endl;
using std::vector;
using std::cerr;

class JetPlot : public JetBase
{
public:
    JetPlot(TTree * tree, const char* file1, const char* file2, Int_t mode, Int_t definition) :
        JetBase::JetBase(tree, file1, file2, mode, definition)
    {
        fParticleStudy = true;
    }

    virtual void     InitLoop();
    virtual void     PostLoop();
    //virtual void     PostProcessing();
};

#endif // ANALYSIS_H
