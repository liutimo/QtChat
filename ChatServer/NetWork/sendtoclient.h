#ifndef SENDTOCLIENT_H
#define SENDTOCLIENT_H

#include "msgstructure.h"
#include <unistd.h>
#include <pthread.h>


//通用发送函数
void sendMsg(int fd, MsgType msgtype, char *data, ssize_t size);

//发送心跳包响应包
void sendResponseHeartBeatMsg(int fd);

//正常的通讯

void sendResponseLoginMsg(int fd, ResponseLoginMsg *msg);


void sendResponseUserInfo(int fd, const char *);

//send friendlist
void sendResponseFriendList(int fd, const char*);

//sendMessage
void sendMessage(int fd, ReceivedMessageMsg *msg);

//发送离线消息给用户
void sendfflineMessage(int fd, ResponseOfflineMessage *msg);

//发生异常时需要发送的消息






#endif // !SENDTOCLIENT_H

