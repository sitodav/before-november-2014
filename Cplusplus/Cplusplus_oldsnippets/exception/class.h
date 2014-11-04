//class.h

#include <iostream>
#include <cstdlib>
#include <cstring>


using namespace std;

class classeA{
	
	protected:
		static int x;
		
	public:
		int y;
		classeA(int val=1):y(val){}
		void moltiplica(int fatt=2);

};
