#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define   UINT_MAX 10000000

int main()
{

int i,j;
for (i=0; i < UINT_MAX; i++)
  for (j=0; j < UINT_MAX; j++)
  {
    putchar('.');
    fflush(stdout);
  }




}
