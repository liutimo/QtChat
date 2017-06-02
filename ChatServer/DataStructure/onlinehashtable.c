#include "onlinehashtable.h"
#include <stdio.h>
#include <string.h>
void init_onlineuser()
{
	head = (OnlineUserLinklist*)malloc(sizeof(OnlineUserLinklist));

	if (head == NULL)
	{
		printf("In onlinehashtable.c:内存空间不够， 初始化在线用户列表失败\n");
		exit(-1);
	}
}

//添加一个在线用户
void addOnlineUser(OnlineUser *user)
{
	OnlineUserLinklist* node = (OnlineUserLinklist*)malloc(sizeof(OnlineUserLinklist));
	
	node->user.fd = user->fd;
	strcpy(node->user.userid, user->userid);

	node->next = head->next;
	head->next = node;
}

//删除一个在线用户
void delOnlineUser(const int fd)
{
	OnlineUserLinklist* before = head;
	OnlineUserLinklist* current = head->next;
	while (current && current->user.fd != fd)
	{
		before = before->next;
		current = current->next;
	}

	if (current)
	{
		before->next = current->next;
		free(current);
	}
	else
		return;
}

//根据用户id 查找在线用户列表。如果在线返回ture。
bool findOnlineUser(const char *userid)
{
	OnlineUserLinklist* current = head->next;
	while (current && strcmp(current->user.userid, userid) != 0)
	{
		current = current->next;
	}

	if (current)
		return true;
	else
		return false;
}