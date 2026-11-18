#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../include/process.h"

pid_t create_child_process(void)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    return pid;
}

int execute_command(const char *command)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0)
    {
        printf("[Child] PID: %d\n", getpid());
        printf("[Child] Executing command: %s\n", command);

        execl("/bin/sh", "sh", "-c", command, (char *)NULL);

        perror("execl");
        exit(EXIT_FAILURE);
    }

    printf("[Parent] PID: %d\n", getpid());
    printf("[Parent] Created child PID: %d\n", pid);

    wait_for_process(pid);

    return 0;
}

void wait_for_process(pid_t pid)
{
    int status;

    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid");
        return;
    }

    if (WIFEXITED(status))
    {
        printf("[Parent] Child %d terminated normally.\n", pid);
        printf("[Parent] Exit status: %d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status))
    {
        printf("[Parent] Child %d terminated by signal %d.\n",
               pid,
               WTERMSIG(status));
    }
}

void process_demo(void)
{
    printf("\n========================================\n");
    printf("          PROCESS MANAGEMENT DEMO\n");
    printf("========================================\n\n");

    pid_t pid = create_child_process();

    if (pid == -1)
    {
        return;
    }

    if (pid == 0)
    {
        printf("[Child]\n");
        printf("PID        : %d\n", getpid());
        printf("Parent PID : %d\n", getppid());
        printf("State      : Running\n");

        sleep(2);

        printf("[Child] Process is terminating.\n");
        exit(0);
    }
    else
    {
        printf("[Parent]\n");
        printf("PID        : %d\n", getpid());
        printf("Child PID  : %d\n", pid);
        printf("State      : Waiting for child\n");

        wait_for_process(pid);

        printf("[Parent] Child process completed.\n");
    }

    printf("\nProcess lifecycle demonstration completed.\n");
}
