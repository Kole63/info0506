#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Nombre d'allumetes>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *ent;
    int n = strtol(argv[1], &ent, 10);

    //Semaphore pour synchroniser les fils
    int semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0600);

    semctl(semid, 0, SETVAL, 1);
    semctl(semid, 1, SETVAL, 0);
    
    int k;

    for (int j = 0; j < 2; j++) {
        if ((k = fork()) == 0) {

            struct sembuf A;
            A.sem_flg = 0;

            for (int i = 0; i < 5; i++) {

                // Attendre son tour
                A.sem_num = j;
                A.sem_op = -1;
                semop(semid, &A, 1);

                printf("Fils %d : C'est mon tour\n", j + 1);
                fflush(stdout);

                // Donner le tour à l'autre fils
                A.sem_num = 1 - j;
                A.sem_op = +1;
                semop(semid, &A, 1);
            }

            exit(0);
        }
    }

    wait(NULL);
    wait(NULL);

    semctl(semid, 0, IPC_RMID);

    return 0;
}