void plotVariants()
{
  //Choose filenames to open

  //Initialize histograms and open ROOT files and fetch the stored objects
  TFile* fzj = TFile::Open("./output_ROOT_files/CMSJES_P8_Zjet_30000_1.root");
  //TFile* fzj_var = TFile::Open("./output_ROOT_files/CMSJES_P8_Zjet_100000_HCAL3.root");
  TFile* fzj_var = TFile::Open("./output_ROOT_files/CMSJES_P8_Zjet_30000_C.root");

  TProfile *pr_Rjet=0;
  TProfile *pr_Rjet_var=0;

  //Create Histograms
  fzj->GetObject("prRjet"   ,pr_Rjet  );
  fzj_var->GetObject("prRjet",pr_Rjet_var);

  TH1D* h_Rjet     = pr_Rjet->ProjectionX();
  TH1D* h_Rjet_var = pr_Rjet_var->ProjectionX();

  h_Rjet_var->Divide(h_Rjet);

  //Canvas
  TCanvas* canv_var = new TCanvas("canv_var","",600,600);
  canv_var->SetLeftMargin(0.15);
  canv_var->SetBottomMargin(0.13);

  h_Rjet_var->SetMarkerStyle(kFullCircle); h_Rjet_var->SetMarkerColor(kBlack);
  h_Rjet_var->SetLineColor(kBlack); 

  //Legend
  TLegend* lz_Rjet = new TLegend(0.58,0.2,0.89,0.40);
  lz_Rjet->SetBorderSize(0);
  //lz_Rjet->AddEntry(h_Rjet_var, "HCAL + 3%",   "p");
  lz_Rjet->AddEntry(h_Rjet_var, "C + 3%",   "p");

  //Title and axis setup
  h_Rjet_var->SetStats(0); //Suppress stat box
  h_Rjet_var->SetTitle("");
  h_Rjet_var->SetAxisRange(0.955,1.05,"Y"); //Vertical axis limits
  h_Rjet_var->GetXaxis()->SetMoreLogLabels();
  h_Rjet_var->GetXaxis()->SetNoExponent();
  canv_var->SetLogx();
  h_Rjet_var->GetYaxis()->SetTitleOffset(1.8);
  h_Rjet_var->GetXaxis()->SetTitleOffset(1.2);

  gPad->SetTickx(); gPad->SetTicky();

  //Savefile name setup
  //string savename = "./plots/varPlots/varHCAL";
  string savename = "./plots/varPlots/varSPR";
  savename+=".eps";

  //Plot
  h_Rjet_var->Draw("P");
  lz_Rjet->Draw("SAMEP");

  //Save plot
  canv_var->Print(savename.c_str());
}
