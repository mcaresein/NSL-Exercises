#include <iostream>
#include <string>

using namespace std;


int CheckPath(int* path, unsigned int size){
   sort(path, path+size);
   cout << "Check... ";
   int i=1;
   while(i<size && path[i-1]==i){ i++;}
   
   if(i==size){cout << " OK" << endl; return 0;}
   else {cerr << "Il vettore non è un sentiero!" << endl; return -1;}
}
void CheckMap(int** map, unsigned int map_size){
    for(int i=0; i<map_size; i++){
        cout << "Checking map's path " << i << "... ";
        CheckPath(map[i], sizeof(map[i])/sizeof(map[i][0]) ); 
    }

}

int main(){
    int nc=5;  //#cities
    int np=3;  //#paths
    int p[nc],q[nc],r[nc];
    int** map = new int*[np];
    
    for(int i=0; i<nc; i++) p[i] = i+1; 
    for(int i=0; i<nc; i++) q[i] = i;   
    for(int i=0; i<nc; i++) r[i] = i+1;
    map[0]=p; map[1]=q; map[2]=r;
    
    CheckMap(map,np);
    //CheckPath(p, nc);
    }

