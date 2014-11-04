//class.h
#include <cstring>
#include <iostream>
#include <cstdlib>
using namespace std;

class classeB{
	protected:
		string x;
	public:
	classeB(string value="blank"):x(value){}
	string getX();
	classeB& operator*(classeB &secObj){
		
		classeB *p=new classeB(x+secObj.getX());
		
		return *p;

	}
	

};


string classeB::getX(){
	return x;
}






template <typename T>
T& quad(T obj){
	T *new_obj=new T(obj*obj);
	
	return *new_obj;

}
