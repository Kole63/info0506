#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/shm.h>

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

    //Memoire partagee 
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    int *shared_counter = (int *)shmat(shmid, NULL, 0);
    *shared_counter = n;


    int k;

    for (int j = 0; j < 2; j++) {
        if ((k = fork()) == 0) {

            struct sembuf A;
            A.sem_flg = 0;

            while (*shared_counter > 0) {

                // Attendre son tour
                A.sem_num = j;
                A.sem_op = -1;
                semop(semid, &A, 1);

                //generer un random entre 1 et 3
                int random = (rand() % 3) + 1;

                //Prendre une à trois allumettes
                if (*shared_counter >= random) {
                    *shared_counter -= random;
                    printf("Fils %d a pris %d allumettes, il en reste %d\n", j + 1, random, *shared_counter);
                    if (*shared_counter == 0) {
                        printf("Fils %d a perdu la partie\n", j + 1);
                    }
                } else if (*shared_counter > 0 && *shared_counter < random) {
                    printf("Fils %d est le perdant de la partie, il en reste 0 allumettes\n", j + 1);
                    *shared_counter = 0;
                }
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
    shmdt(shared_counter);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}