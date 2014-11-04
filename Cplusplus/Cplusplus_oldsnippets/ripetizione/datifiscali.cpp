#ifndef H_FISC
#define H_FISC
#include "datifiscali.h"
#endif
#include <iostream>
#include <cstring>

using namespace std;


datifiscali::datifiscali(int redd,string cod){
	codicefiscale=cod;
	reddito=redd;

}

int datifiscali::getReddito(){
	return reddito;
}

string datifiscali::getCodiceFiscale(){
	return codicefiscale;

}
