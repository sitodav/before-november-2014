#include "priority_queue.h"
#include <iostream>

using namespace std;

priority_queue::priority_queue(){
	//mettiamo in 1 un elemento fittizio per sovradimensionare il vector	
	queue.push_back(elem_principale(' ',-1,-1));	

}

priority_queue::priority_queue(vector<elem_principale> vect_tocopy){
	//mettiamo in 1 un elemento fittizio per sovradimensionare il vector	
	queue.push_back(elem_principale(' ',-1,-1));
	
	//generiamo dal grafo la queue
	for(int i=0;i<vect_tocopy.size();i++){
		queue.push_back(vect_tocopy[i]);
			
	}

}


void priority_queue::add_element(elem_principale nuovo_elemento){
	queue.push_back(nuovo_elemento);

}



int priority_queue::get_ind_onqueue(char label){
	for(int i=1;i<queue.size();i++)
		if(queue[i].get_label()==label)
			return i;

}

void priority_queue::decrementa_priority_elemento(char label,int new_prior){
	int i=get_ind_onqueue(label);
	if(queue[i].get_key_prior()<new_prior)
		return; //perche' stiamo cercando di incrementare priorita', ma per l'uso che dobbiamo fare della min-queue e' un'operazione che non dovra' mai essere fatta
	queue[i].set_new_key_priority(new_prior);
	heapify_bottom_up(i);
}


elem_principale priority_queue::extract_min(){
	elem_principale root=queue[1]; //la radice heap nel vector e' sovradimensionato e' 1
	queue[1]=queue[queue.size()-1];
	queue.pop_back();
	heapify_top_down(1);
	root.set_available(false);
	return root;
}

void priority_queue::heapify_bottom_up(int i){ //va a salire, swappando, e si ferma se non occorre swappare perche' il padre e' minore di noi, o se non esiste il padre, quindi se sono arrivato sulla radice
	
	if(i/2<1) return; //sono radice (in 1 perche' array sovradimensionato) 

	if(queue[i].get_key_prior()<queue[i/2].get_key_prior()){ //swap
		
		swap(i,i/2);
		//chiamata ricorsiva	
		heapify_bottom_up(i/2); 
	}
	 
}

void priority_queue::heapify_top_down(int i){
	if(2*i>queue.size()-1) //non ha figlio sinistro, e quindi sicuramente non ha figlio dx, quindi e' foglia
		return;
	elem_principale temp(' ',-1,-1);
	if(2*i+1>queue.size()-1){ 
		//se arriva qui vuol dire che ha figlio sinistro ma non destro
		//in tal caso il controllo e' solo sul sinistro
		if(queue[i].get_key_prior()>queue[2*i].get_key_prior()){ //se mio figlio sinistro e' minore di me
			swap(i,2*i);
			//e ricorsiva in basso
			heapify_top_down(2*i);		
		}
	}
	else{
		//ha entrambi i figlii
		int imin;
		imin=( (queue[2*i].get_key_prior()<queue[2*i+1].get_key_prior()) ? 2*i : 2*i+1 );	//imin conterrà il  min tra i 2 figli
		if(queue[imin].get_key_prior()>=queue[i].get_key_prior()) //inserisco anche il caso uguale cosi' evito di fare chimate ricorsive verso il basso nel caso in cui le priorita' siano uguali
			imin=i;
		//ora imin contiene indice elemento minore tra noi (i), e i nostri 2 figli
		if(imin==i) //allora heap e' rispettato verso il basso e non devo fare piu' nulla
			return;
		if(imin==2*i){
			
			swap(i,2*i);
			//e ricorsiva in basso
			heapify_top_down(2*i);	
			
		}
		else if(imin==2*i+1){
			
			swap(i,2*i+1);
			//e ricorsiva in basso
			heapify_top_down(2*i+1);	
		}
			
	}
}	

void priority_queue::swap(int i,int j){
	elem_principale temp(' ',-1,-1);
	temp=queue[i];
	queue[i]=queue[j];
	queue[j]=temp;

}


