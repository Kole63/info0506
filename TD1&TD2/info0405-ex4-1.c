#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main() {
    int *T;

    for(int i = 0; i < 5; i++) {
        if(fork() == 0) {
            T = (int*)malloc(10 * sizeof(int));
            if(T == NULL) {
                perror("Erreur malloc");
                exit(EXIT_FAILURE);
            }

            printf("PID %d : adresse = %p\n", getpid(), (void*)T);

            free(T);
            exit(EXIT_SUCCESS);
        }
    }

    // Le père attend les 5 fils
    for(int i = 0; i < 5; i++) {
        wait(NULL);
    }

    return 0;
}