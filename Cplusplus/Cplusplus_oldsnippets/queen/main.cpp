#include <iostream>
#include <cstdlib>


using namespace std;

int n;
int *ind_riga;
int tot_solut=0;
int tot_backtrack=0;
int num_chiamate_ric=0;

int myabs(int a){
	return (a>=0) ? a : -1*a;

}

bool posso(int riga,int col){
	
	
	for(int j=0;j<col;j++){
			if(ind_riga[j]==riga)
				return false;
			if(ind_riga[j]>riga && ind_riga[j]+j==riga+col)
					return false;

			if(ind_riga[j]<riga && ind_riga[j]-j==riga-col)
				return false;		
	}
	
	
	return true;

}

void stampa_scacchiera(){
	tot_solut++;
	cout<<endl;
	cout<<"num soluzione:>> "<<tot_solut<<"\nbacktrack effettuati>> "<<tot_backtrack<<endl;
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++)
			if(ind_riga[j]==i)
				cout<<"|X|";
			else cout<<"| |";
		cout<<endl;		
	}
	
	usleep(100000);
}


bool funz(int col){

	for(int i=0;i<n;i++){
		if(posso(i, col)==false)
			continue;
		ind_riga[col]=i;
		if(col!=n-1 && !funz(col+1)){
			tot_backtrack++;
			continue;
		}
		if(col==n-1)
			stampa_scacchiera();	
	}
	return false;

}

int main(int argc,char *argv[]){
cout<<"quante regine/size caselle ?"<<endl;
cin>>n;
ind_riga=new int[n];
for(int i=0;i<n;i++)
	ind_riga[i]=-1;
funz(0);


}
