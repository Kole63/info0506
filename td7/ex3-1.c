#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

//1) Ecrire un programme qui liste les fichiers que contient le repertoire courant

int main(int argc, char *argv[]) {
    DIR *dir = opendir(".");
    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
        printf("%s\n", ent->d_name);
    }
    closedir(dir);
    return 0;
}
