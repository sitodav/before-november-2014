
#include <cstdlib>
#include "nodo_adiacente.h"
#include <list>
using namespace std;

class elem_principale{
	char label;
	list<nodo_adiacente> lista_adiacenze;
	int key_prior;
	int ind_padre;
	bool available;
	
	
	
public:
	elem_principale(char labl,int key,int padre):label(labl),key_prior(key),ind_padre(padre),available(true){}
	void set_new_key_priority(int newkey){key_prior=newkey;}
	void add_adiacenza(char label,int peso,int ind_on_vect_nodo);	
	char get_label(){return label;}
	int get_key_prior(){return key_prior;}
	void set_available(bool val){available=val;}
	list<nodo_adiacente> &get_list(){return lista_adiacenze;}
	bool get_available(){return available;}
	void set_padre(int i){ind_padre=i;}
	int get_padre(){return ind_padre;}

};
