#ifndef SENDTOCLIENT_H
#define SENDTOCLIENT_H

#include "msgstructure.h"
#include <unistd.h>


//通用发送函数
void sendMsg(int fd, MsgType msgtype, char *data, ssize_t size);

//发送心跳包响应包
void sendResponseHeartBeatMsg(int fd);

//正常的通讯
void sendResponseLoginMsg(int fd, ResponseLoginMsg *msg);





//发生异常时需要发送的消息




#endif // !SENDTOCLIENT_H

