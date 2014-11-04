#ifndef H_ANAG
#define H_ANAG
#include "datianagrafici.h"
#endif


#include <iostream>
#include <cstring>

using namespace std;

datianagrafici::datianagrafici(string date,string city){ //construttore, vale anche come costruttore di default perche' 
//nell'header sono definiti tutti i parametri come parametri di default
	data=date;
	citta=city;

}

string datianagrafici::getCitta(){
	return citta;
}

string datianagrafici::getData(){
	return data;
}
