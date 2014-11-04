#include <iostream>
#include <cstdlib>
#include <ctime>
#include "quick.h"

using namespace std;



int main(int argc,char *argv[]){
srand(time(NULL));
int *a=new int[10];
for(int i=0;i<10;i++){
	a[i]=rand()%100;
	cout<<a[i]<<" ";
}

cout<<endl<<endl;
QuickSort sortor;
sortor.sort(a,0,9);
for(int i=0;i<10;i++)
	cout<<a[i]<<" ";






}
