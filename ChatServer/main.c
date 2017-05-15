//
// Created by liuzheng on 17-5-14.
//


#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "Socket/server.h"


#define TRUE 1

struct pollfd pollfds[1024];
nfds_t pollsize = 0;


void *handle(void *arg)
{
    int ret, i, j = 1, len = 0;
    char buf[4096] = "s";

    printf("开entry io thread\n");

    while (j == 1)
    {
        ret = poll(pollfds, pollsize, 0);
        if (ret > 0) {
            for (int i = 0; i < pollsize; i++) {
                if (pollfds[i].revents & POLLIN) {

                    memset(buf, 0, sizeof(buf));
                    len = read(pollfds[i].fd, buf, sizeof(buf));

                    if(strcmp(buf, "q") == 0)
                    {
                        close(pollfds[i].fd);
                        printf("client exit\n");
                    }

                }
                if (pollfds[i].revents & POLLOUT) {
                    int  i = write(pollfds[i].fd, buf, len);
                    if (i < 0)
                        printf("send error\n");
                    len = 0;
                }
            }
        }
    }
    printf("开entry io thread\n");
}


int main()
{

    printf("server starting...\n");

    pthread_t pid;


    int fd = init();

    if(fd == -1)
    {
        printf("server init failed\n");
        exit(-1);
    }

    pthread_create(&pid, NULL, handle, NULL);

    start(fd, pollfds, &pollsize);

    pthread_join(pid, NULL);
}
