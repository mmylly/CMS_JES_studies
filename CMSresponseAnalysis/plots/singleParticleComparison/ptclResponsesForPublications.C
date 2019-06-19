#include "TCanvas.h"
#include "TF1.h"

//A handy function to read in parameter values
void reader(string file, double eta, TF1* func) {

  //Open file for reading
  ifstream in;
  in.open(file);
  if (!in.is_open()) {
    cout << "Error opening file!" << endl;
    return;
  }

  //Init
  string read;
  int linesRead = 0;	//Stepper
  double p0=0,p1=0,p2=0,p3=0,p4=0,p5=0;	//Init dummies to read into
  while (getline(in,read) && linesRead != (int(eta*10)-1)) ++linesRead;
  stringstream stream;
  stream << read;
  string name = func->GetName();

  //Read params from the appropriate file
  //1st string on line is eta region low. bd. so read p0 twice to get the param.
  if (name.find("frg")!=string::npos) {
    stream >>p0>>p0>>p1>>p2>>p3>>p4>>p5;
    func->SetParameters(p0,p1,p2,p3,p4,p5);
  } else if (name.find("fre")!=string::npos) {
    stream >>p0>>p0>>p1>>p2>>p3>>p4;
    func->SetParameters(p0,p1,p2,p3,p4);
  } else if (name.find("frmu")!=string::npos) {
    stream >>p0>>p0>>p1>>p2>>p3;
    func->SetParameters(p0,p1,p2,p3);
  } else if (name.find("frS0")!=string::npos) {
    stream >>p0>>p0>>p1>>p2;	//Hadron style param. file
    func->SetParameters(p0, pow((1.192/0.75),1-p2), p2);
  } else if (name.find("frSm")!=string::npos) {
    stream >>p0>>p0>>p1>>p2;	//Hadron style param. file
    func->SetParameters(p0, pow((1.197/0.75),1-p2), p2);
  } else if (name.find("frSp")!=string::npos) {
    stream >>p0>>p0>>p1>>p2;	//Hadron style param. file
    func->SetParameters(p0, pow((1.189/0.75),1-p2), p2);
  } else if (name.find("frXi0")!=string::npos) {
    stream >>p0>>p0>>p1>>p2;	//Hadron style param. file
    func->SetParameters(p0, pow((1.315/0.75),1-p2), p2);
  } else if (name.find("frXim")!=string::npos) {
    stream >>p0>>p0>>p1>>p2;	//Hadron style param. file
    func->SetParameters(p0, pow((1.322/0.75),1-p2), p2);
  } else {
    stream >>p0>>p0>>p1>>p2;
    func->SetParameters(p0,p1,p2);
  }
  in.close();

}

/* THE MAIN FUNCTION */

