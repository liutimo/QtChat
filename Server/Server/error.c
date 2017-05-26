#include "server.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

extern int fd;

void err_sys(const char *error)
{
	fprintf(stderr, "%s\n", error);

	/*ÇåÀí*/
	if(fd > 0)
		close(fd);

	exit(-1);
}