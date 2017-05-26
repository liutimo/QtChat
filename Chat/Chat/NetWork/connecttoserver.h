#ifndef CONNECTTOSERVER_H
#define CONNECTTOSERVER_H

#include <QMutex>
#include <QTcpSocket>

#include "msgstructure.h"

class ConnectToServer : public QTcpSocket
{
    Q_OBJECT
public:
    void send(Msg *msg, ssize_t length);
    void sendLoginMsg(LoginMsg loginmsg);
    static ConnectToServer* getInstance();

protected:
    void recv();

signals:
    void loginStatus(LoginStatus s);

private:
     ConnectToServer(QObject *parent = Q_NULLPTR);

     static ConnectToServer *server;
     static QMutex *mutex;

};

#endif // CONNECTTOSERVER_H
