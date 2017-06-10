#ifndef FORWARD_H
#define FORWARD_H
/*
 * 服务器提供的所有转发服务
 */


/*
当一个用户给好友发送消息时,服务器查找在线用户队列.
如果该用户在线,则将消息转发给查询到的好友(fd表示该好友的描述字).
否则,将消息放入离线消息队列,当用户上线时,检查离线消息队列中是否有未读消息.
*/
void forwardmessage(int fd, int friend_fd, const char *message);


#endif // FORWARD_H
