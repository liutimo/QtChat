#ifndef POLLFDLINK_H
#define POLLFDLINK_H

#include <poll.h>
#include <stdbool.h>

typedef struct _pollfdlink
{
	struct pollfd fd;
	struct _pollfdlink *next;
}pollfdlink;


pollfdlink* init_pollfd();

pollfdlink* find_pollfd(pollfdlink *head, int fd);

bool del_pollfd(pollfdlink *head, int fd);

#endif // !POLLFDLINK_H

