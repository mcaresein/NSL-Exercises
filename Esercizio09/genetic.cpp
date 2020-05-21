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

Path::Path(int ncities, Random& rnd) {
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
    if(path.size()!=newpath.size()) {cerr << "I due path non sono lunghi uguali " << endl; exit(-1);}
    for(int i=0; i< path.size(); i++) path[i]=newpath[i];
  }
void Path::SetElem(int element, int index){path[index]=element;}
void Path::DropLast(){path.erase(path.end()-1);}
void Path::Append(int elem){path.insert(path.begin(),elem); this->Shift(1);}
int Path::GetLength(){return path.size();}
void Path::Erase(int i){path.erase(path.begin()+i);}
Path::iterator Path::IsIn(int elem, int index){return find(path.begin()+index ,path.end(),elem); }


int Path::CheckPath(){
   vector<int> temp=path;
   int path_size=path.size();
   sort(temp.begin(), temp.end());
   cout << "Check... ";
   int i=0;
   while(i<path_size && temp[i]==i){i++;}
   if(i==path_size){cout << " OK" << endl; return 0;}
   else {cerr << "Il vettore non rappresenta un sentiero!" << endl; exit(-1);}
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





RoadBook::RoadBook(unsigned int npaths, unsigned int ncities, Random& rnd): Path(ncities, rnd){
    for(int i=0; i<npaths; i++){
        roadbook.push_back(Path(ncities, rnd));
    }
    //roadbook=newrdbk;
}

vector<Path> RoadBook::GetRoadBook(){return roadbook;}

int RoadBook::GetRoadBookSize(){return roadbook.size();}

int RoadBook::CheckRoadBook(){       //O INT VISTO CHE CHECKPATH E' INT????
    for(int i=0; i<this->GetRoadBookSize(); i++) {
        cout << "Checking roadbook's path " << i << "... ";
        roadbook[i].CheckPath();
    }
    return 0;
}

void RoadBook::Crossover(Random& rnd){
  int i= rnd.Rannyu(0, this->GetRoadBookSize());
  int j= rnd.Rannyu(0, this->GetRoadBookSize());
  if (i=j) j= rnd.Rannyu(0, this->GetRoadBookSize());
  cout << i << j << endl;

  vector<int> mom=roadbook[i].GetPath();  vector <int> om=mom;
  vector<int> dad=roadbook[j].GetPath();  vector <int> ad=dad;
  cout << "ahj" << roadbook[i].GetLength()<< endl;
  int cut=rnd.Rannyu(0, roadbook[i].GetLength());
  //cout << cut << endl;

  //roadbook[4].PrintPath();


  // for(int k=0; k<mom.size(); k++) cout << mom[k];
  // cout << endl;
  // for(int k=0; k<mom.size(); k++) cout << dad[k];
  // cout << endl;

  for(int k=0; k<om.size();  k++){
      if (find(mom.begin()+cut,mom.end(),ad[k]) == mom.end()){
         ad.erase(ad.begin()+k); k--;
      }
  }

  for(int k=0; k<ad.size();  k++){
      if (find(dad.begin()+cut,dad.end(),om[k]) == dad.end()){
        om.erase(om.begin()+k); k--;
      }
  }

  for(int k=0; k<mom.size(); k++){
      mom[k+cut]=ad[k];
      dad[k+cut]=om[k];
  }
  // for(int k=0; k<mom.size(); k++) cout << mom[k];
  // cout << endl;
  // for(int k=0; k<mom.size(); k++) cout << dad[k];
  // cout << endl;

  roadbook[i].SetPath(mom);
  roadbook[j].SetPath(dad);

  //roadbook[j].CheckPath();

}

void RoadBook::Mutate(Random& rnd){

  MutationsPTR mutations[5]={&Path::Inversion, &Path::Shift, &Path::Swap, &Path::GroupSwap, &Path::GroupShift};

  vector<int> mutations_index = {0,1,2,3,4}; //indici delle mutazioni
  for(int i=0; i<this->GetRoadBookSize(); i++){
      random_shuffle(mutations_index.begin(), mutations_index.end());
      for(int j=4; j>=0; j--){/* ciclo sul numero di mutazioni*/
          if (rnd.Rannyu()>0.9){
            (roadbook[i].*mutations[mutations_index[j]])(rnd);
            //cout << mutations_index[j];
          }
          else j=-1;
      }
      //cout << endl;
  }



  //roadbook[0].PrintPath();

}



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
void  Sehenswurdigkeiten::PrintSehenswurdigkeiten(char* filename, Path path){
    ofstream file;
    file.open(filename);
    int size=path.GetLength();

    for(int i=0; i<size; i++){
        int index=path.GetPath()[i];
        file << sehenswurdigkeiten[index].x << "   " << sehenswurdigkeiten[index].y <<endl;
    }
    file << sehenswurdigkeiten[path.GetPath()[0]].x << "   " << sehenswurdigkeiten[path.GetPath()[0]].y <<endl;

    file.close();

    //rnd.SaveSeed();
}
