//SERVER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#define MAX_CLIENTS 100
#define MAX_NOTIFICHE 20
#define E_WRITING " :impossibile scrivere sul socket! Connection Aborting...\n"
#define E_READING " :impossibile leggere sul socket! Connection Aborting...\n" 
#define ABORT " :Connection Aborting\n"
#define FEEDBACK1 "Segnalazione non aggiunta.\nAvviata invece gestione della buca!\n\0"
#define FEEDBACK2 "Operazione confermata dal server\n\0"
#define FEEDBACK3 "Errore di sistema del server! Impossibile completare op.\n\0"
#define FEEDBACK4 "Impossibile eliminare segnalazione specificata\n\0"
#define LOGGEDNOIDENT " ha loggato--in attesa di identificazione\n"
#define DISCONNECTED " : si e' disconnesso\n" 
#define ADDED " : ha aggiunto una segnalazione di una buca\n"
#define STARTGESTIONE " : ha fatto attivare la gestione di una buca\n"
#define MAX_SEGNALAZ_SOSPESE 2
#define SIZE_RECORDSEGNALAZIONE 94
#define DELETED " : ha eliminato sua segnalazione di buca\n"


//IMPLEMENTA LA FULL_READ AL POSTO DI TUTTE LE READ!




int fdLogFile,fdBucheFile; //i file li apriamo nel main thread e li usiamo nei thread (quindi i fd dei file li tengo globali), am siccome in contesto multithreading, i thread condividono il puntatore dell'offeset sul file, ogni volta che vogliamo scrivere su un file o leggerlo, da un thread, accediamo ad un mutex, ci spostiamo in una posizione desiderata, facciamo quello che dobbiamo fare, rilasciamo il mutex e continuiamo (

pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER,mutex2=PTHREAD_MUTEX_INITIALIZER,mutex3=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex4=PTHREAD_MUTEX_INITIALIZER;

struct client{ //in queste struct salviamo i dati dei client connessi (usiamo array di struct globale allocato dinamicamente sul puntatore tabClients)
	pthread_t tid;
	int connSockFd;
	char *nome;
	char *cognome;
	char *nick;
	char *eta;
	char *ip;

}	*tabClients;


struct notifica{ //qui usiamo un array di queste struct (tabNotifiche) allocato dinamicamente
	char *nickDest; //per salvare le notifiche da girare ai client quando si connettono
	char *messaggio;
}	*tabNotifiche;
int lastNotif=0; //lo usiamo come indice dell'ultimo elemento da sovrascrivere, riazzerandolo
//quando raggiunge MAX_NOTIFICHE


//mentre nella tab dei client, se finiscon oi posti vuoti un client non puo' connettersi, e ogni volta che un client va via puo' liberare qualunque posto, la tab delle notifiche la riempiamo in modulo. Cioe' la riempiamo tutta, poi ricominciam oa riempirla da capo. In quest modo le notifiche obsolete verranno cancellate dopo un tot di tempo, a seconda della dimensione della tabNotifiche


void add_log(char *mex); //questa funzione si occupa di gestire i log sul logfile
void *connHandlerThread(void * arg); //questa e' la funzione in cui parte ogni thread che gestisce ogni client connesso
void data_erase(struct client *myRecord ); //questa funzione chiude e azzera alcuni elementi relativi ai client che si disconnettono
int checkAndAdd(struct client *myRecord,char *riferimento,char *x,char *y); //questa funzione controlla se quando e' richiesto di inserire una nuova segnalazione di buca, questa va inserita (e in tal caso lo fa) oppure, se ce ne sono gia' 2, di lanciare la gestione
int checkAndDel(struct client *myRecord, char *riferimento); //questa funzione controlla se esiste (e se e' dell'utente) la segnalazione che l'utente chiede di cancellare e in caso affermativo la cancella


