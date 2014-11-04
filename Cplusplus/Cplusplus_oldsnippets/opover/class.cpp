#include "class.h"
#include <string>
#include <cstdlib>
#include <iostream>
using namespace std;

a::a(int val){
	dato=val;

}

int a::operator+(class a &secondo){
	return dato+secondo.dato;

}


int a::operator-(){
	return -dato;

}
