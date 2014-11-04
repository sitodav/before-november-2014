#include "alg.h"
#include <ctime>
#include <iostream>
#include <cstdlib>

using namespace std;

void alg::alloca_riempi(){
	srand(time(NULL));
	a=new int[size];
	cout<<endl;
	for(int i=0;i<size;i++){
		*(a+i)=rand()%100;
		cout<<a[i]<<" ";
	}

}
//overloading di funz, noi dall'esterno lanciamo solo la funz senza parametri, pero' non e' quella usata davvero, poiche' per la ricorsione bisogna passare ogni volta gli indici da stringere. Quindi da funz chiamiamo la funz con intp e int u come parametri, e gli passiamo come parametri gli estremi che almeno la prima volta soddisfano che in a[1]<a[n]

int alg::funz(){ 
	int i=0;
	while(a[i]>a[size-1])
		i++;
	return funz(i,size-1);

}

int alg::funz(int p,int u){
	if(p==u-1 && a[p]<a[u])
		return p;
	else{
		int mediano=(p+u)/2;
		if(a[mediano]<a[u])
			return 	funz(mediano,u);
		else if(a[mediano]>a[p])
			return funz(p,mediano);
		else return -1;
	}
	
	
}
