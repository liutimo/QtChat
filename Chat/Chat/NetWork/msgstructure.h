#ifndef MSGSTRUCTURE_H
#define MSGSTRUCTURE_H

#include "marco.h"

//消息类型
typedef enum _MsgType {
    HEARTBEAT = 0,                  //心跳包
    REQUESTLOGIN,                   //请求登陆
    RESPONSELOGIN,                  //告知客户端登录状态
    RESPONSEFRIENDLIST,             //好友列表
    EXIT,                           //退出消息
    REQUESTFORWORDMESSAGE,          //请求服务器转发消息
    RECEIVEDMESSAGE,                //收到消息
    RESPONSEUSERINFO,               //回应登录用户的信息
    REQUESTUSERINFO,                //请求登录用户的信息
    REQUESTOFFLINEMESSAGE,          //请求当前登录用户的离线消息
    RESPONSEOFFLINEMESSAGE,         //响应用户的请求离线消息的消息
    RESPONSACKOFFLINEMSG,           //告知服务器， 离线消息已经收到
    REQUESTMOVEFRIENDTOGROUP,       //移动好友到指定分组
    REQUESTUPDATESIGNAURE,          //更新个性签名
    REQUESTGROUPINFO,               //请求群组信息
    RESPONSEGROUPINFO,              //回应群组信息
    REQUESTGROUPMEMBERINFO,         //请求群组成员信息
    RESPONSEGROUPMEMBERINFO,        //回应群组成员信息
    REQUESTCHANGESTATUS,            //更改状态
    RESPONSEFRIENDSTATUSCHANGE,     //告知客户端好友状态改变
    REQUESTFORWARDGROUPMESSAGE,     //请求服务器转发群组消息
    REQUESTDELETEFRIEND,            //请求删除一个好友
    REQUESTCREATEGROUP,             //请求创建分组
    REQUESTSEARCHFRIEND,            //请求搜索好友
    RESPONSESEARCHFRIEND,           //响应客户端搜索好友的消息
    REQUESTADDFRIENDACK,            //发送添加好友确认请求
    FORWARDADDFRIENDACK,            //转发添加好友请求
    ADDFRIENDSTATUS,                //添加好友的结果  同意或者拒绝
    DELETEGROUP,                    //删除分组
    RENAMEFRIENDGROUP,              //重命名
    REQUESTCREATECHATGROUP,         //创建聊天群
    Test
}MsgType;


typedef enum _UserStauts {
    UserOnLine = 0,                     //在线
    UserOffLine,                        //离线
    UserHide                            //隐身
}UserStatus;

//心跳包.  request和response都使用这个结构
typedef struct _HeartBackMsg {
    char status;    				//确定为 'a';
}HeartBeatMsg;

//登录状态
typedef enum _LoginStatus {
    LOGINSUCCESS = 0,               //成功登录
    LOGINPWERROR,                   //密码错误
    LOGINUNKNOW,                    //未知用户登录
    LOGINREPEAT                     //重复登陆
}LoginStatus;

//为了避免粘包，发送这个结构体来告知后面的数据的长度
typedef struct _Resquest {
    long len;
}Request;

typedef struct _Response {
    long len;
}Response;


//通用消息结构
typedef struct _Msg {
    unsigned int m_uiCheckCrc;            //消息校验 0xAFAFAFAF
    MsgType type;   //消息类型
    int len;        //消息长度
    char data[0];   //消息
}Msg;


//登陆消息
typedef struct _LoginMsg {
    char userid[USERIDMAXLEN];
    char password[PASSWORDMAXLEN];
}LoginMsg;

//登录响应消息
typedef struct _ResponseLoginMsg {
    LoginStatus ls;				//登录状态
}ResponseLoginMsg;

//friendlist response
typedef struct _ResponseFriendList {
    int len;
    char friendlist[0];
}ResponseFriendList;

typedef struct _ExitMsg {

}ExitMsg;


