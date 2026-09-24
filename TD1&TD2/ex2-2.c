#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
     //2 ) Ecrire un programme qui cr´ee s´equentiellement 10 processus fils. Ces processus fils afficheront leur Pid.
        for (int i = 0; i < 10; i++) {
            if (fork() == 0) {
                printf("Processus fils %d, Pid: %d\n", i + 1, getpid());
                exit(EXIT_SUCCESS);
            }
        }
    exit(EXIT_SUCCESS);
}