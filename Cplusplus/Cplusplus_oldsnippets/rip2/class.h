#include <iostream>
using namespace std;

class classeA{
	private:
	int x;
	
	public:
	classeA(int val=12):x(val){}
	int getX() const{return x;}
	virtual void f(){cout<<"sono A"<<endl;}


};


class classeB: public classeA{
	classeA obj;
	public:
	classeB(int val=12):classeA(val){obj=classeA(val*2);}
	void print();
	void f(){cout<<"sono B"<<endl;}
};


class classeC: public classeB{
	classeB obj;
	public:
	classeC(int val=12):classeB(val){obj=classeB(val*2);}
	void print();
	void f(){cout<<"sono C"<<endl;}
};

void funz(classeA &obj);

class classeD: public classeC{
		
	public:
	classeD(int value=10):classeC(value){}
	virtual void prova() const=0;

};


class classeE: public classeD{
	public:
	void prova() const{cout<<"sono getter"<<endl; x=10;}

};

