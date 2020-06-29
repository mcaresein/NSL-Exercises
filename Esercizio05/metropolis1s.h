#ifndef __Metropolis1s__
#define __Metropolis1s__
#include <string>
#include <fstream>
#include <cmath>
#include "metropolisRW.h"

class metropolis1s: public metropolisRW {
public:
   metropolis1s(double x0, double y0, double z0): metropolisRW(x0, y0, z0){};
   metropolis1s(Coordinates S0): metropolisRW(S0){};

   void MakeUniformStep(Random &rnd);
   void MakeGaussStep(Random &rnd);
   double GetPDFValue();

};

#endif

/*-----------------------------------------
child class ot implement a metropolis rw
with acceptance rate using 1s Hydrogen
squared wavefunction as limit distribution.
-------------------------------------------*/
