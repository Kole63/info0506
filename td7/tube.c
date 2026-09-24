#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int T[2];
    int i, b;

    if (pipe(T) == -1) {
        perror("Erreur pipe");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("Erreur fork");
        exit(1);
    }

    if (pid == 0) {
        // --- PROCESSUS FILS (Lecteur) ---
        close(T[1]); // Ferme l'écriture inutile
        
        printf("[Fils] En attente de données...\n");
        
        // On lit et on vérifie si la lecture a réussi
        while (read(T[0], &b, sizeof(int)) > 0) {
            printf("[Fils] Reçu : %d\n", b);
            if (b < 0) break; // Arrêt si nombre négatif
        }

        close(T[0]);
        printf("[Fils] Terminé.\n");
        exit(0);
    } else {
        // --- PROCESSUS PÈRE (Écrivain) ---
        close(T[0]); // Ferme la lecture inutile

        printf("[Père] Entrez des entiers (négatif pour quitter) :\n");
        
        do {
            printf("> ");
            if (scanf("%d", &i) != 1) break; // Sécurité si saisie non-numérique
            
            write(T[1], &i, sizeof(int)); // Envoie le nombre (même le négatif)
        } while (i >= 0);

        close(T[1]); // Signale la fin (EOF) au fils
        wait(NULL);  // Attend la fin du fils
        printf("[Père] Fils terminé, je m'arrête.\n");
    }

    return 0;
}