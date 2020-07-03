#include <sstream>

void extractWeight() {

  //Construct input filename
  stringstream instream;
  instream << "./output_ROOT_files/CMSJES_P8_dijet_5000000.root";


  TFile* f = new TFile(instream.str().c_str());

  TProfile *pr = (TProfile*)f->Get("prWeight");

  for (int i=1; i!=pr->GetNbinsX()+1; i++) {
    cout <<  " " << pr->GetBinContent(i) << endl;

  }

}

