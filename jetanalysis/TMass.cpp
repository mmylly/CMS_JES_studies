#include "TMass.h"


bool TMass::SelectionParams()
{
    return true;
}

void TMass::PostLoop()
{
    fJetVars.SetZero();
    for (auto part : fHardPartons) {
        fJetEvent->AddJet(part.px(),
                          part.py(),
                          part.pz(),
                          part.e(),
                          fJetVars,
                          fWeight,
                          100+fPDGCode[part.user_index()],
                          fStatus);
    }

    for (auto part : fCorrPartons) {
        fJetEvent->AddJet(part.px(),
                          part.py(),
                          part.pz(),
                          part.e(),
                          fJetVars,
                          fWeight,
                          200+fPDGCode[part.user_index()],
                          fStatus);
    }
}
