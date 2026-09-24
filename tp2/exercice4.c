#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

typedef struct {
    int x;
    int h;
} Couple;

static volatile sig_atomic_t arret_demande = 0;

static void traiter_signal(int signal_recu) {
    (void) signal_recu;
    arret_demande = 1;
}

static int lancer_piece(void) {
    int faces = 0;
    for (int i = 0; i < 10; i++) {
        if (rand() % 2 == 0) {
            faces++;
        }
    }
    return faces;
}

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

    struct sigaction action = {0};
    action.sa_handler = traiter_signal;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGUSR1, &action, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    sigset_t masque_signal;
    sigemptyset(&masque_signal);
    sigaddset(&masque_signal, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &masque_signal, NULL) == -1) {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    int tubes[n][2];
    pid_t fils[n];
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
            int valeurs[n];
            int actif = 1;
            int tour = 1;

            if (sigprocmask(SIG_UNBLOCK, &masque_signal, NULL) == -1) {
                perror("sigprocmask");
                exit(EXIT_FAILURE);
            }

            srand((unsigned int) (time(NULL) ^ getpid()));

            for (int j = 0; j < n; j++) {
                if (j != precedent) {
                    close(tubes[j][0]);
                }
                if (j != i) {
                    close(tubes[j][1]);
                }
            }

            while (1) {
                couple.x = actif ? lancer_piece() : -1;
                couple.h = 1;
                if (actif) {
                    printf("Fils %d (tour %d) propose X=%d\n", i + 1, tour, couple.x);
                    fflush(stdout);
                }

                if (write(tubes[i][1], &couple, sizeof(couple)) != sizeof(couple)) {
                    if (arret_demande) {
                        break;
                    }
                    perror("write");
                    exit(EXIT_FAILURE);
                }

                for (int j = 0; j < n; j++) {
                    ssize_t lus = read(tubes[precedent][0], &recu, sizeof(recu));
                    if (lus != sizeof(recu)) {
                        if (arret_demande) {
                            break;
                        }
                        perror("read");
                        exit(EXIT_FAILURE);
                    }
                    valeurs[j] = recu.x;
                    if (recu.h < n) {
                        recu.h++;
                        if (write(tubes[i][1], &recu, sizeof(recu)) != sizeof(recu)) {
                            if (arret_demande) {
                                break;
                            }
                            perror("write");
                            exit(EXIT_FAILURE);
                        }
                    }
                }

                if (arret_demande) {
                    break;
                }

                int maximum = -1;
                int nombre_maximum = 0;
                for (int j = 0; j < n; j++) {
                    if (valeurs[j] > maximum) {
                        maximum = valeurs[j];
                        nombre_maximum = 1;
                    } else if (valeurs[j] == maximum) {
                        nombre_maximum++;
                    }
                }

                if (actif && couple.x == maximum && nombre_maximum == 1) {
                    printf("Fils %d est le vainqueur avec X=%d\n", i + 1, couple.x);
                    fflush(stdout);
                    if (kill(getppid(), SIGUSR1) == -1) {
                        perror("kill");
                    }
                    break;
                }
                if (nombre_maximum == 1) {
                    break;
                }
                if (actif && couple.x != maximum) {
                    actif = 0;
                    printf("Fils %d devient passif\n", i + 1);
                    fflush(stdout);
                }
                tour++;
            }

            if (close(tubes[i][1]) == -1 || close(tubes[precedent][0]) == -1) {
                perror("close");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
        fils[i] = pid;
    }

    if (sigprocmask(SIG_UNBLOCK, &masque_signal, NULL) == -1) {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++) {
        close(tubes[i][0]);
        close(tubes[i][1]);
    }
    if (arret_demande) {
        for (int i = 0; i < n; i++) {
            kill(fils[i], SIGUSR1);
        }
        arret_demande = 0;
    }
    for (int i = 0; i < n; i++) {
        while (wait(NULL) == -1) {
            if (errno != EINTR) {
                perror("wait");
                return EXIT_FAILURE;
            }
            if (arret_demande) {
                for (int j = 0; j < n; j++) {
                    kill(fils[j], SIGUSR1);
                }
                arret_demande = 0;
            }
        }
    }
    return EXIT_SUCCESS;
}