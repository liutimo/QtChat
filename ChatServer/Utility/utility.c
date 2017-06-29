#include "utility.h"
#include "DataBase/database.h"
#include "DataStructure/onlinehashtable.h"

#include <errno.h>

#define MAXLINE 4096

ssize_t readn(int filedes, void *buff, size_t nbytes)
{
    ssize_t nread;
    ssize_t nleft = 0;
    char *ptr;

    ptr = buff;

    int readok = 0;

//    while (nleft > 0) {
//        if ((nread = recv(filedes, ptr, nleft, 0)) < 0) {
//            if (errno == EINTR)
//                nread = 0; /* and call read() again */
//            else
//                return (-1);
//        }
//        else if (nread == 0)
//            break; /* EOF */
//        nleft -= nread;
//        ptr += nread;
//    }

    while(1)
    {
        //一次读取MAXLINE字节数据
        if((nread = read(filedes, ptr + nleft, nbytes)) < 0)
        {
            if (errno == EAGAIN)
            {
                //读取完成
                readok = 1;
                break;
            }
        }
        else if (nread == nbytes)
        {
            return 1;
        }
    }

    if(readok == 1)
    {
        return 1;
    }
    else
        return -1;

//    return (nbytes - nleft); /* return >= 0 */
}


ssize_t writen(int filedes, const void *buff, size_t nbytes)
{
//    ssize_t nleft;
//    ssize_t nwritten;
//    const char *ptr;

//    ptr = buff;
//    nleft = nbytes;
//    while (nleft > 0) {
//        if ((nwritten = write(filedes, ptr, nleft)) <= 0) {
//            return -1;
//        }
//        else if (nwritten == nleft)
//            return 1;
//        nleft -= nwritten;
//        ptr += nwritten;

//        printf("发送了%d字节的数据\n",nwritten);
//    }

//    return nbytes;

    return write(filedes, buff, nbytes);


//    ssize_t written = 0;
//    while(1)
//    {
//        written = write(filedes, buff, nbytes);//写时，数据大小一直在变化
//        if(written < 0)
//        {
//            if(errno == EAGAIN || errno == EWOULDBLOCK)
//            {
//                return 1;
//            }
//            else
//            {
//                return -1;
//            }

//        }
//        else
//        {
//            nbytes -= written;
//            buff += written;
//        }
//    }

//    int nwrite;
//    int n = 0;;
//    while(1)
//    {
//        nwrite = write(filedes, buff + n, nbytes);//写时，数据大小一直在变化
//        if(nwrite <= nbytes)
//        {
//            if(errno == EAGAIN || errno == EWOULDBLOCK)
//            {
//                continue;
//            }
//            else
//            {
//                break;//or return;
//            }

//        }
//        else
//        {
//            n += nwrite;
//            nbytes -= nwrite;
//        }
//    }
}


ssize_t readline(int filedes, void *buff, size_t maxlen)
{

    //do noting !!
    return 0;

}


