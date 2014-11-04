#include "priority_queue.h"

class prim_controller{
	graph &grafo;
	priority_queue &min_queue;
	
public:
	prim_controller(graph &gr,priority_queue &qu ):grafo(gr),min_queue(qu){}
	void get_mst();
	void stampa_albero_mst();
	void riazzera_campi_grafo();
	void riazzera_min_queue();

};
