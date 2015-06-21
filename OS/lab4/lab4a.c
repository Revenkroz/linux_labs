#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int fd[2];

void *reader();
void *writer();

int main()
{
    pthread_t rd, wr;
    int result;

    result = pipe(fd);
    if (result < 0) {
        perror("pipe");
        exit(1);
    }
    printf("Starting threads...\n");
    pthread_create(&rd, NULL, reader, NULL);
    pthread_create(&wr, NULL, writer, NULL);

    pthread_join(rd, NULL);
    pthread_join(wr, NULL);

    return 0;
}

void *reader()
{
    char buf[1024];

    sleep(2);

    read(fd[0], buf, sizeof(buf));

    printf("READER: Time recieved from Writer: %s\n", buf); 
}

void *writer()
{
    time_t writerTime;
    time(&writerTime);

    write(fd[1], ctime(&writerTime), strlen(ctime(&writerTime))+1);
}