#include "forward.h"
#include "sendtoclient.h"
#include "msgstructure.h"
#include "DataStructure/onlinehashtable.h"
#include "DataBase/database.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void forwardmessage(int fd, int friend_fd, RequestForwordMessageMsg *fmsg)
{

    ssize_t length = sizeof(ReceivedMessageMsg) + fmsg->length;

    ReceivedMessageMsg *msg = (ReceivedMessageMsg*)malloc(sizeof(char) * length);
    bzero(msg, length);
    memcpy(msg, fmsg, sizeof(char) * length);

    char *userid = findOnlineUserWithFd(fd);

    strcpy(msg->friendid, userid);

    sendMessage(friend_fd, msg);

    free(msg);
}


//转发消息给群组的其余成员
void forwardgroupmessage(int fd, RequestForwordGroupMessage *rmsg)
{
    //获取该群中所有成员的id
    init_mysql();
    char** memberid = get_memberid(rmsg->groupid);

    //遍历整个成员id， 如果在线就发送转发， 不在线就存入离线消息数据库
    int i = 0;
    while(memberid[i])
    {
        int cfd = findOnlineUserWithUid(memberid[i]);

        if (cfd != fd && cfd != -1)
        {
            //在线
            sendGroupMessage(cfd, rmsg);
            printf("群成员 %s 在线, 消息内容： %s\n", memberid[i], rmsg->message);
        }
        else if(cfd != fd)
        {
            printf("群成员 %s 不在线\n", memberid[i]);
            //不在线
//            set_group_offlinechatlog(rmsg->userid, rmsg->groupid, memberid[i], rmsg->message,
//                                     rmsg->font, rmsg->size, rmsg->color);
        }


        free(memberid[i]);
        ++i;
    }

    free(memberid);

    close_mysql();
}
