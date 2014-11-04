#include "persona.h"
#include <iostream>
#include <cstring>

using namespace std;

int main(int argc,char *argv[]){
	persona pers1("Giovanni Esposito");
	cout<<"Nome: "<<pers1.getNome()<<endl;
	cout<<"CodFiscale: "<<pers1.getCodiceFiscale()<<endl;
	cout<<"Reddito: "<<pers1.getReddito()<<endl;
	cout<<"Data Nascita: "<<pers1.getData()<<endl;
	cout<<"Citta' "<<pers1.getCitta()<<endl;





}
