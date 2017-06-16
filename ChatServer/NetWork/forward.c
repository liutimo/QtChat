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

    write(STDOUT_FILENO, fmsg->message, fmsg->length);

    ssize_t length = sizeof(ReceivedMessageMsg) + fmsg->length;

    ReceivedMessageMsg *msg = (ReceivedMessageMsg*)malloc(sizeof(char) * length);


    memcpy(msg, fmsg, sizeof(char) * length);

    char *userid = findOnlineUserWithFd(fd);

    printf("message from %s send to %s\n", userid, findOnlineUserWithFd(friend_fd));
    printf("font %s color  %s size %s\n", fmsg->font, fmsg->color, fmsg->size);



    strcpy(msg->friendid, userid);

    sendMessage(friend_fd, msg);

    free(msg);
}
