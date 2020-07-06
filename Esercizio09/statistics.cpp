#include"statistics.h"
#include<iostream>
#include <fstream>
#include<cmath>
#include<algorithm>

using namespace std;

double Error(double ave, double av2, int n){
   if(n==0) return 0;
   else return sqrt((av2 - pow(ave,2) )/n);
}

/* Every Random Number extraction is a "throw".
An "experiment" is a measurement of the interesting quantity every Nthrow throws.
A " block " is a set of experiments Nexperiments
*/

Statistics::Statistics(int Nexperiments, int Nblocks){
   _ave.resize(Nblocks,0);
   _av2.resize(Nblocks,0);
   _Nblocks=Nblocks;
   _NexperimentsPerBlock=Nexperiments/Nblocks;
   cout << "You are going to perform  " << _NexperimentsPerBlock << " Experiments per block. \n Aren't you?! You'd better do it..." << endl;
}

Statistics::~Statistics(){}

void Statistics::SetBlocks(int Nblocks){_Nblocks=Nblocks;}
int Statistics::GetBlocks(){return _Nblocks;}

void Statistics::UpdateAveAv2(double sum_of_observables_per_block, int block_index){
   _ave[block_index]=sum_of_observables_per_block/_NexperimentsPerBlock; //mean inside a block
   _av2[block_index]=_ave[block_index]*_ave[block_index];
}

void Statistics::PrintAveAv2(){
   for (int i = 0; i < _Nblocks; i++) {
      cout << _ave[i] << " " << _av2[i] << endl;
   }
}

vector<double> Statistics::GetAveAv2(int block_index){
   vector<double> x={_ave[block_index], _av2[block_index]};
   return x;
}

vector<vector <double> > Statistics::GetAveAv2(){
   vector< vector<double > > x={_ave, _av2};
   return  x;
}

std::vector< std::vector<double> > Statistics::BlockAverageVariance(){//vector<double> ave, vector<double> av2, int Nblocks){
   //if(ave.size() != av2.size()){cerr<< "AVE and AV2 have different size!" << endl; exit(-1);}



   int dim=_ave.size();
   vector<double> sum_prog(dim,0);
   vector<double> su2_prog(dim,0);
   vector<double> err_prog(dim,0);
   for(int i=0;i<_Nblocks; i++){
      for(int j=0; j<i+1; j++){
        sum_prog[i] += _ave[j]; // SUM_{j=0,i} r_j;
        su2_prog[i] += _av2[j]; //SUM_{j=0,i} (r_j)^2;
      }
      sum_prog[i]/=(i+1); // Cumulative average
      su2_prog[i]/=(i+1); // Cumulative square average
      err_prog[i] = Error(sum_prog[i],su2_prog[i],i); // Statistical uncertainty
      //cout << i << " error: " << err_prog[i] << endl;;
   }

   //BUGGGG! accessibile fuori dal dominio senza dare errori. perchè?!?!?
   //cout << _ave[19] << " ave size: " << _ave.size() << endl;

   vector< vector<double > > averagevariance={sum_prog, err_prog };
   //averagevariance[0]=sum_prog;
   //averagevariance[1]=err_prog;
   return averagevariance;

}

void Statistics::SaveBlockAverageVariance(string filename){ //vector<double> ave, vector<double> av2, int Nblocks, string filename){
   ofstream newfile(filename);
   int dim=_ave.size();
   vector< vector <double> > averagevariance(2);
   averagevariance.resize(2,vector<double> (dim,0));
   averagevariance=this->BlockAverageVariance();

   for(int i=0; i< dim; i++){
      newfile << i << " " << averagevariance[0][i] << " " << averagevariance[1][i] << endl;
   }
   newfile.close();
}
