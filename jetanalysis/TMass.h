/////////////////////////////////////////////////
// T mass analysis                             //
// Hannu Siikonen 13.6.2016                    //
/////////////////////////////////////////////////

#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "JetBase.h"

class TMass : public JetBase
{
public:
    TMass(TTree * tree, const char* file1, const char* file2, Int_t mode, Int_t definition) :
        JetBase::JetBase(tree, file1, file2, mode, definition)
    {
        fParticleStudy = false;
        fMode = 4;
        fAddNonJet = true;
    }

    virtual bool     SelectionParams();
    virtual void     PostLoop();
};

#endif // ANALYSIS_H
