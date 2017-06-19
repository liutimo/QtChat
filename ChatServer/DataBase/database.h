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
#endif // !DATABASE_H
