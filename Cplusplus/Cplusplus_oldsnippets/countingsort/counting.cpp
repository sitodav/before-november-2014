#include "counting.h"
#include <cassert>
#include <iostream>
#include <cstdlib>

using namespace std;

int * counting::sort(int *a,int size,int rSx,int rDx){
	
	
	int *c=new int[rDx+1];
	int *b=new int[size];
	for(int i=rSx;i<=rDx;i++)
		c[i]=0;
	
	for(int i=0;i<size;i++){
		c[a[i]]+=1;	
	}
	
	for(int i=rSx+1;i<=rDx;i++)
		c[i]+=c[i-1];
	for(int i=0;i<rDx+1;i++){
		cout<<c[i]<<" ";	
	}
	for(int i=size-1;i>=0;i--){
		b[c[a[i]]-1]=a[i];
		c[a[i]]--;	
	}


	
	cout<<endl;
	return b;

}




