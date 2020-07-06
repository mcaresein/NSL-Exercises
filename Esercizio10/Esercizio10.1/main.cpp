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
#include <numeric>

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

  Anneal(rdbk, circle);

  // for(int i=0; i<NUMBER_OF_PATHS; i++){
  // //  rdbk.GetRoadBook()[i].CheckPath();
  // }

  PrintBest(rdbk,circle,"bestpath.txt");
  cerr << "Done!\n";
  return 0;
}

void Input(){
   ifstream citiescoordinates[2];
   citiescoordinates[0].open("squareworld");
   citiescoordinates[1].open("circleworld");
   citiescoordinates[0] >> NUMBER_OF_CITIES;

   x_of_cities.resize(2, vector<double>(NUMBER_OF_CITIES,0));
   y_of_cities.resize(2, vector<double>(NUMBER_OF_CITIES,0));


   ifstream ReadInput;
   ReadInput.open("input.dat");
   ReadInput >> NUMBER_OF_PATHS;
   ReadInput >> MOVES_PER_TEMPERATURE;
   ReadInput >> T_MIN;
   ReadInput >> T_MAX;
   ReadInput >> T_STEP;
   ReadInput >> VERBOSE;

   cerr << "This program aim to solve TSP problem by simulated annealing\n";
   cerr << "T_MAX: " << T_MAX << endl;
   cerr << "T_MIN: " << T_MIN << endl;
   cerr << "T_STEP: " << T_STEP << endl;
   cerr << "System is cooled at non equally spaced temperature, calculated with the formula T=T-(T-T_MIN)/T_STEP-0.001" << endl;
   cerr << "Number of paths: " << NUMBER_OF_PATHS << endl;
   if(VERBOSE) cerr << "Verbosity is on!\n";

   ReadInput.close();

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
            accepted+=1;
        }
        attempted+=1;
    }
}

void Anneal(RoadBook& rdbk, Sehenswurdigkeiten citieshape){
   ofstream bestdistance,distvstemp;
   bestdistance.open("output_bestdistance.dat");
   distvstemp.open("distvstemp.dat");
   double T=T_MAX;
   //for (double t = T_MAX; t>T_MIN ; t-=T_STEPS){
   int k=0;
   while (T>T_MIN ){
      cerr << "T = " << T << endl;
      beta=1./T;
      for ( int i = 0; i<MOVES_PER_TEMPERATURE; i++){
         if(i==0) distvstemp << T << " " << citieshape.GetDistance(rdbk.GetRoadBook()[GetDistanceSortIndexes(rdbk, citieshape)[0]]) << endl; k++;

         Mutate(rdbk, citieshape);
         if (MOVES_PER_TEMPERATURE%iprint==0) citieshape.PrintDistances("paths.txt", rdbk, "A");
         bestdistance << k << " " << citieshape.GetDistance(rdbk.GetRoadBook()[GetDistanceSortIndexes(rdbk, citieshape)[0]]) << endl; k++;
      }
      T=T-(T-T_MIN)/T_STEP-0.001;
   }
   bestdistance.close();
   distvstemp.close();
}

void PrintBest(RoadBook rdbk, Sehenswurdigkeiten citieshape, string filename){
  Path bestpath=rdbk.GetRoadBook()[0];
  bestpath.PrintPath();
  int bestindex=0;
  for (int i = 0; i < rdbk.GetRoadBookSize(); i++) {
      if ( citieshape.GetDistance(bestpath)>citieshape.GetDistance(rdbk.GetRoadBook()[i]) ){
          bestindex=i;
          bestpath=rdbk.GetRoadBook()[i];
      }
  }
  cerr << "Best Path's length is " << citieshape.GetDistance(bestpath) << endl;
  citieshape.PrintSehenswurdigkeiten(filename, bestpath, "W");
}

std::vector<int> GetDistanceSortIndexes(RoadBook& rdbk, Sehenswurdigkeiten cities){
   vector<int> sort_indices(NUMBER_OF_PATHS,0);
   vector<double> distances;

   for(int i=0; i<NUMBER_OF_PATHS; i++){
       distances.push_back(cities.GetDistance(rdbk.GetRoadBook()[i]));
   }
   vector<double> d=distances;

   stable_sort(d.begin(), d.end());

   for (size_t i = 0; i < distances.size(); i++) {
      sort_indices[i]=find(distances.begin(),distances.end(), d[i])-distances.begin();
   }

   sort_indices=sort(distances);
   if(VERBOSE){
      for (size_t i = 0; i < NUMBER_OF_PATHS; i++) {
         std::cout << distances[i] << ' ';
      } std::cout << '\n';
      for (size_t i = 0; i < NUMBER_OF_PATHS; i++) {
         std::cout << d[i] << ' ';
      } std::cout << '\n';
      for (size_t i = 0; i < NUMBER_OF_PATHS; i++) {
         std::cout << sort_indices[i] << ' ';
      } std::cout << '\n';
   }
   return sort_indices;
}

std::vector<int> sort(const std::vector<double> &v) {
  // initialize original index locations
  vector<int> idx(v.size());
  iota(idx.begin(), idx.end(), 0);

  // sort indexes based on comparing values in v
  sort(idx.begin(), idx.end(),
       [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});

  return idx;
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
