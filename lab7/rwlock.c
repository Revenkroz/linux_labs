#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

pthread_rwlock_t rwlock;

void *reader();
void *writer();

int main()
{
	pthread_t rd, wr;
	int chk;

	chk = pthread_rwlock_init(&rwlock, NULL);
	if (chk != 0) {
		perror("pthread_rwlock_init");
		exit(1);
	}

	printf("MAIN: grab the first rdlock\n");
	pthread_rwlock_rdlock(&rwlock);

	printf("MAIN: grab the same rdlock\n");
  	pthread_rwlock_rdlock(&rwlock);

	printf("MAIN: Starting threads...\n");
	pthread_create(&rd, NULL, reader, NULL);

	printf("MAIN: unlock the first rdlock\n");
  	pthread_rwlock_unlock(&rwlock);

	pthread_create(&wr, NULL, writer, NULL);

	sleep(2);
	printf("MAIN: unlock the second rdlock\n");
  	pthread_rwlock_unlock(&rwlock);

	printf("MAIN: Waiting for the threads\n");
	pthread_join(rd, NULL);
	pthread_join(wr, NULL);

	pthread_rwlock_destroy(&rwlock);
	printf("Norm.\n");

	return 0;
}

void *reader()
{
	printf("Starting rdLocker...\n");
	pthread_rwlock_rdlock(&rwlock);

	sleep(2);

	printf("Unlock the rdlock\n");
	pthread_rwlock_unlock(&rwlock);
}

void *writer()
{
	printf("Starting wrLocker...\n");
	pthread_rwlock_wrlock(&rwlock);

	sleep(2);

	printf("Unlock the wrlock\n");
	pthread_rwlock_unlock(&rwlock);
}
