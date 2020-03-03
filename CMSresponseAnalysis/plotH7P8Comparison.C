void plotH7P8Comparison()
{
  //Open files
  TFile* fP8 = TFile::Open("./output_ROOT_files/CMSJES_P8_Zjet_1000000.root");
  TFile* fH7 = TFile::Open("./output_ROOT_files/CMSJES_H7_Zjet_1000000.root");

  TProfile* pr_P8_Rjet;
  TProfile* pr_P8_Rjetb;
  TProfile* pr_P8_Rjetc;
  TProfile* pr_P8_Rjets;
  TProfile* pr_P8_Rjetud;
  TProfile* pr_P8_Rjetg;

  TProfile* pr_H7_Rjet;
  TProfile* pr_H7_Rjetb;
  TProfile* pr_H7_Rjetc;
  TProfile* pr_H7_Rjets;
  TProfile* pr_H7_Rjetud;
  TProfile* pr_H7_Rjetg;

  //Get response profiles
  fP8->GetObject("prRjet",   pr_P8_Rjet);
  fP8->GetObject("prRjetb",  pr_P8_Rjetb);
  fP8->GetObject("prRjetc",  pr_P8_Rjetc);
  fP8->GetObject("prRjets",  pr_P8_Rjets);
  fP8->GetObject("prRjetud", pr_P8_Rjetud);
  fP8->GetObject("prRjetg",  pr_P8_Rjetg);

  fH7->GetObject("prRjet",   pr_H7_Rjet);
  fH7->GetObject("prRjetb",  pr_H7_Rjetb);
  fH7->GetObject("prRjetc",  pr_H7_Rjetc);
  fH7->GetObject("prRjets",  pr_H7_Rjets);
  fH7->GetObject("prRjetud", pr_H7_Rjetud);
  fH7->GetObject("prRjetg",  pr_H7_Rjetg);

  //Change to histograms
  TH1D* h_P8_Rjet   = pr_P8_Rjet  ->ProjectionX();
  TH1D* h_P8_Rjetb  = pr_P8_Rjetb ->ProjectionX();
  TH1D* h_P8_Rjetc  = pr_P8_Rjetc ->ProjectionX();
  TH1D* h_P8_Rjets  = pr_P8_Rjets ->ProjectionX();
  TH1D* h_P8_Rjetud = pr_P8_Rjetud->ProjectionX();
  TH1D* h_P8_Rjetg  = pr_P8_Rjetg ->ProjectionX();

  TH1D* h_H7_Rjet   = pr_H7_Rjet  ->ProjectionX();
  TH1D* h_H7_Rjetb  = pr_H7_Rjetb ->ProjectionX();
  TH1D* h_H7_Rjetc  = pr_H7_Rjetc ->ProjectionX();
  TH1D* h_H7_Rjets  = pr_H7_Rjets ->ProjectionX();
  TH1D* h_H7_Rjetud = pr_H7_Rjetud->ProjectionX();
  TH1D* h_H7_Rjetg  = pr_H7_Rjetg ->ProjectionX();


////////////////////// PLOTTING //////////////////////////
  TCanvas* canv = new TCanvas("canv","",500,400);

  //Pythia8
  h_P8_Rjet->SetMarkerStyle(2);                    h_P8_Rjet->SetMarkerColor(kBlack);
  h_P8_Rjet->SetLineColor(kBlack);                 h_P8_Rjet->SetMarkerSize(0.8);
  h_P8_Rjetb->SetMarkerStyle(kFullCircle);       h_P8_Rjetb->SetMarkerColor(kRed+1);
  h_P8_Rjetb->SetLineColor(kRed+1);                h_P8_Rjetb->SetMarkerSize(0.8);
  h_P8_Rjetc->SetMarkerStyle(kFullTriangleDown); h_P8_Rjetc->SetMarkerColor(kGreen+3);
  h_P8_Rjetc->SetLineColor(kGreen+3);              h_P8_Rjetc->SetMarkerSize(0.8);
  h_P8_Rjets->SetMarkerStyle(kFullTriangleUp);   h_P8_Rjets->SetMarkerColor(kOrange+7);
  h_P8_Rjets->SetLineColor(kOrange+7);             h_P8_Rjets->SetMarkerSize(0.8);
  h_P8_Rjetud->SetMarkerStyle(kFullDiamond);     h_P8_Rjetud->SetMarkerColor(kMagenta+2);
  h_P8_Rjetud->SetLineColor(kMagenta+2);           h_P8_Rjetud->SetMarkerSize(0.8);
  h_P8_Rjetg->SetMarkerStyle(kFullSquare);        h_P8_Rjetg->SetMarkerColor(kBlue+1);
  h_P8_Rjetg->SetLineColor(kBlue+1);                h_P8_Rjetg->SetMarkerSize(0.8);

  //Herwig7
  h_H7_Rjet->SetMarkerStyle(5);                  h_H7_Rjet->SetMarkerColor(kBlack);
  h_H7_Rjet->SetLineColor(kBlack);               h_H7_Rjet->SetMarkerSize(0.8);
  h_H7_Rjetb->SetMarkerStyle(kOpenCircle);       h_H7_Rjetb->SetMarkerColor(kRed+1);
  h_H7_Rjetb->SetLineColor(kRed+1) ;             h_H7_Rjetb->SetMarkerSize(0.8);
  h_H7_Rjetc->SetMarkerStyle(kOpenTriangleDown); h_H7_Rjetc->SetMarkerColor(kGreen+3);
  h_H7_Rjetc->SetLineColor(kGreen+3);            h_H7_Rjetc->SetMarkerSize(0.8);
  h_H7_Rjets->SetMarkerStyle(kOpenTriangleUp);   h_H7_Rjets->SetMarkerColor(kOrange+7);
  h_H7_Rjets->SetLineColor(kOrange+7);           h_H7_Rjets->SetMarkerSize(0.8);
  h_H7_Rjetud->SetMarkerStyle(kOpenDiamond);     h_H7_Rjetud->SetMarkerColor(kMagenta+2);
  h_H7_Rjetud->SetLineColor(kMagenta+2);         h_H7_Rjetud->SetMarkerSize(0.8);
  h_H7_Rjetg->SetMarkerStyle(kOpenSquare);        h_H7_Rjetg->SetMarkerColor(kBlue+1);
  h_H7_Rjetg->SetLineColor(kBlue+1);              h_H7_Rjetg->SetMarkerSize(0.8);



  //Legend
  TLegend* lz_Rjet = new TLegend(0.4,0.18,0.7,0.4);
  lz_Rjet->SetBorderSize(0);
  lz_Rjet->AddEntry(h_P8_Rjet,   "All [P8]", "p");
  lz_Rjet->AddEntry(h_H7_Rjet,   "All [H7]", "p");
  lz_Rjet->AddEntry(h_P8_Rjetb,  "b [P8]", "p");
  lz_Rjet->AddEntry(h_H7_Rjetb,  "b [H7]", "p");
  lz_Rjet->AddEntry(h_P8_Rjetc,  "c [P8]", "p");
  lz_Rjet->AddEntry(h_H7_Rjetc,  "c [H7]", "p");
  lz_Rjet->AddEntry(h_P8_Rjets,  "s [P8]", "p");
  lz_Rjet->AddEntry(h_H7_Rjets,  "s [H7]", "p");
  lz_Rjet->AddEntry(h_P8_Rjetud, "ud [P8]", "p");
  lz_Rjet->AddEntry(h_H7_Rjetud, "ud [H7]", "p");
  lz_Rjet->AddEntry(h_P8_Rjetg,  "g [P8]", "p");
  lz_Rjet->AddEntry(h_H7_Rjetg,  "g [H7]", "p");
  lz_Rjet->SetNColumns(2);



  TH1D* setup = new TH1D("setup","", h_P8_Rjet->GetXaxis()->GetNbins(),
		                     h_P8_Rjet->GetXaxis()->GetXmin(), 
                                     h_P8_Rjet->GetXaxis()->GetXmax());
  setup->SetStats(0); //Suppress stat box
  setup->SetAxisRange(0.902,0.95,"Y"); //Vertical axis limits
  setup->GetXaxis()->SetMoreLogLabels();
  setup->GetXaxis()->SetNoExponent();
  setup->GetYaxis()->SetTitleOffset(1.3);
  setup->GetXaxis()->SetTitleOffset(1.2);
  setup->GetYaxis()->SetTitle("R_{jet}");
  setup->GetXaxis()->SetTitle("p_{T,gen}^{jet} [GeV]");
  setup->GetXaxis()->SetTitleSize(0.042);
  setup->GetYaxis()->SetTitleSize(0.042);

  canv->SetLogx();
  canv->SetLeftMargin(0.12);
  canv->SetBottomMargin(0.12);

  TLatex *tex = new TLatex();
  tex->SetNDC();
  tex->SetTextSize(0.04);
  tex->SetTextColor(kBlack);

  gPad->SetTickx(); gPad->SetTicky();

  //Plot
  setup->Draw();
  h_P8_Rjet->Draw("SAMEP");
  h_H7_Rjet->Draw("SAMEP");
  h_P8_Rjetb->Draw("SAMEP");
  h_H7_Rjetb->Draw("SAMEP");
  h_P8_Rjetc->Draw("SAMEP");
  h_H7_Rjetc->Draw("SAMEP");
  h_P8_Rjets->Draw("SAMEP");
  h_H7_Rjets->Draw("SAMEP");
  h_P8_Rjetud->Draw("SAMEP");
  h_H7_Rjetud->Draw("SAMEP");
  h_P8_Rjetg->Draw("SAMEP");
  h_H7_Rjetg->Draw("SAMEP");
  lz_Rjet->Draw("SAMEP");

  tex->DrawLatex(0.72,0.8,"|#eta^{jet}|<1.3");
  //Save plot
  canv->Print("./plots/H7P8Comparison/H7P8Rjet.pdf"); delete canv;

//Difference plot  
  TCanvas* canv2 = new TCanvas("canv2","",500,400);

  h_H7_Rjet->Divide(h_P8_Rjet);
  h_H7_Rjetb->Divide(h_P8_Rjetb);
  h_H7_Rjetc->Divide(h_P8_Rjetc);
  h_H7_Rjets->Divide(h_P8_Rjets);
  h_H7_Rjetud->Divide(h_P8_Rjetud);
  h_H7_Rjetg->Divide(h_P8_Rjetg);


  //Legend
  TLegend* lz_Rjet2 = new TLegend(0.7,0.62,0.8,0.85);
  lz_Rjet2->SetBorderSize(0);
  lz_Rjet2->AddEntry(h_H7_Rjet,   "#bf{All}", "p");
  lz_Rjet2->AddEntry(h_H7_Rjetb,  "#bf{b}", "p");
  lz_Rjet2->AddEntry(h_H7_Rjetc,  "#bf{c}", "p");
  lz_Rjet2->AddEntry(h_H7_Rjets,  "#bf{s}", "p");
  lz_Rjet2->AddEntry(h_H7_Rjetud, "#bf{ud}", "p");
  lz_Rjet2->AddEntry(h_H7_Rjetg,  "#bf{g}", "p");

  TH1D* setup2 = new TH1D("setup2","", h_H7_Rjet->GetXaxis()->GetNbins(),
		                       h_H7_Rjet->GetXaxis()->GetXmin(), 
                                       h_H7_Rjet->GetXaxis()->GetXmax());
  setup2->SetStats(0); //Suppress stat box
  setup2->SetAxisRange(0.987,1.017,"Y"); //Vertical axis limits
  setup2->GetXaxis()->SetMoreLogLabels();
  setup2->GetXaxis()->SetNoExponent();
  setup2->GetYaxis()->SetTitleOffset(1.4);
  setup2->GetXaxis()->SetTitleOffset(1.2);
  setup2->GetYaxis()->SetTitle("R_{jet}^{H7} / R_{jet}^{P8}");
  setup2->GetXaxis()->SetTitle("p_{T,gen}^{jet} [GeV]");
  setup2->GetXaxis()->SetTitleSize(0.042);
  setup2->GetYaxis()->SetTitleSize(0.042);

  canv2->SetLogx();
  canv2->SetLeftMargin(0.12);
  canv2->SetBottomMargin(0.12);

  gPad->SetTickx();   gPad->SetTicky();

  TLine *line = new TLine(31.75,1,1258.25,1); 


  setup2->Draw();
  line->Draw("SAME");
  h_H7_Rjet->Draw("SAMEP");
  h_H7_Rjetb->Draw("SAMEP");
  h_H7_Rjetc->Draw("SAMEP");
  h_H7_Rjets->Draw("SAMEP");
  h_H7_Rjetud->Draw("SAMEP");
  h_H7_Rjetg->Draw("SAMEP");
  lz_Rjet2->Draw("SAMEP");

  tex->DrawLatex(0.21,0.8,"|#eta^{jet}|<1.3");

  canv2->Print("./plots/H7P8Comparison/H7P8Rjet_divide.pdf"); delete canv2;

  TCanvas* canvall = new TCanvas("canvall","",500,400);
  TLegend* lz_Rjetall = new TLegend(0.7,0.62,0.8,0.85);
  lz_Rjetall->SetBorderSize(0);
  lz_Rjetall->AddEntry(h_H7_Rjet,  "#bf{all jets}", "p");
  TH1D* setupall = new TH1D("setupall","", h_H7_Rjet->GetXaxis()->GetNbins(),
                           h_H7_Rjet->GetXaxis()->GetXmin(), h_H7_Rjet->GetXaxis()->GetXmax());
  canvall->SetLogx();
  canvall->SetLeftMargin(0.12);
  canvall->SetBottomMargin(0.12);
  gPad->SetTickx();   gPad->SetTicky();
  setup2->Draw();
  line->Draw("SAME");
  h_H7_Rjet->Draw("SAMEP");
  lz_Rjetall->Draw("SAMEP");
  tex->DrawLatex(0.21,0.8,"|#eta^{jet}|<1.3");
  canvall->Print("./plots/H7P8Comparison/H7P8Rjetall_divide.pdf"); delete canvall;

// b jets
  TCanvas* canvb = new TCanvas("canv2","",500,400);
  TLegend* lz_Rjetb = new TLegend(0.7,0.62,0.8,0.85);
  lz_Rjetb->SetBorderSize(0);
  lz_Rjetb->AddEntry(h_H7_Rjetb,  "#bf{b jets}", "p");
  TH1D* setupb = new TH1D("setupb","", h_H7_Rjet->GetXaxis()->GetNbins(),
                           h_H7_Rjet->GetXaxis()->GetXmin(), h_H7_Rjet->GetXaxis()->GetXmax());
  canvb->SetLogx();
  canvb->SetLeftMargin(0.12);
  canvb->SetBottomMargin(0.12);
  gPad->SetTickx();   gPad->SetTicky();
  setup2->Draw();
  line->Draw("SAME");
  h_H7_Rjetb->Draw("SAMEP");
  lz_Rjetb->Draw("SAMEP");
  tex->DrawLatex(0.21,0.8,"|#eta^{jet}|<1.3");
  canvb->Print("./plots/H7P8Comparison/H7P8Rjetb_divide.pdf"); delete canvb;


// c jets
  TCanvas* canvc = new TCanvas("canv2","",500,400);
  TLegend* lz_Rjetc = new TLegend(0.7,0.62,0.8,0.85);
  lz_Rjetc->SetBorderSize(0);
  lz_Rjetc->AddEntry(h_H7_Rjetc,  "#bf{c jets}", "p");
  canvc->SetLogx();
  canvc->SetLeftMargin(0.12);
  canvc->SetBottomMargin(0.12);
  gPad->SetTickx();   gPad->SetTicky();
  setup2->Draw();
  line->Draw("SAME");
  h_H7_Rjetc->Draw("SAMEP");
  lz_Rjetc->Draw("SAMEP");
  tex->DrawLatex(0.21,0.8,"|#eta^{jet}|<1.3");
  canvc->Print("./plots/H7P8Comparison/H7P8Rjetc_divide.pdf"); delete canvc;


// s jets
  TCanvas* canvs = new TCanvas("canv2","",500,400);
  TLegend* lz_Rjets = new TLegend(0.7,0.62,0.8,0.85);
  lz_Rjets->SetBorderSize(0);
  lz_Rjets->AddEntry(h_H7_Rjets,  "#bf{s jets}", "p");
  canvs->SetLogx();
  canvs->SetLeftMargin(0.12);
  canvs->SetBottomMargin(0.12);
  gPad->SetTickx();   gPad->SetTicky();
  setup2->Draw();
  line->Draw("SAME");
  h_H7_Rjets->Draw("SAMEP");
  lz_Rjets->Draw("SAMEP");
  tex->DrawLatex(0.21,0.8,"|#eta^{jet}|<1.3");
  canvs->Print("./plots/H7P8Comparison/H7P8Rjets_divide.pdf"); delete canvs;


// ud jets
  TCanvas* canvud = new TCanvas("canv2","",500,400);
  TLegend* lz_Rjetud = new TLegend(0.7,0.62,0.82,0.85);
  lz_Rjetud->SetBorderSize(0);
  lz_Rjetud->AddEntry(h_H7_Rjetud,  "#bf{ud jets}", "p");
  canvud->SetLogx();
  canvud->SetLeftMargin(0.12);
  canvud->SetBottomMargin(0.12);
  gPad->SetTickx();   gPad->SetTicky();
  setup2->Draw();
  line->Draw("SAME");
  h_H7_Rjetud->Draw("SAMEP");
  lz_Rjetud->Draw("SAMEP");
  tex->DrawLatex(0.21,0.8,"|#eta^{jet}|<1.3");
  canvud->Print("./plots/H7P8Comparison/H7P8Rjetud_divide.pdf"); delete canvud;


// g jets
  TCanvas* canvg = new TCanvas("canv2","",500,400);
  TLegend* lz_Rjetg = new TLegend(0.7,0.62,0.8,0.85);
  lz_Rjetg->SetBorderSize(0);
  lz_Rjetg->AddEntry(h_H7_Rjetg,  "#bf{g jets}", "p");
  canvg->SetLogx();
  canvg->SetLeftMargin(0.12);
  canvg->SetBottomMargin(0.12);
  gPad->SetTickx();   gPad->SetTicky();
  setup2->Draw();
  line->Draw("SAME");
  h_H7_Rjetg->Draw("SAMEP");
  lz_Rjetg->Draw("SAMEP");
  tex->DrawLatex(0.21,0.8,"|#eta^{jet}|<1.3");
  canvg->Print("./plots/H7P8Comparison/H7P8Rjetg_divide.pdf"); delete canvg;

  gApplication->Terminate(0); //Exit root command prompt
}
































