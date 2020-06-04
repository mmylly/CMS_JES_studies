void calcVariants()
{
  #include <stdio.h>
  #include <string.h>

  int temp=-1;

  string filename="";


  cout << "(1) Pythia 8 (2) Herwig 7" << endl;
  while (temp!=1 && temp!=2) cin >> temp;
  string gen="";
  if      (temp==1) gen = "P8_";
  else if (temp==2) gen = "H7_";

  filename.append(gen);

  //Sample type
  temp=-1;
  cout << "(1) Zjet (2) dijet" << endl;
  while (temp!=1 && temp!=2) cin >> temp;
  string mode="";
  if      (temp==1) mode = "Zjet_";
  else if (temp==2) mode = "dijet_";
  else {
    cout << "ERROR: Unknown mode" << endl;
    return;
  }

  filename.append(mode);

  //Number of events
  cout << "#Events? (1) 3k (2) 10k (3) 30k (4) 100k (5) 1M (6) 5M" << endl;
  temp=-1;	//Reinit
  while (temp<1 || temp >6) cin >> temp;
  string n_evts = "3000";	//#Events in the file
  if      (temp==2) n_evts = "10000";
  else if (temp==3) n_evts = "30000";
  else if (temp==4) n_evts = "100000";
  else if (temp==5) n_evts = "1000000";
  else if (temp==6) n_evts = "5000000";

  filename.append(n_evts);

  string folder = "output_ROOT_files/CMSJES_";
  filename = folder.append(filename);
  string fn_temp = filename;

  //C-parameter variation
  TFile* fzj           = TFile::Open(filename.append(".root").c_str());
  filename = fn_temp;
  TFile* fzj_Cp3       = TFile::Open(filename.append("_varCp3.root").c_str());
  filename = fn_temp;
  TFile* fzj_Cm3       = TFile::Open(filename.append("_varCm3.root").c_str());
  filename = fn_temp;
  TFile* fzj_HadHCALp3 = TFile::Open(filename.append("_varHadHCALp3.root").c_str());
  filename = fn_temp;
  TFile* fzj_HadHCALm3 = TFile::Open(filename.append("_varHadHCALm3.root").c_str());
  filename = fn_temp;
  TFile* fzj_HadECALp3 = TFile::Open(filename.append("_varHadECALp3.root").c_str());
  filename = fn_temp;
  TFile* fzj_HadECALm3 = TFile::Open(filename.append("_varHadECALm3.root").c_str());
  filename = fn_temp;
  TFile* fzj_Trkm1     = TFile::Open(filename.append("_varTrkEffm1.root").c_str());
  filename = fn_temp;
  TFile* fzj_Trkm3     = TFile::Open(filename.append("_varTrkEffm3.root").c_str());
  filename = fn_temp;
  TFile* fzj_ECALm1    = TFile::Open(filename.append("_varECALm1.root").c_str());
  filename = fn_temp;
  TFile* fzj_ECALm3    = TFile::Open(filename.append("_varECALm3.root").c_str());
  filename = fn_temp;
  TFile* fzj_Photonm1  = TFile::Open(filename.append("_varPhotonm1.root").c_str());
  filename = fn_temp;
  TFile* fzj_Photonm3  = TFile::Open(filename.append("_varPhotonm3.root").c_str());

  string outname = "./output_ROOT_files/varPlots_";
  outname.append(gen).append(mode).append(n_evts).append(".root");

  //All jets
  TProfile *pr_Rjet           = 0;
  TProfile *pr_Rjet_Cp3       = 0;
  TProfile *pr_Rjet_Cm3       = 0;
  TProfile *pr_Rjet_HadHCALp3 = 0;
  TProfile *pr_Rjet_HadHCALm3 = 0;
  TProfile *pr_Rjet_HadECALp3 = 0;
  TProfile *pr_Rjet_HadECALm3 = 0;
  TProfile *pr_Rjet_Trkm1     = 0;
  TProfile *pr_Rjet_Trkm3     = 0;
  TProfile *pr_Rjet_ECALm1    = 0;
  TProfile *pr_Rjet_ECALm3    = 0;
  TProfile *pr_Rjet_Photonm1  = 0;
  TProfile *pr_Rjet_Photonm3  = 0;
  TProfile *pr_Rjet_calo     = 0;
  TProfile *pr_Rjet_calo_Cp3 = 0;
  TProfile *pr_Rjet_calo_Cm3 = 0;

  //b jets
  TProfile *pr_Rjetb           = 0;
  TProfile *pr_Rjetb_Cp3       = 0;
  TProfile *pr_Rjetb_Cm3       = 0;
  TProfile *pr_Rjetb_HadHCALp3 = 0;
  TProfile *pr_Rjetb_HadHCALm3 = 0;
  TProfile *pr_Rjetb_HadECALp3 = 0;
  TProfile *pr_Rjetb_HadECALm3 = 0;
  TProfile *pr_Rjetb_Trkm1     = 0;
  TProfile *pr_Rjetb_Trkm3     = 0;
  TProfile *pr_Rjetb_ECALm1    = 0;
  TProfile *pr_Rjetb_ECALm3    = 0;
  TProfile *pr_Rjetb_Photonm1  = 0;
  TProfile *pr_Rjetb_Photonm3  = 0;

  //c jets
  TProfile *pr_Rjetc           = 0;
  TProfile *pr_Rjetc_Cp3       = 0;
  TProfile *pr_Rjetc_Cm3       = 0;
  TProfile *pr_Rjetc_HadHCALp3 = 0;
  TProfile *pr_Rjetc_HadHCALm3 = 0;
  TProfile *pr_Rjetc_HadECALp3 = 0;
  TProfile *pr_Rjetc_HadECALm3 = 0;
  TProfile *pr_Rjetc_Trkm1     = 0;
  TProfile *pr_Rjetc_Trkm3     = 0;
  TProfile *pr_Rjetc_ECALm1    = 0;
  TProfile *pr_Rjetc_ECALm3    = 0;
  TProfile *pr_Rjetc_Photonm1  = 0;
  TProfile *pr_Rjetc_Photonm3  = 0;

  //s jets
  TProfile *pr_Rjets           = 0;
  TProfile *pr_Rjets_Cp3       = 0;
  TProfile *pr_Rjets_Cm3       = 0;
  TProfile *pr_Rjets_HadHCALp3 = 0;
  TProfile *pr_Rjets_HadHCALm3 = 0;
  TProfile *pr_Rjets_HadECALp3 = 0;
  TProfile *pr_Rjets_HadECALm3 = 0;
  TProfile *pr_Rjets_Trkm1     = 0;
  TProfile *pr_Rjets_Trkm3     = 0;
  TProfile *pr_Rjets_ECALm1    = 0;
  TProfile *pr_Rjets_ECALm3    = 0;
  TProfile *pr_Rjets_Photonm1  = 0;
  TProfile *pr_Rjets_Photonm3  = 0;

  //ud jets
  TProfile *pr_Rjetud           = 0;
  TProfile *pr_Rjetud_Cp3       = 0;
  TProfile *pr_Rjetud_Cm3       = 0;
  TProfile *pr_Rjetud_HadHCALp3 = 0;
  TProfile *pr_Rjetud_HadHCALm3 = 0;
  TProfile *pr_Rjetud_HadECALp3 = 0;
  TProfile *pr_Rjetud_HadECALm3 = 0;
  TProfile *pr_Rjetud_Trkm1     = 0;
  TProfile *pr_Rjetud_Trkm3     = 0;
  TProfile *pr_Rjetud_ECALm1    = 0;
  TProfile *pr_Rjetud_ECALm3    = 0;
  TProfile *pr_Rjetud_Photonm1  = 0;
  TProfile *pr_Rjetud_Photonm3  = 0;

  //g jets
  TProfile *pr_Rjetg           = 0;
  TProfile *pr_Rjetg_Cp3       = 0;
  TProfile *pr_Rjetg_Cm3       = 0;
  TProfile *pr_Rjetg_HadHCALp3 = 0;
  TProfile *pr_Rjetg_HadHCALm3 = 0;
  TProfile *pr_Rjetg_HadECALp3 = 0;
  TProfile *pr_Rjetg_HadECALm3 = 0;
  TProfile *pr_Rjetg_Trkm1     = 0;
  TProfile *pr_Rjetg_Trkm3     = 0;
  TProfile *pr_Rjetg_ECALm1    = 0;
  TProfile *pr_Rjetg_ECALm3    = 0;
  TProfile *pr_Rjetg_Photonm1  = 0;
  TProfile *pr_Rjetg_Photonm3  = 0;


  fzj          ->GetObject("prRjet", pr_Rjet);
  fzj_Cp3      ->GetObject("prRjet", pr_Rjet_Cp3);
  fzj_Cm3      ->GetObject("prRjet", pr_Rjet_Cm3);
  fzj_HadHCALp3->GetObject("prRjet", pr_Rjet_HadHCALp3);
  fzj_HadHCALm3->GetObject("prRjet", pr_Rjet_HadHCALm3);
  fzj_HadECALp3->GetObject("prRjet", pr_Rjet_HadECALp3);
  fzj_HadECALm3->GetObject("prRjet", pr_Rjet_HadECALm3);
  fzj_Trkm1    ->GetObject("prRjet", pr_Rjet_Trkm1);
  fzj_Trkm3    ->GetObject("prRjet", pr_Rjet_Trkm3);
  fzj_ECALm1   ->GetObject("prRjet", pr_Rjet_ECALm1);
  fzj_ECALm3   ->GetObject("prRjet", pr_Rjet_ECALm3);
  fzj_Photonm1 ->GetObject("prRjet", pr_Rjet_Photonm1);
  fzj_Photonm3 ->GetObject("prRjet", pr_Rjet_Photonm3);

  fzj->GetObject("prRjet_calo", pr_Rjet_calo);
  fzj_Cp3->GetObject("prRjet_calo", pr_Rjet_calo_Cp3);
  fzj_Cm3->GetObject("prRjet_calo", pr_Rjet_calo_Cm3);

  fzj          ->GetObject("prRjetb", pr_Rjetb);
  fzj_Cp3      ->GetObject("prRjetb", pr_Rjetb_Cp3);
  fzj_Cm3      ->GetObject("prRjetb", pr_Rjetb_Cm3);
  fzj_HadHCALp3->GetObject("prRjetb", pr_Rjetb_HadHCALp3);
  fzj_HadHCALm3->GetObject("prRjetb", pr_Rjetb_HadHCALm3);
  fzj_HadECALp3->GetObject("prRjetb", pr_Rjetb_HadECALp3);
  fzj_HadECALm3->GetObject("prRjetb", pr_Rjetb_HadECALm3);
  fzj_Trkm1    ->GetObject("prRjetb", pr_Rjetb_Trkm1);
  fzj_Trkm3    ->GetObject("prRjetb", pr_Rjetb_Trkm3);
  fzj_ECALm1   ->GetObject("prRjetb", pr_Rjetb_ECALm1);
  fzj_ECALm3   ->GetObject("prRjetb", pr_Rjetb_ECALm3);
  fzj_Photonm1 ->GetObject("prRjetb", pr_Rjetb_Photonm1);
  fzj_Photonm3 ->GetObject("prRjetb", pr_Rjetb_Photonm3);

  fzj          ->GetObject("prRjetc", pr_Rjetc);
  fzj_Cp3      ->GetObject("prRjetc", pr_Rjetc_Cp3);
  fzj_Cm3      ->GetObject("prRjetc", pr_Rjetc_Cm3);
  fzj_HadHCALp3->GetObject("prRjetc", pr_Rjetc_HadHCALp3);
  fzj_HadHCALm3->GetObject("prRjetc", pr_Rjetc_HadHCALm3);
  fzj_HadECALp3->GetObject("prRjetc", pr_Rjetc_HadECALp3);
  fzj_HadECALm3->GetObject("prRjetc", pr_Rjetc_HadECALm3);
  fzj_Trkm1    ->GetObject("prRjetc", pr_Rjetc_Trkm1);
  fzj_Trkm3    ->GetObject("prRjetc", pr_Rjetc_Trkm3);
  fzj_ECALm1   ->GetObject("prRjetc", pr_Rjetc_ECALm1);
  fzj_ECALm3   ->GetObject("prRjetc", pr_Rjetc_ECALm3);
  fzj_Photonm1 ->GetObject("prRjetc", pr_Rjetc_Photonm1);
  fzj_Photonm3 ->GetObject("prRjetc", pr_Rjetc_Photonm3);

  fzj          ->GetObject("prRjets", pr_Rjets);
  fzj_Cp3      ->GetObject("prRjets", pr_Rjets_Cp3);
  fzj_Cm3      ->GetObject("prRjets", pr_Rjets_Cm3);
  fzj_HadHCALp3->GetObject("prRjets", pr_Rjets_HadHCALp3);
  fzj_HadHCALm3->GetObject("prRjets", pr_Rjets_HadHCALm3);
  fzj_HadECALp3->GetObject("prRjets", pr_Rjets_HadECALp3);
  fzj_HadECALm3->GetObject("prRjets", pr_Rjets_HadECALm3);
  fzj_Trkm1    ->GetObject("prRjets", pr_Rjets_Trkm1);
  fzj_Trkm3    ->GetObject("prRjets", pr_Rjets_Trkm3);
  fzj_ECALm1   ->GetObject("prRjets", pr_Rjets_ECALm1);
  fzj_ECALm3   ->GetObject("prRjets", pr_Rjets_ECALm3);
  fzj_Photonm1 ->GetObject("prRjets", pr_Rjets_Photonm1);
  fzj_Photonm3 ->GetObject("prRjets", pr_Rjets_Photonm3);

  fzj          ->GetObject("prRjetud", pr_Rjetud);
  fzj_Cp3      ->GetObject("prRjetud", pr_Rjetud_Cp3);
  fzj_Cm3      ->GetObject("prRjetud", pr_Rjetud_Cm3);
  fzj_HadHCALp3->GetObject("prRjetud", pr_Rjetud_HadHCALp3);
  fzj_HadHCALm3->GetObject("prRjetud", pr_Rjetud_HadHCALm3);
  fzj_HadECALp3->GetObject("prRjetud", pr_Rjetud_HadECALp3);
  fzj_HadECALm3->GetObject("prRjetud", pr_Rjetud_HadECALm3);
  fzj_Trkm1    ->GetObject("prRjetud", pr_Rjetud_Trkm1);
  fzj_Trkm3    ->GetObject("prRjetud", pr_Rjetud_Trkm3);
  fzj_ECALm1   ->GetObject("prRjetud", pr_Rjetud_ECALm1);
  fzj_ECALm3   ->GetObject("prRjetud", pr_Rjetud_ECALm3);
  fzj_Photonm1 ->GetObject("prRjetud", pr_Rjetud_Photonm1);
  fzj_Photonm3 ->GetObject("prRjetud", pr_Rjetud_Photonm3);

  fzj          ->GetObject("prRjetg", pr_Rjetg);
  fzj_Cp3      ->GetObject("prRjetg", pr_Rjetg_Cp3);
  fzj_Cm3      ->GetObject("prRjetg", pr_Rjetg_Cm3);
  fzj_HadHCALp3->GetObject("prRjetg", pr_Rjetg_HadHCALp3);
  fzj_HadHCALm3->GetObject("prRjetg", pr_Rjetg_HadHCALm3);
  fzj_HadECALp3->GetObject("prRjetg", pr_Rjetg_HadECALp3);
  fzj_HadECALm3->GetObject("prRjetg", pr_Rjetg_HadECALm3);
  fzj_Trkm1    ->GetObject("prRjetg", pr_Rjetg_Trkm1);
  fzj_Trkm3    ->GetObject("prRjetg", pr_Rjetg_Trkm3);
  fzj_ECALm1   ->GetObject("prRjetg", pr_Rjetg_ECALm1);
  fzj_ECALm3   ->GetObject("prRjetg", pr_Rjetg_ECALm3);
  fzj_Photonm1 ->GetObject("prRjetg", pr_Rjetg_Photonm1);
  fzj_Photonm3 ->GetObject("prRjetg", pr_Rjetg_Photonm3);

  TH1D* h_Rjet           = pr_Rjet->ProjectionX();
  TH1D* h_Rjet_Cp3       = pr_Rjet_Cp3->ProjectionX();
  TH1D* h_Rjet_Cm3       = pr_Rjet_Cm3->ProjectionX();
  TH1D* h_Rjet_HadHCALp3 = pr_Rjet_HadHCALp3->ProjectionX();
  TH1D* h_Rjet_HadHCALm3 = pr_Rjet_HadHCALm3->ProjectionX();
  TH1D* h_Rjet_HadECALp3 = pr_Rjet_HadECALp3->ProjectionX();
  TH1D* h_Rjet_HadECALm3 = pr_Rjet_HadECALm3->ProjectionX();
  TH1D* h_Rjet_Trkm1     = pr_Rjet_Trkm1->ProjectionX();
  TH1D* h_Rjet_Trkm3     = pr_Rjet_Trkm3->ProjectionX();
  TH1D* h_Rjet_ECALm1    = pr_Rjet_ECALm1->ProjectionX();
  TH1D* h_Rjet_ECALm3    = pr_Rjet_ECALm3->ProjectionX();
  TH1D* h_Rjet_Photonm1  = pr_Rjet_Photonm1->ProjectionX();
  TH1D* h_Rjet_Photonm3  = pr_Rjet_Photonm3->ProjectionX();
  TH1D* h_Rjet_calo      = pr_Rjet_calo->ProjectionX();
  TH1D* h_Rjet_calo_Cp3  = pr_Rjet_calo_Cp3->ProjectionX();
  TH1D* h_Rjet_calo_Cm3  = pr_Rjet_calo_Cm3->ProjectionX();



  TH1D* h_Rjetb           = pr_Rjetb->ProjectionX();
  TH1D* h_Rjetb_Cp3       = pr_Rjetb_Cp3->ProjectionX();
  TH1D* h_Rjetb_Cm3       = pr_Rjetb_Cm3->ProjectionX();
  TH1D* h_Rjetb_HadHCALp3 = pr_Rjetb_HadHCALp3->ProjectionX();
  TH1D* h_Rjetb_HadHCALm3 = pr_Rjetb_HadHCALm3->ProjectionX();
  TH1D* h_Rjetb_HadECALp3 = pr_Rjetb_HadECALp3->ProjectionX();
  TH1D* h_Rjetb_HadECALm3 = pr_Rjetb_HadECALm3->ProjectionX();
  TH1D* h_Rjetb_Trkm1     = pr_Rjetb_Trkm1->ProjectionX();
  TH1D* h_Rjetb_Trkm3     = pr_Rjetb_Trkm3->ProjectionX();
  TH1D* h_Rjetb_ECALm1    = pr_Rjetb_ECALm1->ProjectionX();
  TH1D* h_Rjetb_ECALm3    = pr_Rjetb_ECALm3->ProjectionX();
  TH1D* h_Rjetb_Photonm1  = pr_Rjetb_Photonm1->ProjectionX();
  TH1D* h_Rjetb_Photonm3  = pr_Rjetb_Photonm3->ProjectionX();

  TH1D* h_Rjetc           = pr_Rjetc->ProjectionX();
  TH1D* h_Rjetc_Cp3       = pr_Rjetc_Cp3->ProjectionX();
  TH1D* h_Rjetc_Cm3       = pr_Rjetc_Cm3->ProjectionX();
  TH1D* h_Rjetc_HadHCALp3 = pr_Rjetc_HadHCALp3->ProjectionX();
  TH1D* h_Rjetc_HadHCALm3 = pr_Rjetc_HadHCALm3->ProjectionX();
  TH1D* h_Rjetc_HadECALp3 = pr_Rjetc_HadECALp3->ProjectionX();
  TH1D* h_Rjetc_HadECALm3 = pr_Rjetc_HadECALm3->ProjectionX();
  TH1D* h_Rjetc_Trkm1     = pr_Rjetc_Trkm1->ProjectionX();
  TH1D* h_Rjetc_Trkm3     = pr_Rjetc_Trkm3->ProjectionX();
  TH1D* h_Rjetc_ECALm1    = pr_Rjetc_ECALm1->ProjectionX();
  TH1D* h_Rjetc_ECALm3    = pr_Rjetc_ECALm3->ProjectionX();
  TH1D* h_Rjetc_Photonm1  = pr_Rjetc_Photonm1->ProjectionX();
  TH1D* h_Rjetc_Photonm3  = pr_Rjetc_Photonm3->ProjectionX();

  TH1D* h_Rjets           = pr_Rjets->ProjectionX();
  TH1D* h_Rjets_Cp3       = pr_Rjets_Cp3->ProjectionX();
  TH1D* h_Rjets_Cm3       = pr_Rjets_Cm3->ProjectionX();
  TH1D* h_Rjets_HadHCALp3 = pr_Rjets_HadHCALp3->ProjectionX();
  TH1D* h_Rjets_HadHCALm3 = pr_Rjets_HadHCALm3->ProjectionX();
  TH1D* h_Rjets_HadECALp3 = pr_Rjets_HadECALp3->ProjectionX();
  TH1D* h_Rjets_HadECALm3 = pr_Rjets_HadECALm3->ProjectionX();
  TH1D* h_Rjets_Trkm1     = pr_Rjets_Trkm1->ProjectionX();
  TH1D* h_Rjets_Trkm3     = pr_Rjets_Trkm3->ProjectionX();
  TH1D* h_Rjets_ECALm1    = pr_Rjets_ECALm1->ProjectionX();
  TH1D* h_Rjets_ECALm3    = pr_Rjets_ECALm3->ProjectionX();
  TH1D* h_Rjets_Photonm1  = pr_Rjets_Photonm1->ProjectionX();
  TH1D* h_Rjets_Photonm3  = pr_Rjets_Photonm3->ProjectionX();

  TH1D* h_Rjetud           = pr_Rjetud->ProjectionX();
  TH1D* h_Rjetud_Cp3       = pr_Rjetud_Cp3->ProjectionX();
  TH1D* h_Rjetud_Cm3       = pr_Rjetud_Cm3->ProjectionX();
  TH1D* h_Rjetud_HadHCALp3 = pr_Rjetud_HadHCALp3->ProjectionX();
  TH1D* h_Rjetud_HadHCALm3 = pr_Rjetud_HadHCALm3->ProjectionX();
  TH1D* h_Rjetud_HadECALp3 = pr_Rjetud_HadECALp3->ProjectionX();
  TH1D* h_Rjetud_HadECALm3 = pr_Rjetud_HadECALm3->ProjectionX();
  TH1D* h_Rjetud_Trkm1     = pr_Rjetud_Trkm1->ProjectionX();
  TH1D* h_Rjetud_Trkm3     = pr_Rjetud_Trkm3->ProjectionX();
  TH1D* h_Rjetud_ECALm1    = pr_Rjetud_ECALm1->ProjectionX();
  TH1D* h_Rjetud_ECALm3    = pr_Rjetud_ECALm3->ProjectionX();
  TH1D* h_Rjetud_Photonm1  = pr_Rjetud_Photonm1->ProjectionX();
  TH1D* h_Rjetud_Photonm3  = pr_Rjetud_Photonm3->ProjectionX();

  TH1D* h_Rjetg           = pr_Rjetg->ProjectionX();
  TH1D* h_Rjetg_Cp3       = pr_Rjetg_Cp3->ProjectionX();
  TH1D* h_Rjetg_Cm3       = pr_Rjetg_Cm3->ProjectionX();
  TH1D* h_Rjetg_HadHCALp3 = pr_Rjetg_HadHCALp3->ProjectionX();
  TH1D* h_Rjetg_HadHCALm3 = pr_Rjetg_HadHCALm3->ProjectionX();
  TH1D* h_Rjetg_HadECALp3 = pr_Rjetg_HadECALp3->ProjectionX();
  TH1D* h_Rjetg_HadECALm3 = pr_Rjetg_HadECALm3->ProjectionX();
  TH1D* h_Rjetg_Trkm1     = pr_Rjetg_Trkm1->ProjectionX();
  TH1D* h_Rjetg_Trkm3     = pr_Rjetg_Trkm3->ProjectionX();
  TH1D* h_Rjetg_ECALm1    = pr_Rjetg_ECALm1->ProjectionX();
  TH1D* h_Rjetg_ECALm3    = pr_Rjetg_ECALm3->ProjectionX();
  TH1D* h_Rjetg_Photonm1  = pr_Rjetg_Photonm1->ProjectionX();
  TH1D* h_Rjetg_Photonm3  = pr_Rjetg_Photonm3->ProjectionX();

  //All jets
  h_Rjet_Cp3->Divide(h_Rjet);
  h_Rjet_Cp3->SetTitle("All jets, Hadron response +3%");
  h_Rjet_Cp3->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_Cp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjet_Cm3->Divide(h_Rjet);
  h_Rjet_Cm3->SetTitle("All jets, Hadron response -3%");
  h_Rjet_Cm3->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_Cm3->GetYaxis()->SetTitleOffset(1.2);
  
  h_Rjet_HadHCALp3->Divide(h_Rjet);
  h_Rjet_HadHCALp3->SetTitle("All jets, Hadron HCAL +3%");
  h_Rjet_HadHCALp3->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_HadHCALp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjet_HadHCALm3->Divide(h_Rjet);
  h_Rjet_HadHCALm3->SetTitle("All jets, Hadron HCAL -3%");
  h_Rjet_HadHCALm3->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_HadHCALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjet_HadECALp3->Divide(h_Rjet);
  h_Rjet_HadECALp3->SetTitle("All jets, Hadron ECAL +3%");
  h_Rjet_HadECALp3->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_HadECALp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjet_HadECALm3->Divide(h_Rjet);
  h_Rjet_HadECALm3->SetTitle("All jets, Hadron ECAL -3%");
  h_Rjet_HadECALm3->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_HadECALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjet_Trkm1->Divide(h_Rjet);
  h_Rjet_Trkm1->SetTitle("All jets, Tracking efficiency -1%");
  h_Rjet_Trkm1->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_Trkm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjet_Trkm3->Divide(h_Rjet);
  h_Rjet_Trkm3->SetTitle("All jets, Tracking efficiency -3%");
  h_Rjet_Trkm3->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_Trkm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjet_ECALm1->Divide(h_Rjet);
  h_Rjet_ECALm1->SetTitle("All jets, ECAL -1%");
  h_Rjet_ECALm1->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_ECALm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjet_ECALm3->Divide(h_Rjet);
  h_Rjet_ECALm3->SetTitle("All jets, ECAL -3%");
  h_Rjet_ECALm3->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_ECALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjet_Photonm1->Divide(h_Rjet);
  h_Rjet_Photonm1->SetTitle("All jets, Photon response -1%");
  h_Rjet_Photonm1->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_Photonm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjet_Photonm3->Divide(h_Rjet);
  h_Rjet_Photonm3->SetTitle("All jets, Photon response -3%");
  h_Rjet_Photonm3->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_Photonm3->GetYaxis()->SetTitleOffset(1.2);


  h_Rjet_calo_Cp3->Divide(h_Rjet_calo);
  h_Rjet_calo_Cp3->SetTitle("Calo jets, C+3%");
  h_Rjet_calo_Cp3->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_calo_Cp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjet_calo_Cm3->Divide(h_Rjet_calo);
  h_Rjet_calo_Cm3->SetTitle("Calo jets, C-3%");
  h_Rjet_calo_Cm3->GetYaxis()->SetTitle("R_{jet}^{var} / R_{jet}");
  h_Rjet_calo_Cm3->GetYaxis()->SetTitleOffset(1.2);

  //b jet

  h_Rjetb_Cp3->Divide(h_Rjetb);
  h_Rjetb_Cp3->SetTitle("b-jets, Hadron response +3%");
  h_Rjetb_Cp3->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_Cp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetb_Cm3->Divide(h_Rjetb);
  h_Rjetb_Cm3->SetTitle("b-jets, Hadron response -3%");
  h_Rjetb_Cm3->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_Cm3->GetYaxis()->SetTitleOffset(1.2);
  
  h_Rjetb_HadHCALp3->Divide(h_Rjetb);
  h_Rjetb_HadHCALp3->SetTitle("b-jets, Hadron HCAL +3%");
  h_Rjetb_HadHCALp3->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_HadHCALp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetb_HadHCALm3->Divide(h_Rjetb);
  h_Rjetb_HadHCALm3->SetTitle("b-jets, Hadron HCAL -3%");
  h_Rjetb_HadHCALm3->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_HadHCALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetb_HadECALp3->Divide(h_Rjetb);
  h_Rjetb_HadECALp3->SetTitle("b-jets, Hadron ECAL +3%");
  h_Rjetb_HadECALp3->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_HadECALp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetb_HadECALm3->Divide(h_Rjetb);
  h_Rjetb_HadECALm3->SetTitle("b-jets, Hadron ECAL -3%");
  h_Rjetb_HadECALm3->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_HadECALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetb_Trkm1->Divide(h_Rjetb);
  h_Rjetb_Trkm1->SetTitle("b-jets, Tracking efficiency -1%");
  h_Rjetb_Trkm1->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_Trkm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetb_Trkm3->Divide(h_Rjetb);
  h_Rjetb_Trkm3->SetTitle("b-jets, Tracking efficiency -3%");
  h_Rjetb_Trkm3->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_Trkm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetb_ECALm1->Divide(h_Rjetb);
  h_Rjetb_ECALm1->SetTitle("b-jets, ECAL -1%");
  h_Rjetb_ECALm1->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_ECALm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetb_ECALm3->Divide(h_Rjetb);
  h_Rjetb_ECALm3->SetTitle("b-jets, ECAL -3%");
  h_Rjetb_ECALm3->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_ECALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetb_Photonm1->Divide(h_Rjetb);
  h_Rjetb_Photonm1->SetTitle("b-jets, Photon response -1%");
  h_Rjetb_Photonm1->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_Photonm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetb_Photonm3->Divide(h_Rjetb);
  h_Rjetb_Photonm3->SetTitle("b-jets, Photon response -3%");
  h_Rjetb_Photonm3->GetYaxis()->SetTitle("R_{b-jet}^{var} / R_{b-jet}");
  h_Rjetb_Photonm3->GetYaxis()->SetTitleOffset(1.2);

  //c jets
  h_Rjetc_Cp3->Divide(h_Rjetc);
  h_Rjetc_Cp3->SetTitle("c-jets, Hadron response +3%");
  h_Rjetc_Cp3->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_Cp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetc_Cm3->Divide(h_Rjetc);
  h_Rjetc_Cm3->SetTitle("c-jets, Hadron response -3%");
  h_Rjetc_Cm3->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_Cm3->GetYaxis()->SetTitleOffset(1.2);
  
  h_Rjetc_HadHCALp3->Divide(h_Rjetc);
  h_Rjetc_HadHCALp3->SetTitle("c-jets, Hadron HCAL +3%");
  h_Rjetc_HadHCALp3->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_HadHCALp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetc_HadHCALm3->Divide(h_Rjetc);
  h_Rjetc_HadHCALm3->SetTitle("c-jets, Hadron HCAL -3%");
  h_Rjetc_HadHCALm3->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_HadHCALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetc_HadECALp3->Divide(h_Rjetc);
  h_Rjetc_HadECALp3->SetTitle("c-jets, Hadron ECAL +3%");
  h_Rjetc_HadECALp3->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_HadECALp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetc_HadECALm3->Divide(h_Rjetc);
  h_Rjetc_HadECALm3->SetTitle("c-jets, Hadron ECAL -3%");
  h_Rjetc_HadECALm3->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_HadECALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetc_Trkm1->Divide(h_Rjetc);
  h_Rjetc_Trkm1->SetTitle("c-jets, Tracking efficiency -1%");
  h_Rjetc_Trkm1->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_Trkm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetc_Trkm3->Divide(h_Rjetc);
  h_Rjetc_Trkm3->SetTitle("c-jets, Tracking efficiency -3%");
  h_Rjetc_Trkm3->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_Trkm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetc_ECALm1->Divide(h_Rjetc);
  h_Rjetc_ECALm1->SetTitle("c-jets, ECAL -1%");
  h_Rjetc_ECALm1->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_ECALm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetc_ECALm3->Divide(h_Rjetc);
  h_Rjetc_ECALm3->SetTitle("c-jets, ECAL -3%");
  h_Rjetc_ECALm3->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_ECALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetc_Photonm1->Divide(h_Rjetc);
  h_Rjetc_Photonm1->SetTitle("c-jets, Photon response -1%");
  h_Rjetc_Photonm1->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_Photonm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetc_Photonm3->Divide(h_Rjetc);
  h_Rjetc_Photonm3->SetTitle("c-jets, Photon response -3%");
  h_Rjetc_Photonm3->GetYaxis()->SetTitle("R_{c-jet}^{var} / R_{c-jet}");
  h_Rjetc_Photonm3->GetYaxis()->SetTitleOffset(1.2);

  //s jet
  h_Rjets_Cp3->Divide(h_Rjets);
  h_Rjets_Cp3->SetTitle("s-jets, Hadron response +3%");
  h_Rjets_Cp3->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_Cp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjets_Cm3->Divide(h_Rjets);
  h_Rjets_Cm3->SetTitle("s-jets, Hadron response -3%");
  h_Rjets_Cm3->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_Cm3->GetYaxis()->SetTitleOffset(1.2);
  
  h_Rjets_HadHCALp3->Divide(h_Rjets);
  h_Rjets_HadHCALp3->SetTitle("s-jets, Hadron HCAL +3%");
  h_Rjets_HadHCALp3->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_HadHCALp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjets_HadHCALm3->Divide(h_Rjets);
  h_Rjets_HadHCALm3->SetTitle("s-jets, Hadron HCAL -3%");
  h_Rjets_HadHCALm3->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_HadHCALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjets_HadECALp3->Divide(h_Rjets);
  h_Rjets_HadECALp3->SetTitle("s-jets, Hadron ECAL +3%");
  h_Rjets_HadECALp3->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_HadECALp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjets_HadECALm3->Divide(h_Rjets);
  h_Rjets_HadECALm3->SetTitle("s-jets, Hadron ECAL -3%");
  h_Rjets_HadECALm3->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_HadECALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjets_Trkm1->Divide(h_Rjets);
  h_Rjets_Trkm1->SetTitle("s-jets, Tracking efficiency -1%");
  h_Rjets_Trkm1->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_Trkm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjets_Trkm3->Divide(h_Rjets);
  h_Rjets_Trkm3->SetTitle("s-jets, Tracking efficiency -3%");
  h_Rjets_Trkm3->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_Trkm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjets_ECALm1->Divide(h_Rjets);
  h_Rjets_ECALm1->SetTitle("s-jets, ECAL -1%");
  h_Rjets_ECALm1->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_ECALm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjets_ECALm3->Divide(h_Rjets);
  h_Rjets_ECALm3->SetTitle("s-jets, ECAL -3%");
  h_Rjets_ECALm3->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_ECALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjets_Photonm1->Divide(h_Rjets);
  h_Rjets_Photonm1->SetTitle("s-jets, Photon response -1%");
  h_Rjets_Photonm1->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_Photonm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjets_Photonm3->Divide(h_Rjets);
  h_Rjets_Photonm3->SetTitle("s-jets, Photon response -3%");
  h_Rjets_Photonm3->GetYaxis()->SetTitle("R_{s-jet}^{var} / R_{s-jet}");
  h_Rjets_Photonm3->GetYaxis()->SetTitleOffset(1.2);
 
  //ud jet
  h_Rjetud_Cp3->Divide(h_Rjetud);
  h_Rjetud_Cp3->SetTitle("ud-jets, Hadron response +3%");
  h_Rjetud_Cp3->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_Cp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetud_Cm3->Divide(h_Rjetud);
  h_Rjetud_Cm3->SetTitle("ud-jets, Hadron response -3%");
  h_Rjetud_Cm3->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_Cm3->GetYaxis()->SetTitleOffset(1.2);
  
  h_Rjetud_HadHCALp3->Divide(h_Rjetud);
  h_Rjetud_HadHCALp3->SetTitle("ud-jets, Hadron HCAL +3%");
  h_Rjetud_HadHCALp3->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_HadHCALp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetud_HadHCALm3->Divide(h_Rjetud);
  h_Rjetud_HadHCALm3->SetTitle("ud-jets, Hadron HCAL -3%");
  h_Rjetud_HadHCALm3->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_HadHCALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetud_HadECALp3->Divide(h_Rjetud);
  h_Rjetud_HadECALp3->SetTitle("ud-jets, Hadron ECAL +3%");
  h_Rjetud_HadECALp3->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_HadECALp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetud_HadECALm3->Divide(h_Rjetud);
  h_Rjetud_HadECALm3->SetTitle("ud-jets, Hadron ECAL -3%");
  h_Rjetud_HadECALm3->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_HadECALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetud_Trkm1->Divide(h_Rjetud);
  h_Rjetud_Trkm1->SetTitle("ud-jets, Tracking efficiency -1%");
  h_Rjetud_Trkm1->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_Trkm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetud_Trkm3->Divide(h_Rjetud);
  h_Rjetud_Trkm3->SetTitle("ud-jets, Tracking efficiency -3%");
  h_Rjetud_Trkm3->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_Trkm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetud_ECALm1->Divide(h_Rjetud);
  h_Rjetud_ECALm1->SetTitle("ud-jets, ECAL -1%");
  h_Rjetud_ECALm1->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_ECALm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetud_ECALm3->Divide(h_Rjetud);
  h_Rjetud_ECALm3->SetTitle("ud-jets, ECAL -3%");
  h_Rjetud_ECALm3->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_ECALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetud_Photonm1->Divide(h_Rjetud);
  h_Rjetud_Photonm1->SetTitle("ud-jets, Photon response -1%");
  h_Rjetud_Photonm1->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_Photonm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetud_Photonm3->Divide(h_Rjetud);
  h_Rjetud_Photonm3->SetTitle("ud-jets, Photon response -3%");
  h_Rjetud_Photonm3->GetYaxis()->SetTitle("R_{ud-jet}^{var} / R_{ud-jet}");
  h_Rjetud_Photonm3->GetYaxis()->SetTitleOffset(1.2);

  //g jet
  h_Rjetg_Cp3->Divide(h_Rjetg);
  h_Rjetg_Cp3->SetTitle("g-jets, Hadron response +3%");
  h_Rjetg_Cp3->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_Cp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetg_Cm3->Divide(h_Rjetg);
  h_Rjetg_Cm3->SetTitle("g-jets, Hadron response -3%");
  h_Rjetg_Cm3->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_Cm3->GetYaxis()->SetTitleOffset(1.2);
  
  h_Rjetg_HadHCALp3->Divide(h_Rjetg);
  h_Rjetg_HadHCALp3->SetTitle("g-jets, Hadron HCAL +3%");
  h_Rjetg_HadHCALp3->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_HadHCALp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetg_HadHCALm3->Divide(h_Rjetg);
  h_Rjetg_HadHCALm3->SetTitle("g-jets, Hadron HCAL -3%");
  h_Rjetg_HadHCALm3->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_HadHCALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetg_HadECALp3->Divide(h_Rjetg);
  h_Rjetg_HadECALp3->SetTitle("g-jets, Hadron ECAL +3%");
  h_Rjetg_HadECALp3->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_HadECALp3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetg_HadECALm3->Divide(h_Rjetg);
  h_Rjetg_HadECALm3->SetTitle("g-jets, Hadron ECAL -3%");
  h_Rjetg_HadECALm3->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_HadECALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetg_Trkm1->Divide(h_Rjetg);
  h_Rjetg_Trkm1->SetTitle("g-jets, Tracking efficiency -1%");
  h_Rjetg_Trkm1->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_Trkm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetg_Trkm3->Divide(h_Rjetg);
  h_Rjetg_Trkm3->SetTitle("g-jets, Tracking efficiency -3%");
  h_Rjetg_Trkm3->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_Trkm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetg_ECALm1->Divide(h_Rjetg);
  h_Rjetg_ECALm1->SetTitle("g-jets, ECAL -1%");
  h_Rjetg_ECALm1->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_ECALm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetg_ECALm3->Divide(h_Rjetg);
  h_Rjetg_ECALm3->SetTitle("g-jets, ECAL -3%");
  h_Rjetg_ECALm3->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_ECALm3->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetg_Photonm1->Divide(h_Rjetg);
  h_Rjetg_Photonm1->SetTitle("g-jets, Photon response -1%");
  h_Rjetg_Photonm1->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_Photonm1->GetYaxis()->SetTitleOffset(1.2);

  h_Rjetg_Photonm3->Divide(h_Rjetg);
  h_Rjetg_Photonm3->SetTitle("g-jets, Photon response -3%");
  h_Rjetg_Photonm3->GetYaxis()->SetTitle("R_{g-jet}^{var} / R_{g-jet}");
  h_Rjetg_Photonm3->GetYaxis()->SetTitleOffset(1.2);

  // Fraction variation
  TProfile *prchf   =0; 
  TProfile *prnhf   =0; 
  TProfile *prgammaf=0; 
  TProfile *pref    =0;

  TProfile *prchf_Cp3   =0; 
  TProfile *prnhf_Cp3   =0; 
  TProfile *prgammaf_Cp3=0; 
  TProfile *pref_Cp3    =0;

  TProfile *prchf_Cm3   =0; 
  TProfile *prnhf_Cm3   =0; 
  TProfile *prgammaf_Cm3=0; 
  TProfile *pref_Cm3    =0;

  TProfile *prchf_HadHCALp3   =0; 
  TProfile *prnhf_HadHCALp3   =0; 
  TProfile *prgammaf_HadHCALp3=0; 
  TProfile *pref_HadHCALp3    =0;

  TProfile *prchf_HadHCALm3   =0; 
  TProfile *prnhf_HadHCALm3   =0; 
  TProfile *prgammaf_HadHCALm3=0; 
  TProfile *pref_HadHCALm3    =0;

  TProfile *prchf_HadECALp3   =0; 
  TProfile *prnhf_HadECALp3   =0; 
  TProfile *prgammaf_HadECALp3=0; 
  TProfile *pref_HadECALp3    =0;

  TProfile *prchf_HadECALm3   =0; 
  TProfile *prnhf_HadECALm3   =0; 
  TProfile *prgammaf_HadECALm3=0; 
  TProfile *pref_HadECALm3    =0;

  TProfile *prchf_Trkm1   =0; 
  TProfile *prnhf_Trkm1   =0; 
  TProfile *prgammaf_Trkm1=0; 
  TProfile *pref_Trkm1    =0;

  TProfile *prchf_Trkm3   =0; 
  TProfile *prnhf_Trkm3   =0; 
  TProfile *prgammaf_Trkm3=0; 
  TProfile *pref_Trkm3    =0;

  TProfile *prchf_ECALm1   =0; 
  TProfile *prnhf_ECALm1   =0; 
  TProfile *prgammaf_ECALm1=0; 
  TProfile *pref_ECALm1    =0;

  TProfile *prchf_ECALm3   =0; 
  TProfile *prnhf_ECALm3   =0; 
  TProfile *prgammaf_ECALm3=0; 
  TProfile *pref_ECALm3    =0;

  TProfile *prchf_Photonm1   =0; 
  TProfile *prnhf_Photonm1   =0; 
  TProfile *prgammaf_Photonm1=0; 
  TProfile *pref_Photonm1    =0;

  TProfile *prchf_Photonm3   =0; 
  TProfile *prnhf_Photonm3   =0; 
  TProfile *prgammaf_Photonm3=0; 
  TProfile *pref_Photonm3    =0;


  fzj->GetObject("prchf"    ,prchf   );
  fzj->GetObject("prnhf"    ,prnhf   );
  fzj->GetObject("prgammaf" ,prgammaf);
  fzj->GetObject("pref"     ,pref    );

  fzj_Cp3->GetObject("prchf"    ,prchf_Cp3   );
  fzj_Cp3->GetObject("prnhf"    ,prnhf_Cp3   );
  fzj_Cp3->GetObject("prgammaf" ,prgammaf_Cp3);
  fzj_Cp3->GetObject("pref"     ,pref_Cp3    );

  fzj_Cm3->GetObject("prchf"    ,prchf_Cm3   );
  fzj_Cm3->GetObject("prnhf"    ,prnhf_Cm3   );
  fzj_Cm3->GetObject("prgammaf" ,prgammaf_Cm3);
  fzj_Cm3->GetObject("pref"     ,pref_Cm3    );

  fzj_HadHCALp3->GetObject("prchf"    ,prchf_HadHCALp3   );
  fzj_HadHCALp3->GetObject("prnhf"    ,prnhf_HadHCALp3   );
  fzj_HadHCALp3->GetObject("prgammaf" ,prgammaf_HadHCALp3);
  fzj_HadHCALp3->GetObject("pref"     ,pref_HadHCALp3    );

  fzj_HadHCALm3->GetObject("prchf"    ,prchf_HadHCALm3   );
  fzj_HadHCALm3->GetObject("prnhf"    ,prnhf_HadHCALm3   );
  fzj_HadHCALm3->GetObject("prgammaf" ,prgammaf_HadHCALm3);
  fzj_HadHCALm3->GetObject("pref"     ,pref_HadHCALm3    );

  fzj_HadECALp3->GetObject("prchf"    ,prchf_HadECALp3   );
  fzj_HadECALp3->GetObject("prnhf"    ,prnhf_HadECALp3   );
  fzj_HadECALp3->GetObject("prgammaf" ,prgammaf_HadECALp3);
  fzj_HadECALp3->GetObject("pref"     ,pref_HadECALp3    );

  fzj_HadECALm3->GetObject("prchf"    ,prchf_HadECALm3   );
  fzj_HadECALm3->GetObject("prnhf"    ,prnhf_HadECALm3   );
  fzj_HadECALm3->GetObject("prgammaf" ,prgammaf_HadECALm3);
  fzj_HadECALm3->GetObject("pref"     ,pref_HadECALm3    );

  fzj_Trkm1->GetObject("prchf"    ,prchf_Trkm1   );
  fzj_Trkm1->GetObject("prnhf"    ,prnhf_Trkm1   );
  fzj_Trkm1->GetObject("prgammaf" ,prgammaf_Trkm1);
  fzj_Trkm1->GetObject("pref"     ,pref_Trkm1    );

  fzj_Trkm3->GetObject("prchf"    ,prchf_Trkm3   );
  fzj_Trkm3->GetObject("prnhf"    ,prnhf_Trkm3   );
  fzj_Trkm3->GetObject("prgammaf" ,prgammaf_Trkm3);
  fzj_Trkm3->GetObject("pref"     ,pref_Trkm3    );

  fzj_ECALm1->GetObject("prchf"    ,prchf_ECALm1   );
  fzj_ECALm1->GetObject("prnhf"    ,prnhf_ECALm1   );
  fzj_ECALm1->GetObject("prgammaf" ,prgammaf_ECALm1);
  fzj_ECALm1->GetObject("pref"     ,pref_ECALm1    );

  fzj_ECALm3->GetObject("prchf"    ,prchf_ECALm3   );
  fzj_ECALm3->GetObject("prnhf"    ,prnhf_ECALm3   );
  fzj_ECALm3->GetObject("prgammaf" ,prgammaf_ECALm3);
  fzj_ECALm3->GetObject("pref"     ,pref_ECALm3    );

  fzj_Photonm1->GetObject("prchf"    ,prchf_Photonm1   );
  fzj_Photonm1->GetObject("prnhf"    ,prnhf_Photonm1   );
  fzj_Photonm1->GetObject("prgammaf" ,prgammaf_Photonm1);
  fzj_Photonm1->GetObject("pref"     ,pref_Photonm1    );

  fzj_Photonm3->GetObject("prchf"    ,prchf_Photonm3   );
  fzj_Photonm3->GetObject("prnhf"    ,prnhf_Photonm3   );
  fzj_Photonm3->GetObject("prgammaf" ,prgammaf_Photonm3);
  fzj_Photonm3->GetObject("pref"     ,pref_Photonm3    );


  TH1D* hchf    = prchf   ->ProjectionX();
  TH1D* hnhf    = prnhf   ->ProjectionX();
  TH1D* hgammaf = prgammaf->ProjectionX();
  TH1D* hef     = pref    ->ProjectionX();

  TH1D* hchf_Cp3    = prchf_Cp3   ->ProjectionX();
  TH1D* hnhf_Cp3    = prnhf_Cp3   ->ProjectionX();
  TH1D* hgammaf_Cp3 = prgammaf_Cp3->ProjectionX();
  TH1D* hef_Cp3     = pref_Cp3    ->ProjectionX();

  TH1D* hchf_Cm3    = prchf_Cm3   ->ProjectionX();
  TH1D* hnhf_Cm3    = prnhf_Cm3   ->ProjectionX();
  TH1D* hgammaf_Cm3 = prgammaf_Cm3->ProjectionX();
  TH1D* hef_Cm3     = pref_Cm3    ->ProjectionX();

  TH1D* hchf_HadHCALp3    = prchf_HadHCALp3   ->ProjectionX();
  TH1D* hnhf_HadHCALp3    = prnhf_HadHCALp3   ->ProjectionX();
  TH1D* hgammaf_HadHCALp3 = prgammaf_HadHCALp3->ProjectionX();
  TH1D* hef_HadHCALp3     = pref_HadHCALp3    ->ProjectionX();

  TH1D* hchf_HadHCALm3    = prchf_HadHCALm3   ->ProjectionX();
  TH1D* hnhf_HadHCALm3    = prnhf_HadHCALm3   ->ProjectionX();
  TH1D* hgammaf_HadHCALm3 = prgammaf_HadHCALm3->ProjectionX();
  TH1D* hef_HadHCALm3     = pref_HadHCALm3    ->ProjectionX();

  TH1D* hchf_HadECALp3    = prchf_HadECALp3   ->ProjectionX();
  TH1D* hnhf_HadECALp3    = prnhf_HadECALp3   ->ProjectionX();
  TH1D* hgammaf_HadECALp3 = prgammaf_HadECALp3->ProjectionX();
  TH1D* hef_HadECALp3     = pref_HadECALp3    ->ProjectionX();

  TH1D* hchf_HadECALm3    = prchf_HadECALm3   ->ProjectionX();
  TH1D* hnhf_HadECALm3    = prnhf_HadECALm3   ->ProjectionX();
  TH1D* hgammaf_HadECALm3 = prgammaf_HadECALm3->ProjectionX();
  TH1D* hef_HadECALm3     = pref_HadECALm3    ->ProjectionX();

  TH1D* hchf_Trkm1    = prchf_Trkm1   ->ProjectionX();
  TH1D* hnhf_Trkm1    = prnhf_Trkm1   ->ProjectionX();
  TH1D* hgammaf_Trkm1 = prgammaf_Trkm1->ProjectionX();
  TH1D* hef_Trkm1     = pref_Trkm1    ->ProjectionX();

  TH1D* hchf_Trkm3    = prchf_Trkm3   ->ProjectionX();
  TH1D* hnhf_Trkm3    = prnhf_Trkm3   ->ProjectionX();
  TH1D* hgammaf_Trkm3 = prgammaf_Trkm3->ProjectionX();
  TH1D* hef_Trkm3     = pref_Trkm3    ->ProjectionX();

  TH1D* hchf_ECALm1    = prchf_ECALm1   ->ProjectionX();
  TH1D* hnhf_ECALm1    = prnhf_ECALm1   ->ProjectionX();
  TH1D* hgammaf_ECALm1 = prgammaf_ECALm1->ProjectionX();
  TH1D* hef_ECALm1     = pref_ECALm1    ->ProjectionX();

  TH1D* hchf_ECALm3    = prchf_ECALm3   ->ProjectionX();
  TH1D* hnhf_ECALm3    = prnhf_ECALm3   ->ProjectionX();
  TH1D* hgammaf_ECALm3 = prgammaf_ECALm3->ProjectionX();
  TH1D* hef_ECALm3     = pref_ECALm3    ->ProjectionX();

  TH1D* hchf_Photonm1    = prchf_Photonm1   ->ProjectionX();
  TH1D* hnhf_Photonm1    = prnhf_Photonm1   ->ProjectionX();
  TH1D* hgammaf_Photonm1 = prgammaf_Photonm1->ProjectionX();
  TH1D* hef_Photonm1     = pref_Photonm1    ->ProjectionX();

  TH1D* hchf_Photonm3    = prchf_Photonm3   ->ProjectionX();
  TH1D* hnhf_Photonm3    = prnhf_Photonm3   ->ProjectionX();
  TH1D* hgammaf_Photonm3 = prgammaf_Photonm3->ProjectionX();
  TH1D* hef_Photonm3     = pref_Photonm3    ->ProjectionX();

  //hchf_Cp3->Divide(hchf);
  hchf_Cp3->Add(hchf,-1);
  hchf_Cp3->SetTitle("Charged hadron fraction, Hadron response +3%");
  hchf_Cp3->GetYaxis()->SetTitle("chf_{var} - chf");
  //hnhf_Cp3->Divide(hnhf);
  hnhf_Cp3->Add(hnhf,-1);
  hnhf_Cp3->SetTitle("Neutral hadron fraction, Hadron response +3%");
  hnhf_Cp3->GetYaxis()->SetTitle("nhf_{var} - nhf");
  //hgammaf_Cp3->Divide(hgammaf);
  hgammaf_Cp3->Add(hgammaf,-1);
  hgammaf_Cp3->SetTitle("Photon fraction, Hadron response +3%");
  hgammaf_Cp3->GetYaxis()->SetTitle("gammaf_{var} - gammaf");
  //hef_Cp3->Divide(hef);
  hef_Cp3->Add(hef,-1);
  hef_Cp3->SetTitle("Electron fraction, Hadron response +3%");
  hef_Cp3->GetYaxis()->SetTitle("ef_{var} - ef");

  //hchf_Cm3->Divide(hchf);
  hchf_Cm3->Add(hchf,-1);
  hchf_Cm3->SetTitle("Charged hadron fraction, Hadron response -3%");
  hchf_Cm3->GetYaxis()->SetTitle("chf_{var} - chf");
  //hnhf_Cm3->Divide(hnhf);
  hnhf_Cm3->Add(hnhf,-1);
  hnhf_Cm3->SetTitle("Neutral hadron fraction, Hadron response -3%");
  hnhf_Cm3->GetYaxis()->SetTitle("nhf_{var} - nhf");
  //hgammaf_Cm3->Divide(hgammaf);
  hgammaf_Cm3->Add(hgammaf,-1);
  hgammaf_Cm3->SetTitle("Photon fraction, Hadron response -3%");
  hgammaf_Cm3->GetYaxis()->SetTitle("gammaf_{var} - gammaf");
  //hef_Cm3->Divide(hef);
  hef_Cm3->Add(hef,-1);
  hef_Cm3->SetTitle("Electron fraction, Hadron response -3%");
  hef_Cm3->GetYaxis()->SetTitle("ef_{var} - ef");

  //hchf_HadHCALp3->Divide(hchf);
  hchf_HadHCALp3->Add(hchf,-1);
  hchf_HadHCALp3->SetTitle("Charged hadron fraction, Hadron HCAL +3%");
  hchf_HadHCALp3->GetYaxis()->SetTitle("chf_{var} - chf");
  //hnhf_HadHCALp3->Divide(hnhf);
  hnhf_HadHCALp3->Add(hnhf,-1);
  hnhf_HadHCALp3->SetTitle("Neutral hadron fraction, Hadron HCAL +3%");
  hnhf_HadHCALp3->GetYaxis()->SetTitle("nhf_{var} - nhf");
  //hgammaf_HadHCALp3->Divide(hgammaf);
  hgammaf_HadHCALp3->Add(hgammaf,-1);
  hgammaf_HadHCALp3->SetTitle("Photon fraction, Hadron HCAL +3%");
  hgammaf_HadHCALp3->GetYaxis()->SetTitle("gammaf_{var} - gammaf");
  //hef_HadHCALp3->Divide(hef);
  hef_HadHCALp3->Add(hef,-1);
  hef_HadHCALp3->SetTitle("Electron fraction, Hadron HCAL +3%");
  hef_HadHCALp3->GetYaxis()->SetTitle("ef_{var} - ef");

  //hchf_HadHCALm3->Divide(hchf);
  hchf_HadHCALm3->Add(hchf,-1);
  hchf_HadHCALm3->SetTitle("Charged hadron fraction, Hadron HCAL -3%");
  hchf_HadHCALm3->GetYaxis()->SetTitle("chf_{var} - chf");
  //hnhf_HadHCALm3->Divide(hnhf);
  hnhf_HadHCALm3->Add(hnhf,-1);
  hnhf_HadHCALm3->SetTitle("Neutral hadron fraction, Hadron HCAL -3%");
  hnhf_HadHCALm3->GetYaxis()->SetTitle("nhf_{var} - nhf");
  //hgammaf_HadHCALm3->Divide(hgammaf);
  hgammaf_HadHCALm3->Add(hgammaf,-1);
  hgammaf_HadHCALm3->SetTitle("Photon fraction, Hadron HCAL -3%");
  hgammaf_HadHCALm3->GetYaxis()->SetTitle("gammaf_{var} - gammaf");
  //hef_HadHCALm3->Divide(hef);
  hef_HadHCALm3->Add(hef,-1);
  hef_HadHCALm3->SetTitle("Electron fraction, Hadron HCAL -3%");
  hef_HadHCALm3->GetYaxis()->SetTitle("ef_{var} - ef");

  //hchf_HadECALp3->Divide(hchf);
  hchf_HadECALp3->Add(hchf,-1);
  hchf_HadECALp3->SetTitle("Charged hadron fraction, Hadron ECAL +3%");
  hchf_HadECALp3->GetYaxis()->SetTitle("chf_{var} - chf");
  //hnhf_HadECALp3->Divide(hnhf);
  hnhf_HadECALp3->Add(hnhf,-1);
  hnhf_HadECALp3->SetTitle("Neutral hadron fraction, Hadron ECAL +3%");
  hnhf_HadECALp3->GetYaxis()->SetTitle("nhf_{var} - nhf");
  //hgammaf_HadECALp3->Divide(hgammaf);
  hgammaf_HadECALp3->Add(hgammaf,-1);
  hgammaf_HadECALp3->SetTitle("Photon fraction, Hadron ECAL +3%");
  hgammaf_HadECALp3->GetYaxis()->SetTitle("gammaf_{var} - gammaf");
  //hef_HadECALp3->Divide(hef);
  hef_HadECALp3->Add(hef,-1);
  hef_HadECALp3->SetTitle("Electron fraction, Hadron ECAL +3%");
  hef_HadECALp3->GetYaxis()->SetTitle("ef_{var} - ef");

  //hchf_HadECALm3->Divide(hchf);
  hchf_HadECALm3->Add(hchf,-1);
  hchf_HadECALm3->SetTitle("Charged hadron fraction, Hadron ECAL -3%");
  hchf_HadECALm3->GetYaxis()->SetTitle("chf_{var} - chf");
  //hnhf_HadECALm3->Divide(hnhf);
  hnhf_HadECALm3->Add(hnhf,-1);
  hnhf_HadECALm3->SetTitle("Neutral hadron fraction, Hadron ECAL -3%");
  hnhf_HadECALm3->GetYaxis()->SetTitle("nhf_{var} - nhf");
  //hgammaf_HadECALm3->Divide(hgammaf);
  hgammaf_HadECALm3->Add(hgammaf,-1);
  hgammaf_HadECALm3->SetTitle("Photon fraction, Hadron ECAL -3%");
  hgammaf_HadECALm3->GetYaxis()->SetTitle("gammaf_{var} - gammaf");
  //hef_HadECALm3->Divide(hef);
  hef_HadECALm3->Add(hef,-1);
  hef_HadECALm3->SetTitle("Electron fraction, Hadron ECAL -3%");
  hef_HadECALm3->GetYaxis()->SetTitle("ef_{var} - ef");

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

  //hchf_ECALm1->Divide(hchf);
  hchf_ECALm1->Add(hchf,-1);
  hchf_ECALm1->SetTitle("Charged hadron fraction, ECAL -1%");
  hchf_ECALm1->GetYaxis()->SetTitle("chf_{var} - chf");
  //hnhf_ECALm1->Divide(hnhf);
  hnhf_ECALm1->Add(hnhf,-1);
  hnhf_ECALm1->SetTitle("Neutral hadron fraction, ECAL -1%");
  hnhf_ECALm1->GetYaxis()->SetTitle("nhf_{var} - nhf");
  //hgammaf_ECALm1->Divide(hgammaf);
  hgammaf_ECALm1->Add(hgammaf,-1);
  hgammaf_ECALm1->SetTitle("Photon fraction, ECAL -1%");
  hgammaf_ECALm1->GetYaxis()->SetTitle("gammaf_{var} - gammaf");
  //hef_ECALm1->Divide(hef);
  hef_ECALm1->Add(hef,-1);
  hef_ECALm1->SetTitle("Electron fraction, ECAL -1%");
  hef_ECALm1->GetYaxis()->SetTitle("ef_{var} - ef");

  //hchf_ECALm3->Divide(hchf);
  hchf_ECALm3->Add(hchf,-1);
  hchf_ECALm3->SetTitle("Charged hadron fraction, ECAL -3%");
  hchf_ECALm3->GetYaxis()->SetTitle("chf_{var} - chf");
  //hnhf_ECALm3->Divide(hnhf);
  hnhf_ECALm3->Add(hnhf,-1);
  hnhf_ECALm3->SetTitle("Neutral hadron fraction, ECAL -3%");
  hnhf_ECALm3->GetYaxis()->SetTitle("nhf_{var} - nhf");
  //hgammaf_ECALm3->Divide(hgammaf);
  hgammaf_ECALm3->Add(hgammaf,-1);
  hgammaf_ECALm3->SetTitle("Photon fraction, ECAL -3%");
  hgammaf_ECALm3->GetYaxis()->SetTitle("gammaf_{var} - gammaf");
  //hef_ECALm3->Divide(hef);
  hef_ECALm3->Add(hef,-1);
  hef_ECALm3->SetTitle("Electron fraction, ECAL -3%");
  hef_ECALm3->GetYaxis()->SetTitle("ef_{var} - ef");



  hchf_Photonm1->Add(hchf,-1);
  hchf_Photonm1->SetTitle("Charged hadron fraction, Photon response -1%");
  hchf_Photonm1->GetYaxis()->SetTitle("chf_{var} - chf");
  hnhf_Photonm1->Add(hnhf,-1);
  hnhf_Photonm1->SetTitle("Neutral hadron fraction, Photon response -1%");
  hnhf_Photonm1->GetYaxis()->SetTitle("nhf_{var} - nhf");
  hgammaf_Photonm1->Add(hgammaf,-1);
  hgammaf_Photonm1->SetTitle("Photon fraction, Photon response -1%");
  hgammaf_Photonm1->GetYaxis()->SetTitle("gammaf_{var} - gammaf");
  hef_Photonm1->Add(hef,-1);
  hef_Photonm1->SetTitle("Electron fraction, Photon response -1%");
  hef_Photonm1->GetYaxis()->SetTitle("ef_{var} - ef");

  hchf_Photonm3->Add(hchf,-1);
  hchf_Photonm3->SetTitle("Charged hadron fraction, Photon response -3%");
  hchf_Photonm3->GetYaxis()->SetTitle("chf_{var} - chf");
  hnhf_Photonm3->Add(hnhf,-1);
  hnhf_Photonm3->SetTitle("Neutral hadron fraction, Photon response -3%");
  hnhf_Photonm3->GetYaxis()->SetTitle("nhf_{var} - nhf");
  hgammaf_Photonm3->Add(hgammaf,-1);
  hgammaf_Photonm3->SetTitle("Photon fraction, Photon response -3%");
  hgammaf_Photonm3->GetYaxis()->SetTitle("gammaf_{var} - gammaf");
  hef_Photonm3->Add(hef,-1);
  hef_Photonm3->SetTitle("Electron fraction, Photon response -3%");
  hef_Photonm3->GetYaxis()->SetTitle("ef_{var} - ef");

  TFile *foutvar = new TFile(outname.c_str(),"RECREATE");

  h_Rjet_Cp3      ->Write("h_Rjet_Cp3");
  h_Rjet_Cm3      ->Write("h_Rjet_Cm3");
  h_Rjet_HadHCALp3->Write("h_Rjet_HadHCALp3");
  h_Rjet_HadHCALm3->Write("h_Rjet_HadHCALm3");
  h_Rjet_HadECALp3->Write("h_Rjet_HadECALp3");
  h_Rjet_HadECALm3->Write("h_Rjet_HadECALm3");
  h_Rjet_Trkm1    ->Write("h_Rjet_Trkm1");
  h_Rjet_Trkm3    ->Write("h_Rjet_Trkm3");
  h_Rjet_ECALm1   ->Write("h_Rjet_ECALm1");
  h_Rjet_ECALm3   ->Write("h_Rjet_ECALm3");
  h_Rjet_Photonm1 ->Write("h_Rjet_Photonm1");
  h_Rjet_Photonm3 ->Write("h_Rjet_Photonm3");
  h_Rjet_calo_Cm3->Write("h_Rjet_calo_Cm3");
  h_Rjet_calo_Cp3->Write("h_Rjet_calo_Cp3");
  //h_Rjet_Trk->Write("h_Rjet_Trk");
  //h_Rjet_Photon->Write("h_Rjet_Photon");



  h_Rjetb_Cp3      ->Write("h_Rjetb_Cp3");
  h_Rjetb_Cm3      ->Write("h_Rjetb_Cm3");
  h_Rjetb_HadHCALp3->Write("h_Rjetb_HadHCALp3");
  h_Rjetb_HadHCALm3->Write("h_Rjetb_HadHCALm3");
  h_Rjetb_HadECALp3->Write("h_Rjetb_HadECALp3");
  h_Rjetb_HadECALm3->Write("h_Rjetb_HadECALm3");
  h_Rjetb_Trkm1    ->Write("h_Rjetb_Trkm1");
  h_Rjetb_Trkm3    ->Write("h_Rjetb_Trkm3");
  h_Rjetb_ECALm1   ->Write("h_Rjetb_ECALm1");
  h_Rjetb_ECALm3   ->Write("h_Rjetb_ECALm3");
  h_Rjetb_Photonm1 ->Write("h_Rjetb_Photonm1");
  h_Rjetb_Photonm3 ->Write("h_Rjetb_Photonm3");


  h_Rjetc_Cp3      ->Write("h_Rjetc_Cp3");
  h_Rjetc_Cm3      ->Write("h_Rjetc_Cm3");
  h_Rjetc_HadHCALp3->Write("h_Rjetc_HadHCALp3");
  h_Rjetc_HadHCALm3->Write("h_Rjetc_HadHCALm3");
  h_Rjetc_HadECALp3->Write("h_Rjetc_HadECALp3");
  h_Rjetc_HadECALm3->Write("h_Rjetc_HadECALm3");
  h_Rjetc_Trkm1    ->Write("h_Rjetc_Trkm1");
  h_Rjetc_Trkm3    ->Write("h_Rjetc_Trkm3");
  h_Rjetc_ECALm1   ->Write("h_Rjetc_ECALm1");
  h_Rjetc_ECALm3   ->Write("h_Rjetc_ECALm3");
  h_Rjetc_Photonm1 ->Write("h_Rjetc_Photonm1");
  h_Rjetc_Photonm3 ->Write("h_Rjetc_Photonm3");

  h_Rjets_Cp3      ->Write("h_Rjets_Cp3");
  h_Rjets_Cm3      ->Write("h_Rjets_Cm3");
  h_Rjets_HadHCALp3->Write("h_Rjets_HadHCALp3");
  h_Rjets_HadHCALm3->Write("h_Rjets_HadHCALm3");
  h_Rjets_HadECALp3->Write("h_Rjets_HadECALp3");
  h_Rjets_HadECALm3->Write("h_Rjets_HadECALm3");
  h_Rjets_Trkm1    ->Write("h_Rjets_Trkm1");
  h_Rjets_Trkm3    ->Write("h_Rjets_Trkm3");
  h_Rjets_ECALm1   ->Write("h_Rjets_ECALm1");
  h_Rjets_ECALm3   ->Write("h_Rjets_ECALm3");
  h_Rjets_Photonm1 ->Write("h_Rjets_Photonm1");
  h_Rjets_Photonm3 ->Write("h_Rjets_Photonm3");

  h_Rjetud_Cp3      ->Write("h_Rjetud_Cp3");
  h_Rjetud_Cm3      ->Write("h_Rjetud_Cm3");
  h_Rjetud_HadHCALp3->Write("h_Rjetud_HadHCALp3");
  h_Rjetud_HadHCALm3->Write("h_Rjetud_HadHCALm3");
  h_Rjetud_HadECALp3->Write("h_Rjetud_HadECALp3");
  h_Rjetud_HadECALm3->Write("h_Rjetud_HadECALm3");
  h_Rjetud_Trkm1    ->Write("h_Rjetud_Trkm1");
  h_Rjetud_Trkm3    ->Write("h_Rjetud_Trkm3");
  h_Rjetud_ECALm1   ->Write("h_Rjetud_ECALm1");
  h_Rjetud_ECALm3   ->Write("h_Rjetud_ECALm3");
  h_Rjetud_Photonm1 ->Write("h_Rjetud_Photonm1");
  h_Rjetud_Photonm3 ->Write("h_Rjetud_Photonm3");

  h_Rjetg_Cp3      ->Write("h_Rjetg_Cp3");
  h_Rjetg_Cm3      ->Write("h_Rjetg_Cm3");
  h_Rjetg_HadHCALp3->Write("h_Rjetg_HadHCALp3");
  h_Rjetg_HadHCALm3->Write("h_Rjetg_HadHCALm3");
  h_Rjetg_HadECALp3->Write("h_Rjetg_HadECALp3");
  h_Rjetg_HadECALm3->Write("h_Rjetg_HadECALm3");
  h_Rjetg_Trkm1    ->Write("h_Rjetg_Trkm1");
  h_Rjetg_Trkm3    ->Write("h_Rjetg_Trkm3");
  h_Rjetg_ECALm1   ->Write("h_Rjetg_ECALm1");
  h_Rjetg_ECALm3   ->Write("h_Rjetg_ECALm3");
  h_Rjetg_Photonm1 ->Write("h_Rjetg_Photonm1");
  h_Rjetg_Photonm3 ->Write("h_Rjetg_Photonm3");


  hchf_Cp3         ->Write("hchf_Cp3");
  hnhf_Cp3         ->Write("hnhf_Cp3");
  hgammaf_Cp3      ->Write("hgammaf_Cp3");
  hef_Cp3          ->Write("hef_Cp3");
  hchf_Cm3         ->Write("hchf_Cm3");
  hnhf_Cm3         ->Write("hnhf_Cm3");
  hgammaf_Cm3      ->Write("hgammaf_Cm3");
  hef_Cm3          ->Write("hef_Cm3");
  hchf_HadHCALp3   ->Write("hchf_HadHCALp3");
  hnhf_HadHCALp3   ->Write("hnhf_HadHCALp3");
  hgammaf_HadHCALp3->Write("hgammaf_HadHCALp3");
  hef_HadHCALp3    ->Write("hef_HadHCALp3");
  hchf_HadHCALm3   ->Write("hchf_HadHCALm3");
  hnhf_HadHCALm3   ->Write("hnhf_HadHCALm3");
  hgammaf_HadHCALm3->Write("hgammaf_HadHCALm3");
  hef_HadHCALm3    ->Write("hef_HadHCALm3");
  hchf_HadECALp3   ->Write("hchf_HadECALp3");
  hnhf_HadECALp3   ->Write("hnhf_HadECALp3");
  hgammaf_HadECALp3->Write("hgammaf_HadECALp3");
  hef_HadECALp3    ->Write("hef_HadECALp3");
  hchf_HadECALm3   ->Write("hchf_HadECALm3");
  hnhf_HadECALm3   ->Write("hnhf_HadECALm3");
  hgammaf_HadECALm3->Write("hgammaf_HadECALm3");
  hef_HadECALm3    ->Write("hef_HadECALm3");
  hchf_Trkm1       ->Write("hchf_Trkm1");
  hnhf_Trkm1       ->Write("hnhf_Trkm1");
  hgammaf_Trkm1    ->Write("hgammaf_Trkm1");
  hef_Trkm1        ->Write("hef_Trkm1");
  hchf_Trkm3       ->Write("hchf_Trkm3");
  hnhf_Trkm3       ->Write("hnhf_Trkm3");
  hgammaf_Trkm3    ->Write("hgammaf_Trkm3");
  hef_Trkm3        ->Write("hef_Trkm3");
  hchf_ECALm1      ->Write("hchf_ECALm1");
  hnhf_ECALm1      ->Write("hnhf_ECALm1");
  hgammaf_ECALm1   ->Write("hgammaf_ECALm1");
  hef_ECALm1       ->Write("hef_ECALm1");
  hchf_ECALm3      ->Write("hchf_ECALm3");
  hnhf_ECALm3      ->Write("hnhf_ECALm3");
  hgammaf_ECALm3   ->Write("hgammaf_ECALm3");
  hef_ECALm3       ->Write("hef_ECALm3");
  
  hchf_Photonm1    ->Write("hchf_Photonm1");
  hnhf_Photonm1    ->Write("hnhf_Photonm1");
  hgammaf_Photonm1 ->Write("hgammaf_Photonm1");
  hef_Photonm1     ->Write("hef_Photonm1");
  hchf_Photonm3    ->Write("hchf_Photonm3");
  hnhf_Photonm3    ->Write("hnhf_Photonm3");
  hgammaf_Photonm3 ->Write("hgammaf_Photonm3");
  hef_Photonm3     ->Write("hef_Photonm3");


  foutvar->Write();
  foutvar->Close();

  gApplication->Terminate(0);

}





