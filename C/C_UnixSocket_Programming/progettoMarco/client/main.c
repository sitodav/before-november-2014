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




int connSockFd;



void * thread_lettore(void *arg);
void visualizza(void);
void aggiungi(void);
void elimina(void);

int main(int argc,char *argv[]){
if(argc!=3){
	printf("\nerrore: passami ip e porta server!\n");
	exit(1);

}

int serverPort;
struct sockaddr_in servind;
char *toSend;
pthread_t tid;

sscanf(argv[2],"%d",&serverPort);

servind.sin_family=AF_INET;
servind.sin_port=htons(serverPort);
if(inet_pton(AF_INET,argv[1],&servind.sin_addr)!=1){
	perror("\n");
	exit(1);
}

//struct pronta per connetterci
if((connSockFd=socket(AF_INET,SOCK_STREAM,0))<0){
	perror("\n");
	exit(1);
}
	
char *nick=calloc(20,sizeof(char)),*nome=calloc(20,sizeof(char)),*cognome=calloc(20,sizeof(char)),*eta=calloc(4,sizeof(char));

printf("\nInserisci nick\n");
fgets(nick,20,stdin);
nick[strlen(nick)-1]=' ';

printf("\nInserisci nome\n");
fgets(nome,20,stdin);
nome[strlen(nome)-1]=' ';

printf("\nInserisci cognome\n");
fgets(cognome,20,stdin);
cognome[strlen(cognome)-1]=' ';

printf("\nInserisci eta\n");
fgets(eta,4,stdin);
eta[strlen(eta)-1]=' ';



printf("\n%s%s%s%s\n",nick,nome,cognome,eta);
fflush(stdout);

if(connect(connSockFd,(struct sockaddr *)&servind,sizeof(servind))<0){
	perror("\n");
	exit(1);

}

printf("\nSIAMO CONNESSI AL SERVER\n");
fflush(stdout);
toSend=(char *)calloc(100,sizeof(char));
//creiamo un messaggio che segua il protocollo per l'invio dei dati della persona


strcpy(toSend,"                                                               ");
toSend[0]=3;
strncpy(toSend+1,nick,strlen(nick)-1); 
strncpy(toSend+21,nome,strlen(nome)-1);
strncpy(toSend+41,cognome,strlen(cognome)-1);
strncpy(toSend+61,eta,strlen(eta)-1);

if(write(connSockFd,toSend,strlen(toSend))<0){
	perror("\n");
	exit(1); 
}

printf("\nDati inviati!\n");
fflush(stdout);

//lanciamo il thread lettore, che si mette in attesa di leggere sul socket

int ret;
if((ret=pthread_create(&tid,NULL,thread_lettore,(void *)NULL))!=0){
	printf("errore 100 %s\n",strerror(ret));
	exit(1);
}	


int ris;

while(1){
	printf("\n[1]:visualizza lista segnalazioni effettuate\n");
	printf("\n[2]:aggiungi una segnalazione\n");
	printf("\n[3]:elimina una segnalazione\n");
	fflush(stdout);
	scanf("%d",&ris);
	while(getchar()!='\n');
	if(ris==1)
		visualizza();
	else if(ris==2)
		aggiungi();
	else if(ris==3)
		elimina();
	else{
		printf("\nScelta non valida, riprova!\n"); fflush(stdout);		
		continue;
	}
}


}

void * thread_lettore(void *arg){
int ret;
char *buff=NULL;

while(1){
		
	buff=(char *)calloc(100,sizeof(char));	
	ret=read(connSockFd,buff,100);
	if(ret==0){
		printf("\nIl server ci ha disconnessi!\n");
		fflush(stdout);
		//quindi buttiamo giu' tutto il processo, e non solo questo thread
		exit(0);	
	}
	if(ret<0){
		printf("\nErrore in lettura!\n");
		fflush(stdout);
		//anche in questo caso uccidiamo tutto il processo
		exit(0);	
	}
	printf("%s",buff);
	fflush(stdout);
	
	
	
}

}



void visualizza(void){
char *toSend=(char *)calloc(100,sizeof(char));
//creiamo un messaggio che segua il protocollo per l'invio richiesta di ricezione lista
//di tutte le buche pubblicate


toSend[0]=1;
toSend[1]='\0'; //non ci interessa cosa c'e' nel resto del messaggio, perche' tanto per questo tipo di richiesta al server bastera' 1 nel primo byte di flag

if(write(connSockFd,toSend,strlen(toSend))<0){
	perror("\n");
	exit(1); 
}
//e ritorna al main chiamante



}


void aggiungi(void){
char *toSend=(char *)calloc(28,sizeof(char));
strcpy(toSend,"                            ");
toSend[0]=2;

char *riferimento=(char *)calloc(20,sizeof(char));
printf("\nInserisci riferimento\n"); fflush(stdout);
fgets(riferimento,20,stdin);
riferimento[strlen(riferimento)-1]=' '; //togliamo il \n

char *x=(char *)calloc(20,sizeof(char));
printf("\nInserisci distanza x da riferimento (min 000-max 999g)\n");
fgets(x,20,stdin);
x[strlen(x)-1]=' ';

char *y=(char *)calloc(20,sizeof(char));
printf("\nInserisci distanza y da riferimento (min 000-max 999)\n");
fgets(y,20,stdin);
printf("%s",y); fflush(stdout);
y[strlen(y)-1]=' ';


printf("\nSicuro di voler inviare segnalazione?(y/qualunque):%s%s%s\n",riferimento,x,y);
fflush(stdout);
char ris;
scanf("%c",&ris);
while(getchar()!='\n');
if(ris=='y'){
	
	
	
	strcpy(toSend+1,riferimento);
	toSend[strlen(toSend)]=' ';
	toSend[21]='\0';
	strcat(toSend,x);
	toSend[24]='\0';
	strcat(toSend,y);
	toSend[27]='\0';
	
	if(write(connSockFd,toSend,strlen(toSend))<0){
		perror("\n");
		exit(1); 	
	}
	printf("\nSEGNALAZIONE INVIATA CON SUCCESSO!\n");
	fflush(stdout);

}

else {
	printf("\nOPERAZIONE ANNULLATA!\n"); 
	fflush(stdout);
}
}



void elimina(void){

char toSend[22]="                      ";

char *riferimento=(char *)calloc(20,sizeof(char));
printf("\nInserisci riferimento richiesta da cancellare\n"); fflush(stdout);
fgets(riferimento,20,stdin);
riferimento[strlen(riferimento)-1]=' '; //togliamo il \n

toSend[0]=4;
strcpy(toSend+1,riferimento);
toSend[strlen(toSend)]=' ';
toSend[21]='\0';

if(write(connSockFd,toSend,strlen(toSend))<0){
		perror("\n");
		exit(1); 	
	}
printf("\nRICHIESTA INVIATA CON SUCCESSO\n");
fflush(stdout);



}
