#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_CHAIRS 5
#define NUM_CUSTOMERS 10

sem_t customers;
sem_t barbers;
sem_t mutex;
int waiting = 0;

void *barber(void *arg) {
    while (1) {
        sem_wait(&customers);
        sem_wait(&mutex);
        waiting--;
        sem_post(&barbers);
        sem_post(&mutex);

        // Cutting hair
        printf("Barber is cutting hair.\n");
    }
    return NULL;
}

void *customer(void *arg) {
    while (1) {
        sem_wait(&mutex);
        if (waiting < NUM_CHAIRS) {
            waiting++;
            sem_post(&customers);
            sem_post(&mutex);
            sem_wait(&barbers);
            return NULL;
        }
        sem_post(&mutex);

        // Customer leaves because there are no empty chairs
        printf("Customer leaves because there are no empty chairs.\n");
    }
}

int main() {
    pthread_t barber_tid;
    pthread_t customers_tid[NUM_CUSTOMERS];

    sem_init(&customers, 0, 0);
    sem_init(&barbers, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&barber_tid, NULL, barber, NULL);
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_create(&customers_tid[i], NULL, customer, NULL);
    }

    pthread_join(barber_tid, NULL);
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customers_tid[i], NULL);
    }

    sem_destroy(&customers);
    sem_destroy(&barbers);
    sem_destroy(&mutex);

    return 0;
}
