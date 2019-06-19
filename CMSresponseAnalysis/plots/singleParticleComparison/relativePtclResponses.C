#include "TCanvas.h"
#include "TF1.h"

//A handy function to read in parameter values
void reader(string fileNumer, string fileDenom, double eta, TF1* func) {

  //Open files for reading
  ifstream denom, numer;
  denom.open(fileDenom);  numer.open(fileNumer);
  if (!denom.is_open() || !numer.is_open()) {
    cout << "Error opening files!" << endl;
    return;
  }

  //Init
  string readDenom, readNumer;
  int lineFind = 0;			//Stepper to find the line to read
  double p[12];				//Dummies to read into
  for (int i=0; i!=12; ++i) p[i]=0;

  while (getline(denom,readDenom) &&
         getline(numer,readNumer) &&
         lineFind != (int(eta*10)-1)) ++lineFind;
  stringstream streamDenom, streamNumer;
  streamDenom << readDenom;  streamNumer << readNumer;
  string name = func->GetName();

  //Read params from the appropriate file
  //1st string on line is eta region low. bd. so read p0 twice to get the param.
  if (name=="frg") {		//gamma
    streamDenom >>p[0]>>p[0]>>p[1]>>p[2]>>p[3]>>p[4]>>p[5];
    streamNumer >>p[6]>>p[6]>>p[7]>>p[8]>>p[9]>>p[10]>>p[11];
    func->SetParameters(p);
    for (int i=0; i!=12; ++i) cout << func->GetParameter(i) << endl;
  } else if (name=="fre") {	//electron
    streamDenom >>p[0]>>p[0]>>p[1]>>p[2]>>p[3]>>p[4];
    streamNumer >>p[5]>>p[5]>>p[6]>>p[7]>>p[8]>>p[9];
    for (int i=0; i!=10; ++i) func->SetParameter(i,p[i]);
  } else if (name=="frmu") {	//muon
    streamDenom >>p[0]>>p[0]>>p[1]>>p[2]>>p[3];
    streamNumer >>p[4]>>p[4]>>p[5]>>p[6]>>p[7];
    for (int i=0; i!=8; ++i) func->SetParameter(i,p[i]);
  } else {
    streamDenom >>p[0]>>p[0]>>p[1]>>p[2];
    streamNumer >>p[3]>>p[3]>>p[4]>>p[5];
    for (int i=0; i!=6; ++i) func->SetParameter(i,p[i]);
  }
  denom.close();
  numer.close();

}

//Aux function for calculating relative photon response
Double_t fPhot(Double_t *x, Double_t *par) {
  double E = x[0];
  double p0=par[0];  double p1=par[1];  double p2 =par[2];   double p3 =par[3];
  double p4=par[4];  double p5=par[5];  double p6 =par[6];   double p7 =par[7];
  double p8=par[8];  double p9=par[9];  double p10=par[10];  double p11=par[11];
  // etc
  double rphot1 = 0.25*p0*( 1 + erf( (E + p1)/sqrt(2*p2*p2) ) )
                         *( 1 + erf( (E + p3)/sqrt(2*p4*p4) ) ) + p5;
  double rphot2 = 0.25*p6*( 1 + erf( (E + p7)/sqrt(2*p8*p8) ) )
                         *( 1 + erf( (E + p9)/sqrt(2*p10*p10) ) )+p11;
  return rphot1/rphot2;
}

/* THE MAIN FUNCTION */

