#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h> 
#include <fcntl.h>

int main() 
{
	int rd, wd;
	char path[1024] = "fifo.001";
	char buf[2048];
	time_t childTime, pTime;
	pid_t pid;

	unlink(path);
	if(mkfifo(path, S_IRWXU) == 0)
	{
		switch(pid = fork()) {
			case -1:
				perror("fork");
				exit(1);
			case 0:
				sleep(2);
				time (&childTime);

				if((wd = open(path, O_RDWR)) == - 1) {
					perror("open");
					exit(0);
				} 

				write(wd, ctime(&childTime), strlen(ctime(&childTime))+1); 
				close(wd);

				exit(0);
			default:
				time (&pTime);

				if ((rd = open(path, O_RDONLY)) == -1) { 
				 	perror("open"); 
				 	exit(0); 
				} 

				read(rd, buf, sizeof(buf)); 
				close(rd); 

				printf("\nChild process created at: %s\n", buf);
				printf("Parent: %s\n", ctime(&pTime));
		}
	} else {
		perror("mkfifo");
		exit(1);
	}

	return 0;
}