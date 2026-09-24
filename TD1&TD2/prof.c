#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
   int choix;
   while((choix = menu()) != 4) {
    if(fork() == 0) {
        switch(choix) {
            case 1:
                execlp("ls", "ls", "-al", NULL);
                break;
            case 2:
                execlp("pwd", "pwd", NULL);
                break;
            case 3:
                execlp("ps", "ps", NULL);
                break;
            default:
                fprintf(stderr, "Choix invalide. Veuillez choisir 1, 2 ou 3.\n");
                exit(1);
        }
    }
   }
}