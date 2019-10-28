
/*
void readValues() {
	TFile* filein = new TFile("jecdataGH.root");
	TDirectory* dir = filein->GetDirectory("data");
        TDirectory* dir2 = dir->GetDirectory("eta00-13");
        TGraphErrors* ffile = (TGraphErrors*)dir2->Get("mpfchs1_zmmjet_a30");
	ffile->Print();
}
*/

/*
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

	cout << endl;

        cout << "2018-03-06 Data pTbal" << endl;
	TFile* f5 = new TFile("zjet_combination_07Aug2017_Summer16_JECV6_Zmm_GH_2018-03-06.root");
        TGraphErrors* pTbaldata = (TGraphErrors*)f5->Get("Data_PtBal_CHS_a30_eta_00_13_L1L2L3");
	pTbaldata->Print();

        cout << endl;

        cout << "2018-03-06 MC pTbal" << endl;
	TFile* f6 = new TFile("zjet_combination_07Aug2017_Summer16_JECV6_Zmm_GH_2018-03-06.root");
        TGraphErrors* pTbalMC = (TGraphErrors*)f6->Get("MC_PtBal_CHS_a30_eta_00_13_L1L2L3");
	pTbalMC->Print();


}



void readValues() {
	TFile* filein = new TFile("pchf.root");
	TProfile* pchf = (TProfile*)filein->Get("pchf");
        //TDirectory* dir2 = dir->GetDirectory("eta00-13");
        //TGraphErrors* ffile = (TGraphErrors*)dir2->Get("mpfchs1_zmmjet_a30");
        for (int i=1; i!=pchf->GetNbinsX()+1; ++i) {
          cout << pchf->GetBinCenter(i)  << " " << pchf->GetBinContent(i) << endl;
        }
}
*/

/*
//2018 MPF notype1
void readValues() {
        cout << "FullCombination_Zmm_2018ABCD" << endl;
	TFile* f1 = new TFile("FullCombination_Zmm_17Sep2018_Autumn18_JECv17.root");
        TDirectory* dir = f1->GetDirectory("Run2018ABCD");
        TGraphErrors* MPF = (TGraphErrors*)dir->Get("MC_MPF-notypeI_CHS_a30_eta_00_13_L1L2Res");
	MPF->Print();
}
*/
//2018 pTbal 
void readValues() {
        cout << "FullCombination_Zmm_2018ABCD" << endl;
	TFile* f1 = new TFile("FullCombination_Zmm_17Sep2018_Autumn18_JECv17.root");
        TDirectory* dir = f1->GetDirectory("Run2018ABCD");
        TGraphErrors* MPF = (TGraphErrors*)dir->Get("MC_PtBal_CHS_a30_eta_00_13_L1L2Res");
	MPF->Print();
}

















