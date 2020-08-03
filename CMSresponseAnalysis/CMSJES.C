#define CMSJES_cxx
#include "CMSJES.h"

//Created using ROOT TMakeClass
void CMSJES::Loop()
{

  //fstream weight_stream;

  clock_t t_begin = clock();

  //Variant flags
  bool varCp3,       varCm3, 
       varHadHCALp3, varHadHCALm3, 
       varHadECALp3, varHadECALm3, 
       varTrkEffm1,  varTrkEffm3,
       varECALm1,    varECALm3,
       varPhotonm1,  varPhotonm3,
       varbfracp50,  varcfracp50;
  varCp3       = 0;
  varCm3       = 0;
  varHadHCALp3 = 0;
  varHadHCALm3 = 0;
  varHadECALp3 = 0;
  varHadECALm3 = 0;
  varTrkEffm1  = 0;
  varTrkEffm3  = 0;
  varECALm1    = 0;
  varECALm3    = 0;
  varPhotonm1  = 0;
  varPhotonm3  = 0;

  varbfracp50  = 0;
  varcfracp50  = 0;

  if ((varCp3 + varCm3 + varHadHCALp3 + varHadHCALm3 + varHadECALp3 + varHadECALm3 
       + varTrkEffm1 + varTrkEffm3 + varECALm1 + varECALm3 + varPhotonm1 + varPhotonm3
       + varbfracp50 + varcfracp50) > 1) 
    {cout << "More than one variation enabled!" << endl; return;}

  //Initialize rng's
  srand(1); //srand(time(0));
  TRandom3* gRandom = new TRandom3();
  gRandom->SetSeed(1); //gRandom->SetSeed(0);

  //Initialize cells with ECAL holes
  TFile* coldCellsFile = new TFile("./data_and_MC_input/coldCells/coldjetsmc-18runA.root");
  TH2D*  coldCells     = (TH2D*)coldCellsFile->Get("h2hole");

  int studyMode = -1;
  if (ReadName.find("Zjet")!=string::npos) {
    studyMode = 3;
    cout << "File is for Z+jet sample, with study mode: "<< studyMode << endl;
  } else if (ReadName.find("dijet")!=string::npos) {
    studyMode = 1;
    cout << "File is for dijet sample, with study mode: "<< studyMode << endl;
  } else {
    cout << "Error: File not for Z+jet or dijet sample! Exiting." << endl;
    return;
  }

  if (fChain == 0) return;
  Long64_t nentries          = fChain->GetEntriesFast();
  string outname             = "./output_ROOT_files/CMSJES_" + ReadName; //Output file
  if (varCp3)    outname    += "_varCp3";
  if (varCm3)    outname    += "_varCm3";
  if (varHadHCALp3) outname += "_varHadHCALp3";
  if (varHadHCALm3) outname += "_varHadHCALm3";
  if (varHadECALp3) outname += "_varHadECALp3";
  if (varHadECALm3) outname += "_varHadECALm3";
  if (varTrkEffm1) outname  += "_varTrkEffm1";
  if (varTrkEffm3) outname  += "_varTrkEffm3";
  if (varECALm1) outname    += "_varECALm1";
  if (varECALm3) outname    += "_varECALm3";
  if (varPhotonm1) outname  += "_varPhotonm1";
  if (varPhotonm3) outname  += "_varPhotonm3";
  if (varbfracp50) outname  += "_varbfracp50";
  if (varcfracp50) outname  += "_varcfracp50";

  outname += ".root";
  TFile *fout = new TFile(outname.c_str(),"RECREATE");

  //Binit https://github.com/miquork/jetphys/blob/master/settings.h_template
  //int const nbinsMPF = 15;
  //const double binsxMPF[nbinsMPF] = {31.75, 41.0, 50.5, 63.5, 83.0, 105.25, 132.5, 173.25, 228.25, 300.0, 391.25, 503.75, 681.75, 951.5, 1258.25};
  int const nbinsMPF = 25;
  const double binsxMPF[nbinsMPF] = 
  {15, 21, 28, 37, 49, 64, 84, 114, 153, 196, 272, 330, 395, 468, 548, 686, 846, 1032, 1248, 1588, 2000, 2500, 3103, 3832, 4713};


  //Jet flavour dependent MPF responses *b = b-jets, *g = g-jets, *lq=(u,d,s,c)-jets
  string MPFTitle = ";p_{T,reco}^{tag} [GeV];R_{MPF}";
  TProfile* prMPF   = new TProfile("prMPF"  , MPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prMPFb  = new TProfile("prMPFb" , MPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prMPFg  = new TProfile("prMPFg" , MPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prMPFlq = new TProfile("prMPFlq", MPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prMPFud = new TProfile("prMPFud", MPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prMPFs  = new TProfile("prMPFs" , MPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prMPFc  = new TProfile("prMPFc" , MPFTitle.c_str(), nbinsMPF-1, binsxMPF);

  string genMPFTitle = ";p_{T,gen}^{tag} [GeV];R_{MPF}^{gen}";
  TProfile* prgenMPF   = new TProfile("prgenMPF"  , genMPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prgenMPFb  = new TProfile("prgenMPFb" , genMPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prgenMPFg  = new TProfile("prgenMPFg" , genMPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prgenMPFlq = new TProfile("prgenMPFlq", genMPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prgenMPFud = new TProfile("prgenMPFud", genMPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prgenMPFs  = new TProfile("prgenMPFs" , genMPFTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prgenMPFc  = new TProfile("prgenMPFc" , genMPFTitle.c_str(), nbinsMPF-1, binsxMPF);



  //Weight profile
  TProfile* prWeight      = new TProfile("prWeight", ";p_{T,gen}^{jet} [GeV]", nbinsMPF-1, binsxMPF);
  TProfile* prWeight_tagr = new TProfile("prWeight_tagr", ";p_{T,reco}^{tag} [GeV]", nbinsMPF-1, binsxMPF);

  const double meanWeightP8dijet[nbinsMPF]      = {0.0218607, 0.00973148, 0.00353589, 0.00115248,
0.000358334, 0.000109171, 3.05491e-05, 8.09794e-06, 2.39395e-06, 6.86805e-07, 2.06424e-07, 8.95608e-08, 4.10371e-08, 1.97634e-08, 8.60722e-09, 3.25596e-09, 1.3113e-09, 5.50357e-10, 2.19032e-10, 7.6741e-11, 2.84612e-11, 1.10867e-11, 4.45531e-12, 1.93198e-12};

  const double meanWeightP8dijet_tagr[nbinsMPF] = {0.0180234, 0.00720017, 0.00257523, 0.0008387, 
0.000260716, 8.00849e-05, 2.26835e-05, 6.20397e-06, 1.82375e-06, 5.37633e-07, 1.58169e-07, 
6.85572e-08, 3.13857e-08, 1.50493e-08, 6.52928e-09, 2.4602e-09, 9.894e-10, 4.14897e-10, 
1.66348e-10, 5.9262e-11, 2.23251e-11, 8.83442e-12, 3.63681e-12, 1.59603e-12,};

  const double meanWeightH7Zjet[nbinsMPF]       = {0.0497897, 0.0291266, 0.015731, 0.00810937, 
0.00403705, 0.00196961, 0.000883006, 0.000377276, 0.00017098, 7.50946e-05, 3.41785e-05, 
1.97551e-05, 1.17914e-05, 7.28266e-06, 4.22776e-06, 2.23685e-06, 1.21976e-06, 
6.90772e-07, 3.80347e-07, 1.94956e-07, 1.02363e-07, 5.61487e-08, 4.6301e-08, 0}; 

  const double meanWeightH7Zjet_tagr[nbinsMPF]  = {0.122819, 0.0613592, 0.0288065, 0.0131956, 
0.00593023, 0.00267921, 0.00113128, 0.000465321, 0.000204634, 8.77645e-05, 3.91411e-05, 
2.23041e-05, 1.31884e-05, 8.07739e-06, 4.63002e-06, 2.42433e-06, 1.31735e-06, 7.40482e-07, 
4.10247e-07, 2.10503e-07, 1.11086e-07, 6.31412e-08, 4.00088e-08, 1.74876e-08};

  const double meanWeightH7dijet[nbinsMPF]      = {0.0212449, 0.0101049, 0.00348692, 0.00119542, 
0.000364203, 0.00010316, 3.24372e-05, 1.05647e-05, 2.18735e-06, 6.35687e-07, 1.90362e-07, 
8.26376e-08, 3.8129e-08, 1.85116e-08, 8.10081e-09, 3.11052e-09, 1.26076e-09, 5.40562e-10, 
2.18514e-10, 7.8032e-11, 2.94985e-11, 1.1506e-11, 4.63531e-12, 2.11902e-12};

  const double meanWeightH7dijet_tagr[nbinsMPF] = {0.0181432, 0.0074702, 0.00261957, 0.00088802, 
0.00024907, 7.7181e-05, 2.74693e-05, 6.90087e-06, 1.69465e-06, 5.02663e-07, 1.47365e-07, 
6.41552e-08, 2.9521e-08, 1.42132e-08, 6.20323e-09, 2.36782e-09, 9.64066e-10, 4.14199e-10, 
1.68973e-10, 6.09594e-11, 2.35871e-11, 9.25547e-12, 3.96632e-12, 1.67556e-12};


  //Jet response
  string RjetTitle    = ";p_{T,gen}^{jet} [GeV]";
         RjetTitle   += ";p_{T,reco}^{jet} / p_{T,gen}^{jet}";
  string RjetbTitle   = ";p_{T,gen}^{b-jet} [GeV]";
         RjetbTitle  += ";p_{T,reco}^{b-jet} / p_{T,gen}^{b-jet}";
  string RjetgTitle   = ";p_{T,gen}^{g-jet} [GeV]";
         RjetgTitle  += ";p_{T,reco}^{g-jet} / p_{T,gen}^{g-jet}";
  string RjetlqTitle  = ";p_{T,gen}^{lq-jet} [GeV]";
         RjetlqTitle += ";p_{T,reco}^{lq-jet} / p_{T,gen}^{lq-jet}";
  string RjetudTitle  = ";p_{T,gen}^{ud-jet} [GeV]";
         RjetudTitle += ";p_{T,reco}^{ud-jet} / p_{T,gen}^{ud-jet}";
  string RjetsTitle   = ";p_{T,gen}^{s-jet} [GeV]";
         RjetsTitle  += ";p_{T,reco}^{s-jet} / p_{T,gen}^{s-jet}";
  string RjetcTitle   = ";p_{T,gen}^{c-jet} [GeV]";
         RjetcTitle  += ";p_{T,reco}^{c-jet} / p_{T,gen}^{c-jet}";
  TProfile* prRjet   = new TProfile("prRjet",   RjetTitle.c_str(),   nbinsMPF-1, binsxMPF);
  TProfile* prRjetb  = new TProfile("prRjetb",  RjetbTitle.c_str(),  nbinsMPF-1, binsxMPF);
  TProfile* prRjetg  = new TProfile("prRjetg",  RjetgTitle.c_str(),  nbinsMPF-1, binsxMPF);
  TProfile* prRjetlq = new TProfile("prRjetlq", RjetlqTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prRjetud = new TProfile("prRjetud", RjetudTitle.c_str(), nbinsMPF-1, binsxMPF);
  TProfile* prRjets  = new TProfile("prRjets",  RjetsTitle.c_str(),  nbinsMPF-1, binsxMPF);
  TProfile* prRjetc  = new TProfile("prRjetc",  RjetcTitle.c_str(),  nbinsMPF-1, binsxMPF);

  //Jet response with tag binning
  string RjetvstagTitle    = ";p_{T,reco}^{tag} [GeV];p_{T,reco}^{jet} / p_{T,gen}^{jet}";
  string RjetvstagbTitle   = ";p_{T,reco}^{tag} [GeV];p_{T,reco}^{b-jet} / p_{T,gen}^{b-jet}";
  string RjetvstaggTitle   = ";p_{T,reco}^{tag} [GeV];p_{T,reco}^{g-jet} / p_{T,gen}^{g-jet}";
  string RjetvstaglqTitle  = ";p_{T,reco}^{tag} [GeV];p_{T,reco}^{lq-jet} / p_{T,gen}^{lq-jet}";
  string RjetvstagudTitle  = ";p_{T,reco}^{tag} [GeV];p_{T,reco}^{ud-jet} / p_{T,gen}^{ud-jet}";
  string RjetvstagsTitle   = ";p_{T,reco}^{tag} [GeV];p_{T,reco}^{s-jet} / p_{T,gen}^{s-jet}";
  string RjetvstagcTitle   = ";p_{T,reco}^{tag} [GeV];p_{T,reco}^{c-jet} / p_{T,gen}^{c-jet}";
  TProfile* prRjetvstag   = new TProfile("prRjetvstag",   RjetvstagTitle.c_str(),   nbinsMPF-1,binsxMPF);
  TProfile* prRjetvstagb  = new TProfile("prRjetvstagb",  RjetvstagbTitle.c_str(),  nbinsMPF-1,binsxMPF);
  TProfile* prRjetvstagg  = new TProfile("prRjetvstagg",  RjetvstaggTitle.c_str(),  nbinsMPF-1,binsxMPF);
  TProfile* prRjetvstaglq = new TProfile("prRjetvstaglq", RjetvstaglqTitle.c_str(), nbinsMPF-1,binsxMPF);
  TProfile* prRjetvstagud = new TProfile("prRjetvstagud", RjetvstagudTitle.c_str(), nbinsMPF-1,binsxMPF);
  TProfile* prRjetvstags  = new TProfile("prRjetvstags",  RjetvstagsTitle.c_str(),  nbinsMPF-1,binsxMPF);
  TProfile* prRjetvstagc  = new TProfile("prRjetvstagc",  RjetvstagcTitle.c_str(),  nbinsMPF-1,binsxMPF);

  TProfile* prRjet_calo  = new TProfile("prRjet_calo",  RjetcTitle.c_str(),  nbinsMPF-1, binsxMPF);

  //gen-jet particle type number
  TProfile* prgenNch   =new TProfile("prgenNch"   ,";p_{T,reco}^{tag} [GeV]; N_{ch}^{gen}",    nbinsMPF-1, binsxMPF);
  TProfile* prgenNnh   =new TProfile("prgenNnh"   ,";p_{T,reco}^{tag} [GeV]; N_{nh}^{gen}",    nbinsMPF-1, binsxMPF);
  TProfile* prgenNgamma=new TProfile("prgenNgamma",";p_{T,reco}^{tag} [GeV]; N_{#gamma}^{gen}",nbinsMPF-1, binsxMPF);
  TProfile* prgenNe    =new TProfile("prgenNe"    ,";p_{T,reco}^{tag} [GeV]; N_{e}^{gen}",     nbinsMPF-1, binsxMPF);
  TProfile* prgenNmu   =new TProfile("prgenNmu"   ,";p_{T,reco}^{tag} [GeV]; N_{mu}^{gen}",    nbinsMPF-1, binsxMPF);

  //reco-jet particle type number
  TProfile* prrecoNch   =new TProfile("prrecoNch"   ,";p_{T,reco}^{tag} [GeV]; N_{ch}^{reco}",    nbinsMPF-1, binsxMPF);
  TProfile* prrecoNnh   =new TProfile("prrecoNnh"   ,";p_{T,reco}^{tag} [GeV]; N_{nh}^{reco}",    nbinsMPF-1, binsxMPF);
  TProfile* prrecoNgamma=new TProfile("prrecoNgamma",";p_{T,reco}^{tag} [GeV]; N_{#gamma}^{reco}",nbinsMPF-1, binsxMPF);
  TProfile* prrecoNe    =new TProfile("prrecoNe"    ,";p_{T,reco}^{tag} [GeV]; N_{e}^{reco}",     nbinsMPF-1, binsxMPF);
  TProfile* prrecoNmu   =new TProfile("prrecoNmu"   ,";p_{T,reco}^{tag} [GeV]; N_{mu}^{reco}",    nbinsMPF-1, binsxMPF);

  //b-jets
  TProfile* prgenNchb    =new TProfile("prgenNchb"    ,";p_{T,reco}^{tag} [GeV]; N_{ch}^{gen} b-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prgenNnhb    =new TProfile("prgenNnhb"    ,";p_{T,reco}^{tag} [GeV]; N_{nh}^{gen} b-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prgenNgammab =new TProfile("prgenNgammab" ,";p_{T,reco}^{tag} [GeV]; N_{#gamma}^{gen} b-jets", nbinsMPF-1, binsxMPF);
  TProfile* prgenNeb     =new TProfile("prgenNeb"     ,";p_{T,reco}^{tag} [GeV]; N_{e}^{gen} b-jets",      nbinsMPF-1, binsxMPF);
  TProfile* prgenNmub    =new TProfile("prgenNmub"    ,";p_{T,reco}^{tag} [GeV]; N_{mu}^{gen} b-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prrecoNchb   =new TProfile("prrecoNchb"   ,";p_{T,reco}^{tag} [GeV]; N_{ch}^{reco} b-jets",    nbinsMPF-1, binsxMPF);
  TProfile* prrecoNnhb   =new TProfile("prrecoNnhb"   ,";p_{T,reco}^{tag} [GeV]; N_{nh}^{reco} b-jets",    nbinsMPF-1, binsxMPF);
  TProfile* prrecoNgammab=new TProfile("prrecoNgammab",";p_{T,reco}^{tag} [GeV]; N_{#gamma}^{reco} b-jets",nbinsMPF-1, binsxMPF);
  TProfile* prrecoNeb    =new TProfile("prrecoNeb"    ,";p_{T,reco}^{tag} [GeV]; N_{e}^{reco} b-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prrecoNmub   =new TProfile("prrecoNmub"   ,";p_{T,reco}^{tag} [GeV]; N_{mu}^{reco} b-jets",    nbinsMPF-1, binsxMPF);

  //gluon jet compositions
  TProfile* prgenNchg    =new TProfile("prgenNchg"    ,";p_{T,reco}^{tag} [GeV]; N_{ch}^{gen} gluon-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prgenNnhg    =new TProfile("prgenNnhg"    ,";p_{T,reco}^{tag} [GeV]; N_{nh}^{gen} gluon-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prgenNgammag =new TProfile("prgenNgammag" ,";p_{T,reco}^{tag} [GeV]; N_{#gamma}^{gen} gluon-jets", nbinsMPF-1, binsxMPF);
  TProfile* prgenNeg     =new TProfile("prgenNeg"     ,";p_{T,reco}^{tag} [GeV]; N_{e}^{gen} gluon-jets",      nbinsMPF-1, binsxMPF);
  TProfile* prgenNmug    =new TProfile("prgenNmug"    ,";p_{T,reco}^{tag} [GeV]; N_{mu}^{gen} gluon-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prrecoNchg   =new TProfile("prrecoNchg"   ,";p_{T,reco}^{tag} [GeV]; N_{ch}^{reco} gluon-jets",    nbinsMPF-1, binsxMPF);
  TProfile* prrecoNnhg   =new TProfile("prrecoNnhg"   ,";p_{T,reco}^{tag} [GeV]; N_{nh}^{reco} gluon-jets",    nbinsMPF-1, binsxMPF);
  TProfile* prrecoNgammag=new TProfile("prrecoNgammag",";p_{T,reco}^{tag} [GeV]; N_{#gamma}^{reco} gluon-jets",nbinsMPF-1, binsxMPF);
  TProfile* prrecoNeg    =new TProfile("prrecoNeg"    ,";p_{T,reco}^{tag} [GeV]; N_{e}^{reco} gluon-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prrecoNmug   =new TProfile("prrecoNmug"   ,";p_{T,reco}^{tag} [GeV]; N_{mu}^{reco} gluon-jets",    nbinsMPF-1, binsxMPF);

  //lq jet compositions
  TProfile* prgenNchlq    =new TProfile("prgenNchlq"    ,";p_{T,reco}^{tag} [GeV]; N_{ch}^{gen} lq-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prgenNnhlq    =new TProfile("prgenNnhlq"    ,";p_{T,reco}^{tag} [GeV]; N_{nh}^{gen} lq-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prgenNgammalq =new TProfile("prgenNgammalq" ,";p_{T,reco}^{tag} [GeV]; N_{#gamma}^{gen} lq-jets", nbinsMPF-1, binsxMPF);
  TProfile* prgenNelq     =new TProfile("prgenNelq"     ,";p_{T,reco}^{tag} [GeV]; N_{e}^{gen} lq-jets",      nbinsMPF-1, binsxMPF);
  TProfile* prgenNmulq    =new TProfile("prgenNmulq"    ,";p_{T,reco}^{tag} [GeV]; N_{mu}^{gen} lq-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prrecoNchlq   =new TProfile("prrecoNchlq"   ,";p_{T,reco}^{tag} [GeV]; N_{ch}^{reco} lq-jets",    nbinsMPF-1, binsxMPF);
  TProfile* prrecoNnhlq   =new TProfile("prrecoNnhlq"   ,";p_{T,reco}^{tag} [GeV]; N_{nh}^{reco} lq-jets",    nbinsMPF-1, binsxMPF);
  TProfile* prrecoNgammalq=new TProfile("prrecoNgammalq",";p_{T,reco}^{tag} [GeV]; N_{#gamma}^{reco} lq-jets",nbinsMPF-1, binsxMPF);
  TProfile* prrecoNelq    =new TProfile("prrecoNelq"    ,";p_{T,reco}^{tag} [GeV]; N_{e}^{reco} lq-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prrecoNmulq   =new TProfile("prrecoNmulq"   ,";p_{T,reco}^{tag} [GeV]; N_{mu}^{reco} lq-jets",    nbinsMPF-1, binsxMPF);

  //ud jet compositions
  TProfile* prgenNchud    =new TProfile("prgenNchud"    ,";p_{T,reco}^{tag} [GeV]; N_{ch}^{gen} ud-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prgenNnhud    =new TProfile("prgenNnhud"    ,";p_{T,reco}^{tag} [GeV]; N_{nh}^{gen} ud-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prgenNgammaud =new TProfile("prgenNgammaud" ,";p_{T,reco}^{tag} [GeV]; N_{#gamma}^{gen} ud-jets", nbinsMPF-1, binsxMPF);
  TProfile* prgenNeud     =new TProfile("prgenNeud"     ,";p_{T,reco}^{tag} [GeV]; N_{e}^{gen} ud-jets",      nbinsMPF-1, binsxMPF);
  TProfile* prgenNmuud    =new TProfile("prgenNmuud"    ,";p_{T,reco}^{tag} [GeV]; N_{mu}^{gen} ud-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prrecoNchud   =new TProfile("prrecoNchud"   ,";p_{T,reco}^{tag} [GeV]; N_{ch}^{reco} ud-jets",    nbinsMPF-1, binsxMPF);
  TProfile* prrecoNnhud   =new TProfile("prrecoNnhud"   ,";p_{T,reco}^{tag} [GeV]; N_{nh}^{reco} ud-jets",    nbinsMPF-1, binsxMPF);
  TProfile* prrecoNgammaud=new TProfile("prrecoNgammaud",";p_{T,reco}^{tag} [GeV]; N_{#gamma}^{reco} ud-jets",nbinsMPF-1, binsxMPF);
  TProfile* prrecoNeud    =new TProfile("prrecoNeud"    ,";p_{T,reco}^{tag} [GeV]; N_{e}^{reco} ud-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prrecoNmuud   =new TProfile("prrecoNmuud"   ,";p_{T,reco}^{tag} [GeV]; N_{mu}^{reco} ud-jets",    nbinsMPF-1, binsxMPF);

  //s jet compositions
  TProfile* prgenNchs    =new TProfile("prgenNchs"    ,";p_{T,reco}^{tag} [GeV]; N_{ch}^{gen} s-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prgenNnhs    =new TProfile("prgenNnhs"    ,";p_{T,reco}^{tag} [GeV]; N_{nh}^{gen} s-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prgenNgammas =new TProfile("prgenNgammas" ,";p_{T,reco}^{tag} [GeV]; N_{#gamma}^{gen} s-jets", nbinsMPF-1, binsxMPF);
  TProfile* prgenNes     =new TProfile("prgenNes"     ,";p_{T,reco}^{tag} [GeV]; N_{e}^{gen} s-jets",      nbinsMPF-1, binsxMPF);
  TProfile* prgenNmus    =new TProfile("prgenNmus"    ,";p_{T,reco}^{tag} [GeV]; N_{mu}^{gen} s-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prrecoNchs   =new TProfile("prrecoNchs"   ,";p_{T,reco}^{tag} [GeV]; N_{ch}^{reco} s-jets",    nbinsMPF-1, binsxMPF);
  TProfile* prrecoNnhs   =new TProfile("prrecoNnhs"   ,";p_{T,reco}^{tag} [GeV]; N_{nh}^{reco} s-jets",    nbinsMPF-1, binsxMPF);
  TProfile* prrecoNgammas=new TProfile("prrecoNgammas",";p_{T,reco}^{tag} [GeV]; N_{#gamma}^{reco} s-jets",nbinsMPF-1, binsxMPF);
  TProfile* prrecoNes    =new TProfile("prrecoNes"    ,";p_{T,reco}^{tag} [GeV]; N_{e}^{reco} s-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prrecoNmus   =new TProfile("prrecoNmus"   ,";p_{T,reco}^{tag} [GeV]; N_{mu}^{reco} s-jets",    nbinsMPF-1, binsxMPF);

  //c jet compositions
  TProfile* prgenNchc    =new TProfile("prgenNchc"    ,";p_{T,reco}^{tag} [GeV]; N_{ch}^{gen} c-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prgenNnhc    =new TProfile("prgenNnhc"    ,";p_{T,reco}^{tag} [GeV]; N_{nh}^{gen} c-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prgenNgammac =new TProfile("prgenNgammac" ,";p_{T,reco}^{tag} [GeV]; N_{#gamma}^{gen} c-jets", nbinsMPF-1, binsxMPF);
  TProfile* prgenNec     =new TProfile("prgenNec"     ,";p_{T,reco}^{tag} [GeV]; N_{e}^{gen} c-jets",      nbinsMPF-1, binsxMPF);
  TProfile* prgenNmuc    =new TProfile("prgenNmuc"    ,";p_{T,reco}^{tag} [GeV]; N_{mu}^{gen} c-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prrecoNchc   =new TProfile("prrecoNchc"   ,";p_{T,reco}^{tag} [GeV]; N_{ch}^{reco} c-jets",    nbinsMPF-1, binsxMPF);
  TProfile* prrecoNnhc   =new TProfile("prrecoNnhc"   ,";p_{T,reco}^{tag} [GeV]; N_{nh}^{reco} c-jets",    nbinsMPF-1, binsxMPF);
  TProfile* prrecoNgammac=new TProfile("prrecoNgammac",";p_{T,reco}^{tag} [GeV]; N_{#gamma}^{reco} c-jets",nbinsMPF-1, binsxMPF);
  TProfile* prrecoNec    =new TProfile("prrecoNec"    ,";p_{T,reco}^{tag} [GeV]; N_{e}^{reco} c-jets",     nbinsMPF-1, binsxMPF);
  TProfile* prrecoNmuc   =new TProfile("prrecoNmuc"   ,";p_{T,reco}^{tag} [GeV]; N_{mu}^{reco} c-jets",    nbinsMPF-1, binsxMPF);


  //Jet flavour fraction histos: FFb = b-jets, FFg = g-jets, FFlq=(u,d,s,c)-jets
  TH1D* FFb = new TH1D("FFb",  "",nbinsMPF-1,binsxMPF);
  TH1D* FFg = new TH1D("FFg",  "",nbinsMPF-1,binsxMPF);
  TH1D* FFud= new TH1D("FFud", "",nbinsMPF-1,binsxMPF);
  TH1D* FFs = new TH1D("FFs",  "",nbinsMPF-1,binsxMPF);
  TH1D* FFc = new TH1D("FFc",  "",nbinsMPF-1,binsxMPF);
  TH1D* FFlq= new TH1D("FFlq", "",nbinsMPF-1,binsxMPF);
  TH1D* FFa = new TH1D("FFa",  "",nbinsMPF-1,binsxMPF); //All

  //Response function R_h (h for hadron)
  TF1 *fr_h = new TF1("frh","[0]*(1-[1]*pow(x,[2]-1))",0,4000);

  // C-parameter variation +- 3%
  //if (varCp3) { fr_h = new TF1("frh","1.03*[0]*(1-[1]*pow(x,[2]-1))",0,4000);}
  //if (varCm3) { fr_h = new TF1("frh","0.97*[0]*(1-[1]*pow(x,[2]-1))",0,4000);}
  fr_h->Print();

  //Used in HCAL calibration, pion response parameters now in |eta|<1.3
  TF1 *fr_all  = new TF1("fr_all", "x*1.044*(1-1.293*pow(x,0.543-1))",0,4000);
  TF1 *fr_hcal = new TF1("fr_hcal","x*1.123*(1-1.345*pow(x,0.421-1))",0,4000);
  TF1 *fr_ecal = new TF1("fr_ecal","x*1.076*(1-1.403*pow(x,0.646-1))",0,4000);

  //Higher resolution
  int reso = 10000; fr_h->SetNpx(reso); fr_hcal->SetNpx(reso); fr_ecal->SetNpx(reso);

  // INITIALIZATIONS
  Long64_t nbytes = 0, nb = 0;
  TLorentzVector p4;      //4-vector temp.
  TLorentzVector p4_calo; //calorimeter cell 4-vector

  //Calorimeter and track 4-vectors
  TLorentzVector p4_chc;
  TLorentzVector p4_cht;
  TLorentzVector p4_rescale; //rescaled 4-vector 

  TLorentzVector tag_g;      //Parton level tag object 4-vector
  TLorentzVector tag_r;	     //With muon smearing

  TLorentzVector probe_g;    //Generator level without neutrinos  
  TLorentzVector probe_r;    //PF reconstructed probe 4-vector
  TLorentzVector probe_calo; //Calo reconstructed probe 4-vector

  TLorentzVector jet2_g;     //Second leading pT jet
  TLorentzVector jet2_r;     //Second leading pT jet
  TLorentzVector jet3_g;     //Third leading pT jet
  TLorentzVector jet3_r;     //Third leading pT jet

  //Cell energies of the probe object
  double probe_ch;
  double probe_nh;
  double probe_gamma;
  double probe_e;
  double probe_mu;
  double jet2_ch;
  double jet2_nh;
  double jet2_gamma;
  double jet2_e;
  double jet2_mu;

  //Particle type numbers in gen level
  int probe_genNch;
  int probe_genNnh;
  int probe_genNgamma;
  int probe_genNe;
  int probe_genNmu;
  int jet2_genNch;
  int jet2_genNnh;
  int jet2_genNgamma;
  int jet2_genNe;
  int jet2_genNmu;

  //Particle type numbers in reco level
  int probe_recoNch;
  int probe_recoNnh;
  int probe_recoNgamma;
  int probe_recoNe;
  int probe_recoNmu;
  int jet2_recoNch;
  int jet2_recoNnh;
  int jet2_recoNgamma;
  int jet2_recoNe;
  int jet2_recoNmu;

  //Partial derivative values for a hadron
  unsigned int i_tag   = 0;   //Stepper to find tag object index
  unsigned int i_tag1;        //1st tag muon
  unsigned int i_tag2;        //2nd tag muon
  int i_mujet1;
  int i_mujet2;
  unsigned int i_probe = 0;   //       -||-     probe jet index
  unsigned int i_jet2  = 1;   //       -||-     2nd jet index
  unsigned int i_jet3  = 2;   //       -||-     3rd jet index
  double eta_muon      = 2.3; //Max |eta| for a single muon in Z+jet      
  double eta_tag_z     = 2.5; //Max |eta| for mumu tag object
  double eta_probe     = 1.3; //Max |eta| for probe jets
  int PDG              = 1;   //Shorthand, to store a particle's PDGID
  int JI               = 0;   //Shorthand, particle's jet index
  double phiMin        = 2.8; //Minimum azimuth angle between tag and probe
  double pTmin_probe_g = 10;  //Minimum probe gen-jet p_T (GeV)
  double pTmin_probe_r = 15;  //Minimum probe reco-jet p_T (GeV)
  double pTmin_muon    = 15;  //Minimum single tag muon pT (GeV)
  double pTmin_tag_z   = 15;  //Minimum tag muon pair pT (GeV)   
  double resp          = 1.0; //SPR value                (dummy init)
  double respA         = 0.0; //Calorimeter response for All-hadrons
  double respEHE       = 0.0; //Calorimeter response for EHE-hadrons
  double respHHe       = 0.0; //Calorimeter response for HHe-hadrons
  double R_MPF_g       = 0.0;
  double R_MPF_r       = 0.0;   //MC-reco'd MPF response
  double Rjet          = 0.0; //Jet pT-response
  double Rjet_calo     = 0.0; //Calo jet pT-response

  double RCone         = 0.4; //Jet radius

  unsigned long njets;          //#jets in the event, for resizing vectors

  TLorentzVector MET_g;
  TLorentzVector MET_r;         //Reconstructed MET four vector

  double sumEt         = 0.0;
  vector<TLorentzVector> jets_g;//Gen lvl jet 4-vectors

  //HHe / (HHe + EHE)
  double HHeFrac = 0.45;

  //CMS detector related parameters
  double Bfield = 3.8; //Tesla
  double Rt     = 1.2; //Tracker radius

  //MET calculation related variables
  double cellPhi; double cellEta; double cellE;  double cellPt;  double delta; double cellSigma;
  double eff;     double newPhi;  double calReso; double trkReso; bool trkFail; 
  double cellE_calo; int probeFlav;

  //Histograms to contain the gen level particle content in probe
  TH1F* h_e;      TH1F* h_mu;     TH1F* h_gamma;  TH1F* h_gpi0;
  TH1F* h_K;      TH1F* h_K0S;    TH1F* h_K0L;    TH1F* h_L;
  TH1F* h_p;      TH1F* h_n;      TH1F* h_nu;     TH1F* h_pi;
  TH1F* h_Xi0;    TH1F* h_Xim;    TH1F* h_Om;     TH1F* h_Sig0;
  TH1F* h_Sigm;   TH1F* h_Sigp;   
  TH1F* h_other;  TH1F* h_all;	//For normalizing histo stacks
  TH1F* h_ptr;	//To contain one of the above pointer sets at a time
  vector<int> otherIDs;	//To contain PDGIDs of unclassified particles


  //Machinery for histogram stacking
  vector<string> hTitles;
  THStack* hstack;
  THStack* hstackOther;
  //hTitles.push_back("#font[132]{Z+jet probe particle content (gen)}");
  hTitles.push_back("");

  //Instantiate and setup composition histos if needed
  int const nbins_h = 30; int const nbins_chf = 23;
  const double bins_h_lo = 0.0;
  const double bins_h_hi = 3000;
  //From https://github.com/miquork/jetphys/blob/master/settings.h_template
  const double bins_chf[24] = {15, 21, 28, 37, 49, 64, 84, 114, 153, 196, 272, 330, 395, 468,
                               548, 686, 846, 1032, 1248, 1588, 2000, 2500, 3103, 3832};

  //Jet energy fraction profiles
  TProfile* prchf   =new TProfile("prchf"   ,";p_{T,reco}^{tag} [GeV]; chf",   nbins_chf,bins_chf);
  TProfile* prnhf   =new TProfile("prnhf"   ,";p_{T,reco}^{tag} [GeV]; nhf",   nbins_chf,bins_chf);
  TProfile* prgammaf=new TProfile("prgammaf",";p_{T,reco}^{tag} [GeV]; gammaf",nbins_chf,bins_chf);
  TProfile* pref    =new TProfile("pref"    ,";p_{T,reco}^{tag} [GeV]; ef",    nbins_chf,bins_chf);
  TProfile* prmuf   =new TProfile("prmuf"   ,";p_{T,reco}^{tag} [GeV]; muf",   nbins_chf,bins_chf);

  //b jet compositions
  TProfile* prchfb   =new TProfile("prchfb"   ,";p_{T,reco}^{tag} [GeV]; chf b-jets",   nbins_chf,bins_chf);
  TProfile* prnhfb   =new TProfile("prnhfb"   ,";p_{T,reco}^{tag} [GeV]; nhf b-jets",   nbins_chf,bins_chf);
  TProfile* prgammafb=new TProfile("prgammafb",";p_{T,reco}^{tag} [GeV]; gammaf b-jets",nbins_chf,bins_chf);
  TProfile* prefb    =new TProfile("prefb"    ,";p_{T,reco}^{tag} [GeV]; ef b-jets",    nbins_chf,bins_chf);
  TProfile* prmufb   =new TProfile("prmufb"   ,";p_{T,reco}^{tag} [GeV]; muf b-jets",   nbins_chf,bins_chf);

  //gluon jet compositions
  TProfile* prchfg   =new TProfile("prchfg"   ,";p_{T,reco}^{tag} [GeV]; chf gluon-jets",   nbins_chf,bins_chf);
  TProfile* prnhfg   =new TProfile("prnhfg"   ,";p_{T,reco}^{tag} [GeV]; nhf gluon-jets",   nbins_chf,bins_chf);
  TProfile* prgammafg=new TProfile("prgammafg",";p_{T,reco}^{tag} [GeV]; gammaf gluon-jets",nbins_chf,bins_chf);
  TProfile* prefg    =new TProfile("prefg"    ,";p_{T,reco}^{tag} [GeV]; ef gluon-jets",    nbins_chf,bins_chf);
  TProfile* prmufg   =new TProfile("prmufg"   ,";p_{T,reco}^{tag} [GeV]; muf gluon-jets",   nbins_chf,bins_chf);

  //lq jet compositions
  TProfile* prchflq   =new TProfile("prchflq"   ,";p_{T,reco}^{tag} [GeV]; chf lq-jets",   nbins_chf,bins_chf);
  TProfile* prnhflq   =new TProfile("prnhflq"   ,";p_{T,reco}^{tag} [GeV]; nhf lq-jets",   nbins_chf,bins_chf);
  TProfile* prgammaflq=new TProfile("prgammaflq",";p_{T,reco}^{tag} [GeV]; gammaf lq-jets",nbins_chf,bins_chf);
  TProfile* preflq    =new TProfile("preflq"    ,";p_{T,reco}^{tag} [GeV]; ef lq-jets",    nbins_chf,bins_chf);
  TProfile* prmuflq   =new TProfile("prmuflq"   ,";p_{T,reco}^{tag} [GeV]; muf lq-jets",   nbins_chf,bins_chf);

  //ud jet compositions
  TProfile* prchfud   =new TProfile("prchfud"   ,";p_{T,reco}^{tag} [GeV]; chf ud-jets",   nbins_chf,bins_chf);
  TProfile* prnhfud   =new TProfile("prnhfud"   ,";p_{T,reco}^{tag} [GeV]; nhf ud-jets",   nbins_chf,bins_chf);
  TProfile* prgammafud=new TProfile("prgammafud",";p_{T,reco}^{tag} [GeV]; gammaf ud-jets",nbins_chf,bins_chf);
  TProfile* prefud    =new TProfile("prefud"    ,";p_{T,reco}^{tag} [GeV]; ef ud-jets",    nbins_chf,bins_chf);
  TProfile* prmufud   =new TProfile("prmufud"   ,";p_{T,reco}^{tag} [GeV]; muf ud-jets",   nbins_chf,bins_chf);

  //s jet compositions
  TProfile* prchfs   =new TProfile("prchfs"   ,";p_{T,reco}^{tag} [GeV]; chf s-jets",   nbins_chf,bins_chf);
  TProfile* prnhfs   =new TProfile("prnhfs"   ,";p_{T,reco}^{tag} [GeV]; nhf s-jets",   nbins_chf,bins_chf);
  TProfile* prgammafs=new TProfile("prgammafs",";p_{T,reco}^{tag} [GeV]; gammaf s-jets",nbins_chf,bins_chf);
  TProfile* prefs    =new TProfile("prefs"    ,";p_{T,reco}^{tag} [GeV]; ef s-jets",    nbins_chf,bins_chf);
  TProfile* prmufs   =new TProfile("prmufs"   ,";p_{T,reco}^{tag} [GeV]; muf s-jets",   nbins_chf,bins_chf);

  //c jet compositions
  TProfile* prchfc   =new TProfile("prchfc"   ,";p_{T,reco}^{tag} [GeV]; chf c-jets",   nbins_chf,bins_chf);
  TProfile* prnhfc   =new TProfile("prnhfc"   ,";p_{T,reco}^{tag} [GeV]; nhf c-jets",   nbins_chf,bins_chf);
  TProfile* prgammafc=new TProfile("prgammafc",";p_{T,reco}^{tag} [GeV]; gammaf c-jets",nbins_chf,bins_chf);
  TProfile* prefc    =new TProfile("prefc"    ,";p_{T,reco}^{tag} [GeV]; ef c-jets",    nbins_chf,bins_chf);
  TProfile* prmufc   =new TProfile("prmufc"   ,";p_{T,reco}^{tag} [GeV]; muf c-jets",   nbins_chf,bins_chf);

  if (GetcontHistos()) {
    h_e     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //e^+-
    h_gamma = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Photons
    h_gpi0  = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //pi^0 phot.
    h_K     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Kaons K^+-
    h_K0S   = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //K^0_S
    h_K0L   = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //K^0_L
    h_L     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Lambda
    h_mu    = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //mu
    h_n     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Neutrons
    h_nu    = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Neutrinos
    h_pi    = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //pi^+-
    h_p     = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Protons
    h_other = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi);
    h_Xi0   = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Xi^0
    h_Xim   = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Xi^-
    h_Sig0  = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Sigma^0
    h_Sigm  = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Sigma^-
    h_Sigp  = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Sigma^+
    h_Om    = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi); //Omega^-
    //All particle types
    h_all   = new TH1F("", "", nbins_h, bins_h_lo, bins_h_hi);
    //For stacking the above histograms to plot them simultaneously
    hstack      = new THStack("", hTitles[0].c_str() );
    hstackOther = new THStack("","");
    //Fill colours
    h_e->SetFillColor(    kAzure+1);  h_mu->SetFillColor(  kViolet-3);
    h_gamma->SetFillColor(41);        h_gpi0->SetFillColor(42);
    h_pi->SetFillColor(   kAzure-9);  h_K->SetFillColor(    kTeal-9);
    h_K0S->SetFillColor(  kTeal-8);   h_K0L->SetFillColor( kTeal-7);
    h_L->SetFillColor(    kCyan-1);
    h_p->SetFillColor(    kGray+2);   h_n->SetFillColor(   kGray);
    h_other->SetFillColor(kOrange-9); h_nu->SetFillColor(  kWhite);
    h_Xi0->SetFillColor(  kOrange-8); h_Xim->SetFillColor( kOrange-4);
    h_Sig0->SetFillColor( kOrange-7); h_Sigm->SetFillColor(kOrange-5);
    h_Sigp->SetFillColor( kOrange-9); h_Om->SetFillColor(  kOrange-3);
    //More narrow lines for better readability
    h_e->SetLineWidth(    1);  h_mu->SetLineWidth(   1);
    h_gamma->SetLineWidth(1);  h_gpi0->SetLineWidth( 1);
    h_pi->SetLineWidth(   1);  h_K->SetLineWidth(    1);
    h_K0S->SetLineWidth(  1);  h_K0L->SetLineWidth(  1);
    h_p->SetLineWidth(    1);  h_n->SetLineWidth(    1);
    h_L->SetLineWidth(    1);
    h_other->SetLineWidth(1);  h_nu->SetLineWidth(   1);
    h_Xi0->SetLineWidth(  1);  h_Xim->SetLineWidth(  1);
    h_Sig0->SetLineWidth( 1);  h_Sigm->SetLineWidth( 1);
    h_Sigp->SetLineWidth( 1);  h_Om->SetLineWidth(   1);
    //Line colours
    h_e->SetLineColor(    kBlack);  h_mu->SetLineColor(   kBlack);
    h_gamma->SetLineColor(kBlack);  h_gpi0->SetLineColor( kBlack);
    h_pi->SetLineColor(   kBlack);  h_K->SetLineColor(    kBlack);
    h_K0S->SetLineColor(  kBlack);  h_K0L->SetLineColor(  kBlack);
    h_p->SetLineColor(    kBlack);  h_n->SetLineColor(    kBlack);
    h_L->SetLineColor(    kBlack);
    h_other->SetLineColor(kBlack);  h_nu->SetLineColor(   kBlack);
    h_Xi0->SetLineColor(  kBlack);  h_Xim->SetLineColor(  kBlack);
    h_Sig0->SetLineColor( kBlack);  h_Sigm->SetLineColor( kBlack);
    h_Sigp->SetLineColor( kBlack);  h_Om->SetLineColor(   kBlack);
  }

  /* LOOP EVENTS */
  cout << "Entering loop";
  if (!printProg) cout << ", progress printing disabled"; cout << endl;

  //Machinery for skipping events that didn't pass cuts the last time
  fstream cutflag_stream;
  string cutflag_file = "./cutflag_files/" + ReadName;
  cutflag_file += ".bin";
  if (GetuseEarlierCuts()) cout << "Checking for "; 
  else                     cout << "Writing new ";
  cout << cutflag_file << endl;
  char readTemp;
  Long64_t rejected=0, passed=0; //#events rejected and passed in sample

  //Read usable events from file, else write to file at the end of each looping
  if (GetuseEarlierCuts()) {
    cutflag_stream.open(cutflag_file, std::ios::in);		//Read mode
    for (Long64_t l=1; cutflag_stream.read(&readTemp,1); ++l) {
      cutflag_stream.seekg(l);		//Proceed to next byte in file
      if (readTemp=='1') {passedCuts.push_back(true);  ++passed;  }
      else               {passedCuts.push_back(false); ++rejected;}
    }
    cutflag_stream.close();
    if (passedCuts.size()==0) {
      cout << "The file contains no events, writing a new file" << endl;
      SetuseEarlierCuts(false);
      for (long l=0; l != nentries; ++l) passedCuts.push_back(false);
    } else if (GetprintProg()) {
      cout << "Read " << rejected + passed << " events:" << endl;
      cout << "  Passed " << passed << ", rejected " << rejected << endl;
      cout << "  passedCuts.size()=" << passedCuts.size() << " events" << endl;
    }
  } else for (long l=0; l != nentries; ++l) passedCuts.push_back(false);

  //For keeping track of how many events pass each event cut
  int mumuCut = 0;
  int invM = 0;
  int tagCut = 0;
  int alpha = 0;
  int genprobeCut = 0;
  int b2b = 0;
  int probeCut = 0;
  int METcut = 0;
  int weightCut = 0;
  int evt = 0;

//***********************************************************************************************
  //Granularity of cells following the granularity of the HCAL 
  int nPhi = 72; int nEta = 119;
  TH2D* cht          = new TH2D("cht"          ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* chtPt        = new TH2D("chtPt"        ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* chtP_curv    = new TH2D("chtP_curv"    ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* chcECAL      = new TH2D("chcECAL"      ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* chcHCAL      = new TH2D("chcHCAL"      ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* chECAL_curv  = new TH2D("chECAL_curv"  ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* chHCAL_curv  = new TH2D("chHCAL_curv"  ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* sigmaTrk     = new TH2D("sigmaTrk"     ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* sigmaTrk_curv= new TH2D("sigmaTrk_curv","",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* nhECAL       = new TH2D("nhECAL"       ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* nhHCAL       = new TH2D("nhHCAL"       ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* ne           = new TH2D("ne"           ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);
  TH2D* eCalo        = new TH2D("eCalo"        ,"",nPhi,-TMath::Pi(),TMath::Pi(),nEta,-5.2,5.2);

  const double bins_chhEff[41] = {0.1, 0.122, 0.149, 0.182, 0.223, 0.272, 0.332, 0.406, 0.496, 
                                  0.605, 0.740, 0.904, 1.104, 1.349, 1.648, 2.013, 2.460, 3.004,
                                  3.670, 4.483, 5.477, 6.690, 8.174, 9.983, 12.198, 14.898,
                                  18.202, 22.232, 27.163, 33.176, 40.536, 49.508, 60.492, 
                                  73.880, 90.272, 110.249, 134.713, 164.522, 201.031, 245.513, 
                                  300.0};

  TProfile* chhEff   = new TProfile("chhEff", "", 40, bins_chhEff);

  //Charged hadron energy fraction from Hannu's dijet studies
  TF1* pchf = new TF1("pchf","[0]*pow(x,2) + [1]*x + [2]", 0, 5000);
  pchf->SetParameters(6.862211e-09, -0.0001268141, 0.6283211);

  //Pion track resolution from our sample:
  TF1* piTrkReso = new TF1("piTrkReso", "0.01336 + 8.548e-5*x", 0, 5000);

  //Relative resolutions from Trackin paper:
  TF1* muTrkReso = new TF1("muTrkReso","4.66042e-07*pow(x,2)+0.00010326*x+0.0080762", 0, 5000); 

  //Full muon system resolution q/p in function of pT -> dp = qoverperror()*P*P
  // DPovP = dp/P
  TF1* muFSReso = new TF1("muFSReso","0.002579707*pow(x,0.4552789)", 0, 5000);

  // 68% parameters
  //TF1* allTrkReso = new TF1("allTrkReso","2.77564e-06*pow(x,2)+0.000124*x+0.010373" , 0, 5000);

  // 90% parametes
  TF1* allTrkReso = new TF1("allTrkReso","2.006093e-05*pow(x,2)+0.00103068*x+0.0190268",0,5000);

  
//***********************************************************************************************
  //string weight_file = "./weights.txt";
  //weight_stream.open(weight_file, std::ios::out); //Write mode

  //Loop Tree entries = events
  for (Long64_t jentry=0; jentry != nentries; ++jentry) {
    //Print progress for long runs
    if ((GetprintProg() && jentry % 1000==0)){
      cout << "Looping event " << jentry; 
      if (studyMode == 1) cout << " in dijet" << endl;
      if (studyMode == 3) cout << " in Z+jet" << endl;
    }

    // Bad entry for P8 dijet 5M sample
    //if (jentry == 468473) continue;

    //Skip events that didn't pass cuts earlier. Useful in e.g. repeating Loop
    if (GetuseEarlierCuts() && passedCuts.size()>jentry && !passedCuts[jentry]) continue;

    Long64_t ientry = LoadTree(jentry);	//Load new event
    if (ientry < 0) break;		//When no more events
    nb = fChain->GetEntry(jentry); nbytes += nb;

    //Reinit
    i_tag = 0; i_probe = 0; i_jet2 = 1; i_jet3 = 2;
    p4.SetPtEtaPhiE(0,0,0,0);      p4_calo.SetPtEtaPhiE(0,0,0,0); 
    MET_r.SetPtEtaPhiE(0,0,0,0);   sumEt = 0.0;
    MET_g.SetPtEtaPhiE(0,0,0,0);

    tag_g.SetPtEtaPhiE(0,0,0,0);   tag_r.SetPtEtaPhiE(0,0,0,0);
    
    probe_g.SetPtEtaPhiE(0,0,0,0);
    probe_r.SetPtEtaPhiE(0,0,0,0); 
    probe_calo.SetPtEtaPhiE(0,0,0,0);

    jet2_g.SetPtEtaPhiE(0,0,0,0);
    jet2_r.SetPtEtaPhiE(0,0,0,0);
    jet3_g.SetPtEtaPhiE(0,0,0,0);
    jet3_r.SetPtEtaPhiE(0,0,0,0);

    probe_ch    = 0.0; jet2_ch    = 0.0;
    probe_nh    = 0.0; jet2_nh    = 0.0;
    probe_gamma = 0.0; jet2_gamma = 0.0;
    probe_e     = 0.0; jet2_e     = 0.0;
    probe_mu    = 0.0; jet2_mu    = 0.0;

    probe_genNch    = 0; probe_recoNch    = 0;
    probe_genNnh    = 0; probe_recoNnh    = 0;
    probe_genNgamma = 0; probe_recoNgamma = 0;
    probe_genNe     = 0; probe_recoNe     = 0;
    probe_genNmu    = 0; probe_recoNmu    = 0;
    jet2_genNch     = 0; jet2_recoNch     = 0;
    jet2_genNnh     = 0; jet2_recoNnh     = 0;
    jet2_genNgamma  = 0; jet2_recoNgamma  = 0;
    jet2_genNe      = 0; jet2_recoNe      = 0;
    jet2_genNmu     = 0; jet2_recoNmu     = 0;

    jets_g.clear();
    njets = (unsigned long)jet_pt->size();
    jets_g.resize(njets);
    for (int i=0; i!=jets_g.size(); ++i) { //All objects have njets size
      jets_g[i].SetPtEtaPhiE(0,0,0,0);
    }

    //**************** Z+JET: FIND AND RECONSTRUCT TAG MUONS ****************//
    if (studyMode == 3) {
      int muPDG=13; int muTAG=3; //mu PDGID and parton tag
      if (ReadName.find("H7")!=string::npos) muTAG=2; 

      i_tag1 = -137;	       // These values won't change if...
      i_tag2 = -731;	       // ...muons not found.

      for (int a=0; a!= prtn_tag->size(); ++a) {

        // Find muons with tag == 3 and store those in i_tag1 and i_tag2
        if ((*prtn_tag)[a] == muTAG && abs((*prtn_pdgid)[a]) == muPDG) {
          if      (i_tag1 == -137) i_tag1 = a;
          else if (i_tag2 == -731) i_tag2 = a;
          else {cout << "More than two muons with tag 3 in the event." << endl; continue;}
        }
      }

      if (i_tag1 == -137 && i_tag2 == -731) continue; //No two muons found

      //***** 1st muon *****
      p4.SetPtEtaPhiE((*prtn_pt )[i_tag1], (*prtn_eta)[i_tag1],
                      (*prtn_phi)[i_tag1], (*prtn_e  )[i_tag1]);

      tag_g = p4;//gen lvl

      p4 *= gRandom->Gaus(1,muFSReso->Eval(p4.Pt()));

      if (fabs(p4.Eta()) > eta_muon || p4.Pt() < pTmin_muon) continue;

      tag_r = p4;

      //***** 2nd muon *****
      p4.SetPtEtaPhiE((*prtn_pt )[i_tag2], (*prtn_eta)[i_tag2],
                      (*prtn_phi)[i_tag2], (*prtn_e  )[i_tag2]);

      tag_g += p4;//gen lvl

      p4 *= gRandom->Gaus(1,muFSReso->Eval(p4.Pt()));

      if (fabs(p4.Eta()) > eta_muon || p4.Pt() < pTmin_muon) continue; mumuCut++;

      tag_r += p4;      

      //Invariant mass in range 70-110 GeV since Z boson mass is 91 GeV
      if (tag_g.M()<70.0 || tag_g.M()>110.0) continue; invM ++;

      //Tag eta and pT cuts:
      if (fabs(tag_r.Eta()) > eta_tag_z || tag_r.Pt() < pTmin_tag_z) continue; tagCut++;

      /********************* SELECT THE PROBE JET AND 2nd JET ********************/
      i_mujet1 = -1; i_mujet2 = -1;

      // If muon is found from the jet it is not acceptable
      // Confirm implementation
      for (int j=0; j != prtcl_pt->size(); ++j) { // Loop over all particles
        if ((*prtn_pt )[i_tag1]==(*prtcl_pt)[j]) {
          i_mujet1 = int((*prtcl_jet)[j]);
        }
        if ((*prtn_pt )[i_tag2]==(*prtcl_pt)[j]) {
          i_mujet2 = int((*prtcl_jet)[j]);
        }
      }

      for (int i=0; i < njets; ++i) {
        if (i_probe == i_mujet1 || i_probe == i_mujet2) {
          i_probe ++; i_jet2++; i_jet3 ++;
        } else if (i_jet2 == i_mujet1 || i_jet2 == i_mujet2) {
          i_jet2 ++; i_jet3 ++;
        } else if (i_jet3 == i_mujet1 || i_jet3 == i_mujet2) {
          i_jet3 ++;
        }
      }

    } else if (studyMode == 1) {
      //Probe
      i_probe = 0;
      //2nd jet
      i_jet2  = 1;
      //3rd jet
      i_jet3  = 2;
    }


    /******** RECONSTRUCT GEN-LEVEL JETS AND ADD LEPTONS TO THE PROBE *********/
    for (int i=0; i != prtcl_pt->size(); ++i) {

      JI = (*prtcl_jet)[i]; 
      PDG = abs((*prtcl_pdgid)[i]);
      p4.SetPtEtaPhiE((*prtcl_pt )[i], (*prtcl_eta)[i],	(*prtcl_phi)[i], (*prtcl_e  )[i]);

      Response(PDG, p4.Eta(), p4.E(), p4.Pt(), Rt, Bfield, HHeFrac, 
               fr_h, resp, respA, respEHE, respHHe);
 
      //Reconstruct gen-level jets, at the moment has no neutrinos
      jets_g[JI] += (isNeutrino(PDG) ? 0:1)*p4; //Gen lvl


      if ((*prtcl_jet)[i]==i_probe) { //Probe number of particles
        if (isChHadron(PDG)) {    
          probe_genNch ++;
          if (resp!=0.0) probe_recoNch ++;
        } else if (PDG==11) {
          probe_genNe ++;
          if (resp!=0.0) probe_recoNe ++;
        } else if (PDG==13) {
          probe_genNmu ++;
          if (resp!=0.0) probe_recoNmu ++;
        } else if (PDG==20 || PDG==22) {
          probe_genNgamma ++;
          if (resp!=0.0) probe_recoNgamma ++;
        } else if (PDG==130 ||PDG==310 || PDG==3122 || PDG==2112 || PDG==3212 || PDG==3322) {
          probe_genNnh    ++;
          if (resp!=0.0) probe_recoNnh ++;
        }
      } else if ((*prtcl_jet)[i] == i_jet2) { //2nd jet number of particles
        if (isChHadron(PDG)) {    
          jet2_genNch ++;
          if (resp!=0.0) jet2_recoNch ++;
        } else if (PDG==11) {
          jet2_genNe ++;
          if (resp!=0.0) jet2_recoNe ++;
        } else if (PDG==13) {
          jet2_genNmu ++;
          if (resp!=0.0) jet2_recoNmu ++;
        } else if (PDG==20 || PDG==22) {
          jet2_genNgamma ++;
          if (resp!=0.0) jet2_recoNgamma ++;
        } else if (PDG==130 ||PDG==310 || PDG==3122 || PDG==2112 || PDG==3212 || PDG==3322) {
          jet2_genNnh    ++;
          if (resp!=0.0) jet2_recoNnh ++;
        }
      }

      //Electrons to the reconstructed jets
      if (PDG==11) { 
        if ((*prtcl_jet)[i]==i_probe) { //Probe
          probe_r += resp*p4;
          probe_e += resp*p4.E();
        } else if ((*prtcl_jet)[i] == i_jet2) { //2nd jet
          jet2_r += resp*p4;
          jet2_e += resp*p4.E();
        } else if ((*prtcl_jet)[i] == i_jet3) { //3rd jet
          jet3_r += resp*p4;
        }
      } else if (PDG==13) { //Muons to the reconstructed jets
        if ((*prtcl_jet)[i]==i_probe) { //Probe
          probe_r  += resp*p4;
          probe_mu += resp*p4.E();
        } else if ((*prtcl_jet)[i] == i_jet2) { //2nd jet
          jet2_r  += resp*p4;
          jet2_mu += resp*p4.E();
        } else if ((*prtcl_jet)[i] == i_jet3) { //3rd jet
          jet3_r  += resp*p4;
        }
      }
    } //Loop particles in jets


    /********************* RECONSTRUCT GEN LEVEL PROBE, jet2, jet3 *********************/

    //Gen lvl as output by FastJet
    if (njets > 0 && njets > i_probe) {
      probe_g.SetPtEtaPhiE((*jet_pt)[i_probe], (*jet_eta)[i_probe],
                          (*jet_phi)[i_probe],   (*jet_e)[i_probe] );
    } else {
      probe_g.SetPtEtaPhiE(0,0,0,0);
    }

    if (njets > 1 && njets > i_jet2) {
      jet2_g.SetPtEtaPhiE( (*jet_pt)[i_jet2], (*jet_eta)[i_jet2],
                          (*jet_phi)[i_jet2],   (*jet_e)[i_jet2] );
    } else {
      jet2_g.SetPtEtaPhiE(0,0,0,0);
    }

    if (njets > 2 && njets > i_jet3) {
      jet3_g.SetPtEtaPhiE( (*jet_pt)[i_jet3], (*jet_eta)[i_jet3],
                          (*jet_phi)[i_jet3],   (*jet_e)[i_jet3] );
    } else {
      jet3_g.SetPtEtaPhiE(0,0,0,0);
    }

    /****************************** COMMON CUTS FOR Z+JET ******************************/
    if (studyMode == 3) {
      //PF probe-tag cuts
      if (fabs(probe_g.Eta())>eta_probe || probe_g.Pt()<pTmin_probe_g) continue; genprobeCut ++;

      //Back-to-back cut
      if (fabs(tag_g.DeltaPhi(probe_g)) < phiMin) continue; b2b ++;
    } 
    /****************************** COMMON CUTS FOR DIJET ******************************/
    if (studyMode == 1) {
      //probe and 2nd jet cuts, Both must have pT higher than 10 GeV
      if (probe_g.Pt()<pTmin_probe_g || jet2_g.Pt()<pTmin_probe_g) continue;

      //Both must be in the barrel
      if (fabs(probe_g.Eta())>eta_probe || fabs(jet2_g.Eta())>eta_probe) continue;
      genprobeCut ++;

      //Back-to-back cut
      if (fabs(probe_g.DeltaPhi(jet2_g)) < phiMin) continue; b2b ++;
    }
    /********************** PROBE GEN PARTICLE CONTENT **********************/
    
    if (GetcontHistos()) {
      for (unsigned int i=0; i!=prtcl_pdgid->size(); ++i) {

        JI  = (*prtcl_jet)[i];
        PDG = abs((*prtcl_pdgid)[i]); 

        //See if ptcl belongs to the probe jet
        if (JI==i_probe) {
          switch (PDG) {
            case 11   : h_e    ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 22   : h_gamma->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 20   : h_gpi0 ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 321  : h_K    ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 310  : h_K0S  ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 130  : h_K0L  ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 3122 : h_L    ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 13   : h_mu   ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 2112 : h_n    ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 211  : h_pi   ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            case 2212 : h_p    ->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
            default :
              if (isNeutrino(PDG)) {
                h_nu->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
              }
              else if (isStrangeB(PDG)) {
                switch (PDG) {
                  case 3112 : h_Sigm->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
                  case 3212 : h_Sig0->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
                  case 3222 : h_Sigp->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
                  case 3312 : h_Xim->Fill( (*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
                  case 3322 : h_Xi0->Fill( (*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
                  case 3334 : h_Om->Fill(  (*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]); break;
                  default : cout<<"Error: isStrangeB passed ID "<<PDG<<endl;
                }
                //Fill the general "other particles" histogram already here
                h_other->Fill((*jet_e)[(*prtcl_jet)[i_probe]],(*prtcl_e)[i]);
              } //Xi, Sigma, Omega^-
              //List further unknown particle PDGIDs
              if (find(otherIDs.begin(),otherIDs.end(),PDG)==otherIDs.end()&& 
                  !isNeutrino(PDG)  ) otherIDs.push_back(PDG);
          } //Switch PDG (general)
          h_all->Fill((*jet_e)[(*prtcl_jet)[i_probe]], (*prtcl_e)[i]);
        }
      }
    }
    


    /******************* RECONSTRUCT PARTICLES NOT IN JETS *******************/
    //Reset histograms
    cht          ->Reset();	//Original directions of
    chtPt        ->Reset();	//Charged hadrons
    chtP_curv    ->Reset();	//Linking track momentum to the cell
    chcECAL      ->Reset();	//For calo-trk weighting
    chcHCAL      ->Reset();	//For calo-trk weighting
    chECAL_curv  ->Reset();	//Calo jet calculation
    chHCAL_curv  ->Reset();	//       -||-
    nhECAL       ->Reset();	//Neutral hadrons depositing in ECAL (also ch when trk fails)
    nhHCAL       ->Reset();	//           -||-               HCAL         -||-
    ne           ->Reset();	//Photon cells
    eCalo        ->Reset();	//Calo jet calculation
    sigmaTrk     ->Reset();	//For weighting
    sigmaTrk_curv->Reset();	//For shadowing effect


    //Probe flavor check, prtn_tag=0 for outgoing hard process partons
    //for (unsigned int j = 0; j != prtn_tag->size(); ++j) {
    //  if ((*prtn_jet)[j]==i_probe && (*prtn_tag)[j]==0) {
    //    probeFlav = abs((*prtn_pdgid)[j]);
    //    continue;
    //  }
    //}
    //if(probeFlav != 5) continue; //Uncomment to study only specific jets

    //****************** MET calculation *******************//
    //Add tag object to the MET

    if (studyMode==3) {
      MET_r -= tag_r;
      sumEt += tag_r.Et();
    }

    //Loop over all particles
    for (int i=0; i!=prtclnij_pt->size(); ++i) {
      
      PDG = fabs((*prtclnij_pdgid)[i]);
      p4.SetPtEtaPhiE((*prtclnij_pt )[i],(*prtclnij_eta)[i],
                      (*prtclnij_phi)[i],(*prtclnij_e  )[i]);


      if (!isNeutrino(PDG)) MET_g -= p4; //Gen level MET over all particles


      Response((*prtclnij_pdgid)[i], p4.Eta(), p4.E(), p4.Pt(), Rt, Bfield, HHeFrac, 
               fr_h, resp, respA, respEHE, respHHe);

      if (resp == 0.0) continue;

      p4 *= resp;

      switch (PDG) {
        //PHOTON
        case 20 : case 22 : 
          ne->Fill(p4.Phi(), p4.Eta(), p4.E());
          break;

        //LEPTONS
        case 13 : case 11 :
          if (studyMode == 3) {
            // Not one of the tag muons
            if ((*prtclnij_pt)[i]!=(*prtn_pt)[i_tag1]&&(*prtclnij_pt )[i]!=(*prtn_pt)[i_tag2]){ 
              MET_r -= p4; //Muons and electrons to the MET
              sumEt += p4.Et();
            }
          } else if (studyMode == 1) {
            MET_r -= p4;
            sumEt += p4.Et();
          }

          //Electron ECAL deposit taking into account track curvature with response = 1
          if (PDG == 11) {
            newPhi = trackDeltaPhi((*prtclnij_pdgid)[i], (*prtclnij_phi)[i], 
                                   (*prtclnij_pt )[i], Rt, Bfield);

            p4.SetPtEtaPhiE((*prtclnij_pt )[i], (*prtclnij_eta)[i], newPhi, (*prtclnij_e)[i]);
            eCalo->Fill(p4.Phi(), p4.Eta(), p4.E()); //Fill electron calo grid
          }

          break;

        //CHARGED HADRONS
        case 211 : case 321 : case 2212 : case 3112 : case 3222 : case 3312 : case 3334 :
          trkFail = 0;
          eff = 1.0; //Initially no track failing

          //Efficiency from chs
          if (fabs(p4.Eta() < 2.5)) {
            for (int ijet=0; ijet!=njets; ++ijet) {
              if (p4.DeltaR(jets_g[ijet]) < RCone && ijet != i_mujet1 && ijet != i_mujet2) {
                eff = pchf->Eval(jets_g[ijet].Pt()) / 0.607;
                eff -= 0.1381969 - 0.1606539/pow(2,((*prtclnij_pt)[i]/45.31742));
              }
            }
          }

          eff = min(eff,0.96);

          if ((*prtclnij_pt)[i] < 0.6) {
            eff = 2021.944 - 2022.2565603/(1 + pow(((*prtclnij_pt)[i]/36533650), 0.4126626));
          } else if ((*prtclnij_pt)[i] > 0.6 && (*prtclnij_pt)[i] < 10.0) {
            eff -= 0.03836*pow((*prtclnij_pt)[i],-0.70570) - 0.003;
          }

          eff = max(eff,0.0);

          if (varTrkEffm1) eff *= 0.99; //-1% variation to track efficiency
          if (varTrkEffm3) eff *= 0.97; //-3% variation to track efficiency

          //Tracking efficiency plot
          if (fabs((*prtclnij_eta)[i]) < 2.5) {
            chhEff->Fill((*prtclnij_pt)[i], eff);
          }

          if (resp != 1) eff = 0.0; //If not in tracker

          if ( ((double)rand() / (double)RAND_MAX) > eff ) trkFail = 1;

          //Track resolutions
          trkReso = max(0.0, allTrkReso->Eval((*prtclnij_pt)[i]) * p4.P());

          //Phi where particle exits the tracker / enters ECAL
          newPhi = trackDeltaPhi((*prtclnij_pdgid)[i], (*prtclnij_phi)[i], 
                                 (*prtclnij_pt )[i], Rt, Bfield);

          if (!trkFail) { //resp == 1.0
            cht  ->Fill(p4.Phi(), p4.Eta(), p4.E() );
            chtPt->Fill(p4.Phi(), p4.Eta(), p4.Pt());

            if (doesReachEcal((*prtclnij_pdgid)[i], (*prtclnij_pt )[i], Bfield, Rt)) {
              chtP_curv->Fill(newPhi, p4.Eta(), p4.P());
              sigmaTrk->Fill(p4.Phi(), p4.Eta(), pow(trkReso,2));
              sigmaTrk_curv->Fill(newPhi, p4.Eta(), pow(trkReso,2));
            } 
          }
          
          //Re-initialize
          p4.SetPtEtaPhiE((*prtclnij_pt )[i], (*prtclnij_eta)[i], newPhi, (*prtclnij_e)[i]);

          //Calorimeter deposit for the weighting
          if (!trkFail) {
            //chc->Fill((*prtclnij_phi)[i], p4.Eta(), respA*p4.E());
            chcECAL->Fill((*prtclnij_phi)[i], p4.Eta(),
                           0.5*(1-HHeFrac)*respEHE*p4.E());
            chcHCAL->Fill((*prtclnij_phi)[i], p4.Eta(), 
                          (0.5*(1-HHeFrac)*respEHE + HHeFrac*respHHe)*p4.E());
          }

          //ECAL and HCAL deposits
          if ( ((double)rand()/(double)RAND_MAX) > HHeFrac ) { //EHE path
            p4 *= respEHE;

            if (trkFail) {
              nhECAL->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E());
              nhHCAL->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E());
            } else {
              chECAL_curv->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E());
              chHCAL_curv->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E());
            }
          } else { //HHe path 
            p4 *= respHHe;

            if (trkFail) {
              nhHCAL->Fill(p4.Phi(), p4.Eta(), p4.E());
            } else {
              chHCAL_curv->Fill(p4.Phi(), p4.Eta(), p4.E());
            }
          }
          break;

        //NEUTRAL HADRONS
        case 130 : case 310 : case 3122 : case 2112 : case 3212 : case 3322 :
          p4.SetPtEtaPhiE((*prtclnij_pt )[i],(*prtclnij_eta)[i],
                          (*prtclnij_phi)[i],(*prtclnij_e  )[i]);

          if ( ((double)rand()/(double)RAND_MAX) > HHeFrac ) { //EHE path
            p4 *= respEHE;
            nhECAL->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E()); //ECAL
            nhHCAL->Fill(p4.Phi(), p4.Eta(), 0.5*p4.E()); //HCAL
          } else { //HHe path 
            p4 *= respHHe;
            nhHCAL->Fill(p4.Phi(), p4.Eta(), p4.E()); //HCAL
          }
          break;

        default : 
          if (fabs(PDG) != 12 && fabs(PDG) != 14 && fabs(PDG) != 16) {
            cout << "PDGID " << PDG << endl;
          }
      }
    } //Particle loop

    // ***************** Loop over cells ******************
    for (int i=1; i!=cht->GetNbinsX()+1; ++i) {
      for (int j=1; j!=cht->GetNbinsY()+1; ++j) {

        
        //Skip if no energy deposit in the cell
        if (cht->GetBinContent(i,j)         == 0.0 && chtPt->GetBinContent(i,j)       == 0.0 &&
            chtP_curv->GetBinContent(i,j)   == 0.0 && 
            chcECAL->GetBinContent(i,j)     == 0.0 && chcHCAL->GetBinContent(i,j)     == 0.0 &&
            chECAL_curv->GetBinContent(i,j) == 0.0 && chHCAL_curv->GetBinContent(i,j) == 0.0 &&
            nhECAL->GetBinContent(i,j)      == 0.0 && nhHCAL->GetBinContent(i,j)      == 0.0 &&
            ne->GetBinContent(i,j)          == 0.0 && eCalo->GetBinContent(i,j)       == 0.0)
            continue;
        

        double w = 1.0; int iEtaCold; double totalChargedMomentum; double chc = 0.0;
        double nhHCAL_calib = 0.0;    double chc_calib = 0.0;      double caloResolution = 0.0;
        p4_chc.SetPtEtaPhiE(0,0,0,0); p4_cht.SetPtEtaPhiE(0,0,0,0);
        p4.SetPtEtaPhiE(0,0,0,0);     p4_rescale.SetPtEtaPhiE(0,0,0,0);
        delta = 0.0; cellSigma = 0.0;        

        cellPhi = cht->GetXaxis()->GetBinCenter(i);
        cellEta = cht->GetYaxis()->GetBinCenter(j);
        
        
        if (varCp3) {
          nhECAL     ->SetBinContent(i,j, 1.03*nhECAL->GetBinContent(i,j));
          nhHCAL     ->SetBinContent(i,j, 1.03*nhHCAL->GetBinContent(i,j));
          chcECAL    ->SetBinContent(i,j, 1.03*chcECAL->GetBinContent(i,j));
          chcHCAL    ->SetBinContent(i,j, 1.03*chcHCAL->GetBinContent(i,j));
          chECAL_curv->SetBinContent(i,j, 1.03*chECAL_curv->GetBinContent(i,j));
          chHCAL_curv->SetBinContent(i,j, 1.03*chHCAL_curv->GetBinContent(i,j));
        } else if (varCm3) {
          nhECAL     ->SetBinContent(i,j, 0.97*nhECAL->GetBinContent(i,j));
          nhHCAL     ->SetBinContent(i,j, 0.97*nhHCAL->GetBinContent(i,j));
          chcECAL    ->SetBinContent(i,j, 0.97*chcECAL->GetBinContent(i,j));
          chcHCAL    ->SetBinContent(i,j, 0.97*chcHCAL->GetBinContent(i,j));
          chECAL_curv->SetBinContent(i,j, 0.97*chECAL_curv->GetBinContent(i,j));
          chHCAL_curv->SetBinContent(i,j, 0.97*chHCAL_curv->GetBinContent(i,j));
        } else if (varHadHCALp3) {
          nhHCAL     ->SetBinContent(i,j, 1.03*nhHCAL->GetBinContent(i,j));
          chcHCAL    ->SetBinContent(i,j, 1.03*chcHCAL->GetBinContent(i,j));
          chHCAL_curv->SetBinContent(i,j, 1.03*chHCAL_curv->GetBinContent(i,j));
        } else if (varHadHCALm3) {
          nhHCAL     ->SetBinContent(i,j, 0.97*nhHCAL->GetBinContent(i,j));
          chcHCAL    ->SetBinContent(i,j, 0.97*chcHCAL->GetBinContent(i,j));
          chHCAL_curv->SetBinContent(i,j, 0.97*chHCAL_curv->GetBinContent(i,j));
        } else if (varHadECALp3) {
          nhECAL     ->SetBinContent(i,j, 1.03*nhECAL->GetBinContent(i,j));
          chcECAL    ->SetBinContent(i,j, 1.03*chcECAL->GetBinContent(i,j));
          chECAL_curv->SetBinContent(i,j, 1.03*chECAL_curv->GetBinContent(i,j));
        } else if (varHadECALm3) {
          nhECAL     ->SetBinContent(i,j, 0.97*nhECAL->GetBinContent(i,j));
          chcECAL    ->SetBinContent(i,j, 0.97*chcECAL->GetBinContent(i,j));
          chECAL_curv->SetBinContent(i,j, 0.97*chECAL_curv->GetBinContent(i,j));
        } else if (varECALm1) { //ECAL scale variation -1% for photons and hadrons
          nhECAL     ->SetBinContent(i,j, 0.99*nhECAL->GetBinContent(i,j));
          chcECAL    ->SetBinContent(i,j, 0.99*chcECAL->GetBinContent(i,j));
          chECAL_curv->SetBinContent(i,j, 0.99*chECAL_curv->GetBinContent(i,j));
          ne         ->SetBinContent(i,j, 0.99*ne->GetBinContent(i,j));
        } else if (varECALm3) { //ECAL scale variation -3% for photons and hadrons
          nhECAL     ->SetBinContent(i,j, 0.97*nhECAL->GetBinContent(i,j));
          chcECAL    ->SetBinContent(i,j, 0.97*chcECAL->GetBinContent(i,j));
          chECAL_curv->SetBinContent(i,j, 0.97*chECAL_curv->GetBinContent(i,j));
          ne         ->SetBinContent(i,j, 0.97*ne->GetBinContent(i,j));
        } else if (varPhotonm1) {
          ne         ->SetBinContent(i,j, 0.99*ne->GetBinContent(i,j)); // Photon scale -1%
        } else if (varPhotonm3) {
          ne         ->SetBinContent(i,j, 0.97*ne->GetBinContent(i,j)); // Photon scale -3%
        }

        //**** Cold cells
        iEtaCold = floor((cellEta+fabs(coldCells->GetXaxis()->GetBinLowEdge(1)))/
                          coldCells->GetXaxis()->GetBinWidth(1) + 1);

        //In phi-direction, one cell covers two in the cold cells histogram.
        //If either one of those is cold, 150 GeV cut is applied to the ECAL.
	//Effect nevertheless minor, might remove it in the future?
        if (coldCells->GetBinContent(iEtaCold,i*2  ) > 1.0 || 
            coldCells->GetBinContent(iEtaCold,i*2-1) > 1.0) {
          
          if (nhECAL->GetBinContent(i,j) + ne->GetBinContent(i,j) > 150.0) {
            nhECAL->SetBinContent(i,j,0);
            ne->SetBinContent(i,j,150);
          }
        }
        //****

        //Calo cell four vector
        cellE_calo = ne->GetBinContent(i,j)          + eCalo->GetBinContent(i,j)
                   + nhECAL->GetBinContent(i,j)      + nhHCAL->GetBinContent(i,j) 
                   + chECAL_curv->GetBinContent(i,j) + chHCAL_curv->GetBinContent(i,j);
        p4_calo.SetPtEtaPhiE(cellE_calo/cosh(cellEta), cellEta, cellPhi, cellE_calo);

        if (p4_calo.DeltaR(probe_g) < RCone) {
          probe_calo += p4_calo;
        }


        //HCAL calibration
        if (nhHCAL->GetBinContent(i,j) > 0.0) {
          if (nhHCAL->GetBinContent(i,j) < 7798.2) {
            nhHCAL_calib = fr_hcal->GetX(nhHCAL->GetBinContent(i,j), 0.1, 7000.0);
          } else {
            nhHCAL_calib = 7000.0;
          }
        }


 
        //***** PF-code calorimeter resolution *****
        totalChargedMomentum = chtP_curv->GetBinContent(i,j);

        caloResolution = fabs(cellEta) < 1.48 
                         ? sqrt(1.02*1.02/max(totalChargedMomentum,1.0) + 0.065*0.065)
                         : sqrt(1.20*1.20/max(totalChargedMomentum,1.0) + 0.028*0.028);

        caloResolution *= totalChargedMomentum;


        //TotalError
        cellSigma  = sqrt(pow(caloResolution,2) + sigmaTrk_curv->GetBinContent(i,j));
        cellSigma *= (1.0 + exp(-totalChargedMomentum/100.0));
        //******************************************


        //Calibrated calorimetric energy for neutral hadrons and photons
        delta = nhECAL->GetBinContent(i,j) + ne->GetBinContent(i,j) + nhHCAL_calib;

        //Calibrated calorimetric energy deposit from charged hadrons

        chc = chcECAL->GetBinContent(i,j) + chcHCAL->GetBinContent(i,j);

        if (chc > 0.0) {
          if (chc < 7142.73) {
            chc_calib = fr_all->GetX(chc, 0.1, 7000.0);
          } else {
            chc_calib = 7000.0;
          }
        }


        if (delta < cellSigma) { //Shadowing
          nhHCAL_calib = 0.0;
          nhECAL->SetBinContent(i,j,0.0); 
          ne    ->SetBinContent(i,j,0.0);
          chc_calib += delta; // Move shadowed energy to ch energy deposit
        }

        //Calculate caloResolution for not curved tracks
        p4.SetPtEtaPhiE(chtPt->GetBinContent(i,j), cellEta, cellPhi, 
                        cht->GetBinContent(i,j));
        
        caloResolution = fabs(cellEta) < 1.48 
                         ? sqrt(1.02*1.02/max(p4.P(),1.0) + 0.065*0.065)
                         : sqrt(1.20*1.20/max(p4.P(),1.0) + 0.028*0.028);
        caloResolution *= p4.P();
        
        //****************************** WEIGHTING ********************************
        w = pow(caloResolution,2) / 
            (sigmaTrk->GetBinContent(i,j) + pow(caloResolution,2));

        if (sqrt(sigmaTrk->GetBinContent(i,j))/p4.P() > 0.1) {
          p4_chc.SetPtEtaPhiE(chc_calib/cosh(cellEta), cellEta, cellPhi, chc_calib);
          p4_cht.SetPtEtaPhiE(chtPt->GetBinContent(i,j), cellEta, cellPhi, 
                              cht->GetBinContent(i,j));

          p4_rescale = w*p4_cht + (1-w)*p4_chc;

        } else { //Normal case
          p4_rescale.SetPtEtaPhiE(chtPt->GetBinContent(i,j), cellEta, cellPhi, 
                                  cht->GetBinContent(i,j));
        }

        //Cell 4-vector
        cellE = ne->GetBinContent(i,j) + nhECAL->GetBinContent(i,j) + nhHCAL_calib;
        cellPt = cellE/cosh(cellEta);
        p4.SetPtEtaPhiE(cellPt, cellEta, cellPhi, cellE); //Calibrated calorimeter
        p4 += p4_rescale;                                 //Charged hadron deposit

        MET_r -= p4;
        sumEt += p4.Et();

        //Jet Reconstruction
        if (p4.DeltaR(probe_g) < RCone) {
          probe_r     += p4;
          probe_ch    += p4_rescale.E();
          probe_nh    += nhHCAL_calib;
          probe_gamma += nhECAL->GetBinContent(i,j) + ne->GetBinContent(i,j);
        }
        //Second leading jet reconstruction for Alpha cut
        if (p4.DeltaR(jet2_g) < RCone && jet2_g.Pt() != 0.0) {
          jet2_r     += p4;
          jet2_ch    += p4_rescale.E();
          jet2_nh    += nhHCAL_calib;
          jet2_gamma += nhECAL->GetBinContent(i,j) + ne->GetBinContent(i,j);
        }
        if (p4.DeltaR(jet3_g) < RCone && jet3_g.Pt() != 0.0) {
          jet3_r += p4;
        }
      }
    }

    /**************************** RECO CUTS ****************************/

    if (studyMode == 1) { //Dijet reco cuts
      //alpha cut
      if (jet3_r.Pt() > 0.3*(0.5*(probe_r.Pt() + jet2_r.Pt()))) continue; alpha ++;

      //reco jet pT cuts
      if (probe_r.Pt() < pTmin_probe_r    || jet2_r.Pt() < pTmin_probe_r) continue;
      if (fabs(probe_r.Eta()) > eta_probe || fabs(jet2_r.Eta()) > eta_probe) continue;
      probeCut ++;

      //MET cut
      if (MET_r.Pt() > 45.0 && MET_r.Pt() > 0.4*sumEt) continue; METcut ++;

    } else if (studyMode == 3) { //Z+jet cuts
      //alpha cut
      if (jet2_r.Pt() > 0.3*tag_r.Pt()) continue; alpha ++;

      //reco probe cuts
      if (fabs(probe_r.Eta()) > eta_probe || probe_r.Pt() < pTmin_probe_r) continue; probeCut ++;

      //MET cut
      if (MET_r.Pt() > 45.0 && MET_r.Pt() > 0.4*sumEt) continue; METcut ++;
      
    }

    double totalE, weight_temp, meanWeight, meanWeight_tagr;

    /**************************** FILL HISTOGRAMS ****************************/
    for (unsigned int i = 0; i!=((studyMode==1) ? 2:1); ++i) {

      probeFlav = -1;

      //Dijet cuts
      if (studyMode ==1) {
        if (i==0) {
          tag_r = jet2_r;
          tag_g = jet2_g;
        } else if (i==1) {
          tag_r   = probe_r;
          tag_g   = probe_g;
          probe_r = jet2_r;
          probe_g = jet2_g;
          //JEF
          probe_ch    = jet2_ch;
          probe_nh    = jet2_nh;
          probe_gamma = jet2_gamma;
          probe_e     = jet2_e;
          probe_mu    = jet2_mu;
          i_probe     = i_jet2;

          //N
          probe_genNch    = jet2_genNch;
          probe_genNe     = jet2_genNe;
          probe_genNmu    = jet2_genNmu;
          probe_genNgamma = jet2_genNgamma;
          probe_genNnh    = jet2_genNnh;

          probe_recoNch    = jet2_recoNch;
          probe_recoNe     = jet2_recoNe;
          probe_recoNmu    = jet2_recoNmu;
          probe_recoNgamma = jet2_recoNgamma;
          probe_recoNnh    = jet2_recoNnh;
        }
      }

      //CHECK JET FLAVOUR: FIND FLAVOUR-DEPENDENT QUANTITIES
      //Loop partons to find where jets originated from
      //Probe flavor check, prtn_tag=0 for outgoing hard process partons
      for (unsigned int j = 0; j != prtn_tag->size(); ++j) {
        if ((*prtn_jet)[j]==i_probe && (*prtn_tag)[j]==0) {
          probeFlav = abs((*prtn_pdgid)[j]);
          continue;
        }
      }

      weight_temp = weight;

      for (unsigned int i=1; i < nbinsMPF; ++i) {
        if (probe_g.Pt() >= binsxMPF[nbinsMPF-1]) {
          if (studyMode ==1 && ReadName.find("P8")!=string::npos) {
            meanWeight = meanWeightP8dijet[nbinsMPF-2];
          } else if (studyMode ==3 && ReadName.find("H7")!=string::npos) {
            meanWeight = meanWeightH7Zjet[nbinsMPF-2];
          } else if (studyMode ==1 && ReadName.find("H7")!=string::npos) {
            meanWeight = meanWeightH7dijet[nbinsMPF-2];
          }
          break;
        } else if (probe_g.Pt() < binsxMPF[i]) {
          if (studyMode ==1 && ReadName.find("P8")!=string::npos) {
            meanWeight = meanWeightP8dijet[i-1];
          } else if (studyMode ==3 && ReadName.find("H7")!=string::npos) {
            meanWeight = meanWeightH7Zjet[i-1];
          } else if (studyMode ==1 && ReadName.find("H7")!=string::npos) {
            meanWeight = meanWeightH7dijet[i-1];
          }
          break;
        }
      }
      for (unsigned int i=1; i < nbinsMPF; ++i) {
        if (tag_r.Pt() >= binsxMPF[nbinsMPF-1]) {
          if (studyMode ==1 && ReadName.find("P8")!=string::npos) {
            meanWeight_tagr = meanWeightP8dijet_tagr[nbinsMPF-2];
          } else if (studyMode ==3 && ReadName.find("H7")!=string::npos) {
            meanWeight_tagr = meanWeightH7Zjet_tagr[nbinsMPF-2];
          } else if (studyMode ==1 && ReadName.find("H7")!=string::npos) {
            meanWeight_tagr = meanWeightH7dijet_tagr[nbinsMPF-2];
          }
          break;
        } else if (tag_r.Pt() < binsxMPF[i]) {
          if (studyMode ==1 && ReadName.find("P8")!=string::npos) {
            meanWeight_tagr = meanWeightP8dijet_tagr[i-1];
          } else if (studyMode ==3 && ReadName.find("H7")!=string::npos) {
            meanWeight_tagr = meanWeightH7Zjet_tagr[i-1];
          } else if (studyMode ==1 && ReadName.find("H7")!=string::npos) {
            meanWeight_tagr = meanWeightH7dijet_tagr[i-1];
          }
          break;
        }
      }

      

      //Weight cuts for P8 dijet && H7 Zjet
      if ((studyMode ==1 && ReadName.find("P8")!=string::npos) ||
          (studyMode ==3 && ReadName.find("H7")!=string::npos) ||
          (studyMode ==1 && ReadName.find("H7")!=string::npos)) {

        if (weight_temp > 100*meanWeight) {
          cout << "Probeg: " << probe_g.Pt() << " " << weight_temp << " " << meanWeight << endl;
          continue;
        }
        if (weight_temp > 100*meanWeight_tagr) {
          cout << "Tagr: " << tag_r.Pt() << " " << weight_temp << " " << meanWeight_tagr << endl;
          continue;
        }
      }
      weightCut ++;

      if (varbfracp50 && probeFlav == 5) weight_temp *= 1.5;
      if (varcfracp50 && probeFlav == 4) weight_temp *= 1.5;

      prWeight->Fill(probe_g.Pt(), weight_temp);
      prWeight_tagr->Fill(tag_r.Pt(), weight_temp);

      //weight_stream << probe_g.Pt();
      //weight_stream << " ";
      //weight_stream << tag_r.Pt();
      //weight_stream << " ";
      //weight_stream << weight_temp;
      //weight_stream << "\n";


      evt ++;

      totalE = probe_ch + probe_nh + probe_gamma + probe_e + probe_mu;

      prchf   ->Fill(tag_r.Pt(), probe_ch/totalE,    weight_temp);
      prnhf   ->Fill(tag_r.Pt(), probe_nh/totalE,    weight_temp);
      prgammaf->Fill(tag_r.Pt(), probe_gamma/totalE, weight_temp);
      pref    ->Fill(tag_r.Pt(), probe_e/totalE,     weight_temp);
      prmuf   ->Fill(tag_r.Pt(), probe_mu/totalE,    weight_temp);

      prgenNch    ->Fill(tag_r.Pt(), probe_genNch   , weight_temp);
      prgenNnh    ->Fill(tag_r.Pt(), probe_genNnh   , weight_temp);
      prgenNgamma ->Fill(tag_r.Pt(), probe_genNgamma, weight_temp);
      prgenNe     ->Fill(tag_r.Pt(), probe_genNe    , weight_temp);
      prgenNmu    ->Fill(tag_r.Pt(), probe_genNmu   , weight_temp);

      prrecoNch   ->Fill(tag_r.Pt(), probe_recoNch   , weight_temp);
      prrecoNnh   ->Fill(tag_r.Pt(), probe_recoNnh   , weight_temp);
      prrecoNgamma->Fill(tag_r.Pt(), probe_recoNgamma, weight_temp);
      prrecoNe    ->Fill(tag_r.Pt(), probe_recoNe    , weight_temp);
      prrecoNmu   ->Fill(tag_r.Pt(), probe_recoNmu   , weight_temp);



      //MPF response
      R_MPF_r = 1.0 + (MET_r.Px()*tag_r.Px() + MET_r.Py()*tag_r.Py()) / pow((tag_r.Pt()),2);
      //R_MPF_r = 1.0 + MET_r.Pt()*cos(tag_r.DeltaPhi(MET_r))/tag_r.Pt();

      R_MPF_g = 1.0 + (MET_g.Px()*tag_g.Px() + MET_g.Py()*tag_g.Py()) / pow((tag_g.Pt()),2);

      //Fill MPF profile
      prMPF->Fill(tag_r.Pt(), R_MPF_r, weight_temp);

      prgenMPF->Fill(tag_g.Pt(), R_MPF_g, weight_temp);

      Rjet      = probe_r.Pt()/probe_g.Pt();
      Rjet_calo = probe_calo.Pt()/probe_g.Pt(); 


      //All jets
      prRjet->Fill(probe_g.Pt(), Rjet, weight_temp);
      prRjetvstag->Fill(tag_r.Pt(), Rjet, weight_temp);

      prRjet_calo->Fill(probe_g.Pt(), Rjet_calo, weight_temp);


      //Flavour dependent quantities
      if (probeFlav == 5) {                           //b-jets
        FFb->Fill(tag_r.Pt(), weight_temp);
        prMPFb->Fill(tag_r.Pt(), R_MPF_r, weight_temp);
        prgenMPFb->Fill(tag_g.Pt(), R_MPF_g, weight_temp);
        prRjetb->Fill(probe_g.Pt(), Rjet, weight_temp);
        prRjetvstagb->Fill(tag_r.Pt(), Rjet, weight_temp);

        prchfb   ->Fill(tag_r.Pt(), probe_ch/totalE,    weight_temp);
        prnhfb   ->Fill(tag_r.Pt(), probe_nh/totalE,    weight_temp);
        prgammafb->Fill(tag_r.Pt(), probe_gamma/totalE, weight_temp);
        prefb    ->Fill(tag_r.Pt(), probe_e/totalE,     weight_temp);
        prmufb   ->Fill(tag_r.Pt(), probe_mu/totalE,    weight_temp);

        prgenNchb    ->Fill(tag_r.Pt(), probe_genNch   , weight_temp);
        prgenNnhb    ->Fill(tag_r.Pt(), probe_genNnh   , weight_temp);
        prgenNgammab ->Fill(tag_r.Pt(), probe_genNgamma, weight_temp);
        prgenNeb     ->Fill(tag_r.Pt(), probe_genNe    , weight_temp);
        prgenNmub    ->Fill(tag_r.Pt(), probe_genNmu   , weight_temp);

        prrecoNchb   ->Fill(tag_r.Pt(), probe_recoNch   , weight_temp);
        prrecoNnhb   ->Fill(tag_r.Pt(), probe_recoNnh   , weight_temp);
        prrecoNgammab->Fill(tag_r.Pt(), probe_recoNgamma, weight_temp);
        prrecoNeb    ->Fill(tag_r.Pt(), probe_recoNe    , weight_temp);
        prrecoNmub   ->Fill(tag_r.Pt(), probe_recoNmu   , weight_temp);

      } else if (probeFlav > 0 && probeFlav < 5) {    //Light quark (u,d,s,c) jets (1,2,3,4)
        FFlq->Fill(tag_r.Pt(), weight_temp);
        prMPFlq->Fill(tag_r.Pt(), R_MPF_r, weight_temp);
        prgenMPFlq->Fill(tag_g.Pt(), R_MPF_g, weight_temp);
        prRjetlq->Fill(probe_g.Pt(), Rjet, weight_temp);
        prRjetvstaglq->Fill(tag_r.Pt(), Rjet, weight_temp);

        prchflq   ->Fill(tag_r.Pt(), probe_ch/totalE,    weight_temp);
        prnhflq   ->Fill(tag_r.Pt(), probe_nh/totalE,    weight_temp);
        prgammaflq->Fill(tag_r.Pt(), probe_gamma/totalE, weight_temp);
        preflq    ->Fill(tag_r.Pt(), probe_e/totalE,     weight_temp);
        prmuflq   ->Fill(tag_r.Pt(), probe_mu/totalE,    weight_temp);

        prgenNchlq    ->Fill(tag_r.Pt(), probe_genNch   , weight_temp);
        prgenNnhlq    ->Fill(tag_r.Pt(), probe_genNnh   , weight_temp);
        prgenNgammalq ->Fill(tag_r.Pt(), probe_genNgamma, weight_temp);
        prgenNelq     ->Fill(tag_r.Pt(), probe_genNe    , weight_temp);
        prgenNmulq    ->Fill(tag_r.Pt(), probe_genNmu   , weight_temp);

        prrecoNchlq   ->Fill(tag_r.Pt(), probe_recoNch   , weight_temp);
        prrecoNnhlq   ->Fill(tag_r.Pt(), probe_recoNnh   , weight_temp);
        prrecoNgammalq->Fill(tag_r.Pt(), probe_recoNgamma, weight_temp);
        prrecoNelq    ->Fill(tag_r.Pt(), probe_recoNe    , weight_temp);
        prrecoNmulq   ->Fill(tag_r.Pt(), probe_recoNmu   , weight_temp);

        if (probeFlav == 4) {                         //c-jets
          FFc->Fill(tag_r.Pt(), weight_temp);
          prMPFc->Fill(tag_r.Pt(), R_MPF_r, weight_temp);
          prgenMPFc->Fill(tag_g.Pt(), R_MPF_g, weight_temp);
          prRjetc->Fill(probe_g.Pt(), Rjet, weight_temp);
          prRjetvstagc->Fill(tag_r.Pt(), Rjet, weight_temp);    

          prchfc   ->Fill(tag_r.Pt(), probe_ch/totalE,    weight_temp);
          prnhfc   ->Fill(tag_r.Pt(), probe_nh/totalE,    weight_temp);
          prgammafc->Fill(tag_r.Pt(), probe_gamma/totalE, weight_temp);
          prefc    ->Fill(tag_r.Pt(), probe_e/totalE,     weight_temp);
          prmufc   ->Fill(tag_r.Pt(), probe_mu/totalE,    weight_temp);

          prgenNchc    ->Fill(tag_r.Pt(), probe_genNch   , weight_temp);
          prgenNnhc    ->Fill(tag_r.Pt(), probe_genNnh   , weight_temp);
          prgenNgammac ->Fill(tag_r.Pt(), probe_genNgamma, weight_temp);
          prgenNec     ->Fill(tag_r.Pt(), probe_genNe    , weight_temp);
          prgenNmuc    ->Fill(tag_r.Pt(), probe_genNmu   , weight_temp);

          prrecoNchc   ->Fill(tag_r.Pt(), probe_recoNch   , weight_temp);
          prrecoNnhc   ->Fill(tag_r.Pt(), probe_recoNnh   , weight_temp);
          prrecoNgammac->Fill(tag_r.Pt(), probe_recoNgamma, weight_temp);
          prrecoNec    ->Fill(tag_r.Pt(), probe_recoNe    , weight_temp);
          prrecoNmuc   ->Fill(tag_r.Pt(), probe_recoNmu   , weight_temp);
     
        } else if (probeFlav == 3) {                  //s-jets
          FFs->Fill(tag_r.Pt(), weight_temp);
          prMPFs->Fill(tag_r.Pt(), R_MPF_r, weight_temp);
          prgenMPFs->Fill(tag_g.Pt(), R_MPF_g, weight_temp);
          prRjets->Fill(probe_g.Pt(), Rjet, weight_temp);
          prRjetvstags->Fill(tag_r.Pt(), Rjet, weight_temp);  

          prchfs   ->Fill(tag_r.Pt(), probe_ch/totalE,    weight_temp);
          prnhfs   ->Fill(tag_r.Pt(), probe_nh/totalE,    weight_temp);
          prgammafs->Fill(tag_r.Pt(), probe_gamma/totalE, weight_temp);
          prefs    ->Fill(tag_r.Pt(), probe_e/totalE,     weight_temp);
          prmufs   ->Fill(tag_r.Pt(), probe_mu/totalE,    weight_temp);

          prgenNchs    ->Fill(tag_r.Pt(), probe_genNch   , weight_temp);
          prgenNnhs    ->Fill(tag_r.Pt(), probe_genNnh   , weight_temp);
          prgenNgammas ->Fill(tag_r.Pt(), probe_genNgamma, weight_temp);
          prgenNes     ->Fill(tag_r.Pt(), probe_genNe    , weight_temp);
          prgenNmus    ->Fill(tag_r.Pt(), probe_genNmu   , weight_temp);

          prrecoNchs   ->Fill(tag_r.Pt(), probe_recoNch   , weight_temp);
          prrecoNnhs   ->Fill(tag_r.Pt(), probe_recoNnh   , weight_temp);
          prrecoNgammas->Fill(tag_r.Pt(), probe_recoNgamma, weight_temp);
          prrecoNes    ->Fill(tag_r.Pt(), probe_recoNe    , weight_temp);
          prrecoNmus   ->Fill(tag_r.Pt(), probe_recoNmu   , weight_temp);

        } else if (probeFlav == 1 || probeFlav == 2) {    //(u,d)
          FFud->Fill(tag_r.Pt(), weight_temp);
          prMPFud->Fill(tag_r.Pt(), R_MPF_r, weight_temp);
          prgenMPFud->Fill(tag_g.Pt(), R_MPF_g, weight_temp);
          prRjetud->Fill(probe_g.Pt(), Rjet, weight_temp);
          prRjetvstagud->Fill(tag_r.Pt(), Rjet, weight_temp);

          prchfud   ->Fill(tag_r.Pt(), probe_ch/totalE,    weight_temp);
          prnhfud   ->Fill(tag_r.Pt(), probe_nh/totalE,    weight_temp);
          prgammafud->Fill(tag_r.Pt(), probe_gamma/totalE, weight_temp);
          prefud    ->Fill(tag_r.Pt(), probe_e/totalE,     weight_temp);
          prmufud   ->Fill(tag_r.Pt(), probe_mu/totalE,    weight_temp);

          prgenNchud    ->Fill(tag_r.Pt(), probe_genNch   , weight_temp);
          prgenNnhud    ->Fill(tag_r.Pt(), probe_genNnh   , weight_temp);
          prgenNgammaud ->Fill(tag_r.Pt(), probe_genNgamma, weight_temp);
          prgenNeud     ->Fill(tag_r.Pt(), probe_genNe    , weight_temp);
          prgenNmuud    ->Fill(tag_r.Pt(), probe_genNmu   , weight_temp);

          prrecoNchud   ->Fill(tag_r.Pt(), probe_recoNch   , weight_temp);
          prrecoNnhud   ->Fill(tag_r.Pt(), probe_recoNnh   , weight_temp);
          prrecoNgammaud->Fill(tag_r.Pt(), probe_recoNgamma, weight_temp);
          prrecoNeud    ->Fill(tag_r.Pt(), probe_recoNe    , weight_temp);
          prrecoNmuud   ->Fill(tag_r.Pt(), probe_recoNmu   , weight_temp);

        }
      } else if (probeFlav == 21) {                   //Gluon jets
        FFg->Fill(tag_r.Pt(), weight_temp);
        prMPFg->Fill(tag_r.Pt(), R_MPF_r, weight_temp);
        prgenMPFg->Fill(tag_g.Pt(), R_MPF_g, weight_temp);
        prRjetg->Fill(probe_g.Pt(), Rjet, weight_temp);
        prRjetvstagg->Fill(tag_r.Pt(), Rjet, weight_temp);

        prchfg   ->Fill(tag_r.Pt(), probe_ch/totalE,    weight_temp);
        prnhfg   ->Fill(tag_r.Pt(), probe_nh/totalE,    weight_temp);
        prgammafg->Fill(tag_r.Pt(), probe_gamma/totalE, weight_temp);
        prefg    ->Fill(tag_r.Pt(), probe_e/totalE,     weight_temp);
        prmufg   ->Fill(tag_r.Pt(), probe_mu/totalE,    weight_temp);

        prgenNchg    ->Fill(tag_r.Pt(), probe_genNch   , weight_temp);
        prgenNnhg    ->Fill(tag_r.Pt(), probe_genNnh   , weight_temp);
        prgenNgammag ->Fill(tag_r.Pt(), probe_genNgamma, weight_temp);
        prgenNeg     ->Fill(tag_r.Pt(), probe_genNe    , weight_temp);
        prgenNmug    ->Fill(tag_r.Pt(), probe_genNmu   , weight_temp);

        prrecoNchg   ->Fill(tag_r.Pt(), probe_recoNch   , weight_temp);
        prrecoNnhg   ->Fill(tag_r.Pt(), probe_recoNnh   , weight_temp);
        prrecoNgammag->Fill(tag_r.Pt(), probe_recoNgamma, weight_temp);
        prrecoNeg    ->Fill(tag_r.Pt(), probe_recoNe    , weight_temp);
        prrecoNmug   ->Fill(tag_r.Pt(), probe_recoNmu   , weight_temp);

      } 
      if (probeFlav != -1) FFa->Fill(tag_r.Pt(), weight_temp);
    }

    //If the old list of cut events is not read, a new one is written
    if (!GetuseEarlierCuts()) passedCuts[jentry]=true;
    
   } //Loop Tree entries

  //weight_stream.close();	//Close the output stream

  if (!GetuseEarlierCuts()) {
    //Write the accepted event information into a binary file
    cutflag_stream.open(cutflag_file, std::ios::out); //Write mode
    for (long l=0; l!=passedCuts.size(); ++l) {
      if (passedCuts[l]) cutflag_stream << true;
      else               cutflag_stream << false;
    }
    cutflag_stream.close();	//Close the output stream
  }


 
  if (GetcontHistos()) { //Particle composition and flavour fraction histograms

    //Canvas and related setup
    string canvName = ReadName + "_prtclCont";
    TCanvas* canv = new TCanvas(canvName.c_str(),"",1600,800);
    canv->SetBottomMargin(100);
    canv->SetLeftMargin(0.13);

    canv->Divide(2,1);



    TLegend* lg      = new TLegend(0.865, 0.1, 1.0,  0.9);
    TLegend* lgOther = new TLegend(0.865,0.55, 1.0, 0.90);
    lg->SetBorderSize(0);  lgOther->SetBorderSize(0);   //No frame
    lg->SetFillStyle(0);   lgOther->SetFillStyle(0);    //No background fill

    //Normalize histograms by the sum of all ptcls and add to stack
    h_gamma->Divide(h_all);	hstack->Add(h_gamma);
    h_gpi0->Divide( h_all);	hstack->Add(h_gpi0 );
    h_pi->Divide(   h_all);	hstack->Add(h_pi   );
    h_K->Divide(    h_all);	hstack->Add(h_K    );
    h_K0S->Divide(  h_all);	hstack->Add(h_K0S  );
    h_K0L->Divide(  h_all);	hstack->Add(h_K0L  );
    h_L->Divide(    h_all);	hstack->Add(h_L    );
    h_n->Divide(    h_all);	hstack->Add(h_n    );
    h_e->Divide(    h_all);	hstack->Add(h_e    );
    h_mu->Divide(   h_all);	hstack->Add(h_mu   );
    h_nu->Divide(   h_all);	hstack->Add(h_nu   );
    h_p->Divide(    h_all);	hstack->Add(h_p    );
    //Normalize the Xi, Sigma & Omega histos by the "other particles" histo
    h_Xi0->Divide( h_other);	hstackOther->Add(h_Xi0);
    h_Xim->Divide( h_other);	hstackOther->Add(h_Xim);
    h_Sig0->Divide(h_other);	hstackOther->Add(h_Sig0);
    h_Sigm->Divide(h_other);	hstackOther->Add(h_Sigm);
    h_Sigp->Divide(h_other);	hstackOther->Add(h_Sigp);
    h_Om->Divide( h_other);	hstackOther->Add(h_Om);
    //Finally divide the "other particles" histo
    h_other->Divide(h_all);	hstack->Add(h_other);
    //Legend setup
    lg->AddEntry(h_other, " #font[12]{Other}",     "f");
    lg->AddEntry(h_p,     " #font[12]{p}",         "f");
    lg->AddEntry(h_nu,    " #nu",                  "f");
    lg->AddEntry(h_mu,    " #mu",                  "f");
    lg->AddEntry(h_e,     " #font[12]{e^{#pm}}",   "f");
    lg->AddEntry(h_n,     " #font[12]{n}",         "f");
    lg->AddEntry(h_L,     " #Lambda",              "f");
    lg->AddEntry(h_K0L,   " #font[12]{K^{0}_{L}}", "f");
    lg->AddEntry(h_K0S,   " #font[12]{K^{0}_{S}}", "f");
    lg->AddEntry(h_K,     " #font[12]{K^{#pm}}",   "f");
    lg->AddEntry(h_pi,    " #pi^{#pm}",            "f");
    lg->AddEntry(h_gpi0,  " #gamma(#pi^{0}) ",     "f");
    lg->AddEntry(h_gamma, " #gamma",               "f");
    //The legend for separate "other particles" plot
    lgOther->AddEntry(h_Om,   " #Omega^{-}", "f");
    lgOther->AddEntry(h_Sigp, " #Sigma^{+}", "f");
    lgOther->AddEntry(h_Sigm, " #Sigma^{-}", "f");
    lgOther->AddEntry(h_Sig0, " #Sigma^{0}", "f");
    lgOther->AddEntry(h_Xim,  " #Xi^{-}",    "f");
    lgOther->AddEntry(h_Xi0,  " #Xi^{0}",    "f");

      //General particle composition histograms
    canv->cd(1);

    

    hstack->Draw("HISTO");
    hstack->GetYaxis()->SetRange(0.0,0.5);
    hstack->GetXaxis()->SetTitle("E_{jet}^{gen} (GeV)");
    hstack->GetYaxis()->SetTitle("Fraction of jet E");
    hstack->GetYaxis()->SetTitleSize(0.045);
    hstack->GetXaxis()->SetTitleSize(0.045);
    hstack->GetXaxis()->SetTitleOffset(0.95);
    hstack->GetYaxis()->SetTitleOffset(1.1);

    lg->Draw();

    //More specific "other particles" histograms
    canv->cd(2);
 

    hstackOther->Draw("HISTO");
    hstackOther->GetYaxis()->SetRange(0.0,1.0);
    hstackOther->GetXaxis()->SetTitle("E_{jet}^{gen} (GeV)");
    hstackOther->GetYaxis()->SetTitle("Fraction of #font[12]{other}");
    hstackOther->GetYaxis()->SetTitleSize(0.045);
    hstackOther->GetXaxis()->SetTitleSize(0.045);
    hstackOther->GetXaxis()->SetTitleOffset(0.95);
    hstackOther->GetYaxis()->SetTitleOffset(1.1);
    lgOther->Draw();
    gStyle->SetHistTopMargin(0.);

    //Save particle content histogram plot
    string plotName = "./plots/particleComposition/PC_" + ReadName;
    plotName += ".pdf";	//Filetype suffix
    canv->Print(plotName.c_str());



    //Print unclassified particles
    if (printProg && otherIDs.size()!=0) {
      cout << "Sample includes also these "<<otherIDs.size()<<" PDGIDs:"<<endl;
      for (int i=0; i<otherIDs.size(); ++i) cout << otherIDs[i] << endl;
    }

    //Free memory allocated only if GetcontHistos()==true
    delete lg;     delete lgOther;
    delete h_e;    delete h_mu;    delete h_gamma; delete h_gpi0;
    delete h_K;    delete h_K0S;   delete h_K0L;   delete h_L;
    delete h_n;    delete h_nu;    delete h_pi;
    delete h_p;    delete h_other; delete h_Xi0;   delete h_Xim;
    delete h_Sig0; delete h_Sigm;  delete h_Sigp;
    delete h_Om;   delete h_all;
    delete hstack; delete hstackOther;
    delete canv;

  } //Particle composition histograms

  cout << endl << "Event cuts:" << endl;
  cout << "Total number of events: " << nentries    << endl;
  cout << "Tag muon cuts:          " << mumuCut     << endl;
  cout << "Z invariant mass:       " << invM        << endl;
  cout << "Tag eta pT:             " << tagCut      << endl;
  cout << "Gen level Probe:        " << genprobeCut << endl;
  cout << "Btb tag-probe:          " << b2b         << endl;
  cout << "Alpha:                  " << alpha       << endl;
  cout << "Reco level Probe:       " << probeCut    << endl;
  cout << "MET cut:                " << METcut      << endl;
  cout << "Weight cut:             " << weightCut   << endl;
  cout << "Total events:           " << evt         << endl << endl;
  
  //Charged hadron efficiency Profile
  TCanvas *c4     = new TCanvas("","c4",500,500);
  TGraph *PFeff   = new TGraph("data_and_MC_input/Efficiency/hadron_efficiency_PF.txt");
  TGraph *PFeffFr = new TGraph("data_and_MC_input/Efficiency/hadron_efficiencyPlusFakerate_PF.txt");
  TGraph *TRKeff  = new TGraph("data_and_MC_input/Efficiency/hadron_efficiency_TRK.txt");
  PFeff->SetMarkerStyle(kFullCircle ); TRKeff->SetMarkerStyle(kOpenCircle );
  PFeff->SetMarkerColor(kRed);         TRKeff->SetMarkerColor(kBlue+1);
  PFeffFr->SetMarkerStyle(kFullDiamond );
  PFeffFr->SetMarkerColor(kGreen+2);

  TLegend* lgEff = new TLegend(0.5,0.2,0.8,0.40 );
  lgEff->AddEntry(chhEff, "#font[132]{Our eff}", "l");
  lgEff->AddEntry(PFeff,  "#font[132]{PF eff}", "p");
  lgEff->AddEntry(PFeffFr,"#font[132]{PF eff+fakerate}", "p");
  lgEff->AddEntry(TRKeff, "#font[132]{TRK eff}", "p");
  lgEff->SetBorderSize(0); lgEff->SetFillStyle(0);

  PFeff->GetXaxis()->SetTitle("pT");
  PFeff->GetYaxis()->SetTitle("Efficiency");
   
  TAxis *PFaxis = PFeff->GetXaxis();
  PFaxis->SetLimits(0.1,300);
  PFeff->GetHistogram()->SetMaximum(1.1);
  PFeff->GetHistogram()->SetMinimum(0.0);
  PFeff->Draw("ap");      lgEff->Draw("same");
  PFeffFr->Draw("samep"); TRKeff->Draw("samep");

  gStyle->SetOptStat();
  c4->SetLogx();
  chhEff->Draw("same");
  chhEff->SetLineColor(kBlack);
  chhEff->SetLineWidth(3);
  string savename = "./plots/Efficiency/trkEff.eps";
  c4->Print(savename.c_str());

  //Save CMSJES TTree
  fout->Write();
  fout->Close();

  //Free single-particle response functions from memory
  delete fr_h; delete fr_hcal;

  clock_t t_end = clock();
  cout << "Execution time: " << (double)(t_end - t_begin) / CLOCKS_PER_SEC << endl;

} //CMSJES::Loop

void CMSJES::Response(int pdgid, double pseudorap, double energy, double pT, double Rt, 
                      double Bfield, double HHeFrac, TF1* frH, double& retMC, double& retA, 
                      double& retEHE, double& retHHe)
{
  retMC = 0.0; retA = 0.0; retEHE = 0.0; retHHe = 0.0;
  bool zero = false; //If true, return zero responses

  double sfCh = 0.0; //Charged particle step function
  double sfN  = 0.0; //Neutral hadron step function

  //Responses for different groups
  double cat1     = 0.0;    double cat2     = 0.0;     double cat3     = 0.0;
  double cat1_EHE = 0.0;    double cat2_EHE = 0.0;     double cat3_EHE = 0.0;
  double cat1_HHe = 0.0;    double cat2_HHe = 0.0;     double cat3_HHe = 0.0;

  double resp_p      = 0.0; double resp_ap      = 0.0; double resp_n      = 0.0;
  double resp_an     = 0.0; double resp_Kpm     = 0.0; double resp_K0     = 0.0;
  double resp_p_EHE  = 0.0; double resp_ap_EHE  = 0.0; double resp_n_EHE  = 0.0;
  double resp_an_EHE = 0.0; double resp_Kpm_EHE = 0.0; double resp_K0_EHE = 0.0;
  double resp_p_HHe  = 0.0; double resp_ap_HHe  = 0.0; double resp_n_HHe  = 0.0;
  double resp_an_HHe = 0.0; double resp_Kpm_HHe = 0.0; double resp_K0_HHe = 0.0;

  //Pion responses for ECAL and HCAL
  double resp_pi     = 0.0; double resp_pi_EHE = 0.0; double resp_pi_HHe = 0.0; 

  if (    pT > 0.3) sfCh = 1.0; // Step function for charged particles
  if (energy > 2.0) sfN  = 1.0; // Step function for calo deposits 


  if (isChHadron(pdgid)) {
    if (fabs(pseudorap) > 1.3 && pT < 1.0) sfN = 0.0;
  }

  if (!doesReachEcal(pdgid, pT, Bfield, Rt)) sfN = 0.0;

  //Check if particle outside good eta region
  if (fabs(pseudorap) > 5.2) zero = true;

  unsigned int row = int(fabs(pseudorap)*10); //Param matrix row from |eta|

  //Assert there's no pi^0 (PDGID 111) or eta (221) after parton shower
  if (abs(pdgid)==111 || abs(pdgid)==221) { 
    cout << "WARNING: pi^0 (111) or eta (221) found! PDGID: " << pdgid
         << "Returning zero response" << endl; zero = true;
  }

  //Neutrino responses are zero
  if (isNeutrino(abs(pdgid))) zero = true;

  //CALCULATE RESPONSES
  for (int i_r=0; i_r<(zero?0:1); ++i_r) {
    
    //Pion EHE and HHe responses from |eta|<1.3
    frH->SetParameters(params_pi_EHE[row][0], params_pi_EHE[row][1], //EHE
                       params_pi_EHE[row][2], 1, 0, 1); 
    resp_pi_EHE = frH->Eval(energy);

    frH->SetParameters(params_pi_HHe[row][0], params_pi_HHe[row][1], //HHe
                       params_pi_HHe[row][2], 1, 0, 1); 
    resp_pi_HHe = frH->Eval(energy);
    //....................................................................

    //cat1 cat2 cat3 responsens
    frH->SetParameters(params_cat1[row][0], params_cat1[row][1],
                       params_cat1[row][2], 1, 0, 1);
    cat1 = frH->Eval(energy);

    frH->SetParameters(params_cat2[row][0], params_cat2[row][1],
                       params_cat2[row][2], 1, 0, 1);
    cat2 = frH->Eval(energy);

    frH->SetParameters(params_cat3[row][0], params_cat3[row][1],
                       params_cat3[row][2], 1, 0, 1);
    cat3 = frH->Eval(energy);

    //p,ap,n,an,Kpm,K0
    frH->SetParameters(params_pi[row][0], params_pi[row][1],
                       params_pi[row][2], 1, 0, 1);
    resp_pi = frH->Eval(energy);

    frH->SetParameters(params_p[row][0], params_p[row][1],
                       params_p[row][2], 1, 0, 1);
    resp_p = frH->Eval(energy);

    frH->SetParameters(params_ap[row][0], params_ap[row][1],
                       params_ap[row][2], 1, 0, 1);
    resp_ap = frH->Eval(energy);

    frH->SetParameters(params_n[row][0], params_n[row][1],
                       params_n[row][2], 1, 0, 1);
    resp_n = frH->Eval(energy);

    frH->SetParameters(params_an[row][0], params_an[row][1],
                       params_an[row][2], 1, 0, 1);
    resp_an = frH->Eval(energy);

    frH->SetParameters(params_Kpm[row][0], params_Kpm[row][1],
                       params_Kpm[row][2], 1, 0, 1);
    resp_Kpm = frH->Eval(energy);

    frH->SetParameters(params_K0[row][0], params_K0[row][1],
                       params_K0[row][2], 1, 0, 1);
    resp_K0 = frH->Eval(energy);

    //EHE responses
    cat1_EHE = cat1*(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    cat2_EHE = cat2*(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    cat3_EHE = cat3*(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));

    resp_p_EHE   = resp_p  *(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_ap_EHE  = resp_ap *(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_n_EHE   = resp_n  *(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_an_EHE  = resp_an *(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_Kpm_EHE = resp_Kpm*(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_K0_EHE  = resp_K0 *(resp_pi_EHE/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));

    //HHe responses
    cat1_HHe = cat1*(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    cat2_HHe = cat2*(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    cat3_HHe = cat3*(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));

    resp_p_HHe   = resp_p  *(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_ap_HHe  = resp_ap *(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_n_HHe   = resp_n  *(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_an_HHe  = resp_an *(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_Kpm_HHe = resp_Kpm*(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));
    resp_K0_HHe  = resp_K0 *(resp_pi_HHe/(HHeFrac*resp_pi_HHe + (1-HHeFrac)*resp_pi_EHE));

    switch (pdgid) {
      //PHOTON
      case 20 :
      case 22 :
        retMC = sfCh;
        break;

      //LEPTONS
      case  11 : //e
      case -11 : //e
        retMC = sfCh; 
        break;
      case  13 : //mu
      case -13 : //mu
        retMC = sfCh; 
        break;
      
      //cat1: antineutron, K0S, K0L, pi+, pi-
      case   211 : //pi+
      case  -211 : //pi-
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = resp_pi*sfN; 
        retA   = resp_pi*sfN;
        retEHE = resp_pi_EHE*sfN;
        retHHe = resp_pi_HHe*sfN;

        break;

      //cat2: neutron, antiproton, K+, K-
      case -3122 : //anti-Lambda
      case -3212 : //anti-Sigma^0
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = cat2*sfN; 
        retA   = cat2*sfN;
        retEHE = cat2_EHE*sfN;
        retHHe = cat2_HHe*sfN;
        break;

      //cat3: proton
      case  3122 : //Lambda
      case  3212 : //Sigma^0
      case  3322 : //Xi^0         cat=4
      case -3322 : //anti-Xi^0
      case  3312 : //Xi^-         cat=5
      case -3312 : //Xi^-         cat=4
      case  3112 : //Sigma^-      cat=4
      case -3112 : //anti-Sigma^-
      case  3222 : //Sigma^+      cat=4
      case -3222 : //anti-Sigma^+
      case  3334 : //Omega^-      cat=6
      case -3334 : //anti-Omega^- cat=5
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = cat3*sfN; 
        retA   = cat3*sfN;
        retEHE = cat3_EHE*sfN;
        retHHe = cat3_HHe*sfN;
        break;

      case  2212 : //p
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = resp_p*sfN; 
        retA   = resp_p*sfN;
        retEHE = resp_p_EHE*sfN;
        retHHe = resp_p_HHe*sfN;
        break;

      case -2212 : //ap
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = resp_ap*sfN; 
        retA   = resp_ap*sfN;
        retEHE = resp_ap_EHE*sfN;
        retHHe = resp_ap_HHe*sfN;
        break;

      case  2112 : //n
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = resp_n*sfN; 
        retA   = resp_n*sfN;
        retEHE = resp_n_EHE*sfN;
        retHHe = resp_n_HHe*sfN;
        break;

      case -2112 : //an
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = resp_an*sfN; 
        retA   = resp_an*sfN;
        retEHE = resp_an_EHE*sfN;
        retHHe = resp_an_HHe*sfN;
        break;

      case   321 : //K+
      case  -321 : //K-
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = resp_Kpm*sfN; 
        retA   = resp_Kpm*sfN;
        retEHE = resp_Kpm_EHE*sfN;
        retHHe = resp_Kpm_HHe*sfN;
        break;

      case   310 : //K^0_S
      case   130 : //K^0_L
        if ((fabs(pseudorap) < 2.5) && fabs(Charge(pdgid))) retMC = sfCh;
        else retMC = resp_K0*sfN; 
        retA   = resp_K0*sfN;
        retEHE = resp_K0_EHE*sfN;
        retHHe = resp_K0_HHe*sfN;
        break;

      default : 
        zero=true;
        cout << "Unknown particle PDG: " << pdgid << endl;
        continue;	 
    }
  }

  //Set results. Check for NaN and negative results if positive demanded
  if (zero || isnan(retMC)  || retMC  <=0) retMC  = 0.0;
  if (zero || isnan(retA)   || retA   <=0) retA   = 0.0;
  if (zero || isnan(retEHE) || retEHE <=0) retEHE = 0.0;
  if (zero || isnan(retHHe) || retHHe <=0) retHHe = 0.0;

} //Response

//-----------------------------------------------------------------------------
//Check if this particle is a neutrino
//Param:	id	The particle's PDGDID
//Returns true if the particle is a nu_e, nu_mu, nu_tau
bool CMSJES::isNeutrino(int id) {
  int PDG = abs(id);
  switch (PDG) {
    case 12 : return true;
    case 14 : return true;
    case 16 : return true;
    default : return false;
  }
}
//-----------------------------------------------------------------------------
//Check if this particle is one of those for which D0 had no response
//Param:	id	The particle's PDGDID
//Returns true if the particle is a Xi, Sigma or Omega^-
bool CMSJES::isStrangeB(int id) {
  int PDG = abs(id);
  switch (PDG) {
    case 3112 : return true;
    case 3212 : return true;
    case 3222 : return true;
    case 3312 : return true;
    case 3322 : return true;
    case 3334 : return true;
    default   : return false;
  }
}

//-----------------------------------------------------------------------------
//Construct input filenames for functions using multiple CMSJES 
//objects.
//Properties are chosen based on the object doing this analysis, since the
//CMSJES objects instantiated in this function will have the same properties.
// => The user cannot e.g. accidentally enter a file savename that would
//    imply properties that are not really there.
//Saves the filenames into the zjFile string 
//belonging to this CMSJES object.

void CMSJES::InputNameConstructor() {

  //Prevent overwriting existing filenames
  if (zjFile!="") return;

  //Init strings to contain parts of the resulting filenames
  string num = "";	//#events in the sample to study

  //Generator and sample event content indicators
  if (ReadName.find("P8")!=string::npos) {zjFile="P8_";}
  else if (ReadName.find("H7")!=string::npos) {zjFile="H7_";}
  zjFile += "Zjet_";

  //#events
  if      (ReadName.find("10000000") !=string::npos) num = "10000000";
  else if (ReadName.find("5000000")  !=string::npos) num = "5000000";
  else if (ReadName.find("1000000")  !=string::npos) num = "1000000";
  else if (ReadName.find("600000")   !=string::npos) num = "600000";
  else if (ReadName.find("300000")   !=string::npos) num = "300000";
  else if (ReadName.find("100000")   !=string::npos) num = "100000";
  else if (ReadName.find("30000")    !=string::npos) num = "30000";
  else if (ReadName.find("10000")    !=string::npos) num = "10000";
  else if (ReadName.find("3000")     !=string::npos) num = "3000";
  else {
    cout << "ERROR: fitting not supported for this file" << endl;
    return;
  }
  //The resulting filenames. Suffix ".root" to be added in CMSJES constructor
  zjFile += num;
}


void CMSJES::plotEff(int gen, int Nevt) {
  //Choose filenames to open
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);
  TFile* fzj = TFile::Open(zjetFile.c_str());

  TProfile *chhEff=0;
  fzj->GetObject("chhEff",chhEff);

  //Charged hadron efficiency Profile
  TCanvas *c4     = new TCanvas("","c4",500,500);
  c4->SetLeftMargin(0.12);
  TGraph *PFeff   = new TGraph("data_and_MC_input/Efficiency/hadron_efficiency_PF.txt");
  TGraph *PFeffFr = new TGraph("data_and_MC_input/Efficiency/hadron_efficiencyPlusFakerate_PF.txt");
  PFeff->SetMarkerStyle(kFullDiamond );
  PFeff->SetMarkerColor(kBlue);
  PFeffFr->SetMarkerStyle(kFullCircle );
  PFeffFr->SetMarkerColor(kRed);

  TLegend* lgEff = new TLegend(0.3,0.2,0.75,0.40 );
  lgEff->AddEntry(chhEff, "Our efficiency", "l");
  lgEff->AddEntry(PFeffFr,"PF efficiency + fakerate", "p");
  lgEff->AddEntry(PFeff,  "PF efficiency", "p");
  lgEff->SetBorderSize(0); lgEff->SetFillStyle(0);

  PFeff->GetXaxis()->SetTitleOffset(1.0);
  PFeff->GetYaxis()->SetTitleOffset(1.3);
  PFeff->GetXaxis()->SetTitle("p_{T} (GeV)");
  PFeff->GetXaxis()->SetTitleSize(0.045);
  PFeff->GetYaxis()->SetTitle("Tracking efficiency");
  PFeff->GetYaxis()->SetTitleSize(0.045);

  TLatex *tex = new TLatex();
  tex->SetNDC();
  tex->SetTextSize(0.04);
  tex->SetTextColor(kBlack);

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
  chhEff->Draw("same");
  chhEff->SetLineColor(kBlack);
  chhEff->SetLineWidth(5);

  tex->DrawLatex(0.75,0.82,"|#eta|<2.5");

  string plotName = "./plots/Efficiency/trkEff_" + ReadName + ".pdf";
  c4->Print(plotName.c_str());
  delete c4;
}

void CMSJES::plotJEF(int gen, int Nevt) {
  //Choose filenames to open
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);

  TFile* fzj = TFile::Open(zjetFile.c_str());

  TGraph *chf   = new TGraph("data_and_MC_input/Chf/chf.txt");
  TGraph *nhf = new TGraph("data_and_MC_input/Chf/nhf.txt");
  chf->SetMarkerStyle(kFullDiamond); nhf->SetMarkerStyle(kOpenSquare);
  chf->SetMarkerColor(kBlack);       nhf->SetMarkerColor(kBlack);
  chf->SetMarkerSize(1);             nhf->SetMarkerSize(0.7);

  TProfile *prchf=0; TProfile *prnhf=0; TProfile *prgammaf=0; TProfile *pref=0;

  THStack* hstack_JEF;
  hstack_JEF = new THStack("hstackJEF","");


  fzj->GetObject("prchf"    ,prchf   );
  fzj->GetObject("prnhf"    ,prnhf   );
  fzj->GetObject("prgammaf" ,prgammaf);
  fzj->GetObject("pref"     ,pref    );

  TH1D* hchf    = prchf   ->ProjectionX();
  TH1D* hnhf    = prnhf   ->ProjectionX();
  TH1D* hgammaf = prgammaf->ProjectionX();
  TH1D* hef     = pref    ->ProjectionX();

  hchf   ->SetFillColor(kRed-7);   hchf->SetLineWidth(1);    hchf->SetLineColor(kBlack);
  hnhf   ->SetFillColor(kGreen-6); hnhf->SetLineWidth(1);    hnhf->SetLineColor(kBlack);
  hef    ->SetFillColor(kCyan+1);  hef->SetLineWidth(1);     hef->SetLineColor(kBlack);
  hgammaf->SetFillColor(kBlue-7);  hgammaf->SetLineWidth(1); hgammaf->SetLineColor(kBlack);


  //Canvas and related setup
  TCanvas* canv_c = new TCanvas("canv_JEF","",500,400);
  TLegend* lg     = new TLegend(0.9, 0.1, 1.0, 0.9);
  lg->SetBorderSize(0); //No frame
  lg->SetTextSize(0.028);
  lg->SetFillStyle(0);
  canv_c->SetLogx();
  canv_c->SetBottomMargin(0.13);

  hstack_JEF->Add(hchf);
  hstack_JEF->Add(hnhf);
  hstack_JEF->Add(hgammaf);
  hstack_JEF->Add(hef);

  //Legend setup
  lg->AddEntry(hef,     "e",     "f");
  lg->AddEntry(hgammaf, "#gamma","f");
  lg->AddEntry(hnhf,    "NH",    "f");
  lg->AddEntry(hchf,    "CH",    "f");

  TLegend* lgEff = new TLegend(0.15,0.25,0.4,0.35);
  lgEff->AddEntry(nhf,"#bf{NH Run2016GH}", "p");
  lgEff->AddEntry(chf,"#bf{CH Run2016GH}", "p");
  lgEff->SetBorderSize(0); lgEff->SetFillStyle(0);
  lgEff->SetTextSize(0.035);


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
  //hstack_JEF->Draw("same"); //For error bars

  gStyle->SetHistTopMargin(0.);

  gPad->SetTickx(); 
  gPad->SetTicky();

  lg->Draw();
  lgEff->Draw();
  chf->Draw("samep");
  nhf->Draw("samep");

  TLatex *tex = new TLatex();
  tex->SetNDC();
  tex->SetTextSize(0.04);
  tex->SetTextColor(kBlack);
  tex->DrawLatex(0.21,0.41,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.21,0.36,"#alpha<0.3");
  tex->DrawLatex(0.75,0.92,"#sqrt{s}=13 TeV");



  string plotName = "./plots/particleComposition/JEF_probe_" + ReadName + ".pdf";
  canv_c->Print(plotName.c_str());
  delete canv_c;
}

//Flavour fraction
void CMSJES::plotFF(int gen, int Nevt) {
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);

  TFile* fzj = TFile::Open(zjetFile.c_str());


  TH1D* FFa  = 0;
  TH1D* FFb  = 0;
  TH1D* FFg  = 0;
  TH1D* FFud = 0;
  TH1D* FFs  = 0;
  TH1D* FFc  = 0;
  TH1D* FFlq = 0;

  fzj->GetObject("FFa" ,FFa );
  fzj->GetObject("FFb" ,FFb );
  fzj->GetObject("FFg" ,FFg );
  fzj->GetObject("FFud",FFud);
  fzj->GetObject("FFs" ,FFs);
  fzj->GetObject("FFc" ,FFc);
  fzj->GetObject("FFlq",FFlq);


  string FFstackTitle;
  FFstackTitle  = "";
  FFstackTitle += ";p_{T,tag}^{reco} (GeV)";
  FFstackTitle += ";Jet flavour fraction";

  THStack* FFstack = new THStack("", FFstackTitle.c_str());

  FFb->Divide(FFa);   FFg->Divide(FFa);    FFlq->Divide(FFa);
  FFud->Divide(FFa);  FFs->Divide(FFa);    FFc->Divide(FFa);
  FFstack->Add(FFg);  FFstack->Add(FFud);  FFstack->Add(FFs); 
  FFstack->Add(FFc);   FFstack->Add(FFb);


  string FFcanvName = ReadName + "_FF";
  TCanvas* FFcanv = new TCanvas(FFcanvName.c_str(),"",500,400);
  TLegend* FFlg = new TLegend(0.91, 0.6, 1.00, 0.90);
  FFcanv->SetLogx();
  FFlg->SetBorderSize(0); FFlg->SetFillStyle(0);
  FFcanv->SetBottomMargin(0.13);
  FFlg->AddEntry(FFb,  " b",  "f");
  FFlg->AddEntry(FFc,  " c",  "f");
  FFlg->AddEntry(FFs,  " s",  "f");
  FFlg->AddEntry(FFud, " ud", "f");
  FFlg->AddEntry(FFg,  " g",  "f");

  /*
  FFb ->SetFillColor(kRed-7);  
  FFc ->SetFillColor(kGreen-6);
  FFs ->SetFillColor(kOrange-4);
  FFud->SetFillColor(kMagenta-6);
  FFg ->SetFillColor(kBlue-7);
  */

  FFb ->SetFillColor(46);  
  FFc ->SetFillColor(30);
  FFs ->SetFillColor(42);
  FFud->SetFillColor(40);
  FFg ->SetFillColor(38);

  FFstack->Draw("HISTO");

  FFstack->GetXaxis()->SetMoreLogLabels();
  FFstack->GetXaxis()->SetNoExponent();

  FFstack->GetXaxis()->SetTitleOffset(1.1);
  FFstack->GetYaxis()->SetTitleOffset(0.9);

  FFstack->GetYaxis()->SetTitle("Jet flavour fraction");
  FFstack->GetYaxis()->SetTitleSize(0.045);
  FFstack->GetXaxis()->SetTitle("p_{T,tag}^{reco} (GeV)");
  FFstack->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); 
  gPad->SetTicky();
  gPad->Modified();

  gStyle->SetHistTopMargin(0.);


  FFlg->Draw();

  TLatex *tex = new TLatex();
  tex->SetNDC();
  tex->SetTextSize(0.04);
  tex->SetTextColor(kBlack);
  tex->DrawLatex(0.21,0.41,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.21,0.36,"#alpha<0.3");
  tex->DrawLatex(0.75,0.92,"#sqrt{s}=13 TeV");


  FFcanvName = "./plots/FlavourFraction/" + FFcanvName + ".pdf";
  FFcanv->Print(FFcanvName.c_str());
  delete FFcanv;
}

//-------------------------------------------------------------------------------------------
void CMSJES::plotMPF(int gen, int Nevt)
{
  //Choose filenames to open
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);

  //Initialize histograms and open ROOT files and fetch the stored objects
  //  TH1 params: name, title, #bins, #lowlimit, #highlimit
  TFile* fzj = TFile::Open(zjetFile.c_str());
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

  mc_zj_MPFntI_2018->SetMarkerStyle( kOpenSquare); mc_zj_MPFntI_2018->SetMarkerColor( kBlack  );

  //Legend
  TLegend* lz_MPF = new TLegend(0.6,0.76,0.87,0.87);
  lz_MPF->SetBorderSize(0);
  lz_MPF->AddEntry(hzj_MPF, "All jets", "p");
  lz_MPF->AddEntry(mc_zj_MPFntI_2018, "FullSim 2018ABCD", "p");

  //Title and axis setup
  hzj_MPF->SetStats(0); //Suppress stat box
  hzj_MPF->SetTitle("");
  hzj_MPF->SetAxisRange(0.89,0.95,"Y"); //Vertical axis limits

  hzj_MPF->GetXaxis()->SetMoreLogLabels();
  hzj_MPF->GetXaxis()->SetNoExponent();
  hzj_MPF->GetYaxis()->SetTitleOffset(1.1);
  hzj_MPF->GetXaxis()->SetTitleOffset(1.1);
  hzj_MPF->GetXaxis()->SetTitle("p_{T,tag}^{reco} (GeV)");
  hzj_MPF->GetYaxis()->SetTitle("R_{MPF}");
  hzj_MPF->GetYaxis()->SetTitleSize(0.05);
  hzj_MPF->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx();   gPad->SetTicky();

  //Plot
  hzj_MPF->Draw("p");
  mc_zj_MPFntI_2018->Draw("P SAME");
  lz_MPF->Draw();

  TLatex *tex = new TLatex(); tex->SetNDC();
  tex->SetTextSize(0.04); tex->SetTextColor(kBlack);
  tex->DrawLatex(0.75,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.18,0.82,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.18,0.77,"#alpha<0.3");

  string plotName = "./plots/mpf/MPF_zmmjet_" + ReadName + ".pdf";
  canv_MPF->Print(plotName.c_str()); delete canv_MPF;

  //All flavours
  TCanvas* canv_all = new TCanvas("canv_all","",500,400);
  canv_all->SetLeftMargin(0.13); canv_all->SetBottomMargin(0.13);
  canv_all->SetLogx();

  hzj_MPF->SetMarkerStyle(5); hzj_MPF->SetLineColor(kBlack); 
  hzj_MPF->SetMarkerColor(kBlack);
  hzj_MPFb->SetMarkerStyle(kFullCircle);       hzj_MPFb->SetMarkerColor(kRed+1);
  hzj_MPFb->SetLineColor(kRed+1);
  hzj_MPFc->SetMarkerStyle(kFullTriangleDown); hzj_MPFc->SetMarkerColor(kGreen+3);
  hzj_MPFc->SetLineColor(kGreen+3);
  hzj_MPFs->SetMarkerStyle(kFullTriangleUp); hzj_MPFs->SetMarkerColor(kOrange+7);
  hzj_MPFs->SetLineColor(kOrange+7);
  hzj_MPFud->SetMarkerStyle(kFullDiamond);    hzj_MPFud->SetMarkerColor(kMagenta+2);
  hzj_MPFud->SetLineColor(kMagenta+2);
  hzj_MPFg ->SetMarkerStyle(kFullSquare);   hzj_MPFg ->SetMarkerColor(kBlue+1);
  hzj_MPFg->SetLineColor(kBlue+1);

  TLegend* lz_all = new TLegend(0.68,0.7,0.86,0.87);
  lz_all->SetBorderSize(0);
  lz_all->AddEntry(hzj_MPF,   "All", "p");
  lz_all->AddEntry(hzj_MPFb,  "b",   "p");
  lz_all->AddEntry(hzj_MPFc,  "c",   "p");
  lz_all->AddEntry(hzj_MPFs,  "s",   "p");
  lz_all->AddEntry(hzj_MPFud, "ud",  "p");
  lz_all->AddEntry(hzj_MPFg,  "g",   "p");
  lz_all->SetNColumns(2);

  TH1D* setup = new TH1D("setup","", hzj_MPF->GetXaxis()->GetNbins(),
			 hzj_MPF->GetXaxis()->GetXmin(), hzj_MPF->GetXaxis()->GetXmax());

  setup->SetStats(0); //Suppress stat box
  setup->SetTitle("");
  setup->SetAxisRange(0.84,0.98,"Y"); //Vertical axis limits
  setup->GetXaxis()->SetMoreLogLabels();
  setup->GetXaxis()->SetNoExponent();
  setup->GetYaxis()->SetTitleOffset(1.1);
  setup->GetXaxis()->SetTitleOffset(1.1);
  setup->GetXaxis()->SetTitle("p_{T,tag}^{reco} (GeV)");
  setup->GetYaxis()->SetTitle("R_{MPF}");
  setup->GetYaxis()->SetTitleSize(0.05);
  setup->GetXaxis()->SetTitleSize(0.05);
  gPad->SetTickx(); gPad->SetTicky();

  setup->Draw();
  hzj_MPF->Draw("SAMEP");
  hzj_MPFb->Draw("SAMEP");
  hzj_MPFc->Draw("SAMEP");
  hzj_MPFs->Draw("SAMEP");
  hzj_MPFud->Draw("SAMEP");
  hzj_MPFg->Draw("SAMEP");
  lz_all->Draw("SAMEP");

  TLatex *tex2 = new TLatex(); tex2->SetNDC();
  tex2->SetTextSize(0.04); tex2->SetTextColor(kBlack);
  tex2->DrawLatex(0.75,0.92,"#sqrt{s}=13 TeV");
  tex2->DrawLatex(0.18,0.82,"|#eta^{probe}|<1.3");
  tex2->DrawLatex(0.18,0.77,"#alpha<0.3");

  plotName = "./plots/mpf/MPF_zmmjet_all_" + ReadName + ".pdf";
  canv_all->Print(plotName.c_str());
  delete canv_all;
}

//-------------------------------------------------------------------------------------------
void CMSJES::plotRjet(int gen, int Nevt)
{
  //Choose filenames to open
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);

  //Initialize histograms and open ROOT files and fetch the stored objects
  TFile* fzj = TFile::Open(zjetFile.c_str());
  TProfile *przj_Rjet=0; TProfile *przj_Rjetb=0; TProfile *przj_Rjetg=0; TProfile *przj_Rjetlq=0;
  TProfile *przj_Rjetud=0; TProfile *przj_Rjets=0; TProfile *przj_Rjetc=0;

  //Create Histograms
  fzj->GetObject("prRjet"   ,przj_Rjet  );
  fzj->GetObject("prRjetb"  ,przj_Rjetb );
  fzj->GetObject("prRjetg"  ,przj_Rjetg );
  fzj->GetObject("prRjetlq" ,przj_Rjetlq);
  fzj->GetObject("prRjetud" ,przj_Rjetud);
  fzj->GetObject("prRjets"  ,przj_Rjets);
  fzj->GetObject("prRjetc"  ,przj_Rjetc);
  TH1D* hzj_Rjet   = przj_Rjet  ->ProjectionX();
  TH1D* hzj_Rjetb  = przj_Rjetb ->ProjectionX();
  TH1D* hzj_Rjetg  = przj_Rjetg ->ProjectionX();
  TH1D* hzj_Rjetlq = przj_Rjetlq->ProjectionX();
  TH1D* hzj_Rjetud = przj_Rjetud->ProjectionX();
  TH1D* hzj_Rjets  = przj_Rjets ->ProjectionX();
  TH1D* hzj_Rjetc  = przj_Rjetc ->ProjectionX();

  //////////////////////////// GLUON DIFFERENCE ///////////////////////////////
  TH1D* h_diffbg  = (TH1D*)hzj_Rjetb->Clone("h_diffbg");
  TH1D* h_diffudg = (TH1D*)hzj_Rjetud->Clone("h_diffudg");
  TH1D* h_diffsg  = (TH1D*)hzj_Rjets->Clone("h_diffsg");
  TH1D* h_diffcg  = (TH1D*)hzj_Rjetc->Clone("h_diffcg");

  h_diffbg->Add(hzj_Rjetg,-1);  //b-response  - g-response
  h_diffudg->Add(hzj_Rjetg,-1); //ud-response - g-response
  h_diffsg->Add(hzj_Rjetg,-1);  //s-response  - g-response
  h_diffcg->Add(hzj_Rjetg,-1);  //c-response  - g-response

  TGraph *diff_b  = new TGraph("data_and_MC_input/Response/diffToGluon_b.txt" );
  TGraph *diff_s  = new TGraph("data_and_MC_input/Response/diffToGluon_s.txt" );
  TGraph *diff_c  = new TGraph("data_and_MC_input/Response/diffToGluon_c.txt" );
  TGraph *diff_ud = new TGraph("data_and_MC_input/Response/diffToGluon_ud.txt");
  diff_b ->SetMarkerStyle(kOpenCircle);       diff_b ->SetMarkerColor(kRed+1);
  diff_ud->SetMarkerStyle(kOpenDiamond);      diff_ud->SetMarkerColor(kMagenta+2);
  diff_s ->SetMarkerStyle(kOpenTriangleUp);   diff_s ->SetMarkerColor(kOrange+1);
  diff_c ->SetMarkerStyle(kOpenTriangleDown); diff_c ->SetMarkerColor(kGreen+2);

  h_diffbg ->SetMarkerStyle(kFullCircle);       h_diffbg ->SetMarkerColor(kRed+1);
  h_diffudg->SetMarkerStyle(kFullDiamond);      h_diffudg->SetMarkerColor(kMagenta+2);
  h_diffsg ->SetMarkerStyle(kFullTriangleUp);   h_diffsg ->SetMarkerColor(kOrange+1);
  h_diffcg ->SetMarkerStyle(kFullTriangleDown); h_diffcg ->SetMarkerColor(kGreen+2 );
  h_diffbg ->SetLineColor(kRed+1);        
  h_diffudg->SetLineColor(kMagenta+2);
  h_diffsg ->SetLineColor(kOrange+1);     
  h_diffcg ->SetLineColor(kGreen+2 );

  TCanvas* canv_diffg = new TCanvas("canv_diffg","",500,500);
  canv_diffg->SetLeftMargin(0.15);
  canv_diffg->SetBottomMargin(0.13);

  //Legend
  TLegend* lz_diffg = new TLegend(0.5,0.7,0.85,0.85);
  lz_diffg->SetNColumns(2);
  lz_diffg->SetBorderSize(0);
  lz_diffg->AddEntry(h_diffbg,           "b (Our)");
  lz_diffg->AddEntry(diff_b,  "b (FullSim)",  "p");
  lz_diffg->AddEntry(h_diffcg,           "c (Our)");
  lz_diffg->AddEntry(diff_c,  "c (FullSim)",  "p");
  lz_diffg->AddEntry(h_diffsg,           "s (Our)");
  lz_diffg->AddEntry(diff_s,  "s (FullSim)",  "p");
  lz_diffg->AddEntry(h_diffudg,         "ud (Our)");
  lz_diffg->AddEntry(diff_ud, "ud (Fullsim)", "p");

  h_diffbg->GetYaxis()->SetTitle("Difference to gluon response");
  h_diffbg->GetYaxis()->SetTitleSize(0.042);
  h_diffbg->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  h_diffbg->GetXaxis()->SetTitleSize(0.05);

  h_diffbg->GetYaxis()->SetTitleOffset(1.5);
  h_diffbg->GetXaxis()->SetTitleOffset(1.1);
  h_diffbg->GetXaxis()->SetMoreLogLabels();
  h_diffbg->GetXaxis()->SetNoExponent();

  gPad->SetTickx();   gPad->SetTicky();

  h_diffbg->SetStats(0); h_diffbg->SetTitle("");
  h_diffbg->SetAxisRange(-0.01,0.058,"Y"); //Vertical axis limits

  TLine *line = new TLine(31.75,0,1258.25,0); 

  h_diffbg->Draw("P");
  line->Draw("SAME");
  diff_ud->Draw("SAMEP");
  diff_b->Draw("SAMEP");
  diff_s->Draw("SAMEP");
  diff_c->Draw("SAMEP");
  h_diffudg->Draw("SAMEP");
  h_diffsg->Draw("SAMEP");
  h_diffcg->Draw("SAMEP");
  lz_diffg->Draw("SAMEP");
  canv_diffg->SetLogx();

  TLatex *tex = new TLatex(); tex->SetNDC();
  tex->SetTextSize(0.04); tex->SetTextColor(kBlack);
  tex->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.2,0.81,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.2,0.76,"#alpha<0.3");

  string plotName = "./plots/Rjet/gluonDiff_" + ReadName + ".pdf";
  canv_diffg->Print(plotName.c_str());
  delete canv_diffg;


  //Canvas
  TCanvas* canv_Rjet = new TCanvas("canv_Rjet","",500,500);
  canv_Rjet->SetLeftMargin(0.15);
  canv_Rjet->SetBottomMargin(0.13);

  hzj_Rjet  ->SetMarkerStyle(kOpenCircle);      hzj_Rjet  ->SetMarkerColor(kBlack);
  hzj_Rjetb ->SetMarkerStyle(kFullCircle);      hzj_Rjetb ->SetMarkerColor(kRed+1);
  hzj_Rjetg ->SetMarkerStyle(kFullSquare);      hzj_Rjetg ->SetMarkerColor(kBlue+1);
  hzj_Rjetud->SetMarkerStyle(kFullDiamond);     hzj_Rjetud->SetMarkerColor(kMagenta+2);
  hzj_Rjets->SetMarkerStyle(kFullTriangleUp);   hzj_Rjets->SetMarkerColor(kOrange+1);
  hzj_Rjetc->SetMarkerStyle(kFullTriangleDown); hzj_Rjetc->SetMarkerColor(kGreen+2);

  hzj_Rjet  ->SetLineColor(kBlack);              
  hzj_Rjetb ->SetLineColor(kRed+1);
  hzj_Rjetg ->SetLineColor(kBlue+1);             
  hzj_Rjetud->SetLineColor(kMagenta+2);
  hzj_Rjets ->SetLineColor(kOrange+1);            
  hzj_Rjetc ->SetLineColor(kGreen+2);

  //Legend
  TLegend* lz_Rjet = new TLegend(0.62,0.75,0.87,0.87);
  lz_Rjet->SetNColumns(2);
  lz_Rjet->SetBorderSize(0);
  lz_Rjet->AddEntry(hzj_Rjet,   "All",   "p");
  lz_Rjet->AddEntry(hzj_Rjetb,  "b",     "p");
  lz_Rjet->AddEntry(hzj_Rjetc,  "c",     "p");
  lz_Rjet->AddEntry(hzj_Rjets,  "s",     "p");
  lz_Rjet->AddEntry(hzj_Rjetud, "ud",    "p");
  lz_Rjet->AddEntry(hzj_Rjetg,  "gluon", "p");

  //Title and axis setup
  hzj_Rjet->SetStats(0); //Suppress stat box
  hzj_Rjet->SetTitle("");
  hzj_Rjet->SetAxisRange(0.912,0.955,"Y");
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
  hzj_Rjetb->Draw("SAMEP");
  hzj_Rjetg->Draw("SAMEP");
  hzj_Rjetud->Draw("SAMEP");
  hzj_Rjets->Draw("SAMEP");
  hzj_Rjetc->Draw("SAMEP");
  lz_Rjet->Draw("SAMEP");

  TLatex *tex2 = new TLatex(); tex2->SetNDC();
  tex2->SetTextSize(0.04); tex2->SetTextColor(kBlack);
  tex2->DrawLatex(0.7,0.92,"#sqrt{s}=13 TeV");
  tex2->DrawLatex(0.2,0.82,"|#eta^{probe}|<1.3");
  tex2->DrawLatex(0.2,0.77,"#alpha<0.3");


  plotName = "./plots/Rjet/Rjet_" + ReadName + ".pdf";
  canv_Rjet->Print(plotName.c_str()); delete canv_Rjet;
}

void CMSJES::plotF(int gen, int Nevt)
{
  //Choose filenames to open
  string nameAdd, zjetFile;
  plotQuery(nameAdd, zjetFile, gen, Nevt);

  //Initialize histograms and open ROOT files and fetch the stored objects
  TFile* fzj = TFile::Open(zjetFile.c_str());
  TProfile *przj_F=0; 
  TProfile *przj_Fb=0; 
  TProfile *przj_Fg=0; 
  TProfile *przj_Fud=0;
  TProfile *przj_Fs=0;
  TProfile *przj_Fc=0;

  //Create F Histograms
  fzj->GetObject("prRjet"   ,przj_F  );
  fzj->GetObject("prRjetb"  ,przj_Fb );
  fzj->GetObject("prRjetg"  ,przj_Fg );
  fzj->GetObject("prRjetud" ,przj_Fud);
  fzj->GetObject("prRjets"  ,przj_Fs);
  fzj->GetObject("prRjetc"  ,przj_Fc);
  TH1D* hzj_F   = przj_F->ProjectionX();
  TH1D* hzj_Fb  = przj_Fb->ProjectionX();
  TH1D* hzj_Fg  = przj_Fg->ProjectionX();
  TH1D* hzj_Fud = przj_Fud->ProjectionX();
  TH1D* hzj_Fs  = przj_Fs->ProjectionX();
  TH1D* hzj_Fc  = przj_Fc->ProjectionX();

  hzj_Fb->Divide(hzj_F);
  hzj_Fg->Divide(hzj_F);
  hzj_Fud->Divide(hzj_F);
  hzj_Fs->Divide(hzj_F);
  hzj_Fc->Divide(hzj_F);

  //Canvas
  TCanvas* canv_F = new TCanvas("canv_F","",500,400);
  canv_F->SetLeftMargin(0.15);
  canv_F->SetBottomMargin(0.13);

  hzj_Fb ->SetMarkerStyle(kFullCircle);      hzj_Fb ->SetMarkerColor(kRed+1);
  hzj_Fg ->SetMarkerStyle(kFullSquare);      hzj_Fg ->SetMarkerColor(kBlue+1);
  hzj_Fud->SetMarkerStyle(kFullDiamond);     hzj_Fud->SetMarkerColor(kMagenta+2);
  hzj_Fs->SetMarkerStyle(kFullTriangleUp);   hzj_Fs->SetMarkerColor(kOrange+1);
  hzj_Fc->SetMarkerStyle(kFullTriangleDown); hzj_Fc->SetMarkerColor(kGreen+2);

  hzj_Fb ->SetLineColor(kRed+1);              
  hzj_Fg ->SetLineColor(kBlue+1);
  hzj_Fud->SetLineColor(kMagenta+2);
  hzj_Fs->SetLineColor(kOrange+1);
  hzj_Fc->SetLineColor(kGreen+2);

  double markersize = 0.8;
  hzj_Fb ->SetMarkerSize(markersize);              
  hzj_Fg ->SetMarkerSize(markersize);
  hzj_Fud->SetMarkerSize(markersize);
  hzj_Fs->SetMarkerSize(markersize);
  hzj_Fc->SetMarkerSize(markersize);

  //Legend
  TLegend* lz_F = new TLegend(0.62,0.65,0.8,0.88);
  lz_F->SetBorderSize(0);
  lz_F->AddEntry(hzj_Fb,  "b",     "p");
  lz_F->AddEntry(hzj_Fc,  "c",     "p");
  lz_F->AddEntry(hzj_Fs,  "s",     "p");
  lz_F->AddEntry(hzj_Fud, "ud",    "p");
  lz_F->AddEntry(hzj_Fg,  "gluon", "p");

  //Title and axis setup
  hzj_Fb->SetStats(0); //Suppress stat box
  hzj_Fb->SetTitle("");
  hzj_Fb->SetAxisRange(0.98,1.02,"Y"); //Vertical axis limits

  hzj_Fb->GetYaxis()->SetTitleOffset(1.1);
  hzj_Fb->GetXaxis()->SetTitleOffset(1.1);
  hzj_Fb->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  hzj_Fb->GetXaxis()->SetTitleSize(0.05);
  hzj_Fb->GetYaxis()->SetTitle("F");
  hzj_Fb->GetYaxis()->SetTitleSize(0.05);

  hzj_Fb->GetXaxis()->SetMoreLogLabels();
  hzj_Fb->GetXaxis()->SetNoExponent();
  canv_F->SetLogx();
  gPad->SetTickx(); gPad->SetTicky();

  TLine *line = new TLine(31.75,1,1258.25,1); 

  //Plot
  hzj_Fb->Draw("P");
  line->Draw("SAME");
  hzj_Fg->Draw("SAMEP");
  hzj_Fud->Draw("SAMEP");
  hzj_Fs->Draw("SAMEP");
  hzj_Fc->Draw("SAMEP");
  lz_F->Draw("SAMEP");

  TLatex *tex = new TLatex(); tex->SetNDC();
  tex->SetTextSize(0.04); tex->SetTextColor(kBlack);
  tex->DrawLatex(0.75,0.92,"#sqrt{s}=13 TeV");
  tex->DrawLatex(0.2,0.82,"|#eta^{probe}|<1.3");
  tex->DrawLatex(0.2,0.77,"#alpha<0.3");

  //Save plot
  string plotName = "./plots/F/F_" + ReadName + ".pdf";
  canv_F->Print(plotName.c_str()); delete canv_F;
}

void CMSJES::plotVarSPR(int gen, int Nevt)
{

  string outname = "./output_ROOT_files/varPlots_5000000"; //Output file
  outname += ".root";
  TFile *foutvar = new TFile(outname.c_str(),"RECREATE");

  // SPR +-3% JEC paper
  TGraph *JEC_SPRp3      = new TGraph("data_and_MC_input/Response/JEC_PFJet_SPRplus3.txt" );
  TGraph *JEC_SPRm3      = new TGraph("data_and_MC_input/Response/JEC_PFJet_SPRminus3.txt" );
  TGraph *JEC_calo_SPRp3 = new TGraph("data_and_MC_input/Response/JEC_Calo_SPRplus3.txt" );
  TGraph *JEC_calo_SPRm3 = new TGraph("data_and_MC_input/Response/JEC_Calo_SPRminus3.txt" );

  JEC_SPRp3->SetMarkerStyle(kOpenCircle);            JEC_SPRp3->SetMarkerColor(kRed);
  JEC_SPRm3->SetMarkerStyle(kOpenSquare);            JEC_SPRm3->SetMarkerColor(kBlue+1);
  JEC_calo_SPRp3->SetMarkerStyle(kOpenTriangleUp);   JEC_calo_SPRp3->SetMarkerColor(kOrange+1);
  JEC_calo_SPRm3->SetMarkerStyle(kOpenTriangleDown); JEC_calo_SPRm3->SetMarkerColor(kGreen+2);

  double markersize = 0.8;
  JEC_SPRp3->SetMarkerSize(markersize);
  JEC_SPRm3->SetMarkerSize(markersize);
  JEC_calo_SPRp3->SetMarkerSize(markersize);
  JEC_calo_SPRm3->SetMarkerSize(markersize);

  TFile* fzj     = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_5000000.root");
  TFile* fzj_Cp3 = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_5000000_varCp3.root");
  TFile* fzj_Cm3 = TFile::Open("output_ROOT_files/CMSJES_P8_Zjet_5000000_varCm3.root");

  TProfile *pr_Rjet = 0;
  TProfile *pr_Rjet_b = 0; 
  TProfile *pr_Rjet_Cp3 = 0;
  TProfile *pr_Rjet_Cp3_b = 0;
  TProfile *pr_Rjet_Cm3 = 0;
  TProfile *pr_Rjet_Cm3_b = 0;
  TProfile *pr_Rjet_calo = 0;
  TProfile *pr_Rjet_calo_Cp3 = 0;
  TProfile *pr_Rjet_calo_Cm3 = 0;

  //Create Histograms
  fzj    ->GetObject("prRjet", pr_Rjet);
  fzj    ->GetObject("prRjetb",pr_Rjet_b);
  fzj_Cp3->GetObject("prRjet", pr_Rjet_Cp3);
  fzj_Cp3->GetObject("prRjetb", pr_Rjet_Cp3_b);
  fzj_Cm3->GetObject("prRjet", pr_Rjet_Cm3);
  fzj_Cm3->GetObject("prRjetb", pr_Rjet_Cm3_b);

  fzj    ->GetObject("prRjet_calo",pr_Rjet_calo);
  fzj_Cp3->GetObject("prRjet_calo",pr_Rjet_calo_Cp3);
  fzj_Cm3->GetObject("prRjet_calo",pr_Rjet_calo_Cm3);

  TH1D* h_Rjet       = pr_Rjet->ProjectionX();
  TH1D* h_Rjet_b     = pr_Rjet_b->ProjectionX();
  TH1D* h_Rjet_Cp3   = pr_Rjet_Cp3->ProjectionX();
  TH1D* h_Rjet_Cp3_b = pr_Rjet_Cp3_b->ProjectionX();
  TH1D* h_Rjet_Cm3   = pr_Rjet_Cm3->ProjectionX();
  TH1D* h_Rjet_Cm3_b = pr_Rjet_Cm3_b->ProjectionX();

  TH1D* h_Rjet_calo     = pr_Rjet_calo->ProjectionX();
  TH1D* h_Rjet_calo_Cp3 = pr_Rjet_calo_Cp3->ProjectionX();
  TH1D* h_Rjet_calo_Cm3 = pr_Rjet_calo_Cm3->ProjectionX();

  h_Rjet_Cp3->Divide(h_Rjet);
  h_Rjet_Cp3_b->Divide(h_Rjet_b);
  h_Rjet_Cm3->Divide(h_Rjet);
  h_Rjet_Cm3_b->Divide(h_Rjet_b);
  h_Rjet_calo_Cp3->Divide(h_Rjet_calo);
  h_Rjet_calo_Cm3->Divide(h_Rjet_calo);

  //Canvas
  TCanvas* canv_var = new TCanvas("canv_var","",500,400);
  canv_var->SetLeftMargin(0.15);
  canv_var->SetBottomMargin(0.13);

  h_Rjet_Cp3->SetMarkerStyle(kFullCircle); h_Rjet_Cp3->SetMarkerColor(kRed);
  h_Rjet_Cm3->SetMarkerStyle(kFullSquare); h_Rjet_Cm3->SetMarkerColor(kBlue+1);
  h_Rjet_Cp3->SetLineColor(kRed);          h_Rjet_Cm3->SetLineColor(kBlue+1); 

  h_Rjet_calo_Cp3->SetMarkerStyle(kFullTriangleUp);   h_Rjet_calo_Cp3->SetMarkerColor(kOrange+1);
  h_Rjet_calo_Cm3->SetMarkerStyle(kFullTriangleDown); h_Rjet_calo_Cm3->SetMarkerColor(kGreen+2);
  h_Rjet_calo_Cp3->SetLineColor(kOrange+1);           h_Rjet_calo_Cm3->SetLineColor(kGreen+2); 

  h_Rjet_Cp3->SetMarkerSize(markersize);        h_Rjet_Cm3->SetMarkerSize(markersize);
  h_Rjet_calo_Cp3->SetMarkerSize(markersize);   h_Rjet_calo_Cm3->SetMarkerSize(markersize);

  //Legend
  TLegend* lz_Rjet = new TLegend(0.18,0.67,0.88,0.89);
  lz_Rjet->SetNColumns(2);
  lz_Rjet->SetBorderSize(0);
  lz_Rjet->AddEntry(h_Rjet_Cp3,      "Reco jet C+3% (Our)",   "p");
  lz_Rjet->AddEntry(h_Rjet_calo_Cp3, "Calo jet C+3% (Our)",   "p");
  lz_Rjet->AddEntry(h_Rjet_Cm3,      "Reco jet C-3% (Our)",   "p");
  lz_Rjet->AddEntry(h_Rjet_calo_Cm3, "Calo jet C-3% (Our)",   "p");
  lz_Rjet->AddEntry(JEC_SPRp3,       "Reco jet C+3% (FastSim)", "p");
  lz_Rjet->AddEntry(JEC_calo_SPRp3,  "Calo jet C+3% (FastSim)", "p");
  lz_Rjet->AddEntry(JEC_SPRm3,       "Reco jet C-3% (FastSim)", "p");  
  lz_Rjet->AddEntry(JEC_calo_SPRm3,  "Calo jet C-3% (FastSim)", "p"); 


  TH1D* setup = new TH1D("setup","", h_Rjet_Cp3->GetXaxis()->GetNbins(),
			 h_Rjet_Cp3->GetXaxis()->GetXmin(), h_Rjet_Cp3->GetXaxis()->GetXmax());

  //Title and axis setup
  setup->SetStats(0); //Suppress stat box
  setup->SetTitle("");
  setup->SetAxisRange(0.97,1.05,"Y"); //Vertical axis limits
  setup->GetXaxis()->SetMoreLogLabels();
  setup->GetXaxis()->SetNoExponent();
  canv_var->SetLogx();
  setup->GetYaxis()->SetTitleOffset(1.1);
  setup->GetXaxis()->SetTitleOffset(1.1);
  setup->GetYaxis()->SetTitle("F^{var}");
  setup->GetYaxis()->SetTitleSize(0.05);
  setup->GetXaxis()->SetTitle("p_{T,jet}^{gen} (GeV)");
  setup->GetXaxis()->SetTitleSize(0.05);

  gPad->SetTickx(); gPad->SetTicky();

  //Savefile name setup
  string savename = "./plots/varPlots/varSPR";
  savename+=".eps";

  //Plot
  setup->Draw();
  JEC_calo_SPRp3->Draw("SAMEP");
  JEC_SPRp3->Draw("SAMEP");
  JEC_SPRm3->Draw("SAMEP");
  JEC_calo_SPRm3->Draw("SAMEP");
  h_Rjet_calo_Cp3->Draw("SAMEP");
  h_Rjet_calo_Cm3->Draw("SAMEP");
  h_Rjet_Cm3->Draw("SAMEP");
  h_Rjet_Cp3->Draw("SAMEP");
  lz_Rjet->Draw("SAMEP");

  //Save plot
  canv_var->Print(savename.c_str());

  delete canv_var;

  foutvar->Write();
  foutvar->Close();

}

//-----------------------------------------------------------------------------
//User interface to choose which files to open in plotting functions
//Params:	nameAdd		Additions to the standard CMSJES output names
//		djstr, gjstr	Set dijet & gammajet all flavor filenames here
//		gen,alg,rad,...
//		ct,XS	Preset values if user interface omitted
//N.B. only the interesting cases are enabled in this function ATM
void CMSJES::plotQuery(string& nameAdd, string& zjstr, int& gen, int& Nevt)
{
  //Init
  string respStr = "./output_ROOT_files/CMSJES_";
  string root = ".root";

  /* User interface */
  //Choose generator
  cout << "(1) H7 (2) P8" << endl;
  while (gen<1 || gen>2) cin >> gen;
  respStr += (gen==2 ? "P8_": (gen==1 ? "H7_": ""));

  //Set #events
  string num;
  cout << "#Events (0) 3k (1) 10k (2) 30k (3) 100k (4) 600k (5) 1M (6) 5M (7) 10M" << endl;
  while (Nevt<0 || Nevt>7) cin >> Nevt;
  if      (Nevt==0) num = "3000";
  else if (Nevt==1) num = "10000";
  else if (Nevt==2) num = "30000";
  else if (Nevt==3) num = "100000";
  else if (Nevt==4) num = "600000";
  else if (Nevt==5) num = "1000000";
  else if (Nevt==6) num = "5000000";
  else if (Nevt==7) num = "10000000";

  //Construct all-flavor filenames
  zjstr = respStr + "Zjet_" + num + root;

  //Additions in filename
  nameAdd = num;
} //plotQuery


//Phi value where particle hits the calorimeter
double CMSJES::trackDeltaPhi(int pdgid, double phi, double pT, double Rt, double Bfield) {
  
  double Rp = pT/(0.3*Bfield);
  double dPhi;
  double newPhi;

  //What is the correct sign here?
  dPhi = Charge(pdgid)*(TMath::Pi()/2 - TMath::ACos(Rt/(2*Rp)));

  newPhi = phi + dPhi;

  if ( newPhi > TMath::Pi() )       newPhi -= TMath::Pi();
  else if ( newPhi < -TMath::Pi() ) newPhi += TMath::Pi();

  return newPhi;
}


int CMSJES::Charge(int pdgid) {
  int charge;

  switch (pdgid) {
    //Neutral
    case    12 ://e-neutrino
    case   -12 :
    case    14 ://mu-neutrino
    case   -14 :
    case    16 ://tau-neutrino
    case   -16 :
    case    20 ://photon
    case    22 ://photon from pi0
    case   130 ://K_L0
    case   310 ://K_S0
    case  3122 ://Lambda0
    case -3122 ://anti-Lambda0
    case  2112 ://n
    case -2112 ://anti-n
    case  3212 ://Sigma0
    case -3212 ://anti-Sigma0
    case  3322 ://Xi0
    case -3322 ://anti-Xi0
      charge = 0;
      break;

    //Positive charge
    case   -11 ://e+
    case   -13 ://mu+
    case   211 ://pi+
    case   321 ://K+
    case  2212 ://p+
    case -3112 ://anti-Sigma+
    case  3222 ://Sigma+
    case -3312 ://anti-Xi+
    case -3334 ://anti-Omega+
      charge = 1;
      break;  
  
    //Negative charge
    case    11 ://e-
    case    13 ://mu-
    case  -211 ://pi-
    case  -321 ://K-
    case -2212 ://anti-p
    case  3112 ://Sigma-
    case -3222 ://anti-Sigma-
    case  3312 ://Xi-
    case  3334 ://Omega-
      charge = -1;
      break;
    default : 
         cout << "Unknown particle: " << pdgid << endl;
  }
  return charge;
}


bool CMSJES::isChHadron(int pdgid) {
  bool isChH;
  int PDG = fabs(pdgid);

  switch (PDG) {
    case  211 ://pi+-
    case  321 ://K+-
    case  2212 ://p
    case  3112 ://Sigma-
    case  3222 ://Sigma+
    case  3312 ://Xi+-
    case  3334 ://Omega+-
      isChH = 1;
      break;
    default : 
      isChH = 0;
  }
  return isChH;
}

bool CMSJES::doesReachEcal(int pdgid, double pT, double B, double Rt) {
  double Rp = pT/(0.3*B);

  if (!fabs(Charge(pdgid))) return true;

  if (Rp > 0.5*Rt) return true;
  else return false;
}








