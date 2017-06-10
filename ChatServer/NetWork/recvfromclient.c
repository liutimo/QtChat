#include "recvfromclient.h"
#include "sendtoclient.h"
#include "Utility/utility.h"
#include "msgstructure.h"
#include "DataBase/database.h"
#include "error.h"
#include "DataStructure/onlinehashtable.h"
#include <sys/epoll.h>
#include <stdio.h>
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
        printf("client error occur \n");
        close(fd);
        return;
    }
    Msg *msg = (Msg*)buf;

    switch (msg->type)
    {
    case REQUESTLOGIN:
        handleLoginMsg(fd, msg);
        break;
    case HEARTBEAT:
        handleHeartBeatMsg(fd);
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
