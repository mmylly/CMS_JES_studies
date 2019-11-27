

void weighting() {

  const int nrows = 7; //number of particles
  const double P[nrows]  = {10. , 100., 200., 200., 500., 800., 1000.}; //Particle momenta

  double E = 100000.; // Calo deposits
  double P_our = 0.;

  TMatrixTSym<double> a(nrows);
  TVectorD b(nrows);
  double Ptot = 0.0;   double Ptot_rescale = 0.0;
  double CaloReso = 0.0;
  double sigma2E = 0.0;
  double sigma2i = 0.0;
  double dP[nrows];
  double dPtot = 0.0;

  TF1* TrackResolution = new TF1("", "4.66e-07*pow(x,2) + 0.0001032*x + 0.008076", 0, 5000);

  for (int i = 0; i < nrows; i++) {
    Ptot += P[i];
    dP[i] = TrackResolution->Eval(P[i])*P[i]; //Absolute trk resolution
    dPtot += pow(dP[i],2);
    cout << dP[i] << endl;
  }

  CaloReso = sqrt(1.02*1.02/Ptot + 0.065*0.065)*Ptot; //Absolute calo resolution
  sigma2E = CaloReso*CaloReso;

  for (int i = 0; i < nrows; i++) {

    sigma2i = dP[i]*dP[i]; //trkReso^2

    a(i, i) =   1. / sigma2i + 1. / sigma2E;
    b(i)    = P[i] / sigma2i + E  / sigma2E;

    for (int j = 0; j < nrows; j++) {
      if (i == j) continue;
      a(i, j) = 1./sigma2E;
    }
  }

  // solve ax = b
  TDecompChol decomp(a);
  bool ok = false;
  TVectorD x = decomp.Solve(b, ok);

  //a.Print();
  //x.Print();
  //b.Print();
  if (ok) {
    for (int i = 0; i < nrows; i++) {
      Ptot_rescale += x(i);
      cout << "rescalefactor(" << i << ") = " <<  x(i) / P[i] << endl;
    }
  }

  cout.precision(20);

  cout << endl << "Ptot: " << Ptot << endl; 
  cout << "P_rescale: "  << Ptot_rescale << endl;

  //Our method:
  double w;
  w = sigma2E / (sigma2E + dPtot);

  cout << w << endl;

  P_our = w*Ptot + (1-w)*E;

  cout << "Our scaled P: " << P_our << endl;

}






















