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
  // 1: MPF plot
  // 2: Loop one sample and produce all the plots
  // 3: Loop + MPF
  int mode = 3;

  switch (mode) {
    case 0 :
      d.Loop();
      break;
    case 1 :
      d.Loop();
      d.plotMPF(3,-1);
      break;
    case 2 :
      d.Loop();
      d.plotMPF(3,-1);
      d.plotRjet(3,-1);
      d.plotVariants(3,-1);
      //d.plotRjet(3,-1);
      //d.plotF(3,-1);
      break;
    case 3 :
      //d.Loop();
      //d.plotMPF(3,-1);
      //d.plotRjet(3,-1);
      d.plotVariants(3,-1);
      //d.plotF(3,-1);
      break;
    default: cout << "Unknown mode chosen in main.cpp" << endl;
  }

  cout << "Main exiting succesfully" << endl;
  return 1;
}
