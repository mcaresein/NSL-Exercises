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
#include "statistics.h"
#include "functions.h"

using namespace std;

Random rnd;

int main(){
   InitializeRandomNumberGenerator();
   Input();
   Sehenswurdigkeiten square(x_of_cities[0], y_of_cities[0]);
   Sehenswurdigkeiten circle(x_of_cities[1], y_of_cities[1]);
   Statistics rstatistics(NUMBER_OF_GENERATIONS*NUMBER_OF_PATHS/2, NUMBER_OF_GENERATIONS);  //SERVE QUA CHE SIA PARI

   RoadBook rdbk(NUMBER_OF_PATHS,NUMBER_OF_CITIES, rnd);


   ofstream bestdistance;
   bestdistance.open("output_bestdistance.dat");
   for (int nb = 0; nb < NUMBER_OF_GENERATIONS; nb++) {
      GiveBirth(rdbk, rnd, square);
      if(VERBOSE) rdbk.PrintRoadbook();
      rdbk.CheckRoadBook(!VERBOSE);
      SumBestHalfDistances(rdbk, square, rstatistics, nb);
      if((nb+1)%10==0){
         std::cerr << "Generazione " << nb+1 << '\n';
      }
      bestdistance << nb << " " << square.GetDistance(rdbk.GetRoadBook()[GetDistanceSortIndexes(rdbk, square)[0]]) << endl;

   }
   bestdistance.close();
   rstatistics.SaveBlockAverageVariance("rstatistics");
   cout << "----------" << endl;
   SaveBest(rdbk, square, "bestpath_square.txt");
   cout << "Done!";
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
   citiescoordinates[0].close();
   citiescoordinates[1].close();

   ifstream ReadInput;
   ReadInput.open("input.dat");

   cerr << "Hi, I'm GeneticTSP, a program for salesmans to walk less\n";
   cerr << "Reading input.dat\n";
   ReadInput >> NUMBER_OF_GENERATIONS;
   ReadInput >> NUMBER_OF_PATHS;
   ReadInput >> VERBOSE;
   cerr << "Number of Generations: " << NUMBER_OF_GENERATIONS << endl;
   cerr << "Number of paths: " << NUMBER_OF_PATHS << endl;
   if(VERBOSE) cerr << "I'm gonna be very verbose! :)";
   ReadInput.close();

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

void GiveBirth(RoadBook& rdbk, Random& rnd, Sehenswurdigkeiten cities){
   RoadBook newrdbk(NUMBER_OF_PATHS,NUMBER_OF_CITIES);

   for (int i = 0; i < NUMBER_OF_PATHS; i+=2){
      RoadBook temp(rdbk);
      std::vector<int> selected=Selection(rnd, rdbk, cities);
      if (VERBOSE){cout <<"Selected: " << selected[0] << " "<<  selected[1] << endl;}

      //mutation
      if (rnd.Rannyu()>0.5)  temp.Crossover(rnd, selected[0], selected[1],VERBOSE);
      temp.Mutate(rnd, selected[0], selected[1], VERBOSE);
      newrdbk.SetPath(temp.GetRoadBook()[selected[0]], i);
      newrdbk.SetPath(temp.GetRoadBook()[selected[1]],i+1);
   }
   rdbk=newrdbk;
}
vector<int> Selection(Random& rnd, RoadBook& rdbk, Sehenswurdigkeiten cities){

   vector<int> sort_indices=GetDistanceSortIndexes(rdbk, cities);
   vector<int> selected(2,0);
   selected[0]=sort_indices[floor(pow(rnd.Rannyu(), 9)*NUMBER_OF_PATHS)];
   selected[1]=sort_indices[floor(pow(rnd.Rannyu(), 9)*NUMBER_OF_PATHS)];

   return selected;
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

void SaveBest(RoadBook rdbk, Sehenswurdigkeiten citieshape, string filename){
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
  if(VERBOSE)bestpath.PrintPath();
  cerr << "Best Path's length is " << citieshape.GetDistance(bestpath) << endl;
  citieshape.PrintSehenswurdigkeiten(filename, bestpath, "W");
}

int SumBestHalfDistances(RoadBook rdbk, Sehenswurdigkeiten citieshape, Statistics& rstatistics, int nb){
   vector<int> sort_indices=GetDistanceSortIndexes(rdbk, citieshape);

   double sum=0;
   for (int i = 0; i < sort_indices.size()/2; i++) {
      sum+=citieshape.GetDistance(rdbk.GetRoadBook()[sort_indices[i]]);
   }
   rstatistics.UpdateAveAv2(sum, nb);
   return sum;
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
