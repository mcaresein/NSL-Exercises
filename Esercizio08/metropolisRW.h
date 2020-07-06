#ifndef __MetropolisRW__
#define __MetropolisRW__
#include <string>
#include <fstream>
#include <cmath>
#include <iostream>

#include "random.h"


struct Coordinates{
   double x;
};

class metropolisRW {
public:
   metropolisRW(Coordinates S0);
   metropolisRW(double x0);
   ~metropolisRW(){};

   Coordinates GetCoordinates();
   void SetCoordinates(Coordinates pos);
   void PrintRate();
   double GetR();


   virtual void MakeUniformStep(Random &rnd)=0;
   virtual void MakeGaussStep(Random &rnd)=0;
   virtual double GetPDFValue()=0;


protected:
   //occhio che per come è ora stai riscrivendo se fai 2p e 1s insieme!!
   Coordinates _rw;
   double _Naccept, _Nthrows;
};

#endif

/*-----------------------------------------
base class for implementation of a random walk with
accept reject for Metropolis' implementation pourposes
-------------------------------------------*/
