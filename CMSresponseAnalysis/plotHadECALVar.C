void plotHadECALVar()
{

  double markersize = 0.8;

  //Variants file
  TFile* fzj = TFile::Open("./output_ROOT_files/varPlots_P8_Zjet_5000000.root");

  TH1D* h_Rjet_HadECALp3;
  TH1D* h_Rjet_HadECALm3;
  TH1D* h_Rjetb_HadECALp3;
  TH1D* h_Rjetb_HadECALm3;
  TH1D* h_Rjetc_HadECALp3;
  TH1D* h_Rjetc_HadECALm3;
  TH1D* h_Rjets_HadECALp3;
  TH1D* h_Rjets_HadECALm3;
  TH1D* h_Rjetud_HadECALp3;
  TH1D* h_Rjetud_HadECALm3;
  TH1D* h_Rjetg_HadECALp3;
  TH1D* h_Rjetg_HadECALm3;


  //Create Histograms
  fzj->GetObject("h_Rjet_HadECALp3", h_Rjet_HadECALp3);
  fzj->GetObject("h_Rjet_HadECALm3", h_Rjet_HadECALm3);

  fzj->GetObject("h_Rjetb_HadECALp3", h_Rjetb_HadECALp3);
  fzj->GetObject("h_Rjetb_HadECALm3", h_Rjetb_HadECALm3);

  fzj->GetObject("h_Rjetc_HadECALp3", h_Rjetc_HadECALp3);
  fzj->GetObject("h_Rjetc_HadECALm3", h_Rjetc_HadECALm3);

  fzj->GetObject("h_Rjets_HadECALp3", h_Rjets_HadECALp3);
  fzj->GetObject("h_Rjets_HadECALm3", h_Rjets_HadECALm3);

  fzj->GetObject("h_Rjetud_HadECALp3", h_Rjetud_HadECALp3);
  fzj->GetObject("h_Rjetud_HadECALm3", h_Rjetud_HadECALm3);

  fzj->GetObject("h_Rjetg_HadECALp3", h_Rjetg_HadECALp3);
  fzj->GetObject("h_Rjetg_HadECALm3", h_Rjetg_HadECALm3);

// ECAL
  TCanvas* canv_HadECAL = new TCanvas("canv_HadECAL","",500,500);
  canv_HadECAL->SetLeftMargin(0.15); canv_HadECAL->SetBottomMargin(0.13);

  TGraph *JEC_ECALp3 = new TGraph("data_and_MC_input/Response/JEC_PFJet_ECALplus3.txt" );
  TGraph *JEC_ECALm3 = new TGraph("data_and_MC_input/Response/JEC_PFJet_ECALminus3.txt" );

  JEC_ECALp3->SetMarkerStyle(kOpenCircle); JEC_ECALp3->SetMarkerColor(kRed);
  JEC_ECALm3->SetMarkerStyle(kOpenSquare); JEC_ECALm3->SetMarkerColor(kBlue+1);
  JEC_ECALp3->SetMarkerSize(markersize);
  JEC_ECALm3->SetMarkerSize(markersize);

  h_Rjet_HadECALp3->SetMarkerStyle(kFullCircle); h_Rjet_HadECALm3->SetMarkerStyle(kFullSquare);
  h_Rjet_HadECALp3->SetMarkerColor(kRed);    h_Rjet_HadECALm3->SetMarkerColor(kBlue+1);
  h_Rjet_HadECALp3->SetLineColor(kRed);      h_Rjet_HadECALm3->SetLineColor(kBlue+1);
  h_Rjet_HadECALp3->SetMarkerSize(markersize); h_Rjet_HadECALm3->SetMarkerSize(markersize);

  TLegend* lz_HadECAL = new TLegend(0.18,0.73,0.75,0.87);
  //lz_ECAL->SetNColumns(2);
  lz_HadECAL->SetBorderSize(0);
  lz_HadECAL->AddEntry(h_Rjet_HadECALp3,  "Hadron ECAL+3% (Our)", "p");
  lz_HadECAL->AddEntry(JEC_ECALp3,     "Hadron ECAL+3% (FastSim)", "p");
  lz_HadECAL->AddEntry(h_Rjet_HadECALm3,  "Hadron ECAL-3% (Our)", "p");
  lz_HadECAL->AddEntry(JEC_ECALm3,     "Hadron ECAL-3% (FastSim)", "p"); 

  TH1D* setup = new TH1D("setup","", h_Rjet_HadECALp3->GetXaxis()->GetNbins(), 28, 1240);
  //Title and axis setup
  setup->SetStats(0); //Suppress stat box
  setup->SetAxisRange(0.99,1.011,"Y"); //Vertical axis limits

  h_Rjet_HadECALp3->SetAxisRange(28,1248,"X");
  h_Rjet_HadECALm3->SetAxisRange(28,1248,"X");

  setup->GetXaxis()->SetMoreLogLabels();
  setup->GetXaxis()->SetNoExponent();
  canv_HadECAL->SetLogx();
  setup->GetYaxis()->SetTitleOffset(1.5);
  setup->GetXaxis()->SetTitleOffset(1.1);
  setup->GetYaxis()->SetTitle("F^{var}");
  setup->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup->GetYaxis()->SetTitleSize(0.05);
  setup->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();



  //Plot
  setup->Draw();
  JEC_ECALp3->Draw("SAMEP");
  JEC_ECALm3->Draw("SAMEP");
  h_Rjet_HadECALp3->Draw("SAMEP");
  h_Rjet_HadECALm3->Draw("SAMEP");
  lz_HadECAL->Draw("SAMEP");

  TLatex *tex = new TLatex(); tex->SetNDC();
  tex->SetTextSize(0.04); tex->SetTextColor(kBlack);
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");

  setup->GetXaxis()->SetRangeUser(28,2000);
  gPad->Modified();

  //Save plot
  canv_HadECAL->Print("./plots/varPlots/hadECAL/varHadECAL.pdf"); delete canv_HadECAL;


// C +-3%

  TCanvas* canv_HadECAL_all = new TCanvas("canv_HadECAL_all","",500,500);
  canv_HadECAL_all->SetLeftMargin(0.15); canv_HadECAL_all->SetBottomMargin(0.13);

  h_Rjet_HadECALp3->SetMarkerStyle(2);             h_Rjet_HadECALm3->SetMarkerStyle(5);
  h_Rjet_HadECALp3->SetMarkerColor(kBlack);        h_Rjet_HadECALm3->SetMarkerColor(kBlack);
  h_Rjet_HadECALp3->SetLineColor(kBlack);          h_Rjet_HadECALm3->SetLineColor(kBlack);
  h_Rjet_HadECALp3->SetMarkerSize(markersize);     h_Rjet_HadECALm3->SetMarkerSize(markersize);

  h_Rjetb_HadECALp3->SetMarkerStyle(kFullCircle);  h_Rjetb_HadECALm3->SetMarkerStyle(kOpenCircle);
  h_Rjetb_HadECALp3->SetMarkerColor(kRed+1);       h_Rjetb_HadECALm3->SetMarkerColor(kRed+1);
  h_Rjetb_HadECALp3->SetLineColor(kRed+1);         h_Rjetb_HadECALm3->SetLineColor(kRed+1);
  h_Rjetb_HadECALp3->SetMarkerSize(markersize);    h_Rjetb_HadECALm3->SetMarkerSize(markersize);

  h_Rjetc_HadECALp3->SetMarkerStyle(kFullTriangleDown); 
  h_Rjetc_HadECALm3->SetMarkerStyle(kOpenTriangleDown); 
  h_Rjetc_HadECALp3->SetMarkerColor(kGreen+3);          
  h_Rjetc_HadECALm3->SetMarkerColor(kGreen+3);
  h_Rjetc_HadECALp3->SetLineColor(kGreen+3);            
  h_Rjetc_HadECALm3->SetLineColor(kGreen+3);
  h_Rjetc_HadECALp3->SetMarkerSize(markersize);         
  h_Rjetc_HadECALm3->SetMarkerSize(markersize);

  h_Rjets_HadECALp3->SetMarkerStyle(kFullTriangleUp); 
  h_Rjets_HadECALm3->SetMarkerStyle(kOpenTriangleUp);
  h_Rjets_HadECALp3->SetMarkerColor(kOrange+7);       
  h_Rjets_HadECALm3->SetMarkerColor(kOrange+7);
  h_Rjets_HadECALp3->SetLineColor(kOrange+7);         
  h_Rjets_HadECALm3->SetLineColor(kOrange+7);
  h_Rjets_HadECALp3->SetMarkerSize(markersize);       
  h_Rjets_HadECALm3->SetMarkerSize(markersize);

  h_Rjetud_HadECALp3->SetMarkerStyle(kFullDiamond); 
  h_Rjetud_HadECALm3->SetMarkerStyle(kOpenDiamond);
  h_Rjetud_HadECALp3->SetMarkerColor(kMagenta+2);   
  h_Rjetud_HadECALm3->SetMarkerColor(kMagenta+2);
  h_Rjetud_HadECALp3->SetLineColor(kMagenta+2);     
  h_Rjetud_HadECALm3->SetLineColor(kMagenta+2);
  h_Rjetud_HadECALp3->SetMarkerSize(markersize);    
  h_Rjetud_HadECALm3->SetMarkerSize(markersize);

  h_Rjetg_HadECALp3->SetMarkerStyle(kFullSquare); 
  h_Rjetg_HadECALm3->SetMarkerStyle(kOpenSquare);
  h_Rjetg_HadECALp3->SetMarkerColor(kBlue+1);     
  h_Rjetg_HadECALm3->SetMarkerColor(kBlue+1);
  h_Rjetg_HadECALp3->SetLineColor(kBlue+1);       
  h_Rjetg_HadECALm3->SetLineColor(kBlue+1);
  h_Rjetg_HadECALp3->SetMarkerSize(markersize);   
  h_Rjetg_HadECALm3->SetMarkerSize(markersize);

  //Legend
  TLegend* lz_HadECAL_all = new TLegend(0.2,0.65,0.82,0.87);
  lz_HadECAL_all->SetBorderSize(0);
  lz_HadECAL_all->AddEntry(h_Rjet_HadECALp3, "#bf{All jets Hadron ECAL+3%}", "p");
  lz_HadECAL_all->AddEntry(h_Rjet_HadECALm3, "#bf{All jets Hadron ECAL-3%}", "p");
  lz_HadECAL_all->AddEntry(h_Rjetb_HadECALp3, "#bf{b jets Hadron ECAL+3%}", "p");
  lz_HadECAL_all->AddEntry(h_Rjetb_HadECALm3, "#bf{b jets Hadron ECAL-3%}", "p");
  lz_HadECAL_all->AddEntry(h_Rjetc_HadECALp3, "#bf{c jets Hadron ECAL+3%}", "p");
  lz_HadECAL_all->AddEntry(h_Rjetc_HadECALm3, "#bf{c jets Hadron ECAL-3%}", "p");
  lz_HadECAL_all->AddEntry(h_Rjets_HadECALp3, "#bf{s jets Hadron ECAL+3%}", "p");
  lz_HadECAL_all->AddEntry(h_Rjets_HadECALm3, "#bf{s jets Hadron ECAL-3%}", "p");
  lz_HadECAL_all->AddEntry(h_Rjetud_HadECALp3, "#bf{ud jets Hadron ECAL+3%}", "p");
  lz_HadECAL_all->AddEntry(h_Rjetud_HadECALm3, "#bf{ud jets Hadron ECAL-3%}", "p");
  lz_HadECAL_all->AddEntry(h_Rjetg_HadECALp3, "#bf{g jets Hadron ECAL+3%}", "p");
  lz_HadECAL_all->AddEntry(h_Rjetg_HadECALm3, "#bf{g jets Hadron ECAL-3%}", "p");
  lz_HadECAL_all->SetNColumns(2);
  TH1D* setup_all = new TH1D("setup_all","", h_Rjet_HadECALp3->GetXaxis()->GetNbins(), 15, 1248);


  //Title and axis setup
  setup_all->SetStats(0); //Suppress stat box
  setup_all->SetAxisRange(0.98,1.03,"Y"); //Vertical axis limits
  h_Rjetb_HadECALp3->SetAxisRange(15, 1248,"X");
  h_Rjetb_HadECALm3->SetAxisRange(15, 1248,"X");
  h_Rjetc_HadECALp3->SetAxisRange(15, 1248,"X");
  h_Rjetc_HadECALm3->SetAxisRange(15, 1248,"X");
  h_Rjets_HadECALp3->SetAxisRange(15, 1248,"X");
  h_Rjets_HadECALm3->SetAxisRange(15, 1248,"X");
  h_Rjetud_HadECALp3->SetAxisRange(15, 1248,"X");
  h_Rjetud_HadECALm3->SetAxisRange(15, 1248,"X");
  h_Rjetg_HadECALp3->SetAxisRange(15, 1248,"X");
  h_Rjetg_HadECALm3->SetAxisRange(15, 1248,"X");
  h_Rjet_HadECALp3->SetAxisRange(15, 1248,"X");
  h_Rjet_HadECALm3->SetAxisRange(15, 1248,"X");
  setup_all->GetXaxis()->SetMoreLogLabels();
  setup_all->GetXaxis()->SetNoExponent();
  canv_HadECAL_all->SetLogx();
  setup_all->GetYaxis()->SetTitleOffset(1.5);
  setup_all->GetXaxis()->SetTitleOffset(1.1);
  setup_all->GetYaxis()->SetTitle("Response ratio");
  setup_all->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_all->GetYaxis()->SetTitleSize(0.05);
  setup_all->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  TLine *line = new TLine(15,1,1258.25,1); 

  //Plot
  setup_all->Draw();
  line->Draw("SAME");
  h_Rjetb_HadECALp3->Draw("SAMEP");
  h_Rjetb_HadECALm3->Draw("SAMEP");
  h_Rjetc_HadECALp3->Draw("SAMEP");
  h_Rjetc_HadECALm3->Draw("SAMEP");
  h_Rjets_HadECALp3->Draw("SAMEP");
  h_Rjets_HadECALm3->Draw("SAMEP");
  h_Rjetud_HadECALp3->Draw("SAMEP");
  h_Rjetud_HadECALm3->Draw("SAMEP");
  h_Rjetg_HadECALp3->Draw("SAMEP");
  h_Rjetg_HadECALm3->Draw("SAMEP");
  h_Rjet_HadECALp3->Draw("SAMEP");
  h_Rjet_HadECALm3->Draw("SAMEP");
  lz_HadECAL_all->Draw("SAMEP");

  TLatex *texa = new TLatex(); 
  texa->SetNDC();
  texa->SetTextSize(0.04); 
  texa->SetTextColor(kBlack);
  texa->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texa->DrawLatex(0.25,0.21,"|#eta^{probe}|<1.3");
  texa->DrawLatex(0.25,0.16,"#alpha<0.3");

  //Save plot
  canv_HadECAL_all->Print("./plots/varPlots/hadECAL/varHadECAL_all.pdf"); delete canv_HadECAL_all;


  // b jets
  TCanvas* canv_HadECAL_b = new TCanvas("canv_HadECAL_b","",500,500);
  canv_HadECAL_b->SetLeftMargin(0.15); canv_HadECAL_b->SetBottomMargin(0.13);

  TLegend* lz_HadECALb = new TLegend(0.18,0.68,0.6,0.86);
  lz_HadECALb->SetBorderSize(0);
  lz_HadECALb->AddEntry(h_Rjet_HadECALp3, "All jets Hadron ECAL+3%", "p");
  lz_HadECALb->AddEntry(h_Rjet_HadECALm3, "All jets Hadron ECAL-3%", "p");
  lz_HadECALb->AddEntry(h_Rjetb_HadECALp3, "b jets Hadron ECAL+3%", "p");
  lz_HadECALb->AddEntry(h_Rjetb_HadECALm3, "b jets Hadron ECAL-3%", "p");
  TH1D* setupb = new TH1D("setupb","", h_Rjet_HadECALp3->GetXaxis()->GetNbins(),
			 28,1248);
  //Title and axis setup
  setupb->SetStats(0); //Suppress stat box
  setupb->SetAxisRange(0.99,1.011,"Y"); //Vertical axis limits
  setupb->GetXaxis()->SetMoreLogLabels();
  setupb->GetXaxis()->SetNoExponent();
  canv_HadECAL_b->SetLogx();
  setupb->GetYaxis()->SetTitleOffset(1.5);
  setupb->GetXaxis()->SetTitleOffset(1.1);
  setupb->GetYaxis()->SetTitle("F^{var}");
  setupb->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupb->GetYaxis()->SetTitleSize(0.05);
  setupb->GetXaxis()->SetTitleSize(0.05);

  h_Rjetb_HadECALp3->SetAxisRange(28,1248,"X");
  h_Rjetb_HadECALm3->SetAxisRange(28,1248,"X");

  gPad->SetTickx(); gPad->SetTicky();
  //Plot
  setupb->Draw();
  h_Rjetb_HadECALp3->Draw("SAMEP");
  h_Rjetb_HadECALm3->Draw("SAMEP");
  h_Rjet_HadECALp3->Draw("SAMEP");
  h_Rjet_HadECALm3->Draw("SAMEP");
  lz_HadECALb->Draw("SAMEP");

  TLatex *texb = new TLatex(); 
  texb->SetNDC();
  texb->SetTextSize(0.04); 
  texb->SetTextColor(kBlack);
  texb->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texb->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texb->DrawLatex(0.23,0.2,"#alpha<0.3");

  //Save plot
  canv_HadECAL_b->Print("./plots/varPlots/hadECAL/varHadECAL_b.pdf"); delete canv_HadECAL_b;

  // c jets
  TCanvas* canv_HadECAL_c = new TCanvas("canv_HadECAL_c","",500,500);
  canv_HadECAL_c->SetLeftMargin(0.15); canv_HadECAL_c->SetBottomMargin(0.13);
  TLegend* lz_HadECALc = new TLegend(0.18,0.68,0.6,0.86);
  lz_HadECALc->SetBorderSize(0);
  lz_HadECALc->AddEntry(h_Rjet_HadECALp3, "All jets Hadron ECAL+3%", "p");
  lz_HadECALc->AddEntry(h_Rjet_HadECALm3, "All jets Hadron ECAL-3%", "p");
  lz_HadECALc->AddEntry(h_Rjetc_HadECALp3, "c jets Hadron ECAL+3%", "p");
  lz_HadECALc->AddEntry(h_Rjetc_HadECALm3, "c jets Hadron ECAL-3%", "p");
  TH1D* setupc = new TH1D("setupc","", h_Rjet_HadECALp3->GetXaxis()->GetNbins(), 28, 1240);
  //Title and axis setup
  setupc->SetStats(0); //Suppress stat box
  setupc->SetAxisRange(0.99,1.011,"Y"); //Vertical axis limits
  setupc->GetXaxis()->SetMoreLogLabels();
  setupc->GetXaxis()->SetNoExponent();
  canv_HadECAL_c->SetLogx();
  setupc->GetYaxis()->SetTitleOffset(1.5);
  setupc->GetXaxis()->SetTitleOffset(1.1);
  setupc->GetYaxis()->SetTitle("F^{var}");
  setupc->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupc->GetYaxis()->SetTitleSize(0.05);
  setupc->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();
  setupc->Draw();
  h_Rjetc_HadECALp3->Draw("SAMEP");
  h_Rjetc_HadECALm3->Draw("SAMEP");
  h_Rjet_HadECALp3->Draw("SAMEP");
  h_Rjet_HadECALm3->Draw("SAMEP");
  lz_HadECALc->Draw("SAMEP");

  TLatex *texc = new TLatex(); 
  texc->SetNDC();
  texc->SetTextSize(0.04); 
  texc->SetTextColor(kBlack);
  texc->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texc->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texc->DrawLatex(0.23,0.2,"#alpha<0.3");

  canv_HadECAL_c->Print("./plots/varPlots/hadECAL/varHadECAL_c.pdf"); delete canv_HadECAL_c;

  // s jets
  TCanvas* canv_HadECAL_s = new TCanvas("canv_HadECAL_s","",500,500);
  canv_HadECAL_s->SetLeftMargin(0.15); canv_HadECAL_s->SetBottomMargin(0.13);
  TLegend* lz_HadECALs = new TLegend(0.18,0.68,0.6,0.86);
  lz_HadECALs->SetBorderSize(0);
  lz_HadECALs->AddEntry(h_Rjet_HadECALp3, "All jets Hadron ECAL+3%", "p");
  lz_HadECALs->AddEntry(h_Rjet_HadECALm3, "All jets Hadron ECAL-3%", "p");
  lz_HadECALs->AddEntry(h_Rjets_HadECALp3, "s jets Hadron ECAL+3%", "p");
  lz_HadECALs->AddEntry(h_Rjets_HadECALm3, "s jets Hadron ECAL-3%", "p");
  TH1D* setups = new TH1D("setups","", h_Rjet_HadECALp3->GetXaxis()->GetNbins(), 28, 1240);
  //Title and axis setup
  setups->SetStats(0); //Suppress stat box
  setups->SetAxisRange(0.99,1.011,"Y"); //Vertical axis limits
  setups->GetXaxis()->SetMoreLogLabels();
  setups->GetXaxis()->SetNoExponent();
  canv_HadECAL_s->SetLogx();
  setups->GetYaxis()->SetTitleOffset(1.5);
  setups->GetXaxis()->SetTitleOffset(1.1);
  setups->GetYaxis()->SetTitle("F^{var}");
  setups->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setups->GetYaxis()->SetTitleSize(0.05);
  setups->GetXaxis()->SetTitleSize(0.05);


  gPad->SetTickx(); gPad->SetTicky();
  setups->Draw();
  lz_HadECALs->Draw("SAMEP");
  h_Rjets_HadECALp3->Draw("SAMEP");
  h_Rjets_HadECALm3->Draw("SAMEP");
  h_Rjet_HadECALp3->Draw("SAMEP");
  h_Rjet_HadECALm3->Draw("SAMEP");

  TLatex *texs = new TLatex(); 
  texs->SetNDC();
  texs->SetTextSize(0.04); 
  texs->SetTextColor(kBlack);
  texs->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texs->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texs->DrawLatex(0.23,0.2,"#alpha<0.3");

  canv_HadECAL_s->Print("./plots/varPlots/hadECAL/varHadECAL_s.pdf"); delete canv_HadECAL_s;

  // ud jets
  TCanvas* canv_HadECAL_ud = new TCanvas("canv_HadECAL_ud","",500,500);
  canv_HadECAL_ud->SetLeftMargin(0.15); canv_HadECAL_ud->SetBottomMargin(0.13);
  TLegend* lz_HadECALud = new TLegend(0.18,0.68,0.6,0.86);
  lz_HadECALud->SetBorderSize(0);
  lz_HadECALud->AddEntry(h_Rjet_HadECALp3, "All jets Hadron ECAL+3%", "p");
  lz_HadECALud->AddEntry(h_Rjet_HadECALm3, "All jets Hadron ECAL-3%", "p");
  lz_HadECALud->AddEntry(h_Rjetud_HadECALp3, "ud jets Hadron ECAL+3%", "p");
  lz_HadECALud->AddEntry(h_Rjetud_HadECALm3, "ud jets Hadron ECAL-3%", "p");
  TH1D* setupud = new TH1D("setupud","", h_Rjet_HadECALp3->GetXaxis()->GetNbins(), 28, 1240);
  //Title and axis setup
  setupud->SetStats(0); //Suppress stat box
  setupud->SetAxisRange(0.99,1.011,"Y"); //Vertical axis limits
  setupud->GetXaxis()->SetMoreLogLabels();
  setupud->GetXaxis()->SetNoExponent();
  canv_HadECAL_ud->SetLogx();
  setupud->GetYaxis()->SetTitleOffset(1.5);
  setupud->GetXaxis()->SetTitleOffset(1.1);
  setupud->GetYaxis()->SetTitle("F^{var}");
  setupud->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupud->GetYaxis()->SetTitleSize(0.05);
  setupud->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();
  setupud->Draw();
  lz_HadECALud->Draw("SAMEP");
  h_Rjetud_HadECALp3->Draw("SAMEP");
  h_Rjetud_HadECALm3->Draw("SAMEP");
  h_Rjet_HadECALp3->Draw("SAMEP");
  h_Rjet_HadECALm3->Draw("SAMEP");

  TLatex *texud = new TLatex(); 
  texud->SetNDC();
  texud->SetTextSize(0.04); 
  texud->SetTextColor(kBlack);
  texud->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texud->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texud->DrawLatex(0.23,0.2,"#alpha<0.3");

  canv_HadECAL_ud->Print("./plots/varPlots/hadECAL/varHadECAL_ud.pdf"); delete canv_HadECAL_ud;

  // g jets
  TCanvas* canv_HadECAL_g = new TCanvas("canv_HadECAL_g","",500,500);
  canv_HadECAL_g->SetLeftMargin(0.15); canv_HadECAL_g->SetBottomMargin(0.13);
  TLegend* lz_HadECALg = new TLegend(0.18,0.68,0.6,0.86);
  lz_HadECALg->SetBorderSize(0);
  lz_HadECALg->AddEntry(h_Rjet_HadECALp3, "All jets Hadron ECAL+3%", "p");
  lz_HadECALg->AddEntry(h_Rjet_HadECALm3, "All jets Hadron ECAL-3%", "p");
  lz_HadECALg->AddEntry(h_Rjetg_HadECALp3, "g jets Hadron ECAL+3%", "p");
  lz_HadECALg->AddEntry(h_Rjetg_HadECALm3, "g jets Hadron ECAL-3%", "p");
  TH1D* setupg = new TH1D("setupg","", h_Rjet_HadECALp3->GetXaxis()->GetNbins(), 28, 1240);
  //Title and axis setup
  setupg->SetStats(0); //Suppress stat box
  setupg->SetAxisRange(0.99,1.011,"Y"); //Vertical axis limits
  setupg->GetXaxis()->SetMoreLogLabels();
  setupg->GetXaxis()->SetNoExponent();
  canv_HadECAL_g->SetLogx();
  setupg->GetYaxis()->SetTitleOffset(1.5);
  setupg->GetXaxis()->SetTitleOffset(1.1);
  setupg->GetYaxis()->SetTitle("F^{var}");
  setupg->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupg->GetYaxis()->SetTitleSize(0.05);
  setupg->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();
  setupg->Draw();
  lz_HadECALg->Draw("SAMEP");
  h_Rjetg_HadECALp3->Draw("SAMEP");
  h_Rjetg_HadECALm3->Draw("SAMEP");
  h_Rjet_HadECALp3->Draw("SAMEP");
  h_Rjet_HadECALm3->Draw("SAMEP");

  TLatex *texg = new TLatex(); 
  texg->SetNDC();
  texg->SetTextSize(0.04); 
  texg->SetTextColor(kBlack);
  texg->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texg->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texg->DrawLatex(0.23,0.2,"#alpha<0.3");

  canv_HadECAL_g->Print("./plots/varPlots/hadECAL/varHadECAL_g.pdf"); delete canv_HadECAL_g;


  gApplication->Terminate(0); //Exit root command prompt

}
