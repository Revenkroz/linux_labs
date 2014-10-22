#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PI 3.14159265

void getTime(struct tm **currentTime);

int main()
{
	struct tm *currentTime;
	getTime(&currentTime);
	// printf("Current Hour: %d\n", currentTime->tm_hour);

	int processesNum = (int) pow(2, currentTime->tm_hour);
	// int processesNum = 8832;
	// printf("%d\n", processesNum);

	pid_t pid;
	int status;
	double sine;
	int i = 0;

	for(i; i < processesNum; i++) {
		switch(pid = fork()) {
		  case -1:
		        perror("fork");
		        exit(1); 
		  case 0:
				sine = sin(PI * currentTime->tm_min / 14); 
		        exit(status);
		  default:
		        wait(&status);
		        printf("PID: %d | Exit Status: %d \n", (int) pid, WEXITSTATUS(status));
		}
	}

	time_t timer;
	time(&timer);
	double elapsedTime = difftime(timer, mktime(currentTime));
	printf("Processes generated in %.f seconds. \n\n", elapsedTime);

	return 0;
}

void getTime(struct tm **currentTime)
{
	time_t rawTime;
	time(&rawTime);
	*currentTime = localtime(&rawTime);
}