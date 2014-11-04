#include "matrice.h"
#include <iostream>
#include <cstdlib>
using namespace std;

matrice::matrice(int n):size_game(n),corr(n){
	mat=new int*[size_game];
	for(int i=0;i<size_game;i++)
		mat[i]=new int[size_game];

}

matrice::matrice(matrice &mat_to_copy):size_game(mat_to_copy.ret_size()),corr(mat_to_copy.ret_size()){//Se non avessimo ridefinito noi il copy constructor, quello inserito dal compilatore ne avrebbe creato uno che uguagliava il contenuto del puntatore a puntatore mat di quest'oggetto, con quello dell'oggetto da copiare. Ma cosi' non andava bene.
mat=new int*[size_game];
	for(int i=0;i<size_game;i++)
		mat[i]=new int[size_game];

for(int i=0;i<size_game;i++)
	for(int j=0;i<size_game;i++)
		mat[i][j]=mat_to_copy.getter(i,j);

//Siccome e' hardcoded il contenuto di ogni oggetto strutt_corrispondenze_quadri, e dipende dal size della matrice, non c'e' bisogno di lanciare un copy constructor di questo che copy il contenuto dell'oggetto di classe strutt_corrispondenze_quadri contenuto in mat_to_copy, poiche' tanto a parita' di size, i contenuti generati saranno uguali.E nella lista di inizializzazione di questo costruttore, comunque abbiamo gia' richiamato il costruttore della classe strutt_corrispondenze_quadri per l'oggetto corr, con argomento il size.
	


}

matrice::~matrice(){
	for(int i=0;i<size_game;i++)
		delete[] mat[i]; //dealloco ogni puntatore della matrice 2d dichiarata 					dinamicamente*/	
	delete[] mat; //e dealloco il puntatore a puntatori
	
}

void matrice::default_clues(){
int coeff[27][3]={{8,0,0},{1,2,1},{4,0,3},{6,0,5},{4,1,6},{6,2,6},{5,2,7},{7,0,8},{5,3,0},{9,3,2},{4,5,1},{8,5,2},{3,3,4},{7,4,4},{2,5,4},{9,8,3},{2,8,5},{7,3,6},{8,3,7},{1,5,6},{3,5,8},{3,8,0},{5,6,1},{2,6,2},{1,7,2},{9,6,7},{5,8,8}};

for(int i=0;i<27;i++)
	mat[coeff[i][1]][coeff[i][2]]=coeff[i][0];



}

bool matrice::is_invalid(int k,int i,int j){
	int myquad=corr.get_quadro(i,j);
	
	//controllo se esiste gia' quel valore sulla stessa riga
	for(int x=0;x<size_game;x++)
		if(mat[i][x]==k)
			return true; //invalido
	//controllo se esiste gia' quel valore sulla stessa colonna
	for(int y=0;y<size_game;y++)
		if(mat[y][j]==k)
			return true; //invalido*/
	//controllo a seconda del quadrato di appartenenza
	//per un dato quadrato (quello al quale apparteniamo)
	//il range di coordinate all'interno delle quali dobbiamo controllare, lo otteniamo
	//guardando sempre alla "tabella" delle corrispondenze
	int sr=corr.get_start_r(myquad);
	int er=corr.get_end_r(myquad);
	int sc=corr.get_start_c(myquad);
	int ec=corr.get_end_c(myquad);
	//e controlliamo all'interno dei 2 range di coordinate	
	for(int y=sr;y<=er;y++)
		for(int x=sc;x<=ec;x++)
			if(k==mat[y][x])
				return true; //invalido 
	

	return false; //valido
	

}

int matrice::getter(int i,int j){
	return mat[i][j];
}

int matrice::setter(int k,int i,int j){
	mat[i][j]=k;
}

void matrice::stampa_matrice(){
	for(int i=0;i<size_game;i++){
		for(int j=0;j<size_game;j++)
			cout<<mat[i][j]<<" ";
		cout<<endl;
	}
}
