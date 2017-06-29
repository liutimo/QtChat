
#include "connecttoserver.h"
#include "marco.h"

#include <QDebug>
#include <QThread>
ConnectToServer::ConnectToServer(QObject *parent) : QTcpSocket(parent)
{
    connectToHost(IP, PORT);

    connect(this, &ConnectToServer::readyRead, this, &ConnectToServer::recv);
}

ConnectToServer* ConnectToServer::server = NULL;
QMutex* ConnectToServer::mutex = new QMutex();

//??????
ConnectToServer* ConnectToServer::getInstance()
{
    mutex->lock();

    if(server == NULL || !server->isValid())
        server = new ConnectToServer();

    mutex->unlock();

    return server;
}

void ConnectToServer::closeServer()
{
    this->close();
    server = NULL;
}


void ConnectToServer::send(MsgType msgtype, char *data, ssize_t size)
{
//    mutex->lock();
    int memsize = (sizeof(Msg) + size) *sizeof(char);
    char *buf = new char[memsize];
    Msg *msg = (Msg*)buf;

    memset(msg, 0, memsize);
    msg->m_uiCheckCrc = 0xAFAFAFAF;
    msg->type = msgtype;
    msg->len = size;

    memcpy(msg->data, data, msg->len);

    Request r;
    r.len = memsize;


    if( -1 == write((char *)&r, sizeof(r))) {
        qDebug() << "发送 Request包失败";
        delete []buf;
        return;
    }

    if (-1 == write((char *)msg, r.len))
        qDebug() << "发送 Msg包失败";

    delete []buf;

//    mutex->unlock();
}

void ConnectToServer::sendLoginMsg(LoginMsg *loginmsg)
{
    send(REQUESTLOGIN, (char*)loginmsg, sizeof(LoginMsg));
}

void ConnectToServer::sendHeartBeatMsg(HeartBeatMsg *hearteabtmsg)
{
    send(HEARTBEAT, (char*)hearteabtmsg, sizeof(HeartBeatMsg));
}

void ConnectToServer::sendRequestForwordMessageMsg(RequestForwordMessageMsg *msg)
{
    send(REQUESTFORWORDMESSAGE, (char *)msg, sizeof(RequestForwordMessageMsg) + msg->length);
}

void ConnectToServer::sendRequestUserInfoMsg(RequestUserInfoMsg *msg)
{
    send(REQUESTUSERINFO, (char *)msg, sizeof(RequestUserInfoMsg));
}

void ConnectToServer::sendRequestOfflineMessage()
{
    RequestOfflineMessage *msg = new  RequestOfflineMessage;

    send(REQUESTOFFLINEMESSAGE, (char*)msg, sizeof(RequestOfflineMessage));

    delete msg;
}

void ConnectToServer::sendRequestAckOfflineMsg()
{
    ResponseAckOfflineMsg *msg = new  ResponseAckOfflineMsg;

    send(RESPONSACKOFFLINEMSG, (char*)msg, sizeof(ResponseAckOfflineMsg));

    delete msg;
}

void ConnectToServer::sendRequestMoveFriendToGroup(const QString&userid,const QString &grouptype)
{
    RequestMoveFriendToGroup *msg = new RequestMoveFriendToGroup;
    strcpy(msg->userid, userid.toUtf8().data());
    strcpy(msg->grouptype, grouptype.toUtf8().data());

    send(REQUESTMOVEFRIENDTOGROUP, (char*)msg, sizeof(RequestMoveFriendToGroup));
    delete msg;
}
void ConnectToServer::sendRequestUpdateSignature(const QString &signature)
{
    RequestUpdateSignature *msg = (RequestUpdateSignature *)new char[sizeof(RequestUpdateSignature) + signature.toUtf8().size()];
    msg->length = signature.toUtf8().size();
    strcpy(msg->sig, signature.toUtf8());

    send(REQUESTUPDATESIGNAURE, (char*)msg, sizeof(RequestUpdateSignature) + msg->length);
    delete msg;
}

void ConnectToServer::sendRequestGroupInfo()
{
    RequestGroupInfo *msg = (RequestGroupInfo*)new char[sizeof(RequestGroupInfo)];
    send(REQUESTGROUPINFO, (char*)msg, sizeof(RequestGroupInfo));
    delete msg;
}

