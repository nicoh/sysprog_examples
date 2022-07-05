#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <assert.h>
#define THREAD_NUM 8


sem_t sem_empty;
sem_t sem_full;
pthread_mutex_t mutex_buffer;


#define MAX_BUFFER 10

int buffer[MAX_BUFFER];
int count = 0;

void* produce(void* args) {
    while (1) {
        // Produce
        int x = rand() % 100;

        sem_wait(&sem_empty);
        pthread_mutex_lock(&mutex_buffer);
	buffer[count] = x;
        count++;
	pthread_mutex_unlock(&mutex_buffer);
        sem_post(&sem_full);
    }
}

void* consume(void* args) {
        while (1) {
        int y;

        // Remove from the buffer
        sem_wait(&sem_full);
        pthread_mutex_lock(&mutex_buffer);
	y = buffer[count - 1];
        count--;
	pthread_mutex_unlock(&mutex_buffer);
        sem_post(&sem_empty);

        // Consume
        printf("Got %d\n", y);
        //sleep(1);
    }
}

int main(int argc, char* argv[]) {
    
    pthread_mutex_init(&mutex_buffer, NULL);
    
    sem_init(&sem_empty, 0, 10);
    sem_init(&sem_full, 0, 0);
    pthread_t producer;
    pthread_t consumer; 


    pthread_create(&producer, NULL, &produce, NULL);
    pthread_create(&consumer, NULL, &consume, NULL); 

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    
    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    pthread_mutex_destroy(&mutex_buffer);
    return 0;
}
