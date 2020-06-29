#include "metropolisRW.h"

metropolisRW::metropolisRW(Coordinates S0){
   _rw.x = S0.x;
   _rw.y = S0.y;
   _rw.z = S0.z;
   _Naccept=0;
   _Nthrows=0;

};

metropolisRW::metropolisRW(double x0, double y0, double z0){
   _rw.x = x0;
   _rw.y = y0;
   _rw.z = z0;
   _Naccept=0;
   _Nthrows=0;
};


Coordinates metropolisRW::GetCoordinates(){
   return _rw;
};

void metropolisRW::SetCoordinates(double x0, double y0, double z0){
   _rw.x = x0;
   _rw.y = y0;
   _rw.z = z0;
};

void metropolisRW::SetCoordinates(Coordinates pos){
   _rw=pos;
};

void metropolisRW::PrintRate(){std::cerr << "Acceptance : " << _Naccept/ _Nthrows*100 << "%\n";}

double metropolisRW::GetR(){return  sqrt(_rw.x*_rw.x+ _rw.y*_rw.y+ _rw.z*_rw.z);};
