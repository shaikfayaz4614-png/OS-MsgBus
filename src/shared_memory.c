#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "../include/shared_memory.h"

int create_shared_memory()
{
    int fd;

    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

    if (fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, sizeof(SharedMemory)) == -1)
    {
        perror("ftruncate");
        close(fd);
        exit(EXIT_FAILURE);
    }

    return fd;
}

SharedMemory *map_shared_memory(int fd)
{
    SharedMemory *shm;

    shm = mmap(
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
        exit(EXIT_FAILURE);
    }

    return shm;
}

void initialize_shared_memory(SharedMemory *shm)
{
    memset(shm, 0, sizeof(SharedMemory));

    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;

    pthread_mutexattr_init(&mutex_attr);
    pthread_condattr_init(&cond_attr);

    /*
     * Allows different processes to use
     * the same mutex.
     */
    pthread_mutexattr_setpshared(
        &mutex_attr,
        PTHREAD_PROCESS_SHARED
    );

    /*
     * Allows different processes to use
     * the same condition variable.
     */
    pthread_condattr_setpshared(
        &cond_attr,
        PTHREAD_PROCESS_SHARED
    );

    pthread_mutex_init(
        &shm->mutex,
        &mutex_attr
    );

    pthread_cond_init(
        &shm->condition,
        &cond_attr
    );

    pthread_mutexattr_destroy(&mutex_attr);
    pthread_condattr_destroy(&cond_attr);

    shm->message_count = 0;
}

