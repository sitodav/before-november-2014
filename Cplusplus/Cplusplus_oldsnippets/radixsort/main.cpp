#include "radix.h"
#include <cstdlib>
#include <iostream>
#include <ctime>
#define n 5
#define m 3

using namespace std;

int main(int argc,char *argv[]){
srand(time(NULL));
int **toSort=new int *[n];

for(int i=0;i<n;i++){
	*(toSort+i)=new int[m];
	for(int j=0;j<m;j++){
		toSort[i][j]=rand()%10;
		cout<<toSort[i][j];	
	}
	cout<<endl;
}

radix sorter;
int **sorted=sorter.sort(toSort,n,m);
cout<<"----------------------"<<endl;
for(int j=0;j<n;j++){
	for(int i=0;i<m;i++)
		cout<<sorted[j][i];
	cout<<endl;
}

}
