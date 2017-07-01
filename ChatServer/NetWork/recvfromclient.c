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
#include <errno.h>


extern struct epoll_event ev;
extern int epoll_fd ;

#define RECV_BUF_MAX_SIZE 4096

char* memstr(char* full_data, int full_data_len, char* substr)
{
    if (full_data == NULL || full_data_len <= 0 || substr == NULL) {
        return NULL;
    }

    if (*substr == '\0') {
        return NULL;
    }

    int sublen = strlen(substr);

    int i;
    char* cur = full_data;
    int last_possible = full_data_len - sublen + 1;
    for (i = 0; i < last_possible; i++) {
        if (*cur == *substr) {
            //assert(full_data_len - i >= sublen);
            if (memcmp(cur, substr, sublen) == 0) {
                //found
                return cur;
            }
        }
        cur++;
    }

    return NULL;
}

void recvConnectionMsg(int socketfd)
{
    Msg *msg = NULL, *sendMsg = NULL;
    //接收缓存
    char *recvBuf = (char *)malloc((2 * RECV_BUF_MAX_SIZE + 1) * sizeof(char));
    memset(recvBuf, 0, 2 * RECV_BUF_MAX_SIZE * sizeof(char));
    //recvBuf尾部指针，用于接收数据
    char *pRearBuf = recvBuf;
    //recvBuf头部指针，用于传递数据
    char *pHeadBuf = recvBuf;
    //实际接收到得字节数量
    int recvRet = 0;
    //消息长度
    int msgLen = 0;
    int clientSocketfd = socketfd;
    int msgStructLen = sizeof(Msg);
    //发送的数据包不会超过RECV_BUF_MAX_SIZE
    //如果超过RECV_BUF_MAX_SIZE可能出现了TCP
    //粘包现象
    while(recvRet = recv(clientSocketfd, pRearBuf, RECV_BUF_MAX_SIZE, 0))
    {
        //读数据错误
        if(recvRet < 0)
        {
            if(errno == EAGAIN)
            {
                //这次没有数据了，下次再来
                break;
            }
            else
            {
                ev.data.fd = clientSocketfd;
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clientSocketfd, &ev);
                close(clientSocketfd);
                break;
            }
        }
        msgLen += recvRet;
        if (msgLen > 2 * RECV_BUF_MAX_SIZE)
        {
            //消息长度大于缓存长度
            exit(-1);
        }
        //接收到总数据已经大于了RECV_BUF_MAX_SIZE
        //为了安全做一定处理
        if(msgLen > RECV_BUF_MAX_SIZE)
        {
            unsigned char crc[5];
            memset(crc, 0xAF, 4);
            //在收到得buf中查找0xAFAFAFAF标志位
            void *findMove = memstr(pHeadBuf, msgLen, (void *)crc);
            if(findMove)
            {
                int move = (char *)findMove - recvBuf;
                msgLen -= move;
                memcpy(recvBuf, findMove, msgLen);
                pHeadBuf = recvBuf;
                pRearBuf += msgLen;
            }
            else
            {

                break;
            }
        }
        //如果收到包长度小于结构体长度，暂定为丢弃
        //实际可能会出现拆包情况，收到小于包长度得
        //数据包
        if(msgLen < msgStructLen)
        {
            //指正移到缓存后
            pRearBuf += msgLen;
            continue;
        }
        recvBuf[msgLen] = '\0';
        //暂时没有想到好的解决方法
        //当且仅当发生TCP粘包时会执行这个loop
        //其它情况都无视这个loop
stickyPackageLoop:
        //翻译buf
        msg = (Msg *)pHeadBuf;
        //校验位是否正确，如果正确则执行下一步
        if(msg->m_uiCheckCrc != (unsigned int)0xAFAFAFAF)
        {
            //矫正
            //尽量校正，校正成功则继续
            //否则continue，直到这个数据包被放弃
            unsigned char crc[5];
            memset(crc, 0xAF, 4);
            //在收到得buf中查找0xAFAFAFAF标志位
            void *findMove = memstr(pHeadBuf, msgLen, (void *)crc);
            if(findMove)
            {
                //找到标志位
                pHeadBuf = (char *)findMove;
                msgLen -= (pRearBuf - pHeadBuf);
                //重新翻译buf
                msg = (Msg *)pHeadBuf;
            }
            else
            {
                //没有找到标志位
                continue;
            }
        }

        if(msgLen < msgStructLen + msg->len)
        {
            //拆包
            pRearBuf += msgLen;
            continue;
        }
        //投递数据包
        //现在消息是异步的，所以需要malloc一段空间用来存放msg消息
        sendMsg = (Msg *)malloc(msgStructLen + msg->len);
        memcpy(sendMsg, msg, msgStructLen + msg->len);
        handle(sendMsg, clientSocketfd);
        sendMsg = NULL;
        msgLen -= msgStructLen + msg->len;
        if(msgLen > 0)
        {
            //黏包
            pHeadBuf = pHeadBuf + msgStructLen + msg->len;
            goto stickyPackageLoop;
        }
        //一轮结束pRearBuf和pHeadBuf指针重新指向recvBuf
        pRearBuf = recvBuf;
        pHeadBuf = recvBuf;
    }
    free(recvBuf);
}



