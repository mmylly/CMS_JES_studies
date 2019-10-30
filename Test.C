/* Herwig stuff */
#include "ThePEG/Handlers/AnalysisHandler.h"
#include "ThePEG/EventRecord/Event.h"
#include "ThePEG/EventRecord/Particle.h"
#include "ThePEG/EventRecord/StandardSelectors.h"
#include "ThePEG/Interface/ClassDocumentation.h"
#include "ThePEG/Repository/CurrentGenerator.h"
#include "ThePEG/Repository/EventGenerator.h"
#include "ThePEG/Repository/UseRandom.h"
#include "ThePEG/Utilities/DescribeClass.h"
#include "ThePEG/PDT/StandardMatchers.h"
#include "ThePEG/PDT/EnumParticles.h"
#include "ThePEG/Config/Unitsystem.h"
#include "ThePEG/Persistency/PersistentOStream.h"
#include "ThePEG/Persistency/PersistentIStream.h"

/* ROOT stuff */
#include "TROOT.h"
#include "TChain.h"
#include "TTree.h"
#include "TMatrix.h"
#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"
#include "TMath.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TVector2.h"
#include "TBranch.h"

//#include "generic/help_functions.h"
#include "events/PrtclEvent.h"

/* stdlib */
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

/* Fastjet */
#include "fastjet/config.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/GridMedianBackgroundEstimator.hh"
#include "fastjet/D0RunIIConePlugin.hh"
#include "fastjet/internal/numconsts.hh"

using std::string;
using std::vector;
using std::pair;
using std::map;
using std::cerr;
using std::cout;
using std::endl;
using std::runtime_error;

int main(void) {
    cout << "Jaa" << endl;
    auto jdef1 = fastjet::JetDefinition(fastjet::antikt_algorithm,0.5);
    auto plugin = fastjet::D0RunIIConePlugin(0.5,6,0.5);
    auto jdef2 = fastjet::JetDefinition(&plugin);
    cout << "Juu" << endl;
}
