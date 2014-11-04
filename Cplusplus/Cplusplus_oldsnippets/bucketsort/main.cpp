#include "bucket.h"
#include <iostream>
#include <cstdlib>
using namespace std;
#define n 5
#define a 0
#define b 1


int main(int argc,char *argv[]){

float toSort[n]={0.78,0.55,0.92,0.38,0.22};
bucket sorter;
float *res=sorter.sort(toSort,n,a,b);
for(int i=0;i<n;i++)
	cout<<res[i]<<" ";
cout<<endl;







}


