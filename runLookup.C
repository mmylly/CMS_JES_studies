// Spurdous
R__LOAD_LIBRARY(Lookup.C);

void runLookup() {

    Lookup suu("pjets_pythia8_dijet_1000.root");
    suu.Loop();
}
