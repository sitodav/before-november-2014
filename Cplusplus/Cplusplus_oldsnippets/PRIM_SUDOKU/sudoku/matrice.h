#include "strutt_corrispondenze_quadri.h"

class matrice{
private:
	int **mat;
	strutt_corrispondenze_quadri corr;
	int size_game;
public:
	
	matrice(int n=9); //default constructor
	matrice(matrice &mat_to_copy); //copy constructor
	~matrice(); //destructor
	void default_clues();
	int getter(int i,int j);
	int setter(int k,int i,int j);
	int ret_size(){return size_game;}
	bool is_invalid(int k,int i,int j);
	void stampa_matrice();




};


	

