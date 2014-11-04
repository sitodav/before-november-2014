#include "class.h"
#include <iostream>
using namespace std;

void classe::funz(int a){
	
	cout<<"SONO IN BASE CLASS"<<endl;

}

int derivedclass::funz(){
	cout<<"SONO IN DERIVED CLASS"<<endl;
	funz(3);
	return 2;


}
