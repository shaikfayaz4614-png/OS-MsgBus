#include <stdio.h>
#include <pthread.h>
#include <sched.h>

#define THREADS 4
#define ITERATIONS 100000

static long unsafe_counter = 0;
static long safe_counter = 0;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *unsafe_worker(void *arg)
{
    (void)arg;

    for (int i = 0; i < ITERATIONS; i++)
    {
        unsafe_counter++;

        if (i % 1000 == 0)
            sched_yield();
    }

    return NULL;
}

void *safe_worker(void *arg)
{
    (void)arg;

    for (int i = 0; i < ITERATIONS; i++)
    {
        pthread_mutex_lock(&mutex);

        safe_counter++;

        pthread_mutex_unlock(&mutex);

        if (i % 1000 == 0)
            sched_yield();
    }

    return NULL;
}

int main()
{
    pthread_t threads[THREADS];

    printf("========================================\n");
    printf("          RACE CONDITION TEST\n");
    printf("========================================\n\n");

    printf("Threads    : %d\n", THREADS);
    printf("Iterations : %d\n", ITERATIONS);
    printf("Expected   : %ld\n\n",
           (long)THREADS * ITERATIONS);

    printf("[Test 1] Without mutex...\n");

    unsafe_counter = 0;

    for (int i = 0; i < THREADS; i++)
        pthread_create(&threads[i], NULL, unsafe_worker, NULL);

    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("Unsafe counter: %ld\n\n", unsafe_counter);

    printf("[Test 2] With mutex...\n");

    safe_counter = 0;

    for (int i = 0; i < THREADS; i++)
        pthread_create(&threads[i], NULL, safe_worker, NULL);

    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("Safe counter:   %ld\n", safe_counter);

    printf("\nRace condition demonstration completed.\n");

    pthread_mutex_destroy(&mutex);

    return 0;
}
