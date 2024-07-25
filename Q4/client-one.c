#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

void *receive_messages(void *socket_desc) {
    int sock = *(int *)socket_desc;
    char buffer[BUFFER_SIZE];
    int read_size;

    while ((read_size = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[read_size] = '\0';
        printf("Received: %s\n", buffer);
    }

    if (read_size == 0) {
        printf("Server disconnected\n");
    } else if (read_size == -1) {
        perror("recv failed");
    }

    pthread_exit(NULL);
}

int main() {
    int sock;
    struct sockaddr_in server;
    char message[BUFFER_SIZE];
    pthread_t thread_id;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket");
        return 1;
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect failed");
        return 1;
    }

    printf("Connected to server (Client One)\n");

    // Create a thread to receive messages
    if (pthread_create(&thread_id, NULL, receive_messages, (void *)&sock) < 0) {
        perror("Could not create thread");
        return 1;
    }

    // Send messages
    while (1) {
        printf("Client One: ");
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0';

        char full_message[BUFFER_SIZE + 20];
        snprintf(full_message, sizeof(full_message), "Client One: %s", message);

        if (send(sock, full_message, strlen(full_message), 0) < 0) {
            perror("Send failed");
            return 1;
        }
    }

    close(sock);
    return 0;
}
