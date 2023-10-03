# Basic Client-Server Interaction in C

This repository contains basic implementations of a client and a server interacting over a network using socket programming in C. The client sends its name and a number to the server, the server then receives and processes the data, and sends its own name and a number back to the client.

## Client Code

- The client connects to the server using the server's IP address and port number.
- It sends a string containing its name and a number (between 1 and 100) to the server.
- After sending, it waits for a message from the server, extracts the server's name and number from the received message, and prints them.
- Finally, it closes the socket and exits.

## Server Code

- The server binds to a specific IP address and port number and listens for incoming connections.
- Upon receiving a connection request, it accepts it, forks a new process to handle the client, and goes back to listening for more connections.
- The child process receives data from the connected client, extracts the client's name and number, and prints them along with the server's name and a randomly generated number.
- The child process sends the server's name and number back to the client, then closes the client socket and exits.
- The parent process continues to listen for more connections.

## Prerequisites

- Basic knowledge of C programming
- Familiarity with socket programming in C

## Compilation and Usage

### Compilation

To compile the client and server code, use the gcc compiler as follows:

```shell
gcc -o client client_code.c
gcc -o server server_code.c

./server
./client
