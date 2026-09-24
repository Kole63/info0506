#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    int k = 0;
    int i;
    for (i = 0 ; i < 5 ; i++ )
    {
    k = fork();
    printf(" i = %d, k = %d, pid = %d \n",i,k,getpid());
    }
    if (k)
    {
    exit(0);
    }
    // execlp("/bin/ps","ps","-e",NULL);
    printf(" i = %d, k = %d, pid = %d \n",i,k,getpid());
    exit(0);
}
