/****************************************
*   数据库相关操作						* 
*	单例模式，全局只拥有实例。			*
*	因此得保证是线程安全的。			*
*	created by lz 2017/5/25  12:32	    *
*****************************************/

#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>
#include <pthread.h>

//mysql 连接实例
MYSQL *mysql;

//mysql 结果集
MYSQL_RES *mysql_res;

//mysql 结果集中的一行
MYSQL_ROW *mysql_row;


//初始化数据库连接
int init_mysql();

//执行SQL
int execute_mysql(const char *sql);

//关闭连接
void close_mysql();

//错误处理
void print_error_mysql(const char *msg);

/*********************我是分界线************************/
/******************应用对数据库的操作*******************/

//判断用户id和密码是否匹配
int login_check_mysql(const char *userid, const char *password);



#endif // !DATABASE_H
