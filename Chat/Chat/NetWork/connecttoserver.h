#ifndef CONNECTTOSERVER_H
#define CONNECTTOSERVER_H

#include <QTcpSocket>

#include "msgstructure.h"

class ConnectToServer : public QTcpSocket
{
    Q_OBJECT
public:
    ConnectToServer(QObject *parent = Q_NULLPTR);
    void send(Msg *msg, ssize_t length);
    void sendLoginMsg(LoginMsg loginmsg);
};

#endif // CONNECTTOSERVER_H