void recvMsg(int fd)
{
    ssize_t size = sizeof(Request);

    while(1)
    {
        char *buf = (char *)malloc(sizeof(Request));
        int flag = read(fd, buf, size);
        if(flag < 0)
        {
            if(errno == EAGAIN)
            {
                //无数据可读
                free(buf);
                break;
            }
            else
                return;
        }
        else
        {
            Request *r = (Request*)buf;
            ssize_t length = r->len;

            free(buf);

            buf = (char*)malloc(length);

            int flag2 = read(fd, buf, length);

            if(flag2 < 0)
            {
                if(errno == EAGAIN)
                {
                    //无数据可读
                    free(buf);
                    break;
                }
            }
            else
            {
                Msg *msg = (Msg*)buf;

                handle(msg, fd);

                free(msg);
            }
        }
    }

}

void handle(Msg *msg, int fd)
{
    switch (msg->type)
    {

    case REQUESTLOGIN:
        printf("REQUESTLOGIN\n");
        handleLoginMsg(fd, msg);
        break;
    case HEARTBEAT:
        printf("HEARTBEAT\n");
        handleHeartBeatMsg(fd);
        break;
    case REQUESTFORWORDMESSAGE:
        printf("REQUESTFORWORDMESSAGE\n");
        handleForwordMessageMsg(fd, msg);
        break;
    case REQUESTUSERINFO: {
        printf("REQUESTUSERINFO\n");
        init_mysql();
        sendResponseUserInfo(fd, get_userinfo_json(findOnlineUserWithFd(fd)));
        close_mysql();
        break;
    }
    case REQUESTOFFLINEMESSAGE: {
        printf("REQUESTOFFLINEMESSAGE\n");
        init_mysql();
        char *json = get_offline_message(findOnlineUserWithFd(fd));
        close_mysql();
        ResponseOfflineMessage *rom = (ResponseOfflineMessage*)malloc(sizeof(ResponseOfflineMessage) + strlen(json));
        rom->length = strlen(json);
        strcpy(rom->json, json);
        sendfflineMessage(fd, rom);
        free(rom);
        free(json);
        break;
    }
    case RESPONSACKOFFLINEMSG: {
        printf("RESPONSACKOFFLINEMSG\n");
        handleOfflineAckMessage(fd);
        break;
    }
    case REQUESTMOVEFRIENDTOGROUP: {
        printf("REQUESTMOVEFRIENDTOGROUP\n");
        RequestMoveFriendToGroup *rmsg = (RequestMoveFriendToGroup*)malloc(sizeof(RequestMoveFriendToGroup));
        memset(rmsg, 0, sizeof(RequestMoveFriendToGroup));
        memcpy(rmsg, msg->data, msg->len);
        handleMoveFriendToGroup(fd, rmsg);
        free(rmsg);
        break;
    }
    case REQUESTUPDATESIGNAURE: {
        printf("REQUESTUPDATESIGNAURE\n");
        RequestUpdateSignature *rmsg = (RequestUpdateSignature*)malloc(msg->len);
        memcpy(rmsg, msg->data, msg->len);
        handleUpdateSignature(fd, rmsg);
        free(rmsg);
        break;
    }
    case REQUESTGROUPINFO: {
        printf("REQUESTGROUPINFO\n");
        handleRequestGroupMessage(fd);
        break;
    }
    case REQUESTGROUPMEMBERINFO: {
        printf("REQUESTGROUPMEMBERINFO\n");
        handleRequestGroupMemberMessage(fd);
        break;
    }
    case REQUESTCHANGESTATUS: {
        printf("REQUESTCHANGESTATUS\n");
        handleRequestChangeStatus(fd, msg);
        break;
    }
    case REQUESTFORWARDGROUPMESSAGE: {
        printf("REQUESTFORWARDGROUPMESSAGE\n");
        handleFrowardGroupMsg(fd, msg);
        break;
    }
    case EXIT: {
        printf("EXIT\n");
        handleExitMsg(fd);
        break;
    }
    case REQUESTDELETEFRIEND: {
        printf("REQUESTDELETEFRIEND\n");
        handleDeleteFriendMsg(fd, msg);
        break;
    }
    case REQUESTCREATEGROUP:{
        printf("REQUESTCREATEGROUP\n");
        handleCreateGroupMsg(fd, msg);
        break;
    }
    case REQUESTSEARCHFRIEND: {
        printf("REQUESTSEARCHFRIEND\n");
        handleSearchFriendMsg(fd, msg);
        break;
    }
    case REQUESTADDFRIENDACK: {
        printf("REQUESTADDFRIENDACK\n");
        handleAddFriendMsg(fd, msg);
        break;
    }
    case ADDFRIENDSTATUS: {
        printf("ADDFRIENDSTATUS\n");
        handleAddFriendAckMsg(fd, msg);
        break;
    }
    default:
        break;
    }

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
        char *json = get_friendlist_json(lmsg.userid);
        char** friends = get_friends(findOnlineUserWithFd(fd));
        sendResponseFriendList(fd, json);
        //        printf("%s\n", json);
        free(json);
        close_mysql();

        int  i = 0;


        ResponseFriendStatusChange *rfsc = (ResponseFriendStatusChange*)malloc(sizeof(ResponseFriendStatusChange));
        strcpy(rfsc->userid, findOnlineUserWithFd(fd));

        while (friends[i] != NULL) {
            int fd;
            if((fd = findOnlineUserWithUid(friends[i])) != -1)
            {
                rfsc->status = 1;
                sendFriendStatusChange(fd, rfsc);
                printf("发送%s上线信息给%s\n", rfsc->userid, friends[i]);
            }
            ++i;
        }



        free(rfsc);
        free(friends);
    }

}

