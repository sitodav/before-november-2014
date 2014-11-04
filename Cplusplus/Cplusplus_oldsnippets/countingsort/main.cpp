#include "counting.h"
#include <ctime>
#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc,char *argv[]){
srand(time(NULL));
cout<<"ins size array"<<endl;
int size;
cin>>size;
int *a=new int[size];
int min,max;
cout<<endl;
for(int i=0;i<size;i++){
	*(a+i)=rand()%10;
	if(i==0){
		min=a[i];
		max=min;	
	}
		
	if(a[i]<min)
		min=a[i];
	else if(a[i]>max)
		max=a[i];
	cout<<a[i]<<" ";
}
cout<<endl;

counting sortingobj;
int *sorted=sortingobj.sort(a,size,min,max);
for(int i=0;i<size;i++)
	cout<<sorted[i]<<" ";
cout<<endl;	



}
