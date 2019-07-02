

void readValues() {
	TFile* filein = new TFile("jecdataGH.root");
	TDirectory* dir = filein->GetDirectory("data");
        TDirectory* dir2 = dir->GetDirectory("eta00-13");
        TGraphErrors* ffile = (TGraphErrors*)dir2->Get("mpfchs1_zmmjet_a30");
	ffile->Print();
}