void ConnectToServer::sendRequestGroupMemberInfo()
{
    RequestGroupMemberInfo *msg = new RequestGroupMemberInfo;
    send(REQUESTGROUPMEMBERINFO, (char*)msg, sizeof(RequestGroupMemberInfo));
    delete msg;
}

void ConnectToServer::sendRequestChangeStatus(int status)
{
    RequestChangeStatus *s = new RequestChangeStatus;
    s->status = status;

    send(REQUESTCHANGESTATUS, (char*)s, sizeof(RequestChangeStatus));

    delete s;
}

void ConnectToServer::sendForwordGroupMessage(ForwordGroupMessage *msg)
{
    send(REQUESTFORWARDGROUPMESSAGE, (char*)msg, msg->length + sizeof(ForwordGroupMessage));
}

void ConnectToServer::sendRequestExitMessage()
{
    RequestExit *msg = new RequestExit;
    send(EXIT, (char*)msg, sizeof(RequestExit));
    delete msg;
}

/*****************************???????????????**************************************/

void ConnectToServer::recv()
{
    QByteArray bytearray = this->readAll();
    Msg *msg = (Msg*)bytearray.data();

    switch (msg->type) {
    case RESPONSELOGIN: {
        qDebug() << "登陆回应";
        ResponseLoginMsg *rlm = new ResponseLoginMsg;
        memcpy(rlm, msg->data, sizeof(ResponseLoginMsg));
        emit loginStatus(rlm->ls);
        delete rlm;
        break;
    }
    case HEARTBEAT: {
        qDebug() << "心跳回应";
        emit responseHeartBeat();
        break;
    }
    case RESPONSEFRIENDLIST: {
        qDebug() << "好友列表回应";
        char *buf = new char[msg->len];
        ResponseFriendList *rf = (ResponseFriendList*)buf;
        memcpy(rf, msg->data, msg->len);
        emit responseFriendList(QByteArray(rf->friendlist));
        delete []buf;
        break;
    }
    case RESPONSEUSERINFO: {
        qDebug() << "用户信息回应";
        ResponseUserinfo *rui = (ResponseUserinfo*)new char[msg->len];
        memcpy(rui, msg->data, msg->len);
        emit responseUserInfo(QByteArray(rui->userinfo));
        delete rui;
        break;
    }
    case RECEIVEDMESSAGE: {
        qDebug() << "消息回应";
        ReceivedMessageMsg *rmsg = (ReceivedMessageMsg*)new char[msg->len];
        memcpy(rmsg, msg->data, msg->len);
        emit receivedMessage(rmsg);
        break;
    }
    case RESPONSEOFFLINEMESSAGE: {
        qDebug() << "离线消息回应";
        ResponseOfflineMessage *rom = (ResponseOfflineMessage*)new char[msg->len + 1];
        memcpy(rom, msg->data, msg->len);
        rom->json[rom->length] = '\0';
        emit receivedOfflineMessage(QByteArray(rom->json));
        delete rom;
        break;
    }
    case RESPONSEGROUPINFO: {
        qDebug() << "群组信息回应";
        ResponseGroupInfo *rgi = (ResponseGroupInfo*)new char[msg->len];
        memcpy(rgi, msg->data, msg->len);
        emit receivedGroupInfo(QByteArray(rgi->json));
        delete rgi;
        break;
    }
    case RESPONSEGROUPMEMBERINFO: {
        qDebug() << "群成员信息回应";
        ResponseGroupMemberInfo *info = (ResponseGroupMemberInfo*)new char[msg->len];
        memcpy(info,msg->data, msg->len);
        emit receivedGroupMemberInfo(info->json);
        delete info;
        break;
    }
    case RESPONSEFRIENDSTATUSCHANGE: {
        qDebug() << "状态改变回应";
        ResponseFriendStatusChange *rfsc = new ResponseFriendStatusChange;
        memcpy(rfsc, msg->data, msg->len);
        qDebug() << rfsc->status;
        emit friendStatusChange(rfsc->userid, rfsc->status == 1 ? 1 : 2);
        delete rfsc;
        break;
    }
    case REQUESTFORWARDGROUPMESSAGE: {
        qDebug() << "群消息回应";
        ForwordGroupMessage *rmsg = (ForwordGroupMessage*)new char[msg->len];
        memcpy(rmsg,msg->data, msg->len);
        emit receivedGroupMessage(rmsg);
        break;
    }
    default:
        qDebug() << "未知";
        break;
    }
}
