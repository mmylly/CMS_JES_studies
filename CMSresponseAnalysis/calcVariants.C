void calcVariants()
{
  //C-parameter variation
  TFile* fzj        = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_5000000.root");
  TFile* fzj_Cp3    = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_5000000_varCp3.root");
  TFile* fzj_Cm3    = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_5000000_varCm3.root");
  TFile* fzj_Trk    = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_5000000_varTrkEff.root");
  TFile* fzj_Photon = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_5000000_varPhoton.root");

  //All jets
  TProfile *pr_Rjet        = 0;
  TProfile *pr_Rjet_Cp3    = 0;
  TProfile *pr_Rjet_Cm3    = 0;
  TProfile *pr_Rjet_Trk    = 0;
  TProfile *pr_Rjet_Photon = 0;

  //b jets
  TProfile *pr_Rjetb        = 0;
  TProfile *pr_Rjetb_Cp3    = 0;
  TProfile *pr_Rjetb_Cm3    = 0;
  TProfile *pr_Rjetb_Trk    = 0;
  TProfile *pr_Rjetb_Photon = 0;

  //c jets
  TProfile *pr_Rjetc        = 0;
  TProfile *pr_Rjetc_Cp3    = 0;
  TProfile *pr_Rjetc_Cm3    = 0;
  TProfile *pr_Rjetc_Trk    = 0;
  TProfile *pr_Rjetc_Photon = 0;

  //s jets
  TProfile *pr_Rjets        = 0;
  TProfile *pr_Rjets_Cp3    = 0;
  TProfile *pr_Rjets_Cm3    = 0;
  TProfile *pr_Rjets_Trk    = 0;
  TProfile *pr_Rjets_Photon = 0;

  //ud jets
  TProfile *pr_Rjetud        = 0;
  TProfile *pr_Rjetud_Cp3    = 0;
  TProfile *pr_Rjetud_Cm3    = 0;
  TProfile *pr_Rjetud_Trk    = 0;
  TProfile *pr_Rjetud_Photon = 0;

  //g jets
  TProfile *pr_Rjetg        = 0;
  TProfile *pr_Rjetg_Cp3    = 0;
  TProfile *pr_Rjetg_Cm3    = 0;
  TProfile *pr_Rjetg_Trk    = 0;
  TProfile *pr_Rjetg_Photon = 0;

  fzj    ->GetObject("prRjet", pr_Rjet);
  fzj_Cp3->GetObject("prRjet", pr_Rjet_Cp3);
  fzj_Cm3->GetObject("prRjet", pr_Rjet_Cm3);
  fzj_Trk->GetObject("prRjet", pr_Rjet_Trk);
  fzj_Photon->GetObject("prRjet", pr_Rjet_Photon);

  fzj    ->GetObject("prRjetb",    pr_Rjetb);
  fzj_Cp3->GetObject("prRjetb",    pr_Rjetb_Cp3);
  fzj_Cm3->GetObject("prRjetb",    pr_Rjetb_Cm3);
  fzj_Trk->GetObject("prRjetb",    pr_Rjetb_Trk);
  fzj_Photon->GetObject("prRjetb", pr_Rjetb_Photon);

  fzj    ->GetObject("prRjetc",    pr_Rjetc);
  fzj_Cp3->GetObject("prRjetc",    pr_Rjetc_Cp3);
  fzj_Cm3->GetObject("prRjetc",    pr_Rjetc_Cm3);
  fzj_Trk->GetObject("prRjetc",    pr_Rjetc_Trk);
  fzj_Photon->GetObject("prRjetc", pr_Rjetc_Photon);

  fzj    ->GetObject("prRjets",    pr_Rjets);
  fzj_Cp3->GetObject("prRjets",    pr_Rjets_Cp3);
  fzj_Cm3->GetObject("prRjets",    pr_Rjets_Cm3);
  fzj_Trk->GetObject("prRjets",    pr_Rjets_Trk);
  fzj_Photon->GetObject("prRjets", pr_Rjets_Photon);

  fzj    ->GetObject("prRjetud",    pr_Rjetud);
  fzj_Cp3->GetObject("prRjetud",    pr_Rjetud_Cp3);
  fzj_Cm3->GetObject("prRjetud",    pr_Rjetud_Cm3);
  fzj_Trk->GetObject("prRjetud",    pr_Rjetud_Trk);
  fzj_Photon->GetObject("prRjetud", pr_Rjetud_Photon);

  fzj    ->GetObject("prRjetg",    pr_Rjetg);
  fzj_Cp3->GetObject("prRjetg",    pr_Rjetg_Cp3);
  fzj_Cm3->GetObject("prRjetg",    pr_Rjetg_Cm3);
  fzj_Trk->GetObject("prRjetg",    pr_Rjetg_Trk);
  fzj_Photon->GetObject("prRjetg", pr_Rjetg_Photon);

  TH1D* h_Rjet        = pr_Rjet->ProjectionX();
  TH1D* h_Rjet_Cp3    = pr_Rjet_Cp3->ProjectionX();
  TH1D* h_Rjet_Cm3    = pr_Rjet_Cm3->ProjectionX();
  TH1D* h_Rjet_Trk    = pr_Rjet_Trk->ProjectionX();
  TH1D* h_Rjet_Photon = pr_Rjet_Photon->ProjectionX();

  TH1D* h_Rjetb        = pr_Rjetb->ProjectionX();
  TH1D* h_Rjetb_Cp3    = pr_Rjetb_Cp3->ProjectionX();
  TH1D* h_Rjetb_Cm3    = pr_Rjetb_Cm3->ProjectionX();
  TH1D* h_Rjetb_Trk    = pr_Rjetb_Trk->ProjectionX();
  TH1D* h_Rjetb_Photon = pr_Rjetb_Photon->ProjectionX();

  TH1D* h_Rjetc        = pr_Rjetc->ProjectionX();
  TH1D* h_Rjetc_Cp3    = pr_Rjetc_Cp3->ProjectionX();
  TH1D* h_Rjetc_Cm3    = pr_Rjetc_Cm3->ProjectionX();
  TH1D* h_Rjetc_Trk    = pr_Rjetc_Trk->ProjectionX();
  TH1D* h_Rjetc_Photon = pr_Rjetc_Photon->ProjectionX();

  TH1D* h_Rjets        = pr_Rjets->ProjectionX();
  TH1D* h_Rjets_Cp3    = pr_Rjets_Cp3->ProjectionX();
  TH1D* h_Rjets_Cm3    = pr_Rjets_Cm3->ProjectionX();
  TH1D* h_Rjets_Trk    = pr_Rjets_Trk->ProjectionX();
  TH1D* h_Rjets_Photon = pr_Rjets_Photon->ProjectionX();

  TH1D* h_Rjetud        = pr_Rjetud->ProjectionX();
  TH1D* h_Rjetud_Cp3    = pr_Rjetud_Cp3->ProjectionX();
  TH1D* h_Rjetud_Cm3    = pr_Rjetud_Cm3->ProjectionX();
  TH1D* h_Rjetud_Trk    = pr_Rjetud_Trk->ProjectionX();
  TH1D* h_Rjetud_Photon = pr_Rjetud_Photon->ProjectionX();

  TH1D* h_Rjetg        = pr_Rjetg->ProjectionX();
  TH1D* h_Rjetg_Cp3    = pr_Rjetg_Cp3->ProjectionX();
  TH1D* h_Rjetg_Cm3    = pr_Rjetg_Cm3->ProjectionX();
  TH1D* h_Rjetg_Trk    = pr_Rjetg_Trk->ProjectionX();
  TH1D* h_Rjetg_Photon = pr_Rjetg_Photon->ProjectionX();

  //All jets
  h_Rjet_Cp3->Divide(h_Rjet);
  h_Rjet_Cp3->SetTitle("All jets, Hadron response +3%");
  h_Rjet_Cp3->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_Cp3->GetYaxis()->SetTitleOffset(1.2);
  h_Rjet_Cm3->Divide(h_Rjet);
  h_Rjet_Cm3->SetTitle("All jets, Hadron response -3%");
  h_Rjet_Cm3->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_Cm3->GetYaxis()->SetTitleOffset(1.2);
  h_Rjet_Trk->Divide(h_Rjet);
  h_Rjet_Trk->SetTitle("All jets, Tracking efficiency -1%");
  h_Rjet_Trk->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_Trk->GetYaxis()->SetTitleOffset(1.2);
  h_Rjet_Photon->Divide(h_Rjet);
  h_Rjet_Photon->SetTitle("All jets, Photon response -1%");
  h_Rjet_Photon->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_Photon->GetYaxis()->SetTitleOffset(1.2);

  //b jet
  h_Rjetb_Cp3->Divide(h_Rjetb);
  h_Rjetb_Cp3->SetTitle("b-jets, Hadron response +3%");
  h_Rjetb_Cp3->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_Cp3->GetYaxis()->SetTitleOffset(1.2);
  h_Rjetb_Cm3->Divide(h_Rjetb);
  h_Rjetb_Cm3->SetTitle("b-jets, Hadron response -3%");
  h_Rjetb_Cm3->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_Cm3->GetYaxis()->SetTitleOffset(1.2);
  h_Rjetb_Trk->Divide(h_Rjetb);
  h_Rjetb_Trk->SetTitle("b-jets, Tracking efficiency -1%");
  h_Rjetb_Trk->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_Trk->GetYaxis()->SetTitleOffset(1.2);
  h_Rjetb_Photon->Divide(h_Rjetb);
  h_Rjetb_Photon->SetTitle("b-jets, Photon response -1%");
  h_Rjetb_Photon->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_Photon->GetYaxis()->SetTitleOffset(1.2);

  //c jets
  h_Rjetc_Cp3->Divide(h_Rjetc);
  h_Rjetc_Cp3->SetTitle("c-jets, Hadron response +3%");
  h_Rjetc_Cp3->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_Cp3->GetYaxis()->SetTitleOffset(1.2);
  h_Rjetc_Cm3->Divide(h_Rjetc);
  h_Rjetc_Cm3->SetTitle("c-jets, Hadron response -3%");
  h_Rjetc_Cm3->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_Cm3->GetYaxis()->SetTitleOffset(1.2);
  h_Rjetc_Trk->Divide(h_Rjetc);
  h_Rjetc_Trk->SetTitle("c-jets, Tracking efficiency -1%");
  h_Rjetc_Trk->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_Trk->GetYaxis()->SetTitleOffset(1.2);
  h_Rjetc_Photon->Divide(h_Rjetc);
  h_Rjetc_Photon->SetTitle("c-jets, Photon response -1%");
  h_Rjetc_Photon->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_Photon->GetYaxis()->SetTitleOffset(1.2);

  //s jet
  h_Rjets_Cp3->Divide(h_Rjets);
  h_Rjets_Cp3->SetTitle("s-jets, Hadron response +3%");
  h_Rjets_Cp3->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_Cp3->GetYaxis()->SetTitleOffset(1.2);
  h_Rjets_Cm3->Divide(h_Rjets);
  h_Rjets_Cm3->SetTitle("s-jets, Hadron response -3%");
  h_Rjets_Cm3->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_Cm3->GetYaxis()->SetTitleOffset(1.2);
  h_Rjets_Trk->Divide(h_Rjets);
  h_Rjets_Trk->SetTitle("s-jets, Tracking efficiency -1%");
  h_Rjets_Trk->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_Trk->GetYaxis()->SetTitleOffset(1.2);
  h_Rjets_Photon->Divide(h_Rjets);
  h_Rjets_Photon->SetTitle("s-jets, Photon response -1%");
  h_Rjets_Photon->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_Photon->GetYaxis()->SetTitleOffset(1.2);

  //ud jet
  h_Rjetud_Cp3->Divide(h_Rjetud);
  h_Rjetud_Cp3->SetTitle("ud-jets, Hadron response +3%");
  h_Rjetud_Cp3->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_Cp3->GetYaxis()->SetTitleOffset(1.2);
  h_Rjetud_Cm3->Divide(h_Rjetud);
  h_Rjetud_Cm3->SetTitle("ud-jets, Hadron response -3%");
  h_Rjetud_Cm3->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_Cm3->GetYaxis()->SetTitleOffset(1.2);
  h_Rjetud_Trk->Divide(h_Rjetud);
  h_Rjetud_Trk->SetTitle("ud-jets, Tracking efficiency -1%");
  h_Rjetud_Trk->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_Trk->GetYaxis()->SetTitleOffset(1.2);
  h_Rjetud_Photon->Divide(h_Rjetud);
  h_Rjetud_Photon->SetTitle("ud-jets, Photon response -1%");
  h_Rjetud_Photon->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_Photon->GetYaxis()->SetTitleOffset(1.2);

  //g jet
  h_Rjetg_Cp3->Divide(h_Rjetg);
  h_Rjetg_Cp3->SetTitle("g-jets, Hadron response +3%");
  h_Rjetg_Cp3->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_Cp3->GetYaxis()->SetTitleOffset(1.2);
  h_Rjetg_Cm3->Divide(h_Rjetg);
  h_Rjetg_Cm3->SetTitle("g-jets, Hadron response -3%");
  h_Rjetg_Cm3->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_Cm3->GetYaxis()->SetTitleOffset(1.2);
  h_Rjetg_Trk->Divide(h_Rjetg);
  h_Rjetg_Trk->SetTitle("g-jets, Tracking efficiency -1%");
  h_Rjetg_Trk->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_Trk->GetYaxis()->SetTitleOffset(1.2);
  h_Rjetg_Photon->Divide(h_Rjetg);
  h_Rjetg_Photon->SetTitle("g-jets, Photon response -1%");
  h_Rjetg_Photon->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_Photon->GetYaxis()->SetTitleOffset(1.2);

  string outname = "./output_ROOT_files/varPlots_5000000"; //Output file
  outname += ".root";
  TFile *foutvar = new TFile(outname.c_str(),"RECREATE");

  h_Rjet_Cm3->Write("h_Rjet_Cm3");
  h_Rjet_Cp3->Write("h_Rjet_Cp3");
  h_Rjet_Trk->Write("h_Rjet_Trk");
  h_Rjet_Photon->Write("h_Rjet_Photon");
  h_Rjetb_Cm3->Write("h_Rjetb_Cm3");
  h_Rjetb_Cp3->Write("h_Rjetb_Cp3");
  h_Rjetb_Trk->Write("h_Rjetb_Trk");
  h_Rjetb_Photon->Write("h_Rjetb_Photon");
  h_Rjetc_Cm3->Write("h_Rjetc_Cm3");
  h_Rjetc_Cp3->Write("h_Rjetc_Cp3");
  h_Rjetc_Trk->Write("h_Rjetc_Trk");
  h_Rjetc_Photon->Write("h_Rjetc_Photon");
  h_Rjets_Cm3->Write("h_Rjets_Cm3");
  h_Rjets_Cp3->Write("h_Rjets_Cp3");
  h_Rjets_Trk->Write("h_Rjets_Trk");
  h_Rjets_Photon->Write("h_Rjets_Photon");
  h_Rjetud_Cm3->Write("h_Rjetud_Cm3");
  h_Rjetud_Cp3->Write("h_Rjetud_Cp3");
  h_Rjetud_Trk->Write("h_Rjetud_Trk");
  h_Rjetud_Photon->Write("h_Rjetud_Photon");
  h_Rjetg_Cm3->Write("h_Rjetg_Cm3");
  h_Rjetg_Cp3->Write("h_Rjetg_Cp3");
  h_Rjetg_Trk->Write("h_Rjetg_Trk");
  h_Rjetg_Photon->Write("h_Rjetg_Photon");

  foutvar->Write();
  foutvar->Close();

}





