#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>



// 4 ) ´ Ecrire un programme qui s’arrˆete apr`es trois r´eceptions du signal SIGINT. Le programme devra afficher
// r´eguli`erement un message d’attente jusqu’`a sa terminaison en indiquant le nombre de signaux SIGINT re¸cus


static int count = 0;
int handler(int S) {
    Signal(S, handler);
    count++;
    printf("SIG INT Reçu\n");
}

int main(int argc, char *argv[]) {

    Signal(SIGINT, handler);

    printf("Processus d : %d\n",getpid());

    while(count < 3) +
        printf("Attente Sig INT %d Reçu\n",count);
        sleep(4);
    }

    return 0;

}