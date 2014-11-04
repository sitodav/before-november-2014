//classi.h
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;


class baseClass{

protected:
	string *name;

public:
	baseClass(string nome="base");
	virtual void print();
	~baseClass();

};




class derivedClass: public baseClass{


public:
	derivedClass(string nome="derivata");
        void print(); //@override
	~derivedClass();
};
