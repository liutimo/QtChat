#include "error.h"
#include "forward.h"
#include "recvfromclient.h"
#include "sendtoclient.h"
#include "Utility/utility.h"
#include "msgstructure.h"
#include "DataBase/database.h"
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
    char *buf = (char *)malloc(sizeof(Request));
    readn(fd, buf, sizeof(Request));

    Request *r = (Request*)buf;
    ssize_t length = r->len;

    free(buf);
    buf = NULL;

    if (length < 0){
        //???????
//        e.data.fd = fd;
//        delOvnlineUserWithFd(fd);
//        ev.events = EPOLLIN|EPOLLET;
//        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
        printf("client error occur \n");
//        close(fd);
        return;
    }

    buf = (char *)malloc(sizeof(char) * length);
    int len = readn(fd, buf, length);
    if (len != length){
//        ev.data.fd = fd;
//        delOnlineUserWithFd(fd);
//        ev.events = EPOLLIN|EPOLLET;
//        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
        printf("read data error\n");
//        close(fd);
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
        break;
    case REQUESTFORWORDMESSAGE:
        handleForwordMessageMsg(fd, msg);
        break;
    case REQUESTUSERINFO: {
        init_mysql();
        sendResponseUserInfo(fd, get_userinfo_json(findOnlineUserWithFd(fd)));
        close_mysql();
        break;
    }
    case REQUESTOFFLINEMESSAGE: {
        init_mysql();
        char *json = get_offline_message(findOnlineUserWithFd(fd));
        ResponseOfflineMessage *rom = (ResponseOfflineMessage*)malloc(sizeof(ResponseOfflineMessage) + strlen(json));
        rom->length = strlen(json);
        memcpy(rom->json, json, rom->length);
        sendfflineMessage(fd, rom);
        free(rom);
        free(json);
        close_mysql();
    }
    case RESPONSACKOFFLINEMSG: {
        handleOfflineAckMessage(fd);
        break;
    }
    case REQUESTMOVEFRIENDTOGROUP: {
        RequestMoveFriendToGroup *rmsg = (RequestMoveFriendToGroup*)malloc(sizeof(RequestMoveFriendToGroup));
        memset(rmsg, 0, sizeof(RequestMoveFriendToGroup));
        memcpy(rmsg, msg->data, msg->len);
        handleMoveFriendToGroup(fd, rmsg);
        free(rmsg);
        break;
    }
    case REQUESTUPDATESIGNAURE: {
        RequestUpdateSignature *rmsg = (RequestUpdateSignature*)malloc(msg->len);
        memcpy(rmsg, msg->data, msg->len);
        handleUpdateSignature(fd, rmsg);
        free(rmsg);
        break;
    }
    case REQUESTGROUPINFO: {
        handleRequestGroupMessage(fd);
        break;
    }
    case REQUESTGROUPMEMBERINFO: {
        handleRequestGroupMemberMessage(fd);
        break;
    }
    case REQUESTCHANGESTATUS: {
        handleRequestChangeStatus(fd, msg);
        break;
    }
    case REQUESTFORWARDGROUPMESSAGE: {
        handleFrowardGroupMsg(fd, msg);
    }
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

    ResponseLoginMsg r_msg;

    if(findOnlineUserWithUid(lmsg.userid) != -1 )
    {
        r_msg.ls = LOGINREPEAT;
        sendResponseLoginMsg(fd, &r_msg);
        return;
    }

    init_mysql();
    int status = login_check_mysql(lmsg.userid, lmsg.password);
    close_mysql();

    switch (status)
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
        OnlineUser user;
        user.fd = fd;
        strcpy(user.userid, lmsg.userid);
        addOnlineUser(&user);
        break;
    }

    sendResponseLoginMsg(fd, &r_msg);

    if (loginSuccess)
    {
        init_mysql();
        sendResponseFriendList(fd, get_friendlist_json(lmsg.userid));
        close_mysql();
    }

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

    int friend_fd = findOnlineUserWithUid(fmsg->friendid);


    char *message = (char *)malloc(sizeof(char) *fmsg->length + 1);
    strcpy(message, fmsg->message);
    message[fmsg->length] = '\0';

