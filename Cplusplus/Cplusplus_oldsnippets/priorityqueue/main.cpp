#include "priority_queue.h"
#include <cstring>
#include <cstdlib>
#include <iostream>

using namespace std;


int main(){
	priority_queue queue1;
	queue1.insertNew(10,string("weweee"));
	queue1.insertNew(18,string("CIAOOO"));
	queue1.insertNew(22,string("C"));
	queue1.insertNew(4,string("CIAOOO"));
	queue1.stampa();
	obj<string> max=queue1.extractAndRetMax();
	cout<<"Il max ha (lo estraggo) priorita': "<<max.getPri()<<" e stringa: "<<max.getVal()<<endl;
cout<<"NUOVA STAMPA"<<endl;
	queue1.stampa();
	queue1.increase_prior(2,100);
	cout<<"NUOVA STAMPA"<<endl;
	queue1.stampa();

}