int main(int argc,char *argv[]){

tabClients=(struct client *)calloc(MAX_CLIENTS,sizeof(struct client));
tabNotifiche=(struct notifica *)calloc(MAX_NOTIFICHE,sizeof(struct notifica)); 




int listenSockFd,connSockFd,myPort,i,ret;

//apriamo i file
if((fdLogFile=open("logfile.txt",O_APPEND | O_WRONLY | O_CREAT,00666) )<0){
	fprintf(stderr,"\nErrore riga 84\n");
	exit(1);
}

if((fdBucheFile=open("buchefile.txt",O_RDWR | O_CREAT,00666))<0){
	fprintf(stderr,"\nErrore riga 89\n");
	exit(1);
}

struct sockaddr_in myAddr;

if(argc!=3){
	fprintf(stderr,"\nerrore: passami ip su cui bindare(0 per bindare su INADDR_ANY) e porta!\n");
	exit(1);
}

sscanf(argv[2],"%d",&myPort);
myAddr.sin_port=htons(myPort);
myAddr.sin_family=AF_INET;

if(!strcmp(argv[1],"0")) //se si passa 0 come indirizzo di bind, binda su ANY
	myAddr.sin_addr.s_addr=htons(INADDR_ANY);
else
	inet_pton(AF_INET,argv[1],&myAddr.sin_addr);
//la struct e' pronta

if((listenSockFd=socket(AF_INET,SOCK_STREAM,0))<0){
	fprintf(stderr,"\nErrore riga 111\n");
	exit(1);
}


if(bind(listenSockFd,(struct sockaddr *)&myAddr,sizeof(myAddr))<0){
	fprintf(stderr,"\nErrore riga 117\n");
	exit(1);
}

if(listen(listenSockFd,1024)<0){
	fprintf(stderr,"\nErrore riga 122\n");
	exit(1);
}


//socket pronto a ricevere connessioni

while(1){ //ciclo while in cui rimane il main thread
	do
		connSockFd=accept(listenSockFd,NULL,NULL);
	while	(connSockFd<0 && errno==EINTR);
	if(connSockFd<0){
		fprintf(stderr,"\nErrore riga 134\n");
		exit(1);	
	}
	//client connesso su connsock
	//accesso al mutex3 per trovare una posizione libera sulla tabella utenti connessi (serve la critical section
	//per evitare che magari accediamo alla tabella mentre uno dei thread sta gestendo la cancellazione del suo utente da questa)
	pthread_mutex_lock(&mutex3);
	for(i=0;i<MAX_CLIENTS;i++)
		if(tabClients[i].connSockFd==0) //a 0 vuol dire che nessuno l'ha preso
			break;
	if(i==MAX_CLIENTS){ //non sono stati trovati posti disponibili, quindi viene rifiutata la conn al client
		pthread_mutex_unlock(&mutex3);		
		continue;
	}
	else{ //trovato iesimo posto vuoto
		tabClients[i].connSockFd=connSockFd;
		pthread_mutex_unlock(&mutex3);
		//rilasciamo il mutex, e creiamo il thread al quale passiamo il resto della gestione del client
		//mentre il main thread torna a gestire il listening socket
		if((ret=pthread_create(&(tabClients[i].tid),NULL,connHandlerThread,(void *)(tabClients+i)  ))!=0){
			fprintf(stderr,"\nErrore riga 154 %s\n",strerror(ret));
		}
		
		
	}	
	

}


}