//    /("%d  |||  %s\n", fmsg->length, message);

    init_mysql();
    set_chatlog(findOnlineUserWithFd(fd), fmsg->friendid, message, fmsg->font, fmsg->size, fmsg->color);
    close_mysql();



    if(friend_fd == -1)
    {
        init_mysql();
        set_offline_message(findOnlineUserWithFd(fd), fmsg->friendid, message, fmsg->font, fmsg->size, fmsg->color);
        close_mysql();
    }
    else if(friend_fd > 0)
    {
        //forward
        forwardmessage(fd, friend_fd, fmsg);
    }
    free(message);
    free(fmsg);
}

void handleOfflineAckMessage(int fd)
{
    init_mysql();

    del_offline_message(findOnlineUserWithFd(fd));

    close_mysql();
}

void handleMoveFriendToGroup(int fd, RequestMoveFriendToGroup *rmsg)
{
    //rmsg->userid, rmsg->grouptype

    init_mysql();
    move_friend_to_group(findOnlineUserWithFd(fd), rmsg->userid, rmsg->grouptype);
    close_mysql();
}

void handleUpdateSignature(int fd, RequestUpdateSignature *msg)
{
    init_mysql();

    char*message = malloc(msg->length + 1);
    strcpy(message, msg->sig);
    message[msg->length] = '\0';
    update_user_signature(findOnlineUserWithFd(fd), message);
    free(message);
    close_mysql();
}

void handleRequestGroupMessage(int fd)
{
    //根据fd找到对应的用户id
    //然后查找数据库找到所有的群

    init_mysql();
    char *group = get_group(findOnlineUserWithFd(fd));
    close_mysql();

    if(group == NULL)
        return;

    ResponseGroupInfo *msg = (ResponseGroupInfo*)malloc(sizeof(ResponseGroupInfo) + strlen(group));
    printf("%s\n", group);
    msg->length = strlen(group);
    strcpy(msg->json, group);

    sendGroupInfo(fd, msg);

    free(msg);
    free(group);
}

void handleRequestGroupMemberMessage(int fd)
{
    init_mysql();
    char *memberinfo = get_group_member(findOnlineUserWithFd(fd));
    close_mysql();

    if(memberinfo == NULL)
        return;

    size_t size = sizeof(ResponseGroupMemberInfo) + strlen(memberinfo) + 1;
    ResponseGroupMemberInfo *info = (ResponseGroupMemberInfo*)malloc(size);

    info->length = strlen(memberinfo) + 1;
    strcpy(info->json, memberinfo);
    info->json[info->length] = '\0';
    sendGroupMemberInfo(fd, info);

    free(info);
    free(memberinfo);
}

void handleRequestChangeStatus(int fd, Msg*msg)
{
    RequestChangeStatus *rcs = (RequestChangeStatus*)malloc(msg->len);
    memcpy(rcs, msg->data, msg->len);

    init_mysql();
    char** friends = get_friends(findOnlineUserWithFd(fd));
    close_mysql();

    ResponseFriendStatusChange *rfsc = (ResponseFriendStatusChange*)malloc(sizeof(ResponseFriendStatusChange));
    strcpy(rfsc->userid, findOnlineUserWithFd(fd));


    OnlineUserNode *node = findWithFd(fd);

    switch (rcs->status) {
    case UserOffLine:                               //离线做离线处理
        node->user.status = UserOffLine;
        ev.data.fd = fd;
        ev.events = EPOLLIN|EPOLLET;
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
        close(fd);

        delOnlineUserWithUid(findOnlineUserWithFd(fd));
        delOnlineUserWithFd(fd);
        break;
    case UserOnLine:                                //用户上线
        rfsc->status = UserOnLine;
        node->user.status = UserOnLine;
        break;
    case UserHide:                                  //用户隐身
        rfsc->status = UserHide;
        node->user.status = UserHide;
        break;
    default:
        break;
    }

    int  i = 0;
    while (friends[i] != NULL) {
        int fd;
        if((fd = findOnlineUserWithUid(friends[i])) != -1)
        {
            sendFriendStatusChange(fd, rfsc);
        }
        ++i;
    }

    free(rfsc);
    free(friends);
}

void handleFrowardGroupMsg(int fd, Msg *msg)
{
    RequestForwordGroupMessage *rmsg = (RequestForwordGroupMessage*)malloc(msg->len);
    memcpy(rmsg, msg->data, msg->len);

    forwardgroupmessage(fd, rmsg);
}






























