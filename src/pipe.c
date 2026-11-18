#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "../include/ipc.h"

void pipe_demo(void)
{
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return;
    }

    printf("\n========================================\n");
    printf("           ANONYMOUS PIPE DEMO\n");
    printf("========================================\n\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        /* Child reads from the pipe */
        close(fd[1]);

        char buffer[256];

        ssize_t bytes_read = read(
            fd[0],
            buffer,
            sizeof(buffer) - 1
        );

        if (bytes_read < 0)
        {
            perror("read");
            close(fd[0]);
            exit(EXIT_FAILURE);
        }

        buffer[bytes_read] = '\0';

        printf("[Child] PID: %d\n", getpid());
        printf("[Child] Message received: %s\n", buffer);

        close(fd[0]);
        exit(EXIT_SUCCESS);
    }
    else
    {
        /* Parent writes to the pipe */
        close(fd[0]);

        const char *message =
            "Hello child! Message sent through anonymous pipe.";

        printf("[Parent] PID: %d\n", getpid());
        printf("[Parent] Sending message...\n");

        if (write(fd[1], message, strlen(message)) == -1)
        {
            perror("write");
        }
        else
        {
            printf("[Parent] Message sent successfully.\n");
        }

        close(fd[1]);

        waitpid(pid, NULL, 0);

        printf("[Parent] Child process completed.\n");
    }

    printf("\nAnonymous pipe demonstration completed.\n");
}
