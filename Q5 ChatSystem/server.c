// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 4
#define USERNAME_LEN 32
#define MSG_LEN 512

// Client structure to hold client info
typedef struct {
    int socket;
    char username[USERNAME_LEN];
    int active;
} Client;

Client clients[MAX_CLIENTS];                    // Array to hold clients
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex for thread safety

// Function declarations
void *handle_client(void *arg);
void broadcast_online_users();
void send_message_to(char *sender, char *receiver, char *message);

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    pthread_t tid;

    // Create TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Prepare server address struct
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;   // Accept connections from any IP
    address.sin_port = htons(PORT);

    // Bind socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started on port %d\n", PORT);

    // Main accept loop
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        // Find an available client slot
        pthread_mutex_lock(&clients_mutex);
        int i;
        for (i = 0; i < MAX_CLIENTS; ++i) {
            if (!clients[i].active) {
                clients[i].socket = new_socket;
                clients[i].active = 1;
                pthread_create(&tid, NULL, &handle_client, &clients[i]);
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);

        // If server full
        if (i == MAX_CLIENTS) {
            char *msg = "Server full\n";
            send(new_socket, msg, strlen(msg), 0);
            close(new_socket);
        }
    }

    return 0;
}

// Thread function to handle client communication
void *handle_client(void *arg) {
    Client *cli = (Client *)arg;
    char buffer[MSG_LEN];
    char username[USERNAME_LEN];

    // Step 1: Authenticate user by username
    send(cli->socket, "Enter username: ", 16, 0);
    int len = recv(cli->socket, username, USERNAME_LEN, 0);
    if(len <= 0) {
        close(cli->socket);
        cli->active = 0;
        return NULL;
    }
    username[len - 1] = '\0';  // Remove newline character
    strcpy(cli->username, username);

    printf("%s connected.\n", cli->username);
    send(cli->socket, "Authenticated.\n", 15, 0);

    // Broadcast updated online users list to all clients
    broadcast_online_users();

    // Step 2: Handle incoming messages from client
    while (1) {
        memset(buffer, 0, MSG_LEN);
        int bytes = recv(cli->socket, buffer, MSG_LEN, 0);
        if (bytes <= 0) {
            break;  // Client disconnected
        }

        // Expected message format: TO:<receiver>:<message>
        if (strncmp(buffer, "TO:", 3) == 0) {
            char *receiver = strtok(buffer + 3, ":");
            char *msg = strtok(NULL, "\n");
            if (receiver && msg) {
                send_message_to(cli->username, receiver, msg);
            }
        }
    }

    // Step 3: Client disconnect cleanup
    close(cli->socket);
    pthread_mutex_lock(&clients_mutex);
    cli->active = 0;
    pthread_mutex_unlock(&clients_mutex);

    // Broadcast updated online users list
    broadcast_online_users();

    printf("%s disconnected.\n", cli->username);
    return NULL;
}

// Sends the list of online users to all active clients
void broadcast_online_users() {
    char list[MSG_LEN] = "Online Users:\n";
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            strcat(list, clients[i].username);
            strcat(list, "\n");
        }
    }
    // Send updated list to all clients
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            send(clients[i].socket, list, strlen(list), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Sends a message from sender to receiver if receiver is online
void send_message_to(char *sender, char *receiver, char *message) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && strcmp(clients[i].username, receiver) == 0) {
            char buffer[MSG_LEN];
            snprintf(buffer, sizeof(buffer), "From %s: %s\n", sender, message);
            send(clients[i].socket, buffer, strlen(buffer), 0);
            pthread_mutex_unlock(&clients_mutex);
            return;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}
