#include "class.h"
#include <iostream>
#include <cstdlib>

using namespace std;


int main(){
	list_manager<int,int> lman1; 
	while(1){
		cout<<"1 crea nuova lista\n2 inserisce elem in lista\n3 stampa lista"<<endl;
		int ris;
		int new_dato;	
		int key;
		cin>>ris;
		switch(ris){
			case 1:
				cout<<"ins nuovo primo dato"<<endl;
				
				cin>>new_dato;
				cout<<"id ritornato "<<lman1.new_list(new_dato)<<endl;
				
				break;
			case 2:
				cout<<"inserisci id lista e dato"<<endl;
				
				cin>>key>>new_dato;
				lman1.add_new(key,new_dato);
				break;
			case 3:
				cout<<"inserisci id lista da stampare"<<endl;
				
				cin>>key;
				lman1.stamp_single(key);
				break;
			default:
				continue;		
		}
		
	}

}