void *connHandlerThread(void * arg){
//per gestire le situazioni di errore all'interno dei thread, scriviam ouna funzione data_erase che chiude gli fd aperti e cancella il record sulla tabella del client di questo thread,e poi il thread muore ritornando

struct sockaddr_in clientAddr;
struct client *myRecord=(struct client *)(arg);
int size=sizeof(clientAddr),ret,i;
char *buff=(char *)calloc(100,sizeof(char));
char *riferimento=(char *)calloc(21,sizeof(char));
char *x=calloc(4,sizeof(char));
char *y=calloc(4,sizeof(char));


char *logBuff=(char *)calloc(50,sizeof(char)),*bigBuff=(char *)calloc(SIZE_RECORDSEGNALAZIONE+1,sizeof(char));;

//per prima cosa, otteniamo l'ip del client per il quale questo thread gestisce la connessione



myRecord->nick=calloc(21,sizeof(char));

myRecord->nome=calloc(21,sizeof(char));

myRecord->cognome=calloc(21,sizeof(char));

myRecord->eta=calloc(3,sizeof(char));

myRecord->ip=calloc(INET_ADDRSTRLEN,sizeof(char));




if(getpeername( myRecord->connSockFd,(struct sockaddr *)&clientAddr,&size )<0){
	fprintf(stderr,"\nErrore riga 199\n");
	data_erase(myRecord);
	return;
}

if(inet_ntop(AF_INET,&clientAddr.sin_addr, myRecord->ip,INET_ADDRSTRLEN)==NULL){
	fprintf(stderr,"\nErrore riga 205\n");
	data_erase(myRecord);	
	return;
}

//abbiamo ip del client, e scriviamo un log che dice che quell'ip si e' connesso



strcpy(logBuff,myRecord->ip);
strcat(logBuff,LOGGEDNOIDENT);
add_log( logBuff); //usiamo funzione scritta da noi che lavora sul logfile


while(1){
		
	ret=read(myRecord->connSockFd,buff,100); 
	
	
	
	if(ret<0){
		fprintf(stderr,"\nErrore riga 226\n");
		
		
		
		
		strcpy(logBuff,myRecord->ip);
		strcat(logBuff,E_READING);
		add_log( logBuff);
		data_erase(myRecord);
		return;
	}
	else if(ret==0){ //il client ci ha mandato FIN per disconnettersi
		
		

		
			
		strcpy(logBuff,myRecord->ip);
		strcat(logBuff,DISCONNECTED);
		add_log( logBuff );
		data_erase(myRecord);
		return;
	}
	else{ //allora abbiamo letto normalmente, quindi controlliamo il codice (di protocollo) del messaggio ricevuto
		if(buff[0]==3){ //il client ci sta mandando i suoi dati
				
			//tiriamo fuori (secondo il protocollo) i vari dati inviatici dal 				client e ce li salviamo nella struct globale dei client 
			strncpy( myRecord->nick,buff+1,20 );
			myRecord->nick[20]='\0';
			strncpy( myRecord->nome,buff+21,20);
			myRecord->nome[20]='\0';
			strncpy( myRecord->cognome,buff+41,20);
			myRecord->cognome[20]='\0';
			strncpy( myRecord->eta,buff+61,2);
			myRecord->eta[2]='\0';
			
			//e concateniamo le varie stringhe per creare un messaggio da 				scrivere nel log file che indichi che l'utente che si era connesso 				prima con quel dato ip, ora ha fornito i dati identificativi
			
			
			
					
			strcpy(logBuff,myRecord->ip);
			strcat(logBuff," IS> ");
			strcat(logBuff,myRecord->nick);
			strcat(logBuff,myRecord->nome);
			strcat(logBuff,myRecord->cognome);
			strcat(logBuff,myRecord->eta);
			strcat(logBuff,"\n");
			
			add_log(logBuff);
			//ora che siamo identificati da un nick, possiamo scaricare verso il 				client che stiamo gestendo, tutti i messaggi relativi alle vecchie 				segnalazioni gestite
			//accediamo quindi alla tabNotifiche sotto mutex4
			pthread_mutex_lock(&mutex4);
			
			for(i=0;i<MAX_NOTIFICHE;i++){
				if(tabNotifiche[i].nickDest==NULL)
					break;
				
				if( !strcmp(myRecord->nick,tabNotifiche[i].nickDest) )	//allora c'e' corrispondenza, e inviamo la notifica al client che stiamo gestendo			 	
				 {	
					
					if(write(myRecord->connSockFd,tabNotifiche[i].messaggio,strlen(tabNotifiche[i].messaggio))<0){				pthread_mutex_unlock(&mutex4);
						fprintf(stderr,"\nErrore riga 288\n");

						
											
						strcpy(logBuff,myRecord->ip);
						strcat(logBuff,ABORT);
						add_log( logBuff);
						data_erase(myRecord);
						return;
						
					}
				
				}	
						
			}
			pthread_mutex_unlock(&mutex4);
			
		}

		else if(buff[0]==1){ //il client vuole vedere la lista delle segnalazioni, quindi facciamo lock del mutex1, spostiamo il puntatore al file delle buche all'inizio, leggiamo tutto il file, rilasciamo lock e inviamo il tutto al client
						
			pthread_mutex_lock(&mutex1);
			lseek(fdBucheFile,0,SEEK_SET);
			ret=0;

			while((ret=read(fdBucheFile,bigBuff,SIZE_RECORDSEGNALAZIONE))==SIZE_RECORDSEGNALAZIONE) {
			//inviamolo al client
				
				bigBuff[SIZE_RECORDSEGNALAZIONE]='\0';			
		
				if(write(myRecord->connSockFd,bigBuff,SIZE_RECORDSEGNALAZIONE)<0){
					fprintf(stderr,"\nErrore riga 319\n");
					pthread_mutex_unlock(&mutex1);

					
					

					strcpy(logBuff,myRecord->ip);
					strcat(logBuff,E_WRITING);
					add_log( logBuff);
					data_erase(myRecord);
					return;
						
				}
				strcpy(bigBuff,"\0");
			}
			if(ret<0){
					fprintf(stderr,"\nErrore riga 335\n");
					pthread_mutex_unlock(&mutex1);
					
					

					strcpy(logBuff,myRecord->ip);
					strcat(logBuff,E_READING);
					add_log( logBuff);
					data_erase(myRecord);
					return;
							
				}

			pthread_mutex_unlock(&mutex1);
			//se siamo arrivati qui vuol dire che siamo riusciti a mandare al client il contenuto del file
			//con tutte le buche salvate
			//quindi ricominciamo
			continue;
			
			
			
		
		}
		else if(buff[0]==2){ //allora il client ci sta inviando i dati per una nuova 					    segnalazione
			//per prima cosa li tiriamo fuori
			

			strncpy(riferimento,buff+1,20);
			riferimento[20]='\0';
			strncpy(x,buff+21,3);
			x[3]='\0';
			strncpy(y,buff+24,3);
			y[3]='\0';
			
			int check;
			//poi li passiamo ad una funzione che si occupa di vedere se ci sono nel file delle buche (accede in mutex1)gia' 2 segnalazioni nelle vicinanze, e in quel caso rilascia mutex1 e ritorna 0, non aggiungendo la segnalazione, ma si occupera' di A) cambiare lo stato delle 2 segnalazioni gia' presenti (xke' vuol dire che con la terza segn arrivata abbiamo avviato un'opera di gestione) B) mettere le notifiche nella tabNotifiche che verranno consegnate ai 2 utenti che avevano fatto quella segnalazione, e noi scriveremo direttamente adesso sul socket del client che non e' stata gestita la sua segnalazione xke' invece grazie a lui e' stata avviata opera di gestione. In caso invece non ci siano gia' 2 segnalaz nel file etc, la funzione la aggiunge e stop, e ritorna 1, e noi avvisiamo il client
			if((check=checkAndAdd(myRecord,riferimento,x,y))==0){ //la funz ha ritornato 0, quindi ci sono gia'
			//2 segnalazioni nel file nelle vicinanze di quel riferimento, quindi
			//non abbiamo creato la segnalazione nel file(e quindi lo diciamo al 				client) ma abbiamo avviato gestione (la funzione si e' occupata di 
			//cambiare lo stato di quelle buche e mettere nella tabNotifiche le 				notifiche relative ai segnalatori). Noi dobbiamo quindi avvisare 
			//soltanto il client  
				if(write(myRecord->connSockFd,FEEDBACK1,64)<0){
					fprintf(stderr,"\nErrore riga 377\n");
					
					

					strcpy(logBuff,myRecord->ip);
					strcat(logBuff,E_WRITING);
					add_log( logBuff);
					data_erase(myRecord);
					return;
									
				}
				//e aggiungiamo un log sul file relativo ad inizio gestione
				
				

				strcpy(logBuff,myRecord->ip);
				strcat(logBuff,STARTGESTIONE);
				add_log( logBuff);
			
			}
			else if(check==1){ //in questo caso la funzione checkAndAdd ha ritornato 1, 				quindi la segnalazione e' stata normalmente aggiunta! Confermiamo al 				client
				if(write(myRecord->connSockFd,FEEDBACK2,34)<0)		{
					fprintf(stderr,"\nErrore riga 399\n");
					
					
					strcpy(logBuff,myRecord->ip);
					strcat(logBuff,E_WRITING);
					add_log( logBuff);
					data_erase(myRecord);
					return;
									
				}
				
				
	
				strcpy(logBuff,myRecord->ip);
				strcat(logBuff,ADDED);
				add_log( logBuff);
				
			}
			else if(check<0){ //la funzione non e' riuscita a scrivere sul file, 						avvisiamo il client
				if(write(myRecord->connSockFd,FEEDBACK3,58)<0)		{
					fprintf(stderr,"\nErrore riga 419\n");
					
					
					strcpy(logBuff,myRecord->ip);

					strcat(logBuff,E_WRITING);
					add_log( logBuff);
					data_erase(myRecord);
					return;
				}
						
			}
		}
		else if(buff[0]==4){
			//il client vuole cancellare segnalazione. Basta che ci invia il riferimento, e noi identificheremo il record come primo che ha corrispondenza nick & riferimento
		     
		     	
		     
		     strncpy(riferimento,buff+1,20);
		     riferimento[20]='\0';
	
		   //lanciamo una funzione che ritorna 1 se riesce a trovare e ad eliminare il record, altrimenti ritorna 0, o -1 in caso di errore. Noi a seconda dei casi avvisiamo il client confermando o meno.
		     int del;
		     del=checkAndDel(myRecord,riferimento);

		     if(del>0){ //avvisiamo il client mandandogli conferma e aggiorniamo log
				if(write(myRecord->connSockFd,FEEDBACK2,34)<0)	{
					fprintf(stderr,"\nErrore riga 446\n");
					
					

					strcpy(logBuff,myRecord->ip);
					strcat(logBuff,E_WRITING);
					add_log( logBuff);
					data_erase(myRecord);
					return;
									
				}
				//e aggiungiamo un log sul filelog
				
				

				strcpy(logBuff,myRecord->ip);
				strcat(logBuff,DELETED);
				add_log( logBuff);
	
		     }
		     else if(del==0){	
				if(write(myRecord->connSockFd,FEEDBACK4,64)<0){
					fprintf(stderr,"\nErrore riga 468\n");
					
					

					strcpy(logBuff,myRecord->ip);
					strcat(logBuff,E_WRITING);
					add_log( logBuff);
					data_erase(myRecord);
					return;
									
				}
				//e non aggiungiamo niente sul log
		     }
		     else{
				if(write(myRecord->connSockFd,FEEDBACK3,58)<0)		{
					fprintf(stderr,"\nErrore riga 483\n");
					
					

					strcpy(logBuff,myRecord->ip);
					strcat(logBuff,E_WRITING);
					add_log( logBuff);
					data_erase(myRecord);
					return;
				}
				//e niente sul log
		     }		
			
		}
		
			
	}
	

}



}



