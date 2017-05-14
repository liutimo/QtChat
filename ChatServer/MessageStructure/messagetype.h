//
// Created by liuzheng on 17-5-14.
//

#ifndef CHATSERVER_MESSAGETYPE_H
#define CHATSERVER_MESSAGETYPE_H

#include <stddef.h>
#include "limits.h"

typedef enum _MessageType {
    LZ_LOGIN = 0    //登录 热切申请request login
}MessageType;



typedef struct _Msg {
    MessageType msgtype;
    size_t len;
    char data[0];
}Msg;


//the client send this structure when he want to login the server
typedef struct _UserLogin {
    char ip[IP_MAXLEN];                         //the user's IP address
    char username[USERNAME_MAXLEN];         //the username
    char userpw[USERPW_MAXLEN];
    /*status   online or other*/
}UserLogin;



#endif //CHATSERVER_MESSAGETYPE_H
