#ifndef SENDTOCLIENT_H
#define SENDTOCLIENT_H

#include "msgstructure.h"
#include <unistd.h>


//通用发送函数

void sendMsg(int fd, Msg *msg, ssize_t msgsize);


//正常的通讯

void sendResponseLoginMsg(int fd, ResponseLoginMsg *msg);





//发生异常时需要发送的消息




#endif // !SENDTOCLIENT_H

