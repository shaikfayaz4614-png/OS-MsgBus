#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "../include/logger.h"

#define LOG_FILE "msgbus.log"

void logger_demo(void)
{
    const char *message =
        "[MsgBus] Message broker event recorded.\n";

    char buffer[256];

    printf("\n========================================\n");
    printf("             FILE I/O DEMO\n");
    printf("========================================\n\n");

    printf("[Logger] Opening file: %s\n", LOG_FILE);

    int fd = open(
        LOG_FILE,
        O_CREAT | O_WRONLY | O_APPEND,
        0644
    );

    if (fd == -1)
    {
        perror("open");
        return;
    }

    printf("[Logger] File descriptor: %d\n", fd);

    ssize_t written = write(
        fd,
        message,
        strlen(message)
    );

    if (written == -1)
    {
        perror("write");
        close(fd);
        return;
    }

    printf("[Logger] %ld bytes written.\n",
           (long)written);

    close(fd);

    printf("[Logger] File closed.\n");

    fd = open(LOG_FILE, O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return;
    }

    ssize_t bytes_read = read(
        fd,
        buffer,
        sizeof(buffer) - 1
    );

    if (bytes_read == -1)
    {
        perror("read");
        close(fd);
        return;
    }

    buffer[bytes_read] = '\0';

    printf("\n[Logger] Data read from file:\n");
    printf("----------------------------------------\n");
    printf("%s", buffer);
    printf("----------------------------------------\n");

    close(fd);

    printf("[Logger] File closed.\n");

    printf("\nFile I/O demonstration completed.\n");
}
