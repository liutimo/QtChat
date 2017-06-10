/*
	服务器的基本架构
*/

#ifndef SERVER_H
#define SERVER_H

#include <poll.h>
#include <sys/epoll.h>
//返回创建的服务器的套接字
int init();


//int poll(struct pollfd fdarray[], nfds_t nfds, int timeout);
//成功返回准备就绪的描述符数量。超时返回0， 出错返回 - 1.
//开始接受连接，并且进行IO复用
//void start(int fd, struct pollfd *fds, nfds_t *size);

void start(int fd);

#endif // !SERVER_H

