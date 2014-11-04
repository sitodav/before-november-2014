#include "funzioni.h"



void get_graph_values(vector<char> &vertici,vector<string> &archi,vector<int> &pesi){
	

	int n_vertici,n_archi;
	cout<<"Inserisci il numero dei vertici del grafo (non orientato):"<<endl;
	cin>>n_vertici;
	cout<<"Inserisci il numero degli archi del grafo (non orientato):"<<endl;
	cin>>n_archi;
	int i=0;
	while(i++<n_vertici){
		cout<<"Inserisci etichetta vertice (singolo char):"<<endl;
		char v;
		cin>>v;
		vertici.push_back(v);
	}
	i=0;
	while(i++<n_archi){
		cout<<"inserisci (stringa bidimensionale) che indica arco:"<<endl;
		string arco;	
		cin>>arco;
		
		archi.push_back(arco);
		
		cout<<"Inserisci peso per l'arco "<<archi[archi.size()-1]<<" :"<<endl;
		int peso;
		cin>>peso;
		pesi.push_back(peso);
	}	


}

