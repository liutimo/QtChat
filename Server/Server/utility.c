#include "utility.h"
#include <errno.h>

ssize_t readn(int filedes, void *buff, ssize_t nbytes)
{
	ssize_t nread;
	ssize_t nleft;
	char *ptr;

	ptr = buff;
	nleft = nbytes;
	while (nleft > 0) {
		if ((nread = read(filedes, ptr, nleft)) < 0) {
			if (errno == EINTR)
				nread = 0; /* and call read() again */
			else
				return (-1);
		}
		else if (nread == 0)
			break; /* EOF */
		nleft -= nread;
		ptr += nread;
	}
	return (nbytes - nleft); /* return >= 0 */
}


ssize_t writen(int filedes, const void *buff, ssize_t nbytes)
{
	ssize_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = buff;
	nleft = nbytes;
	while (nleft > 0) {
		if ((nwritten == write(filedes, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return -1;    //error
		}

		nleft -= nwritten;
		ptr += nwritten;
	}

	return nbytes;
}


ssize_t readline(int filedes, void *buff, ssize_t maxlen)
{

	//do noting !!
	return 0;

}