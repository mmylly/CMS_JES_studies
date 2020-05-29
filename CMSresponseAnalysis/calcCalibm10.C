void calcCalibm10()
{


  TFile* fzj           = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_5000000_Calibm10.root");
  TFile* fzj_Trkm1     = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_5000000_Calibm10_varTrkEffm1.root");
  TFile* fzj_Trkm3     = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_5000000_Calibm10_varTrkEffm3.root");

  string outname = "./output_ROOT_files/varPlots_5000000_Calibm10.root"; //Output file



  //All jets
  TProfile *pr_Rjet           = 0;
  TProfile *pr_Rjet_Trkm1     = 0;
  TProfile *pr_Rjet_Trkm3     = 0;

  //b jets
  TProfile *pr_Rjetb           = 0;
  TProfile *pr_Rjetb_Trkm1     = 0;
  TProfile *pr_Rjetb_Trkm3     = 0;

  //c jets
  TProfile *pr_Rjetc           = 0;
  TProfile *pr_Rjetc_Trkm1     = 0;
  TProfile *pr_Rjetc_Trkm3     = 0;

  //s jets
  TProfile *pr_Rjets           = 0;
  TProfile *pr_Rjets_Trkm1     = 0;
  TProfile *pr_Rjets_Trkm3     = 0;

  //ud jets
  TProfile *pr_Rjetud           = 0;
  TProfile *pr_Rjetud_Trkm1     = 0;
  TProfile *pr_Rjetud_Trkm3     = 0;

  //g jets
  TProfile *pr_Rjetg           = 0;
  TProfile *pr_Rjetg_Trkm1     = 0;
  TProfile *pr_Rjetg_Trkm3     = 0;

  fzj          ->GetObject("prRjet", pr_Rjet);
  fzj_Trkm1    ->GetObject("prRjet", pr_Rjet_Trkm1);
  fzj_Trkm3    ->GetObject("prRjet", pr_Rjet_Trkm3);


  fzj          ->GetObject("prRjetb", pr_Rjetb);
  fzj_Trkm1    ->GetObject("prRjetb", pr_Rjetb_Trkm1);
  fzj_Trkm3    ->GetObject("prRjetb", pr_Rjetb_Trkm3);

  fzj          ->GetObject("prRjetc", pr_Rjetc);
  fzj_Trkm1    ->GetObject("prRjetc", pr_Rjetc_Trkm1);
  fzj_Trkm3    ->GetObject("prRjetc", pr_Rjetc_Trkm3);

  fzj          ->GetObject("prRjets", pr_Rjets);
  fzj_Trkm1    ->GetObject("prRjets", pr_Rjets_Trkm1);
  fzj_Trkm3    ->GetObject("prRjets", pr_Rjets_Trkm3);

  fzj          ->GetObject("prRjetud", pr_Rjetud);
  fzj_Trkm1    ->GetObject("prRjetud", pr_Rjetud_Trkm1);
  fzj_Trkm3    ->GetObject("prRjetud", pr_Rjetud_Trkm3);

  fzj          ->GetObject("prRjetg", pr_Rjetg);
  fzj_Trkm1    ->GetObject("prRjetg", pr_Rjetg_Trkm1);
  fzj_Trkm3    ->GetObject("prRjetg", pr_Rjetg_Trkm3);

  TH1D* h_Rjet           = pr_Rjet->ProjectionX();
  TH1D* h_Rjet_Trkm1     = pr_Rjet_Trkm1->ProjectionX();
  TH1D* h_Rjet_Trkm3     = pr_Rjet_Trkm3->ProjectionX();

  TH1D* h_Rjetb           = pr_Rjetb->ProjectionX();
  TH1D* h_Rjetb_Trkm1     = pr_Rjetb_Trkm1->ProjectionX();
  TH1D* h_Rjetb_Trkm3     = pr_Rjetb_Trkm3->ProjectionX();

  TH1D* h_Rjetc           = pr_Rjetc->ProjectionX();
  TH1D* h_Rjetc_Trkm1     = pr_Rjetc_Trkm1->ProjectionX();
  TH1D* h_Rjetc_Trkm3     = pr_Rjetc_Trkm3->ProjectionX();

  TH1D* h_Rjets           = pr_Rjets->ProjectionX();
  TH1D* h_Rjets_Trkm1     = pr_Rjets_Trkm1->ProjectionX();
  TH1D* h_Rjets_Trkm3     = pr_Rjets_Trkm3->ProjectionX();

  TH1D* h_Rjetud           = pr_Rjetud->ProjectionX();
  TH1D* h_Rjetud_Trkm1     = pr_Rjetud_Trkm1->ProjectionX();
  TH1D* h_Rjetud_Trkm3     = pr_Rjetud_Trkm3->ProjectionX();

  TH1D* h_Rjetg           = pr_Rjetg->ProjectionX();
  TH1D* h_Rjetg_Trkm1     = pr_Rjetg_Trkm1->ProjectionX();
  TH1D* h_Rjetg_Trkm3     = pr_Rjetg_Trkm3->ProjectionX();


  h_Rjet_Trkm1->Divide(h_Rjet);
  h_Rjet_Trkm1->SetTitle("All jets, Tracking efficiency -1%");
  h_Rjet_Trkm1->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_Trkm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjet_Trkm3->Divide(h_Rjet);
  h_Rjet_Trkm3->SetTitle("All jets, Tracking efficiency -3%");
  h_Rjet_Trkm3->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_Trkm3->GetYaxis()->SetTitleOffset(1.2);


  h_Rjetb_Trkm1->Divide(h_Rjetb);
  h_Rjetb_Trkm1->SetTitle("b-jets, Tracking efficiency -1%");
  h_Rjetb_Trkm1->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_Trkm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetb_Trkm3->Divide(h_Rjetb);
  h_Rjetb_Trkm3->SetTitle("b-jets, Tracking efficiency -3%");
  h_Rjetb_Trkm3->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_Trkm3->GetYaxis()->SetTitleOffset(1.2);



  h_Rjetc_Trkm1->Divide(h_Rjetc);
  h_Rjetc_Trkm1->SetTitle("c-jets, Tracking efficiency -1%");
  h_Rjetc_Trkm1->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_Trkm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetc_Trkm3->Divide(h_Rjetc);
  h_Rjetc_Trkm3->SetTitle("c-jets, Tracking efficiency -3%");
  h_Rjetc_Trkm3->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_Trkm3->GetYaxis()->SetTitleOffset(1.2);


  h_Rjets_Trkm1->Divide(h_Rjets);
  h_Rjets_Trkm1->SetTitle("s-jets, Tracking efficiency -1%");
  h_Rjets_Trkm1->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_Trkm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjets_Trkm3->Divide(h_Rjets);
  h_Rjets_Trkm3->SetTitle("s-jets, Tracking efficiency -3%");
  h_Rjets_Trkm3->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_Trkm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetud_Trkm1->Divide(h_Rjetud);
  h_Rjetud_Trkm1->SetTitle("ud-jets, Tracking efficiency -1%");
  h_Rjetud_Trkm1->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_Trkm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetud_Trkm3->Divide(h_Rjetud);
  h_Rjetud_Trkm3->SetTitle("ud-jets, Tracking efficiency -3%");
  h_Rjetud_Trkm3->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_Trkm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetg_Trkm1->Divide(h_Rjetg);
  h_Rjetg_Trkm1->SetTitle("g-jets, Tracking efficiency -1%");
  h_Rjetg_Trkm1->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_Trkm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetg_Trkm3->Divide(h_Rjetg);
  h_Rjetg_Trkm3->SetTitle("g-jets, Tracking efficiency -3%");
  h_Rjetg_Trkm3->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_Trkm3->GetYaxis()->SetTitleOffset(1.2);


  // Fraction variation
  TProfile *prchf   =0; 
  TProfile *prnhf   =0; 
  TProfile *prgammaf=0; 
  TProfile *pref    =0;

  TProfile *prchf_Trkm1   =0; 
  TProfile *prnhf_Trkm1   =0; 
  TProfile *prgammaf_Trkm1=0; 
  TProfile *pref_Trkm1    =0;

  TProfile *prchf_Trkm3   =0; 
  TProfile *prnhf_Trkm3   =0; 
  TProfile *prgammaf_Trkm3=0; 
  TProfile *pref_Trkm3    =0;

  fzj->GetObject("prchf"    ,prchf   );
  fzj->GetObject("prnhf"    ,prnhf   );
  fzj->GetObject("prgammaf" ,prgammaf);
  fzj->GetObject("pref"     ,pref    );

  fzj_Trkm1->GetObject("prchf"    ,prchf_Trkm1   );
  fzj_Trkm1->GetObject("prnhf"    ,prnhf_Trkm1   );
  fzj_Trkm1->GetObject("prgammaf" ,prgammaf_Trkm1);
  fzj_Trkm1->GetObject("pref"     ,pref_Trkm1    );

  fzj_Trkm3->GetObject("prchf"    ,prchf_Trkm3   );
  fzj_Trkm3->GetObject("prnhf"    ,prnhf_Trkm3   );
  fzj_Trkm3->GetObject("prgammaf" ,prgammaf_Trkm3);
  fzj_Trkm3->GetObject("pref"     ,pref_Trkm3    );


  TH1D* hchf    = prchf   ->ProjectionX();
  TH1D* hnhf    = prnhf   ->ProjectionX();
  TH1D* hgammaf = prgammaf->ProjectionX();
  TH1D* hef     = pref    ->ProjectionX();

  TH1D* hchf_Trkm1    = prchf_Trkm1   ->ProjectionX();
  TH1D* hnhf_Trkm1    = prnhf_Trkm1   ->ProjectionX();
  TH1D* hgammaf_Trkm1 = prgammaf_Trkm1->ProjectionX();
  TH1D* hef_Trkm1     = pref_Trkm1    ->ProjectionX();

  TH1D* hchf_Trkm3    = prchf_Trkm3   ->ProjectionX();
  TH1D* hnhf_Trkm3    = prnhf_Trkm3   ->ProjectionX();
  TH1D* hgammaf_Trkm3 = prgammaf_Trkm3->ProjectionX();
  TH1D* hef_Trkm3     = pref_Trkm3    ->ProjectionX();



  //hchf_Trkm1->Divide(hchf);
  hchf_Trkm1->Add(hchf,-1);
  hchf_Trkm1->SetTitle("Charged hadron fraction, Tracking efficiency -1%");
  hchf_Trkm1->GetYaxis()->SetTitle("chf_{var} - chf");
  //hnhf_Trkm1->Divide(hnhf);
  hnhf_Trkm1->Add(hnhf,-1);
  hnhf_Trkm1->SetTitle("Neutral hadron fraction, Tracking efficiency -1%");
  hnhf_Trkm1->GetYaxis()->SetTitle("nhf_{var} - nhf");
  //hgammaf_Trkm1->Divide(hgammaf);
  hgammaf_Trkm1->Add(hgammaf,-1);
  hgammaf_Trkm1->SetTitle("Photon fraction, Tracking efficiency -1%");
  hgammaf_Trkm1->GetYaxis()->SetTitle("gammaf_{var} - gammaf");
  //hef_Trkm1->Divide(hef);
  hef_Trkm1->Add(hef,-1);
  hef_Trkm1->SetTitle("Electron fraction, Tracking efficiency -1%");
  hef_Trkm1->GetYaxis()->SetTitle("ef_{var} - ef");

  //hchf_Trkm3->Divide(hchf);
  hchf_Trkm3->Add(hchf,-1);
  hchf_Trkm3->SetTitle("Charged hadron fraction, Tracking efficiency -3%");
  hchf_Trkm3->GetYaxis()->SetTitle("chf_{var} - chf");
  //hnhf_Trkm3->Divide(hnhf);
  hnhf_Trkm3->Add(hnhf,-1);
  hnhf_Trkm3->SetTitle("Neutral hadron fraction, Tracking efficiency -3%");
  hnhf_Trkm3->GetYaxis()->SetTitle("nhf_{var} - nhf");
  //hgammaf_Trkm3->Divide(hgammaf);
  hgammaf_Trkm3->Add(hgammaf,-1);
  hgammaf_Trkm3->SetTitle("Photon fraction, Tracking efficiency -3%");
  hgammaf_Trkm3->GetYaxis()->SetTitle("gammaf_{var} - gammaf");
  //hef_Trkm3->Divide(hef);
  hef_Trkm3->Add(hef,-1);
  hef_Trkm3->SetTitle("Electron fraction, Tracking efficiency -3%");
  hef_Trkm3->GetYaxis()->SetTitle("ef_{var} - ef");



  TFile *foutvar = new TFile(outname.c_str(),"RECREATE");


  h_Rjet_Trkm1    ->Write("h_Rjet_Trkm1");
  h_Rjet_Trkm3    ->Write("h_Rjet_Trkm3");


  h_Rjetb_Trkm1    ->Write("h_Rjetb_Trkm1");
  h_Rjetb_Trkm3    ->Write("h_Rjetb_Trkm3");

  h_Rjetc_Trkm1    ->Write("h_Rjetc_Trkm1");
  h_Rjetc_Trkm3    ->Write("h_Rjetc_Trkm3");

  h_Rjets_Trkm1    ->Write("h_Rjets_Trkm1");
  h_Rjets_Trkm3    ->Write("h_Rjets_Trkm3");

  h_Rjetud_Trkm1    ->Write("h_Rjetud_Trkm1");
  h_Rjetud_Trkm3    ->Write("h_Rjetud_Trkm3");

  h_Rjetg_Trkm1    ->Write("h_Rjetg_Trkm1");
  h_Rjetg_Trkm3    ->Write("h_Rjetg_Trkm3");


  hchf_Trkm1       ->Write("hchf_Trkm1");
  hnhf_Trkm1       ->Write("hnhf_Trkm1");
  hgammaf_Trkm1    ->Write("hgammaf_Trkm1");
  hef_Trkm1        ->Write("hef_Trkm1");
  hchf_Trkm3       ->Write("hchf_Trkm3");
  hnhf_Trkm3       ->Write("hnhf_Trkm3");
  hgammaf_Trkm3    ->Write("hgammaf_Trkm3");
  hef_Trkm3        ->Write("hef_Trkm3");


  foutvar->Write();
  foutvar->Close();

}





