#include <stdio.h>
#include <stdlib.h>
void executer_commande() {
    printf("voulez-vous exécuter une commande ? (o/n) : ");
    char reponse;
    if (scanf(" %c", &reponse) != 1) {
        fprintf(stderr, "Entrée invalide. Veuillez entrer 'o' ou 'n'.\n");
        exit(1);
    }
    if (reponse != 'o' && reponse != 'O') {
        printf("Aucune commande exécutée.\n");
        return;
    }
    printf("Choisissez une commande à exécuter :\n");
    printf("1. ls -al\n");
    printf("2. pwd\n");
    printf("3. ps\n");
    int choix;
    if (scanf("%d", &choix) != 1) {
        fprintf(stderr, "Entrée invalide. Veuillez entrer un nombre.\n");
        exit(1);
    }
    int pid = fork();
    if (pid < 0) {
        perror("Erreur fork");
        exit(1);
    }
    if (pid == 0) {
        // --- PROCESSUS FILS ---
        switch (choix) {
            case 1:
                execlp("ls", "ls", "-al", NULL);
                break;
            case 2:
                execlp("pwd", "pwd", NULL);
                break;
            case 3:
                execlp("ps", "ps", NULL);
                break;
            default:
                fprintf(stderr, "Choix invalide. Veuillez choisir 1, 2 ou 3.\n");
                exit(1);
        }
        // Si on arrive ici, c'est que execlp a échoué
        perror("Erreur exec");
        exit(1);
    } else {
        // --- PROCESSUS PÈRE ---
        wait(NULL);
        printf("Commande terminée.\n");
    }

}
int main() {
    executer_commande();

    return 0;
}