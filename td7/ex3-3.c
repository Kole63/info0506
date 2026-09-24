#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>


/** 3) Ecrire un programme qui liste les fichiers que contient le repertoire courant avec leur taille et l’UID de son
propri´etaire.
*/
int main(int argc, char *argv[]) {
    DIR *dir = opendir(".");
    struct stat buf;
    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
        stat(ent->d_name, &buf);
        printf("Nom %s Taille %ld Uid %d\n", ent->d_name, buf.st_size, buf.st_uid);
    }
    closedir(dir);
    return 0;
}