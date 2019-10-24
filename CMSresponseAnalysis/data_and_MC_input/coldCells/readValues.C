
void readValues() {
	TFile* filein = new TFile("coldjetsmc-18runA.root");
	TH2D* hist = (TH2D*)filein->Get("h2cold");
        cout << hist->GetNbinsX() << " " << hist->GetNbinsY() << endl;
        for (int i=1; i!=hist->GetNbinsX()+1; ++i) {
          for (int j=1; j!=hist->GetNbinsY()+1; ++j) {
            //cout << hist->GetXaxis()->GetBinLowEdge(0) << endl;
            //cout << endl;
            //cout << hist->GetXaxis()->GetBinLowEdge(i) << " " <<
            //        hist->GetXaxis()->GetBinLowEdge(i) + hist->GetXaxis()->GetBinWidth(i) << 
            //        " " << hist->GetXaxis()->GetBinWidth(i) << endl;

            

            //phiIdx = floor(((*prtclnij_phi)[k]+TMath::Pi())/phiStep) + 1;
            //etaIdx = floor(((*prtclnij_eta)[k]+5.2)/etaStep) + 1;

            //cout << hist->GetYaxis()->GetBinLowEdge(j) << " " <<
            //        hist->GetYaxis()->GetBinLowEdge(j) + hist->GetYaxis()->GetBinWidth(j) << 
            //        " " << hist->GetYaxis()->GetBinWidth(j) << endl;
            //cout << hist->GetXaxis()->GetBinCenter(i) << " " 
            //     << hist->GetYaxis()->GetBinCenter(j) << endl;
           
            if (hist->GetBinContent(i,j)>=0.0){
	    cout << hist->GetBinContent(i,j) << " ";
            cout << hist->GetXaxis()->GetBinCenter(i) << " ";
            cout << hist->GetYaxis()->GetBinCenter(j) << endl;}
          }
        }
        int iPhi; double phi;
        int iEta; double eta;
        eta = -2.2312;
        phi = -1.40658; //27,40,51.9279

        iPhi = floor((phi+fabs(hist->GetYaxis()->GetBinLowEdge(2)))/hist->GetYaxis()->GetBinWidth(2) + 2);
        if (phi < hist->GetYaxis()->GetBinLowEdge(2)) iPhi = 1;
        if (phi > hist->GetYaxis()->GetBinLowEdge(144)) iPhi = 145;
        iEta = floor((eta+fabs(hist->GetXaxis()->GetBinLowEdge(1)))/hist->GetXaxis()->GetBinWidth(1) + 1);
        cout << iEta << " " << iPhi << " " << hist->GetBinContent(iEta,iPhi) << endl;
}

