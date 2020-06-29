#include "metropolis1s.h"

double WaveFunction2_1s(Coordinates pos){
   double r=sqrt(pos.x*pos.x + pos.y*pos.y + pos.z*pos.z);
   return exp(-2*r)/M_PI;
}

// Trial uniformi
void metropolis1s::MakeUniformStep(Random &rnd){
   double stepsize=2.4; //to be tuned for 50% accept rate

   Coordinates new_pos;
   new_pos.x = _rw.x + (rnd.Rannyu(0,1)-0.5)*stepsize;
   new_pos.y = _rw.y + (rnd.Rannyu(0,1)-0.5)*stepsize;
   new_pos.z = _rw.z + (rnd.Rannyu(0,1)-0.5)*stepsize;

   double accrej=rnd.Rannyu(0,1);

   double A=std::min( 1., WaveFunction2_1s(new_pos)/WaveFunction2_1s(_rw) );
   //std::cout << A << '\n';
   if (accrej<A){
      this->SetCoordinates(new_pos);
      _Naccept+=1;
   }
   _Nthrows+=1;

}

// Trial gaussiani
void metropolis1s::MakeGaussStep(Random &rnd){
   double stepsize=0.75; //to be tuned for 50% accept rate

   Coordinates new_pos;
   new_pos.x = _rw.x + rnd.Gauss(0,stepsize);
   new_pos.y = _rw.y + rnd.Gauss(0,stepsize);
   new_pos.z = _rw.z + rnd.Gauss(0,stepsize);

   double accrej=rnd.Rannyu(0,1);

   double A=std::min( 1., WaveFunction2_1s(new_pos)/WaveFunction2_1s(_rw) );

   if (accrej<A){
      this->SetCoordinates(new_pos);
      _Naccept+=1;
   }
   _Nthrows+=1;

}

double metropolis1s::GetPDFValue(){return WaveFunction2_1s(this->GetCoordinates());}
