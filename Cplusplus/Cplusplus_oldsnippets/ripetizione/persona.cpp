#include "persona.h"

persona::persona(string name,int reddito,string cod,string citta,string data): dat1(data,citta),dat2(reddito,cod)
{
	nome=name;

}

string persona::getNome(){
	return nome;

}


int persona::getReddito(){
	return dat2.getReddito();

}


string persona::getCodiceFiscale(){
	return dat2.getCodiceFiscale();
}


string persona::getCitta(){
	return dat1.getCitta();
}


string persona::getData(){
	return dat1.getData();
}




