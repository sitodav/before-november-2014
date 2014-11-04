/*m thread leggon ola loro colonna e aggiornano cont occorrenze, e inseriscono sul buffer condiviso trovati, indice dell'occorrenza sulla matrice. Un m+1esimo thread estrae tutti gli elementi inseriti sul buffer trovati, e li stampa (togliendoli dal buffer)
FACCIAMO CHE OGNI VOLTA CHE PARTE IL PROCESSO ESTRATTORE, ESTRAE TUTTI QUELLI CHE PUO' FINO A SVUOTARE TUTTO IL BUFFER, QUINDI AL GIRO SUCCESSIVO IL PROSSIMO INSERITORE DOVRA' INSERIRE DI NUOVO IN POSIZIONE 0


quindi scomponendo il problema di sincronizzazione degli m+1 thread, in sottoproblemi di sincronizzazione, ne analizziamo i tipi:

-c'e' un problema di race condition tra gli m thread, per l'accesso alla variabile contatore totOccorrenze 
-C'E' UN PROBLEMA DI SEGNALAZIONE PERCHE' APPENA IL BUFFER NON E' VUOTO (IND ULTIMO!=-1) DEVE ESSERE AVVIATO L'ESTRATTORE, E QUINDI C'E' UN PROBLEMA DI RACE CONDITION SU INDICE ULTIMO INSERITO TRA GLI M PROCESSI INSERENTI PERCHE' DEVONOS APERE DOVE INSERIRE, E TRA QUESTI E L'ESTRATTORE PERCHE' QUESTO PER LUI E' LA CONDIZIONE DEL PREDICATO DELLA VARIABILE DI CONDIZIONE USATA PER IMPLEMENTARE LA SEGNALAZIONE (ED INOLTRE L'ESTRATTORE RIAZZERA A -1 IND ULTIMO POICHE' SVUOTA TUTTO IL BUFFER) 

QUINDI UTILIZZIAMO UN MUTEX CHE COPRE CONTTOTOCCORRENZE E CHE COPRE INDULTIMO ED E' ANCHE IL MUTEX ASSOCIATO ALLA VARIABILE DI CONDIZIONE CHE HA COME VAR DEL PREDICATO PROPRIO IND ULTIMO */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

struct found{
	int riga;
	int colonna;

};


//var globali che necessitano di sincronizzazione all'accesso
int totOcc=0;
int indUltimo=-1;
struct found *trovati; //lo allocheremo dinamicamente per mxn, funziona FIFO e sara' allocato lungo al max per evitare di dover usare modulo per buffer circolare

//strutture di sincronizzazione
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER; //mutex su totOcc usato dagli m-esimi thread e su indUltimoInserito usato da tutti, anche per la valutazione da parte di m+1 del predicato

pthread_cond_t cond1; //var di condizione, che usa mutex1 come mutex associato

//var globali che non necessitano di sincronizzazione
int **matrix; //la allocheremo su misura passata da cmd, non richiede sincronizzazione poiche' ogni thread legge solo la sua colonna
int key;
int n; //ogni thread deve sapere quanto e' "alta" la colonna, e' inutile passarlo come argomento, la mettiamo globale

void * thread_cercatore_start(void * arg); //ai cercatori passiamo come argomento solo puntatore ad una variabile che indica l'indice su cui deve lavorare 
void * thread_estrattore_start(void * arg);


