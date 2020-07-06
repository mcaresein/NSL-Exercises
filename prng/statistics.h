#include<vector>
#include<string>

#ifndef __Statistics__
#define __Statistics__

double Error(double, double, int);

class Statistics{
public:
   Statistics(int Nexperiments, int Nblocks);
   ~Statistics();

   void SetBlocks(int Nblocks);
   int GetBlocks();

   void UpdateAveAv2(double sum_of_observables_per_block, int block_index); //I want sum of measurements
   void PrintAveAv2();
   std::vector<double> GetAveAv2(int block_index);
   std::vector< std::vector <double> > GetAveAv2();

   void SaveBlockAverageVariance(std::string);//std::vector<double>, std::vector<double>, int, std::string);
   std::vector< std::vector<double> > BlockAverageVariance();//std::vector<double>, std::vector<double>, int);


private:
   std::vector<double> _ave;
   std::vector<double> _av2;
   int _Nblocks;
   int _NexperimentsPerBlock;
};

#endif

/*-----------------------------------------------------------------------------
this object contains average and average^2 to be used for Block averaging.
This is meant to be initialized in the beginning of the code.
Than the method UpdateAveAv2 is meant to be called on the end of each block.
It will update its internal variables storing average and square average  USING
THE PARTIAL (in-block) SUM OF THE OBSERVED QUANTITY  EXPLICITLY ACCOUNTED IN THE
MAIN. BlockAverageVariance calculates average and variance via blocking method.
------------------------------------------------------------------------------*/
