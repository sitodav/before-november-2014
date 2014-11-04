#include "network.h"


gestore_rete::gestore_rete(int port,char * ip_numerico){
		fallito=false;
	
		porta=port;
		address.sin_port=htons(porta);
		
		address.sin_family=AF_INET;
		
		address.sin_addr.s_addr=inet_addr(ip_numerico);
		//address.sin_addr.s_addr=htonl(ip_numerico); //l'ip deve essere già passato
		//come numerico, ma non già in network order

		//il socket viene creato tramite i metodi della classe figlia, non qui quindi
}



void gestore_rete::invia(char *buff){
	if(write(socketfd,buff,1024)<0){
		perror("");
		exit(1);	
	}

}




int gestore_rete::ricevi(char *buff){
//questa funzione ritorna 0 se non abbiamo ricevuto niente e la select è andata in timeout
//altrimenti >0 se abbiamo letto qualcosa (ed il risultato viene messo in buff)
//se invece il peer si disconnette (la read ritorna 0) o se c'è un errore sulla read
//il programma termina
	fd_set set1;
	FD_ZERO(&set1);
	FD_SET(socketfd,&set1);
	struct timeval time; 
	time.tv_sec=0.0;
	time.tv_usec=10.0;

	if(select(socketfd+1,&set1,NULL,NULL,&time)==0) //è scattata per il timeout allora
		return 0;
	

	int ret;
	memset(buff,0,1024);
	if((ret=read(socketfd,buff,1024))<=0){
		//quindi il peer si è disconnesso (RST by peer, se legge 0 bytes) oppure c'è stato errore
		perror("");
		exit(1);
	}
	return 1;


}


//----------------------------------------------*




gestore_client::gestore_client(int port,char *srv_ip): gestore_rete(port,srv_ip){
		//a questo punto la struct address è riempita con la porta, la famiglia e l'ip
		if((socketfd=socket(AF_INET,SOCK_STREAM,0))<0){
				perror("");
				errore=string("errore creazione socket");
				fallito=true;
		}

}




void gestore_client::start_comunicazione(){
		if(fallito)
			return;
		if(connect(socketfd,(struct sockaddr *)&address,sizeof(address))<0){
			errore=string("Errore di connessione");
			perror("");
			fallito=true;	
			return;	
		}
		cout<<"connesso al server"<<endl;
}







//------------------------------------------------*





gestore_server::gestore_server(int port) : gestore_rete(port,"0.0.0.0"){
	//a questo punto la struct address è riempita con la porta, la famiglia e l'ip
		
	if((socketfd=socket(AF_INET,SOCK_STREAM,0))<0){
				perror("");
				errore=string("errore creazione socket");
				fallito=true;
	}
}



void gestore_server::start_comunicazione(){
		if(fallito)
			return;
		
		if(bind(socketfd,(struct sockaddr *)&address,sizeof(address))<0){
			perror("");
			errore=string("errore bind");
			fallito=true;
			return;		
		}
		
		if(listen(socketfd,1024)<0){
			perror("");
			errore=string("errore listen");
			fallito=true;
			return;
		}
		
		int t;
		if((t=accept(socketfd,0,0))<0){
			perror("");
			errore=string("errore accept");
			fallito=true;
			return;
		
		}	
		//in t c'è il socket di connessione, e in socketfd c'è ancora quello di listen
		//che chiudiamo, e ci salviamo quello in t
		close(socketfd);
		socketfd=t;
		cout<<"CLIENT CONNESSO"<<endl;	
}


