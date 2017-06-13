/****************************************
*   哈希表								*
*	维护在线用户						*
*	需要保证是线程安全的				*
*	created by lz 2017/5/26  13:44	    *
*****************************************/

#ifndef ONLINEHASHTABLE_H
#define ONLINEHASHTABLE_H

#include "NetWork/marco.h"
#include <stdbool.h>
//typedef struct _OnlineHashTable {
//	
//	//any??
//}OnlineHashTable;


typedef struct _OnlineUser {
	int fd;								//在线用户所拥有的socket描述符
	char userid[USERIDMAXLEN];			//在线用户的ID。 根据此ID来查询还有是否在线
}OnlineUser;

//节点
typedef struct _OnlineUserLinklist {
	OnlineUser user;
	struct _OnlineUserLinklist* next;
}OnlineUserLinklist;

OnlineUserLinklist *head;


void init_onlineuser();

//添加一个在线用户
void addOnlineUser(OnlineUser *user);

//删除一个在线用户
void delOnlineUser(const int fd);

//根据用户id 查找在线用户列表。如果在线返回ture。
int findOnlineUserWithUid(const char *userid);
char* findOnlineUserWithFd(const int fd);


#endif // !ONLINEHASHTABLE_H
