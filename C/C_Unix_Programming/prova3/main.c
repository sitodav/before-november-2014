#include <signal.h>
#include <unistd.h>
#include <stdio.h>

void handler (int sig);

int main(void)
{
int i;

signal(SIGINT,handler);

for(;;)
{
printf("\nProva a fermarmi\n");
fflush(stdout);
for(i=0;i<5;i++)
{
sleep(1);
printf(".");
fflush(stdout);
}
printf("\n");
}
}

void handler (int sig)
{
printf("\nNon voglio terminare\n");
}
