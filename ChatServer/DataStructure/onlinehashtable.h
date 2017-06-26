/****************************************
*   哈希表								*
*	维护在线用户						*
*	需要保证是线程安全的				*
*	created by lz 2017/5/26  13:44	    *
*****************************************/

#ifndef ONLINEHASHTABLE_H
#define ONLINEHASHTABLE_H

#include "NetWork/marco.h"
#include "NetWork/msgstructure.h"
#include <stdbool.h>
//typedef struct _OnlineHashTable {
//	
//	//any??
//}OnlineHashTable;


typedef struct _OnlineUser {
	int fd;								//在线用户所拥有的socket描述符
	char userid[USERIDMAXLEN];			//在线用户的ID。 根据此ID来查询还有是否在线
    UserStatus status;
}OnlineUser;

//节点
typedef struct _OnlineUserNode {
    OnlineUser user;
    struct OnlineUserNode* next;
}OnlineUserNode;

//OnlineUserLinklist *head;

//以fd为散列值的哈希表
OnlineUserNode* *fd_head;
//以用户id为散列值的哈希表
OnlineUserNode* *uid_head;


void init_onlineuser();

//添加一个在线用户
void addOnlineUser(OnlineUser *user);
void addOnlineUserWithUid(OnlineUser *user);
void addOnlineUserWithFd(OnlineUser *user);
//删除一个在线用户
void delOnlineUserWithFd(const int fd);
void delOnlineUserWithUid(const char *uid);
//根据用户id 查找在线用户列表。如果在线返回ture。
int findOnlineUserWithUid(const char *userid);
char* findOnlineUserWithFd(const int fd);

//查找用户id，返回一个节点指针
OnlineUserNode* findWithUid(const char *userid);
OnlineUserNode* findWithFd(const int fd);

#endif // !ONLINEHASHTABLE_H
