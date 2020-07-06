#include "genetic.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <fstream>

using namespace std;


typedef  void (Path::* MutationsPTR) (Random&);
enum MUTATIONS { INVERSION=0, SHIFT=1, SWAP=2, GROUPSWAP=3, GROUPSHIFT=4};

//funzione che miracolosamente mette le pbc sull'indice i di un vettore lungo length
int IndexPBC(int i, int length){
  return int(i+length*ceil(abs(i)/length))%length;
}

vector<int> Swap(vector<int> p, int a, int b){
    int temp=p[a];
    p[a]=p[b];
    p[b]=temp;
    return p;
}



//================================== PATH =====================================//


Path::Path(int ncities){ path.resize(ncities,0);}
Path::Path(int ncities, Random& rnd){
    path = vector<int>(ncities,0);
    for(int i=0; i<ncities; i++) path[i] = i;
    for(int i=path.size()-1;i>0;i--){
         int randomindex=rnd.Rannyu(0,path.size());
         path=::Swap(path,i,randomindex);
     }
}

void Path::PrintPath(){ for (int i=0; i<path.size(); i++) cout << path[i] << " "; cout << endl;};
vector<int> Path::GetPath(){return path;}
void Path::SetPath(vector<int> newpath){
   if (newpath.size()!=path.size()){ cerr << "ERROR: Path::SetPath \nI due path non sono lunghi uguali"; exit(-1);}
   for (int i = 0; i < path.size(); i++) {
      path[i]=newpath[i];
   }
}
int Path::GetLength(){return path.size();}

int Path::CheckPath(bool silent){
   vector<int> temp=path;
   int path_size=path.size();
   sort(temp.begin(), temp.end());
   if(!silent) cout << "Check... ";
   int i=0;
   while(i<path_size && temp[i]==i){i++;}
   if(!silent) cout << " OK" << endl;
   if(i==path_size){return 0;}
   else {cerr << "ERROR: Path::CheckPath \nIl vettore non rappresenta un sentiero!" << endl; exit(-1);}
}

//mutazioni
void Path::Swap(unsigned int index_a, unsigned int index_b){
    path=::Swap(path, IndexPBC(index_a, path.size()), IndexPBC(index_b, path.size()) );
}

void Path::Shift(unsigned int nshifts){     // cicli antiorari : (012)->(120)
    int ishifts=0;
    while(ishifts<nshifts){
         for(int ilength=0; ilength<path.size()-1; ilength++){ this->Swap(IndexPBC(ilength, path.size()), IndexPBC(1+ilength, path.size()) );}
    ishifts++;
    }
}

void Path::Swap(Random& rnd){this->Swap(rnd.Rannyu(0, this->GetLength()), rnd.Rannyu(0, this->GetLength()));}

void Path::Shift(Random& rnd){this->Shift(rnd.Rannyu(0, this->GetLength()));}

void Path::GroupShift(Random& rnd){ //warning floating point exception
   int begin=rnd.Rannyu(0, path.size()/2.);
   int end=rnd.Rannyu(begin, path.size());
   int index=rnd.Rannyu(begin, end);
   //cout << begin << " " << index << " " << end << endl;

   rotate(path.begin()+begin, path.begin() + index, path.begin() + end);
}

void Path::GroupSwap(Random& rnd){
      int half_size=path.size()/2.;
      int begin=rnd.Rannyu(0, half_size);
      int end=rnd.Rannyu(begin, half_size);
      for(int i=begin; i<end; i++) this->Swap(i,half_size+i);
}

void Path::Inversion(Random& rnd){
   unsigned int end=rnd.Rannyu(0,path.size());
   unsigned int start=rnd.Rannyu(0,end);
   //cout << start << " " << end <<  endl;
   for(int i=0; i<(end-start+1)/2; i++){ this->Swap(start+i, end-i);
   }
}


//=================================== ROADBOOK ===================================//

RoadBook::RoadBook(unsigned int npaths, unsigned int ncities): Path(ncities){
    for(int i=0; i<npaths; i++){
        roadbook.push_back(Path(ncities));
    }
}

