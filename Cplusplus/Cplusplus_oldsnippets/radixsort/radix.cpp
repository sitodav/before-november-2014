#include <iostream>
#include "radix.h"

using namespace std;



int **radix::sort(int **a,int n,int m){
	
	int **final=new int*[n];
	for(int j=0;j<n;j++)
		*(final+j)=new int[m];
	

	int **b=new int *[n];
	for(int j=0;j<n;j++)
		*(b+j)=new int[m];	
	cout<<endl;
	//a>final
	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++)
			*(*(final+i)+j)=*(*(a+i)+j);
					
		
		
	
	for(int ord=m-1;ord>=0;ord--){
		
		counting_sort(final,b,n,m,ord); //final>countingsort>b
		//b>final
		for(int i=0;i<n;i++)
			for(int j=0;j<m;j++)
				*(*(final+i)+j)=*(*(b+i)+j);
				
	}		
			
	return final;
	
}


void radix::counting_sort(int **a,int **b,int n,int m,int ord){
	int *count=new int[10];
	for(int i=0;i<10;i++)
		count[i]=0;

	for(int j=0;j<n;j++)
		count[ a[j][ord]  ]++;

	for(int j=1;j<10;j++)
		count[j]+=count[j-1]; 

	for(int j=n-1;j>=0;j--)
		sistema(a,b,count,j,ord,m);		

}



void radix::sistema(int **a,int **b,int *c,int whotoput,int ord,int m){
	int wheretoput=c[a[whotoput][ord]]-1;
	c[a[whotoput][ord]]--;
	for(int j=0;j<m;j++)
		b[wheretoput][j]=a[whotoput][j];

}
