void plotDijetZjetComparison() {

  TFile* fzj  = TFile::Open("./output_ROOT_files/CMSJES_P8_Zjet_1000000.root");
  TFile* fdj = TFile::Open("./output_ROOT_files/CMSJES_P8_dijet_1000000.root");


  TProfile *przj_MPF=0, *prdj_MPF=0; 

  fzj->GetObject("prMPF", przj_MPF);
  fdj->GetObject("prMPF", prdj_MPF);

  TH1D* hzj_MPF   = przj_MPF->ProjectionX();
  TH1D* hdj_MPF   = prdj_MPF->ProjectionX();

  static int const nMC_MPFntI = 14;
  double zj_MC_pTp_MPFntI_2018[nMC_MPFntI];
  double zj_MC_MPFntI_2018[nMC_MPFntI];
  double zj_MC_MPFntI_ER_2018[nMC_MPFntI];

  ifstream inzj_MC_MPFntI_2018;

  inzj_MC_MPFntI_2018.open("./data_and_MC_input/MPF/FullCombination_Zmm_17Sep2018_Autumn18_JECv17/2018MPF-notypeI_zmmjet_mc");

  for (int step=0; step != nMC_MPFntI; ++step) { //CMS 2018 MPF-notypeI MC points
    inzj_MC_MPFntI_2018   >> zj_MC_pTp_MPFntI_2018[step] >> zj_MC_MPFntI_2018[step] >> zj_MC_MPFntI_ER_2018[step];
  }
  inzj_MC_MPFntI_2018.close();
  //CMS MC points
  TGraphErrors* mc_zj_MPFntI_2018 = new TGraphErrors(nMC_MPFntI, zj_MC_pTp_MPFntI_2018,
                                                     zj_MC_MPFntI_2018, 0, zj_MC_MPFntI_ER_2018);
  //Canvas
  TCanvas* canv_MPF = new TCanvas("MPF","",500,400);
  canv_MPF->SetLeftMargin(0.13);
  canv_MPF->SetBottomMargin(0.13);
  canv_MPF->SetLogx();

  hzj_MPF->SetLineColor(kBlack);
  hzj_MPF->SetMarkerStyle(kFullCircle);       
  hzj_MPF->SetMarkerColor(kBlack);
  hdj_MPF->SetLineColor(kBlack);
  hdj_MPF->SetMarkerStyle(kOpenCircle);       
  hdj_MPF->SetMarkerColor(kBlack);

  mc_zj_MPFntI_2018->SetMarkerStyle( kOpenSquare); 
  mc_zj_MPFntI_2018->SetMarkerColor( kBlack  );

  //Legend
  TLegend* lz_MPF = new TLegend(0.6,0.76,0.87,0.87);
  lz_MPF->SetBorderSize(0);
  lz_MPF->AddEntry(hzj_MPF, "Z+jet", "p");
  lz_MPF->AddEntry(hdj_MPF, "Dijet", "p");
  lz_MPF->AddEntry(mc_zj_MPFntI_2018, "FullSim 2018ABCD", "p");

  //Title and axis setup
  hzj_MPF->SetStats(0); //Suppress stat box
  hzj_MPF->SetTitle("");
  hzj_MPF->SetAxisRange(0.8,1.1,"Y"); //Vertical axis limits

  hzj_MPF->GetXaxis()->SetMoreLogLabels();
  hzj_MPF->GetXaxis()->SetNoExponent();
  hzj_MPF->GetYaxis()->SetTitleOffset(1.1);
  hzj_MPF->GetXaxis()->SetTitleOffset(1.1);
  hzj_MPF->GetXaxis()->SetTitle("p_{T,tag}^{reco} (GeV)");
  hzj_MPF->GetYaxis()->SetTitle("R_{MPF}");
  hzj_MPF->GetYaxis()->SetTitleSize(0.05);
  hzj_MPF->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  //Plot
  hzj_MPF->Draw("p");
  hdj_MPF->Draw("P SAME");
  mc_zj_MPFntI_2018->Draw("P SAME");
  lz_MPF->Draw();

  TLatex *tex = new TLatex(); tex->SetNDC();
  tex->SetTextSize(0.04); tex->SetTextColor(kBlack);
  tex->DrawLatex(0.75,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.18,0.82,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.18,0.77,"#alpha<0.3");

  string plotName = "./plots/DijetZjetComparison/MPF_dijetZjet.pdf";
  canv_MPF->Print(plotName.c_str()); delete canv_MPF;

//Rjet

  TProfile *przj_Rjet=0, *prdj_Rjet=0; 

  //Create Histograms
  fzj->GetObject("prRjet",przj_Rjet  );
  fdj->GetObject("prRjet",prdj_Rjet  );

  TH1D* hzj_Rjet   = przj_Rjet  ->ProjectionX();
  TH1D* hdj_Rjet   = prdj_Rjet  ->ProjectionX();


  //Canvas
  TCanvas* canv_Rjet = new TCanvas("canv_Rjet","",500,400);
  canv_Rjet->SetLeftMargin(0.15);
  canv_Rjet->SetBottomMargin(0.13);

  hzj_Rjet  ->SetMarkerStyle(kFullCircle);    hzj_Rjet  ->SetMarkerColor(kBlack);
  hdj_Rjet  ->SetMarkerStyle(kOpenCircle);      hdj_Rjet  ->SetMarkerColor(kBlack);

  hzj_Rjet  ->SetLineColor(kBlack);
  hdj_Rjet  ->SetLineColor(kBlack);             

  //Legend
  TLegend* lz_Rjet = new TLegend(0.62,0.75,0.87,0.87);
  lz_Rjet->SetNColumns(2);
  lz_Rjet->SetBorderSize(0);
  lz_Rjet->AddEntry(hzj_Rjet,   "Z+jet",   "p");
  lz_Rjet->AddEntry(hdj_Rjet,   "Dijet",   "p");

  //Title and axis setup
  hzj_Rjet->SetStats(0); //Suppress stat box
  hzj_Rjet->SetTitle("");
  hzj_Rjet->SetAxisRange(0.88,0.98,"Y");
  //hzj_Rjet->SetAxisRange(31.75,1258,"X");
  //hzj_Rjet->GetXaxis()->SetRangeUser(31.75,1258); 

  hzj_Rjet->GetXaxis()->SetMoreLogLabels();
  hzj_Rjet->GetXaxis()->SetNoExponent();
  canv_Rjet->SetLogx();
  hzj_Rjet->GetYaxis()->SetTitleOffset(1.5);
  hzj_Rjet->GetXaxis()->SetTitleOffset(1.1);

  hzj_Rjet->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  hzj_Rjet->GetXaxis()->SetTitleSize(0.05);
  hzj_Rjet->GetYaxis()->SetTitle("R_{jet}");
  hzj_Rjet->GetYaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  //Plot
  hzj_Rjet->Draw("P");
  hdj_Rjet->Draw("SAMEP");
  lz_Rjet->Draw("SAMEP");

  TLatex *tex2 = new TLatex(); tex2->SetNDC();
  tex2->SetTextSize(0.04); tex2->SetTextColor(kBlack);
  tex2->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex2->DrawLatex(0.2,0.82,"|#eta^{probe}|<1.3");
  tex2->DrawLatex(0.2,0.77,"#alpha<0.3");


  string plotName2 = "./plots/DijetZjetComparison/Rjet_dijetZjet.pdf";
  canv_Rjet->Print(plotName2.c_str()); delete canv_Rjet;

//JEF


  TGraph *chf   = new TGraph("data_and_MC_input/Chf/chf.txt");
  TGraph *chnhf = new TGraph("data_and_MC_input/Chf/chnhf.txt");
  chf->SetMarkerStyle(kFullDiamond); chnhf->SetMarkerStyle(kOpenSquare);
  chf->SetMarkerColor(kBlack);       chnhf->SetMarkerColor(kBlack);
  chf->SetMarkerSize(1);             chnhf->SetMarkerSize(0.7);

  TProfile *przchf=0; TProfile *prznhf=0; TProfile *przgammaf=0; TProfile *przef=0;
  TProfile *prdchf=0; TProfile *prdnhf=0; TProfile *prdgammaf=0; TProfile *prdef=0;

  THStack* hstack_JEF, *hstackdj_JEF;
  hstack_JEF = new THStack("hstackJEF","");
  hstackdj_JEF = new THStack("hstackdjJEF","");

  fzj->GetObject("prchf"    ,przchf   );
  fzj->GetObject("prnhf"    ,prznhf   );
  fzj->GetObject("prgammaf" ,przgammaf);
  fzj->GetObject("pref"     ,przef    );

  fdj->GetObject("prchf"    ,prdchf   );
  fdj->GetObject("prnhf"    ,prdnhf   );
  fdj->GetObject("prgammaf" ,prdgammaf);
  fdj->GetObject("pref"     ,prdef    );

  TH1D* hzchf    = przchf   ->ProjectionX();
  TH1D* hznhf    = prznhf   ->ProjectionX();
  TH1D* hzgammaf = przgammaf->ProjectionX();
  TH1D* hzef     = przef    ->ProjectionX();

  TH1D* hdchf    = prdchf   ->ProjectionX();
  TH1D* hdnhf    = prdnhf   ->ProjectionX();
  TH1D* hdgammaf = prdgammaf->ProjectionX();
  TH1D* hdef     = prdef    ->ProjectionX();


  hzchf   ->SetFillColor(kRed-7);   hzchf->SetLineWidth(1);    hzchf->SetLineColor(kBlack);
  hznhf   ->SetFillColor(kGreen-6); hznhf->SetLineWidth(1);    hznhf->SetLineColor(kBlack);
  hzef    ->SetFillColor(kCyan+1);  hzef->SetLineWidth(1);     hzef->SetLineColor(kBlack);
  hzgammaf->SetFillColor(kBlue-7);  hzgammaf->SetLineWidth(1); hzgammaf->SetLineColor(kBlack);

  hdchf   ->SetFillColor(kRed-7);   hdchf->SetLineWidth(1);    hdchf->SetLineColor(kBlack);
  hdnhf   ->SetFillColor(kGreen-6); hdnhf->SetLineWidth(1);    hdnhf->SetLineColor(kBlack);
  hdef    ->SetFillColor(kCyan+1);  hdef->SetLineWidth(1);     hdef->SetLineColor(kBlack);
  hdgammaf->SetFillColor(kBlue-7);  hdgammaf->SetLineWidth(1); hdgammaf->SetLineColor(kBlack);

  hdchf   ->SetMarkerStyle(kOpenCircle); 
  hdnhf   ->SetMarkerStyle(kOpenSquare); 
  hdef    ->SetMarkerStyle(kOpenTriangleUp); 
  hdgammaf->SetMarkerStyle(kOpenTriangleDown); 

  //Canvas and related setup
  TCanvas* canv_c = new TCanvas("canv_JEF","",500,400);
  TLegend* lg     = new TLegend(0.88, 0.1, 0.98, 0.9);
  lg->SetBorderSize(0); //No frame
  lg->SetTextSize(0.028);
  lg->SetFillStyle(0);
  canv_c->SetLogx();
  canv_c->SetBottomMargin(0.13);
  canv_c->SetRightMargin(0.13);

  hstack_JEF->Add(hzchf);
  hstack_JEF->Add(hznhf);
  hstack_JEF->Add(hzgammaf);
  hstack_JEF->Add(hzef);

  hstackdj_JEF->Add(hdchf);
  hstackdj_JEF->Add(hdnhf);
  hstackdj_JEF->Add(hdgammaf);
  hstackdj_JEF->Add(hdef);

  //Legend setup
  lg->AddEntry(hzef,     "e Z+jet",     "f");
  lg->AddEntry(hzgammaf, "#gamma Z+jet","f");
  lg->AddEntry(hznhf,    "NH Z+jet",    "f");
  lg->AddEntry(hzchf,    "CH Z+jet",    "f");
  lg->AddEntry(hdef,     "e dijet",     "p");
  lg->AddEntry(hdgammaf, "#gamma dijet","p");
  lg->AddEntry(hdnhf,    "NH dijet",    "p");
  lg->AddEntry(hdchf,    "CH dijet",    "p");

  TLegend* lgJEF = new TLegend(0.15,0.25,0.4,0.35);
  //lgJEF->AddEntry(chnhf,"#bf{NH Run2016GH}", "p");
  //lgJEF->AddEntry(chf,  "#bf{CH Run2016GH}", "p");
  lgJEF->SetBorderSize(0); lgJEF->SetFillStyle(0);
  lgJEF->SetTextSize(0.035);


  hstack_JEF->Draw("HISTO");
  hstack_JEF->GetYaxis()->SetRangeUser(0.0,1.0);
  hstack_JEF->Draw("HISTO");
  hstack_JEF->SetTitle("");
  hstack_JEF->GetXaxis()->SetMoreLogLabels();
  hstack_JEF->GetXaxis()->SetNoExponent();

  hstack_JEF->GetXaxis()->SetTitleOffset(1.1);
  hstack_JEF->GetYaxis()->SetTitleOffset(0.9);
  hstack_JEF->GetYaxis()->SetTitle("Jet energy fraction");
  hstack_JEF->GetYaxis()->SetTitleSize(0.045);
  hstack_JEF->GetXaxis()->SetTitle("p_{T,tag}^{reco} (GeV)");
  hstack_JEF->GetXaxis()->SetTitleSize(0.05);

  hstackdj_JEF->Draw("SAME");




  //hstack_JEF->Draw("same"); //For error bars

  gStyle->SetHistTopMargin(0.);

  gPad->SetTickx(); 
  gPad->SetTicky();

  lg->Draw();
  lgJEF->Draw();
  //chf->Draw("samep");
  //chnhf->Draw("samep");

  TLatex *tex3 = new TLatex();
  tex3->SetNDC();
  tex3->SetTextSize(0.04);
  tex3->SetTextColor(kBlack);
  tex3->DrawLatex(0.21,0.41,"|#eta^{probe}|<1.3");
  tex3->DrawLatex(0.21,0.36,"#alpha<0.3");
  tex3->DrawLatex(0.75,0.92,"#sqrt{s}=13 TeV");


  string plotName3 = "./plots/DijetZjetComparison/JEF_dijetZjet.pdf";
  //string plotName = "./plots/particleComposition/JEF_probe_" + ReadName + ".pdf";
  canv_c->Print(plotName3.c_str());
  delete canv_c;

//Tracking efficiency


  TProfile *zjchhEff=0, *djchhEff=0;
  fzj->GetObject("chhEff",zjchhEff);
  fdj->GetObject("chhEff",djchhEff);

  //Charged hadron efficiency Profile
  TCanvas *c4     = new TCanvas("","c4",500,400);
  c4->SetLeftMargin(0.12);
  TGraph *PFeff   = new TGraph("data_and_MC_input/Efficiency/hadron_efficiency_PF.txt");
  TGraph *PFeffFr = new TGraph("data_and_MC_input/Efficiency/hadron_efficiencyPlusFakerate_PF.txt");
  PFeff->SetMarkerStyle(kFullDiamond );
  PFeff->SetMarkerColor(kBlue);
  PFeffFr->SetMarkerStyle(kFullCircle );
  PFeffFr->SetMarkerColor(kRed);

  //TH1D* hzjEff    = zjchhEff   ->ProjectionX();
  //TH1D* hdjEff    = djchhEff   ->ProjectionX();

  zjchhEff->SetLineColor(kBlack);
  zjchhEff->SetMarkerStyle(kFullCircle);
  //zjchhEff->SetLineWidth(5);
  djchhEff->SetLineColor(kBlack);
  djchhEff->SetMarkerStyle(kOpenCircle);
  //djchhEff->SetLineWidth(5);




  TLegend* lgEff = new TLegend(0.3,0.2,0.75,0.40 );
  lgEff->AddEntry(zjchhEff, "Z+jet", "p");
  lgEff->AddEntry(djchhEff, "Dijet", "p");
  lgEff->AddEntry(PFeffFr,"PF efficiency + fakerate", "p");
  lgEff->AddEntry(PFeff,  "PF efficiency", "p");
  lgEff->SetBorderSize(0); lgEff->SetFillStyle(0);

  PFeff->GetXaxis()->SetTitleOffset(1.0);
  PFeff->GetYaxis()->SetTitleOffset(1.3);
  PFeff->GetXaxis()->SetTitle("p_{T} (GeV)");
  PFeff->GetXaxis()->SetTitleSize(0.045);
  PFeff->GetYaxis()->SetTitle("Tracking efficiency");
  PFeff->GetYaxis()->SetTitleSize(0.045);

  TLatex *tex4 = new TLatex();
  tex4->SetNDC();
  tex4->SetTextSize(0.04);
  tex4->SetTextColor(kBlack);

  TAxis *PFaxis = PFeff->GetXaxis();
  PFaxis->SetLimits(0.2,300);
  PFeff->SetTitle("");
  PFeff->GetHistogram()->SetMaximum(1.0);
  PFeff->GetHistogram()->SetMinimum(0.5);
  PFeff->GetXaxis()->SetMoreLogLabels();
  PFeff->GetXaxis()->SetNoExponent();
  PFeff->Draw("ap"); 
  lgEff->Draw("same");
  PFeffFr->Draw("samep");

  PFeff->SetMarkerSize(1.5);
  PFeffFr->SetMarkerSize(1.2);

  gPad->SetTickx(); gPad->SetTicky();

  gStyle->SetOptStat();
  c4->SetLogx();
  zjchhEff->Draw("same");


  djchhEff->Draw("same");


  tex4->DrawLatex(0.75,0.82,"|#eta|<2.5");
  string plotName4 = "./plots/DijetZjetComparison/trkEff_dijetZjet.pdf";
  c4->Print(plotName4.c_str());
  delete c4;



}






















