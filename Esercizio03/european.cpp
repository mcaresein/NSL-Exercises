#include <cmath>
#include <fstream>
#include <iostream>

#include "european.h"


double N(double x){
   return 0.5*( 1 - erf(x/sqrt(2)) );
}

European::European(double delivery_time, double street_price){
   _T=delivery_time;
   _K=street_price;
}

double European::GetPrice_Call(){return _EuropeanCall;}

double European::GetPrice_Put(){ return _EuropeanPut ;}

double GetNSteps();


//questa sarebbe l'implementazione corretta se avessi gli indici giusti...
void European::UpdatePrice(gbm Asset, double this_time){
   // double sigma=Asset.GetVolatility();
   double timestep=_T/Asset.GetNSteps();
   double r=Asset.Getr();
   double ST=Asset.GetValueAtTime(this_time-timestep);
   //std::cout << " "<< this_time <<      " " << ST  << " " << ST-_K<< '\n';
   double PayoffCall= ST - _K;
   double PayoffPut = _K - ST;
   _EuropeanCall = exp(-r*this_time)*std::max(0., PayoffCall)  ;
   _EuropeanPut  = exp(-r*this_time)*std::max(0.,  PayoffPut)  ;
}

//bad programming: uso questo sgamo perchè evidentemente non so contare gli inidici e faceva casino prima
void European::ToDatePrice(gbm Asset,double this_time){
   double r=Asset.Getr();
   double ST=Asset.GetLastValue();
   double PayoffCall= ST - _K;
   double PayoffPut = _K - ST;
   _EuropeanCall = exp(-r*this_time)*std::max(0., PayoffCall)  ;
   _EuropeanPut  = exp(-r*this_time)*std::max(0.,  PayoffPut)  ;
}
