#include <sstream>

void extractWeight() {

  //Construct input filename
  stringstream instream;
  instream << "./CMSJES_H7_dijet_5000000.root";


  TFile* f = new TFile(instream.str().c_str());

  TProfile *pr  = (TProfile*)f->Get("prWeight");
  TProfile *prtagr = (TProfile*)f->Get("prWeight_tagr");

  cout << "Probe gen: " << endl;

  for (int i=1; i!=pr->GetNbinsX()+1; i++) {
    cout << pr->GetBinContent(i) << ", " << endl;
  }

  cout << "Tag reco: " << endl;
  for (int i=1; i!=pr->GetNbinsX()+1; i++) {
    cout << prtagr->GetBinContent(i) << ", " << endl;
  }

}

