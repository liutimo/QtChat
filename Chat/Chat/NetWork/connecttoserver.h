#ifndef CONNECTTOSERVER_H
#define CONNECTTOSERVER_H

#include <QMutex>
#include <QTcpSocket>

#include "msgstructure.h"

class ConnectToServer : public QTcpSocket
{
    Q_OBJECT
public:
    static ConnectToServer* getInstance();
    void closeServer();


    void send(MsgType msgtype, char *data, ssize_t size);
    void sendLoginMsg(LoginMsg *loginmsg);
    void sendHeartBeatMsg(HeartBeatMsg *hearteabtmsg);
    void sendRequestForwordMessageMsg(RequestForwordMessageMsg *msg);
    void sendRequestUserInfoMsg(RequestUserInfoMsg *msg);

protected:
    void recv();

signals:
    void loginStatus(LoginStatus s);
    void responseHeartBeat();
    void responseFriendList(QByteArray);
    void receivedMessage(const QString &msg, const QString &color, const QString &size,  const QString &family);
    void responseUserInfo(QByteArray);

private:
    ConnectToServer(QObject *parent = Q_NULLPTR);

    static ConnectToServer *server;
    static QMutex *mutex;

};

#endif // CONNECTTOSERVER_H