// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define USERNAME_LEN 32
#define MSG_LEN 512

int sockfd;
char username[USERNAME_LEN];

// Thread function to receive messages from server and print them
void *receive_handler(void *arg) {
    char buffer[MSG_LEN];
    while (1) {
        memset(buffer, 0, MSG_LEN);
        int bytes = recv(sockfd, buffer, MSG_LEN, 0);
        if (bytes <= 0) {
            printf("Disconnected from server.\n");
            exit(EXIT_FAILURE);
        }
        printf("%s", buffer);
        fflush(stdout);
    }
    return NULL;
}

int main() {
    struct sockaddr_in serv_addr;
    pthread_t recv_thread;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    // Receive prompt "Enter username: "
    char buffer[MSG_LEN];
    memset(buffer, 0, MSG_LEN);
    recv(sockfd, buffer, MSG_LEN, 0);
    printf("%s", buffer);

    // Get username from stdin
    fgets(username, USERNAME_LEN, stdin);

    // Send username to server
    send(sockfd, username, strlen(username), 0);

    // Receive authentication confirmation
    memset(buffer, 0, MSG_LEN);
    recv(sockfd, buffer, MSG_LEN, 0);
    printf("%s", buffer);

    // Start thread to receive messages from server
    pthread_create(&recv_thread, NULL, receive_handler, NULL);

    // Main loop: prompt user to send messages
    while (1) {
        printf("\nEnter recipient username (or 'exit' to quit): ");
        char recipient[USERNAME_LEN];
        fgets(recipient, USERNAME_LEN, stdin);
        recipient[strcspn(recipient, "\n")] = 0; // remove newline

        if (strcmp(recipient, "exit") == 0) {
            break;
        }

        printf("Enter message: ");
        char message[MSG_LEN];
        fgets(message, MSG_LEN, stdin);
        message[strcspn(message, "\n")] = 0; // remove newline

        // Format message: TO:<recipient>:<message>
        char sendbuf[MSG_LEN + USERNAME_LEN + 10];
        snprintf(sendbuf, sizeof(sendbuf), "TO:%s:%s", recipient, message);

        send(sockfd, sendbuf, strlen(sendbuf), 0);
    }

    printf("Exiting chat.\n");
    close(sockfd);
    return 0;
}
