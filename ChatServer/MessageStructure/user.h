//
// Created by liuzheng on 17-5-14.
//  this .h create a structure about the user who has login server
//

#ifndef CHATSERVER_USER_H
#define CHATSERVER_USER_H

#include "limits.h"

typedef struct _User {
    char id[USERID_MAXLEN];
    char username[USERNAME_MAXLEN];
    char ip[IP_MAXLEN];
    /* blank */
}User;
#endif //CHATSERVER_USER_H
