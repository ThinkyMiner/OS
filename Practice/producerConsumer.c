#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define N 5

int buffer[N];
sem_t empty, full, mutex;
int in = 0, out = 0;

void *producer(void *arg) {
  int item;
  for (int i = 0; i < 10; i++) {
    item = i;

    sem_wait(&empty);
    sem_wait(&mutex);

    buffer[in] = item;
    printf("Produced: %d\n", item);
    in = (in + 1) % N;

    sem_post(&mutex);
    sem_post(&full);
  }
  pthread_exit(NULL);
}

void *consumer(void *arg) {
  int item;
  for (int i = 0; i < 10; i++) {
    sem_wait(&full);
    sem_wait(&mutex);

    item = buffer[out];
    printf("Consumed: %d\n", item);
    out = (out + 1) % N;

    sem_post(&mutex);
    sem_post(&empty);
  }
  pthread_exit(NULL);
}

int main() {
  pthread_t producerThread, consumerThread;

  sem_init(&empty, 0, N);
  sem_init(&full, 0, 0);
  sem_init(&mutex, 0, 1);

  pthread_create(&producerThread, NULL, producer, NULL);
  pthread_create(&consumerThread, NULL, consumer, NULL);

  pthread_join(producerThread, NULL);
  pthread_join(consumerThread, NULL);

  sem_destroy(&empty);
  sem_destroy(&full);
  sem_destroy(&mutex);

  return 0;
}
