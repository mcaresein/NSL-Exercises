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
#include "variables.h"
#include "genetic.h"
#include "functions.h"

using namespace std;

Random rnd;

int main(){
  InitializeRandomNumberGenerator();
  Input();
  //cout << NUMBER_OF_CITIES;
  Sehenswurdigkeiten square(x_of_cities[0], y_of_cities[0]);
  Sehenswurdigkeiten circle(x_of_cities[1], y_of_cities[1]);

  RoadBook rdbk(NUMBER_OF_PATHS,NUMBER_OF_CITIES, rnd);
  UpdateProbabilities(rdbk, square);

  for(int i=0; i<NUMBER_OF_PATHS; i++){
//      cout << probabilities[i]<< endl;
  }
  // for(int i=0; i<NUMBER_OF_PATHS; i++){
  //     rdbk.GetRoadBook()[i].PrintPath();
  // }

  rdbk.Crossover(rnd);

  return 0;
}

void Input(){
  /*un po' barocco (come il resto del codice del resto):
  lo script di python produce le due mappe con le stesse città ed il numero è salvato nella prima riga del primo file prodotto, ovvero squareworld*/
  ifstream citiescoordinates[2];
  citiescoordinates[0].open("squareworld");
  citiescoordinates[1].open("circleworld");
  citiescoordinates[0] >> NUMBER_OF_CITIES;

  x_of_cities.resize(2, vector<double>(NUMBER_OF_CITIES,0));
  y_of_cities.resize(2, vector<double>(NUMBER_OF_CITIES,0));



  for (int i= 0; i < NUMBER_OF_CITIES; i++){
    citiescoordinates[0]>> x_of_cities[0][i] >> y_of_cities[0][i];
    citiescoordinates[1]>> x_of_cities[1][i] >> y_of_cities[1][i];
  }
  probabilities.resize(NUMBER_OF_PATHS,0);

}

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

void UpdateProbabilities(RoadBook rdbk, Sehenswurdigkeiten cities){
  double sumprob=0;
  for(int i=0; i<NUMBER_OF_PATHS; i++){
      probabilities[i]=cities.GetDistance(rdbk.GetRoadBook()[i]);
      sumprob+=probabilities[i];
  }
  for(int i=0; i<NUMBER_OF_PATHS; i++) probabilities[i]/=sumprob;

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/}
