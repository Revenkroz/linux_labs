#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char const *argv[])
{
	int status, shmid;
	char *shm, *str, *time_temp;

	time_t childTime, pTime;

	shmid = shmget(IPC_PRIVATE, 12800, IPC_CREAT | IPC_EXCL | 0666);
	if(shmid < 0){
		perror("shmget");
		exit(1);
	}

	shm = (char*)shmat(shmid, NULL, 0);
	if(shm == (char*)(-1)){
		perror("shmat");
		exit(1);
	}

	switch(fork()){
		case -1:
			perror("fork");
			exit(1);
		case 0:
			sleep(2);
			time(&childTime);

			shm = (char*)shmat(shmid, NULL, 0);
			if(shm == (char*)(-1)){
				perror("shmat");
				exit(1);
			}

			sprintf(shm, ctime(&childTime));

			exit(0);
		default:
			time(&pTime);

			wait(&status);

			printf("Parent process was created at: %s\n", ctime(&pTime));
			printf("Child process was created at: %s\n", shm);

			shmdt(shm);
	}

	return 0;
}