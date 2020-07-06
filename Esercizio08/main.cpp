/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#include "random.h"
#include "metropolisRW.h"
#include "metropolis_trial.h"
#include "statistics.h"

double N=1E6;  //1E6
double Nblocks=100; //100
double throws_per_block=N/Nblocks;

using namespace std;

int main (int argc, char *argv[]){
   Random rnd;
   int seed[4];
   int p1, p2;
   std::ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else std::cerr << "PROBLEM: Unable to open Primes" << std::endl;
   Primes.close();

   std::ifstream input("seed.in");
   std::string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else std::cerr << "PROBLEM: Unable to open seed.in" << std::endl;

   if (argc!=4) { cerr << "Usage: "<< argv[0] << " mu sigma file_to_store_averages \nTry again"; exit(-1);}
   double mu= stod(argv[1]);
   double sigma= stod(argv[2]);

   metropolis_trial walker(0., mu, sigma );

   Statistics walker_stat(N,Nblocks);

   Coordinates t;


   for (int nb = 0; nb < Nblocks; nb++) {
      double sum=0.;
      double energy=0.;
      for (int tb = 0; tb < throws_per_block; tb++) {
         walker.MakeUniformStep(rnd);
         std::cout << walker.GetCoordinates().x<< '\n';
         //std::cout << walker.GetEnergy() ;
         energy+=walker.GetEnergy();
      }
      if ( (nb+1)%10==0) walker.PrintRate();
      walker_stat.UpdateAveAv2(energy, nb);
   }
   walker_stat.SaveBlockAverageVariance("energy");

   return 0;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
