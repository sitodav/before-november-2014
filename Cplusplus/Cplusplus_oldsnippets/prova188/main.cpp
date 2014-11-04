#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>

#define n 4

using namespace std;


namespace mine{
int min(int a,int b,int c=10000){
	
	return ( (a<b) ? ( (a<c) ? a : c )   : ( ( b<c) ? b : c ) );
	
}
}

string path;
int **t;

int count(int i,int j,char prov='f'){
	stringsream ss(stringstream::in | stringstream::out);
	int a,b,c,min;
	if(j==0)
		return t[i][j];
	
	switch(prov){
		case 's':
			
			if(j>0)
				a=count(i,j-1,'d');
			if(i<n-1)
				b=count(i+1,j,'s');
			min=+mine::min(a,b);
			if(min==a)
				path+='d';
			else if(min==b)
				path+='g';
			return t[i][j]+min;
			break;
		case 'g':
			
			if(i>0)
				a=count(i-1,j,'g');
			if(j>0)
				b=count(i,j-1,'d');
			min=mine::min(a,b);
			if(min==a)
				path+='g';
			else if(min==b)
				path+='d';
			return t[i][j]+min;
			break;
		default:
			
			if(i>0)
				a=count(i-1,j,'g');
			if(i<n-1)
				b=count(i+1,j,'s');
			if(j>0)
				c=count(i,j-1,'d');
			min=mine::min(a,b,c);
			if(min==a)
				path+='g';
			else if(min==b)
				path+='s';
			else path+='d';
			return t[i][j]+min;
			break;
			
		
	}
	

}



int uscita(){


int minimo=10000,a;
for(int i=0;i<n;i++){
	a=count(i,n-1);
	if(a<minimo)
		minimo=a;
}

return minimo;

}

int main(){ 
srand(time(NULL));
t=new int*[n];
path=string(" ");
for(int i=0;i<n;i++){
	*(t+i)=new int[n];
	for(int j=0;j<n;j++){
		*(*(t+i)+j)=rand()%5;
		cout<<t[i][j]<<" ";	
	}
	cout<<endl;	
}

cout<<uscita()<<endl;	
cout<<path<<endl;


}
