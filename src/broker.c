#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>

#include "../include/shared_memory.h"

SharedMemory *shm;

void cleanup(int sig)
{
    printf("\n\n[Broker] Shutting down...\n");

    if (shm != NULL)
    {
        munmap(shm, sizeof(SharedMemory));
    }

    exit(0);
}

int main()
{
    int fd;

    printf("========================================\n");
    printf("        MSGBUS MESSAGE BROKER\n");
    printf("========================================\n\n");

    printf("[Broker] Starting...\n");

    signal(SIGINT, cleanup);

    fd = create_shared_memory();

    printf("[Broker] Shared memory created.\n");

    shm = map_shared_memory(fd);

    printf("[Broker] Shared memory mapped.\n");

    initialize_shared_memory(shm);

    printf("[Broker] Shared memory initialized.\n");

    printf("\n----------------------------------------\n");
    printf("Broker PID       : %d\n", getpid());
    printf("Shared Memory    : %s\n", SHM_NAME);
    printf("Memory Size      : %lu bytes\n",
           sizeof(SharedMemory));
    printf("Status           : RUNNING\n");
    printf("----------------------------------------\n");

    printf("\n[Broker] Waiting for publishers/subscribers...\n");
    printf("[Broker] Press Ctrl+C to stop.\n\n");

    while (1)
    {
        sleep(1);
    }

    return 0;
}
