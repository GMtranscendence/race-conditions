#include <stdio.h>
#include <string.h>
#include <sys/msg.h>

#define BUFFER 100
#define DB_SIZE 100


struct Message {
    long type;
    int lineToDelete;
}msg;

struct Database{
  int line_number;
  char key[BUFFER];
  char value[BUFFER];
}db[DB_SIZE];

void show_db(){
    for(int i = 0; i < DB_SIZE-1; i++){
        if (strcmp(db[i].key, "") == 0 && strcmp(db[i].value, "") == 0){
            break;
        }
        printf("%d.\t'%s'\t'%s'\n", db[i].line_number, db[i].key, db[i].value);
    }
}

void generate_db(){
    char key[] = "key";
    for(int i = 1; i < DB_SIZE; i++){
        db[i-1].line_number = i;
        sprintf(db[i-1].key, "%s %d", "key", i);
        sprintf(db[i-1].value, "%s %d", "value", i);
    }
    db[DB_SIZE-1].line_number = 0;
    strcpy(db[DB_SIZE-1].key, "");
    strcpy(db[DB_SIZE-1].value, "");
    show_db();
}

void delete_db_line(int line){
    if (strcmp(db[line-1].key, "") != 0 || strcmp(db[line-1].value, "") != 0){
        printf("Deleted line: %d.\t%s\t%s\n", db[line-1].line_number, db[line-1].key, db[line-1].value);
    }
    else{
        printf("Couldn't delete the line\n");
    }
    for(int i = line; i < DB_SIZE; i++){
        db[i-1] = db[i];
        db[i-1].line_number--;
    }
}

int main() {
    generate_db();
    const key_t key = ftok(".", 'm');
    const int queueId = msgget(key, IPC_CREAT | 0666);
    int count = 0;
    while(1) {
        struct Message msg;
        msgrcv(queueId, &msg, sizeof(msg), 1, 0);
        delete_db_line(msg.lineToDelete);
        if (count == 49){
            show_db();
        }
        count++;
    }
    return 0;
}
