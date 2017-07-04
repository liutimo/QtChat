#include "database.h"
#include "NetWork/marco.h"
#include "Utility/cJSON.h"
#include "DataStructure/onlinehashtable.h"
#include <stdio.h>
#include <string.h>

int init_mysql()
{
    mysql = mysql_init(NULL);

    //连接数据库。如果失败，返回-1.
    if (!mysql_real_connect(mysql, DATABASE_HOST, DATABASE_USER, DATABASE_PASSWORD,
        DATABASE_DBNAME, DATABASE_PORT, NULL, 0))
        return -1;

    return execute_mysql("set names utf8");
}

int execute_mysql(const char *sql)
{
    if (mysql_real_query(mysql, sql, strlen(sql)))
        return -1;

    return 0;
}

void close_mysql()
{
    mysql_close(mysql);
}

void print_error_mysql(const char *msg)
{
    if (msg)
        printf("%s : %s\n", msg, mysql_error(mysql));
    else
        puts(mysql_error(mysql));
}

/*
* 查询判断用户是否能够匹配
* 返回值包括   匹配   密码错误   以及 用户不存在
*/
int login_check_mysql(const char *userid, const char *password)
{
    char sql[DATABASE_SQLMAXLENGTH];
    sprintf(sql, "select userid, password from user where userid='%s';", userid);

    if (execute_mysql(sql) == -1)
        print_error_mysql(sql);

    //获取查询的结果集
    mysql_res = mysql_store_result(mysql);

    int r_count = mysql_num_rows(mysql_res);

    if (r_count < 1) {
        return DATABASE_USER_NOTEXIST;
    }

    //userid  是唯一的，因此确当结果集合只有一行
    mysql_row = mysql_fetch_row(mysql_res);


    if (strcmp(password, mysql_row[1]) == 0)
        return DATABASE_USER_CORRECT;
    else
        return DATABASE_USER_PASSWORDERROR;
}

char *get_friendlist_json(const char *userid)
{

    cJSON *root = NULL;

    root = cJSON_CreateObject();

    char sql_getgroup[DATABASE_SQLMAXLENGTH], sql_getfriends[DATABASE_SQLMAXLENGTH];
    memset(sql_getgroup, 0, DATABASE_SQLMAXLENGTH);
    memset(sql_getfriends, 0, DATABASE_SQLMAXLENGTH);

    sprintf(sql_getgroup, "select groupname from chat_friend_group where userid='%s';", userid);
    sprintf(sql_getfriends, "select friendid, username, remark, groupname, personalizedsignature,imagepath,birthofdate, sex, mobile, mail "
                            "from friendlist, userinfo, chat_friend_group "
                            "where friendlist.userid='%s' and "
                            "chat_friend_group.groupid = friendlist.groupid and "
                            "friendlist.friendid = userinfo.userid;", userid);

    printf("%s\n", sql_getfriends);

    //get group
    if (execute_mysql(sql_getgroup) == -1)
        print_error_mysql(sql_getgroup);

    mysql_res = mysql_store_result(mysql);

    while((mysql_row = mysql_fetch_row(mysql_res)) != NULL) {
        cJSON_AddItemToObject(root, mysql_row[0], cJSON_CreateArray());
    }

    //get friends

    if (execute_mysql(sql_getfriends) == -1)
        print_error_mysql(sql_getfriends);

    mysql_res = mysql_store_result(mysql);

    //mysql_row[3] == frouptype
    while((mysql_row = mysql_fetch_row(mysql_res)) != NULL) {
        cJSON *node = root->child;
        while(node)
        {
            if(strcmp(node->string, mysql_row[3]) == 0)
            {
                cJSON *current = node;
                if(current == NULL)
                    printf("null\n");

                cJSON *tmp = cJSON_CreateObject();
                cJSON_AddStringToObject(tmp, "friendid", mysql_row[0]);
                cJSON_AddStringToObject(tmp, "username", mysql_row[1]);
                if(mysql_row[2] != NULL)
                    cJSON_AddStringToObject(tmp, "remark", mysql_row[2]);
                else
                    cJSON_AddStringToObject(tmp, "remark", "");
                cJSON_AddStringToObject(tmp, "groupname", mysql_row[3]);
                cJSON_AddStringToObject(tmp, "personalizedsignature", mysql_row[4]);
                cJSON_AddStringToObject(tmp, "imagepath", mysql_row[5]);
                cJSON_AddStringToObject(tmp, "birthofdate", mysql_row[6]);
                cJSON_AddStringToObject(tmp, "sex", mysql_row[7]);
                cJSON_AddStringToObject(tmp, "mobile", mysql_row[8]);
                cJSON_AddStringToObject(tmp, "mail", mysql_row[9]);
                OnlineUserNode* u = findWithUid(mysql_row[0]);
                if(u != NULL)
                    cJSON_AddStringToObject(tmp, "status", "online");
                else
                    cJSON_AddStringToObject(tmp, "status", "offline");
                cJSON_AddItemToArray(node, tmp);
            }
            node = node->next;
        }
    }

    return cJSON_PrintUnformatted(root);
}

