#include "server.h"

#include <stdlib.h>
#include <stdio.h>

void err_sys(const char *error)
{
	fprintf(stderr, "%s\n", error);
	exit(-1);
}