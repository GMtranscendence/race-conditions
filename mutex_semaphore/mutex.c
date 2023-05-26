#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_NUM 2
#define BUFF_SIZE 10

pthread_mutex_t mutexProduct;
pthread_cond_t condProducer;
pthread_cond_t condConsumer;

int buffer[BUFF_SIZE];
int count = 0;

void* producer(){
    int item_id = 1;
    while(1){
        pthread_mutex_lock(&mutexProduct);
        while (count > 9){
            pthread_cond_wait(&condProducer, &mutexProduct);
        }
        usleep(rand()%10000);
        buffer[count++] = item_id++;
        printf("Produced product id: %d\n", item_id);
        printf("Count: %d\n", count);
        pthread_mutex_unlock(&mutexProduct);
        pthread_cond_signal(&condConsumer);
    }
}

void* consumer(){
    while(1){
        pthread_mutex_lock(&mutexProduct);
        while (count < 1){
            pthread_cond_wait(&condConsumer, &mutexProduct);
        }
        usleep(rand()%10000);
        int y = buffer[--count];
        printf("Consumed product id: %d\n", y);
        printf("Count: %d\n", count);
        pthread_mutex_unlock(&mutexProduct);
        pthread_cond_signal(&condProducer);
    }
}

int main() {
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexProduct, NULL);
    pthread_cond_init(&condProducer, NULL);
    pthread_cond_init(&condConsumer, NULL);

    for (int i = 0; i < THREAD_NUM; i++){
        if (i % 2 == 0){
            if (pthread_create(&th[i], NULL, &producer, NULL) != 0){
                perror("Failed to create a producer thread");
            }
        }
        else{
            if (pthread_create(&th[i], NULL, &consumer, NULL) != 0){
                perror("Failed to create a consumer thread");
            }
        }
    }
    for (int i = 0; i < THREAD_NUM; i++){
        if (pthread_join(th[i], NULL) != 0){
            perror("Failed to join a thread");
        }
    }

    pthread_mutex_destroy(&mutexProduct);
    pthread_cond_destroy(&condProducer);
    pthread_cond_destroy(&condConsumer);

    return 0;
}
