#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


// 4) Ecrire un programme qui inverse un fichier source dans un fichier destination
//en utilisant lseek
int main(int argc, char *argv[]) {
    char c;
    int d;
    int src = open(argv[1], O_RDONLY);
    int dst = open(argv[2], O_WRONLY|O_CREAT, 0644);
    lseek(src, -1, SEEK_END);
    read(src, &c, 1);
    write(dst, &c, 1);
    while (lseek(src, -2, SEEK_CUR) != 0) {
        read(src, &c, 1);
        write(dst, &c, 1);
    }
    read(src, &c, 1);
    write(dst, &c, 1);
    close(dst);
    close(src);
    return 0;
}