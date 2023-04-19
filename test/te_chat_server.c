#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "debug.h"
#include <poll.h>

#define LISTENQUE 32

typedef struct{
	struct sockaddr_in addr;
	size_t index;
	char *msg;
}t_client;

int main(int argc, char *argv[])
{
	if (argc != 2) {T return 1;}

	int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (soc_fd < 0) {T return 1;}

	//setsockopt(soc_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);//byteorder(3) ip(7)
	addr.sin_port = htons(atoi(argv[1]));
	if (bind(soc_fd, &addr, sizeof(addr)) == -1) {T return 1;}

	if (listen(soc_fd, LISTENQUE) == -1) {T return 1;}

	t_client *c = malloc(sizeof(t_client));
	struct pollfd * fds  = malloc(sizeof(struct pollfd) * 2);
	size_t pll = 1;
	if (!fds) {T return 1;}
	fds[0].fd = soc_fd;
	fds[0].events = POLLIN;
	//socket(7) SO_KEEPALIVE
	while (1)
	{
		
	}
	
	

}