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

#include "statistics.h"
#include "random.h"

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


   //vector<double> head_d(3, 0.);
   //vector<double> head_c(3, 0.);

   int a=1;

   int NSteps=100;
   int N=10000;

   Statistics r2_d_Stat(N*NSteps, NSteps );
   Statistics r2_c_Stat(N*NSteps, NSteps );

   vector<vector<double> > head_d(3,vector<double> (N,0));
   vector<vector<double> > head_c(3,vector<double> (N,0));
   double r2_d=0, r2_c=0;

   //cout << << endl;
   for(int i=0;i<NSteps; i++){
      r2_d=0.; r2_c=0.;
      for(int n=0; n<N; n++){
        double n1=rnd.Rannyu();
        double n2=rnd.Rannyu();

        //-------------------discrete RW
        //cout << head_d[0][n] << " " << head_d[1][n] << " " << head_d[2][n] << endl;
        //attenzione che floor sballa quando è x.9...9 con più di 15 cifre!
        head_d[floor(n1*3)][n]+=pow(-1, floor(2*n2))*a;

        //cout << " " << r2_d << endl;
        r2_d+=head_d[0][n]*head_d[0][n]+head_d[1][n]*head_d[1][n]+head_d[2][n]*head_d[2][n];

        //--------------------continuous RW
        //cout <<<< head_c[0] << " " << head_c[1] << " " << head_c[2] << endl;
        head_c[0][n]+=a*sin(n1*M_PI)*cos(n2*2*M_PI);
        head_c[1][n]+=a*sin(n1*M_PI)*sin(n2*2*M_PI);
        head_c[2][n]+=a*cos(n1*M_PI);

        //cout << " " << r2_c << endl;
        r2_c+=head_c[0][n]*head_c[0][n]+head_c[1][n]*head_c[1][n]+head_c[2][n]*head_c[2][n];

      }
      r2_d_Stat.UpdateAveAv2(r2_d,i);
      r2_c_Stat.UpdateAveAv2(r2_c,i);

      // vector<double> head_c(3,0.);
      // for(int i=0;i<NSteps; i++){
      //    double n1=rnd.Rannyu();
      //    double n2=rnd.Rannyu();
      //
      //    //--------------------continuous RW
      //    //cout <<<< head_c[0] << " " << head_c[1] << " " << head_c[2] << endl;
      //    head_c[0]+=a*sin(n1*M_PI)*cos(n2*2*M_PI);
      //    head_c[1]+=a*sin(n1*M_PI)*sin(n2*2*M_PI);
      //    head_c[2]+=a*cos(n1*M_PI);
      //
      //    //cout << " " << r2_c << endl;
      //    r2_c=head_c[0]*head_c[0]+head_c[1]*head_c[1]+head_c[2]*head_c[2];
      //    r2_c_Stat.UpdateAveAv2(r2_c,i);
      //
      // }
 }
   r2_d_Stat.PrintAveAv2();
   r2_c_Stat.PrintAveAv2();

   //ATTENZIONE: SALVA <R^2> E LA SUA STD_DEV!!!!!
   r2_d_Stat.SaveBlockAverageVariance("discrete.txt");
   r2_c_Stat.SaveBlockAverageVariance("continuous.txt");

   // double rms;
   // ofstream discrete("discrete.txt");
   // if (discrete.is_open()){
   //    for(int i=0; i<NSteps; i++){
   //       //cout << ave_r2_d[i] << endl;
   //       rms=sqrt(ave_r2_d[i]);
   //       discrete << i << " " << rms << " " << sqrt( sqrt( (av2_r2_d[i] - pow(ave_r2_d[i],2))/i ) )<< endl;
   //    }
   // } else cerr << "PROBLEM: Unable to open discrete.txt" << endl;
   // discrete.close();
   //
   // ofstream continuum("continuum.txt");
   // if (continuum.is_open()){
   //    for(int i=0; i<NSteps; i++){
   //       //cout << ave_r2_d[i] << endl;
   //       rms=sqrt(ave_r2_c[i]);
   //       continuum << i << " " << rms << " " << sqrt( sqrt( (av2_r2_c[i] - pow(ave_r2_c[i],2))/i ) )<< endl;
   //    }
   // } else cerr << "PROBLEM: Unable to open continuum.txt" << endl;
   // continuum.close();

   rnd.SaveSeed();
   return 0;
}



void UTStatistica(Random rnd){ //FAI IL SERIO E PASSA BY REFERENCE
   //UNIT TEST SULLA DISTRIBUZIONE UNIFORME PER LA ClASSE STATISTICA
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
