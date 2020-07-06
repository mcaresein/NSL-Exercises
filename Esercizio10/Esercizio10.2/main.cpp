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
#include "mpi.h"

using namespace std;

Random rnd;

int main(int argc, char** argv){

   MPI_Init(&argc, &argv);

   int ierr, nprocess, myrank;
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
   MPI_Comm_size(MPI_COMM_WORLD, &nprocess);

   InitializeRandomNumberGenerator();
   Input(myrank);
   Sehenswurdigkeiten square(x_of_cities[0], y_of_cities[0]);
   Sehenswurdigkeiten circle(x_of_cities[1], y_of_cities[1]);

   if (NUMBER_OF_PATHS%nprocess!=0 && myrank==0){cerr << "path non equamente divisibili tra i processori!";exit(-1);}

   //begin bruttissimo --- serve per far sfalsare i due numeri casuali
   for (size_t i = 0; i <= rnd.Rannyu(0,myrank*nprocess); i++) rnd.Rannyu();
   //end bruttissimo
   RoadBook rdbk(NUMBER_OF_PATHS/nprocess,NUMBER_OF_CITIES, rnd);
   Anneal(rdbk, circle, myrank);

   //if (myrank==0)
   RoadBook g_rdbk(NUMBER_OF_PATHS, NUMBER_OF_CITIES, rnd);

   //manda tutto
   int mysize=rdbk.GetRoadBookSize()*rdbk.GetRoadBook()[0].GetLength();
   int mypackedroadbook[mysize];
   int myindex=0;
   for(int i=0;i<rdbk.GetRoadBookSize(); i++){
      for( int j=0; j<rdbk.GetRoadBook()[i].GetLength(); j++ ){
          mypackedroadbook[myindex]=rdbk.GetRoadBook()[i].GetPath()[j];
          myindex++;
      }
   }
   int size=g_rdbk.GetRoadBookSize()*g_rdbk.GetRoadBook()[0].GetLength();
   int packedroadbook[size];
   MPI_Gather(mypackedroadbook, mysize, MPI_INT, packedroadbook, mysize, MPI_INT, 0, MPI_COMM_WORLD);

   //  ~rdbk(); s'ha da chiamare??


   if (myrank==0){
      Path tempath(NUMBER_OF_CITIES, rnd);
      int pathidx=0;
      for(int i=0;i<size; i++){
          tempath.SetElem(packedroadbook[i],i%NUMBER_OF_CITIES);
          if ((i+1)%NUMBER_OF_CITIES==0){g_rdbk.SetPath(tempath, pathidx ); pathidx++;  }
      }
      PrintBest(g_rdbk,circle,"bestpath.txt");
      cout << "Done!";
   }



   MPI_Finalize();
   return 0;
}

void Input(int myrank){
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

  ifstream ReadInput;
  ReadInput.open("input.dat");
  ReadInput >> NUMBER_OF_PATHS;
  ReadInput >> MOVES_PER_TEMPERATURE;
  ReadInput >> T_MIN;
  ReadInput >> T_MAX;
  ReadInput >> T_STEP;
  if (myrank==0){
     cerr << "This program aim to solve TSP problem by parallel simulated annealing\n";
     cerr << "T_MAX: " << T_MAX << endl;
     cerr << "T_MIN: " << T_MIN << endl;
     cerr << "T_STEP: " << T_STEP << endl;
     cerr << "System is cooled at non equally spaced temperature, calculated with the formula T=T-(T-T_MIN)/T_STEP-0.001" << endl;
     cerr << "Number of paths: " << NUMBER_OF_PATHS << endl;
  }
  ReadInput.close();


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

};


void Mutate(RoadBook& rdbk, Sehenswurdigkeiten cities){
    Path trialpath(rdbk.GetRoadBook()[0].GetLength(),rnd);  //giusto per definirlo, poi è sovrascritto nel loop
    double trial=0.;
    vector<double> newprobabilities=probabilities;

    for(int i=0; i<rdbk.GetRoadBookSize(); i++){
        trialpath=rdbk.GetRoadBook()[i];


        int rand=rnd.Rannyu(0,5);
        if (rand==0) trialpath.Inversion(rnd);
        if (rand==1) trialpath.Shift(rnd);
        if (rand==2) trialpath.Swap(rnd);
        if (rand==3) trialpath.GroupShift(rnd);
        if (rand==4) trialpath.GroupSwap(rnd);

        //proposta
        trial=exp(-beta*(cities.GetDistance(trialpath)-cities.GetDistance(rdbk.GetRoadBook()[i])));
        if (trial>=rnd.Rannyu(0,1)) {
            rdbk.SetPath(trialpath, i);

        }

    }
}

void Anneal(RoadBook& rdbk, Sehenswurdigkeiten citieshape, int myrank){
    double T=T_MAX;
    //for (double t = T_MAX; t>T_MIN ; t-=T_STEPS){
    while (T>T_MIN ){
        if (myrank==0) cout << "T = " << T << endl;
        beta=1./T;
        for ( int i = 0; i<MOVES_PER_TEMPERATURE; i++){
            Mutate(rdbk, citieshape);
            if (MOVES_PER_TEMPERATURE%iprint==0) citieshape.PrintDistances("paths"+to_string(myrank)+".txt", rdbk, "A");
        }
        T=T-(T-T_MIN)/T_STEP-0.001;
    }

}

void PrintBest(RoadBook& rdbk, Sehenswurdigkeiten citieshape, string filename){
  Path bestpath=rdbk.GetRoadBook()[0];
  int bestindex=0;
  for (int i = 0; i < rdbk.GetRoadBookSize(); i++) {
      if ( citieshape.GetDistance(bestpath)>citieshape.GetDistance(rdbk.GetRoadBook()[i]) ){
         bestindex=i;
         bestpath=rdbk.GetRoadBook()[i];
      }
  }
  //bestpath.PrintPath();
  cout << "Best Path's length: " << citieshape.GetDistance(bestpath) << endl;
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
