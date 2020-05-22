void plotECALVar()
{

  double markersize = 0.8;

  //Variants file
  TFile* fzj = TFile::Open("./output_ROOT_files/varPlots_5000000.root");

  TH1D* h_Rjet_ECALm1;
  TH1D* h_Rjet_ECALm3;
  TH1D* h_Rjetb_ECALm1;
  TH1D* h_Rjetb_ECALm3;
  TH1D* h_Rjetc_ECALm1;
  TH1D* h_Rjetc_ECALm3;
  TH1D* h_Rjets_ECALm1;
  TH1D* h_Rjets_ECALm3;
  TH1D* h_Rjetud_ECALm1;
  TH1D* h_Rjetud_ECALm3;
  TH1D* h_Rjetg_ECALm1;
  TH1D* h_Rjetg_ECALm3;

  //Create Histograms
  fzj->GetObject("h_Rjet_ECALm1", h_Rjet_ECALm1);
  fzj->GetObject("h_Rjet_ECALm3", h_Rjet_ECALm3);
  fzj->GetObject("h_Rjetb_ECALm1", h_Rjetb_ECALm1);
  fzj->GetObject("h_Rjetb_ECALm3", h_Rjetb_ECALm3);
  fzj->GetObject("h_Rjetc_ECALm1", h_Rjetc_ECALm1);
  fzj->GetObject("h_Rjetc_ECALm3", h_Rjetc_ECALm3);
  fzj->GetObject("h_Rjets_ECALm1", h_Rjets_ECALm1);
  fzj->GetObject("h_Rjets_ECALm3", h_Rjets_ECALm3);
  fzj->GetObject("h_Rjetud_ECALm1", h_Rjetud_ECALm1);
  fzj->GetObject("h_Rjetud_ECALm3", h_Rjetud_ECALm3);
  fzj->GetObject("h_Rjetg_ECALm1", h_Rjetg_ECALm1);
  fzj->GetObject("h_Rjetg_ECALm3", h_Rjetg_ECALm3);

// ECAL -1%
  TCanvas* canv_ECALm1 = new TCanvas("canv_ECALm1","",500,500);
  canv_ECALm1->SetLeftMargin(0.15); canv_ECALm1->SetBottomMargin(0.13);

  h_Rjet_ECALm1->SetMarkerStyle(5);                  h_Rjet_ECALm1->SetMarkerColor(kBlack);
  h_Rjet_ECALm1->SetLineColor(kBlack);               h_Rjet_ECALm1->SetMarkerSize(0.8);
  h_Rjetb_ECALm1->SetMarkerStyle(kOpenCircle);       h_Rjetb_ECALm1->SetMarkerColor(kRed+1);
  h_Rjetb_ECALm1->SetLineColor(kRed+1);              h_Rjetb_ECALm1->SetMarkerSize(0.8);
  h_Rjetc_ECALm1->SetMarkerStyle(kOpenTriangleDown); h_Rjetc_ECALm1->SetMarkerColor(kGreen+3);
  h_Rjetc_ECALm1->SetLineColor(kGreen+3);            h_Rjetc_ECALm1->SetMarkerSize(0.8);
  h_Rjets_ECALm1->SetMarkerStyle(kOpenTriangleUp);   h_Rjets_ECALm1->SetMarkerColor(kOrange+7);
  h_Rjets_ECALm1->SetLineColor(kOrange+7);           h_Rjets_ECALm1->SetMarkerSize(0.8);
  h_Rjetud_ECALm1->SetMarkerStyle(kOpenDiamond);     h_Rjetud_ECALm1->SetMarkerColor(kMagenta+2);
  h_Rjetud_ECALm1->SetLineColor(kMagenta+2);         h_Rjetud_ECALm1->SetMarkerSize(0.8);
  h_Rjetg_ECALm1->SetMarkerStyle(kOpenSquare);       h_Rjetg_ECALm1->SetMarkerColor(kBlue+1);
  h_Rjetg_ECALm1->SetLineColor(kBlue+1);             h_Rjetg_ECALm1->SetMarkerSize(0.8);

  h_Rjet_ECALm1->SetAxisRange(28, 1240,"X");
  h_Rjetb_ECALm1->SetAxisRange(28, 1240,"X");
  h_Rjetc_ECALm1->SetAxisRange(28, 1240,"X");
  h_Rjets_ECALm1->SetAxisRange(28, 1240,"X");
  h_Rjetud_ECALm1->SetAxisRange(28, 1240,"X");
  h_Rjetg_ECALm1->SetAxisRange(28, 1240,"X");

  //Legend
  TLegend* lz_Rjet_ECALm1 = new TLegend(0.45,0.19,0.8,0.29);
  lz_Rjet_ECALm1->SetBorderSize(0);
  lz_Rjet_ECALm1->AddEntry(h_Rjet_ECALm1,  "All jets ECAL-1%", "p");
  lz_Rjet_ECALm1->AddEntry(h_Rjetb_ECALm1, "b jets ECAL-1%", "p");
  lz_Rjet_ECALm1->AddEntry(h_Rjetc_ECALm1, "c jets ECAL-1%", "p");
  lz_Rjet_ECALm1->AddEntry(h_Rjets_ECALm1, "s jets ECAL-1%", "p");
  lz_Rjet_ECALm1->AddEntry(h_Rjetud_ECALm1, "ud jets ECAL-1%", "p");
  lz_Rjet_ECALm1->AddEntry(h_Rjetg_ECALm1,  "g jets ECAL-1%", "p");
  //lz_Rjet_ECALm1->SetNColumns(2);

  TH1D* setup_ECALm1 = new TH1D("setup_ECALm1","", h_Rjet_ECALm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_ECALm1->SetStats(0);
  setup_ECALm1->SetAxisRange(0.9825,1.002,"Y");
  setup_ECALm1->GetXaxis()->SetMoreLogLabels();
  setup_ECALm1->GetXaxis()->SetNoExponent();
  canv_ECALm1->SetLogx();
  setup_ECALm1->GetYaxis()->SetTitleOffset(1.5); 
  setup_ECALm1->GetXaxis()->SetTitleOffset(1.1);
  setup_ECALm1->GetYaxis()->SetTitle("F^{var}");
  setup_ECALm1->GetYaxis()->SetTitleSize(0.05);
  setup_ECALm1->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_ECALm1->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  TLine *line = new TLine(31.75,1,1258.25,1); 
  //Plot
  setup_ECALm1->Draw();
  line->Draw("SAME");
  h_Rjetb_ECALm1->Draw("SAMEP");
  h_Rjetc_ECALm1->Draw("SAMEP");
  h_Rjets_ECALm1->Draw("SAMEP");
  h_Rjetud_ECALm1->Draw("SAMEP");
  h_Rjetg_ECALm1->Draw("SAMEP");
  h_Rjet_ECALm1->Draw("SAMEP");
  lz_Rjet_ECALm1->Draw("SAMEP");

  TLatex *tex = new TLatex(); tex->SetNDC();
  tex->SetTextSize(0.04); tex->SetTextColor(kBlack);
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");

  //Save plot
  canv_ECALm1->Print("./plots/varPlots/ECAL/varECALm1_all.pdf"); delete canv_ECALm1;

  //b jet
  TCanvas* canv_ECALm1b = new TCanvas("canv_ECALm1b","",500,500); 
  canv_ECALm1b->SetLogx();
  canv_ECALm1b->SetLeftMargin(0.15); 
  canv_ECALm1b->SetBottomMargin(0.13);
  TLegend* lz_ECALm1b = new TLegend(0.45,0.19,0.8,0.29); lz_ECALm1b->SetBorderSize(0);
  lz_ECALm1b->AddEntry(h_Rjet_ECALm1,  "All jets ECAL-1%", "p");
  lz_ECALm1b->AddEntry(h_Rjetb_ECALm1, "b jets ECAL-1%", "p");
  TH1D* setup_ECALm1b = new TH1D("setup_ECALm1b","", h_Rjet_ECALm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_ECALm1b->SetStats(0);
  setup_ECALm1b->SetAxisRange(0.9825,1.002,"Y");
  setup_ECALm1b->GetXaxis()->SetMoreLogLabels();  setup_ECALm1b->GetXaxis()->SetNoExponent();
  setup_ECALm1b->GetYaxis()->SetTitleOffset(1.5); 
  setup_ECALm1b->GetXaxis()->SetTitleOffset(1.1);
  setup_ECALm1b->GetYaxis()->SetTitle("F^{var}");
  setup_ECALm1b->GetYaxis()->SetTitleSize(0.05);
  setup_ECALm1b->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_ECALm1b->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_ECALm1b->Draw();
  line->Draw("SAME");
  h_Rjetb_ECALm1->Draw("SAMEP");
  h_Rjet_ECALm1->Draw("SAMEP");
  lz_ECALm1b->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_ECALm1b->Print("./plots/varPlots/ECAL/varECALm1_b.pdf"); delete canv_ECALm1b;

  //c jet
  TCanvas* canv_ECALm1c = new TCanvas("canv_ECALm1c","",500,500); canv_ECALm1c->SetLogx();
  canv_ECALm1c->SetLeftMargin(0.15); canv_ECALm1c->SetBottomMargin(0.13);
  TLegend* lz_ECALm1c = new TLegend(0.45,0.19,0.8,0.29); lz_ECALm1c->SetBorderSize(0);
  lz_ECALm1c->AddEntry(h_Rjet_ECALm1,  "All jets ECAL-1%", "p");
  lz_ECALm1c->AddEntry(h_Rjetc_ECALm1, "c jets ECAL-1%", "p");
  TH1D* setup_ECALm1c = new TH1D("setup_ECALm1c","", h_Rjet_ECALm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_ECALm1c->SetStats(0);
  setup_ECALm1c->SetAxisRange(0.9825,1.002,"Y");
  setup_ECALm1c->GetXaxis()->SetMoreLogLabels();  setup_ECALm1c->GetXaxis()->SetNoExponent();
  setup_ECALm1c->GetYaxis()->SetTitleOffset(1.5); 
  setup_ECALm1c->GetXaxis()->SetTitleOffset(1.1);
  setup_ECALm1c->GetYaxis()->SetTitle("F^{var}");
  setup_ECALm1c->GetYaxis()->SetTitleSize(0.05);
  setup_ECALm1c->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_ECALm1c->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_ECALm1c->Draw();
  line->Draw("SAME");
  h_Rjetc_ECALm1->Draw("SAMEP");
  h_Rjet_ECALm1->Draw("SAMEP");
  lz_ECALm1c->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_ECALm1c->Print("./plots/varPlots/ECAL/varECALm1_c.pdf"); delete canv_ECALm1c;

  //s jet
  TCanvas* canv_ECALm1s = new TCanvas("canv_ECALm1s","",500,500); canv_ECALm1s->SetLogx();
  canv_ECALm1s->SetLeftMargin(0.15); canv_ECALm1s->SetBottomMargin(0.13);
  TLegend* lz_ECALm1s = new TLegend(0.45,0.19,0.8,0.29); lz_ECALm1s->SetBorderSize(0);
  lz_ECALm1s->AddEntry(h_Rjet_ECALm1,  "All jets ECAL-1%", "p");
  lz_ECALm1s->AddEntry(h_Rjets_ECALm1, "s jets ECAL-1%", "p");
  TH1D* setup_ECALm1s = new TH1D("setup_ECALm1s","", h_Rjet_ECALm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_ECALm1s->SetStats(0);
  setup_ECALm1s->SetAxisRange(0.9825,1.002,"Y");
  setup_ECALm1s->GetXaxis()->SetMoreLogLabels();  setup_ECALm1s->GetXaxis()->SetNoExponent();
  setup_ECALm1s->GetYaxis()->SetTitleOffset(1.5); 
  setup_ECALm1s->GetXaxis()->SetTitleOffset(1.1);
  setup_ECALm1s->GetYaxis()->SetTitle("F^{var}");
  setup_ECALm1s->GetYaxis()->SetTitleSize(0.05);
  setup_ECALm1s->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_ECALm1s->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_ECALm1s->Draw();
  line->Draw("SAME");
  h_Rjets_ECALm1->Draw("SAMEP");
  h_Rjet_ECALm1->Draw("SAMEP");
  lz_ECALm1s->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_ECALm1s->Print("./plots/varPlots/ECAL/varECALm1_s.pdf"); delete canv_ECALm1s;

  //ud jet
  TCanvas* canv_ECALm1ud = new TCanvas("canv_ECALm1ud","",500,500); canv_ECALm1ud->SetLogx();
  canv_ECALm1ud->SetLeftMargin(0.15); canv_ECALm1ud->SetBottomMargin(0.13);
  TLegend* lz_ECALm1ud = new TLegend(0.45,0.19,0.8,0.29); lz_ECALm1ud->SetBorderSize(0);
  lz_ECALm1ud->AddEntry(h_Rjet_ECALm1,  "All jets ECAL-1%", "p");
  lz_ECALm1ud->AddEntry(h_Rjetud_ECALm1, "ud jets ECAL-1%", "p");
  TH1D* setup_ECALm1ud = new TH1D("setup_ECALm1ud","", h_Rjet_ECALm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_ECALm1ud->SetStats(0);
  setup_ECALm1ud->SetAxisRange(0.9825,1.002,"Y");
  setup_ECALm1ud->GetXaxis()->SetMoreLogLabels();  setup_ECALm1ud->GetXaxis()->SetNoExponent();
  setup_ECALm1ud->GetYaxis()->SetTitleOffset(1.5); 
  setup_ECALm1ud->GetXaxis()->SetTitleOffset(1.1);
  setup_ECALm1ud->GetYaxis()->SetTitle("F^{var}");
  setup_ECALm1ud->GetYaxis()->SetTitleSize(0.05);
  setup_ECALm1ud->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_ECALm1ud->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_ECALm1ud->Draw();
  line->Draw("SAME");
  h_Rjetud_ECALm1->Draw("SAMEP");
  h_Rjet_ECALm1->Draw("SAMEP");
  lz_ECALm1ud->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_ECALm1ud->Print("./plots/varPlots/ECAL/varECALm1_ud.pdf"); delete canv_ECALm1ud;

  //g jet
  TCanvas* canv_ECALm1g = new TCanvas("canv_ECALm1g","",500,500); canv_ECALm1g->SetLogx();
  canv_ECALm1g->SetLeftMargin(0.15); canv_ECALm1g->SetBottomMargin(0.13);
  TLegend* lz_ECALm1g = new TLegend(0.45,0.19,0.8,0.29); lz_ECALm1g->SetBorderSize(0);
  lz_ECALm1g->AddEntry(h_Rjet_ECALm1,  "All jets ECAL-1%", "p");
  lz_ECALm1g->AddEntry(h_Rjetg_ECALm1, "g jets ECAL-1%", "p");
  TH1D* setup_ECALm1g = new TH1D("setup_ECALm1g","", h_Rjet_ECALm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_ECALm1g->SetStats(0);
  setup_ECALm1g->SetAxisRange(0.9825,1.002,"Y");
  setup_ECALm1g->GetXaxis()->SetMoreLogLabels();  setup_ECALm1g->GetXaxis()->SetNoExponent();
  setup_ECALm1g->GetYaxis()->SetTitleOffset(1.5); 
  setup_ECALm1g->GetXaxis()->SetTitleOffset(1.1);
  setup_ECALm1g->GetYaxis()->SetTitle("F^{var}");
  setup_ECALm1g->GetYaxis()->SetTitleSize(0.05);
  setup_ECALm1g->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_ECALm1g->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_ECALm1g->Draw();
  line->Draw("SAME");
  h_Rjetg_ECALm1->Draw("SAMEP");
  h_Rjet_ECALm1->Draw("SAMEP");
  lz_ECALm1g->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_ECALm1g->Print("./plots/varPlots/ECAL/varECALm1_g.pdf"); delete canv_ECALm1g;

// ECAL -3%
  TCanvas* canv_ECALm3 = new TCanvas("canv_ECALm3","",500,500);
  canv_ECALm3->SetLeftMargin(0.15); canv_ECALm3->SetBottomMargin(0.13);

  h_Rjet_ECALm3->SetMarkerStyle(5);                  h_Rjet_ECALm3->SetMarkerColor(kBlack);
  h_Rjet_ECALm3->SetLineColor(kBlack);               h_Rjet_ECALm3->SetMarkerSize(0.8);
  h_Rjetb_ECALm3->SetMarkerStyle(kOpenCircle);       h_Rjetb_ECALm3->SetMarkerColor(kRed+1);
  h_Rjetb_ECALm3->SetLineColor(kRed+1);              h_Rjetb_ECALm3->SetMarkerSize(0.8);
  h_Rjetc_ECALm3->SetMarkerStyle(kOpenTriangleDown); h_Rjetc_ECALm3->SetMarkerColor(kGreen+3);
  h_Rjetc_ECALm3->SetLineColor(kGreen+3);            h_Rjetc_ECALm3->SetMarkerSize(0.8);
  h_Rjets_ECALm3->SetMarkerStyle(kOpenTriangleUp);   h_Rjets_ECALm3->SetMarkerColor(kOrange+7);
  h_Rjets_ECALm3->SetLineColor(kOrange+7);           h_Rjets_ECALm3->SetMarkerSize(0.8);
  h_Rjetud_ECALm3->SetMarkerStyle(kOpenDiamond);     h_Rjetud_ECALm3->SetMarkerColor(kMagenta+2);
  h_Rjetud_ECALm3->SetLineColor(kMagenta+2);         h_Rjetud_ECALm3->SetMarkerSize(0.8);
  h_Rjetg_ECALm3->SetMarkerStyle(kOpenSquare);       h_Rjetg_ECALm3->SetMarkerColor(kBlue+1);
  h_Rjetg_ECALm3->SetLineColor(kBlue+1);             h_Rjetg_ECALm3->SetMarkerSize(0.8);

  h_Rjet_ECALm3->SetAxisRange(28, 1240,"X");
  h_Rjetb_ECALm3->SetAxisRange(28, 1240,"X");
  h_Rjetc_ECALm3->SetAxisRange(28, 1240,"X");
  h_Rjets_ECALm3->SetAxisRange(28, 1240,"X");
  h_Rjetud_ECALm3->SetAxisRange(28, 1240,"X");
  h_Rjetg_ECALm3->SetAxisRange(28, 1240,"X");

  //Legend
  TLegend* lz_Rjet_ECALm3 = new TLegend(0.45,0.19,0.8,0.29);
  lz_Rjet_ECALm3->SetBorderSize(0);
  lz_Rjet_ECALm3->AddEntry(h_Rjet_ECALm3,  "All jets ECAL-3%", "p");
  lz_Rjet_ECALm3->AddEntry(h_Rjetb_ECALm3, "b jets ECAL-3%", "p");
  lz_Rjet_ECALm3->AddEntry(h_Rjetc_ECALm3, "c jets ECAL-3%", "p");
  lz_Rjet_ECALm3->AddEntry(h_Rjets_ECALm3, "s jets ECAL-3%", "p");
  lz_Rjet_ECALm3->AddEntry(h_Rjetud_ECALm3, "ud jets ECAL-3%", "p");
  lz_Rjet_ECALm3->AddEntry(h_Rjetg_ECALm3,  "g jets ECAL-3%", "p");
  //lz_Rjet_ECALm3->SetNColumns(2);

  TH1D* setup_ECALm3 = new TH1D("setup_ECALm3","", h_Rjet_ECALm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_ECALm3->SetStats(0);
  setup_ECALm3->SetAxisRange(0.9825,1.002,"Y");
  setup_ECALm3->GetXaxis()->SetMoreLogLabels();
  setup_ECALm3->GetXaxis()->SetNoExponent();
  canv_ECALm3->SetLogx();
  setup_ECALm3->GetYaxis()->SetTitleOffset(1.5); 
  setup_ECALm3->GetXaxis()->SetTitleOffset(1.1);
  setup_ECALm3->GetYaxis()->SetTitle("F^{var}");
  setup_ECALm3->GetYaxis()->SetTitleSize(0.05);
  setup_ECALm3->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_ECALm3->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  //Plot
  setup_ECALm3->Draw();
  line->Draw("SAME");
  h_Rjetb_ECALm3->Draw("SAMEP");
  h_Rjetc_ECALm3->Draw("SAMEP");
  h_Rjets_ECALm3->Draw("SAMEP");
  h_Rjetud_ECALm3->Draw("SAMEP");
  h_Rjetg_ECALm3->Draw("SAMEP");
  h_Rjet_ECALm3->Draw("SAMEP");
  lz_Rjet_ECALm3->Draw("SAMEP");

  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");

  //Save plot
  canv_ECALm3->Print("./plots/varPlots/ECAL/varECALm3_all.pdf"); delete canv_ECALm3;

  //b jet
  TCanvas* canv_ECALm3b = new TCanvas("canv_ECALm3b","",500,500); 
  canv_ECALm3b->SetLogx();
  canv_ECALm3b->SetLeftMargin(0.15); 
  canv_ECALm3b->SetBottomMargin(0.13);
  TLegend* lz_ECALm3b = new TLegend(0.45,0.19,0.8,0.29); lz_ECALm3b->SetBorderSize(0);
  lz_ECALm3b->AddEntry(h_Rjet_ECALm3,  "All jets ECAL-3%", "p");
  lz_ECALm3b->AddEntry(h_Rjetb_ECALm3, "b jets ECAL-3%", "p");
  TH1D* setup_ECALm3b = new TH1D("setup_ECALm3b","", h_Rjet_ECALm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_ECALm3b->SetStats(0);
  setup_ECALm3b->SetAxisRange(0.9825,1.002,"Y");
  setup_ECALm3b->GetXaxis()->SetMoreLogLabels();  setup_ECALm3b->GetXaxis()->SetNoExponent();
  setup_ECALm3b->GetYaxis()->SetTitleOffset(1.5); 
  setup_ECALm3b->GetXaxis()->SetTitleOffset(1.1);
  setup_ECALm3b->GetYaxis()->SetTitle("F^{var}");
  setup_ECALm3b->GetYaxis()->SetTitleSize(0.05);
  setup_ECALm3b->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_ECALm3b->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_ECALm3b->Draw();
  line->Draw("SAME");
  h_Rjetb_ECALm3->Draw("SAMEP");
  h_Rjet_ECALm3->Draw("SAMEP");
  lz_ECALm3b->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_ECALm3b->Print("./plots/varPlots/ECAL/varECALm3_b.pdf"); delete canv_ECALm3b;

  //c jet
  TCanvas* canv_ECALm3c = new TCanvas("canv_ECALm3c","",500,500); canv_ECALm3c->SetLogx();
  canv_ECALm3c->SetLeftMargin(0.15); canv_ECALm3c->SetBottomMargin(0.13);
  TLegend* lz_ECALm3c = new TLegend(0.45,0.19,0.8,0.29); lz_ECALm3c->SetBorderSize(0);
  lz_ECALm3c->AddEntry(h_Rjet_ECALm3,  "All jets ECAL-3%", "p");
  lz_ECALm3c->AddEntry(h_Rjetc_ECALm3, "c jets ECAL-3%", "p");
  TH1D* setup_ECALm3c = new TH1D("setup_ECALm3c","", h_Rjet_ECALm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_ECALm3c->SetStats(0);
  setup_ECALm3c->SetAxisRange(0.9825,1.002,"Y");
  setup_ECALm3c->GetXaxis()->SetMoreLogLabels();  setup_ECALm3c->GetXaxis()->SetNoExponent();
  setup_ECALm3c->GetYaxis()->SetTitleOffset(1.5); 
  setup_ECALm3c->GetXaxis()->SetTitleOffset(1.1);
  setup_ECALm3c->GetYaxis()->SetTitle("F^{var}");
  setup_ECALm3c->GetYaxis()->SetTitleSize(0.05);
  setup_ECALm3c->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_ECALm3c->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_ECALm3c->Draw();
  line->Draw("SAME");
  h_Rjetc_ECALm3->Draw("SAMEP");
  h_Rjet_ECALm3->Draw("SAMEP");
  lz_ECALm3c->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_ECALm3c->Print("./plots/varPlots/ECAL/varECALm3_c.pdf"); delete canv_ECALm3c;

  //s jet
  TCanvas* canv_ECALm3s = new TCanvas("canv_ECALm3s","",500,500); canv_ECALm3s->SetLogx();
  canv_ECALm3s->SetLeftMargin(0.15); canv_ECALm3s->SetBottomMargin(0.13);
  TLegend* lz_ECALm3s = new TLegend(0.45,0.19,0.8,0.29); lz_ECALm3s->SetBorderSize(0);
  lz_ECALm3s->AddEntry(h_Rjet_ECALm3,  "All jets ECAL-3%", "p");
  lz_ECALm3s->AddEntry(h_Rjets_ECALm3, "s jets ECAL-3%", "p");
  TH1D* setup_ECALm3s = new TH1D("setup_ECALm3s","", h_Rjet_ECALm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_ECALm3s->SetStats(0);
  setup_ECALm3s->SetAxisRange(0.9825,1.002,"Y");
  setup_ECALm3s->GetXaxis()->SetMoreLogLabels();  setup_ECALm3s->GetXaxis()->SetNoExponent();
  setup_ECALm3s->GetYaxis()->SetTitleOffset(1.5); 
  setup_ECALm3s->GetXaxis()->SetTitleOffset(1.1);
  setup_ECALm3s->GetYaxis()->SetTitle("F^{var}");
  setup_ECALm3s->GetYaxis()->SetTitleSize(0.05);
  setup_ECALm3s->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_ECALm3s->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_ECALm3s->Draw();
  line->Draw("SAME");
  h_Rjets_ECALm3->Draw("SAMEP");
  h_Rjet_ECALm3->Draw("SAMEP");
  lz_ECALm3s->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_ECALm3s->Print("./plots/varPlots/ECAL/varECALm3_s.pdf"); delete canv_ECALm3s;

  //ud jet
  TCanvas* canv_ECALm3ud = new TCanvas("canv_ECALm3ud","",500,500); canv_ECALm3ud->SetLogx();
  canv_ECALm3ud->SetLeftMargin(0.15); canv_ECALm3ud->SetBottomMargin(0.13);
  TLegend* lz_ECALm3ud = new TLegend(0.45,0.19,0.8,0.29); lz_ECALm3ud->SetBorderSize(0);
  lz_ECALm3ud->AddEntry(h_Rjet_ECALm3,  "All jets ECAL-3%", "p");
  lz_ECALm3ud->AddEntry(h_Rjetud_ECALm3, "ud jets ECAL-3%", "p");
  TH1D* setup_ECALm3ud = new TH1D("setup_ECALm3ud","", h_Rjet_ECALm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_ECALm3ud->SetStats(0);
  setup_ECALm3ud->SetAxisRange(0.9825,1.002,"Y");
  setup_ECALm3ud->GetXaxis()->SetMoreLogLabels();  setup_ECALm3ud->GetXaxis()->SetNoExponent();
  setup_ECALm3ud->GetYaxis()->SetTitleOffset(1.5); 
  setup_ECALm3ud->GetXaxis()->SetTitleOffset(1.1);
  setup_ECALm3ud->GetYaxis()->SetTitle("F^{var}");
  setup_ECALm3ud->GetYaxis()->SetTitleSize(0.05);
  setup_ECALm3ud->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_ECALm3ud->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_ECALm3ud->Draw();
  line->Draw("SAME");
  h_Rjetud_ECALm3->Draw("SAMEP");
  h_Rjet_ECALm3->Draw("SAMEP");
  lz_ECALm3ud->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_ECALm3ud->Print("./plots/varPlots/ECAL/varECALm3_ud.pdf"); delete canv_ECALm3ud;

  //g jet
  TCanvas* canv_ECALm3g = new TCanvas("canv_ECALm3g","",500,500); canv_ECALm3g->SetLogx();
  canv_ECALm3g->SetLeftMargin(0.15); canv_ECALm3g->SetBottomMargin(0.13);
  TLegend* lz_ECALm3g = new TLegend(0.45,0.19,0.8,0.29); lz_ECALm3g->SetBorderSize(0);
  lz_ECALm3g->AddEntry(h_Rjet_ECALm3,  "All jets ECAL-3%", "p");
  lz_ECALm3g->AddEntry(h_Rjetg_ECALm3, "g jets ECAL-3%", "p");
  TH1D* setup_ECALm3g = new TH1D("setup_ECALm3g","", h_Rjet_ECALm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_ECALm3g->SetStats(0);
  setup_ECALm3g->SetAxisRange(0.9825,1.002,"Y");
  setup_ECALm3g->GetXaxis()->SetMoreLogLabels();  setup_ECALm3g->GetXaxis()->SetNoExponent();
  setup_ECALm3g->GetYaxis()->SetTitleOffset(1.5); 
  setup_ECALm3g->GetXaxis()->SetTitleOffset(1.1);
  setup_ECALm3g->GetYaxis()->SetTitle("F^{var}");
  setup_ECALm3g->GetYaxis()->SetTitleSize(0.05);
  setup_ECALm3g->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_ECALm3g->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_ECALm3g->Draw();
  line->Draw("SAME");
  h_Rjetg_ECALm3->Draw("SAMEP");
  h_Rjet_ECALm3->Draw("SAMEP");
  lz_ECALm3g->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_ECALm3g->Print("./plots/varPlots/ECAL/varECALm3_g.pdf"); delete canv_ECALm3g;


  gApplication->Terminate(0); //Exit root command prompt

}
