void plotSPRVar()
{

  double markersize = 0.8;

  //Variants file
  TFile* fzj = TFile::Open("./output_ROOT_files/varPlots_5000000.root");

  TH1D* h_Rjet_Cp3;
  TH1D* h_Rjet_Cm3;
  TH1D* h_Rjetb_Cp3;
  TH1D* h_Rjetb_Cm3;
  TH1D* h_Rjetc_Cp3;
  TH1D* h_Rjetc_Cm3;
  TH1D* h_Rjets_Cp3;
  TH1D* h_Rjets_Cm3;
  TH1D* h_Rjetud_Cp3;
  TH1D* h_Rjetud_Cm3;
  TH1D* h_Rjetg_Cp3;
  TH1D* h_Rjetg_Cm3;

  TH1D* h_Rjet_calo_Cp3;
  TH1D* h_Rjet_calo_Cm3;

  //Create Histograms
  fzj->GetObject("h_Rjet_Cp3", h_Rjet_Cp3);
  fzj->GetObject("h_Rjet_Cm3", h_Rjet_Cm3);
  fzj->GetObject("h_Rjetb_Cp3", h_Rjetb_Cp3);
  fzj->GetObject("h_Rjetb_Cm3", h_Rjetb_Cm3);
  fzj->GetObject("h_Rjetc_Cp3", h_Rjetc_Cp3);
  fzj->GetObject("h_Rjetc_Cm3", h_Rjetc_Cm3);
  fzj->GetObject("h_Rjets_Cp3", h_Rjets_Cp3);
  fzj->GetObject("h_Rjets_Cm3", h_Rjets_Cm3);
  fzj->GetObject("h_Rjetud_Cp3", h_Rjetud_Cp3);
  fzj->GetObject("h_Rjetud_Cm3", h_Rjetud_Cm3);
  fzj->GetObject("h_Rjetg_Cp3", h_Rjetg_Cp3);
  fzj->GetObject("h_Rjetg_Cm3", h_Rjetg_Cm3);

  fzj->GetObject("h_Rjet_calo_Cp3", h_Rjet_calo_Cp3);
  fzj->GetObject("h_Rjet_calo_Cm3", h_Rjet_calo_Cm3);



// SPR
  // SPR +-3% JEC paper
  TGraph *JEC_SPRp3      = new TGraph("data_and_MC_input/Response/JEC_PFJet_SPRplus3.txt" );
  TGraph *JEC_SPRm3      = new TGraph("data_and_MC_input/Response/JEC_PFJet_SPRminus3.txt" );
  TGraph *JEC_calo_SPRp3 = new TGraph("data_and_MC_input/Response/JEC_Calo_SPRplus3.txt" );
  TGraph *JEC_calo_SPRm3 = new TGraph("data_and_MC_input/Response/JEC_Calo_SPRminus3.txt" );

  JEC_SPRp3->SetMarkerStyle(kOpenCircle);            JEC_SPRp3->SetMarkerColor(kRed);
  JEC_SPRm3->SetMarkerStyle(kOpenSquare);            JEC_SPRm3->SetMarkerColor(kBlue+1);
  JEC_calo_SPRp3->SetMarkerStyle(kOpenTriangleUp);   JEC_calo_SPRp3->SetMarkerColor(kOrange+1);
  JEC_calo_SPRm3->SetMarkerStyle(kOpenTriangleDown); JEC_calo_SPRm3->SetMarkerColor(kGreen+2);

  JEC_SPRp3->SetMarkerSize(markersize);
  JEC_SPRm3->SetMarkerSize(markersize);
  JEC_calo_SPRp3->SetMarkerSize(markersize);
  JEC_calo_SPRm3->SetMarkerSize(markersize);


  //Canvas
  TCanvas* canv_spr = new TCanvas("canv_spr","",500,400);
  canv_spr->SetLeftMargin(0.15);
  canv_spr->SetBottomMargin(0.13);

  h_Rjet_Cp3->SetMarkerStyle(kFullCircle); h_Rjet_Cp3->SetMarkerColor(kRed);
  h_Rjet_Cm3->SetMarkerStyle(kFullSquare); h_Rjet_Cm3->SetMarkerColor(kBlue+1);
  h_Rjet_Cp3->SetLineColor(kRed);          h_Rjet_Cm3->SetLineColor(kBlue+1); 

  h_Rjet_calo_Cp3->SetMarkerStyle(kFullTriangleUp);   h_Rjet_calo_Cp3->SetMarkerColor(kOrange+1);
  h_Rjet_calo_Cm3->SetMarkerStyle(kFullTriangleDown); h_Rjet_calo_Cm3->SetMarkerColor(kGreen+2);
  h_Rjet_calo_Cp3->SetLineColor(kOrange+1);           h_Rjet_calo_Cm3->SetLineColor(kGreen+2); 

  h_Rjet_Cp3->SetMarkerSize(markersize);        h_Rjet_Cm3->SetMarkerSize(markersize);
  h_Rjet_calo_Cp3->SetMarkerSize(markersize);   h_Rjet_calo_Cm3->SetMarkerSize(markersize);

  //Legend
  TLegend* lz_Rjet = new TLegend(0.32,0.67,0.88,0.87);
  lz_Rjet->SetNColumns(2);
  lz_Rjet->SetBorderSize(0);
  lz_Rjet->AddEntry(h_Rjet_Cp3,      "Reco jet C+3% (Our)",   "p");
  lz_Rjet->AddEntry(JEC_SPRp3,       "Reco jet C+3% (FastSim)", "p");
  lz_Rjet->AddEntry(h_Rjet_Cm3,      "Reco jet C-3% (Our)",   "p");
  lz_Rjet->AddEntry(JEC_SPRm3,       "Reco jet C-3% (FastSim)", "p"); 
  lz_Rjet->AddEntry(h_Rjet_calo_Cp3, "Calo jet C+3% (Our)",   "p");
  lz_Rjet->AddEntry(JEC_calo_SPRp3,  "Calo jet C+3% (FastSim)", "p");
  lz_Rjet->AddEntry(h_Rjet_calo_Cm3, "Calo jet C-3% (Our)",   "p");
  lz_Rjet->AddEntry(JEC_calo_SPRm3,  "Calo jet C-3% (FastSim)", "p"); 


  TH1D* setup_spr = new TH1D("setup_spr","", h_Rjet_Cp3->GetXaxis()->GetNbins(), 28, 1588);

  //Title and axis setup
  setup_spr->SetStats(0); //Suppress stat box
  setup_spr->SetTitle("");
  setup_spr->SetAxisRange(0.974,1.05,"Y"); //Vertical axis limits
  h_Rjet_Cm3->SetAxisRange(28, 1588,"X"); 
  h_Rjet_Cp3->SetAxisRange(28, 1588,"X"); 
  h_Rjet_calo_Cm3->SetAxisRange(28, 1588,"X"); 
  h_Rjet_calo_Cp3->SetAxisRange(28, 1588,"X");
  setup_spr->GetXaxis()->SetMoreLogLabels();
  setup_spr->GetXaxis()->SetNoExponent();
  canv_spr->SetLogx();
  setup_spr->GetYaxis()->SetTitleOffset(1.1);
  setup_spr->GetXaxis()->SetTitleOffset(1.1);
  setup_spr->GetYaxis()->SetTitle("F^{var}");
  setup_spr->GetYaxis()->SetTitleSize(0.05);
  setup_spr->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_spr->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  //Plot
  setup_spr->Draw();
  JEC_calo_SPRp3->Draw("SAMEP");
  JEC_SPRp3->Draw("SAMEP");
  JEC_SPRm3->Draw("SAMEP");
  JEC_calo_SPRm3->Draw("SAMEP");
  h_Rjet_calo_Cp3->Draw("SAMEP");
  h_Rjet_calo_Cm3->Draw("SAMEP");
  h_Rjet_Cm3->Draw("SAMEP");
  h_Rjet_Cp3->Draw("SAMEP");
  lz_Rjet->Draw("SAMEP");

  TLatex *tex = new TLatex(); tex->SetNDC();
  tex->SetTextSize(0.04); tex->SetTextColor(kBlack);
  tex->DrawLatex(0.75,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.18,0.82,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.18,0.77,"#alpha<0.3");

  //Save plot
  canv_spr->Print("./plots/varPlots/SPR/varSPR.pdf");
  delete canv_spr;


// All flavours
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
  TH1D* setup = new TH1D("setup","", h_Rjet_Cp3->GetXaxis()->GetNbins(), 28, 1248);


  //Title and axis setup
  setup->SetStats(0); //Suppress stat box
  setup->SetAxisRange(0.977,1.028,"Y"); //Vertical axis limits
  h_Rjetb_Cm3->SetAxisRange(28, 1248,"X");
  h_Rjetb_Cp3->SetAxisRange(28, 1248,"X");
  h_Rjetc_Cm3->SetAxisRange(28, 1248,"X");
  h_Rjetc_Cp3->SetAxisRange(28, 1248,"X");
  h_Rjets_Cm3->SetAxisRange(28, 1248,"X");
  h_Rjets_Cp3->SetAxisRange(28, 1248,"X");
  h_Rjetud_Cm3->SetAxisRange(28, 1248,"X");
  h_Rjetud_Cp3->SetAxisRange(28, 1248,"X");
  h_Rjetg_Cm3->SetAxisRange(28, 1248,"X");
  h_Rjetg_Cp3->SetAxisRange(28, 1248,"X");
  h_Rjet_Cm3->SetAxisRange(28, 1248,"X");
  h_Rjet_Cp3->SetAxisRange(28, 1248,"X");


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

  TLatex *texall = new TLatex(); 
  texall->SetNDC();
  texall->SetTextSize(0.04); texall->SetTextColor(kBlack);
  texall->DrawLatex(0.75,0.92,"#sqrt{s}=13 TeV");
  texall->DrawLatex(0.23,0.28,"|#eta^{probe}|<1.3");
  texall->DrawLatex(0.23,0.23,"#alpha<0.3");

  //Save plot
  canv_C_all->Print("./plots/varPlots/SPR/varC_all.pdf");
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
  TH1D* setupb = new TH1D("setupb","", h_Rjet_Cp3->GetXaxis()->GetNbins(), 28, 1248);
  //Title and axis setup
  setupb->SetStats(0); //Suppress stat box
  setupb->SetAxisRange(0.979,1.021,"Y"); //Vertical axis limits
  h_Rjetb_Cm3->SetAxisRange(28, 1248,"X");
  h_Rjetb_Cp3->SetAxisRange(28, 1248,"X");
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

  TLatex *texb = new TLatex(); 
  texb->SetNDC();
  texb->SetTextSize(0.04); texb->SetTextColor(kBlack);
  texb->DrawLatex(0.75,0.92,"#sqrt{s}=13 TeV");
  texb->DrawLatex(0.23,0.30,"|#eta^{probe}|<1.3");
  texb->DrawLatex(0.23,0.25,"#alpha<0.3");

  //Save plot
  canv_C_b->Print("./plots/varPlots/SPR/varC_b.pdf"); delete canv_C_b;

  // c jets
  TCanvas* canv_C_c = new TCanvas("canv_C_c","",500,500);
  canv_C_c->SetLeftMargin(0.15); canv_C_c->SetBottomMargin(0.13);
  TLegend* lz_Cc = new TLegend(0.2,0.68,0.5,0.86);
  lz_Cc->SetBorderSize(0);
  lz_Cc->AddEntry(h_Rjet_Cp3, "All jets C+3%", "p");
  lz_Cc->AddEntry(h_Rjet_Cm3, "All jets C-3%", "p");
  lz_Cc->AddEntry(h_Rjetc_Cp3, "c jets C+3%", "p");
  lz_Cc->AddEntry(h_Rjetc_Cm3, "c jets C-3%", "p");
  TH1D* setupc = new TH1D("setupc","", h_Rjet_Cp3->GetXaxis()->GetNbins(), 28, 1248);
  //Title and axis setup
  setupc->SetStats(0); //Suppress stat box
  setupc->SetAxisRange(0.977,1.023,"Y"); //Vertical axis limits
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

  TLatex *texc = new TLatex(); 
  texc->SetNDC();
  texc->SetTextSize(0.04); 
  texc->SetTextColor(kBlack);
  texc->DrawLatex(0.75,0.92,"#sqrt{s}=13 TeV");
  texc->DrawLatex(0.23,0.30,"|#eta^{probe}|<1.3");
  texc->DrawLatex(0.23,0.25,"#alpha<0.3");

  canv_C_c->Print("./plots/varPlots/SPR/varC_c.pdf"); delete canv_C_c;

  // s jets
  TCanvas* canv_C_s = new TCanvas("canv_C_s","",500,500);
  canv_C_s->SetLeftMargin(0.15); canv_C_s->SetBottomMargin(0.13);
  TLegend* lz_Cs = new TLegend(0.2,0.68,0.5,0.86);
  lz_Cs->SetBorderSize(0);
  lz_Cs->AddEntry(h_Rjet_Cp3, "All jets C+3%", "p");
  lz_Cs->AddEntry(h_Rjet_Cm3, "All jets C-3%", "p");
  lz_Cs->AddEntry(h_Rjets_Cp3, "s jets C+3%", "p");
  lz_Cs->AddEntry(h_Rjets_Cm3, "s jets C-3%", "p");
  TH1D* setups = new TH1D("setups","", h_Rjet_Cp3->GetXaxis()->GetNbins(), 28, 1248);
  //Title and axis setup
  setups->SetStats(0); //Suppress stat box
  setups->SetAxisRange(0.977,1.023,"Y"); //Vertical axis limits
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

  TLatex *texs = new TLatex(); 
  texs->SetNDC();
  texs->SetTextSize(0.04); 
  texs->SetTextColor(kBlack);
  texs->DrawLatex(0.75,0.92,"#sqrt{s}=13 TeV");
  texs->DrawLatex(0.23,0.30,"|#eta^{probe}|<1.3");
  texs->DrawLatex(0.23,0.25,"#alpha<0.3");

  canv_C_s->Print("./plots/varPlots/SPR/varC_s.pdf"); delete canv_C_s;

  // ud jets
  TCanvas* canv_C_ud = new TCanvas("canv_C_ud","",500,500);
  canv_C_ud->SetLeftMargin(0.15); canv_C_ud->SetBottomMargin(0.13);
  TLegend* lz_Cud = new TLegend(0.2,0.68,0.5,0.86);
  lz_Cud->SetBorderSize(0);
  lz_Cud->AddEntry(h_Rjet_Cp3, "All jets C+3%", "p");
  lz_Cud->AddEntry(h_Rjet_Cm3, "All jets C-3%", "p");
  lz_Cud->AddEntry(h_Rjetud_Cp3, "ud jets C+3%", "p");
  lz_Cud->AddEntry(h_Rjetud_Cm3, "ud jets C-3%", "p");
  TH1D* setupud = new TH1D("setupud","", h_Rjet_Cp3->GetXaxis()->GetNbins(), 28, 1248);
  //Title and axis setup
  setupud->SetStats(0); //Suppress stat box
  setupud->SetAxisRange(0.982,1.018,"Y"); //Vertical axis limits
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

  TLatex *texud = new TLatex(); 
  texud->SetNDC();
  texud->SetTextSize(0.04); 
  texud->SetTextColor(kBlack);
  texud->DrawLatex(0.75,0.92,"#sqrt{s}=13 TeV");
  texud->DrawLatex(0.23,0.30,"|#eta^{probe}|<1.3");
  texud->DrawLatex(0.23,0.25,"#alpha<0.3");

  canv_C_ud->Print("./plots/varPlots/SPR/varC_ud.pdf"); delete canv_C_ud;

  // g jets
  TCanvas* canv_C_g = new TCanvas("canv_C_g","",500,500);
  canv_C_g->SetLeftMargin(0.15); canv_C_g->SetBottomMargin(0.13);
  TLegend* lz_Cg = new TLegend(0.2,0.68,0.5,0.86);
  lz_Cg->SetBorderSize(0);
  lz_Cg->AddEntry(h_Rjet_Cp3, "All jets C+3%", "p");
  lz_Cg->AddEntry(h_Rjet_Cm3, "All jets C-3%", "p");
  lz_Cg->AddEntry(h_Rjetg_Cp3, "g jets C+3%", "p");
  lz_Cg->AddEntry(h_Rjetg_Cm3, "g jets C-3%", "p");
  TH1D* setupg = new TH1D("setupg","", h_Rjet_Cp3->GetXaxis()->GetNbins(), 28, 1248);
  //Title and axis setup
  setupg->SetStats(0); //Suppress stat box
  setupg->SetAxisRange(0.984,1.016,"Y"); //Vertical axis limits
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

  TLatex *texg = new TLatex(); 
  texg->SetNDC();
  texg->SetTextSize(0.04); 
  texg->SetTextColor(kBlack);
  texg->DrawLatex(0.75,0.92,"#sqrt{s}=13 TeV");
  texg->DrawLatex(0.23,0.30,"|#eta^{probe}|<1.3");
  texg->DrawLatex(0.23,0.25,"#alpha<0.3");

  canv_C_g->Print("./plots/varPlots/SPR/varC_g.pdf"); delete canv_C_g;

  gApplication->Terminate(0); //Exit root command prompt

}
