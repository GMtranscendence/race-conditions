#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_NUM 2
#define POROM_SIZE 10*2

pthread_mutex_t mutexQueue;

int desCount = 0;
int pihCount = 0;

void* check_count(){
    if (pihCount == desCount || desCount == 0 || pihCount == 0){
        usleep(rand()%1000000);
        return NULL;
    }

    double percentage_of_des = ((double)desCount / POROM_SIZE) / 4.0;

    if (percentage_of_des > 0.75){
        while (pihCount > 0){
            usleep(rand()%1000000);
            printf("Піхотинець номер %d вийшов з борту\n", pihCount--);
        }
        while (desCount < POROM_SIZE){
            usleep(rand()%1000000);
            printf("Десантник номер %d зайшов на борт\n", ++desCount);
        }
        return NULL;
    }
    else if (percentage_of_des < 0.25){
        while (desCount > 0){
            usleep(rand()%1000000);
            printf("Десантник номер %d вийшов з борту\n", desCount--);
        }
        while (pihCount < POROM_SIZE){
            usleep(rand()%1000000);
            printf("Піхотинець номер %d зайшов на борт\n", ++pihCount);
        }
        return NULL;
    }

    if (POROM_SIZE % 2 == 0){
        int increment = (desCount < pihCount) ? 1 : -1;
        int decrement = -increment;
        char incText[70];
        char decText[70];

        while (desCount != pihCount) {
            usleep(rand()%1000000);
            desCount += increment;
            if (increment == 1){
                sprintf(incText, "Десантник номер %d зайшов на борт\n", desCount);
                sprintf(decText,  "Піхотинець номер %d вийшов з борту\n", pihCount);
            }
            else {
                sprintf(incText, "Десантник номер %d вийшов з борту\n", desCount);
                sprintf(decText,  "Піхотинець номер %d зайшов на борт\n", pihCount);
            }
            printf("%s\n", incText);
            usleep(rand()%1000000);
            printf("%s\n", decText);
            pihCount += decrement;
        }
    }
}

void* pih(){
    int item_id = 1;
    while(1){
        usleep(rand()%1000000);
        pthread_mutex_lock(&mutexQueue);
        printf("Піхотинець номер %d зайшов на борт\n", ++pihCount);
        printf("Наступний!\n\n");
        if (desCount + pihCount >= POROM_SIZE){
            printf("Перевірка кількості воїнів на борті...\n");
            check_count();
            printf("\nНа паромі %d піхотинців та %d десантників\n", pihCount, desCount);
            printf("Паром відправляється. Очікуємо повернення...\n\n");
            usleep(rand()%10000000);
            desCount = 0;
            pihCount = 0;
        }
        pthread_mutex_unlock(&mutexQueue);
    }
}

void* des(){
    while(1){
//        while(desCount + pihCount <= POROM_SIZE){
        usleep(rand()%100000);
        pthread_mutex_lock(&mutexQueue);
        printf("Десантник номер %d зайшов на борт\n", ++desCount);
        printf("Наступний!\n\n");
        if (desCount + pihCount >= POROM_SIZE){
            printf("Перевірка кількості воїнів на борті...\n");
            check_count();
            printf("На паромі %d піхотинців та %d десантників\n", pihCount, desCount);
            printf("Паром відправляється. Очікуємо повернення...\n\n");
            usleep(rand()%10000000);
            desCount = 0;
            pihCount = 0;
        }
        pthread_mutex_unlock(&mutexQueue);
//        } 

    }
}

int main() {
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexQueue, NULL);

    for (int i = 0; i < THREAD_NUM; i++){
        if (i % 2 == 0){
            if (pthread_create(&th[i], NULL, &pih, NULL) != 0){
                perror("Failed to create a pihotynec thread");
            }
        }
        else{
            if (pthread_create(&th[i], NULL, &des, NULL) != 0){
                perror("Failed to create a desantnyk thread");
            }
        }
    }
    for (int i = 0; i < THREAD_NUM; i++){
        if (pthread_join(th[i], NULL) != 0){
            perror("Failed to join a thread");
        }
    }

    pthread_mutex_destroy(&mutexQueue);

    return 0;
}

