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
#include <cmath>
#include "variables.h"
#include "genetic.h"
#include "functions.h"

using namespace std;

Random rnd;

int main(){
  InitializeRandomNumberGenerator();
  Input();
  Sehenswurdigkeiten square(x_of_cities[0], y_of_cities[0]);
  Sehenswurdigkeiten circle(x_of_cities[1], y_of_cities[1]);

  RoadBook rdbk(NUMBER_OF_PATHS,NUMBER_OF_CITIES, rnd);

  // for(int i=0; i<NUMBER_OF_PATHS; i++){
  //     rdbk.GetRoadBook()[i].PrintPath();
  // }
  // UpdateProbabilities(rdbk,square);

  Anneal(rdbk, square);

  // for(int i=0; i<NUMBER_OF_PATHS; i++){
  // //  rdbk.GetRoadBook()[i].CheckPath();
  // }

  PrintBest(rdbk,square,"bestpath.txt");
  return 0;
}

void Input(){
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
  temperature=T_MAX;
  beta=1./temperature;


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
  //probabilities.resize(NUMBER_OF_CITIES, 1./NUMBER_OF_CITIES);
};


void Mutate(RoadBook& rdbk, Sehenswurdigkeiten cities){
    Path trialpath(rdbk.GetRoadBook()[0].GetLength(),rnd);  //giusto per definirlo, poi è sovrascritto nel loop
    double trial=0.;
    vector<double> newprobabilities=probabilities;

    for(int i=0; i<rdbk.GetRoadBookSize(); i++){
        trialpath=rdbk.GetRoadBook()[i];
        // cout << "----------" << i << "-----------"<< endl;
        // trialpath.PrintPath();

        int rand=rnd.Rannyu(0,5);
        if (rand==0) trialpath.Inversion(rnd);
        if (rand==1) trialpath.Shift(rnd);
        if (rand==2) trialpath.Swap(rnd);
        if (rand==3) trialpath.GroupShift(rnd);
        if (rand==4) trialpath.GroupSwap(rnd);

        //cout<< endl;
        //trialpath.PrintPath();
        // roadbook[i].PrintPath();

        //proposta
        trial=exp(-beta*(cities.GetDistance(trialpath)-cities.GetDistance(rdbk.GetRoadBook()[i])));
        //cout << trial << endl;
        if (trial>=rnd.Rannyu(0,1)) {
            rdbk.SetPath(trialpath, i);

        }

    }
}

void Anneal(RoadBook& rdbk, Sehenswurdigkeiten citieshape){
    double T=T_MAX;
    //for (double t = T_MAX; t>T_MIN ; t-=T_STEPS){
    while (T>T_MIN ){
        cout << "T = " << T << endl;
        beta=1./T;
        for ( int i = 0; i<MOVES_PER_TEMPERATURE; i++){
            Mutate(rdbk, citieshape);
            if (MOVES_PER_TEMPERATURE%iprint==0) citieshape.PrintDistances("paths.txt", rdbk, "A");
        }
        T=T-(T-T_MIN)/T_STEP-0.001;
    }
}

void PrintBest(RoadBook rdbk, Sehenswurdigkeiten citieshape, string filename){
  Path bestpath=rdbk.GetRoadBook()[0];
  bestpath.PrintPath();
  rdbk.GetRoadBook()[0].PrintPath();
  int bestindex=0;
  for (int i = 0; i < rdbk.GetRoadBookSize(); i++) {
      if ( citieshape.GetDistance(bestpath)>citieshape.GetDistance(rdbk.GetRoadBook()[i]) ){
          bestindex=i;
          bestpath=rdbk.GetRoadBook()[i];
      }
  }
  bestpath.PrintPath();
  cout << citieshape.GetDistance(bestpath) << endl;
  citieshape.PrintSehenswurdigkeiten(filename, bestpath, "W");
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
