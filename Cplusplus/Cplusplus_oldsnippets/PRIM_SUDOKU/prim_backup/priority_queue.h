//min priority queue implementata con heap (array sovradimensionato di 1)
#include <vector>
#include "graph.h"

using namespace std;

class priority_queue{
	
	vector<elem_principale> queue;
	void heapify_bottom_up(int i); //i indice sul vector che implementa l'heap
	void heapify_top_down(int i); //i indice sul vector che implementa l'heap
	void swap(int i,int j);
	int get_ind_onqueue(char label);
public:
	priority_queue(); //default constructor
	priority_queue(vector<elem_principale> vect_tocopy); //overloaded constructor
	elem_principale extract_min(); 
	void add_element(elem_principale nuovo_elemento); //metodo per aggiungere elemento singolo	
	void decrementa_priority_elemento(char label,int new_prior); //decremento elemento scelto (passando label)	
	bool is_empty(){return (queue.size()==1) ? true : false;}
	
};

