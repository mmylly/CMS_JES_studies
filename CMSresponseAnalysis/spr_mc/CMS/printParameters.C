void printParameters() {

  //double C = 1.055; double a = -1.192; double m = -0.38;  // antineutron EHE |eta|<0.5
  //double C = 1.064; double a = -1.402; double m = -0.771; // antineutron HHe |eta|<0.5
  //double C = 1.163; double a = -1.117; double m = -0.274; // antiproton EHE |eta|<0.5
  //double C = 1.098; double a = -1.091; double m = -0.551; // antiproton HHe |eta|<0.5
  //double C = 1.035; double a = -1.426; double m = -0.430; // K0L EHE |eta|<0.5
  //double C = 1.067; double a = -1.370; double m = -0.728; // K0L HHe |eta|<0.5
  //double C = 1.040; double a = -1.361; double m = -0.416; // K0S EHE |eta|<0.5
  //double C = 1.065; double a = -1.649; double m = -0.774; // K0S HHe |eta|<0.5
  //double C = 1.040; double a = -1.757; double m = -0.455; // neutron EHE |eta|<0.5
  //double C = 1.067; double a = -1.980; double m = -0.735; // neutron HHe |eta|<0.5
  //double C = 1.108; double a = -1.606; double m = -0.363; // proton EHE |eta|<0.5
  //double C = 1.089; double a = -1.999; double m = -0.652; // proton HHe |eta|<0.5
  //double C = 1.085; double a = -1.443; double m = -0.365; // Km EHE |eta|<0.5
  //double C = 1.095; double a = -1.544; double m = -0.635; // Km HHe |eta|<0.5
  //double C = 1.036; double a = -1.976; double m = -0.463; // Kp EHE |eta|<0.5
  //double C = 1.092; double a = -1.405; double m = -0.621; // Kp HHe |eta|<0.5
  double C = 1.036; double a = -1.976; double m = -0.463; // pion EHE |eta|<1.3
  double C = 1.092; double a = -1.405; double m = -0.621; // pion HHe |eta|<1.3

  for (int i=0; i!=53; ++i) {
    cout << i << " " << C << " " << -1*a << " " << m+1 << endl;
  }
}



















