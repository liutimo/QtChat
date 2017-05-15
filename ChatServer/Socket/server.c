//
// Created by liuzheng on 17-5-14.
//

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

#include "server.h"
#include "socketmarco.h"


//init a sock and return server's fd
int init()
{
    int fd;

    struct sockaddr_in server;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((bind(fd, (struct sockaddr*)&server, sizeof(server))) < 0)
        return -1;


    if ((listen(fd, LISTENNUM)) < 0)
        return -1;

    return fd;
}
//start server
void start(int fd, struct pollfd fds[], nfds_t *size)
{
    struct sockaddr_in client;
    socklen_t len;
    int client_fd;

    while (1)
    {
        if ((client_fd = accept(fd, (struct sockaddr*)&client, &len)) < 0)
            continue;

        fds[*size].fd = client_fd;
        fds[*size].events = (POLLIN | POLLOUT);
        ++(*size);

        printf("connecte from %s\n", inet_ntoa(client.sin_addr));
    }


}
