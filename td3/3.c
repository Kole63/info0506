//Ecrivons le code de la partie serveur UDP en C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#define PORT 3000
//Prof proposition de code pour le serveur UDP


int main() {

    int b;
    //defintion du processus local serveur
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in s;
    memset(&s, 0, sizeof(struct sockaddr_in));
    s.sin_family = AF_INET;
    s.sin_port = htons(PORT);
    inet_aton("127.0.0.1", &s.sin_addr);

    sendto(sock, &b, sizeof(int), 0, (struct sockaddr*)&s, sizeof(s));
    recvfrom(sock, &b, sizeof(int), 0, (struct sockaddr*)&s, sizeof(s));
    printf("Time from client: %d\n", b);

    close(sock);
    return 0;
}