RoadBook::RoadBook(unsigned int npaths, unsigned int ncities, Random& rnd): Path(ncities, rnd){
    for(int i=0; i<npaths; i++){
        roadbook.push_back(Path(ncities, rnd));
    }
}

RoadBook::RoadBook(const RoadBook& rdbk){
    for(int i=0; i<rdbk.GetRoadBookSize(); i++){
        roadbook.push_back(rdbk.GetRoadBook()[i]);
    }
}


RoadBook& RoadBook::operator=(const RoadBook& rdbk){
   for (size_t i = 0; i < rdbk.GetRoadBookSize(); i++) {
      this->SetPath(rdbk.GetRoadBook()[i],i);
   }
   return *this;
}

vector<Path> RoadBook::GetRoadBook() const {return roadbook;}

int RoadBook::GetRoadBookSize() const {return roadbook.size();}

void RoadBook::SetPath(Path path, int index){
   if(path.GetLength()!=this->GetRoadBook()[index].GetLength()) {cerr << "ERROR: Roadbook::SetPath: I due path non sono lunghi uguali: lunghezza del sostituito: " <<  roadbook[index].GetLength() << " e lunghezza del sostituto " << path.GetLength() << endl; exit(-1);}
   roadbook[index]=path;
}

int RoadBook::CheckRoadBook(bool silent){
    for(int i=0; i<this->GetRoadBookSize(); i++) {
        if(!silent) cout << "Checking roadbook's path " << i << "... ";
        roadbook[i].CheckPath(silent);
    }
    return 0;
}

void RoadBook::PrintRoadbook(){
   for(int i=0; i<this->GetRoadBookSize(); i++){
      this->GetRoadBook()[i].PrintPath();
   }
}

void RoadBook::Crossover(Random& rnd, int min_index, int min2_index,bool v){ //(mom, dad) -> (mad, dom)
   if(v) cout << min_index << min2_index << endl;
   std::cout << "Crossover" << '\n';
   vector<int> mom=roadbook[min_index].GetPath();  vector <int> om=mom;
   vector<int> dad=roadbook[min2_index].GetPath();  vector <int> ad=dad;

   int cut=rnd.Rannyu(0, roadbook[min_index].GetLength());
   if(v) cout << "cut = " << cut<< endl;

   if(v){
      cout << "mom = ";
      for(int k=0; k<mom.size(); k++){
         cout << mom[k] << " ";
      }cout << endl;

      cout << "dad = ";
      for(int k=0; k<dad.size(); k++){
         cout << dad[k] << " ";
      }cout << endl;
   }

   for(int k=0; k<ad.size();  k++){
      if (find(mom.begin()+cut,mom.end(),ad[k]) == mom.end()){
         ad.erase(ad.begin()+k); k--;
      }
   }

   for(int k=0; k<om.size();  k++){
      if (find(dad.begin()+cut,dad.end(),om[k]) == dad.end()){
        om.erase(om.begin()+k); k--;
      }
   }
   if(v){
      cout << " ad = ";
      for(int k=0; k<ad.size(); k++){
         cout << ad[k] << " ";
      }cout << endl;

      cout << " om = ";
      for(int k=0; k<om.size(); k++){
         cout << om[k] << " ";
      }cout << endl;
   }

   for(int k=0; k<om.size(); k++){
      mom[k+cut]=ad[k];  //which is mad, here for convenience
      dad[k+cut]=om[k];  //which is dom, here for convenience
   }

   roadbook[min_index].SetPath(mom);
   roadbook[min2_index].SetPath(dad);

   if(v){
      cout << "mad = ";
      for(int k=0; k<mom.size(); k++){
        cout << mom[k] << " ";
      }cout << endl;
      cout << "dom = ";
      roadbook[min2_index].PrintPath();
      std::cout << '\n';
   }
}

