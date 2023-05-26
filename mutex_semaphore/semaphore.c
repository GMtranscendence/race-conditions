#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_NUM 8
#define BUFF_SIZE 10

sem_t semFull;
sem_t semEmpty;
sem_t semGlobal;

int buffer[BUFF_SIZE];
int count = 0;

void* producer(){
    int item_id = 1;
    while(1){
        sem_wait(&semEmpty);
        sem_wait(&semGlobal);
        usleep(rand()%10000);
        buffer[count++] = item_id++;
        printf("Produced product id: %d\n", item_id);
        printf("Count: %d\n", count);
        sem_post(&semGlobal);
        sem_post(&semFull);
    }
}

void* consumer(){
    while(1){
        sem_wait(&semFull);
        sem_wait(&semGlobal);
        usleep(rand()%10000);
        int y = buffer[--count];
        printf("Consumed product id: %d\n", y);
        printf("Count: %d\n", count);
        sem_post(&semGlobal);
        sem_post(&semEmpty);

    }
}

int main() {
    pthread_t th[THREAD_NUM];
    sem_init(&semEmpty, 0, 10);
    sem_init(&semFull, 0, 0);
    sem_init(&semGlobal, 0, 1);
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

    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    sem_destroy(&semGlobal);

    return 0;
}
