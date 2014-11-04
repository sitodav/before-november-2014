#include "classi.h"



outputter::outputter(string path):file_stream(path){
	stream.open(path_file.c_str(),fstream::out | fstream::app);

}


void outputter::usalo(string stringa){
	stream<<stringa;
	stream<<endl;
}
