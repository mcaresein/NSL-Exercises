#include "metropolisRW.h"

metropolisRW::metropolisRW(Coordinates S0){
   _rw=S0;
   _Naccept=0;
   _Nthrows=0;

};

metropolisRW::metropolisRW(double x0){
   _rw.x=x0;
   _Naccept=0;
   _Nthrows=0;

};

Coordinates metropolisRW::GetCoordinates(){
   return _rw;
};

void metropolisRW::SetCoordinates(Coordinates pos){
   _rw=pos;
};

void metropolisRW::PrintRate(){std::cerr << "Acceptance : " << _Naccept/ _Nthrows*100 << "%\n";}

//double metropolisRW::GetR(){return  sqrt(_rw.x*_rw.x+ _rw.y*_rw.y+ _rw.z*_rw.z);};