void relativePtclResponses() {

  //Hadron response functions (for the main plot)
  string Rhad="[0]*(1-[1]*pow(x/0.75,[2]-1))/([3]*(1-[4]*pow(x/0.75,[5]-1)))";
  TF1* fr_p   = new TF1("frp",  Rhad.c_str(),0,251);
  TF1* fr_n   = new TF1("frn",  Rhad.c_str(),0,251);
  TF1* fr_pi  = new TF1("frpi", Rhad.c_str(),0,251);
  TF1* fr_K   = new TF1("frK",  Rhad.c_str(),0,251);
  TF1* fr_K0L = new TF1("frK0L",Rhad.c_str(),0,251);
  TF1* fr_K0S = new TF1("frK0S",Rhad.c_str(),0,251);
  TF1* fr_L   = new TF1("frL",  Rhad.c_str(),0,251);
  //A workaround to set axis range
  TF1* lim = new TF1("lim","0",0.8,10);
    
  //Photon response
  string Rgam = "0.25*[0]*( 1 + erf( (x + [1])/sqrt(2*[2]*[2]) ) )";
  Rgam +=               "*( 1 + erf( (x + [3])/sqrt(2*[4]*[4]) ) ) + [5]";
  Rgam +=     "/(0.25*[6]*( 1 + erf( (x + [7])/sqrt(2*[8]*[8]) ) )";
  Rgam +=               "*( 1 + erf( (x + [9])/sqrt(2*[10]*[10]) ) )+[11])";
  int nParPhot = 12;	//#parameters for the relative gamma response
  TF1* fr_gam = new TF1("frg",fPhot,0.01,251,nParPhot);

  //Electron response
  string Re = "0.25*[0]*( 1 + erf( (x + [1])/sqrt(2*[2]*[2]) ) )";
  Re +=               "*( 1 + erf( (x + [3])/sqrt(2*[4]*[4]) ) )";
  Re +=     "/(0.25*[5]*( 1 + erf( (x + [6])/sqrt(2*[7]*[7]) ) )";
  Re +=               "*( 1 + erf( (x + [8])/sqrt(2*[9]*[9]) ) ))";
  TF1* fr_e = new TF1("fre",Re.c_str(),0.0,251);

  //Muon response
  string Rmu = "([0]+[1]*x)*TMath::Landau(x,[2],[3])";
  Rmu +=     "/(([4]+[5]*x)*TMath::Landau(x,[6],[7]))";
  TF1* fr_mu = new TF1("frmu",Rmu.c_str(),0,251);

  //Higher resolution
  int reso = 2000;
  fr_gam->SetNpx(reso);   fr_mu->SetNpx(reso);   fr_e->SetNpx(reso);
  fr_p->SetNpx(reso);     fr_pi->SetNpx(reso);   fr_K->SetNpx(reso);
  fr_K0L->SetNpx(reso);   fr_K0S->SetNpx(reso);  fr_L->SetNpx(reso);
  fr_n->SetNpx(reso);

  //Colors
  fr_p->SetLineColor(  kBlack);    fr_pi->SetLineColor( kTeal);
  fr_e->SetLineColor(  kBlue);     fr_K->SetLineColor(  kViolet+1);
  fr_K0S->SetLineColor(kRed);      fr_K0L->SetLineColor(kAzure+7);
  fr_L->SetLineColor(  kGreen+1);  fr_mu->SetLineColor( kOrange+2);
  fr_gam->SetLineColor(kOrange);   fr_n->SetLineColor(  kGray+1);
  lim->SetLineColor(   kWhite);	//Invisible

  //Legend
  TLegend* leg = new TLegend(0.9,0.1,0.99,0.9);
  leg->AddEntry(fr_p,  "#font[12]{p}",        "l");
  leg->AddEntry(fr_n,  "#font[12]{n}",        "l");
  leg->AddEntry(fr_pi, "#pi^{#pm}",           "l");
  leg->AddEntry(fr_K,  "#font[12]{K^{#pm}}",  "l");
  leg->AddEntry(fr_K0S,"#font[12]{K^{0}_{S}}","l");
  leg->AddEntry(fr_K0L,"#font[12]{K^{0}_{L}}","l");
  leg->AddEntry(fr_L,  "#Lambda",             "l");
  leg->AddEntry(fr_e,  "#font[12]{e^{#pm}}",  "l");
  leg->AddEntry(fr_mu, "#mu^{#pm}",           "l");
  leg->AddEntry(fr_gam,"#gamma",              "l");

  //N.B. Set upper limit of the eta region to plot in 0.1 steps
  for (double etaMax = 0.1; etaMax <= /*0.15*/0.4/*3.2*/; etaMax += 0.1) {
    if (etaMax < 0.1 || etaMax > 3.2) {
      cout << "Particle outside good eta region" << endl;
      return;
    }

    //Instantiate and divide canvas
    TCanvas* canv = new TCanvas("c","",600,600);

    /* Response functions. Energy is x here due to ROOT conventions */

    /* Set params for different |eta| values and draw */
    string dir = "../../spr_mc/";
    string IIa = "RunIIa";
    string IIb="";
    bool P20ToP17 = true;	//Flag to use P20ToP17 corrected run IIb params
    if (P20ToP17) IIb = "RunIIb1-P20ToP17";
    else IIb = "RunIIb1";
    reader(dir+IIa+"/photon.txt",   dir+IIb+"/photon.txt",   etaMax, fr_gam);
    reader(dir+IIa+"/proton.txt",   dir+IIb+"/proton.txt",   etaMax, fr_p  );
    reader(dir+IIa+"/pion.txt",     dir+IIb+"/pion.txt",     etaMax, fr_pi );
    reader(dir+IIa+"/kaon.txt",     dir+IIb+"/kaon.txt",     etaMax, fr_K  );
    reader(dir+IIa+"/kshort.txt",   dir+IIb+"/kshort.txt",   etaMax, fr_K0S);
    reader(dir+IIa+"/klong.txt",    dir+IIb+"/klong.txt",    etaMax, fr_K0L);
    reader(dir+IIa+"/lambda.txt",   dir+IIb+"/lambda.txt",   etaMax, fr_L  );
    reader(dir+IIa+"/muon.txt",     dir+IIb+"/muon.txt",     etaMax, fr_mu );
    reader(dir+IIa+"/electron.txt", dir+IIb+"/electron.txt", etaMax, fr_e  );
    reader(dir+IIa+"/neutron.txt",  dir+IIb+"/neutron.txt",  etaMax, fr_n  );

    //General plot setup
    canv->SetLogx();
    fr_gam->GetYaxis()->SetTitle("#font[12]{R}");
    fr_gam->GetYaxis()->SetTitleSize(0.05);
    fr_gam->GetYaxis()->SetTitleOffset(0.7);
    fr_gam->SetMaximum(1.2);		//Set vertical...
    fr_gam->SetMinimum(0.8);		//...axis range
    fr_gam->GetXaxis()->SetTitle("#font[12]{E}_{#font[132]{gen}}");
    fr_gam->GetXaxis()->SetTitleSize(0.05);
    fr_gam->GetXaxis()->SetTitleOffset(0.3);
    fr_gam->GetXaxis()->SetNoExponent(1);

    //Construct plot title and figure filename
    stringstream title;
    stringstream figName;
    title << "#font[132]{"<<etaMax-0.1
          <<" < |#eta| < "<<etaMax<<", #font[12]{R}^{IIb}/#font[12]{R}^{IIa}}";
    if (P20ToP17) title << "#font[132]{, P20ToP17}";
    figName << "relative_resp_"
	    << int(etaMax-0.1) << int(10*etaMax-1) % 10 << "_eta_"
	    << int(etaMax)     << int(10*etaMax  ) % 10 << ".eps";
    fr_gam->SetTitle(title.str().c_str());

    //Plot
    fr_gam->Draw();		fr_e->Draw("SAME");	fr_mu->Draw("SAME");
    fr_pi->Draw("SAME");	fr_p->Draw("SAME");	fr_n->Draw("SAME");
    fr_K->Draw("SAME");		fr_K0S->Draw("SAME");	fr_K0L->Draw("SAME");
    fr_L->Draw("SAME");
    leg->Draw("SAME");

    //Save plot
    string printName = "./relativeResponses/runIIa_vs_runIIb1";
    if (P20ToP17) printName += "-P20ToP17/";
    else printName += "/";
    printName += figName.str();
    canv->Print(printName.c_str());

    //Free memory
    delete canv;

  } //Loop eta regions

  delete leg;

}
