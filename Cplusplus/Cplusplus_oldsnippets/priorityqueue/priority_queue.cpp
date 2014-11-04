#include "priority_queue.h"

//la radice del vector quindi e' in indice 1

priority_queue::priority_queue(){
	vect1.push_back(*new obj<string>(1000,"BLANK"));
	//in indice 0 ci sara' un elemento (con priorita' altissima cosi' rimane per sicurezza sempre li')
	//che non fa parte dell'heap ma serve per far partire sempre da 1 il vector
}

void priority_queue::heapify_topdown(int i){

	int indFiglioSx=i*2;
	int indFiglioDx=i*2+1;
	obj<string> temp_swap(0," ");
		
	if(indFiglioSx<=vect1.size()-1 && indFiglioDx<=vect1.size()-1){
		if(vect1[i].getPri()>=vect1[indFiglioSx].getPri() && vect1[i].getPri()>=vect1[indFiglioDx].getPri())
			return;
		int maxPri= ( (vect1[indFiglioSx].getPri()>vect1[indFiglioDx].getPri())  ? vect1[indFiglioSx].getPri() : vect1[indFiglioDx].getPri()  );
		if(maxPri==vect1[indFiglioSx].getPri()){
			temp_swap=vect1[i];
			vect1[i]=vect1[indFiglioSx];
			vect1[indFiglioSx]=temp_swap;
			heapify_topdown(i*2);		
		}
		else{
			temp_swap=vect1[i];
			vect1[i]=vect1[indFiglioDx];
			vect1[indFiglioDx]=temp_swap;
			heapify_topdown(i*2+1);
		}
		
								
	}
	else if(indFiglioSx<=vect1.size()-1){
		if(vect1[i].getPri()>=vect1[indFiglioSx].getPri())
			return;
		temp_swap=vect1[i];
		vect1[i]=vect1[indFiglioSx];
		vect1[indFiglioSx]=temp_swap;
		heapify_topdown(i*2);
			
	}	
	else if(indFiglioDx<=vect1.size()-1){
		if(vect1[i].getPri()>=vect1[indFiglioDx].getPri())
			return;
		temp_swap=vect1[i];
		vect1[i]=vect1[indFiglioDx];
		vect1[indFiglioDx]=temp_swap;
		heapify_topdown(i*2+1);
	}

	
}



void priority_queue::heapify_bottomup(int i){
	obj<string> temp_swap(0," ");
	//vector< obj<string> >::iterator it=vect1.end();
	int j=i;	
	while(j>1 && vect1[j].getPri()>vect1[j/2].getPri()){
		temp_swap=vect1[j];
		vect1[j]=vect1[j/2];		
		vect1[j/2]=temp_swap;
		j/=2;	
	}
	
	

}




obj<string> priority_queue::retMax(){
	return vect1[1];

}



obj<string> priority_queue::extractAndRetMax(){
	obj<string> toRet(0," ");
	toRet=vect1[1];
	vect1[1]=vect1[vect1.size()-1];
	vect1.resize(vect1.size()-1);
	heapify_topdown(1);
	return toRet;

}



void priority_queue::increase_prior(int i,int newPrior){
	//puo' solo incrementare la priorita', quindi puo' controllare solo in bottomup
	vect1[i].setPri(newPrior);
	heapify_bottomup(i);

}




void priority_queue::insertNew(int prior,string str){
		
	vect1.push_back(*new obj<string>(prior,str));
	heapify_bottomup(vect1.size()-1); 

}



void priority_queue::stampa(){
for(vector<obj<string> >::iterator it=vect1.begin()+1;it!=vect1.end();it++)
	cout<<(*it).getVal()<<" "<<(*it).getPri()<<endl;

}


