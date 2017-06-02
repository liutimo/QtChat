#include "pollfdLink.h"
#include <stdlib.h>

pollfdlink* init_pollfd()
{
	pollfdlink *head = (pollfdlink*)malloc(sizeof(pollfdlink));

	head->next = NULL;

	return head;
}


pollfdlink* find_pollfd(pollfdlink *head, int fd)
{
	pollfdlink *node = head->next;
	
	while (node)
	{
		if (node->fd.fd == fd)
			return node;
		node = node->next;
	}

	return NULL;
}

bool del_pollfd(pollfdlink *head, int fd)
{
	pollfdlink *node = head->next;
	pollfdlink *before = head;
	while (node)
	{
		if (node->fd.fd == fd)
		{
			before = node->next;
			free(node);
			node = NULL;
			return true;
		}
		node = node->next;
		before = before->next;
	}

	return false;
}
