#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <assert.h>
#define THREAD_NUM 8


pthread_mutex_t mutex_buffer;


#define MAX_BUFFER 10

int buffer[MAX_BUFFER];
int count = 0;

void* produce(void* args) {
    while (1) {
        // Produce
        int x = rand() % 100;

        pthread_mutex_lock(&mutex_buffer);
	if(count < MAX_BUFFER) {
           buffer[count] = x;
	   count++;
	}
	pthread_mutex_unlock(&mutex_buffer);
    }
}

void* consume(void* args) {
        while (1) {
        int y;

        // Remove from the buffer
        pthread_mutex_lock(&mutex_buffer);
	if(count > 0) {
	   y = buffer[count - 1];
           count--;
	}
	pthread_mutex_unlock(&mutex_buffer);

        // Consume
        printf("Got %d\n", y);
    }
}

int main(int argc, char* argv[]) {
    
    pthread_mutex_init(&mutex_buffer, NULL);
    
    pthread_t producer;
    pthread_t consumer; 


    pthread_create(&producer, NULL, &produce, NULL);
    pthread_create(&consumer, NULL, &consume, NULL); 

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    
    pthread_mutex_destroy(&mutex_buffer);
    return 0;
}
