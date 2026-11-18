#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#include "../include/shared_memory.h"

int main()
{
    int fd;
    SharedMemory *shm;

    printf("====================================\n");
    printf("       MSGBUS SHARED MEMORY TEST\n");
    printf("====================================\n");

    printf("Creating shared memory...\n");

    fd = create_shared_memory();

    printf("Shared memory created successfully.\n");

    printf("Mapping shared memory...\n");

    shm = map_shared_memory(fd);

    printf("Shared memory mapped successfully.\n");

    initialize_shared_memory(shm);

    printf("Shared memory initialized successfully.\n");

    printf("Shared memory name : %s\n", SHM_NAME);
    printf("Memory size        : %lu bytes\n",
           sizeof(SharedMemory));

    printf("Process PID        : %d\n", getpid());

    munmap(shm, sizeof(SharedMemory));
    close(fd);

    printf("\nIPC test completed successfully.\n");

    return 0;
}
