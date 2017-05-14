//
// Created by liuzheng on 17-5-14.
//


#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "Socket/server.h"




struct pollfd pollfds[1024];
int pollsize = 0;


void *handle(void *arg)
{
    int ret, i, len = 0;
    char buf[4096];
    struct timeval t;
    t.tv_sec = 1;
    printf("开entry io thread\n");

    while (1)
    {
        ret = poll(pollfds, pollsize, 0);
        if (ret > 0) {
            for (int i = 0; i < pollsize; i++) {
                if (pollfds[i].revents & POLLIN) {
                    len = read(pollfds[i].fd, buf, sizeof(buf));
                    write(STDOUT_FILENO, buf, len);
                }

                if (pollfds[i].revents & POLLOUT) {
                    write(pollfds[i].fd, buf, len);
                }
            }
        }
    }
}


int main()
{

    printf("server starting...\n");

    int fd = init();
    pthread_t pid;

    if(fd == -1)
    {
        printf("server init failed\n");
        exit(-1);
    }

    pthread_create(&pid, NULL, handle, NULL);

    start(fd, pollfds, &pollsize);

    pthread_join(pid, NULL);
}
