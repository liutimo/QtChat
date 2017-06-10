#include "sendtoclient.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void sendMsg(int fd, MsgType msgtype, char *data, ssize_t size)
{
    //Msg *msg = (Msg*)malloc(sizeof(Msg) + size);

    char *buf = malloc(sizeof(char) * (sizeof(Msg) + size));
    Msg *msg = (Msg*)buf;

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
    sendMsg(fd,RESPONSELOGIN, r_msg, sizeof(ResponseLoginMsg));
}

void sendResponseFriendList(int fd, const char *list)
{
    ResponseFriendList *f = (ResponseFriendList*)malloc(sizeof(ResponseFriendList) + strlen(list) + 1);

    f->len = strlen(list) + 1;

    printf("%d\n", f->len);

    memcpy(f->friendlist, list, f->len);

    sendMsg(fd, RESPONSEFRIENDLIST, f, sizeof(ResponseFriendList) + f->len);
}

void sendMessage(int fd, ReceivedMessageMsg *msg)
{
    sendMsg(fd, RECEIVEDMESSAGE, msg, sizeof(ReceivedMessageMsg) + msg->length);
}
