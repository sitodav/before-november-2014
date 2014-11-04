#include "elem_principale.h"
#include <iostream>
using namespace std;



void elem_principale::add_adiacenza(char label,int peso,int ind_on_vect_nodo){
	
	lista_adiacenze.push_back(nodo_adiacente(label,peso,ind_on_vect_nodo));

}




