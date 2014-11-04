#include <cmath>
#include "strutt_corrispondenze_quadri.h"
using namespace std;

void strutt_corrispondenze_quadri::setter(int i,int sr,int er,int sc,int ec){
	corrispondenze[i].setter(sr,er,sc,ec);


}

void strutt_corrispondenze_quadri::setter_all(){
	int n=(int)sqrt(size_game);
	for(int i=0;i<n;i++)
	     for(int j=0;j<n;j++)
		   corrispondenze[j+(i*n)].setter(i*n,((i+1)*n)-1,j*n,((j+1)*n)-1);
		
	     
	
}

int strutt_corrispondenze_quadri::get_start_r(int i){
	return corrispondenze[i].get_start_r();

}

int strutt_corrispondenze_quadri::get_end_r(int i){
	return corrispondenze[i].get_end_r();
}

int strutt_corrispondenze_quadri::get_start_c(int i){
	return corrispondenze[i].get_start_c();
}

int strutt_corrispondenze_quadri::get_end_c(int i){
	return corrispondenze[i].get_end_c();
}

int strutt_corrispondenze_quadri::get_quadro(int i,int j){
	for(int k=0;k<size_game;k++)
		if(i>=(corrispondenze[k].get_start_r()) && i<=(corrispondenze[k].get_end_r())
		&& j>=(corrispondenze[k].get_start_c()) && j<=(corrispondenze[k].get_end_c()))
			return k;
}
