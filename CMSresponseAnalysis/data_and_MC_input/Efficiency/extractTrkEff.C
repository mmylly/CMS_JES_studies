#include <sstream>

void extractTrkEff() {

  //Construct input filename
  stringstream instream;
  instream << "./CMSJES_P8_Zjet_100000.root";


  TFile* f = new TFile(instream.str().c_str());

  TProfile *eff  = (TProfile*)f->Get("chhEff");

  for (int i=1; i!=eff->GetNbinsX()+1; i++) {
    cout << eff->GetXaxis()->GetBinCenter(i) << " " << eff->GetBinContent(i) << endl;
  }

}

