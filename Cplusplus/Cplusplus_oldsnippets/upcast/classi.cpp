#include "classi.h"
#include <iostream>
using namespace std;

baseClass::baseClass(string nome){
	name=new string(nome);	
	

}

void baseClass::print(){
	std::cout<<"Stampa da base"<<endl;
	std::cout<<name<<endl;


}


baseClass::~baseClass(){
	delete name;

}


//---------------------------------------------------------------------------------------------------



derivedClass::derivedClass(string nome):baseClass(nome){
	
}


void derivedClass::print(){
	std::cout<<"Stampa da derived"<<endl;
	cout<<name<<endl;
}


derivedClass::~derivedClass(){


}
