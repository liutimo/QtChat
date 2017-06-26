#include "onlinehashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void init_onlineuser()
{
    //	head = (OnlineUserLinklist*)malloc(sizeof(OnlineUserLinklist));
    fd_head = (OnlineUserNode**)malloc(sizeof(OnlineUserNode*) * 1024);
    uid_head = (OnlineUserNode**)malloc(sizeof(OnlineUserNode*) * 1024);

    if (fd_head == NULL || uid_head == NULL)
    {
        printf("In onlinehashtable.c:内存空间不够， 初始化在线用户列表失败\n");
        exit(-1);
    }
}

//添加一个在线用户
void addOnlineUserWithFd(OnlineUser *user)
{
    OnlineUserNode *current1 = fd_head[user->fd - 3];
    OnlineUserNode *node1 = (OnlineUserNode*)malloc(sizeof(OnlineUserNode));

    node1->user.fd = user->fd;
    node1->user.status = user->status;
    strcpy(node1->user.userid, user->userid);

    if(current1 == NULL) {
        fd_head[user->fd - 3] = node1;
        return;
    }

    while(current1 && current1->next)
        current1 = current1->next;

    current1->next = node1;
}



void addOnlineUserWithUid(OnlineUser *user)
{
    OnlineUserNode *current2 = uid_head[atoi(user->userid) % 123456];
    OnlineUserNode *node2 = (OnlineUserNode*)malloc(sizeof(OnlineUserNode));

    node2->user.fd = user->fd;
    node2->user.status = user->status;
    strcpy(node2->user.userid, user->userid);

    if(current2 == NULL) {
        uid_head[atoi(user->userid) % 123456] = node2;
        return;
    }

    while( current2->next)
        current2 = current2->next;

    current2->next = node2;
}

void addOnlineUser(OnlineUser *user)
{
    addOnlineUserWithFd(user);
    addOnlineUserWithUid(user);
}

//删除一个在线用户
void delOnlineUserWithFd(const int fd)
{
    OnlineUserNode *current1= fd_head[fd - 3];
    OnlineUserNode *before1 = current1;

    if(current1 == NULL)
        return;

    if(current1->user.fd == fd)
    {
        fd_head[fd - 3] = NULL;
        free(current1);
        return;
    }

    current1 = current1->next;

    while(current1)
    {
        if(current1->user.fd == fd)
        {
            before1->next = current1->next;
            free(current1);
            return;
        }
    }
}

void delOnlineUserWithUid(const char *uid)
{
    OnlineUserNode *current1= uid_head[atoi(uid) %123456];
    OnlineUserNode *before1 = current1;

    if(current1 == NULL)
        return;

    if(strcmp(uid, current1->user.userid) == 0)
    {
        uid_head[atoi(uid) %123456] = NULL;
        free(current1);
        return;
    }

    current1 = current1->next;

    while(current1)
    {
        if(strcmp(uid, current1->user.userid) == 0)
        {
            before1->next = current1->next;
            free(current1);
            return;
        }
    }
}

char* findOnlineUserWithFd(const int fd)
{
    OnlineUserNode *current= fd_head[fd - 3];

    while(current)
    {
        if(current->user.fd == fd)
        {
            return current->user.userid;
        }

        current = current->next;
    }

    return NULL;
}

//根据用户id 查找在线用户列表。如果在线返回fd。
int findOnlineUserWithUid(const char *userid)
{

    OnlineUserNode *current= uid_head[atoi(userid) % 123456];

    while(current)
    {
        if(strcmp(userid, current->user.userid) == 0)
        {
            return current->user.fd;
        }

        current = current->next;
    }

    return -1;
}

OnlineUserNode* findWithFd(const int fd)
{
    OnlineUserNode *current= fd_head[fd - 3];
    while (current && fd != current->user.fd)
    {
        current = current->next;
    }

    if (current)
        return current;
    else
        return NULL;
}

OnlineUserNode* findWithUid(const char *userid)
{
    OnlineUserNode *current= uid_head[atoi(userid) % 123456];

    while(current)
    {
        if(strcmp(userid, current->user.userid) == 0)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}
