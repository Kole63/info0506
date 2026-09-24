#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
            int suivant = (i + 1) % n;
            char message[128];
            int longueur;

            for (int j = 0; j < n; j++) {
                if (j != precedent) {
                    close(tubes[j][0]);
                }
                if (j != i) {
                    close(tubes[j][1]);
                }
            }

            longueur = snprintf(message, sizeof(message),"Processus %d (pid=%ld) -> processus %d", i + 1, (long) getpid(), suivant + 1);
            if (write(tubes[i][1], message, (size_t) longueur) != longueur) {
                printf("Erreur d'ecriture\n");
                _exit(EXIT_FAILURE);
            }

            longueur = (int) read(tubes[precedent][0], message, sizeof(message) - 1);
            if (longueur == -1) {
                printf("Erreur de lecture\n");
                _exit(EXIT_FAILURE);
            }
            message[longueur] = '\0';
            printf("Fils %d a recu : %s\n", i + 1, message);
            fflush(stdout);
            close(tubes[i][1]);
            close(tubes[precedent][0]);
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