#ifndef RECVFROMCLIENT_H
#define RECVFROMCLIENT_H
#include "msgstructure.h"

void recvMsg(int fd);

void handleLoginMsg(int fd, Msg *msg);

#endif // !RECVFROMCLIENT_H