int main(int argc,char *argv[]){
if(argc!=4){
	printf("\nuso: n m key\n");
	exit(1);
}

srand((unsigned int)time(NULL));

int m,i,j;
sscanf(argv[1],"%d",&n); sscanf(argv[2],"%d",&m); sscanf(argv[3],"%d",&key);
printf("\ndevo cercare %d\n",key);
fflush(stdout);
matrix=(int **)calloc(n,sizeof(int *));
trovati=(struct found *)calloc(n*m,sizeof(struct found)); //allochiamo il buffer trovati

for(i=0;i<n;i++){ //allocazione, riempimento e stampa della matrice
	matrix[i]=(int *)calloc(m,sizeof(int));
	for(j=0;j<m;j++){
		matrix[i][j]=rand()%3;
		printf("%d ",matrix[i][j]);
	}
	printf("\n");	
}

pthread_t *tid=(pthread_t *)calloc(m,sizeof(pthread_t));
int *ind=(int *)calloc(m,sizeof(int));
//creo m thread (1 per colonna) e gli passo come arg l'indirizzo iesimo elemento dell'array indici, ovvero il thread nella sua funzione ricevera' puntatore all'elemento che contiene il valore dell'indice della colonna della matrice sul quale deve lavorare
int ret;
for(i=0;i<m;i++){
	ind[i]=i;
	if((ret=pthread_create(tid+i,NULL,thread_cercatore_start,(void *)(ind+i)))!=0){
		printf("\n%s\n",strerror(ret));
		exit(1);
	}
}
//creo anche il thread estrattore, a lui non passo niente come argomento
pthread_t estrattore;
if((ret=pthread_create(&estrattore,NULL,thread_estrattore_start,NULL))!=0){
	printf("\n%s",strerror(ret));
	exit(1);
}

//aspetto che terminino i soli thread cercatori  
for(i=0;i<m;i++)
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("%s",strerror(ret));	
		exit(1);	
	}
//stampo tot var occorrenze sulla quale adesso non c'e' piu' rischio di race condition perche' gli m cercatori son terminati e l'estrattore non la usa
printf("\nTotale occorrenze: %d\n",totOcc); fflush(stdout);
sleep(5); //aspetto un po', casomai deve fare un ultima svuotata e stampata di buffer l'estrattore, e poi il main thread termina, terminando il processo e anceh l'estrattore che era rimasto vivo
exit(0);

}


void * thread_cercatore_start(void * arg){
int i,myCol=*((int *)arg);

for(i=0;i<n;i++){
	if(matrix[i][myCol]==key){ //trovata occ
		pthread_mutex_lock(&mutex1);
		//INIZIO SEZIONE CRITICA
		totOcc++;
		indUltimo++;
		trovati[indUltimo].riga=i;
		trovati[indUltimo].colonna=myCol;
		//FINE SEZIONE CRITICA
			        
		//if(indUltimo!=-1)
		pthread_cond_signal(&cond1);
		pthread_mutex_unlock(&mutex1);
	}
//oGNI VOLTA CHE TROVO OCCORRENZA, INCREMENTO TOTOCC (SEMPRE SOTTO MUTUA ESCLUSIONE), POI ACCEDENDO IN MUTUA ESCLUSIONE ALLA VARIABILE INDULTIMO, LA INCREMENTO E INSERISCO LI', E SICCOME QUANDO INSERISCO ALLORA SICURAMENTE CI SARA' UN ELEMENTO, NON HO BISOGNO DI NESSUN PREDICATO DA VALUTARE PER LANCIARE LA SIGNAL SULLA VAR DI CONDIZIONE

}



}

void * thread_estrattore_start(void * arg){
//il thread estrattore non termina mai (verra' terminato a fine main thread quindi a fine processo)

while(1) {
	
	fflush(stdout);
	pthread_mutex_lock(&mutex1);
	//SEZIONE CRITICA SULLA VALUTAZIONE E MODIFICA VARIABILE NEL PREDICATO DELLA 		VARIABILE DI CONDIZIONE, PROTETTA USANDO IL MUTEX ASSOCIATO ALLA VAR DI CONDIZIONE 
	while(indUltimo<0)
		pthread_cond_wait(&cond1,&mutex1);
	
	//quando il codice arriva qua, indUltimo sara >=0, e saremo protetti sotto il mutex1 		che risultera' acquisito e bloccato da noi (ribloccato automaticamente dalla wait se 		c'eravamo addormentati su questa)
	//ora dobbiamo estrarre e stampare gli elementi da trovati, finche' non lo svuotiamo
	while(indUltimo>=0){
		//lo stampo
		printf("occ trovata in riga %d   colonna %d\n",trovati[indUltimo].riga,trovati[indUltimo].colonna);
		fflush(stdout);
		//lo svuoto
		trovati[indUltimo].riga=0;
		trovati[indUltimo].colonna=0;
		//e decremento indUltimo
		indUltimo--;
	}

	//arrivati qui vuol dire che abbiamo svuotato tutto il buffer trovati, indUltimo e' 		tornato a -1, e ritorniamo all'inizio del while generale, quindi ci riaddormentiamo

	//FINE SEZIONE CRITICA
	pthread_mutex_unlock(&mutex1);

}

}



