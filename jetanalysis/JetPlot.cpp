#include "JetPlot.h"


//void JetPlot::PostProcessing()
//{
//    fJetVars.constituents = 1;
//    int fl = -fFlavour-1;
//
//    for (auto part : fJetParts) {
//        if (part.user_index() < 0)
//            continue;
//
//        fJetEvent->AddJet(part.px(),
//                          part.py(),
//                          part.pz(),
//                          part.e(),
//                          fJetVars,
//                          fWeight,
//                          fl);
//    }
//
//    fJetVars.constituents = fJetParts.size();
//}

void JetPlot::InitLoop()
{
    return;
}

void JetPlot::PostLoop()
{
    for (unsigned i = 0; i != fPrtcls_; ++i) {
        int status = fAnalysisStatus[i];
        int flavId = fPDGCode[i];
        fastjet::PseudoJet part(fX[i],fY[i], fZ[i], fT[i]);

        if (status==8) {
            fJetVars.constituents = -1;
            fFlavour += 100;
        } else if (status==7) {
            fJetVars.constituents = -2;
            fFlavour += 200;
        } else if (status==6) {
            fJetVars.constituents = -3;
        } else if (status==1 || status==2) {
            fJetVars.constituents = -4;
        } else {
            continue;
        }

        fJetEvent->AddJet(part.px(),
                          part.py(),
                          part.pz(),
                          part.e(),
                          fJetVars,
                          fWeight,
                          flavId,
                          fStatus);
        fJetVars.constituents = 0;
    }
}
