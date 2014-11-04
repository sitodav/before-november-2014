#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;

class persona{
	protected:
		string name;
	public:
	persona(string nome="DefaulPersontName");
	virtual void stampa_dati()=0;
		
};


class studente: public persona{
	
	public:
	studente(string nome="DefaultStudentName");
	void stampa_dati();
	
};

class professore: public persona{

	public:
	professore(string nome="DefaultProfessorName");
	void stampa_dati();

};


void richiama_stampa_su(persona &obj);


