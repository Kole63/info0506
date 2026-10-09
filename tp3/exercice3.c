#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <time.h>

bool is_packet_empty(int *shared_counter, int n) {
    for (int i = 0; i < n; i++) {
        if (shared_counter[i] > 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Nombre de paquets>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);

    if (n <= 0) {
        fprintf(stderr, "Le nombre de paquets doit etre positif\n");
        exit(EXIT_FAILURE);
    }

    int semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0600);
    semctl(semid, 0, SETVAL, 1);
    semctl(semid, 1, SETVAL, 0);

    int shmid = shmget(IPC_PRIVATE, n * sizeof(int),IPC_CREAT | 0600);
    int *shared_counter = (int *)shmat(shmid, NULL, 0);

    int v = 1;
    for (int i = 0; i < n; i++) {
        shared_counter[i] = v;
        v += 2;
    }

    for (int j = 0; j < 2; j++) {
        if (fork() == 0) {

            srand(time(NULL) ^ getpid());

            struct sembuf A;
            A.sem_flg = 0;

            while (1) {

                // Attendre son tour
                A.sem_num = j;
                A.sem_op = -1;
                semop(semid, &A, 1);

                // Vérifier après avoir obtenu son tour
                if (is_packet_empty(shared_counter, n)) {
                    A.sem_num = 1 - j;
                    A.sem_op = +1;
                    semop(semid, &A, 1);
                    break;
                }

                int random_index = rand() % n;

                while (shared_counter[random_index] <= 0) {
                    random_index = rand() % n;
                }

                int number_in_packet = shared_counter[random_index];

                int number_to_take = (rand() % number_in_packet) + 1;

                shared_counter[random_index] -= number_to_take;

                printf("Fils %d a pris %d allumettes du paquet %d, il en reste %d\n",j + 1,number_to_take,random_index + 1,shared_counter[random_index]);

                if (is_packet_empty(shared_counter, n)) {
                    printf("Fils %d a perdu la partie\n", j + 1);
                }

                fflush(stdout);

                // Donner le tour à l'autre fils
                A.sem_num = 1 - j;
                A.sem_op = +1;
                semop(semid, &A, 1);
            }

            shmdt(shared_counter);
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