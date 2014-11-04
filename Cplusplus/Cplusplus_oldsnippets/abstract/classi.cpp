#include "classi.h"
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

//-----------------------def di classe persona------
persona::persona(string nome){
	name=nome;
}




//-----------------------def di classe studente------
studente::studente(string nome):persona(nome){
	
}

void studente::stampa_dati(){
	cout<<"Sono uno studente e mi chiamo: "<<name<<endl;
}



//----------------------def di classe professore---
professore::professore(string nome):persona(nome){

}

void professore::stampa_dati(){
	cout<<"sono un professore e mi chiamo: "<<name<<endl;
}



///////////////////

void richiama_stampa_su(persona &obj){
	obj.stampa_dati();

}



