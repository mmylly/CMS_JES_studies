{
   THStack *hs = new THStack("hs","test stacked histograms");
   TH1F *h1 = new TH1F("h1","test hstack",100,-4,4);
   h1->Sumw2();
   h1->FillRandom("gaus",20000);
   h1->SetFillColor(kRed);
   h1->SetMarkerStyle(21);
   h1->SetMarkerColor(kRed);
   hs->Add(h1,"hist");
   TH1F *h2 = new TH1F("h2","test hstack",100,-4,4);
   h2->Sumw2();
   h2->FillRandom("gaus",15000);
   h2->SetFillColor(kBlue);
   h2->SetMarkerStyle(21);
   h2->SetMarkerColor(kBlue);
   hs->Add(h2,"hist");
   TH1F *h3 = new TH1F("h3","test hstack",100,-4,4);
   h3->FillRandom("gaus",10000);
   h3->SetFillColor(kGreen);
   h3->SetMarkerStyle(21);
   h3->SetMarkerColor(kGreen);
   hs->Add(h3);
   hs->Draw();
}
