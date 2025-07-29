#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_QUEUE_SIZE 210
#define MAX_ITEMS 10  // Total items to produce and consume and also limit the run.

int queue[MAX_QUEUE_SIZE];
int count = 0;
int front = 0;
int rear = 0;
int items_produced = 0;
int items_consumed = 0;

FILE* logfile;

// Mutex and condition variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void log_message(const char* message) {
    printf("%s", message);          // Print to console
    fprintf(logfile, "%s", message); // Log to file
    fflush(logfile);                 // Ensure it writes immediately
}

void enqueue(int item) {
    queue[rear] = item;
    rear = (rear + 1) % MAX_QUEUE_SIZE;
    count++;
}

int dequeue() {
    int item = queue[front];
    front = (front + 1) % MAX_QUEUE_SIZE;
    count--;
    return item;
}

void* producer(void* arg) {
    int item = 1;
    while (1) {
        sleep(2); // Simulate production time

        pthread_mutex_lock(&mutex);

        // Stop after producing MAX_ITEMS
        if (items_produced >= MAX_ITEMS) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        while (count == MAX_QUEUE_SIZE) {
            log_message("Queue is full. Producer is waiting...\n");
            pthread_cond_wait(&not_full, &mutex);
        }

        enqueue(item);
        items_produced++;

        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Produced item %d | Queue size: %d\n", item, count);
        log_message(buffer);

        item++;

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* consumer(void* arg) {
    while (1) {
        sleep(3); // Simulate consumption time

        pthread_mutex_lock(&mutex);

        // Stop after consuming MAX_ITEMS
        if (items_consumed >= MAX_ITEMS) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        while (count == 0) {
            log_message("Queue is empty. Consumer is waiting...\n");
            pthread_cond_wait(&not_empty, &mutex);
        }

        int item = dequeue();
        items_consumed++;

        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Consumed item %d | Queue size: %d\n", item, count);
        log_message(buffer);

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread;

    logfile = fopen("simulation_log.txt", "w");
    if (!logfile) {
        perror("Failed to open log file");
        return 1;
    }

    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    log_message("Simulation complete.\n");
    fclose(logfile);

    return 0;
}
