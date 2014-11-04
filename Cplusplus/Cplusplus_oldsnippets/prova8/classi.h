#include <fstream>
#include <cstring>
using namespace std;


class file_stream{
protected:
	fstream stream;
	const string path_file;
public:
	file_stream(string path="file.txt"):path_file(path){}
	string get_path() const;
	virtual void usalo(string stringa)=0;
	void chiudilo();

};


class outputter: public file_stream{
public:
	outputter(string path="file.txt");
	void usalo(string stringa);
	

};



class inputter: public file_stream{
public:
	inputter(string path="file.txt");
	void usalo(string stringa=" ");
	

};
