#include <stdio.h>
#include <unistd.h>
//#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#define N 3

void fileInfo(int fd){
    struct stat file_info;
    fstat(fd, &file_info);
    printf("\n----------------------------\n");
    printf("ID: %ld\n", file_info.st_dev);
    printf("Inode: %ld\n", file_info.st_ino);
    printf("File Permissions: %o\n", file_info.st_mode & 0777);
    printf("File Size: %ld bytes\n", file_info.st_size);
    printf("Block Size: %ld\n", file_info.st_blksize);
    printf("File Owner: %d\n", file_info.st_uid);
    printf("File Group: %d\n", file_info.st_gid);
    printf("Last Accessed: %s", ctime(&file_info.st_atime));
    printf("Last Modified: %s", ctime(&file_info.st_mtime));
    printf("Last Status Change: %s", ctime(&file_info.st_ctime));
    printf("----------------------------\n");
}

void fileOperation(int processNum){
    FILE *file;
    char buffer[100];
    int bytes_to_write = 6;
    // read
    if(processNum == 0){
        file = fopen("./test.txt", "rb");
        int bytes_read = fread(buffer, 1, 100, file);

        if (bytes_read > 0) {
            printf("Bytes read: %d\n", bytes_read);
            printf("Data read: %.*s\n", bytes_read, buffer);  // Print the read data
        } else {
            printf("No data read.\n");
        }
        printf("\nFinished reading. Displaying info...\n");

    }
    // write
    else if(processNum == 1){
        char data[] = "Write bytes"; // Data to be written
        file = fopen("./test.txt", "wb");
        fwrite(data, sizeof(char), bytes_to_write, file);
        printf("\nFinished writing. Displaying info...\n");
    }
    // append
    else if(processNum == 2){
        char data[] = "Append bytes"; // Data to be written
        file = fopen("./test.txt", "ab");
        fwrite(data, sizeof(char), bytes_to_write, file);
        printf("\nFinished appending. Displaying info...\n");
    }
    fileInfo(fileno(file));
    fclose(file);
}

int main() {
    for (int i = 0; i < N; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            printf("Fork failed.\n");
            return 1;
        } else if (pid == 0) {
            fileOperation(i);
            return 0;
        }
    }
    return 0;
}
