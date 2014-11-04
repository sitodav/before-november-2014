#include <iostream>
#include <cstdlib>
#include <map>
#include <iterator>

using namespace std;

template<typename tKey,typename tData>
class list_manager{
	
		class nodo{
			public:
				nodo *pnext;
				tData dato;
				nodo(tData val=0):pnext(NULL),dato(val){}	
		};


		static int id;
		map<int, nodo *> map1;
	
	public:
		void stamp_all();
		void stamp_single(tKey key);
		void add_new(tKey key,tData new_dato);
		void del(tKey key,tData to_erase);
		void change(tKey key,tData old_value,tData new_value);
		int new_list(tData first_dato);

};


template<typename tKey,typename tData>
int list_manager<tKey,tData>::id=0;


template<typename tKey,typename tData>
void list_manager<tKey,tData>::stamp_all(){


}


template<typename tKey,typename tData>
void list_manager<tKey,tData>::stamp_single(tKey key){
	typename map<int,nodo *>::iterator it=map1.find(key);
	if(it==map1.end()){
		cout<<"Key not found!"<<endl;
		return ;	
	}
	cout<<"Key found!"<<endl;
	nodo *plav=(*it).second;
	while(plav!=NULL){
		cout<<plav->dato<<endl;
		plav=plav->pnext;		
	}
}


template<typename tKey,typename tData>
void list_manager<tKey,tData>::add_new(tKey key,tData new_dato){

	typename map<int, nodo *>::iterator it=map1.find(key);

	if(it==map1.end()){
		cout<<"Key not found!"<<endl;
		return;	
	}
	cout<<"Key found!"<<endl;
	nodo *plav=(*it).second;
	//ci spostiamo alla fine della lista
	
	while(plav->pnext!=NULL)
		plav=plav->pnext;
	plav->pnext=new nodo(new_dato);					
	
	
}

template<typename tKey,typename tData>
void list_manager<tKey,tData>::del(tKey key,tData to_erase){
	cout<<"del";

}

template<typename tKey,typename tData>
void list_manager<tKey,tData>::change(tKey key,tData old_value,tData new_value){
	cout<<"change";

}

template<typename tKey,typename tData>
int list_manager<tKey,tData>::new_list(tData first_dato){
	id++;
	map1[id]=new nodo(first_dato);
	return id;

}

