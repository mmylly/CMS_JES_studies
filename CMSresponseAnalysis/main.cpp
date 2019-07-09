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
  // 5: Loop() + flavCorr for debug
  int mode = 4;

  switch (mode) {
    case 0 : d.Loop();       break;
    case 1 : d.MultiLoop();  break;
    case 2 :	//IMPORTANT: you ought to choose a dijet file w/o/ eta cuts!
      //RunIIa
      d.FindFJtoMC();
      break;
    case 3 :
      d.FitGN();
      break;
    case 4 :
      d.flavCorr(true, 3, 0);
      d.plotPT(3, 0, false, false);
      d.plotMPF(3,0);
      break;
    case 5 :
      d.Loop();
      d.flavCorr(true, 3, 0);
      break;
    default: cout << "Unknown mode chosen in main.cpp" << endl;
  }

  cout << "Main exiting succesfully" << endl;
  return 1;
}
