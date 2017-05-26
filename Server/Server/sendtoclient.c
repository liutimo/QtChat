#include "sendtoclient.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void sendMsg(int fd, Msg *msg, ssize_t msgsize)
{
	writen(fd, (void*)msg, msgsize);
}


void sendResponseLoginMsg(int fd, ResponseLoginMsg *r_msg)
{
	Msg *msg = (Msg*)malloc(sizeof(Msg) + sizeof(ResponseLoginMsg));

	msg->type = RESPONSELOGIN;
	msg->len = sizeof(ResponseLoginMsg);

	memcpy(msg->data, (char*)r_msg, msg->len);

	sendMsg(fd, msg, sizeof(Msg) + msg->len);

	free(msg);
}