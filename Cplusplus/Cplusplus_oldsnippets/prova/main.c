#include "heads.h"




int main(){
	struct nodo *phead=new struct nodo;
	crea_nodo(3,0,phead);
	visita_albero(phead);
	int count=cerca(0,phead);
	cout<<"contatore e' "<<count<<endl;

}
