#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "../include/signals.h"

static volatile sig_atomic_t signal_received = 0;

static void handle_signal(int signal_number)
{
    signal_received = signal_number;

    if (signal_number == SIGUSR1)
    {
        printf("[Child] Received SIGUSR1\n");
    }
    else if (signal_number == SIGUSR2)
    {
        printf("[Child] Received SIGUSR2\n");
    }
    else if (signal_number == SIGINT)
    {
        printf("[Child] Received SIGINT\n");
    }
}

static int install_handler(int signal_number)
{
    struct sigaction action;

    action.sa_handler = handle_signal;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    return sigaction(signal_number, &action, NULL);
}

void signal_demo(void)
{
    printf("\n========================================\n");
    printf("             SIGNAL DEMO\n");
    printf("========================================\n\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        /* Child process */

        if (install_handler(SIGUSR1) == -1 ||
            install_handler(SIGUSR2) == -1 ||
            install_handler(SIGINT) == -1)
        {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }

        printf("[Child] PID: %d\n", getpid());
        printf("[Child] Signal handlers installed.\n");
        printf("[Child] Waiting for signals...\n");

        /*
         * Wait for signals from the parent.
         */
        for (int i = 0; i < 3; i++)
        {
            signal_received = 0;

            while (signal_received == 0)
            {
                pause();
            }
        }

        printf("[Child] All signals received.\n");
        printf("[Child] Exiting.\n");

        exit(EXIT_SUCCESS);
    }
    else
    {
        /* Parent process */

        printf("[Parent] PID: %d\n", getpid());
        printf("[Parent] Child PID: %d\n", pid);

        sleep(1);

        printf("\n[Parent] Sending SIGUSR1...\n");

        if (kill(pid, SIGUSR1) == -1)
        {
            perror("kill");
        }

        sleep(1);

        printf("[Parent] Sending SIGUSR2...\n");

        if (kill(pid, SIGUSR2) == -1)
        {
            perror("kill");
        }

        sleep(1);

        printf("[Parent] Sending SIGINT...\n");

        if (kill(pid, SIGINT) == -1)
        {
            perror("kill");
        }

        waitpid(pid, NULL, 0);

        printf("\n[Parent] Child process completed.\n");
    }

    printf("\nSignal demonstration completed.\n");
}
