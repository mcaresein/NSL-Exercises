#include <iostream>
#include <string>

using namespace std;


void SwapInt(int* p, int a, int b){
    int temp=*(p+a);
    *(p+a)=*(p+b);
    *(p+b)=temp;
}

void RandomPermute(int* p, int size , int n){
    for(int i=size-1;i>0;i--){
        SwapInt(p,i,rand()%(i+1));
    }
}

int main()
{
    srand(1);
    int n=5;
    int p[n];

    for(int i=0; i<n; i++) *(p+i) = i;
    RandomPermute(p,  *(&p+1)-p,n);
    for(int i=0; i<n; i++) cout << *(p+i);
}