void handleHeartBeatMsg(int fd)
{
    sendResponseHeartBeatMsg(fd);
}

void handleExitMsg(int fd)
{

    printf("%s离线\n", findOnlineUserWithFd(fd));
    //从在线用户中移除
    delOnlineUserWithUid(findOnlineUserWithFd(fd));
    delOnlineUserWithFd(fd);

    //从epoll监听中移除
    ev.data.fd = fd;
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

    msg->length = strlen(group);
    strcpy(msg->json, group);

    sendGroupInfo(fd, msg);

    printf("send 群列表 %s\n", group);

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
    case 1:                                //用户上线
        rfsc->status = 1;
        node->user.status = UserOnLine;
        break;
    case 2:                                  //用户隐身
        rfsc->status = 2;
        node->user.status = UserHide;
        break;
    case 3:                               //离线做离线处理
        node->user.status = 3;
        ev.data.fd = fd;
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
        close(fd);

        delOnlineUserWithUid(findOnlineUserWithFd(fd));
        delOnlineUserWithFd(fd);
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

    free(rmsg);
}

void handleDeleteFriendMsg(int fd, Msg *msg)
{
    RequestDeleteFriend *rmsg = (RequestDeleteFriend*)malloc(msg->len);
    memcpy(rmsg, msg->data, msg->len);

    printf("%s 请求删除 好友%s\n", findOnlineUserWithFd(fd), rmsg->friendid);

    init_mysql();
    deletefriend(findOnlineUserWithFd(fd), rmsg->friendid);
    deletefriend(rmsg->friendid, findOnlineUserWithFd(fd));
    close_mysql();

    free(rmsg);
}

