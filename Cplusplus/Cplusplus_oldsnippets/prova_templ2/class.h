//class.h
#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;


string operator*(string &a,int b){
	string buf(a);

	for(int i=0;i<b;i++)
		buf+=a;	
	return buf;

}

template <typename T_VALUE>
class classeA{
	protected:
		T_VALUE val;
	public:
		classeA(T_VALUE initVal=0):val(initVal){}
		T_VALUE getVal();
		void raddoppia();
};


template <typename T_VALUE>
T_VALUE classeA<T_VALUE>::getVal(){
	return val;

}

template <typename T_VALUE>
void classeA<T_VALUE>::raddoppia(){
	val=val*2;
}

