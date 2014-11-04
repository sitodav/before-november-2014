#include <cstdlib>
#include <iostream>
using namespace std;

class nodo_adiacente{

	
	int ind_on_vect_nodo;
	int peso_arco;
	char label;
	
public:
	nodo_adiacente(char etichetta,int peso,int ind_on_vect):label(etichetta),peso_arco(peso),ind_on_vect_nodo(ind_on_vect){}
	int get_peso(){return peso_arco;}
	char get_label(){return label;}
	int get_ind_on_vect_nodo(){return ind_on_vect_nodo;}



};
