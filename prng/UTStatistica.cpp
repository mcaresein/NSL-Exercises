/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

   ////////////////////////////////////////////////////
   // UNIT TEST per la classe Statistics.            //
   // Riproduce l'analisi dati contenuta nel punto 1 //
   // dell'esercizio 1 del notebook 1.               //
   ////////////////////////////////////////////////////


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#include "statistics.h"
#include "random.h"

///  DATI INIZIALI  ///
int S0=100; //initial price
int T = 1; //delivery time
int K=100; //Strike price
double R=0.1; //risk-free interest rate:
double SIGMA=0.25; //volatility

using namespace std;

int main (int argc, char *argv[]){

   Random rnd;
   int seed[4];
   int p1, p2;
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;


   int k;
   int N=100;
   int M=100000;
   int L=M/N;
   Statistics statistica(M,N) ;

   for(int i=0; i<N; i++){
       double sum = 0;
       for(int j=0; j<L; j++){
           k = j+i*L;
           sum += rnd.Rannyu();
      }
      statistica.UpdateAveAv2(sum,i);
   }
   string filename="test";
   //SaveBlockAverageVariance(ave, av2, N, filename);
   statistica.SaveBlockAverageVariance(filename);

   rnd.SaveSeed();
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
