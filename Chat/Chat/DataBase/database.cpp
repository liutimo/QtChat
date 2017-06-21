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
        sql_query.prepare(QString("insert into localuserinfo(userid, userpw) values('%1', '%2')").arg(userid, password));
        sql_query.exec();
    }
}

void DataBase::setFriendList(QList<QVector<QString>> friends)
{
    QString sql = "insert into friendlist(userid, friendid, username, remark, "
                  "personalizedsignature, grouptype, imagepath, birthofdate"
                  ", sex, mobile, mail) values('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11')";

    QSqlQuery sql_query;

    for (QVector<QString> onefriend : friends)
    {
        QString s = sql.arg(AllVariable::getLoginUserId(),onefriend[0], onefriend[1], onefriend[2], onefriend[3],
                onefriend[4], onefriend[5], onefriend[6], onefriend[7]).arg(onefriend[8], onefriend[9]);
        qDebug() << s;
        sql_query.prepare(s);
        sql_query.exec();
    }

}
QList<QVector<QString>> DataBase::getFriendList()
{
    QList<QVector<QString>> friends;

    QString sql = "select friendid, username, remark, grouptype, personalizedsignature, imagepath from friendlist where userid='%1';";

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

        QVector<QString> oneFriend;
        oneFriend.append(friendid);
        oneFriend.append(username);
        oneFriend.append(remarke);
        oneFriend.append(grouptype);
        oneFriend.append(personalizedsignature);
        oneFriend.append(imagepath);
        qDebug() << imagepath;
        friends.append(oneFriend);
    }

    return friends;
}

QStringList DataBase::getGroup()
{
    QStringList groups;
    QString sql = "select distinct grouptype from friendlist;";

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();

    while(sql_query.next())
    {
        groups << sql_query.value(0).toString();
    }

    return groups;
}

void DataBase::setChatLog(const QString &senderid, const QString &receiverid, const QString &content)
{
    QString sql("insert into chatlog(senderid, receiverid, content) values('%1', '%2', '%3')");
    sql = sql.arg(senderid, receiverid, content);

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();
}

void DataBase::moveFriendToGroup(const QString &userid, const QString &group)
{
    QString sql("update friendlist set grouptype='%1' where friendid='%2' and userid='%3';");
    sql = sql.arg(group, userid, AllVariable::getLoginUserId());

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();
}

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
