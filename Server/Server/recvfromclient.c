#include "recvfromclient.h"
#include "utility.h"
#include "msgstructure.h"
#include "error.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void recvMsg(int fd)
{
	
	//获取后面一个包的长度
	char *buf = (char *)malloc(sizeof(Request));
	readn(fd, buf, sizeof(Request));
	Request *r = (Request*)buf;
	ssize_t length = r->len;
	free(buf);
	buf = NULL;

	buf = (char *)malloc(sizeof(char) * length);

	if (readn(fd, buf, length) != length)
		err_sys("读取消息失败");
	
	Msg *msg = (Msg*)malloc(sizeof(Msg) + sizeof(LoginMsg));

	memcpy(msg, buf, length);

	if (msg->type == REQUESTLOGIN)
	{
		write(STDOUT_FILENO, "login\n", 6);

		LoginMsg m;
		memcpy(&m, msg->data, sizeof(LoginMsg));

		write(STDOUT_FILENO, m.userid, strlen(m.userid));
		write(STDOUT_FILENO, m.password, strlen(m.password));

	}



	/*LoginMsg *msg = (LoginMsg*)malloc(sizeof(LoginMsg));
	strcpy(msg->userid, "123456");
	strcpy(msg->password, "654321");

	Msg *s = (Msg*)malloc(sizeof(Msg) + sizeof(LoginMsg));

	memcpy(s->data, msg, sizeof(LoginMsg));

	LoginMsg m;
	memcpy(&m, s->data, sizeof(LoginMsg));

	printf("%s\n%s\n", m.userid, m.password);
	*/

	free(buf);
}