#ifndef H_PERS
#define H_PERS
#include "datifiscali.h"
#include "datianagrafici.h"
#endif
#include <iostream>
#include <cstring>
using namespace std;

class persona{
	protected:
	string nome;
	datianagrafici dat1;
	datifiscali dat2;

	public:
	persona(string name="Davide Sito",int reddito=100,string cod="STIDVD88",string citta="Napoli",string data="28/04/88"); //costruttore classe persona: i par di default vanno indicati qui, la catena dei costruttori degli elementi di quest'aggregazione (dat1 e dat2) va indicata nella parte della definizione del corpo del costruttore

	string getNome();
	int getReddito();
	string getCodiceFiscale();
	string getCitta();
	string getData();



};


