#include <iostream>
#include <fstream>

/* Reading R^MC params from files, find averages weighted by int. luminosity */

void printLaTeXtable() {

  vector<string> fileNames;

  //string dir = "./RunIIa/";
  string dir = "./RunIIb1/";
  //string dir = "./RunIIb1-P20ToP17/";

  fileNames.push_back(dir + "electron.txt");
  fileNames.push_back(dir + "kaon.txt"    );
  fileNames.push_back(dir + "klong.txt"   );
  fileNames.push_back(dir + "kshort.txt"  );
  fileNames.push_back(dir + "lambda.txt"  );
  fileNames.push_back(dir + "muon.txt"    );
  fileNames.push_back(dir + "neutron.txt" );
  fileNames.push_back(dir + "photon.txt"  );
  fileNames.push_back(dir + "pion.txt"    );
  fileNames.push_back(dir + "proton.txt"  );


  double p0=0.0, p1=0.0, p2=0.0, p3=0.0, p4=0.0, p5=0.0;  //Temps to read into

  for (int i=0; i!=fileNames.size(); ++i) {

    //Print the start of LaTeX table commands
    cout << "\\begin{table}[H]"                                         << endl;
    cout << "\\centering"                                               << endl;
    cout << "\\caption{";
    if      (dir=="./RunIIa/")           cout << "RunIIa";
    else if (dir=="./RunIIb1/")          cout << "RunIIb";
    else if (dir=="./RunIIb1-P20ToP17/") cout << "RunIIb-P20ToP17";
    cout << " parameters for ";
    if      (fileNames[i].find("electron")!=std::string::npos) cout<<"$e^\\pm$";
    else if (fileNames[i].find("kaon")!=std::string::npos) cout<<"$K^\\pm$";
    else if (fileNames[i].find("klong")!=std::string::npos) cout<<"$K^0_L$";
    else if (fileNames[i].find("kshort")!=std::string::npos) cout<<"$K^0_S$";
    else if (fileNames[i].find("lambda")!=std::string::npos) cout<<"$\\Lambda$";
    else if (fileNames[i].find("muon")!=std::string::npos) cout<<"$\\mu^\\pm$";
    else if (fileNames[i].find("neutron")!=std::string::npos) cout<<"$n$";
    else if (fileNames[i].find("photon")!=std::string::npos) cout<<"$\\gamma$";
    else if (fileNames[i].find("pion")!=std::string::npos) cout<<"$\\pi^\\pm$";
    else if (fileNames[i].find("proton")!=std::string::npos) cout<<"$p$";
    cout << ".}" << endl;
    cout << "\\label{";
    if      (fileNames[i].find("electron")!=std::string::npos) cout << "e";
    else if (fileNames[i].find("kaon"    )!=std::string::npos) cout << "K";
    else if (fileNames[i].find("klong"   )!=std::string::npos) cout << "KL";
    else if (fileNames[i].find("kshort"  )!=std::string::npos) cout << "KS";
    else if (fileNames[i].find("lambda"  )!=std::string::npos) cout << "Lambda";
    else if (fileNames[i].find("muon"    )!=std::string::npos) cout << "mu";
    else if (fileNames[i].find("neutron" )!=std::string::npos) cout << "n";
    else if (fileNames[i].find("photon"  )!=std::string::npos) cout << "gamma";
    else if (fileNames[i].find("pion"    )!=std::string::npos) cout << "pi";
    else if (fileNames[i].find("proton"  )!=std::string::npos) cout << "p";
    cout << "_table_";
    if      (dir=="./RunIIa/")           cout << "a";
    else if (dir=="./RunIIb1/")          cout << "b";
    else if (dir=="./RunIIb1-P20ToP17/") cout << "b-P20ToP17";
    cout << "}" << endl;
    if        (fileNames[i].find("electron")!=std::string::npos) {
      cout << "\\begin{tabular}{c l l l l l}"                           <<endl;
      cout << "$|\\eta|$ region & $p^{(0)}$  & $p^{(1)}$"
           << "  & $p^{(2)}$ & $p^{(3)}$  & $p^{(4)}$\\\\"              <<endl;
    } else if (fileNames[i].find("muon"    )!=std::string::npos) {
      cout << "\\begin{tabular}{c l l l l}"                             <<endl;
      cout << "$|\\eta|$ region & $p^{(0)}$  & $p^{(1)}$"
           << "  & $p^{(2)}$ & $p^{(3)}$\\\\"                           <<endl;
    } else if (fileNames[i].find("photon"  )!=std::string::npos) {
      cout << "\\begin{tabular}{c l l l l l l}"                         <<endl;
      cout << "$|\\eta|$ region &$p^{(0)}$ &$p^{(1)}$ &$p^{(2)}$"
           << " &$p^{(3)}$ &$p^{(4)}$ &$p^{(5)}$\\\\"                   <<endl;
    } else {
      cout<<"\\begin{tabular}{c l l l}"                                 <<endl;
      cout<<"$|\\eta|$ region & $p^{(0)}$  & $p^{(1)}$  & $p^{(2)}$\\\\"<<endl;
    }
    cout << "\\hline" << endl;

    //Open file for reading
    ifstream in;
    in.open(fileNames[i]);
    if (!in.is_open()) {cout << "Error opening file!" << endl;  return;}

    //Read the parameters
    for (double etaMin=0.0; etaMin < 1.0; etaMin+=0.1) {
      //1st string on line is eta region low. bd. so read p0 2x to get the param.
      if (fileNames[i].find("photon")!=std::string::npos) {
        in >>p0>>p0>>p1>>p2>>p3>>p4>>p5;
        cout << "(";
        if      (etaMin==0.0         ) cout << "0.0";
        else if (int(10.1*etaMin)==10) cout << "1.0";
        else if (int(10.1*etaMin)==20) cout << "2.0";
        else if (int(10.1*etaMin)==30) cout << "3.0";
        else cout<< etaMin;
        cout << ", ";
        if      (int(10.1*(etaMin+0.1))==10) cout << "1.0";
        else if (int(10.1*(etaMin+0.1))==20) cout << "2.0";
        else if (int(10.1*(etaMin+0.1))==30) cout << "3.0";
        else cout << etaMin+0.1;
        cout << ")  & "
             << p0 << "  & " << p1 << "  & " << p2 << "  & "
             << p3 << "  & " << p4 << "  & " << p5;
      } else if (fileNames[i].find("electron")!=std::string::npos) {
        in >>p0>>p0>>p1>>p2>>p3>>p4;
        cout << "(";
        if      (etaMin==0.0         ) cout << "0.0";
        else if (int(10.1*etaMin)==10) cout << "1.0";
        else if (int(10.1*etaMin)==20) cout << "2.0";
        else if (int(10.1*etaMin)==30) cout << "3.0";
        else cout<< etaMin;
        cout << ", ";
        if      (int(10.1*(etaMin+0.1))==10) cout << "1.0";
        else if (int(10.1*(etaMin+0.1))==20) cout << "2.0";
        else if (int(10.1*(etaMin+0.1))==30) cout << "3.0";
        else cout << etaMin+0.1;
        cout << ")  & "
             << p0 << "  & " << p1 << "  & " << p2 << "  & "
             << p3 << "  & " << p4;
      } else if (fileNames[i].find("muon")!=std::string::npos) {
        in >>p0>>p0>>p1>>p2>>p3;
        cout << "(";
        if      (etaMin==0.0         ) cout << "0.0";
        else if (int(10.1*etaMin)==10) cout << "1.0";
        else if (int(10.1*etaMin)==20) cout << "2.0";
        else if (int(10.1*etaMin)==30) cout << "3.0";
        else cout<< etaMin;
        cout << ", ";
        if      (int(10.1*(etaMin+0.1))==10) cout << "1.0";
        else if (int(10.1*(etaMin+0.1))==20) cout << "2.0";
        else if (int(10.1*(etaMin+0.1))==30) cout << "3.0";
        else cout << etaMin+0.1;
        cout << ")  & "
             << p0 << "  & " << p1 << "  & " << p2 << "  & " << p3;
      } else {
        in >>p0>>p0>>p1>>p2;
        cout << "(";
        if      (etaMin==0.0         ) cout << "0.0";
        else if (int(10.1*etaMin)==10) cout << "1.0";
        else if (int(10.1*etaMin)==20) cout << "2.0";
        else if (int(10.1*etaMin)==30) cout << "3.0";
        else cout<< etaMin;
        cout << ", ";
        if      (int(10.1*(etaMin+0.1))==10) cout << "1.0";
        else if (int(10.1*(etaMin+0.1))==20) cout << "2.0";
        else if (int(10.1*(etaMin+0.1))==30) cout << "3.0";
        else cout << etaMin+0.1;
        cout << ")  & "
             << p0 <<"  & "<< p1 <<"  & "<< p2;
      }
      if (etaMin<3.1) cout << "  \\\\" << endl;
      else            cout << endl;
    } //Loop |eta| regions

    //Print the end of LaTeX table commands
    cout << "\\end{tabular}" << endl;
    cout << "\\end{table}\n"   << endl;

    in.close();

  } //Loop files
}

