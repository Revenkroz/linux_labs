#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

int main()
{
	int portno = 25565, sock_fd;
	struct sockaddr_in serv_addr, cl_addr; 
	int recvlen; 
	char readbuf[2048], writebuf[2048];
	socklen_t addrlen = sizeof(cl_addr);

	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("socket");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(portno);

	if ((bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
	 	perror("bind");
		return -1;
	}

	printf("Waiting on port %d\n", portno);
	while(1) {
		memset(&writebuf, 0, sizeof(writebuf));
		sleep(1);
		recvlen = recvfrom(sock_fd, readbuf, 2048, 0, (struct sockaddr*)&cl_addr, &addrlen) ;
		printf("received %d bytes\n", recvlen);
		if (recvlen > 0) {
			printf("Received message: \"%s\"\n", &readbuf[0]);
		}
		sprintf(writebuf, "Recieved!");
		sendto(sock_fd, writebuf, strlen(writebuf), 0, (struct sockaddr*)&cl_addr,addrlen);
	}

	return 0;
}
