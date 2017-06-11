#include "forward.h"
#include "sendtoclient.h"
#include "msgstructure.h"
#include "DataStructure/onlinehashtable.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void forwardmessage(int fd, int friend_fd, const char *message)
{
    ssize_t length = sizeof(ReceivedMessageMsg) + strlen(message) + 1;

    ReceivedMessageMsg *msg = (ReceivedMessageMsg*)malloc(sizeof(char) * length);

    char *userid = findOnlineUserWithFd(fd);

    printf("message from %s send to %s\n", userid, findOnlineUserWithFd(friend_fd));

    strcpy(msg->friendid, userid);
    msg->length = strlen(message);
    memcpy(msg->message, message, strlen(message) + 1);

    sendMessage(friend_fd, msg);

    free(msg);
}
