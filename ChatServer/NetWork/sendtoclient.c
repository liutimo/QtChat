#include "sendtoclient.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

extern pthread_mutex_t mutex;

void sendMsg(int fd, MsgType msgtype, char *data, ssize_t size)
{
    pthread_mutex_lock(&mutex);
    char *buf = malloc(sizeof(char) * (sizeof(Msg) + size));
    Msg *msg = (Msg*)buf;
	msg->type = msgtype;
	msg->len = size;
	memcpy(msg->data, data, msg->len);

    int n = writen(fd, (void*)msg, size + sizeof(Msg));

    if(n==size + sizeof(Msg))
        printf("send data len = %d\n",  size + sizeof(Msg));


	free(msg);
    pthread_mutex_unlock(&mutex);
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

void sendResponseUserInfo(int fd, const char *userinfo)
{
    ResponseUserinfo *f = (ResponseUserinfo*)malloc(sizeof(ResponseUserinfo) + strlen(userinfo) + 1);
    f->length = strlen(userinfo) + 1;

    memcpy(f->userinfo, userinfo, f->length);

    sendMsg(fd, RESPONSEUSERINFO, f, sizeof(ResponseUserinfo) + f->length);
}

void sendResponseFriendList(int fd, const char *list)
{
    ResponseFriendList *f = (ResponseFriendList*)malloc(sizeof(ResponseFriendList) + strlen(list) + 1);
    f->len = strlen(list) + 1;

    memcpy(f->friendlist, list, f->len);

    sendMsg(fd, RESPONSEFRIENDLIST, f, sizeof(ResponseFriendList) + f->len);
}

void sendMessage(int fd, ReceivedMessageMsg *msg)
{
    sendMsg(fd, RECEIVEDMESSAGE, msg, sizeof(ReceivedMessageMsg) + msg->length);
}

//发送离线消息给用户
void sendfflineMessage(int fd, ResponseOfflineMessage *msg)
{
    sendMsg(fd, RESPONSEOFFLINEMESSAGE, msg, msg->length + sizeof(ResponseOfflineMessage));
}

void sendGroupInfo(int fd, ResponseGroupInfo *msg)
{
    sendMsg(fd, RESPONSEGROUPINFO, msg, msg->length + sizeof(ResponseGroupInfo));
}
void sendGroupMemberInfo(int fd, ResponseGroupMemberInfo *msg)
{
    sendMsg(fd, RESPONSEGROUPMEMBERINFO, msg, msg->length + sizeof(ResponseGroupMemberInfo));
}

void sendFriendStatusChange(int fd, ResponseFriendStatusChange *msg)
{
    sendMsg(fd, RESPONSEFRIENDSTATUSCHANGE, msg, sizeof(ResponseFriendStatusChange));
}