typedef struct _RequestForwordMessageMsg {
    char friendid[USERIDMAXLEN];
    char font[30];
    char size[3];
    char color[8];
    long length;
    char message[0];
}RequestForwordMessageMsg;

typedef struct _ReceivedMessageMsg {
    char friendid[USERIDMAXLEN];
    char font[30];
    char size[3];
    char color[8];
    long length;
    char message[0];
}ReceivedMessageMsg;

typedef struct _ResponseUserinfo {
    int length;
    char userinfo[0];
}ResponseUserinfo;


//请求用户的用户信息。是一个空包，因为并不需要用户任何的标志，仅仅凭借当前的fd就能获取用户的用户id
typedef struct _RequestUserInfoMsg {
}RequestUserInfoMsg;


//请求登录用户的离线消息，发送一个空包
typedef struct _RequestOfflineMessage {
}RequestOfflineMessage;

//请求当前用户的离线消息
typedef struct _ResponseOfflineMessage {
    int length;
    char json[0];
}ResponseOfflineMessage;

//空包
typedef struct _ResponsAckOfflineMsg {
}ResponseAckOfflineMsg;

//移动好友请求  移动目的userid到grouptype
typedef struct _RequestMoveFriendToGroup {
    char userid[USERIDMAXLEN];
    char grouptype[20];
}RequestMoveFriendToGroup;


//个性签名更新请求
typedef struct _RequestUpdateSignature {
    int length;
    char sig[0];
}RequestUpdateSignature;

//请求群组信息  空包
typedef struct _RequestGroupInfo {

}RequestGroupInfo;

typedef struct _ResponseGroupInfo {
    int length;
    char json[0];
}ResponseGroupInfo;

//请求群组成员信息  空包
typedef struct _RequestGroupMemberInfo {

}RequestGroupMemberInfo;

//回应的群组成员信息
typedef struct _ResponseGroupMemberInfo {
    int length;
    char json[0];
}ResponseGroupMemberInfo;

//请求更改状态
typedef struct _RequestChangeStatus {
    int status;
}RequestChangeStatus;

typedef struct _ResponseFriendStatusChange {
    char userid[USERIDMAXLEN];
    int status;
}ResponseFriendStatusChange;


typedef struct _ForwordGroupMessage {
    char userid[USERIDMAXLEN];
    char groupid[USERIDMAXLEN];
    char font[30];
    char size[3];
    char color[8];
    long length;
    char message[0];
}ForwordGroupMessage;

//退出消息
typedef struct _RequestExit {

}RequestExit;


//请求删除一个好友
typedef struct _RequestDeleteFriend {
    char friendid[USERIDMAXLEN];
}RequestDeleteFriend;


typedef struct _RequestCreateGroup {
    int length;
    char groupname[0];
}RequestCreateGroup;

typedef struct _RequestSearchFriend {
    char userid[USERIDMAXLEN];
}RequestSearchFriend;

typedef struct _ResponseSearchFriend {
    int length;
    char json[0];
}ResponseSearchFriend;

typedef struct _RequestAddFriendAck {
    char friendid[USERIDMAXLEN];
    char group[USERIDMAXLEN];
    int length;
    char validate[0];
}RequestAddFriendAck;

typedef struct _ForwardAddFriendAck {
    char sendid[USERIDMAXLEN];
    int length;
    char validate[0];
}ForwardAddFriendAck;


typedef struct _AddFriendResult {
    char userid[USERIDMAXLEN];  // 请求添加好友的id
    char group[64];
    int status;                 //1同意， 2 拒绝
}AddFriendResult;

typedef struct _DeleteGroup {
    char groupname[64];
}DeleteGroup;

typedef struct _RenameFriendGroup {
    char oldgroupname[64];      //旧名称
    char newgroupname[64];      //新名称
}RenameFriendGroup;

typedef struct _CreateChatGroup {
    char groupname[64];
    int length;
    char json[0];
}CreateChatGroup;

#endif // MSGSTRUCTURE_H
