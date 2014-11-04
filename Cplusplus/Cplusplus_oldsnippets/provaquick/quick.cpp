#include "quick.h"

void QuickSort::sort(int a[],int p,int u){
		if(p<u){
			int q=partiziona(a,p,u);
			sort(a,p,q);
			sort(a,q+1,u);			

		}
	}


void QuickSort::swap(int a[],int i,int j){
		int temp=a[i];
		a[i]=a[j];
		a[j]=temp;

	}

int QuickSort::partiziona(int a[],int p,int u){
		int i=p,j=u,piv=a[p];
		while(1){
			while(a[i]<piv)
				i++;
			while(a[j]>piv)
				j--;
			if(i<j)
				swap(a,i,j);
			else break;			
				
		}
		return j;
	}
