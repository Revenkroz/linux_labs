#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

int main() 
{
	int s; 
	char c = ' ';
	time_t myTime;
	char buff[1024], server_buff[1024];

	s = socket (AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in server;
     	server.sin_family = AF_INET;
     	server.sin_addr.s_addr = inet_addr("127.0.0.1");
     	server.sin_port   = htons(25565);

    int i = 0;
	for(i; i < 5; i++) {
		time(&myTime);
		strcpy(buff, ctime(&myTime));
		sleep(1);

		sendto(s, buff, sizeof(buff), 0, (struct sockaddr *)&server, sizeof(server));
		
		memset(&server_buff, 0, sizeof(server_buff));
		read(s, server_buff, 1024);
		printf("Response: %s\n", server_buff);
	}

	close(s);
	printf("Connection closed\n");
	return 0;
}

