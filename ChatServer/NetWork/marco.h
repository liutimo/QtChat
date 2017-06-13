#ifndef MARCO_H
#define MARCO_H

//端口
#define PORT 55555

//用户名长度限制
#define USERIDMAXLEN 20

//加密后密码长度
#define PASSWORDMAXLEN 128

//服务器监听的最大值限定
#define LISTENMAXNUM 128


/*********数据库相关配置************/
#define DATABASE_HOST "localhost"
#define DATABASE_USER "root"
#define DATABASE_PASSWORD "123456"
#define DATABASE_DBNAME "chat"
#define DATABASE_PORT 3306
#define DATABASE_SQLMAXLENGTH 2048  //sql语句最长限定

//查询返回类型 从 129开始

//登陆相关
#define DATABASE_USER_NOTEXIST 129
#define DATABASE_USER_PASSWORDERROR 130
#define DATABASE_USER_CORRECT 131


#endif // MARCO_H
