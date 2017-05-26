#include "connecttoserver.h"
#include "marco.h"

#include <QDebug>

ConnectToServer::ConnectToServer(QObject *parent) : QTcpSocket(parent)
{
    connectToHost(IP, PORT);

    connect(this, &ConnectToServer::readyRead, this, &ConnectToServer::recv);
    connect(this, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
          [=](QAbstractSocket::SocketError socketError){ qDebug() << socketError; });
}

ConnectToServer* ConnectToServer::server = NULL;
QMutex* ConnectToServer::mutex = new QMutex();

//单例模式
ConnectToServer* ConnectToServer::getInstance()
{
    mutex->lock();

    if(server == NULL)
        server = new ConnectToServer();

    mutex->unlock();

    return server;
}


void ConnectToServer::send(Msg *msg, ssize_t length)
{
    Request r;
    r.len = length;

    //首先发送一个包，告知服务器后面发送的数据的长度
    if( -1 == write((char *)&r, sizeof(r)))
        qDebug() << "发送消息失败";
    if (-1 == write((char *)msg, length))
         qDebug() << "发送消息失败";

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




/*****************************接受服务器发来的消息**************************************/

void ConnectToServer::recv()
{
    QByteArray bytearray = this->readAll();
    Msg *msg = (Msg*)bytearray.data();

    qDebug() << "收到服务器消息";
    switch (msg->type) {
    case RESPONSELOGIN: {
        ResponseLoginMsg *rlm = new ResponseLoginMsg;
        memcpy(rlm, msg->data, sizeof(ResponseLoginMsg));
        emit loginStatus(rlm->ls);
        delete rlm;
        }
        break;
    default:
        break;
    }

    delete msg;
}
