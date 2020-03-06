void plotMPF()
{

  TFile* fzj = TFile::Open("./output_ROOT_files/varPlots_5000000.root");

  TProfile *przj_MPF=0;  TProfile *przj_MPFb=0;  TProfile *przj_MPFc=0;
  TProfile *przj_MPFs=0; TProfile *przj_MPFud=0; TProfile *przj_MPFg=0;

  fzj->GetObject("prMPF",   przj_MPF);
  fzj->GetObject("prMPFb",  przj_MPFb);
  fzj->GetObject("prMPFc",  przj_MPFc);
  fzj->GetObject("prMPFs",  przj_MPFs);
  fzj->GetObject("prMPFud", przj_MPFud);
  fzj->GetObject("prMPFg",  przj_MPFg);

  TH1D* hzj_MPF   = przj_MPF  ->ProjectionX();
  TH1D* hzj_MPFb  = przj_MPFb ->ProjectionX();
  TH1D* hzj_MPFc  = przj_MPFc ->ProjectionX();
  TH1D* hzj_MPFs  = przj_MPFs ->ProjectionX();
  TH1D* hzj_MPFud = przj_MPFud->ProjectionX();
  TH1D* hzj_MPFg  = przj_MPFg ->ProjectionX();

  //CMS MC points
  TGraphErrors* mc_zj_MPFntI_2018 = new TGraphErrors(nMC_MPFntI, zj_MC_pTp_MPFntI_2018,
                                                     zj_MC_MPFntI_2018, 0, zj_MC_MPFntI_ER_2018);
  //Canvas
  TCanvas* canv_MPF = new TCanvas("MPF","",500,400);
  canv_MPF->SetLeftMargin(0.13);
  canv_MPF->SetBottomMargin(0.13);
  canv_MPF->SetLogx();

  hzj_MPF->SetLineColor( kBlack);
  hzj_MPF->SetMarkerStyle(kFullCircle);       
  hzj_MPF->SetMarkerColor(kBlack);

  mc_zj_MPFntI_2018->SetMarkerStyle( kOpenSquare); mc_zj_MPFntI_2018->SetMarkerColor(kBlack);
  //Legend
  TLegend* lz_MPF = new TLegend(0.62,0.70,0.89,0.89);
  lz_MPF->SetBorderSize(0);
  lz_MPF->AddEntry(hzj_MPF, "All jets"  , "p");
  lz_MPF->AddEntry(mc_zj_MPFntI_2018, "FullSim 2018ABCD", "p");

  //Title and axis setup
  hzj_MPF->SetStats(0); //Suppress stat box
  hzj_MPF->SetTitle("");
  hzj_MPF->SetAxisRange(0.86,1.0,"Y"); //Vertical axis limits

  hzj_MPF->GetXaxis()->SetMoreLogLabels();
  hzj_MPF->GetXaxis()->SetNoExponent();
  hzj_MPF->GetYaxis()->SetTitleOffset(1.1);
  hzj_MPF->GetXaxis()->SetTitleOffset(1.1);
  hzj_MPF->GetXaxis()->SetTitle("p_{T,tag}^{reco} (GeV)");
  hzj_MPF->GetYaxis()->SetTitle("R_{MPF}");
  hzj_MPF->GetYaxis()->SetTitleSize(0.05);
  hzj_MPF->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx();   gPad->SetTicky();

  //Savefile name setup
  string savename = "./plots/mpf/MPF_zmmjet";
  string MPFtitle = hzj_MPF->GetTitle();
  savename+=".pdf";

  //Plot
  hzj_MPF->Draw("p");
  mc_zj_MPFntI_2018->Draw("P SAME");
  lz_MPF->Draw();

  //Save plot
  canv_MPF->Print(savename.c_str());

  //All flavours
  hzj_MPF->SetLineColor( kBlack);
  hzj_MPF->SetMarkerStyle(kFullCircle);       
  hzj_MPF->SetMarkerColor(kBlack);

  hzj_MPFb ->SetMarkerStyle(kFullSquare);       hzj_MPFb ->SetMarkerColor(kRed  );
  hzj_MPFg ->SetMarkerStyle(kFullTriangleUp);   hzj_MPFg ->SetMarkerColor(kBlue+1);
  hzj_MPFlq->SetMarkerStyle(kFullTriangleDown); hzj_MPFlq->SetMarkerColor(kGreen+2);
  hzj_MPFb ->SetLineColor(kRed+1);              hzj_MPFg->SetLineColor(kBlue+1);

}

