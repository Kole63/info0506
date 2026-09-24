#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

static int est_fichier_pid(const char *nom) {
    size_t longueur = strlen(nom);

    if (longueur <= 7 || strncmp(nom, "pid", 3) != 0 ||
        strcmp(nom + longueur - 4, ".txt") != 0) {
        return 0;
    }

    for (size_t i = 3; i < longueur - 4; i++) {
        if (nom[i] < '0' || nom[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    DIR *dir = opendir(".");
    struct stat buf;
    struct dirent *ent;
    (void) argc;
    (void) argv;

    if (dir == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    int nombres_par_heure[24] = {0};

    while ((ent = readdir(dir)) != NULL) {
        stat(ent->d_name, &buf);
        if (est_fichier_pid(ent->d_name)) {
            FILE *fichier = fopen(ent->d_name, "r");
            long long timestamp;

            if (fichier == NULL) {
                perror(ent->d_name);
                continue;
            }

            if (fscanf(fichier, "%lld", &timestamp) == 1) {
                time_t instant = (time_t) timestamp;
                struct tm *date = localtime(&instant);

                if (date != NULL) {
                    nombres_par_heure[date->tm_hour]++;
                }
            }
            fclose(fichier);
        }
        
    }
    closedir(dir);

    for (int heure = 0; heure < 24; heure++) {
        printf("%02d:00 : %d processus\n", heure, nombres_par_heure[heure]);
    }

    return 0;
}