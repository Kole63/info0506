#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

//4) Ecrire un programme qui liste r´ecursivement le contenu d’un r´epertoire.


int main(int argc, char *argv[]) {
    DIR *dir = opendir(argv[1]);
    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
        printf("%s\n", ent->d_name);
    }
    closedir(dir);
    return 0;
}
