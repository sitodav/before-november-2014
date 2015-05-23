#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    char path[FILENAME_MAX];
    getcwd(path,sizeof(path));
    printf("\npath:%s",path);
}
