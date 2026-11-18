#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

#include "../include/ipc.h"

#define FIFO_PATH "/tmp/msgbus_fifo"

void fifo_demo(void)
{
    printf("\n========================================\n");
    printf("             NAMED FIFO DEMO\n");
    printf("========================================\n\n");

    /* Remove old FIFO if it exists */
    unlink(FIFO_PATH);

    /* Create named FIFO */
    if (mkfifo(FIFO_PATH, 0666) == -1)
    {
        perror("mkfifo");
        return;
    }

    printf("[FIFO] Created: %s\n", FIFO_PATH);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        unlink(FIFO_PATH);
        return;
    }

    if (pid == 0)
    {
        /* Child acts as receiver */
        char buffer[256];

        int fd = open(FIFO_PATH, O_RDONLY);

        if (fd == -1)
        {
            perror("open FIFO for reading");
            exit(EXIT_FAILURE);
        }

        ssize_t bytes_read = read(
            fd,
            buffer,
            sizeof(buffer) - 1
        );

        if (bytes_read < 0)
        {
            perror("read");
            close(fd);
            exit(EXIT_FAILURE);
        }

        buffer[bytes_read] = '\0';

        printf("[Child] PID: %d\n", getpid());
        printf("[Child] Message received: %s\n", buffer);

        close(fd);

        exit(EXIT_SUCCESS);
    }
    else
    {
        /* Parent acts as sender */
        sleep(1);

        int fd = open(FIFO_PATH, O_WRONLY);

        if (fd == -1)
        {
            perror("open FIFO for writing");
            waitpid(pid, NULL, 0);
            unlink(FIFO_PATH);
            return;
        }

        const char *message =
            "Hello child! Message sent through named FIFO.";

        printf("[Parent] PID: %d\n", getpid());
        printf("[Parent] Sending message through FIFO...\n");

        if (write(fd, message, strlen(message)) == -1)
        {
            perror("write");
        }
        else
        {
            printf("[Parent] Message sent successfully.\n");
        }

        close(fd);

        waitpid(pid, NULL, 0);

        printf("[Parent] Child process completed.\n");
    }

    /* Remove named FIFO */
    unlink(FIFO_PATH);

    printf("\nNamed FIFO demonstration completed.\n");
}
