void plotVariants()
{

  double markersize = 0.8;

  //Variants file
  TFile* fzj = TFile::Open("./output_ROOT_files/varPlots_5000000.root");

  TH1D* h_Rjet_Cp3;
  TH1D* h_Rjet_Cm3;
  TH1D* h_Rjet_Trk;
  TH1D* h_Rjet_Photon;

  TH1D* h_Rjetb_Cp3;
  TH1D* h_Rjetb_Cm3;
  TH1D* h_Rjetb_Trk;
  TH1D* h_Rjetb_Photon;

  TH1D* h_Rjetc_Cp3;
  TH1D* h_Rjetc_Cm3;
  TH1D* h_Rjetc_Trk;
  TH1D* h_Rjetc_Photon;

  TH1D* h_Rjets_Cp3;
  TH1D* h_Rjets_Cm3;
  TH1D* h_Rjets_Trk;
  TH1D* h_Rjets_Photon;

  TH1D* h_Rjetud_Cp3;
  TH1D* h_Rjetud_Cm3;
  TH1D* h_Rjetud_Trk;
  TH1D* h_Rjetud_Photon;

  TH1D* h_Rjetg_Cp3;
  TH1D* h_Rjetg_Cm3;
  TH1D* h_Rjetg_Trk;
  TH1D* h_Rjetg_Photon;


  //Create Histograms
  fzj->GetObject("h_Rjet_Cp3", h_Rjet_Cp3);
  fzj->GetObject("h_Rjet_Cm3", h_Rjet_Cm3);
  fzj->GetObject("h_Rjet_Trk", h_Rjet_Trk);
  fzj->GetObject("h_Rjet_Photon", h_Rjet_Photon);

  fzj->GetObject("h_Rjetb_Cp3", h_Rjetb_Cp3);
  fzj->GetObject("h_Rjetb_Cm3", h_Rjetb_Cm3);
  fzj->GetObject("h_Rjetb_Trk", h_Rjetb_Trk);
  fzj->GetObject("h_Rjetb_Photon", h_Rjetb_Photon);

  fzj->GetObject("h_Rjetc_Cp3", h_Rjetc_Cp3);
  fzj->GetObject("h_Rjetc_Cm3", h_Rjetc_Cm3);
  fzj->GetObject("h_Rjetc_Trk", h_Rjetc_Trk);
  fzj->GetObject("h_Rjetc_Photon", h_Rjetc_Photon);

  fzj->GetObject("h_Rjets_Cp3", h_Rjets_Cp3);
  fzj->GetObject("h_Rjets_Cm3", h_Rjets_Cm3);
  fzj->GetObject("h_Rjets_Trk", h_Rjets_Trk);
  fzj->GetObject("h_Rjets_Photon", h_Rjets_Photon);

  fzj->GetObject("h_Rjetud_Cp3", h_Rjetud_Cp3);
  fzj->GetObject("h_Rjetud_Cm3", h_Rjetud_Cm3);
  fzj->GetObject("h_Rjetud_Trk", h_Rjetud_Trk);
  fzj->GetObject("h_Rjetud_Photon", h_Rjetud_Photon);

  fzj->GetObject("h_Rjetg_Cp3", h_Rjetg_Cp3);
  fzj->GetObject("h_Rjetg_Cm3", h_Rjetg_Cm3);
  fzj->GetObject("h_Rjetg_Trk", h_Rjetg_Trk);
  fzj->GetObject("h_Rjetg_Photon", h_Rjetg_Photon);

// C +-3%

  TCanvas* canv_C_all = new TCanvas("canv_C_all","",500,400);
  canv_C_all->SetLeftMargin(0.15);
  canv_C_all->SetBottomMargin(0.13);

  h_Rjet_Cp3->SetMarkerStyle(2);             h_Rjet_Cm3->SetMarkerStyle(5);
  h_Rjet_Cp3->SetMarkerColor(kBlack);        h_Rjet_Cm3->SetMarkerColor(kBlack);
  h_Rjet_Cp3->SetLineColor(kBlack);          h_Rjet_Cm3->SetLineColor(kBlack);
  h_Rjet_Cp3->SetMarkerSize(markersize);     h_Rjet_Cm3->SetMarkerSize(markersize);

  h_Rjetb_Cp3->SetMarkerStyle(kFullCircle);  h_Rjetb_Cm3->SetMarkerStyle(kOpenCircle);
  h_Rjetb_Cp3->SetMarkerColor(kRed+1);       h_Rjetb_Cm3->SetMarkerColor(kRed+1);
  h_Rjetb_Cp3->SetLineColor(kRed+1);         h_Rjetb_Cm3->SetLineColor(kRed+1);
  h_Rjetb_Cp3->SetMarkerSize(markersize);    h_Rjetb_Cm3->SetMarkerSize(markersize);

  h_Rjetc_Cp3->SetMarkerStyle(kFullTriangleDown); h_Rjetc_Cm3->SetMarkerStyle(kOpenTriangleDown); 
  h_Rjetc_Cp3->SetMarkerColor(kGreen+3);          h_Rjetc_Cm3->SetMarkerColor(kGreen+3);
  h_Rjetc_Cp3->SetLineColor(kGreen+3);            h_Rjetc_Cm3->SetLineColor(kGreen+3);
  h_Rjetc_Cp3->SetMarkerSize(markersize);         h_Rjetc_Cm3->SetMarkerSize(markersize);

  h_Rjets_Cp3->SetMarkerStyle(kFullTriangleUp); h_Rjets_Cm3->SetMarkerStyle(kOpenTriangleUp);
  h_Rjets_Cp3->SetMarkerColor(kOrange+7);       h_Rjets_Cm3->SetMarkerColor(kOrange+7);
  h_Rjets_Cp3->SetLineColor(kOrange+7);         h_Rjets_Cm3->SetLineColor(kOrange+7);
  h_Rjets_Cp3->SetMarkerSize(markersize);       h_Rjets_Cm3->SetMarkerSize(markersize);

  h_Rjetud_Cp3->SetMarkerStyle(kFullDiamond); h_Rjetud_Cm3->SetMarkerStyle(kOpenDiamond);
  h_Rjetud_Cp3->SetMarkerColor(kMagenta+2);   h_Rjetud_Cm3->SetMarkerColor(kMagenta+2);
  h_Rjetud_Cp3->SetLineColor(kMagenta+2);     h_Rjetud_Cm3->SetLineColor(kMagenta+2);
  h_Rjetud_Cp3->SetMarkerSize(markersize);    h_Rjetud_Cm3->SetMarkerSize(markersize);

  h_Rjetg_Cp3->SetMarkerStyle(kFullSquare); h_Rjetg_Cm3->SetMarkerStyle(kOpenSquare);
  h_Rjetg_Cp3->SetMarkerColor(kBlue+1);     h_Rjetg_Cm3->SetMarkerColor(kBlue+1);
  h_Rjetg_Cp3->SetLineColor(kBlue+1);       h_Rjetg_Cm3->SetLineColor(kBlue+1);
  h_Rjetg_Cp3->SetMarkerSize(markersize);   h_Rjetg_Cm3->SetMarkerSize(markersize);

  //Legend
  TLegend* lz_C = new TLegend(0.2,0.65,0.55,0.87);
  lz_C->SetBorderSize(0);
  lz_C->AddEntry(h_Rjet_Cp3, "All jets C+3%", "p");
  lz_C->AddEntry(h_Rjet_Cm3, "All jets C-3%", "p");
  lz_C->AddEntry(h_Rjetb_Cp3, "b jets C+3%", "p");
  lz_C->AddEntry(h_Rjetb_Cm3, "b jets C-3%", "p");
  lz_C->AddEntry(h_Rjetc_Cp3, "c jets C+3%", "p");
  lz_C->AddEntry(h_Rjetc_Cm3, "c jets C-3%", "p");
  lz_C->AddEntry(h_Rjets_Cp3, "s jets C+3%", "p");
  lz_C->AddEntry(h_Rjets_Cm3, "s jets C-3%", "p");
  lz_C->AddEntry(h_Rjetud_Cp3, "ud jets C+3%", "p");
  lz_C->AddEntry(h_Rjetud_Cm3, "ud jets C-3%", "p");
  lz_C->AddEntry(h_Rjetg_Cp3, "g jets C+3%", "p");
  lz_C->AddEntry(h_Rjetg_Cm3, "g jets C-3%", "p");
  lz_C->SetNColumns(2);
  TH1D* setup = new TH1D("setup","", h_Rjet_Cp3->GetXaxis()->GetNbins(),
			 h_Rjet_Cp3->GetXaxis()->GetXmin(), h_Rjet_Cp3->GetXaxis()->GetXmax());


  //Title and axis setup
  setup->SetStats(0); //Suppress stat box
  setup->SetAxisRange(0.978,1.028,"Y"); //Vertical axis limits
  setup->GetXaxis()->SetMoreLogLabels();
  setup->GetXaxis()->SetNoExponent();
  canv_C_all->SetLogx();
  setup->GetYaxis()->SetTitleOffset(1.1);
  setup->GetXaxis()->SetTitleOffset(1.1);
  setup->GetYaxis()->SetTitle("F^{var}");
  setup->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup->GetYaxis()->SetTitleSize(0.05);
  setup->GetXaxis()->SetTitleSize(0.05);



  gPad->SetTickx(); gPad->SetTicky();

  //Plot
  setup->Draw();
  h_Rjetb_Cm3->Draw("SAMEP");
  h_Rjetb_Cp3->Draw("SAMEP");
  h_Rjetc_Cm3->Draw("SAMEP");
  h_Rjetc_Cp3->Draw("SAMEP");
  h_Rjets_Cm3->Draw("SAMEP");
  h_Rjets_Cp3->Draw("SAMEP");
  h_Rjetud_Cm3->Draw("SAMEP");
  h_Rjetud_Cp3->Draw("SAMEP");
  h_Rjetg_Cm3->Draw("SAMEP");
  h_Rjetg_Cp3->Draw("SAMEP");
  h_Rjet_Cm3->Draw("SAMEP");
  h_Rjet_Cp3->Draw("SAMEP");
  lz_C->Draw("SAMEP");
  //Save plot
  canv_C_all->Print("./plots/varPlots/varC_all.pdf");
  delete canv_C_all;

  // b jets
  TCanvas* canv_C_b = new TCanvas("canv_C_b","",500,400);
  canv_C_b->SetLeftMargin(0.15); canv_C_b->SetBottomMargin(0.13);

  TLegend* lz_Cb = new TLegend(0.2,0.68,0.5,0.86);
  lz_Cb->SetBorderSize(0);
  lz_Cb->AddEntry(h_Rjet_Cp3, "All jets C+3%", "p");
  lz_Cb->AddEntry(h_Rjet_Cm3, "All jets C-3%", "p");
  lz_Cb->AddEntry(h_Rjetb_Cp3, "b jets C+3%", "p");
  lz_Cb->AddEntry(h_Rjetb_Cm3, "b jets C-3%", "p");
  TH1D* setupb = new TH1D("setupb","", h_Rjet_Cp3->GetXaxis()->GetNbins(),
			 h_Rjet_Cp3->GetXaxis()->GetXmin(), h_Rjet_Cp3->GetXaxis()->GetXmax());
  //Title and axis setup
  setupb->SetStats(0); //Suppress stat box
  setupb->SetAxisRange(0.98,1.02,"Y"); //Vertical axis limits
  setupb->GetXaxis()->SetMoreLogLabels();
  setupb->GetXaxis()->SetNoExponent();
  canv_C_b->SetLogx();
  setupb->GetYaxis()->SetTitleOffset(1.1);
  setupb->GetXaxis()->SetTitleOffset(1.1);
  setupb->GetYaxis()->SetTitle("F^{var}");
  setupb->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupb->GetYaxis()->SetTitleSize(0.05);
  setupb->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();
  //Plot
  setupb->Draw();
  h_Rjetb_Cm3->Draw("SAMEP");
  h_Rjetb_Cp3->Draw("SAMEP");
  h_Rjet_Cm3->Draw("SAMEP");
  h_Rjet_Cp3->Draw("SAMEP");
  lz_Cb->Draw("SAMEP");
  //Save plot
  canv_C_b->Print("./plots/varPlots/varC_b.pdf"); delete canv_C_b;

  // c jets
  TCanvas* canv_C_c = new TCanvas("canv_C_c","",500,400);
  canv_C_c->SetLeftMargin(0.15); canv_C_c->SetBottomMargin(0.13);
  TLegend* lz_Cc = new TLegend(0.2,0.68,0.5,0.86);
  lz_Cc->SetBorderSize(0);
  lz_Cc->AddEntry(h_Rjet_Cp3, "All jets C+3%", "p");
  lz_Cc->AddEntry(h_Rjet_Cm3, "All jets C-3%", "p");
  lz_Cc->AddEntry(h_Rjetc_Cp3, "c jets C+3%", "p");
  lz_Cc->AddEntry(h_Rjetc_Cm3, "c jets C-3%", "p");
  TH1D* setupc = new TH1D("setupc","", h_Rjet_Cp3->GetXaxis()->GetNbins(),
			 h_Rjet_Cp3->GetXaxis()->GetXmin(), h_Rjet_Cp3->GetXaxis()->GetXmax());
  //Title and axis setup
  setupc->SetStats(0); //Suppress stat box
  setupc->SetAxisRange(0.978,1.022,"Y"); //Vertical axis limits
  setupc->GetXaxis()->SetMoreLogLabels();
  setupc->GetXaxis()->SetNoExponent();
  canv_C_c->SetLogx();
  setupc->GetYaxis()->SetTitleOffset(1.1);
  setupc->GetXaxis()->SetTitleOffset(1.1);
  setupc->GetYaxis()->SetTitle("F^{var}");
  setupc->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupc->GetYaxis()->SetTitleSize(0.05);
  setupc->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();
  setupc->Draw();
  h_Rjetc_Cm3->Draw("SAMEP");
  h_Rjetc_Cp3->Draw("SAMEP");
  h_Rjet_Cm3->Draw("SAMEP");
  h_Rjet_Cp3->Draw("SAMEP");
  lz_Cc->Draw("SAMEP");
  canv_C_c->Print("./plots/varPlots/varC_c.pdf"); delete canv_C_c;

  // s jets
  TCanvas* canv_C_s = new TCanvas("canv_C_s","",500,400);
  canv_C_s->SetLeftMargin(0.15); canv_C_s->SetBottomMargin(0.13);
  TLegend* lz_Cs = new TLegend(0.2,0.68,0.5,0.86);
  lz_Cs->SetBorderSize(0);
  lz_Cs->AddEntry(h_Rjet_Cp3, "All jets C+3%", "p");
  lz_Cs->AddEntry(h_Rjet_Cm3, "All jets C-3%", "p");
  lz_Cs->AddEntry(h_Rjets_Cp3, "s jets C+3%", "p");
  lz_Cs->AddEntry(h_Rjets_Cm3, "s jets C-3%", "p");
  TH1D* setups = new TH1D("setups","", h_Rjet_Cp3->GetXaxis()->GetNbins(),
			 h_Rjet_Cp3->GetXaxis()->GetXmin(), h_Rjet_Cp3->GetXaxis()->GetXmax());
  //Title and axis setup
  setups->SetStats(0); //Suppress stat box
  setups->SetAxisRange(0.978,1.022,"Y"); //Vertical axis limits
  setups->GetXaxis()->SetMoreLogLabels();
  setups->GetXaxis()->SetNoExponent();
  canv_C_s->SetLogx();
  setups->GetYaxis()->SetTitleOffset(1.1);
  setups->GetXaxis()->SetTitleOffset(1.1);
  setups->GetYaxis()->SetTitle("F^{var}");
  setups->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setups->GetYaxis()->SetTitleSize(0.05);
  setups->GetXaxis()->SetTitleSize(0.05);


  gPad->SetTickx(); gPad->SetTicky();
  setups->Draw();
  lz_Cs->Draw("SAMEP");
  h_Rjets_Cm3->Draw("SAMEP");
  h_Rjets_Cp3->Draw("SAMEP");
  h_Rjet_Cm3->Draw("SAMEP");
  h_Rjet_Cp3->Draw("SAMEP");

  canv_C_s->Print("./plots/varPlots/varC_s.pdf"); delete canv_C_s;

  // ud jets
  TCanvas* canv_C_ud = new TCanvas("canv_C_ud","",500,400);
  canv_C_ud->SetLeftMargin(0.15); canv_C_ud->SetBottomMargin(0.13);
  TLegend* lz_Cud = new TLegend(0.2,0.68,0.5,0.86);
  lz_Cud->SetBorderSize(0);
  lz_Cud->AddEntry(h_Rjet_Cp3, "All jets C+3%", "p");
  lz_Cud->AddEntry(h_Rjet_Cm3, "All jets C-3%", "p");
  lz_Cud->AddEntry(h_Rjetud_Cp3, "ud jets C+3%", "p");
  lz_Cud->AddEntry(h_Rjetud_Cm3, "ud jets C-3%", "p");
  TH1D* setupud = new TH1D("setupud","", h_Rjet_Cp3->GetXaxis()->GetNbins(),
			 h_Rjet_Cp3->GetXaxis()->GetXmin(), h_Rjet_Cp3->GetXaxis()->GetXmax());
  //Title and axis setup
  setupud->SetStats(0); //Suppress stat box
  setupud->SetAxisRange(0.978,1.022,"Y"); //Vertical axis limits
  setupud->GetXaxis()->SetMoreLogLabels();
  setupud->GetXaxis()->SetNoExponent();
  canv_C_ud->SetLogx();
  setupud->GetYaxis()->SetTitleOffset(1.1);
  setupud->GetXaxis()->SetTitleOffset(1.1);
  setupud->GetYaxis()->SetTitle("F^{var}");
  setupud->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupud->GetYaxis()->SetTitleSize(0.05);
  setupud->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();
  setupud->Draw();
  lz_Cud->Draw("SAMEP");
  h_Rjetud_Cm3->Draw("SAMEP");
  h_Rjetud_Cp3->Draw("SAMEP");
  h_Rjet_Cm3->Draw("SAMEP");
  h_Rjet_Cp3->Draw("SAMEP");

  canv_C_ud->Print("./plots/varPlots/varC_ud.pdf"); delete canv_C_ud;

  // g jets
  TCanvas* canv_C_g = new TCanvas("canv_C_g","",500,400);
  canv_C_g->SetLeftMargin(0.15); canv_C_g->SetBottomMargin(0.13);
  TLegend* lz_Cg = new TLegend(0.2,0.68,0.5,0.86);
  lz_Cg->SetBorderSize(0);
  lz_Cg->AddEntry(h_Rjet_Cp3, "All jets C+3%", "p");
  lz_Cg->AddEntry(h_Rjet_Cm3, "All jets C-3%", "p");
  lz_Cg->AddEntry(h_Rjetg_Cp3, "g jets C+3%", "p");
  lz_Cg->AddEntry(h_Rjetg_Cm3, "g jets C-3%", "p");
  TH1D* setupg = new TH1D("setupg","", h_Rjet_Cp3->GetXaxis()->GetNbins(),
			 h_Rjet_Cp3->GetXaxis()->GetXmin(), h_Rjet_Cp3->GetXaxis()->GetXmax());
  //Title and axis setup
  setupg->SetStats(0); //Suppress stat box
  setupg->SetAxisRange(0.978,1.022,"Y"); //Vertical axis limits
  setupg->GetXaxis()->SetMoreLogLabels();
  setupg->GetXaxis()->SetNoExponent();
  canv_C_g->SetLogx();
  setupg->GetYaxis()->SetTitleOffset(1.1);
  setupg->GetXaxis()->SetTitleOffset(1.1);
  setupg->GetYaxis()->SetTitle("F^{var}");
  setupg->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupg->GetYaxis()->SetTitleSize(0.05);
  setupg->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();
  setupg->Draw();
  lz_Cg->Draw("SAMEP");
  h_Rjetg_Cm3->Draw("SAMEP");
  h_Rjetg_Cp3->Draw("SAMEP");
  h_Rjet_Cm3->Draw("SAMEP");
  h_Rjet_Cp3->Draw("SAMEP");

  canv_C_g->Print("./plots/varPlots/varC_g.pdf"); delete canv_C_g;


//////////////////// Tracking efficiency /////////////////////////
  TCanvas* canv_trk = new TCanvas("canv_trk","",500,400);
  canv_trk->SetLeftMargin(0.15); canv_trk->SetBottomMargin(0.13);

  h_Rjet_Trk->SetMarkerStyle(5);                  h_Rjet_Trk->SetMarkerColor(kBlack);
  h_Rjet_Trk->SetLineColor(kBlack);               h_Rjet_Trk->SetMarkerSize(0.8);
  h_Rjetb_Trk->SetMarkerStyle(kOpenCircle);       h_Rjetb_Trk->SetMarkerColor(kRed+1);
  h_Rjetb_Trk->SetLineColor(kRed+1);              h_Rjetb_Trk->SetMarkerSize(0.8);
  h_Rjetc_Trk->SetMarkerStyle(kOpenTriangleDown); h_Rjetc_Trk->SetMarkerColor(kGreen+3);
  h_Rjetc_Trk->SetLineColor(kGreen+3);            h_Rjetc_Trk->SetMarkerSize(0.8);
  h_Rjets_Trk->SetMarkerStyle(kOpenTriangleUp);   
  h_Rjets_Trk->SetMarkerColor(kOrange+7);
  h_Rjets_Trk->SetLineColor(kOrange+7);           h_Rjets_Trk->SetMarkerSize(0.8);
  h_Rjetud_Trk->SetMarkerStyle(kOpenDiamond);     h_Rjetud_Trk->SetMarkerColor(kMagenta+2);
  h_Rjetud_Trk->SetLineColor(kMagenta+2);         h_Rjetud_Trk->SetMarkerSize(0.8);
  h_Rjetg_Trk->SetMarkerStyle(kOpenSquare);       h_Rjetg_Trk->SetMarkerColor(kBlue+1);
  h_Rjetg_Trk->SetLineColor(kBlue+1);             h_Rjetg_Trk->SetMarkerSize(0.8);

  //Legend
  TLegend* lz_Rjet_trk = new TLegend(0.3,0.72,0.6,0.85);
  lz_Rjet_trk->SetBorderSize(0);
  lz_Rjet_trk->AddEntry(h_Rjet_Trk,  "All jets", "p");
  lz_Rjet_trk->AddEntry(h_Rjetb_Trk, "b jets", "p");
  lz_Rjet_trk->AddEntry(h_Rjetc_Trk, "c jets", "p");
  lz_Rjet_trk->AddEntry(h_Rjets_Trk, "s jets", "p");
  lz_Rjet_trk->AddEntry(h_Rjetud_Trk, "ud jets", "p");
  lz_Rjet_trk->AddEntry(h_Rjetg_Trk,  "g jets", "p");
  lz_Rjet_trk->SetNColumns(2);

  TH1D* setup_trk = new TH1D("setup_trk","", h_Rjet_Trk->GetXaxis()->GetNbins(),
		                             h_Rjet_Trk->GetXaxis()->GetXmin(), 
                                             h_Rjet_Trk->GetXaxis()->GetXmax());
  setup_trk->SetStats(0); //Suppress stat box
  setup_trk->SetAxisRange(0.9938,1.0035,"Y"); //Vertical axis limits
  setup_trk->GetXaxis()->SetMoreLogLabels();
  setup_trk->GetXaxis()->SetNoExponent();
  canv_trk->SetLogx();
  setup_trk->GetYaxis()->SetTitleOffset(1.1); 
  setup_trk->GetXaxis()->SetTitleOffset(1.1);
  setup_trk->GetYaxis()->SetTitle("F^{var}");
  setup_trk->GetYaxis()->SetTitleSize(0.05);
  setup_trk->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_trk->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  TLine *line = new TLine(31.75,1,1258.25,1); 
  //Plot
  setup_trk->Draw();
  line->Draw("SAME");
  h_Rjetb_Trk->Draw("SAMEP");
  h_Rjetc_Trk->Draw("SAMEP");
  h_Rjets_Trk->Draw("SAMEP");
  h_Rjetud_Trk->Draw("SAMEP");
  h_Rjetg_Trk->Draw("SAMEP");
  h_Rjet_Trk->Draw("SAMEP");
  lz_Rjet_trk->Draw("SAMEP");

  //Save plot
  canv_trk->Print("./plots/varPlots/varTrk_all.pdf"); delete canv_trk;

  //b jet
  TCanvas* canv_trkb = new TCanvas("canv_trkb","",500,400); canv_trk->SetLogx();
  canv_trkb->SetLeftMargin(0.15); canv_trkb->SetBottomMargin(0.13);
  TLegend* lz_trkb = new TLegend(0.3,0.75,0.6,0.85); lz_trkb->SetBorderSize(0);
  lz_trkb->AddEntry(h_Rjet_Trk,  "All jets", "p");
  lz_trkb->AddEntry(h_Rjetb_Trk, "b jets", "p");
  TH1D* setup_trkb = new TH1D("setup_trkb","", h_Rjet_Trk->GetXaxis()->GetNbins(),
		                             h_Rjet_Trk->GetXaxis()->GetXmin(), 
                                             h_Rjet_Trk->GetXaxis()->GetXmax());
  setup_trkb->SetStats(0);
  setup_trkb->SetAxisRange(0.9948,1.0033,"Y");
  setup_trkb->GetXaxis()->SetMoreLogLabels();  setup_trkb->GetXaxis()->SetNoExponent();
  setup_trkb->GetYaxis()->SetTitleOffset(1.1); 
  setup_trkb->GetXaxis()->SetTitleOffset(1.1);
  setup_trkb->GetYaxis()->SetTitle("F^{var}");
  setup_trkb->GetYaxis()->SetTitleSize(0.05);
  setup_trkb->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_trkb->GetXaxis()->SetTitleSize(0.05);

  setup_trkb->Draw();
  line->Draw("SAME");
  h_Rjetb_Trk->Draw("SAMEP");
  h_Rjet_Trk->Draw("SAMEP");
  lz_trkb->Draw("SAMEP");
  canv_trkb->Print("./plots/varPlots/varTrk_b.pdf"); delete canv_trkb;

  //c jet
  TCanvas* canv_trkc = new TCanvas("canv_trkc","",500,400); canv_trkc->SetLogx();
  canv_trkc->SetLeftMargin(0.15); canv_trkc->SetBottomMargin(0.13);
  TLegend* lz_trkc = new TLegend(0.3,0.75,0.6,0.85); lz_trkc->SetBorderSize(0);
  lz_trkc->AddEntry(h_Rjet_Trk,  "All jets", "p");
  lz_trkc->AddEntry(h_Rjetc_Trk, "c jets", "p");
  TH1D* setup_trkc = new TH1D("setup_trkc","", h_Rjet_Trk->GetXaxis()->GetNbins(),
		                             h_Rjet_Trk->GetXaxis()->GetXmin(), 
                                             h_Rjet_Trk->GetXaxis()->GetXmax());
  setup_trkc->SetStats(0);
  setup_trkc->SetAxisRange(0.9938,1.0035,"Y");
  setup_trkc->GetXaxis()->SetMoreLogLabels();  setup_trkc->GetXaxis()->SetNoExponent();
  setup_trkc->GetYaxis()->SetTitleOffset(1.1); 
  setup_trkc->GetXaxis()->SetTitleOffset(1.1);
  setup_trkc->GetYaxis()->SetTitle("F^{var}");
  setup_trkc->GetYaxis()->SetTitleSize(0.05);
  setup_trkc->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_trkc->GetXaxis()->SetTitleSize(0.05);

  setup_trkc->Draw();
  line->Draw("SAME");
  h_Rjetc_Trk->Draw("SAMEP");
  h_Rjet_Trk->Draw("SAMEP");
  lz_trkc->Draw("SAMEP");
  canv_trkc->Print("./plots/varPlots/varTrk_c.pdf"); delete canv_trkc;

  //s jet
  TCanvas* canv_trks = new TCanvas("canv_trks","",500,400); canv_trks->SetLogx();
  canv_trks->SetLeftMargin(0.15); canv_trks->SetBottomMargin(0.13);
  TLegend* lz_trks = new TLegend(0.3,0.75,0.6,0.85); lz_trks->SetBorderSize(0);
  lz_trks->AddEntry(h_Rjet_Trk,  "All jets", "p");
  lz_trks->AddEntry(h_Rjets_Trk, "s jets", "p");
  TH1D* setup_trks = new TH1D("setup_trks","", h_Rjet_Trk->GetXaxis()->GetNbins(),
		                             h_Rjet_Trk->GetXaxis()->GetXmin(), 
                                             h_Rjet_Trk->GetXaxis()->GetXmax());
  setup_trks->SetStats(0);
  setup_trks->SetAxisRange(0.9938,1.0035,"Y");
  setup_trks->GetXaxis()->SetMoreLogLabels();  setup_trks->GetXaxis()->SetNoExponent();
  setup_trks->GetYaxis()->SetTitleOffset(1.1); 
  setup_trks->GetXaxis()->SetTitleOffset(1.1);
  setup_trks->GetYaxis()->SetTitle("F^{var}");
  setup_trks->GetYaxis()->SetTitleSize(0.05);
  setup_trks->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_trks->GetXaxis()->SetTitleSize(0.05);

  setup_trks->Draw();
  line->Draw("SAME");
  h_Rjets_Trk->Draw("SAMEP");
  h_Rjet_Trk->Draw("SAMEP");
  lz_trks->Draw("SAMEP");
  canv_trks->Print("./plots/varPlots/varTrk_s.pdf"); delete canv_trks;

  //ud jet
  TCanvas* canv_trkud = new TCanvas("canv_trkud","",500,400); canv_trkud->SetLogx();
  canv_trkud->SetLeftMargin(0.15); canv_trkud->SetBottomMargin(0.13);
  TLegend* lz_trkud = new TLegend(0.3,0.75,0.6,0.85); lz_trkud->SetBorderSize(0);
  lz_trkud->AddEntry(h_Rjet_Trk,  "All jets", "p");
  lz_trkud->AddEntry(h_Rjetud_Trk,"ud jets", "p");
  TH1D* setup_trkud = new TH1D("setup_trkud","", h_Rjet_Trk->GetXaxis()->GetNbins(),
		                                 h_Rjet_Trk->GetXaxis()->GetXmin(), 
                                                 h_Rjet_Trk->GetXaxis()->GetXmax());
  setup_trkud->SetStats(0);
  setup_trkud->SetAxisRange(0.9938,1.0035,"Y");
  setup_trkud->GetXaxis()->SetMoreLogLabels();  setup_trkud->GetXaxis()->SetNoExponent();
  setup_trkud->GetYaxis()->SetTitleOffset(1.1); 
  setup_trkud->GetXaxis()->SetTitleOffset(1.1);
  setup_trkud->GetYaxis()->SetTitle("F^{var}");
  setup_trkud->GetYaxis()->SetTitleSize(0.05);
  setup_trkud->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_trkud->GetXaxis()->SetTitleSize(0.05);

  setup_trkud->Draw();
  line->Draw("SAME");
  h_Rjetud_Trk->Draw("SAMEP");
  h_Rjet_Trk->Draw("SAMEP");
  lz_trkud->Draw("SAMEP");
  canv_trkud->Print("./plots/varPlots/varTrk_ud.pdf"); delete canv_trkud;

  //g jet
  TCanvas* canv_trkg = new TCanvas("canv_trkg","",500,400); canv_trkg->SetLogx();
  canv_trkg->SetLeftMargin(0.15); canv_trkg->SetBottomMargin(0.13);
  TLegend* lz_trkg = new TLegend(0.3,0.75,0.6,0.85); lz_trkg->SetBorderSize(0);
  lz_trkg->AddEntry(h_Rjet_Trk,  "All jets", "p");
  lz_trkg->AddEntry(h_Rjetg_Trk,"g jets", "p");
  TH1D* setup_trkg = new TH1D("setup_trkg","", h_Rjet_Trk->GetXaxis()->GetNbins(),
		                               h_Rjet_Trk->GetXaxis()->GetXmin(), 
                                               h_Rjet_Trk->GetXaxis()->GetXmax());
  setup_trkg->SetStats(0);
  setup_trkg->SetAxisRange(0.9938,1.0035,"Y");
  setup_trkg->GetXaxis()->SetMoreLogLabels();  setup_trkg->GetXaxis()->SetNoExponent();
  setup_trkg->GetYaxis()->SetTitleOffset(1.1); 
  setup_trkg->GetXaxis()->SetTitleOffset(1.1);
  setup_trkg->GetYaxis()->SetTitle("F^{var}");
  setup_trkg->GetYaxis()->SetTitleSize(0.05);
  setup_trkg->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_trkg->GetXaxis()->SetTitleSize(0.05);

  setup_trkg->Draw();
  line->Draw("SAME");
  h_Rjetg_Trk->Draw("SAMEP");
  h_Rjet_Trk->Draw("SAMEP");
  lz_trkg->Draw("SAMEP");
  canv_trkg->Print("./plots/varPlots/varTrk_g.pdf"); delete canv_trkg;



//////////////////// Photon scale variation /////////////////////////
  TCanvas* canv_Photon = new TCanvas("canv_Photon","",500,400);
  canv_Photon->SetLeftMargin(0.15); canv_Photon->SetBottomMargin(0.13);

  h_Rjet_Photon->SetMarkerStyle(5);                  h_Rjet_Photon->SetMarkerColor(kBlack);
  h_Rjet_Photon->SetLineColor(kBlack);               h_Rjet_Photon->SetMarkerSize(0.8);
  h_Rjetb_Photon->SetMarkerStyle(kOpenCircle);       h_Rjetb_Photon->SetMarkerColor(kRed+1);
  h_Rjetb_Photon->SetLineColor(kRed+1);              h_Rjetb_Photon->SetMarkerSize(0.8);
  h_Rjetc_Photon->SetMarkerStyle(kOpenTriangleDown); h_Rjetc_Photon->SetMarkerColor(kGreen+3);
  h_Rjetc_Photon->SetLineColor(kGreen+3);            h_Rjetc_Photon->SetMarkerSize(0.8);
  h_Rjets_Photon->SetMarkerStyle(kOpenTriangleUp);   h_Rjets_Photon->SetMarkerColor(kOrange+7);
  h_Rjets_Photon->SetLineColor(kOrange+7);           h_Rjets_Photon->SetMarkerSize(0.8);
  h_Rjetud_Photon->SetMarkerStyle(kOpenDiamond);     h_Rjetud_Photon->SetMarkerColor(kMagenta+2);
  h_Rjetud_Photon->SetLineColor(kMagenta+2);         h_Rjetud_Photon->SetMarkerSize(0.8);
  h_Rjetg_Photon->SetMarkerStyle(kOpenSquare);       h_Rjetg_Photon->SetMarkerColor(kBlue+1);
  h_Rjetg_Photon->SetLineColor(kBlue+1);             h_Rjetg_Photon->SetMarkerSize(0.8);

  //Legend
  TLegend* lz_Rjet_Photon = new TLegend(0.35,0.18,0.55,0.38);
  lz_Rjet_Photon->SetBorderSize(0);
  lz_Rjet_Photon->AddEntry(h_Rjet_Photon,  "All jets", "p");
  lz_Rjet_Photon->AddEntry(h_Rjetb_Photon, "b jets", "p");
  lz_Rjet_Photon->AddEntry(h_Rjetc_Photon, "c jets", "p");
  lz_Rjet_Photon->AddEntry(h_Rjets_Photon, "s jets", "p");
  lz_Rjet_Photon->AddEntry(h_Rjetud_Photon, "ud jets", "p");
  lz_Rjet_Photon->AddEntry(h_Rjetg_Photon,  "g jets", "p");
  //lz_Rjet_Photon->SetNColumns(2);

  TH1D* setup_Photon = new TH1D("setup_Photon","", h_Rjet_Photon->GetXaxis()->GetNbins(),
		                             h_Rjet_Photon->GetXaxis()->GetXmin(), 
                                             h_Rjet_Photon->GetXaxis()->GetXmax());
  setup_Photon->SetStats(0);
  setup_Photon->SetAxisRange(0.993,1.002,"Y");
  setup_Photon->GetXaxis()->SetMoreLogLabels();
  setup_Photon->GetXaxis()->SetNoExponent();
  canv_Photon->SetLogx();
  setup_Photon->GetYaxis()->SetTitleOffset(1.1); 
  setup_Photon->GetXaxis()->SetTitleOffset(1.1);
  setup_Photon->GetYaxis()->SetTitle("F^{var}");
  setup_Photon->GetYaxis()->SetTitleSize(0.05);
  setup_Photon->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photon->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  TLine *line_Photon = new TLine(31.75,1,1258.25,1); 
  //Plot
  setup_Photon->Draw();
  line_Photon->Draw("SAME");
  h_Rjetb_Photon->Draw("SAMEP");
  h_Rjetc_Photon->Draw("SAMEP");
  h_Rjets_Photon->Draw("SAMEP");
  h_Rjetud_Photon->Draw("SAMEP");
  h_Rjetg_Photon->Draw("SAMEP");
  h_Rjet_Photon->Draw("SAMEP");
  lz_Rjet_Photon->Draw("SAMEP");

  //Save plot
  canv_Photon->Print("./plots/varPlots/varPhoton_all.pdf"); delete canv_Photon;

  //b jet
  TCanvas* canv_Photonb = new TCanvas("canv_Photonb","",500,400); canv_Photon->SetLogx();
  canv_Photonb->SetLeftMargin(0.15); canv_Photonb->SetBottomMargin(0.13);
  TLegend* lz_Photonb = new TLegend(0.3,0.2,0.6,0.3); lz_Photonb->SetBorderSize(0);
  lz_Photonb->AddEntry(h_Rjet_Photon,  "All jets", "p");
  lz_Photonb->AddEntry(h_Rjetb_Photon, "b jets", "p");
  TH1D* setup_Photonb = new TH1D("setup_Photonb","", h_Rjet_Photon->GetXaxis()->GetNbins(),
		                             h_Rjet_Photon->GetXaxis()->GetXmin(), 
                                             h_Rjet_Photon->GetXaxis()->GetXmax());
  setup_Photonb->SetStats(0);
  setup_Photonb->SetAxisRange(0.9932,1.0016,"Y");
  setup_Photonb->GetXaxis()->SetMoreLogLabels();  setup_Photonb->GetXaxis()->SetNoExponent();
  setup_Photonb->GetYaxis()->SetTitleOffset(1.1); 
  setup_Photonb->GetXaxis()->SetTitleOffset(1.1);
  setup_Photonb->GetYaxis()->SetTitle("F^{var}");
  setup_Photonb->GetYaxis()->SetTitleSize(0.05);
  setup_Photonb->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photonb->GetXaxis()->SetTitleSize(0.05);


  setup_Photonb->Draw();
  line->Draw("SAME");
  h_Rjetb_Photon->Draw("SAMEP");
  h_Rjet_Photon->Draw("SAMEP");
  lz_Photonb->Draw("SAMEP");
  canv_Photonb->Print("./plots/varPlots/varPhoton_b.pdf"); delete canv_Photonb;

  //c jet
  TCanvas* canv_Photonc = new TCanvas("canv_Photonc","",500,400); canv_Photon->SetLogx();
  canv_Photonc->SetLeftMargin(0.15); canv_Photonc->SetBottomMargin(0.13);
  TLegend* lz_Photonc = new TLegend(0.3,0.75,0.6,0.85); lz_Photonc->SetBorderSize(0);
  lz_Photonc->AddEntry(h_Rjet_Photon,  "All jets", "p");
  lz_Photonc->AddEntry(h_Rjetc_Photon, "c jets", "p");
  TH1D* setup_Photonc = new TH1D("setup_Photonc","", h_Rjet_Photon->GetXaxis()->GetNbins(),
		                             h_Rjet_Photon->GetXaxis()->GetXmin(), 
                                             h_Rjet_Photon->GetXaxis()->GetXmax());
  setup_Photonc->SetStats(0);
  setup_Photonc->SetAxisRange(0.993,1.002,"Y");
  setup_Photonc->GetXaxis()->SetMoreLogLabels();  setup_Photonc->GetXaxis()->SetNoExponent();
  setup_Photonc->GetYaxis()->SetTitleOffset(1.1); 
  setup_Photonc->GetXaxis()->SetTitleOffset(1.1);
  setup_Photonc->GetYaxis()->SetTitle("F^{var}");
  setup_Photonc->GetYaxis()->SetTitleSize(0.05);
  setup_Photonc->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photonc->GetXaxis()->SetTitleSize(0.05);

  setup_Photonc->Draw();
  line->Draw("SAME");
  h_Rjetc_Photon->Draw("SAMEP");
  h_Rjet_Photon->Draw("SAMEP");
  lz_Photonc->Draw("SAMEP");
  canv_Photonc->Print("./plots/varPlots/varPhoton_c.pdf"); delete canv_Photonc;

  //s jet
  TCanvas* canv_Photons = new TCanvas("canv_Photons","",500,400); canv_Photon->SetLogx();
  canv_Photons->SetLeftMargin(0.15); canv_Photons->SetBottomMargin(0.13);
  TLegend* lz_Photons = new TLegend(0.3,0.75,0.6,0.85); lz_Photons->SetBorderSize(0);
  lz_Photons->AddEntry(h_Rjet_Photon,  "All jets", "p");
  lz_Photons->AddEntry(h_Rjets_Photon, "s jets", "p");
  TH1D* setup_Photons = new TH1D("setup_Photons","", h_Rjet_Photon->GetXaxis()->GetNbins(),
		                             h_Rjet_Photon->GetXaxis()->GetXmin(), 
                                             h_Rjet_Photon->GetXaxis()->GetXmax());
  setup_Photons->SetStats(0);
  setup_Photons->SetAxisRange(0.993,1.002,"Y");
  setup_Photons->GetXaxis()->SetMoreLogLabels();  setup_Photons->GetXaxis()->SetNoExponent();
  setup_Photons->GetYaxis()->SetTitleOffset(1.1); 
  setup_Photons->GetXaxis()->SetTitleOffset(1.1);
  setup_Photons->GetYaxis()->SetTitle("F^{var}");
  setup_Photons->GetYaxis()->SetTitleSize(0.05);
  setup_Photons->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photons->GetXaxis()->SetTitleSize(0.05);

  setup_Photons->Draw();
  line->Draw("SAME");
  h_Rjets_Photon->Draw("SAMEP");
  h_Rjet_Photon->Draw("SAMEP");
  lz_Photons->Draw("SAMEP");
  canv_Photons->Print("./plots/varPlots/varPhoton_s.pdf"); delete canv_Photons;

  //ud jet
  TCanvas* canv_Photonud = new TCanvas("canv_Photonud","",500,400); canv_Photon->SetLogx();
  canv_Photonud->SetLeftMargin(0.15); canv_Photonud->SetBottomMargin(0.13);
  TLegend* lz_Photonud = new TLegend(0.3,0.75,0.6,0.85); lz_Photonud->SetBorderSize(0);
  lz_Photonud->AddEntry(h_Rjet_Photon,  "All jets", "p");
  lz_Photonud->AddEntry(h_Rjetud_Photon, "ud jets", "p");
  TH1D* setup_Photonud = new TH1D("setup_Photonud","", h_Rjet_Photon->GetXaxis()->GetNbins(),
		                             h_Rjet_Photon->GetXaxis()->GetXmin(), 
                                             h_Rjet_Photon->GetXaxis()->GetXmax());
  setup_Photonud->SetStats(0);
  setup_Photonud->SetAxisRange(0.993,1.002,"Y");
  setup_Photonud->GetXaxis()->SetMoreLogLabels();  setup_Photonud->GetXaxis()->SetNoExponent();
  setup_Photonud->GetYaxis()->SetTitleOffset(1.1); 
  setup_Photonud->GetXaxis()->SetTitleOffset(1.1);
  setup_Photonud->GetYaxis()->SetTitle("F^{var}");
  setup_Photonud->GetYaxis()->SetTitleSize(0.05);
  setup_Photonud->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photonud->GetXaxis()->SetTitleSize(0.05);

  setup_Photonud->Draw();
  line->Draw("SAME");
  h_Rjetud_Photon->Draw("SAMEP");
  h_Rjet_Photon->Draw("SAMEP");
  lz_Photonud->Draw("SAMEP");
  canv_Photonud->Print("./plots/varPlots/varPhoton_ud.pdf"); delete canv_Photonud;

  //g jet
  TCanvas* canv_Photong = new TCanvas("canv_Photong","",500,400); canv_Photon->SetLogx();
  canv_Photong->SetLeftMargin(0.15); canv_Photong->SetBottomMargin(0.13);
  TLegend* lz_Photong = new TLegend(0.3,0.75,0.6,0.85); lz_Photong->SetBorderSize(0);
  lz_Photong->AddEntry(h_Rjet_Photon,  "All jets", "p");
  lz_Photong->AddEntry(h_Rjetg_Photon, "g jets", "p");
  TH1D* setup_Photong = new TH1D("setup_Photong","", h_Rjet_Photon->GetXaxis()->GetNbins(),
		                             h_Rjet_Photon->GetXaxis()->GetXmin(), 
                                             h_Rjet_Photon->GetXaxis()->GetXmax());
  setup_Photong->SetStats(0);
  setup_Photong->SetAxisRange(0.993,1.002,"Y");
  setup_Photong->GetXaxis()->SetMoreLogLabels();  setup_Photong->GetXaxis()->SetNoExponent();
  setup_Photong->GetYaxis()->SetTitleOffset(1.1); 
  setup_Photong->GetXaxis()->SetTitleOffset(1.1);
  setup_Photong->GetYaxis()->SetTitle("F^{var}");
  setup_Photong->GetYaxis()->SetTitleSize(0.05);
  setup_Photong->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photong->GetXaxis()->SetTitleSize(0.05);

  setup_Photong->Draw();
  line->Draw("SAME");
  h_Rjetg_Photon->Draw("SAMEP");
  h_Rjet_Photon->Draw("SAMEP");
  lz_Photong->Draw("SAMEP");
  canv_Photong->Print("./plots/varPlots/varPhoton_g.pdf"); delete canv_Photong;

  gApplication->Terminate(0); //Exit root command prompt

}
