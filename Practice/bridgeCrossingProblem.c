#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PEDESTRIANS 5
#define NUM_VEHICLES 3
#define MAX_CROSSING_TIME 3

sem_t bridge_mutex;
sem_t pedestrian_queue;
sem_t vehicle_queue;

void *pedestrian(void *arg) {
    int id = *(int *)arg;

    // Waiting to cross the bridge
    sem_wait(&pedestrian_queue);
    sem_wait(&bridge_mutex);
    sem_post(&pedestrian_queue);

    // Crossing the bridge
    printf("Pedestrian %d is crossing the bridge\n", id);
    sleep(1 + rand() % MAX_CROSSING_TIME);

    sem_post(&bridge_mutex);
    printf("Pedestrian %d has crossed the bridge\n", id);

    return NULL;
}

void *vehicle(void *arg) {
    int id = *(int *)arg;

    // Waiting to cross the bridge
    sem_wait(&vehicle_queue);
    sem_wait(&bridge_mutex);
    sem_post(&vehicle_queue);

    // Crossing the bridge
    printf("Vehicle %d is crossing the bridge\n", id);
    sleep(1 + rand() % MAX_CROSSING_TIME);

    sem_post(&bridge_mutex);
    printf("Vehicle %d has crossed the bridge\n", id);

    return NULL;
}

int main() {
    pthread_t pedestrians[NUM_PEDESTRIANS];
    pthread_t vehicles[NUM_VEHICLES];
    int pedestrian_ids[NUM_PEDESTRIANS];
    int vehicle_ids[NUM_VEHICLES];

    sem_init(&bridge_mutex, 0, 1);
    sem_init(&pedestrian_queue, 0, 1);
    sem_init(&vehicle_queue, 0, 1);

    // Create pedestrian threads
    for (int i = 0; i < NUM_PEDESTRIANS; i++) {
        pedestrian_ids[i] = i + 1;
        pthread_create(&pedestrians[i], NULL, pedestrian, &pedestrian_ids[i]);
    }

    // Create vehicle threads
    for (int i = 0; i < NUM_VEHICLES; i++) {
        vehicle_ids[i] = i + 1;
        pthread_create(&vehicles[i], NULL, vehicle, &vehicle_ids[i]);
    }

    // Wait for pedestrian threads to finish
    for (int i = 0; i < NUM_PEDESTRIANS; i++) {
        pthread_join(pedestrians[i], NULL);
    }

    // Wait for vehicle threads to finish
    for (int i = 0; i < NUM_VEHICLES; i++) {
        pthread_join(vehicles[i], NULL);
    }

    sem_destroy(&bridge_mutex);
    sem_destroy(&pedestrian_queue);
    sem_destroy(&vehicle_queue);

    return 0;
}
