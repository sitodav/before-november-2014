
//i 2 oggetti grafo e prior queue sono entita' separate. Sta all'algoritmo che ne fa utilizzo (prim) far si' che ad ogni azione su una, corrisponda azione corrispettiva sull'altra, per far si che siano aggiornate l'una rispetto all'altra, e per questo si usa la classe controller_prim




#include "funzioni.h"

using namespace std;




int main(){
	vector<char> vertici;
	vector<string> archi;
	vector<int> pesi;
	
	get_graph_values(vertici,archi,pesi);
	
	graph grafo(vertici,archi,pesi);
	//grafo.stampa_rappresentazione();

	priority_queue min_queue(grafo.get_vect_nodi());

	prim_controller prim_alg(grafo,min_queue);
	prim_alg.get_mst();
	

	

}
