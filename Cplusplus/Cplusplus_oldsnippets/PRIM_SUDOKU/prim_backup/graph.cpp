#include "graph.h"





graph::graph(vector<char> vertici,vector<string> archi,vector<int> pesi){


	
	for(vector<char>::iterator it=vertici.begin();it!=vertici.end();it++)
		vect_nodi.push_back(elem_principale(*it,10000,-1)); 

	genera_liste_adiacenti(archi,pesi);
	
	
}




void graph::genera_liste_adiacenti(vector<string> archi,vector<int> pesi){
	
	char first_v,end_v;
	for(int i=0;i<archi.size();i++){
		
		 first_v=archi[i][0];
		 end_v=archi[i][1];
		int peso=pesi[i];
		//abbiamo gli estremi dell'arco, il primo estremo first_v verra' cercato sull'array dei nodi, 
		//e quando si trova una corrispondenza, per quello viene aggiunto il secondo vertice tra i suoi adiacenti 
		//e viceversa, siccome essendo per ipotesi il grafo non orientato, c'e' ridondanza del doppio verso nelle
		//adiacenze
		for(vector<elem_principale>::iterator it2=vect_nodi.begin();it2!=vect_nodi.end();it2++){

			if(first_v==(*it2).get_label()){
				//dobbiamo riscorrere per sapere la posizione del nodo adiacante, sull'array poiche'
				//e' un info che dovremo passare alla funzione per inserire l'adiacenza
				//cout<<"Aggiungo nodo "<<end_v<<" come adiacente ad "<<first_v<<endl;
				int end_v_onvect;
				for(int k=0;k<vect_nodi.size();k++)
					if(vect_nodi[k].get_label()==end_v)
						end_v_onvect=k;
				(*it2).add_adiacenza(end_v,peso,end_v_onvect);
			}
			if(end_v==(*it2).get_label()){
				//cout<<"Aggiungo nodo "<<first_v<<" come adiacente ad "<<end_v<<endl;
				int start_v_onvect;
				for(int k=0;k<vect_nodi.size();k++)
					if(vect_nodi[k].get_label()==first_v)
						start_v_onvect=k;
				(*it2).add_adiacenza(first_v,peso,start_v_onvect);						
				}
				
		}
	}

}


int graph::get_posizione_on_vectnodi(char label){
	for(int i=0;i<vect_nodi.size();i++)
		if(vect_nodi[i].get_label()==label)
			return i;

}




void graph::stampa_rappresentazione(){
	for(vector<elem_principale>::iterator it=vect_nodi.begin();it!=vect_nodi.end();it++){
		cout<<"nodo: "<<it->get_label();
		cout<<" suoi adiacenti : ";
		
		for(list<nodo_adiacente>::iterator it2=it->get_list().begin();it2!=it->get_list().end();it2++)
			cout<<" "<<it2->get_label();
		
		cout<<endl;

	}


}
