#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define NUM_PHILOSOPHERS 5

#define MUTEX_NAME "/my_mutex"
#define FORK_SEM_NAME_PREFIX "/fork_"

#define THINKING 0
#define HUNGRY 1
#define EATING 2

sem_t *forks[NUM_PHILOSOPHERS]; // Semaphores for each chopstick
sem_t *mutex; // Mutex for critical sections

int state[NUM_PHILOSOPHERS]; // Philosopher states (THINKING, HUNGRY, EATING)

void cleanup(int signum) {
    // Close and unlink named semaphores for forks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        char sem_name[50];
        sprintf(sem_name, "%s%d", FORK_SEM_NAME_PREFIX, i);
        sem_close(forks[i]);
        sem_unlink(sem_name);
    }

    // Close and unlink named mutex
    sem_close(mutex);
    sem_unlink(MUTEX_NAME);

    exit(signum);
}

void thinking(int i) {
    printf("Philosopher %d is thinking\n", i);
}

void eating(int i) {
    printf("Philosopher %d is eating\n", i);
}

void pickup_forks(int i) {
    sem_wait(mutex);
    state[i] = HUNGRY;
    printf("Philosopher %d is hungry\n", i);
    sem_post(mutex);

    sem_wait(forks[i]);
    sem_wait(forks[(i + 1) % NUM_PHILOSOPHERS]);

    sem_wait(mutex);
    state[i] = EATING;
    printf("Philosopher %d picked up both forks and is eating\n", i);
    sem_post(mutex);
}

void putdown_forks(int i) {
    sem_wait(mutex);
    state[i] = THINKING;
    printf("Philosopher %d put down both forks\n", i);
    sem_post(mutex);

    sem_post(forks[i]);
    sem_post(forks[(i + 1) % NUM_PHILOSOPHERS]);
}

void* philosopher(void* arg) {
    int i = *(int*)arg;
    int mealCount = 0;

    while (mealCount < 10) {
        thinking(i);
        pickup_forks(i);
        eating(i);
        putdown_forks(i);
        mealCount++;
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];

    // Unlink semaphores at the start
    sem_unlink(MUTEX_NAME);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        char sem_name[50];
        sprintf(sem_name, "%s%d", FORK_SEM_NAME_PREFIX, i);
        sem_unlink(sem_name);
    }

    // Register the cleanup function to be called on termination
    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);

    mutex = sem_open(MUTEX_NAME, O_CREAT | O_EXCL, 0644, 1);
    if (mutex == SEM_FAILED) {
        perror("sem_open (mutex)");
        exit(1);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        char sem_name[50];
        sprintf(sem_name, "%s%d", FORK_SEM_NAME_PREFIX, i);
        forks[i] = sem_open(sem_name, O_CREAT | O_EXCL, 0644, 1);
        if (forks[i] == SEM_FAILED) {
            perror("sem_open (forks)");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        state[i] = THINKING;
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, philosopher, id);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_exit(NULL); // Ensure main thread doesn't exit until all philosopher threads have finished

    return 0;
}