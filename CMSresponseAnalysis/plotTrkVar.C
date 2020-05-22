void plotTrkVar()
{

  double markersize = 0.8;

  //Variants file
  TFile* fzj = TFile::Open("./output_ROOT_files/varPlots_5000000.root");

  TH1D* h_Rjet_Trkm1;
  TH1D* h_Rjet_Trkm3;
  TH1D* h_Rjetb_Trkm1;
  TH1D* h_Rjetb_Trkm3;
  TH1D* h_Rjetc_Trkm1;
  TH1D* h_Rjetc_Trkm3;
  TH1D* h_Rjets_Trkm1;
  TH1D* h_Rjets_Trkm3;
  TH1D* h_Rjetud_Trkm1;
  TH1D* h_Rjetud_Trkm3;
  TH1D* h_Rjetg_Trkm1;
  TH1D* h_Rjetg_Trkm3;

  //Create Histograms
  fzj->GetObject("h_Rjet_Trkm1", h_Rjet_Trkm1);
  fzj->GetObject("h_Rjet_Trkm3", h_Rjet_Trkm3);
  fzj->GetObject("h_Rjetb_Trkm1", h_Rjetb_Trkm1);
  fzj->GetObject("h_Rjetb_Trkm3", h_Rjetb_Trkm3);
  fzj->GetObject("h_Rjetc_Trkm1", h_Rjetc_Trkm1);
  fzj->GetObject("h_Rjetc_Trkm3", h_Rjetc_Trkm3);
  fzj->GetObject("h_Rjets_Trkm1", h_Rjets_Trkm1);
  fzj->GetObject("h_Rjets_Trkm3", h_Rjets_Trkm3);
  fzj->GetObject("h_Rjetud_Trkm1", h_Rjetud_Trkm1);
  fzj->GetObject("h_Rjetud_Trkm3", h_Rjetud_Trkm3);
  fzj->GetObject("h_Rjetg_Trkm1", h_Rjetg_Trkm1);
  fzj->GetObject("h_Rjetg_Trkm3", h_Rjetg_Trkm3);

// Trk -1%
  TCanvas* canv_Trkm1 = new TCanvas("canv_Trkm1","",500,500);
  canv_Trkm1->SetLeftMargin(0.15); canv_Trkm1->SetBottomMargin(0.13);

  h_Rjet_Trkm1->SetMarkerStyle(5);                  h_Rjet_Trkm1->SetMarkerColor(kBlack);
  h_Rjet_Trkm1->SetLineColor(kBlack);               h_Rjet_Trkm1->SetMarkerSize(0.8);
  h_Rjetb_Trkm1->SetMarkerStyle(kOpenCircle);       h_Rjetb_Trkm1->SetMarkerColor(kRed+1);
  h_Rjetb_Trkm1->SetLineColor(kRed+1);              h_Rjetb_Trkm1->SetMarkerSize(0.8);
  h_Rjetc_Trkm1->SetMarkerStyle(kOpenTriangleDown); h_Rjetc_Trkm1->SetMarkerColor(kGreen+3);
  h_Rjetc_Trkm1->SetLineColor(kGreen+3);            h_Rjetc_Trkm1->SetMarkerSize(0.8);
  h_Rjets_Trkm1->SetMarkerStyle(kOpenTriangleUp);   h_Rjets_Trkm1->SetMarkerColor(kOrange+7);
  h_Rjets_Trkm1->SetLineColor(kOrange+7);           h_Rjets_Trkm1->SetMarkerSize(0.8);
  h_Rjetud_Trkm1->SetMarkerStyle(kOpenDiamond);     h_Rjetud_Trkm1->SetMarkerColor(kMagenta+2);
  h_Rjetud_Trkm1->SetLineColor(kMagenta+2);         h_Rjetud_Trkm1->SetMarkerSize(0.8);
  h_Rjetg_Trkm1->SetMarkerStyle(kOpenSquare);       h_Rjetg_Trkm1->SetMarkerColor(kBlue+1);
  h_Rjetg_Trkm1->SetLineColor(kBlue+1);             h_Rjetg_Trkm1->SetMarkerSize(0.8);

  h_Rjet_Trkm1->SetAxisRange(28, 1240,"X");
  h_Rjetb_Trkm1->SetAxisRange(28, 1240,"X");
  h_Rjetc_Trkm1->SetAxisRange(28, 1240,"X");
  h_Rjets_Trkm1->SetAxisRange(28, 1240,"X");
  h_Rjetud_Trkm1->SetAxisRange(28, 1240,"X");
  h_Rjetg_Trkm1->SetAxisRange(28, 1240,"X");

  //Legend
  TLegend* lz_Rjet_Trkm1 = new TLegend(0.45,0.19,0.85,0.29);
  lz_Rjet_Trkm1->SetBorderSize(0);
  lz_Rjet_Trkm1->AddEntry(h_Rjet_Trkm1,  "All jets Tracking-1%", "p");
  lz_Rjet_Trkm1->AddEntry(h_Rjetb_Trkm1, "b jets Tracking-1%", "p");
  lz_Rjet_Trkm1->AddEntry(h_Rjetc_Trkm1, "c jets Tracking-1%", "p");
  lz_Rjet_Trkm1->AddEntry(h_Rjets_Trkm1, "s jets Tracking-1%", "p");
  lz_Rjet_Trkm1->AddEntry(h_Rjetud_Trkm1, "ud jets Tracking-1%", "p");
  lz_Rjet_Trkm1->AddEntry(h_Rjetg_Trkm1,  "g jets Tracking-1%", "p");
  //lz_Rjet_Trkm1->SetNColumns(2);

  TH1D* setup_Trkm1 = new TH1D("setup_Trkm1","", h_Rjet_Trkm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Trkm1->SetStats(0);
  setup_Trkm1->SetAxisRange(0.9882,1.005,"Y");
  setup_Trkm1->GetXaxis()->SetMoreLogLabels();
  setup_Trkm1->GetXaxis()->SetNoExponent();
  canv_Trkm1->SetLogx();
  setup_Trkm1->GetYaxis()->SetTitleOffset(1.5); 
  setup_Trkm1->GetXaxis()->SetTitleOffset(1.1);
  setup_Trkm1->GetYaxis()->SetTitle("F^{var}");
  setup_Trkm1->GetYaxis()->SetTitleSize(0.05);
  setup_Trkm1->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Trkm1->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  TLine *line = new TLine(31.75,1,1258.25,1); 
  //Plot
  setup_Trkm1->Draw();
  line->Draw("SAME");
  h_Rjetb_Trkm1->Draw("SAMEP");
  h_Rjetc_Trkm1->Draw("SAMEP");
  h_Rjets_Trkm1->Draw("SAMEP");
  h_Rjetud_Trkm1->Draw("SAMEP");
  h_Rjetg_Trkm1->Draw("SAMEP");
  h_Rjet_Trkm1->Draw("SAMEP");
  lz_Rjet_Trkm1->Draw("SAMEP");

  TLatex *tex = new TLatex(); tex->SetNDC();
  tex->SetTextSize(0.04); tex->SetTextColor(kBlack);
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");

  //Save plot
  canv_Trkm1->Print("./plots/varPlots/Trk/varTrkm1_all.pdf"); delete canv_Trkm1;

  //b jet
  TCanvas* canv_Trkm1b = new TCanvas("canv_Trkm1b","",500,500); 
  canv_Trkm1b->SetLogx();
  canv_Trkm1b->SetLeftMargin(0.15); 
  canv_Trkm1b->SetBottomMargin(0.13);
  TLegend* lz_Trkm1b = new TLegend(0.45,0.19,0.85,0.29); lz_Trkm1b->SetBorderSize(0);
  lz_Trkm1b->AddEntry(h_Rjet_Trkm1,  "All jets Tracking-1%", "p");
  lz_Trkm1b->AddEntry(h_Rjetb_Trkm1, "b jets Tracking-1%", "p");
  TH1D* setup_Trkm1b = new TH1D("setup_Trkm1b","", h_Rjet_Trkm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Trkm1b->SetStats(0);
  setup_Trkm1b->SetAxisRange(0.9882,1.005,"Y");
  setup_Trkm1b->GetXaxis()->SetMoreLogLabels();  setup_Trkm1b->GetXaxis()->SetNoExponent();
  setup_Trkm1b->GetYaxis()->SetTitleOffset(1.5); 
  setup_Trkm1b->GetXaxis()->SetTitleOffset(1.1);
  setup_Trkm1b->GetYaxis()->SetTitle("F^{var}");
  setup_Trkm1b->GetYaxis()->SetTitleSize(0.05);
  setup_Trkm1b->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Trkm1b->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Trkm1b->Draw();
  line->Draw("SAME");
  h_Rjetb_Trkm1->Draw("SAMEP");
  h_Rjet_Trkm1->Draw("SAMEP");
  lz_Trkm1b->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Trkm1b->Print("./plots/varPlots/Trk/varTrkm1_b.pdf"); delete canv_Trkm1b;

  //c jet
  TCanvas* canv_Trkm1c = new TCanvas("canv_Trkm1c","",500,500); canv_Trkm1c->SetLogx();
  canv_Trkm1c->SetLeftMargin(0.15); canv_Trkm1c->SetBottomMargin(0.13);
  TLegend* lz_Trkm1c = new TLegend(0.45,0.19,0.85,0.29); lz_Trkm1c->SetBorderSize(0);
  lz_Trkm1c->AddEntry(h_Rjet_Trkm1,  "All jets Tracking-1%", "p");
  lz_Trkm1c->AddEntry(h_Rjetc_Trkm1, "c jets Tracking-1%", "p");
  TH1D* setup_Trkm1c = new TH1D("setup_Trkm1c","", h_Rjet_Trkm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Trkm1c->SetStats(0);
  setup_Trkm1c->SetAxisRange(0.9882,1.005,"Y");
  setup_Trkm1c->GetXaxis()->SetMoreLogLabels();  setup_Trkm1c->GetXaxis()->SetNoExponent();
  setup_Trkm1c->GetYaxis()->SetTitleOffset(1.5); 
  setup_Trkm1c->GetXaxis()->SetTitleOffset(1.1);
  setup_Trkm1c->GetYaxis()->SetTitle("F^{var}");
  setup_Trkm1c->GetYaxis()->SetTitleSize(0.05);
  setup_Trkm1c->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Trkm1c->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Trkm1c->Draw();
  line->Draw("SAME");
  h_Rjetc_Trkm1->Draw("SAMEP");
  h_Rjet_Trkm1->Draw("SAMEP");
  lz_Trkm1c->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Trkm1c->Print("./plots/varPlots/Trk/varTrkm1_c.pdf"); delete canv_Trkm1c;

  //s jet
  TCanvas* canv_Trkm1s = new TCanvas("canv_Trkm1s","",500,500); canv_Trkm1s->SetLogx();
  canv_Trkm1s->SetLeftMargin(0.15); canv_Trkm1s->SetBottomMargin(0.13);
  TLegend* lz_Trkm1s = new TLegend(0.45,0.19,0.85,0.29); lz_Trkm1s->SetBorderSize(0);
  lz_Trkm1s->AddEntry(h_Rjet_Trkm1,  "All jets Tracking-1%", "p");
  lz_Trkm1s->AddEntry(h_Rjets_Trkm1, "s jets Tracking-1%", "p");
  TH1D* setup_Trkm1s = new TH1D("setup_Trkm1s","", h_Rjet_Trkm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Trkm1s->SetStats(0);
  setup_Trkm1s->SetAxisRange(0.9882,1.005,"Y");
  setup_Trkm1s->GetXaxis()->SetMoreLogLabels();  setup_Trkm1s->GetXaxis()->SetNoExponent();
  setup_Trkm1s->GetYaxis()->SetTitleOffset(1.5); 
  setup_Trkm1s->GetXaxis()->SetTitleOffset(1.1);
  setup_Trkm1s->GetYaxis()->SetTitle("F^{var}");
  setup_Trkm1s->GetYaxis()->SetTitleSize(0.05);
  setup_Trkm1s->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Trkm1s->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Trkm1s->Draw();
  line->Draw("SAME");
  h_Rjets_Trkm1->Draw("SAMEP");
  h_Rjet_Trkm1->Draw("SAMEP");
  lz_Trkm1s->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Trkm1s->Print("./plots/varPlots/Trk/varTrkm1_s.pdf"); delete canv_Trkm1s;

  //ud jet
  TCanvas* canv_Trkm1ud = new TCanvas("canv_Trkm1ud","",500,500); canv_Trkm1ud->SetLogx();
  canv_Trkm1ud->SetLeftMargin(0.15); canv_Trkm1ud->SetBottomMargin(0.13);
  TLegend* lz_Trkm1ud = new TLegend(0.45,0.19,0.85,0.29); lz_Trkm1ud->SetBorderSize(0);
  lz_Trkm1ud->AddEntry(h_Rjet_Trkm1,  "All jets Tracking-1%", "p");
  lz_Trkm1ud->AddEntry(h_Rjetud_Trkm1, "ud jets Tracking-1%", "p");
  TH1D* setup_Trkm1ud = new TH1D("setup_Trkm1ud","", h_Rjet_Trkm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Trkm1ud->SetStats(0);
  setup_Trkm1ud->SetAxisRange(0.9882,1.005,"Y");
  setup_Trkm1ud->GetXaxis()->SetMoreLogLabels();  setup_Trkm1ud->GetXaxis()->SetNoExponent();
  setup_Trkm1ud->GetYaxis()->SetTitleOffset(1.5); 
  setup_Trkm1ud->GetXaxis()->SetTitleOffset(1.1);
  setup_Trkm1ud->GetYaxis()->SetTitle("F^{var}");
  setup_Trkm1ud->GetYaxis()->SetTitleSize(0.05);
  setup_Trkm1ud->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Trkm1ud->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Trkm1ud->Draw();
  line->Draw("SAME");
  h_Rjetud_Trkm1->Draw("SAMEP");
  h_Rjet_Trkm1->Draw("SAMEP");
  lz_Trkm1ud->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Trkm1ud->Print("./plots/varPlots/Trk/varTrkm1_ud.pdf"); delete canv_Trkm1ud;

  //g jet
  TCanvas* canv_Trkm1g = new TCanvas("canv_Trkm1g","",500,500); canv_Trkm1g->SetLogx();
  canv_Trkm1g->SetLeftMargin(0.15); canv_Trkm1g->SetBottomMargin(0.13);
  TLegend* lz_Trkm1g = new TLegend(0.45,0.19,0.85,0.29); lz_Trkm1g->SetBorderSize(0);
  lz_Trkm1g->AddEntry(h_Rjet_Trkm1,  "All jets Tracking-1%", "p");
  lz_Trkm1g->AddEntry(h_Rjetg_Trkm1, "g jets Tracking-1%", "p");
  TH1D* setup_Trkm1g = new TH1D("setup_Trkm1g","", h_Rjet_Trkm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Trkm1g->SetStats(0);
  setup_Trkm1g->SetAxisRange(0.9882,1.005,"Y");
  setup_Trkm1g->GetXaxis()->SetMoreLogLabels();  setup_Trkm1g->GetXaxis()->SetNoExponent();
  setup_Trkm1g->GetYaxis()->SetTitleOffset(1.5); 
  setup_Trkm1g->GetXaxis()->SetTitleOffset(1.1);
  setup_Trkm1g->GetYaxis()->SetTitle("F^{var}");
  setup_Trkm1g->GetYaxis()->SetTitleSize(0.05);
  setup_Trkm1g->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Trkm1g->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Trkm1g->Draw();
  line->Draw("SAME");
  h_Rjetg_Trkm1->Draw("SAMEP");
  h_Rjet_Trkm1->Draw("SAMEP");
  lz_Trkm1g->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Trkm1g->Print("./plots/varPlots/Trk/varTrkm1_g.pdf"); delete canv_Trkm1g;

// Trk -3%
  TCanvas* canv_Trkm3 = new TCanvas("canv_Trkm3","",500,500);
  canv_Trkm3->SetLeftMargin(0.15); canv_Trkm3->SetBottomMargin(0.13);

  h_Rjet_Trkm3->SetMarkerStyle(5);                  h_Rjet_Trkm3->SetMarkerColor(kBlack);
  h_Rjet_Trkm3->SetLineColor(kBlack);               h_Rjet_Trkm3->SetMarkerSize(0.8);
  h_Rjetb_Trkm3->SetMarkerStyle(kOpenCircle);       h_Rjetb_Trkm3->SetMarkerColor(kRed+1);
  h_Rjetb_Trkm3->SetLineColor(kRed+1);              h_Rjetb_Trkm3->SetMarkerSize(0.8);
  h_Rjetc_Trkm3->SetMarkerStyle(kOpenTriangleDown); h_Rjetc_Trkm3->SetMarkerColor(kGreen+3);
  h_Rjetc_Trkm3->SetLineColor(kGreen+3);            h_Rjetc_Trkm3->SetMarkerSize(0.8);
  h_Rjets_Trkm3->SetMarkerStyle(kOpenTriangleUp);   h_Rjets_Trkm3->SetMarkerColor(kOrange+7);
  h_Rjets_Trkm3->SetLineColor(kOrange+7);           h_Rjets_Trkm3->SetMarkerSize(0.8);
  h_Rjetud_Trkm3->SetMarkerStyle(kOpenDiamond);     h_Rjetud_Trkm3->SetMarkerColor(kMagenta+2);
  h_Rjetud_Trkm3->SetLineColor(kMagenta+2);         h_Rjetud_Trkm3->SetMarkerSize(0.8);
  h_Rjetg_Trkm3->SetMarkerStyle(kOpenSquare);       h_Rjetg_Trkm3->SetMarkerColor(kBlue+1);
  h_Rjetg_Trkm3->SetLineColor(kBlue+1);             h_Rjetg_Trkm3->SetMarkerSize(0.8);

  h_Rjet_Trkm3->SetAxisRange(28, 1240,"X");
  h_Rjetb_Trkm3->SetAxisRange(28, 1240,"X");
  h_Rjetc_Trkm3->SetAxisRange(28, 1240,"X");
  h_Rjets_Trkm3->SetAxisRange(28, 1240,"X");
  h_Rjetud_Trkm3->SetAxisRange(28, 1240,"X");
  h_Rjetg_Trkm3->SetAxisRange(28, 1240,"X");

  //Legend
  TLegend* lz_Rjet_Trkm3 = new TLegend(0.45,0.19,0.85,0.29);
  lz_Rjet_Trkm3->SetBorderSize(0);
  lz_Rjet_Trkm3->AddEntry(h_Rjet_Trkm3,  "All jets Tracking-3%", "p");
  lz_Rjet_Trkm3->AddEntry(h_Rjetb_Trkm3, "b jets Tracking-3%", "p");
  lz_Rjet_Trkm3->AddEntry(h_Rjetc_Trkm3, "c jets Tracking-3%", "p");
  lz_Rjet_Trkm3->AddEntry(h_Rjets_Trkm3, "s jets Tracking-3%", "p");
  lz_Rjet_Trkm3->AddEntry(h_Rjetud_Trkm3, "ud jets Tracking-3%", "p");
  lz_Rjet_Trkm3->AddEntry(h_Rjetg_Trkm3,  "g jets Tracking-3%", "p");
  //lz_Rjet_Trkm3->SetNColumns(2);

  TH1D* setup_Trkm3 = new TH1D("setup_Trkm3","", h_Rjet_Trkm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Trkm3->SetStats(0);
  setup_Trkm3->SetAxisRange(0.9882,1.005,"Y");
  setup_Trkm3->GetXaxis()->SetMoreLogLabels();
  setup_Trkm3->GetXaxis()->SetNoExponent();
  canv_Trkm3->SetLogx();
  setup_Trkm3->GetYaxis()->SetTitleOffset(1.5); 
  setup_Trkm3->GetXaxis()->SetTitleOffset(1.1);
  setup_Trkm3->GetYaxis()->SetTitle("F^{var}");
  setup_Trkm3->GetYaxis()->SetTitleSize(0.05);
  setup_Trkm3->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Trkm3->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  //Plot
  setup_Trkm3->Draw();
  line->Draw("SAME");
  h_Rjetb_Trkm3->Draw("SAMEP");
  h_Rjetc_Trkm3->Draw("SAMEP");
  h_Rjets_Trkm3->Draw("SAMEP");
  h_Rjetud_Trkm3->Draw("SAMEP");
  h_Rjetg_Trkm3->Draw("SAMEP");
  h_Rjet_Trkm3->Draw("SAMEP");
  lz_Rjet_Trkm3->Draw("SAMEP");

  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");

  //Save plot
  canv_Trkm3->Print("./plots/varPlots/Trk/varTrkm3_all.pdf"); delete canv_Trkm3;

  //b jet
  TCanvas* canv_Trkm3b = new TCanvas("canv_Trkm3b","",500,500); 
  canv_Trkm3b->SetLogx();
  canv_Trkm3b->SetLeftMargin(0.15); 
  canv_Trkm3b->SetBottomMargin(0.13);
  TLegend* lz_Trkm3b = new TLegend(0.45,0.19,0.85,0.29); lz_Trkm3b->SetBorderSize(0);
  lz_Trkm3b->AddEntry(h_Rjet_Trkm3,  "All jets Tracking-3%", "p");
  lz_Trkm3b->AddEntry(h_Rjetb_Trkm3, "b jets Tracking-3%", "p");
  TH1D* setup_Trkm3b = new TH1D("setup_Trkm3b","", h_Rjet_Trkm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Trkm3b->SetStats(0);
  setup_Trkm3b->SetAxisRange(0.9882,1.005,"Y");
  setup_Trkm3b->GetXaxis()->SetMoreLogLabels();  setup_Trkm3b->GetXaxis()->SetNoExponent();
  setup_Trkm3b->GetYaxis()->SetTitleOffset(1.5); 
  setup_Trkm3b->GetXaxis()->SetTitleOffset(1.1);
  setup_Trkm3b->GetYaxis()->SetTitle("F^{var}");
  setup_Trkm3b->GetYaxis()->SetTitleSize(0.05);
  setup_Trkm3b->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Trkm3b->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Trkm3b->Draw();
  line->Draw("SAME");
  h_Rjetb_Trkm3->Draw("SAMEP");
  h_Rjet_Trkm3->Draw("SAMEP");
  lz_Trkm3b->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Trkm3b->Print("./plots/varPlots/Trk/varTrkm3_b.pdf"); delete canv_Trkm3b;

  //c jet
  TCanvas* canv_Trkm3c = new TCanvas("canv_Trkm3c","",500,500); canv_Trkm3c->SetLogx();
  canv_Trkm3c->SetLeftMargin(0.15); canv_Trkm3c->SetBottomMargin(0.13);
  TLegend* lz_Trkm3c = new TLegend(0.45,0.19,0.85,0.29); lz_Trkm3c->SetBorderSize(0);
  lz_Trkm3c->AddEntry(h_Rjet_Trkm3,  "All jets Tracking-3%", "p");
  lz_Trkm3c->AddEntry(h_Rjetc_Trkm3, "c jets Tracking-3%", "p");
  TH1D* setup_Trkm3c = new TH1D("setup_Trkm3c","", h_Rjet_Trkm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Trkm3c->SetStats(0);
  setup_Trkm3c->SetAxisRange(0.9882,1.005,"Y");
  setup_Trkm3c->GetXaxis()->SetMoreLogLabels();  setup_Trkm3c->GetXaxis()->SetNoExponent();
  setup_Trkm3c->GetYaxis()->SetTitleOffset(1.5); 
  setup_Trkm3c->GetXaxis()->SetTitleOffset(1.1);
  setup_Trkm3c->GetYaxis()->SetTitle("F^{var}");
  setup_Trkm3c->GetYaxis()->SetTitleSize(0.05);
  setup_Trkm3c->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Trkm3c->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Trkm3c->Draw();
  line->Draw("SAME");
  h_Rjetc_Trkm3->Draw("SAMEP");
  h_Rjet_Trkm3->Draw("SAMEP");
  lz_Trkm3c->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Trkm3c->Print("./plots/varPlots/Trk/varTrkm3_c.pdf"); delete canv_Trkm3c;

  //s jet
  TCanvas* canv_Trkm3s = new TCanvas("canv_Trkm3s","",500,500); canv_Trkm3s->SetLogx();
  canv_Trkm3s->SetLeftMargin(0.15); canv_Trkm3s->SetBottomMargin(0.13);
  TLegend* lz_Trkm3s = new TLegend(0.45,0.19,0.85,0.29); lz_Trkm3s->SetBorderSize(0);
  lz_Trkm3s->AddEntry(h_Rjet_Trkm3,  "All jets Tracking-3%", "p");
  lz_Trkm3s->AddEntry(h_Rjets_Trkm3, "s jets Tracking-3%", "p");
  TH1D* setup_Trkm3s = new TH1D("setup_Trkm3s","", h_Rjet_Trkm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Trkm3s->SetStats(0);
  setup_Trkm3s->SetAxisRange(0.9882,1.005,"Y");
  setup_Trkm3s->GetXaxis()->SetMoreLogLabels();  setup_Trkm3s->GetXaxis()->SetNoExponent();
  setup_Trkm3s->GetYaxis()->SetTitleOffset(1.5); 
  setup_Trkm3s->GetXaxis()->SetTitleOffset(1.1);
  setup_Trkm3s->GetYaxis()->SetTitle("F^{var}");
  setup_Trkm3s->GetYaxis()->SetTitleSize(0.05);
  setup_Trkm3s->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Trkm3s->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Trkm3s->Draw();
  line->Draw("SAME");
  h_Rjets_Trkm3->Draw("SAMEP");
  h_Rjet_Trkm3->Draw("SAMEP");
  lz_Trkm3s->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Trkm3s->Print("./plots/varPlots/Trk/varTrkm3_s.pdf"); delete canv_Trkm3s;

  //ud jet
  TCanvas* canv_Trkm3ud = new TCanvas("canv_Trkm3ud","",500,500); canv_Trkm3ud->SetLogx();
  canv_Trkm3ud->SetLeftMargin(0.15); canv_Trkm3ud->SetBottomMargin(0.13);
  TLegend* lz_Trkm3ud = new TLegend(0.45,0.19,0.85,0.29); lz_Trkm3ud->SetBorderSize(0);
  lz_Trkm3ud->AddEntry(h_Rjet_Trkm3,  "All jets Tracking-3%", "p");
  lz_Trkm3ud->AddEntry(h_Rjetud_Trkm3, "ud jets Tracking-3%", "p");
  TH1D* setup_Trkm3ud = new TH1D("setup_Trkm3ud","", h_Rjet_Trkm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Trkm3ud->SetStats(0);
  setup_Trkm3ud->SetAxisRange(0.9882,1.005,"Y");
  setup_Trkm3ud->GetXaxis()->SetMoreLogLabels();  setup_Trkm3ud->GetXaxis()->SetNoExponent();
  setup_Trkm3ud->GetYaxis()->SetTitleOffset(1.5); 
  setup_Trkm3ud->GetXaxis()->SetTitleOffset(1.1);
  setup_Trkm3ud->GetYaxis()->SetTitle("F^{var}");
  setup_Trkm3ud->GetYaxis()->SetTitleSize(0.05);
  setup_Trkm3ud->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Trkm3ud->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Trkm3ud->Draw();
  line->Draw("SAME");
  h_Rjetud_Trkm3->Draw("SAMEP");
  h_Rjet_Trkm3->Draw("SAMEP");
  lz_Trkm3ud->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Trkm3ud->Print("./plots/varPlots/Trk/varTrkm3_ud.pdf"); delete canv_Trkm3ud;

  //g jet
  TCanvas* canv_Trkm3g = new TCanvas("canv_Trkm3g","",500,500); canv_Trkm3g->SetLogx();
  canv_Trkm3g->SetLeftMargin(0.15); canv_Trkm3g->SetBottomMargin(0.13);
  TLegend* lz_Trkm3g = new TLegend(0.45,0.19,0.85,0.29); lz_Trkm3g->SetBorderSize(0);
  lz_Trkm3g->AddEntry(h_Rjet_Trkm3,  "All jets Tracking-3%", "p");
  lz_Trkm3g->AddEntry(h_Rjetg_Trkm3, "g jets Tracking-3%", "p");
  TH1D* setup_Trkm3g = new TH1D("setup_Trkm3g","", h_Rjet_Trkm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Trkm3g->SetStats(0);
  setup_Trkm3g->SetAxisRange(0.9882,1.005,"Y");
  setup_Trkm3g->GetXaxis()->SetMoreLogLabels();  setup_Trkm3g->GetXaxis()->SetNoExponent();
  setup_Trkm3g->GetYaxis()->SetTitleOffset(1.5); 
  setup_Trkm3g->GetXaxis()->SetTitleOffset(1.1);
  setup_Trkm3g->GetYaxis()->SetTitle("F^{var}");
  setup_Trkm3g->GetYaxis()->SetTitleSize(0.05);
  setup_Trkm3g->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Trkm3g->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Trkm3g->Draw();
  line->Draw("SAME");
  h_Rjetg_Trkm3->Draw("SAMEP");
  h_Rjet_Trkm3->Draw("SAMEP");
  lz_Trkm3g->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Trkm3g->Print("./plots/varPlots/Trk/varTrkm3_g.pdf"); delete canv_Trkm3g;


  gApplication->Terminate(0); //Exit root command prompt

}
