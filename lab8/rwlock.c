#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <fcntl.h>

struct thread_data{
	int shmid;
	pthread_rwlock_t *rwlock_ptr;
};

void *second_thread(void *this_data)
{
	time_t childTime;
	char *shm;

	struct thread_data *data;
	data = (struct thread_data *)this_data;
	pthread_rwlock_wrlock(data->rwlock_ptr);

	sleep(5);

	shm = (char *)shmat(data->shmid, NULL, 0);
	if(shm == (char *)(-1)){
		perror("shmat");
		exit(1);
	}

	time(&childTime);

	sprintf(shm, ctime(&childTime));
	pthread_rwlock_unlock(data->rwlock_ptr);
}

int main(int argc, char const *argv[])
{
	int shmid;
	char *shm;
	pthread_t thid;
	time_t pTime;
	time_t start, end; // timer

	pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
	if(pthread_rwlock_init(&lock, NULL) != 0){
		perror("pthread_rwlock_init");
		exit(1);
	}

	shmid = shmget(IPC_PRIVATE, 28, IPC_CREAT | 0666);
	if(shmid < 0){
		perror("shmget");
		exit(1);
	}

	shm = (char *)shmat(shmid, NULL, 0);
	if(shm == (char *)(-1)){
		perror("shmat");
		exit(1);
	}

	struct thread_data data;
	data.rwlock_ptr = &lock;
	data.shmid = shmid;

	if(pthread_create(&thid, NULL, second_thread, &data) == 0){
		time(&pTime);

		sleep(1);

		time(&start);
		if(pthread_rwlock_rdlock(&lock) == 0){
			time(&end);
			double elapsedTime = difftime(end, start);
			printf("%.f seconds waited\n", elapsedTime);

			printf("\nParent process was created at: %s", ctime(&pTime));
			printf("Child process was created at: %s\n", shm);
		}
		else {
			perror("pthread_cond_wait");
			exit(1);
		}
	}
	else {
		perror("pthread_rwlock_rdlock");
		exit(1);
	}

	return 0;
}