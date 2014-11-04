#include <iostream>
#include <cstdlib>
#include <ctime>


using namespace std;


struct nodo{
	struct nodo *pleft,*pright;
	int key;
};


void crea_nodo(int max,int actual,struct nodo *phead);
void visita_albero(struct nodo *phead);
int
 cerca(int key,struct nodo *phead);
