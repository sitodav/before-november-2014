#include "prim_controller.h"
#include <iostream>
#include <vector>

using namespace std;


void prim_controller::get_mst(){
		//iniziamo l'algoritmo dal primo nodo del grafo (la scelta del nodo da cui partire è ininfluente)
		min_queue.decrementa_priority_elemento(grafo.get_elem_principale().get_label(),0); 
		//siccome il nodo la cui priorita' e' stata aggiornata, verra' subito estratto coome primo, e quindi
		//risultera' non disponibile per i successivi confronti sul grafo, possiamo non aggiornargli la priorita'
		//anche sul vector di nodi dell'oggetto rafo

		while(!min_queue.is_empty()){
			//lavorando con i nodi, aggiorniamo delle copie dalla parte della min queue solo i campi che ci interessano (ad esempio non aggiorniamo i padri o la validita' sulle copie della min queue)
			elem_principale estratto=min_queue.extract_min();
			//ora ci serve sapere l'indice (sul vect nodi del grafo) al quale e' posizionato il nodo estratto
			int ind_on_vectnodi=grafo.get_posizione_on_vectnodi(estratto.get_label());
			//cout<<"estratto dall'heap il nodo che su grafo ha indice "<<ind_on_vectnodi<<endl;
			//il suo indice sul vector principale del grafo,ora sta in ind_on_vectnodi
			//ora sappiamo quale lista di adiacenze consultare sul grafo
			//settiamo sulla copia DEL GRAFO la validita' a false del nodo estratto che indica
			//che e' stato tolto dalla coda di priorita'
			//grafo.set_availability(ind_on_vectnodi,false);
			grafo.get_elem_principale(ind_on_vectnodi).set_available(false);
			
			for(list<nodo_adiacente>::iterator it=estratto.get_list().begin();it!=estratto.get_list().end();it++)
			{
				//ci occorre sapere sul vector principale dei nodi (nel grafo) a che indice sta il nodo
				//trovato nella lista di adiacenti al nodo estratto
				int ind_on_vect_nodi2=it->get_ind_on_vect_nodo();
		

		

				if(grafo.get_elem_principale(ind_on_vect_nodi2).get_available()) //cioe' se il nodo adiacente sta ancora nella queue   
					if(it->get_peso()<grafo.get_elem_principale(ind_on_vect_nodi2).get_key_prior()){
						//cioe' se il peso di quell'adiacenza, e' minore dell'attuale priorita' 						del nodo adiacente, allora:

				/*		cout<<"il padre di "<<grafo.get_elem_principale(ind_on_vect_nodi2).get_label()<<" diventa "<<grafo.get_elem_principale(ind_on_vectnodi).get_label()<<endl;
*/



						grafo.get_elem_principale(ind_on_vect_nodi2).set_padre(ind_on_vectnodi);
						//cioe' l'indice del nodo adiacente all'estratto, diventa proprio 							l'estratto
						grafo.get_elem_principale(ind_on_vect_nodi2).set_new_key_priority(it->get_peso());					//il nodo adiacente prende come nuova priorita' il peso adiacenza
						//e aggiorniamo la priorita' anche sulla rispettiva copia nell'heap
				
						min_queue.decrementa_priority_elemento(it->get_label(),it->get_peso());
					}
				
			}
			
		}
		stampa_albero_mst();
		riazzera_campi_grafo();
		riazzera_min_queue();
		
}


void prim_controller::stampa_albero_mst(){
	int tot_peso=0;
	cout<<"--ALBERO DI COPERTURA MINIMO:--"<<endl;
	vector<elem_principale> vect1=grafo.get_vect_nodi();
	for(vector<elem_principale>::iterator it=vect1.begin();it!=vect1.end();it++)
		if(it->get_padre()==-1)
			continue;
			//cout<<it->get_label()<<" e' radice MST"<<endl;
		else {
			tot_peso+=it->get_key_prior();
			cout<<grafo.get_elem_principale(it->get_padre()).get_label()<<" - "<<it->get_label()<<endl;
		
		}//else cout<<it->get_label()<<" e' figlio di "<<grafo.get_elem_principale(it->get_padre()).get_label()<<endl;
	cout<<"peso totale MST: "<<tot_peso<<endl;


}



void prim_controller::riazzera_campi_grafo(){
	for(int i=0;i<grafo.get_vect_nodi().size();i++){
		grafo.get_elem_principale(i).set_new_key_priority(1000);	
		grafo.get_elem_principale(i).set_available(true);
		grafo.get_elem_principale(i).set_padre(-1);
	}

}

void prim_controller::riazzera_min_queue(){
	min_queue=priority_queue(grafo.get_vect_nodi());

}



