#include "database.h"
#include "../allvariable.h"
#include <QDebug>

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("chat.db");
    database.open();
}

DataBase * DataBase::instance = NULL;
QMutex * DataBase::mutex = new QMutex();


//获取数据库链接实例
DataBase* DataBase::getInstance()
{
    mutex->lock();

    if(instance == NULL)
    {
        instance = new DataBase();
    }

    mutex->unlock();

    return instance;
}

//获取当前电脑的登陆过并且保存了的用户信息
QPair<QString, QString> DataBase::getLocalUserInfo()
{
    QSqlQuery sql_query;
    sql_query.prepare("select * from localuserinfo;");
    sql_query.exec();
    while(sql_query.next())
    {
        int id = sql_query.value(0).toInt();
        QString name = sql_query.value(1).toString();
        QString pw = sql_query.value(2).toString();
        return QPair<QString, QString>(name, pw);
    }
    return QPair<QString, QString>("","");
}

//设置当前电脑登陆过并且保存的用户信息
void DataBase::setLoaclUserInfo(const QString& userid, const QString &password)
{
    QString sql = QString("update localuserinfo set userpw='%1' where userid='%2'").arg(password, userid);

    QSqlQuery sql_query;
    sql_query.prepare(QString("select * from localuserinfo where userid='%1'").arg(userid));
    sql_query.exec();
    if(sql_query.next())
    {
        sql_query.prepare(sql);
        sql_query.exec();
    }
    else
    {
        sql_query.prepare(QString("delete from localuserinfo where userid='%1'and userpw='%2';").arg(userid, password));
        sql_query.exec();
        sql_query.prepare(QString("insert into localuserinfo(userid, userpw) values('%1', '%2')").arg(userid, password));
        sql_query.exec();
    }
}

//保存好友列表信息
void DataBase::setFriendList(QList<QVector<QString>> friends)
{

    QString sql_clear = "delete from friendlist where userid=?;";
    QSqlQuery s;
    s.prepare(sql_clear);
    s.addBindValue(AllVariable::getLoginUserId());
    s.exec();


    QString sql = "insert into friendlist(userid, friendid, username, remark, "
                  "personalizedsignature, grouptype, imagepath, birthofdate"
                  ", sex, mobile, mail, status) values('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12')";

    QString sql2 = "delete from friendlist where userid='%1', friendid='%2';";

    for (QVector<QString> onefriend : friends)
    {
        QString s = sql.arg(AllVariable::getLoginUserId(),onefriend[0], onefriend[1], onefriend[2], onefriend[3],
                onefriend[4], onefriend[5], onefriend[6], onefriend[7]).arg(onefriend[8], onefriend[9],
                onefriend[10] == "online" ? "1" : "2");

        QSqlQuery sql_query;

        sql_query.prepare(sql2.arg(AllVariable::getLoginUserId(), onefriend[0]));
        sql_query.exec();

        sql_query.prepare(s);
        sql_query.exec();
    }

}

//获取好友列表信息
QList<QVector<QString>> DataBase::getFriendList()
{
    QList<QVector<QString>> friends;

    QString sql = "select friendid, username, remark, grouptype, personalizedsignature, imagepath, status from friendlist where userid='%1';";

    sql =sql.arg(AllVariable::getLoginUserId());


    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();

    while(sql_query.next())
    {
        QString friendid = sql_query.value(0).toString();
        QString username= sql_query.value(1).toString();
        QString remarke = sql_query.value(2).toString();
        QString grouptype = sql_query.value(3).toString();
        QString personalizedsignature = sql_query.value(4).toString();
        QString imagepath = sql_query.value(5).toString();
        QString status = sql_query.value(6).toString();
        QVector<QString> oneFriend;
        oneFriend.append(friendid);
        oneFriend.append(username);
        oneFriend.append(remarke);
        oneFriend.append(grouptype);
        oneFriend.append(personalizedsignature);
        oneFriend.append(imagepath);
        oneFriend.append(status);
        friends.append(oneFriend);
    }

    return friends;
}

//获取好友分组
QStringList DataBase::getGroup()
{
    QStringList groups;
    QString sql = "select groupname from chat_friend_group where userid=?;";

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.addBindValue(AllVariable::getLoginUserId());
    sql_query.exec();

    while(sql_query.next())
    {
        groups << sql_query.value(0).toString();
    }

    return groups;
}

//保存好友聊天记录
void DataBase::setChatLog(const QString &senderid, const QString &receiverid, const QString &content)
{
    QString sql("insert into chatlog(senderid, receiverid, content) values('%1', '%2', '%3')");
    sql = sql.arg(senderid, receiverid, content);

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();
}

//好友分组移动 == 更新好友分组
void DataBase::moveFriendToGroup(const QString &userid, const QString &group)
{
    QString sql("update friendlist set grouptype='%1' where friendid='%2' and userid='%3';");
    sql = sql.arg(group, userid, AllVariable::getLoginUserId());

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();
}

