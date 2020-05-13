#include <iostream>
#include <string>

using namespace std;

// Example program
#include <iostream>
#include <algorithm>

using namespace std;

int CheckPath(int* path, int size){
   sort(path, path+size);
   cout << "Check... ";
   int i=1;
   while(i<size && path[i-1]==i){ cout << "loop" << i<< endl; i++;}
   cout << i;
   //while(i<=size){ cout << path[i]; i++;}

   if(i==size){cout << " OK" << endl; return 0;}
   else {cerr << "Il vettore non è una mappa!" << endl; return -1;}
}



int main(){
    int n=5;
    int p[n];
    for(int i=0; i<n; i++) p[i] = i*i;
    CheckPath(p, n);
}
//void SwapInt(int* p, int a, int b){
//    int temp=*(p+a);
//    *(p+a)=*(p+b);
//    *(p+b)=temp;
//}
//
//void RandomPermute(int* p, int size , int n){
//    for(int i=size-1;i>0;i--){
//        SwapInt(p,i,rand()%(i+1));
//    }
//}
//
//int main()
//{
//    srand(1);
//    int n=5;
//    int p[n];
//
//    for(int i=0; i<n; i++) *(p+i) = i;
//    RandomPermute(p,  *(&p+1)-p,n);
//    for(int i=0; i<n; i++) cout << *(p+i);
//}
//
