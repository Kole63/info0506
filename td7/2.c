#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
/**2 ) Ecrire un programme qui crypte un fichier texte source dans un fichier destination, selon l’algorithme de
C´esar (d´ecalage des lettres de x cases, x ´etant pass´e en param`etre au programme) */
int main(int argc, char *argv[]) {
    int src = open(argv[1], O_RDONLY);
    int dst = open(argv[2], O_WRONLY|O_CREAT, 0644);

    int dec = atoi(argv[3]);
    char buf;
    while (read(src, &buf, 1) == 1) {
        buf += dec;
        write(dst, &buf, 1);
    }
    close(src);
    close(dst);
}