char *get_userinfo_json(const char *userid)
{
    cJSON *root = NULL;

    root = cJSON_CreateObject();

    char sql_getuserinfo[DATABASE_SQLMAXLENGTH];
    memset(sql_getuserinfo, 0, DATABASE_SQLMAXLENGTH);

    sprintf(sql_getuserinfo, "select username, personalizedsignature, imagepath from userinfo where userid='%s';", userid);

    //get group
    if (execute_mysql(sql_getuserinfo) == -1)
        print_error_mysql(sql_getuserinfo);

    mysql_res = mysql_store_result(mysql);

    if((mysql_row = mysql_fetch_row(mysql_res)) != NULL) {
        cJSON_AddStringToObject(root, "username", mysql_row[0]);
        cJSON_AddStringToObject(root, "ps", mysql_row[1]);
        cJSON_AddStringToObject(root, "imagepath", mysql_row[2]);
    }

    return cJSON_PrintUnformatted(root);
}


//logid      | int(11)   | NO   | PRI | NULL              | auto_increment |
//| senderid   | char(10)  | NO   | MUL | NULL              |                |
//| content    | text      | YES  |     | NULL              |                |
//| chattime   | timestamp | NO   |     | CURRENT_TIMESTAMP |                |
//| receiverid

void set_chatlog(const char *sender, const char *receiver, const char *content, const char *fontfamliy, const char *fontsize, const char *fontcolor)
{
    char sql_insert_chatlog[DATABASE_SQLMAXLENGTH];

    sprintf(sql_insert_chatlog, "insert into chatlog(senderid, receiverid, content, fontfamliy, fontsize, fontcolor) "
                                "values('%s', '%s', '%s', '%s', '%s', '%s')", sender, receiver, content, fontfamliy, fontsize, fontcolor);

    if (execute_mysql(sql_insert_chatlog) == -1)
        print_error_mysql(sql_insert_chatlog);
}

void set_offline_message(const char *sender, const char *receiver, const char *content, const char *fontfamliy, const char *fontsize, const char *fontcolor)
{
    char sql_insert_offlinemessage[DATABASE_SQLMAXLENGTH];

    sprintf(sql_insert_offlinemessage, "insert into offlinemessage(senderid, receiverid, content, fontfamliy, fontsize, fontcolor) "
                                "values('%s', '%s', '%s', '%s', '%s', '%s')", sender, receiver, content, fontfamliy, fontsize, fontcolor);
    if (execute_mysql(sql_insert_offlinemessage) == -1)
        print_error_mysql(sql_insert_offlinemessage);
}



/*
*   JSON 格式
*   {
*       'senderid' :
*        {
*           "content" : "";
*           'font': '';
*           'color' : '';
*        }
*   }
*
*/
char *get_offline_message(const char *userid)
{
    cJSON *root = cJSON_CreateObject();

    char sql_get_offlinemessage[DATABASE_SQLMAXLENGTH];
    char sql_get_offlinesenderid[DATABASE_SQLMAXLENGTH];

    sprintf(sql_get_offlinemessage, "select senderid, content, fontfamliy, fontsize, fontcolor from offlinemessage where receiverid='%s';", userid);
    sprintf(sql_get_offlinesenderid, "select distinct senderid from offlinemessage where receiverid='%s';", userid);


    if(execute_mysql(sql_get_offlinesenderid) == -1)
        print_error_mysql(sql_get_offlinesenderid);

    mysql_res = mysql_store_result(mysql);

    while((mysql_row = mysql_fetch_row(mysql_res)) != NULL) {
        cJSON_AddItemToObject(root, mysql_row[0], cJSON_CreateArray());
    }

    if(execute_mysql(sql_get_offlinemessage) == -1)
        print_error_mysql(sql_get_offlinemessage);

    mysql_res = mysql_store_result(mysql);

    while((mysql_row = mysql_fetch_row(mysql_res)) != NULL) {
        cJSON *node = root->child;
        while(node)
        {
            if(strcmp(node->string, mysql_row[0]) == 0)
            {
                cJSON *current = node;
                if(current == NULL)
                    printf("null\n");

                cJSON *tmp = cJSON_CreateObject();
                cJSON_AddStringToObject(tmp, "content", mysql_row[1]);
                cJSON_AddStringToObject(tmp, "fontfamliy", mysql_row[2]);
                cJSON_AddStringToObject(tmp, "fontsize", mysql_row[3]);
                cJSON_AddStringToObject(tmp, "fontcolor", mysql_row[4]);
                cJSON_AddItemToArray(node, tmp);
            }
            node = node->next;
        }
    }

    return cJSON_PrintUnformatted(root);
}

