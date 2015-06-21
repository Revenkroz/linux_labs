#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int main()
{
	struct sockaddr_in server;
	int sock_fd, c_fd;
	char readbuf[2048], writebuf[2048];
	char addr_p[1024];
	int n;
	
 	server.sin_family = AF_INET;
 	server.sin_addr.s_addr = inet_addr("127.0.0.1");
 	server.sin_port = htons(25565);
	
	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

	bind(sock_fd, (struct sockaddr *)&server, sizeof(server));
	
	listen(sock_fd, 10);

	sprintf(writebuf, "Received!");
	printf("Waiting...\n");

	c_fd = accept(sock_fd, NULL, NULL);

	while(1) {
		n = recv(c_fd, readbuf, sizeof(readbuf), 0);
		if(n < 0) {
			perror("recv");
			exit(1);
		}
		else if(n == 0) {
			printf("The connection has been closed.\n");
			close(c_fd);
			break;
		}
		else {
			printf("Received message: %s\n", readbuf);
			memset(&readbuf, 0, sizeof(readbuf));
			send(c_fd, writebuf, sizeof(writebuf), 0);
		}

		//close(c_fd);
	}
	
	close(sock_fd);

	return 0;
}
