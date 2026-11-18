#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

pid_t create_child_process(void);
int execute_command(const char *command);
void wait_for_process(pid_t pid);
void process_demo(void);

#endif