void handleCreateGroupMsg(int fd, Msg*msg)
{
    RequestCreateGroup *rmsg = (RequestCreateGroup*)malloc(msg->len);

    memcpy(rmsg, msg->data, msg->len);

    char *groupname = malloc(sizeof(char) * rmsg->length);

    strcpy(groupname, rmsg->groupname);
    groupname[rmsg->length - 1] = '\0';

    init_mysql();
    insert_friend_group(findOnlineUserWithFd(fd), groupname);
    close_mysql();
    free(groupname);
    free(rmsg);
}

void handleSearchFriendMsg(int fd, Msg*msg)
{
    RequestSearchFriend *rmsg1 = (RequestSearchFriend*)malloc(msg->len);
    memcpy(rmsg1, msg->data, msg->len);

    init_mysql();
    char *json = get_friend_info(rmsg1->userid);
    printf("%s\n", json);
    close_mysql();

    int len = strlen(json);

    ResponseSearchFriend *rmsg2 = (ResponseSearchFriend*)malloc(sizeof(ResponseSearchFriend) + len);
    rmsg2->length = len;
    strcpy(rmsg2->json, json);

    sendSearchResult(fd, rmsg2);

    free(json);
}

void handleAddFriendMsg(int fd, Msg*msg)
{
    RequestAddFriendAck *rmsg = (RequestAddFriendAck*)malloc(msg->len);
    memcpy(rmsg, msg->data, msg->len);

    int f = findOnlineUserWithUid(rmsg->friendid);

    ForwardAddFriendAck *s = (ForwardAddFriendAck*)malloc(sizeof(ForwardAddFriendAck) + rmsg->length);
    strcpy(s->sendid, findOnlineUserWithFd(fd));
    s->length = rmsg->length;
    if(s->length != 0)
        strcpy(s->validate, rmsg->validate);

     init_mysql();
    if (f != -1) {
        sendFrowardAddFriendAck(f, s);
        add_friend_reply(findOnlineUserWithFd(fd),rmsg->friendid, rmsg->group, rmsg->validate, 1);
    }
    else
    {
        add_friend_reply(findOnlineUserWithFd(fd),rmsg->friendid, rmsg->group, rmsg->validate, 2);
    }
    close_mysql();
    free(s);
}

void handleAddFriendAckMsg(int fd, Msg *msg)
{
    AddFriendResult *rmsg = (AddFriendResult*)malloc(msg->len);
    memcpy(rmsg, msg->data, msg->len);

    if(rmsg->status == 1)
    {
        //添加进数据库  fd是需要验证方。
        init_mysql();
        char *groupid1 = get_gorupid(rmsg->userid, get_addfriend_reply_group(rmsg->userid, findOnlineUserWithFd(fd)));
        char *groupid2 = get_gorupid(findOnlineUserWithFd(fd), rmsg->group);
        add_friend(rmsg->userid, findOnlineUserWithFd(fd), groupid1);
        add_friend(findOnlineUserWithFd(fd), rmsg->userid, groupid2);
        close_mysql();

    }
    else if(rmsg->status == 2)
    {

    }

}
