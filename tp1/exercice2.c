#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {

    time_t rawtime;
    char buffer[255];


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
            char buff[255];
            char nom_fichier[255] = "pid";
            char chaine[255];
            sprintf(chaine, "%d", getpid());
            strcat(strcpy(buff, nom_fichier), chaine);
            strcat(buff, ".txt");
            int dst = open(buff, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (dst == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            rawtime = time(NULL);
            snprintf(buffer, sizeof(buffer), "%lld", (long long) rawtime);
            write(dst, buffer, strlen(buffer));
            close(dst);
            exit(EXIT_SUCCESS);
        }
        sleep(s);
    }


    return 0;
}