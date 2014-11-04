#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <deque>
#include <algorithm>
#include <iterator>

using namespace std;



int main(int argc,char *argv[]){

	deque<int> deq1;
	ostream_iterator<int> it1(cout,"//");
	
	for(int i=0;i<10;i++)
		deq1.push_back(i*11);
	cout<<endl;	
	copy(deq1.begin(),deq1.end(),it1);
	deq1.push_front(28);
	cout<<endl;
	copy(deq1.begin(),deq1.end(),it1);
	deq1.insert(deq1.end(),100);
	cout<<endl;
	copy(deq1.begin(),deq1.end(),it1);
	deq1.pop_front();
	deq1.pop_front();
	cout<<endl;
	copy(deq1.begin(),deq1.end(),it1);
	deque<int>::iterator it2;
	
	it2=deq1.end()-1;
	int c=0;
	while(c++<=10){
		deq1.insert(it2,-1);
		it2--;	
		if(it2<=deq1.begin())
			break;
	}


	cout<<"-------------"<<endl;
	copy(deq1.begin(),deq1.end(),it1);

}
