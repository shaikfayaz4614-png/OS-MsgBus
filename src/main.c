#include <stdio.h>
#include <stdlib.h>

#include "../include/synchronization.h"
#include "../include/logger.h"

static void show_menu()
{
    printf("\n========================================\n");
    printf("          MSGBUS OS PROJECT\n");
    printf("========================================\n");
    printf("1. Synchronization Demo\n");
    printf("2. File I/O Demo\n");
    printf("3. Memory Demo\n");
    printf("4. Race Condition Test\n");
    printf("5. Deadlock Test\n");
    printf("6. Exit\n");
    printf("========================================\n");
}

int main()
{
    int choice;

    printf("\n========================================\n");
    printf("       MSGBUS MESSAGE BUS SYSTEM\n");
    printf("========================================\n");
    printf("Operating Systems Project\n");
    printf("Ubuntu Linux\n");

    while (1)
    {
        show_menu();

        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input.\n");
            return 1;
        }

        switch (choice)
        {
            case 1:
                synchronization_demo();
                break;

            case 2:
                logger_demo();
                break;

            case 3:
                system("./memory_test");
                break;

            case 4:
                system("./race_test");
                break;

            case 5:
                system("./deadlock_test");
                break;

            case 6:
                printf("\nExiting MsgBus.\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
