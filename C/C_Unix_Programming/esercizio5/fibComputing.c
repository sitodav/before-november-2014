#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


//questo processo calcola fib dell'ordine delle prime n cifre dove n e' passato come input
//da cmd line, e il risultato del calcolo e' passato come exit status
int main(int argc,char *argv[],char *envp[]){
int n,i,actual,minus1,minus2;

if(argc!=2)
	exit(0);

n=atoi(argv[1]);

minus1=1; 
minus2=1;
//fib e' calcolato per i primi 2 ordini

if(n<=1)
	exit(n);
if(n==2)
	exit(1);

for(i=3;i<=n;i++){
	actual=minus1+minus2;
	minus2=minus1;
	minus1=actual;
}

exit(actual);



}