void ptclResponsesForPublications() {

  //N.B. Set upper limit of the eta region to plot in 0.1 steps
  for (double etaMax = 0.1; etaMax < 0.15/*3.2*/; etaMax += 0.1) {
    if (etaMax < 0.1 || etaMax > 3.2) {
      cout << "Particle outside good eta region" << endl;
      return;
    }

    //Instantiate and divide canvas
    TCanvas* canv = new TCanvas("c","",600,450);

    /* Response functions. Energy is x here due to ROOT conventions */

    //Hadron response functions (for the main plot)
    TF1* fr_Ba =new TF1("frBa", "[0]*(1-[1]*pow(x/0.75,[2]-1))",0,251);//baryon
    TF1* fr_Ma =new TF1("frpia","[0]*(1-[1]*pow(x/0.75,[2]-1))",0,251);
    TF1* fr_Bb =new TF1("frBb", "[0]*(1-[1]*pow(x/0.75,[2]-1))",0,251);//baryon
    TF1* fr_Mb =new TF1("frpib","[0]*(1-[1]*pow(x/0.75,[2]-1))",0,251);
    //A workaround to set axis range
    TF1* lim = new TF1("lim","0",0.8,10);    
    //Photon response
    string Rgam = "0.25*[0]*( 1 + erf( (x + [1])/sqrt(2*[2]*[2]) ) )";
    Rgam +=               "*( 1 + erf( (x + [3])/sqrt(2*[4]*[4]) ) ) + [5]";
    TF1* fr_gama = new TF1("frga",Rgam.c_str(),0.01,251);
    TF1* fr_gamb = new TF1("frgb",Rgam.c_str(),0.01,251);
    //Electron response
    string Re = "(x>0.3?1:0)*0.25*[0]*( 1 + erf( (x + [1])/sqrt(2*[2]*[2]) ) )";
    Re +=                           "*( 1 + erf( (x + [3])/sqrt(2*[4]*[4]) ) )";
    TF1* fr_ea = new TF1("frea",Re.c_str(),0.0,251);
    TF1* fr_eb = new TF1("freb",Re.c_str(),0.0,251);
    //Muon response
    TF1* fr_mua=new TF1("frmua","([0]+[1]*x)*TMath::Landau(x,[2],[3])",0,251);
    TF1* fr_mub=new TF1("frmub","([0]+[1]*x)*TMath::Landau(x,[2],[3])",0,251);

    //Higher resolution
    int reso = 2000;
    fr_gama->SetNpx(reso);  fr_mua->SetNpx(reso);  fr_ea->SetNpx(reso);
    fr_Ba->SetNpx(reso);    fr_Ma->SetNpx(reso);
    fr_gamb->SetNpx(reso);  fr_mub->SetNpx(reso);  fr_eb->SetNpx(reso);
    fr_Bb->SetNpx(reso);    fr_Mb->SetNpx(reso);

    /* Set params for different |eta| values and draw */
    string dir = "./../../spr_mc/";
    string runa = "RunIIa";
    string runb = "RunIIb1-P20ToP17";
    reader(dir+runa+"/photon.txt",   etaMax, fr_gama);
    reader(dir+runa+"/proton.txt",   etaMax, fr_Ba  );
    reader(dir+runa+"/pion.txt",     etaMax, fr_Ma  );
    reader(dir+runa+"/muon.txt",     etaMax, fr_mua );
    reader(dir+runa+"/electron.txt", etaMax, fr_ea  );
    reader(dir+runb+"/photon.txt",   etaMax, fr_gamb);
    reader(dir+runb+"/proton.txt",   etaMax, fr_Bb  );
    reader(dir+runb+"/pion.txt",     etaMax, fr_Mb  );
    reader(dir+runb+"/muon.txt",     etaMax, fr_mub );
    reader(dir+runb+"/electron.txt", etaMax, fr_eb  );

    //Legend
    TLegend* lega = new TLegend(0.12,0.5,0.35,0.845);
    TLegend* legb = new TLegend(0.18,0.5,0.37,0.845);
    lega->SetBorderSize(0);  legb->SetBorderSize(0);	//No box around legend
    lega->AddEntry(fr_Ba,  " ","l");  lega->AddEntry(fr_Ma,  " ","l");
    lega->AddEntry(fr_ea,  " ","l");  lega->AddEntry(fr_mua, " ","l");
    lega->AddEntry(fr_gama," ","l");
    legb->AddEntry(fr_Bb,  "#font[12]{p}","l");
    legb->AddEntry(fr_Mb,  "#font[132]{#pi^{#pm}}", "l");
    legb->AddEntry(fr_eb,  "#font[12]{e^{#pm}}", "l");
    legb->AddEntry(fr_mub, "#mu^{#pm}",          "l");
    legb->AddEntry(fr_gamb,"#gamma",             "l");

    //Colors
    fr_Ba->SetLineColor( 42); fr_Ma->SetLineColor( 17); fr_ea->SetLineColor(9);
    fr_mua->SetLineColor(30); fr_gama->SetLineColor(1); lim->SetLineColor( 0);
    fr_Bb->SetLineColor( 42); fr_Mb->SetLineColor( 17); fr_eb->SetLineColor(9);
    fr_mub->SetLineColor(30); fr_gamb->SetLineColor(1);

    //Line widths, make baryon and meson bundles thicker than others
    fr_ea->SetLineWidth(3);  fr_mua->SetLineWidth(3); fr_gama->SetLineWidth(3);
    fr_Ba->SetLineWidth(3);  fr_Ma->SetLineWidth( 3);
    fr_eb->SetLineWidth(3);  fr_mub->SetLineWidth(3); fr_gamb->SetLineWidth(3);
    fr_Bb->SetLineWidth(3);  fr_Mb->SetLineWidth( 3);

    //Make run IIa response curves dashed
    fr_ea->SetLineStyle(kDashed);    fr_mua->SetLineStyle(kDashed);
    fr_gama->SetLineStyle(kDashed);  fr_Ba->SetLineStyle(kDashed);
    fr_Ma->SetLineStyle(kDashed);

    //General plot setup
    canv->SetLogx();
    fr_gama->GetYaxis()->SetTitle("#font[12]{R}");
    fr_gama->GetYaxis()->SetTitleSize(0.05);
    fr_gama->GetYaxis()->SetTitleOffset(0.7);
    fr_gama->SetMaximum(1.05);		//Set vertical...
    fr_gama->SetMinimum(0);		//...axis range
    fr_gama->GetXaxis()->SetTitle("#font[132]{#font[12]{E}_{gen} [GeV]}");
    fr_gama->GetXaxis()->SetTitleSize(0.05);
    fr_gama->GetXaxis()->SetTitleOffset(0.8);
    fr_gama->GetXaxis()->SetNoExponent(1);

    //Construct plot title and figure filename
    string title = "";
    fr_gama->SetTitle(title.c_str());


    //Plot
    fr_gama->Draw();       fr_Ma->Draw("SAME");  fr_Ba->Draw("SAME");
    fr_mua->Draw("SAME");  fr_ea->Draw("SAME");  lega->Draw("SAME");
    fr_gamb->Draw("SAME"); fr_Mb->Draw("SAME");  fr_Bb->Draw("SAME");
    fr_mub->Draw("SAME");  fr_eb->Draw("SAME");  legb->Draw("SAME");

    //Name the runs above the legend
    TLatex latex;
    latex.SetTextSize(0.05);
    latex.SetTextAlign(13);  //align at top
    latex.DrawLatex(0.015,1.04,	"#font[132]{IIa}");
    latex.DrawLatex(0.03,1.04,	"#font[132]{IIb}");
    latex.SetTextSize(0.025);
    latex.DrawLatex(0.025,0.99,	"P20ToP17");
    //Save plot
    stringstream figName;
    figName << "resp_"
	    << int(etaMax-0.1) << int(10*etaMax-1) % 10 << "_eta_"
	    << int(etaMax)     << int(10*etaMax  ) % 10
	    << ".eps";
    canv->Print(figName.str().c_str());

    delete canv;  delete lega;  delete legb;	//Free memory

  } //Loop eta regions

}
