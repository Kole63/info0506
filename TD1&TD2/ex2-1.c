#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int k = 0;
    for ( int i = 0 ; i < 10; i++ )
    {
    if((k = fork()) == 0)
    {
        printf(" i = %d, k = %d, pid = %d \n",i,k,getpid()); 
        exit(EXIT_SUCCESS);
    }
    wait(NULL);
    }
    exit(EXIT_SUCCESS);
}