void data_erase(struct client *myRecord ){
	pthread_mutex_lock(&mutex3);

	close(myRecord->connSockFd);
	myRecord->connSockFd=0;
	

	pthread_mutex_unlock(&mutex3);
	
}

void add_log(char *mex){

//accediamo in mutua esclusione al file dei log, ci spostiamo alla fine del file (in realta' non ce ne sarebbe bisogno
//visto che essendo aperto in append, ogni write scrive alla fine del file), aggiungiamo il messaggio del log
//e rilasciamo il mutex

pthread_mutex_lock(&mutex2);
lseek(fdLogFile,0,SEEK_END);
if(write(fdLogFile,mex,strlen(mex))<0)
	fprintf(stderr,"\nErrore riga 530\n");
	//proseguaimo senza terminare il processo se c'e' errore di scrittura del log

pthread_mutex_unlock(&mutex2);
return;

}


int checkAndAdd(struct client *myRecord,char *riferimento,char *x,char *y){
//tieni presente che il campo coord x sono 3 char, e che un char che rappresenta il numero 0
//vale 48 
char *buff=(char *)calloc(SIZE_RECORDSEGNALAZIONE,sizeof(char));
char *recordToSave;


int ret,count=0,diff,diff2;
int nostraXDecimal=0,nostraYDecimal=0,xDecimal=0,yDecimal=0;

int buff2[MAX_SEGNALAZ_SOSPESE];

nostraXDecimal+=((int)x[0]-48)*100+((int)x[1]-48)*10+((int)x[2]-48);
nostraYDecimal+=((int)y[0]-48)*100+((int)y[1]-48)*10+((int)y[2]-48);



//dobbiamo aprire il file sotto mutex1, leggere un record alla volta (secondo il protocollo
//di salvataggio dei record sul file di testo) e controllare prima se gia' e' presente
//questo stesso riferimento

pthread_mutex_lock(&mutex1);

lseek(fdBucheFile,0,SEEK_SET);
while((ret=read(fdBucheFile,buff,SIZE_RECORDSEGNALAZIONE))>0){
	if(count==MAX_SEGNALAZ_SOSPESE) 
		break; //per come gestiamo il file non puo' accadere questo, ma lo mettiamo per evitare che possa accadere per una cattiva gestione e capiti seg fault su buff2
	if(!strncmp(riferimento,buff+1,20) && buff[0]=='0'){ //allora abbiamo trovato segnalazione con stato a 0 (non risolta) e stesso riferimento
		//allora dobbiamo fare il calcolo delle coordinate, che sono in char
		//ma dobbiamo paragonarle in int	
	
		xDecimal=((int)buff[21]-48)*100+((int)buff[22]-48)*10+((int)buff[23]-48);
		yDecimal=((int)buff[24]-48)*100+((int)buff[25]-48)*10+((int)buff[26]-48);
		
		//e ora facciamo i confronti //HO MESSO AL PIU' di 10 m di distanza 			complessiva in entrambe le coordinate
		diff=xDecimal-nostraXDecimal; 
		diff2=yDecimal-nostraYDecimal;
		if(diff>-10 && diff<10 && diff2>-10 && diff2<10 ){ //allora conteggio valido
			
			count++;
		//e ci salviamo in una struttura dati apposita (e piccola) gli 			offset di inizio record sul file senza doverci riscorrere tutto il file possiamo modificare gli stati dei record direttamente saltando al loro offset, e possiamo tirarci fuori dai record i dati dei client, senza dover tenere ancora impegnato il mutex1 del file		
			
			buff2[count-1]=lseek(fdBucheFile,0,SEEK_CUR);
			buff2[count-1]-=SIZE_RECORDSEGNALAZIONE;

				
		}
		
	}
		

}



if(ret<0){
	fprintf(stderr,"\nErrore riga 595\n");
	pthread_mutex_unlock(&mutex1);
	return -1;

}





if(count==MAX_SEGNALAZ_SOSPESE){
//arrivato qui, buff2 tiene i valori di offset di inizio record nel file dove ci sono le 
//segnalazioni che hanno coordinate valide e riferimento per iniziare la gestione della buca 
//invece di aggiungere la segnalazione
//ci spostiamo sugli offset di inizio dei record e modifichiamo il primo char di stato e tiriamo fuori i record con i dati dei segnalatori, per aggiungere notifiche alla tab delle notifiche, che poi questi leggeranno (sempre ovviamente 2 notifiche per i 2 segnalatori che avevano lasciato la segnalazione che ora stiamo gestendo)

	int i,j;
	
	

	for(i=0;i<MAX_SEGNALAZ_SOSPESE;i++){
		//modifica del che di stato segnalazione (sovrascriviamo 1)
		lseek(fdBucheFile,buff2[i],SEEK_SET);
		if(write(fdBucheFile,"1",sizeof(char))<0){
			fprintf(stderr,"\nErrore riga 619\n");
			pthread_mutex_unlock(&mutex1);
			
			return -1;		
		}
		//tiriamo fuori il record, e con i dati creiamo notifica che mettiamo 			nell'elemento che allochiamo della tab notifiche (alla quale accediamo sotto 			mutex4)
		lseek(fdBucheFile,buff2[i],SEEK_SET);
		if(read(fdBucheFile,buff,SIZE_RECORDSEGNALAZIONE)<0){
			fprintf(stderr,"\nErrore riga 627\n");
			pthread_mutex_unlock(&mutex1);
			
			return -1;	
		}

		pthread_mutex_lock(&mutex4);
		
		if(lastNotif==MAX_NOTIFICHE)
			lastNotif=0;

		
		tabNotifiche[lastNotif].nickDest=(char *)calloc(21,sizeof(char));
		
		strncpy(tabNotifiche[lastNotif].nickDest,buff+27,20);
		tabNotifiche[lastNotif].nickDest[20]='\0';
	

		
		tabNotifiche[lastNotif].messaggio=(char *)calloc(50,sizeof(char));
		
		strcpy(tabNotifiche[lastNotif].messaggio,"NOTIFICA>:Iniziata gestione buca: \0");
		strcat(tabNotifiche[lastNotif].messaggio,riferimento); //riferimento lo prendiamo direttamente da input a funzione, tanto e' per quello che e' partito tutto
		


		lastNotif++;
		pthread_mutex_unlock(&mutex4);



		
	}
	
	
	pthread_mutex_unlock(&mutex1);
	return 0;

}

else{ //non ci sono gia' le segnalazioni presenti in numero sufficiente o in distanze valide
//per poter avviare la gestione, quindi ci limitiamo ad aggiungere la segnalazione sul file, e la funzione ritorna 1 (cosi' il thread chiamante sa che deve avvisare il client che la segnalazione e' avvenuta)
	
		
	
	
	//generiamo quindi il formato di record da salvare nel file
	
	recordToSave=(char *)calloc(SIZE_RECORDSEGNALAZIONE,sizeof(char));
	
	int i;
	for(i=0;i<SIZE_RECORDSEGNALAZIONE;i++)
		recordToSave[i]=' ';
	recordToSave[0]='0'; //perche' ovviamente non e' gestita
	recordToSave[1]='\0'; //perche' la strcat incolla sul primo \0 che trova nella dest
	strcat(recordToSave,riferimento);
	recordToSave[21]='\0';
	strcat(recordToSave,x);
	recordToSave[24]='\0';
	strcat(recordToSave,y);
	recordToSave[27]='\0';
	strcat(recordToSave,myRecord->nick);
	//il nick arriva fino a char di indice 46 (20 char in tutto partendo dal 27 incluso)
	//e sul 47 c'e' gia' l'\0 poiche' myRecord->nick in realta' e' di 21 char, di cui il 		21esimo e' \0
	strcat(recordToSave,myRecord->nome);
	//e lo stesso per nome e gli altri dati
	strcat(recordToSave,myRecord->cognome);
	strcat(recordToSave,myRecord->eta);
	strcat(recordToSave,"FINE\n");
	
	lseek(fdBucheFile,0,SEEK_END);
	
	if(write(fdBucheFile,recordToSave,SIZE_RECORDSEGNALAZIONE)<0){
		fprintf(stderr,"\nErrore riga 700\n");
		pthread_mutex_unlock(&mutex1);
		
		return -1;	
	}	
	
	

	pthread_mutex_unlock(&mutex1);
	return 1;	
}



}

