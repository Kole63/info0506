//Ecrivons le code de la partie cliente UDP en C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#define PORT 3000
//Prof proposition de code pour le client UDP


int main() {

    int b;
    //defintion du processus local serveur
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in s, c;
    socklen_t cl = sizeof(c);
    memset(&s, 0, sizeof(struct sockaddr_in));
    s.sin_family = AF_INET;
    s.sin_port = htons(PORT);
    s.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr*)&s, sizeof(s));
    while(1) {
        int lu = 0;
        lu = recvfrom(sock, &b, sizeof(int), 0, (struct sockaddr*)&c, &cl);
        b = time(NULL);
        sendto(sock, &b, sizeof(int), 0, (struct sockaddr*)&c, cl);
        printf("Client %s:%d, time: %d\n", inet_ntoa(c.sin_addr), ntohs(c.sin_port), b);
    }

    close(sock);
    return 0;
}



























// ma prop
// int main() {
//     int sockclient;
//     int sockserver;
//     struct sockaddr_in serverAddr;
//     char buffer[1024];

//     // Création du socket UDP
//     sockclient = socket(AF_INET, SOCK_DGRAM, 0);

//     memset(&serverAddr, 0, sizeof(serverAddr));
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(8080);
//     serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

//     sendto(sockclient, "Hello, Server!", strlen("Hello, Server!"), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
//     ;
//     recvfrom(sockclient, buffer, sizeof(buffer), 0, NULL, NULL);
//     printf("Message from server: %s\n", buffer);


//     //serveur socket
//     sockserver = socket(AF_INET, SOCK_DGRAM, 0);
//     bind(sockserver, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
//     recvfrom(sockserver, buffer, sizeof(buffer), 0, NULL, NULL);
//     printf("Message from client: %s\n", buffer);
//     sendto(sockserver, "Hello, Client!", strlen("Hello, Client!"), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

//     close(sockclient);
//     close(sockserver);

    
//     return 0;
// }