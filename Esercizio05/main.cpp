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
#include "metropolis1s.h"
#include "metropolis2p.h"
#include "statistics.h"

double N=1E6;  //1E6
double Nblocks=100; //100
double throws_per_block=N/Nblocks;

int Nomega=20; //discretization of omega (power spectrum related)


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

   metropolis1s walker_1s(0.,0.,0.);
   metropolis2p walker_2p(0.,0.,0.);

   Statistics walker_1s_stat(N,Nblocks);
   Statistics walker_2p_stat(N,Nblocks);

   Coordinates t;

   for (double nb = 0; nb < Nblocks; nb++) {
      double sum=0;
      for (size_t tb = 0; tb < throws_per_block; tb++) {
         walker_1s.MakeGaussStep(rnd);
         sum+=walker_1s.GetR();
         std::cout << walker_1s.GetR() << '\n';
      }
      walker_1s.PrintRate();
      walker_1s_stat.UpdateAveAv2(sum, nb);
   }
   //walker_1s_stat.SaveBlockAverageVariance("Gauss2p.txt");


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