void RoadBook::Mutate(Random& rnd, int min_index, int min2_index, bool v){

   MutationsPTR mutations[5]={&Path::Inversion, &Path::Shift, &Path::Swap, &Path::GroupSwap, &Path::GroupShift};
   vector<int> mutations_index = {0,1,2,3,4}; //indici delle mutazioni

   //mutate the first
   random_shuffle(mutations_index.begin(), mutations_index.end());
   cout << "mutazione del primo: ";
   for(int j=4; j>=0; j--){/* ciclo sul numero di mutazioni*/
      if (rnd.Rannyu()>0.9){
         (roadbook[min_index].*mutations[mutations_index[j]])(rnd);
         cout << mutations_index[j];
      }
      else j=-1;
   }
   cout << endl;
   //mutate the second
   random_shuffle(mutations_index.begin(), mutations_index.end());
   cout << "mutazione del secondo: ";
   for(int j=4; j>=0; j--){/* ciclo sul numero di mutazioni*/
      if (rnd.Rannyu()>0.9){
         (roadbook[min2_index].*mutations[mutations_index[j]])(rnd);
         cout << mutations_index[j];
      }
      else j=-1;
   }
   cout << endl;
}

//============================= SEHENSWURDIGKEITEN =============================//

Sehenswurdigkeiten::Sehenswurdigkeiten(vector<double> x_of_cities, vector<double> y_of_cities){
    vector<City> m_sehenswurdigkeiten(x_of_cities.size());
    for(int i=0; i<x_of_cities.size(); i++){
        m_sehenswurdigkeiten[i].x=x_of_cities[i];
        m_sehenswurdigkeiten[i].y=y_of_cities[i];
    }
    sehenswurdigkeiten=m_sehenswurdigkeiten;
}

vector<City> Sehenswurdigkeiten::GetSehenswurdigkeiten(){
    return sehenswurdigkeiten;
}

int Sehenswurdigkeiten::GetLength(){return sehenswurdigkeiten.size();}

double Sehenswurdigkeiten::GetDistance(Path path){
    double dist=0.0;
    int j, k;
    for(int i=0; i<this->GetLength(); i++){
        j=path.GetPath()[IndexPBC(i, path.GetLength())];
        k=path.GetPath()[IndexPBC(i+1, path.GetLength())];
        dist += sqrt( (sehenswurdigkeiten[j].x-sehenswurdigkeiten[k].x)*(sehenswurdigkeiten[j].x-sehenswurdigkeiten[k].x) +
              (sehenswurdigkeiten[j].y-sehenswurdigkeiten[k].y)*(sehenswurdigkeiten[j].y-sehenswurdigkeiten[k].y) );
        }
     return dist;
}

double Sehenswurdigkeiten::GetDistance2(Path path){
    double dist=0.0;
    int j, k;
    for(int i=0; i<this->GetLength(); i++){
        j=path.GetPath()[IndexPBC(i, path.GetLength())];
        k=path.GetPath()[IndexPBC(i+1, path.GetLength())];
        dist+=(sehenswurdigkeiten[j].x-sehenswurdigkeiten[k].x)*(sehenswurdigkeiten[j].x-sehenswurdigkeiten[k].x) +
              (sehenswurdigkeiten[j].y-sehenswurdigkeiten[k].y)*(sehenswurdigkeiten[j].y-sehenswurdigkeiten[k].y) ;
        }
     return dist;
}
void  Sehenswurdigkeiten::PrintSehenswurdigkeiten(string filename, Path path, string mode){
    ofstream file;
    if (mode=="A") file.open(filename,ios::app);
    else if (mode == "W") file.open(filename);
    else {cerr << "ERROR: Sehenswurdigkeiten::PrintSehenswurdigkeiten \nUnable to open file: no mode specified or mispelled" << endl; exit(-1);}

    int size=path.GetLength();

    for(int i=0; i<size; i++){
        int index=path.GetPath()[i];
        file << sehenswurdigkeiten[index].x << "   " << sehenswurdigkeiten[index].y <<endl;
    }
    file << sehenswurdigkeiten[path.GetPath()[0]].x << "   " << sehenswurdigkeiten[path.GetPath()[0]].y <<endl;

    file.close();

    //rnd.SaveSeed();
}
