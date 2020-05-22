void plotHadHCALVar()
{

  double markersize = 0.8;

  //Variants file
  TFile* fzj = TFile::Open("./output_ROOT_files/varPlots_5000000.root");

  TH1D* h_Rjet_HadHCALp3;
  TH1D* h_Rjet_HadHCALm3;
  TH1D* h_Rjetb_HadHCALp3;
  TH1D* h_Rjetb_HadHCALm3;
  TH1D* h_Rjetc_HadHCALp3;
  TH1D* h_Rjetc_HadHCALm3;
  TH1D* h_Rjets_HadHCALp3;
  TH1D* h_Rjets_HadHCALm3;
  TH1D* h_Rjetud_HadHCALp3;
  TH1D* h_Rjetud_HadHCALm3;
  TH1D* h_Rjetg_HadHCALp3;
  TH1D* h_Rjetg_HadHCALm3;


  //Create Histograms
  fzj->GetObject("h_Rjet_HadHCALp3", h_Rjet_HadHCALp3);
  fzj->GetObject("h_Rjet_HadHCALm3", h_Rjet_HadHCALm3);

  fzj->GetObject("h_Rjetb_HadHCALp3", h_Rjetb_HadHCALp3);
  fzj->GetObject("h_Rjetb_HadHCALm3", h_Rjetb_HadHCALm3);

  fzj->GetObject("h_Rjetc_HadHCALp3", h_Rjetc_HadHCALp3);
  fzj->GetObject("h_Rjetc_HadHCALm3", h_Rjetc_HadHCALm3);

  fzj->GetObject("h_Rjets_HadHCALp3", h_Rjets_HadHCALp3);
  fzj->GetObject("h_Rjets_HadHCALm3", h_Rjets_HadHCALm3);

  fzj->GetObject("h_Rjetud_HadHCALp3", h_Rjetud_HadHCALp3);
  fzj->GetObject("h_Rjetud_HadHCALm3", h_Rjetud_HadHCALm3);

  fzj->GetObject("h_Rjetg_HadHCALp3", h_Rjetg_HadHCALp3);
  fzj->GetObject("h_Rjetg_HadHCALm3", h_Rjetg_HadHCALm3);

// HCAL
  TCanvas* canv_HadHCAL = new TCanvas("canv_HadHCAL","",500,500);
  canv_HadHCAL->SetLeftMargin(0.15); canv_HadHCAL->SetBottomMargin(0.13);

  TGraph *JEC_HCALp3 = new TGraph("data_and_MC_input/Response/JEC_PFJet_HCALplus3.txt" );
  TGraph *JEC_HCALm3 = new TGraph("data_and_MC_input/Response/JEC_PFJet_HCALminus3.txt" );

  JEC_HCALp3->SetMarkerStyle(kOpenCircle); JEC_HCALp3->SetMarkerColor(kRed);
  JEC_HCALm3->SetMarkerStyle(kOpenSquare); JEC_HCALm3->SetMarkerColor(kBlue+1);
  JEC_HCALp3->SetMarkerSize(markersize);
  JEC_HCALm3->SetMarkerSize(markersize);

  h_Rjet_HadHCALp3->SetMarkerStyle(kFullCircle); h_Rjet_HadHCALm3->SetMarkerStyle(kFullSquare);
  h_Rjet_HadHCALp3->SetMarkerColor(kRed);    h_Rjet_HadHCALm3->SetMarkerColor(kBlue+1);
  h_Rjet_HadHCALp3->SetLineColor(kRed);      h_Rjet_HadHCALm3->SetLineColor(kBlue+1);
  h_Rjet_HadHCALp3->SetMarkerSize(markersize); h_Rjet_HadHCALm3->SetMarkerSize(markersize);

  TLegend* lz_HadHCAL = new TLegend(0.18,0.73,0.75,0.87);
  //lz_HCAL->SetNColumns(2);
  lz_HadHCAL->SetBorderSize(0);
  lz_HadHCAL->AddEntry(h_Rjet_HadHCALp3,  "Hadron HCAL+3% (Our)", "p");
  lz_HadHCAL->AddEntry(JEC_HCALp3,        "Hadron HCAL+3% (FastSim)", "p");
  lz_HadHCAL->AddEntry(h_Rjet_HadHCALm3,  "Hadron HCAL-3% (Our)", "p");
  lz_HadHCAL->AddEntry(JEC_HCALm3,        "Hadron HCAL-3% (FastSim)", "p"); 

  //TH1D* setup = new TH1D("setup","", 
  //                       h_Rjet_HCALp3->GetXaxis()->GetNbins(),
  //		 h_Rjet_HCALp3->GetXaxis()->GetXmin(), 
  //                       h_Rjet_HCALp3->GetXaxis()->GetXmax());

  TH1D* setup = new TH1D("setup","", h_Rjet_HadHCALp3->GetXaxis()->GetNbins(), 28, 1240);
  //Title and axis setup
  setup->SetStats(0); //Suppress stat box
  setup->SetAxisRange(0.985,1.02,"Y"); //Vertical axis limits
  //setup->SetAxisRange(28,2000,"X"); //Horizontal axis limits

  h_Rjet_HadHCALp3->SetAxisRange(28,1248,"X");
  h_Rjet_HadHCALm3->SetAxisRange(28,1248,"X");

  setup->GetXaxis()->SetMoreLogLabels();
  setup->GetXaxis()->SetNoExponent();
  canv_HadHCAL->SetLogx();
  setup->GetYaxis()->SetTitleOffset(1.5);
  setup->GetXaxis()->SetTitleOffset(1.1);
  setup->GetYaxis()->SetTitle("F^{var}");
  setup->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup->GetYaxis()->SetTitleSize(0.05);
  setup->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();



  //Plot
  setup->Draw();
  JEC_HCALp3->Draw("SAMEP");
  JEC_HCALm3->Draw("SAMEP");
  h_Rjet_HadHCALp3->Draw("SAMEP");
  h_Rjet_HadHCALm3->Draw("SAMEP");
  lz_HadHCAL->Draw("SAMEP");

  TLatex *tex = new TLatex(); tex->SetNDC();
  tex->SetTextSize(0.04); tex->SetTextColor(kBlack);
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");

  setup->GetXaxis()->SetRangeUser(28,2000);
  gPad->Modified();

  //Save plot
  canv_HadHCAL->Print("./plots/varPlots/hadHCAL/varHCAL.pdf"); delete canv_HadHCAL;

  //All flavours  

  TCanvas* canv_HadHCAL_all = new TCanvas("canv_HadHCAL_all","",500,500);
  canv_HadHCAL_all->SetLeftMargin(0.15); canv_HadHCAL_all->SetBottomMargin(0.13);

  h_Rjet_HadHCALp3->SetMarkerStyle(2);             h_Rjet_HadHCALm3->SetMarkerStyle(5);
  h_Rjet_HadHCALp3->SetMarkerColor(kBlack);        h_Rjet_HadHCALm3->SetMarkerColor(kBlack);
  h_Rjet_HadHCALp3->SetLineColor(kBlack);          h_Rjet_HadHCALm3->SetLineColor(kBlack);
  h_Rjet_HadHCALp3->SetMarkerSize(markersize);     h_Rjet_HadHCALm3->SetMarkerSize(markersize);

  h_Rjetb_HadHCALp3->SetMarkerStyle(kFullCircle);  h_Rjetb_HadHCALm3->SetMarkerStyle(kOpenCircle);
  h_Rjetb_HadHCALp3->SetMarkerColor(kRed+1);       h_Rjetb_HadHCALm3->SetMarkerColor(kRed+1);
  h_Rjetb_HadHCALp3->SetLineColor(kRed+1);         h_Rjetb_HadHCALm3->SetLineColor(kRed+1);
  h_Rjetb_HadHCALp3->SetMarkerSize(markersize);    h_Rjetb_HadHCALm3->SetMarkerSize(markersize);

  h_Rjetc_HadHCALp3->SetMarkerStyle(kFullTriangleDown); 
  h_Rjetc_HadHCALm3->SetMarkerStyle(kOpenTriangleDown); 
  h_Rjetc_HadHCALp3->SetMarkerColor(kGreen+3);          
  h_Rjetc_HadHCALm3->SetMarkerColor(kGreen+3);
  h_Rjetc_HadHCALp3->SetLineColor(kGreen+3);            
  h_Rjetc_HadHCALm3->SetLineColor(kGreen+3);
  h_Rjetc_HadHCALp3->SetMarkerSize(markersize);         
  h_Rjetc_HadHCALm3->SetMarkerSize(markersize);

  h_Rjets_HadHCALp3->SetMarkerStyle(kFullTriangleUp); 
  h_Rjets_HadHCALm3->SetMarkerStyle(kOpenTriangleUp);
  h_Rjets_HadHCALp3->SetMarkerColor(kOrange+7);       
  h_Rjets_HadHCALm3->SetMarkerColor(kOrange+7);
  h_Rjets_HadHCALp3->SetLineColor(kOrange+7);         
  h_Rjets_HadHCALm3->SetLineColor(kOrange+7);
  h_Rjets_HadHCALp3->SetMarkerSize(markersize);       
  h_Rjets_HadHCALm3->SetMarkerSize(markersize);

  h_Rjetud_HadHCALp3->SetMarkerStyle(kFullDiamond); 
  h_Rjetud_HadHCALm3->SetMarkerStyle(kOpenDiamond);
  h_Rjetud_HadHCALp3->SetMarkerColor(kMagenta+2);   
  h_Rjetud_HadHCALm3->SetMarkerColor(kMagenta+2);
  h_Rjetud_HadHCALp3->SetLineColor(kMagenta+2);     
  h_Rjetud_HadHCALm3->SetLineColor(kMagenta+2);
  h_Rjetud_HadHCALp3->SetMarkerSize(markersize);    
  h_Rjetud_HadHCALm3->SetMarkerSize(markersize);

  h_Rjetg_HadHCALp3->SetMarkerStyle(kFullSquare); 
  h_Rjetg_HadHCALm3->SetMarkerStyle(kOpenSquare);
  h_Rjetg_HadHCALp3->SetMarkerColor(kBlue+1);     
  h_Rjetg_HadHCALm3->SetMarkerColor(kBlue+1);
  h_Rjetg_HadHCALp3->SetLineColor(kBlue+1);       
  h_Rjetg_HadHCALm3->SetLineColor(kBlue+1);
  h_Rjetg_HadHCALp3->SetMarkerSize(markersize);   
  h_Rjetg_HadHCALm3->SetMarkerSize(markersize);

  //Legend
  TLegend* lz_HadHCAL_all = new TLegend(0.2,0.65,0.55,0.87);
  lz_HadHCAL_all->SetBorderSize(0);
  lz_HadHCAL_all->AddEntry(h_Rjet_HadHCALp3, "All jets Hadron HCAL+3%", "p");
  lz_HadHCAL_all->AddEntry(h_Rjet_HadHCALm3, "All jets Hadron HCAL-3%", "p");
  lz_HadHCAL_all->AddEntry(h_Rjetb_HadHCALp3, "b jets Hadron HCAL+3%", "p");
  lz_HadHCAL_all->AddEntry(h_Rjetb_HadHCALm3, "b jets Hadron HCAL-3%", "p");
  lz_HadHCAL_all->AddEntry(h_Rjetc_HadHCALp3, "c jets Hadron HCAL+3%", "p");
  lz_HadHCAL_all->AddEntry(h_Rjetc_HadHCALm3, "c jets Hadron HCAL-3%", "p");
  lz_HadHCAL_all->AddEntry(h_Rjets_HadHCALp3, "s jets Hadron HCAL+3%", "p");
  lz_HadHCAL_all->AddEntry(h_Rjets_HadHCALm3, "s jets Hadron HCAL-3%", "p");
  lz_HadHCAL_all->AddEntry(h_Rjetud_HadHCALp3, "ud jets Hadron HCAL+3%", "p");
  lz_HadHCAL_all->AddEntry(h_Rjetud_HadHCALm3, "ud jets Hadron HCAL-3%", "p");
  lz_HadHCAL_all->AddEntry(h_Rjetg_HadHCALp3, "g jets Hadron HCAL+3%", "p");
  lz_HadHCAL_all->AddEntry(h_Rjetg_HadHCALm3, "g jets Hadron HCAL-3%", "p");
  lz_HadHCAL_all->SetNColumns(2);
  TH1D* setup_all = new TH1D("setup_all","", h_Rjet_HadHCALp3->GetXaxis()->GetNbins(), 28, 1240);


  //Title and axis setup
  setup_all->SetStats(0); //Suppress stat box
  setup_all->SetAxisRange(0.978,1.028,"Y"); //Vertical axis limits
  h_Rjetb_HadHCALp3->SetAxisRange(28, 1240,"X");
  h_Rjetb_HadHCALm3->SetAxisRange(28, 1240,"X");
  h_Rjetc_HadHCALp3->SetAxisRange(28, 1240,"X");
  h_Rjetc_HadHCALm3->SetAxisRange(28, 1240,"X");
  h_Rjets_HadHCALp3->SetAxisRange(28, 1240,"X");
  h_Rjets_HadHCALm3->SetAxisRange(28, 1240,"X");
  h_Rjetud_HadHCALp3->SetAxisRange(28, 1240,"X");
  h_Rjetud_HadHCALm3->SetAxisRange(28, 1240,"X");
  h_Rjetg_HadHCALp3->SetAxisRange(28, 1240,"X");
  h_Rjetg_HadHCALm3->SetAxisRange(28, 1240,"X");
  h_Rjet_HadHCALp3->SetAxisRange(28, 1240,"X");
  h_Rjet_HadHCALm3->SetAxisRange(28, 1240,"X");
  setup_all->GetXaxis()->SetMoreLogLabels();
  setup_all->GetXaxis()->SetNoExponent();
  canv_HadHCAL_all->SetLogx();
  setup_all->GetYaxis()->SetTitleOffset(1.5);
  setup_all->GetXaxis()->SetTitleOffset(1.1);
  setup_all->GetYaxis()->SetTitle("F^{var}");
  setup_all->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_all->GetYaxis()->SetTitleSize(0.05);
  setup_all->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  //Plot
  setup_all->Draw();
  h_Rjetb_HadHCALp3->Draw("SAMEP");
  h_Rjetb_HadHCALm3->Draw("SAMEP");
  h_Rjetc_HadHCALp3->Draw("SAMEP");
  h_Rjetc_HadHCALm3->Draw("SAMEP");
  h_Rjets_HadHCALp3->Draw("SAMEP");
  h_Rjets_HadHCALm3->Draw("SAMEP");
  h_Rjetud_HadHCALp3->Draw("SAMEP");
  h_Rjetud_HadHCALm3->Draw("SAMEP");
  h_Rjetg_HadHCALp3->Draw("SAMEP");
  h_Rjetg_HadHCALm3->Draw("SAMEP");
  h_Rjet_HadHCALp3->Draw("SAMEP");
  h_Rjet_HadHCALm3->Draw("SAMEP");
  lz_HadHCAL_all->Draw("SAMEP");

  TLatex *texa = new TLatex(); 
  texa->SetNDC();
  texa->SetTextSize(0.04); 
  texa->SetTextColor(kBlack);
  texa->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texa->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texa->DrawLatex(0.23,0.2,"#alpha<0.3");

  //Save plot
  canv_HadHCAL_all->Print("./plots/varPlots/hadHCAL/varHCAL_all.pdf"); delete canv_HadHCAL_all;


  // b jets
  TCanvas* canv_HadHCAL_b = new TCanvas("canv_HadHCAL_b","",500,500);
  canv_HadHCAL_b->SetLeftMargin(0.15); canv_HadHCAL_b->SetBottomMargin(0.13);

  TLegend* lz_HadHCALb = new TLegend(0.18,0.68,0.6,0.86);
  lz_HadHCALb->SetBorderSize(0);
  lz_HadHCALb->AddEntry(h_Rjet_HadHCALp3, "All jets Hadron HCAL+3%", "p");
  lz_HadHCALb->AddEntry(h_Rjet_HadHCALm3, "All jets Hadron HCAL-3%", "p");
  lz_HadHCALb->AddEntry(h_Rjetb_HadHCALp3, "b jets Hadron HCAL+3%", "p");
  lz_HadHCALb->AddEntry(h_Rjetb_HadHCALm3, "b jets Hadron HCAL-3%", "p");
  TH1D* setupb = new TH1D("setupb","", h_Rjet_HadHCALp3->GetXaxis()->GetNbins(),
			 28,1248);
  //Title and axis setup
  setupb->SetStats(0); //Suppress stat box
  setupb->SetAxisRange(0.983,1.017,"Y"); //Vertical axis limits
  setupb->GetXaxis()->SetMoreLogLabels();
  setupb->GetXaxis()->SetNoExponent();
  canv_HadHCAL_b->SetLogx();
  setupb->GetYaxis()->SetTitleOffset(1.5);
  setupb->GetXaxis()->SetTitleOffset(1.1);
  setupb->GetYaxis()->SetTitle("F^{var}");
  setupb->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupb->GetYaxis()->SetTitleSize(0.05);
  setupb->GetXaxis()->SetTitleSize(0.05);

  h_Rjetb_HadHCALp3->SetAxisRange(28,1248,"X");
  h_Rjetb_HadHCALm3->SetAxisRange(28,1248,"X");

  gPad->SetTickx(); gPad->SetTicky();
  //Plot
  setupb->Draw();
  h_Rjetb_HadHCALp3->Draw("SAMEP");
  h_Rjetb_HadHCALm3->Draw("SAMEP");
  h_Rjet_HadHCALp3->Draw("SAMEP");
  h_Rjet_HadHCALm3->Draw("SAMEP");
  lz_HadHCALb->Draw("SAMEP");

  TLatex *texb = new TLatex(); 
  texb->SetNDC();
  texb->SetTextSize(0.04); 
  texb->SetTextColor(kBlack);
  texb->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texb->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texb->DrawLatex(0.23,0.2,"#alpha<0.3");

  //Save plot
  canv_HadHCAL_b->Print("./plots/varPlots/hadHCAL/varHCAL_b.pdf"); delete canv_HadHCAL_b;

  // c jets
  TCanvas* canv_HadHCAL_c = new TCanvas("canv_HadHCAL_c","",500,500);
  canv_HadHCAL_c->SetLeftMargin(0.15); canv_HadHCAL_c->SetBottomMargin(0.13);
  TLegend* lz_HadHCALc = new TLegend(0.18,0.68,0.6,0.86);
  lz_HadHCALc->SetBorderSize(0);
  lz_HadHCALc->AddEntry(h_Rjet_HadHCALp3, "All jets Hadron HCAL+3%", "p");
  lz_HadHCALc->AddEntry(h_Rjet_HadHCALm3, "All jets Hadron HCAL-3%", "p");
  lz_HadHCALc->AddEntry(h_Rjetc_HadHCALp3, "c jets Hadron HCAL+3%", "p");
  lz_HadHCALc->AddEntry(h_Rjetc_HadHCALm3, "c jets Hadron HCAL-3%", "p");
  TH1D* setupc = new TH1D("setupc","", h_Rjet_HadHCALp3->GetXaxis()->GetNbins(), 28, 1240);
  //Title and axis setup
  setupc->SetStats(0); //Suppress stat box
  setupc->SetAxisRange(0.982,1.019,"Y"); //Vertical axis limits
  setupc->GetXaxis()->SetMoreLogLabels();
  setupc->GetXaxis()->SetNoExponent();
  canv_HadHCAL_c->SetLogx();
  setupc->GetYaxis()->SetTitleOffset(1.5);
  setupc->GetXaxis()->SetTitleOffset(1.1);
  setupc->GetYaxis()->SetTitle("F^{var}");
  setupc->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupc->GetYaxis()->SetTitleSize(0.05);
  setupc->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();
  setupc->Draw();
  h_Rjetc_HadHCALp3->Draw("SAMEP");
  h_Rjetc_HadHCALm3->Draw("SAMEP");
  h_Rjet_HadHCALp3->Draw("SAMEP");
  h_Rjet_HadHCALm3->Draw("SAMEP");
  lz_HadHCALc->Draw("SAMEP");

  TLatex *texc = new TLatex(); 
  texc->SetNDC();
  texc->SetTextSize(0.04); 
  texc->SetTextColor(kBlack);
  texc->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texc->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texc->DrawLatex(0.23,0.2,"#alpha<0.3");

  canv_HadHCAL_c->Print("./plots/varPlots/hadHCAL/varHCAL_c.pdf"); delete canv_HadHCAL_c;

  // s jets
  TCanvas* canv_HadHCAL_s = new TCanvas("canv_HadHCAL_s","",500,500);
  canv_HadHCAL_s->SetLeftMargin(0.15); canv_HadHCAL_s->SetBottomMargin(0.13);
  TLegend* lz_HadHCALs = new TLegend(0.18,0.68,0.6,0.86);
  lz_HadHCALs->SetBorderSize(0);
  lz_HadHCALs->AddEntry(h_Rjet_HadHCALp3, "All jets Hadron HCAL+3%", "p");
  lz_HadHCALs->AddEntry(h_Rjet_HadHCALm3, "All jets Hadron HCAL-3%", "p");
  lz_HadHCALs->AddEntry(h_Rjets_HadHCALp3, "s jets Hadron HCAL+3%", "p");
  lz_HadHCALs->AddEntry(h_Rjets_HadHCALm3, "s jets Hadron HCAL-3%", "p");
  TH1D* setups = new TH1D("setups","", h_Rjet_HadHCALp3->GetXaxis()->GetNbins(), 28, 1240);
  //Title and axis setup
  setups->SetStats(0); //Suppress stat box
  setups->SetAxisRange(0.982,1.018,"Y"); //Vertical axis limits
  setups->GetXaxis()->SetMoreLogLabels();
  setups->GetXaxis()->SetNoExponent();
  canv_HadHCAL_s->SetLogx();
  setups->GetYaxis()->SetTitleOffset(1.5);
  setups->GetXaxis()->SetTitleOffset(1.1);
  setups->GetYaxis()->SetTitle("F^{var}");
  setups->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setups->GetYaxis()->SetTitleSize(0.05);
  setups->GetXaxis()->SetTitleSize(0.05);


  gPad->SetTickx(); gPad->SetTicky();
  setups->Draw();
  lz_HadHCALs->Draw("SAMEP");
  h_Rjets_HadHCALp3->Draw("SAMEP");
  h_Rjets_HadHCALm3->Draw("SAMEP");
  h_Rjet_HadHCALp3->Draw("SAMEP");
  h_Rjet_HadHCALm3->Draw("SAMEP");

  TLatex *texs = new TLatex(); 
  texs->SetNDC();
  texs->SetTextSize(0.04); 
  texs->SetTextColor(kBlack);
  texs->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texs->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texs->DrawLatex(0.23,0.2,"#alpha<0.3");

  canv_HadHCAL_s->Print("./plots/varPlots/hadHCAL/varHCAL_s.pdf"); delete canv_HadHCAL_s;

  // ud jets
  TCanvas* canv_HadHCAL_ud = new TCanvas("canv_HadHCAL_ud","",500,500);
  canv_HadHCAL_ud->SetLeftMargin(0.15); canv_HadHCAL_ud->SetBottomMargin(0.13);
  TLegend* lz_HadHCALud = new TLegend(0.18,0.68,0.6,0.86);
  lz_HadHCALud->SetBorderSize(0);
  lz_HadHCALud->AddEntry(h_Rjet_HadHCALp3, "All jets Hadron HCAL+3%", "p");
  lz_HadHCALud->AddEntry(h_Rjet_HadHCALm3, "All jets Hadron HCAL-3%", "p");
  lz_HadHCALud->AddEntry(h_Rjetud_HadHCALp3, "ud jets Hadron HCAL+3%", "p");
  lz_HadHCALud->AddEntry(h_Rjetud_HadHCALm3, "ud jets Hadron HCAL-3%", "p");
  TH1D* setupud = new TH1D("setupud","", h_Rjet_HadHCALp3->GetXaxis()->GetNbins(), 28, 1240);
  //Title and axis setup
  setupud->SetStats(0); //Suppress stat box
  setupud->SetAxisRange(0.986,1.014,"Y"); //Vertical axis limits
  setupud->GetXaxis()->SetMoreLogLabels();
  setupud->GetXaxis()->SetNoExponent();
  canv_HadHCAL_ud->SetLogx();
  setupud->GetYaxis()->SetTitleOffset(1.5);
  setupud->GetXaxis()->SetTitleOffset(1.1);
  setupud->GetYaxis()->SetTitle("F^{var}");
  setupud->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupud->GetYaxis()->SetTitleSize(0.05);
  setupud->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();
  setupud->Draw();
  lz_HadHCALud->Draw("SAMEP");
  h_Rjetud_HadHCALp3->Draw("SAMEP");
  h_Rjetud_HadHCALm3->Draw("SAMEP");
  h_Rjet_HadHCALp3->Draw("SAMEP");
  h_Rjet_HadHCALm3->Draw("SAMEP");

  TLatex *texud = new TLatex(); 
  texud->SetNDC();
  texud->SetTextSize(0.04); 
  texud->SetTextColor(kBlack);
  texud->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texud->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texud->DrawLatex(0.23,0.2,"#alpha<0.3");

  canv_HadHCAL_ud->Print("./plots/varPlots/hadHCAL/varHCAL_ud.pdf"); delete canv_HadHCAL_ud;

  // g jets
  TCanvas* canv_HadHCAL_g = new TCanvas("canv_HadHCAL_g","",500,500);
  canv_HadHCAL_g->SetLeftMargin(0.15); canv_HadHCAL_g->SetBottomMargin(0.13);
  TLegend* lz_HadHCALg = new TLegend(0.18,0.68,0.6,0.86);
  lz_HadHCALg->SetBorderSize(0);
  lz_HadHCALg->AddEntry(h_Rjet_HadHCALp3, "All jets Hadron HCAL+3%", "p");
  lz_HadHCALg->AddEntry(h_Rjet_HadHCALm3, "All jets Hadron HCAL-3%", "p");
  lz_HadHCALg->AddEntry(h_Rjetg_HadHCALp3, "g jets Hadron HCAL+3%", "p");
  lz_HadHCALg->AddEntry(h_Rjetg_HadHCALm3, "g jets Hadron HCAL-3%", "p");
  TH1D* setupg = new TH1D("setupg","", h_Rjet_HadHCALp3->GetXaxis()->GetNbins(), 28, 1240);
  //Title and axis setup
  setupg->SetStats(0); //Suppress stat box
  setupg->SetAxisRange(0.987,1.013,"Y"); //Vertical axis limits
  setupg->GetXaxis()->SetMoreLogLabels();
  setupg->GetXaxis()->SetNoExponent();
  canv_HadHCAL_g->SetLogx();
  setupg->GetYaxis()->SetTitleOffset(1.5);
  setupg->GetXaxis()->SetTitleOffset(1.1);
  setupg->GetYaxis()->SetTitle("F^{var}");
  setupg->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupg->GetYaxis()->SetTitleSize(0.05);
  setupg->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();
  setupg->Draw();
  lz_HadHCALg->Draw("SAMEP");
  h_Rjetg_HadHCALp3->Draw("SAMEP");
  h_Rjetg_HadHCALm3->Draw("SAMEP");
  h_Rjet_HadHCALp3->Draw("SAMEP");
  h_Rjet_HadHCALm3->Draw("SAMEP");

  TLatex *texg = new TLatex(); 
  texg->SetNDC();
  texg->SetTextSize(0.04); 
  texg->SetTextColor(kBlack);
  texg->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texg->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texg->DrawLatex(0.23,0.2,"#alpha<0.3");

  canv_HadHCAL_g->Print("./plots/varPlots/hadHCAL/varHCAL_g.pdf"); delete canv_HadHCAL_g;


  gApplication->Terminate(0); //Exit root command prompt

}
