#ifndef RECVFROMCLIENT_H
#define RECVFROMCLIENT_H
#include "msgstructure.h"

void recvMsg(int fd);

void handleLoginMsg(int fd, Msg *msg);

void handleHeartBeatMsg(int fd);

void handleExitMsg(int fd);

void handleForwordMessageMsg(int fd, Msg *msg);

void handleOfflineAckMessage(int fd);
#endif // !RECVFROMCLIENT_H

