#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
//    Ecrire un programme qui cr´ee s´equentiellement 10 processus fils. Ces processus fils cr´eeront 10 processus fils.
// Chaque processus affichera son Pid et chaque processus p`ere attendra la terminaison de tous ses processus fils
// avant de terminer.
    for (int i = 0; i < 10; i++) {
        if (fork() == 0) {
            printf("Processus fils %d, Pid: %d\n", i + 1, getpid());
            for (int j = 0; j < 10; j++) {
                if (fork() == 0) {
                    printf("Processus petit-fils %d.%d, Pid: %d\n", i + 1, j + 1, getpid());
                    exit(EXIT_SUCCESS);
                }
            }
            for (int j = 0; j < 10; j++) {
                wait(NULL);
            }
            exit(EXIT_SUCCESS);
        }
    }
    for (int i = 0; i < 10; i++) {
        wait(NULL);
    }
    exit(EXIT_SUCCESS);
}