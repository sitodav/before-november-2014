
#include "heads.h"


void crea_nodo(int max,int actual,struct nodo *phead){
	phead->key=rand()%5;
	if(actual>=max)
		return;
	phead->pleft=new struct nodo;
	phead->pright=new struct nodo;
	crea_nodo(max,actual+1,phead->pleft);
	crea_nodo(max,actual+1,phead->pright);
	
}


void visita_albero(struct nodo *phead){
	cout<<"key: "<<phead->key<<endl;
	if(phead->pleft!=NULL)
		visita_albero(phead->pleft);
	if(phead->pright!=NULL)
		visita_albero(phead->pright);

}


int cerca(int key,struct nodo *phead){
	int mycount=0;	
	if(key==phead->key) 
		mycount++;
	if(phead->pleft!=NULL)
		mycount+=cerca(key,phead->pleft);
	if(phead->pright!=NULL)
		mycount+=cerca(key,phead->pright);
	return mycount;
}