void del_offline_message(const char *userid)
{
    char sql_del_offline[DATABASE_SQLMAXLENGTH];
    char sql_del_group_offline[DATABASE_SQLMAXLENGTH];
    sprintf(sql_del_offline, "delete from offlinemessage where receiverid='%s';", userid);
    sprintf(sql_del_group_offline, "delete from chat_group_offlinemessage where receiverid='%s';", userid);

    if(execute_mysql(sql_del_offline) == -1)
        print_error_mysql(sql_del_offline);

    if(execute_mysql(sql_del_group_offline) == -1)
        print_error_mysql(sql_del_group_offline);
}

void move_friend_to_group(char *userid, char *friendid, char *groupid)
{
    char sql_move_to_group[DATABASE_SQLMAXLENGTH];

    sprintf(sql_move_to_group, "update friendlist set groupid='%s' where userid='%s' and friendid='%s';"
            , groupid, userid, friendid);

    if(execute_mysql(sql_move_to_group) == -1)
        print_error_mysql(sql_move_to_group);
}


void update_user_signature(const char *userid, const char *sig)
{
    char sql_update_signature[DATABASE_SQLMAXLENGTH];
    sprintf(sql_update_signature, "update userinfo set personalizedsignature='%s' where userid='%s';", sig, userid);

    if(execute_mysql(sql_update_signature) == -1)
        print_error_mysql(sql_update_signature);
}

char *get_group(const char*userid)
{
    char sql_get_group[DATABASE_SQLMAXLENGTH];
    sprintf(sql_get_group, "select chat_group.groupid, groupname, groupimage from chat_group, chat_groupmember "
                            "where chat_group.groupid=chat_groupmember.groupid and chat_groupmember.memberid='%s';", userid);

//    printf("%s\n", sql_get_group);

    if(execute_mysql(sql_get_group) == -1)
        print_error_mysql(sql_get_group);

    mysql_res = mysql_store_result(mysql);

    cJSON *root = cJSON_CreateArray();

    while((mysql_row = mysql_fetch_row(mysql_res)) != NULL) {
        cJSON *tmp = cJSON_CreateArray();
        cJSON_AddStringToObject(tmp, "groupid", mysql_row[0]);
        cJSON_AddStringToObject(tmp, "groupname", mysql_row[1]);
        cJSON_AddStringToObject(tmp, "groupimage", mysql_row[2]);
        cJSON_AddItemToArray(root, tmp);

    }

    return cJSON_PrintUnformatted(root);
}

