#include <iostream>
#include <cstring>

using namespace std;

class a{
public:
    void funz() throw(string);
};

void a::funz()  {
    throw string("oi");
}

int main(){
a obj;
obj.funz();

}
