void plotHCALVar()
{

  double markersize = 0.8;

  //Variants file
  TFile* fzj = TFile::Open("./output_ROOT_files/varHCAL_5000000.root");

  TH1D* h_Rjet_HCALp3;
  TH1D* h_Rjet_HCALm3;
  TH1D* h_Rjetb_HCALp3;
  TH1D* h_Rjetb_HCALm3;
  TH1D* h_Rjetc_HCALp3;
  TH1D* h_Rjetc_HCALm3;
  TH1D* h_Rjets_HCALp3;
  TH1D* h_Rjets_HCALm3;
  TH1D* h_Rjetud_HCALp3;
  TH1D* h_Rjetud_HCALm3;
  TH1D* h_Rjetg_HCALp3;
  TH1D* h_Rjetg_HCALm3;


  //Create Histograms
  fzj->GetObject("h_Rjet_HCALp3", h_Rjet_HCALp3);
  fzj->GetObject("h_Rjet_HCALm3", h_Rjet_HCALm3);

  fzj->GetObject("h_Rjetb_HCALp3", h_Rjetb_HCALp3);
  fzj->GetObject("h_Rjetb_HCALm3", h_Rjetb_HCALm3);

  fzj->GetObject("h_Rjetc_HCALp3", h_Rjetc_HCALp3);
  fzj->GetObject("h_Rjetc_HCALm3", h_Rjetc_HCALm3);

  fzj->GetObject("h_Rjets_HCALp3", h_Rjets_HCALp3);
  fzj->GetObject("h_Rjets_HCALm3", h_Rjets_HCALm3);

  fzj->GetObject("h_Rjetud_HCALp3", h_Rjetud_HCALp3);
  fzj->GetObject("h_Rjetud_HCALm3", h_Rjetud_HCALm3);

  fzj->GetObject("h_Rjetg_HCALp3", h_Rjetg_HCALp3);
  fzj->GetObject("h_Rjetg_HCALm3", h_Rjetg_HCALm3);

// HCAL
  TCanvas* canv_HCAL = new TCanvas("canv_HCAL_b","",500,500);
  canv_HCAL->SetLeftMargin(0.15); canv_HCAL->SetBottomMargin(0.13);

  TGraph *JEC_HCALp3 = new TGraph("data_and_MC_input/Response/JEC_PFJet_HCALplus3.txt" );
  TGraph *JEC_HCALm3 = new TGraph("data_and_MC_input/Response/JEC_PFJet_HCALminus3.txt" );

  JEC_HCALp3->SetMarkerStyle(kOpenCircle); JEC_HCALp3->SetMarkerColor(kRed);
  JEC_HCALm3->SetMarkerStyle(kOpenSquare); JEC_HCALm3->SetMarkerColor(kBlue+1);
  JEC_HCALp3->SetMarkerSize(markersize);
  JEC_HCALm3->SetMarkerSize(markersize);

  h_Rjet_HCALp3->SetMarkerStyle(kFullCircle); h_Rjet_HCALm3->SetMarkerStyle(kFullSquare);
  h_Rjet_HCALp3->SetMarkerColor(kRed);    h_Rjet_HCALm3->SetMarkerColor(kBlue+1);
  h_Rjet_HCALp3->SetLineColor(kRed);      h_Rjet_HCALm3->SetLineColor(kBlue+1);
  h_Rjet_HCALp3->SetMarkerSize(markersize); h_Rjet_HCALm3->SetMarkerSize(markersize);

  TLegend* lz_HCAL = new TLegend(0.18,0.73,0.75,0.87);
  //lz_HCAL->SetNColumns(2);
  lz_HCAL->SetBorderSize(0);
  lz_HCAL->AddEntry(h_Rjet_HCALp3,  "Hadron HCAL+3% (Our)", "p");
  lz_HCAL->AddEntry(JEC_HCALp3,     "Hadron HCAL+3% (FastSim)", "p");
  lz_HCAL->AddEntry(h_Rjet_HCALm3,  "Hadron HCAL-3% (Our)", "p");
  lz_HCAL->AddEntry(JEC_HCALm3,     "Hadron HCAL-3% (FastSim)", "p"); 

  //TH1D* setup = new TH1D("setup","", 
  //                       h_Rjet_HCALp3->GetXaxis()->GetNbins(),
  //		 h_Rjet_HCALp3->GetXaxis()->GetXmin(), 
  //                       h_Rjet_HCALp3->GetXaxis()->GetXmax());

  TH1D* setup = new TH1D("setup","", h_Rjet_HCALp3->GetXaxis()->GetNbins(), 28, 1240);
  //Title and axis setup
  setup->SetStats(0); //Suppress stat box
  setup->SetAxisRange(0.985,1.02,"Y"); //Vertical axis limits
  //setup->SetAxisRange(28,2000,"X"); //Horizontal axis limits

  h_Rjet_HCALp3->SetAxisRange(28,1248,"X");
  h_Rjet_HCALm3->SetAxisRange(28,1248,"X");

  setup->GetXaxis()->SetMoreLogLabels();
  setup->GetXaxis()->SetNoExponent();
  canv_HCAL->SetLogx();
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
  h_Rjet_HCALp3->Draw("SAMEP");
  h_Rjet_HCALm3->Draw("SAMEP");
  lz_HCAL->Draw("SAMEP");

  TLatex *tex = new TLatex(); tex->SetNDC();
  tex->SetTextSize(0.04); tex->SetTextColor(kBlack);
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.23,0.2,"#alpha<0.3");

  setup->GetXaxis()->SetRangeUser(28,2000);
  gPad->Modified();

  //Save plot
  canv_HCAL->Print("./plots/varPlots/hadHCAL/varHCAL.pdf"); delete canv_HCAL;


