/*Si progetti un programma in ambiente Linux che, impiegando la libreria pthread, crei un thread che
estrae N volte (con N passato da riga di comando) un numero casuale compreso tra 0 e 10 e ne calcoli il
fattoriale ponendolo in una variabile condivisa e un thread che stampi a video il fattoriale calcolato N
volte prelevandolo dalla variabile condivisa.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#define CHIUSA 1
#define APERTA 0

//glob
int fatt=0;
int lockReader=CHIUSA;
int lockWriter=APERTA;

void * thread1_start(void * arg);
void * thread2_start(void *arg);


int main(int argc,char *argv[]){
srand((unsigned int)time(NULL));

if(argc!=2){
	printf("\nUSO: n\n");
	exit(1);
}

int n;
sscanf(argv[1],"%d",&n);
pthread_t tid[2];

int i,ret;

ret=pthread_create(tid,NULL,thread1_start,(void *)&n);
if(ret){
	printf("\n%s",strerror(ret));
	exit(1);
}

printf("\nCreato thread con TID %u\n",(unsigned int)tid[0]);
fflush(stdout);

ret=pthread_create(tid+1,NULL,thread2_start,(void *)&n);

if(ret){
	printf("\n%s",strerror(ret));
	exit(1);
}

printf("\nCreato thread con TID %u\n",(unsigned int)tid[1]);
fflush(stdout);

//al main thread non interessa prendere la struttura dati di ritorno di nessun thread 
//ma deve attendere perche' se termina prima che i thread abbiano completato, termina tutto il processo
for(i=0;i<2;i++){
	ret=pthread_join(tid[i],NULL);
	if(ret){
		printf("\n%s\n",strerror(ret));
		exit(1);	
	}
}






}

//i thread non dovranno ritornare nessuna struttura dati visto che lavorano enrtambi con la variabile condivisa fatt. L'unica cosa sulla quale dobbiamo prestare attenzione, e' sincronizzare il thread che stampa a video il fattoriale, in modo tale che stampi in tutto n volte e ogni volta per un fattoriale diverso (se andasse troppo veloce potrebbe stampare n volte lo stesso fattoriale perche' il thread che lo calcola non ha ancora modificato la variabile globale, che condividono)

void * thread1_start(void * arg){
//scrittore
//dobbiamo creare sincronizzazione tra lettore e scrittore
//per implementare la busy wait, invece dela sleep nel lettore, usiamo una busy wait con variabile di lock che puo' essere usata come variabile globale, e quindi puo' essere attivata e disattivata senza l'uso di segnali in quato i 2 thread la condividono

int i,k,num,tot,n;
n=*((int *)arg);
for(i=0;i<=n;i++){
	// volte viene fatto tutto questo
	while(lockWriter==CHIUSA); //busy wait
	//lockReader=CHIUSA; //blocco il lettore
	num=rand()%11; //genero numero
	tot=1;
	if(i!=n) //non abbiamo terminato
		for(k=2;k<=num;k++)
			tot*=k; //calcolo il fattoriale
	else tot=-1; //abbiamo terminato quindi
	printf("\nSono SCRITTORE: STAMPAMI FATT (CHE CALCOLO IO) DI %d\n",num);
	fflush(stdout);
	fatt=tot; //aggiorno la var globale <--- SEZIONE CRITICA
	lockWriter=CHIUSA; //e' una doppia sicurezza, in quanto e' vero che lo scrittore prima di iniziare blocca me, ma se io ricomincio il ciclo abbastanza velocemente dopo aver sbloccato il lettore, io entro, lui entra, lui blocca la mia lock ma io sono gia' entrato e blocco la sua, ma lui gia' e' entrato. 
//In quest omodo evito anche di fare 2 giri di fila senza che il lettor ene abbia fatto almeno 1, visto che questo gli farebbe saltare la lettura di un fattoriale, che si andrebbe a perdere
	lockReader=APERTA;//sblocco lettore

}
//quando lo scrittore ha terminato i suoi n fattoriali, mette fattoriale a -1, e cosi'
//il lettore capisce che ha finito

pthread_exit((void *)NULL);

}

void * thread2_start(void *arg){
//lettore
int i=0;
while(1){
	while(lockReader==CHIUSA); //busy wait
	//lockWriter=CHIUSA; //blocco scrittore
	if(fatt==-1){
		printf("\nSono lettore: numeri terminati\n");
		pthread_exit((void *)NULL);
	}
	i++;
	printf("\nSono thread lettore: il fattoriale %desimo e' %d\n",i,fatt);
	fflush(stdout);
	lockReader=CHIUSA;
	lockWriter=APERTA;
}
	



}


