#include "class.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

int main(int argc,char *argv[]){
	class a obj1,obj2(4);
	cout<<obj1.dato<<" "<<obj2.dato<<endl;
	cout<<obj1+obj2;
	cout<<-obj2;

}
