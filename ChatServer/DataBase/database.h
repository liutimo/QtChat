/****************************************
*   ???????						*
*	?????????????			*
*	????????????			*
*	created by lz 2017/5/25  12:32	    *
*****************************************/

#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>
#include <pthread.h>

//mysql ????
MYSQL *mysql;

//mysql ???
MYSQL_RES *mysql_res;

//mysql ???????
MYSQL_ROW *mysql_row;


void start_mysql();
//????????
int init_mysql();

//??SQL
int execute_mysql(const char *sql);

//????
void close_mysql();

//????
void print_error_mysql(const char *msg);

/*********************?????************************/
/******************?????????*******************/

//????id???????
int login_check_mysql(const char *userid, const char *password);

//get friendlist with json
char *get_friendlist_json(const char *userid);

//get user info
char *get_userinfo_json(const char *userid);

//insert chat log
void set_chatlog(const char *sender, const char *receiver, const char *content, const char *fontfamliy, const char *fontsize, const char *fontcolor);

//store offline message;
void set_offline_message(const char *sender, const char *receiver, const char *content, const char *fontfamliy, const char *fontsize, const char *fontcolor);

//查找离线消息
char *get_offline_message(const char *userid);

//删除离线消息
void del_offline_message(const char *userid);

//移动好友
void move_friend_to_group(char *userid, char *friendid, char *groupid);

//更新个性签名
void update_user_signature(const char *userid, const char *sig);

char *get_group(const char*userid);

char *get_group_member(const char *userid);

char **get_friends(const char *userid);

//将群聊天消息放入聊天记录
void set_group_chatlog(const char *senderid, const char *groupid, const char *content, const char *fontfamliy, const char *fontsize, const char *fontcolor);
void set_group_offlinechatlog(const char *sender,const char *groupid ,const char *receiver, const char *content, const char *fontfamliy, const char *fontsize, const char *fontcolor);


//通过群组id获取成员id
char **get_memberid(const char *groupid);

//删除一个好友
void deleteOneFriend(const char *userid, const char* friendid);

void insert_friend_group(const char *userid, const char *groupname);

char* get_friend_info(const char *userid);

void add_friend(const char *userid, const char *friendid, const char *groupid);

char* get_groupid(const char*userid, const char *groupname);

void add_friend_reply(const char*userid, const char* friendid, const char*groupname,
                      const char*validate, const int handle);

char* get_addfriend_reply_group(const char*userid, const char*friendid);

void deletefriend(const char*userid, const char*friendid);

void delete_friend_group(const char*userid, const char*groupname);

void update_friend_group(const char *userid, const char *oldid, const char *newid);

void update_friend_groupname(/*const char *userid,*/ const char *groupid, const char *groupname);

//返回groupid
int create_chat_group(const char *groupname);

void add_chat_group_member(const int groupid, const char *userid);

char* get_group_offline_message(const char *receivedid);

void delete_one_member(const char *groupid, const char *memberid);

char * get_group_id(const char *groupname);
#endif // !DATABASE_H;