char *get_group_member(const char *userid)
{
    char sql_get_groupid[DATABASE_SQLMAXLENGTH];
    char sql_get_groupmember[DATABASE_SQLMAXLENGTH];
    sprintf(sql_get_groupid, "select distinct groupid from chat_groupmember where memberid='%s';", userid);
    sprintf(sql_get_groupmember, "select groupid, memberid, userinfo.username, imagepath from chat_groupmember, userinfo "
                                 "where userid=memberid and groupid in "
                                  "(select groupid from chat_groupmember "
                                  "where chat_groupmember.memberid='%s');", userid);

    cJSON *root = cJSON_CreateObject();

//    /*printf*/("%s\n", sql_get_groupid);

    if(execute_mysql(sql_get_groupid) == -1)
        print_error_mysql(sql_get_groupid);

    mysql_res = mysql_store_result(mysql);

    while((mysql_row = mysql_fetch_row(mysql_res)) != NULL)
    {
        cJSON_AddItemToObject(root, mysql_row[0], cJSON_CreateArray());
    }


    if(execute_mysql(sql_get_groupmember) == -1)
        print_error_mysql(sql_get_groupmember);

    mysql_res = mysql_store_result(mysql);
//    printf("%s\n", sql_get_groupmember);

    while((mysql_row = mysql_fetch_row(mysql_res)) != NULL)
    {
        cJSON *node = root->child;
        while(node)
        {
            if(strcmp(node->string, mysql_row[0]) == 0)
            {
                cJSON *current = node;
                if(current == NULL)
                    printf("null\n");

                cJSON *tmp = cJSON_CreateObject();
                cJSON_AddStringToObject(tmp, "memberid", mysql_row[1]);
                cJSON_AddStringToObject(tmp, "membername", mysql_row[2]);
                cJSON_AddStringToObject(tmp, "memberimage", mysql_row[3]);
                cJSON_AddItemToArray(node, tmp);
            }
            node = node->next;
        }
    }

    return cJSON_PrintUnformatted(root);
}

char **get_friends(const char *userid)
{
    char sql_get_friendcount[DATABASE_SQLMAXLENGTH];
    char sql_get_friendsid[DATABASE_SQLMAXLENGTH];
    sprintf(sql_get_friendcount, "select count(friendid) from friendlist where userid='%s';", userid);
    sprintf(sql_get_friendsid, "select friendid from friendlist where userid='%s';", userid);

    if(execute_mysql(sql_get_friendcount) == -1)
        print_error_mysql(sql_get_friendcount);

    mysql_res = mysql_store_result(mysql);

    mysql_row = mysql_fetch_row(mysql_res);
    int count = atoi(mysql_row[0]);

    char **friends = (char**)malloc(sizeof(char*) * count + 1);
    friends[count] = NULL;

    if(execute_mysql(sql_get_friendsid) == -1)
        print_error_mysql(sql_get_friendsid);

    mysql_res = mysql_store_result(mysql);

    int i = 0;
    while((mysql_row = mysql_fetch_row(mysql_res)) != NULL)
    {
        char *friendid = (char*)malloc(strlen(mysql_row[0]) + 1);
        strcpy(friendid, mysql_row[0]);
        friends[i++] = friendid;
    }

    return friends;
}

void set_group_chatlog(const char *senderid, const char *groupid, const char *content, const char *fontfamliy, const char *fontsize, const char *fontcolor)
{
    char sql_insert_groupchatlog[DATABASE_SQLMAXLENGTH];

    sprintf(sql_insert_groupchatlog, "insert into chat_group_chatrecord(senderid, groupid, content, fontfamliy, fontsize, fontcolor) "
                                "values('%s', '%s', '%s', '%s', '%s', '%s')", senderid, groupid, content, fontfamliy, fontsize, fontcolor);
//    printf("%s\n", sql_insert_groupchatlog);
    if (execute_mysql(sql_insert_groupchatlog) == -1)
        print_error_mysql(sql_insert_groupchatlog);
}

void set_group_offlinechatlog(const char *sender,const char *groupid ,const char *receiver, const char *content, const char *fontfamliy, const char *fontsize, const char *fontcolor)
{
    char sql_insert_offlinemessage[DATABASE_SQLMAXLENGTH];

    sprintf(sql_insert_offlinemessage, "insert into chat_group_offlinemessage(senderid, groupid,receiverid, content, fontfamliy, fontsize, fontcolor) "
                                "values('%s', '%s', '%s' ,'%s', '%s', '%s', '%s')", sender, groupid, receiver, content, fontfamliy, fontsize, fontcolor);
//    printf("%s\n", sql_insert_offlinemessage);
    if (execute_mysql(sql_insert_offlinemessage) == -1)
        print_error_mysql(sql_insert_offlinemessage);
}

