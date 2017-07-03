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
#include <fcntl.h>

struct epoll_event ev, events[1024];
int epoll_fd ;
pthread_mutex_t mutex;


void setnonblocking(int sockfd)
{

    int opts;

    opts = fcntl(sockfd, F_GETFL);

    if(opts < 0) {
        perror("fcntl(F_GETFL)\n");

        return;
    }

    opts = (opts | O_NONBLOCK);

    if(fcntl(sockfd, F_SETFL, opts) < 0) {

        perror("fcntl(F_SETFL)\n");

        return;
    }
}
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

    setnonblocking(server);

    return server;
}

void start(int fd)
{
    struct epoll_event ev, events[2048];
    int epoll_fd ;

    struct sockaddr_in client;
    int len;

    epoll_fd = epoll_create(2048);

    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);

    for (; ;)
    { 

        int nfds = epoll_wait(epoll_fd, events, 2048, -1);
        printf("epoll当前可读的fd数目%d\n", nfds);
        for (int i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == fd)
            {
                //new connect
                int sock = accept(fd, (struct sockaddr*)&client, &len);
                setnonblocking(sock);
                ev.data.fd = sock;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &ev);
            }
            else if (events[i].events & EPOLLIN)
            {
                // read from client
//                recvMsg(events[i].data.fd);void recvConnectionMsg(int socketfd, int epfd, struct epoll_event *ev);
                recvConnectionMsg(events[i].data.fd);
            }

        }
    }
}


