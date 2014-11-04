#include "bucket.h"
#include <iostream>
#include <cstdlib>
#define k 10

using namespace std;


float * bucket::sort(float *a,int n,int p,int u){
	struct nodo **buckets=new struct nodo *[k];
	for(int i=0;i<k;i++)
		buckets[i]=NULL;
	struct nodo *plav;

	for(int i=0;i<n;i++){	
		//scegliamo in quale deve andare tra i buckets
		int j=0;
		while(1){
			float sx=(((float)(u-p))/k)*j;
			float dx=(((float)(u-p))/k)*(j+1);

			if( a[i]>sx  &&  a[i]<=dx  ) { //allora va in indice j
							
				if(buckets[j]==NULL){
					buckets[j]=new struct nodo;
					buckets[j]->dato=a[i];				
				}
				else{	plav=buckets[j];
					while(plav->pnext!=NULL)
						plav=plav->pnext;
					plav->pnext=new struct nodo;
					plav->pnext->dato=a[i];
				}
				break;			
			}
			j++;
		}	

	}
	//seconda parte, dobbiamo riordinare ogni linked list, e ricopiare i dati ordinati una lista per volta nell'array 	che restituiremo
	//CREA ALGORITMO CHE FA SORT TRA GLI ELEMENTI SULLA STESSA LISTA (IN QUESTO ESEMPIO, AVENDO MESSO UN SOLO NUMERO PER OGNI VALORE DELLA CIFRA DEI DECIMALI, ANDRA' 'UN SOLO ELEMENTO IN OGNI LINKED LIST, QUINDI NON C'E' BISOGNO DI FARE NESSU NSORT ALL'INTERNO DI OGNI LISTA, E QUINDI GIA? COSI? SARANNO ORDINATI

	int iToPut=0;
	float *sorted=new float[n];

	for(int j=0;j<k;j++){ //scorro sulle teste delle liste e per ogni testa nella lista, e metto i dati nell'array in ordine di come appaiono 
		plav=buckets[j];
		
		while(plav!=NULL){
			sorted[iToPut++]=plav->dato;
			plav=plav->pnext;		
		}
	}
	
	return sorted;





}
