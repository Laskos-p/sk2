#include <iostream>
#include <cstring>
#include <thread>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 1100
#define SERVER "127.0.0.1"

int main() {
    int SocketFD;
    struct sockaddr_in sa;
    char buff[1024];


    SocketFD = socket(PF_INET, SOCK_STREAM, 0);
    if (SocketFD == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORT);
    sa.sin_addr.s_addr = inet_addr(SERVER);

    if (connect(SocketFD, (struct sockaddr *) &sa, sizeof sa) == -1) {
        perror("Error");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    int recvError;
    while (true) {
        // system("clear");
        // receive and print message
        memset(buff, 0, sizeof buff);
        recvError = recv(SocketFD, buff, sizeof buff, 0);
        if (recvError == -1) {
            perror("Error");
            close(SocketFD);
            exit(EXIT_FAILURE);
        } else if (recvError == 0) {
            std::cout << "Server disconnected\n";
            close(SocketFD);
            exit(EXIT_SUCCESS);
        }

        std::cout << buff;
        if (strcmp(buff, "Disconnecting...\n") == 0) {
            close(SocketFD);
            exit(EXIT_SUCCESS);
        }

        // receive request for message
        memset(buff, 0, sizeof buff);
        recvError = recv(SocketFD, buff, sizeof buff, 0);
        if (recvError == -1) {
            perror("Error");
            close(SocketFD);
            exit(EXIT_FAILURE);
        } else if (recvError == 0) {
            std::cout << "Server disconnected\n";
            close(SocketFD);
            exit(EXIT_SUCCESS);
        }
        if (strcmp(buff, "SEND SOMETHING") != 0) continue;
        // std::cout << "> ";

        // send message
        memset(buff, 0, sizeof buff);
        fgets(buff, sizeof buff, stdin);
        if (send(SocketFD, buff, sizeof buff, 0) == -1) {
            perror("Error sending message");
            close(SocketFD);
            exit(EXIT_FAILURE);
        }
    }
    close(SocketFD);
}