/*
IL COPY CONSTRUCTOR E' RIDEFINITO PER QUELLE CLASSI AL CUI INTERNO CONTENGONO UN PUNTATORE COME VARIABILE DI CLASSE, QUINDI PER LE QUALI NON VA BENE L'UTILIZZO DEL COPY CONSTRUCTOR INSERITO DI DEFAULT DAL COMPILATORE, IN QUANTO SI ANDREBBE IN CONTRO A SHALLOW COPY
TUTTAVIA NON E' DEFINITO PER LA CLASSE STRUTT_CORRISPONDENZE_QUADRI IN QUANTO ESSENDO UNA CLASSE HARDCODED PER LA QUALE IL CONTENUTO DEGLI OGGETTI DIPENDE DAL SIZE, NON C'E' BISOGNO DI DEFINIRNE UN COPY CONSTRUCTOR IN QUANTO PER OGNI SIZE IL CONTENUTO SARA' SEMPRE UGUALE
*/

//consideriamo come ciclo di backtracking quando devo ritornare alla chiamata precedente padre (dalla chiamata attuale) perche' mi sono bloccato, e ho bisogno che un mio antenato modifichi qualcosa. In quel caso incremento il contatore di cicli di backtracking.



#include <iostream>
#include <cstdlib>
#include "funzioni.h"

using namespace std;


int main(){

matrice *matrix;
int n_cicli_backtracking=0;
int n_soluz=0;


int ris;
cout<<"[1]lanciare griglia standard 9 con \"clues\" predefiniti"<<endl;
cout<<"[2]leggere da file di testo la griglia"<<endl;
cout<<"[3]scegliere a runtime dimensione griglia e \"clues\""<<endl;
cin>>ris;

if(ris==1){
	
	matrix=new matrice(9);
	matrix->default_clues();
	matrix->stampa_matrice(); cout<<endl;
}
else if(ris==3){
	cout<<"Inserisci n dove n-esimo sudoku=ogni quadrato di nxn caselle, in tot nxn quadrati"<<endl;
	int n;
	cin>>n;
	
	matrix=new matrice(n*n);

	//chiediamo all'utente di inserire i "clues"
	set_clues(matrix);
}
else{
	read_clues(matrix);
}

cout<<"[1].Trova la prima soluzione valida"<<endl<<"[2].Trova tutte le possibili soluzioni"<<endl;
int ris2;
cin>>ris2;

switch(ris2){
	case 1:
	        if(riempi_unica(n_cicli_backtracking,matrix,0,0)) //se tutto il ramo ricorsivo riesce a ritornare true 
		    matrix->stampa_matrice(); //tutta lamatrice e' riempita e la stampiamo
		else cout<<"Non c'e' soluzione"<<endl;
		break;	
	case 2:
		riempi_plurime(n_soluz,n_cicli_backtracking,matrix,0,0);
		cout<<"Trovate in tutto "<<n_soluz<<" soluzioni"<<endl;
}

cout<<"Cicli di backtracking totali: "<<n_cicli_backtracking<<endl;

delete matrix;
}











