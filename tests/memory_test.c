#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main()
{
    printf("========================================\n");
    printf("       VIRTUAL MEMORY DEMO\n");
    printf("========================================\n\n");

    long page_size = sysconf(_SC_PAGESIZE);

    printf("System page size: %ld bytes\n", page_size);

    /*
     * malloc
     */
    int *data = malloc(5 * sizeof(int));

    if (data == NULL)
    {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < 5; i++)
        data[i] = i + 1;

    printf("\nmalloc address: %p\n", (void *)data);

    /*
     * calloc
     */
    int *zeros = calloc(5, sizeof(int));

    if (zeros == NULL)
    {
        perror("calloc");
        free(data);
        return 1;
    }

    printf("calloc address: %p\n", (void *)zeros);

    /*
     * realloc
     */
    data = realloc(data, 10 * sizeof(int));

    if (data == NULL)
    {
        perror("realloc");
        free(zeros);
        return 1;
    }

    printf("realloc address: %p\n", (void *)data);

    /*
     * mmap anonymous memory
     */
    void *mapped = mmap(
        NULL,
        page_size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );

    if (mapped == MAP_FAILED)
    {
        perror("mmap");
        free(data);
        free(zeros);
        return 1;
    }

    strcpy((char *)mapped, "Memory mapped successfully.");

    printf("mmap address:   %p\n", mapped);
    printf("mmap contents:  %s\n", (char *)mapped);

    /*
     * fork + copy-on-write demonstration
     */
    printf("\nCreating child process with fork()...\n");

    int shared_value = 100;

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        munmap(mapped, page_size);
        free(data);
        free(zeros);
        return 1;
    }

    if (pid == 0)
    {
        printf("[Child] PID: %d\n", getpid());
        printf("[Child] Address of shared_value: %p\n",
               (void *)&shared_value);

        printf("[Child] Before modification: %d\n",
               shared_value);

        shared_value = 200;

        printf("[Child] After modification: %d\n",
               shared_value);

        _exit(0);
    }
    else
    {
        waitpid(pid, NULL, 0);

        printf("[Parent] PID: %d\n", getpid());
        printf("[Parent] shared_value: %d\n",
               shared_value);

        printf("[Parent] Child modification did not change "
               "parent memory.\n");
        printf("[Parent] This demonstrates copy-on-write behavior.\n");
    }

    munmap(mapped, page_size);
    free(data);
    free(zeros);

    printf("\nMemory resources released.\n");
    printf("Virtual memory demonstration completed.\n");

    return 0;
}
