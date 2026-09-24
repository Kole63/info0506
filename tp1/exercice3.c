#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

static void enregistrer_date(void) {
    char nom_fichier[64];
    char buffer[32];
    time_t rawtime = time(NULL);

    snprintf(nom_fichier, sizeof(nom_fichier), "pid%d.txt", getpid());
    snprintf(buffer, sizeof(buffer), "%lld", (long long) rawtime);

    int dst = open(nom_fichier, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    write(dst, buffer, strlen(buffer));
    close(dst);
}

static void creer_processus_fils(int n, int s) {
    if (n <= 0 || s <= 0) {
        return;
    }

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("Processus fils avec pid : %d\n", getpid());
            enregistrer_date();
            creer_processus_fils(n - 1, s - 1);
            exit(EXIT_SUCCESS);
        }   
        sleep(s);
    }

    for (int i = 0; i < n - 1; i++) {
        wait(NULL);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s <nombre_de_processus> <delai_en_secondes>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n;
    int s;
    char *endptr1, *endptr2;
    long val1 = strtol(argv[1], &endptr1, 10);
    long val2 = strtol(argv[2], &endptr2, 10);
    n = val1;
    s = val2;

    if (n <= 0 || s <= 0) {
        fprintf(stderr, "Les deux paramètres doivent être strictement positifs.\n");
        return EXIT_FAILURE;
    }

    creer_processus_fils(n, s);

    return 0;
}