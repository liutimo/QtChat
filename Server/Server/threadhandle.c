#include "threadhandle.h"
#include "recvfromclient.h"
#include "marco.h"

#include <poll.h>
#include <unistd.h>
#include <stdio.h>



extern struct pollfd pollfds[LISTENMAXNUM];
extern nfds_t size;


void *pollhandle(void *arg)
{
	int ret, i;

	printf("开始进入线程\n");

	while (1)
	{
		ret = poll(pollfds, size, 0);
		if (ret > 0) {
			for (int i = 0; i < size; i++) {
				if (pollfds[i].revents & POLLIN) {
					//这里可以考虑使用线程池来完成
					recvMsg(pollfds[i].fd);
				}
			}
		}
	}
}