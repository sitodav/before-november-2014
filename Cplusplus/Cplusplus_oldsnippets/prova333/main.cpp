#include <cstdlib>
#include <iostream>
#include <ctime>
#define n 5
using namespace std;






int **tab;
int conteggi[n][n]={-1};

int count(int i,int j){
	
	if(j==i)
		return 0;
	if(j==i+1){
		conteggi[i][j]=tab[i][j];
		return tab[i][j];
	}
	int min=1000,a;	
	for(int k=0;k<j;k++){
		cout<<j-1<<endl;
		if(conteggi[k][i]==-1)
			conteggi[k][j]=tab[k][j]+count(i,k);
		if(conteggi[k][j]<min)
			min=conteggi[k][j];	
	}
	return min;

}


int main(int argc,char *argv[]){

srand((unsigned int)time(NULL));
tab=new int*[n];
for(int i=0;i<n;i++){
	tab[i]=new int[n];
	for(int j=0;j<n;j++){
		conteggi[i][j]=-1;
		if(j>i)
			tab[i][j]=1+rand()%5;
		else tab[i][j]=0;
		cout<<tab[i][j]<<" ";
	}
	cout<<endl;
	
}

for(int j=0;j<n;j++){
	for(int i=0;i<n;i++)
		if(j>i){
			cout<<i<<" -> "<<j<<" a costo "<<tab[i][j]<<endl;		
		}
}

tab[0][n-1]=28;
cout<<"----->"<<count(0,n-1)<<endl;


for(int j=0;j<n;j++){
	for(int i=0;i<n;i++)
		cout<<conteggi[j][i]<<" ";
	cout<<endl;
}






}
