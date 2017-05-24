#include "server.h"
#include "error.h"
#include "marco.h"



#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>    //sockaddr_in
#include <errno.h>




int init()
{
	int server;
	struct sockaddr_in serv_sock;
	 
	if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("创建socket失败");

	memset(&serv_sock, 0, sizeof(serv_sock));
	serv_sock.sin_family = AF_INET;
	serv_sock.sin_port = htons(PORT);
	serv_sock.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(server, (struct sockaddr*)&serv_sock, sizeof(serv_sock)) < 0) {
		printf("%s\n", strerror(errno));
		err_sys("bind失败");
	}
	if (listen(server, LISTENMAXNUM) < 0)
		err_sys("listen 失败");
	

	return server;
}

void start(int sfd, struct pollfd *fds, nfds_t *size)
{
	printf("等待用户接入\n");
	
	char info[1024];
	for (; ;)
	{
		struct sockaddr_in client;
		socklen_t len;

		int sock = accept(sfd, (struct sockaddr*)&client, &len);
		
		sprintf(info, "来自%s的连接\n", inet_ntoa(client.sin_addr));
		write(STDOUT_FILENO, info, strlen(info) + 1);

		fds[*size].fd = sock;
		fds[*size].events = POLLIN;
		++(*size);
	}
}