#include <stdio.h>
#include <stdlib.h>
// 3 ) Corriger ce code pour qu’il cr´ee 5 processus.

int main(int argc, char* argv[])
{
    int k = 0;
    for ( int i = 0 ; i < 5 ; i++ )
    {
    if((k = fork()) == 0)
    {
        // printf(" i = %d, k = %d, pid = %d \n",i,k,getpid()); 
        exit(EXIT_SUCCESS);
    }
    printf(" i = %d, k = %d, pid = %d \n",i,k,getpid());
    }
    exit(EXIT_SUCCESS);
}
