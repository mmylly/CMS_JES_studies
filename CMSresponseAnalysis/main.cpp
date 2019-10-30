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
  // 2: Fit
  // 3: run many samples and produce their plots at once
  // 4: Loop() + flavCorr for debug
  int mode = 1;

  switch (mode) {
    case 0 : d.Loop();       break;
    case 1 :
      //d.plotPT(3, -1);
      d.plotMPF(3,-1);
      break;
    case 2 :
      d.Loop();
      d.plotMPF(3,-1);
      break;
    case 3 :
      //d.Loop();
      d.plotF(3,-1);
      break;
    default: cout << "Unknown mode chosen in main.cpp" << endl;
  }

  cout << "Main exiting succesfully" << endl;
  return 1;
}
