#ifndef MSGSTRUCTURE_H
#define MSGSTRUCTURE_H

#include "marco.h"

//消息类型
typedef enum _MsgType {
    HEARTBEAT = 0,			//心跳包
    REQUESTLOGIN,			//请求登陆
    RESPONSELOGIN,			//告知客户端登录状态
    RESPONSEFRIENDLIST,     //好友列表
    EXIT,
    REQUESTFORWORDMESSAGE,  //请求服务器转发消息
    RECEIVEDMESSAGE
}MsgType;


//心跳包.  request和response都使用这个结构
typedef struct _HeartBackMsg {
    char status;				//确定为 'a';
}HeartBeatMsg;

//登录状态
typedef enum _LoginStatus {
    LOGINSUCCESS = 0,       //成功登录
    LOGINPWERROR,			//密码错误
    LOGINUNKNOW,		    //未知用户登录
    LOGINREPEAT             //重复登陆
}LoginStatus;

//为了避免粘包，发送这个结构体来告知后面的数据的长度
typedef struct _Resquest {
    long len;
}Request;

typedef struct _Response {
    long len;
}Response;


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

//登录响应消息
typedef struct _ResponseLoginMsg {
    LoginStatus ls;				//登录状态
}ResponseLoginMsg;

//friendlist response
typedef struct _ResponseFriendList {
    int len;
    char friendlist[0];
}ResponseFriendList;

typedef struct _ExitMsg {

}ExitMsg;


typedef struct _RequestForwordMessageMsg {
    char friendid[USERIDMAXLEN];
    char font[30];
    char size[3];
    char color[8];
    long length;
    char message[0];
}RequestForwordMessageMsg;

typedef struct _ReceivedMessageMsg {
    char friendid[USERIDMAXLEN];
    char font[30];
    char size[3];
    char color[8];
    long length;
    char message[0];
}ReceivedMessageMsg;

#endif // MSGSTRUCTURE_H
