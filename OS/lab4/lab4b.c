#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h> 
#include <fcntl.h>

char path[1024] = "fifo.001";

void *reader();
void *writer();

int main() 
{
    pthread_t rd, wr;

	unlink(path);
	if(mkfifo(path, S_IRWXU) == 0)
	{
		printf("Starting threads...\n");
	    pthread_create(&rd, NULL, reader, NULL);
	    pthread_create(&wr, NULL, writer, NULL);

	    pthread_join(rd, NULL);
	    pthread_join(wr, NULL);
	} else {
		perror("mkfifo");
		exit(1);
	}

	return 0;
}

void *reader()
{
	int rw;
	char buf[2048];

	if ((rw = open(path, O_RDONLY)) == -1) { 
	 	perror("open");
	} 

	read(rw, buf, sizeof(buf)); 
	close(rw); 

    printf("READER: Time recieved from Writer: %s\n", buf); 
}

void *writer()
{
	int wd;
	time_t wrTime;

	sleep(2);
	time (&wrTime);

	if((wd = open(path, O_RDWR)) == -1) {
		perror("open");
	} 

	write(wd, ctime(&wrTime), strlen(ctime(&wrTime))+1); 
	close(wd);
}