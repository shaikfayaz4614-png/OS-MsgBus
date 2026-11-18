#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "../include/shared_memory.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <topic>\n", argv[0]);
        printf("Example: %s sports\n", argv[0]);
        return 1;
    }

    char *topic = argv[1];

    printf("========================================\n");
    printf("            MSGBUS SUBSCRIBER\n");
    printf("========================================\n\n");

    printf("Subscriber PID : %d\n", getpid());
    printf("Topic          : %s\n\n", topic);

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

    printf("Searching for messages...\n\n");

    int found = 0;

    for (int i = 0; i < shm->message_count; i++)
    {
        if (shm->messages[i].active &&
            strcmp(shm->messages[i].topic, topic) == 0)
        {
            printf("----------------------------------------\n");
            printf("Message #%d\n", i + 1);
            printf("Topic        : %s\n",
                   shm->messages[i].topic);
            printf("Message      : %s\n",
                   shm->messages[i].message);
            printf("Publisher PID: %d\n",
                   shm->messages[i].publisher_pid);
            printf("----------------------------------------\n");

            found = 1;
        }
    }

    if (!found)
    {
        printf("No messages found for topic: %s\n", topic);
    }

    pthread_mutex_unlock(&shm->mutex);

    munmap(shm, sizeof(SharedMemory));
    close(fd);

    printf("\nSubscriber completed.\n");

    return 0;
}