// C +-3%

  TCanvas* canv_HCAL_all = new TCanvas("canv_HCAL_all","",500,500);
  canv_HCAL_all->SetLeftMargin(0.15); canv_HCAL_all->SetBottomMargin(0.13);

  h_Rjet_HCALp3->SetMarkerStyle(2);             h_Rjet_HCALm3->SetMarkerStyle(5);
  h_Rjet_HCALp3->SetMarkerColor(kBlack);        h_Rjet_HCALm3->SetMarkerColor(kBlack);
  h_Rjet_HCALp3->SetLineColor(kBlack);          h_Rjet_HCALm3->SetLineColor(kBlack);
  h_Rjet_HCALp3->SetMarkerSize(markersize);     h_Rjet_HCALm3->SetMarkerSize(markersize);

  h_Rjetb_HCALp3->SetMarkerStyle(kFullCircle);  h_Rjetb_HCALm3->SetMarkerStyle(kOpenCircle);
  h_Rjetb_HCALp3->SetMarkerColor(kRed+1);       h_Rjetb_HCALm3->SetMarkerColor(kRed+1);
  h_Rjetb_HCALp3->SetLineColor(kRed+1);         h_Rjetb_HCALm3->SetLineColor(kRed+1);
  h_Rjetb_HCALp3->SetMarkerSize(markersize);    h_Rjetb_HCALm3->SetMarkerSize(markersize);

  h_Rjetc_HCALp3->SetMarkerStyle(kFullTriangleDown); 
  h_Rjetc_HCALm3->SetMarkerStyle(kOpenTriangleDown); 
  h_Rjetc_HCALp3->SetMarkerColor(kGreen+3);          
  h_Rjetc_HCALm3->SetMarkerColor(kGreen+3);
  h_Rjetc_HCALp3->SetLineColor(kGreen+3);            
  h_Rjetc_HCALm3->SetLineColor(kGreen+3);
  h_Rjetc_HCALp3->SetMarkerSize(markersize);         
  h_Rjetc_HCALm3->SetMarkerSize(markersize);

  h_Rjets_HCALp3->SetMarkerStyle(kFullTriangleUp); 
  h_Rjets_HCALm3->SetMarkerStyle(kOpenTriangleUp);
  h_Rjets_HCALp3->SetMarkerColor(kOrange+7);       
  h_Rjets_HCALm3->SetMarkerColor(kOrange+7);
  h_Rjets_HCALp3->SetLineColor(kOrange+7);         
  h_Rjets_HCALm3->SetLineColor(kOrange+7);
  h_Rjets_HCALp3->SetMarkerSize(markersize);       
  h_Rjets_HCALm3->SetMarkerSize(markersize);

  h_Rjetud_HCALp3->SetMarkerStyle(kFullDiamond); 
  h_Rjetud_HCALm3->SetMarkerStyle(kOpenDiamond);
  h_Rjetud_HCALp3->SetMarkerColor(kMagenta+2);   
  h_Rjetud_HCALm3->SetMarkerColor(kMagenta+2);
  h_Rjetud_HCALp3->SetLineColor(kMagenta+2);     
  h_Rjetud_HCALm3->SetLineColor(kMagenta+2);
  h_Rjetud_HCALp3->SetMarkerSize(markersize);    
  h_Rjetud_HCALm3->SetMarkerSize(markersize);

  h_Rjetg_HCALp3->SetMarkerStyle(kFullSquare); 
  h_Rjetg_HCALm3->SetMarkerStyle(kOpenSquare);
  h_Rjetg_HCALp3->SetMarkerColor(kBlue+1);     
  h_Rjetg_HCALm3->SetMarkerColor(kBlue+1);
  h_Rjetg_HCALp3->SetLineColor(kBlue+1);       
  h_Rjetg_HCALm3->SetLineColor(kBlue+1);
  h_Rjetg_HCALp3->SetMarkerSize(markersize);   
  h_Rjetg_HCALm3->SetMarkerSize(markersize);

  //Legend
  TLegend* lz_HCAL_all = new TLegend(0.2,0.65,0.55,0.87);
  lz_HCAL_all->SetBorderSize(0);
  lz_HCAL_all->AddEntry(h_Rjet_HCALp3, "All jets HCAL+3%", "p");
  lz_HCAL_all->AddEntry(h_Rjet_HCALm3, "All jets HCAL-3%", "p");
  lz_HCAL_all->AddEntry(h_Rjetb_HCALp3, "b jets HCAL+3%", "p");
  lz_HCAL_all->AddEntry(h_Rjetb_HCALm3, "b jets HCAL-3%", "p");
  lz_HCAL_all->AddEntry(h_Rjetc_HCALp3, "c jets HCAL+3%", "p");
  lz_HCAL_all->AddEntry(h_Rjetc_HCALm3, "c jets HCAL-3%", "p");
  lz_HCAL_all->AddEntry(h_Rjets_HCALp3, "s jets HCAL+3%", "p");
  lz_HCAL_all->AddEntry(h_Rjets_HCALm3, "s jets HCAL-3%", "p");
  lz_HCAL_all->AddEntry(h_Rjetud_HCALp3, "ud jets HCAL+3%", "p");
  lz_HCAL_all->AddEntry(h_Rjetud_HCALm3, "ud jets HCAL-3%", "p");
  lz_HCAL_all->AddEntry(h_Rjetg_HCALp3, "g jets HCAL+3%", "p");
  lz_HCAL_all->AddEntry(h_Rjetg_HCALm3, "g jets HCAL-3%", "p");
  lz_HCAL_all->SetNColumns(2);
  TH1D* setup_all = new TH1D("setup_all","", h_Rjet_HCALp3->GetXaxis()->GetNbins(), 28, 1240);


  //Title and axis setup
  setup_all->SetStats(0); //Suppress stat box
  setup_all->SetAxisRange(0.978,1.028,"Y"); //Vertical axis limits
  h_Rjetb_HCALp3->SetAxisRange(28, 1240,"X");
  h_Rjetb_HCALm3->SetAxisRange(28, 1240,"X");
  h_Rjetc_HCALp3->SetAxisRange(28, 1240,"X");
  h_Rjetc_HCALm3->SetAxisRange(28, 1240,"X");
  h_Rjets_HCALp3->SetAxisRange(28, 1240,"X");
  h_Rjets_HCALm3->SetAxisRange(28, 1240,"X");
  h_Rjetud_HCALp3->SetAxisRange(28, 1240,"X");
  h_Rjetud_HCALm3->SetAxisRange(28, 1240,"X");
  h_Rjetg_HCALp3->SetAxisRange(28, 1240,"X");
  h_Rjetg_HCALm3->SetAxisRange(28, 1240,"X");
  h_Rjet_HCALp3->SetAxisRange(28, 1240,"X");
  h_Rjet_HCALm3->SetAxisRange(28, 1240,"X");
  setup_all->GetXaxis()->SetMoreLogLabels();
  setup_all->GetXaxis()->SetNoExponent();
  canv_HCAL_all->SetLogx();
  setup_all->GetYaxis()->SetTitleOffset(1.5);
  setup_all->GetXaxis()->SetTitleOffset(1.1);
  setup_all->GetYaxis()->SetTitle("F^{var}");
  setup_all->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup_all->GetYaxis()->SetTitleSize(0.05);
  setup_all->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  //Plot
  setup_all->Draw();
  h_Rjetb_HCALp3->Draw("SAMEP");
  h_Rjetb_HCALm3->Draw("SAMEP");
  h_Rjetc_HCALp3->Draw("SAMEP");
  h_Rjetc_HCALm3->Draw("SAMEP");
  h_Rjets_HCALp3->Draw("SAMEP");
  h_Rjets_HCALm3->Draw("SAMEP");
  h_Rjetud_HCALp3->Draw("SAMEP");
  h_Rjetud_HCALm3->Draw("SAMEP");
  h_Rjetg_HCALp3->Draw("SAMEP");
  h_Rjetg_HCALm3->Draw("SAMEP");
  h_Rjet_HCALp3->Draw("SAMEP");
  h_Rjet_HCALm3->Draw("SAMEP");
  lz_HCAL_all->Draw("SAMEP");

  TLatex *texa = new TLatex(); 
  texa->SetNDC();
  texa->SetTextSize(0.04); 
  texa->SetTextColor(kBlack);
  texa->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texa->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texa->DrawLatex(0.23,0.2,"#alpha<0.3");

  //Save plot
  canv_HCAL_all->Print("./plots/varPlots/hadHCAL/varHCAL_all.pdf"); delete canv_HCAL_all;


  // b jets
  TCanvas* canv_HCAL_b = new TCanvas("canv_HCAL_b","",500,500);
  canv_HCAL_b->SetLeftMargin(0.15); canv_HCAL_b->SetBottomMargin(0.13);

  TLegend* lz_HCALb = new TLegend(0.2,0.68,0.5,0.86);
  lz_HCALb->SetBorderSize(0);
  lz_HCALb->AddEntry(h_Rjet_HCALp3, "All jets HCAL+3%", "p");
  lz_HCALb->AddEntry(h_Rjet_HCALm3, "All jets HCAL-3%", "p");
  lz_HCALb->AddEntry(h_Rjetb_HCALp3, "b jets HCAL+3%", "p");
  lz_HCALb->AddEntry(h_Rjetb_HCALm3, "b jets HCAL-3%", "p");
  TH1D* setupb = new TH1D("setupb","", h_Rjet_HCALp3->GetXaxis()->GetNbins(),
			 28,1248);
  //Title and axis setup
  setupb->SetStats(0); //Suppress stat box
  setupb->SetAxisRange(0.983,1.017,"Y"); //Vertical axis limits
  setupb->GetXaxis()->SetMoreLogLabels();
  setupb->GetXaxis()->SetNoExponent();
  canv_HCAL_b->SetLogx();
  setupb->GetYaxis()->SetTitleOffset(1.5);
  setupb->GetXaxis()->SetTitleOffset(1.1);
  setupb->GetYaxis()->SetTitle("F^{var}");
  setupb->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupb->GetYaxis()->SetTitleSize(0.05);
  setupb->GetXaxis()->SetTitleSize(0.05);

  h_Rjetb_HCALp3->SetAxisRange(28,1248,"X");
  h_Rjetb_HCALm3->SetAxisRange(28,1248,"X");

  gPad->SetTickx(); gPad->SetTicky();
  //Plot
  setupb->Draw();
  h_Rjetb_HCALp3->Draw("SAMEP");
  h_Rjetb_HCALm3->Draw("SAMEP");
  h_Rjet_HCALp3->Draw("SAMEP");
  h_Rjet_HCALm3->Draw("SAMEP");
  lz_HCALb->Draw("SAMEP");

  TLatex *texb = new TLatex(); 
  texb->SetNDC();
  texb->SetTextSize(0.04); 
  texb->SetTextColor(kBlack);
  texb->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texb->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texb->DrawLatex(0.23,0.2,"#alpha<0.3");

  //Save plot
  canv_HCAL_b->Print("./plots/varPlots/hadHCAL/varHCAL_b.pdf"); delete canv_HCAL_b;

  // c jets
  TCanvas* canv_HCAL_c = new TCanvas("canv_HCAL_c","",500,500);
  canv_HCAL_c->SetLeftMargin(0.15); canv_HCAL_c->SetBottomMargin(0.13);
  TLegend* lz_HCALc = new TLegend(0.2,0.68,0.5,0.86);
  lz_HCALc->SetBorderSize(0);
  lz_HCALc->AddEntry(h_Rjet_HCALp3, "All jets HCAL+3%", "p");
  lz_HCALc->AddEntry(h_Rjet_HCALm3, "All jets HCAL-3%", "p");
  lz_HCALc->AddEntry(h_Rjetc_HCALp3, "c jets HCAL+3%", "p");
  lz_HCALc->AddEntry(h_Rjetc_HCALm3, "c jets HCAL-3%", "p");
  TH1D* setupc = new TH1D("setupc","", h_Rjet_HCALp3->GetXaxis()->GetNbins(), 28, 1240);
  //Title and axis setup
  setupc->SetStats(0); //Suppress stat box
  setupc->SetAxisRange(0.982,1.019,"Y"); //Vertical axis limits
  setupc->GetXaxis()->SetMoreLogLabels();
  setupc->GetXaxis()->SetNoExponent();
  canv_HCAL_c->SetLogx();
  setupc->GetYaxis()->SetTitleOffset(1.5);
  setupc->GetXaxis()->SetTitleOffset(1.1);
  setupc->GetYaxis()->SetTitle("F^{var}");
  setupc->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupc->GetYaxis()->SetTitleSize(0.05);
  setupc->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();
  setupc->Draw();
  h_Rjetc_HCALp3->Draw("SAMEP");
  h_Rjetc_HCALm3->Draw("SAMEP");
  h_Rjet_HCALp3->Draw("SAMEP");
  h_Rjet_HCALm3->Draw("SAMEP");
  lz_HCALc->Draw("SAMEP");

  TLatex *texc = new TLatex(); 
  texc->SetNDC();
  texc->SetTextSize(0.04); 
  texc->SetTextColor(kBlack);
  texc->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texc->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texc->DrawLatex(0.23,0.2,"#alpha<0.3");

  canv_HCAL_c->Print("./plots/varPlots/hadHCAL/varHCAL_c.pdf"); delete canv_HCAL_c;

  // s jets
  TCanvas* canv_HCAL_s = new TCanvas("canv_HCAL_s","",500,500);
  canv_HCAL_s->SetLeftMargin(0.15); canv_HCAL_s->SetBottomMargin(0.13);
  TLegend* lz_HCALs = new TLegend(0.2,0.68,0.5,0.86);
  lz_HCALs->SetBorderSize(0);
  lz_HCALs->AddEntry(h_Rjet_HCALp3, "All jets HCAL+3%", "p");
  lz_HCALs->AddEntry(h_Rjet_HCALm3, "All jets HCAL-3%", "p");
  lz_HCALs->AddEntry(h_Rjets_HCALp3, "s jets HCAL+3%", "p");
  lz_HCALs->AddEntry(h_Rjets_HCALm3, "s jets HCAL-3%", "p");
  TH1D* setups = new TH1D("setups","", h_Rjet_HCALp3->GetXaxis()->GetNbins(), 28, 1240);
  //Title and axis setup
  setups->SetStats(0); //Suppress stat box
  setups->SetAxisRange(0.982,1.018,"Y"); //Vertical axis limits
  setups->GetXaxis()->SetMoreLogLabels();
  setups->GetXaxis()->SetNoExponent();
  canv_HCAL_s->SetLogx();
  setups->GetYaxis()->SetTitleOffset(1.5);
  setups->GetXaxis()->SetTitleOffset(1.1);
  setups->GetYaxis()->SetTitle("F^{var}");
  setups->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setups->GetYaxis()->SetTitleSize(0.05);
  setups->GetXaxis()->SetTitleSize(0.05);


  gPad->SetTickx(); gPad->SetTicky();
  setups->Draw();
  lz_HCALs->Draw("SAMEP");
  h_Rjets_HCALp3->Draw("SAMEP");
  h_Rjets_HCALm3->Draw("SAMEP");
  h_Rjet_HCALp3->Draw("SAMEP");
  h_Rjet_HCALm3->Draw("SAMEP");

  TLatex *texs = new TLatex(); 
  texs->SetNDC();
  texs->SetTextSize(0.04); 
  texs->SetTextColor(kBlack);
  texs->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texs->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texs->DrawLatex(0.23,0.2,"#alpha<0.3");

  canv_HCAL_s->Print("./plots/varPlots/hadHCAL/varHCAL_s.pdf"); delete canv_HCAL_s;

  // ud jets
  TCanvas* canv_HCAL_ud = new TCanvas("canv_HCAL_ud","",500,500);
  canv_HCAL_ud->SetLeftMargin(0.15); canv_HCAL_ud->SetBottomMargin(0.13);
  TLegend* lz_HCALud = new TLegend(0.2,0.68,0.5,0.86);
  lz_HCALud->SetBorderSize(0);
  lz_HCALud->AddEntry(h_Rjet_HCALp3, "All jets HCAL+3%", "p");
  lz_HCALud->AddEntry(h_Rjet_HCALm3, "All jets HCAL-3%", "p");
  lz_HCALud->AddEntry(h_Rjetud_HCALp3, "ud jets HCAL+3%", "p");
  lz_HCALud->AddEntry(h_Rjetud_HCALm3, "ud jets HCAL-3%", "p");
  TH1D* setupud = new TH1D("setupud","", h_Rjet_HCALp3->GetXaxis()->GetNbins(), 28, 1240);
  //Title and axis setup
  setupud->SetStats(0); //Suppress stat box
  setupud->SetAxisRange(0.986,1.014,"Y"); //Vertical axis limits
  setupud->GetXaxis()->SetMoreLogLabels();
  setupud->GetXaxis()->SetNoExponent();
  canv_HCAL_ud->SetLogx();
  setupud->GetYaxis()->SetTitleOffset(1.5);
  setupud->GetXaxis()->SetTitleOffset(1.1);
  setupud->GetYaxis()->SetTitle("F^{var}");
  setupud->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupud->GetYaxis()->SetTitleSize(0.05);
  setupud->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();
  setupud->Draw();
  lz_HCALud->Draw("SAMEP");
  h_Rjetud_HCALp3->Draw("SAMEP");
  h_Rjetud_HCALm3->Draw("SAMEP");
  h_Rjet_HCALp3->Draw("SAMEP");
  h_Rjet_HCALm3->Draw("SAMEP");

  TLatex *texud = new TLatex(); 
  texud->SetNDC();
  texud->SetTextSize(0.04); 
  texud->SetTextColor(kBlack);
  texud->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texud->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texud->DrawLatex(0.23,0.2,"#alpha<0.3");

  canv_HCAL_ud->Print("./plots/varPlots/hadHCAL/varHCAL_ud.pdf"); delete canv_HCAL_ud;

  // g jets
  TCanvas* canv_HCAL_g = new TCanvas("canv_HCAL_g","",500,500);
  canv_HCAL_g->SetLeftMargin(0.15); canv_HCAL_g->SetBottomMargin(0.13);
  TLegend* lz_HCALg = new TLegend(0.2,0.68,0.5,0.86);
  lz_HCALg->SetBorderSize(0);
  lz_HCALg->AddEntry(h_Rjet_HCALp3, "All jets HCAL+3%", "p");
  lz_HCALg->AddEntry(h_Rjet_HCALm3, "All jets HCAL-3%", "p");
  lz_HCALg->AddEntry(h_Rjetg_HCALp3, "g jets HCAL+3%", "p");
  lz_HCALg->AddEntry(h_Rjetg_HCALm3, "g jets HCAL-3%", "p");
  TH1D* setupg = new TH1D("setupg","", h_Rjet_HCALp3->GetXaxis()->GetNbins(), 28, 1240);
  //Title and axis setup
  setupg->SetStats(0); //Suppress stat box
  setupg->SetAxisRange(0.987,1.013,"Y"); //Vertical axis limits
  setupg->GetXaxis()->SetMoreLogLabels();
  setupg->GetXaxis()->SetNoExponent();
  canv_HCAL_g->SetLogx();
  setupg->GetYaxis()->SetTitleOffset(1.5);
  setupg->GetXaxis()->SetTitleOffset(1.1);
  setupg->GetYaxis()->SetTitle("F^{var}");
  setupg->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setupg->GetYaxis()->SetTitleSize(0.05);
  setupg->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();
  setupg->Draw();
  lz_HCALg->Draw("SAMEP");
  h_Rjetg_HCALp3->Draw("SAMEP");
  h_Rjetg_HCALm3->Draw("SAMEP");
  h_Rjet_HCALp3->Draw("SAMEP");
  h_Rjet_HCALm3->Draw("SAMEP");

  TLatex *texg = new TLatex(); 
  texg->SetNDC();
  texg->SetTextSize(0.04); 
  texg->SetTextColor(kBlack);
  texg->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  texg->DrawLatex(0.23,0.25,"|#eta^{probe}|<1.3");
  texg->DrawLatex(0.23,0.2,"#alpha<0.3");

  canv_HCAL_g->Print("./plots/varPlots/hadHCAL/varHCAL_g.pdf"); delete canv_HCAL_g;


  gApplication->Terminate(0); //Exit root command prompt

}
