#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    int x;
    int h;
} Couple;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s <nombre_de_processus>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *endptr;
    long valeur = strtol(argv[1], &endptr, 10);
    if (*argv[1] == '\0' || *endptr != '\0' || valeur < 2 || valeur > INT_MAX) {
        fprintf(stderr, "Le nombre de processus doit etre un entier superieur ou egal a 2.\n");
        return EXIT_FAILURE;
    }
    int n = (int) valeur;

    int tubes[n][2];
    for (int i = 0; i < n; i++) {
        if (pipe(tubes[i]) == -1) {
            perror("pipe");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return EXIT_FAILURE;
        }
        if (pid == 0) {
            int precedent = (i + n - 1) % n;
            Couple couple;
            Couple recu;

            srand((unsigned int) (time(NULL) ^ getpid()));

            for (int j = 0; j < n; j++) {
                if (j != precedent) {
                    close(tubes[j][0]);
                }
                if (j != i) {
                    close(tubes[j][1]);
                }
            }

            couple.x = 0;
            for (int j = 0; j < 10; j++) {
                if (rand() % 2 == 0) {
                    couple.x++;
                }
            }
            couple.h = 1;
            // printf("Fils %d : %d Face(s) sur 10 lancers\n", i + 1, couple.x);
            fflush(stdout);

            if (write(tubes[i][1], &couple, sizeof(couple)) != sizeof(couple)) {
                perror("write");
                _exit(EXIT_FAILURE);
            }

            while (1) {
                if (read(tubes[precedent][0], &recu, sizeof(recu)) != sizeof(recu)) {
                    perror("read");
                    _exit(EXIT_FAILURE);
                }
                printf("Fils %d recoit (X=%d, H=%d)\n", i + 1, recu.x, recu.h);
                fflush(stdout);

                if (recu.h == n) {
                    break;
                }

                recu.h++;
                if (write(tubes[i][1], &recu, sizeof(recu)) != sizeof(recu)) {
                    perror("write");
                    _exit(EXIT_FAILURE);
                }
            }

            if (close(tubes[i][1]) == -1 || close(tubes[precedent][0]) == -1) {
                perror("close");
                _exit(EXIT_FAILURE);
            }
            _exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < n; i++) {
        close(tubes[i][0]);
        close(tubes[i][1]);
    }
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }
    return EXIT_SUCCESS;
}