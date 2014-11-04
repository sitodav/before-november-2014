#include <iostream>
#include <cstdlib>
#include <ctime>
#define n 4
#define max_w 9
using namespace std;


int w[n]={2,1,5,3},v[n]={6,4,8,4};


int main(int argc,char *argv[]){
int **tab=new int*[n];
for(int i=0;i<n;i++){
	
	tab[i]=new int[max_w+1];
	for(int j=0;j<=max_w;j++){
		tab[i][j]=-1;
	}
		
}

for(int i=n-1;i>=0;i--)
	if(w[i]<=max_w)
		tab[i][max_w-w[i]]=v[i];



cout<<endl;


for(int i=n-1;i>0;i--){
	for(int j=max_w;j>=0;j--){
		
		if(tab[i][j]==-1)
			continue;
		if(tab[i-1][j]<tab[i][j])
			tab[i-1][j]=tab[i][j];	
	
		if(j>=w[i-1] && tab[i][j]+v[i-1]>tab[i-1][j-w[i-1]])
			tab[i-1][j-w[i-1]]=tab[i][j]+v[i-1];


	}

}

for(int i=0;i<n;i++){
	for(int j=0;j<=max_w;j++){
		if(tab[i][j]==-1)
			cout<<"x"<<" ";
		else
			cout<<tab[i][j]<<" ";
	}
	cout<<endl;

}






}
