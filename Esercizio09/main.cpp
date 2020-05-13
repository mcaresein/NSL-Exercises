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
//#include "random.h"
#include "genetic.h"

int NUMBER_OF_CITIES;
int MOVES_PER_TEMPERATURE;
double T_MIN;
double T_MAX;
int T_STEPS;

double beta, temperature;

int accepted, attempted;
int iprint;

using namespace std;

Random rnd;
void InitializeRandomNumberGenerator(){
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
     // for(int i=0; i<(100000); i++){
     //    cout << rnd.Rannyu() << endl;
     // }
     //
     // rnd.SaveSeed();
};


int main (int argc, char *argv[]){

  InitializeRandomNumberGenerator();
  Path p(4, rnd);
  p.PrintPath();
  //cout << p.GetLength();

  City c;
  vector<int> x_of_cities, y_of_cities;
  x_of_cities={0, 0, 2, 2};
  y_of_cities={0, 2, 0, 2};
  Sehenswurdigkeiten cities(x_of_cities, y_of_cities);
  cities.PrintSehenswurdigkeiten("cane",p);
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