int checkAndDel(struct client *myRecord, char *riferimento){
char *buff=(char *)calloc(SIZE_RECORDSEGNALAZIONE,sizeof(char));
int off,i,ret;

pthread_mutex_lock(&mutex1);

lseek(fdBucheFile,0,SEEK_SET);
while((ret=read(fdBucheFile,buff,SIZE_RECORDSEGNALAZIONE))>0){
	if(!strncmp(riferimento,buff+1,20) && !strncmp(myRecord->nick,buff+27,20) ){ //trovata segnalazione con quel riferimento
		lseek(fdBucheFile,-94,SEEK_CUR);
		if(write(fdBucheFile,"DELETED",7)<0){
			fprintf(stderr,"\nErrore riga 727\n");
			pthread_mutex_unlock(&mutex1);
			return -1;		
		}
		for(i=0;i<86;i++)
			if(write(fdBucheFile,"*",sizeof(char))<0){
			fprintf(stderr,"\nErrore riga 733\n");
			pthread_mutex_unlock(&mutex1);	
			
			return -1;		
			}
		if(write(fdBucheFile,"\n",sizeof(char))<0){
			fprintf(stderr,"\nErrore riga 739\n");
			pthread_mutex_unlock(&mutex1);
			
			return -1;		
		}
		
		//e ritorniamo
		
		pthread_mutex_unlock(&mutex1);
		return 1;	
		
			
	}


}

pthread_mutex_unlock(&mutex1);

return 0;


}



