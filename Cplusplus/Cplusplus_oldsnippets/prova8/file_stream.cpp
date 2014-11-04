#include "classi.h"

string file_stream::get_path() const{
	return path_file;
}

void file_stream::chiudilo(){
	stream.close();
}
