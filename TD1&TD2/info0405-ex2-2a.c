#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void executer_random() {

    int random_number = rand() % 3;

    int pid = fork();

    if (pid < 0) {
        perror("Erreur fork");
        exit(1);
    }

    if (pid == 0) {
        // --- FILS ---
        printf("random = %d\n", random_number);

        switch (random_number) {
            case 0:
                execlp("pwd", "pwd", NULL);   
                break;
            case 1:
                execlp("ls", "ls", "-al", NULL);
                break;
            case 2:
                execlp("ps", "ps", NULL);
                break;
            default:
                fprintf(stderr, "Erreur random\n");
        }

        // Si exec échoue
        perror("Erreur exec");
        exit(1);
    } 
    else {
        // --- PÈRE ---
        wait(NULL);
        printf("Commande aléatoire terminée.\n");
    }
}

int main() {
    srand(time(NULL));
    executer_random();
    return 0;
}