#include "sendtoclient.h"
#include "DataStructure/onlinehashtable.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

extern pthread_mutex_t mutex;

void sendMsg(int fd, MsgType msgtype, char *data, ssize_t size)
{
    pthread_mutex_lock(&mutex);
    char *buf = malloc((sizeof(Msg) + size));
    Msg *msg = (Msg*)buf;
    msg->m_uiCheckCrc = 0xAFAFAFAF;
    msg->type = msgtype;
    msg->len = size;
    memcpy(msg->data, data, msg->len);

    int n = writen(fd, (void*)msg, size + sizeof(Msg));

    if(n == 0){
        close(fd);
        free(msg);
        pthread_mutex_unlock(&mutex);
        return;
    }

    printf("成功发送%d字节数据\n", size + sizeof(Msg));
    free(msg);
    pthread_mutex_unlock(&mutex);
}

void sendResponseHeartBeatMsg(int fd)
{
    printf("发送心跳包\n");
    HeartBeatMsg heartbeatmsg;
    heartbeatmsg.status = 'a';
    sendMsg(fd, HEARTBEAT, &heartbeatmsg, sizeof(HeartBeatMsg));
}

void sendResponseLoginMsg(int fd, ResponseLoginMsg *r_msg)
{
    printf("发送登陆响应消息\n");
    sendMsg(fd,RESPONSELOGIN, r_msg, sizeof(ResponseLoginMsg));
}

void sendResponseUserInfo(int fd, const char *userinfo)
{
    ResponseUserinfo *f = (ResponseUserinfo*)malloc(sizeof(ResponseUserinfo) + strlen(userinfo) + 1);
    f->length = strlen(userinfo) + 1;

    memcpy(f->userinfo, userinfo, f->length);

    sendMsg(fd, RESPONSEUSERINFO, f, sizeof(ResponseUserinfo) + f->length);

    printf("发送当前登陆用户信息\n");
    free(userinfo);
}

void sendResponseFriendList(int fd, const char *list)
{
    ssize_t length = sizeof(ResponseFriendList) + strlen(list) + 1;
    ResponseFriendList *f = (ResponseFriendList*)malloc(length);
    bzero(f, length);
    f->len = strlen(list) + 1;

    strcpy(f->friendlist, list);
    f->friendlist[strlen(list)] = '\0';

    printf("发送好友列表\n");
    sendMsg(fd, RESPONSEFRIENDLIST, f, sizeof(ResponseFriendList) + f->len);
}

void sendMessage(int fd, ReceivedMessageMsg *msg)
{
    printf("发送好友消息\n");
    sendMsg(fd, RECEIVEDMESSAGE, msg, sizeof(ReceivedMessageMsg) + msg->length);
}

//发送离线消息给用户
void sendfflineMessage(int fd, ResponseOfflineMessage *msg)
{
    printf("发送好友离线消息\n");
    sendMsg(fd, RESPONSEOFFLINEMESSAGE, msg, msg->length + sizeof(ResponseOfflineMessage));
}

void sendGroupInfo(int fd, ResponseGroupInfo *msg)
{
    printf("发送群信息\n");
    sendMsg(fd, RESPONSEGROUPINFO, msg, msg->length + sizeof(ResponseGroupInfo));
}
void sendGroupMemberInfo(int fd, ResponseGroupMemberInfo *msg)
{
    printf("发送群成员信息\n");
    sendMsg(fd, RESPONSEGROUPMEMBERINFO, msg, msg->length + sizeof(ResponseGroupMemberInfo));
}

void sendFriendStatusChange(int fd, ResponseFriendStatusChange *msg)
{
    printf("发送好友状态改变信息\n");
    sendMsg(fd, RESPONSEFRIENDSTATUSCHANGE, msg, sizeof(ResponseFriendStatusChange));
}

//发送群组消息
void sendGroupMessage(const int fd, RequestForwordGroupMessage* rmsg)
{
    printf("发送群消息\n");
    sendMsg(fd, REQUESTFORWARDGROUPMESSAGE, rmsg, rmsg->length + sizeof(RequestForwordGroupMessage) + 1);
}

void sendSearchResult(const int fd, ResponseSearchFriend *rmsg)
{
    printf("发送搜索结果\n");
    sendMsg(fd, RESPONSESEARCHFRIEND, rmsg, rmsg->length + sizeof(ResponseSearchFriend));
}

void sendFrowardAddFriendAck(const int fd, ForwardAddFriendAck *rmsg)
{
    printf("转发添加好友请求\n");
    sendMsg(fd, FORWARDADDFRIENDACK, rmsg, rmsg->length + sizeof(ForwardAddFriendAck));
}

void sendGroupOfflineMessage(const int fd, ResponseGroupOfflineMessage *msg)
{
    printf("发送群组离线消息\n");
    sendMsg(fd, RESPONSEGROUPOFFLINEMESSAGEMSG, msg, msg->length + sizeof(ResponseGroupOfflineMessage));
}
