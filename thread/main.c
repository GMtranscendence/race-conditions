#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 1

void cleanup_handler(void *arg) {
    printf("%s", "Canceling a child thread...\n");
}
void *print_lines(void *thread_id) {
    pthread_cleanup_push(cleanup_handler, NULL);
    for (int i = 0; i < 1000; i++) {
        printf("Child thread says: '%s': line %d\n", thread_id, i);
    }
    pthread_cleanup_pop(0);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    char *thread_msg[] = {"Hello", "world", "how", "are you?"};
    for (long t = 0; t < NUM_THREADS; t++) {
        printf("Creating child thread %ld\n", t);
        pthread_create(&threads[t], NULL, print_lines, (void *)thread_msg[t]);
    }
    sleep(2);
    for (long t = 0; t < NUM_THREADS; t++) {
        pthread_cancel(threads[t]);
    }
    pthread_exit(NULL);
}

