#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>


int main()
{
	int s;
	char buff[1024], server_buff[1024];
	time_t myTime;
	
	
	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

	struct sockaddr_in server;
     	server.sin_family = AF_INET;
     	server.sin_addr.s_addr = inet_addr("127.0.0.1");
     	server.sin_port   = htons(25565);

	connect(s, (struct sockaddr *)&server, sizeof(server));

	int i = 0;
	for(i; i < 5; i++) {
		memset(&server_buff, 0, sizeof(server_buff));
		time(&myTime);
		strcpy(buff, ctime(&myTime));
		sleep(1);

		if (send(s, buff, strlen(buff), 0) < 0)
			perror("send");
		
		recv(s, server_buff, sizeof(server_buff), 0);
		printf("Response: %s\n", &server_buff[0]);
	}

	close(s);
	printf("Connection closed\n");

	return 0;
}

