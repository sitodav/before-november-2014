#ifndef corrisp
#define corrisp
#include "elem_corrisp.h"
#endif
#include <iostream>
#include <cstdlib>

using namespace std;



class strutt_corrispondenze_quadri{

	elem_corrisp *corrispondenze;
	int size_game;
public:
	strutt_corrispondenze_quadri(int n=9):corrispondenze(new elem_corrisp[n]),size_game(n){setter_all();}
	~strutt_corrispondenze_quadri(){delete[] corrispondenze;}
	void setter_all();	
	void setter(int i,int sr,int er,int sc,int ec);	
	int get_start_r(int i);
	int get_end_r(int i);
	int get_start_c(int i);
	int get_end_c(int i);
	int get_quadro(int i,int j);
	
	
	



};
