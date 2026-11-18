#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "../include/shared_memory.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <topic> <message>\n", argv[0]);
        printf("Example: %s sports \"India won the match\"\n", argv[0]);
        return 1;
    }

    char *topic = argv[1];
    char *message = argv[2];

    printf("========================================\n");
    printf("             MSGBUS PUBLISHER\n");
    printf("========================================\n\n");

    printf("Publisher PID : %d\n", getpid());
    printf("Topic         : %s\n", topic);
    printf("Message       : %s\n\n", message);

    int fd = shm_open(SHM_NAME, O_RDWR, 0666);

    if (fd == -1)
    {
        perror("shm_open");
        printf("Make sure the broker is running.\n");
        return 1;
    }

    SharedMemory *shm = mmap(
        NULL,
        sizeof(SharedMemory),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
    );

    if (shm == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }

    pthread_mutex_lock(&shm->mutex);

    if (shm->message_count >= MAX_MESSAGES)
    {
        printf("Message buffer is full.\n");

        pthread_mutex_unlock(&shm->mutex);
        munmap(shm, sizeof(SharedMemory));
        close(fd);

        return 1;
    }

    int index = shm->message_count;

    strncpy(
        shm->messages[index].topic,
        topic,
        MAX_TOPIC - 1
    );

    shm->messages[index].topic[MAX_TOPIC - 1] = '\0';

    strncpy(
        shm->messages[index].message,
        message,
        MAX_MESSAGE - 1
    );

    shm->messages[index].message[MAX_MESSAGE - 1] = '\0';

    shm->messages[index].publisher_pid = getpid();
    shm->messages[index].active = 1;

    shm->message_count++;

    printf("Message published successfully.\n");

    pthread_cond_broadcast(&shm->condition);

    pthread_mutex_unlock(&shm->mutex);

    munmap(shm, sizeof(SharedMemory));
    close(fd);

    return 0;
}

