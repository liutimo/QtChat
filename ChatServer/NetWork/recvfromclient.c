#include "recvfromclient.h"
#include "sendtoclient.h"
#include "Utility/utility.h"
#include "msgstructure.h"
#include "DataBase/database.h"
#include "error.h"
#include "forward.h"
#include "DataStructure/onlinehashtable.h"
#include <sys/epoll.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

extern struct epoll_event ev;
extern int epoll_fd ;

void recvMsg(int fd)
{
    //获取后面一个包的长度
    char *buf = (char *)malloc(sizeof(Request));
    readn(fd, buf, sizeof(Request));

    Request *r = (Request*)buf;
    ssize_t length = r->len;

    free(buf);
    buf = NULL;

    if (length < 0){
        //客户端异常处理
        ev.data.fd = fd;
        ev.events = EPOLLIN|EPOLLET;
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
        printf("client error occur \n");
        close(fd);
        return;
    }

    buf = (char *)malloc(sizeof(char) * length);
    int len = readn(fd, buf, length);
    if (len != length){
        ev.data.fd = fd;
        ev.events = EPOLLIN|EPOLLET;
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
        printf("read data error\n");
        close(fd);
        return;
    }


    Msg *msg = (Msg*)buf;
    printf("Request len = %d, read len = %d from client, msg.len=%d\n", length, len, msg->len);
    switch (msg->type)
    {
    case REQUESTLOGIN:
        handleLoginMsg(fd, msg);
        break;
    case HEARTBEAT:
        handleHeartBeatMsg(fd);
        break;
    case REQUESTFORWORDMESSAGE:
        handleForwordMessageMsg(fd, msg);
        break;
    default:
        break;
    }

    free(buf);
}


void handleLoginMsg(int fd, Msg *msg)
{
    LoginMsg lmsg;
    bool loginSuccess = false;
    memcpy(&lmsg, msg->data, msg->len);

    init_mysql();

    ResponseLoginMsg r_msg;

    printf("user %s logining...\n", lmsg.userid);

    switch (login_check_mysql(lmsg.userid, lmsg.password))
    {
    case DATABASE_USER_PASSWORDERROR:
        r_msg.ls = LOGINPWERROR;
        break;
    case DATABASE_USER_NOTEXIST:
        r_msg.ls = LOGINUNKNOW;
        break;
    case DATABASE_USER_CORRECT:
        loginSuccess = true;
        r_msg.ls = LOGINSUCCESS;
        //维持在线状态需要做的事

        //加入在线队列
        OnlineUser user;
        user.fd = fd;
        strcpy(user.userid, lmsg.userid);
        addOnlineUser(&user);
        break;
    }

    sendResponseLoginMsg(fd, &r_msg);

    if (loginSuccess)
        sendResponseFriendList(fd, get_friendlist_json(lmsg.userid));

    printf("%s\n", get_friendlist_json(lmsg.userid));

    close_mysql();
}

void handleHeartBeatMsg(int fd)
{
    sendResponseHeartBeatMsg(fd);
}

void handleExitMsg(int fd)
{
    ev.data.fd = fd;
    ev.events = EPOLLIN|EPOLLET;
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
    close(fd);
}

void handleForwordMessageMsg(int fd, Msg *msg)
{
    RequestForwordMessageMsg *fmsg = (RequestForwordMessageMsg*)malloc(msg->len * sizeof(char));
    memcpy(fmsg, msg->data, msg->len);


    char *message = (char*)malloc(fmsg->length * sizeof(char) + 1);
    strcpy(message, fmsg->message);
    message[fmsg->length * sizeof(char)] = '\0';
    int friend_fd = findOnlineUserWithUid(fmsg->friendid);

    write(STDOUT_FILENO, fmsg->message, fmsg->length);

    if(friend_fd == -1)
    {
        //store message to database;
    }
    else if(friend_fd > 0)
    {
        //forward
        forwardmessage(fd, friend_fd, message);
    }


    free(message);
    free(fmsg);
}
