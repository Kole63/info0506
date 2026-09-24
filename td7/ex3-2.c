#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

//2) Ecrire un programme qui affiche le contenu des fichiers contenant l’extension .txt dans le repertoire courant.

int main(int argc, char *argv[]) {
    DIR *dir = opendir(".");
    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
        if (strstr(ent->d_name, ".txt") != NULL) {
            printf("Contenu du fichier %s\n", ent->d_name);
            int fd = open(ent->d_name, O_RDONLY);
            char buf[255];
            int lu;
            while ((lu = read(fd, buf, 255)) != 0) {
                write(1, buf, lu);
            }
            printf("\n");
            close(fd);
        }
    }
    closedir(dir);
    return 0;
}