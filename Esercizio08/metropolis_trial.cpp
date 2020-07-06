#include "metropolis_trial.h"

double WaveFunction_trial(Coordinates pos, double mu, double sigma){
   //std::cout << "mu: "<< mu << "sigma: " << sigma << " x: " << pos.x << '\n';


   double eplus=(pos.x + mu)*(pos.x + mu)/(2*sigma*sigma);
   double eminus=(pos.x - mu)*(pos.x - mu)/(2*sigma*sigma);

   return (exp(-eplus)+exp(-eminus) )*(exp(-eplus)+exp(-eminus) );
}


metropolis_trial::metropolis_trial(Coordinates S0, double mu, double sigma): metropolisRW(S0){
   _mu=mu;
   _sigma=sigma;
}

metropolis_trial::metropolis_trial(double x0, double mu, double sigma): metropolisRW(x0){
   _mu=mu;
   _sigma=sigma;
   std::cerr << "mu: "<< _mu << " sigma: " << _sigma << '\n';

}

void metropolis_trial::SetOptimizationParameters(double mu, double sigma){
   _mu=mu;
   _sigma=sigma;
}
double metropolis_trial::GetMu(){
   return _mu;
}
double metropolis_trial::GetSigma(){
   return _sigma;
}


void metropolis_trial::PrintOptimizationParameters(){
   std::cerr << "Optimization's parameters: mu = " << _mu << "; sigma = " << _sigma << '\n';
}

// Trial uniformi
void metropolis_trial::MakeUniformStep(Random &rnd){
   double stepsize=4; //to be tuned for 50% accept rate

   //make a move
   Coordinates new_pos;
   new_pos.x = _rw.x + (rnd.Rannyu(0,1)-0.5)*stepsize;

   //metropolis
   double accrej=rnd.Rannyu(0,1);
   double A=std::min( 1., WaveFunction_trial(new_pos, _mu, _sigma)/WaveFunction_trial(_rw,  _mu, _sigma) );
    A=WaveFunction_trial(new_pos, _mu, _sigma)/WaveFunction_trial(_rw,  _mu, _sigma);
   if (accrej<A){
      this->SetCoordinates(new_pos);
      _Naccept+=1;
   }
   _Nthrows+=1;

}

double metropolis_trial::GetPDFValue(){return WaveFunction_trial(this->GetCoordinates(), _mu, _sigma);}

double metropolis_trial::GetEnergy(){
   double x = this->GetCoordinates().x;
   double T =  exp(-(x -_mu)*(x -_mu)/(2*_sigma*_sigma))*( -1 + (x -_mu)*(x -_mu)/_sigma/_sigma )/_sigma/_sigma +
               exp(-(x +_mu)*(x +_mu)/(2*_sigma*_sigma))*( -1 + (x +_mu)*(x +_mu)/_sigma/_sigma )/_sigma/_sigma;
   double V = x*x*x*x-2.5*x*x;
   //return -0.5*T+V;
   return -0.5*T/sqrt(this->GetPDFValue())+V;
}