char **get_memberid(const char *groupid)
{
    char sql_get_member_count[DATABASE_SQLMAXLENGTH];
    char sql_get_membeid[DATABASE_SQLMAXLENGTH];
    sprintf(sql_get_member_count, "select count(memberid) from chat_groupmember where groupid='%s';", groupid);
    sprintf(sql_get_membeid, "select memberid from chat_groupmember where groupid='%s';", groupid);

    if(execute_mysql(sql_get_member_count) == -1)
        print_error_mysql(sql_get_member_count);

    mysql_res = mysql_store_result(mysql);

    mysql_row = mysql_fetch_row(mysql_res);
    int count = atoi(mysql_row[0]);

    char **members = (char**)malloc(sizeof(char*) * count + 1);
    members[count] = NULL;

    if(execute_mysql(sql_get_membeid) == -1)
        print_error_mysql(sql_get_membeid);

    mysql_res = mysql_store_result(mysql);

    int i = 0;
    while((mysql_row = mysql_fetch_row(mysql_res)) != NULL)
    {
        char *memberid = (char*)malloc(strlen(mysql_row[0]) + 1);
        strcpy(memberid, mysql_row[0]);
        members[i++] = memberid;
    }

    return members;
}

void deleteOneFriend(const char *userid, const char* friendid)
{
    char sql_delete_friend[DATABASE_SQLMAXLENGTH];

    sprintf(sql_delete_friend, "delete from friendlist where friendid='%s' and userid='%s';", friendid, userid);

    if(execute_mysql(sql_delete_friend) == -1)
        print_error_mysql(sql_delete_friend);
}

void insert_friend_group(const char *userid, const char *groupname)
{
    char sql_insert_friend_group[DATABASE_SQLMAXLENGTH];

    sprintf(sql_insert_friend_group, "insert into chat_friend_group(userid, groupname) values('%s', '%s');"
            , userid, groupname);

    if(execute_mysql(sql_insert_friend_group) == -1)
        print_error_mysql(sql_insert_friend_group);
}

char* get_friend_info(const char *userid)
{
    char sql_get_friend[DATABASE_SQLMAXLENGTH];

    sprintf(sql_get_friend, "select userid, username, imagepath from userinfo where userid='%s';", userid);

    if (execute_mysql(sql_get_friend) == -1)
        print_error_mysql(sql_get_friend);

    mysql_res = mysql_store_result(mysql);

    cJSON *root = NULL;

    root = cJSON_CreateObject();

    if((mysql_row = mysql_fetch_row(mysql_res)) != NULL) {
        cJSON_AddStringToObject(root, "userid", mysql_row[0]);
        cJSON_AddStringToObject(root, "username", mysql_row[1]);
        cJSON_AddStringToObject(root, "imagepath", mysql_row[2]);
    }

    return cJSON_PrintUnformatted(root);
}


char* get_groupid(const char*userid, const char *groupname)
{

    if(groupname == NULL)
        return NULL;
    char sql[DATABASE_SQLMAXLENGTH];
    sprintf(sql, "select groupid from chat_friend_group where userid='%s' and groupname='%s';", userid, groupname);

    if(execute_mysql(sql) == -1)
        print_error_mysql(sql);

    mysql_res = mysql_store_result(mysql);

    mysql_row = mysql_fetch_row(mysql_res);

    if(mysql_row == NULL)
        return NULL;

    return mysql_row[0];
}

void add_friend(const char *userid, const char *friendid, const char *groupid)
{
    char sql[DATABASE_SQLMAXLENGTH];

    sprintf(sql, "insert into friendlist(userid, friendid, groupid, infochanged) values('%s', '%s', '%s', '%s');", userid, friendid, groupid, "0");

    if(execute_mysql(sql) == -1)
        print_error_mysql(sql);

}

void add_friend_reply(const char*userid, const char* friendid, const char*groupname,
                      const char*validate, const int handle)
{
    char sql[DATABASE_SQLMAXLENGTH];

    sprintf(sql, "insert into chat_add_friend_reply(userid, friendid, groupname, validate"
                 ", handle) values('%s', '%s', '%s', '%s', %d);", userid, friendid, groupname
            ,validate, handle);
    printf("%s\n", sql);

    if(execute_mysql(sql) == -1)
        print_error_mysql(sql);
}


char* get_addfriend_reply_group(const char*userid, const char*friendid)
{
    char sql[DATABASE_SQLMAXLENGTH];
    sprintf(sql, "select groupname from chat_add_friend_reply where userid='%s' and friendid='%s';", userid, friendid);

    if(execute_mysql(sql) == -1)
        print_error_mysql(sql);

    mysql_res = mysql_store_result(mysql);

    mysql_row = mysql_fetch_row(mysql_res);

    return mysql_row[0];
}

void delete_friend_group(const char*userid, const char*groupid)
{
    char sql[DATABASE_SQLMAXLENGTH];

    sprintf(sql, "delete from chat_friend_group where groupid=%s;", groupid);

    printf("%s\n", sql);

    if(execute_mysql(sql) == -1)
        print_error_mysql(sql);
}

