#include "class.h"

int a::x=13;
void a::f(){
	x=17;

}

int & a::getter(){
	int &als=x;
	return als;
}
