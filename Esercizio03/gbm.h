#ifndef __Gbm__
#define __Gbm__
#include <vector>
#include <string>
#include <fstream>
#include "random.h"

class gbm: public std::vector<double> {
public:
   gbm(double S0, double r, double volatility, int T, int NSteps);
   ~gbm(){};

   double GetValueAtTime(double time);
   double GetValueAtPosition(int index);
   double GetLastValue();
   void PrintValues();
   void SaveValues(std::string filename);
   double Getr();
   double GetNSteps();
   double GetVolatility();
   void MakeAStep(double timestep, double randomnumber);

private:
   std::vector<double> _process;
   double _r, _volatility;
   int _NSteps, _T;
   //Random _rnd;

};


#endif
