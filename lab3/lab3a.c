#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main() 
{
	int fd[2], status;
	pid_t pid;
	char buf[1024];

	time_t childTime, pTime;

	pipe(fd);

	switch(pid = fork()) {
		case -1:
			perror("fork");
			exit(1);
		case 0:

			sleep(2);
			time (&childTime);
			
			close(fd[0]);
			write(fd[1], ctime(&childTime), strlen(ctime(&childTime))+1);
			close(fd[1]);


			exit(status);
		default:
			time (&pTime);

			close(fd[1]);
			read(fd[0], buf, sizeof(buf));
			close(fd[0]);
			wait(&status);
			printf("Child process (PID %d) created at: %s\n", getpid(), buf);
			printf("Parent process created at: %s\n", ctime(&pTime));
	}

	return 0;
}