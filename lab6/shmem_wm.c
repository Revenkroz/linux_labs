#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <fcntl.h>

struct thread_data {
	int shmid;
	pthread_mutex_t *mutex_ptr;
};

void *second_thread(void *this_data)
{
	time_t childTime;
	char *shm;

	struct thread_data *data;
	data = (struct thread_data *)this_data;
	pthread_mutex_lock(data->mutex_ptr);

	sleep(5);

	shm = (char *)shmat(data->shmid, NULL, 0);
	if(shm == (char *)(-1)){
		perror("shmat");
		exit(1);
	}

	time(&childTime);

	sprintf(shm, ctime(&childTime));
	pthread_mutex_unlock(data->mutex_ptr);
}

int main(int argc, char const *argv[])
{
	int shmid;
	char *shm;
	pthread_t thid;
	time_t pTime;

	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	if(pthread_mutex_init(&mutex, NULL) != 0){
		perror("pthread_mutex_init");
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

	// запихиваем параметры треда в структуру
	struct thread_data data;
	data.mutex_ptr = &mutex;
	data.shmid = shmid;

	if(pthread_create(&thid, NULL, second_thread, &data) == 0){
		time(&pTime);

		sleep(1);
		while(pthread_mutex_trylock(&mutex) != 0){
			printf("BUSY WAIT!!!\n"); // :)
			sleep(1);
		}

		printf("\nParent process was created at: %s", ctime(&pTime));
		printf("Child process was created at: %s\n", shm);
		shmdt(shm);
	}
	else {
		perror("pthread_create");
		exit(1);
	}

	return 0;
}