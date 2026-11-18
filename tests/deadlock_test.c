#include <stdio.h>
#include <pthread.h>
#include <errno.h>

static pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;

void *thread_one(void *arg)
{
    (void)arg;

    printf("[Thread 1] Locking Mutex A...\n");

    pthread_mutex_lock(&mutex_a);

    printf("[Thread 1] Mutex A locked.\n");

    /*
     * Try instead of blocking forever.
     */
    int result = pthread_mutex_trylock(&mutex_b);

    if (result == EBUSY)
    {
        printf("[Thread 1] Mutex B is busy.\n");
        printf("[Thread 1] Potential deadlock detected.\n");
    }
    else if (result == 0)
    {
        printf("[Thread 1] Mutex B acquired.\n");
        pthread_mutex_unlock(&mutex_b);
    }

    pthread_mutex_unlock(&mutex_a);

    return NULL;
}

void *thread_two(void *arg)
{
    (void)arg;

    printf("[Thread 2] Locking Mutex B...\n");

    pthread_mutex_lock(&mutex_b);

    printf("[Thread 2] Mutex B locked.\n");

    /*
     * Try instead of blocking forever.
     */
    int result = pthread_mutex_trylock(&mutex_a);

    if (result == EBUSY)
    {
        printf("[Thread 2] Mutex A is busy.\n");
        printf("[Thread 2] Potential deadlock detected.\n");
    }
    else if (result == 0)
    {
        printf("[Thread 2] Mutex A acquired.\n");
        pthread_mutex_unlock(&mutex_a);
    }

    pthread_mutex_unlock(&mutex_b);

    return NULL;
}

int main()
{
    pthread_t t1;
    pthread_t t2;

    printf("========================================\n");
    printf("             DEADLOCK TEST\n");
    printf("========================================\n\n");

    printf("Demonstrating circular wait safely.\n\n");

    pthread_create(&t1, NULL, thread_one, NULL);
    pthread_create(&t2, NULL, thread_two, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex_a);
    pthread_mutex_destroy(&mutex_b);

    printf("\nDeadlock demonstration completed safely.\n");

    return 0;
}
