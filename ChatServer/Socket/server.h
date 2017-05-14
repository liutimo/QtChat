//
// Created by liuzheng on 17-5-14.
//

#ifndef CHATSERVER_SERVER_H
#define CHATSERVER_SERVER_H


#include <sys/poll.h>

//init a sock and return server's fd
int init();


//start server
void start(int fd, struct pollfd fds[], int *size);



#endif //CHATSERVER_SERVER_H
