#include <sstream>

void extractBranches() {

  int temp=-1;
  bool NIJ = false;	//Saving also particles not in jets (NIJ)?

  /* Specify event type of the file to convert */

  //Generator prefix
  cout << "(1) pjets_pythia6 (2) pjets_herwig7" << endl;
  while (temp!=1 && temp!=2) cin >> temp;
  string gen="";
  if      (temp==1) gen = "pjets_pythia6_";	//TODO add P8 functionality
  else if (temp==2) gen = "pjets_herwig7_";

  //Sample type
  temp=-1;	//Reinit
  cout << "(1) dijet (2) gammajet" << endl;
  while (temp!=1 && temp !=2) cin >> temp;
  string mode="";
  if      (temp==1) mode = "dijet_";
  else if (temp==2) mode = "gammajet_";
  else {
    cout << "ERROR: Unknown mode" << endl;
    return;
  }

  //#Events in the file
  cout << "#Events? (1) 1M (2) 2M (3) 30k (4) 1000 (5) 80" << endl;
  temp=-1;	//Reinit
  while (temp<1 || temp >5) cin >> temp;
  int n_evts = 1000000;	//#Events in the file
  if      (temp==2) n_evts = 2000000;
  else if (temp==3) n_evts = 30000;
  else if (temp==4) n_evts = 1000;
  else if (temp==5) n_evts = 80;

  /* Jet algorithm */
  cout << "Jet algorithm (1) a-kT (2) D0rIIc" << endl;
  string jetAlg = "a-kT_";
  temp=-1;	//Reinit
  while (temp!=1 && temp!=2 && temp!=3) cin >> temp;
  if (temp==2) jetAlg = "D0rIIc_";
  else if (temp==3) jetAlg = "";

  /* Cone size */
  cout << "R_cone (1) 0.5 (2) 0.7" << endl;
  temp=-1;	//Reinit
  while (temp!=1 && temp!=2 && temp!=3) cin >> temp;
  if      (temp==1) jetAlg += "R05_";
  else if (temp==2) jetAlg += "R07_";
  else if (temp==3) jetAlg += "";

  /* Length scale for decays */
  cout << "ctau (0) N/A (1) 3mm (2) 10mm" << endl;
  temp=-1;	//Reinit
  while (temp<0 || temp>3) cin >> temp;
  string ctau="";
  if      (temp==1) ctau="ct3mm_";
  else if (temp==2) ctau="ct10mm_";
  else if (temp==3) ctau="";

  /* b-enriched */
  cout << "b-enriched? (1) No (2) Yes" << endl;
  temp=-1;	//Reinit
  string bEnr="";
  while (temp!=1 && temp!=2) cin >> temp;
  if (temp==2) bEnr="b-enriched_";

  //Construct input filename
  stringstream instream;
  instream << gen << mode << jetAlg << ctau << bEnr << n_evts << ".root";

  cout << "\nConverting file:\n" << instream.str() << endl;

  //Open input file and fetch the TTree in it	TODO add P8 functionality
  TFile* file_in = new TFile(instream.str().c_str());
  string treeStr = "Pythia6Jets";
  if (gen.find("herwig")!=string::npos) treeStr = "HerwigTree";
  TTree* tree_in = (TTree*)file_in->Get(treeStr.c_str());

  //Deactivate all branches
  tree_in->SetBranchStatus("*",0);

  //For possibly instantiating new empty branches for particles not associated 
  //with jets ("not-in-jets"=NIJ). It is handy that the branches exist even if
  //they  are made empty, so we may use the same analysis codes under 
  //  /gen_handle/responseAnalysis/
  //that assumes the existence of a given list of branches.
  TBranch *b_ID; TBranch *b_pt; TBranch *b_eta; TBranch *b_phi; TBranch *b_e;
  float dum;

  //Reactivate selected branches
  tree_in->SetBranchStatus("weight",          1);
  tree_in->SetBranchStatus("prtcl_jet",       1);	//Particles in jets
  tree_in->SetBranchStatus("prtcl_pdgid",     1);
  tree_in->SetBranchStatus("prtcl_pt",        1);
  tree_in->SetBranchStatus("prtcl_eta",       1);
  tree_in->SetBranchStatus("prtcl_phi",       1);
  tree_in->SetBranchStatus("prtcl_e",         1);
  if (NIJ) {
    tree_in->SetBranchStatus("prtclnij_pdgid",  1);	//Particles not in jets
    tree_in->SetBranchStatus("prtclnij_pt",     1);
    tree_in->SetBranchStatus("prtclnij_eta",    1);
    tree_in->SetBranchStatus("prtclnij_phi",    1);
    tree_in->SetBranchStatus("prtclnij_e",      1);
  }
  tree_in->SetBranchStatus("prtn_jet",        1);	//Partons
  tree_in->SetBranchStatus("prtn_pdgid",      1);
  tree_in->SetBranchStatus("prtn_tag",        1);
  tree_in->SetBranchStatus("prtn_pt",         1);
  tree_in->SetBranchStatus("prtn_eta",        1);
  tree_in->SetBranchStatus("prtn_phi",        1);
  tree_in->SetBranchStatus("prtn_e",          1);
  tree_in->SetBranchStatus("prtn_dr",         1);
  tree_in->SetBranchStatus("jet_pt",          1);	//Gen jets
  tree_in->SetBranchStatus("jet_eta",         1);
  tree_in->SetBranchStatus("jet_phi",         1);
  tree_in->SetBranchStatus("jet_e",           1);
  tree_in->SetBranchStatus("jet_constituents",1);
  tree_in->SetBranchStatus("jet_ptd",         1);
  tree_in->SetBranchStatus("jet_sigma2",      1);
  tree_in->SetBranchStatus("met",             1);	//Neutrino E_T sum

  //New file and TTree, copy the chosen branches
  stringstream outstream;
  string genOut = "P6_";	//TODO add P8 functionality
  if (gen.find("herwig")!=string::npos) genOut = "H7_";
  outstream << genOut << mode << jetAlg << ctau << bEnr << n_evts << ".root";
  TFile* file_out = new TFile(outstream.str().c_str(),"recreate");
  TTree* tree_out = tree_in->CloneTree(0);
  tree_out->CopyEntries(tree_in);

  //Output and file saving
  tree_out->Print();  file_out->Write();

  //Free memory
  delete file_in;     delete file_out;

}

