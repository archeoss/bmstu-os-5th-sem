#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define port 9877
#define buf_size 22

static void set_sockaddr(struct sockaddr_in *addr)
{
	memset((char *)addr, 0, sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = INADDR_ANY;
	addr->sin_port = htons(port);
}

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
	int sockfd;
	struct sockaddr_in srv_addr;
	char buf[buf_size];

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket() fail");
		exit(1);
	}

	set_sockaddr(&srv_addr);

	if (connect(sockfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) 
    {
		perror("connect() fail");
		exit(1);
	}
    printf("CONNECTED. PID = %d\n", getpid());

	snprintf(buf, sizeof(buf), "CLIENT PID = %d", getpid());
	printf("WRITE: %s\n", buf);
	write(sockfd, buf, sizeof(buf));
	memset(buf, 0, sizeof(buf));
	read(sockfd, buf, sizeof(buf));
	printf("READ: %s\n", buf);
	
	close(sockfd);
    return 0;
}
