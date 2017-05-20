#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("chat.db");
    database.open();
    QSqlQuery sql_query;
    sql_query.prepare("select * from localuserinfo;");
    sql_query.exec();
    while(sql_query.next())
                    {
                        int id = sql_query.value(0).toInt();
                        QString name = sql_query.value(1).toString();
                        QString address = sql_query.value(2).toString();
                        qDebug()<<QString("ID:%1  username:%2  passwd:%3").arg(id).arg(name).arg(address);
                    }

}
