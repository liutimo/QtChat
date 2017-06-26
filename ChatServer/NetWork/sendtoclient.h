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

//发送群组信息
void sendGroupInfo(int fd, ResponseGroupInfo *msg);

//发送群组成员信息
void sendGroupMemberInfo(int fd, ResponseGroupMemberInfo *msg);

//发送好友状态改变信息
void sendFriendStatusChange(int fd, ResponseFriendStatusChange *msg);


//发送群组消息
void sendGroupMessage(const int fd, RequestForwordGroupMessage* rmsg);

//发生异常时需要发送的消息





#endif // !SENDTOCLIENT_H

