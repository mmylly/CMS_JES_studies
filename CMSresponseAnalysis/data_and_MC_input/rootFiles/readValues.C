
/*
void readValues() {
	TFile* filein = new TFile("jecdataGH.root");
	TDirectory* dir = filein->GetDirectory("data");
        TDirectory* dir2 = dir->GetDirectory("eta00-13");
        TGraphErrors* ffile = (TGraphErrors*)dir2->Get("mpfchs1_zmmjet_a30");
	ffile->Print();
}
*/


void readValues() {

        cout << "2018-03-06 Data MPF" << endl;
	TFile* f1 = new TFile("zjet_combination_07Aug2017_Summer16_JECV6_Zmm_GH_2018-03-06.root");
        TGraphErrors* MPFdata = (TGraphErrors*)f1->Get("Data_MPF_CHS_a30_eta_00_13_L1L2L3");
	MPFdata->Print();

        cout << endl;

        cout << "2018-03-06 MC MPF" << endl;
	TFile* f2 = new TFile("zjet_combination_07Aug2017_Summer16_JECV6_Zmm_GH_2018-03-06.root");
        TGraphErrors* MPFMC = (TGraphErrors*)f2->Get("MC_MPF_CHS_a30_eta_00_13_L1L2L3");
	MPFMC->Print();

	cout << endl;

        cout << "2018-03-06 Data MPF notypeI" << endl;
	TFile* f3 = new TFile("zjet_combination_07Aug2017_Summer16_JECV6_Zmm_GH_2018-03-06.root");
        TGraphErrors* MPFdatanotypeI = (TGraphErrors*)f3->Get("Data_MPF-notypeI_CHS_a30_eta_00_13_L1L2L3");
	MPFdatanotypeI->Print();

        cout << endl;

        cout << "2018-03-06 MC MPF notypeI" << endl;
	TFile* f4 = new TFile("zjet_combination_07Aug2017_Summer16_JECV6_Zmm_GH_2018-03-06.root");
        TGraphErrors* MPFMCnotypeI = (TGraphErrors*)f4->Get("MC_MPF-notypeI_CHS_a30_eta_00_13_L1L2L3");
	MPFMCnotypeI->Print();
}


