#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstring>
#include "obj.h"
using namespace std;



class priority_queue{	
protected:
	vector< obj<string> > vect1;
	void heapify_topdown(int i);
	void heapify_bottomup(int i);
public:
	priority_queue();
	obj<string> retMax();
	obj<string> extractAndRetMax();
	void increase_prior(int i,int newPrior);
	void insertNew(int prior,string str);
	void stampa();	


};
