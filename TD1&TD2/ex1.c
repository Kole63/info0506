#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int k = 0;
    for ( int i = 0 ; i < 5 ; i++ )
    {
    k = fork();
    printf(" i = %d, k = %d, pid = %d \n",i,k,getpid());
    }
    exit(EXIT_SUCCESS);
}
