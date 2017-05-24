#include "connecttoserver.h"
#include "marco.h"

#include <QDebug>

ConnectToServer::ConnectToServer(QObject *parent) : QTcpSocket(parent)
{
    connectToHost(IP, PORT);
}

void ConnectToServer::send(Msg *msg, ssize_t length)
{
    Request r;
    r.len = length;

    //首先发送一个包，告知服务器后面发送的数据的长度
    write((char *)&r, sizeof(r));

    write((char *)msg, length);

}

void ConnectToServer::sendLoginMsg(LoginMsg loginmsg)
{
    Msg *msg = (Msg*)new char[sizeof(Msg) + sizeof(LoginMsg)];

    memset(msg, 0, sizeof(Msg) + sizeof(LoginMsg));

    msg->type = REQUESTLOGIN;
    msg->len = sizeof(LoginMsg);

    memcpy(msg->data, &loginmsg, msg->len);

    send(msg, sizeof(Msg) + sizeof(LoginMsg));

    delete msg;
}