//获取指定好友信息
QVector<QString> DataBase::getFriendInfo(const QString &userid)
{
    QString sql("select username, sex, birthofdate, mail, mobile, personalizedsignature, imagepath "
                "from friendlist where friendid='%1' limit 1;");
    sql = sql.arg(userid);

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();

    QVector<QString> info;
    info.append(userid);
    while(sql_query.next())
    {
        info.append(sql_query.value(0).toString());
        info.append(sql_query.value(1).toString());
        info.append(sql_query.value(2).toString());
        info.append(sql_query.value(3).toString());
        info.append(sql_query.value(4).toString());
        info.append(sql_query.value(5).toString());
        info.append(sql_query.value(6).toString());
    }

    return info;
}

QString DataBase::getFriendName(const QString &friendid)
{
    QString sql("select username from friendlist where friendid='%1' limit 1;");
    sql = sql.arg(friendid);

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();

    sql_query.next();

    return sql_query.value(0).toString();
}

QString DataBase::getFriendImage(const QString &friendid)
{
    QString sql("select imagepath from friendlist where friendid='%1' limit 1;");
    sql = sql.arg(friendid);

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();

    sql_query.next();

    return sql_query.value(0).toString();
}

//获取最近聊天的好友信息
QVector<QStringList> DataBase::getRecentlyChatFriendInfo(const QStringList &list)
{
    QVector<QStringList> lists;

    QString sql = "select username, personalizedsignature, imagepath, content  from chatlog, "
                  "friendlist where receiverid='%1'"
                  " and friendid=receiverid and userid='%2' order by  chattime desc limit 1;";
    for(int i = list.size() - 1; i >=0; --i)
    {
        QString sq = sql.arg(list.at(i), AllVariable::getLoginUserId());
        //        qDebug() << sq;
        QSqlQuery sql_query;
        sql_query.prepare(sq);
        sql_query.exec();

        QStringList l;
        while(sql_query.next())
        {
            l.append(list.at(i));
            l.append(sql_query.value(0).toString());
            l.append(sql_query.value(1).toString());
            l.append(sql_query.value(2).toString());
            l.append(sql_query.value(3).toString());
        }
        lists.append(l);
    }

    return lists;
}

//搜索
QVector<QStringList> DataBase::searachFriend(const QString &key)
{
    QVector<QStringList> vec;

    QString sql = "select friendid, username, personalizedsignature, imagepath from friendlist where remark "
                  "like '%%1%' or  username like '%%1%' or friendid like'%%1%' and userid='%2';";
    sql = sql.arg(key, AllVariable::getLoginUserId());


    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();


    while(sql_query.next())
    {
        QStringList l;
        l.append(sql_query.value(0).toString());
        l.append(sql_query.value(1).toString());
        l.append(sql_query.value(2).toString());
        l.append(sql_query.value(3).toString());
        l.append(sql_query.value(4).toString());
        vec.append(l);
    }

    return vec;
}

//保存群组信息
void DataBase::setGroupInfo(const QVector<QStringList> &lists)
{

    QString sql1 = "select groupid from chat_group where userid='%1';";
    sql1 = sql1.arg(AllVariable::getLoginUserId());

    QSqlQuery sql_query;
    sql_query.prepare(sql1);
    sql_query.exec();


    QVector<QString> vec;
    while(sql_query.next())
    {
        vec.append(sql_query.value(0).toString());
    }

    for (auto elem : lists)
    {
        bool exist = false;

        for(auto id : vec)
        {
            //存在则跟更新记录
            if(id == elem[0]){
                QString sql2 = "update chat_group set groupname='%1', groupimage='%2' where userid='%3' and groupimage='%4';";
                QSqlQuery sql_query2;
                sql_query2.prepare(sql2.arg(elem[1], elem[2], AllVariable::getLoginUserId(), elem[0]));
                sql_query2.exec();
                exist = true;
            }
        }
        //如果记录不存在， 则插入
        if(!exist)
        {
            QString sql = "insert into chat_group(userid, groupid, groupname, groupimage)"
                          "values('%1', '%2', '%3', '%4');";
            QSqlQuery sql_query1;
            sql_query1.prepare(sql.arg(AllVariable::getLoginUserId(), elem[0], elem[1], elem[2]));
            sql_query1.exec();

        }
    }

}

//获取群组信息
QVector<QStringList> DataBase::getGroupInfo()
{
    QString sql = "select groupid, groupname, groupimage from chat_group where userid='%1';";
    sql = sql.arg(AllVariable::getLoginUserId());

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();

    QVector<QStringList> lists;
    while(sql_query.next())
    {
        QStringList list;
        list.append(sql_query.value(0).toString());
        list.append(sql_query.value(1).toString());
        list.append(sql_query.value(2).toString());
        lists.append(list);
    }

    return lists;
}

void DataBase::setGroupMemberInfo(const QMap<QString, QVector<QStringList>> &map)
{

    QString sql_delete = "delete from chat_groupmember where groupid='%1' and memberid='%2';";
    QString sql = "insert into chat_groupmember(userid, groupid, memberid, membername, memberimage) values('%1', '%2', '%3', '%4', '%5');";

    for(QString key : map.keys())
    {
        for(QStringList elem : map.value(key))
        {
            QSqlQuery sql_query;

            sql_query.prepare(sql_delete.arg((key, elem[0])));
            sql_query.exec();

            sql_query.prepare(sql.arg(AllVariable::getLoginUserId(), key, elem[0], elem[1], elem[2]));
            sql_query.exec();
        }
    }
    emit setGroupMemberFinished();
}

