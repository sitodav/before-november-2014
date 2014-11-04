#include "class.h"
#include <cstring>
#include <iostream>
#include <cstdlib>

using namespace std;


int main(){
	classeA<string> obj("prova");
	obj.raddoppia();
	cout<<obj.getVal();


}
