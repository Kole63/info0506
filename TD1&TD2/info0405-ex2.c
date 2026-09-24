#include <stdio.h>
#include <stdlib.h>


int main() {
    int pid;
    // 1. On crée un nouveau processus
    pid = fork();

    if (pid < 0) {
        // Erreur lors du fork
        perror("Erreur fork");
        exit(1);
    }

    if (pid == 0) {
        // --- PROCESSUS FILS ---
        // On remplace ce processus par la commande "ls"
        // execlp cherche "ls" dans le PATH.
        // Arguments :
        // 1. "ls" (nom du fichier exécutable)
        // 2. "ls" (nom du programme passé en argv[0], convention)
        // 3. "-al" (les arguments)
        // 4. NULL (indique la fin des arguments)
        execlp("ls", "ls", "-al", NULL);

        // Si on arrive ici, c'est que execlp a échoué (ex: commande introuvable)
        perror("Erreur exec");
        exit(1);
    } else {
        // --- PROCESSUS PÈRE ---
        // Le père attend que le fils (ls) ait fini son travail
        wait(NULL);
        printf("Commande terminée.\n");
    }

    return 0;
}