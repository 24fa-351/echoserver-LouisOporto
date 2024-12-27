#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_LEN 1024
// Usage: ./server <port>

// Function to broadcast client messages (return to client)
void broadcastMessage(char* message, int clientSocket) {
    send(clientSocket, message, strlen(message), 0);
}

// Functions to receive client packets
void* handleClient(void* socket) {
    int clientSocket = *(int*)socket;
    char message[MAX_LEN];
    int readBytes;

    // Receive data from the client
    while ((readBytes = recv(clientSocket, message, MAX_LEN - 1, 0)) > 0) {
        message[readBytes] = '\0';
        printf("Client: %s\n", message);

        // Broadcast message back to user
        broadcastMessage(message, clientSocket);
    }

    // Close client socket
    close(clientSocket);
    pthread_exit(NULL);
}

// Function to start the server
void startServer(int port) {
    int serverSocket;
    int clientSocket;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    socklen_t addressSize;

    // Initiate listening port
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0) {
        perror("Error creating listening port");
        exit(1);
    }

    // Configure server socket
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind server socket to server address
    if (bind(serverSocket, (struct sockaddr*)&serverAddress,
             sizeof(serverAddress)) < 0) {
        perror("Error binding server address to server socket");
        exit(1);
    }

    // Limit the number of client to 1
    if (listen(serverSocket, 1) < 0) {
        perror("Error listening for incoming connections");
        exit(1);
    }

    printf("Server started on port: %d", port);

    while (1) {
        addressSize = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress,
                              &addressSize);

        if (clientSocket < 0) {
            perror("Error accepting incoming connection");
            exit(1);
        } else {
            printf("Client connected from address: %d\n",
                   clientAddress.sin_addr.s_addr);
        }

        // Create a new thread to handle client messages
        pthread_t receiveThread;
        pthread_create(&receiveThread, NULL, handleClient,
                       (void*)&clientSocket);
    }
}

// Main driver function
int main(int argc, char* argv[]) {
    int listenPort = 0;

    if (argc < 2 || sscanf(argv[1], "%d", &listenPort) == 0) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    printf("Port: %d\n", listenPort);

    printf("Starting server...\n");
    startServer(listenPort);

    return 0;
}