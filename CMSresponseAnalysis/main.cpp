//Running CMSJES as a ROOT macro is slow. To make things faster, write a script
//of what you want to do here. Then compile using:
//  g++ -o runCMSJES main.cpp CMSJES.C CMSJES.h `root-config --cflags --glibs`
//and run the script:
//  ./runCMSJES

#include "CMSJES.h"

int main() {
  CMSJES d;	//Init. This asks for a file, just choose once
  //Available modes
  // 0: Loop single sample
  // 1: MultiLoop (multithreading: similar gamma+jet and b-enriched samples)
  // 2: FindFJtoMC
  // 3: Fit
  // 4: run many samples and produce their plots at once
  int mode = 4;

  vector<string> runs = {"RunIIb1","RunIIb2","RunIIb34"};
  
  switch (mode) {
    case 0 : d.Loop();       break;
    case 1 : d.MultiLoop();  break;
    case 2 :	//IMPORTANT: you ought to choose a dijet file w/o/ eta cuts!
      //RunIIa
      //d.SetrunIIa(true);
      //d.SetrunIIb(false);
      //d.SetStrangeB(true);
      d.FindFJtoMC();
      //RunIIb
      //d.SetrunIIa(false);
      //d.SetrunIIb(true);
      //d.SetP20ToP17(true);
      //d.Setrun("RunIIb1");
      //d.SetStrangeB(true);
      //d.FindFJtoMC();
      break;
    case 3 :
      d.FitGN();
      break;
    case 4 :
      //for (int run=1; run!=2; ++run) {
        //for (int XS=1; XS!=2; ++XS) {
          //d.flavCorr(true, 1, 1, 1, 2, run, 1, XS);
          // d.flavCorr(true, 1, 1, 1, 2, 2, 1, XS);
          //d.plotPT(1, 1, 1, 2, run, 1, XS,false,false);
          d.plotPT(3, 2, 2, 2, 1, 1, 1, 1, false,false);
        //}
      //}
      break;
    default: cout << "Unknown mode chosen in main.cpp" << endl;
  }

  cout << "Main exiting succesfully" << endl;
  return 1;
}
