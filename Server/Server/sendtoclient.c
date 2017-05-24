#include "sendtoclient.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

void sendMsg(int fd, Msg msg)
{
	char buf[20] = "send msg error\n";
	if (send(fd, (char *)&msg, sizeof(Msg) + msg.len, MSG_DONTWAIT) < 0)
	{
		write(STDOUT_FILENO, buf, strlen(buf) + 1);
	}
}