void plotPhotonVar()
{

  double markersize = 0.8;

  //Variants file
  TFile* fzj = TFile::Open("./output_ROOT_files/varPlots_P8_Zjet_5000000.root");

  TH1D* h_Rjet_Photonm1;
  TH1D* h_Rjet_Photonm3;
  TH1D* h_Rjetb_Photonm1;
  TH1D* h_Rjetb_Photonm3;
  TH1D* h_Rjetc_Photonm1;
  TH1D* h_Rjetc_Photonm3;
  TH1D* h_Rjets_Photonm1;
  TH1D* h_Rjets_Photonm3;
  TH1D* h_Rjetud_Photonm1;
  TH1D* h_Rjetud_Photonm3;
  TH1D* h_Rjetg_Photonm1;
  TH1D* h_Rjetg_Photonm3;

  //Create Histograms
  fzj->GetObject("h_Rjet_Photonm1", h_Rjet_Photonm1);
  fzj->GetObject("h_Rjet_Photonm3", h_Rjet_Photonm3);
  fzj->GetObject("h_Rjetb_Photonm1", h_Rjetb_Photonm1);
  fzj->GetObject("h_Rjetb_Photonm3", h_Rjetb_Photonm3);
  fzj->GetObject("h_Rjetc_Photonm1", h_Rjetc_Photonm1);
  fzj->GetObject("h_Rjetc_Photonm3", h_Rjetc_Photonm3);
  fzj->GetObject("h_Rjets_Photonm1", h_Rjets_Photonm1);
  fzj->GetObject("h_Rjets_Photonm3", h_Rjets_Photonm3);
  fzj->GetObject("h_Rjetud_Photonm1", h_Rjetud_Photonm1);
  fzj->GetObject("h_Rjetud_Photonm3", h_Rjetud_Photonm3);
  fzj->GetObject("h_Rjetg_Photonm1", h_Rjetg_Photonm1);
  fzj->GetObject("h_Rjetg_Photonm3", h_Rjetg_Photonm3);

// Photon -1%
  TCanvas* canv_Photonm1 = new TCanvas("canv_Photonm1","",500,500);
  canv_Photonm1->SetLeftMargin(0.15); canv_Photonm1->SetBottomMargin(0.13);

  h_Rjet_Photonm1->SetMarkerStyle(5);                  h_Rjet_Photonm1->SetMarkerColor(kBlack);
  h_Rjet_Photonm1->SetLineColor(kBlack);               h_Rjet_Photonm1->SetMarkerSize(0.8);
  h_Rjetb_Photonm1->SetMarkerStyle(kFullCircle);       h_Rjetb_Photonm1->SetMarkerColor(kRed+1);
  h_Rjetb_Photonm1->SetLineColor(kRed+1);              h_Rjetb_Photonm1->SetMarkerSize(0.8);
  h_Rjetc_Photonm1->SetMarkerStyle(kFullTriangleDown); h_Rjetc_Photonm1->SetMarkerColor(kGreen+3);
  h_Rjetc_Photonm1->SetLineColor(kGreen+3);            h_Rjetc_Photonm1->SetMarkerSize(0.8);
  h_Rjets_Photonm1->SetMarkerStyle(kFullTriangleUp);   h_Rjets_Photonm1->SetMarkerColor(kOrange+7);
  h_Rjets_Photonm1->SetLineColor(kOrange+7);           h_Rjets_Photonm1->SetMarkerSize(0.8);
  h_Rjetud_Photonm1->SetMarkerStyle(kFullDiamond);     h_Rjetud_Photonm1->SetMarkerColor(kMagenta+2);
  h_Rjetud_Photonm1->SetLineColor(kMagenta+2);         h_Rjetud_Photonm1->SetMarkerSize(0.8);
  h_Rjetg_Photonm1->SetMarkerStyle(kFullSquare);       h_Rjetg_Photonm1->SetMarkerColor(kBlue+1);
  h_Rjetg_Photonm1->SetLineColor(kBlue+1);             h_Rjetg_Photonm1->SetMarkerSize(0.8);

  h_Rjet_Photonm1->SetAxisRange(15, 1248,"X");
  h_Rjetb_Photonm1->SetAxisRange(15, 1248,"X");
  h_Rjetc_Photonm1->SetAxisRange(15, 1248,"X");
  h_Rjets_Photonm1->SetAxisRange(15, 1248,"X");
  h_Rjetud_Photonm1->SetAxisRange(15, 1248,"X");
  h_Rjetg_Photonm1->SetAxisRange(15, 1248,"X");

  //Legend
  TLegend* lz_Rjet_Photonm1 = new TLegend(0.45,0.71,0.8,0.86);
  lz_Rjet_Photonm1->SetBorderSize(0);
  lz_Rjet_Photonm1->AddEntry(h_Rjet_Photonm1,  "#bf{All jets Photon-1%}", "p");
  lz_Rjet_Photonm1->AddEntry(h_Rjetb_Photonm1, "#bf{b jets Photon-1%}", "p");
  lz_Rjet_Photonm1->AddEntry(h_Rjetc_Photonm1, "#bf{c jets Photon-1%}", "p");
  lz_Rjet_Photonm1->AddEntry(h_Rjets_Photonm1, "#bf{s jets Photon-1%}", "p");
  lz_Rjet_Photonm1->AddEntry(h_Rjetud_Photonm1, "#bf{ud jets Photon-1%}", "p");
  lz_Rjet_Photonm1->AddEntry(h_Rjetg_Photonm1,  "#bf{g jets Photon-1%}", "p");
  //lz_Rjet_Photonm1->SetNColumns(2);

  TH1D* setup_Photonm1 = new TH1D("setup_Photonm1","", h_Rjet_Photonm1->GetXaxis()->GetNbins(),
  15, 1248);
  setup_Photonm1->SetStats(0);
  setup_Photonm1->SetAxisRange(0.986,1.005,"Y");
  setup_Photonm1->GetXaxis()->SetMoreLogLabels();
  setup_Photonm1->GetXaxis()->SetNoExponent();
  canv_Photonm1->SetLogx();
  setup_Photonm1->GetYaxis()->SetTitleOffset(1.5); 
  setup_Photonm1->GetXaxis()->SetTitleOffset(1.1);
  setup_Photonm1->GetYaxis()->SetTitle("Response ratio");
  setup_Photonm1->GetYaxis()->SetTitleSize(0.05);
  setup_Photonm1->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photonm1->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  TLine *line = new TLine(15,1,1258.25,1); 
  //Plot
  setup_Photonm1->Draw();
  line->Draw("SAME");
  h_Rjetb_Photonm1->Draw("SAMEP");
  h_Rjetc_Photonm1->Draw("SAMEP");
  h_Rjets_Photonm1->Draw("SAMEP");
  h_Rjetud_Photonm1->Draw("SAMEP");
  h_Rjetg_Photonm1->Draw("SAMEP");
  h_Rjet_Photonm1->Draw("SAMEP");
  lz_Rjet_Photonm1->Draw("SAMEP");

  TLatex *tex = new TLatex(); tex->SetNDC();
  tex->SetTextSize(0.04); tex->SetTextColor(kBlack);
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.35,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.35,0.2,"#alpha<0.3");

  //Save plot
  canv_Photonm1->Print("./plots/varPlots/Photon/varPhotonm1_all.pdf"); delete canv_Photonm1;

  //b jet
  TCanvas* canv_Photonm1b = new TCanvas("canv_Photonm1b","",500,500); 
  canv_Photonm1b->SetLogx();
  canv_Photonm1b->SetLeftMargin(0.15); 
  canv_Photonm1b->SetBottomMargin(0.13);
  TLegend* lz_Photonm1b = new TLegend(0.45,0.19,0.8,0.29); lz_Photonm1b->SetBorderSize(0);
  lz_Photonm1b->AddEntry(h_Rjet_Photonm1,  "All jets Photon-1%", "p");
  lz_Photonm1b->AddEntry(h_Rjetb_Photonm1, "b jets Photon-1%", "p");
  TH1D* setup_Photonm1b = new TH1D("setup_Photonm1b","", h_Rjet_Photonm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Photonm1b->SetStats(0);
  setup_Photonm1b->SetAxisRange(0.986,1.004,"Y");
  setup_Photonm1b->GetXaxis()->SetMoreLogLabels();  setup_Photonm1b->GetXaxis()->SetNoExponent();
  setup_Photonm1b->GetYaxis()->SetTitleOffset(1.5); 
  setup_Photonm1b->GetXaxis()->SetTitleOffset(1.1);
  setup_Photonm1b->GetYaxis()->SetTitle("F^{var}");
  setup_Photonm1b->GetYaxis()->SetTitleSize(0.05);
  setup_Photonm1b->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photonm1b->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Photonm1b->Draw();
  line->Draw("SAME");
  h_Rjetb_Photonm1->Draw("SAMEP");
  h_Rjet_Photonm1->Draw("SAMEP");
  lz_Photonm1b->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Photonm1b->Print("./plots/varPlots/Photon/varPhotonm1_b.pdf"); delete canv_Photonm1b;

  //c jet
  TCanvas* canv_Photonm1c = new TCanvas("canv_Photonm1c","",500,500); canv_Photonm1c->SetLogx();
  canv_Photonm1c->SetLeftMargin(0.15); canv_Photonm1c->SetBottomMargin(0.13);
  TLegend* lz_Photonm1c = new TLegend(0.45,0.19,0.8,0.29); lz_Photonm1c->SetBorderSize(0);
  lz_Photonm1c->AddEntry(h_Rjet_Photonm1,  "All jets Photon-1%", "p");
  lz_Photonm1c->AddEntry(h_Rjetc_Photonm1, "c jets Photon-1%", "p");
  TH1D* setup_Photonm1c = new TH1D("setup_Photonm1c","", h_Rjet_Photonm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Photonm1c->SetStats(0);
  setup_Photonm1c->SetAxisRange(0.986,1.004,"Y");
  setup_Photonm1c->GetXaxis()->SetMoreLogLabels();  setup_Photonm1c->GetXaxis()->SetNoExponent();
  setup_Photonm1c->GetYaxis()->SetTitleOffset(1.5); 
  setup_Photonm1c->GetXaxis()->SetTitleOffset(1.1);
  setup_Photonm1c->GetYaxis()->SetTitle("F^{var}");
  setup_Photonm1c->GetYaxis()->SetTitleSize(0.05);
  setup_Photonm1c->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photonm1c->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Photonm1c->Draw();
  line->Draw("SAME");
  h_Rjetc_Photonm1->Draw("SAMEP");
  h_Rjet_Photonm1->Draw("SAMEP");
  lz_Photonm1c->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Photonm1c->Print("./plots/varPlots/Photon/varPhotonm1_c.pdf"); delete canv_Photonm1c;

  //s jet
  TCanvas* canv_Photonm1s = new TCanvas("canv_Photonm1s","",500,500); canv_Photonm1s->SetLogx();
  canv_Photonm1s->SetLeftMargin(0.15); canv_Photonm1s->SetBottomMargin(0.13);
  TLegend* lz_Photonm1s = new TLegend(0.45,0.19,0.8,0.29); lz_Photonm1s->SetBorderSize(0);
  lz_Photonm1s->AddEntry(h_Rjet_Photonm1,  "All jets Photon-1%", "p");
  lz_Photonm1s->AddEntry(h_Rjets_Photonm1, "s jets Photon-1%", "p");
  TH1D* setup_Photonm1s = new TH1D("setup_Photonm1s","", h_Rjet_Photonm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Photonm1s->SetStats(0);
  setup_Photonm1s->SetAxisRange(0.986,1.004,"Y");
  setup_Photonm1s->GetXaxis()->SetMoreLogLabels();  setup_Photonm1s->GetXaxis()->SetNoExponent();
  setup_Photonm1s->GetYaxis()->SetTitleOffset(1.5); 
  setup_Photonm1s->GetXaxis()->SetTitleOffset(1.1);
  setup_Photonm1s->GetYaxis()->SetTitle("F^{var}");
  setup_Photonm1s->GetYaxis()->SetTitleSize(0.05);
  setup_Photonm1s->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photonm1s->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Photonm1s->Draw();
  line->Draw("SAME");
  h_Rjets_Photonm1->Draw("SAMEP");
  h_Rjet_Photonm1->Draw("SAMEP");
  lz_Photonm1s->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Photonm1s->Print("./plots/varPlots/Photon/varPhotonm1_s.pdf"); delete canv_Photonm1s;

  //ud jet
  TCanvas* canv_Photonm1ud = new TCanvas("canv_Photonm1ud","",500,500); canv_Photonm1ud->SetLogx();
  canv_Photonm1ud->SetLeftMargin(0.15); canv_Photonm1ud->SetBottomMargin(0.13);
  TLegend* lz_Photonm1ud = new TLegend(0.45,0.19,0.8,0.29); lz_Photonm1ud->SetBorderSize(0);
  lz_Photonm1ud->AddEntry(h_Rjet_Photonm1,  "All jets Photon-1%", "p");
  lz_Photonm1ud->AddEntry(h_Rjetud_Photonm1, "ud jets Photon-1%", "p");
  TH1D* setup_Photonm1ud = new TH1D("setup_Photonm1ud","", h_Rjet_Photonm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Photonm1ud->SetStats(0);
  setup_Photonm1ud->SetAxisRange(0.986,1.004,"Y");
  setup_Photonm1ud->GetXaxis()->SetMoreLogLabels();  setup_Photonm1ud->GetXaxis()->SetNoExponent();
  setup_Photonm1ud->GetYaxis()->SetTitleOffset(1.5); 
  setup_Photonm1ud->GetXaxis()->SetTitleOffset(1.1);
  setup_Photonm1ud->GetYaxis()->SetTitle("F^{var}");
  setup_Photonm1ud->GetYaxis()->SetTitleSize(0.05);
  setup_Photonm1ud->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photonm1ud->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Photonm1ud->Draw();
  line->Draw("SAME");
  h_Rjetud_Photonm1->Draw("SAMEP");
  h_Rjet_Photonm1->Draw("SAMEP");
  lz_Photonm1ud->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Photonm1ud->Print("./plots/varPlots/Photon/varPhotonm1_ud.pdf"); delete canv_Photonm1ud;

  //g jet
  TCanvas* canv_Photonm1g = new TCanvas("canv_Photonm1g","",500,500); canv_Photonm1g->SetLogx();
  canv_Photonm1g->SetLeftMargin(0.15); canv_Photonm1g->SetBottomMargin(0.13);
  TLegend* lz_Photonm1g = new TLegend(0.45,0.19,0.8,0.29); lz_Photonm1g->SetBorderSize(0);
  lz_Photonm1g->AddEntry(h_Rjet_Photonm1,  "All jets Photon-1%", "p");
  lz_Photonm1g->AddEntry(h_Rjetg_Photonm1, "g jets Photon-1%", "p");
  TH1D* setup_Photonm1g = new TH1D("setup_Photonm1g","", h_Rjet_Photonm1->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Photonm1g->SetStats(0);
  setup_Photonm1g->SetAxisRange(0.986,1.004,"Y");
  setup_Photonm1g->GetXaxis()->SetMoreLogLabels();  setup_Photonm1g->GetXaxis()->SetNoExponent();
  setup_Photonm1g->GetYaxis()->SetTitleOffset(1.5); 
  setup_Photonm1g->GetXaxis()->SetTitleOffset(1.1);
  setup_Photonm1g->GetYaxis()->SetTitle("F^{var}");
  setup_Photonm1g->GetYaxis()->SetTitleSize(0.05);
  setup_Photonm1g->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photonm1g->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Photonm1g->Draw();
  line->Draw("SAME");
  h_Rjetg_Photonm1->Draw("SAMEP");
  h_Rjet_Photonm1->Draw("SAMEP");
  lz_Photonm1g->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Photonm1g->Print("./plots/varPlots/Photon/varPhotonm1_g.pdf"); delete canv_Photonm1g;

// Photon -3%
  TCanvas* canv_Photonm3 = new TCanvas("canv_Photonm3","",500,500);
  canv_Photonm3->SetLeftMargin(0.15); canv_Photonm3->SetBottomMargin(0.13);

  h_Rjet_Photonm3->SetMarkerStyle(5);                  h_Rjet_Photonm3->SetMarkerColor(kBlack);
  h_Rjet_Photonm3->SetLineColor(kBlack);               h_Rjet_Photonm3->SetMarkerSize(0.8);
  h_Rjetb_Photonm3->SetMarkerStyle(kFullCircle);       h_Rjetb_Photonm3->SetMarkerColor(kRed+1);
  h_Rjetb_Photonm3->SetLineColor(kRed+1);              h_Rjetb_Photonm3->SetMarkerSize(0.8);
  h_Rjetc_Photonm3->SetMarkerStyle(kFullTriangleDown); h_Rjetc_Photonm3->SetMarkerColor(kGreen+3);
  h_Rjetc_Photonm3->SetLineColor(kGreen+3);            h_Rjetc_Photonm3->SetMarkerSize(0.8);
  h_Rjets_Photonm3->SetMarkerStyle(kFullTriangleUp);   h_Rjets_Photonm3->SetMarkerColor(kOrange+7);
  h_Rjets_Photonm3->SetLineColor(kOrange+7);           h_Rjets_Photonm3->SetMarkerSize(0.8);
  h_Rjetud_Photonm3->SetMarkerStyle(kFullDiamond);     h_Rjetud_Photonm3->SetMarkerColor(kMagenta+2);
  h_Rjetud_Photonm3->SetLineColor(kMagenta+2);         h_Rjetud_Photonm3->SetMarkerSize(0.8);
  h_Rjetg_Photonm3->SetMarkerStyle(kFullSquare);       h_Rjetg_Photonm3->SetMarkerColor(kBlue+1);
  h_Rjetg_Photonm3->SetLineColor(kBlue+1);             h_Rjetg_Photonm3->SetMarkerSize(0.8);

  h_Rjet_Photonm3->SetAxisRange(15, 1248,"X");
  h_Rjetb_Photonm3->SetAxisRange(15, 1248,"X");
  h_Rjetc_Photonm3->SetAxisRange(15, 1248,"X");
  h_Rjets_Photonm3->SetAxisRange(15, 1248,"X");
  h_Rjetud_Photonm3->SetAxisRange(15, 1248,"X");
  h_Rjetg_Photonm3->SetAxisRange(15, 1248,"X");

  //Legend
  TLegend* lz_Rjet_Photonm3 = new TLegend(0.2,0.65,0.5,0.85);
  lz_Rjet_Photonm3->SetBorderSize(0);
  lz_Rjet_Photonm3->AddEntry(h_Rjet_Photonm3,  "#bf{All jets Photon-3%}", "p");
  lz_Rjet_Photonm3->AddEntry(h_Rjetb_Photonm3, "#bf{b jets Photon-3%}", "p");
  lz_Rjet_Photonm3->AddEntry(h_Rjetc_Photonm3, "#bf{c jets Photon-3%}", "p");
  lz_Rjet_Photonm3->AddEntry(h_Rjets_Photonm3, "#bf{s jets Photon-3%}", "p");
  lz_Rjet_Photonm3->AddEntry(h_Rjetud_Photonm3, "#bf{ud jets Photon-3%}", "p");
  lz_Rjet_Photonm3->AddEntry(h_Rjetg_Photonm3,  "#bf{g jets Photon-3%}", "p");
  //lz_Rjet_Photonm3->SetNColumns(2);

  TH1D* setup_Photonm3 = new TH1D("setup_Photonm3","", h_Rjet_Photonm3->GetXaxis()->GetNbins(),
  15, 1248);
  setup_Photonm3->SetStats(0);
  setup_Photonm3->SetAxisRange(0.98,1.03,"Y");
  setup_Photonm3->GetXaxis()->SetMoreLogLabels();
  setup_Photonm3->GetXaxis()->SetNoExponent();
  canv_Photonm3->SetLogx();
  setup_Photonm3->GetYaxis()->SetTitleOffset(1.5); 
  setup_Photonm3->GetXaxis()->SetTitleOffset(1.1);
  setup_Photonm3->GetYaxis()->SetTitle("Response ratio");
  setup_Photonm3->GetYaxis()->SetTitleSize(0.05);
  setup_Photonm3->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photonm3->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  //Plot
  setup_Photonm3->Draw();
  line->Draw("SAME");
  h_Rjetb_Photonm3->Draw("SAMEP");
  h_Rjetc_Photonm3->Draw("SAMEP");
  h_Rjets_Photonm3->Draw("SAMEP");
  h_Rjetud_Photonm3->Draw("SAMEP");
  h_Rjetg_Photonm3->Draw("SAMEP");
  h_Rjet_Photonm3->Draw("SAMEP");
  lz_Rjet_Photonm3->Draw("SAMEP");

  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.25,0.21,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.25,0.16,"#alpha<0.3");

  //Save plot
  canv_Photonm3->Print("./plots/varPlots/Photon/varPhotonm3_all.pdf"); delete canv_Photonm3;

  //b jet
  TCanvas* canv_Photonm3b = new TCanvas("canv_Photonm3b","",500,500); 
  canv_Photonm3b->SetLogx();
  canv_Photonm3b->SetLeftMargin(0.15); 
  canv_Photonm3b->SetBottomMargin(0.13);
  TLegend* lz_Photonm3b = new TLegend(0.45,0.19,0.8,0.29); lz_Photonm3b->SetBorderSize(0);
  lz_Photonm3b->AddEntry(h_Rjet_Photonm3,  "All jets Photon-3%", "p");
  lz_Photonm3b->AddEntry(h_Rjetb_Photonm3, "b jets Photon-3%", "p");
  TH1D* setup_Photonm3b = new TH1D("setup_Photonm3b","", h_Rjet_Photonm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Photonm3b->SetStats(0);
  setup_Photonm3b->SetAxisRange(0.986,1.004,"Y");
  setup_Photonm3b->GetXaxis()->SetMoreLogLabels();  setup_Photonm3b->GetXaxis()->SetNoExponent();
  setup_Photonm3b->GetYaxis()->SetTitleOffset(1.5); 
  setup_Photonm3b->GetXaxis()->SetTitleOffset(1.1);
  setup_Photonm3b->GetYaxis()->SetTitle("F^{var}");
  setup_Photonm3b->GetYaxis()->SetTitleSize(0.05);
  setup_Photonm3b->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photonm3b->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Photonm3b->Draw();
  line->Draw("SAME");
  h_Rjetb_Photonm3->Draw("SAMEP");
  h_Rjet_Photonm3->Draw("SAMEP");
  lz_Photonm3b->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Photonm3b->Print("./plots/varPlots/Photon/varPhotonm3_b.pdf"); delete canv_Photonm3b;

  //c jet
  TCanvas* canv_Photonm3c = new TCanvas("canv_Photonm3c","",500,500); canv_Photonm3c->SetLogx();
  canv_Photonm3c->SetLeftMargin(0.15); canv_Photonm3c->SetBottomMargin(0.13);
  TLegend* lz_Photonm3c = new TLegend(0.45,0.19,0.8,0.29); lz_Photonm3c->SetBorderSize(0);
  lz_Photonm3c->AddEntry(h_Rjet_Photonm3,  "All jets Photon-3%", "p");
  lz_Photonm3c->AddEntry(h_Rjetc_Photonm3, "c jets Photon-3%", "p");
  TH1D* setup_Photonm3c = new TH1D("setup_Photonm3c","", h_Rjet_Photonm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Photonm3c->SetStats(0);
  setup_Photonm3c->SetAxisRange(0.986,1.004,"Y");
  setup_Photonm3c->GetXaxis()->SetMoreLogLabels();  setup_Photonm3c->GetXaxis()->SetNoExponent();
  setup_Photonm3c->GetYaxis()->SetTitleOffset(1.5); 
  setup_Photonm3c->GetXaxis()->SetTitleOffset(1.1);
  setup_Photonm3c->GetYaxis()->SetTitle("F^{var}");
  setup_Photonm3c->GetYaxis()->SetTitleSize(0.05);
  setup_Photonm3c->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photonm3c->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Photonm3c->Draw();
  line->Draw("SAME");
  h_Rjetc_Photonm3->Draw("SAMEP");
  h_Rjet_Photonm3->Draw("SAMEP");
  lz_Photonm3c->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Photonm3c->Print("./plots/varPlots/Photon/varPhotonm3_c.pdf"); delete canv_Photonm3c;

  //s jet
  TCanvas* canv_Photonm3s = new TCanvas("canv_Photonm3s","",500,500); canv_Photonm3s->SetLogx();
  canv_Photonm3s->SetLeftMargin(0.15); canv_Photonm3s->SetBottomMargin(0.13);
  TLegend* lz_Photonm3s = new TLegend(0.45,0.19,0.8,0.29); lz_Photonm3s->SetBorderSize(0);
  lz_Photonm3s->AddEntry(h_Rjet_Photonm3,  "All jets Photon-3%", "p");
  lz_Photonm3s->AddEntry(h_Rjets_Photonm3, "s jets Photon-3%", "p");
  TH1D* setup_Photonm3s = new TH1D("setup_Photonm3s","", h_Rjet_Photonm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Photonm3s->SetStats(0);
  setup_Photonm3s->SetAxisRange(0.986,1.004,"Y");
  setup_Photonm3s->GetXaxis()->SetMoreLogLabels();  setup_Photonm3s->GetXaxis()->SetNoExponent();
  setup_Photonm3s->GetYaxis()->SetTitleOffset(1.5); 
  setup_Photonm3s->GetXaxis()->SetTitleOffset(1.1);
  setup_Photonm3s->GetYaxis()->SetTitle("F^{var}");
  setup_Photonm3s->GetYaxis()->SetTitleSize(0.05);
  setup_Photonm3s->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photonm3s->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Photonm3s->Draw();
  line->Draw("SAME");
  h_Rjets_Photonm3->Draw("SAMEP");
  h_Rjet_Photonm3->Draw("SAMEP");
  lz_Photonm3s->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Photonm3s->Print("./plots/varPlots/Photon/varPhotonm3_s.pdf"); delete canv_Photonm3s;

  //ud jet
  TCanvas* canv_Photonm3ud = new TCanvas("canv_Photonm3ud","",500,500); canv_Photonm3ud->SetLogx();
  canv_Photonm3ud->SetLeftMargin(0.15); canv_Photonm3ud->SetBottomMargin(0.13);
  TLegend* lz_Photonm3ud = new TLegend(0.45,0.19,0.8,0.29); lz_Photonm3ud->SetBorderSize(0);
  lz_Photonm3ud->AddEntry(h_Rjet_Photonm3,  "All jets Photon-3%", "p");
  lz_Photonm3ud->AddEntry(h_Rjetud_Photonm3, "ud jets Photon-3%", "p");
  TH1D* setup_Photonm3ud = new TH1D("setup_Photonm3ud","", h_Rjet_Photonm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Photonm3ud->SetStats(0);
  setup_Photonm3ud->SetAxisRange(0.986,1.004,"Y");
  setup_Photonm3ud->GetXaxis()->SetMoreLogLabels();  setup_Photonm3ud->GetXaxis()->SetNoExponent();
  setup_Photonm3ud->GetYaxis()->SetTitleOffset(1.5); 
  setup_Photonm3ud->GetXaxis()->SetTitleOffset(1.1);
  setup_Photonm3ud->GetYaxis()->SetTitle("F^{var}");
  setup_Photonm3ud->GetYaxis()->SetTitleSize(0.05);
  setup_Photonm3ud->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photonm3ud->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Photonm3ud->Draw();
  line->Draw("SAME");
  h_Rjetud_Photonm3->Draw("SAMEP");
  h_Rjet_Photonm3->Draw("SAMEP");
  lz_Photonm3ud->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Photonm3ud->Print("./plots/varPlots/Photon/varPhotonm3_ud.pdf"); delete canv_Photonm3ud;

  //g jet
  TCanvas* canv_Photonm3g = new TCanvas("canv_Photonm3g","",500,500); canv_Photonm3g->SetLogx();
  canv_Photonm3g->SetLeftMargin(0.15); canv_Photonm3g->SetBottomMargin(0.13);
  TLegend* lz_Photonm3g = new TLegend(0.45,0.19,0.8,0.29); lz_Photonm3g->SetBorderSize(0);
  lz_Photonm3g->AddEntry(h_Rjet_Photonm3,  "All jets Photon-3%", "p");
  lz_Photonm3g->AddEntry(h_Rjetg_Photonm3, "g jets Photon-3%", "p");
  TH1D* setup_Photonm3g = new TH1D("setup_Photonm3g","", h_Rjet_Photonm3->GetXaxis()->GetNbins(),
  28, 1240);
  setup_Photonm3g->SetStats(0);
  setup_Photonm3g->SetAxisRange(0.986,1.004,"Y");
  setup_Photonm3g->GetXaxis()->SetMoreLogLabels();  setup_Photonm3g->GetXaxis()->SetNoExponent();
  setup_Photonm3g->GetYaxis()->SetTitleOffset(1.5); 
  setup_Photonm3g->GetXaxis()->SetTitleOffset(1.1);
  setup_Photonm3g->GetYaxis()->SetTitle("F^{var}");
  setup_Photonm3g->GetYaxis()->SetTitleSize(0.05);
  setup_Photonm3g->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_Photonm3g->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup_Photonm3g->Draw();
  line->Draw("SAME");
  h_Rjetg_Photonm3->Draw("SAMEP");
  h_Rjet_Photonm3->Draw("SAMEP");
  lz_Photonm3g->Draw("SAMEP");
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");
  canv_Photonm3g->Print("./plots/varPlots/Photon/varPhotonm3_g.pdf"); delete canv_Photonm3g;


  gApplication->Terminate(0); //Exit root command prompt

}
