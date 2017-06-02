#include "sendtoclient.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void sendMsg(int fd, MsgType msgtype, char *data, ssize_t size)
{
	Msg *msg = (Msg*)malloc(sizeof(Msg) + size);

	msg->type = msgtype;
	msg->len = size;

	memcpy(msg->data, data, msg->len);

	writen(fd, (void*)msg, size + sizeof(Msg));

	free(msg);
}

void sendResponseHeartBeatMsg(int fd)
{
	HeartBeatMsg heartbeatmsg;
	heartbeatmsg.status = 'a';

	sendMsg(fd, HEARTBEAT, &heartbeatmsg, sizeof(HeartBeatMsg));
}

void sendResponseLoginMsg(int fd, ResponseLoginMsg *r_msg)
{
	sendMsg(fd,RESPONSELOGIN, r_msg, sizeof(Msg) + sizeof(ResponseLoginMsg));
}