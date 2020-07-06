#include "metropolis2p.h"

double WaveFunction2_2p(Coordinates pos){
   double r=sqrt(pos.x*pos.x + pos.y*pos.y + pos.z*pos.z);
   return (pos.z*pos.z*exp(-r))/(32*M_PI);
}

// Trial uniformi
void metropolis2p::MakeUniformStep(Random &rnd){
   double stepsize=5.9; //to be tuned for 50% accept rate

   Coordinates new_pos;
   new_pos.x = _rw.x + (rnd.Rannyu(0,1)-0.5)*stepsize;
   new_pos.y = _rw.y + (rnd.Rannyu(0,1)-0.5)*stepsize;
   new_pos.z = _rw.z + (rnd.Rannyu(0,1)-0.5)*stepsize;

   double accrej=rnd.Rannyu(0,1);

   double A=std::min( 1., WaveFunction2_2p(new_pos)/WaveFunction2_2p(_rw) );
   //std::cout << A << '\n';
   if (accrej<A){
      this->SetCoordinates(new_pos);
      _Naccept+=1;
   }
   _Nthrows+=1;

}

// Trial gaussiani
void metropolis2p::MakeGaussStep(Random &rnd){
   double stepsize=1.85; //to be tuned for 50% accept rate

   Coordinates new_pos;
   new_pos.x = _rw.x + rnd.Gauss(0,stepsize);
   new_pos.y = _rw.y + rnd.Gauss(0,stepsize);
   new_pos.z = _rw.z + rnd.Gauss(0,stepsize);

   double accrej=rnd.Rannyu(0,1);

   double A=std::min( 1., WaveFunction2_2p(new_pos)/WaveFunction2_2p(_rw) );

   if (accrej<A){
      this->SetCoordinates(new_pos);
      _Naccept+=1;
   }
   _Nthrows+=1;

}

double metropolis2p::GetPDFValue(){return WaveFunction2_2p(this->GetCoordinates());}
