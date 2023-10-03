#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    const int PORT = 4444;

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Assuming the server is running on the same machine

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int status = connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    if(status == -1) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    char clientName[] = "Client of John Doe";
    int clientNumber;

    printf("Please enter a number between 1 and 100: ");
    scanf("%d", &clientNumber);
    if (clientNumber < 1 || clientNumber > 100) {
        printf("Invalid number. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];

    snprintf(buffer, sizeof(buffer), "Client of %s, %d", clientName, clientNumber);

    ssize_t bytesSent = send(sockfd, buffer, strlen(buffer), 0);

    if(bytesSent == -1) {
        perror("send failed");
        exit(EXIT_FAILURE);
    }

    ssize_t bytesReceived = recv(sockfd, buffer, sizeof(buffer), 0);

    if(bytesReceived == -1) {
        perror("recv failed");
        exit(EXIT_FAILURE);
    }

    buffer[bytesReceived] = '\0'; // Null-terminate the received string

    char serverName[50];
    int serverNumber;

    if(sscanf(buffer, "Server of %[^,], %d", serverName, &serverNumber) != 2) {
        perror("sscanf failed");
        exit(EXIT_FAILURE);
    }

    printf("Received from Server: Server's name is %s and number is %d\n", serverName, serverNumber);

    close(sockfd);
    return 0;
}
