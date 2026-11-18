#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <pthread.h>

#define SHM_NAME "/msgbus_shared"
#define MAX_MESSAGES 10
#define MAX_TOPIC 64
#define MAX_MESSAGE 256

typedef struct {
    char topic[MAX_TOPIC];
    char message[MAX_MESSAGE];
    int publisher_pid;
    int active;
} Message;

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t condition;

    int message_count;

    Message messages[MAX_MESSAGES];

} SharedMemory;

int create_shared_memory();
SharedMemory *map_shared_memory(int fd);
void initialize_shared_memory(SharedMemory *shm);

#endif