void update_friend_group(const char *userid, const char *oldid, const char *newid)
{
    char sql[DATABASE_SQLMAXLENGTH];

    sprintf(sql, "update friendlist set groupid='%s' where userid='%s' and groupid='%s';", newid, userid, oldid);

    printf("%s\n", sql);

    if(execute_mysql(sql) == -1)
        print_error_mysql(sql);
}

void deletefriend(const char*userid, const char*friendid)
{
    char sql[DATABASE_SQLMAXLENGTH];

    sprintf(sql, "delete from friendlist where userid='%s' and friendid='%s';", userid, friendid);
    printf("%s\n", sql);

    if(execute_mysql(sql) == -1)
        print_error_mysql(sql);
}


void update_friend_groupname(/*const char *userid,*/ const char *groupid, const char *groupname)
{
    char sql[DATABASE_SQLMAXLENGTH];

    sprintf(sql, "update chat_friend_group set groupname='%s' where groupid='%s';", groupname, groupid);

    printf("%s\n", sql);

    if (execute_mysql(sql) == -1)
        print_error_mysql(sql);
}

int create_chat_group(const char *groupname)
{
    char sql[DATABASE_SQLMAXLENGTH];

    //选择当前最大的groupid
    sprintf(sql, "select groupid from chat_group order by groupid desc limit 1;");

    if(execute_mysql(sql) == -1)
        print_error_mysql(sql);

    mysql_res = mysql_store_result(mysql);
    mysql_row = mysql_fetch_row(mysql_res);

    if(mysql_row != NULL)
    {
        int groupid = atoi(mysql_row[0]) + 1;
        char sql_create_chat_group[DATABASE_SQLMAXLENGTH];
        sprintf(sql_create_chat_group, "insert into chat_group(groupid, groupname) values('%d', '%s');", groupid, groupname);

        if(execute_mysql(sql_create_chat_group) == -1)
            print_error_mysql(sql_create_chat_group);

        return groupid;
    }

    return 0;
}

void add_chat_group_member(const int groupid, const char *userid)
{
    char sql[DATABASE_SQLMAXLENGTH];

    //选择当前最大的groupid
    sprintf(sql, "insert into chat_groupmember(groupid, memberid) values('%d', '%s');", groupid, userid);

    printf("%s\n", sql);

    if(execute_mysql(sql) == -1)
        print_error_mysql(sql);

}

char* get_group_offline_message(const char *receivedid)
{
    char sql1[DATABASE_SQLMAXLENGTH];
    char sql2[DATABASE_SQLMAXLENGTH];

    sprintf(sql1, "select distinct groupid from chat_group_offlinemessage where receiverid='%s';", receivedid);
    sprintf(sql2, "select groupid, senderid, content, fontfamliy, fontsize, fontcolor "
                 "from chat_group_offlinemessage where receiverid='%s';", receivedid);

    if(execute_mysql(sql1) == -1)
        print_error_mysql(sql1);

    cJSON *root = cJSON_CreateObject();

    mysql_res = mysql_store_result(mysql);
    while((mysql_row = mysql_fetch_row(mysql_res)) != NULL)
    {
        cJSON_AddItemToObject(root, mysql_row[0], cJSON_CreateArray());
    }

    if(execute_mysql(sql2) == -1)
        print_error_mysql(sql2);
     mysql_res = mysql_store_result(mysql);

     while((mysql_row = mysql_fetch_row(mysql_res)) != NULL)
     {
         cJSON *node = root->child;
         while(node)
         {
             if(strcmp(node->string, mysql_row[0]) == 0)
             {
                 cJSON *tmp = cJSON_CreateObject();
                 cJSON_AddStringToObject(tmp, "senderid", mysql_row[1]);
                 cJSON_AddStringToObject(tmp, "content", mysql_row[2]);
                 cJSON_AddStringToObject(tmp, "fontfamliy", mysql_row[3]);
                 cJSON_AddStringToObject(tmp, "fontsize", mysql_row[4]);
                 cJSON_AddStringToObject(tmp, "fontcolor", mysql_row[5]);
                 cJSON_AddItemToArray(node, tmp);
                 break;
             }
             else
                 node = node->next;
         }
     }

     return cJSON_PrintUnformatted(root);
}
