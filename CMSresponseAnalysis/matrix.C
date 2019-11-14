
void matrix() {
        int nrows = 2;
        TMatrixTSym<double> a(nrows);
        TVectorD b(nrows);
        double sigma2E = 5;

        double caloEnergy;
        caloEnergy = 2.0;

        const double P[2] = {1., 2.}; 
        const double dP[2] = {3., 4.};

        for (int i = 0; i < nrows; i++) {
          double sigma2i = dP[i]*dP[i];

          a(i, i) = 1. / sigma2i + 2. / sigma2E;
          b(i) = P[i] / sigma2i + caloEnergy / sigma2E;
          for (int j = 0; j < nrows; j++) {
            if (i == j)
              continue;
            a(i, j) = 1/sigma2E;
          }  
        }

        // solve ax = b
        TDecompChol decomp(a);
        bool ok = false;
        TVectorD x = decomp.Solve(b, ok);

        a.Print();
        x.Print();
        b.Print();
        // for each track create a PFCandidate track
        // with a momentum rescaled to weighted average
        if (ok) {
          for (int i = 0; i < nrows; i++) {
            cout <<  x(i) / P[i] << endl;
          }
        }
}

