#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "../include/synchronization.h"

#define THREAD_COUNT 3
#define ITERATIONS 5

static int shared_counter = 0;

static pthread_mutex_t counter_mutex;
static pthread_cond_t condition;
static sem_t semaphore;

static int work_ready = 0;

static void *worker_thread(void *arg)
{
    int id = *(int *)arg;

    printf("[Thread %d] Started. TID: %lu\n",
           id,
           (unsigned long)pthread_self());

    sem_wait(&semaphore);

    printf("[Thread %d] Entered semaphore section.\n", id);

    for (int i = 0; i < ITERATIONS; i++)
    {
        pthread_mutex_lock(&counter_mutex);

        shared_counter++;

        printf("[Thread %d] Counter = %d\n",
               id,
               shared_counter);

        pthread_mutex_unlock(&counter_mutex);

        usleep(100000);
    }

    printf("[Thread %d] Leaving semaphore section.\n", id);

    sem_post(&semaphore);

    pthread_mutex_lock(&counter_mutex);

    work_ready = 1;

    pthread_cond_signal(&condition);

    pthread_mutex_unlock(&counter_mutex);

    return NULL;
}

static void condition_demo(void)
{
    pthread_mutex_lock(&counter_mutex);

    while (!work_ready)
    {
        printf("[Main] Waiting for worker condition...\n");

        pthread_cond_wait(
            &condition,
            &counter_mutex
        );
    }

    printf("[Main] Condition received from worker.\n");

    pthread_mutex_unlock(&counter_mutex);
}

void synchronization_demo(void)
{
    pthread_t threads[THREAD_COUNT];
    int thread_ids[THREAD_COUNT];

    printf("\n========================================\n");
    printf("       SYNCHRONIZATION DEMO\n");
    printf("========================================\n\n");

    shared_counter = 0;
    work_ready = 0;

    pthread_mutex_init(&counter_mutex, NULL);
    pthread_cond_init(&condition, NULL);

    /*
     * Counting semaphore.
     * One permit means one worker enters
     * the protected resource at a time.
     */
    sem_init(&semaphore, 0, 1);

    printf("[Main] Creating %d worker threads...\n\n",
           THREAD_COUNT);

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        thread_ids[i] = i + 1;

        if (pthread_create(
                &threads[i],
                NULL,
                worker_thread,
                &thread_ids[i]) != 0)
        {
            perror("pthread_create");
            return;
        }
    }

    condition_demo();

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\n[Main] All worker threads completed.\n");
    printf("[Main] Final shared counter: %d\n",
           shared_counter);

    sem_destroy(&semaphore);
    pthread_cond_destroy(&condition);
    pthread_mutex_destroy(&counter_mutex);

    printf("\nSynchronization demonstration completed.\n");
}
