#include <iostream>
#include <cstring>
#include <vector>
#include <iterator>
#include <algorithm>


using namespace std;

int main(){
	
	vector<int> vect1;
	int i;
	for(int i=0;i<10;i++)
		vect1.push_back(i*10);
	for(int i=0;i<vect1.size();i++)
		vect1[i]*=2;
	
	vector<int>::iterator it=vect1.begin();	
	ostream_iterator<int> it2(cout,"-");
	copy(vect1.begin(),vect1.end(),it2);
	int *a=new int[10];
	for(int i=0;i<10;i++)
		*(a+i)=*(it++);
	copy(a,a+10,it2);
}
