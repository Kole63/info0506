//Ecrivons le code de la partie client TCP en C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#define PORT 3000

int main() {

    int ssock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in s;
    memset(&s, 0, sizeof(struct sockaddr_in));
    s.sin_family = AF_INET;
    s.sin_port = htons(PORT);
    s.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(ssock, (struct sockaddr*)&s, sizeof(s));

    /**
        traiter(Requete);
    */

    close(ssock);
    
    return 0;
}