#include "class.h"
#include <iostream>
using namespace std;

void classeB::print(){
	cout<<getX()<<" "<<obj.getX()<<endl;

}

void classeC::print(){
	cout<<getX()<<" "<<endl;
	obj.print();
}

void funz(classeA &obj){
	obj.f();

}




