//Ecrivons le code de la partie serveur TCP en C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#define PORT 3000


int main() {

    int socket asock, lsock;
    socklen_t l = sizeof(struct sockaddr_in);
    lsock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in s, c;
    memset(&s, 0, sizeof(struct sockaddr_in));
    memset(&c, 0, sizeof(struct sockaddr_in));
    s.sin_family = AF_INET;
    s.sin_port = htons(PORT); //htons = host to network short
    s.sin_addr.s_addr = INADDR_ANY;
    bind(lsock, (struct sockaddr*)&s, sizeof(s));
    listen(lsock, 5);

    while(1) {
        asock = accept(lsock, (struct sockaddr*)&c, &l);
        /**
            traiter(Requete);
        */
        close(asock);
    }
    close(lsock);

    return 0;
}