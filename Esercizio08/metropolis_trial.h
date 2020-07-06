#ifndef __Metropolis_trial__
#define __Metropolis_trial__
#include <string>
#include <fstream>
#include <cmath>
#include "metropolisRW.h"

class metropolis_trial: public metropolisRW {
public:
   metropolis_trial(Coordinates S0, double mu, double sigma);
   metropolis_trial(double x0, double mu, double sigma);

   void SetOptimizationParameters(double mu, double sigma);
   double GetMu();
   double GetSigma();
   void PrintOptimizationParameters();

   void MakeUniformStep(Random &rnd);
   void MakeGaussStep(Random &rnd){};

   double GetPDFValue();
   double GetEnergy();
private:
   double _mu, _sigma;

};

#endif

/*-----------------------------------------
child class ot implement a metropolis rw
with acceptance rate using 1s Hydrogen
squared wavefunction as limit distribution.
-------------------------------------------*/
