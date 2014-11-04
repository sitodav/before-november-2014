//non orientato
#include "elem_principale.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
using namespace std;

class graph{
	vector<elem_principale> vect_nodi;
	
	public:
		graph(vector<char> vertici,vector<string> archi,vector<int> pesi);
		elem_principale &get_elem_principale(int i=0){ return vect_nodi[i];}
		void genera_liste_adiacenti(vector<string> archi,vector<int> pesi);
		int get_posizione_on_vectnodi(char label);
		vector<elem_principale> get_vect_nodi(){return vect_nodi;}
		void set_new_key_priority(int i,int newkey){vect_nodi[i].set_new_key_priority(newkey);}	
		void stampa_rappresentazione();
		
	
};
