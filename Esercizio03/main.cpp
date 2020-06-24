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
#include "gbm.h"
#include "european.h"
#include "statistics.h"

///  DATI INIZIALI  ///
//messo ad 1 per far funzionare PrintValue (cattiva programmazione!). Cambiare soltanto NSteps.
//il programma calcola autonomamente i timesteps equispaziati ed procede con la simulazione
const int T = 1; //delivery time.

int S0=100; //initial price
int K=100; //Strike price
double r=0.1; //risk-free interest rate:
double volatility=0.25; //volatility

double NSteps=100;
double N=1E5;
double Nblocks=1E2;
double timestep=T/NSteps;


using namespace std;

Statistics DiscCall_Stat(N,Nblocks);
Statistics DiscPut_Stat(N,Nblocks);

Statistics DirCall_Stat(N,Nblocks);
Statistics DirPut_Stat(N,Nblocks);

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


   double ThrowsPerBlock=N/Nblocks;

   for (int Nb = 0; Nb < Nblocks; Nb++) {

      double sumDiscPut=0, sumDiscCall=0;
      double  sumDirPut=0,  sumDirCall=0;


      for (int Tb = 0; Tb < ThrowsPerBlock; Tb++) {
         gbm DiscAsset(S0, r, volatility, T, NSteps);
         European DiscOption(T,K);

         gbm DirAsset(S0, r, volatility, T, NSteps);
         European DirOption(T,K);

         // --------- discrete -------------
         for (double this_time = timestep; this_time < T; this_time+=timestep){
               DiscAsset.MakeAStep(timestep, rnd.Gauss(0,1) ); //incrementi indipendenti GBM -> dipendono solo dal timestep
         }
         DiscOption.ToDatePrice(DiscAsset, T);

         sumDiscCall+=DiscOption.GetPrice_Call();
         sumDiscPut +=DiscOption.GetPrice_Put();

         // ---------   direct   ---------
         DirAsset.MakeAStep(T, rnd.Gauss(0,1) ); //incrementi indipendenti GBM -> dipendono solo dal timestep
         DirOption.ToDatePrice(DirAsset, T);

         sumDirCall+=DirOption.GetPrice_Call();
         sumDirPut +=DirOption.GetPrice_Put();

      }
      DiscCall_Stat.UpdateAveAv2(sumDiscCall, Nb);
      DiscPut_Stat.UpdateAveAv2( sumDiscPut,  Nb);

      DirCall_Stat.UpdateAveAv2(sumDirCall, Nb);
      DirPut_Stat.UpdateAveAv2( sumDirPut,  Nb);
   }

   DiscCall_Stat.SaveBlockAverageVariance("disc_call.txt");
   DiscPut_Stat.SaveBlockAverageVariance( "disc_put.txt" );

   DirCall_Stat.SaveBlockAverageVariance("dir_call.txt");
   DirPut_Stat.SaveBlockAverageVariance( "dir_put.txt" );

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
