#ifndef MSGSTRUCTURE_H
#define MSGSTRUCTURE_H

#include "marco.h"

//消息类型

typedef enum _MsgType {
    HEARTBEAT = 0,      //心跳包
    REQUESTLOGIN        //请求登陆
}MsgType;

//为了避免粘包，发送这个结构体来告知后面的数据的长度
typedef struct _Resquest {
    long len;
}Request;


//通用消息结构
typedef struct _Msg {
    MsgType type;   //消息类型
    int len;        //消息长度
    char data[0];   //消息
}Msg;


//登陆消息
typedef struct _LoginMsg {
    char userid[USERIDMAXLEN];
    char password[PASSWORDMAXLEN];
}LoginMsg;

#endif // MSGSTRUCTURE_H
