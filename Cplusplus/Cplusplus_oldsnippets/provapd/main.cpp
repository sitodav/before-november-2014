#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#define n 10

using namespace std;

void sel_sort(int *a,int *b){
	for(int i=0;i<n;i++)
		b[i]=a[i];

	for(int i=1;i<n;i++){
		int j=i-1;
		int toins=b[i];
		while(j>=0 && b[j]>toins){
			b[j+1]=b[j];
			j--;		
		}
		b[j+1]=toins;		
	}
}



int greedy_alg(int *a,vector<int> &vect1,int max){
int *b=new int[n];

sel_sort(a,b);
int card=0;
int i=0,res=max;

while(i<n){
	if(b[i]<=res){
		vect1.push_back(b[i]);
		
		card++;
		res-=b[i];
		i++;	
	}
	else break;
		
}

return card;

}

int main(int argc,char *argv[]){
vector<int> vect1;
int *files=new int[n];
srand((unsigned int)time(NULL));
for(int i=0;i<n;i++)
	*(files+i)=1+rand()%40;
int max=10+rand()%100;

for(int i=0;i<n;i++)
		cout<<files[i]<<" ";
cout<<endl<<"max capacity = "<<max<<endl;

cout<<"la max cardinalita' e' "<<greedy_alg(files,vect1,max)<<endl;
for(vector<int>::iterator it=vect1.begin();it!=vect1.end();it++)
	cout<<*it<<endl;



}

