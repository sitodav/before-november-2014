#include <iostream>
#include <cstdlib>
#include "class.h"
#include <vector>

using namespace std;



int **tab;
invalid_set *quad_set;
invalid_set *righe_set;
invalid_set *col_set;
corr *corrispondenze;


int ottieni_quad(int i,int j){
	for(int k=0;k<=8;k++)
		if(i>=corrispondenze[k].start_riga && i<=corrispondenze[k].end_riga 
			&& j>=corrispondenze[k].start_col && j<=corrispondenze[k].end_col)
				return k;

}


bool is_invalid(int k,int i,int j,int quad){
	
	 for(vector<int>::iterator it=quad_set[quad].invalid_vect1.begin();it!=quad_set[quad].invalid_vect1.end();it++)
		if(*it==k)
			return true;
	
	for(vector<int>::iterator it=righe_set[i].invalid_vect1.begin();it!=righe_set[i].invalid_vect1.end();it++)
		if(*it==k)
			return true;
	
	for(vector<int>::iterator it=col_set[j].invalid_vect1.begin();it!=col_set[j].invalid_vect1.end();it++)
		if(*it==k)
			return true;
	
	return false;
	
}

void stampa_matrice(){

	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++)
			cout<<tab[i][j]<<" ";
		cout<<endl;	
	}
}

bool riempi(int i,int j){
	cout<<i<<" "<<j<<endl;
	
	bool a;
	int quad=ottieni_quad(i,j);
	for(int k=1;k<=9;k++){ //numeri da 1 a 9
		if(is_invalid(k,i,j,quad))
			continue;
		tab[i][j]=k; //imposta
		//aggiorna gli invalid set
		quad_set[quad].invalid_vect1.push_back(k);
		righe_set[i].invalid_vect1.push_back(k);
		col_set[j].invalid_vect1.push_back(k);
		

		if(i==8 && j==8){
			stampa_matrice();
			return true;		
		}
		else if(j==8)
			a=riempi(i+1,0);
		else a=riempi(i,j+1);
		
		if(!a){
			//elimina dagli insiemi invalidi l'ultimo elemento inserito
			//poiche' sicuramente quello che abbiamo inserito noi e che dobbiamo 
			//cambiare, e' l'ultimo
			quad_set[quad].invalid_vect1.pop_back();
			righe_set[i].invalid_vect1.pop_back();
			col_set[j].invalid_vect1.pop_back();
			continue;		
		}		
		else return true;

			
	}
	return false;

}






int main(){
tab=new int*[9];
for(int i=0;i<9;i++)
	*(tab+i)=new int[9]();

corrispondenze=new corr[9];
corrispondenze[0].start_riga=0;
corrispondenze[0].end_riga=2;
corrispondenze[0].start_col=0;
corrispondenze[0].end_col=2;

corrispondenze[1].start_riga=0;
corrispondenze[1].end_riga=2;
corrispondenze[1].start_col=3;
corrispondenze[1].end_col=5;

corrispondenze[2].start_riga=0;
corrispondenze[2].end_riga=2;
corrispondenze[2].start_col=6;
corrispondenze[2].end_col=8;


corrispondenze[3].start_riga=3;
corrispondenze[3].end_riga=5;
corrispondenze[3].start_col=0;
corrispondenze[3].end_col=2;

corrispondenze[4].start_riga=3;
corrispondenze[4].end_riga=5;
corrispondenze[4].start_col=3;
corrispondenze[4].end_col=5;

corrispondenze[5].start_riga=3;
corrispondenze[5].end_riga=5;
corrispondenze[5].start_col=6;
corrispondenze[5].end_col=8;

corrispondenze[6].start_riga=6;
corrispondenze[6].end_riga=8;
corrispondenze[6].start_col=0;
corrispondenze[6].end_col=2;

corrispondenze[7].start_riga=6;
corrispondenze[7].end_riga=8;
corrispondenze[7].start_col=3;
corrispondenze[7].end_col=5;

corrispondenze[8].start_riga=6;
corrispondenze[8].end_riga=8;
corrispondenze[8].start_col=6;
corrispondenze[8].end_col=8;




/*for(int i=0;i<9;i++)
	cout<<corrispondenze[i].start_riga<<" "<<corrispondenze[i].end_riga<<" "<<corrispondenze[i].start_col<<" "<<corrispondenze[i].end_col<<endl;*/


quad_set=new invalid_set[9];
righe_set=new invalid_set[9];
col_set=new invalid_set[9];

riempi(0,0);








	

}
