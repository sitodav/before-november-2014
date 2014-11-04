#include "classi.h"
#include <iostream>
using namespace std;



inputter::inputter(string path):file_stream(path){
	stream.open(path_file.c_str(),fstream::in);

}


void inputter::usalo(string stringa){
	char *buff=new char[20]();
	stream.getline(buff,20);
	cout<<buff;
}
