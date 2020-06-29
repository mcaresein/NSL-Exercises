#ifndef __Metropolis2p__
#define __Metropolis2p__
#include <string>
#include <fstream>
#include <cmath>
#include "metropolisRW.h"

class metropolis2p: public metropolisRW {
public:
   metropolis2p(double x0, double y0, double z0): metropolisRW(x0, y0, z0){};
   metropolis2p(Coordinates S0): metropolisRW(S0){};

   void MakeUniformStep(Random &rnd);
   void MakeGaussStep(Random &rnd);
   double GetPDFValue();

};

#endif

/*-----------------------------------------
child class ot implement a metropolis rw
with acceptance rate using 2p Hydrogen
squared wavefunction as limit distribution.
-------------------------------------------*/
