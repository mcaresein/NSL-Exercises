#ifndef __genetic_h_
#define __genetic_h_
#include "random.h"
#include<vector>


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
class Path : private std::vector<int>{
    public:
        Path(int, Random&);     //costruisce una sentiero di città

        std::vector<int> GetPath();
        void SetPath(vector<int>);
        void SetElem(int, int);
        void DropLast();
        void Append(int);
        void Erase(int);
        iterator IsIn(int, int);
        int CheckPath();
        void PrintPath();
        int GetLength();

        //mutazioni
        void Swap(unsigned int, unsigned int);
        void Shift(unsigned int);
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
        RoadBook(unsigned int, unsigned int, Random&);
        vector<Path> GetRoadBook();
        int CheckRoadBook();//VOID O INT VISTO CHE CHECKPATH E' INT????
        int GetRoadBookSize();
        void Crossover(Random&);
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
      void PrintSehenswurdigkeiten(char*, Path);
    private:
      std::vector<City> sehenswurdigkeiten;
};




#endif
