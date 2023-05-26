#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <stdlib.h>

#define K 50
// Define the struct type
struct Message {
    long type;  // Message type
    int lineToDelete;   // Integer value to pass
}msg;

int main() {
    const key_t key = ftok(".", 'm');
    const int queueId = msgget(key, IPC_CREAT | 0666);
    for (int i = 0; i < K; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            printf("Fork failed.\n");
            return 1;
        } else if (pid == 0) {
            // Child process
            srand(getpid());
            int randomNum = (rand() % 100) + 1;
            struct Message msg;
            msg.type = 1;
            msg.lineToDelete = randomNum;
            msgsnd(queueId, &msg, sizeof(msg), 0);
            printf("Data sent: %d\n", msg.lineToDelete);
            return 0;
        }
    }
    return 0;
}