QVector<QStringList> DataBase::getGroupMemberInfo(const QString &groupid)
{
    QString sql = "select memberid, membername, memberimage from chat_groupmember where groupid='%1'";
    sql = sql.arg(groupid);

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();

    QVector<QStringList> vec;
    while(sql_query.next())
    {
        QStringList list;
        list << sql_query.value(0).toString()
             << sql_query.value(1).toString()
             << sql_query.value(2).toString();
        vec.append(list);
    }

    return vec;
}

QString DataBase::getGroupName(const QString &groupid)
{
    QString sql = "select groupname from chat_group where groupid='%1'";
    sql = sql.arg(groupid);

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();
    sql_query.next();
    return sql_query.value(0).toString();
}

QString DataBase::getGroupMemberName(const QString &groupid, const QString &memberid)
{
    QString sql = "select membername from chat_groupmember where groupid='%1' and memberid='%2'";
    sql = sql.arg(groupid, memberid);

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();
    sql_query.next();
    return sql_query.value(0).toString();
}

void DataBase::setGroupOfflineMessage(const QString& groupid, const QString& senderid,
                                      const QString& content, const QString& fontfamliy,
                                      const QString& fontsize, const QString& fontcolor)
{
    QString sql = "insert into chat_group_offline_message(groupid, senderid, content, fontfamliy,"
                  "fontsize, fontcolor) values(?, ?, ?, ?, ?, ?);";

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.addBindValue(groupid);
    sql_query.addBindValue(senderid);
    sql_query.addBindValue(content);
    sql_query.addBindValue(fontfamliy);
    sql_query.addBindValue(fontsize);
    sql_query.addBindValue(fontcolor);

    sql_query.exec();
}

QString DataBase::getGroupIcon(const QString &groupid)
{
    QString sql = "select distinct groupimage from chat_group where groupid=?;";

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.addBindValue(groupid);
    sql_query.exec();
    sql_query.next();

    return sql_query.value(0).toString();
}

QVector<QStringList> DataBase::getGroupOfflineMessage(const QString &groupid)
{
    QString sql = "select senderid, content, fontfamliy, fontsize, fontcolor from chat_group_offline_message where groupid = ?;";

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.addBindValue(groupid);

    sql_query.exec();

    QVector<QStringList> vec;

    while(sql_query.next())
    {
        QStringList list;
        list << sql_query.value(0).toString();
        list << sql_query.value(1).toString();
        list << sql_query.value(2).toString();
        list << sql_query.value(3).toString();
        list << sql_query.value(4).toString();

        vec.append(list);
    }

    sql = "delete from chat_group_offline_message where groupid = ?;";
    sql_query.prepare(sql);
    sql_query.addBindValue(groupid);

    sql_query.exec();
    return vec;
}

void DataBase::deleteFriend(const QString &friendid)
{
    QString sql = "delete from friendlist where userid=? and friendid=?;";

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.addBindValue(AllVariable::getLoginUserId());
    sql_query.addBindValue(friendid);

    sql_query.exec();
}

void DataBase::addFriendGroup(const QString &groupname)
{
    QString sql1 = "delete from chat_friend_group where groupname=? and userid=?;";
    QString sql2 = "insert into chat_friend_group(userid, groupname) values(?, ?);";

    QSqlQuery sql_query;
    sql_query.prepare(sql1);
    sql_query.addBindValue(groupname);
    sql_query.addBindValue(AllVariable::getLoginUserId());
    sql_query.exec();

    QSqlQuery sql_query2;
    sql_query2.prepare(sql2);
    sql_query2.addBindValue(AllVariable::getLoginUserId());
    sql_query2.addBindValue(groupname);
    sql_query2.exec();
}

void DataBase::deleteFriendGroup(const QString &groupname)
{
    QString sql = "delete from chat_friend_group where userid=? and groupname=?;";

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.addBindValue(AllVariable::getLoginUserId());
    sql_query.addBindValue(groupname);

    sql_query.exec();
}

QVector<QStringList> DataBase::get_all_friends()
{
    QString sql = "select friendid, username, imagepath from friendlist where userid=?;";

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.addBindValue(AllVariable::getLoginUserId());

    sql_query.exec();

    QVector<QStringList> vec;

    while(sql_query.next())
    {
        QStringList list;
        list << sql_query.value(0).toString() << sql_query.value(1).toString() << sql_query.value(2).toString();
        vec.push_back(list);
    }

    return vec;
}

void DataBase::delete_group(const QString &userid, const QString &groupid)
{
    QString sql = "delete from chat_group where userid=? and groupid=?;";

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.addBindValue(AllVariable::getLoginUserId());
    sql_query.addBindValue(groupid);

    sql_query.exec();
}

void DataBase::delete_all_groupmember()
{
    QString sql = "delete from chat_groupmember where userid=?;";

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.addBindValue(AllVariable::getLoginUserId());

    sql_query.exec();
}
