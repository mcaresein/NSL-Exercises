#include <cmath>
#include <iostream>

#include "gbm.h"

gbm::gbm(double S0, double r, double volatility, int T, int NSteps){
   _r=r;
   _volatility=volatility;
   _NSteps=NSteps;
   _T=T;
   //_process.resize(NSteps,0);
   _process.push_back(S0);
}

double gbm::GetValueAtTime(double time){
   double index=floor(time*_NSteps+0.1);
   //std::cout << index << '\n';
   return this->GetValueAtPosition(index);
}

double gbm::GetValueAtPosition(int index){
   return _process[index];
}

double gbm::GetLastValue(){return _process.back();}
void   gbm::PrintValues(){
   for (std::vector<double>::iterator i = _process.begin(); i < _process.end(); i++){
      std::cout << *i << std::endl;
   };
}

void   gbm::SaveValues(std::string filename){
   std::ofstream file(filename);
   for (int i = 0; i < _process.size(); i++) {
      file << i << " " << _process[i] << std::endl;
   }
   file.close();
};

double gbm::Getr(){return _r;};

double gbm::GetNSteps(){return _NSteps;};

double gbm::GetVolatility(){return _volatility;};

void gbm::MakeAStep(double timestep, double r1){
   //std::cout << r1 << std::endl;
   double previous=*(_process.end()-1);
   double newvalue= previous*exp( (_r-0.5*pow(_volatility,2) )*timestep + _volatility*r1*sqrt(timestep) );
   //std::cout << previous << " " <<  newvalue << std::endl;
   _process.push_back(newvalue);
}
