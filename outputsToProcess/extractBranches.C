#include <sstream>

void extractBranches() {

  int temp=-1;
  bool NIJ = true;	//Saving also particles not in jets (NIJ)?

  /* Specify event type of the file to convert */

  //Generator prefix
  cout << "(1) pjets_pythia6 (2) pjets_herwig7 (3) pjets_pythia8" << endl;
  while (temp!=1 && temp!=2 && temp!=3) cin >> temp;
  string gen="";
  if      (temp==1) gen = "pjets_pythia6_";
  else if (temp==2) gen = "pjets_herwig7_";
  else if (temp==3) gen = "pjets_pythia8_";

  //Sample type
  temp=-1;	//Reinit
  cout << "(1) dijet (2) gammajet (3) Zjet" << endl;
  while (temp!=1 && temp!=2 && temp!=3) cin >> temp;
  string mode="";
  if      (temp==1) mode = "dijet_";
  else if (temp==2) mode = "gammajet_";
  else if (temp==3) mode = "Zjet_";
  else {
    cout << "ERROR: Unknown mode" << endl;
    return;
  }

  //#Events in the file
  cout << "#Events? (1) 1M (2) 500k (3) 100k (4) 10k (5) 1k (6) 30k" << endl;
  temp=-1;	//Reinit
  while (temp<1 || temp >7) cin >> temp;
  int n_evts = 1000000;	//#Events in the file
  if      (temp==2) n_evts = 500000;
  else if (temp==3) n_evts = 100000;
  else if (temp==4) n_evts = 10000;
  else if (temp==5) n_evts = 1000;
  else if (temp==6) n_evts = 30000;
  else if (temp==7) n_evts = 300000;


  //Construct input filename
  stringstream instream;
  instream << gen << mode << n_evts << ".root";

  cout << "\nConverting file:\n" << instream.str() << endl;

  //Open input file and fetch the TTree in it
  TFile* file_in = new TFile(instream.str().c_str());
  string treeStr = "Pythia6Jets";
  if (gen.find("pythia8")!=string::npos) treeStr = "Pythia8Jets";
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

  tree_in->SetBranchStatus("jet_pt",          1);	//Gen jets
  tree_in->SetBranchStatus("jet_eta",         1);
  tree_in->SetBranchStatus("jet_phi",         1);
  tree_in->SetBranchStatus("jet_e",           1);
  tree_in->SetBranchStatus("met",             1);	//Neutrino E_T sum

  //New file and TTree, copy the chosen branches
  stringstream outstream;
  string genOut = "P6_";
  if (gen.find("pythia8")!=string::npos) genOut = "P8_";
  if (gen.find("herwig")!=string::npos) genOut = "H7_";
  outstream << genOut << mode << n_evts << ".root";
  TFile* file_out = new TFile(outstream.str().c_str(),"recreate");
  TTree* tree_out = tree_in->CloneTree(0);
  tree_out->CopyEntries(tree_in);

  //Output and file saving
  tree_out->Print();  file_out->Write();

  //Free memory
  delete file_in;     delete file_out;

}

