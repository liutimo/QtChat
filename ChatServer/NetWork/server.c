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
#include <pthread.h>

struct epoll_event ev, events[1024];
int epoll_fd ;
pthread_mutex_t mutex;

int init()
{
    int server;
    struct sockaddr_in serv_sock;

    pthread_mutex_init(&mutex, NULL);

    if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("create socket failed");

    memset(&serv_sock, 0, sizeof(serv_sock));
    serv_sock.sin_family = AF_INET;
    serv_sock.sin_port = htons(PORT);
    serv_sock.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server, (struct sockaddr*)&serv_sock, sizeof(serv_sock)) < 0) {
        printf("%s\n", strerror(errno));
        err_sys("bind failed");
    }
    if (listen(server, LISTENMAXNUM) < 0)
        err_sys("listen failed");


    return server;
}
/*
void start(int sfd, struct pollfd *fds, nfds_t *size)
{
    printf("wait user connect...\n");

    char info[1024];
    for (; ;) {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);

        int sock = accept(sfd, (struct sockaddr*)&client, &len);

        if (sock > 0) {
            sprintf(info, "accept connect from %s\n", inet_ntoa(client.sin_addr));
            write(STDOUT_FILENO, info, strlen(info) + 1);

            fds[*size].fd = sock;
            fds[*size].events = POLLIN;
            ++(*size);
        }
    }
}
*/

void start(int fd)
{
    struct epoll_event ev, events[1024];
    int epoll_fd ;

    struct sockaddr_in client;
    int len;

    epoll_fd = epoll_create(2048);

    ev.data.fd = fd;
    ev.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);

    for (; ;)
    {
        int nfds = epoll_wait(epoll_fd, events, 2048, -1);

        for (int i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == fd)
            {
                //new connect
                int sock = accept(fd, (struct sockaddr*)&client, &len);

                ev.data.fd = sock;
                ev.events = EPOLLIN|EPOLLET;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &ev);
            }
            else if (events[i].events & EPOLLIN)
            {
                // read from client
                recvMsg(events[i].data.fd);
            }

        }
    }
}
