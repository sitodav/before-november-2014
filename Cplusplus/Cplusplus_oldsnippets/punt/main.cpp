#include <cstdlib>
#include <cstring>
#include <vector>
#include <iterator>
#include <iostream>
#include <map>

using namespace std;

int main(){

map<int,int> map1;
map1[1]=1;
map1[2]=2;

map<int,int>::iterator it;

map1[4];
map1[3];

map1.find(2)->second=13;
for(it=map1.begin();it!=map1.end();it++)
	cout<<"key "<<it->first<<" value: "<<it->second<<endl;


}
