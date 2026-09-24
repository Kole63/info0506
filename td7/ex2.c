#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
/** 1) Ecrire un programme qui affiche la taille d’un fichier pass´e en param`etre.
2) Ecrire un programme qui affiche les droits d’un fichier pass´e en param`etre */

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDONLY);
    struct stat buf;
    stat(argv[1], &buf);
    printf("Taille du fichier : %ld\n", buf.st_size);
    printf("Droits du fichier : %o\n", buf.st_mode);
    close(fd);
    return 0;

}