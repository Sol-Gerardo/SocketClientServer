#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include<time.h>

// Holds address family and socket functions
#include <sys/socket.h>

// Allows the use of close() function 
// to close the sockets 
#include <netinet/in.h>

int main() {
    const int PORT = 4444;

    struct sockaddr_in *serverAddr = malloc(sizeof *serverAddr);

    // Test if memory allocation failed.
    if(serverAddr == NULL) {
        // Sends the message to error stream.
        fprintf(stderr, "Memory allocation failed for clientAddress");
        exit(EXIT_FAILURE);
    }

    // Set the address family to AF_INET (IPv4)
    serverAddr->sin_family = AF_INET;

    // htons(...) converts integer value to binary form.
    serverAddr->sin_port = htons(PORT);

    // Set the IP address to INADDR_ANY (listen on all interfaces)
    serverAddr->sin_addr.s_addr = htonl(INADDR_ANY);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // Socket creation unsuccessful.
    if(sockfd == -1) {
        // More descriptive error message.
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int status = bind(sockfd, (struct sockaddr*) serverAddr, sizeof(struct sockaddr));
    // Unable to bind socket to IP address.
    if(status == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

   // We will accept up to 4 connections.
   status = listen(sockfd, 4);
   // Unable to listen to incoming connections. 
   if(status == -1) {
        perror("listening failed.");
        exit(EXIT_FAILURE);
   } 

   while(1) {   
        // Create socket for client address that we will be listening for.
        struct sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);
        int clientSocket = accept(sockfd, (struct sockaddr*)&clientAddr, &clientSize);
        if(clientSocket == -1) {
            perror("accepted failed.");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if(pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        if(pid == 0) {
            close(sockfd);

            // To receive data from the client
            char buffer[1024]; // Buffer to store received data 
            ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if(bytesReceived == -1) {
                perror("recv failed.");
                exit(EXIT_FAILURE);
            } 

            // Extract client's name and number
            char clientName[50];
            int clientNumber;
            if(sscanf(buffer, "Client of %[^,], %d", clientName, &clientNumber) != 2) {
                perror("sscanf failed.");
                close(clientSocket);
                close(sockfd);
                free(serverAddr);
                exit(EXIT_FAILURE);
            } 

            if(clientNumber < 1 || clientNumber > 100) {
                perror("Invalid client number.");
                close(clientSocket);
                close(sockfd);
                free(serverAddr);
                exit(EXIT_FAILURE);
            }

            // Print server's and client's information
            char serverName[] = "Server of Gerardo Solis.";
            srand(time(NULL));
            int serverNumber = (rand() % 100) + 1; // Random number between 1 and 100.

            printf("Server's name: %s\n", serverName);
            printf("Server's name: %d\n", serverNumber);
            printf("Client's name: %s\n", clientName);
            printf("Client's number: %d\n", clientNumber);
            int sum = serverNumber + clientNumber;
            printf("Sum: %d\n", sum);

            // Send back server's name and number
            // To send data back to the client 
            snprintf(buffer, sizeof(buffer), "%s, %d", serverName, serverNumber);
            ssize_t bytesSent = send(clientSocket, buffer, bytesReceived, 0);
            if(bytesSent == -1) {
                perror("send failed.");
                exit(EXIT_FAILURE); 
            }
            close(clientSocket);
            exit(0);
        }
        else {
            close(clientSocket);
        }
   }

   return 0;
}