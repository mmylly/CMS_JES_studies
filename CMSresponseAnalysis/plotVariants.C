void plotVariants()
{
  //Choose filenames to open

  //Initialize histograms and open ROOT files and fetch the stored objects
  TFile* fzj = TFile::Open("./output_ROOT_files/CMSJES_P8_Zjet_100000_1.root");
  TFile* fzj_var = TFile::Open("./output_ROOT_files/CMSJES_P8_Zjet_100000_C+3.root");
  TFile* fzj_var2 = TFile::Open("./output_ROOT_files/CMSJES_P8_Zjet_100000_C-3.root");

  // SPR +-3% JEC paper
  TGraph *JEC_SPRp3  = new TGraph("data_and_MC_input/Response/JEC_PFJet_SPRplus3.txt" );
  TGraph *JEC_SPRm3  = new TGraph("data_and_MC_input/Response/JEC_PFJet_SPRminus3.txt" );

  JEC_SPRp3->SetMarkerStyle(kFullCircle); JEC_SPRp3->SetMarkerColor(kRed);
  JEC_SPRm3->SetMarkerStyle(kFullCircle); JEC_SPRm3->SetMarkerColor(kBlue+1);

  TProfile *pr_Rjet=0;
  TProfile *pr_Rjet_var=0;
  TProfile *pr_Rjet_var2=0;

  //Create Histograms
  fzj     ->GetObject("prRjet",pr_Rjet);
  fzj_var ->GetObject("prRjet",pr_Rjet_var);
  fzj_var2->GetObject("prRjet",pr_Rjet_var2);

  TH1D* h_Rjet      = pr_Rjet->ProjectionX();
  TH1D* h_Rjet_var  = pr_Rjet_var->ProjectionX();
  TH1D* h_Rjet_var2 = pr_Rjet_var2->ProjectionX();

  h_Rjet_var->Divide(h_Rjet);
  h_Rjet_var2->Divide(h_Rjet);

  //Canvas
  TCanvas* canv_var = new TCanvas("canv_var","",600,600);
  canv_var->SetLeftMargin(0.15);
  canv_var->SetBottomMargin(0.13);

  h_Rjet_var->SetMarkerStyle(kOpenCircle); h_Rjet_var->SetMarkerColor(kRed);
  h_Rjet_var2->SetMarkerStyle(kOpenCircle); h_Rjet_var2->SetMarkerColor(kBlue+1);
  h_Rjet_var->SetLineColor(kRed); h_Rjet_var2->SetLineColor(kBlue+1); 

  //Legend
  TLegend* lz_Rjet = new TLegend(0.2,0.65,0.5,0.85);
  lz_Rjet->SetBorderSize(0);
  //lz_Rjet->AddEntry(h_Rjet_var, "HCAL + 3%",   "p");
  lz_Rjet->AddEntry(h_Rjet_var, "C + 3%",   "p");
  lz_Rjet->AddEntry(h_Rjet_var2, "C - 3%",   "p");
  lz_Rjet->AddEntry(JEC_SPRp3, "C + 3% (JEC 8TeV)", "p");
  lz_Rjet->AddEntry(JEC_SPRm3, "C - 3% (JEC 8TeV)", "p");  

  //Title and axis setup
  h_Rjet_var->SetStats(0); //Suppress stat box
  h_Rjet_var->SetTitle("");
  h_Rjet_var->SetAxisRange(0.955,1.05,"Y"); //Vertical axis limits
  h_Rjet_var->GetXaxis()->SetMoreLogLabels();
  h_Rjet_var->GetXaxis()->SetNoExponent();
  canv_var->SetLogx();
  h_Rjet_var->GetYaxis()->SetTitleOffset(1.8);
  h_Rjet_var->GetXaxis()->SetTitleOffset(1.2);
  h_Rjet_var->GetYaxis()->SetTitle("Response ratio");
  h_Rjet_var->GetXaxis()->SetTitle("p_{T}^{gen} [GeV]");

  gPad->SetTickx(); gPad->SetTicky();

  //Savefile name setup
  //string savename = "./plots/varPlots/varHCAL";
  string savename = "./plots/varPlots/varSPR";
  savename+=".eps";

  //Plot
  h_Rjet_var->Draw("P");
  h_Rjet_var2->Draw("SameP");
  JEC_SPRp3->Draw("SameP");
  JEC_SPRm3->Draw("SameP");
  lz_Rjet->Draw("SAMEP");

  //Save plot
  canv_var->Print(savename.c_str());
  gApplication->Terminate();
}
