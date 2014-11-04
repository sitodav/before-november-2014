#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/select.h>

using namespace std;



class gestore_rete{
protected:
	int porta;
	struct sockaddr_in address;
	int socketfd;
	string errore;
	bool fallito; 
public:
	gestore_rete(){};
	gestore_rete(int port,char * ip_numerico);
	int get_porta(){return porta;}
	struct sockaddr_in get_address_struct(){return address;}
	int get_socket(){return socketfd;}
	bool get_stato(){return fallito;} //stato relativo a prima che venga connesso il socket
					  //una volta connesso, se avviene errore, questo non viene 						  aggiornato
	string get_errore(){return errore;}
	virtual void start_comunicazione()=0; //ne viene fatto override (il virtual è per garantire il 		late binding) e la definizione non viene data, quindi questa è una classe astratta poichè ha una 	pure virtual function
	void invia(char *buff);
	int ricevi(char *buff);
};


//------------------------*

class gestore_client: public gestore_rete{
public:
	gestore_client(){};
	gestore_client(int port,char *srv_ip);
	void start_comunicazione();
	

};

//------------------------*


class gestore_server: public gestore_rete{
public:
	gestore_server(){};
	gestore_server(int port);
	void start_comunicazione();

};











