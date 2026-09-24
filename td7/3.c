#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// 3) Ecrire un programme permettant de cr´eer un fichier `a trous.

int main(int argc, char *argv[]) {
    char c = 'T';
    int fd = open(argv[1], O_WRONLY|O_CREAT, 0644);
    write(fd, &c, 1);
    lseek(fd,200,SEEK_CUR);
    write(fd, &c, 1);
    lseek(fd,200,SEEK_CUR);
    close(fd);
    return 0;
}