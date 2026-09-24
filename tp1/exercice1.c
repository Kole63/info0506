#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

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
    int k = 0;
    for(int i = 0; i < n; i++) {
        if ((k = fork()) == 0) {
            printf("Processus %d avec pid : %d\n",i,getpid());
            exit(EXIT_SUCCESS);
        }
        sleep(s);
    }


    return 0;
}