#include "database.h"

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
        instance = new DataBase;
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
