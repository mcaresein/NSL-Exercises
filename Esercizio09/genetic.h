#ifndef __genetic_h_
#define __genetic_h_
#include "random.h"
#include <vector>


struct City{
public:
    double x, y;
};

//funzioni per la simulazione
void Move();
void Input();
void Reset();

//funzioni ausiliarie per le classi
int IndexPBC(int);
std::vector<int> Swap(int*, int, int);
void RandomPermute(int*, int);

//oggetti da simulare
class Path : public std::vector<int>{
    public:
        Path(){};
        Path(int);
        Path(int, Random&);     //costruisce una sentiero di città

        std::vector<int> GetPath();
        void SetPath(vector<int>);
        void SetElem(int, int);
        int CheckPath(bool); //true for silent check (!verbose)
        void PrintPath();
        int GetLength();

        //mutazioni
        void Swap(unsigned int, unsigned int);
        void Shift(unsigned int);

        void Swap(Random&);
        void Shift(Random&);
        void GroupSwap(Random&);
        void GroupShift(Random&);
        void Inversion(Random&);


        using vector::begin;
        using vector::end;
        using vector::iterator;

    private:
        std::vector<int> path;
};

class RoadBook: public Path {
   public:
      RoadBook(unsigned int, unsigned int);
      RoadBook(unsigned int, unsigned int, Random&);
      RoadBook(const RoadBook&);

      RoadBook& operator=(const RoadBook&);

      vector<Path>  GetRoadBook() const;
      void SetPath(Path, int);
      int CheckRoadBook(bool );
      int  GetRoadBookSize() const;
      void PrintRoadbook();
      void Crossover(Random&, int, int,bool);
      void Mutate(Random&, int, int,bool);

   private:
      std::vector<Path> roadbook;
};

class Sehenswurdigkeiten{
    public:
      Sehenswurdigkeiten(std::vector<double>, std::vector<double>);
      std::vector<City> GetSehenswurdigkeiten();
      int GetLength();
      double GetDistance(Path);
      double GetDistance2(Path);
      void PrintSehenswurdigkeiten(std::string, Path, std::string mode);
    private:
      std::vector<City> sehenswurdigkeiten;
};




#endif
