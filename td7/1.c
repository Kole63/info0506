#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

//1) Ecrire un programme permettant de copier un fichier source dans un fichier destination.
int main(int argc, char *argv[]) {
    int src = open(argv[1], O_RDONLY);
    int dst = open(argv[2], O_WRONLY|O_CREAT, 0644);
    int lu;
    char buf[255];
    while ((lu = read(src, buf, 255)) != 0) {
        write(dst, buf, lu);
    }
    close(src);
    close(dst);
    return   0;
}
    
