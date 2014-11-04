#include "classi.h"
#include <iostream>

void test(baseClass &obj){
	obj.print();


}

int main(int argc,char *argv[]){
	baseClass obj1;
	derivedClass obj2("NUOVONOMEDERIVED");
	test(obj2);
	



}



