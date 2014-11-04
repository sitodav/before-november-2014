#include <vector>
#include <iostream>

using namespace std;

int &ret(vector<int> &vect1){
	return vect1[0];


}

int main(int argc,char *argv[]){
	vector<int> vect1,vect2;
	int a=17;
	vect1.push_back(a);
	vect2.push_back(ret(vect1));
	vect2[0]++;
	cout<<vect2[0];


}


