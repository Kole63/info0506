#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

// 3 ) Ecrire un programme qui transfert un fichier source via un tube a un processus fils qui l’affichera.


// ma proposition
// int main(int argc, char *argv[]) {

//     int T[2];
    
//     pid_t pid = fork();

//     if (pid == 0) {
//         char buffer[256];
//         ssize_t n;

//         close(T[1]);

//         while ((n = read(T[0], buffer, sizeof(buffer) - 1)) > 0) {
//             buffer[n] = '\0';
//             printf("%s", buffer);
//         }
//         printf("\n");

//         close(T[0]);
//         exit(0);
//     }

//     close(T[0]);

//     int src = open(argv[1], O_RDONLY);

//     char buffer[256];
//     ssize_t n;
//     while ((n = read(src, buffer, sizeof(buffer))) > 0) {
//         write(T[1], buffer, n);
//     }

//     close(src);
//     close(T[1]);
//     wait(NULL);

//     return 0;
// }

// Prof proposition

int main(int argc, char *argv[]) {
    char buf[255];
    int p[2];
    int lu;
    int s = open(argv[1],O_RDONLY);
    pipe(&p[0]);

    if(fork() == 0) {
        close(p[1]);

        read(p[0], &lu, sizeof(lu));
        while (lu != 0) {
            read(p[0], buf, lu);
            buf[lu] = '\0';
            printf("%s", buf);
            read(p[0], &lu, sizeof(lu));
        }

        close(p[0]);
        
        exit(0);
    }

    close(p[0]);

    while ((lu = read(s, buf, 255)) > 0) {
        write(p[1], &lu, sizeof(lu));
        write(p[1], buf, lu);
    }
    lu = 0;
    write(p[1], &lu, sizeof(lu));
    
    wait(NULL);
    close(p[1]);
    close(s);

    exit (0);
}