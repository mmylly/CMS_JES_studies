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
  int mode = 0;

  switch (mode) {
    case 0 :
      //d.Loop();
      //d.plotMPF(-1,-1);
      //d.plotRjet(-1,-1);
      //d.plotF(-1,-1);
      //d.plotFF(-1,-1);
      //d.plotJEF(-1,-1);
      d.plotEff(-1,-1);
      break;
    case 1 :
      d.Loop();
      d.plotEff(-1,-1);
      d.plotMPF(-1,-1);
      d.plotJEF(-1,-1);
      d.plotRjet(-1,-1);
      d.plotF(-1,-1);
      break;
    case 2 :
      d.Loop();
      d.plotRjet(3,-1);
      d.plotJEF(3,-1);
      //d.plotFF(3,-1);
      //d.plotRjet(3,-1);
      break;
    case 3 :
      d.plotVarSPR(3,-1);
      break;
    default: cout << "Unknown mode chosen in main.cpp" << endl;
  }

  cout << "Main exiting succesfully" << endl;
  return 1;
}
