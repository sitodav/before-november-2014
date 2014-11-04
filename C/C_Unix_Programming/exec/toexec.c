#include <stdio.h>
#include <stdlib.h>


int main(int argc,char *argv[]){

printf("\nSono il programma %s, e mi e' stato passato come parametro :",argv[0]);
int i=0;
while(++i<argc)
	printf("arg n%d: %s\n",i,argv[i]);




return 0;
}
